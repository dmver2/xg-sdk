/*  $Id: event_trigger_impl.cpp,v 1.1 2005/11/14 17:07:59 dmitry.vereshchagin Exp $
**
**
*/

#include "event_trigger_impl.h"

namespace GSDK
{

EventTrigger_Impl::EventTrigger_Impl()
:
	wait_handle(NULL)
{
	wait_handle = CreateEvent(NULL, TRUE, FALSE, NULL);
}

EventTrigger_Impl::~EventTrigger_Impl()
{
	CloseHandle(wait_handle);
}

/////////////////////////////////////////////////////////////////////////////
// EventTrigger_Impl attributes:

bool EventTrigger_Impl::get_flag() const
{
	return WaitForSingleObject(wait_handle, 0) != WAIT_TIMEOUT;
}

/////////////////////////////////////////////////////////////////////////////
// EventTrigger_Impl operations:

void EventTrigger_Impl::reset() const
{
	ResetEvent(wait_handle);
}

void EventTrigger_Impl::set_flag()
{
	SetEvent(wait_handle);
}

bool EventTrigger_Impl::wait(int timeout) const
{
	DWORD result = WaitForSingleObject(
		wait_handle,
		(timeout == -1) ? INFINITE : timeout);

	return (result != WAIT_TIMEOUT);
}

} // namespace