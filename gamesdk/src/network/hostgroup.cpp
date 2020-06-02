/*  $Id: hostgroup.cpp,v 1.1 2005/11/14 17:07:59 dmitry.vereshchagin Exp $
**
**
*/

#include "network/hostgroup.h"
#include "network/host.h"

namespace GSDK
{



/////////////////////////////////////////////////////////////////////////////
// HostGroup construction:

HostGroup::HostGroup()
{
}

HostGroup::HostGroup(std::list<Host>& group)
:	std::list<Host>(group)
{
}

/////////////////////////////////////////////////////////////////////////////
// HostGroup operations:

void HostGroup::send(const string &packet_channel, const Packet &packet, bool reliable /*= true*/)
{
	for (iterator it = begin(); it != end(); ++it)
	{
		it->send(packet_channel, packet, reliable);
	}
}

void HostGroup::disconnect()
{
	for (iterator it = begin(); it != end(); ++it)
	{
		it->disconnect();
	}
}

/////////////////////////////////////////////////////////////////////////////
// HostGroup implementation:
/*
HostGroup::iterator HostGroup::get_iterator()
{
	iterator it;
	it.m_sp_it = new HostGroup_Impl::iterator(m_sp_impl->get_iterator());
	return it;
}

bool HostGroup::iterator::has_next()
{
	return m_sp_it->has_next();
}

Host &HostGroup::iterator::next()
{
	return m_sp_it->next();
}

Host &HostGroup::iterator::operator * ()
{
	return m_sp_it->operator * ();
}

Host* HostGroup::iterator::operator -> ()
{
	return m_sp_it->operator -> ();
}

HostGroup::iterator& HostGroup::iterator::operator ++ ()
{
	++(*m_sp_it);
	return *this;
}

HostGroup::iterator HostGroup::iterator::operator ++ (int)
{
	HostGroup::iterator copy = *this;
	++(*m_sp_it);
	return copy;
}
*/

/*
//	
public:

	class iterator : public HostGroup::iterator
	{
		friend class HostGroup_Impl;
	private:
		iterator(HOST_SET &map)
			: m_pset(&map)
			, m_it(map.begin())
		{}

	public:
		bool has_next()
		{
			return m_it != m_pset->end();
		}

		Host next()
		{
			Host host = *m_it;
			++m_it;
			return host;
		}

		Host& operator * ()
		{
			return *m_it;
		}

		Host* operator -> ()
		{
			return ((Host*)&m_it.operator*());
		}

		iterator& operator ++ ()
		{
			++m_it;
			return *this;
		}

		HostGroup::iterator operator ++ (int)
		{
			iterator temp = *this;
			++m_it;
			return temp;
		}

	private:
		HOST_SET *m_pset;
		HOST_SET::iterator m_it;
	};

};
*/

} // namespace