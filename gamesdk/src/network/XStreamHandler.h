/* $Id: XStreamHandler.h,v 1.1 2005/11/14 17:07:59 dmitry.vereshchagin Exp $
**
**
*/

#pragma once
#include <network/netinterface.h>

namespace GSDK
{

class XStreamHandler
{
public:
	XStreamHandler(NetInterface_Impl *pnetifc);
	~XStreamHandler(void);
	void handle_packet(Packet packet, IPAddress& addr );
	void query_handler(const SOCKADDR_IN &addr, int bytes_available, char *buffer);

private:
	NetInterface_Impl *m_pnetwork;
	static const char s_sz_topic[];
};

} // namespace