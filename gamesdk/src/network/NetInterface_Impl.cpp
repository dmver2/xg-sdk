/*	$Id: NetInterface_Impl.cpp,v 1.5 2005/12/29 13:19:53  Exp $
**	Game SDK
**
 */

#include "log.h"
#include "system.h"
#include "netinterface_impl.h"
#include "host_impl.h"
#include "thread.h"
#include "event_listener.h"
#include "netexception.h"
#include "outputstream_memory.h"
#include "event_trigger_socket.h"
#include <iostream>
#include <sstream>
#include <iomanip>

namespace GSDK
{
	const TCHAR NetInterface_Impl::s_sztopic[] = _T("Network");

#ifdef USE_ICMP
	void fill_icmp_data(char* icmp_data, int datasize);
	ushort checksum(ushort* buffer, int size);
#endif

	NetInterface_Impl::NetInterface_Impl()
		:
		m_bdebug(false)
		, m_bserver(false)
		, m_shutdown(false)
		, m_udp_thread(new Thread_Runnable_v0<NetInterface_Impl>(this, &NetInterface_Impl::udp_thread_run), true)
		, m_main_thread(new Thread_Runnable_v0<NetInterface_Impl>(this, &NetInterface_Impl::main), true)
	{
	}

	NetInterface_Impl::~NetInterface_Impl()
	{
		shutdown();
	}

	/////////////////////////////////////////////////////////////////////////////
	// NetInterface_Impl attributes:


	Host NetInterface_Impl::connect(const IPAddress& address)
	{
		// Establish protocol communication:
		MutexSection mutex_section(m_mutex);
		Host_Impl* p_host;
		//Host_Impl* found = m_udp_ip_port_map[address];
		//if (found)
		//	p_host = found;

		p_host = new Host_Impl(this, address);
		p_host->add_ref();
		m_hostlist.push_back(p_host);

		//	On the client side map address to host immediately
		m_udp_ip_port_map[address] = p_host;

		OutputStream_Memory cmd;
		cmd.write_char8(Host_Impl::msg_udp_init);
		enqueue_packet(address, cmd.get_data());
		return Host(p_host);
	}

	void NetInterface_Impl::start_server(const string& port)
	{
		m_bserver = true;
		m_udp_sock = Socket(Socket::udp);
		m_udp_sock.bind(IPAddress(port));
		init();
	}

	void NetInterface_Impl::start_client(const string& port)
	{
		m_udp_sock = Socket(Socket::udp);
		try
		{
			m_udp_sock.bind(IPAddress(port));
		}
		catch (Exception x)
		{
			// Ok, if client couldn't perform bind.
		}
		ushort n_port = m_udp_sock.get_source_address().get_port();

		init();
	}

	void NetInterface_Impl::stop()
	{
		shutdown();
	}

	void NetInterface_Impl::enqueue_packet(const IPAddress& r_address, const string& r_data)
	{
		// todo: think of package multiplication.
		if (r_data.length() > m_udp_sock.get_max_msg_size())
			throw new Exception(_T("Data too long."));

		AutoLockSection<Mutex> mutex_section(m_mutex);
		m_out_queue.push(AddrMsgPair(r_address, r_data));
		m_out_queue_trigger.set_flag();
	}

	void NetInterface_Impl::setup_udp_map(Host_Impl* p_host, const string& address, unsigned int secret)
	{
		MutexSection mutex_section(m_mutex);

		// adds host to udp connection pending list
		m_udp_secret_ip_map[SecretIPPair(secret, address)] = p_host;
	}

	void NetInterface_Impl::remove_udp_map(Host_Impl* p_host)
	{
		MutexSection mutex_section(m_mutex);

		// removes host from the list of udp ready connection if avialable
		m_udp_ip_port_map.erase(p_host->m_addr);
	}

	void NetInterface_Impl::received_netpacket(const string& channel, const Packet& packet, Host_Impl* p_host)
	{
		if (m_bdebug)
		{
			Log::printf(
				s_sztopic
				, Log::debug
				, _T("Packet arrived in channel: %s; from %s")
				, channel.c_str()
				, p_host->m_addr.to_string().c_str());
		}
		received_netpackets[channel].push(PacketHostPair(packet, Host(p_host)));
		m_recv_trigger.set_flag();
	}

