/*	$Id: Timer_Impl.h,v 1.1 2005/11/14 17:07:59 dmitry.vereshchagin Exp $
**	
**
*/

#pragma once

#include <vector>
#include <memory>
#include "types.h"
#include "thread.h"
#include "sharedptr.h"
#include "decl6n.h"
#include "synchronization.h"
#include "exception.h"
#include "timertask.h"
#include "referenceable.h"
#include "timer.h"


#define synchronized(x) AutoLockSection<CriticalSection> lock(x)

namespace GSDK
{

class TimerTask_Impl /*: public Domain::Referenceable*/
{

public:
    CriticalSection lock;

	int state;

    /**
     * Next execution time for this task in the format returned by
     * System::get_absolute_time, assuming this task is schedule for execution.
     * For repeating tasks, this field is updated prior to each task execution.
     */
    int64 next_execution_time;

	/**
     * Period in milliseconds for repeating tasks.  A positive value indicates
     * fixed-rate execution.  A negative value indicates fixed-delay execution.
     * A value of 0 indicates a non-repeating task.
     */
	int64 period;

    /**
     * Creates a new timer task.
     */

	TimerTask_Impl()
		: next_execution_time(0)
		, period(0)
		, state(TimerTask::NEW)
	{
    }

	~TimerTask_Impl()
	{
    }

	// Function name   : TimerTask
	// Description     : 
	// Return type     : 
	// Argument        : Runnable runnable
	// Argument        : int64 next
	// Argument        : int64 period
	
/*
	TimerTask_Impl(Runnable &runnable, int64 next, int64 period)
		: next_execution_time(next)
		, period(period)
		, runnable(runnable)
		, state(NEW)f
	{
    }
*/

    /**
     * Cancels this timer task.  If the task has been scheduled for one-time
     * execution and has not yet run, or has not yet been scheduled, it will
     * never run.  If the task has been scheduled for repeated execution, it
     * will never run again.  (If the task is running when this call occurs,
     * the task will run to completion, but will never run again.)
     *
     * <p>Note that calling this method from within the <tt>run</tt> method of
     * a repeating timer task absolutely guarantees that the timer task will
     * not run again.
     *
     * <p>This method may be called repeatedly; the second and subsequent 
     * calls have no effect.
     *
     * @return true if this task is scheduled for one-time execution and has
     *         not yet run, or this task is scheduled for repeated execution.
     *         Returns false if the task was scheduled for one-time execution
     *         and has already run, or if the task was never scheduled, or if
     *         the task was already canceled.  (Loosely speaking, this method
     *         returns <tt>true</tt> if it prevents one or more scheduled
     *         executions from taking place.)
     */
    bool cancel() {
        AutoLockSection<CriticalSection> cs(lock);
        bool result = (state == TimerTask::SCHEDULED);
		state = TimerTask::CANCELLED;
        return result;
    }

    /**
     * Returns the <i>scheduled</i> execution time of the most recent
     * <i>actual</i> execution of this task.  (If this method is invoked
     * while task execution is in progress, the return value is the scheduled
     * execution time of the ongoing task execution.)
     *
     * <p>This method is typically invoked from within a task's run method, to
     * determine whether the current execution of the task is sufficiently
     * timely to warrant performing the scheduled activity:
     * <pre>
     *   public void run() {
     *       if (System::get_absolute_time() - scheduledExecutionTime() >=
     *           MAX_TARDINESS)
     *               return;  // Too late; skip this execution.
     *       // Perform the task
     *   }
     * </pre>
     * This method is typically <i>not</i> used in conjunction with
     * <i>fixed-delay execution</i> repeating tasks, as their scheduled
     * execution times are allowed to drift over time, and so are not terribly
     * significant.
     *
     * @return the time at which the most recent execution of this task was
     *         scheduled to occur, in the format returned by Date.getTime().
     *         The return value is undefined if the task has yet to commence
     *         its first execution.
     * @see Date#getTime()
     */
    int64 get_execution_time() 
	{
		AutoLockSection<CriticalSection> cs(lock);
        return (period < 0 ? next_execution_time + period
                            : next_execution_time - period);
    }

};

class Timer_Impl : public Thread, public Domain::Referenceable
{
public:
//	Timer_Impl(void);
	Timer_Impl(Timer::pfn_get_time);
	~Timer_Impl(void);

public:
    int main(void);
	void schedule_task(TimerTask::sptr task, int64 time, int64 period);
	void cancel();

private:
	void main_loop();

public:
//	std::auto_ptr<TaskQueue> m_queue;
	class TaskQueue *m_queue;
//	Event m_stop_event;
//	Mutex m_lock;
    bool m_ready2schedule;
	Timer::pfn_get_time pfn_time_proc;

};


/**
 * This class represents a timer task queue: a priority queue of TimerTasks,
 * ordered on next_execution_time.  Each Timer object has one of these, which it
 * shares with its TimerThread.  Internally this class uses a heap, which
 * offers log(n) performance for the add, remove_min and reschedule
 * operations, and constant time performance for the the get_min operation.
 */
class TaskQueue 
{
private:
    /**
     * Priority queue represented as a balanced binary heap: the two children
     * of queue[n] are queue[2*n] and queue[2*n+1].  The priority queue is
     * ordered on the next_execution_time field: The TimerTask with the lowest
     * next_execution_time is in queue[1] (assuming the queue is nonempty).  For
     * each node n in the heap, and each descendant of n, d,
     * n.next_execution_time <= d.next_execution_time. 
     */
//	TimerTask::sptr[] m_heap = new TimerTask::sptr[128];
	std::vector<TimerTask::sptr> m_heap;

