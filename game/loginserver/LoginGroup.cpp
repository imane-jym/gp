
#include "LoginGroup.h"
#include "UserSession.h"
#include "Log.h"
#include "DatabaseMysql.h"
#include "QueryResult.h"
#include "Field.h"
#include "miniini/miniini.h"
#include "Handler.h"
#include "EnumDefines.h"
#include "CenterDBCtrl.h"

CLoginGroup * CLoginGroup::m_pInstance = NULL;
//volatile bool CWorld::m_stopEvent = false;

extern DatabaseMysql LoginDatabase;        ///< Accessor to the login database
//extern DatabaseMysql CharacterDatabase;        ///< Accessor to the character database
extern DatabaseMysql OperateDatabase;       ///< Accessor to the Operation database
extern std::vector<std::string> g_vecAd;
extern int g_serverId;

CLoginGroup::CLoginGroup()
{
	m_mapTestAccounts.clear();
	m_mapServerStatus.clear();
}
CLoginGroup::~CLoginGroup()
{



}
CLoginGroup * CLoginGroup::GetInstance()
{
	if (!m_pInstance)
	{
		m_pInstance = new CLoginGroup();
	}
	return m_pInstance;
}
//void CLoginGroup::AddSession(CUserSession *pSession)
//{
//	m_addSessQueue.add(pSession);
//}
//void CLoginGroup::_AddSession(CUserSession *pSession)
//{
//	assert(pSession);
////	printf("new session added~\n");	
//	m_sessions[pSession->GetNetId()] = pSession;
//}

bool CLoginGroup::InitSettings()
{
//	m_timers[WUPDATE_PINGDB].SetInterval(5 * 60 * 1000);
//	m_timers[WUPDATE_CHECK_ALIVE].SetInterval(60 * 1000);
	
	if (!CenterDBCtrl::InitCenterDB(&OperateDatabase, (uint8)g_serverId))
	{
		IME_ERROR("operate db init fail");
		return false;
	}
	CenterDBCtrl::GetNoticeByType((uint32)E_NOTICE_USE_TYPE_LOGIN, g_vecAd);
	OnTimer1min();
	return true;
}
//void CLoginGroup::Update(time_t diff)
//{
//	
//    ///- Update the different timers
//    for (int i = 0; i < WUPDATE_COUNT; ++i)
//    {
//        if (m_timers[i].GetCurrent() >= 0)
//            m_timers[i].Update(diff);
//        else
//            m_timers[i].SetCurrent(0);
//    }
//
//    ///- Ping to keep MySQL connections alive
//    if (m_timers[WUPDATE_PINGDB].Passed())
//    {
//        m_timers[WUPDATE_PINGDB].Reset();
//        //todo
//        IME_LOG("Ping MySQL to keep connection alive");
//        LoginDatabase.Ping();
////      CAreaConf::Ping();
//    }
//
//    if (m_timers[WUPDATE_CHECK_ALIVE].Passed())
//    {
//        m_timers[WUPDATE_CHECK_ALIVE].Reset();
//        OnTimer1min();
//    }
//
//	UpdateSessions(diff);
//
//}

//void CLoginGroup::UpdateSessions(time_t diff)
//{
//	//add new sessions
//	CUserSession *sess;
//	while(!m_addSessQueue.empty())
//	{
//		sess = m_addSessQueue.next();
//		_AddSession(sess);
//	}
//	SessionMap::iterator itr = m_sessions.begin();
//	SessionMap::iterator next;
//	for(; itr != m_sessions.end(); itr = next)
//	{
//		next = itr;
//		++next;
//		CUserSession *pSession = itr->second;
//		if (!pSession->Update(diff))
//		{
//			//todo
//			m_sessions.erase(itr);
//			pSession->Offline();
//			delete pSession;
//		}
//	}
//}
//
//void CLoginGroup::AddUserInfo( uint32 dwAccountId, CUserSession* pSess )
//{
//	m_mapUserInfo[dwAccountId] = pSess;
//}
//
//void CLoginGroup::DelUserInfo( uint32 dwAccountId )
//{
//	m_mapUserInfo.erase(dwAccountId);
//}
//
//void CLoginGroup::ClearUserInfo( CUserSession* pSess )
//{
//	MapUserInfo::iterator it = m_mapUserInfo.begin();
//	for ( ; it != m_mapUserInfo.end(); )
//	{
//		if (it->second == pSess)
//		{
//			m_mapUserInfo.erase(it++);
//		}
//		else
//		{
//			++it;
//		}
//	}
//}
//
//CUserSession* CLoginGroup::FindUserSess( uint32 dwAccountId )
//{
//	MapUserInfo::iterator it = m_mapUserInfo.find(dwAccountId);
//	return it == m_mapUserInfo.end() ? NULL : it->second;
//}

