/*  $Id: slot_v1.h,v 1.1 2005/11/14 17:07:58 dmitry.vereshchagin Exp $
**	Game SDK
**
**
*/

#ifndef header_slot_v1
#define header_slot_v1

#if _MSC_VER > 1000
#pragma once
#endif

#include "slot_impl.h"

namespace GSDK
{

/// 
template <class PARAM1>
class Slot_v1 : public Slot_Impl
{
public:
	virtual void call(PARAM1 param1)=0;
};

/// 
template <class PARAM1>
class FunctionSlot_v1 : public Slot_v1<PARAM1>
{
public:
	typedef void (*Callback)(PARAM1);

	FunctionSlot_v1(Callback callback) : callback(callback) { return; }

	virtual void call(PARAM1 param1) { callback(param1); }

private:
	Callback callback;
};

/// 
template<class Functor, class PARAM1>
class FunctorSlot_v1 : public Slot_v1<PARAM1>
{
public:
	FunctorSlot_v1(const Functor &functor)
	: functor(functor) { return; }

	virtual void call(PARAM1 param1) { functor(param1); }

private:
	Functor functor;
};

/// 
template <class CallbackClass, class PARAM1>
class MethodSlot_v1 : public Slot_v1<PARAM1>
{
public:
	typedef void (CallbackClass::*Callback)(PARAM1);

	MethodSlot_v1(CallbackClass *cb_class, Callback callback)
	: cb_class(cb_class), callback(callback) { return; }

	virtual void call(PARAM1 param1) { (cb_class->*callback)(param1); }

private:
	CallbackClass *cb_class;
	Callback callback;
};

/// 
template <class CallbackClass, class PARAM1, class UserData>
class UserDataMethodSlot_v1 : public Slot_v1<PARAM1>
{
public:
	typedef void (CallbackClass::*Callback)(PARAM1, UserData);

	UserDataMethodSlot_v1(CallbackClass *cb_class, Callback callback, UserData user_data)
	: cb_class(cb_class), callback(callback), user_data(user_data) { return; }

	virtual void call(PARAM1 param1) { (cb_class->*callback)(param1, user_data); }

private:
	CallbackClass *cb_class;
	Callback callback;
	UserData user_data;
};

} // namespace GSDK

#endif
