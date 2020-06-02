/*	$Id: XMLSerializer.cpp,v 1.2 2005/11/17 16:36:20 dmitry.vereshchagin Exp $
 *	XMLSerializer implementation
 *
 */

#include "StdAfx.h"
#include "xmlserializer.h"
#include "gamesdk.h"
#include "gameinfo.h"
#include "gamesession.h"
#include "player.h"

namespace Domain
{

class session2xml : public GameInfo::SessionEnum
{
public:
	session2xml(XMLSerializer &s) : m_serializer(s)
	{}

	void operator() (GameSession &session) override
	{
		m_serializer.serialize_session(session);
	}
private:
	XMLSerializer &m_serializer;
};

class property2xml : public PropertyEnum
{
public:
	property2xml(std::ostream &os) : m_r_out(os)
	{}

	void operator() (const string &name, const string &value)
	{
		m_r_out << "<param>" << std::endl;
		m_r_out << "<name>" << name << "</name>" << std::endl;
		m_r_out << "<value>" << value << "</value>" << std::endl;
		m_r_out << "</param>" << std::endl;
	}
private:
	std::ostream &m_r_out;
};

class player2xml : public PlayerEnum
{
public:
	player2xml(std::ostream &os) 
		: out(os)
	{}

	void operator() (Player& p)
	{
		out << "<player>" << std::endl;
		out << "<id>" << p.get_id() << "</id>" << std::endl;
		out << "<name>" << p.get_name() << "</name>" << std::endl;
		out << "<score>" << p.get_score() << "</score>" << std::endl;
		out << "<ping>" << p.get_ping() << "</ping>" << std::endl;
		out << "<time>" << p.get_time_elapsed() << "</time>" << std::endl;
		out << "<additional-map>" << std::endl;
		p.for_each_property(property2xml(out));
		out << "</additional-map>" << std::endl;
		out << "</player>" << std::endl;
	}

private:
	std::ostream &out;
};

XMLSerializer::XMLSerializer(void)
{
}

XMLSerializer::~XMLSerializer(void)
{
}

void XMLSerializer::serialize(GameInfo &gameInfo)
{
	m_out << "<info>" << std::endl;

	m_out << "<sdk>" << std::endl;
	m_out << "<version>" << GAMESDK::s_version.to_string() << "</version>" << std::endl;
	m_out << "</sdk>" << std::endl;

	m_out << "<game-info>" << std::endl;
	m_out << "<name>" << gameInfo.get_name() << "</name>" << std::endl;
	m_out << "<protocol>" << std::endl;
	m_out << "<name>" << gameInfo.get_protocol() << "</name>" << std::endl;
	m_out << "<version>" << gameInfo.get_version_str() << "</version>" << std::endl;
	m_out << "</protocol>" << std::endl;
	m_out << "</game-info>" << std::endl;

//	serialize_session(gameInfo.get_session(), false);
	serialize_sessions(gameInfo, false);

	m_out << "</info>" << std::endl;
	m_out.flush();
}

void XMLSerializer::serialize_session(GameSession &session, bool flush)
{
	m_out << "<game-session-info>" << std::endl;
	m_out << "<id>" << session.get_id() << "</id>" << std::endl;
	m_out << "<name>" << session.get_name() << "</name>" << std::endl;
	m_out << "<time>" << session.get_time_elapsed() << "</time>" << std::endl;
	m_out << "<max-players>" << session.get_max_players() << "</max-players>" << std::endl;
	m_out << "<map>" << std::endl;
	m_out << "<name>" << session.get_map() << "</name>" << std::endl;
    m_out << "</map>"  << std::endl;

	m_out << "<mods-list>" << std::endl;

	std::vector<GameSession::MODE> modes = session.get_modes();
	for ( std::vector<GameSession::MODE>::iterator it = modes.begin()
			; it != modes.end()
			; ++it )
	{
		m_out << "<mod>" << std::endl;
		m_out << "<id>" << it->first << "</id>" << std::endl;
		m_out << "<name>" << it->second << "</name>" << std::endl;
		m_out << "</mod>" << std::endl;
	}
	m_out << "</mods-list>" << std::endl;

	m_out << "<rules-list>" << std::endl;

	std::vector<GameSession::RULE> rules = session.get_rules();
	for ( std::vector<GameSession::RULE>::iterator it2 = rules.begin()
			; it2 != rules.end()
			; ++it2 )
	{
		m_out << "<rule>" << std::endl;
        m_out << "<name>" << it2->first << "</name>"  << std::endl;;
        m_out << "<value>" << it2->second << "</value>"  << std::endl;
		m_out << "</rule>" << std::endl;
	}
	m_out << "</rules-list>" << std::endl;

	serialize_players(session, false);

	m_out << "</game-session-info>" << std::endl;

	if(flush)
		m_out << std::flush;
}

void XMLSerializer::serialize_sessions(GameInfo &gameInfo, bool flush)
{

    m_out << "<sessions-list>" << std::endl;

	gameInfo.for_each_session(
			session2xml(*this)
			);

	m_out << "</sessions-list>" << std::endl;

	if(flush)
		m_out << std::flush;
}

void XMLSerializer::serialize_players(GameSession &session, bool flush)
{

    m_out << "<players-list>" << std::endl;

	session.for_each_player(player2xml(m_out));

	m_out << "</players-list>" << std::endl;

	if(flush)
		m_out << std::flush;
}

const char* XMLSerializer::get_data() const
{
	return m_out.str().c_str();
}

string XMLSerializer::get_str() const
{
	return m_out.str();
}

uint XMLSerializer::get_size() 
{
	return m_out.tellp();
}

} // namespace Domain
