/**	$Id: domain.h,v 1.1 2005/11/14 17:07:58 dmitry.vereshchagin Exp $
 *  Domain header
 */

#ifndef header_domain
#define header_domain

#if _MSC_VER > 1000
#pragma once
#endif

/// 
namespace Domain {
};
 
#include "GameInfo.h"
#include "GamePortal.h"
#include "GameServerAddress.h"
#include "GameSession.h"
#include "Player.h"

#if defined (_MSC_VER)
#ifndef _DLLEXPORT
#if !defined (_DEBUG)
#pragma comment(lib, "gamesdk_dll.lib")
//#pragma comment(lib, "AxisClient_1_4.lib") 
//#pragma comment(lib, "xerces-c_2.lib")
#else
#pragma comment(lib, "gamesdk_dll.lib")
//#pragma comment(lib, "AxisClient_1_4_d.lib") 
//#pragma comment(lib, "xerces-c_2d.lib")
#endif // #if !defined (_DEBUG)
#endif // #ifndef _DLLEXPORT

#pragma comment(lib, "ws2_32.lib")
#endif // #if defined (_MSC_VER)

#endif // #ifndef header_domain