/*	$Id: Player_Impl.h,v 1.2 2005/11/17 16:36:20 dmitry.vereshchagin Exp $
** 
**  Player_Impl interface
*/

#ifndef header_player_impl
#define header_player_impl

#if _MSC_VER > 1000
#pragma once
#endif
#pragma warning( disable : 4996 ) 

#include <unordered_map>
#include <string>
#include "types.h"
#include "referenceable.h"
#include "player.h"

namespace Domain
{

class Player_Impl : public Referenceable
{
//: Typedefs
	typedef std::unordered_map<string, string> PROPERTY_MAP;

public:
// ctor
	Player_Impl(const string &name, int id);
// dtor
	virtual ~Player_Impl(void);

// methods
	void set_score(unsigned int score);
	void set_ping(unsigned int ping);
	void set_timeout(bool flag);

	unsigned int get_id() const;
	unsigned int get_time_elapsed() const;
	unsigned int get_score() const;
	unsigned int get_ping() const;
	bool get_timeout() const;
	string get_name() const;
	string get_property(const string& name) const;
	string set_property(const string& name, const string& value);
	string remove_property(const string& name);
	void remove_properties();

	void get_properties(/*out*/std::vector<Player::PROPERTY> &properties);

	void for_each_property(PropertyEnum &functor);

	template<class _Fn2> inline
	_Fn2 for_each_property(_Fn2 _Func)
	{	// perform function for each element
		AutoLockSection<Mutex> lock( GameInfo_Impl::s_mutex );
		for (PROPERTY_MAP::iterator it = m_properties.begin()
			; it != m_properties.end(); )
			_Func((Player::PROPERTY&)*it++);
//			_Func(it->first, it->second);
		return (_Func);
	}

// data
private:
	const unsigned int m_id;
	const string m_name;
	unsigned int m_score;
	unsigned int m_ping;
	unsigned int m_start_time;
	bool m_b_timeout;
	PROPERTY_MAP m_properties;
};

} // namespace Domain

#endif //#ifndef header_player_impl
