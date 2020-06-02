/*	$Id: NetInterface_Impl.h,v 1.2 2005/11/17 16:36:20 dmitry.vereshchagin Exp $
**	NetInterface_Impl interface
**
**/

#ifndef header_netinterface
#define header_netinterface

#if _MSC_VER > 1000
#pragma once
#endif

#include "signals.h"
#include "host.h"
#include "hostgroup.h"
#include "decl6n.h"

namespace GSDK
{

class CxxLibExport NetInterface
{
public:
	NetInterface(void);

	NetInterface(class NetInterface_Impl &rimpl);

	NetInterface(const NetInterface &copy);
	
	virtual ~NetInterface(void);

	Signal_v1<const Host &> &sig_host_connected();

	Signal_v1<const Host &> &sig_host_reconnected();

	Signal_v1<const Host &> &sig_host_disconnected();

	Signal_v2<Packet &, const Host &> &sig_packet_received(const string &packet_channel);

	void set_debug(bool f_debug = true);

	void get_all( HostGroup &rgroup);

	bool is_server() const;

private:
	NetInterface &operator = (const NetInterface &rcopy);

public:
	void start_server(const string &port);

	void start_client(const string &port = string());

	void stop();

	Host connect(const IPAddress &address);

	void send_all(const string &packet_channel, const Packet &packet, bool reliable = true);

	void disconnect_all();

	void synchronize_time();

private:
	NetInterface_Impl *m_impl;
};

}   

#endif  