/*  $Id: signal.h,v 1.1 2005/11/14 17:07:58 dmitry.vereshchagin Exp $
**	Game SDK
**
*/

#ifndef header_signal
#define header_signal

#if _MSC_VER > 1000
#pragma once
#endif

namespace GSDK
{
// Forward declaration
class SigSlot;

//! 
class AbstractSignal
{
public:
	/// 
	virtual ~AbstractSignal() { return; }

public:
	/// 
	/// 
	virtual void disconnect(const SigSlot& disconnect_slot)=0;
};

} // namespace GSDK
#endif
