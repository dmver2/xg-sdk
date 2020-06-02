/*	$Id: System.cpp,v 1.2 2005/11/17 16:36:20 dmitry.vereshchagin Exp $
** 
**
*/

#include <sys/timeb.h>
#include "stdafx.h"
#include "tchar.h"
#include "stdlib.h"
#include "system.h"
#include <synchronization.h>

namespace GSDK
{

	///	
	static int64 s_delta;

	/// 
	static Mutex  s_mutex;

	/// 
	static Signal_v1<int64> s_sig_network_time_adjusted;

/*
	///	
	static int64 s_delta;

	/// 
	static Mutex  s_mutex;

	/// 
	static Signal_v1<int64> s_sig_network_time_adjusted;
*/

string System::get_exe_path(void)
{
	// Get path to executable:
	TCHAR sz_module_name[_MAX_PATH];
	TCHAR sz_drive[_MAX_DRIVE];
	TCHAR sz_dir[_MAX_DIR];
	TCHAR sz_file_name[0x100];
	TCHAR sz_ext[0x100];
	::GetModuleFileName(0, sz_module_name, _MAX_PATH);
	::_tsplitpath(sz_module_name, sz_drive, sz_dir, sz_file_name, sz_ext);

	return string(sz_drive) + string(sz_dir); 
}

string System::get_exe_pathname(void)
{
	// Get path to executable:
	TCHAR sz_module_name[_MAX_PATH];
	TCHAR sz_drive[_MAX_DRIVE];
	TCHAR sz_dir[_MAX_DIR];
	TCHAR sz_file_name[0x100];
	TCHAR sz_ext[0x100];
	::GetModuleFileName(0, sz_module_name, _MAX_PATH);
	::_tsplitpath(sz_module_name, sz_drive, sz_dir, sz_file_name, sz_ext);

	return string(sz_drive) + string(sz_dir) + _T('\\') + string(sz_file_name); 
}

int64 System::get_time(void)
{
	static LARGE_INTEGER perf_frequency, perf_counter;
	static bool first_time = true;

	if (first_time)
	{
		::QueryPerformanceFrequency(&perf_frequency);
		perf_frequency.QuadPart /= 1000;
		first_time = false;
	}

	::QueryPerformanceCounter(&perf_counter);
	return (int64) (perf_counter.QuadPart / perf_frequency.QuadPart);
}

void System::sleep(int millis)
{
	::Sleep(millis);
}

int64 System::get_absolute_time()
{
	struct __timeb64 t;

	_ftime64( &t );

	return t.time * 1000 + t.millitm;
}

int64 System::get_network_time()
{
	struct __timeb64 t;

	_ftime64( &t );

	AutoLockSection<Mutex> lock(s_mutex);
	return t.time * 1000 + t.millitm + s_delta;
}

void System::adjust_network_time(int64 delta)
{
	AutoLockSection<Mutex> lock(s_mutex);
	s_delta = delta;
	s_sig_network_time_adjusted(delta);
}

Signal_v1<int64> &System::sig_network_time_adjusted()
{
	return s_sig_network_time_adjusted;
}

} // namespace GSDK