	/////////////////////////////////////////////////////////////////////////////
	// NetInterface_Impl implementation:

	void NetInterface_Impl::init()
	{
		if (m_shutdown)
			m_shutdown = false;

#ifdef USE_ICMP
		m_raw_sock = Socket(Socket::raw);
#endif

		m_recv_trigger.reset();
		m_connect_trigger.reset();
		m_disconn_trigger.reset();
		// Start main thread to handle signals.
		m_main_thread.start();
		// Start udp thread to send/recive pending udp packets.
		m_udp_thread.start();
	}

	void NetInterface_Impl::shutdown()
	{
		{ // enter mutex
			MutexSection mutex_section(m_mutex);
			if (m_shutdown)
				return;
			m_shutdown = true;

			// Disconnect all m_hostlist:
			while(!m_hostlist.empty())
			{
				Host_Impl* p_host = m_hostlist.front();
				p_host->do_shutdown();
				p_host->release_ref();
				m_hostlist.pop_front();
			}
//			m_hostlist = std::list<Host_Impl*>();

			// Empty list of received network packets.
			received_netpackets = RECEIVED_NETPACKETS();

		} //leave mutex
		// Stop threads.
		m_shutdown_trigger.set_flag();

/*
		std::vector<HANDLE> vhandles(2);
		Thread threads[] = { m_udp_thread, m_main_thread };
		for (int i = 0; i < ARRAY_LENGTH(threads); ++i) 
		{
			if (threads[i].is_alive())
				vhandles.push_back(threads[i].get_handle());
		}
		if (vhandles.size())
		{
			::WaitForMultipleObjects(vhandles.size(), vhandles.data(), TRUE, INFINITE);
		}
*/
		HANDLE handles[] = { m_udp_thread.get_handle(), m_main_thread.get_handle() };
		DWORD dw_wait = ::WaitForMultipleObjects(ARRAY_LENGTH(handles), handles, TRUE, INFINITE);
		Log::log(s_sztopic, Log::debug, "all threads stopped");
	}

	void NetInterface_Impl::main()
	{
		try
		{
			EventListener listener;
			listener.add_trigger(&m_recv_trigger);
			listener.add_trigger(&m_connect_trigger);
			listener.add_trigger(&m_disconn_trigger);
			listener.add_trigger(&m_shutdown_trigger);

			while (true)
			{
				listener.wait();
				{
					if (m_shutdown_trigger.get_flag())
						return;

					MutexSection mutex_section(m_mutex);

					if (m_recv_trigger.get_flag())
					{
						// Signal received network packets on each channel.
						RECEIVED_NETPACKETS::iterator it;
						for (it = received_netpackets.begin(); it != received_netpackets.end(); ++it)
						{
							Signal_v2<Packet&, const Host&>& chan_sig = map_netpacket_receive[it->first];
							std::queue<PacketHostPair>& packets = it->second;

							while (!packets.empty())
							{
								mutex_section.leave();
								//							try {
								chan_sig(packets.front().first, packets.front().second);
								/*							}

															catch(Exception e)
															{
																// User exception
															}
															catch(...)
															{
																// Unhandled exception
															}
								*/
								mutex_section.enter();
								packets.pop();
							}
						}
						m_recv_trigger.reset();
					}	// if(m_recv_trigger.get_flag()) 

					if (m_connect_trigger.get_flag())
					{
						// Signal incoming connections:
						while (!m_new_connections.empty())
						{
//							all.insert(m_new_connections.front());
							mutex_section.leave();
							sig_host_connected(m_new_connections.front());
							mutex_section.enter();
							m_new_connections.pop();
						}
						m_connect_trigger.reset();
					}
					// Signal incoming disconnections:
					if (m_disconn_trigger.get_flag())
					{
						while (!m_disconnections.empty())
						{
//							all.erase(m_disconnections.front());
							mutex_section.leave();
							sig_host_disconnected(m_disconnections.front());
							mutex_section.enter();
							m_disconnections.pop();
						}
						m_disconn_trigger.reset();

						// Check if any host handles need to be removed: (keep connected intact)
						for(HOST_LIST::iterator it_hosts = m_hostlist.begin(); it_hosts != m_hostlist.end();)
						{
							Host_Impl* p_host = *it_hosts;
							if (p_host->m_disconnected)
							{
								p_host->release_ref();
								m_hostlist.erase(it_hosts++);
							}
							else
							{
								++it_hosts;
							}
						}
					}
				}
				//			System::sleep(100);
			} // while
		} // try
		catch (Exception ex)
		{
			Log::printf(_T("NetInterface::main failed. %s"), Log::SEVERITY::fatal, ex.get_message());
		}
	}

