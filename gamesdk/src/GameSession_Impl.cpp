/*	$Id: GameSession_Impl.cpp,v 1.2 2005/11/17 16:36:20 dmitry.vereshchagin Exp $
** 
**  GameSession_Impl implementation
**
*/

#include "gamesession_impl.h"
#include "gameinfo_impl.h"
#include "gamesession.h"
#include "system.h"
#include "synchronization.h"

namespace Domain
{

struct make_players_vector
{
	make_players_vector(std::vector<Player> &rv_players)
		:	m_rvector_players(rv_players)
	{}

	void operator()(const Player& player) 
	{
		m_rvector_players.push_back(player);
	}

	std::vector<Player> &m_rvector_players;
};

using namespace GSDK;

uint GameSession_Impl::m_player_index = 0;


GameSession_Impl::GameSession_Impl(void) 
:
	m_map()
  , m_max_players(0)
  , m_start_time(System::get_time()/1000)
{
}

GameSession_Impl::GameSession_Impl(const GameSession_Impl &copy)
: 
	m_map(copy.m_map)
  , m_max_players(copy.m_max_players)
  , m_start_time(copy.m_start_time)
  , m_modes(copy.m_modes)
  , m_rules(copy.m_rules)
  , m_map_id(copy.m_map_id)
  , m_map_name(copy.m_map_name)
{
//	m_map_name.insert(copy.m_map_name.begin(), copy.m_map_name.end());
//	m_map_id.insert(copy.m_map_id.begin(), copy.m_map_id.end());
}


GameSession_Impl::~GameSession_Impl(void)
{
	AutoLockSection<Mutex> lock( GameInfo_Impl::s_mutex );
}

uint GameSession_Impl::get_id() const
{
	return m_id;
}

const TCHAR* GameSession_Impl::get_name() const
{
	AutoLockSection<Mutex> lock( GameInfo_Impl::s_mutex );
	return m_name.c_str();
}

unsigned int GameSession_Impl::get_time_elapsed() const
{
	AutoLockSection<Mutex> lock( GameInfo_Impl::s_mutex );
	return System::get_time()/1000 - m_start_time;
}

unsigned int GameSession_Impl::get_max_players() const
{
	AutoLockSection<Mutex> lock( GameInfo_Impl::s_mutex );
	return m_max_players;
}

const TCHAR* GameSession_Impl::get_map() const
{
	AutoLockSection<Mutex> lock( GameInfo_Impl::s_mutex );
	return m_map.c_str();
}


Player GameSession_Impl::new_player(const string &name)
{
	AutoLockSection<Mutex> lock( GameInfo_Impl::s_mutex );
	NAME2PLAYER_HASH_MAP::iterator it = m_map_name.find(name);
	if(it == m_map_name.end()) 
	{
		Player player(new Player_Impl(name, m_player_index++));
		m_map_id.insert(std::pair<uint, Player>(player.get_id(), player));
		m_map_name.insert(std::pair<const string, Player>(name, player));
		return player;
	}
	return Player::null;
}

Player GameSession_Impl::get_player(uint id) const
{
	AutoLockSection<Mutex> lock( GameInfo_Impl::s_mutex );
	ID2PLAYER_HASH_MAP::const_iterator it = m_map_id.find(id);
	if(it != m_map_id.end())
		return it->second;
	return Player::null;
}

Player GameSession_Impl::get_player(const string &name) const
{
	AutoLockSection<Mutex> lock( GameInfo_Impl::s_mutex );
	NAME2PLAYER_HASH_MAP::const_iterator it = m_map_name.find(name);
	if(it != m_map_name.end())
		return it->second;
	return Player::null;
}

Player GameSession_Impl::remove_player(const Player& player)
{
	AutoLockSection<Mutex> lock( GameInfo_Impl::s_mutex );
	Player found = Player::null;

	NAME2PLAYER_HASH_MAP::iterator it2 = m_map_name.find(player.get_name());
	if(it2 != m_map_name.end()) 
	{
		found = it2->second;
		m_map_name.erase(it2);
	}

	ID2PLAYER_HASH_MAP::iterator it = m_map_id.find(player.get_id());
	if(it != m_map_id.end()) 
	{
		found = it->second;
		m_map_id.erase(it);
	}
	return found;
}

void GameSession_Impl::remove_players()
{
	AutoLockSection<Mutex> lock( GameInfo_Impl::s_mutex );
	m_map_id.clear();
	m_map_name.clear();
}

void GameSession_Impl::set_max_players(unsigned int max_players)
{
	AutoLockSection<Mutex> lock( GameInfo_Impl::s_mutex );
	m_max_players = max_players;
}

void GameSession_Impl::set_name(const TCHAR* sz_name)
{
	AutoLockSection<Mutex> lock( GameInfo_Impl::s_mutex );
	m_name = sz_name;
}

void GameSession_Impl::set_map(const TCHAR* sz_map)
{
	AutoLockSection<Mutex> lock( GameInfo_Impl::s_mutex );
	m_map = sz_map;
}

void GameSession_Impl::set_modes(const std::vector<GameSession::MODE> &modes)
{
	AutoLockSection<Mutex> lock( GameInfo_Impl::s_mutex );
	m_modes.erase(m_modes.begin(), m_modes.end());
	m_modes.insert(m_modes.begin(), modes.begin(), modes.end());
}

std::vector<GameSession::MODE> GameSession_Impl::get_modes() const 
{
	AutoLockSection<Mutex> lock( GameInfo_Impl::s_mutex );
	return m_modes;
}

void GameSession_Impl::set_rules(const std::vector<GameSession::RULE> &rules)
{
	AutoLockSection<Mutex> lock( GameInfo_Impl::s_mutex );
	m_rules.erase(m_rules.begin(), m_rules.end());
	m_rules.insert(m_rules.begin(), rules.begin(), rules.end());
}

std::vector<GameSession::RULE> GameSession_Impl::get_rules() const 
{
	AutoLockSection<Mutex> lock( GameInfo_Impl::s_mutex );
	return m_rules;
}

void GameSession_Impl::for_each_player(PlayerEnum &functor) const
{
	AutoLockSection<Mutex> lock( GameInfo_Impl::s_mutex );
	for (ID2PLAYER_HASH_MAP::const_iterator it = m_map_id.begin()
		; it != m_map_id.end(); )
		functor((Player&)it++->second);

	//for (ID2PLAYER_HASH_MAP::const_iterator it = m_map_id.begin()
	//	; it != m_map_id.end(); ++it)
	//	functor((Player&)it->second);
//	return (functor);
}

void GameSession_Impl::get_players(std::vector<Player> &players) const
{
	AutoLockSection<Mutex> lock(GameInfo_Impl::s_mutex);
	for_each_player(make_players_vector(players));
//	return for_each_player<make_players_vector>(make_players_vector()).m_vector_players;
//	make_players_vector enumerator;
//	for_each_player<make_players_vector>(enumerator);
//	return enumerator.m_vector_players;
}

} //namespace Domain