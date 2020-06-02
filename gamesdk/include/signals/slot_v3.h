/*  $Id: slot_v3.h,v 1.1 2005/11/14 17:07:58 dmitry.vereshchagin Exp $
**	Game SDK
**
**
*/

#ifndef header_slot_v3
#define header_slot_v3

#if _MSC_VER > 1000
#pragma once
#endif

#include "slot_impl.h"

namespace GSDK
{

/// 
template <class PARAM1, class PARAM2, class PARAM3>
class Slot_v3 : public Slot_Impl
{
public:
	virtual void call(PARAM1 param1, PARAM2 param2, PARAM3 param3)=0;
};

/// 
template <class PARAM1, class PARAM2, class PARAM3>
class FunctionSlot_v3 : public Slot_v3<PARAM1, PARAM2, PARAM3>
{
public:
	typedef void (*Callback)(PARAM1, PARAM2, PARAM3);

	FunctionSlot_v3(Callback callback) : callback(callback) { return; }

	virtual void call(PARAM1 param1, PARAM2 param2, PARAM3 param3) { callback(param1, param2, param3); }

private:
	Callback callback;
};

/// 
template<class Functor, class PARAM1, class PARAM2, class PARAM3>
class FunctorSlot_v3 : public Slot_v3<PARAM1, PARAM2, PARAM3>
{
public:
	FunctorSlot_v3(const Functor &functor)
	: functor(functor) { return; }

	virtual void call(PARAM1 param1, PARAM2 param2, PARAM3 param3) { functor(param1, param2, param3); }

private:
	Functor functor;
};

/// 
template <class CallbackClass, class PARAM1, class PARAM2, class PARAM3>
class MethodSlot_v3 : public Slot_v3<PARAM1, PARAM2, PARAM3>
{
public:
	typedef void (CallbackClass::*Callback)(PARAM1, PARAM2, PARAM3);

	MethodSlot_v3(CallbackClass *cb_class, Callback callback)
	: cb_class(cb_class), callback(callback) { return; }

	virtual void call(PARAM1 param1, PARAM2 param2, PARAM3 param3) { (cb_class->*callback)(param1, param2, param3); }

private:
	CallbackClass *cb_class;
	Callback callback;
};

/// 
template <class CallbackClass, class PARAM1, class PARAM2, class PARAM3, class UserData>
class UserDataMethodSlot_v3 : public Slot_v3<PARAM1, PARAM2, PARAM3>
{
public:
	typedef void (CallbackClass::*Callback)(PARAM1, PARAM2, PARAM3, UserData);

	UserDataMethodSlot_v3(CallbackClass *cb_class, Callback callback, UserData user_data)
	: cb_class(cb_class), callback(callback), user_data(user_data) { return; }

	virtual void call(PARAM1 param1, PARAM2 param2, PARAM3 param3) { (cb_class->*callback)(param1, param2, param3, user_data); }

private:
	CallbackClass *cb_class;
	Callback callback;
	UserData user_data;
};

} //namespace GSDK


#endif
