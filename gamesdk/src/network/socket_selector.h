/*  $Id: socket_selector.h,v 1.1 2005/11/14 17:07:59 dmitry.vereshchagin Exp $
**
**
*/

#ifndef header_socket_selector
#define header_socket_selector

#if _MSC_VER > 1000
#pragma once
#endif

#ifdef WIN32
#include <winsock2.h> // This has to be included before <list> else winsock.h somehow is included
#endif

#include <list>
#include <unordered_set>
#include "synchronization.h"
#include "thread.h"

namespace GSDK
{

class Thread;
class Mutex;
class EventTrigger_Socket;

class SocketSelector : Runnable
{
// 
public:
	//: Creates the socket select listener thread.
	SocketSelector();

	//: Shut down socket select listener thread.
	virtual ~SocketSelector();

// 
public:

// 
public:
	//: Start a read listen for the socket.
	void listen_read(EventTrigger_Socket *socket);

	//: Start a write listen for the socket.
	void listen_write(EventTrigger_Socket *socket);

	//: Start a exception listen for the socket.
	void listen_exception(EventTrigger_Socket *socket);

	//: Remove socket from the read listening.
	void remove_read(EventTrigger_Socket *socket);

	//: Remove socket from the write listening.
	void remove_write(EventTrigger_Socket *socket);

	//: Remove socket from the exception listening.
	void remove_exception(EventTrigger_Socket *socket);

// 
private:
	//: Make the socket select thread break its listen and check for updates.
	//- <p>This is used to make it start listening on new sockets, and stop listening
	//- on old sockets.</p>
	void signal_listen_thread();

	//: Socket select thread function.
	virtual void run();

	Thread m_listen_thread;
	Mutex m_mutex;
	volatile bool m_stop_thread;

	std::unordered_set<EventTrigger_Socket*> m_read_sockets;
	std::unordered_set<EventTrigger_Socket*> m_write_sockets;
	std::unordered_set<EventTrigger_Socket*> m_exception_sockets;

	WSAEVENT m_signal_event;

	std::list<Thread> m_thread_list;
};

} // namespace

#endif
