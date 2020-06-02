/*	$Id: GamePortal_Impl.cpp,v 1.2 2005/11/17 16:36:20 dmitry.vereshchagin Exp $
**
**
*/

#include "GamePortal_Impl.h"
#include <atlbase.h>

namespace Domain
{

	GamePortal_Impl::GamePortal_Impl(IN const TCHAR* sz_end_point_uri)
	{
	}

	GamePortal_Impl::~GamePortal_Impl()
	{
	}

	void GamePortal_Impl::get_servers_by_game_name(
		IN const TCHAR* sz_game_name, OUT std::vector<GameServerAddress>& addresses) throw (GSDK::Exception)
	{
		for(int i=0; i < 3; ++i)
		{
			const GameServerAddress* s = new GameServerAddress("localhost", 8080);
			addresses.push_back(*s);
		}

	}

} // namespace