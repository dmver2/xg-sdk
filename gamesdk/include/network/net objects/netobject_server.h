/*  $Id: netobject_server.h,v 1.1 2005/11/14 17:07:58 dmitry.vereshchagin Exp $
**	Game SDK
**
**
*/

#ifndef header_netobject_server
#define header_netobject_server

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
	class NetObject_Server_Impl;
	class Packet;
	class NetObject_Controller;
	class Host;
	class HostGroup;

/// 
/**	NetObject_Server 
 *	
 *	
 *	
 */
class CxxLibExport NetObject_Server
{
// 
public:
	/// 
	/**	
	 *	
	 *	@param controller	
	 */	
	NetObject_Server(NetObject_Controller *controller);

	/// 
	/** 
	 *	@param copy	
	 */
	NetObject_Server(const NetObject_Server &copy);

	/// 
	/// 
	virtual ~NetObject_Server();

// 
public:
	/// 
	/// @return 
	int get_id() const;

// 
public:
	/// 
	/**	
	 *	@param group 
	 *	@param msg_type 
	 *	@param message	
	 */
	void send(HostGroup &group, int msg_type, const Packet &message);

	/// 
	/**	
	 *	@param host 
	 *	@param msg_type 
	 *	@param message	
	 */
	void send(const Host &host, int msg_type, const Packet &message);

	/// 
	/**	
	 *	@param msg_type 
	 *	@param message	
	 */
	void send_all(int msg_type, const Packet &message);

// 
public:
	/// 
	/**	
	 *	@param msg_type 
	 *	@return 
	 *	
	 *	Host &host - 
	 *	Packet &packet - 
	 */
	Signal_v2<const Host &, Packet &> &sig_msg_received(int msg_type);

// 
private:
	/// 
	NetObject_Server_Impl *p_impl;
};

}	// namespace

#endif
