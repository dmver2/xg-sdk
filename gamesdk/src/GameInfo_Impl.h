/*	$Id: GameInfo_Impl.h,v 1.2 2005/11/17 16:36:20 dmitry.vereshchagin Exp $
 * 
 *	GameInfo_Impl interface
 */

#ifndef header_gameinfo_impl
#define header_gameinfo_impl

#if _MSC_VER > 1000
#pragma once
#endif

#include <string>
#include <tchar.h>

#include "types.h"
#include "gameinfo.h"
#include "gamesession.h"
#include "gamesession_impl.h"
#include "decl6n.h"
#include "synchronization.h"
#include "netsession.h"

namespace Domain
{
class GameSession;

class GameInfo_Impl : public Referenceable
{
friend class GameInfo;
friend class GameSession_Impl;
friend class Player_Impl;

private:
// ctor
	GameInfo_Impl(const string &name
				, const string &proto
				, const version &version);

public:
// dtor
	virtual ~GameInfo_Impl(void);
public:

	GameSession new_session();
	GameSession get_session(int session_id) const;
	void close_session(GameSession);
	GameInfo::SessionEnum &for_each_session(GameInfo::SessionEnum &functor) const;
	std::vector<GameSession> &get_sessions(std::vector<GameSession> &sessions) const;

	string get_name() const;
	string get_protocol() const;
	version get_version() const;
    
// data members
private: 
	const string m_name;
	const string m_proto;
	const version m_version;

//	GameSession_Impl *m_psession;
	typedef std::unordered_map<int, GameSession>	MAP_ID2SESSION;
	std::unordered_map<int, GameSession>	m_sessions;
	int m_session_id;

	GSDK::NetSession m_net_session;

	static GSDK::Mutex s_mutex;

	static GameInfo s_instance;
};

} //namespace Domain

#endif //#ifndef header_gameinfo_impl