/*	$Id: NetInterface.cpp,v 1.2 2005/11/17 16:36:20 dmitry.vereshchagin Exp $
**  Net Interface implementation
**
*/

#include "network/netinterface.h"
#include "netinterface_impl.h"
#include "host_impl.h"

namespace GSDK
{

	NetInterface::NetInterface(void)
		: m_impl(new NetInterface_Impl())
	{
		m_impl->add_ref();
	}

	NetInterface::NetInterface(const NetInterface& copy)
		: m_impl(copy.m_impl)
	{
		if (m_impl) m_impl->add_ref();
	}

	NetInterface::NetInterface(NetInterface_Impl& rimpl)
		: m_impl(&rimpl)
	{
		if (m_impl) m_impl->add_ref();
	}

	NetInterface::~NetInterface()
	{
		if (m_impl) m_impl->release_ref();
	}

	NetInterface& NetInterface::operator = (const NetInterface& rcopy)
	{
		if (this != &rcopy)
		{
			if (m_impl) m_impl->release_ref();
			m_impl = rcopy.m_impl;
			if (m_impl) m_impl->add_ref();
		}
		return *this;
	}


	Signal_v1<const Host&>& NetInterface::sig_host_connected()
	{
		return m_impl->sig_host_connected;
	}

	Signal_v1<const Host&>& NetInterface::sig_host_reconnected()
	{
		return m_impl->sig_host_reconnected;
	}

	Signal_v1<const Host&>& NetInterface::sig_host_disconnected()
	{
		return m_impl->sig_host_disconnected;
	}

	Signal_v2<Packet&, const Host&>& NetInterface::sig_packet_received(const string& packetchannel)
	{
		return m_impl->map_netpacket_receive[packetchannel];
	}

	Host NetInterface::connect(const IPAddress& address)
	{
		return m_impl->connect(address);
	}

	CxxLibExport void NetInterface::start_server(const string& port)
	{
		m_impl->start_server(port);
	}

	void NetInterface::start_client(const string& port )
	{
		m_impl->start_client(port);
	}

	void NetInterface::stop()
	{
		m_impl->stop();
	}

	void NetInterface::get_all( HostGroup& host_group)
	{
		if (host_group.size() != 0)
			host_group.clear();
		AutoLockSection<Mutex> lock(m_impl->m_mutex);
		for (NetInterface_Impl::HOST_LIST::iterator it = m_impl->m_hostlist.begin()
			;	it != m_impl->m_hostlist.end()
			;	++it)
			host_group.push_back(*it);
	}

	void NetInterface::set_debug(bool f_debug   )
	{
		m_impl->set_debug(f_debug);
	}

	void NetInterface::send_all(const string& packet_channel, const Packet& packet, bool reliable )
	{
		AutoLockSection<Mutex> lock(m_impl->m_mutex);
		for (NetInterface_Impl::HOST_LIST::iterator it = m_impl->m_hostlist.begin()
			;	it != m_impl->m_hostlist.end()
			;	++it) {
			Host(*it).send(packet_channel, packet, reliable);
		}
	}

	void NetInterface::disconnect_all()
	{
		AutoLockSection<Mutex> lock(m_impl->m_mutex);
		for (NetInterface_Impl::HOST_LIST::iterator it = m_impl->m_hostlist.begin()
			;	it != m_impl->m_hostlist.end()
			;	++it)
			Host(*it).disconnect(_T("disconnect_all"));
	}

	bool NetInterface::is_server() const
	{
		return m_impl->m_bserver;
	}

	void NetInterface::synchronize_time()
	{
		m_impl->synchronize_time();
	}


}  