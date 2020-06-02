/*  $Id: host_impl.cpp,v 1.4 2006/01/12 15:01:15  Exp $
**	Game SDK
**
**
*/

#include <network/Network_Setup.h>
#include "host_impl.h"
#include "network/ipaddress.h"
#include "netinterface_impl.h"
#include "packet_impl.h"
#include "thread.h"
#include "event_trigger.h"
#include "event_listener.h"
#include "exception.h"
#include "outputstream_memory.h"
#include "system.h"
#include <iostream>

namespace GSDK
{

/////////////////////////////////////////////////////////////////////////////
// Host_Impl construction:

Host_Impl::Host_Impl(
	NetInterface_Impl *netifc,
	const IPAddress & addr)
:	
	m_disconnected(false)
,	m_addr(addr)
,	m_netifc(netifc)
,	m_local_atom_counter(0)
,	m_available(false)
,	m_shutdown(false)
,	m_disconn_received(false)
{
	m_netifc->m_new_connections.push(Host(this));
	m_netifc->m_connect_trigger.set_flag();
}

Host_Impl::~Host_Impl()
{
	do_shutdown();
}


void Host_Impl::send_udp_info()
{
// server side method
	unsigned short secret;

	try
	{
		secret = m_netifc->secret_number_generator.get_number();
	}
	catch (Exception e)
	{
		return;
	}

	m_netifc->setup_udp_map(this, m_addr.get_address(), secret);

	OutputStream_Memory cmd;
	cmd.write_uchar8(msg_udp_ack);
	cmd.write_uint16(secret);
	
	crypto_factory.createPublicKey(cmd);
	
	m_netifc->enqueue_packet(m_addr, cmd.get_data());
}

// Send packet to host using reliable protocol.
void Host_Impl::send_packet(const string &packet_channel, Packet_Impl *packet)
{
	MutexSection mutex_section(m_mutex);
	if(m_disconnected || m_shutdown)
		return;

	OutputStream_Memory cmd;

	uint16 atom_id;
	std::unordered_map<string, int>::iterator it = m_local_channels.find(packet_channel);
	if (it == m_local_channels.end())
	{
		atom_id = m_local_atom_counter++;
		m_local_channels[packet_channel] = atom_id;

		cmd.write_uchar8(msg_create_channel);
		cmd.write_uint16(atom_id);
		cmd.write_string(packet_channel);
	}
	else
		atom_id = it->second;
	cmd.write_uchar8(msg_netpacket_msg);
	cmd.write_uint16(atom_id); 
//	cmd.write_uint16(packet->size);
	cmd.write(packet->data, packet->size);

	if(!m_available)
		m_msg_out_queue.push(cmd.get_data());
	else
		m_netifc->enqueue_packet(m_addr, cmd.get_data());

}

// Send packet to host using udp.
void Host_Impl::send_packet_udp(const string &packet_channel, Packet_Impl *packet)
{
	AutoLockSection<Mutex> mutex_section(m_mutex);
	if(m_disconnected || m_shutdown)
		return;
	OutputStream_Memory cmd;

	uint16 atom_id;
	std::unordered_map<string, int>::iterator it = m_local_channels.find(packet_channel);
	if (it == m_local_channels.end())
	{
		OutputStream_Memory cmd_atom;
		
		atom_id = m_local_atom_counter++;
		m_local_channels[packet_channel] = atom_id;

		cmd.write_uchar8(msg_create_channel);
		cmd.write_uint16(atom_id);
		cmd.write_string(packet_channel);

		if(!m_available)
			m_msg_out_queue.push(cmd.get_data());
		else
			m_netifc->enqueue_packet(m_addr, cmd_atom.get_data());
	}
	else
		atom_id = it->second;

	cmd.write_uchar8(msg_netpacket_msg);
	cmd.write_uint16(atom_id);
	cmd.write(packet->data, packet->size);

	if(!m_available)
		m_msg_out_queue.push(cmd.get_data());
	else
		m_netifc->enqueue_packet(m_addr, cmd.get_data());
}

void Host_Impl::disconnect(string reason)
{
	AutoLockSection<Mutex> lock(m_mutex);
	disconnect_reason = reason;
	m_disconnected = true;

	AutoLockSection<Mutex> mutex_section(m_netifc->m_mutex);
	m_netifc->m_disconnections.push(Host(this));
	m_netifc->m_disconn_trigger.set_flag();
}

void Host_Impl::do_shutdown()
{
	AutoLockSection<Mutex> lock(m_mutex);
	if(!m_shutdown)
	{
		m_shutdown = true;
		if(!m_disconn_received)
		{
			const char ach_cmd_disconn[] = {(char)msg_disconnect};
			AutoLockSection<Mutex> lock(m_netifc->m_mutex);
			m_netifc->m_udp_sock.send(ach_cmd_disconn, 1, m_addr);
			m_netifc->remove_udp_map(this);
		}
	}
}

void Host_Impl::adjust_time(Packet &packet)
{
// client side
	int len = packet.input.size() - packet.input.tell();
	if(len >= 3 * sizeof(int64))
	{
		int64 T1 = packet.input.read_int64();
		int64 T2 = packet.input.read_int64();
		int64 T3 = packet.input.read_int64();
		int64 T4 = System::get_absolute_time();
		
		int64 time_offset = ((T2 - T1) + (T3 - T4)) / 2;
//		int64 rtt = ((T4 - T1) - (T2 - T3));
		System::adjust_network_time(time_offset);
	}
}

void Host_Impl::init_time_sync()
{
// client side
	OutputStream_Memory time_msg;
	time_msg.write_uchar8(msg_net_time);
	time_msg.write_uchar8(2);
//	AutoLockSection<Mutex> mutex_section(m_netifc->m_mutex);
	m_netifc->enqueue_packet(m_addr, time_msg.get_data());
}

void Host_Impl::query_net_time()
{
// client side
	OutputStream_Memory time_msg;
	time_msg.write_uchar8(msg_net_time);
	time_msg.write_uchar8(0);
	time_msg.write_int64(System::get_network_time());
//	AutoLockSection<Mutex> mutex_section(m_netifc->m_mutex);
	m_netifc->m_udp_sock.send(time_msg.get_ptr(), time_msg.size(), m_addr);
}

void Host_Impl::reply_net_time(Packet &packet)
{
//	server side
	int64 T = packet.input.read_int64();
	OutputStream_Memory time_msg;
	time_msg.write_uchar8(msg_net_time);
	time_msg.write_uchar8(1);
	int64 T2 = System::get_network_time();
	time_msg.write_int64(T);
	time_msg.write_int64(T2);
	time_msg.write_int64(T2);
	AutoLockSection<Mutex> mutex_section(m_netifc->m_mutex);
	m_netifc->m_udp_sock.send(time_msg.get_ptr(), time_msg.size(), m_addr);
}

void Host_Impl::received_connect(Packet &packet)
{
//	server side method
	try {
		packet.input.read_char8();
		packet.input.read_uint16();
		crypto_factory.readSessionKey(packet.input);
		//char key[17] = {0};
		//crypto_factory.getSessionKey(reinterpret_cast<BYTE*>(key));
		//Log::log("crypto-session-key", Log::debug, key);
		AutoLockSection<Mutex> mutex_section(m_mutex);
		if(!m_available)
		{
			m_available = true;
			OutputStream_Memory cmd;
			cmd.write_uchar8(msg_udp_connect);
			{
				AutoLockSection<Mutex> mutex_section(m_netifc->m_mutex);
				m_netifc->m_udp_sock.send(cmd.get_ptr(), cmd.size(), m_addr);
			}
			while(!m_msg_out_queue.empty())
			{	
				const string &rmsg = m_msg_out_queue.front();
				m_netifc->enqueue_packet(m_addr, rmsg);
				m_msg_out_queue.pop();
			}
		}
	}
	catch(Exception)
	{
		///
	}
}

void Host_Impl::received_udp_packet(Packet &packet)
{
	try
	{
		unsigned char command = packet.input.read_uchar8();
		switch (command)
		{
		case msg_udp_connect: // client
			{
				AutoLockSection<Mutex> mutex_section(m_mutex);
				if(!m_available)
				{
					m_available = true;
//					OutputStream_Memory cmd;
//					cmd.write_uchar8(msg_udp_connect);
//					m_netifc->enqueue_packet(m_addr, cmd.get_data());
					{
						AutoLockSection<Mutex> mutex_section(m_netifc->m_mutex);
//						m_netifc->m_udp_sock.send(cmd.get_ptr(), cmd.size(), m_addr);
						query_net_time();
					}
					while(!m_msg_out_queue.empty())
					{	
						const string &rmsg = m_msg_out_queue.front();
						m_netifc->enqueue_packet(m_addr, rmsg);
						m_msg_out_queue.pop();
					}
				}
			}
			break;

		case msg_disconnect:
			{
				m_disconn_received = true;
				disconnect(_T("Graceful disconnect"));
			}
			break;

		case msg_netpacket_msg:
			{
				std::unordered_map<int, string>::iterator iter = m_remote_channels.find(packet.input.read_uint16());

				// if it's an unknown channel, ignore message.
				if (iter == m_remote_channels.end()) 
					break;

				string channel_name = iter->second;

				int pos = packet.input.tell();
				Packet message(packet.get_data()+pos, packet.get_size()-pos);
				AutoLockSection<Mutex> mutex_section(m_netifc->m_mutex);
				m_netifc->received_netpacket(channel_name, message, this);
			}
			break;

		case msg_create_channel:
			{
				int atom_id = packet.input.read_uint16();
				string atom_str = packet.input.read_string();
				m_remote_channels[atom_id] = atom_str;
				int pos = packet.input.tell();
				if(packet.get_size() > pos)
				{
					Packet message(packet.get_data()+pos, packet.get_size()-pos);
					received_udp_packet(message);
				}
			}
			break;

		case msg_udp_ack: // client
			{
				udp_connect_secret = packet.input.read_uint16();
				crypto_factory.readPublicKey(packet.input);

				OutputStream_Memory conn_msg;
				conn_msg.write_uchar8(msg_udp_connect);
				conn_msg.write_uint16(udp_connect_secret);
				crypto_factory.createSessionKey(conn_msg);
				//char key[17] = {0};
				//crypto_factory.getSessionKey((BYTE*)key);
				//Log::log("crypto-session-key", Log::debug, key);

				AutoLockSection<Mutex> mutex_section(m_netifc->m_mutex);

				m_netifc->m_udp_sock.send(conn_msg.get_ptr(), conn_msg.size(), m_addr);
//				m_netifc->m_udp_sock.send(packet.get_data(), packet.get_size(), m_addr);
				// todo: continue to send this packet until a msg_udp_connect is received

//				m_netifc->m_udp_ip_port_map[m_addr] = this;
			}
			break;

		case msg_net_time:
			{
				int time_cmd = packet.input.read_uchar8();
				switch(time_cmd)
				{
				case 0:
					reply_net_time(packet);
					break;
				case 1:
					adjust_time(packet);
					break;
				case 2:
					query_net_time();
					break;
				}
			}
			break;
		} // switch (command)
	}
	catch (Exception error)
	{
		// bogus packet. 
	}
} //received_udp_packet

size_t Host_Impl::hash() const
{
	return m_addr.hash();
}

} // namespace
