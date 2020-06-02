/*  $Id: socket.cpp,v 1.2 2005/12/09 08:37:07 dmitry.vereshchagin Exp $
** Game SDK
**
**
*/

#ifdef _MSC_VER
#pragma warning (disable:4786)
#endif

#include "socket_impl.h"
#include "netexception.h"
#include <log.h>
#include <WS2tcpip.h>
#include <Mstcpip.h>

namespace GSDK
{

#pragma comment(lib, "ws2_32.lib")
#define SIO_UDP_CONNRESET _WSAIOW(IOC_VENDOR, 12)

	TCHAR Socket::LOGTOPIC[] = "SOCKET";

	/////////////////////////////////////////////////////////////////////////////
	// Socket construction:

	Socket::Socket()
		: m_impl(0)
	{
	}

	Socket::Socket(int socket)
		: m_impl(new Socket_Impl)
	{
		m_impl->add_ref();
		m_impl->sock = socket;

		input.socket = m_impl;
		output.socket = m_impl;
	}

	Socket::Socket(Type type, Protocol protocol)
		: m_impl(new Socket_Impl)
	{
		m_impl->add_ref();

		int domain;
		int in_type = SOCK_DGRAM, in_proto = 0; //IPPROTO_IP

		switch (type)
		{
		case tcp: in_type = SOCK_STREAM;	break;
		case udp: in_type = SOCK_DGRAM;		break;
		case raw: in_type = SOCK_RAW;
			in_proto = IPPROTO_ICMP;		break;
		}

		switch (protocol)
		{
		case ipv4:
			domain = PF_INET;
			break;
		case ipv6:
			domain = PF_INET6;
			break;
		default:
			throw Exception(_T("Socket::Socket failed: Invalid protocol specified"));
			domain = 0;
			break;
		}

		m_impl->sock = socket(domain, in_type, in_proto);
		m_impl->domain = domain;
		m_impl->protocol = in_proto;
		m_proto = type;
		if (m_impl->sock < 0)
			throw Exception(string(_T("Socket::Socket failed: ")).append(Socket_Impl::get_error_message()).c_str());

		input.socket = m_impl;
		output.socket = m_impl;
	}

	Socket::Socket(const Socket& copy)
		: m_impl(copy.m_impl)
	{
		if (m_impl) m_impl->add_ref();
		input.socket = m_impl;
		output.socket = m_impl;
	}

	Socket::~Socket()
	{
		if (m_impl) m_impl->release_ref();
	}

	/////////////////////////////////////////////////////////////////////////////
	// Socket attributes:

	int Socket::get_socket() const
	{
		return m_impl->sock;
	}

	int Socket::get_max_msg_size() const
	{
		unsigned int udp_max_len = 0;
		int opt_len = sizeof(udp_max_len);
		int rc = ::getsockopt(m_impl->sock, SOL_SOCKET, SO_MAX_MSG_SIZE, (char*)&udp_max_len, &opt_len);
		// No error for tcp socket returned rc=0 and udp_max_len=-1
		return udp_max_len;
	}

	EventTrigger* Socket::get_read_trigger() const
	{
		if (m_impl->read == NULL)
			m_impl->read = m_impl->create_read_trigger();
		return m_impl->read;
	}

	EventTrigger* Socket::get_write_trigger() const
	{
		if (m_impl->write == NULL)
			m_impl->write = m_impl->create_write_trigger();
		return m_impl->write;
	}

	EventTrigger* Socket::get_exception_trigger() const
	{
		if (m_impl->exception == NULL)
			m_impl->exception = m_impl->create_exception_trigger();
		return m_impl->exception;
	}

	IPAddress Socket::get_source_address() const
	{
		//TODO:
		// Fix this garbage
		// IPv4 specific hack

		sockaddr_in addr_in;
		memset(&addr_in, 0, sizeof(addr_in));
		addr_in.sin_family = AF_INET;

		int size = sizeof(sockaddr_in);

		int result = getsockname(m_impl->sock, (sockaddr*)&addr_in, (socklen_t*)&size);
		if (result != 0) throw Exception(_T("Socket::get_source_address() failed."));

		return Socket_Impl::create_ip_address(addr_in);
	}

	IPAddress Socket::get_dest_address() const
	{
		//TODO:
		// Fix this garbage
		// IPv4 specific hack

		sockaddr_in addr_in;
		memset(&addr_in, 0, sizeof(addr_in));
		addr_in.sin_family = AF_INET;

		int size = sizeof(sockaddr_in);

		int result = getpeername(m_impl->sock, (sockaddr*)&addr_in, (socklen_t*)&size);
		if (result != 0) throw Exception(_T("Socket::get_dest_address() failed."));

		return Socket_Impl::create_ip_address(addr_in);
	}

