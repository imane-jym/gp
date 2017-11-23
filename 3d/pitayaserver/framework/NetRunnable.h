#ifndef _NET_RUNNABLE_H
#define _NET_RUNNABLE_H
#include "zthread/ZThread.h"
#include "Server.h"

class CNetRunnable: public ZThread::Runnable
{
public:
	void run();
private:
	CServer m_server;
};

#endif
