/*	$Id: GamePortal_Impl.h,v 1.2 2005/11/17 16:36:20 dmitry.vereshchagin Exp $
**
**
*/

#pragma once
#include <decl6n.h>
#include <Exception.h>
#include <vector>
#include <GameServerAddress.h>
#include <igameportal.h>
//#include "soap\gameservice3.hpp"

namespace Domain
{
	class GamePortal_Impl : public IGamePortal
	{
	public:
		GamePortal_Impl(IN const TCHAR* sz_end_point_uri);
		virtual ~GamePortal_Impl();
		void get_servers_by_game_name(IN const TCHAR* sz_game_name
			, OUT std::vector<GameServerAddress>& addresses) throw (GSDK::Exception) override;
	private:
		
	};

} // namespace