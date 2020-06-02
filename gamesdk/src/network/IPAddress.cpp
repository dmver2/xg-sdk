/* $Id: IPAddress.cpp,v 1.1 2005/11/14 17:07:59 dmitry.vereshchagin Exp $
 * Game SDK
 *
 */

#include "network/ipaddress.h"
#include <atlbase.h>
#include <WS2tcpip.h>
#include "../Util.h"

namespace GSDK
{

IPAddress::IPAddress()
: m_port(0)
{
	m_netaddr.S_un.S_addr = INADDR_ANY;
}

IPAddress::IPAddress(const in_addr &addr, ushort port)
:	m_port(port)
,	m_netaddr(addr)
{
//	m_netaddr.S_un.S_addr = addr;
}

IPAddress::IPAddress(const string &hostname, const string &port)
{
	set_address(hostname, port);
}

IPAddress::IPAddress(const string &hostname, unsigned int port)
: m_port(port)
{
	InetPton(AF_INET, hostname.c_str(), &m_netaddr.S_un);
	if(INADDR_NONE == m_netaddr.S_un.S_addr)
	{
		hostent *phostent = ::gethostbyname(hostname.c_str());
		if(phostent != NULL)
			memcpy(&m_netaddr, phostent->h_addr_list[0], phostent->h_length); 
		else
			throw new Exception(_T("IPAddress::set_adderss failed"));
	}
}

IPAddress::IPAddress(const string &port)
{
	m_port = std::stoi(port);
	m_netaddr.S_un.S_addr = INADDR_ANY;
}

IPAddress::IPAddress(const IPAddress& copy)
	:	m_netaddr(copy.m_netaddr)
	,	m_port(copy.m_port)
{
}

IPAddress::~IPAddress(void)
{
}

ushort	IPAddress::get_port() const
{
	return m_port;
}

in_addr	IPAddress::get_addr() const
{
	return m_netaddr;
}

string IPAddress::get_address() const
{
	//USES_CONVERSION;
	//char *buf = ::inet_ntoa(m_netaddr);
	const TCHAR* pchAddr = ::inet_ntoa(m_netaddr);
	TCHAR tmp[0x20] = { 0 };
	::_stprintf(tmp, _T("%s"), pchAddr);
	return string(tmp);
}

string IPAddress::to_string() const
{
//	USES_CONVERSION;
//	const TCHAR *pchAddr = A2T(::inet_ntoa(m_netaddr));

/*
	sprintf(buf, "%d.%d.%d.%d"
		, m_netaddr && 0xFF000000
		, m_netaddr && 0xFF0000
		, m_netaddr && 0xFF00
		, m_netaddr && 0xFF
		);
*/	
	TCHAR tmp[0x20] = {0};
	::_stprintf(tmp, _T("%s:%d"), ::inet_ntoa(m_netaddr), m_port);
	return string(tmp);
}

TCHAR* IPAddress::host_as_text(TCHAR buf[0x10], size_t& length) const
{
	const TCHAR* pchAddr = ::inet_ntoa(m_netaddr);
	length = ::_stprintf_s(buf, 0x10, _T("%s"), pchAddr);
	return buf;
}

TCHAR* IPAddress::to_string(TCHAR buf[0x16], size_t &length) const
{
	length = ::_stprintf_s(buf, 0x16, _T("%s:%d"), ::inet_ntoa(m_netaddr), m_port);
	return buf;
}

void IPAddress::set_address(const string &hostname, const string &port)
	throw(Exception)
{
	m_port = std::stoi(port);
	InetPton(AF_INET, hostname.c_str(), &m_netaddr.S_un);
	if(INADDR_NONE == m_netaddr.S_un.S_addr)
	{
		hostent *phostent = ::gethostbyname(hostname.c_str());
		if(phostent != NULL)
			memcpy(&m_netaddr, phostent->h_addr_list[0], phostent->h_length); 
		else
			throw new Exception(_T("IPAddress::set_address failed"));
	}
}

IPAddress& IPAddress::operator = (const IPAddress& other)
{
	if(this != &other)
	{
		m_port = other.m_port;
		m_netaddr = other.m_netaddr;
	}
	return *this;
}

bool IPAddress::operator == (const IPAddress& other) const
{
	return (this == &other) ? true : 
		m_netaddr.S_un.S_addr == other.m_netaddr.S_un.S_addr
		&& m_port == other.m_port;
}

bool IPAddress::operator != (const IPAddress& other) const
{
	return !(*this == other);
}

bool IPAddress::operator < (const IPAddress& other) const
{
	bool ret_val;
	if (this == &other) 
		ret_val = false;
	else if(m_netaddr.S_un.S_addr == other.m_netaddr.S_un.S_addr)
		ret_val = (m_port < other.m_port);
	else
		ret_val = (m_netaddr.S_un.S_addr < other.m_netaddr.S_un.S_addr);
	return ret_val;
}

bool IPAddress::operator > (const IPAddress& other) const
{
	return other < *this;
}

void IPAddress::get_addrinfo(int type, sockaddr &addr, int &len, int domain) const
{
	// Domain ignored since it will always be ipv4
	sockaddr_in ret;
	ret.sin_family = AF_INET;
	ret.sin_port = ::htons(m_port);
	ret.sin_addr.s_addr = m_netaddr.S_un.S_addr;
	len = sizeof(sockaddr_in);
	memcpy(&addr, &ret, len);
}

void IPAddress::set_port(ushort port)
{
	m_port = port;
}

/*
void IPAddress::dns_lookup() const
{
	char *name = new char[NI_MAXHOST];
	int error = getnameinfo(info[0].ai_addr, info[0].ai_addrlen, name, NI_MAXHOST, NULL, 0, 0);

	if(error)
	{
		delete[] name;
		throw Exception(gai_strerror(error));
	}
	
	string ret(name);
	delete[] name;
	
	return ret;
}
*/

size_t IPAddress::hash() const
{
	return Util::hash_code(m_netaddr.S_un.S_addr);
}

} //namespace GSDK
