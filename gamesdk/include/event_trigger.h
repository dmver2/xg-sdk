/*  $Id: event_trigger.h,v 1.1 2005/11/14 17:07:58 dmitry.vereshchagin Exp $
** Game SDK
**
*/

#ifndef header_event_trigger
#define header_event_trigger

#if _MSC_VER > 1000
#pragma once
#endif

#include "sharedptr.h"
#include "decl6n.h"

#pragma warning (disable : 4251)

namespace GSDK
{

class CxxLibExport EventListener;
class CxxLibExport EventTrigger_Impl;

/// 
/**	
 *	
 */
class CxxLibExport EventTrigger
{
public:
// 
	enum 
	{
		infinite = -1
	};

	/// 
	/// 
	EventTrigger();

	/// 
	/// 
	/// @param copy 
	EventTrigger(const EventTrigger &copy);

	/// 
	/// 
	virtual ~EventTrigger();

public:
	/// 
	/// @return 
	bool get_flag() const;

public:
	/// 
	void reset() const;

	/// 
	void set_flag();

	/// 
	/// @param timeout 
	/// @return 
	bool wait(int timeout = infinite);

// 
private:
	/// 
	sharedptr<EventTrigger_Impl> sp_impl;

	friend class EventListener_Impl;
};

} // namespace

#endif
