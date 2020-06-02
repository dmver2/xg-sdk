/*  $Id: outputstream_packet.cpp,v 1.1 2005/11/14 17:07:59 dmitry.vereshchagin Exp $
**
**
*/

#include "network/outputstream_packet.h"
#include "network/packet.h"
#include "outputstream_packet_impl.h"
#include "packet_impl.h"

namespace GSDK
{

/////////////////////////////////////////////////////////////////////////////
// OutputStream_Packet construction:

OutputStream_Packet::OutputStream_Packet(const Packet &netpacket)
: impl(new OutputStream_Packet_Impl(netpacket.impl, true))
{
}

OutputStream_Packet::~OutputStream_Packet()
{
	delete impl;
}

/////////////////////////////////////////////////////////////////////////////
// OutputStream_Packet attributes:

int OutputStream_Packet::tell() const
{
	return impl->position;
}

int OutputStream_Packet::size() const
{
	return impl->netpacket->size;
}

/////////////////////////////////////////////////////////////////////////////
// OutputStream_Packet operations:

int OutputStream_Packet::write(const void *data, int size)
{
	impl->netpacket->resize(impl->position+size);
	memcpy(impl->netpacket->data+impl->position, data, size);
	impl->position += size;
	return size;
}

void OutputStream_Packet::open()
{
}

void OutputStream_Packet::close()
{
}

OutputStream *OutputStream_Packet::clone()
{
	return new OutputStream_Packet(*this);
}

/////////////////////////////////////////////////////////////////////////////
// OutputStream_Packet implementation:

OutputStream_Packet::OutputStream_Packet()
: impl(new OutputStream_Packet_Impl(0, false))
{
}

} // namespace GSDK
