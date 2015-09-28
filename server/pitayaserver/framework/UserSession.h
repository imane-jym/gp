#ifndef _USER_SESSION_H
#define _USER_SESSION_H


#include "zthread/ZThread.h"
#include "zthread/FastMutex.h"
#include "zthread/LockedQueue.h"
#include "zthread/Runnable.h"
#include "zthread/Thread.h"
#include "WorldPacket.h"
#include <map>
#include "Defines.h"
#include "NetClient.h"


class CUserSession
{

public:

	CUserSession();
	CUserSession(ZThread::CountedPtr<CNetClient> ptrNetClient);
	virtual ~CUserSession();

	virtual void QueuePacket(WorldPacket &packet);
	virtual void SendPacket(WorldPacket * packet);
	virtual bool Update(time_t diff) = 0;
	void SetNetId(uint32 dwNetId) { m_dwNetId = dwNetId;}
	uint32 GetNetId() { return m_dwNetId;}
	std::string GetRemoteIP();
	void CloseSocket();

	virtual void AddSessionToWorker() {};
//	virtual void Online();
	virtual void Offline();

protected:
	//CNetClient * m_pNetClient;
	ZThread::CountedPtr<CNetClient> m_ptrNetClient;
	uint32 m_dwNetId;
	ZThread::LockedQueue<WorldPacket*,ZThread::FastMutex> m_recvQueue;

	std::string m_strIP;
};
#endif
