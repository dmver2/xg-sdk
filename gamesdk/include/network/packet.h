/*  $Id: packet.h,v 1.1 2005/11/14 17:07:58 dmitry.vereshchagin Exp $
** Game SDK
**
*/

#ifndef header_gsdk_packet
#define header_gsdk_packet

#if _MSC_VER > 1000
#pragma once
#endif

#include "inputstream_packet.h"
#include "outputstream_packet.h"
#include <string>
#include <list>
#include "decl6n.h"

namespace GSDK
{

class Host;
class Packet_Impl;

/// 
/**	
 *	
 *	
 */
class CxxLibExport Packet
{
//: 
public:
	/// 
	/**	
	 *	
	 */
	Packet();

	/// 
	/// 
	/// @param data 
	/// @param size 
	Packet(const void *data, int size);

 	/// 
	/// 
	/// @param data STL-
	Packet(const string &data);

	/// 
	/// @param copy 
	Packet(const Packet &copy);

	/// 
	~Packet();

//! 
public:
	/// 
	/// @return 
	unsigned char *get_data();

	/// 
	/// @return 
	const unsigned char *get_data() const;

	/// 
	/// @return 
	int get_size() const;

	/// 
	/// 
	InputStream_Packet input;

	/// 
	OutputStream_Packet output;

// 
public:
	/// 
	/// @param new_size 
	void resize(int new_size);

	/// 
	/// @param copy 
	Packet &operator =(const Packet &copy);

//! 
private:
	//: 
	friend class InputStream_Packet;
	friend class OutputStream_Packet;
	friend class Host;
	friend class NetInterface;

	/// 
	Packet_Impl *impl;
};

}/// namespace GSDK

#endif
