/*  $Id: InputStream_Memory.cpp,v 1.1 2005/11/14 17:07:58 dmitry.vereshchagin Exp $
**
**
*/

#include "inputstream_memory.h"

namespace GSDK
{

InputStream_Memory::InputStream_Memory(
	void *_data,
	int size,
	bool delete_data)
	:	
		m_data((unsigned char *) _data)
	,	m_pos(0)
	,	m_length(size)
	,	m_delete_data(delete_data)
{
}

InputStream_Memory::InputStream_Memory(
	const InputStream_Memory &copy)
	: 
		m_data(new unsigned char[copy.size()])
	,	m_pos(copy.m_pos)
	,	m_length(copy.size())
	,	m_delete_data(true)
{
	memcpy(m_data, copy.m_data, m_length);
}

InputStream_Memory::~InputStream_Memory()
{
	if (m_delete_data) delete[] m_data;
}

int InputStream_Memory::read(void *_data, int size)
{
	if(m_pos + size > m_length)
		throw new Exception("InputStream_Memory::read failed");
	
	memcpy(_data, m_data + m_pos, size);
	m_pos += size;

	return size;
}

void InputStream_Memory::open()
{
	m_pos = 0;
}

void InputStream_Memory::close()
{
}

InputStream *InputStream_Memory::clone() const
{
//	InputStream_Memory *ret = new InputStream_Memory(m_data, m_length, m_delete_data);
//	ret->seek(m_pos, seek_set);
	InputStream_Memory *ret = new InputStream_Memory(*this);
	return ret;
}

int InputStream_Memory::tell() const
{
	return m_pos;
}

void InputStream_Memory::seek(int _pos, SeekEnum seek_type)
{
	switch (seek_type)
	{
	case seek_set:
		m_pos = _pos;
		break;
	
	case seek_cur:
		m_pos += _pos;
		break;
	
	case seek_end:
		m_pos = m_length + _pos;
		break;
	
	default: // invalid seek type!
		throw new Exception("InputStream_Memory::seek invalid seek type!");
	}
}

int InputStream_Memory::size() const
{
	return m_length;
}

void InputStream_Memory::push_position()
{
	throw new Exception("Not implemented"); // not implemented yet.
}

void InputStream_Memory::pop_position()
{
	throw new Exception("Not implemented"); // not implemented yet.
}

void InputStream_Memory::purge()
{
	memset(m_data, 0, m_length);
}

} //namespace GSDK
