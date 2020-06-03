/* $Id: Timer_Impl.cpp,v 1.1 2005/11/14 17:07:59 dmitry.vereshchagin Exp $
**
**
*/

#include ".\timer_impl.h"
#include "system.h"

#if defined (_DEBUG) && defined (_CONSOLE)
#define TIMER_PRINT_EXCEPTION
#include <iostream>
#endif

namespace GSDK
{

/*
Timer_Impl::Timer_Impl(void)
: m_queue(new TaskQueue())
, m_ready2schedule(true)
, pfn_time_proc(System::get_absolute_time)
{
	start();
}
*/

Timer_Impl::Timer_Impl(Timer::pfn_get_time pfn)
: m_queue(new TaskQueue())
, m_ready2schedule(true)
, pfn_time_proc(pfn)
{
	start();
}

Timer_Impl::~Timer_Impl(void)
{
	cancel();
	wait();
	delete m_queue;
}


void Timer_Impl::schedule_task(TimerTask::sptr task, int64 time, int64 period) 
{
	if (time < 0)
		throw Exception("Illegal execution time.");

	synchronized(m_queue->lock);
	if (!m_ready2schedule)
		throw Exception("Timer already canceled.");

	TimerTask_Impl *ptask = task->m_impl;
	{
		synchronized(ptask->lock);
		if (ptask->state != TimerTask::NEW)
			throw Exception("Task already scheduled or canceled");
		ptask->next_execution_time = time;
		ptask->period = period;
		ptask->state = TimerTask::SCHEDULED;
	}

	m_queue->add(task);
	if (m_queue->get_min() == task)
		m_queue->signal();
}

void Timer_Impl::cancel() 
{
	synchronized(m_queue->lock);
    m_ready2schedule = false;
    m_queue->clear();
}

int Timer_Impl::main()
{
    try 
	{
        main_loop();
    } catch(...) 
	{
        // Someone killed this Thread, behave as if Timer canceled
//		synchronized(m_queue->lock);
//        m_ready2schedule = false;
//        m_queue->clear();  // Eliminate obsolete references
		return -1;
    }
	return 0;
}

void Timer_Impl::main_loop() 
{
	while (true) 
	{
		try 
		{
			TimerTask_Impl *p_task_impl;
			TimerTask::sptr task;
			bool taskFired;
			{
				synchronized(m_queue->lock);
				// Wait for queue to become non-empty
				while (m_queue->is_empty() && m_ready2schedule)
				{	
					m_queue->lock.signal();
					m_queue->wait();
					m_queue->lock.wait();
				}
				if (m_queue->is_empty())
					break; // Queue is empty and will forever remain; die

				// Queue nonempty; look at first evt and do the right thing
				int64 currentTime, executionTime;
				task = m_queue->get_min();
				p_task_impl = task->m_impl;
				{
					synchronized(p_task_impl->lock);
					if (p_task_impl->state == TimerTask::CANCELLED) {
						m_queue->remove_min();
						continue;  // No action required, poll m_queue again
					}
					currentTime = pfn_time_proc();
					executionTime = p_task_impl->next_execution_time;
					if (taskFired = (executionTime<=currentTime)) {
						if (p_task_impl->period == 0) { // Non-repeating, remove
							m_queue->remove_min();
							p_task_impl->state = TimerTask::EXECUTED;
						} else { // Repeating task, reschedule
							m_queue->reschedule(
								p_task_impl->period<0 ? currentTime - p_task_impl->period
								: executionTime + p_task_impl->period);
						}
					}
				}
				if (!taskFired) // Task hasn't yet fired; wait
				{
					m_queue->lock.signal();
					m_queue->wait(executionTime - currentTime);
					m_queue->lock.wait();
				}
			}
			if (taskFired)  // Task fired; run it, holding no locks
				task->run();
		} catch(Exception &e) 
		{
#ifdef TIMER_PRINT_EXCEPTION
			std::cout << e.get_message() << std::endl;
#endif
		}
	} // while(true)
}

} // namespace