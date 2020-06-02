/*  $Id: netobject_client_impl.cpp,v 1.1 2005/11/14 17:07:59 dmitry.vereshchagin Exp $
**	Game SDK
**
**
*/

#include <network/packet.h>
#include "netobject_client_impl.h"
#include "netobject_controller_impl.h"

namespace GSDK
{

//===========================================================================
// NetObject_Client construction:

NetObject_Client_Impl::NetObject_Client_Impl(
	int obj_id,
	const Host &server,
	NetObject_Controller_Impl *controller)
: m_obj_id(obj_id)
, m_server(server)
, m_controller(controller)
, m_msg2signal()
{
	NetObject_Controller_Impl::ClientID client_id(server, obj_id);
	m_controller->m_client_objects[client_id] = this;
}

NetObject_Client_Impl::~NetObject_Client_Impl()
{
	NetObject_Controller_Impl::ClientID client_id(m_server, m_obj_id);
	m_controller->m_client_objects.erase(client_id);
}

//===========================================================================
// NetObject_Client_Impl attributes:


//===========================================================================
// NetObject_Client_Impl operations:

void NetObject_Client_Impl::receive(int msg_type, Packet &packet)
{
	MAP_MSG2SIGNAL::iterator it = m_msg2signal.find(msg_type);
	if (it != m_msg2signal.end())
	{
		it->second(packet);
	}
	else
	{
// todo: unhandled packet signal fired
	}
}

void NetObject_Client_Impl::send(int msg_type, const Packet &message)
{
	Packet netpacket;
	netpacket.output.write_bool8(true);
	netpacket.output.write_int32(m_obj_id);
	netpacket.output.write_int32(msg_type);
	netpacket.output.write(message.get_data(), message.get_size());
	m_server.send(m_controller->m_channel, netpacket);
}

} // namespace