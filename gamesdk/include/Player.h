/*	$Id: Player.h,v 1.1 2005/11/14 17:07:58 dmitry.vereshchagin Exp $
 *  Player interface
 */

#ifndef header_player
#define header_player

#if _MSC_VER > 1000
#pragma once
#endif

#include <string>
#include <vector>
#include <tchar.h>
#include "decl6n.h"

namespace Domain
{

	class PropertyEnum;

/// 
/**
 * 
 */
class CxxLibExport Player
{

// typedefs
public:
	typedef std::pair<string, string> PROPERTY;

private:
/** 
 *	
 */
//	Player();

public:
/**
 *	
 *  
 */
	Player(class Player_Impl *impl);

public:
/**
 *	
 */
	Player(const Player& copy);

/**
 *	
 */
	virtual ~Player(void);

// 
/**
 *
 */
	Player &Player::operator = (const Player &other_instance);

/**
 *
 */
	bool Player::operator == (const Player &other_instance) const;

/**
 *
 */
	bool Player::operator != (const Player &other_instance) const;

/**
 *
 */
	bool Player::operator < (const Player &other_instance) const;

// 

/// 
/**	
 *	@param score	
 */
	void set_score(unsigned int score);

///	
/**
 *	@param ping	ping	
 */
	void set_ping(unsigned int ping);

///	
/**	
 *	@param	flag	
 */
	void set_timeout(bool flag);

///	
/**
 *	@return 
 */
	unsigned int get_id() const;

///	
/**	
 *	@return 
 */
	unsigned int get_time_elapsed() const;

///	
/**	
 *
 *	@return	
 */
	unsigned int get_score() const;

///	
/**	
 *	@return 
 */
	unsigned int get_ping() const;

///	
/** 
 *	@return 
 */
	string get_name() const;

///	
/**	
 *	@return 
 */
	bool get_timeout() const;

///	
/**	
 *	@param	name 
 *	@return 
 */
	string get_property(const string& name) const;

///	
/**	
 *	@param name	
 *	@param value	
 */
	string set_property(const string& name, const string& value);

///	
/**	
 *	@param name 
 */
	string remove_property(const string& name);
	
///	
/**	
 *
 */
	void remove_properties();

///	
/**	
 *	@param properties	
 */
	void get_properties(/*out*/ std::vector<PROPERTY> &properties);

///	
/**	
 *	@param functor	
 */
	void for_each_property(PropertyEnum &functor);

/**
 *
 */
/*
	template<class _Fn2> inline
	_Fn2 for_each_property(_Fn2 _Func) 
	{
		return m_impl->for_each_property<_Fn2>(_Func);
	}
*/

// 
private:
/**
 * 
 */
	Player_Impl *m_impl;

public:
/**
 * 
 */
	static Player null;
};

class CxxLibExport PropertyEnum 
{
public:
	virtual void operator()(const string &name, const string &value) = 0;
};

} // namespace Domain

#endif
