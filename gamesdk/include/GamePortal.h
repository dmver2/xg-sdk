/*	$Id: GamePortal.h,v 1.1 2005/11/14 17:07:58 dmitry.vereshchagin Exp $
**	
*/

#pragma once
#pragma warning (disable : 4251)
#include <decl6n.h>
#include <vector>
#include <sharedptr.h>

namespace Domain
{
class GameServerAddress;
class GamePortal_Impl;

///	
class CxxLibExport GamePortal
{
public:
	/// 
	/**	
	 *	@param sz_end_point_uri URI 
	 */
	GamePortal(IN const TCHAR *sz_end_point_uri);

	/// 
	~GamePortal(void);

	/// 
	/**	
	 *	@param sz_game_name 
	 *	@param r_addresses	
	 */
	void get_servers_by_game_name(IN const TCHAR *sz_game_name
								, OUT std::vector<GameServerAddress> &r_addresses);

private:
	/// 
	GSDK::sharedptr<GamePortal_Impl> m_impl;
};

} // namespace