/*  $Id: host.h,v 1.6 2006/01/18 17:05:02  Exp $
** Game SDK
**
*/

#ifndef header_gsdk_host
#define header_gsdk_host

#if _MSC_VER > 1000
#pragma once
#endif

#include <string>
#include "ipaddress.h"
#include "decl6n.h"

namespace GSDK
{

class NetInterface;
class Packet;
class Host_Impl;

/// 
class CxxLibExport Host
{
// 
public:
	/// 
	Host();

	/// 
	/// @param copy 
	Host(const Host &copy);

	/// 
	/// 
	virtual ~Host();

// 
public:
	/// 
	/// @return IPAddress 
	const IPAddress get_address() const;

	/// 
	/// @return 
	/// @param pKey - 
	int get_session_key(u_char *pKey);

	/// 
	/// @return NetInterface 
	NetInterface get_interface();

	/// 
	/// @return 
	bool is_disconnected() const;

	/// 
	///
	/// @return 
	string get_disconnect_reason() const;

	/// 
	///
	/// @param rhost 
	/// @return 
	bool operator == (const Host &rhost) const;

	/// 
	/// <p>
	///
	/// @param rhost 
	/// @return 
	bool operator < (const Host &rhost) const;

	/// 
	/// <p>
	///
	/// @param rhost 
	/// @return 
	bool operator > (const Host &rhost) const;

//! 
public:
	/// 
	///
	/// @param reason 
	void disconnect(string reason = string(_T("Unexpected disconnect")));

	/// 
	/// 
	/// @param packet_channel 
	/// @param packet 
	/// @param reliable	
	void send(
			const string &packet_channel
		,	const Packet &packet
		,	bool reliable = true) const;

	/// 
	/// @param rhost 
	/// @return 
	Host &operator = (const Host &rhost);

	/// 
	/// @return 
	size_t hash() const;

	//: not for export, internal use only
	///
	string to_string() const;

	TCHAR* to_string(TCHAR buf[0x16], size_t& length) const;

	TCHAR* address_as_text(TCHAR buf[0x16], size_t& length) const;

//: 
public:
	/// 
	/// @param impl - 
	Host(class Host_Impl *impl);

//: 
private:
	/// 
	Host_Impl *m_impl;
};

} // namespace

#endif //#ifndef header_gsdk_host
