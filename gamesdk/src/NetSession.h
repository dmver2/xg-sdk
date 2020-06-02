/*	$Id: NetSession.h,v 1.2 2005/11/17 16:36:20 dmitry.vereshchagin Exp $
**
**
*/

#pragma once

#include <winsock2.h>
#include <string>
#include <map>
#include <list>
#include <queue>
#include "thread.h"
#include "synchronization.h"
//#include "mtqueue.h"

namespace GSDK {

class NetSession
{
public:
	NetSession(void);
	virtual ~NetSession(void);
	void start(const string& in_port);
	void stop();
	void shutdown();
	void listen_thread();
//	void send_thread();
	void query_handler(const SOCKADDR_IN &from, int bytes_available, char *buffer);

//	typedef std::pair<IPAddress, string> IPMessagePair;
//	typedef std::pair<Packet, Host> PacketHostPair;

private:
	SOCKET m_sock;

	class WorkThread : public Thread {
	public:
		WorkThread(NetSession* s) 
			:	m_s(s)
		{
		};

		virtual  int main ()
		{
			m_s->listen_thread();
			return 0;
		};


	private:
		NetSession *m_s;
	};

	WorkThread *m_work_thread;

	Event	m_event;

	static const TCHAR s_sz_topic[];
};

} //namespace GSDK
