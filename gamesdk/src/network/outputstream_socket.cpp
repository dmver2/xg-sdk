/*  $Id: outputstream_socket.cpp,v 1.1 2005/11/14 17:07:59 dmitry.vereshchagin Exp $
**
**
**
*/

#include "network/outputstream_socket.h"
#include "network/socket.h"
#include "socket_impl.h"

namespace GSDK
{

/////////////////////////////////////////////////////////////////////////////
// OutputStream_Socket Construction:

OutputStream_Socket::OutputStream_Socket(const Socket &sock)
: socket(sock.m_impl), ref_count_socket(true)
{
	socket->add_ref();
}

OutputStream_Socket::~OutputStream_Socket()
{
	if (ref_count_socket) socket->release_ref();
}

/////////////////////////////////////////////////////////////////////////////
// OutputStream_Socket Attributes:

int OutputStream_Socket::tell() const
{
	return 0;
}

int OutputStream_Socket::size() const
{
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// OutputStream_Socket Operations:

int OutputStream_Socket::write(const void *data, int size)
{
	Socket s(socket);
	char *d = (char*) data;
	int total_sent = 0;
	while (size-total_sent > 0)
	{
		int sent = s.send(d+total_sent, size-total_sent);
		if (sent <= 0)
			break;

		total_sent += sent;
	}
	return total_sent;
}
	
void OutputStream_Socket::open()
{
}

void OutputStream_Socket::close()
{
}

OutputStream *OutputStream_Socket::clone()
{
	return new OutputStream_Socket(Socket(socket));
}

} // namespace