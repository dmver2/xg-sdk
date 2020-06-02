/*  $Id: netobject_controller.h,v 1.1 2005/11/14 17:07:58 dmitry.vereshchagin Exp $
** Game SDK
**
*/

#ifndef header_netobject_controller
#define header_netobject_controller

#if _MSC_VER > 1000
#pragma once
#endif

#ifdef _MSC_VER
#pragma warning(disable : 4786)
#endif

#include <string>
#include <signals.h>
#include <network/host.h>
#include <network/packet.h>

namespace GSDK
{

class NetObject_Controller_Impl;
class HostGroup;
class NetObject_Client;

/// 
/** 
 *	
 *	
 *	
 */
class CxxLibExport NetObject_Controller
{
// 
public:
	/// 
	/**
	 *
	 */
	NetObject_Controller(class NetInterface *netinterface, const string &channel);

	/// 
	/** 
	*	@param copy	
	*/
	NetObject_Controller(const NetObject_Controller &copy);

	/// 
	virtual ~NetObject_Controller();

// 
public:

// 
public:
	/// 
	/// @param copy 
	NetObject_Controller &operator =(const NetObject_Controller &copy);

public:
	/// 
	/** 
	 *	
	 *	
	 *	
	 *	@return 
	 *	
	 *	NetObject_Client &r_created_netobject - 
	 *	int msg_type - 
	 *	Packet &packet - 
	 */
	Signal_v3<NetObject_Client &, int, Packet &> &sig_create_object();

// 
public:
	/// 
	NetObject_Controller_Impl *m_impl;
};

}	// namespace

#endif
