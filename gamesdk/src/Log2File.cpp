/* $Id: Log2File.cpp,v 1.1 2005/11/14 17:07:58 dmitry.vereshchagin Exp $
** Game SDK
**
*/
#include "log2file.h"
#include "log2file_impl.h"
#include "system.h"

#include <stdarg.h>
#include "exception.h"

namespace GSDK
{
	Log2File::sptr Log2File::get_instance(const TCHAR* sz_logname)
	{
		static CriticalSection cs;
		AutoLockSection<CriticalSection> lock(cs);
		Log2File::sptr sp_log(new Log2File_Impl(sz_logname));
		return sp_log;

/*
		std::unordered_map<string, Log2File::sptr>::iterator it = s_logregistry.find(sz_logname);
		if (it == s_logregistry.end())
		{
			sp_log = new Log2File_Impl(sz_logname);
//			const_cast<string&>(sp_log->m_name) = sz_logname;
			s_logregistry.insert(std::unordered_map<string, Log2File::sptr>::value_type(sz_logname, sp_log));
		}
		else
			sp_log = it->second;
		return sp_log;
*/
	}

	Log2File::~Log2File(void)
	{
//		s_logregistry.erase(m_name);
/*
		auto found = s_logregistry.find(m_name);
		if(found != s_logregistry.end())
			s_logregistry.erase(found);
*/
	}

	/*
	void Log2File::puts(const string &msg)
	{
		puts(msg.c_str(), msg.length());
	}

	uint Log2File::puts(const TCHAR* msg, uint len)
	{
		AutoLockSection<CriticalSection> lock(m_cs);
		open();
		log_date_time();
		unsigned long nwritten = write(msg, len * sizeof(TCHAR));
		if(msg[len-1] != '\n')
			write(_T("\r\n"), 2 * sizeof(TCHAR));
		close();
		return nwritten;
	}
	*/

	Log2File::Log2File(const TCHAR* name)
		: m_name(name)
	{
	}

	void Log2File::log_date_time(void)
	{
		SYSTEMTIME st;
		::GetLocalTime(&st);
		TCHAR chbuff[25] = { 0 };
		//								   "DD-MM-YYYY hh:mm:ss.uuu"
		int nlen = ::_stprintf(chbuff, _T("%02u-%02u-%04u %02u:%02u:%02u.%03u\t"),
			st.wDay, st.wMonth, st.wYear,
			st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);

		write(chbuff, nlen * sizeof(TCHAR));
	}

} // namespace
