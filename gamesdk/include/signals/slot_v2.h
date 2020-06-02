/*  $Id: slot_v2.h,v 1.1 2005/11/14 17:07:58 dmitry.vereshchagin Exp $
**	Game SDK
**
**
*/


#ifndef header_slot_v2
#define header_slot_v2

#if _MSC_VER > 1000
#pragma once
#endif

#include "slot_impl.h"

namespace GSDK
{

/// 
template <class PARAM1, class PARAM2>
class Slot_v2 : public Slot_Impl
{
public:
	virtual void call(PARAM1 param1, PARAM2 param2)=0;
};

/// 
template <class PARAM1, class PARAM2>
class FunctionSlot_v2 : public Slot_v2<PARAM1, PARAM2>
{
public:
	typedef void (*Callback)(PARAM1, PARAM2);

	FunctionSlot_v2(Callback callback) : callback(callback) { return; }

	virtual void call(PARAM1 param1, PARAM2 param2) { callback(param1, param2); }

private:
	Callback callback;
};

/// 
template<class Functor, class PARAM1, class PARAM2>
class FunctorSlot_v2 : public Slot_v2<PARAM1, PARAM2>
{
public:
	FunctorSlot_v2(const Functor &functor)
	: functor(functor) { return; }

	virtual void call(PARAM1 param1, PARAM2 param2) { functor(param1, param2); }

private:
	Functor functor;
};

/// 
template <class CallbackClass, class PARAM1, class PARAM2>
class  MethodSlot_v2 : public Slot_v2<PARAM1, PARAM2>
{
public:
	typedef void (CallbackClass::*Callback)(PARAM1, PARAM2);

	MethodSlot_v2(CallbackClass *cb_class, Callback callback)
	: cb_class(cb_class), callback(callback) { return; }

	virtual void call(PARAM1 param1, PARAM2 param2) { (cb_class->*callback)(param1, param2); }

private:
	CallbackClass *cb_class;
	Callback callback;
};

/// 
template <class CallbackClass, class PARAM1, class PARAM2, class UserData>
class UserDataMethodSlot_v2 : public Slot_v2<PARAM1, PARAM2>
{
public:
	typedef void (CallbackClass::*Callback)(PARAM1, PARAM2, UserData);

	UserDataMethodSlot_v2(CallbackClass *cb_class, Callback callback, UserData user_data)
	: cb_class(cb_class), callback(callback), user_data(user_data) { return; }

	virtual void call(PARAM1 param1, PARAM2 param2) { (cb_class->*callback)(param1, param2, user_data); }

private:
	CallbackClass *cb_class;
	Callback callback;
	UserData user_data;
};

} //namespace GSDK

#endif
