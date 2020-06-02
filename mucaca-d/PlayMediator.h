#pragma once

#include <api.h>
#include <network.h>

#include "../game_logic/framework.h"
#include "../game_logic/bitset2d.h"
#include <unordered_map>


using namespace std;
using namespace GSDK;

#define MOVE1 CGameLogic<3>::TTTOutcome::MOVE1
#define MOVE2 CGameLogic<3>::TTTOutcome::MOVE2
#define ONE_WIN CGameLogic<3>::TTTOutcome::ONE_WIN
#define TWO_WIN CGameLogic<3>::TTTOutcome::TWO_WIN
#define DRAW CGameLogic<3>::TTTOutcome::DRAW

#define EMPTY  CGameLogic<3>::TTTField::EMPTY
#define ONE  CGameLogic<3>::TTTField::ONE
#define TWO CGameLogic<3>::TTTField::TWO


class CPlayMediator
{
	typedef CGameLogic<3> logic;
	typedef const Host nethost;

private:
	struct host_hash_comparator : public std::hash_compare<Host>
	{
		size_t operator()(const Host& keyval) const
		{
			return keyval.hash();
		}

	};

	unordered_map<nethost, logic*, host_hash_comparator> servers;

public:
	static TCHAR LOG_CATEGORY[];

	Packet start_game(nethost& host, Packet& packet);
	Packet get_response(nethost& host, Packet& packet);
	Packet game_over(nethost& host, Packet& packet);
	void quit_all();
};

