/*	$Id: Thread_Impl.cpp,v 1.2 2005/11/17 16:36:20 dmitry.vereshchagin Exp $
**
**
*/

// Thread_Impl.cpp: implementation of the Thread_Impl class.
//
//////////////////////////////////////////////////////////////////////
#include "Thread.h"
#include "Thread_Impl.h"
#include <process.h>

//////////////////////////////////////////////////////////////////////
// class Thread Memebers
//////////////////////////////////////////////////////////////////////

namespace GSDK
{

unsigned int __stdcall Thread_Impl::stdthread ( Thread * thread )
{
   int rc = thread ->main();
   _endthreadex(rc);
   return rc;
}

bool Thread_Impl::start ( int prio/*=THREAD_PRIORITY_NORMAL*/, bool suspended/*=false*/ )
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

bool Thread_Impl::start ( Thread::PTHREAD_PROC proc, int prio/*=THREAD_PRIORITY_NORMAL*/, bool suspended/*=false*/ )
{
   if ( m_handle )
      return false;
   m_handle = (HANDLE)_beginthreadex ( m_security, 
                                     m_stack_size, 
                                     (PBEGIN_THREADEX_PROC) proc,
                                     this,suspended ? CREATE_SUSPENDED : 0,
                                     &m_tid );
   return (m_handle != NULL);
}

int Thread_Impl::main()
{
	if(m_runnable)
		m_runnable->run();
	return 0;
}

} // namespace GSDK
