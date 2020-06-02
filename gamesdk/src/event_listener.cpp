/*  $Id: event_listener.cpp,v 1.1 2005/11/14 17:07:59 dmitry.vereshchagin Exp $
**
**
*/

#include "event_listener.h"
#include "event_listener_impl.h"

namespace GSDK
{

/////////////////////////////////////////////////////////////////////////////
// EventListener construction:

EventListener::EventListener()
: impl(NULL)
{
	impl = new EventListener_Impl(this);
}

EventListener::EventListener(EventTrigger *trigger)
: impl(NULL)
{
	impl = new EventListener_Impl(this);

	add_trigger(trigger);
}

EventListener::~EventListener()
{
	if (impl) delete impl;
}

/////////////////////////////////////////////////////////////////////////////
// EventListener attributes:

/////////////////////////////////////////////////////////////////////////////
// EventListener operations:

void EventListener::add_trigger(EventTrigger *trigger)
{
	if (impl) impl->add_trigger(trigger);
}

void EventListener::remove_trigger(EventTrigger *trigger)
{
	if (impl) impl->remove_trigger(trigger);
}

void EventListener::clear()
{
	if (impl) impl->clear();
}

void EventListener::reset_all()
{
	if (impl) impl->reset_all();
}

bool EventListener::wait(int timeout)
{
	if (impl) return impl->wait(timeout);
	else return false;
}

/////////////////////////////////////////////////////////////////////////////
// EventListener implementation:

} // namespace