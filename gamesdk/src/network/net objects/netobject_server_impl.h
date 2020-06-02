/*  $Id: netobject_server_impl.h,v 1.1 2005/11/14 17:07:59 dmitry.vereshchagin Exp $
**
*/

#ifndef header_netobject_server_impl
#define header_netobject_server_impl

#if _MSC_VER > 1000
#pragma once
#endif

#include <signals.h>
#include <unordered_map>
#include <Referenceable.h>

namespace GSDK
{

class Host;
class Packet;
class HostGroup;
class NetObject_Controller_Impl;

class NetObject_Server_Impl: public Domain::Referenceable
{
// Construction:
public:
	NetObject_Server_Impl(NetObject_Controller_Impl *controller);

	~NetObject_Server_Impl();

// Attributes:
public:
	int m_obj_id;

	NetObject_Controller_Impl *m_pcontroller;

	std::unordered_map< int, Signal_v2<const Host &, Packet &> > sig_msg_received;

// Operations:
public:
	void receive(int msg_type, const Host &from, Packet &packet);

	void send(HostGroup &group, int msg_type, const Packet &message);
	void send(const Host &host, int msg_type, const Packet &message);
	void send_all(int msg_type, const Packet &message);

// Implementation:
private:
	// Packet marshaller
	Packet &marshall(int msg_type, const Packet &input_packet, OUT Packet &out_packet);
};

} // namespace
#endif
