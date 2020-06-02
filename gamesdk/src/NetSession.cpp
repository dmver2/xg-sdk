/*	$Id: NetSession.cpp,v 1.2 2005/11/17 16:36:20 dmitry.vereshchagin Exp $
**
**
*/

#include "winsock2.h"
#include "netsession.h"
#ifdef SOAP_HANDLER
#include "xmlserializer.h"
#endif //SOAP_HANDLER
#include "types.h"
#include "domain.h"
#include "network/network_setup.h"
#include "network/socket.h"
#include "exception.h"
#include "log.h"

namespace GSDK {

	const TCHAR NetSession::s_sz_topic[] = _T("XStream");

	NetSession::NetSession(void)
		: m_sock(INVALID_SOCKET)
		, m_event()
		, m_work_thread(0)
	{
	}

	NetSession::~NetSession(void)
	{
		if (m_event.wait(0) != WAIT_OBJECT_0)
			stop();
	}

	void NetSession::start(const string& in_port)
	{
		GSDK::Network_Setup::startup();
		m_sock = ::socket(AF_INET, SOCK_DGRAM, 0);
		if (m_sock == INVALID_SOCKET)
			throw Exception((_T("GSDK::NetSession::start failed ") + Socket::get_error_message()).data());

		ushort port = std::stoi(in_port);
		port = ::htons(port);
		SOCKADDR_IN addr = { AF_INET, port, { INADDR_ANY }, {0} };
		int rc = ::bind(m_sock, (SOCKADDR*)&addr, sizeof(addr));
		if (rc != 0)
			throw Exception((_T("GSDK::NetSession::start failed ") + Socket::get_error_message()).data());
		// rc==0 - ok

		m_work_thread = new WorkThread(this);
		m_work_thread->start();
	}

	void NetSession::stop()
	{
		m_event.signal();
		::closesocket(m_sock);
		if (m_work_thread)
			m_work_thread->wait();
		delete m_work_thread;
		m_work_thread = 0;
		GSDK::Network_Setup::shutdown();
	}

	void NetSession::listen_thread()
	{
		char* buffer = 0;
		try
		{
			buffer = new char[4096];

			while (true)
			{
				/*
							SOCKADDR_IN addr_from;
							addr_from.sin_addr.S_un.S_addr = INADDR_ANY;
							addr_from.sin_family = AF_INET;
							addr_from.sin_port = 0;
							memset(addr_from.sin_zero, 0, sizeof(addr_from.sin_zero));
				*/
				SOCKADDR_IN addr_from = { AF_INET, 0, { INADDR_ANY }, {0} };

				int addr_len = sizeof(addr_from);
				int rc = ::recvfrom(m_sock, buffer, 4096, 0, (sockaddr*)&addr_from, &addr_len);
				{
					if (m_event.wait(0) == WAIT_OBJECT_0)
						break;
					if (rc == SOCKET_ERROR)
					{
						Log::printf(s_sz_topic
							, Log::error
							, _T("NetSession::listen_thread runtime error. %s")
							, Socket::get_error_message().c_str());

					}
					else
						query_handler(addr_from, rc, buffer);
				}
			}
		}
		catch (...)
		{
			Log::printf(s_sz_topic
				, Log::fatal
				, _T("NetSession::listen_thread unexpected exception. Error code %d")
				, ::GetLastError());
		}

		delete[] buffer;

	}

