/*	$Id: Timer.h,v 1.1 2005/11/14 17:07:58 dmitry.vereshchagin Exp $
**	Game SDK
*/

#pragma once

#include "types.h"
#include "sharedptr.h"
#include "decl6n.h"
#include "timertask.h"
#include "exception.h"
#include "system.h"

#pragma warning (disable : 4290)

namespace GSDK
{

/// 
/** 
 *	
 *	
 *	
 *	<p>
 *	
 *	
 *	
 *	<p>
 *	
 *	<p>
 *	
 *	<p>
 *	
 *	<p>
 *	
 *	
 *	<p>
 *	
 *	
 */
class CxxLibExport Timer
{
public:
	typedef int64 ( *pfn_get_time) (void);

/// 
/**
 *	
 *	
 *	
 *	schedule*.
 *	@param pfn 
 *	
 */
	Timer(pfn_get_time pfn = System::get_absolute_time);

/// 
/**
 *	
 *	
 *	
 */
	virtual ~Timer(void);

	/// 
	/**	
	 *	
	 *	
	 *	@param timer 
	 */
	Timer(const Timer& timer);

	/// 
	/**	
	 *	@param timer 
	 *	@return 
	 */
	Timer& operator= (const Timer& timer);

	/// 
	/** 
	 *	@param timer 
	 *  @return 
	 */
	bool operator== (const Timer& timer) const;

/** 
 *	
 *	
 *	
 *	
 *	@param task 
 *	@param ms_delay 
 *	@param ms_period 
 */
	void schedule(TimerTask::sptr task, uint64 ms_delay = 0, uint64 ms_period = 0) throw(Exception);

/**
 *	
 *	
 *	
 *	@param task 
 *	@param ms_first_time 
 *	@param ms_period 
 */
	void schedule_at(TimerTask::sptr task, uint64 ms_first_time, uint64 ms_period = 0) throw(Exception);

/**
 *	
 *	
 *	
 *	@param task 
 *	@param ms_delay 
 *	@param ms_period 
 */
	void schedule_fixed_rate(TimerTask::sptr task, uint64 ms_delay = 0, uint64 ms_period = 0) throw(Exception);

/**
 *	
 *	
 *	
 *	@param task 
 *	@param ms_first_time 
 *	@param ms_period 
 */
	void schedule_fixed_rate_at(TimerTask::sptr task, uint64 ms_first_time, uint64 ms_period = 0) throw(Exception);

/// 
/**
 *	
 *	
  *	<p>
*/
	void cancel() throw();

/// 
/** 
 *	@param delta 
 */
	void adjust(int64 delta);

private:

/// 
	class Timer_Impl *m_impl;
};

} // namespace