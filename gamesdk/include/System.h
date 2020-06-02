/* $Id: System.h,v 1.1 2005/11/14 17:07:58 dmitry.vereshchagin Exp $
**  System interface
*/

#ifndef _header_system_impl_
#define _header_system_impl_

#if _MSC_VER > 1000
#pragma once
#endif

#pragma warning (disable : 4251)

#include <string>
#include "types.h"
#include "decl6n.h"
#include "signals.h"

namespace GSDK
{
class Mutex;

/// 
/*!
	
	
*/
class CxxLibExport System
{
// 
	DECLARE_NO_INSTANCE(System)
	DECLARE_NO_COPY(System)

public:
/*!
	
*/
	static int64 get_time();

/*!
	
*/
	static string get_exe_path();

/*!
	
	
*/
	static string get_exe_pathname();

/*!
	
	\param	millis 
*/
	static void sleep(int millis);

/*!
	
	
*/
	static int64 get_absolute_time();


/*!
	
	
	@see adjust_network_time
*/
	static int64 get_network_time();

/*!
	
	
	\param delta	
*/
	static void adjust_network_time(int64 delta);

	/// 
	/** 
	 *	
	 */
	static Signal_v1<int64> &sig_network_time_adjusted();

};

} // namespace GSDK
#endif // _header_system_impl_