	void NetInterface_Impl::udp_thread_run()
	{
		char* buffer = 0;

		try
		{
			EventListener listener;

#ifdef USE_ICMP
			icmp_test();
			listener.add_trigger(m_raw_sock.get_read_trigger());
#endif
			listener.add_trigger(m_udp_sock.get_read_trigger());

			listener.add_trigger(&m_out_queue_trigger);

			listener.add_trigger(&m_shutdown_trigger);

			int msg_size = m_udp_sock.get_max_msg_size();
			buffer = new char[msg_size];
			memset(buffer, 0, msg_size);

			while (true)
			{
				listener.wait();
				if (m_shutdown_trigger.get_flag()) // netinterface is destructing it self
					break;

#ifdef USE_ICMP
				if (m_raw_sock.get_read_trigger()->get_flag())
				{
					IPAddress from;
					int bytes_available = 0;
					try
					{
						bytes_available = m_raw_sock.recv(buffer, msg_size, from);
					}
					catch (NetException nx)
					{
						if (m_shutdown_trigger.get_flag())
							break;
					}
					handle_icmp(buffer, bytes_available, from);

				}
#endif
				if (m_udp_sock.get_read_trigger()->get_flag())
				{
					IPAddress from;
					int bytes_available = 0;
					try
					{
						bytes_available = m_udp_sock.recv(buffer, msg_size, from);
					}
					catch (NetException& nx)
					{
						if (WSAECONNRESET == nx.get_error_code())
						{
							listener.remove_trigger(m_udp_sock.get_read_trigger());
							IPAddress ipaddress = m_udp_sock.get_source_address();
							m_udp_sock = Socket(Socket::udp);
							m_udp_sock.bind(ipaddress);
							listener.add_trigger(m_udp_sock.get_read_trigger());
						}
						//					((EventTrigger_Socket*)m_udp_sock.get_read_trigger())->start_listen();
						if (m_bdebug)
							Log::printf(s_sztopic
								, Log::error
								, _T("NetInterface_Impl::udp_thread_run runtime error. %s (%d)")
								, nx.get_message()
								, nx.get_error_code());
					}
					if (m_bdebug)
					{
						std::basic_ostringstream<TCHAR> out;
						out << _T("from: ") << from.to_string() << _T(" len: ") << bytes_available << '\t';
						for (int i = 0; i < bytes_available; ++i)
						{
							ushort ubyte = 0xFF & buffer[i];
							out << std::hex << std::setw(2) << std::setfill('0') << ubyte;
						}
						out << std::endl; // '\r' << '\n';
						Log::log(
							s_sztopic
							, Log::debug
							, out.str().c_str());
					}

					if (m_bserver)
						dispatch_udp_message_server(from, bytes_available, buffer);
					else
						dispatch_udp_message_client(from, bytes_available, buffer);
				}

				// look for data to send
				if (m_out_queue_trigger.get_flag())
				{
					AddrMsgPair udp_message;

					while (!m_out_queue.empty())
					{
						udp_message = m_out_queue.front();

						m_udp_sock.send(udp_message.second.c_str(), udp_message.second.size(), udp_message.first);

						m_out_queue.pop();
					}

					m_out_queue_trigger.reset();
				}

			} // while
		}
		catch (Exception err)
		{
			if (m_bdebug)
				Log::printf(
					s_sztopic
					, Log::fatal
					, _T("NetInterface_Impl::udp_thread_run failed. %s")
					, err.get_message());
		}

		delete[] buffer;
	}

