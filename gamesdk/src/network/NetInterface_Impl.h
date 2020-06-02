/*	$Id: NetInterface_Impl.h,v 1.2 2005/11/17 16:36:20 dmitry.vereshchagin Exp $
** NetInterface_Impl interface
**
**/

#pragma once

#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <map>
#include <list>
#include <queue>
#include <climits>
#include <set>
#include "network/socket.h"
#include "network/host.h"
#include "network/hostgroup.h"
#include "network/packet.h"
#include <Referenceable.h>
#include "synchronization.h"
#include "thread.h"
#include "event_trigger.h"
#include "signals.h"
#include "log.h"
#include "../Util.h"

#define USE_ICMP

#ifdef USE_ICMP

#define ICMP_ECHO 8
#define ICMP_ECHOREPLY 0
#define ICMP_TYPE_DEST_UNREACHABLE	3
#define ICMP_CODE_PORT_UNREACHABLE	3

#define ICMP_MIN 8        

#pragma pack(push, 1)

typedef struct iphdr {
	unsigned int h_len:4;              
	unsigned int version:4;           
	unsigned char tos;                
	unsigned short total_len;           
	unsigned short ident;            
	unsigned short frag_and_flags;  
	unsigned char  ttl; 
	unsigned char proto;               
	unsigned short checksum;         

	unsigned int sourceIP;
	unsigned int destIP;

} IpHeader;

typedef struct _ihdr {
  BYTE i_type;
  BYTE i_code;     
  USHORT i_cksum;
  USHORT i_id;
  USHORT i_seq;
  ULONG timestamp;
} IcmpHeader;

#pragma pack(pop)
#endif

typedef struct tag_udp_dgram 
{
	ushort	n_port_src;
	ushort	n_port_dst;
	ushort	n_dgram_len;
	ushort	n_chksum;
} UDP_dgram;

namespace GSDK
{
class Host_Impl;
class NetInterface_Impl : public Domain::Referenceable
{
public:
	NetInterface_Impl(void);
	virtual ~NetInterface_Impl(void);

public:
	Signal_v1<const Host &> sig_host_connected;

	Signal_v1<const Host &> sig_host_reconnected;

	Signal_v1<const Host &> sig_host_disconnected;

	EventTrigger m_out_queue_trigger;

	typedef std::pair<IPAddress, string> AddrMsgPair;

	std::queue<AddrMsgPair> m_out_queue;

	typedef std::pair<ushort, string> SecretIPPair;
	struct secret_ip_comparator //: public std::hash_compare<SecretIPPair>
	{
		size_t operator()(const SecretIPPair& keyval) const
		{
			std::size_t h2 = std::hash<string>{}(keyval.second);
			std::size_t h1 = std::hash<ushort>{}(keyval.first);
			return h1 ^ (h2 << 1); // or use boost::hash_combine
		}

	};
	typedef std::unordered_map<SecretIPPair, Host_Impl*, secret_ip_comparator> UDP_SECRET_IP_MAP;
	UDP_SECRET_IP_MAP m_udp_secret_ip_map;

	std::queue<Host> m_new_connections;

	std::queue<Host> m_disconnections;

	Socket m_udp_sock;

	bool m_bserver;

#ifdef USE_ICMP
	Socket m_raw_sock;
#endif

	Mutex m_mutex;

public:

	Host connect(const IPAddress &address);

	void start_server(const string &port);

	void start_client(const string &port);

	void stop();

	void setup_udp_map(Host_Impl *host, const string &address, unsigned int secret);

	void remove_udp_map(Host_Impl *host);

	void received_netpacket(const string &channel, const Packet&packet, Host_Impl *host);

	void set_debug(bool f_debug = true);

	void enqueue_packet(const IPAddress &r_address, const string &r_data);

	void synchronize_time();

	EventTrigger m_recv_trigger, m_connect_trigger, m_disconn_trigger;

private:

	virtual void main();
	
	void init();
	
	void shutdown();
	
	void udp_thread_run();
	
	void dispatch_udp_message_server(const IPAddress &from, int bytes_available, char *buffer);
	
	void dispatch_udp_message_client(const IPAddress &from, int bytes_available, char *buffer);

#ifdef USE_ICMP
	void handle_icmp(char *buf, int bytes, const IPAddress & from);
	void icmp_test();
#endif

	Thread m_main_thread;

	Thread m_udp_thread;

	EventTrigger m_shutdown_trigger;

	bool m_bdebug;

	bool m_shutdown;

	static const TCHAR s_sztopic[];

	class SecretPool
	{
	public:
		SecretPool()
		:	initial_number_range(0)
		{
		}

	public:
		unsigned short get_number()
		{
			unsigned short ret_val;

			if (available_numbers.size() > 0)
			{
				std::set<unsigned short>::iterator iter;

				iter = available_numbers.begin();

				ret_val = *iter;

				available_numbers.erase(iter);
			}
			else
			{
				if (initial_number_range == USHRT_MAX)
				{
					throw Exception("SecretPool::get_number failed: All unique numbers have been used up.");
				}

				ret_val = initial_number_range++;
			}

			return ret_val;
		}


		void return_number(unsigned short number)
		{
			if (number >= initial_number_range)
				return;

			available_numbers.insert(number);
		}

	private:
		std::set<unsigned short> available_numbers;

		unsigned short initial_number_range;

	};

public:
	SecretPool secret_number_generator;

	struct host_hash_comparator : public std::hash_compare<Host>
	{
		size_t operator()(const Host& keyval) const
		{
			return keyval.hash();
		}

	};    

	//typedef std::unordered_set<Host, host_hash_comparator> HOST_SET;
	//HOST_SET all;

	typedef std::list<Host_Impl*> HOST_LIST;
	HOST_LIST m_hostlist;

	struct sockaddr_hash_comparator : public std::hash_compare<IPAddress>
	{
		size_t operator()(const IPAddress& keyval) const
		{	
			return keyval.hash();
		}

	};
	std::unordered_map<IPAddress, Host_Impl *, sockaddr_hash_comparator> m_udp_ip_port_map;

	struct string_hash_comparator : public std::hash_compare<string>
	{
	private:
		std::hash<string> stdshash{};
	public:
		size_t operator()(const string& keyval) const
		{	
//			return Util::hash_code(keyval);
			return stdshash(keyval);
		}

	};
	typedef std::unordered_map< string, Signal_v2<Packet&, const Host&>, string_hash_comparator> MAP_NETPACKET_RECEIVE;
	MAP_NETPACKET_RECEIVE	map_netpacket_receive;

	typedef std::pair<Packet, Host> PacketHostPair;
	typedef std::unordered_map< string, std::queue<PacketHostPair>, string_hash_comparator> RECEIVED_NETPACKETS;
	RECEIVED_NETPACKETS	received_netpackets;

};

}   
