/*  $Id: outputstream_socket.h,v 1.1 2005/11/14 17:07:58 dmitry.vereshchagin Exp $
** Game SDK
**
**
*/

#ifndef header_outputstream_socket
#define header_outputstream_socket

#if _MSC_VER > 1000
#pragma once
#endif

#include "outputstream.h"
#include "decl6n.h"

namespace GSDK
{

class Socket;
class Socket_Impl;

/// 
/** 
 *	
 */
class CxxLibExport OutputStream_Socket : public OutputStream
{
//! 
public:
	/// 
	/// 
	/// @param socket 
	OutputStream_Socket(const Socket &socket);

	/// 
	virtual ~OutputStream_Socket();

//: 
public:
	/// 
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
	/// @return: 
	virtual OutputStream *clone();

//: 
private:
	/// 
	friend class Socket;

	/// 
	OutputStream_Socket() { socket = 0; ref_count_socket = false; }

	/// 
	Socket_Impl *socket;

	/// 
	bool ref_count_socket;
};

} // namespace

#endif
