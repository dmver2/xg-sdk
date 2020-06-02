/* $Id: Log2File_Impl.h,v 1.1 2005/11/14 17:07:58 dmitry.vereshchagin Exp $
** Message log file interface
**
*/
#pragma once

#include "stdafx.h"
#include <string>
#include <tchar.h>
#include "synchronization.h"
#include "sharedptr.h"
#include "log2file.h"

namespace GSDK
{

/// 
	class Log2File_Impl : public Log2File
{
public:
	Log2File_Impl(const TCHAR *sz_logname);

public:
	typedef sharedptr<Log2File_Impl>	sptr;

	static sptr get_instance(const string &logname);

	virtual ~Log2File_Impl(void);

	virtual void log(const TCHAR* sz_group, int severity, const TCHAR* sz_message);

//	void log(const string &group, int severity, const string &message);

	// generic method
	/// 
	/** 
	 *	@param msg 
	 *	@param len 
	 *	@return 
	 */
//	virtual uint puts(const TCHAR* msg, uint len); 

	/// 
	/**
	 *	@param msg 
	 *	@param len 
	 *	@return 
	 */
	virtual uint write(const void* data, uint len); 

/*
	virtual void puts(const string &msg);
	virtual void puts(const TCHAR* szmsg);
	virtual void printf(const TCHAR* format, ...);
	virtual void vprintf(TCHAR* format, va_list argptr);
*/
protected:
	virtual bool open();
	virtual void close();
//	virtual void log_date_time();

	HANDLE m_hfile;
	CriticalSection m_cs;
	string m_file_name;

//	std::unordered_map map_severity;

};

}