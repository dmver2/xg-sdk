/* $Id: Network_Setup.h,v 1.1 2005/11/14 17:07:58 dmitry.vereshchagin Exp $
**
**
**/

#pragma once
#include <winsock2.h>
#include "decl6n.h"
#pragma warning ( disable : 4290 ) // warning C4290: C++ exception specification ignored except to indicate a function is not __declspec(nothrow)

namespace GSDK
{
class Exception;

/// 
/// 
class CxxLibExport Network_Setup
{
	volatile static int s_ref_count;
	static WSADATA wsa_data;

/// 
	DECLARE_NO_INSTANCE(Network_Setup)
	DECLARE_NO_COPY(Network_Setup)
public:

	/// 
	/**	
	 *	
	 *	
	 *	
	 *	
	 */
	static void startup() throw (Exception);

	static bool up();

	/// 
	/**	
	 *	
	 *	
	 */
	static void shutdown();
};

}