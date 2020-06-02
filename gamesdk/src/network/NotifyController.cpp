/* $Id: NotifyController.cpp,v 1.1 2005/11/14 17:07:59 dmitry.vereshchagin Exp $
**
*/

#include <network/packet.h>
#include <network/IPAddress.h>
#include "notifycontroller.h"

namespace GSDK 
{

NotifyController::NotifyController(void)
{
}

NotifyController::~NotifyController(void)
{
}

bool NotifyController::store_sent_packet(Packet &packet)
{
	CHECKSUM checksum = crc(packet.get_data(), packet.get_size());
	auto pair = m_packets.insert(MAP_CRC2PACKET::value_type(checksum, packet));
	return pair.second;
}

bool NotifyController::on_ack(Packet &packet)
{
	CHECKSUM checksum = packet.input.read_uint32();
	MAP_CRC2PACKET::size_type erased = m_packets.erase(checksum);
	//MAP_CRC2PACKET::iterator it = m_packets.find(checksum);
	//if(it != m_packets.end())
	return erased != 0;
}

void NotifyController::run()
{
}

NotifyController::CHECKSUM NotifyController::crc(void* pv_data, int len)
{
	return 0xdeadbeef;
}

}