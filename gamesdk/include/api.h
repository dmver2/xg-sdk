/*  $Id: api.h,v 1.2 2006/01/12 17:34:11 dmitry.vereshchagin Exp $
** 
** Game SDK
** 
**/

/// <p>
/// 
/// </p>

#ifndef header_gsdk_main
#define header_gsdk_main

#if _MSC_VER > 1000
#pragma once
#endif

/// 
namespace GSDK {
};
 
#include "decl6n.h"
#include "event_listener.h"
#include "event_trigger.h"
#include "Exception.h"
#include "inputstream.h"
#include "InputStream_Memory.h"
#include "Log2File.h"
#include "outputstream.h"
#include "outputstream_memory.h"
#include "sharedptr.h"
#include "refptr.h"
#include "Synchronization.h"
#include "System.h"
#include "Thread.h"
#include "Timer.h"
#include "Types.h"
#include "weakptr.h"
#include "log.h"
//#include "gameportal.h"
#include "GameServerAddress.h"
#include "Timer.h"
#include "TimerTask.h"
#include "Crypto.h"

#if defined (_MSC_VER)
#ifndef _DLLEXPORT
/*
#if defined (_DEBUG)
#pragma comment(lib, "AxisClient_D.lib")
#pragma comment(lib, "xerces-c_2d.lib")
#else
#pragma comment(lib, "AxisClient.lib")
#pragma comment(lib, "xerces-c_2.lib")
#endif
#else
*/
//#pragma comment(lib, "gamesdk_dll.lib")
#endif // #ifndef _DLLEXPORT

//#pragma comment(lib, "ws2_32.lib")
#endif // #if defined (_MSC_VER)

#endif  //#ifndef header_gsdk_main