/*  $Id: slot_v0.h,v 1.1 2005/11/14 17:07:58 dmitry.vereshchagin Exp $
**	Game SDK
**
**
*/

#ifndef header_slot_v0
#define header_slot_v0

#if _MSC_VER > 1000
#pragma once
#endif

#include "slot_impl.h"
#include <list>

namespace GSDK
{

/// 
/** 
 *	
 */
class Slot_v0 : public Slot_Impl
{
public:
	/// 
	/// 
	virtual void call(class SlotParent_v0 &super)=0;
};

/// 
class SlotParent_v0
{
public:
	typedef Slot_v0 *Slot;
	typedef std::list<Slot>::iterator SlotIterator;

// 
public:
	
	SlotParent_v0(const SlotIterator &pos, const SlotIterator &end)
	: pos(pos), end(end)
	{
	}

// 
public:
	void operator() ()
	{
		if (pos == end) return;

		SlotIterator next = pos;
		next++;

		if ((*pos)->get_slot_ref() > 0)
		{
			SlotParent_v0 super(next, end);
			(*pos)->call(super);
		}
	}

// 
private:
	SlotIterator pos, end;
};

/// 
class FunctionSlot_v0 : public Slot_v0
{
public:
	typedef void (*Callback)();
	typedef void (*VirtualCallback)(SlotParent_v0 &);

	FunctionSlot_v0(Callback callback, VirtualCallback virtual_callback)
	: callback(callback), virtual_callback(virtual_callback) { return; }

	virtual void call(SlotParent_v0 &super)
	{
		if (callback) { super(); callback(); }
		else virtual_callback(super);
	}

private:
	Callback callback;
	VirtualCallback virtual_callback;
};

/// 
template<class Functor>
class FunctorSlot_v0 : public Slot_v0
{
public:
	FunctorSlot_v0(const Functor &functor)
	: functor(functor) { return; }

	virtual void call(SlotParent_v0 &super)
	{
		super();
		functor();
	}

private:
	Functor functor;
};

//: 
template<class Functor>
class FunctorVirtualSlot_v0 : public Slot_v0
{
public:
	FunctorVirtualSlot_v0(const Functor &functor)
	: functor(functor) { return; }

	virtual void call(SlotParent_v0 &super)
	{
		functor(super);
	}

private:
	Functor functor;
};

/// 
template <class CallbackClass>
class MethodSlot_v0 : public Slot_v0
{
public:
	typedef void (CallbackClass::*Callback)();
	typedef void (CallbackClass::*VirtualCallback)(SlotParent_v0 &);

	MethodSlot_v0(CallbackClass *cb_class, Callback callback, VirtualCallback virtual_callback)
	: cb_class(cb_class), callback(callback), virtual_callback(virtual_callback) { return; }

	virtual void call(SlotParent_v0 &super)
	{
		if (callback) { super(); (cb_class->*callback)(); }
		else (cb_class->*virtual_callback)(super);
	}

private:
	CallbackClass *cb_class;
	Callback callback;
	VirtualCallback virtual_callback;
};

/// 
template <class CallbackClass, class UserData>
class UserDataMethodSlot_v0 : public Slot_v0
{
public:
	typedef void (CallbackClass::*Callback)(UserData);
	typedef void (CallbackClass::*VirtualCallback)(SlotParent_v0 &, UserData);

	UserDataMethodSlot_v0(CallbackClass *cb_class, Callback callback, VirtualCallback virtual_callback, UserData user_data)
	: cb_class(cb_class), callback(callback), virtual_callback(virtual_callback), user_data(user_data) { return; }

	virtual void call(SlotParent_v0 &super)
	{
		if (callback) { super(); (cb_class->*callback)(user_data); }
		else (cb_class->*virtual_callback)(super, user_data);
	}

private:
	CallbackClass *cb_class;
	Callback callback;
	VirtualCallback virtual_callback;
	UserData user_data;
};

} //namespace GSDK

#endif
