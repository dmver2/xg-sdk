/*  $Id: slotbuffer_v1.h,v 1.1 2005/11/14 17:07:58 dmitry.vereshchagin Exp $
**	Game SDK
**
**
*/


#ifndef header_slotbuffer_v1
#define header_slotbuffer_v1

#if _MSC_VER > 1000
#pragma once
#endif

#include <vector>
#include "slot.h"

namespace GSDK
{
/// 
template <class PARAM1> class Signal_v1;

/// 
template <class PARAM1>
class SlotBuffer_v1
{
public:
	struct Params
	{
		PARAM1 p1;
	};

// 
public:
	SlotBuffer_v1()
	{
	}

	SlotBuffer_v1(Signal_v1<PARAM1> &signal)
	{
		connect(signal);
	}

// 
public:
	operator bool()
	{
		return !params.empty();
	}

	//: Size
	int size()
	{
		return params.size();
	}

	//: Operator
	Params &operator[](int index)
	{
		return params[index];
	}

// 
public:
	void connect(Signal_v1<PARAM1> &signal)
	{
		slot = signal.connect(this, &SlotBuffer_v1::callback);
	}

	void disconnect(Signal_v1<PARAM1> &signal)
	{
		signal.disconnect(slot);
	}

// 
private:
	void callback(PARAM1 p1)
	{
		Params p;
		p.p1 = p1;

		params.push_back(p);
	}

	std::vector<Params> params;
	SigSlot slot;
};

} // namespace GSDK

#endif
