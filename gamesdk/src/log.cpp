/*  $Id: log.cpp,v 1.1 2005/11/14 17:07:59 dmitry.vereshchagin Exp $
**
**	Game SDK
**
*/

#include "log.h"

namespace GSDK
{

Log::SIGNAL Log::s_signal;
Mutex	Log::m_mutex;


//	Signal_v3<const string &, int, const string &> &Log::get_signal()
//	{
//		return s_signal;
//	}

void Log::log(const TCHAR* sz_group, const TCHAR* sz_message)
{
	AutoLockSection<Mutex> lock(m_mutex);
	s_signal(sz_group, Log::info, sz_message);
}

void Log::log(const TCHAR* sz_group, SEVERITY severity, const TCHAR *sz_message)
{
	AutoLockSection<Mutex> lock(m_mutex);
	s_signal(sz_group, severity, sz_message);
}

void Log::printf(const TCHAR *pszgroup, SEVERITY severity, const TCHAR* szformat, ...)
{
	va_list argptr;
	va_start (argptr, szformat);
	vprintf(pszgroup, severity, szformat, argptr);
	va_end (argptr);
}
	
void Log::vprintf(const TCHAR *pszgroup, SEVERITY severity, const TCHAR* szformat, va_list argptr)
{
	const int buff_len = 0x1000;
	TCHAR chbuff[buff_len] = {0};
	int nlen = 0;
	chbuff[0] = '\r';
	chbuff[1] = '\n';
	nlen = ::_vsntprintf(chbuff, buff_len - 3, szformat, argptr);
	if(nlen < 0)
		nlen = buff_len - 3;
	if(nlen > 1 && chbuff[nlen-1] != '\n')
	{
		chbuff[nlen] = '\r';
		chbuff[nlen + 1] = '\n';
		nlen += 2;
	}
	log(pszgroup, severity, chbuff);
}

SigSlot Log::connect(SLOT *slot)
{
	AutoLockSection<Mutex> lock(m_mutex);
	return s_signal.connect(slot);
}

SigSlot Log::connect(void (*callback)(const TCHAR*, int, const TCHAR*))
{
	AutoLockSection<Mutex> lock(m_mutex);
	return connect(new FunctionSlot_v3<const TCHAR*, int, const TCHAR*>(callback));
}

void Log::disconnect(SigSlot &slot_to_disconnect)
{
	AutoLockSection<Mutex> lock(m_mutex);
	s_signal.disconnect(slot_to_disconnect);
}

} // namespace