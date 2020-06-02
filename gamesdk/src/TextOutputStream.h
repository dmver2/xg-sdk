/*	$Id: TextOutputStream.h,v 1.2 2005/11/17 16:36:20 dmitry.vereshchagin Exp $
**
**
*/

#pragma once

#include <string>
#include <tchar.h>
#include "types.h"
#include "outputstream.h"

namespace GSDK
{

class TextOutputStream : public OutputStream
{
public:
//	TextOutputStream(void);
	virtual ~TextOutputStream(void);

	//virtual TextOutputStream& operator<< (int);
	//virtual TextOutputStream& operator<< (uint);
	//virtual TextOutputStream& operator<< (char);
	//virtual TextOutputStream& operator<< (wchar_t);
	//virtual TextOutputStream& operator<< (short);
	//virtual TextOutputStream& operator<< (ushort);
	//virtual TextOutputStream& operator<< (int64);
	//virtual TextOutputStream& operator<< (uint64);
	//virtual TextOutputStream& operator<< (float);
	//virtual TextOutputStream& operator<< (double);
	//virtual TextOutputStream& operator<< (const char*);
	//virtual TextOutputStream& operator<< (const wchar_t*);
	//virtual TextOutputStream& operator<< (const std::string);
	virtual void write_data(const void*, int len);
	virtual uint tell();

private:
	uint m_pos;
};

} // namespace GSDK