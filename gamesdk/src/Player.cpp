/*	$Id: Player.cpp,v 1.2 2005/11/17 16:36:20 dmitry.vereshchagin Exp $
**
**	Player implementation
**
*/

#include "player.h"
#include "player_impl.h"

namespace Domain 
{

	Player Player::null(0);
/*
Player::Player()
	: m_impl(0)
{
}
*/

Player::Player(Player_Impl *impl)
	: m_impl(impl)
{
	if(m_impl) m_impl->add_ref();
}

Player::Player(const Player& copy)
	: m_impl(copy.m_impl)
{
	if(m_impl) m_impl->add_ref();
}

Player::~Player(void)
{
	if(m_impl) m_impl->release_ref();
}

Player &Player::operator = (const Player &other_instance)
{
	if(this != &other_instance)
	{
		if (m_impl) m_impl->release_ref();
		m_impl = other_instance.m_impl;
		if (m_impl) m_impl->add_ref();
	}
	return *this;
}

bool Player::operator == (const Player &other_instance) const
{
	return (m_impl == other_instance.m_impl);
}

bool Player::operator != (const Player &other_instance) const
{
	return !(this->operator == (other_instance));
}

bool Player::operator < (const Player &other_instance) const
{
	return (m_impl < other_instance.m_impl);
}

unsigned int Player::get_id() const
{
	return m_impl->get_id();
}

unsigned int Player::get_time_elapsed() const
{
	return m_impl->get_time_elapsed(); 
}

unsigned int Player::get_score() const
{
	return m_impl->get_score();
}

unsigned int Player::get_ping() const
{
	return m_impl->get_ping();
}

string Player::get_name() const
{
	return m_impl->get_name();
}

bool Player::get_timeout() const
{
	return m_impl->get_timeout();
}

void Player::set_score(unsigned int score)
{
	m_impl->set_score(score);
}

void Player::set_ping(unsigned int ping)
{
	m_impl->set_ping(ping);
}

void Player::set_timeout(bool flag)
{
	m_impl->set_timeout(flag);
}

string Player::get_property(const string& name) const
{
	return m_impl->get_property(name);
}

string Player::set_property(const string& name, const string& value)
{
	return m_impl->set_property(name, value);
}

string Player::remove_property(const string& name)
{
	return m_impl->remove_property(name);
}

void Player::remove_properties()
{
	return m_impl->remove_properties();
}

void Player::for_each_property(PropertyEnum &functor)
{
	m_impl->for_each_property(functor);
}

void Player::get_properties(std::vector<Player::PROPERTY> &properties)
{
	m_impl->get_properties(properties);
}

} // namespace Domain