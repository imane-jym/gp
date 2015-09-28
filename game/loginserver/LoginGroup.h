#ifndef _LOGIN_GROUP_H
#define _LOGIN_GROUP_H

#include "Defines.h"
#include "zthread/ZThread.h"
#include "zthread/FastMutex.h"
#include "zthread/LockedQueue.h"
#include "zthread/Thread.h"
#include "Timer.h"
#include <map>

class CUserSession;
class INISection;

class CLoginGroup
{
public:

	/// Timers for different object refresh rates
	enum WorldTimers
	{
	    WUPDATE_PINGDB,
	    WUPDATE_CHECK_ALIVE,
	    WUPDATE_COUNT
	};

	typedef struct _STC_SERVER_STATUS
	{
		bool bIsAlive;
		uint16 wAreaId;
		std::string strIp;
		uint32 dwPort;
		uint32 dwVersion;
		uint32 dwLastTickTime;
		uint8 byCanLogin;
		uint8 byStatus;
		uint8 byIsNew;
		std::string strServerName;
		std::string strDomain;
		std::string strResUrl;
		_STC_SERVER_STATUS():bIsAlive(false),wAreaId(0),dwPort(0),dwVersion(0),dwLastTickTime(0),byCanLogin(0),byStatus(0),byIsNew(0)
		{

		}
	}STC_SERVER_STATUS;

	CLoginGroup();
	~CLoginGroup();
	static CLoginGroup * GetInstance();
	typedef std::map<uint32, CUserSession *> SessionMap;
	typedef std::map<uint32, CUserSession*> MapUserInfo;

//	void AddSession(CUserSession *pSession);
//	void Update(time_t diff);
//	void UpdateSessions(time_t diff);
	bool InitSettings();
	
//	void AddUserInfo(uint32 dwAccountId, CUserSession* pSess);
//	void DelUserInfo(uint32 dwAccountId);
//	void ClearUserInfo(CUserSession* pSess);
//	CUserSession* FindUserSess(uint32 dwAccountId);

	void OnTimer1min();

	bool IsAlive(uint16 wAreaId, uint16 wServerId);

	std::map<uint16, STC_SERVER_STATUS>& GetServerList()
	{
		return m_mapServerStatus;
	}

//	bool IsTestAccount(std::string &strAccount);
//
//	void LoadTestAccount(INISection * configPath);
//public:
	//static volatile bool m_stopEvent;

//private:
//	void _AddSession(CUserSession *pSession);
private:
//	ZThread::LockedQueue<CUserSession*, ZThread::FastMutex> m_addSessQueue;
//	SessionMap m_sessions;
	static CLoginGroup * m_pInstance;

//	IntervalTimer m_timers[WUPDATE_COUNT];
//
//	MapUserInfo m_mapUserInfo;	

	std::map<uint16,STC_SERVER_STATUS> m_mapServerStatus;

	std::map<std::string,int> m_mapTestAccounts;
};
//todo singleton
#define sLoginGroup (CLoginGroup::GetInstance())

#endif

