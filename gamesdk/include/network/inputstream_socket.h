/*  $Id: inputstream_socket.h,v 1.1 2005/11/14 17:07:58 dmitry.vereshchagin Exp $
**	Game SDK
**
**
*/

#ifndef header_inputstream_socket
#define header_inputstream_socket

#if _MSC_VER > 1000
#pragma once
#endif

#include "inputstream.h"
#include "decl6n.h"

namespace GSDK
{

class Socket;
class Socket_Impl;

/// 
/**
 *	
 *	
 *	@see Socket
 *	@see InputStream
 */
class CxxLibExport InputStream_Socket : public InputStream
{
// 
public:
	///	
	/** 
	 *	@param socket 
	 */
	InputStream_Socket(const Socket &socket);

	/// 
	/** 
	 */ 
	virtual ~InputStream_Socket();

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
	///	
	virtual void seek(int pos, SeekEnum seek_type);

	///	
	///	
	virtual void push_position();
	
	///	
	///	
	virtual void pop_position();

//! 
private:
	friend class Socket;

	/// 
	/// 
	InputStream_Socket() { socket = 0; ref_count_socket = false; }

	/// 
	Socket_Impl *socket;
	/// 
	bool ref_count_socket;
};

} // namespace

#endif
