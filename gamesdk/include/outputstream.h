/* $Id: outputstream.h,v 1.2 2005/11/17 16:36:20 dmitry.vereshchagin Exp $
 * Game SDK
 *
 */

#ifndef header_outputstream
#define header_outputstream

#if _MSC_VER > 1000
#pragma once
#endif

#include "types.h"
#include <string>
#include "decl6n.h"

namespace GSDK
{

/// 
/**	
 *	
 *	
 */
class CxxLibExport OutputStream
{
// 
public:
	/// 
	///	
	OutputStream() { set_system_mode(); }

	/// 
	virtual ~OutputStream() { return; }

/// 
public:
	///	
	///	
	///	@return 
	virtual int tell() const = 0;

	///	
	///	
	///	@return 
	virtual int size() const = 0;

	///	
	///	@return 
	virtual bool is_little_endian() { return little_endian_mode; }

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
	///	 @param data 
	virtual void write_int64(int64 data);

	///	 
	///	 @param data 
	virtual void write_uint64(uint64 data);

	///	 
	///	 @param data 
	virtual void write_int32(int32 data);

	///	 
	///	 @param data 
	virtual void write_uint32(uint32 data);

	///	 
	///	 @param data 
	virtual void write_int16(int16 data);

	///	 
	///	 @param data 
	virtual void write_uint16(uint16 data);

	///	 
	///	 @param data 
	virtual void write_char8(char data);

	///	 
	///	 @param data 
	virtual void write_uchar8(unsigned char data);

	///	 
	///	 @param data 32-
	virtual void write_float32(float data);

	///	 
	///	 @param data 64-
	virtual void write_float64(double data);

	///	 
	///	 @param data 
	virtual void write_bool8(bool data);

	/// 
	///	
	///	length - 32-
	///	@param str 
	virtual void write_string(const string &str);
	virtual void write_astring(const std::basic_string<char> &str);
	virtual void write_wstring(const std::basic_string<wchar_t> &str);

	/// 
	///	
	///	length - 32-
	///	@param str 
	virtual void write_string(const TCHAR *str);
	virtual void write_astring(const char *str);
	virtual void write_wstring(const wchar_t *str);

	/// 
	///	 
	/// @param data: 
	/// @param size: 
	/// @return: 
	virtual int write(const void *data, int size) = 0;
	
	/// 
	///	 
	virtual void open() = 0;
	
	/// 
	///	 
	virtual void close() = 0;

	///	 
	///	 
	/// @return 
	virtual OutputStream *clone() = 0;

//	
protected:
	bool little_endian_mode;
};

///	
/**	
 *	@param ostream	
 *	@param data	
 *	@return	
 */
CxxLibExport OutputStream &operator << (OutputStream& ostream, int64 data);

///	
/**	
 *	@param ostream	
 *	@param data	
 *	@return	
 */
CxxLibExport OutputStream &operator << (OutputStream& ostream, uint64 data);


///	
/**	
 *	@param ostream	
 *	@param data	
 *	@return	
 */
CxxLibExport OutputStream &operator << (OutputStream& ostream, int32 data);

///	
/**	
 *	@param ostream	
 *	@param data	
 *	@return	
 */
CxxLibExport OutputStream &operator << (OutputStream& ostream, uint32 data);

///	
/**	
 *	@param ostream	
 *	@param data	
 *	@return	
 */
CxxLibExport OutputStream &operator << (OutputStream& ostream, int16 data);

///	
/**	
 *	@param ostream	
 *	@param data	
 *	@return	
 */
CxxLibExport OutputStream &operator << (OutputStream& ostream, uint16 data);

///	
/**	
 *	@param ostream	
 *	@param data	
 *	@return	
 */
CxxLibExport OutputStream &operator << (OutputStream& ostream, char data);

///	
/**	
 *	@param ostream	
 *	@param data	
 *	@return	
 */
CxxLibExport OutputStream &operator << (OutputStream& ostream, uchar data);

///	
/**	
 *	@param ostream	
 *	@param data	
 *	@return	
 */
CxxLibExport OutputStream &operator << (OutputStream& ostream, float data);

///	
/**	
 *	@param ostream	
 *	@param data	
 *	@return	
 */
CxxLibExport OutputStream &operator << (OutputStream& ostream, double data);

///	
/**	
 *	@param ostream	
 *	@param data	
 *	@return	
 */
CxxLibExport OutputStream &operator << (OutputStream& ostream, bool data);

///	
/**	
 *	@param ostream	
 *	@param data	
 *	@return	
 */
CxxLibExport OutputStream &operator << (OutputStream& ostream, const string &data);


///	
/**	
 *	@param ostream	
 *	@param data	
 *	@return	
 */
CxxLibExport OutputStream &operator << (OutputStream& ostream, const wchar_t *data);
CxxLibExport OutputStream &operator << (OutputStream& ostream, const std::basic_string<wchar_t> &data);
CxxLibExport OutputStream &operator << (OutputStream& ostream, const char *data);
CxxLibExport OutputStream &operator << (OutputStream& ostream, const std::basic_string<char> &data);

} //namespace GSDK
#endif // header_outputstream
