/*  $Id: socket_impl.cpp,v 1.2 2005/11/17 16:36:20 dmitry.vereshchagin Exp $
**
**
**
*/

//#ifdef _MSC_VER
//#pragma warning (disable:4786)
//#define snprintf _snprintf
//#endif

#ifndef WIN32
#include <errno.h>
#endif

#include "socket_impl.h"
#include "socket_selector.h"
#include <WS2tcpip.h>
#include <Mstcpip.h>
//#include <log.h>

namespace GSDK
{

	//===========================================================================
	// Socket_Impl static data:

	SocketSelector* Socket_Impl::sp_socket_selector = nullptr;

	//===========================================================================/////////
	// Socket_Impl static operations:

	IPAddress Socket_Impl::create_ip_address(const sockaddr_in& addr_in)
	{
		TCHAR szaddr[0X20];
		InetNtop(AF_INET, &addr_in.sin_addr, szaddr, sizeof(szaddr));
		return IPAddress(szaddr, ntohs(addr_in.sin_port));
	}

	//===========================================================================
	// Socket_Impl construction:

	Socket_Impl::Socket_Impl()
		: sock(-1), read(nullptr), write(nullptr), exception(nullptr)
		//, keep_alive_helper(0)
		, ref_count(0), valid(true)
	{
	}

	Socket_Impl::~Socket_Impl()
	{
		//		Log::printf("DBG", Log::debug, "deleting %p...", read);
		if (read)
			delete read;
		if (write)
			delete write;
		if (exception)
			delete exception;
		disconnect();
		//	delete keep_alive_helper;
	}

	//===========================================================================
	// Socket_Impl attributes:

	//===========================================================================
	// Socket_Impl operations:

	void Socket_Impl::add_ref()
	{
		ref_count++;
	}

	void Socket_Impl::release_ref()
	{
		ref_count--;
		if (ref_count == 0) delete this;
	}

	const TCHAR* Socket_Impl::get_error_message()
	{
		return get_error_message(::WSAGetLastError());
	}

	const TCHAR* Socket_Impl::get_error_message(unsigned long err_code)
	{
		switch (err_code)
		{
		case WSAEACCES:
			return _T("Permission denied");
		case WSAEADDRINUSE:
			return _T("Address already in use");
		case WSAEADDRNOTAVAIL:
			return _T("Cannot assign requested address");
		case WSAEAFNOSUPPORT:
			return _T("Address family not supported by protocol family");
		case WSAEALREADY:
			return _T("Operation already in progress");
		case WSAECONNABORTED:
			return _T("Software caused connection abort");
		case WSAECONNREFUSED:
			return _T("Connection refused");
		case WSAECONNRESET:
			return _T("Connection reset by peer");
		case WSAEDESTADDRREQ:
			return _T("Destination address required");
		case WSAEFAULT:
			return _T("Bad address");
		case WSAEHOSTDOWN:
			return _T("Host is down");
		case WSAEINPROGRESS:
			return _T("Operation now in progress");
		case WSAEINTR:
			return _T("Interrupted function call");
		case WSAEINVAL:
			return _T("Invalid argument");
		case WSAEISCONN:
			return _T("Socket is already connected");
		case WSAEMFILE:
			return _T("Too many open files");
		case WSAEMSGSIZE:
			return _T("Message too long");
		case WSAENETDOWN:
			return _T("Network is down");
		case WSAENETRESET:
			return _T("Network dropped connection on reset");
		case WSAENETUNREACH:
			return _T("Network is unreachable");
		case WSAENOBUFS:
			return _T("No buffer space available");
		case WSAENOPROTOOPT:
			return _T("Bad protocol option");
		case WSAENOTCONN:
			return _T("Socket is not connected");
		case WSAENOTSOCK:
			return _T("Socket operation on nonsocket");
		case WSAEOPNOTSUPP:
			return _T("Operation not supported");
		case WSAEPFNOSUPPORT:
			return _T("Protocol family not supported");
		case WSAEPROCLIM:
			return _T("Too many processes");
		case WSAEPROTONOSUPPORT:
			return _T("Protocol not supported");
		case WSAEPROTOTYPE:
			return _T("Protocol wrong type for socket");
		case WSAESHUTDOWN:
			return _T("Cannot send after socket shutdown");
		case WSAESOCKTNOSUPPORT:
			return _T("Socket type not supported");
		case WSAETIMEDOUT:
			return _T("Connection timed out");
		case WSATYPE_NOT_FOUND:
			return _T("Class type not found");
		case WSAEWOULDBLOCK:
			return _T("Resource temporarily unavailable");
		case WSAHOST_NOT_FOUND:
			return _T("Host not found");
		case WSA_INVALID_HANDLE:
			return _T("Specified event object handle is invalid");
		case WSA_INVALID_PARAMETER:
			return _T("One or more parameters are invalid");
			//case WSAINVALIDPROCTABLE:
			//	return _T("Invalid procedure table from service provider");
			//case WSAINVALIDPROVIDER:
			//	return _T("Invalid service provider version number");
		case WSA_IO_INCOMPLETE:
			return _T("Overlapped I/O event object not in signaled state");
		case WSA_IO_PENDING:
			return _T("Overlapped operations will complete later");
		case WSA_NOT_ENOUGH_MEMORY:
			return _T("Insufficient memory available");
		case WSANOTINITIALISED:
			return _T("Successful WSAStartup not yet performed");
		case WSANO_DATA:
			return _T("Valid name, no data record of requested type");
		case WSANO_RECOVERY:
			return _T("This is a nonrecoverable error");
			//	case WSAPROVIDERFAILEDINIT:
			//		return _T("Unable to initialize a service provider");
			//	case WSASYSCALLFAILURE:
			//		return _T("System call failure");
		case WSASYSNOTREADY:
			return _T("Network subsystem is unavailable");
		case WSATRY_AGAIN:
			return _T("Nonauthoritative host not found");
		case WSAVERNOTSUPPORTED:
			return _T("Winsock.dll version out of range");
		case WSAEDISCON:
			return _T("Graceful shutdown in progress");
			//	case WSA_OPERATION_ABORTED:
			//		return _T("Overlapped operation aborted");
		default:
			return _T("Unknown socket error");
		}
	}

	//===========================================================================/////////
	// Socket_Impl overridables:

	EventTrigger_Socket* Socket_Impl::create_read_trigger()
	{
		return new EventTrigger_Socket(this, EventTrigger_Socket::read);
	}

	EventTrigger_Socket* Socket_Impl::create_write_trigger()
	{
		return new EventTrigger_Socket(this, EventTrigger_Socket::write);
	}

	EventTrigger_Socket* Socket_Impl::create_exception_trigger()
	{
		return new EventTrigger_Socket(this, EventTrigger_Socket::exception);
	}

	void Socket_Impl::disconnect()
	{
		if (sock != -1)
		{
			closesocket(sock);
		}

		sock = -1;
		valid = false;
	}

	//===========================================================================/////////
	// Socket_Impl implementation:

} // namespace
