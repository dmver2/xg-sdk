/*	$Id: TextOutputStream.cpp,v 1.2 2005/11/17 16:36:20 dmitry.vereshchagin Exp $
**
**
*/

#include "StdAfx.h"
#include "stdlib.h"
#include "textoutputstream.h"

namespace GSDK
{

TextOutputStream::TextOutputStream(void)
: m_pos(0)
{
}


TextOutputStream::~TextOutputStream(void)
{
}

//TextOutputStream& TextOutputStream::operator<< (int n)
//{
//	char buf[12] = {0};
//	itoa(n, buf, 10);
//	return write_data(buf, ;
//}
//
//TextOutputStream& TextOutputStream::operator<< (uint)
//{
//}
//
//TextOutputStream& TextOutputStream::operator<< (char);
//TextOutputStream& operator<< (wchar_t);
//TextOutputStream& operator<< (short);
//TextOutputStream& operator<< (ushort);
//TextOutputStream& operator<< (int64);
//TextOutputStream& operator<< (uint64);
//TextOutputStream& operator<< (float);
//TextOutputStream& operator<< (double);
//TextOutputStream& operator<< (const char*);
//TextOutputStream& operator<< (const wchar_t*);
//TextOutputStream& operator<< (const std::string);

} //namespace Domain