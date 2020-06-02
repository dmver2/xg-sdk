/* $Id: Log2File_Impl.cpp,v 1.1 2005/11/14 17:07:58 dmitry.vereshchagin Exp $
** Game SDK
**
*/
#include "log.h"
#include "log2file_impl.h"
#include "system.h"

#include <unordered_map>
#include <stdarg.h>
#include "exception.h"

namespace GSDK
{

	Log2File_Impl::Log2File_Impl(const TCHAR* sz_logname)
		: Log2File(sz_logname)
		, m_hfile(0)
		, m_cs()
	{
		m_file_name = System::get_exe_path() + _T('\\') + sz_logname + _T(".log");
	}

	Log2File_Impl::~Log2File_Impl(void)
	{
	}

	void Log2File_Impl::log(const TCHAR* sz_group, int severity, const TCHAR* sz_message)
	{
		TCHAR* sz_severity;
		switch (severity)
		{
		case Log::debug:	sz_severity = _T("\t[debug]\t");	break;
		case Log::info:		sz_severity = _T("\t[info]\t");		break;
		case Log::warn:		sz_severity = _T("\t[warn]\t");		break;
		case Log::error:	sz_severity = _T("\t[error]\t");	break;
		case Log::fatal:	sz_severity = _T("\t[fatal]\t");	break;
		default:			sz_severity = _T("\t[info]\t");		break;
		}
		string str_logrec = string(sz_group) + sz_severity + sz_message;
		if (str_logrec.c_str()[str_logrec.length() - 1] != '\n')
			str_logrec += _T("\r\n");

		AutoLockSection<CriticalSection> lock(m_cs);
		open();
		log_date_time();
		write(str_logrec.c_str(), str_logrec.length());
		close();
	}

	uint Log2File_Impl::write(const void* data, uint len)
	{
		AutoLockSection<CriticalSection> lock(m_cs);
		bool b_open = open();
		unsigned long nwritten = 0;
		::WriteFile(m_hfile, data, (DWORD)len, &nwritten, NULL);
		if (b_open)
			close();
		return nwritten;
	}


	bool Log2File_Impl::open()
	{
		if (!m_hfile)
		{
			m_hfile = ::CreateFile(m_file_name.c_str()
				, GENERIC_WRITE
				, FILE_SHARE_READ | FILE_SHARE_DELETE | FILE_SHARE_WRITE
				, NULL, OPEN_ALWAYS
				, FILE_ATTRIBUTE_NORMAL
				, NULL);
			if (m_hfile == INVALID_HANDLE_VALUE)
				throw Exception("Log2File_Impl open failed.");
			::SetFilePointer(m_hfile, 0, NULL, FILE_END);
			return true;
		}
		return false;
	}

	void Log2File_Impl::close()
	{
		if (m_hfile != NULL)
		{
			::CloseHandle(m_hfile);
			m_hfile = 0;
		}
	}

} // namespace
