/* 
*  GameSession interface
*/

#ifndef header_gamesession
#define header_gamesession

#if _MSC_VER > 1000
#pragma once
#endif
#pragma warning( disable : 4996 ) 

#include <stdlib.h>
#include <string>
#include <vector>
#include <tchar.h>
#include "types.h"
#include "player.h"
#include "decl6n.h"

namespace Domain
{
class PlayerEnum;

/// 
class CxxLibExport GameSession
{
// types
public:

/** 
 *	
 */
	typedef std::pair<uint, string> MODE;

/**	
 *	
 */
	typedef std::pair<string, string> RULE;

public:

/**
 *	ctor
 */
	GameSession(class GameSession_Impl *impl);

/**
 *	ctor
 */
	GameSession(const GameSession &copy);

/**
 *	dtor
 */
	virtual ~GameSession(void);

// operators

/** 
 *	@param other_instance 
 *	@return 
 */
	GameSession &operator = (const GameSession &other_instance);

/** 
 *  @param other_instance 
 *	@return 
 */
	bool operator == (const GameSession &other_instance) const;

/**
 *	
 */
	bool operator != (const GameSession &other_instance) const;

/** 
 *	
 */
	bool operator < (const GameSession &other_instance) const;

// methods
// Attributes
public:
	/// 
	/// @return 
	unsigned int get_id() const;

	/// 
	/// @return 
	const TCHAR* get_name() const;

///	
/**	
 *	@return 
 */
	unsigned int get_time_elapsed() const;

/// 
/**	
 *	@return 
 */
	unsigned int get_max_players() const;

///	
/** 
 *	@return 
 */
	string get_map() const;

/**	
 *	
 *	@param name	
 *	@return	
 */
	Player new_player(const string &name);

///	
/**	
 *	@name	
 *	@return 
 */
	Player get_player(const string &name) const;

///	
/**
 *	@param id 
 *	@return	
 */
	Player get_player(uint id) const;

/// 
/**	
 *	@param players 
 */
	void get_players(/*out*/ std::vector<Player> &players) const;

///	
/**	
 *	@return	
 */
	size_t GameSession::get_num_players() const;


/**	
 *	@param	player	
 *	@return 
 */
	Player remove_player(const Player &player);

/**	
 *	
 */
	void remove_players();


/**
 *	
 */
/*
template<class _Fn1> inline
	_Fn1 for_each_player(_Fn1 _Func)
	{	// perform function for each element
		return m_impl->for_each_player<_Fn1>(_Func);
	}
*/

///	
/**	
 *	@param functor	
 */
	void for_each_player(PlayerEnum &functor) const;

///	
/**
 *	@param sz_name 
 */
	void set_name(const TCHAR* sz_name);

///	
/**
 *	@param max_players 
 */
	void set_max_players(unsigned int max_players);

///	
/**	
 *	@param sz_map_name	
 */
	void set_map(const TCHAR *sz_map_name);

///	
/**	
 *	@param modes 
 */
	void set_modes(const std::vector<MODE> &modes);

///	
/**
 *	@return 
 */
	std::vector<MODE> get_modes() const;

///	
/**	
 *	@param rules 
 */
	void set_rules(const std::vector<RULE> &rules);

///	
/**
 *	@return 
 */
	std::vector<RULE> get_rules() const;

// data members
private:
	/// 
	GameSession_Impl *m_impl;

public:
/**	
 *
 */
	static GameSession null;

};

class CxxLibExport PlayerEnum 
{
public:
	virtual void operator()(Player&) = 0;
};

} // namespace Domain

#endif
