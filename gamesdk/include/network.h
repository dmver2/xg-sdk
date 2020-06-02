/*  $Id: network.h,v 1.1 2005/11/14 17:07:58 dmitry.vereshchagin Exp $
** Game SDK
**
**
*/

/// <p>
/// 
/// </p>

#ifndef header_gsdk_network_api
#define header_gsdk_network_api

#if _MSC_VER > 1000
#pragma once
#endif

#ifdef WIN32
#pragma warning( disable : 4786)
#endif

#include "network/net objects/netobject_client.h"
#include "network/net objects/netobject_controller.h"
#include "network/net objects/netobject_server.h"

#include "network/host.h"
#include "network/hostgroup.h"
#include "network/inputstream_packet.h"
#include "network/inputstream_socket.h"
#include "network/IPAddress.h"
#include "network/NetInterface.h"
#include "network/Network_Setup.h"
#include "network/outputstream_packet.h"
#include "network/outputstream_socket.h"
#include "network/packet.h"
#include "network/socket.h"
#include "decl6n.h"

#endif
