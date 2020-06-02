/*  $Id: exception_impl.cpp,v 1.1 2005/11/14 17:08:01 dmitry.vereshchagin Exp $
**
**
*/

#include "Types.h"
#include "Exception_Impl.h"


namespace GSDK
{

#ifdef _MBCS	
	Exception_Impl::Exception_Impl(const std::basic_string<char>& message) : m_message(message) //: Exception_ImplBase<std::basic_string<TCHAR>>(message)
	{
//		assign_data(message, message.length());
	}

	Exception_Impl::Exception_Impl(const std::basic_string<wchar_t>& message)
	{
		assign_data(message.c_str(), message.length());
	}


	void Exception_Impl::assign_data(const wchar_t* msg, size_t len) const
	{
		char tmp[0x400] = { 0 };
		size_t converted;
		wcstombs_s(&converted, tmp, sizeof(tmp), msg, (len < sizeof(tmp) ? len : sizeof(tmp)));
		if (0 == converted)
		{
			// REPORT THE CONVERSION ERROR
			//		Log::error("string conversion error");
		}
		(const_cast<string&>(m_message)).assign(tmp);
	}

#elif _UNICODE
	
	Exception_Impl::Exception_Impl(const std::basic_string<char>& message) : m_message(message)
		//: Exception_ImplBase<std::basic_string<TCHAR>>(message)
	{
		//		assign_data(message, message.length());
	}

	Exception_Impl::Exception_Impl(const std::basic_string<wchar_t>& message)
	{
		assign_data(message.c_str(), message.length());
	}

	void Exception_Impl::assign_data(const char* msg, size_t len) const
	{
		wchar_t tmp[0x400] = { 0 };
		size_t converted;
		mbstowcs_s(&converted, tmp, msg, (len < sizeof(tmp) ? len : sizeof(tmp)));
		if (0 == converted)
		{
			// REPORT THE CONVERSION ERROR
	//		Log::error("string conversion error");
		}
		(const_cast<string&>(m_message)).assign(tmp);
	}
#endif
	
} // namespace GSDK