void CLoginGroup::OnTimer1min()
{
	//check alive

	QueryResult *result = LoginDatabase.PQuery(
			"select area_id, "
			"ip, port, version, update_time, can_login, status, is_new, server_name, domain, res_url "
			"from gameserver_info order by area_id");
	if (result)
	{
		//取所有服务器列表
		uint64 u64Cnt = result->GetRowCount();
		for(uint64 i = 0; i < u64Cnt; ++i)
		{
			Field *fields = result->Fetch();
			STC_SERVER_STATUS stcServer;
			stcServer.wAreaId = fields[0].GetUInt16();
			stcServer.strIp = fields[1].GetCppString();
			stcServer.dwPort = fields[2].GetUInt32();
			stcServer.dwVersion = fields[3].GetUInt32();
			uint32 dwTickTime = fields[4].GetUInt32();
			stcServer.byCanLogin = fields[5].GetUInt8();
			stcServer.byStatus = fields[6].GetUInt8();
			stcServer.byIsNew = fields[7].GetUInt8();
			stcServer.strServerName = fields[8].GetCppString();
			stcServer.strDomain = fields[9].GetCppString();
			stcServer.strResUrl = fields[10].GetCppString();
			int nCanLogin = stcServer.byCanLogin;
			std::map<uint16,STC_SERVER_STATUS>::iterator ii = m_mapServerStatus.find(stcServer.wAreaId);
			IME_LOG("areaid %u ip %s port %u servername %s", stcServer.wAreaId, stcServer.strIp.c_str(), stcServer.dwPort, stcServer.strServerName.c_str());
			if (ii != m_mapServerStatus.end())
			{
				//有 判断
//				if (dwTickTime != ii->second.dwLastTickTime)
//					ii->second.bIsAlive = true;
//				else
//					ii->second.bIsAlive = false;
//				ii->second.dwLastTickTime = dwTickTime;
				if (dwTickTime != ii->second.dwLastTickTime)
					stcServer.bIsAlive = true;
				else
					stcServer.bIsAlive = false;
				stcServer.dwLastTickTime = dwTickTime;
				ii->second = stcServer;
			}
			else
			{
				//没有
				stcServer.dwLastTickTime = dwTickTime;
				stcServer.bIsAlive = true;
				m_mapServerStatus[stcServer.wAreaId] = stcServer;
			}
			result->NextRow();
		}
	}
	
//	// check alive connect
//	SessionMap::iterator itr = m_sessions.begin();
//	SessionMap::iterator next;
//	for(; itr != m_sessions.end(); itr = next)
//	{
//		next = itr;
//		++next;
//		CLoginSession *pSession = itr->second;
//		if (pSession->GetdwActiveTime() + CLIENT_ACTIVE_TIME <= time(NULL))
//		{
//			IME_LOG("timeout delete user session");
//			m_sessions.erase(itr);
//			delete pSession;
//		}
//	}

//	std::map<uint16,STC_SERVER_STATUS>::iterator ii = m_mapServerStatus.begin();
//	for(;ii != m_mapServerStatus.end(); ++ii)
//	{
//		IME_ERROR("AreaId %d, ServerId %d IsAlive %d",ii->second.wAreaId,ii->second.wServerId,ii->second.bIsAlive);
//	}

	CenterDBCtrl::GetNoticeByType((uint32)E_NOTICE_USE_TYPE_LOGIN, g_vecAd);
}
bool CLoginGroup::IsAlive(uint16 wAreaId, uint16 wServerId)
{
	//只用serverID
	std::map<uint16,STC_SERVER_STATUS>::iterator ii = m_mapServerStatus.find(wServerId);
	if (ii != m_mapServerStatus.end())
	{
		return ii->second.bIsAlive;
	}
	return false;
}
//bool CLoginGroup::IsTestAccount(std::string &strAccount)
//{
//	std::map<std::string,int>::iterator ii = m_mapTestAccounts.find(strAccount);
//	return ii != m_mapTestAccounts.end();
//}
//void CLoginGroup::LoadTestAccount(INISection * configPath)
//{
//	m_mapTestAccounts.clear();
//	std::vector<std::string> vecAccount;
//	vecAccount.clear();
//	if (!configPath->ReadStrings("test_account",vecAccount))
//	{
//		IME_ERROR("Missing test_account info");
//		return ;
//	}
//	for(int i = 0; i < vecAccount.size(); ++i)
//	{
//		m_mapTestAccounts[vecAccount[i]] = 1;
//	}
//}
