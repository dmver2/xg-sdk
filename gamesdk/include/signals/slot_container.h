/*  $Id: slot_container.h,v 1.1 2005/11/14 17:07:58 dmitry.vereshchagin Exp $
**
*/


#ifndef header_slot_container
#define header_slot_container

#if _MSC_VER > 1000
#pragma once
#endif

#include "slot.h"
#include "signal_v0.h"
#include "signal_v1.h"
#include "signal_v2.h"
#include "signal_v3.h"
#include "signal_v4.h"
#include "signal_v5.h"
#include <list>

namespace GSDK
{
/// 
class SlotContainer
{
public:
// 
	template<class SigClass>
	void connect(SigClass &sig, void (*func)())
	{
		slots.push_back(sig.connect(func));
	}

	template<class SigClass, class Class>
	void connect(SigClass &sig, Class *self, void(Class::*func)())
	{
		slots.push_back(sig.connect(self, func));
	}

	template<class SigClass, class Class, class UserData>
	void connect(SigClass &sig, Class *self, void(Class::*func)(UserData), UserData user_data)
	{
		slots.push_back(sig.connect(self, func, user_data));
	}

	template<class SigClass, class Class, class Param1>
	void connect(SigClass &sig, Class *self, void(Class::*func)(Param1))
	{
		slots.push_back(sig.connect(self, func));
	}

	template<class SigClass, class Class, class Param1, class UserData>
	void connect(SigClass &sig, Class *self, void(Class::*func)(Param1, UserData), UserData user_data)
	{
		slots.push_back(sig.connect(self, func, user_data));
	}

	template<class SigClass, class Class, class Param1, class Param2>
	void connect(SigClass &sig, Class *self, void(Class::*func)(Param1, Param2))
	{
		slots.push_back(sig.connect(self, func));
	}

	template<class SigClass, class Class, class Param1, class Param2, class UserData>
	void connect(SigClass &sig, Class *self, void(Class::*func)(Param1, Param2, UserData), UserData user_data)
	{
		slots.push_back(sig.connect(self, func, user_data));
	}

	template<class SigClass, class Class, class Param1, class Param2, class Param3>
	void connect(SigClass &sig, Class *self, void(Class::*func)(Param1, Param2, Param3))
	{
		slots.push_back(sig.connect(self, func));
	}

	template<class SigClass, class Class, class Param1, class Param2, class Param3, class UserData>
	void connect(SigClass &sig, Class *self, void(Class::*func)(Param1, Param2, Param3, UserData), UserData user_data)
	{
		slots.push_back(sig.connect(self, func, user_data));
	}

	template<class SigClass, class Class, class Param1, class Param2, class Param3, class Param4>
	void connect(SigClass &sig, Class *self, void(Class::*func)(Param1, Param2, Param3, Param4))
	{
		slots.push_back(sig.connect(self, func));
	}

	template<class SigClass, class Class, class Param1, class Param2, class Param3, class Param4, class UserData>
	void connect(SigClass &sig, Class *self, void(Class::*func)(Param1, Param2, Param3, Param4, UserData user_data), UserData user_data)
	{
		slots.push_back(sig.connect(self, func, user_data));
	}

	template<class SigClass, class Class, class Param1, class Param2, class Param3, class Param4, class Param5>
	void connect(SigClass &sig, Class *self, void(Class::*func)(Param1, Param2, Param3, Param4, Param5))
	{
		slots.push_back(sig.connect(self, func));
	}

	template<class SigClass, class Class, class Param1, class Param2, class Param3, class Param4, class Param5, class UserData>
	void connect(SigClass &sig, Class *self, void(Class::*func)(Param1, Param2, Param3, Param4, Param5, UserData user_data), UserData user_data)
	{
		slots.push_back(sig.connect(self, func, user_data));
	}

	template<class SigClass, class Functor>
	void connect_functor(SigClass &sig, const Functor &functor)
	{
		slots.push_back(sig.connect_functor(functor));
	}

private:
	std::list<SigSlot> slots;
};

} // namespace GSDK

#endif
