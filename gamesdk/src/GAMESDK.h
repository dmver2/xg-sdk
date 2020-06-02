/*  $Id: GAMESDK.h,v 1.2 2005/11/17 16:36:20 dmitry.vereshchagin Exp $
 *	
 *
 */

#ifndef header_game_sdk
#define header_game_sdk

#if _MSC_VER > 1000
#pragma once
#endif

#include <string>
#include "tchar.h"
#include "decl6n.h"
#include "version.h"

namespace Domain
{

/**
 *	
 */
class GAMESDK
{

	DECLARE_NO_INSTANCE(GAMESDK)
	DECLARE_NO_COPY(GAMESDK)

public:
	/**
	 * 
	 */
	static const TCHAR		s_sz_title[];

	/**
	 * 
	 */
	static const TCHAR		s_sz_id[];

	/**
	 * 
	 */
	static const version	s_version;

};

} //namespace Domain

#endif // #ifndef header_gsdk_version