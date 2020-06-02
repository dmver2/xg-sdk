/*  $Id: slotbuffer_v0.h,v 1.1 2005/11/14 17:07:58 dmitry.vereshchagin Exp $
**	Game SDK
**
**
*/


#ifndef header_slotbuffer_v0
#define header_slotbuffer_v0

#if _MSC_VER > 1000
#pragma once
#endif

#include "signal_v0.h"

namespace GSDK
{

/// 
class SlotBuffer_v0
{
// 
public:
	SlotBuffer_v0() : count(0)
	{
	}

	SlotBuffer_v0(Signal_v0 &signal) : count(0)
	{
		connect(signal);
	}

// 
public:
	operator bool()
	{
		return count > 0;
	}

	int size()
	{
		return count;
	}

// 
public:
	void connect(Signal_v0 &signal)
	{
		slot = signal.connect(this, &SlotBuffer_v0::callback);
	}

	void disconnect(Signal_v0 &signal)
	{
		signal.disconnect(slot);
	}

// 
private:
	void callback()
	{
		count++;
	}

	SigSlot slot;
	int count;
};

} // namespace

#endif
