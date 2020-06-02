/*  $Id: Thread.h,v 1.1 2005/11/14 17:07:58 dmitry.vereshchagin Exp $
**	Game SDK
**
*/
// Thread.h: interface for the Thread class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(HEADER_THREAD_H_9505A529_5FA1_4CDD_8E75_1786B495BD98)
#define HEADER_THREAD_H_9505A529_5FA1_4CDD_8E75_1786B495BD98

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "stdafx.h"
#include "types.h"
#include "decl6n.h"

namespace GSDK
{

/// 
class CxxLibExport Runnable
{
// 
public:
	/// 
	virtual ~Runnable() { return; }

public:
// 
	/// 
	virtual void run() = 0;
};

//////////////////////////////////////////////////////////////////////
/// 
//////////////////////////////////////////////////////////////////////
/** 
 *	
 *	<p>
 *	<p>C
 *		
 *		
 *		
 *		
 *	</p>
 */
class CxxLibExport Thread
{
public:
	typedef unsigned int (__stdcall * PTHREAD_PROC ) ( Thread *);

	//////////////////////////////////////////////////////////////////////
	// Construction/Destruction
	//////////////////////////////////////////////////////////////////////
	/// 
	/// 
	Thread() ;

	/// 
	/** 
	 *	@param runnable 
	 *	@param delete_runnable 
	 */
//	Thread(Runnable *runnable, bool delete_runnable = false);

	/// 
	/**
	 *	@param pfn 
	 */
	Thread(PTHREAD_PROC pfn);

	/// 
	/** 
	 *	@param runnable 
	 *	@param delete_runnable 
	 *	@param lpsa 
	 *	@param stack_size 
	 */
	Thread(Runnable *runnable, bool delete_runnable = false, LPSECURITY_ATTRIBUTES lpsa = NULL, ulong stack_size = 0);

	virtual ~Thread();
	

	//////////////////////////////////////////////////////////////////////
	// Methods
	//////////////////////////////////////////////////////////////////////

	/// 
	/**	@param prio 
	 *	@param suspended 
	 *	@return 
	 */
	bool     start( int prio = THREAD_PRIORITY_NORMAL, bool suspended = false );


	/// 
	/**
	 *	@param prio	
	 *	THREAD_PRIORITY_NORMAL 
	 *	THREAD_PRIORITY_ABOVE_NORMAL 
	 *	THREAD_PRIORITY_BELOW_NORMAL 
	 *	THREAD_PRIORITY_HIGHEST 
	 *	THREAD_PRIORITY_IDLE 
	 *	THREAD_PRIORITY_LOWEST 
	 *	THREAD_PRIORITY_TIME_CRITICAL 
	 *
	 *	@return 
	 */
	bool    set_priority (int prio);

	///	
	///	@return 
	ulong    suspend();

	///	
	///	@return 
	///	
	///	
	///	
	ulong    resume();

	///	
	/**	
	 *	
	 *	
	 *	
	 *	
	 *	@param code 
	 *	@return 
	 */
	bool     terminate (int code);

	///	
	/** 
	 *	@param	timeout 
	 *	@return	
	 */
	ulong    wait ( time_t timeout = INFINITE );

	///	
	HANDLE   get_handle() const { return m_handle; }

	/// 
	ulong    get_id() const { return m_tid; }

	ulong exit_code() { ulong rc = 0; return ::GetExitCodeThread(m_handle, &rc) ? rc : -1; }

	bool is_alive() { return this->exit_code() == STILL_ACTIVE; }

private:
	/// 
	Thread &operator = (const Thread &) { return *this; }

protected:
	//////////////////////////////////////////////////////////////////////
	// Virtual Members
	//////////////////////////////////////////////////////////////////////
	/// 
	///	
	///	
	/// 
	/// @return 
	virtual  int main ();

	typedef unsigned int (__stdcall * PBEGIN_THREADEX_PROC ) ( void * );
	//////////////////////////////////////////////////////////////////////
	// Prrotected Members
	//////////////////////////////////////////////////////////////////////
	static unsigned int __stdcall stdthread( Thread * thread );

	/// 
	HANDLE   m_handle;

	/// 
	u_int    m_tid;

	/// 
	ulong    m_stack_size;

	/// 
	Runnable *m_runnable;

	/// 
	bool	 m_delete_runnable;

#ifdef WIN32
	/// 
	LPSECURITY_ATTRIBUTES   m_security;
#endif
};


/// 
/// 
/// 
template<class Class>
class Thread_Runnable_v0 : public Runnable
{
public:
	/// 
	typedef void (Class::*MemberFunction)();

	/// 
	/** 
	 *	@param self 
	 *	@param func 
	 */
	Thread_Runnable_v0(Class *self, MemberFunction func)
	: m_self(self), m_func(func)
	{
	}

private:
	/// 
	virtual void run()
	{
		(*m_self.*m_func)();
	}

	/// 
	Class *m_self;

	/// 
	MemberFunction m_func;
};

/// 
/// 
/// 
template<class Class, class Param>
class Thread_Runnable_v1 : public Runnable
{
public:
	/// 
	typedef void (Class::*MemberFunction)(Param &param);

	/// 
	/** 
	 *	@param self 
	 *	@param func 
	 *	@param param 
	 */
	Thread_Runnable_v1(Class *self, MemberFunction func, const Param &param)
	: m_self(self), m_func(func), param(param)
	{
	}

private:
	/// 
	virtual void run()
	{
		(*m_self.*m_func)(param);
	}

	/// 
	Class *m_self;

	/// 
	MemberFunction m_func;

	/// 
	Param param;
};

/// 
/// 
/// 
template<class Class, class Param1, class Param2>
class Thread_Runnable_v2 : public Runnable
{
public:
	/// 
	typedef void (Class::*MemberFunction)(Param1 &param1, Param2 &param2);

	/// 
	/** 
	 *	@param self 
	 *	@param func 
	 *	@param param1 
	 *	@param param2 
	 */
	Thread_Runnable_v2(Class *self, MemberFunction func, const Param1 &param1, const Param2 &param2)
	: m_self(self), m_func(func), param1(param1), param2(param2)
	{
	}

private:
	/// 
	virtual void run()
	{
		(*m_self.*m_func)(param1, param2);
	}

	/// 
	Class *m_self;

	/// 
	MemberFunction m_func;

	/// 
	Param1 param1;

	/// 
	Param2 param2;
};

} // namespace GSDK

#endif // !defined(HEADER_THREAD_H_9505A529_5FA1_4CDD_8E75_1786B495BD98)
