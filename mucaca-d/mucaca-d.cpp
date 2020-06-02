// mucaca-d.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <tchar.h>
#include "TickTtNetwork.h"
#include "mucaca-d.h"

const TCHAR TOPIC[] = _T("STDOUT");

struct game_flow {

private:
	CTickTtNetwork& m_g;
	volatile bool terminated;

public:
	game_flow(CTickTtNetwork& g) : m_g(g), terminated(0)
	{
	}

	~game_flow() {
		m_g.quit_all();
	}

	CTickTtNetwork* operator -> ()
	{
		return &m_g;
	}
};

class StdHandler
{
public:
	static const TCHAR* s_severity[];
	static const TCHAR* severity_text(Log::SEVERITY severity, TCHAR* buf);

	StdHandler(std::basic_ostream<TCHAR>& c);

	void log(const TCHAR* topic, int severity, const TCHAR* msg);

private:
	std::basic_ostream<TCHAR>& m_out;
};

const TCHAR* StdHandler::s_severity[] = { _T("debug"), _T("info"), _T("warn"), _T("error"), _T("fatal") };

StdHandler::StdHandler(std::basic_ostream<TCHAR>& c) : m_out(c)
{
}

const TCHAR* StdHandler::severity_text(Log::SEVERITY severity, TCHAR tmp[32])
{
	return (0 <= severity && severity < ARRAYSIZE(s_severity)) ? s_severity[severity] : _itoa(severity, tmp, 10);
}

void StdHandler::log(const TCHAR* topic, int severity, const TCHAR* msg)
{
	TCHAR tmp[32] = { 0 };
	const TCHAR* text = severity_text(static_cast<Log::SEVERITY>(severity), tmp);
	this->m_out << topic << _T('\t') << text << _T('\t') << msg << std::endl;
}

class StdoutFunctor
{
public:
	void operator() (const TCHAR* topic, int severity, const TCHAR* msg)
	{
		TCHAR tmp[32] = { 0 };
		const TCHAR* text = StdHandler::severity_text(static_cast<Log::SEVERITY>(severity), tmp);
		int n = 0;
		const TCHAR* fmt = (msg && 0 != (n = _tcslen(msg)) && '\n' != msg[n - 1])
			? _T("%s\t[%s]\t%s\n") : _T("%s\t[%s]\t%s");

		printf_s(fmt, topic, text, msg);
	}
};

Event g_stop_event;

BOOL CtrlHandler(DWORD ctrl)
{
	switch (ctrl)
	{
	case CTRL_C_EVENT:
		g_stop_event.signal();
		return TRUE;

	default:
		return FALSE;
	}
}

#undef main

int _tmain(int argc, const TCHAR* argv[])
{
	::SetConsoleCtrlHandler((PHANDLER_ROUTINE)CtrlHandler, TRUE);
	//    StdHandler handler(std::cout);
	//    SigSlot logSlot = Log::connect(&handler, &StdHandler::log);
	const SigSlot logSlot2 = Log::connect_functor(StdoutFunctor());

	Log::log(TOPIC, _T("Enter"));
	{
		CTickTtNetwork ttn;
//		game_flow g(ttn);
		ttn.start_controller();
		g_stop_event.wait();
		ttn.quit_all();
	}
	Log::printf(TOPIC, Log::info, _T("Quit"));
	return 0;
}
