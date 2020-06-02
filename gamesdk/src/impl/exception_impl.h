/*  $Id: exception_impl.h,v 1.1 2005/11/14 17:08:01 dmitry.vereshchagin Exp $
**
**
*/

#ifndef exception_impl_header
#define exception_impl_header

#if _MSC_VER > 1000
#pragma once
#endif

#include "Types.h"


namespace GSDK
{
	/// 
	/**
	 *
	 */
	//template <class T> class Exception_ImplBase
	//{
	//public:
	//	Exception_ImplBase() {}
	//	
	//	explicit Exception_ImplBase(const T message) :m_message(message)
	//	{}

	//	virtual ~Exception_ImplBase(void)
	//	{ }

	//	const T m_message;

	//};

	/// 
	/**
	 *
	 */
	class Exception_Impl //: public Exception_ImplBase<std::basic_string<TCHAR> >
	{
	public:
		Exception_Impl(const std::basic_string<char>&);

		Exception_Impl(const std::basic_string<wchar_t>&);

#ifdef _MBCS
		void assign_data(const wchar_t* msg, size_t len) const;
#elif _UNICODE		
		void assign_data(const char* msg, size_t len) const;
#endif		
		const string m_message;
	};

} // namespace GSDK

#endif //#if !defined exception_impl_header
