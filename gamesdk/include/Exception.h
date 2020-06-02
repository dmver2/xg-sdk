/*  $Id: Exception.h,v 1.1 2005/11/14 17:07:57 dmitry.vereshchagin Exp $
**
**
*/

#ifndef header_exception
#define header_exception

#if _MSC_VER > 1000
#pragma once
#endif

#include "decl6n.h"
#include <atlbase.h>

namespace GSDK
{
	// forward declaration
	class Exception_Impl;
	
	/// 
	/**
	 *
	 */

	class CxxLibExport Exception
	{
	public:

		explicit Exception(const TCHAR* message);

//		explicit Exception(const wchar_t* message);

		Exception(const char* message, int length);

//		Exception(const wchar_t* message, int length);

		virtual ~Exception();

		virtual const TCHAR* get_message() const;

	private:
		
		//: The exception implementation
		const Exception_Impl* m_impl;

	};

} // namespace GSDK

#endif //#ifndef header_exception