/*	$Id: IGamePortal.h,v 1.1 2005/11/10 10:01:09 dmitry.vereshchagin Exp $
**
**
*/

#pragma once
#include <decl6n.h>
#include <Exception.h>
#include <vector>
#include <GameServerAddress.h>
#include <igameportal.h>


namespace Domain
{
	/// 
	/*abstract*/ class IGamePortal
	{
	public:
		virtual void get_servers_by_game_name(IN const TCHAR* sz_game_name
			, OUT std::vector<GameServerAddress>& addresses) throw (GSDK::Exception) = 0;
		virtual ~IGamePortal() {}
	};
}
