/*  $Id: signal_v1.h,v 1.1 2005/11/14 17:07:58 dmitry.vereshchagin Exp $
**	Game SDK
**
*/


#ifndef header_signal_v1
#define header_signal_v1

#if _MSC_VER > 1000
#pragma once
#endif

#ifdef _MSC_VER
#pragma warning ( disable : 4786 )
#pragma warning ( disable : 4355 ) // warning C4355: 'this' : used in base member initializer list
#endif

#include "signal.h"
#include "slot.h"
#include "slot_v1.h"
#include <list>

namespace GSDK
{

/// 
template <class PARAM1>
class Signal_v1 : public AbstractSignal
{
public:
	typedef Slot_v1<PARAM1> *Slot;
	typedef typename std::list<Slot>::iterator SlotIterator;

	class Signal_v1_Impl
	{
	public:
		Signal_v1_Impl(Signal_v1 *owner) : in_call(false), deleted(false), in_call_remove_slots(0), owner(owner) { return; }

		~Signal_v1_Impl()
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
		Signal_v1 *owner;
	};

// 
public:
	Signal_v1() : impl(new Signal_v1_Impl(this))
	{
	}

	Signal_v1(const Signal_v1 &copy) : impl(new Signal_v1_Impl(this))
	{
		impl->slots = copy.impl->slots;
		for (SlotIterator slot_it = impl->slots.begin(); slot_it != impl->slots.end(); slot_it++)
		{
			Slot slot = *slot_it;
			slot->add_signal_ref(this);
		}
	}

	~Signal_v1()
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
	Signal_v1 &operator =(const Signal_v1 &copy)
	{
		if(this == &copy)
			return *this;
		if (impl->in_call) impl->deleted = true;
		else delete impl;

		impl = new Signal_v1_Impl(this);
		impl->slots = copy.impl->slots;
		for (SlotIterator slot_it = impl->slots.begin(); slot_it != impl->slots.end(); slot_it++)
		{
			Slot slot = *slot_it;
			slot->add_signal_ref(this);
		}
		return *this;
	}

	void operator() (PARAM1 param1)
	{
		call(param1);
	}

	void call(PARAM1 param1)
	{
		Signal_v1_Impl *data = impl;
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

				slot->call(param1);
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

	SigSlot connect(void (*callback)(PARAM1))
	{
		return connect(new FunctionSlot_v1<PARAM1>(callback));
	}

	template<class Functor>
	SigSlot connect_functor(const Functor &functor)
	{
		return connect(new FunctorSlot_v1<Functor, PARAM1>(functor));
	}

	template <class CallbackClass>
	SigSlot connect(
		CallbackClass *cb_class,
		void (CallbackClass::*callback)(PARAM1))
	{
		return connect(new MethodSlot_v1<CallbackClass, PARAM1>(cb_class, callback));
	}

	template <class CallbackClass, class UserData>
	SigSlot connect(
		CallbackClass *cb_class,
		void (CallbackClass::*callback)(PARAM1, UserData),
		UserData user_data)
	{
		return connect(new UserDataMethodSlot_v1<CallbackClass, PARAM1, UserData>(cb_class, callback, user_data));
	}

	void disconnect(const SigSlot& disconnect_slot) override;

// 
private:
	void catch_final(Signal_v1_Impl *data, std::list<SlotIterator> &remove_slots)
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

	Signal_v1_Impl *impl;
};

template <class PARAM1>
void Signal_v1<PARAM1>::disconnect(const SigSlot& disconnect_slot)
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
				if (impl->slots.size())
					slot_it = impl->slots.erase(slot_it);
				else
					break;
			}
		}
		else
			++slot_it;
	}
}
} //namespace GSDK

#endif
