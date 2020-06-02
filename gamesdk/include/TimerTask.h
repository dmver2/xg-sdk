/*	$Id: TimerTask.h,v 1.1 2005/11/14 17:07:58 dmitry.vereshchagin Exp $
**	Game SDK
*/

#pragma once

#include "types.h"
#include "thread.h"
#include "sharedptr.h"
#include "decl6n.h"

namespace GSDK
{

///	
/** 
 *	
 *	
 */

class CxxLibExport TimerTask : public Runnable 
{

public:

	/// 
	enum STATE {
				NEW = 0			///< 
			, 	SCHEDULED		///< 
			,	EXECUTED		///< 
			,	CANCELLED		///< 
	};

	/// smart pointer
	typedef sharedptr<TimerTask> sptr;

protected:
	/// 
	/** 
	 */
	TimerTask();

	/// 
	/** 
	 *	@param rcopy 
	 */
	TimerTask(const TimerTask& rcopy);

	/// 
	/** 
	 *	@param rcopy 
	 *	@return 
	 */
	TimerTask& operator= (const TimerTask& rcopy);

public:
	/// 
	/** 
	 *
	 */
	virtual ~TimerTask(void);

	/// 
	/**	
	 *	
	 *	
	 *	
	 *	
	 *	@see Runnable::run()
	 *	@see Timer
	 */
	virtual void run(void) = 0;

	/// 
	/** 
	 *	
	 *	
	 *	
	 *	
	 *
	 *	<p>
	 *	
	 *
	 *	<p>
	 *
	 *	<p>
	 *	
	 *
	 *	@return 
	 *	
	 *	
	 */
    bool cancel();

	/// 
	/** 
	 *	<p>
	 *	
     * <code>
     *   void SomeTimerTask::run() 
	 *	 {
     *       if (System::get_absolute_time() - get_execution_time() >= MAX_TIMEOUT)
     *               return;  // 
     *       // 
     *   }
     * </code>
	 *
	 *	@return 
	 *	
	 */
    int64 get_execution_time() const;

	/// 
	/**
	 * @return 
	 */
	int get_state() const;

	/// 
	void renew();

private:
	/// 
	class TimerTask_Impl *m_impl;

friend class Timer_Impl;
friend class TaskQueue;
};

} // namespace
