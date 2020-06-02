/*  $Id: slot_impl.h,v 1.1 2005/11/14 17:07:58 dmitry.vereshchagin Exp $
** Game SDK
**
*/

#ifndef header_slot_impl
#define header_slot_impl

#if _MSC_VER > 1000
#pragma once
#endif

#include <list>
#include "signal.h"
#include "decl6n.h"

#pragma warning (disable : 4251)

namespace GSDK
{

class SigSlot;

class CxxLibExport Slot_Impl
{
// 
public:
	Slot_Impl();

	virtual ~Slot_Impl();

// 
public:
	int get_slot_ref() const;

	int get_signal_ref() const;

	bool is_persistent() const;

// 
public:
	void add_slot_ref();

	void release_slot_ref();

	void add_signal_ref(AbstractSignal *signal);

	void release_signal_ref(AbstractSignal *signal);

	void set_persistent(bool flag = true);

// 
private:
	void check_delete();

	bool persistent;

	int slot_ref_count;

	int signal_ref_count;

	std::list<AbstractSignal *> connected_signals;
};

} // namespace

#endif
