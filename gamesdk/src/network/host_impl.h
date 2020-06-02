/*  $Id: host_impl.h,v 1.4 2005/11/13 12:41:11 dmitry.vereshchagin Exp $
**
**
*/

#ifndef header_host_impl
#define header_host_impl

#if _MSC_VER > 1000
#pragma once
#endif

#include "network/socket.h"
#include "network/packet.h"
#include "synchronization.h"
#include "types.h"
#include "thread.h"
#include "event_trigger.h"
#include "refptr.h"

#include <map>
#include <unordered_map>
// #include <string>
#include <queue>
#include "network/ipaddress.h"
#include <Referenceable.h>
#include "../cryptoFactory.h"

namespace GSDK
{

class NetInterface_Impl;
class Packet_Impl;
class Packet;
class EventListener;

class Host_Impl : public Domain::Referenceable
{
// 
public:

	Host_Impl(
		NetInterface_Impl *netifc,
		const IPAddress& addr);

	virtual ~Host_Impl();

// 
public:

	IPAddress m_addr;
	bool m_disconnected;
	string disconnect_reason;
	refptr<NetInterface_Impl> m_netifc;
	Mutex m_mutex;
	CryptoFactory crypto_factory;

// 
public:

	void received_udp_packet(Packet &packet);

	void received_connect(Packet &packet);

	void send_packet(const string &packet_channel, Packet_Impl *packet);

	void send_packet_udp(const string &packet_channel, Packet_Impl *packet);

	void disconnect(string reason);

	void do_shutdown();

	size_t hash() const;

// 
private:
	void send_udp_info();
	void adjust_time(Packet &packet);
	void init_time_sync();
	void query_net_time();
	void reply_net_time(Packet &packet);

	Packet m_packet;

	std::unordered_map<string, int> m_local_channels;

	int m_local_atom_counter;

	std::unordered_map<int, string> m_remote_channels;

	/// Queue of messages to be sent across reliable protocol.
//	std::queue<string> out_msg_queue;

	/// Queue of messages awaiting protocol handshaking completion
	std::queue<string> m_msg_out_queue;


	/// message types used over socket.
	enum
	{
		msg_create_channel		= 0,
		msg_udp_ack,			// 1
		msg_udp_connect,		// 2
		msg_udp_init,			// 3
		msg_disconnect,			// 4
		msg_net_time,			// 5
		msg_netstream_connect	= 32,
		msg_netstream_msg,		// 33
		msg_netstream_closed,	// 34
		msg_netpacket_msg		= 64,
		msg_xstream				= 128
	};

	/// Connect secret to be sent to server in response to a msg_udp_ack.
	unsigned short udp_connect_secret;
	
	/// Protocol handshaking completion flag
	/// If true, UDP communication has been established
	bool m_available;

	bool m_shutdown;

	bool m_disconn_received;

	friend class NetInterface_Impl;
};

} // namespace GSDK

#endif
