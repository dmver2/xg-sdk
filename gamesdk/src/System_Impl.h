/* $Id: System_Impl.h,v 1.1 2005/11/14 17:07:58 dmitry.vereshchagin Exp $
**  System_Impl 
*/

#ifndef _header_system_impl_
#define _header_system_impl_

#if _MSC_VER > 1000
#pragma once
#endif

#pragma warning (disable : 4251)

#include <string>
#include "types.h"
#include "decl6n.h"
#include "signals.h"

namespace GSDK
{
class Mutex;

class System_Impl
{
	DECLARE_NO_INSTANCE(System_Impl)
	DECLARE_NO_COPY(System_Impl)

public:

	///	смещение сетевого времени
	static int64 s_delta;

	/// синхронизатор доступа к статическим данным
	static Mutex  s_mutex;

	/// сигнал для подключеия обработчика синхронизации сетевого времени
	static Signal_v1<int64> s_sig_network_time_adjusted;

};

} // namespace

#endif // #ifndef _header_system_impl_
