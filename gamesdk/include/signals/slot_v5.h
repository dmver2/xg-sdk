/*  $Id: slot_v5.h,v 1.1 2005/11/14 17:07:58 dmitry.vereshchagin Exp $
**	Game SDK
**
**
*/

#ifndef header_slot_v5
#define header_slot_v5

#if _MSC_VER > 1000
#pragma once
#endif

#include "slot_impl.h"

namespace GSDK
{

/// 
template <class PARAM1, class PARAM2, class PARAM3, class PARAM4, class PARAM5>
class Slot_v5 : public Slot_Impl
{
public:
	virtual void call(PARAM1 param1, PARAM2 param2, PARAM3 param3, PARAM4 param4, PARAM5 param5)=0;
};

/// 
template <class PARAM1, class PARAM2, class PARAM3, class PARAM4, class PARAM5>
class FunctionSlot_v5 : public Slot_v5<PARAM1, PARAM2, PARAM3, PARAM4, PARAM5>
{
public:
	typedef void (*Callback)(PARAM1, PARAM2, PARAM3, PARAM4, PARAM5);

	FunctionSlot_v5(Callback callback) : callback(callback) { return; }

	virtual void call(PARAM1 param1, PARAM2 param2, PARAM3 param3, PARAM4 param4, PARAM5 param5) { callback(param1, param2, param3, param4, param5); }

private:
	Callback callback;
};

/// 
template<class Functor, class PARAM1, class PARAM2, class PARAM3, class PARAM4, class PARAM5>
class FunctorSlot_v5 : public Slot_v5<PARAM1, PARAM2, PARAM3, PARAM4, PARAM5>
{
public:
	FunctorSlot_v5(const Functor &functor)
	: functor(functor) { return; }

	virtual void call(PARAM1 param1, PARAM2 param2, PARAM3 param3, PARAM4 param4, PARAM5 param5) { functor(param1, param2, param3, param4, param5); }

private:
	Functor functor;
};

/// 
template <class CallbackClass, class PARAM1, class PARAM2, class PARAM3, class PARAM4, class PARAM5>
class MethodSlot_v5 : public Slot_v5<PARAM1, PARAM2, PARAM3, PARAM4, PARAM5>
{
public:
	typedef void (CallbackClass::*Callback)(PARAM1, PARAM2, PARAM3, PARAM4, PARAM5);

	MethodSlot_v5(CallbackClass *cb_class, Callback callback)
	: cb_class(cb_class), callback(callback) { return; }

	virtual void call(PARAM1 param1, PARAM2 param2, PARAM3 param3, PARAM4 param4, PARAM5 param5) { (cb_class->*callback)(param1, param2, param3, param4, param5); }

private:
	CallbackClass *cb_class;
	Callback callback;
};

/// 
template <class CallbackClass, class PARAM1, class PARAM2, class PARAM3, class PARAM4, class PARAM5, class UserData>
class UserDataMethodSlot_v5 : public Slot_v5<PARAM1, PARAM2, PARAM3, PARAM4, PARAM5>
{
public:
	typedef void (CallbackClass::*Callback)(PARAM1, PARAM2, PARAM3, PARAM4, PARAM5, UserData);

	UserDataMethodSlot_v5(CallbackClass *cb_class, Callback callback, UserData user_data)
	: cb_class(cb_class), callback(callback), user_data(user_data) { return; }

	virtual void call(PARAM1 param1, PARAM2 param2, PARAM3 param3, PARAM4 param4, PARAM5 param5) { (cb_class->*callback)(param1, param2, param3, param4, param5, user_data); }

private:
	CallbackClass *cb_class;
	Callback callback;
	UserData user_data;
};

} // namespace GSDK


#endif
