/*	$Id: Thread.cpp,v 1.2 2005/11/17 16:36:20 dmitry.vereshchagin Exp $
**
**
*/

// Thread.cpp: implementation of the Thread class.
//
//////////////////////////////////////////////////////////////////////
#include "Thread.h"
#include <process.h>

//////////////////////////////////////////////////////////////////////
// class Thread Memebers
//////////////////////////////////////////////////////////////////////

namespace GSDK
{

class Thread_Proc : public Runnable
{
public:
	Thread_Proc(Thread::PTHREAD_PROC pfn_proc, Thread *p_thread)
		:	m_pfn_proc(pfn_proc)
		,	m_p_thread(p_thread)
	{
	}

	virtual void run()
	{
		m_pfn_proc(m_p_thread);
	}

private:
	Thread::PTHREAD_PROC m_pfn_proc;
	Thread* m_p_thread;
};


Thread::Thread() 
	: 	m_handle(0) 
	,	m_tid(0)
	,	m_stack_size(0)
	,	m_security(0)
	,	m_runnable(0)
	,	m_delete_runnable(0)
{
}

//Thread::Thread(Runnable *runnable, bool delete_runnable/* = false */) 
//	: 	m_handle(0) 
//	,	m_tid(0)
//	,	m_stack_size(0)
//	,	m_security(0)
//	,	m_runnable(runnable)
//	,	m_delete_runnable(delete_runnable)
//{
//}


Thread::Thread(PTHREAD_PROC pfn)
	: 	m_handle(0) 
	,	m_tid(0)
	,	m_stack_size(0)
	,	m_security(0)
	,	m_runnable(new Thread_Proc(pfn, this))
	,	m_delete_runnable(true)
{
}

Thread::Thread(Runnable *runnable, bool delete_runnable/* = false*/, LPSECURITY_ATTRIBUTES lpsa /* = NULL */, ulong stack_size /* = 0*/)
	: 	m_handle(0) 
	,	m_tid(0)
	,	m_stack_size(stack_size)
	,	m_security(lpsa)
	,	m_runnable(runnable)
	,	m_delete_runnable(delete_runnable)
{
}

Thread::~Thread() 
{ 
	if(m_handle)
		::CloseHandle(m_handle); 
	if(m_delete_runnable)
		delete m_runnable;
}

unsigned int __stdcall Thread::stdthread ( Thread * thread )
{
   int rc = thread ->main();
   _endthreadex(rc);
   ::CloseHandle(thread->m_handle);
   thread->m_handle = NULL;
   return rc;
}

bool Thread::start ( int prio/*=THREAD_PRIORITY_NORMAL*/, bool suspended/*=false*/ )
{
   
   if ( m_handle )
      return false;
   m_handle = (HANDLE)_beginthreadex ( m_security, 
                                     m_stack_size, 
                                     (PBEGIN_THREADEX_PROC)stdthread,
                                     this,suspended ? CREATE_SUSPENDED : 0,
                                     &m_tid );
   return ( m_handle != NULL);
}


//bool Thread::start ( Thread::PTHREAD_PROC proc, int prio/*=THREAD_PRIORITY_NORMAL*/, bool suspended/*=false*/ )
//{
//   if ( m_handle )
//      return false;
//   m_handle = (HANDLE)_beginthreadex ( m_security, 
//                                     m_stack_size, 
//                                     (Thread::PBEGIN_THREADEX_PROC) proc,
//                                     this,suspended ? CREATE_SUSPENDED : 0,
//                                     &m_tid );
//   return (m_handle != NULL);
//}

int Thread::main()
{
	if(m_runnable)
		m_runnable->run();
	return 0;
}

ulong Thread::wait ( time_t timeout /*= INFINITE*/ )
{ 
	if(m_handle == NULL)
		return WAIT_FAILED;

	ulong rc = ::WaitForSingleObject(m_handle,timeout); 
	if(rc == WAIT_OBJECT_0)
	{
		::CloseHandle(m_handle);
		m_handle = NULL;
	}	
	return rc;
}

bool Thread::terminate (int code)
{ 
	bool rc;
	if(m_handle)
	{
		rc = (::TerminateThread(m_handle,code) != 0); 
		::CloseHandle(m_handle);
		m_handle = NULL;
	}
	else
		rc = true;
	return rc;
}

ulong Thread::resume() 
{ 
	return ::ResumeThread(m_handle);  
}

bool    Thread::set_priority (int prio) 
{ 
	return (FALSE != ::SetThreadPriority(m_handle,prio));  
}

ulong    Thread::suspend() 
{ 
	return ::SuspendThread(m_handle); 
}

} // namespace GSDK