    /**
     * The number of tasks in the priority queue.  (The tasks are stored in
     * queue[1] up to queue[size]).
     */
    int size;

	/**
	 *	Queue synchronization object
	 *	wait and signal
	 */
public:
	CriticalSection lock;

	Event event;

public:

	TaskQueue()
		: m_heap(16)
		, size (0)
		, event(false, false)
	{
	}

    /**
     * Adds a new task to the priority queue.
     */
	void add(TimerTask::sptr sp_task) 
	{
        // Grow backing store if necessary
		if (++size == m_heap.size()) 
		{
			m_heap.resize(2*m_heap.size());
            //TimerTask[] newQueue = new TimerTask[2*m_heap.length];
            //System.arraycopy(m_heap, 0, newQueue, 0, size);
            //m_heap = newQueue;
        }

        m_heap[size] = sp_task;
//		ptask->add_ref();
        fix_up(size);
    }

    /**
     * Return the "head task" of the priority queue.  (The head task is an
     * task with the lowest next_execution_time.)
     */
	TimerTask::sptr get_min() 
	{
        return m_heap[1];
    }

    /**
     * Remove the head task from the priority queue.
     */
    void remove_min() 
	{
        m_heap[1] = m_heap[size];
//		m_heap[size]->release_ref();  // Drop extra reference to prevent memory leak
		m_heap[size--] = TimerTask::sptr(); // NULL
        fix_down(1);
    }

    /**
     * Sets the next_execution_time associated with the head task to the 
     * specified value, and adjusts priority queue accordingly.
     */
    void reschedule(int64 new_time) 
	{
        m_heap[1]->m_impl->next_execution_time = new_time;
        fix_down(1);
    }

    /**
     * Returns true if the priority queue contains no elements.
     */
    bool is_empty() 
	{
        return size==0;
    }

    /**
     * Removes all elements from the priority queue.
     */
    void clear() 
	{
        // Null out task references to prevent memory leak
        for (int i=1; i<=size; i++)
		{
//			m_heap[i]->release_ref();
			m_heap[i] = TimerTask::sptr(); // NULL
		}
        size = 0;
    }

    /**
     * Establishes the heap invariant (described above) assuming the heap
     * satisfies the invariant except possibly for the leaf-node indexed by k
     * (which may have a next_execution_time less than its parent's).
     *
     * This method functions by "promoting" queue[k] up the hierarchy
     * (by swapping it with its parent) repeatedly until queue[k]'s
     * next_execution_time is greater than or equal to that of its parent.
     */
    void fix_up(int k) 
	{
        while (k > 1) 
		{
            int j = k >> 1;
			if (m_heap[j]->m_impl->next_execution_time <= m_heap[k]->m_impl->next_execution_time)
                break;
			TimerTask::sptr tmp = m_heap[j];  
			m_heap[j] = m_heap[k]; 
			m_heap[k] = tmp;
            k = j;
        }
    }

    /**
     * Establishes the heap invariant (described above) in the subtree
     * rooted at k, which is assumed to satisfy the heap invariant except
     * possibly for node k itself (which may have a next_execution_time greater
     * than its children's).
     *
     * This method functions by "demoting" queue[k] down the hierarchy
     * (by swapping it with its smaller child) repeatedly until queue[k]'s
     * next_execution_time is less than or equal to those of its children.
     */
    void fix_down(int k) 
	{
        int j;
        while ((j = k << 1) <= size) 
		{
            if (j < size &&
				m_heap[j]->m_impl->next_execution_time > m_heap[j+1]->m_impl->next_execution_time)
                j++; // j indexes smallest kid
            if (m_heap[k]->m_impl->next_execution_time <= m_heap[j]->m_impl->next_execution_time)
                break;
			TimerTask::sptr tmp = m_heap[j];  
			m_heap[j] = m_heap[k]; 
			m_heap[k] = tmp;
            k = j;
        }
    }

	ulong wait(time_t timeout = INFINITE)
	{
		return event.wait(timeout);
	}

	bool signal()
	{
		return event.signal();
	}

    /**
     *	Adjust
     */
    void adjust(int64 delta) 
	{
        for (int i=1; i<=size; i++)
		{
			m_heap[i]->m_impl->next_execution_time += delta;
		}
    }

};

} // namespace
