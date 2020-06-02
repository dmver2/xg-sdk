/*  $Id: inputstream_packet.h,v 1.1 2005/11/14 17:07:58 dmitry.vereshchagin Exp $
** Game SDK
**
*/

#ifndef header_inputsource_netpacket
#define header_inputsource_netpacket

#if _MSC_VER > 1000
#pragma once
#endif

#include "inputstream.h"
#include "decl6n.h"

namespace GSDK
{

class Packet;
class InputStream_Packet_Impl;

/// 
//- !group=Network!
//- !header=network.h!
/**
 *	
 *	
 *	@see Packet
 *	@see InputStream
 */

class CxxLibExport InputStream_Packet : public InputStream
{
// 
public:
	///	
	/** 
	 *	@param packet 
	 */
	InputStream_Packet(const Packet &packet);

	/// 
	/** 
	 */ 
	virtual ~InputStream_Packet();

//	
public:
	///	
	///	@return	
	virtual int tell() const;

	///	
	///	@return	
	virtual int size() const;

// 
public:
	///	
	///	@param data: 
	///	@param size: 
	///	@return 
	virtual int read(void *data, int size);
	
	///	
	///	
	virtual void open();
	
	///	
	///	
	virtual void close();

	///	
	/// @return 
	virtual InputStream *clone() const;

	///	
	///	@param pos: 
	///	@param seek_type: 
	///	InputStream::seek_set - 
	///	InputStream::seek_cur - 
	///	InputStream::seek_end - 
	virtual void seek(int pos, SeekEnum seek_type);

	///	
	///	
	virtual void push_position();
	
	///	
	///	
	virtual void pop_position();

// 
private:
	friend class Packet;

	/// 
	/// 
	InputStream_Packet();

	/// 
	InputStream_Packet_Impl *impl;
};

} // namespace GSDK

#endif