	void NetSession::query_handler(const SOCKADDR_IN& addr, int bytes_available, char* buffer)
	{
		//	const char ping_query[] = {-1, -1, 99, 111};
		char multiple_packet_response_header[] = { '\xFE', '\xFF', '\xFF', '\xFF' };
		const char ping_header[] = { -1, -1, 112, 114 };

		char* pch = buffer;

		int query_type = ntohl(*(uint*)pch);
		pch += sizeof(int);
		if (query_type == 0x80)
		{
#ifdef SOAP_HANDLER
			ushort pv1 = ntohs(*(ushort*)pch);
			pch += sizeof(ushort);
			ushort pv2 = ntohs(*(ushort*)pch);
			pch += sizeof(ushort);
			ushort max_udp = ntohs(*(ushort*)pch);
			pch += sizeof(ushort);
			uint stream_id = htonl(*(uint*)pch);
			pch += sizeof(int);
			ushort len = ntohs(*(ushort*)pch);
			pch += sizeof(len);
			char* query = pch;
			query[len] = '\0';

			Log::printf(s_sz_topic
				, Log::debug
				, "Query: type=0x%x query: \"%s\" from: %s:%d"
				, query_type
				, query
				, ::inet_ntoa(addr.sin_addr)
				, addr.sin_port);

			//std::cout << "[XStream] Query: type=0x" << std::hex << query_type << " query: " << query  
			//	<< " from "	<< std::dec 
			//	<< (int)addr.sin_addr.S_un.S_un_b.s_b1 << '.' 
			//	<< (int)addr.sin_addr.S_un.S_un_b.s_b2 << '.' 
			//	<< (int)addr.sin_addr.S_un.S_un_b.s_b3 << '.' 
			//	<< (int)addr.sin_addr.S_un.S_un_b.s_b4 << '.' 
			//	<< ':' << addr.sin_port << std::endl;

			Domain::XMLSerializer serializer;
			if (strcmp(query, "/info") == 0)
			{
				serializer.serialize(Domain::GameInfo::get_instance());
			}
			else if (strcmp(query, "/info/game-session-info") == 0)
			{
				serializer.serialize_sessions(Domain::GameInfo::get_instance());
			}
			//else if (strcmp(query, "/info/game-session-info/players-list") == 0)
			//{
			//	serializer.serialize_players(domain::gameinfo::get_instance().get_session());
			//}
			uint response_len = serializer.get_size();
			/*
					const char* buf = serializer.get_data();
					std::cout << "length: " << response_len << std::endl;
					std::cout << buf << std::endl;
			*/
			string xml = serializer.get_str();
			//		std::cout << "[XStream] response length: " << response_len << std::endl;
			//		std::cout << xml << std::endl;
			const char* buf = xml.c_str();

			/*
					Packet out_packet;
			//			out_packet.output.set_little_endian_mode();
					out_packet.output.write_uint32(0x81); // response code
					out_packet.output.write_uint16(1); // G2P major version
					out_packet.output.write_uint16(0); // G2P minor version
					out_packet.output.write_uint32(0); // errorcode
					out_packet.output.write_uint16(12); // encoding len
					out_packet.output.write("windows-1251", 12); // encoding
					out_packet.output.write_uint32(response_len); // response length
					out_packet.output.write_uint32(stream_id); // stream id
			*/

			//			m_out_queue.push(IPMessagePair(addr, 
			//				string((const char*)out_packet.get_data(), out_packet.get_size())
			//				));

			//			m_in_sock.send(out_packet.get_data(), out_packet.get_size(), addr);

			uint packet_size = max_udp - 9;
			if (response_len > packet_size)
			{
				int packets = response_len / packet_size;
				if (response_len % packet_size)
					packets++;

				if (packets > 15)
				{
					Log::printf(s_sz_topic
						, Log::error
						, "Response too large. lenght=%d"
						, response_len);
					return;
				}

				for (int i = 0; i < packets; ++i)
				{
					//				Packet out_packet2;
					char outp[65507] = { 0 };
					char* pout = outp;
					memcpy(pout, multiple_packet_response_header, 4);
					pout += 4;
					//				out_packet2.output.write(multiple_packet_response_header, 4);
					int n = ntohl(stream_id);
					memcpy(pout, &n, sizeof(n));
					pout += sizeof(n);
					//				out_packet2.output.write_int32(ntohl(stream_id)); 
					byte index = i << 4 | packets; //(packet_index << 4) | packets;
					*pout = index;
					pout++;
					//				out_packet2.output.write_char8(index); // packet index
					if (response_len < packet_size)
						packet_size = response_len;
					memcpy(pout, buf, packet_size);
					pout += packet_size;
					//				out_packet2.output.write(buf, packet_size);
					buf += packet_size;
					response_len -= packet_size;
					::sendto(m_sock, outp, pout - outp, 0, (sockaddr*)&addr, sizeof(addr));
					//				m_in_sock.send(outp, pout - outp, addr);
				}
			}
			else
			{

				char outp[65507] = { 0 };
				char* pout = outp;
				memcpy(pout, multiple_packet_response_header, 4);
				pout += 4;
				//				out_packet2.output.write(multiple_packet_response_header, 4);
				int n = ntohl(stream_id);
				memcpy(pout, &n, sizeof(n));
				pout += sizeof(n);
				//				out_packet2.output.write_int32(ntohl(stream_id)); 
				byte index = 1; //(packet_index << 4) | packets;
				*pout = index;
				pout++;
				//				out_packet2.output.write_char8(index); // packet index
				memcpy(pout, buf, response_len);
				pout += response_len;
				//				out_packet2.output.write(buf, packet_size);
				//			response_len -= packet_size;
				::sendto(m_sock, outp, pout - outp, 0, (sockaddr*)&addr, sizeof(addr));

				/*
							Packet out_packet2;
				//			out_packet2.output.set_little_endian_mode();
							out_packet2.output.write(multiple_packet_response_header, 4);
				//			out_packet2.output.write_int32(0xFEFFFFFF);
							out_packet2.output.write_int32(ntohl(stream_id));
							byte index = 1; //(packet_index << 4) | packets;
							out_packet2.output.write_char8(index); // packet index
							out_packet2.output.write(buf, response_len);
				*/

				//			m_out_queue.push(IPMessagePair(addr, 
				//				string((const char*)out_packet2.get_data(), out_packet2.get_size())
				//				));

				//			m_in_sock.send(out_packet2.get_data(), out_packet2.get_size(), addr);
			}

#endif //SOAP_HANDLER
		} //if(query_type == 0x80)

		else if (query_type = 0xffff636f) // ping { -1, -1, 99, 111 }
		{
			Log::printf(s_sz_topic
				, Log::debug
				, _T("Ping query: type=0x%x from: %s:%d")
				, query_type
				, ::inet_ntoa(addr.sin_addr)
				, addr.sin_port);

			//		std::cout << "[XStream] Ping query: type=0x" << std::hex << query_type << std::endl;
			//			<< " from " << addr.get_address() << ':' << std::dec << addr.get_port() << std::endl;
			//		Packet ping_response;
			//		ping_response.output.write(ping_header, 4);
			::sendto(m_sock, ping_header, 4, 0, (sockaddr*)&addr, sizeof(addr));
			//		m_in_sock.send(ping_response.get_data(), ping_response.get_size(), addr);
		}	//if(query_type = 0xffff636f)
		else
			Log::printf(s_sz_topic
				, Log::debug
				, _T("Unknown query: type=0x%x from: %s:%d")
				, query_type
				, ::inet_ntoa(addr.sin_addr)
				, addr.sin_port);

		//		std::cout << "[XStream] Unknown query: type=0x" << std::hex << query_type 
		//			<< " from " << addr.get_address() << ':' << std::dec << addr.get_port() 
		//			<< std::endl;
	}

} // namespace GSDK
