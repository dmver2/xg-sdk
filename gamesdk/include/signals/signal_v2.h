/*  $Id: signal_v2.h,v 1.1 2005/11/14 17:07:58 dmitry.vereshchagin Exp $
**	Game SDK
**
*/


#ifndef header_signal_v2
#define header_signal_v2

#if _MSC_VER > 1000
#pragma once
#endif

#ifdef _MSC_VER
#pragma warning ( disable : 4786 )
#pragma warning ( disable : 4355 ) // warning C4355: 'this' : used in base member initializer list
#endif

#include "signal.h"
#include "slot.h"
#include "slot_v2.h"
#include <list>

namespace GSDK
{

//: Signal V2 Class
//- !group=Signal/System!
//- !header=signals.h!

template <class PARAM1, class PARAM2>
class Signal_v2 : public AbstractSignal
{
public:
	typedef Slot_v2<PARAM1, PARAM2> *Slot;
	typedef typename std::list<Slot>::iterator SlotIterator;

	class Signal_v2_Impl
	{
	public:
		Signal_v2_Impl(Signal_v2 *owner) : in_call(false), deleted(false), in_call_remove_slots(0), owner(owner) { return; }

		~Signal_v2_Impl()
		{
			for (SlotIterator slot_it = slots.begin(); slot_it != slots.end(); slot_it++)
			{
				Slot slot = *slot_it;
				slot->release_signal_ref(owner);
			}
		}

		bool in_call, deleted;
		std::list<SlotIterator> *in_call_remove_slots;
		std::list<Slot> slots;
		Signal_v2 *owner;
	};

// 
public:
	//: Signal V2 Constructor
	Signal_v2() : impl(new Signal_v2_Impl(this))
	{
	}

	Signal_v2(const Signal_v2 &copy) : impl(new Signal_v2_Impl(this))
	{
		impl->slots = copy.impl->slots;
		for (SlotIterator slot_it = impl->slots.begin(); slot_it != impl->slots.end(); slot_it++)
		{
			Slot slot = *slot_it;
			slot->add_signal_ref(this);
		}
	}

	//: Signal V2 Destructor
	~Signal_v2()
	{
		if (impl->in_call) impl->deleted = true;
		else delete impl;
	}

// 
public:
	bool has_slots_connected() const
	{
		return !impl->slots.empty();
	}

// 
public:
	Signal_v2 &operator =(const Signal_v2 &copy)
	{
		if(this == &copy)
			return *this;
		if (impl->in_call) impl->deleted = true;
		else delete impl;

		impl = new Signal_v2_Impl(this);
		impl->slots = copy.impl->slots;
		for (SlotIterator slot_it = impl->slots.begin(); slot_it != impl->slots.end(); slot_it++)
		{
			Slot slot = *slot_it;
			slot->add_signal_ref(this);
		}
		return *this;
	}

	//: Operator
	void operator() (PARAM1 param1, PARAM2 param2)
	{
		call(param1, param2);
	}

	//: Call
	void call(PARAM1 param1, PARAM2 param2)
	{
		Signal_v2_Impl *data = impl;
		data->in_call = true;

		std::list<SlotIterator> remove_slots;
		try
		{
			data->in_call_remove_slots = &remove_slots;

			// call slots connected to signal:
			for (SlotIterator slot_it = data->slots.begin(); slot_it != data->slots.end(); slot_it++)
			{
				Slot slot = *slot_it;

				// skip slot if it has no more references left in application.
				// (make it pending for removal)
				if (slot->get_slot_ref() == 0 && slot->is_persistent() == false)
				{
					if (std::find(remove_slots.begin(), remove_slots.end(), slot_it)==remove_slots.end())
						remove_slots.push_back(slot_it);
				}

				slot->call(param1, param2);
				if (data->deleted) break;
			}
		}
		catch (...)
		{
			catch_final(data, remove_slots);
			throw;
		}
		catch_final(data, remove_slots);
	}

	//: Connect slot.
	SigSlot connect(Slot slot)
	{
		slot->add_signal_ref(this);
		impl->slots.push_back(slot);
		return SigSlot(slot);
	}

	//: Connect callback function slot.
	SigSlot connect(void (*callback)(PARAM1, PARAM2))
	{
		return connect(new FunctionSlot_v2<PARAM1, PARAM2>(callback));
	}

	//: Connect functor slot.
	template<class Functor>
	SigSlot connect_functor(const Functor &functor)
	{
		return connect(new FunctorSlot_v2<Functor, PARAM1, PARAM2>(functor));
	}

	//: Connect member function slot.
	template <class CallbackClass>
	SigSlot connect(
		CallbackClass *cb_class,
		void (CallbackClass::*callback)(PARAM1, PARAM2))
	{
		return connect(new MethodSlot_v2<CallbackClass, PARAM1, PARAM2>(cb_class, callback));
	}

	//: Connect member function with user data slot.
	template <class CallbackClass, class UserData>
	SigSlot connect(
		CallbackClass *cb_class,
		void (CallbackClass::*callback)(PARAM1, PARAM2, UserData),
		UserData user_data)
	{
		return connect(new UserDataMethodSlot_v2<CallbackClass, PARAM1, PARAM2, UserData>(cb_class, callback, user_data));
	}

	//: Disconnect
	void disconnect(const SigSlot& disconnect_slot)
	{
		for (SlotIterator slot_it = impl->slots.begin(); slot_it != impl->slots.end();)
		{
			Slot slot = *slot_it;
			if (disconnect_slot.impl == slot)
			{
				if (impl->in_call)
				{
					impl->in_call_remove_slots->push_back(slot_it);
					++slot_it;
				}
				else
				{
					slot->release_signal_ref(this);
					slot_it = impl->slots.erase(slot_it);
				}
			}
			else
				++slot_it;
		}
	}

// 
private:
	void catch_final(Signal_v2_Impl *data, std::list<SlotIterator> &remove_slots)
	{
		data->in_call_remove_slots = 0;

		// remove all slots no longer connected to any SigSlot.
		typename std::list<SlotIterator>::iterator remove_it;
		for (remove_it = remove_slots.begin(); remove_it != remove_slots.end(); remove_it++)
		{
			Slot slot = **remove_it;
			slot->release_signal_ref(this);
			data->slots.erase(*remove_it);
		}

		data->in_call = false;
		if (data->deleted) delete data;
	}

	Signal_v2_Impl *impl;
};

} // namespace GSDK

#endif
