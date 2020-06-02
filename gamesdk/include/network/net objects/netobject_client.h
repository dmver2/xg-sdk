/*  $Id: netobject_client.h,v 1.1 2005/11/14 17:07:58 dmitry.vereshchagin Exp $
**	Game SDK
**
**
*/


#ifndef header_netobject_client
#define header_netobject_client

#if _MSC_VER > 1000
#pragma once
#endif

#ifdef _MSC_VER
#pragma warning(disable : 4786)
#endif

#include <string>
#include <signals.h>

namespace GSDK
{

class NetObject_Client_Impl;
class Packet;
class NetObject_Controller_Impl;
class Host;

/// 
/**	NetObject_Client 
*	
*	
*	
*/
class CxxLibExport NetObject_Client
{
// 
public:
	/// 
	/**	
	 *	@param obj_id 
	 *	@param server 
	 *	@param controller 
	 */
	NetObject_Client(int obj_id, const Host &server, NetObject_Controller_Impl *controller);

	/// 
	/** 
	*	@param copy	
	*/
	NetObject_Client(const NetObject_Client &copy);

	/// 
	/// 
	virtual ~NetObject_Client();

// 
public:
	/// 
	/// @return 
	int get_id() const;

	/// 
	/// @return 
	const Host &get_server() const;

// 
public:
	/// 
	/**	
	 *	@param msg_type 
	 *	@param message	
	 */
	void send(int msg_type, const Packet &message);

// 
public:
	/// 
	/**	
	 *	
	 *	@param msg_type 
	 *	@return 
	 *	
	 *	Packet &packet - 
	 */
	Signal_v1<Packet &> &sig_msg_received(int msg_type);

// 
private:
	/// 
	NetObject_Client_Impl *p_impl;
};

} // namespace
#endif
