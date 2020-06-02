/*  $Id: event_listener.h,v 1.1 2005/11/14 17:07:58 dmitry.vereshchagin Exp $
**
**
*/

#ifndef header_event_listener
#define header_event_listener

#if _MSC_VER > 1000
#pragma once
#endif

#include "decl6n.h"

namespace GSDK
{

class EventTrigger;
class EventListener_Impl;

/// 
/** 
 *	
 */
class CxxLibExport EventListener
{
public:
	EventListener();

	EventListener(EventTrigger *trigger);

	virtual ~EventListener();

public:

public:
	void add_trigger(EventTrigger *trigger);

	void remove_trigger(EventTrigger *trigger);

	void clear();

	void reset_all();

	bool wait(int timeout = -1);

private:
	EventListener_Impl *impl;
};

} // namespace GSDK

#endif
