/*  $Id: socket_impl.h,v 1.1 2005/11/14 17:07:59 dmitry.vereshchagin Exp $
**	Game SDK
**
**
*/

#ifndef header_socket_impl
#define header_socket_impl

#include "socket_selector.h"
#include "network/socket.h"
#include "event_trigger_socket.h"
#include "types.h"
#include <stack>

#include <windows.h>
typedef int socklen_t;


namespace GSDK
{

class EventTrigger_Socket;
class Socket_Impl;
class SocketSelector;

/*
class Socket_Helper : public KeepAlive
{
public:
	Socket_Helper(Socket_Impl *impl) : impl(impl) { return; }

	virtual void keep_alive();

	Socket_Impl *impl;

	Signal_v0 sig_read_triggered;

	Signal_v0 sig_write_triggered;

	Signal_v0 sig_exception_triggered;
	
	Signal_v0 sig_disconnected;
};
*/

class Socket_Impl
{
public:
	static SocketSelector *sp_socket_selector;
//! Static operations:
public:
	static IPAddress create_ip_address(const sockaddr_in &addr_in);

// 
public:
	Socket_Impl();

	virtual ~Socket_Impl();

// 
public:
	int sock;

	int domain; // PF_INET, PF_INET6

	int protocol; // SOCK_STREAM, SOCK_DGRAM

	std::stack<string> push_stack;

	EventTrigger_Socket *read, *write, *exception;

//	Socket_Helper *keep_alive_helper;

// 
public:
	void add_ref();

	void release_ref();

	static const TCHAR* get_error_message();

	static const TCHAR* get_error_message(unsigned long code);

//! Overridables:
public:
	virtual EventTrigger_Socket *create_read_trigger();

	virtual EventTrigger_Socket *create_write_trigger();

	virtual EventTrigger_Socket *create_exception_trigger();

// 
private:
	int ref_count;
	
	//: True if the socket is connected, false if it is no longer in use
	bool valid;

public:
	//: Internal function to shutdown a socket that has disconnected
	void disconnect();

};

} // namespace

#endif
