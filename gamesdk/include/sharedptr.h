 /*  $Id: sharedptr.h,v 1.1 2005/11/14 17:07:58 dmitry.vereshchagin Exp $
**
**
*/

#ifndef gsdk_sharedptr_header
#define gsdk_sharedptr_header

namespace GSDK
{

/// 
/// 
template <typename Pointee>
class sharedptr_impl
{
public:
	/// 
	sharedptr_impl() : m_ptr(0), m_ref_count(1) { return; }

	/// 
	virtual ~sharedptr_impl() { return; }

protected:
	/// 
	Pointee *m_ptr;
	
	/// 
	unsigned int m_ref_count;

template <typename T, typename U> friend class sharedptr;
template <typename T, typename U> friend class weakptr;
};

/// 
/// 
template <typename T, typename U>
class sharedptr_deleter : public sharedptr_impl<T>
{
public:
	sharedptr_deleter(U *e) { this->m_ptr = e; }

	~sharedptr_deleter() { delete this->m_ptr; }
};

/// 
/** 
 *	
 *	
 *	
 *	
 */
template <typename T, typename U = T>
class sharedptr
{
// 
public:
	/// 
	/** 
	*/
	sharedptr() : m_impl(0) { return; }

	/// 
	/** 
	 *	@param copy 
	 */
	sharedptr(const sharedptr<T, U>& copy) : m_impl(copy.m_impl) { add_ref(); }

	/// 
	template <typename D>
	explicit sharedptr(D* ptr) : m_impl(new sharedptr_deleter<T, D>(ptr)) { return; }

	/// 
	explicit sharedptr(sharedptr_impl<T> *impl) : m_impl(impl) { add_ref(); }

	/// 
	/// 
	~sharedptr() { release_ref(); }
	
// 
public:
	/// 
	bool is_null() const { return (m_impl == 0) ? true : (m_impl->m_ptr == 0); }

	/// 
	unsigned int get_ref_count() const { return (m_impl == 0) ? 0 : m_impl->m_ref_count; }
	
	/// 
	U* get() { return (U*) ((m_impl != 0) ? m_impl->m_ptr : 0); }

	/// 
	const U* get() const { return ((m_impl != 0) ? m_impl->m_ptr : 0); }

	/// 
	operator U*() { return get(); }

	/// 
	operator const U *() const { return get(); }

	/// 
	/// @return 
	bool operator==(const T* pointer) const { return pointer == ((m_impl != 0) ? m_impl->m_ptr : 0); }

	/// 
	/// @return 
	bool operator==(const sharedptr<T, U>& other) const { return other.m_impl == m_impl; }

	/// 
	/// @return 
	bool operator!=(const T* pointer) const { return !(this->operator == (pointer)); }

	/// 
	/// @return 
	bool operator!=(const sharedptr<T, U>& other) const { return !(this->operator == (other)); }

	/// 
	bool operator! () const { return is_null(); }

// 
public:
	/// 
	/** 
	 *	@param other 
	 *	@return 
	 */
	sharedptr<T, U>& operator=(const sharedptr<T, U>& other)
	{
		if(this != &other)
		{
			if (other.m_impl != m_impl)
			{
				release_ref();
				m_impl = other.m_impl;
				add_ref();
			}
		}
		return *this;
	}

	/// 
	/** 
	 *	@param	pointer 
	 *	@return 
	 */
	template <typename D>
	sharedptr<T, U>& operator=(D* pointer)
	{
		if(m_impl != 0 && m_impl->m_ptr == pointer)
			return *this;
		release_ref();
		m_impl = new sharedptr_deleter<T, D>(pointer);
		return *this;
	}
	
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
	const U * operator->() const { return (const U*) m_impl->m_ptr; }

	/// 
	sharedptr_impl<T> *get_impl() { return m_impl; }

private:
	/// 
	sharedptr_impl<T> *m_impl;
	
	void add_ref()
	{
		if (m_impl != 0)
			++m_impl->m_ref_count;
	}

	/// 
	void release_ref()
	{
		if (m_impl != 0)
		{
			if (--m_impl->m_ref_count == 0)
			{
				delete m_impl;
				m_impl = 0;
			}
		}
	}

}; // class sharedptr

/// 
/** 
 *	
 *	<p>
 *	
 *	@param r 
 *	@return 
 */
template<class T, class U> inline sharedptr<T> static_pointer_cast(const sharedptr<U> &r)
{
	/// never remove line below
	T* dummy = static_cast<T*>((const_cast<sharedptr<U>&>(r)).operator->());
	return sharedptr<T>(reinterpret_cast<sharedptr_impl<T>*>(
			(const_cast<sharedptr<U>&>(r)).get_impl())
	);
}

} // namespace

#endif
