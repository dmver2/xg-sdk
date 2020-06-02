/*  $Id: signal_v5.h,v 1.1 2005/11/14 17:07:58 dmitry.vereshchagin Exp $
**	Game SDK
**
**
*/

#ifndef header_signal_v5
#define header_signal_v5

#if _MSC_VER > 1000
#pragma once
#endif

#ifdef _MSC_VER
#pragma warning ( disable : 4786 )
#pragma warning ( disable : 4355 ) // warning C4355: 'this' : used in base member initializer list
#endif

#include "signal.h"
#include "slot.h"
#include "slot_v5.h"
#include <list>

namespace GSDK
{

/// 

template <class PARAM1, class PARAM2, class PARAM3, class PARAM4, class PARAM5>
class Signal_v5 : public AbstractSignal
{
public:
	typedef Slot_v5<PARAM1, PARAM2, PARAM3, PARAM4, PARAM5> *Slot;
	typedef typename std::list<Slot>::iterator SlotIterator;

	class Signal_v5_Impl
	{
	public:
		Signal_v5_Impl(Signal_v5 *owner) : in_call(false), deleted(false), in_call_remove_slots(0), owner(owner) { return; }

		~Signal_v5_Impl()
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
		Signal_v5 *owner;
	};

// 
public:
	Signal_v5() : impl(new Signal_v5_Impl(this))
	{
	}

	Signal_v5(const Signal_v5 &copy) : impl(new Signal_v5_Impl(this))
	{
		impl->slots = copy.impl->slots;
		for (SlotIterator slot_it = impl->slots.begin(); slot_it != impl->slots.end(); slot_it++)
		{
			Slot slot = *slot_it;
			slot->add_signal_ref(this);
		}
	}

	~Signal_v5()
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
	Signal_v5 &operator =(const Signal_v5 &copy)
	{
		if(this == &copy)
			return *this;
		if (impl->in_call) impl->deleted = true;
		else delete impl;

		impl = new Signal_v5_Impl(this);
		impl->slots = copy.impl->slots;
		for (SlotIterator slot_it = impl->slots.begin(); slot_it != impl->slots.end(); slot_it++)
		{
			Slot slot = *slot_it;
			slot->add_signal_ref(this);
		}
		return *this;
	}

	void operator() (PARAM1 param1, PARAM2 param2, PARAM3 param3, PARAM4 param4, PARAM5 param5)
	{
		call(param1, param2, param3, param4, param5);
	}

	void call(PARAM1 param1, PARAM2 param2, PARAM3 param3, PARAM4 param4, PARAM5 param5)
	{
		Signal_v5_Impl *data = impl;
		data->in_call = true;

		std::list<SlotIterator> remove_slots;
		try
		{
			data->in_call_remove_slots = &remove_slots;

			for (SlotIterator slot_it = data->slots.begin(); slot_it != data->slots.end(); slot_it++)
			{
				Slot slot = *slot_it;

				if (slot->get_slot_ref() == 0 && slot->is_persistent() == false)
				{
					if (std::find(remove_slots.begin(), remove_slots.end(), slot_it)==remove_slots.end())
						remove_slots.push_back(slot_it);
				}

				slot->call(param1, param2, param3, param4, param5);
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

	SigSlot connect(Slot slot)
	{
		slot->add_signal_ref(this);
		impl->slots.push_back(slot);
		return SigSlot(slot);
	}

	SigSlot connect(void (*callback)(PARAM1, PARAM2, PARAM3, PARAM4, PARAM5))
	{
		return connect(new FunctionSlot_v5<PARAM1,PARAM2,PARAM3,PARAM4,PARAM5>(callback));
	}

	template<class Functor>
	SigSlot connect_functor(const Functor &functor)
	{
		return connect(new FunctorSlot_v5<Functor, PARAM1, PARAM2, PARAM3, PARAM4, PARAM5>(functor));
	}

	template <class CallbackClass>
	SigSlot connect(
		CallbackClass *cb_class,
		void (CallbackClass::*callback)(PARAM1, PARAM2, PARAM3, PARAM4, PARAM5))
	{
		return connect(new MethodSlot_v5<CallbackClass, PARAM1, PARAM2, PARAM3, PARAM4, PARAM5>(cb_class, callback));
	}

	template <class CallbackClass, class UserData>
	SigSlot connect(
		CallbackClass *cb_class,
		void (CallbackClass::*callback)(PARAM1, PARAM2, PARAM3, PARAM4, PARAM5, UserData),
		UserData user_data)
	{
		return connect(new UserDataMethodSlot_v5<CallbackClass, PARAM1, PARAM2, PARAM3, PARAM4, PARAM5, UserData>(cb_class, callback, user_data));
	}

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
	void catch_final(Signal_v5_Impl *data, std::list<SlotIterator> &remove_slots)
	{
		data->in_call_remove_slots = 0;

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

	Signal_v5_Impl *impl;
};

}

#endif
