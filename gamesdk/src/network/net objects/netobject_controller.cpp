/*  $Id: netobject_controller.cpp,v 1.1 2005/11/14 17:07:59 dmitry.vereshchagin Exp $
**	Game SDK
**
**
*/

#include <network/net objects/netobject_controller.h>
#include "netobject_controller_impl.h"

namespace GSDK
{

//===========================================================================
// CL_NetObjectController construction:

NetObject_Controller::NetObject_Controller(NetInterface *netinterface, const string &channel)
: m_impl(0)
{
	m_impl = new NetObject_Controller_Impl(netinterface, channel);
}

NetObject_Controller::NetObject_Controller(const NetObject_Controller &copy)
: m_impl(copy.m_impl)
{
	if (m_impl) m_impl->add_ref();
}

NetObject_Controller::~NetObject_Controller()
{
	if (m_impl) m_impl->release_ref();
}

//===========================================================================
// CL_NetObjectController attributes:

//===========================================================================
// NetObject_Controller operations:

NetObject_Controller &NetObject_Controller::operator =(const NetObject_Controller &copy)
{
	if (m_impl) m_impl->release_ref();
	m_impl = copy.m_impl;
	if (m_impl) m_impl->add_ref();
	return *this;
}

//===========================================================================
// NetObject_Controller signals:

Signal_v3<NetObject_Client &, int, Packet &> &NetObject_Controller::sig_create_object()
{
	return m_impl->m_sig_create_object;
}

//===========================================================================
// NetObject_Controller implementation:
}
