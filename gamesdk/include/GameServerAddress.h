/*	$Id: GameServerAddress.h,v 1.1 2005/11/14 17:07:58 dmitry.vereshchagin Exp $
**	
**	
*/

#pragma once
#include <decl6n.h>

namespace Domain 
{

/// 
class CxxLibExport GameServerAddress
{
public:
	
	/// 
	GameServerAddress(void)
	:	m_str_host()
	,	m_n_port(0)
	{
	}

	GameServerAddress(const TCHAR* sz_host, int port)
		: m_str_host(sz_host)
		, m_n_port(port)
	{
	}


	/// 
	/// @param port 
	void set_port(ushort port)
	{
		m_n_port = port;
	}

	/// 
	/// @param sz_host 
	void set_host(const TCHAR* sz_host)
	{
		m_str_host = sz_host;
	}

	/// 
	/// @return 
	unsigned short get_port(void) const
	{
		return m_n_port;
	}

	/// 
	/// @return 
	const TCHAR* get_host() const
	{
		return m_str_host.c_str();
	}

private:
	/// 
	std::basic_string<TCHAR> m_str_host;

	/// 
	ushort m_n_port;
};

} // namespace