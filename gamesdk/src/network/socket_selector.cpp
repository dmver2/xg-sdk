/*  $Id: socket_selector.cpp,v 1.1 2005/11/14 17:07:59 dmitry.vereshchagin Exp $
**
**
*/

#ifdef _MSC_VER
#pragma warning (disable:4786 4996)
#endif

#include "socket_selector.h"
#include "event_trigger_socket.h"
#include "socket_impl.h"

#include <assert.h>
#include <vector>
#include <algorithm>
#include <map>

namespace GSDK
{

/////////////////////////////////////////////////////////////////////////////
// SocketSelector construction:

SocketSelector::SocketSelector()
: m_listen_thread(this, false)
, m_mutex()
, m_stop_thread(false)
{
	m_signal_event = ::WSACreateEvent();

	m_listen_thread.start();
}

SocketSelector::~SocketSelector()
{
	m_stop_thread = true;
	signal_listen_thread();
	m_listen_thread.wait();


	m_read_sockets.clear();
	m_write_sockets.clear();
	m_exception_sockets.clear();
	::WSACloseEvent(m_signal_event);
}

/////////////////////////////////////////////////////////////////////////////
// SocketSelector attributes:

/////////////////////////////////////////////////////////////////////////////
// SocketSelector operations:

void SocketSelector::listen_read(EventTrigger_Socket *socket)
{
	{
		AutoLockSection<Mutex> mutex_section(m_mutex);
		if (m_read_sockets.find(socket) == m_read_sockets.end())
			m_read_sockets.insert(socket);
	}
	
	signal_listen_thread();
}

void SocketSelector::listen_write(EventTrigger_Socket *socket)
{
	{
		AutoLockSection<Mutex> mutex_section(m_mutex);
		if (m_write_sockets.find(socket) == m_write_sockets.end())
			m_write_sockets.insert(socket);
	}
	
	signal_listen_thread();
}

void SocketSelector::listen_exception(EventTrigger_Socket *socket)
{
	{
		AutoLockSection<Mutex> mutex_section(m_mutex);
		if (m_exception_sockets.find(socket) == m_exception_sockets.end())
			m_exception_sockets.insert(socket);
	}
	
	signal_listen_thread();
}

void SocketSelector::remove_read(EventTrigger_Socket *socket)
{
	{
		AutoLockSection<Mutex> mutex_section(m_mutex);
		m_read_sockets.erase(socket);
	}
	
	signal_listen_thread();
}

void SocketSelector::remove_write(EventTrigger_Socket *socket)
{
	{
		AutoLockSection<Mutex> mutex_section(m_mutex);
		m_write_sockets.erase(socket);
	}
	
	signal_listen_thread();
}

void SocketSelector::remove_exception(EventTrigger_Socket *socket)
{
	{
		AutoLockSection<Mutex> mutex_section(m_mutex);
		m_exception_sockets.erase(socket);
	}

	signal_listen_thread();
}

/////////////////////////////////////////////////////////////////////////////
// SocketSelector implementation:

void SocketSelector::run()
{
	while (!m_stop_thread)
	{
		std::vector<WSAEVENT> events;
		std::vector<int> sockets;
		std::list<EventTrigger_Socket*>::const_iterator it;

		{
			AutoLockSection<Mutex> mutex_section(m_mutex);

			::WSAResetEvent(m_signal_event);
			events.push_back(m_signal_event);

			std::map<int, unsigned int> flags;

			for (it = m_read_sockets.begin(); it != m_read_sockets.end(); ++it)
			{
				int sock = (*it)->get_socket()->sock;
				std::map<int, unsigned int>::iterator it_flag = flags.find(sock);
				if (it_flag == flags.end()) 
					flags.insert(it_flag
						, std::map<int, unsigned int>::value_type(sock, FD_READ|FD_ACCEPT|FD_CLOSE));
//					flags[sock] = FD_READ|FD_ACCEPT|FD_CLOSE;
				else 
					it_flag->second |= FD_READ|FD_ACCEPT|FD_CLOSE;
//					flags[sock] |= FD_READ|FD_ACCEPT|FD_CLOSE;
			}
			for (it = m_write_sockets.begin(); it != m_write_sockets.end(); ++it)
			{
				int sock = (*it)->get_socket()->sock;
				std::map<int, unsigned int>::iterator it_flag = flags.find(sock);
				if (it_flag == flags.end()) 
					flags.insert(it_flag
						, std::map<int, unsigned int>::value_type(sock, FD_WRITE|FD_CONNECT));
				else 
					it_flag->second |= FD_WRITE|FD_CONNECT;
//				if (flags.find(sock) == flags.end()) flags[sock] = FD_WRITE|FD_CONNECT;
//				else flags[sock] |= FD_WRITE|FD_CONNECT;
			}
			for (it = m_exception_sockets.begin(); it != m_exception_sockets.end(); ++it)
			{
				int sock = (*it)->get_socket()->sock;
				std::map<int, unsigned int>::iterator it_flag = flags.find(sock);
				if (it_flag == flags.end()) 
					flags.insert(it_flag
						, std::map<int, unsigned int>::value_type(sock, FD_OOB));
				else 
					it_flag->second |= FD_OOB;
//				if (flags.find(sock) == flags.end()) flags[sock] = FD_OOB;
//				else flags[sock] |= FD_OOB;
			}

			std::map<int, unsigned int>::iterator it_flags;
			for (it_flags = flags.begin(); it_flags != flags.end(); ++it_flags)
			{
				WSAEVENT event = ::WSACreateEvent();
				::WSAEventSelect(it_flags->first, event, it_flags->second);
				events.push_back(event);
				sockets.push_back(it_flags->first);
			}
		}

		int num_events = events.size();

		if(num_events > WSA_MAXIMUM_WAIT_EVENTS)
		{
		}

		DWORD result = ::WSAWaitForMultipleEvents(
			num_events, &events[0], FALSE, WSA_INFINITE, FALSE);

		assert(
			result != WSA_WAIT_FAILED &&
			result != WAIT_IO_COMPLETION &&
			result != WSA_WAIT_TIMEOUT &&
			result >= WSA_WAIT_EVENT_0);

		for (int i=1; i<num_events; i++)
		{
			::WSAEventSelect(sockets[i-1], events[i], 0);
			::WSACloseEvent(events[i]);
		}

		if (result == WSA_WAIT_EVENT_0) continue; // signal event

		int sock = sockets[result - WSA_WAIT_EVENT_0-1];

		// ok, we found a trigger. Now thanks to winsock lameness, we could only listen for ALL events for a socket,
		// so now we do a old-school select() on this socket. As usual Microsoft managed to fuck up.

		{
			fd_set rfds, wfds, efds;
			FD_ZERO(&rfds);
			FD_ZERO(&wfds);
			FD_ZERO(&efds);
			FD_SET(sock, &rfds);
			FD_SET(sock, &wfds);
			FD_SET(sock, &efds);

			timeval tv;
			tv.tv_sec = 0;
			tv.tv_usec = 0;
			
			int result = select(sock+1, &rfds, &wfds, &efds, &tv);

			// verify trigger is still valid. if it is, raise its flag.
			AutoLockSection<Mutex> mutex_section(m_mutex);
			if (FD_ISSET(sock, &rfds))
			{
				for (it = m_read_sockets.begin(); it != m_read_sockets.end(); ++it)
				{
					EventTrigger_Socket *trigger = *it;
					if (trigger->get_socket()->sock == sock)
					{
						trigger->set_flag();
						m_read_sockets.erase(it);
						break;
					}
				}
			}
			if (FD_ISSET(sock, &wfds))
			{
				for (it = m_write_sockets.begin(); it != m_write_sockets.end(); ++it)
				{
					EventTrigger_Socket *trigger = *it;
					if (trigger->get_socket()->sock == sock)
					{
						trigger->set_flag();
						m_write_sockets.erase(it);
						break;
					}
				}
			}
			if (FD_ISSET(sock, &efds))
			{
				for (it = m_exception_sockets.begin(); it != m_exception_sockets.end(); ++it)
				{
					EventTrigger_Socket *trigger = *it;
					if (trigger->get_socket()->sock == sock)
					{
						trigger->set_flag();
						m_exception_sockets.erase(it);
						break;
					}
				}
			}
		}
	} // while
}

void SocketSelector::signal_listen_thread()
{
	WSASetEvent(m_signal_event);
}

} // namespace
