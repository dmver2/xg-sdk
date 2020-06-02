/*	$Id: NetException.h,v 1.1 2005/11/14 17:07:58 dmitry.vereshchagin Exp $
**	Game SDK
**
*/

#pragma once
#include "exception.h"
#include "decl6n.h"

namespace GSDK
{

class NetException :
	public Exception
{
public:
	NetException(const char* message, unsigned int error_code)
		: Exception(message)
		, m_error_code(error_code)
	{
	}

	NetException(const char* message, int len, unsigned int error_code)
		: Exception(message, len)
		, m_error_code(error_code)
	{
	}

	~NetException(void)
	{
	}

	unsigned int get_error_code() const
	{
		return m_error_code;
	}
	
private:
	unsigned int m_error_code;
};

} // namespace