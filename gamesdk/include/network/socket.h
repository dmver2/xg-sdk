/*  $Id: socket.h,v 1.1 2005/11/14 17:07:58 dmitry.vereshchagin Exp $
**	Game SDK
**
*/

#ifndef header_socket
#define header_socket

#if _MSC_VER > 1000
#pragma once
#endif

#include <winsock2.h>
#include "signals.h"
#include "ipaddress.h"
#include "inputstream_socket.h"
#include "outputstream_socket.h"
#include "decl6n.h"

namespace GSDK
{

class EventTrigger;
class Socket_Impl;

/// 
/** 
 *	
 *	
 *	
 *	
 *	
 *	
*/
class CxxLibExport Socket
{
	//	
	friend class InputStream_Socket;
	friend class OutputStream_Socket;

	// 
public:
	//! 
	enum Type
	{
			tcp		///< TCP
		,	udp		///< UDP
		,	raw		///< ICMP
	};

	//! 
	enum ShutdownHow
	{
		shutdown_receive,	///< 
		shutdown_send		///< 
	};

	//! 
	enum Protocol
	{
		ipv4,	///< IPv4
		ipv6	///< IPv6
	};

//: 
public:

	//! 
	/**	
	 *	
	 */
	Socket();

	//! 
	/*! 
	    @param socket 
	*/
	Socket(int socket);

	//! 
	//! @param type 
	//! @param proto 
	Socket(Type type, Protocol proto = ipv4);

	//! 
	/// 
	//! @param copy 
	Socket(const Socket &copy);

	//! 
	//! 
	virtual ~Socket();

//: 
public:
	//! 
	InputStream_Socket input;

	//! 
	OutputStream_Socket output;

	//! 
	int get_socket() const;

	//! 
	/// 
	/// 
	int get_max_msg_size() const;

	//! 
	//! @see EventTrigger
	//! 
	/// @return EventTrigger* 
	EventTrigger *get_read_trigger() const;

	//! 
	//! @see EventTrigger
	//! 
	/// @return EventTrigger* 
	EventTrigger *get_write_trigger() const;

	//! 
	//! @see EventTrigger
	/// @return EventTrigger* 
	EventTrigger *get_exception_trigger() const;

	//! 
	/// @return 
	IPAddress get_source_address() const;

	//! 
	/// @return 
	IPAddress get_dest_address() const;

//!
//	std::list<IPAddress> get_broadcast_addresses(const string&) const;

//: 
public:
	//! 
	//!	
	/// @param other 
	Socket &operator =(const Socket &other);

	//! 
	//! @param nonblocking 
	void set_nonblocking(bool nonblocking = true);

	//! 
	void set_nodelay(bool nodelay = true);

	//! 
	void set_broadcasting(bool broadcasting = true);

	//! 
	//! @param data 
	//! @param size 
	/// @return 
	int send(const void *data, int size);

	//! 
	//! @param data 
	//! @param size 
	//! @param dest IP 
	//! @return 
	int send(const void *data, int size, const IPAddress &dest);

	//! 
	//! @param msg 
	//! @return 
	int send(const string &msg);

	//! 
	//! @param data 
	//! @param size 
	//! @return 
	int recv(void *data, int size);

	//! 
	//! 
	//! @param data 
	//! @param size 
	//! @param from 
	//! @return 
	int recv(void *data, int size, /*out*/ IPAddress &from);

	//! 
	//! @param address 
	void connect(const IPAddress &address) const;

	//! 
	//! @param shutdown_type 
	void shutdown(ShutdownHow shutdown_type);

	//! 
	//! 
	//! @param address 
	void bind(const IPAddress &address);

	//! 
	//! @param backlog 
	void listen(int backlog = SOMAXCONN);

	//! 
	//! @return 
	Socket accept();

	//!	
	/// 
	void close();

	//! 
	/// @return 
	static string get_error_message();

	//! 
	///	@param code 
	/// @return 
	static string get_error_message(unsigned long code);

//: 
public:
	//! 
	//! 
	//! @param impl 
	Socket(class Socket_Impl *impl);

//: 
private:
	//! 
	Socket_Impl *m_impl;

	//! 
	Type m_proto;

	static TCHAR LOGTOPIC[];
};

} // namespace

#endif
