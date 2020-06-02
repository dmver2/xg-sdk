/*  $Id: outputstream_memory.h,v 1.1 2005/11/14 17:07:58 dmitry.vereshchagin Exp $
**
**	MTU Game SDK
**
*/

#ifndef header_outputstream_memory
#define header_outputstream_memory

#if _MSC_VER > 1000
#pragma once
#endif

#include "outputstream.h"
#include "decl6n.h"

namespace GSDK
{

/// 
/*!
	
	
	
	
	
	
	
*/
class CxxLibExport OutputStream_Memory : public OutputStream
{
// 
public:
	/// 
	/// 
	/// @param blocksize 
	OutputStream_Memory(unsigned int blocksize = 4*1024);

	/// 
	/** 
	 *	@param buffer 
	 *	@param size 
	 */
	OutputStream_Memory(const void* buffer, unsigned int size);

	/// 
	virtual ~OutputStream_Memory();

	/// 
	string get_data() const;

	/// 
	const void* get_ptr() const;

	/// 
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
	virtual int tell() const;

	/// 
	virtual int size() const;

	/// 
	/// 
	virtual void purge();

	virtual OutputStream *clone();

private:
	/// 
	/// 
	unsigned int	m_blocksize;

	/// 
	unsigned char	*m_data;

	/// 
	unsigned int	m_size;

	/// 
	unsigned int	m_pos;

};

} // namespace GSDK
#endif
