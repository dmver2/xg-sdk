/* GameInfo interface
*
*/

#ifndef header_gameinfo
#define header_gameinfo

#if _MSC_VER > 1000
#pragma once
#endif

#include <string>
#include <vector>
#include <tchar.h>
#include "version.h"
#include "decl6n.h"

namespace Domain
{
class GameSession;
class GameInfo_Impl;

/// 
class CxxLibExport GameInfo
{
public:
	/// 
	class CxxLibExport SessionEnum 
	{
	public:
		/// 
	    /// 
		virtual void operator()(GameSession&) = 0;
	};

// ctor
	/// 
	GameInfo(class GameInfo_Impl* impl);

public:
// copy ctor
	/// 
	GameInfo(const GameInfo &copy);

public:
// dtor
	virtual ~GameInfo(void);

//operators
	GameInfo& operator= (const GameInfo &other);

	bool operator == (const GameInfo &other_instance) const;

	bool operator != (const GameInfo &other_instance) const;

	bool operator < (const GameInfo &other_instance) const;

public:
	/// 
	/// 
	///	
	/**
	 *	@param	name	
	 *	@param	proto	
	 *	@param	version	
	 *	@return	
	 */
	static GameInfo create(const string &name
				, const string &proto
				, const version &version );

	/// 
	/**
	 *	@return 
	 *	
	 */
	static GameInfo get_instance();

	///	
	/** 
	 *	
	 *	
	 *	@return	
	 */
	GameSession new_session();

	///	
	///	@return 
	GameSession get_session(unsigned int session_id) const;

	///	
	/** 
	 *	@param session	
	 */
	void close_session(GameSession session);

	/// 
	/** 
	 *	@param functor	
	 *	@return 
	 */
	GameInfo::SessionEnum &for_each_session(GameInfo::SessionEnum &functor) const;

	/// 
	/**	
	*	@param sessions 
	*	
	*	@return 
	*/
	std::vector<GameSession> &get_sessions(/*out*/ std::vector<GameSession> &sessions) const;

	///	
	///	@return	
	string get_name() const;

	///	
	///	@return	
	string get_protocol() const;

	/// 
	///	@return 
	version get_version() const;

	/// 
	///	@return 
	string get_version_str() const;
    
// data members
private: 

	GameInfo_Impl *m_impl;

public:

	static GameInfo null;

};


} //namespace Domain

#endif
