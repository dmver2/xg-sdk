/*  $Id: event_trigger_socket.h,v 1.1 2005/11/14 17:07:59 dmitry.vereshchagin Exp $
**	Game SDK
**
**
*/

#ifndef header_event_trigger_socket
#define header_event_trigger_socket

#if _MSC_VER > 1000
#pragma once
#endif

#include "event_trigger.h"

namespace GSDK
{

class SocketSelector;
class Socket_Impl;

///	
/**	
 *	
 *	
 *	@see Socket
 */
class EventTrigger_Socket : public EventTrigger
{
public:
/// 
	enum Type
	{
		read,		///< 
		write,		///< 
		exception	///< 
	};

//	
public:
	/// 
	/**	
	 *	@param m_psock_impl 
	 *	@param type 
	 */
	EventTrigger_Socket(Socket_Impl *m_psock_impl, Type type);

	/// 
	/// 
	~EventTrigger_Socket();

//	
public:
	///	
	/// @return 
	Socket_Impl *get_socket() const { return m_psock_impl; }

	/// 
	/// @return Type 
	Type get_type() const { return type; }

// 
public:
	/// 
	/// 
	void start_listen();

// 
private:
	/// 
	Socket_Impl *m_psock_impl;
	/// 
	Type type;
};

} // namespace
#endif
