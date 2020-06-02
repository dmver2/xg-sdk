/*  $Id: log.h,v 1.1 2005/11/14 17:07:58 dmitry.vereshchagin Exp $
**
**  Game SDK
**
*/

#ifndef header_log_65B5E559_7FED_44FE_8FAE_2632D98D6370
#define header_log_65B5E559_7FED_44FE_8FAE_2632D98D6370

#if _MSC_VER > 1000
#pragma once
#endif

#include "tchar.h"
#include "signals.h"
#include "decl6n.h"
#include "synchronization.h"

#pragma warning (disable : 4251)

namespace GSDK
{
/// 
/**	
 *	
 *	@see Log2File
 */
class CxxLibExport Log
{
	DECLARE_NO_INSTANCE(Log);
	DECLARE_NO_COPY(Log);

public:
	typedef Slot_v3<const TCHAR*, int, const TCHAR*> SLOT;
	typedef Signal_v3<const TCHAR*, int, const TCHAR*> SIGNAL;

	/// 
	enum SEVERITY
	{
			debug		//<	
		,	info		//<	
		,	warn		//< 
		,	error		//< 
		,	fatal		//< 
	};

public:
	/// 
	/**	
	 *	
	 *	@param sz_group 
	 *	@param sz_message 
	 */
	static void log(const TCHAR *sz_group, const TCHAR *sz_message);

	/// 
	/** 
	 *	@param sz_group	
	 *	@param severity	
	 *	@param sz_message	
	 */
	static void log(const TCHAR *sz_group, SEVERITY severity, const TCHAR *sz_message);

	/// 
	/** 
	 *	
	 *	
	 *	@param sz_group	
	 *	@param severity	
	 *	@param sz_format	
	 *	@param ... 
	 */
	static void printf(const TCHAR *sz_group, SEVERITY severity, const TCHAR* sz_format, ...);
	
	/// 
	/** 
	 *	
	 *	
	 *	@param szgroup	
	 *	@param severity	
	 *	@param szformat	
	 *	@param argptr	
	 */
	static void vprintf(const TCHAR *szgroup, SEVERITY severity, const TCHAR* szformat, va_list argptr);
	
	/// 
	/**	
	 *	@param slot 
	 *	@return 
	 */
	static SigSlot connect(SLOT *slot);
	
	/// 
	/**	
	 *	@param callback 
	 *	@return 
	 */
	static SigSlot connect(void (*callback)(const TCHAR*, int, const TCHAR*));

	/// 
	/**	
	 *	@param functor 
	 *	@return 
	 */
	template<class Functor>
	static SigSlot connect_functor(const Functor &functor)
	{
		return connect(new FunctorSlot_v3<Functor, const TCHAR*, int, const TCHAR*>(functor));
	}

	/// 
	/**	
	 *	@param handler 
	 *	@param method 
	 *	@return 
	 */
	template <class Handler>
	static SigSlot connect(
		Handler *handler,
		void (Handler::*method)(const TCHAR*, int, const TCHAR*))
	{
		return connect(new MethodSlot_v3<Handler, const TCHAR*, int, const TCHAR*>(handler, method));
	}

	/// 
	/**	
	 *	@param handler 
	 *	@param method 
	 *	@param user_data 
	 *	@return 
	 */
	template <class Handler, class Data>
	static SigSlot connect(
		Handler *handler,
		void (Handler::*method)(const TCHAR*, int, const TCHAR*, Data),
		Data user_data)
	{
		return connect(new UserDataMethodSlot_v3<Handler, const TCHAR*, int, const TCHAR*, Data>(handler, method, user_data));
	}

	/// 
	/**
	 *	@param slot_to_disconnect 
	 */
	static void disconnect(SigSlot &slot_to_disconnect);

// Implementation:
private:
	static SIGNAL	s_signal;
	static Mutex	m_mutex;
};

} // namespace

#endif	//	#ifndef header_log_65B5E559_7FED_44FE_8FAE_2632D98D6370
