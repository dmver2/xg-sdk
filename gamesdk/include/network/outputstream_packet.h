/*  $Id: outputstream_packet.h,v 1.1 2005/11/14 17:07:58 dmitry.vereshchagin Exp $
**	Game SDK
**
*/

#ifndef header_outputstream_netpacket
#define header_outputstream_netpacket

#if _MSC_VER > 1000
#pragma once
#endif

#include "outputstream.h"
#include "decl6n.h"

namespace GSDK
{

class Packet;
class OutputStream_Packet_Impl;

/// 
/// @see Packet.
/** 
 *	
 */
class CxxLibExport OutputStream_Packet : public OutputStream
{
//! 
public:
	/// 
	/// 
	/// @param packet 
	OutputStream_Packet(const Packet &packet);

	/// 
	virtual ~OutputStream_Packet();

//: 
public:
	/// 
	///	@return: 
	virtual int tell() const;

	/// 
	/// @return: 
	virtual int size() const;

//: 
public:
	/// 
	/// @param data: 
	/// @param size: 
	/// @return: 
	virtual int write(const void *data, int size);
	
	/// 
	/// 
	virtual void open();
	
	/// 
	/// 
	virtual void close();

	/// 
	/// 
	/// @return: 
	virtual OutputStream *clone();

//: 
private:
	friend class Packet;

	/// 
	/// 
	OutputStream_Packet();

	/// 
	OutputStream_Packet_Impl *impl;
};

} // namespace GSDK

#endif