	void NetInterface_Impl::dispatch_udp_message_server(const IPAddress& from, int bytes_available, char* buffer)
	{
		if (bytes_available > 0)
		{
			// todo: search for ip address in the udp connection map and dispatch message
			Packet packet(buffer, bytes_available);

			// get the implementation of the host by IP-addr
			std::unordered_map<IPAddress, Host_Impl*, sockaddr_hash_comparator>::iterator
				iter_ip_port_map = m_udp_ip_port_map.find(from);

			// if host is not listed, try peeking into data and see if it's a udp connection request
			if (iter_ip_port_map == m_udp_ip_port_map.end())
			{
				char sig = packet.input.read_char8();

				switch (sig)
				{
				case Host_Impl::msg_udp_init:
				{
					Host_Impl* p_host = new Host_Impl(this, from);
					p_host->add_ref();
					m_hostlist.push_back(p_host);
//					all.insert(Host(p_host));
					p_host->send_udp_info();
				}
				break;

				case Host_Impl::msg_udp_connect:
				{
					// get secret(magic) key
					unsigned short secret = packet.input.read_uint16();

					UDP_SECRET_IP_MAP::iterator iter_secret_ip_map
						= m_udp_secret_ip_map.find(SecretIPPair(secret, from.get_address()));

					// check if host is listed in pending list
					if (iter_secret_ip_map != m_udp_secret_ip_map.end())
					{
						// FOUND!!!
						Host_Impl* p_host = iter_secret_ip_map->second; // (*iter_secret_ip_map).second;

						// set udp adress to host
						p_host->m_addr = from;

						// return secret(magic) key to number generator
						secret_number_generator.return_number(secret);

						// now add host to m_udp_ip_port_map list
						m_udp_ip_port_map[from] = p_host;
						// and remove them from pending list
						m_udp_secret_ip_map.erase(iter_secret_ip_map);

						// Rebuild data
//						packet = Packet();
//						packet.output.write_char8(Host_Impl::msg_udp_connect);

						// send data to host
						packet.input.seek(0, InputStream::seek_set);
						p_host->received_connect(packet);
						//						p_host->received_udp_packet(packet);
					}
					else
					{
						// ambiguous remote host trying connect with fake secret(magic) key???
						if (m_bdebug)
							Log::printf(s_sztopic
								, Log::info
								, _T("Ambiguous host from %s %d")
								, from.to_string().c_str()
								, sig);
					}
				}
				break;
				default:
					if (m_bdebug)
						Log::printf(s_sztopic
							, Log::info
							, _T("Unhandled packet from %s %d")
							, from.to_string().c_str()
							, sig);
				} // switch(sig)
			}
			else
			{
				// Host found
				// send data to host
				iter_ip_port_map->second->received_udp_packet(packet);
			}
		}
	}

	void NetInterface_Impl::dispatch_udp_message_client(const IPAddress& from, int bytes_available, char* buffer)
	{
		if (bytes_available > 0)
		{
			Packet packet(buffer, bytes_available);

			// get the host by IP-addr
			std::unordered_map<IPAddress, Host_Impl*, sockaddr_hash_comparator>::iterator iter_ip_port_map
				= m_udp_ip_port_map.find(from);

			// if host is listed send data
			if (iter_ip_port_map != m_udp_ip_port_map.end())
			{
				// Host found
				// send data to host
				iter_ip_port_map->second->received_udp_packet(packet);
			}
			else
			{
				if (m_bdebug)
					Log::printf(s_sztopic, Log::debug, _T("Unhandled packet from %s %d"), from.to_string().c_str(), (0xFF && buffer[0]));
			}
		}
	}



	void NetInterface_Impl::set_debug(bool f_debug /* = true */)
	{
		m_bdebug = f_debug;
	}

	void NetInterface_Impl::synchronize_time()
	{
		if (m_bserver)
		{
			for (std::list<Host_Impl*>::iterator it = m_hostlist.begin(); it != m_hostlist.end(); ++it)
			{
				Host_Impl* p_host = *it;
				p_host->init_time_sync();
			}
		}
		else
			for (std::list<Host_Impl*>::iterator it = m_hostlist.begin(); it != m_hostlist.end(); ++it)
			{
				Host_Impl* p_host = *it;
				p_host->query_net_time();
			}
	}

#ifdef USE_ICMP

