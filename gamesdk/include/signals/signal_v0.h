/*  $Id: signal_v0.h,v 1.1 2005/11/14 17:07:58 dmitry.vereshchagin Exp $
**	Game SDK
**
*/


#ifndef header_signal_v0
#define header_signal_v0

#if _MSC_VER > 1000
#pragma once
#endif

#ifdef _MSC_VER
#pragma warning ( disable : 4786 )
#pragma warning ( disable : 4355 ) // warning C4355: 'this' : used in base member initializer list
#endif

#include "signal.h"
#include "slot.h"
#include "slot_v0.h"
#include <list>
#include <algorithm>

namespace GSDK
{

/// 
class Signal_v0 : public AbstractSignal
{
public:
	/// 
	typedef Slot_v0 *Slot;

	/// 
	typedef std::list<Slot>::iterator SlotIterator;

	/// 
	class Signal_v0_Impl
	{
	public:
		Signal_v0_Impl(Signal_v0 *owner) : in_call(false), deleted(false), in_call_remove_slots(0), owner(owner) { return; }

		~Signal_v0_Impl()
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
		Signal_v0 *owner;
	};

// 
public:
	Signal_v0() : impl(new Signal_v0_Impl(this))
	{
	}

	Signal_v0(const Signal_v0 &copy) : impl(new Signal_v0_Impl(this))
	{
		impl->slots = copy.impl->slots;
		for (SlotIterator slot_it = impl->slots.begin(); slot_it != impl->slots.end(); slot_it++)
		{
			Slot slot = *slot_it;
			slot->add_signal_ref(this);
		}
	}

	~Signal_v0()
	{
		if (impl->in_call) impl->deleted = true;
		else delete impl;
	}

// 
public:
	Signal_v0 &operator =(const Signal_v0 &copy)
	{
		if(this == &copy)
			return *this;
		if (impl->in_call) impl->deleted = true;
		else delete impl;

		impl = new Signal_v0_Impl(this);
		impl->slots = copy.impl->slots;
		for (SlotIterator slot_it = impl->slots.begin(); slot_it != impl->slots.end(); slot_it++)
		{
			Slot slot = *slot_it;
			slot->add_signal_ref(this);
		}
		return *this;
	}

	void operator() ()
	{
		call();
	}

	void call()
	{
		Signal_v0_Impl *data = impl;
		data->in_call = true;

		std::list<SlotIterator> remove_slots;
		try
		{
			data->in_call_remove_slots = &remove_slots;

			// call slots connected to signal:
			SlotParent_v0 super(data->slots.begin(), data->slots.end());
			super();
		}
		catch(...)
		{
			catch_final(data, remove_slots);
			throw;
		}

		catch_final(data, remove_slots);
	}

	SigSlot connect(Slot slot)
	{
		slot->add_signal_ref(this);
		impl->slots.push_front(slot);
		return SigSlot(slot);
	}

	SigSlot connect(void (*callback)())
	{
		return connect(new FunctionSlot_v0(callback, 0));
	}

	SigSlot connect_virtual(void (*virtual_callback)(SlotParent_v0 &))
	{
		return connect(new FunctionSlot_v0(0, virtual_callback));
	}

	template<class Functor>
	SigSlot connect_functor(const Functor &functor)
	{
		return connect(new FunctorSlot_v0<Functor>(functor));
	}

	template<class Functor>
	SigSlot connect_functor_virtual(const Functor &functor)
	{
		return connect(new FunctorVirtualSlot_v0<Functor>(functor));
	}

	template <class CallbackClass>
	SigSlot connect(CallbackClass *cb_class, void (CallbackClass::*callback)())
	{
		return connect(new MethodSlot_v0<CallbackClass>(cb_class, callback, 0));
	}

	template <class CallbackClass>
	SigSlot connect_virtual(CallbackClass *cb_class, void (CallbackClass::*virtual_callback)(SlotParent_v0 &))
	{
		return connect(new MethodSlot_v0<CallbackClass>(cb_class, 0, virtual_callback));
	}

	template <class CallbackClass, class UserData>
	SigSlot connect(
		CallbackClass *cb_class,
		void (CallbackClass::*callback)(UserData),
		UserData user_data)
	{
		return connect(new UserDataMethodSlot_v0<CallbackClass, UserData>(cb_class, callback, 0, user_data));
	}

	template <class CallbackClass, class UserData>
	SigSlot connect_virtual(
		CallbackClass *cb_class,
		void (CallbackClass::*virtual_callback)(SlotParent_v0 &, UserData),
		UserData user_data)
	{
		return connect(new UserDataMethodSlot_v0<CallbackClass, UserData>(cb_class, 0, virtual_callback, user_data));
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
	void catch_final(Signal_v0_Impl *data, std::list<SlotIterator> &remove_slots)
	{
		data->in_call_remove_slots = 0;

		for (SlotIterator slot_it = data->slots.begin(); slot_it != data->slots.end(); slot_it++)
		{
			Slot slot = *slot_it;

			if (slot->get_slot_ref() == 0 && slot->is_persistent() == false)
			{
				if (std::find(remove_slots.begin(), remove_slots.end(), slot_it)==remove_slots.end())
					remove_slots.push_back(slot_it);
			}
		}

		std::list<SlotIterator>::iterator remove_it;
		for (remove_it = remove_slots.begin(); remove_it != remove_slots.end(); remove_it++)
		{
			Slot slot = **remove_it;
			slot->release_signal_ref(this);
			data->slots.erase(*remove_it);
		}

		data->in_call = false;
		if (data->deleted) delete data;
	}

	Signal_v0_Impl *impl;
};

} //namespace GSDK
#endif
