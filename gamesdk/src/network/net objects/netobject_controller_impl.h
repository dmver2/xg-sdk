/*  $Id: netobject_controller_impl.h,v 1.1 2005/11/14 17:07:59 dmitry.vereshchagin Exp $
**	Game SDK
**
**
*/

#ifndef header_netobject_controller_impl
#define header_netobject_controller_impl

#if _MSC_VER > 1000
#pragma once
#endif

#ifdef _MSC_VER
#pragma warning(disable : 4786)
#endif

#include <network/host.h>
#include <signals.h>
#include <string>
#include <map>
#include <Referenceable.h>

namespace GSDK
{

class NetInterface;
class Packet;
class NetObject_Client;
class NetObject_Client_Impl;
class NetObject_Server_Impl;

class NetObject_Controller_Impl : public Domain::Referenceable
{
//! Construction:
public:
	NetObject_Controller_Impl(NetInterface *netinterface, const string &channel);

	~NetObject_Controller_Impl();

// Attributes:
public:
	Signal_v3<NetObject_Client &, int, Packet &> m_sig_create_object;

	std::map<int, NetObject_Server_Impl *> m_server_objects;

	typedef std::pair<Host, int> ClientID;

	std::map<ClientID, NetObject_Client_Impl *> m_client_objects;

	NetInterface *m_netinterface;

	string m_channel;

	int m_obj_id_counter;

// Operations:
public:

// Implementation:
public:
	void on_packet_receive(Packet &packet, const Host &from);

	SigSlot slot_received_netpacket;

//	int ref_count;
};

}	// namespace

#endif
