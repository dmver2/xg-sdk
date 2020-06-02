/*	$Id: Network_Setup.cpp,v 1.2 2005/11/17 16:36:20 dmitry.vereshchagin Exp $
**
** Network_Setup implementation
*/
#include "network/network_setup.h"
#include "exception.h"
#include "network/socket.h"
#include "socket_impl.h"
#include "socket_selector.h"
#include "event_trigger_socket.h"
#include <log.h>

namespace GSDK
{

volatile int Network_Setup::s_ref_count = 0;
WSADATA Network_Setup::wsa_data = {};


void Network_Setup::startup(void) throw (Exception)
{
	if (::InterlockedCompareExchange((long*)&s_ref_count, 1, 0))
		return;

	WORD wVersionRequested;
	wVersionRequested = MAKEWORD( 2, 2 );
	int err = ::WSAStartup( wVersionRequested, &wsa_data );
	if (err != 0) {
		string errmsg(_T("Network_Setup::startup. WSAStartup failed. ") + Socket::get_error_message());
		Log::log("Network_Setup", Log::error, errmsg.data());
		throw Exception(errmsg.data());
	}
	
	Socket_Impl::sp_socket_selector = new SocketSelector();
}

bool Network_Setup::up()
{
	return ::InterlockedAnd((long*)&s_ref_count, 1);
}

void Network_Setup::shutdown(void)
{
	if (::InterlockedCompareExchange((long*)&s_ref_count, -1, 1))
		return;

	delete Socket_Impl::sp_socket_selector;
	Socket_Impl::sp_socket_selector = 0;
	::WSACleanup();
}

} // namespace GSDK