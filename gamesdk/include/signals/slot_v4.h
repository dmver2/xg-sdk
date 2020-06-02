/*  $Id: slot_v4.h,v 1.1 2005/11/14 17:07:58 dmitry.vereshchagin Exp $
**	Game SDK
**
**
*/

#ifndef header_slot_v4
#define header_slot_v4

#if _MSC_VER > 1000
#pragma once
#endif

#include "slot_impl.h"

namespace GSDK
{
/// 
template <class PARAM1, class PARAM2, class PARAM3, class PARAM4>
class Slot_v4 : public Slot_Impl
{
public:
	virtual void call(PARAM1 param1, PARAM2 param2, PARAM3 param3, PARAM4 param4)=0;
};

/// 
template <class PARAM1, class PARAM2, class PARAM3, class PARAM4>
class FunctionSlot_v4 : public Slot_v4<PARAM1, PARAM2, PARAM3, PARAM4>
{
public:
	typedef void (*Callback)(PARAM1, PARAM2, PARAM3, PARAM4);

	FunctionSlot_v4(Callback callback) : callback(callback) { return; }

	virtual void call(PARAM1 param1, PARAM2 param2, PARAM3 param3, PARAM4 param4) { callback(param1, param2, param3, param4); }

private:
	Callback callback;
};

/// 
template<class Functor, class PARAM1, class PARAM2, class PARAM3, class PARAM4>
class FunctorSlot_v4 : public Slot_v4<PARAM1, PARAM2, PARAM3, PARAM4>
{
public:
	FunctorSlot_v4(const Functor &functor)
	: functor(functor) { return; }

	virtual void call(PARAM1 param1, PARAM2 param2, PARAM3 param3, PARAM4 param4) { functor(param1, param2, param3, param4); }

private:
	Functor functor;
};

/// 
template <class CallbackClass, class PARAM1, class PARAM2, class PARAM3, class PARAM4>
class MethodSlot_v4 : public Slot_v4<PARAM1, PARAM2, PARAM3, PARAM4>
{
public:
	typedef void (CallbackClass::*Callback)(PARAM1, PARAM2, PARAM3, PARAM4);

	//: Methane Slot V4 constructor
	MethodSlot_v4(CallbackClass *cb_class, Callback callback)
	: cb_class(cb_class), callback(callback) { return; }

	//: Call
	virtual void call(PARAM1 param1, PARAM2 param2, PARAM3 param3, PARAM4 param4) { (cb_class->*callback)(param1, param2, param3, param4); }

private:
	CallbackClass *cb_class;
	Callback callback;
};

/// 
template <class CallbackClass, class PARAM1, class PARAM2, class PARAM3, class PARAM4, class UserData>
class UserDataMethodSlot_v4 : public Slot_v4<PARAM1, PARAM2, PARAM3, PARAM4>
{
public:
	typedef void (CallbackClass::*Callback)(PARAM1, PARAM2, PARAM3, PARAM4, UserData);

	//: User Data Methane Slot V4 Constructor
	UserDataMethodSlot_v4(CallbackClass *cb_class, Callback callback, UserData user_data)
	: cb_class(cb_class), callback(callback), user_data(user_data) { return; }

	//: Call
	virtual void call(PARAM1 param1, PARAM2 param2, PARAM3 param3, PARAM4 param4) { (cb_class->*callback)(param1, param2, param3, param4, user_data); }

private:
	CallbackClass *cb_class;
	Callback callback;
	UserData user_data;
};

} // namespace GSDK

#endif
