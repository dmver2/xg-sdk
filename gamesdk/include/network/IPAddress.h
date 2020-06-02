/* $Id: IPAddress.h,v 1.1 2005/11/14 17:07:58 dmitry.vereshchagin Exp $
 *	Game SDK
 *	
 */

#ifndef header_ipaddress
#define header_ipaddress

#if _MSC_VER > 1000
#pragma once
#endif
#pragma warning  ( disable : 4290 )

#include <winsock2.h>
// #include <string>

#include "exception.h"
#include "types.h"
#include "decl6n.h"

namespace GSDK
{

/// 
/** 
 *	
 */

class CxxLibExport IPAddress
{
// 
public:
/// 
	IPAddress();

/** 
*	
*	@param addr IP 
*	@param port 
*/
	IPAddress(const in_addr &addr, ushort port);

/** 
*	
*	@param hostname 
*	@param port 
*/
	IPAddress(const string &hostname, const string &port);

/** 
*	
*	@param hostname IP 
*	@param port 
*/
	IPAddress(const string &hostname, unsigned int port);

/** 
 *	
 *	@param port 
 */
	IPAddress(const string &port);

/** 
 *	
 *	@param copy 
 */
	IPAddress(const IPAddress& copy);

/// 
	virtual ~IPAddress(void);

//:	

/**
*	
*	m_netaddr 
*	@param other 
*/
	IPAddress& operator = (const IPAddress& other);

/**
*	
*	m_netaddr 
*	@param other 
*	@return 
*/
	bool operator == (const IPAddress& other) const;

/**
 *
 */
	bool operator != (const IPAddress& other) const;

/**
 *	
 *	m_netaddr 
 *	@param other 
 */
	bool operator < (const IPAddress& other) const;

/**
*	
*	m_netaddr 
*	@param other 
*/
	bool operator > (const IPAddress& other) const;

// 
	/// 
	/// @return IP 
	ushort	get_port() const;

	/// 
	/// @return IP 
	in_addr	get_addr() const;

	/// 
	/// @return 
	string	get_address() const;

	/// <summary>
	/// 
	/// </summary>
	/// <param name="buf"></param>
	/// <param name="length"></param>
	/// <returns></returns>
	TCHAR* to_string(TCHAR buf[0x20], size_t& length) const;

	/// 
	/// @return 
	string to_string() const;

	/// <summary>
	/// 
	/// </summary>
	/// <param name="buf"></param>
	/// <param name="length"></param>
	/// <returns></returns>
	TCHAR* host_as_text(TCHAR buf[0x10], size_t& length) const;

	///	
	///	@param port 
	void set_port(ushort port);

// 

	/// 
	/// @param hostname 
	/// @param port 
	void set_address(const string &hostname, const string &port) throw(Exception);

	/// 
	/// 
	/// @param type	
	/// @param addr	
	/// @param len	
	/// @param domain 
	void get_addrinfo(int type, /*out*/ sockaddr &addr, /*out*/ int &len, int domain = PF_INET) const;

	/// 
	/// @return 
	size_t hash() const;


private:
// 
	in_addr m_netaddr;
	ushort m_port;

};

} // namespace GSDK

#endif //#ifndef header_ipaddress