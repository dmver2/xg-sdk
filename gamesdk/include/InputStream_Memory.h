/*  $Id: InputStream_Memory.h,v 1.1 2005/11/14 17:07:58 dmitry.vereshchagin Exp $
**
**
*/

#ifndef header_inputsource_memory
#define header_inputsource_memory

#if _MSC_VER > 1000
#pragma once
#endif

#include "inputstream.h"
#include "decl6n.h"

namespace GSDK
{
/// 
/// 
/// @see InputStream
//- !header=api.h!

class CxxLibExport InputStream_Memory : public InputStream
{
public:
	///	
	/** 
	 *	@param data	
	 *  @param size	
	 *	@param delete_data 
	 */
	InputStream_Memory(void *data, int size, bool delete_data = false);

	/// 
	/// 
	InputStream_Memory(const InputStream_Memory &rcopy);

	/// 
	/// 
	virtual ~InputStream_Memory();

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
	///	@return	
	virtual int tell() const;

	///	
	///	@param pos: 
	///	@param seek_type: 
	///	InputStream::seek_set - 
	///	InputStream::seek_cur - 
	///	InputStream::seek_end - 
	virtual void seek(int pos, SeekEnum seek_type);

	///	
	///	@return	
	virtual int size() const;

	///	
	///	
	virtual void push_position();
	
	///	
	///	
	virtual void pop_position();

	/// 
	virtual void purge();

private:
	unsigned char *m_data;
	unsigned int m_pos, m_length;
	bool m_delete_data;
};

} // namespace GSDK

#endif
