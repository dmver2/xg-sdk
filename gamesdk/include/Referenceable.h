/*	$Id: Referenceable.h,v 1.2 2005/11/17 16:36:20 dmitry.vereshchagin Exp $
** 
**  Referenceable interface and implementation
**  
*/

#ifndef header_referenceable_
#define header_referenceable_

#if _MSC_VER > 1000
#pragma once
#endif

#include "types.h"

namespace Domain 
{

/*
* 
*/
class Referenceable
{
public:

// ctor
	Referenceable(void)
	: m_ref(0)
	{
	}

// dtor
	virtual ~Referenceable(void)
	{
	}

	uint add_ref(void);

	uint get_ref(void) const;

	uint release_ref(void);

	uint operator++ ();

	uint operator++ (int);

private:
	volatile uint m_ref;

};

inline uint Referenceable::operator++()
{
	return ++m_ref;
}

inline uint Referenceable::operator++(int)
{
	return m_ref++;
}

	
inline uint Referenceable::add_ref()
{
	return ++m_ref;
}

inline uint Referenceable::get_ref() const
{
	return m_ref;
}

inline uint Referenceable::release_ref()
{
	--m_ref;
	if (m_ref == 0)
	{
 		delete this;
		return 0;
	}
	return m_ref;
}

} // namespace Domain

#endif // header_referenceable_
