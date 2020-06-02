/*  $Id: netobject_client_impl.h,v 1.1 2005/11/14 17:07:59 dmitry.vereshchagin Exp $
**	Game SDK
**
**
*/

#ifndef header_netobject_client_generic
#define header_netobject_client_generic

#if _MSC_VER > 1000
#pragma once
#endif

#include <signals.h>
#include <network/host.h>
#include <unordered_map>
#include <Referenceable.h>
#include "../../util.h"

namespace GSDK
{

class NetObject_Controller_Impl;

class NetObject_Client_Impl : public Domain::Referenceable
{
// Construction:
public:
	NetObject_Client_Impl(int obj_id, const Host &server, NetObject_Controller_Impl *controller);

	~NetObject_Client_Impl();

// Attributes:
public:
	int m_obj_id;

	Host m_server;

	NetObject_Controller_Impl *m_controller;

	typedef std::unordered_map< int, Signal_v1<Packet &>, Util::int_hash_comparator> MAP_MSG2SIGNAL;
	MAP_MSG2SIGNAL m_msg2signal;

// Operations:
public:
	void receive(int msg_type, Packet &packet);

	void send(int msg_type, const Packet &message);

// Implementation:

};

}	// namespace

#endif
