/* $Id: NotifyController.h,v 1.1 2005/11/14 17:07:59 dmitry.vereshchagin Exp $
**
*/
#include <decl6n.h>
#include <unordered_map>

#pragma once

namespace GSDK 
{
class Packet;
class IPAddress;
class Host;

class NotifyController
{
public:
	NotifyController(void);
	virtual ~NotifyController(void);
	
	bool store_sent_packet(Packet &packet);
	bool on_ack(Packet &packet);
	void run();

private:
	typedef uint CHECKSUM;
	typedef std::unordered_map<CHECKSUM, Packet> MAP_CRC2PACKET;
	MAP_CRC2PACKET m_packets;

	static CHECKSUM crc(void* pv_data, int len);

};

} // namespace