#pragma once
#include "PlayMediator.h"
#include <api.h>
#include <network.h>
#include <sharedptr.h>

using namespace GSDK;

class CTickTtNetwork
{
public:
	typedef sharedptr<Log2File>	splog;
	static const TCHAR S_NAME[];

	CTickTtNetwork();
public:
	~CTickTtNetwork();

	void start_controller();
	//void quit();
	//void turn();
	void quit_all();
	int process_messages();

private:
	NetInterface* m_net;
	splog m_log;
	SigSlot m_slot_start;
	SigSlot m_slot_turn;
	SigSlot m_slot_quit;
	SigSlot m_log_slot;
	SigSlot m_slot_connect;
	SigSlot m_slot_reconnect;
	SigSlot m_slot_disconnected;
	CPlayMediator m_mediator;

	void host_attached(const Host&);
	void host_reconnected(const Host& host);
	void on_disconnected(const Host& host);
	void on_start(Packet&, const Host&);
	void on_turn(Packet&, const Host&);
	void on_quit(Packet&, const Host&);
};
