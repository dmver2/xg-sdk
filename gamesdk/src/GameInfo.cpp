/*	$Id: GameInfo.cpp,v 1.2 2005/11/17 16:36:20 dmitry.vereshchagin Exp $
**	GameInfo implementation
**
*/

#include "gameinfo.h"
#include "gamesession.h"
#include "synchronization.h"
#include "gameinfo_impl.h"
#include "xmlserializer.h"

namespace Domain
{
	using namespace GSDK;

	GameInfo	GameInfo::null(0);

	GameInfo::GameInfo(GameInfo_Impl* impl)
		:
		m_impl(impl)
	{
		if (m_impl)
			m_impl->add_ref();
	}

	GameInfo::GameInfo(const GameInfo& copy)
		: m_impl(copy.m_impl)
	{
		if (m_impl)
			m_impl->add_ref();
	}

	GameInfo::~GameInfo(void)
	{
		if (m_impl)
			m_impl->release_ref();
	}

	GameInfo GameInfo::create(const string& name
		, const string& proto
		, const version& version)
	{
		AutoLockSection<Mutex> lock(GameInfo_Impl::s_mutex);
		if (GameInfo_Impl::s_instance == GameInfo::null)
			GameInfo_Impl::s_instance =
			new GameInfo_Impl(name
				, proto
				, version);
		return GameInfo_Impl::s_instance;
	}

	GameInfo GameInfo::get_instance()
	{
		AutoLockSection<Mutex> lock(GameInfo_Impl::s_mutex);
		return GameInfo_Impl::s_instance;
	}

	GameInfo& GameInfo::operator = (const GameInfo& other_instance)
	{
		if (this != &other_instance)
		{
			GameInfo_Impl* p_impl = m_impl;
			m_impl = other_instance.m_impl;
			if (m_impl)
				m_impl->add_ref();
			if (p_impl)
				p_impl->release_ref();
		}
		return *this;
	}

	bool GameInfo::operator == (const GameInfo& other_instance) const
	{
		return (m_impl == other_instance.m_impl);
	}

	bool GameInfo::operator != (const GameInfo& other_instance) const
	{
		return !(this->operator == (other_instance));
	}

	bool GameInfo::operator < (const GameInfo& other_instance) const
	{
		return (m_impl < other_instance.m_impl);
	}


	string GameInfo::get_name() const
	{
		return m_impl->get_name();
	}

	string GameInfo::get_protocol() const
	{
		return m_impl->get_protocol();
	}

	version GameInfo::get_version() const
	{
		return m_impl->get_version();
	}

	string GameInfo::get_version_str() const
	{
		return m_impl->get_version().to_string();
	}

	GameSession GameInfo::get_session(uint session_id) const
	{
		return m_impl->get_session(session_id);
	}

	GameSession GameInfo::new_session()
	{
		return m_impl->new_session();
	}

	void GameInfo::close_session(GameSession session)
	{
		m_impl->close_session(session);
	}

	GameInfo::SessionEnum& GameInfo::for_each_session(GameInfo::SessionEnum& functor) const
	{
		return m_impl->for_each_session(functor);
	}

	std::vector<GameSession>& GameInfo::get_sessions(std::vector<GameSession>& sessions) const
	{
		return m_impl->get_sessions(sessions);
	}

} // namespace Domain