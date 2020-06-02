/*  $Id: event_trigger_socket.cpp,v 1.1 2005/11/14 17:07:59 dmitry.vereshchagin Exp $
**
**
**
*/

#ifdef _MSC_VER
#pragma warning (disable:4786)
#endif

#include <assert.h>
#include "socket_selector.h"
#include "event_trigger_socket.h"
#include "socket_impl.h"
#include "../event_trigger_impl.h"

namespace GSDK
{

/////////////////////////////////////////////////////////////////////////////
// Construction:

EventTrigger_Socket::EventTrigger_Socket(Socket_Impl *impl, Type type)
: m_psock_impl(impl), type(type)
{
	assert(Socket_Impl::sp_socket_selector != 0);

	start_listen();
}

EventTrigger_Socket::~EventTrigger_Socket()
{
	switch (type)
	{
	case read:      Socket_Impl::sp_socket_selector->remove_read(this); break;
	case write:     Socket_Impl::sp_socket_selector->remove_write(this); break;
	case exception: Socket_Impl::sp_socket_selector->remove_exception(this); break;
	}
}

/////////////////////////////////////////////////////////////////////////////
// Attributes:

/////////////////////////////////////////////////////////////////////////////
// Operations:

/////////////////////////////////////////////////////////////////////////////
// Implementation:

void EventTrigger_Socket::start_listen()
{
	reset();
	switch (type)
	{
	case read:      Socket_Impl::sp_socket_selector->listen_read(this); break;
	case write:     Socket_Impl::sp_socket_selector->listen_write(this); break;
	case exception: Socket_Impl::sp_socket_selector->listen_exception(this); break;
	}
}

} // namespace