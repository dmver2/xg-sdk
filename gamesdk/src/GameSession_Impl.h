/*	$Id: GameSession_Impl.h,v 1.2 2005/11/17 16:36:20 dmitry.vereshchagin Exp $
 *  GameSession_Impl interface
 */

#ifndef header_gamesession_impl
#define header_gamesession_impl

#if _MSC_VER > 1000
#pragma once
#endif
#pragma warning( disable : 4996 ) 

#include <stdlib.h>
#include <string>
#include <unordered_map>
#include <vector>
#include <tchar.h>
#include "types.h"
#include "gamesession.h"
#include "player.h"
#include "player_impl.h"
#include "util.h"

namespace Domain
{

/**
 *
 */
class GameSession_Impl : public Referenceable
{
friend class GameInfo_Impl;
friend class GameSession;

private:
/**
 *
 */
	typedef std::unordered_map<uint, Player, GSDK::Util::int_hash_comparator> ID2PLAYER_HASH_MAP;
/**
 *
 */
	typedef std::unordered_map<const string, Player, GSDK::Util::string_hash_comparator> NAME2PLAYER_HASH_MAP;

private:
/**
 * ctor
 */ 
	GameSession_Impl(void);

/**
 * copy ctor
 */ 
	GameSession_Impl(const GameSession_Impl &copy);

public:
/**
 *	dtor
 */ 
	virtual ~GameSession_Impl(void);

// methods
/**
 *
 */
	unsigned int get_time_elapsed() const;
/**
 *
 */
	unsigned int get_max_players() const;

/**
 *
 */
	uint get_id() const;

/**
 *
 */
	const TCHAR* get_name() const;

/**
 *
 */
	const TCHAR* get_map() const;

/**
 *
 */
	Player new_player(const string &name);

/**
 *
 */
	Player get_player(const string &name) const;

/**
 *
 */
	Player get_player(uint id) const;

/**
 *
 */
	Player remove_player(const Player& player);

/**
 *
 */
	template<class _Fn1> inline
	_Fn1 for_each_player(_Fn1 _Func) const
	{	// perform function for each element
		AutoLockSection<Mutex> lock( GameInfo_Impl::s_mutex );
		for (ID2PLAYER_HASH_MAP::const_iterator it = m_map_id.begin()
			; it != m_map_id.end(); )
			_Func((Player&)it++->second);
		return (_Func);
	}

/**
 *
 */
	void for_each_player(PlayerEnum &functor) const;

/**
 *
 */
	void get_players(std::vector<Player> &players) const;


/**
 *
 */
	void remove_players();

/**
 *
 */
	void set_max_players(unsigned int);

/**
 *
 */
	void set_name(const TCHAR*);

/**
 *
 */
	void set_map(const TCHAR*);

/**
 *
 */
	void set_modes(const std::vector<GameSession::MODE> &modes);

/**
 *
 */
	std::vector<GameSession::MODE> get_modes() const;

/**
 *
 */
	void set_rules(const std::vector<GameSession::RULE> &rules);

/**
 *
 */
	std::vector<GameSession::RULE> get_rules() const;

// data members
private:
/**
 *
 */
	unsigned int m_start_time;
/**
 *
 */
	unsigned int m_max_players;

/**
 *
 */
	string m_map;

/**
 *
 */
	string m_name;

/**
 *
 */
	uint m_id;

/**
 *	
 */
	ID2PLAYER_HASH_MAP m_map_id;

/**
 *	
 */
	NAME2PLAYER_HASH_MAP m_map_name;

/**
 *	
 */
	std::vector<GameSession::MODE> m_modes;

/**
 *
 */
	std::vector<GameSession::RULE> m_rules;

/**
 *	
 */
	static uint m_player_index;

};

} // namespace Domain

#endif // #ifndef header_gamesession_impl