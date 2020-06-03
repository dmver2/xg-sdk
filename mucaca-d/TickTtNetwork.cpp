#include "StdAfx.h"
#include "TickTtNetwork.h"

const TCHAR CTickTtNetwork::S_NAME[] = "Tic-Tac-Toe";

CTickTtNetwork::CTickTtNetwork(void) : m_net(new NetInterface()),
m_log(Log2File::get_instance(S_NAME))
{
	Network_Setup::startup();
	m_net->set_debug(true);
	m_slot_connect = m_net->sig_host_connected().connect(this, &CTickTtNetwork::host_attached).set_persistent(true);
	m_slot_reconnect = m_net->sig_host_reconnected().connect(this, &CTickTtNetwork::host_reconnected).set_persistent(true);
	m_slot_start = m_net->sig_packet_received(_T("start")).connect(this, &CTickTtNetwork::on_start).set_persistent(true);
	m_slot_turn = m_net->sig_packet_received(_T("turn")).connect(this, &CTickTtNetwork::on_turn).set_persistent(true);
	m_slot_quit = m_net->sig_packet_received(_T("quit")).connect(this, &CTickTtNetwork::on_quit).set_persistent(true);
	m_slot_disconnected = m_net->sig_host_disconnected().connect(this, &CTickTtNetwork::on_disconnected);
	m_log_slot = Log::connect(m_log.get(), &Log2File::log);
}

CTickTtNetwork::~CTickTtNetwork(void)
{
	delete m_net;
	Network_Setup::shutdown();
}

void CTickTtNetwork::start_controller()
{
	m_net->start_server(_T("2008"));
	Log::log(S_NAME, "controller started");
}

//void CTickTtNetwork::turn(Packet p, const Host& h)
//{
//}

void CTickTtNetwork::quit_all()
{
	Log::log(S_NAME, "graceful shutdown");
	m_mediator.quit_all();
//	m_net->disconnect_all();
	m_net->stop();
}

int CTickTtNetwork::process_messages()
{
	static int i;
	TCHAR tmp[0xC] = { 0 };
	Log::printf(S_NAME, Log::debug, "#%d", i++);
	return 1;
}

void CTickTtNetwork::host_attached(const Host& host)
{
	TCHAR buf[0x16] = { 0 }; size_t length = 0;
	Log::printf(S_NAME, Log::debug, "attached: %s", host.address_as_text(buf, length));
}

void CTickTtNetwork::host_reconnected(const Host& host)
{
	TCHAR buf[0x16] = { 0 }; size_t length = 0;
	Log::printf(S_NAME, Log::debug, "reconnected: %s", host.address_as_text(buf, length));
}

void CTickTtNetwork::on_disconnected(const Host& host)
{
	TCHAR buf[0x16] = { 0 }; size_t length = 0;
	Log::printf(S_NAME, Log::debug, "detached: %s", host.address_as_text(buf, length));
}


void CTickTtNetwork::on_start(Packet& p, const Host& h)
{
	Log::log(S_NAME, "start");
	h.send("start", m_mediator.start_game(h, p));
}

void CTickTtNetwork::on_turn(Packet& p, const Host& h)
{
	Log::log(S_NAME, "turn");
	h.send("turn", m_mediator.get_response(h, p));
}

void CTickTtNetwork::on_quit(Packet& p, const Host& h)
{
	Log::log(S_NAME, "game over");
	Packet reply = m_mediator.game_over(h, p);
	if (reply.get_size())
		h.send("quit", reply);
}
