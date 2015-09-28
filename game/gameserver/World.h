

#ifndef _WORLD_H
#define _WORLD_H
#include "Defines.h"
#include "zthread/ZThread.h"
#include "zthread/FastMutex.h"
#include "zthread/LockedQueue.h"
#include "zthread/Thread.h"
#include "Timer.h"
#include "stdint.h"
#include "WorldBoss.h"
#include "CommonType.h"
#include "SessCtrl.h"
#include <map>
#include <vector>

#define MAKE_FIND_GROUPID(userid,groupid) ((uint64_t)userid << 32 | groupid)

//class CUserSession;
//class CRelayClientSession;
class CRole;
class CHandlerPlayersession;
class WorldPacket;
class CGroup;
class CNotice;
class CAnnouncement;
class CRobot;

class CWorld
{
public:

	/// Timers for different object refresh rates
	enum WorldTimers
	{
	    WUPDATE_PINGDB,
		WUPDATE_30min,
		WUPDATE_LOGIN,
		WUPDATE_10min,
		WUPDATE_1s,
		WUPDATE_CHARGE,
	    WUPDATE_COUNT
	};
	
	CWorld();
	~CWorld();
	
//	void Init();

	//CWorldInfo* GetCArena() { return m_pArena; }

	static CWorld * GetInstance();
//	typedef std::map<uint32, CUserSession *> SessionMap;			// map<AccountId , ... >
	typedef std::map<uint32, CHandlerPlayersession*> MapSessionByUserId;	// map<AccountId , ... > ͨ
//	void AddSession(CUserSession *pSession);
//	void Update(time_t diff);
//	void UpdateSessions(time_t diff);
	bool InitSettings();

//	void UpdateRelayClient(time_t diff);
//	CRelayClientSession * GetRelayClientSession() { return m_pRelayClientSession;}

	void AddUserAuth(uint32 dwUserId, CHandlerPlayersession* pSession);
	void DelUserAuth(uint32 dwUserId);
	CRole* FindUserRole(uint32 dwUserId);
	CHandlerPlayersession* FindSess(uint32 dwUserId);
	MapSessionByUserId& GetUserMap() { return m_mapUserAuth; }
	void AllOffline();

//	bool SendToRelay(WorldPacket& pkg);
//
//	void SendPkgToAll(WorldPacket& pkg);
//	void SendPkgToPart( WorldPacket& pkg, uint32_t off, uint32_t number);

	void Update1min();
	void Update10min();
	void Update30min();

	void UpdateLoginInfo();
	void Update1s();
	void UpdateCharge();

	void SetIp(std::string &strIp);
	void SetPort(int nPort);
	void SetServerId(std::string strServerId);
//	uint32 GetServerId() { return m_dwServerId;}
	bool IsTarget(uint32_t serverid)
	{
		return (find(m_vecServerId.begin(), m_vecServerId.end(), serverid) != m_vecServerId.end());
	}
	std::vector<uint32_t> GetServerId()
	{
		return m_vecServerId;
	}

	uint32 GetOnlineNum();

	void MsgAll(std::string &strMsg);

	bool ReloadConfig(const std::string& strConfigName);

	void GetRoleList(std::vector<CRole*>& vecRole);


//	void FlushAllUserRank();
//	void FlushAllUserPrestige();
	uint32_t GetRankCDTime();
//	uint32_t GetPrestigeTime()
//	{return m_PrestigeTime;};
//	uint32_t GetSortRank(uint32 userid)
//	{
//		if (m_ArenaUser.find(userid) != m_ArenaUser.end())
//		{
//			return m_ArenaUser[userid];
//		}
//		return 0;
//	}

	CGroup *GetCGroup(uint64_t groupid);
	CGroup *GetCGroupByUserId(uint32_t userid);

	int AddCGroup(uint64_t groupid, CGroup *p);
	void SaveAllData();

