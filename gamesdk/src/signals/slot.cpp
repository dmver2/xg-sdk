/*  $Id: slot.cpp,v 1.1 2005/11/14 17:07:59 dmitry.vereshchagin Exp $
**	Game SDK
**
*/

#include "signals/slot.h"
#include "signals/slot_impl.h"

/////////////////////////////////////////////////////////////////////////////
// SigSlot construction:
namespace GSDK
{

SigSlot::SigSlot() : impl(0)
{
}

SigSlot::SigSlot(const SigSlot &copy) : impl(copy.impl)
{
	if (impl != 0) impl->add_slot_ref();
}

SigSlot::~SigSlot()
{
	if (impl != 0) impl->release_slot_ref();
}

/////////////////////////////////////////////////////////////////////////////
// SigSlot attributes:

int SigSlot::get_slot_ref() const
{
	if (impl) return impl->get_slot_ref();
	else return 0;
}

int SigSlot::get_signal_ref() const
{
	if (impl) return impl->get_signal_ref();
	else return 0;
}

bool SigSlot::is_persistent() const
{
	return impl->is_persistent();
}

/////////////////////////////////////////////////////////////////////////////
// SigSlot operations:

SigSlot &SigSlot::operator =(const SigSlot &copy)
{
	if(this != &copy)
	{
		if (impl) impl->release_slot_ref();
		impl = copy.impl;
		if (impl) impl->add_slot_ref();
	}
	return *this;
}

SigSlot& SigSlot::set_persistent(bool flag)
{
	if (impl) impl->set_persistent(flag);
	return *this;
}

/////////////////////////////////////////////////////////////////////////////
// SigSlot implementation:

SigSlot::SigSlot(Slot_Impl *impl) : impl(impl)
{
	if (impl != 0) impl->add_slot_ref();
}

} // namespace GSDK
