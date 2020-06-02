/*  $Id: inputstream_packet.cpp,v 1.1 2005/11/14 17:07:59 dmitry.vereshchagin Exp $
**
**
*/

#include <assert.h>
#include "network/packet.h"
#include "network/inputstream_packet.h"
#include "inputstream_packet_impl.h"
#include "packet_impl.h"

namespace GSDK
{

/////////////////////////////////////////////////////////////////////////////
// InputStream_Packet construction:

InputStream_Packet::InputStream_Packet(const Packet &netpacket)
: impl(new InputStream_Packet_Impl(netpacket.impl, true))
{
}

InputStream_Packet::~InputStream_Packet()
{
	delete impl;
}

/////////////////////////////////////////////////////////////////////////////
// InputStream_Packet attributes:

int InputStream_Packet::tell() const
{
	return impl->position;
}

int InputStream_Packet::size() const
{
	return impl->netpacket->size;
}

/////////////////////////////////////////////////////////////////////////////
// InputStream_Packet operations:

#define gsdk_min(a, b) ((a < b) ? a : b)

int InputStream_Packet::read(void *data, int size)
{
	int received = gsdk_min(size, impl->netpacket->size - impl->position);
	memcpy(data, impl->netpacket->data+impl->position, received);
	impl->position += received;
	return received;
}

void InputStream_Packet::open()
{
}

void InputStream_Packet::close()
{
}

InputStream *InputStream_Packet::clone() const
{
	return new InputStream_Packet(*this);
}

void InputStream_Packet::seek(int pos, SeekEnum seek_type)
{
	switch (seek_type)
	{
	case seek_set:
		impl->position = pos;
		break;

	case seek_cur:
		impl->position += pos;
		break;

	case seek_end:
		impl->position = size()+pos;
		break;
	}
}

void InputStream_Packet::push_position()
{
	assert(false); // not implemented.
}

void InputStream_Packet::pop_position()
{
	assert(false); // not implemented.
}


/////////////////////////////////////////////////////////////////////////////
// InputStream_Packet implementation:

InputStream_Packet::InputStream_Packet()
: impl(new InputStream_Packet_Impl(0, false))
{
}

} // namespace GSDK