	void NetInterface_Impl::handle_icmp(char* buf, int bytes, const IPAddress& from)
	{

		IpHeader* iphdr;
		IcmpHeader* icmphdr;
		unsigned short iphdrlen;

		iphdr = (IpHeader*)buf;

		iphdrlen = iphdr->h_len * 4; // number of 32-bit words *4 = bytes

		if (bytes < iphdrlen + ICMP_MIN) {
			return;
		}

		icmphdr = (IcmpHeader*)(buf + iphdrlen);

		if (icmphdr->i_type == ICMP_TYPE_DEST_UNREACHABLE
			&& icmphdr->i_code == ICMP_CODE_PORT_UNREACHABLE)
		{
			IpHeader* ip_hdr = (IpHeader*)(buf + iphdrlen + 8);
			UDP_dgram* dgram = (UDP_dgram*)(buf + iphdrlen + 8 + ip_hdr->h_len * 4);
			ushort dest_port = ::ntohs(dgram->n_port_dst);
			ushort src_port = ::ntohs(dgram->n_port_src);
			IPAddress sock_addr_from(from);
			sock_addr_from.set_port(dest_port);

			std::unordered_map<IPAddress, Host_Impl*, sockaddr_hash_comparator>::iterator it_host
				= m_udp_ip_port_map.find(sock_addr_from);
			if (it_host != m_udp_ip_port_map.end())
			{
				char buf[] = { (char)5 };
				Packet packet(buf, 1);
				it_host->second->received_udp_packet(packet);
			}
		}

		if (m_bdebug)
			Log::printf(s_sztopic, Log::debug, _T("ICMP data len = %d bytes from %s icmp_type = %d icmp_code = %d")
				, bytes
				, from.get_address().c_str()
				, icmphdr->i_type
				, icmphdr->i_code);
	}

	/**
	 *	Sends test ICMP-packet, necessary for raw socket initialization.
	 */
	void NetInterface_Impl::icmp_test()
	{
		static int seq_no = 0;
		const int MAX_PACKET = 1024;
		int datasize = 32;
		//	char * icmp_data = new char[MAX_PACKET];
		//	memset(icmp_data,0,MAX_PACKET);
		char icmp_data[MAX_PACKET] = { 0 };
		fill_icmp_data(icmp_data, datasize);

		((IcmpHeader*)icmp_data)->i_cksum = 0;
		((IcmpHeader*)icmp_data)->timestamp = GetTickCount();

		((IcmpHeader*)icmp_data)->i_seq = seq_no++;
		((IcmpHeader*)icmp_data)->i_cksum = checksum((USHORT*)icmp_data,
			datasize);

		sockaddr_in dest;
		memset(&dest, 0, sizeof(dest));
		dest.sin_family = AF_INET;
		dest.sin_addr.S_un.S_un_b.s_b1 = 127;
		dest.sin_addr.S_un.S_un_b.s_b2 = 0;
		dest.sin_addr.S_un.S_un_b.s_b3 = 0;
		dest.sin_addr.S_un.S_un_b.s_b4 = 1;

		m_raw_sock.send(icmp_data, datasize, IPAddress(dest.sin_addr, 0));
		//	m_raw_sock.send(icmp_data, datasize, IPAddress(_T("127.0.0.1"), 0));
			//int bwrote = sendto(	m_icmp_sock
			//				,	icmp_data
			//				,	datasize
			//				,	0
			//				,	(struct sockaddr*)&dest
			//				,	sizeof(dest));

		//	delete [] icmp_data;
	}

	/*
	*	Calculates packet checksum
	*/
	ushort checksum(ushort* buffer, int size)
	{
		unsigned long cksum = 0;

		while (size > 1) {
			cksum += *buffer++;
			size -= sizeof(ushort);
		}

		if (size) {
			cksum += *(UCHAR*)buffer;
		}

		cksum = (cksum >> 16) + (cksum & 0xffff);
		cksum += (cksum >> 16);
		return (ushort)(~cksum);
	}

	/*
		Helper function to fill in various stuff in our ICMP request.
	*/
	void fill_icmp_data(char* icmp_data, int datasize)
	{
		IcmpHeader* icmp_hdr;
		char* datapart;

		icmp_hdr = (IcmpHeader*)icmp_data;

		icmp_hdr->i_type = ICMP_ECHO;
		icmp_hdr->i_code = 0;
		icmp_hdr->i_id = (ushort)GetCurrentProcessId();
		icmp_hdr->i_cksum = 0;
		icmp_hdr->i_seq = 0;

		datapart = icmp_data + sizeof(IcmpHeader);
		//
		// Place some junk in the buffer.
		//
		memset(datapart, 'X', datasize - sizeof(IcmpHeader));
	}
#endif

} // namespace GSDK
