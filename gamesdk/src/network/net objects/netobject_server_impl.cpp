/*  $Id: netobject_server_impl.cpp,v 1.1 2005/11/14 17:07:59 dmitry.vereshchagin Exp $
**	Game SDK
**
**
*/

#include "netobject_server_impl.h"
#include "netobject_controller_impl.h"
#include <network/packet.h>
#include <network/hostgroup.h>
#include <network/netinterface.h>

namespace GSDK 
{

//===========================================================================
// NetObject_Server_Impl construction:

NetObject_Server_Impl::NetObject_Server_Impl(
	NetObject_Controller_Impl *controller)
: m_obj_id(controller->m_obj_id_counter++)
, m_pcontroller(controller)
{
	m_pcontroller->m_server_objects[m_obj_id] = this;
}

NetObject_Server_Impl::~NetObject_Server_Impl()
{
	m_pcontroller->m_server_objects.erase(m_pcontroller->m_server_objects.find(m_obj_id));
}

//===========================================================================
// NetObject_Server_Impl attributes:


//===========================================================================
// NetObject_Server_Impl operations:

void NetObject_Server_Impl::receive(int msg_type, const Host &from, Packet &packet)
{
	std::unordered_map< int, Signal_v2<const Host &, Packet &> >::iterator it;
	it = sig_msg_received.find(msg_type);
	if (it != sig_msg_received.end())
	{
		it->second(from, packet);
	}
	else
	{
		// No message handler found.
		// Inform application somehow?
	}
}

void NetObject_Server_Impl::send(HostGroup &group, int msg_type, const Packet &message)
{
	Packet packet;
	group.send(m_pcontroller->m_channel, marshall(msg_type, message, packet));
}

void NetObject_Server_Impl::send(const Host &host, int msg_type, const Packet &message)
{
	Packet packet;
	host.send(m_pcontroller->m_channel, marshall(msg_type, message, packet));
}

void NetObject_Server_Impl::send_all(int msg_type, const Packet &message)
{
	Packet packet;
	m_pcontroller->m_netinterface->send_all(m_pcontroller->m_channel, marshall(msg_type, message, packet));
}

//===========================================================================
// NetObject_Server_Impl implementation:

Packet & NetObject_Server_Impl::marshall(int msg_type, const Packet &input_packet, OUT Packet &out_packet)
{
	out_packet.output.write_bool8(false);
	out_packet.output.write_int32(m_obj_id);
	out_packet.output.write_int32(msg_type);
	out_packet.output.write(input_packet.get_data(), input_packet.get_size());
	return out_packet;
}

}