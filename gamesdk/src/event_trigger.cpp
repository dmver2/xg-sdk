/*  $Id: event_trigger.cpp,v 1.1 2005/11/14 17:07:59 dmitry.vereshchagin Exp $
**
**
*/

#include "event_trigger.h"
#include "event_trigger_impl.h"

namespace GSDK
{

/////////////////////////////////////////////////////////////////////////////
// EventTrigger construction:

EventTrigger::EventTrigger()
: sp_impl (new EventTrigger_Impl())
{
	reset();
}

EventTrigger::EventTrigger(const EventTrigger &copy)
: sp_impl(copy.sp_impl)
{
}

EventTrigger::~EventTrigger()
{
}

/////////////////////////////////////////////////////////////////////////////
// EventTrigger attributes:

bool EventTrigger::get_flag() const
{
	return sp_impl->get_flag();
}

/////////////////////////////////////////////////////////////////////////////
// EventTrigger operations:

void EventTrigger::reset() const
{
	sp_impl->reset();
}

void EventTrigger::set_flag()
{
	sp_impl->set_flag();
}

bool EventTrigger::wait(int timeout)
{
	return sp_impl->wait(timeout);
}

/////////////////////////////////////////////////////////////////////////////
// EventTrigger implementation:

} // namespace