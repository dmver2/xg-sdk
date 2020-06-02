/*  $Id: outputstream_packet_impl.cpp,v 1.1 2005/11/14 17:07:59 dmitry.vereshchagin Exp $
**
**
*/

#include "outputstream_packet_impl.h"
#include "packet_impl.h"

namespace GSDK
{

/////////////////////////////////////////////////////////////////////////////
// OutputStream_Packet_Impl construction:

OutputStream_Packet_Impl::OutputStream_Packet_Impl(Packet_Impl *netpacket, bool ref_count)
: netpacket(netpacket), position(0), ref_count(ref_count)
{
	if (ref_count) netpacket->add_ref();
}

OutputStream_Packet_Impl::~OutputStream_Packet_Impl()
{
	if (ref_count) netpacket->release_ref();
}

} // namespace