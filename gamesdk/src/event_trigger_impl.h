/*  $Id: event_trigger_impl.h,v 1.1 2005/11/14 17:07:59 dmitry.vereshchagin Exp $
**
**
*/

#ifndef header_event_trigger_impl
#define header_event_trigger_impl

#if _MSC_VER > 1000
#pragma once
#endif

#include "stdafx.h"

namespace GSDK
{

class EventListener;

class EventTrigger_Impl 
{
// 
public:
	EventTrigger_Impl();

	virtual ~EventTrigger_Impl();

// 
public:
	bool get_flag() const;

	HANDLE get_handle() const { return wait_handle; }

// 
public:
	//: Reset trigger flag.
	virtual void reset() const;

	//: Raise trigger flag.
	virtual void set_flag();

	//: Wait until one of the event triggers. The timeout is in milliseconds.
	//return: true if the event triggered, false if it timed out.
	virtual bool wait(int timeout = -1) const;

	friend class EventListener_Impl;

// 
private:
	//: Win32 event handle. Used to wait with WaitForSingleObject().
	HANDLE wait_handle;
};

} // namespace

#endif
