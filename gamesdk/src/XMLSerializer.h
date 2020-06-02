/*	$Id: XMLSerializer.h,v 1.2 2005/11/17 16:36:20 dmitry.vereshchagin Exp $
** 
**  XMLSerializer interface
*/

#ifndef header_xmlserializer
#define header_xmlserializer

#if _MSC_VER > 1000
#pragma once
#endif
#pragma warning( disable : 4996 ) 

#include <iostream>
#include <sstream>
#include "types.h"

namespace Domain
{

class GameInfo;
class GameSession;

/**
 * Class XMLSerializer does GameInfo class to XML binding.
 */
class XMLSerializer
{
public:

/**
 *
 */
	XMLSerializer(void);

/**
 *
 */
	virtual ~XMLSerializer(void);
	
public:

/**
 *
 */
	void serialize(GameInfo &gameInfo);

/**
 *
 */
	void serialize_players(GameSession &session, bool flush = true);

/**
 *
 */
	void serialize_session(GameSession &session, bool flush = true);

/**
 *
 */
	void serialize_sessions(GameInfo &gameInfo, bool flush = true);


/**
 *
 */
	const char* get_data() const;

/**
 *
 */
	string get_str() const;

/**
 *
 */
	uint get_size();

private:
	std::basic_ostringstream<TCHAR> m_out;

};

} // namespace Domain

#endif //#ifndef header_xmlserializer