	/*
	std::list<IPAddress> Socket::get_broadcast_addresses(const string &port) const
	{
	// Should be IPv6 safe

		std::list<IPAddress> broadcast_addresses;

		// if not a IPv4 socket or not a UDP socket
		if(m_impl->domain != PF_INET)
			throw Exception("Socket::get_broadcast_addresses() failed. Broadcasting is for IPv4 only");
		if(m_proto != udp)
			throw Exception("Socket::get_broadcast_addresses() failed. Broadcasting is for UDP only");

		// win32 seems to always use 255.255.255.255 as its broadcast address
		broadcast_addresses.push_back(IPAddress("255.255.255.255", port));
		return broadcast_addresses;
	}
	*/

	/////////////////////////////////////////////////////////////////////////////
	// Socket operations:

	Socket& Socket::operator =(const Socket& other)
	{
		if (this != &other)
		{
			if (m_impl) m_impl->release_ref();
			m_impl = other.m_impl;
			if (m_impl) m_impl->add_ref();
			input.socket = m_impl;
			output.socket = m_impl;
		}
		return *this;
	}

	void Socket::set_nonblocking(bool nonblocking)
	{
		u_long argp = nonblocking ? 1 : 0;
		ioctlsocket(m_impl->sock, FIONBIO, &argp);
	}

	void Socket::set_nodelay(bool nodelay)
	{
		//	static int sol_tcp = -1;
		//	if (sol_tcp == -1) sol_tcp = getprotobyname("tcp")->p_proto;

		int value = nodelay ? 1 : 0;
		setsockopt(m_impl->sock, IPPROTO_TCP, TCP_NODELAY, (const char*)&value, sizeof(int));
	}

	void Socket::set_broadcasting(bool broadcasting)
	{
		// if not a IPv4 socket or not a UDP socket
		if (m_impl->domain != PF_INET)
			throw Exception("Socket::set_broadcasting() failed. Broadcasting is for IPv4 only");
		if (m_proto != udp)
			throw Exception("Socket::set_broadcasting() failed. Broadcasting is for UDP only");

		int on = broadcasting ? 1 : 0;
		setsockopt(m_impl->sock, SOL_SOCKET, SO_BROADCAST, (const char*)&on, sizeof(on));
	}

	int Socket::send(const void* data, int size)
	{
		int result = ::send(
			m_impl->sock,
			static_cast<const char*>(data),
			size,
			0);
		if (result < 0) throw Exception(string(_T("Socket::send failed. ")).append(Socket_Impl::get_error_message()).data());

		if (m_impl->write) m_impl->write->start_listen();
		return result;
	}

	int Socket::send(const void* data, int size, const IPAddress& dest)
	{
		sockaddr addr;
		int len;

		dest.get_addrinfo(m_proto, addr, len, m_impl->domain);

		int result = ::sendto(
			m_impl->sock,
			(const char*)data,
			size,
			0,
			&addr,
			len);
		if (result < 0) throw Exception(string(_T("Socket::send failed. ")).operator+=(Socket_Impl::get_error_message()).data());

		if (m_impl->write) m_impl->write->start_listen();
		return result;
	}

	int Socket::send(const string& msg)
	{
		return send(msg.c_str(), msg.size());
	}

	int Socket::recv(void* data, int size)
	{
		int result = ::recv(m_impl->sock, (char*)data, size, 0);
		if (result < 0)
		{
			int err = ::WSAGetLastError();
			throw NetException((string(_T("Socket::recv failed. ")).append(Socket_Impl::get_error_message(err))).data(), err);
		}

		if (m_impl->read) m_impl->read->start_listen();
		return result;
	}

	/*
	void Socket::push(const string &string)
	{
		m_impl->push_stack.push(string);
	}
	*/

	int Socket::recv(void* data, int size, IPAddress& from)
	{
		char* _data = (char*)data;
		int total = 0;

		// Start filling data with pushed data (if any):
		while (!m_impl->push_stack.empty())
		{
			string& str = m_impl->push_stack.top();
			if (str.size() < static_cast<unsigned int>(size))
			{
				memcpy(_data, str.data(), str.size());
				_data += str.size();
				total += str.size();
				size -= str.size();
				m_impl->push_stack.pop();
			}
			else
			{
				memcpy(_data, str.data(), size);
				str = str.substr(size);
				total += size;
				return total;
			}
		}

		// Fill with data from network socket:
		sockaddr_in addr_in{};
		memset(&addr_in, 0, sizeof(addr_in));
		addr_in.sin_family = AF_INET;

		socklen_t addr_size = sizeof(addr_in);

		int result = ::recvfrom(
			m_impl->sock,
			_data,
			size,
			0,
			(sockaddr*)&addr_in,
			&addr_size);
		if (result < 0)
		{
			int err = ::WSAGetLastError();
			throw NetException((string(_T("Socket::recv failed. ")).append(Socket_Impl::get_error_message(err))).data(), err);
		}
		total += result;

		if (m_impl->read) m_impl->read->start_listen();
		from = Socket_Impl::create_ip_address(addr_in);
		return total;
	}

