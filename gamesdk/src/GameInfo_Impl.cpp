/*	$Id: GameInfo_Impl.cpp,v 1.2 2005/11/17 16:36:20 dmitry.vereshchagin Exp $
** GameInfo_Impl implementation
** 
*/

#include "./gameinfo_impl.h"
#include "gamesession.h"


namespace Domain
{
using namespace GSDK;

Mutex		GameInfo_Impl::s_mutex;

GameInfo	GameInfo_Impl::s_instance(0);

GameInfo_Impl::GameInfo_Impl( const string &name
				  , const string &proto
				  , const version &version)
:
	m_name(name)
,	m_proto(proto)
,	m_version(version)
//,   m_psession(0)
,	m_session_id(0)
{
	m_net_session.start(_T("6465"));
}

GameInfo_Impl::~GameInfo_Impl(void)
{
	m_net_session.stop();
	//if(m_psession != 0)
	//	m_psession->release_ref();
}

string GameInfo_Impl::get_name() const 
{
	return m_name;
}

string GameInfo_Impl::get_protocol() const 
{
	return m_proto;
}

version GameInfo_Impl::get_version() const 
{
	return m_version;
}

//GameSession GameInfo_Impl::get_session() const
//{
//	AutoLockSection<Mutex> lock(s_mutex);
//	return m_psession;
//}

GameSession GameInfo_Impl::new_session()
{
	AutoLockSection<Mutex> lock(s_mutex);
	//if(m_psession == 0)
	//{
	//	m_psession = new GameSession_Impl();
	//	m_psession->add_ref();
	//}
	//else
	//{
	//	GameSession_Impl *p_new_session = new GameSession_Impl(*m_psession);
	//	p_new_session->add_ref();
	//	m_psession->release_ref();
	//	m_psession = p_new_session;
	//}
	GameSession_Impl *p_session_impl = new GameSession_Impl();
	p_session_impl->m_id = ++m_session_id;
	GameSession session(p_session_impl);
	m_sessions.insert(std::unordered_map<int, GameSession>::value_type(p_session_impl->get_id(), session));
	return session;
}

GameSession GameInfo_Impl::get_session(int session_id) const
{
	AutoLockSection<Mutex> lock(s_mutex);
	std::unordered_map<int, GameSession>::const_iterator it = m_sessions.find(session_id);
	if(it != m_sessions.end())
		return it->second;
	else
		return GameSession::null;
}

void GameInfo_Impl::close_session(GameSession session)
{
	AutoLockSection<Mutex> lock(s_mutex);
	std::unordered_map<int, GameSession>::iterator it = m_sessions.find(session.get_id());
	if(it != m_sessions.end())
		m_sessions.erase(it);
}

GameInfo::SessionEnum &GameInfo_Impl::for_each_session(GameInfo::SessionEnum &functor) const
{
	AutoLockSection<Mutex> lock(s_mutex);
	for (std::unordered_map<int, GameSession>::const_iterator it = m_sessions.begin()
		; it != m_sessions.end(); )
		functor((GameSession&)it++->second);
	return functor;
}

std::vector<GameSession>& GameInfo_Impl::get_sessions(std::vector<GameSession> &sessions) const
{
	AutoLockSection<Mutex> lock(s_mutex);
	if(sessions.size() != 0)
		sessions.clear();
	for (std::unordered_map<int, GameSession>::const_iterator it = m_sessions.begin()
		; it != m_sessions.end(); )
		sessions.push_back(it->second);
	return sessions;
}

} // namespace Domain