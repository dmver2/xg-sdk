/*	$Id: GamePortal.cpp,v 1.2 2005/11/17 16:36:20 dmitry.vereshchagin Exp $
**
**
*/

#include <gameportal.h>
#include <gameserveraddress.h>
#include "gameportal_impl.h"
//#include "types.h"

namespace Domain
{

GamePortal::GamePortal(IN const TCHAR *sz_uri)
	: m_impl(new GamePortal_Impl(sz_uri))
{
}

GamePortal::~GamePortal(void)
{
}

void GamePortal::get_servers_by_game_name(IN const TCHAR *sz_game_name, OUT std::vector<GameServerAddress> &r_addresses)
{
	m_impl->get_servers_by_game_name(sz_game_name, r_addresses);
}

} // namespace