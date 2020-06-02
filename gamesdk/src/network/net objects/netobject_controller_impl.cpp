/*  $Id: netobject_controller_impl.cpp,v 1.1 2005/11/14 17:07:59 dmitry.vereshchagin Exp $
**	Game SDK
**
**
*/

#include "netobject_controller_impl.h"
#include "netobject_server_impl.h"
#include "netobject_client_impl.h"
#include <network/net objects/netobject_client.h>
#include <network/packet.h>
#include <network/netinterface.h>

namespace GSDK
{

//===========================================================================
// NetObject_Controller_Impl construction:

NetObject_Controller_Impl::NetObject_Controller_Impl(
	NetInterface *netinterface,
	const string &channel)
: m_netinterface(netinterface), m_channel(channel), m_obj_id_counter(0)
{
	slot_received_netpacket = netinterface->sig_packet_received(channel).connect(
		this, &NetObject_Controller_Impl::on_packet_receive);
}

NetObject_Controller_Impl::~NetObject_Controller_Impl()
{
//	this->m_netinterface->send_all(m_channel, Packet("", 0));
}

//===========================================================================
// NetObject_Controller_Impl attributes:


//===========================================================================
// NetObject_Controller_Impl operations:


//===========================================================================
// NetObject_Controller_Impl implementation:

void NetObject_Controller_Impl::on_packet_receive(
	Packet &packet,
	const Host &from)
{
	bool server_obj = packet.input.read_bool8();
	int obj_id = packet.input.read_int32();
	int msg_type = packet.input.read_int32();
	int sub_pos = packet.input.tell();
	Packet subpacket(packet.get_data()+sub_pos, packet.get_size()-sub_pos);

	if (server_obj)
	{
		std::map<int, NetObject_Server_Impl *>::iterator it;
		it = m_server_objects.find(obj_id);
		if (it != m_server_objects.end())
		{
			it->second->receive(msg_type, from, subpacket);
		}
		else
		{
			// No such server object.
		}
	}
	else
	{
		std::map<ClientID, NetObject_Client_Impl *>::iterator it;
		it = m_client_objects.find(ClientID(from, obj_id));
		if (it != m_client_objects.end())
		{
			it->second->receive(msg_type, subpacket);
		}
		else
		{
			// No such client object. Construct new one and inform application.

			NetObject_Client netobj(obj_id, from, this);
			m_sig_create_object(netobj, msg_type, subpacket);
		}
	}
}

} // namespace
