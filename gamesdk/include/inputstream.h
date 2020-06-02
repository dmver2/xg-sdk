/*  $Id: inputstream.h,v 1.2 2005/11/17 16:36:20 dmitry.vereshchagin Exp $
** Game SDK
**
*/

#ifndef header_inputsource
#define header_inputsource

#if _MSC_VER > 1000
#pragma once
#endif

// #include <string>
#include "types.h"
#include "exception.h"
#include "decl6n.h"

namespace GSDK
{

///	
/*!
	
	
*/
class CxxLibExport InputStream
{
// 
public:
	///	
	enum SeekEnum
	{
		seek_set,	 ///< 
		seek_cur,	 ///< 
		seek_end	 ///< 
	};

// 
public:
	///	
	///	
	InputStream() { set_system_mode(); }

	///	
	virtual ~InputStream() { }

// 
public:
	///	
	///	 
	///	 @return 
	virtual int tell() const = 0;

	///	
	///	 
	///	 @return 
	virtual int size() const = 0;

	///	
	///	@return 
	virtual bool is_little_endian() const { return little_endian_mode; }

// 
public:
	///	 
	virtual void set_system_mode();

	///	 
	virtual void set_big_endian_mode();

	///	 
	///	 
	virtual void set_little_endian_mode();
	
	///	 
	///	 @return 
	virtual int64 read_int64();

	///	 
	///	 @return 
	virtual uint64 read_uint64();
	
	///	 
	///	 @return 
	virtual int32 read_int32();

	///	 
	///	 @return 
	virtual uint32 read_uint32();
	
	///	 
	///	 @return 
	virtual int16 read_int16();

	///	 
	///	 @return 
	virtual uint16 read_uint16();
	
	///	 
	///	 @return 
	virtual char read_char8();

	///	 
	///	 @return 
	virtual unsigned char read_uchar8();

	///	 
	///	 @return 32-
	virtual float read_float32();

	///	 
	///	 @return 64-
	virtual double read_float64();

	///	 
	///	 @return 
	virtual bool read_bool8();

	///	 
	///	 length - 32-
	///	 @return 
	virtual string read_string();

	///	 
	virtual std::basic_string<wchar_t> read_wstring();

	///	 
	virtual std::basic_string<char> read_astring();

	///	 
	///	param data: 
	///	param size: 
	///	 @return 
	///	 
	virtual int read(void *data, int size) = 0;
	
	///	 
	///	 
	virtual void open() = 0;
	
	///	 
	///	 
	virtual void close() = 0;

	///	 
	///	 
	/// @return InputStream* 
	virtual InputStream *clone() const = 0;

	///	 
	///	param pos: 
	///	param seek_type: 
	///	 
	///	 
	virtual void seek(int pos, SeekEnum seek_type) = 0;

	///	 
	///	 
	virtual void push_position() = 0;
	
	///	 
	///	 
	virtual void pop_position() = 0;

// 
protected:
	bool little_endian_mode;
};

///  
/**	
 *	@param istream	
 *	@param data	
 *	@return	
 */
CxxLibExport InputStream &operator >> (InputStream& istream, int64 &data);

///  
/**	
 *	@param istream	
 *	@param data	
 *	@return	
 */
CxxLibExport InputStream &operator >> (InputStream& istream, uint64 &data);

///  
/**	
 *	@param istream	
 *	@param data	
 *	@return	
 */
CxxLibExport InputStream &operator >> (InputStream& istream, int32 &data);

///  
/**	
 *	@param istream	
 *	@param data	
 *	@return	
 */
CxxLibExport InputStream &operator >> (InputStream& istream, uint32 &data);

///  
/**	 
 *	@param istream	
 *	@param data	
 *	@return	
 */
CxxLibExport InputStream &operator >> (InputStream& istream, int16 &data);

///  
/**	
 *	@param istream	
 *	@param data	
 *	@return	
 */
CxxLibExport InputStream &operator >> (InputStream& istream, uint16 &data);

/// 
/**	
 *	@param istream	
 *	@param data	
 *	@return	
 */
CxxLibExport InputStream &operator >> (InputStream& istream, char &data);

///  
/**	
 *	@param istream	
 *	@param data	
 *	@return	
 */
CxxLibExport InputStream &operator >> (InputStream& istream, uchar &data);

///  
/**	
 *	@param istream	
 *	@param data	
 *	@return	
 */
CxxLibExport InputStream &operator >> (InputStream& istream, float &data);

///  
/**	
 *	@param istream	
 *	@param data	
 *	@return	
 */
CxxLibExport InputStream &operator >> (InputStream& istream, double &data);

///  
/**	
 *	@param istream	
 *	@param data	
 *	@return	
 */
CxxLibExport InputStream &operator >> (InputStream& istream, bool &data);

///  
/**	
 *  length - 32-
 *	@param istream	
 *	@param data	
 *	@return	
 */
CxxLibExport InputStream &operator >> (InputStream& istream, std::basic_string<char> &data);
CxxLibExport InputStream &operator >> (InputStream& istream, std::basic_string<wchar_t> &data);

} // namespace GSDK
#endif
