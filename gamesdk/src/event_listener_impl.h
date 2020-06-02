/*  $Id: event_listener_impl.h,v 1.1 2005/11/14 17:07:59 dmitry.vereshchagin Exp $
**
**
*/

#ifndef header_event_listener_impl
#define header_event_listener_impl

#if _MSC_VER > 1000
#pragma once
#endif

#include <list>
#include "event_listener.h"
#include "event_trigger.h"
#include "sharedptr.h"

namespace GSDK
{

class EventTrigger_Impl;

class EventListener_Impl
{
// 
public:
	EventListener_Impl(EventListener *owner);

	virtual ~EventListener_Impl();

// 
public:

// 
public:
	//: Adds an event trigger to the listener.
	void add_trigger(const EventTrigger *trigger);

	//: Removes an event trigger from the listener.
	void remove_trigger(const EventTrigger *trigger);

	//: Removes all event triggers from the listener.
	void clear();

	//: Resets all event triggers added to the listener.
	void reset_all();

	//: Wait until one of the events triggers. The timeout is in milliseconds.
	//return: true if a event triggered, false if it timed out.
	virtual bool wait(int timeout);

	std::list< sharedptr<EventTrigger_Impl> > triggers;

// 
private:
	EventListener *owner;
};

} // namespace

#endif
