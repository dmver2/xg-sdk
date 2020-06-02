/*	$Id: version.h,v 1.1 2005/11/14 17:07:58 dmitry.vereshchagin Exp $
**	Game SDK
*/

#ifndef header_gsdk_version
#define header_gsdk_version

#if _MSC_VER > 1000
#pragma once
#endif

#include "decl6n.h"

namespace Domain
{

/// 
/**	
 */
typedef struct CxxLibExport tag_version
{
	/// 
	unsigned short m_major;

	///	
	unsigned short m_minor;

	///	@return 
	string to_string() const 
	{
		TCHAR buf[12] = {0};
		_sntprintf_s(buf, sizeof(buf), _T("%d.%d"), m_major, m_minor);
		return buf;
	}
} version;

} //namespace Domain

#endif //