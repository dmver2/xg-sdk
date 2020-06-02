/* $Id: System_Impl.cpp,v 1.1 2005/11/14 17:07:58 dmitry.vereshchagin Exp $
**  System_Impl 
*/

#include "synchronization.h"
#include ".\system_impl.h"

namespace GSDK
{
	int64	System_Impl::s_delta = 0;
	Mutex	System_Impl::s_mutex;
	Signal_v1<int64> System_Impl::s_sig_network_time_adjusted;
} // namespace