	void Socket::connect(const IPAddress& address) const
	{
		sockaddr addr{};
		int len = 0;

		address.get_addrinfo(m_proto, addr, len, m_impl->domain);

		int result = ::connect(m_impl->sock, &addr, len);
		if (result < 0)
		{
			throw Exception((string(_T("Socket::connect failed: ")).append(Socket_Impl::get_error_message())).data());
		}

		if (m_impl->read) m_impl->read->start_listen();
		if (m_impl->exception) m_impl->exception->start_listen();
	}

	void Socket::shutdown(ShutdownHow how)
	{
		int in_how = 0;
		switch (how)
		{
		case shutdown_receive: in_how = 0; break;
		case shutdown_send:    in_how = 1; break;
		}

		int result = ::shutdown(m_impl->sock, in_how);
		if (result < 0) throw Exception(_T("Socket::shutdown failed"));
	}

	void Socket::bind(const IPAddress& address)
	{
		sockaddr addr;
		int len;

		memset(&addr, 0, sizeof(addr));
		address.get_addrinfo(m_proto, addr, len, m_impl->domain);

		//	makes no sense for SOCK_DGRAM & IPPROTO_UDP
		//	int option = 1;
		//	setsockopt(m_impl->sock, SOL_SOCKET, SO_REUSEADDR, (const char *) &option, sizeof(option));

			// Set the exclusive address option
		int i_exclusive_val = 1;
		if (0 != setsockopt(m_impl->sock, SOL_SOCKET, SO_EXCLUSIVEADDRUSE,
			(char*)&i_exclusive_val, sizeof(i_exclusive_val)))
		{
			const int err = ::WSAGetLastError();
			Log::printf(LOGTOPIC, Log::error, _T("setsockopt for SO_EXCLUSIVEADDRUSE failed with error: %s (0x%08X)"), Socket_Impl::get_error_message(err), err);
		}

		BOOL b_new_behavior = FALSE;
		DWORD dw_bytes_rc = 0;
		if (0 != ::WSAIoctl(m_impl->sock, SIO_UDP_CONNRESET, &b_new_behavior, sizeof b_new_behavior, NULL, 0, &dw_bytes_rc, NULL, NULL))
		{
			const int err = ::WSAGetLastError();
			Log::printf(LOGTOPIC, Log::error, _T("WSAIoctl for SIO_UDP_CONNRESET failed with error: %s (0x%08X)"), Socket_Impl::get_error_message(err), err);
		}

		const int bound_rc = ::bind(m_impl->sock, &addr, len);
		if (bound_rc != 0) {
			const int err = ::WSAGetLastError();
			TCHAR tmp[0x16]; size_t n = 0; address.to_string(tmp, n);
			TCHAR szmsg[0x200];
			::_stprintf_s<0x200>(szmsg, _T("bind failed for %s with error: %s (0x%08X)"), tmp, Socket_Impl::get_error_message(err), err);
			Log::printf(LOGTOPIC, Log::error, szmsg);
			throw Exception(szmsg);
		}
	}

	void Socket::listen(int backlog /*=SOMAXCONN*/)
	{
		int result = ::listen(m_impl->sock, backlog);
		if (result < 0) throw Exception("Socket::listen failed");
	}

	Socket Socket::accept()
	{
		int new_sock = ::accept(m_impl->sock, NULL, NULL);
		if (new_sock == INVALID_SOCKET) throw Exception(_T("Socket::accept failed"));
		if (m_impl->read) m_impl->read->start_listen();
		return Socket(new_sock);
	}

	void Socket::close()
	{
		if (m_impl) m_impl->disconnect();
	}

	/////////////////////////////////////////////////////////////////////////////
	// Socket signals:

	/*
	Signal_v0 &Socket::sig_read_triggered()
	{
		if (m_impl->keep_alive_helper == 0) m_impl->keep_alive_helper = new Socket_Helper(m_impl);
		return m_impl->keep_alive_helper->sig_read_triggered;
	}

	Signal_v0 &Socket::sig_write_triggered()
	{
		if (m_impl->keep_alive_helper == 0) m_impl->keep_alive_helper = new Socket_Helper(m_impl);
		return m_impl->keep_alive_helper->sig_write_triggered;
	}

	Signal_v0 &Socket::sig_exception_triggered()
	{
		if (m_impl->keep_alive_helper == 0) m_impl->keep_alive_helper = new Socket_Helper(m_impl);
		return m_impl->keep_alive_helper->sig_exception_triggered;
	}

	Signal_v0 &Socket::sig_disconnected()
	{
		if (m_impl->keep_alive_helper == 0) m_impl->keep_alive_helper = new Socket_Helper(m_impl);
		return m_impl->keep_alive_helper->sig_disconnected;
	}
	*/

	/////////////////////////////////////////////////////////////////////////////
	// Socket implementation:

	Socket::Socket(Socket_Impl* impl)
		: m_impl(impl)
	{
		m_impl->add_ref();
		input.socket = m_impl;
		output.socket = m_impl;
	}

	string Socket::get_error_message()
	{
		return Socket_Impl::get_error_message();
	}

	string Socket::get_error_message(unsigned long code)
	{
		return Socket_Impl::get_error_message(code);
	}

} // namespace