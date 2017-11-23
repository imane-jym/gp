

#ifndef _SESSCTRL_H
#define _SESSCTRL_H
#include "Defines.h"
#include "zthread/ZThread.h"
#include "zthread/FastMutex.h"
#include "zthread/LockedQueue.h"
#include "zthread/Thread.h"
#include "Timer.h"
#include <map>
#include <vector>
#include "WorldPacket.h"

#define MAKE_FIND_GROUPID(userid,groupid) ((uint64_t)userid << 32 | groupid)

class CUserSession;
class CRelayClientSession;

class CSessCtrl
{
public:

	/// Timers for different object refresh rates
	enum WorldTimers
	{
		WUPDATE_1s,
		WUPDATE_3s,
		WUPDATE_5s,
		WUPDATE_15s,
		WUPDATE_30s,
	    WUPDATE_1min,
	    WUPDATE_3min,
	    WUPDATE_5min,
		WUPDATE_10min,
		WUPDATE_30min,
	    WUPDATE_COUNT
	};
	
	CSessCtrl();
	~CSessCtrl();
	
//	void Init();

	static CSessCtrl * GetInstance();
	typedef std::map<uint32, CUserSession *> SessionMap;			// map<AccountId , ... >
	void AddSession(CUserSession *pSession);
	void Update(time_t diff);
	void UpdateSessions(time_t diff);
	bool InitSettings();

	void UpdateRelayClient(time_t diff);
//	CRelayClientSession * GetRelayClientSession(int key);
	void AddRelayClientSession(CRelayClientSession *pRelay);

	void AllOffline();

	bool SendToRelay(WorldPacket& pkg, unsigned int key);

	void SendPkgToAll(WorldPacket& pkg);
	void SendPkgToPart( WorldPacket& pkg, uint32_t off, uint32_t number);

	void Update1min();
	void Update3min();
	void Update5min();
	void Update10min();
	void Update30min();

	void Update3s();
	void Update1s();
	void Update5s();
	void Update15s();
	void Update30s();

	uint32 GetOnlineNum();

private:
	void _AddSession(CUserSession *pSession);
private:
	ZThread::LockedQueue<CUserSession*, ZThread::FastMutex> m_addSessQueue;
	SessionMap m_sessions;
	static CSessCtrl * m_pInstance;

	IntervalTimer m_timers[WUPDATE_COUNT];

	std::vector<CRelayClientSession *> m_vecRelayClientSession;
	uint32 m_logtime;
};
//todo singleton
#define sSessCtrl (CSessCtrl::GetInstance())
#endif