	void GroupAuto10(std::vector<CGroup *> &vec, uint32 off)
	{
		std::map<uint64_t, CGroup *>::iterator it;
		int i = 0, j = 10;
		for (it = m_mapGroup.begin(); it != m_mapGroup.end(); it++)
		{
			if (i >= off && j > 0)
			{
				vec.push_back(it->second);
				j--;
			}
			i++;
		}
	}
	void GroupSearchByName(std::vector<CGroup *> &vec, const std::string &name);
	void HandlerGetGroupList(CRole *pCRole, WorldPacket &pkg);
	void HandlerGroupRankSearch(CRole *pCRole, WorldPacket &pkg);
	void HandlerGetGroupDetail(CRole *pCRole, WorldPacket &pkg);
	void HandlerGroupCancel(CRole *pCRole, WorldPacket &pkg);

	void AddWorldChat(const SWorldChat &tmp);
	void HandlerWorldChatInfo(CRole *pCRole);
	void PkgWorldChat(SWorldChat tmp, WorldPacket &info);

	bool IsWhiteList(uint32_t userid)
	{
		return std::find(m_vecWhiteList.begin(), m_vecWhiteList.end(), userid) != m_vecWhiteList.end() ? true : false;
	}
	void SetWhiteList(std::vector<uint32_t> vec)
	{
		m_vecWhiteList = vec;
	}

//	void HandlerSysBroadContent(std::string str);
	CNotice *GetCNotice() {return m_pCNotice;}
	CAnnouncement *GetCAnnouncement() {return m_pCAnnouncement;}
	CObjCtrl *GetWorldBoss() {return m_pWorldBoss;}

	std::map<uint32_t, SServerData>& GetServerList() {return m_mapServer;}

	bool IsExists(std::string username)
	{
		return (m_setAllUserName.find(username) != m_setAllUserName.end());
	}
	void DelUserName(std::string username)
	{
		m_setAllUserName.erase(username);
	}
	void InsertUserName(std::string username)
	{
		m_setAllUserName.insert(username);
	}

	bool IsAllowDrop(uint32_t index, uint32_t number);
	void AddDropItem(uint32_t index, uint32_t number);

	std::map<uint32_t, CRobot>& GetRobRobot() { return m_mapRobRobot; };

	//获取塔的排行榜
	std::vector<STowerRank>* getRank();
//public:
	//static volatile bool m_stopEvent;

//private:
//	void _AddSession(CUserSession *pSession);
private:
//	ZThread::LockedQueue<CUserSession*, ZThread::FastMutex> m_addSessQueue;
//	SessionMap m_sessions;
	static CWorld * m_pInstance;

	IntervalTimer m_timers[WUPDATE_COUNT];

//	CRelayClientSession * m_pRelayClientSession;
	MapSessionByUserId m_mapUserAuth;

	std::string m_strIp;
	int m_nPort;
	std::vector<uint32_t> m_vecServerId;
	std::map<uint32, SServerData> m_mapServer;

//	uint32 m_dwServerId;
//	uint8_t m_CanLogin;
//	uint8_t m_Status;
//	uint8_t m_IsNew;
//	std::string m_strServerName;
	
	uint32 m_RankTime;
//	uint32 m_RankTimeLimit;
//	uint32 m_PrestigeTime;
//
//	std::map<uint32_t, uint32_t> m_ArenaUser;

	std::map<uint64_t, CGroup *> m_mapGroup;
	CNotice *m_pCNotice;
	CAnnouncement *m_pCAnnouncement;
	CObjCtrl *m_pWorldBoss;

	std::list<SWorldChat> m_lstWorldBroadCast;
	std::vector<uint32_t> m_vecWhiteList;

	std::set<std::string> m_setAllUserName;
	std::map<uint32_t, uint32_t> m_mapGlobalDropCount;
	uint32_t m_GlobalDropTime;

	std::map<uint32_t, CRobot> m_mapRobRobot;

	std::vector<STowerRank> rank;
	time_t rowerRankTime;
	time_t addRobotTime;
};
//todo singleton
#define sWorld (CWorld::GetInstance())
#endif

