/* $Id: Log2File.h,v 1.1 2005/11/14 17:07:58 dmitry.vereshchagin Exp $
** Message log file interface
**
*/
#pragma once

#include "decl6n.h"
#include "stdafx.h"
#include <string>
#include <tchar.h>
#include "log.h"
#include "synchronization.h"
#include "sharedptr.h"

namespace GSDK
{

/// 
/// 
class CxxLibExport Log2File
{

public:
	typedef sharedptr<Log2File>	sptr;

	/// 
	/**	
	 *	@param	sz_logname 
	 *	@return 
	 */
	static sptr get_instance(const TCHAR * sz_logname);

	virtual ~Log2File(void);

	/// 
	/** 
	 *	@param sz_group	
	 *	@param severity	
	 *	@param sz_message	
	 */
	virtual void log(const TCHAR* sz_group, int severity, const TCHAR* sz_message) = 0;

	/// 
	/**	
	 *	@param data 
	 *	@param len 
	 *	@return 
	 */
	virtual uint write(const void* data, uint len) = 0; 

	const string get_name() const {
		return m_name;
	}

protected:
	Log2File(const TCHAR* name);
	virtual void log_date_time();
	const string m_name;

//private:
//	static	std::unordered_map<string, Log2File::sptr> s_logregistry;
};

}