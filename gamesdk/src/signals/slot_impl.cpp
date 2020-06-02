/*  $Id: slot_impl.cpp,v 1.1 2005/11/14 17:07:59 dmitry.vereshchagin Exp $
**	Game SDK
**
*/

#include "signals/slot_impl.h"
#include "signals/slot.h"


namespace GSDK
{

/////////////////////////////////////////////////////////////////////////////
// Slot_Impl construction:

Slot_Impl::Slot_Impl() : persistent(false), slot_ref_count(0), signal_ref_count(0)
{
}

Slot_Impl::~Slot_Impl()
{
}

/////////////////////////////////////////////////////////////////////////////
// Slot_Impl attributes:

int Slot_Impl::get_slot_ref() const
{
	return slot_ref_count;
}

int Slot_Impl::get_signal_ref() const
{
	return signal_ref_count;
}

bool Slot_Impl::is_persistent() const
{
	return persistent;
}

/////////////////////////////////////////////////////////////////////////////
// Slot_Impl operations:

void Slot_Impl::add_slot_ref()
{
	slot_ref_count++;
}

void Slot_Impl::release_slot_ref()
{
	slot_ref_count--;
	check_delete();
}

void Slot_Impl::add_signal_ref(AbstractSignal *signal)
{
	connected_signals.push_back(signal);
	signal_ref_count++;
}

void Slot_Impl::release_signal_ref(AbstractSignal *signal)
{
	connected_signals.remove(signal);
	signal_ref_count--;
	check_delete();
}

void Slot_Impl::set_persistent(bool flag)
{
	persistent = flag;
}

/////////////////////////////////////////////////////////////////////////////
// 

void Slot_Impl::check_delete()
{
	// Try to disconnect from signals immediately
	if (slot_ref_count == 0 && signal_ref_count > 0)
	{
		// Avoid recursive loop 
		slot_ref_count++;

		std::list<AbstractSignal *> signals = connected_signals;
		connected_signals.clear(); 

		std::list<AbstractSignal *>::iterator it;
		for (it = signals.begin(); it != signals.end(); ++it)
		{
			AbstractSignal *cur = (*it);
			SigSlot slot(this);
			cur->disconnect(slot);
		}

		slot_ref_count--;
	}

	if (slot_ref_count == 0 && signal_ref_count == 0) 
		delete this;
}

} // namespace GSDK