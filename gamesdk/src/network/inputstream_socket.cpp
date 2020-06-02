/*  $Id: inputstream_socket.cpp,v 1.1 2005/11/14 17:07:59 dmitry.vereshchagin Exp $
**
**
*/

#include "socket_impl.h"
#include "network/socket.h"
#include "network/inputstream_socket.h"

namespace GSDK
{

/////////////////////////////////////////////////////////////////////////////
// InputStream_Socket construction:

InputStream_Socket::InputStream_Socket(const Socket &sock)
: socket(sock.m_impl), ref_count_socket(true)
{
	socket->add_ref();
}

InputStream_Socket::~InputStream_Socket()
{
	if (ref_count_socket) socket->release_ref();
}

/////////////////////////////////////////////////////////////////////////////
// InputStream_Socket attributes:

int InputStream_Socket::tell() const
{
	return 0;
}

int InputStream_Socket::size() const
{
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// InputStream_Socket operations:

int InputStream_Socket::read(void *data, int size)
{
	char *d = (char*) data;
	int gotten = 0;

	Socket s(socket);
	while (size-gotten > 0)
	{
		int received = s.recv(d+gotten, size-gotten);
		if (received == 0)
			break;
		if (received < 0)
		{
			s.get_read_trigger()->wait();
			continue;
		}

		gotten += received;
	}
	return gotten;
}

void InputStream_Socket::open()
{
}

void InputStream_Socket::close()
{
}

InputStream *InputStream_Socket::clone() const
{
	return new InputStream_Socket(Socket(socket));
}

void InputStream_Socket::seek(int pos, SeekEnum seek_type)
{
}

void InputStream_Socket::push_position()
{
}

void InputStream_Socket::pop_position()
{
}

/////////////////////////////////////////////////////////////////////////////
// InputStream_Socket implementation:

} // namespace
