/*  $Id: packet_impl.cpp,v 1.1 2005/11/14 17:07:59 dmitry.vereshchagin Exp $
**
**
*/

#include "packet_impl.h"
#include <string.h>

namespace GSDK
{

/////////////////////////////////////////////////////////////////////////////
// Packet_Impl construction:

Packet_Impl::Packet_Impl()
: data(0), size(0), allocated_size(0), ref_count(0)
{
}

Packet_Impl::~Packet_Impl()
{
	delete[] data;
}

/////////////////////////////////////////////////////////////////////////////
// Packet_Impl attributes:


/////////////////////////////////////////////////////////////////////////////
// Packet_Impl operations:

int Packet_Impl::add_ref()
{
	return ++ref_count;
}

int Packet_Impl::release_ref()
{
	ref_count--;
	if (ref_count > 0) return ref_count;

	delete this;
	return 0;
}

#define gsdk_min(a, b) ((a < b) ? a : b)

void Packet_Impl::resize(int new_size)
{
	if (new_size <= allocated_size)
	{
		size = new_size;
		return;
	}

	int new_allocated_size = new_size + 8*1024;
	unsigned char *old_data = data;
	data = new unsigned char[new_allocated_size];
	memcpy(data, old_data, gsdk_min(size, new_size));
	delete[] old_data;
	allocated_size = new_allocated_size;
	size = new_size;
}

/////////////////////////////////////////////////////////////////////////////
// Packet_Impl implementation:

} // namespace 