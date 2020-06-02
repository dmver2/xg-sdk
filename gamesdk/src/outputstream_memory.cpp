/*  $Id: outputstream_memory.cpp,v 1.1 2005/11/14 17:07:59 dmitry.vereshchagin Exp $
**
**
*/

#include "outputstream_memory.h"

namespace GSDK
{

OutputStream_Memory::OutputStream_Memory(unsigned int blocksize)
: 	m_blocksize(blocksize)
,	m_size(blocksize)
,	m_data(new unsigned char[blocksize])
,	m_pos(0)
{
}

OutputStream_Memory::OutputStream_Memory(const void* buffer, unsigned int size)
: 	m_blocksize(size)
,	m_size(size)
,	m_data(new unsigned char[size])
,	m_pos(0)
{
	write(buffer, size);
}

OutputStream_Memory::~OutputStream_Memory()
{
	delete[] m_data;
}

string OutputStream_Memory::get_data() const
{
	return string((const TCHAR*)m_data, m_pos);
}

const void *OutputStream_Memory::get_ptr() const 
{
	return  m_data;
}

int OutputStream_Memory::write(const void *data, int size)
{
	if (m_pos + size > m_size) // extend array with blocksize
	{
		int new_size = m_size + size + m_blocksize;

		unsigned char *old = m_data;
		m_data = new unsigned char[new_size];

		memcpy(m_data, old, m_pos);
		delete[] old;
		m_size = new_size;
	}
	
	memcpy(m_data + m_pos, data, size);
	m_pos += size;
	
	return size;
}

void OutputStream_Memory::open()
{
}

void OutputStream_Memory::close()
{
}

int OutputStream_Memory::tell() const
{
	return m_pos;
}

int OutputStream_Memory::size() const
{
	return m_pos;
}

void OutputStream_Memory::purge()
{
	memset(m_data,0,m_size);
	m_pos = 0;
}

OutputStream *OutputStream_Memory::clone()
{
	OutputStream_Memory *pnew_obj = new OutputStream_Memory(m_size);
	pnew_obj->m_blocksize = m_blocksize;
	pnew_obj->write(m_data, m_pos);
	return pnew_obj;
}

} // namespace GSDK