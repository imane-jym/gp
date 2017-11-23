/*
 * RelayClientSession.h
 *
 *  Created on: 2011-7-13
 *      Author: yq
 */

#ifndef RELAYCLIENTSESSION_H_
#define RELAYCLIENTSESSION_H_

#include "UserSession.h"
#include "Defines.h"
#include "Timer.h"
#include "Handler.h"
#include <map>

class WorldPacket;
class CClient;

class CRelayClientSession : public CUserSession
{
public:
	/// Timers for different object refresh rates
	enum RelayTimers
	{
		USER_UPDATE_TIMER_1s,
		USER_UPDATE_TIMER_3s,
		USER_UPDATE_TIMER_5s,
		USER_UPDATE_TIMER_1min,
		USER_UPDATE_TIMER_10min,
		USER_UPDATE_TIMER_30min,
	    USER_UPDATE_COUNT
	};

	typedef void (CRelayClientSession::*OpcodeHandlerFunc)(WorldPacket &packet);
	typedef std::map<uint16, OpcodeHandlerFunc> OpcodeHandlerMap;

	CRelayClientSession();
	virtual ~CRelayClientSession();

	void SetClient(CClient *pClient) { m_pClient = pClient;};
	void SetdwKey(int key) { m_dwKey = key;}
	unsigned int GetdwKey() { return m_dwKey;}

//	void GetdwActiveTime() { return m_dwActiveTime; }

	virtual void SendPacket(WorldPacket * packet);
	virtual bool Update(time_t diff);

	virtual void AddSessionToWorker();
	virtual void Offline();

private:
	CClient * m_pClient;
	unsigned int m_dwKey;
//	uint32_t m_dwActiveTime;

	IntervalTimer m_timers[USER_UPDATE_COUNT];
	CHandlerClientSession m_CHandler;
};
#endif /* RELAYCLIENTSESSION_H_ */
