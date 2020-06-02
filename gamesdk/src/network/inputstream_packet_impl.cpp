/*  $Id: inputstream_packet_impl.cpp,v 1.1 2005/11/14 17:07:59 dmitry.vereshchagin Exp $
**
**
*/

#include "inputstream_packet_impl.h"
#include "packet_impl.h"

namespace GSDK
{

/////////////////////////////////////////////////////////////////////////////
// InputStream_Packet_Impl construction:

InputStream_Packet_Impl::InputStream_Packet_Impl(Packet_Impl *netpacket, bool ref_count)
: netpacket(netpacket), position(0), ref_count(ref_count)
{
	if (ref_count) netpacket->add_ref();
}

InputStream_Packet_Impl::~InputStream_Packet_Impl()
{
	if (ref_count) netpacket->release_ref();
}

} // namespace GSDK
