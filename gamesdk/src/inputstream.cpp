/*  $Id: inputstream.cpp,v 1.2 2005/11/17 16:36:20 dmitry.vereshchagin Exp $
**
**
*/

#include "inputstream.h"

namespace GSDK
{

/////////////////////////////////////////////////////////////////////////////
// InputStream operations:

void InputStream::set_system_mode()
{
	little_endian_mode = true; // fix me: should use whatever endianess the system uses.
}

void InputStream::set_big_endian_mode()
{
	little_endian_mode = false;
}

void InputStream::set_little_endian_mode()
{
	little_endian_mode = true;
}

int64 InputStream::read_int64()
{
	int64 answer;
	if (read(&answer, sizeof(int64)) != sizeof(int64)) throw Exception("InputStream::read_int64() failed");
	return answer;
}

uint64 InputStream::read_uint64()
{
	uint64 answer;
	if (read(&answer, sizeof(uint64)) != sizeof(uint64)) throw Exception("InputStream::read_uint64() failed");
	return answer;
}

int32 InputStream::read_int32()
{
	int32 answer;
	if (read(&answer, sizeof(int32)) != sizeof(int32)) throw Exception("InputStream::read_int32() failed");
	return answer;
}

uint32 InputStream::read_uint32()
{
	uint32 answer;
	if (read(&answer, sizeof(uint32)) != sizeof(uint32)) throw Exception("InputStream::read_uint32() failed");
	return answer;
}

int16 InputStream::read_int16()
{
	int16 answer;
	if (read(&answer, sizeof(int16)) != sizeof(int16)) throw Exception("InputStream::read_int16() failed");
	return answer;
}

uint16 InputStream::read_uint16()
{
	uint16 answer;
	if (read(&answer, sizeof(uint16)) != sizeof(uint16)) throw Exception("InputStream::read_uint16() failed");
	return answer;
}

/*
short InputStream::read_short16()
{
	short answer;
	if (read(&answer, sizeof(short)) != sizeof(short)) throw Exception("InputStream::read_short16() failed");
	return answer;
}

unsigned short InputStream::read_uint16()
{
	unsigned short answer;
	if (read(&answer, sizeof(unsigned short)) != sizeof(unsigned short)) throw Exception("InputStream::read_uint16() failed");
	return answer;
}
*/

char InputStream::read_char8()
{
	char answer;
	if (read(&answer, sizeof(char)) != sizeof(char)) throw Exception("InputStream::read_char8() failed");
	return answer;
}

unsigned char InputStream::read_uchar8()
{
	unsigned char answer;
	if (read(&answer, sizeof(unsigned char)) != sizeof(unsigned char)) throw Exception("InputStream::read_uchar8() failed");
	return answer;
}

float InputStream::read_float32()
{
	float answer;
	if (read(&answer, sizeof(float)) != sizeof(float)) throw Exception("InputStream::read_float32() failed");
	return answer;
}

double InputStream::read_float64()
{
	double answer;
	if (read(&answer, sizeof(answer)) != sizeof(answer)) throw Exception("InputStream::read_float64() failed");
	return answer;
}

bool InputStream::read_bool8()
{
	bool answer;
	if (read(&answer, sizeof(bool)) != sizeof(bool)) throw Exception("InputStream::read_bool8() failed");
	return answer;
}

string InputStream::read_string()
{
	int size = read_int32();

	TCHAR *str = new TCHAR[size];
	try
	{
		read(str, size * sizeof(TCHAR));
		
		string ret(str, size);
		delete[] str;

		return ret;
	}
	catch (...)
	{
		delete[] str;
		throw;
	}
}

std::basic_string<wchar_t> InputStream::read_wstring()
{
	int size = read_int32();

	wchar_t *str = new wchar_t[size];
	try
	{
		read(str, size * sizeof(wchar_t));
		
		std::basic_string<wchar_t> ret(str, size);
		delete[] str;

		return ret;
	}
	catch (...)
	{
		delete[] str;
		throw;
	}
}

std::basic_string<char> InputStream::read_astring()
{
	int size = read_int32();

	char *str = new char[size];
	try
	{
		read(str, size);
		
		std::basic_string<char> ret(str, size);
		delete[] str;

		return ret;
	}
	catch (...)
	{
		delete[] str;
		throw;
	}
}

InputStream &operator >> (InputStream& istream, int64 &data)
{
	data = istream.read_int64();
	return istream;
}

InputStream &operator >> (InputStream& istream, uint64 &data)
{
	data = istream.read_uint64();
	return istream;
}

InputStream &operator >> (InputStream& istream, int32 &data)
{
	data = istream.read_int32();
	return istream;
}

InputStream &operator >> (InputStream& istream, uint32 &data)
{
	data = istream.read_uint32();
	return istream;
}

InputStream &operator >> (InputStream& istream, int16 &data)
{
	data = istream.read_int16();
	return istream;
}

InputStream &operator >> (InputStream& istream, uint16 &data)
{
	data = istream.read_uint16();
	return istream;
}

InputStream &operator >> (InputStream& istream, char &data)
{
	data = istream.read_char8();
	return istream;
}

InputStream &operator >> (InputStream& istream, uchar &data)
{
	data = istream.read_uchar8();
	return istream;
}

InputStream &operator >> (InputStream& istream, float &data)
{
	data = istream.read_float32();
	return istream;
}

InputStream &operator >> (InputStream& istream, double &data)
{
	data = istream.read_float64();
	return istream;
}

InputStream &operator >> (InputStream& istream, bool &data)
{
	data = istream.read_bool8();
	return istream;
}

InputStream &operator >> (InputStream& istream, std::basic_string<wchar_t> &data)
{
	data = istream.read_wstring();
	return istream;
}

InputStream &operator >> (InputStream& istream, std::basic_string<char> &data)
{
	data = istream.read_astring();
	return istream;
}

} // namespace GSDK
