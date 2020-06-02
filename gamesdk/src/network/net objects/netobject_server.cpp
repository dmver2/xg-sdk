/*  $Id: netobject_server.cpp,v 1.1 2005/11/14 17:07:59 dmitry.vereshchagin Exp $
**	Game SDK
**
**
*/

#include <network/net objects/netobject_server.h>
#include <network/net objects/netobject_controller.h>
#include "netobject_server_impl.h"

namespace GSDK
{

//===========================================================================
// construction

NetObject_Server::NetObject_Server(NetObject_Controller *controller)
: p_impl(new NetObject_Server_Impl(controller->m_impl))
{
	p_impl->add_ref();
}

NetObject_Server::NetObject_Server(const NetObject_Server &copy)
: p_impl(copy.p_impl)
{
	if (p_impl) p_impl->add_ref();
}

NetObject_Server::~NetObject_Server()
{
	if (p_impl) p_impl->release_ref();
}

//===========================================================================
// attributes:

int NetObject_Server::get_id() const
{
	return p_impl->m_obj_id;
}

//===========================================================================
// operations:

void NetObject_Server::send(HostGroup &group, int msg_type, const Packet &message)
{
	p_impl->send(group, msg_type, message);
}

void NetObject_Server::send(const Host &host, int msg_type, const Packet &message)
{
	p_impl->send(host, msg_type, message);
}

void NetObject_Server::send_all(int msg_type, const Packet &message)
{
	p_impl->send_all(msg_type, message);
}

//===========================================================================
// signals:

Signal_v2<const Host &, Packet &> &NetObject_Server::sig_msg_received(int msg_type)
{
	return p_impl->sig_msg_received[msg_type];
}

//===========================================================================
// implementation:

}	// namespace