/*	$Id: Timer.cpp,v 1.2 2005/11/17 16:36:20 dmitry.vereshchagin Exp $
**	Timer implementation
**
**
*/

#include <sys/timeb.h>
#include "timer.h"
#include <synchronization.h>
#include "timer_impl.h"
#include "system.h"

namespace GSDK
{
/*
Timer::Timer(void)
: m_impl(new Timer_Impl())
{
	m_impl->add_ref();
}
*/

Timer::Timer(Timer::pfn_get_time pfn)
: m_impl(new Timer_Impl(pfn))
{
	m_impl->add_ref();
}

Timer::~Timer(void)
{
	if(m_impl)
		m_impl->release_ref();
}

Timer::Timer(const Timer& timer)
: m_impl(timer.m_impl)
{
	if (m_impl) 
		m_impl->add_ref();
}

Timer& Timer::operator= (const Timer& timer)
{
	if(this != &timer)
	{
		if (m_impl) m_impl->release_ref();
		m_impl = timer.m_impl;
		if (m_impl) m_impl->add_ref();
	}
	return *this;
}

bool Timer::operator== (const Timer& timer) const
{
	return (this == &timer) ? true : m_impl == timer.m_impl;
}

void Timer::schedule(TimerTask::sptr task
					 , uint64 ms_delay /* = 0 */, uint64 ms_period /* = 0*/ ) throw(Exception)
{
	m_impl->schedule_task(task, m_impl->pfn_time_proc() + ms_delay, -ms_period);
}

void Timer::schedule_at(TimerTask::sptr task
						, uint64 ms_first_time, uint64 ms_period /* = 0 */) throw(Exception)
{
	m_impl->schedule_task(task, ms_first_time, -ms_period);
}

void Timer::schedule_fixed_rate(TimerTask::sptr task
								, uint64 ms_delay /* = 0 */, uint64 ms_period /* = 0*/) throw(Exception)
{
	m_impl->schedule_task(task, m_impl->pfn_time_proc() + ms_delay, ms_period);
}

void Timer::schedule_fixed_rate_at(TimerTask::sptr task
								   , uint64 ms_first_time, uint64 ms_period /*= 0*/) throw(Exception)
{
	m_impl->schedule_task(task, ms_first_time, ms_period);
}

void Timer::cancel()
{
	synchronized(m_impl->m_queue->lock);
	m_impl->m_ready2schedule = false;
	m_impl->m_queue->clear();
	m_impl->m_queue->signal();  // In case queue was already empty.
}

void Timer::adjust(int64 delta)
{
	synchronized(m_impl->m_queue->lock);
	m_impl->m_queue->adjust(delta);
	if(!m_impl->m_queue->is_empty())
		m_impl->m_queue->signal();  
}

} // namespace
