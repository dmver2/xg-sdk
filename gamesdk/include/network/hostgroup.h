#ifndef header_hostgroup
#define header_hostgroup

#if _MSC_VER > 1000
#pragma once
#endif
#pragma warning( disable : 4996 ) 

#include <network/host.h>
#include <list>
#include "decl6n.h"


namespace GSDK
{

	class CxxLibExport HostGroup : public std::list<Host>
	{

	public:
		HostGroup();

		HostGroup(std::list<Host>& group);

		void send(const string& packet_channel, const Packet& packet, bool reliable = true);

		void disconnect();

	};   

} 

#endif
