/*  $Id: weakptr.h,v 1.2 2005/11/17 16:36:20 dmitry.vereshchagin Exp $
**
**
*/

#ifndef _weakptr_header
#define _weakptr_header

#include "sharedptr.h"

namespace GSDK
{
/// 
/** 
 *	
 *	
 */
template <typename T, typename U = T>
class weakptr
{
// 
public:
	weakptr() : m_impl(0) { return; }

	weakptr(sharedptr<T, U> &other) : m_impl(other.get_impl()) { return; }

// 
public:
	bool is_null() const { return (m_impl == 0) ? true : (m_impl->m_ptr == 0); }

	int get_ref_count() const { return (m_impl == 0) ? 0 : m_impl->m_ref_count; }
	
	U* get() { return (U*) ((m_impl != 0) ? m_impl->m_ptr : 0); }

	const U* get() const { return (const U*) ((m_impl != 0) ? m_impl->m_ptr : 0); }

	operator const U*() const { return get(); }

	operator sharedptr<T, U>() const { return sharedptr<T, U>(m_impl); }

	/// 
	/**
	 *	@return 
	 */
	U& operator*() { return *((U*) m_impl->m_ptr); }

	/// 
	/**
	 *	@return 
	 */
	const U & operator*() const { return *((const U*) m_impl->m_ptr); }
	
	/// 
	/**
	 *	@return 
	 */
	U* operator->() { return (U*) m_impl->m_ptr; }

	/// 
	/**
	 *	@return 
	 */
	U const* operator->() const { return (const U*) m_impl->m_ptr; }

	bool operator==(const T* other) const { return other == ((m_impl != 0) ? m_impl->m_ptr : 0); }

	bool operator==(const sharedptr<T, U>& other) const { return other.m_impl == m_impl; }

	/// 
	bool operator! () const { return is_null(); }

// 
private:
	/// 
	sharedptr_impl<T> *m_impl;
};

/// 
/** 
 *	
 *	<p>
 *	
 *	@param r 
 *	@return 
 */
template<class T, class U> inline weakptr<T> static_pointer_cast(const weakptr<U> &r)
{
	/// never remove line below
	T* dummy = static_cast<T*>((const_cast<weakptr<U>&>(r)).operator->());
	return weakptr<T>(reinterpret_cast<sharedptr_impl<T>*>(
			(const_cast<weakptr<U>&>(r)).get_impl())
	);
}

} // namespace

#endif
