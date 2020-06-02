/*	$Id                                                 $
 *	Synchronization.h: interface for the synchronization primitives.
 */

#if !defined(SYNCHRONIZATION_H__B9A20B76_9E57_11D3_8418_00902726205E__INCLUDED_)
#define SYNCHRONIZATION_H__B9A20B76_9E57_11D3_8418_00902726205E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#pragma warning( disable : 4244 ) 


#include "stdafx.h"
#include "types.h"
#include "decl6n.h"

namespace GSDK
{
/// 
/** 
 *	
 */
template<class T>
class AutoLockSection
{
public: 
	AutoLockSection(T &synchronizator)
		: m_synchronizator(synchronizator)
	{
		m_synchronizator.wait();
	}

	~AutoLockSection()
	{
		m_synchronizator.signal();
	}

private:
	T &m_synchronizator;
}; // class AutoLockSection

/// 
/** 
 */
class CriticalSection
{
public:
   //////////////////////////////////////////////////////////////////////
   // Construction/Destruction
   //////////////////////////////////////////////////////////////////////
   CriticalSection() {::InitializeCriticalSection(&cs);};
   ~CriticalSection() {::DeleteCriticalSection(&cs);};
   //////////////////////////////////////////////////////////////////////
   // Members
   //////////////////////////////////////////////////////////////////////
#if(_WIN32_WINNT >= 0x0400)
   bool try_wait() { return ::TryEnterCriticalSection(&cs) != 0;}
#endif
   void wait() { ::EnterCriticalSection(&cs); }
   void signal() {::LeaveCriticalSection(&cs); }

private:
   CRITICAL_SECTION cs;
}; // class CriticalSection

/// 
/**	
 *	
 */
class Mutex
{
public:
   //////////////////////////////////////////////////////////////////////
   // Construction/Destruction
   //////////////////////////////////////////////////////////////////////
   Mutex( bool initial_lock = false ) : m_hMutex(nullptr) { m_hMutex = ::CreateMutex(NULL,initial_lock,NULL); }
   ~Mutex() { ::CloseHandle(m_hMutex);}
   //////////////////////////////////////////////////////////////////////
   // Members
   //////////////////////////////////////////////////////////////////////
   ulong    wait ( time_t timeout = INFINITE )  { return ::WaitForSingleObject(m_hMutex,timeout); }
   bool     signal ()                           { return ::ReleaseMutex(m_hMutex) != 0; }
   operator HANDLE() const                      { return m_hMutex; }

private:

   HANDLE m_hMutex;
}; // class Mutex

/// 
/**	
 *	
 */
class Semaphore
{
public:
   //////////////////////////////////////////////////////////////////////
   // Construction/Destruction
   //////////////////////////////////////////////////////////////////////
   Semaphore( long count = 0, long max = 0x7fffffff ) {  m_hsem = ::CreateSemaphore(NULL,count,max,NULL); }
   ~Semaphore() { ::CloseHandle(m_hsem); }
   //////////////////////////////////////////////////////////////////////
   // Members
   //////////////////////////////////////////////////////////////////////
   ulong    wait ( time_t time = INFINITE )              { return ::WaitForSingleObject(m_hsem,time);  }
   bool     signal ( long count = 1, long * prev = 0 )   { return ::ReleaseSemaphore(m_hsem,count,prev) == TRUE; };
   operator HANDLE() const                               { return m_hsem; }

private:
   HANDLE m_hsem;
}; // class Semaphore

/// 
/**	
 *	
 */
class Event
{
public:
   //////////////////////////////////////////////////////////////////////
   // Construction/Destruction
   //////////////////////////////////////////////////////////////////////
   Event(bool signaled = false, bool manual = true ) { m_hevent = ::CreateEvent(NULL,manual,signaled,NULL); }
   ~Event() { ::CloseHandle(m_hevent); }
   //////////////////////////////////////////////////////////////////////
   // Members
   //////////////////////////////////////////////////////////////////////
   ulong    wait ( time_t time = INFINITE ) const  { return ::WaitForSingleObject(m_hevent,time); }
   bool     get_flag () const	   { return WAIT_TIMEOUT != ::WaitForSingleObject(m_hevent,0); }
   bool     signal ()              { return ::SetEvent(m_hevent) != 0;  }
   bool     reset()                { return ::ResetEvent(m_hevent)!= 0; }
   bool     pulse()                { return ::PulseEvent(m_hevent)!= 0; }
   operator HANDLE() const         { return m_hevent; }

private:
   HANDLE m_hevent;
}; // class Event

/// 
/** 
 *	
 */
class MutexSection
{
// 
public:
	/// 
	/**
	 * @param mutex		
	 * @param locked	
	 */
	MutexSection(Mutex &mutex, bool locked = true)
	: m_mutex(&mutex), m_lock_count(0)
	{
		if (locked) enter();
	}

	/// 
	virtual ~MutexSection()
	{
		while (m_lock_count > 0) leave();
	}

// 
public:
	/// 
	void enter()
	{
		m_mutex->wait();
		m_lock_count++;
	}

	/// 
	void leave()
	{
		if (m_lock_count == 0) return;
		m_mutex->signal();
		m_lock_count--;
	}

// 
private:
	Mutex *m_mutex;

	unsigned int m_lock_count;
}; //class MutexSection

} // namespace GSDK

#endif // !defined(SYNCHRONIZATION_H__B9A20B76_9E57_11D3_8418_00902726205E__INCLUDED_)
