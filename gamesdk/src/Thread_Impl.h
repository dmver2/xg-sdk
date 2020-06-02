/*  $Id: Thread_Impl.h,v 1.1 2005/11/14 17:07:59 dmitry.vereshchagin Exp $
**	Game SDK
**
*/
// Thread_Impl.h: interface for the Thread_Impl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(HEADER_THREAD_IMPL_H_9505A529_5FA1_4CDD_8E75_1786B495BD98)
#define HEADER_THREAD_IMPL_H_9505A529_5FA1_4CDD_8E75_1786B495BD98

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include <iostream>
//using namespace std;

#include "stdafx.h"
#include "thread.h"
#include "types.h"

namespace GSDK
{

//////////////////////////////////////////////////////////////////////
/// 
//////////////////////////////////////////////////////////////////////
/** 
 *
 */
class Thread_Impl
{
typedef unsigned int (__stdcall * PBEGIN_THREADEX_PROC ) ( void * );

public:
	//////////////////////////////////////////////////////////////////////
	// Construction/Destruction
	//////////////////////////////////////////////////////////////////////
	/// 
	/// 
	Thread_Impl() 
		: 	m_handle(0) 
		,	m_tid(0)
		,	m_stack_size(0)
		,	m_security(0)
		,	m_runnable(0)
		,	m_delete_runnable(0)
	{
	}


#ifdef _WIN32
	  Thread_Impl(LPSECURITY_ATTRIBUTES lpsa, ulong stack )
		: 	m_handle(0) 
		,	m_tid(0)
		,	m_stack_size(stack)
		,	m_security(lpsa)
		,	m_runnable(0)
		,	m_delete_runnable(false)
	  {
	  }

	  Thread_Impl(Runnable *runnable
		  , bool delete_runnable = false
		  , LPSECURITY_ATTRIBUTES lpsa = NULL
		  , ulong stack = 0) 
		: 	m_handle(0) 
		,	m_tid(0)
		,	m_stack_size(0)
		,	m_security(lpsa)
		,	m_runnable(runnable)
		,	m_delete_runnable(delete_runnable)
	{
	}

#endif

	  virtual ~Thread_Impl() 
	  { 
		  ::CloseHandle(m_handle); 
		  if(m_delete_runnable)
			  delete m_runnable;
	  };
	  //////////////////////////////////////////////////////////////////////
	  // Methods
	  //////////////////////////////////////////////////////////////////////
	  bool     start( int prio = THREAD_PRIORITY_NORMAL, bool suspended = false );
	  bool     start( Thread::PTHREAD_PROC pfn, int prio = THREAD_PRIORITY_NORMAL, bool suspended = false );
	  ulong    set_priority (int prio) { ::SetThreadPriority(m_handle,prio);  };
	  ulong    suspend() { return ::SuspendThread(m_handle); };
	  ulong    resume() { return ::ResumeThread(m_handle);  };
	  bool     kill (int code) { return ::TerminateThread(m_handle,code) != 0; };
	  ulong    wait ( time_t time = INFINITE ) { return ::WaitForSingleObject(m_handle,time); }
	  HANDLE   get_thread_handle() const { return m_handle; }
	  ulong    get_thread_id() const { return m_tid; }

	  virtual  int main ();

protected:
	//////////////////////////////////////////////////////////////////////
	// Private Members
	//////////////////////////////////////////////////////////////////////
	static unsigned int __stdcall stdthread( Thread * thread );
	
	HANDLE   m_handle;
	u_int    m_tid;
	ulong    m_stack_size;
	Runnable *m_runnable;
	bool	 m_delete_runnable;
	
#ifdef _WIN32
	LPSECURITY_ATTRIBUTES   m_security;
#endif
};

} // namespace GSDK

#endif // !defined(HEADER_THREAD_IMPL_H_9505A529_5FA1_4CDD_8E75_1786B495BD98)
