/*  $Id: slot.h,v 1.1 2005/11/14 17:07:58 dmitry.vereshchagin Exp $
** Game SDK
**
*/

#ifndef header_slot
#define header_slot

#if _MSC_VER > 1000
#pragma once
#endif

#include "decl6n.h"

namespace GSDK
{

class Slot_Impl;

/// 
class CxxLibExport SigSlot
{
// 
public:
	SigSlot();

	SigSlot(const SigSlot &copy);

	~SigSlot();

// 
public:
	/// @return 
	int get_slot_ref() const;

	/// @return 
	int get_signal_ref() const;

	/// 
	/// 
	/// @return 
	bool is_persistent() const;

// 
public:
	SigSlot &operator =(const SigSlot &copy);

	/// 
	/// @param flag 
	SigSlot& set_persistent(bool flag = true);

// 
public:
	SigSlot(Slot_Impl *impl);

private:
	Slot_Impl *impl;

friend class Signal_v0;
template<class P>											friend class Signal_v1;
template<class P1, class P2>								friend class Signal_v2;
template<class P1, class P2, class P3>						friend class Signal_v3;
template<class P1, class P2, class P3, class P4>			friend class Signal_v4;
template<class P1, class P2, class P3, class P4, class P5>	friend class Signal_v5;

};

} //namespace GSDK


#endif
