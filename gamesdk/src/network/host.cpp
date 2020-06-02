/*  $Id: host.cpp,v 1.4 2005/10/18 17:05:02 dmitry.vereshchagin Exp $
** host implementation
**
*/

#include "exception.h"
#include "network/host.h"
#include "network/packet.h"
#include "host_impl.h"
#include "network/netinterface.h"
#include <log.h>

namespace GSDK
{

	/////////////////////////////////////////////////////////////////////////////
	// Host construction:

	Host::Host()
		: m_impl(nullptr)
	{
	}

	Host::Host(const Host& copy)
		: m_impl(copy.m_impl)
	{
		if (m_impl) m_impl->add_ref();
//		Log::printf("Host", Log::debug, "ctor: %p\t%p", this, m_impl);
	}

	Host::~Host()
	{
//		Log::printf("Host", Log::debug, "dtor: %p\t%p", this, m_impl);
		if (nullptr != m_impl) {
			m_impl->release_ref();
		}
	}

	/////////////////////////////////////////////////////////////////////////////
	// Host attributes:

	const IPAddress Host::get_address() const
	{
		if (m_impl) return m_impl->m_addr;
		return IPAddress();
	}

	int Host::get_session_key(u_char* pKey)
	{
		if (m_impl == 0)
			throw Exception(_T("Host::get_session_key() not allowed on an unattached host object"));

		return m_impl->crypto_factory.getSessionKey(pKey);
	}

	NetInterface Host::get_interface()
	{
		if (m_impl == 0)
			throw Exception(_T("Host::get_session() not allowed on an unattached host object"));

		return NetInterface(*m_impl->m_netifc);
	}

	bool Host::is_disconnected() const
	{
		if (m_impl == 0)
			return true;
		else
			return m_impl->m_disconnected;
	}

	string Host::get_disconnect_reason() const
	{
		static string null_msg(_T("Unattached host object"));

		if (m_impl == 0)
			return null_msg;
		else
			return m_impl->disconnect_reason;
	}


	Host& Host::operator = (const Host& rhost)
	{
		if (this != &rhost)
		{
			if (m_impl) m_impl->release_ref();
			m_impl = rhost.m_impl;
			if (m_impl) m_impl->add_ref();
		}
		return *this;
	}

	bool Host::operator == (const Host& rhost) const
	{
		return (m_impl == rhost.m_impl);
	}

	bool Host::operator < (const Host& rhost) const
	{
		return (m_impl < rhost.m_impl);
	}

	bool Host::operator > (const Host& rhost) const
	{
		return (m_impl > rhost.m_impl);
	}

	/////////////////////////////////////////////////////////////////////////////
	// Host operations:

	void Host::disconnect(string reason)
	{
		m_impl->disconnect(reason);
	}

	void Host::send(const string& packet_channel, const Packet& packet, bool reliable) const
	{
		if (m_impl == NULL)
			throw Exception(_T("Host::send not allowed on an unattached host object"));

		if (reliable)
		{
			m_impl->send_packet(packet_channel, packet.impl);
		}
		else
		{
			m_impl->send_packet_udp(packet_channel, packet.impl);
		}
	}

	/////////////////////////////////////////////////////////////////////////////
	// Host implementation:

	Host::Host(class Host_Impl* impl)
		: m_impl(impl)
	{
		if (impl) impl->add_ref();
	}

	size_t Host::hash() const
	{
		return (m_impl != 0) ? m_impl->hash() : 0;
	}

	string Host::to_string() const
	{
		return this->get_address().to_string();
	}

	TCHAR* Host::to_string(TCHAR buf[0x10], size_t& length) const
	{
		return this->get_address().host_as_text(buf, length);
	}

	TCHAR* Host::address_as_text(TCHAR buf[0x16], size_t& length) const
	{
		return this->get_address().to_string(buf, length);
	}

} // namespace