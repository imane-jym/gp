

#ifndef _PLAYER_SESSION_H
#define _PLAYER_SESSION_H


#include "zthread/ZThread.h"
#include "zthread/FastMutex.h"
#include "zthread/LockedQueue.h"
#include "zthread/Runnable.h"
#include "zthread/Thread.h"
#include "WorldPacket.h"
#include <map>
#include "Defines.h"
#include "Timer.h"
#include "UserSession.h"
#include "Handler.h"
#include "NetClient.h"

//class CNetClient;

class CPlayerSession : public CUserSession
{

public:
	/// Timers for different object refresh rates
	enum UserTimers
	{
		USER_UPDATE_TIMER_1s,
		USER_UPDATE_TIMER_3s,
		USER_UPDATE_TIMER_5s,
		USER_UPDATE_TIMER_30s,
		USER_UPDATE_TIMER_1min,
		USER_UPDATE_TIMER_3min,
		USER_UPDATE_TIMER_5min,
		USER_UPDATE_TIMER_10min,
		USER_UPDATE_TIMER_30min,
	    USER_UPDATE_COUNT
	};

	enum KickType
	{
		KICK_NOT_ALIVE // connect is not alive so long
	};

	CPlayerSession(ZThread::CountedPtr<CNetClient> ptrNetClient);
	virtual ~CPlayerSession();

	virtual bool Update(time_t diff);

	virtual void AddSessionToWorker();
	virtual void Offline();
	void OnTimer1s();
	void OnTimer3s();
	void OnTimer5s();
	void OnTimer30s();
	void OnTimer1min();
	void OnTimer3min();
	void OnTimer5min();
	void OnTimer10min();
	void OnTimer30min();

	uint32 GetdwActiveTime(){return m_dwActiveTime;}
	void KickSession(CNetClient::CloseAsyncFlagType e);
	void HandlerKick(uint32_t type);

private:
	IntervalTimer m_timers[USER_UPDATE_COUNT];
	uint32 m_dwActiveTime;
	CHandlerPlayersession m_CHandler;
};
#endif
