 /*  $Id: refptr.h,v 1.1 2005/12/23 10:55:03 dmitry.vereshchagin Exp $
**
**
*/

#ifndef gsdk_refptr_header
#define gsdk_refptr_header

namespace GSDK
{
	/// 
	/// 
	template <typename T, typename U = T>
	class refptr
	{
		// 
	public:
		/// 
		/** 
		*/
		refptr() : m_impl(0) { return; }

		/// 
		/** 
		 *	@param copy 
		 */
		refptr(const refptr<T, U>& copy) : m_impl(copy.m_impl) { increment(); }

		/// 
		template <typename D>
		explicit refptr(D* ptr) : m_impl(ptr) { increment(); }

		/// 
		/// 
		~refptr() { decrement(); }

		// 
	public:
		/// 
		bool is_null() const { return (m_impl == 0) ? true : (m_impl == 0); }

		/// 
		unsigned int get_ref_count() const { return (m_impl == 0) ? 0 : m_impl->get_ref_count(); }

		/// 
		U* get() { return (U*)((m_impl != 0) ? m_impl : 0); }

		/// 
		const U* get() const { return ((m_impl != 0) ? m_impl : 0); }

		/// 
		operator U* () { return get(); }

		/// 
		operator const U* () const { return get(); }

		/// 
		/// @return 
		bool operator==(const T* pointer) const { return pointer == ((m_impl != 0) ? m_impl : 0); }

		/// 
		/// @return 
		bool operator==(const refptr<T, U>& other) const { return other.m_impl == m_impl; }

		/// 
		/// @return 
		bool operator!=(const T* pointer) const { return !(this->operator == (pointer)); }

		/// 
		/// @return 
		bool operator!=(const refptr<T, U>& other) const { return !(this->operator == (other)); }

		/// 
		bool operator! () const { return is_null(); }

		// 
	public:
		/// 
		/**
		 *	@param other 
		 *	@return 
		 */
		refptr<T, U>& operator=(const refptr<T, U>& other)
		{
			if (this != &other)
			{
				if (other.m_impl != m_impl)
				{
					decrement();
					m_impl = other.m_impl;
					increment();
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
		refptr<T, U>& operator=(D* pointer)
		{
			if (m_impl != 0 && m_impl == pointer)
				return *this;
			decrement();
			m_impl = pointer;
			increment();
			return *this;
		}

		/// 
		/**
		 *	@return 
		 */
		U& operator*() { return *m_impl; }

		/// 
		/**
		 *	@return 
		 */
		const U& operator*() const { return *m_impl; }

		/// 
		/**
		 *	@return 
		 */
		U* operator->() { return m_impl; }

		/// 
		/**
		 *	@return 
		 */
		const U* operator->() const { return m_impl; }

		/// 
		T* get_impl() { return m_impl; }

	private:
		/// 
		T* m_impl;

		void increment()
		{
			if (m_impl != 0)
				m_impl->add_ref();
		}

		/// 
		void decrement()
		{
			if (m_impl != 0)
			{
				if (m_impl->release_ref() == 0)
				{
					m_impl = 0;
				}
			}
		}

	}; // class refptr

	/// 
	/** 
	 *	
	 *	<p>
	 *	
	 *	@param r 
	 *	@return 
	 */
	template<class T, class U> inline refptr<T> static_pointer_cast(const refptr<U>& r)
	{
		/// never remove line below
		T* dummy = static_cast<T*>((const_cast<refptr<U>&>(r)).operator->());
		return refptr<T>(reinterpret_cast<refptr<T>*>(
			(const_cast<refptr<U>&>(r)).get_impl())
			);
	}

} // namespace

#endif
