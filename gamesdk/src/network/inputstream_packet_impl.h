/*  $Id: inputstream_packet_impl.h,v 1.1 2005/11/14 17:07:59 dmitry.vereshchagin Exp $
**
**
*/

#ifndef header_inputstream_packet_impl
#define header_inputstream_packet_impl

#if _MSC_VER > 1000
#pragma once
#endif

namespace GSDK
{

class Packet_Impl;

class InputStream_Packet_Impl
{
// 
public:
	InputStream_Packet_Impl(Packet_Impl *netpacket, bool ref_count);

	~InputStream_Packet_Impl();

// 
public:
	Packet_Impl *netpacket;

	int position;

// 
private:
	bool ref_count;
};

} // namespace GSDK

#endif
