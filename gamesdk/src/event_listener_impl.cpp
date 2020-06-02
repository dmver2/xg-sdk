/*  $Id: event_listener_impl.cpp,v 1.1 2005/11/14 17:07:59 dmitry.vereshchagin Exp $
**
**
*/

#include <malloc.h>
#include <list>
#include "exception.h"
#include "event_listener_impl.h"
#include "event_trigger_impl.h"

namespace GSDK
{

/////////////////////////////////////////////////////////////////////////////
// EventListener_Impl construction:

EventListener_Impl::EventListener_Impl(EventListener *owner)
: owner(owner)
{
}

EventListener_Impl::~EventListener_Impl()
{
	clear();
}

/////////////////////////////////////////////////////////////////////////////
// EventListener_Impl attributes:

/////////////////////////////////////////////////////////////////////////////
// EventListener_Impl operations:

void EventListener_Impl::add_trigger(const EventTrigger *trigger)
{
	if(triggers.size() > MAXIMUM_WAIT_OBJECTS)
		throw Exception("Maximum event count exceeded");
	triggers.push_back(trigger->sp_impl);
}

void EventListener_Impl::remove_trigger(const EventTrigger *trigger)
{
	triggers.remove(trigger->sp_impl);
}

void EventListener_Impl::clear()
{
	triggers.clear();
}

void EventListener_Impl::reset_all()
{
	std::list< sharedptr<EventTrigger_Impl> >::iterator it;
	for (it = triggers.begin(); it != triggers.end(); ++it) (*it)->reset();
}


/////////////////////////////////////////////////////////////////////////////
// EventListener_Impl implementation:

bool EventListener_Impl::wait(int timeout)
{
	if (triggers.empty()) // no triggers, just do the sleep.
	{
		::Sleep(timeout);
		return false;
	}

	int num_handles = triggers.size();
//	HANDLE *handles = new HANDLE[num_handles];
//	HANDLE *handles = (HANDLE*)_alloca(num_handles);
	HANDLE handles[MAXIMUM_WAIT_OBJECTS] = {0};

	int i = 0;
	std::list< sharedptr<EventTrigger_Impl> >::iterator it;
	for (it = triggers.begin(); it != triggers.end(); ++it)
	{
		EventTrigger_Impl *trigger = const_cast<EventTrigger_Impl*>(it->get());
		handles[i++] = trigger->wait_handle;
	}

	DWORD result = WaitForMultipleObjects(
		num_handles,
		handles,
		FALSE,
		timeout == -1 ? INFINITE : timeout);

//	delete[] handles;

	if (result == WAIT_TIMEOUT) return false;
	return true;
}

/////////////////////////////////////////////////////////////////////////////
// EventListener_Impl implementation:
} // namespace GSDK
