/*  $Id: packet.cpp,v 1.1 2005/11/14 17:07:59 dmitry.vereshchagin Exp $
**
**
*/

#include "network/packet.h"
#include "network/host.h"
#include "packet_impl.h"
#include "inputstream_packet_impl.h"
#include "outputstream_packet_impl.h"
#include "host_impl.h"

namespace GSDK
{

/////////////////////////////////////////////////////////////////////////////
// Packet construction:

Packet::Packet()
: impl(new Packet_Impl)
{
	input.impl->netpacket = impl;
	output.impl->netpacket = impl;
	impl->add_ref();
}

Packet::Packet(const void *data, int size)
: impl(new Packet_Impl)
{
	input.impl->netpacket = impl;
	output.impl->netpacket = impl;
	impl->add_ref();

	output.write(data, size);
}

Packet::Packet(const string &data)
: impl(new Packet_Impl)
{
	input.impl->netpacket = impl;
	output.impl->netpacket = impl;
	impl->add_ref();

	output.write(data.data(), data.length());
}

Packet::Packet(const Packet &copy)
: impl(copy.impl)
{
	input.impl->netpacket = impl;
	output.impl->netpacket = impl;
	input.impl->position = copy.input.impl->position;
	output.impl->position = copy.output.impl->position;

	if (impl) impl->add_ref();
}

Packet::~Packet()
{
	if (impl) impl->release_ref();
}

/////////////////////////////////////////////////////////////////////////////
// Packet attributes:

unsigned char *Packet::get_data()
{
	return impl->data;
}

const unsigned char *Packet::get_data() const
{
	return impl->data;
}

int Packet::get_size() const
{
	return impl->size;
}

/////////////////////////////////////////////////////////////////////////////
// Packet operations:

void Packet::resize(int new_size)
{
	impl->resize(new_size);
}

Packet &Packet::operator =(const Packet &copy)
{
	if(this != &copy)
	{
		if (impl) impl->release_ref();
		impl = copy.impl;
		if (impl) impl->add_ref();

		input.impl->netpacket = impl;
		output.impl->netpacket = impl;
		input.impl->position = copy.input.impl->position;
		output.impl->position = copy.output.impl->position;
	}
	return *this;
}

/////////////////////////////////////////////////////////////////////////////
// Packet implementation:

} // namespace