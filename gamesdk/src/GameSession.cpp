/*	$Id: GameSession.cpp,v 1.2 2005/11/17 16:36:20 dmitry.vereshchagin Exp $
** 
**  GameSession implementation
*/

#include "gamesession.h"
#include "gamesession_impl.h"
#include "synchronization.h"

namespace Domain
{

GameSession GameSession::null(0);

/*
GameSession::GameSession(void) 
:
	m_impl(0)
{
}
*/

GameSession::GameSession(GameSession_Impl *impl)
:	m_impl(impl)
{
	if (m_impl) m_impl->add_ref();
}

GameSession::GameSession(const GameSession &copy)
:	m_impl(copy.m_impl)
{
	if (m_impl) m_impl->add_ref();
}


GameSession::~GameSession(void)
{
	if (m_impl) m_impl->release_ref();
}

GameSession &GameSession::operator = (const GameSession &other_instance)
{
	if(this != &other_instance)
	{
		if (m_impl) m_impl->release_ref();
		m_impl = other_instance.m_impl;
		if (m_impl) m_impl->add_ref();
	}
	return *this;
}

bool GameSession::operator == (const GameSession &other_instance) const
{
	return (m_impl == other_instance.m_impl);
}

bool GameSession::operator != (const GameSession &other_instance) const
{
	return !(this->operator == (other_instance));
}


bool GameSession::operator < (const GameSession &other_instance) const
{
	return (m_impl < other_instance.m_impl);
}

unsigned int GameSession::get_id() const
{
	return m_impl->get_id();
}

const TCHAR* GameSession::get_name() const
{
	return m_impl->get_name();
}

unsigned int GameSession::get_time_elapsed() const
{
	return m_impl->get_time_elapsed();
}

unsigned int GameSession::get_max_players() const
{
	return m_impl->get_max_players();
}

string GameSession::get_map() const
{
	return m_impl->get_map();
}

Player GameSession::new_player(const string &name)
{
	return m_impl->new_player(name);
}

Player GameSession::get_player(const string &name) const
{
	return m_impl->get_player(name);
}

Player GameSession::get_player(uint id) const
{
	return m_impl->get_player(id);
}

void GameSession::get_players(std::vector<Player> &players) const
{
	m_impl->get_players(players);
}

size_t GameSession::get_num_players() const
{
	return m_impl->m_map_id.size();
}

Player GameSession::remove_player(const Player &player)
{
	return m_impl->remove_player(player);
}

void GameSession::remove_players()
{
	m_impl->remove_players();
}

void GameSession::set_max_players(unsigned int max_players)
{
	m_impl->set_max_players(max_players);
}

void GameSession::set_name(const TCHAR* sz_name)
{
	m_impl->set_name(sz_name);
}

void GameSession::set_map(const TCHAR* sz_map)
{
	m_impl->set_map(sz_map);
}

void GameSession::set_modes(const std::vector<MODE> &modes)
{
	m_impl->set_modes(modes);
}

std::vector<GameSession::MODE> GameSession::get_modes() const
{
	return m_impl->get_modes();
}

void GameSession::set_rules(const std::vector<RULE> &rules)
{
	m_impl->set_rules(rules);
}

std::vector<GameSession::RULE> GameSession::get_rules() const
{
	return m_impl->get_rules();
}

void GameSession::for_each_player(PlayerEnum &functor) const
{
	m_impl->for_each_player(functor);
}

} //namespace Domain