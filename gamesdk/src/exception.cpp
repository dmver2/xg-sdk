#include <atlbase.h>

#include "impl/exception_impl.h"
#include "Exception.h"
#include "impl/exception_impl.h"

//GSDK::Exception::Exception(const wchar_t* msg) : m_impl(new Exception_Impl(msg))
//{}

GSDK::Exception::Exception(const TCHAR* message, int length) : m_impl(new Exception_Impl(std::basic_string<char>(message, length)))
{
}

//GSDK::Exception::Exception(const wchar_t* message, int length) : m_impl(new Exception_Impl(std::basic_string<wchar_t>(message, length)))
//{
//}

GSDK::Exception::Exception(const TCHAR* msg) : m_impl(new Exception_Impl(msg))
{}

GSDK::Exception::~Exception()
{
	delete m_impl;
}

const TCHAR* GSDK::Exception::get_message() const
{
	return m_impl->m_message.data();
}
