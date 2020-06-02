/*  $Id: netobject_client.cpp,v 1.1 2005/11/14 17:07:59 dmitry.vereshchagin Exp $
**	Game SDK
**
**
*/

#include <network/net objects/netobject_client.h>
#include <network/net objects/netobject_controller.h>
#include "netobject_client_impl.h"

namespace GSDK
{

//===========================================================================
// NetObject_Client construction:

NetObject_Client::NetObject_Client(int obj_id, const Host &server, NetObject_Controller_Impl *controller)
: p_impl(new NetObject_Client_Impl(obj_id, server, controller))
{
	p_impl->add_ref();
}

NetObject_Client::NetObject_Client(const NetObject_Client &copy)
: p_impl(copy.p_impl)
{
	if (p_impl) p_impl->add_ref();
}

NetObject_Client::~NetObject_Client()
{
	if (p_impl) p_impl->release_ref();
}

//===========================================================================
// NetObject_Client attributes:

int NetObject_Client::get_id() const
{
	return p_impl->m_obj_id;
}

const Host &NetObject_Client::get_server() const
{
	return p_impl->m_server;
}

//===========================================================================
// NetObject_Client operations:

void NetObject_Client::send(int msg_type, const Packet &message)
{
	p_impl->send(msg_type, message);
}

//===========================================================================
// NetObject_Client signals:

Signal_v1<Packet &> &NetObject_Client::sig_msg_received(int msg_type)
{
	return p_impl->m_msg2signal[msg_type];
}

//===========================================================================
// NetObject_Client implementation:

}	// namespace
