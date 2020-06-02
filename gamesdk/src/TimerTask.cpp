/* $Id: TimerTask.cpp,v 1.1 2005/11/14 17:07:59 dmitry.vereshchagin Exp $
**
**
*/

#include "Timer_Impl.h"
#include "TimerTask.h"

namespace GSDK
{

TimerTask::TimerTask()
: 	m_impl(new TimerTask_Impl())
{
//	m_impl->add_ref();
}

/*
TimerTask::TimerTask(const TimerTask& rcopy)
: m_impl(new TimerTask_Impl(rcopy))
{
	if (m_impl) 
		m_impl->add_ref();
}
*/

TimerTask::TimerTask(const TimerTask& rcopy)
{
}

TimerTask::~TimerTask(void)
{
	delete m_impl;
//	if (m_impl) 
//		m_impl->release_ref();
}

TimerTask& TimerTask::operator= (const TimerTask& rcopy)
{
	return *this;
}

/*
TimerTask& TimerTask::operator= (const TimerTask& rcopy)
{
	if(this != &rcopy)
	{
		if (m_impl) m_impl->release_ref();
		m_impl = rcopy.m_impl;
		if (m_impl) m_impl->add_ref();
	}
	return *this;
}
*/

/*
bool TimerTask::operator == (const TimerTask& other) const 
{
	return (this == &other) ? true : m_impl == other.m_impl;
}

bool TimerTask::operator < (const TimerTask& other) const 
{
	return m_impl < other.m_impl;
}
*/

bool TimerTask::cancel()
{
	return m_impl->cancel();
}

int64 TimerTask::get_execution_time() const
{
	return m_impl->get_execution_time();
}

int TimerTask::get_state() const
{
	return m_impl->state;
}

void TimerTask::renew()
{
	m_impl->state = TimerTask::NEW;
}

} // namespace GSDK
