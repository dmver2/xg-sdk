/*	$Id: Player_Impl.cpp,v 1.2 2005/11/17 16:36:20 dmitry.vereshchagin Exp $
**
** Player_Impl implementation
*/

#include "player_impl.h"
#include "gameinfo_impl.h"
#include "system.h"
#include "synchronization.h"

namespace Domain 
{
using namespace GSDK;

struct make_properties_vector
{
	make_properties_vector(std::vector<Player::PROPERTY> &rvector)
		: m_rvector_prop (rvector)
	{	}

	void operator() (Player::PROPERTY &rproperty)
	{
		m_rvector_prop.push_back(rproperty);
	}

	std::vector<Player::PROPERTY> &m_rvector_prop;
};

Player_Impl::Player_Impl(const string& name, int id)
	: m_name(name)
	, m_start_time(GSDK::System::get_time()/1000)
	, m_id(id)
	, m_score(0)
	, m_properties()
{
	AutoLockSection<Mutex> lock( GameInfo_Impl::s_mutex );
}

Player_Impl::~Player_Impl(void)
{
}

unsigned int Player_Impl::get_id() const
{
	AutoLockSection<Mutex> lock( GameInfo_Impl::s_mutex );
	return m_id;
}

bool Player_Impl::get_timeout() const
{
	AutoLockSection<Mutex> lock( GameInfo_Impl::s_mutex );
	return m_b_timeout;
}


unsigned int Player_Impl::get_time_elapsed() const
{
	AutoLockSection<Mutex> lock( GameInfo_Impl::s_mutex );
	return System::get_time()/1000 - m_start_time;
}

unsigned int Player_Impl::get_score() const
{
	AutoLockSection<Mutex> lock( GameInfo_Impl::s_mutex );
	return m_score;
}

unsigned int Player_Impl::get_ping() const
{
	AutoLockSection<Mutex> lock( GameInfo_Impl::s_mutex );
	return m_ping;
}

string Player_Impl::get_name() const
{
	AutoLockSection<Mutex> lock( GameInfo_Impl::s_mutex );
	return m_name;
}

void Player_Impl::set_score(unsigned int score)
{
	AutoLockSection<Mutex> lock( GameInfo_Impl::s_mutex );
	m_score = score;
}

void Player_Impl::set_ping(unsigned int ping)
{
	AutoLockSection<Mutex> lock( GameInfo_Impl::s_mutex );
	m_ping = ping;
}

void Player_Impl::set_timeout(bool flag)
{
	AutoLockSection<Mutex> lock( GameInfo_Impl::s_mutex );
	m_b_timeout = flag;
}

string Player_Impl::get_property(const string& name) const
{
	AutoLockSection<Mutex> lock( GameInfo_Impl::s_mutex );
	PROPERTY_MAP::const_iterator it = m_properties.find(name);
	if(it != m_properties.end())
		return it->second;
	else
		return string();
}

string Player_Impl::set_property(const string& name, const string& value)
{
	AutoLockSection<Mutex> lock( GameInfo_Impl::s_mutex );
	return m_properties[name] = value;
}

string Player_Impl::remove_property(const string& name)
{
	AutoLockSection<Mutex> lock( GameInfo_Impl::s_mutex );
	string found;
	PROPERTY_MAP::iterator it = m_properties.find(name);
	if(it != m_properties.end())
	{
		found = it->second;
		m_properties.erase(it);
	}
	return found;
}

void Player_Impl::remove_properties()
{
	AutoLockSection<Mutex> lock( GameInfo_Impl::s_mutex );
//	m_properties = PROPERTY_MAP();
	m_properties.clear();
}

void Player_Impl::for_each_property(PropertyEnum &functor)
{
	AutoLockSection<Mutex> lock( GameInfo_Impl::s_mutex );
	for (PROPERTY_MAP::iterator it = m_properties.begin()
		; it != m_properties.end(); )
	{
		PROPERTY_MAP::iterator it2 = it++;
		functor(it2->first, it2->second);
	}
//	return (functor);
}

void Player_Impl::get_properties(std::vector<Player::PROPERTY> &properties)
{
	AutoLockSection<Mutex> lock( GameInfo_Impl::s_mutex );
	for_each_property(make_properties_vector(properties));
}

} // namespace Domain