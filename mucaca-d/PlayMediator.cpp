#include "PlayMediator.h"

TCHAR CPlayMediator::LOG_CATEGORY[] = "Mediator";

Packet CPlayMediator::start_game(nethost& host, Packet& packet)
{
	InputStream_Packet input(packet);
	string s;
	int major_v, minor_v;
	int player_index;

	packet.input >> s >> major_v >> minor_v >> player_index;
	logic::TTTField player = logic::TTTField(player_index);
	//TODO: implement some version comparison
	TCHAR buf[0x16] = { 0 }; size_t length = 0;
	Log::printf(LOG_CATEGORY, Log::info, "start_game: %s from %s", s.data(), host.address_as_text(buf, length));

	servers[host] = new logic();

	Packet p;
	p.output << "start";
	return  p;
}

Packet CPlayMediator::get_response(nethost& host, Packet& packet)
{
	int r, c, np;
	packet.input >> np >> c >> r;
	const SDL_Point move{ c, r };
	const logic::TTTField player = (np == 1) ? ONE : TWO;
	TCHAR hostname[0x16] = { 0 }; size_t length = 0;
	host.address_as_text(hostname, length);
	Log::printf(LOG_CATEGORY, Log::debug, "turn:{%d, %d} from %s", c, r, hostname);

	auto isrv = servers.find(host);
	Packet p;
	if (isrv != servers.end()) {
		if (isrv->second->Move(move, player))
		{
			const SDL_Point countermove = isrv->second->Countermove();
			const int rc = 0;
			p.output << rc;
			p.output << countermove.x << countermove.y;
		}
		else
		{
			Log::printf(LOG_CATEGORY, Log::warn, "cell already filled turn:{%d, %d} from %s", c, r, hostname);
			p.output << 1 << -1 << -1;
		}
	}
	else {
		Log::printf(LOG_CATEGORY, Log::warn, "host %s not connected", hostname);
		const int rc = -1;
		p.output << rc << -1 << -1;
	}
	return p;
}

Packet CPlayMediator::game_over(nethost& host, Packet& packet)
{
	//	size_t erased = servers.erase(host);
	TCHAR hostname[0x16] = { 0 }; size_t length = 0;
	host.address_as_text(hostname, length);
	Log::printf(LOG_CATEGORY, Log::info, "game over from %s", hostname);

	Packet p;
	auto isrv = servers.find(host);
	if (isrv != servers.end()) {
		logic* gplay = isrv->second;
		try {
			logic::TTTField winner;
			bitset2d<3, 3> winline;
			if(MOVE2 >= gplay->CheckWin(winline, gplay->ONE))
				gplay->CheckWin(winline, gplay->TWO);
			switch (gplay->Outcome()) {
			case ONE_WIN:
				winner = ONE;
				break;
			case TWO_WIN:
				winner = TWO;
				break;
			case DRAW:
				winner = EMPTY;
				Log::printf(LOG_CATEGORY, Log::info, "DRAW from host %s", hostname);
			default:
				winner = EMPTY;
				Log::printf(LOG_CATEGORY, Log::info, "QUIT from host %s", hostname);
			}

			if (EMPTY != winner)
				Log::printf(LOG_CATEGORY, Log::info, "player [%d] from host %s WIN!", winner, hostname);
			Log::printf(LOG_CATEGORY, Log::info, "final matrix:\n%s", gplay->MatrixAsString().c_str());
			p.output << gplay->Outcome();
		}
		catch (...) {
			Log::log(LOG_CATEGORY, Log::error, "Unknown error");
		}
		gplay->Clear();
	}
	else
	{
		Log::printf(LOG_CATEGORY, Log::warn, "host %s not connected", hostname);
		const int rc = -1;
		p.output << rc << -1 << -1;
	}
	return p;
}

void CPlayMediator::quit_all()
{
	Log::printf(LOG_CATEGORY, Log::info, "server to be terminated; quit all");
	for (auto it = servers.begin(); it != servers.end(); ++it)
	{
		Packet quitpack;
		quitpack.output << 0 << "server terminated";
		it->first.send("quit", quitpack);
	}
}