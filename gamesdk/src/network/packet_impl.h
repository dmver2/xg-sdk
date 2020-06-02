/*  $Id: packet_impl.h,v 1.1 2005/11/14 17:07:59 dmitry.vereshchagin Exp $
** Network Packet interface
**
*/

#ifndef header_gsdk_packet_impl
#define header_gsdk_packet_impl

#if _MSC_VER > 1000
#pragma once
#endif

namespace GSDK
{

class Packet_Impl
{
// 
public:
	Packet_Impl();

	~Packet_Impl();

// 
public:
	//: Packet data.
	unsigned char *data;

	//: Packet size.
	int size;

// 
public:
	int add_ref();

	int release_ref();

	void resize(int new_size);

// 
private:
	int allocated_size;

	int ref_count;
};

} // namespace GSDK

#endif
