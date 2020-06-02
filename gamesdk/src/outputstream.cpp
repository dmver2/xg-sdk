/*  $Id: outputstream.cpp,v 1.2 2005/11/17 16:36:20 dmitry.vereshchagin Exp $
**
**
*/

#include "outputstream.h"

namespace GSDK
{

/////////////////////////////////////////////////////////////////////////////
// OutputStream operations:

void OutputStream::set_system_mode()
{
	little_endian_mode = true;
}

void OutputStream::set_big_endian_mode()
{
	little_endian_mode = false;
}

void OutputStream::set_little_endian_mode()
{
	little_endian_mode = true;
}

void OutputStream::write_int64(int64 data)
{
	write(&data, sizeof(int64));
}

void OutputStream::write_uint64(uint64 data)
{
	write(&data, sizeof(uint64));
}

void OutputStream::write_int32(int32 data)
{
	write(&data, sizeof(int32));
}

void OutputStream::write_uint32(uint32 data)
{
	write(&data, sizeof(uint32));
}

void OutputStream::write_int16(int16 data)
{
	write(&data, sizeof(int16));
}

void OutputStream::write_uint16(uint16 data)
{
	write(&data, sizeof(uint16));
}

void OutputStream::write_char8(char data)
{
	write(&data, sizeof(char));
}

void OutputStream::write_uchar8(unsigned char data)
{
	write(&data, sizeof(unsigned char));
}

void OutputStream::write_float32(float data)
{
	write(&data, sizeof(float));
}

void OutputStream::write_float64(double data)
{
	write(&data, sizeof(double));
}

void OutputStream::write_bool8(bool data)
{
	write(&data, sizeof(bool));
}

void OutputStream::write_string(const string &str)
{
	int size = str.length();
	write(&size, sizeof(size));
	write(str.data(), size * sizeof(TCHAR));
}

void OutputStream::write_astring(const std::basic_string<char> &str)
{
	int size = str.length();
	write(&size, sizeof(size));
	write(str.data(), size);
}

void OutputStream::write_wstring(const std::basic_string<wchar_t> &str)
{
	int size = str.length();
	write(&size, sizeof(size));
	write(str.data(), size * sizeof(wchar_t));
}

void OutputStream::write_string(const TCHAR* str)
{
	int size = _tcslen(str);
	write(&size, sizeof(size));
	write(str, size * sizeof(TCHAR));
}

void OutputStream::write_astring(const char* str)
{
	int size = strlen(str);
	write(&size, sizeof(size));
	write(str, size);
}

void OutputStream::write_wstring(const wchar_t* str)
{
	int size = wcslen(str);
	write(&size, sizeof(size));
	write(str, size * sizeof(wchar_t));
}

OutputStream &operator << (OutputStream& ostream, int64 data)
{
	ostream.write_int64(data);
	return ostream;
}

OutputStream &operator << (OutputStream& ostream, uint64 data)
{
	ostream.write_uint64(data);
	return ostream;
}

OutputStream &operator << (OutputStream& ostream, int32 data)
{
	ostream.write_int32(data);
	return ostream;
}

OutputStream &operator << (OutputStream& ostream, uint32 data)
{
	ostream.write_uint32(data);
	return ostream;
}

OutputStream &operator << (OutputStream& ostream, int16 data)
{
	ostream.write_int16(data);
	return ostream;
}

OutputStream &operator << (OutputStream& ostream, uint16 data)
{
	ostream.write_uint16(data);
	return ostream;
}

OutputStream &operator << (OutputStream& ostream, char data)
{
	ostream.write_char8(data);
	return ostream;
}

OutputStream &operator << (OutputStream& ostream, uchar data)
{
	ostream.write_uchar8(data);
	return ostream;
}

OutputStream &operator << (OutputStream& ostream, float data)
{
	ostream.write_float32(data);
	return ostream;
}

OutputStream &operator << (OutputStream& ostream, double data)
{
	ostream.write_float64(data);
	return ostream;
}

OutputStream &operator << (OutputStream& ostream, bool data)
{
	ostream.write_bool8(data);
	return ostream;
}

OutputStream &operator << (OutputStream& ostream, const string data)
{
	ostream.write_string(data);
	return ostream;
}

OutputStream &operator << (OutputStream& ostream, const wchar_t *data)
{
	ostream.write_wstring(data);
	return ostream;
}

OutputStream &operator << (OutputStream& ostream, const std::basic_string<wchar_t> &data)
{
	ostream.write_wstring(data);
	return ostream;
}

OutputStream &operator << (OutputStream& ostream, const char *data)
{
	ostream.write_astring(data);
	return ostream;
}

OutputStream &operator << (OutputStream& ostream, const std::basic_string<char> &data)
{
	ostream.write_astring(data);
	return ostream;
}

} // namespace GSDK
