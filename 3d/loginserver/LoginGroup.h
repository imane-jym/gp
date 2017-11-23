#ifndef _LOGIN_GROUP_H
#define _LOGIN_GROUP_H

#include "Defines.h"
//#include "zthread/ZThread.h"
#include "zthread/FastMutex.h"
#include "zthread/LockedQueue.h"
//#include "zthread/Thread.h"
#include "Timer.h"
#include "CenterDBCtrl.h"
#include <map>

class CUserSession;
class INISection;

class CLoginGroup
{
public:

	// 每日1点清理GM权限
	const static uint32 LOGIN_CLEAR_GM_TIME = 3600;

	typedef std::map<uint32,CenterDB::STC_SERVER_STATUS> ServerStatusContainer;

	CLoginGroup();
	~CLoginGroup();
	static CLoginGroup * GetInstance();

	bool InitSettings();
	bool InitLoginDB();
	
	void OnTimer15s();
	void OnTimer5s();
	void OnTimer1s();

	void OnCheckClearGmAuth();

	const ServerStatusContainer& GetAllServerStatus() { return m_mapServerStatus; }

private:
	ServerStatusContainer	m_mapServerStatus;

	uint32 m_ClearGmTime;
};


#define sLoginGroup (CLoginGroup::GetInstance())

#endif

