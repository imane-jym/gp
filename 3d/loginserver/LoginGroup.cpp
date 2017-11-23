#include "LoginGroup.h"
#include "UserSession.h"
#include "Log.h"
#include "DatabaseMysql.h"
#include "QueryResult.h"
#include "Field.h"
#include "miniini/miniini.h"
#include "Util.h"
#include "md5.h"

extern DatabaseMysql loginDatabase;        ///< Accessor to the login database
extern uint8 g_serverId;

using namespace CenterDB;

CLoginGroup::CLoginGroup()
{
	m_ClearGmTime = 0;
	m_mapServerStatus.clear();
}
CLoginGroup::~CLoginGroup()
{

}
CLoginGroup * CLoginGroup::GetInstance()
{
	static CLoginGroup instance;
	return &instance;
}

bool CLoginGroup::InitSettings()
{
//	m_timers[WUPDATE_PINGDB].SetInterval(5 * 60 * 1000);
//	m_timers[WUPDATE_CHECK_ALIVE].SetInterval(15 * 1000);
//	m_timers[WUPDATE_CHECK_GM_CMD].SetInterval(5 * 1000);
//	m_timers[WUPDATE_1sec].SetInterval(1* 1000);
//
//	// 清理帐号的GM权限，每日凌晨1点清理
//	m_timers[WUPDATE_CLEAR_GM_AUTH].SetInterval(5* 60 * 1000);

	InitLoginDB();
	OnTimer15s();
	return true;
}

bool CLoginGroup::InitLoginDB()
{
	bool ret = CenterDBCtrl::InitCenterDB( &loginDatabase,g_serverId );

	if ( ret )
	{
		CenterDBCtrl::InitGmCommand();
		return true;
	}
	return false;
}

void CLoginGroup::OnTimer15s()
{
	IME_LOG( "Check Alive" );

	CenterDBCtrl::GetOrUpdateGameServerStatus( m_mapServerStatus );

	// debug
	IME_LOG( "%u game server...", (uint32)m_mapServerStatus.size() );
//	for ( ServerStatusContainer::iterator it = m_mapServerStatus.begin();
//			it != m_mapServerStatus.end(); ++it )
//	{
//		IME_LOG( "\t server_id[%d] server_name[%s] ip[%s] port[%d] online_num[%d] can_login[%d] alive[%d] stra[%d]", it->second.dwServerId,
//				it->second.strServerName.c_str(), it->second.strIp.c_str(), it->second.dwPort, it->second.dwOnlineNum,
//				it->second.byCanLogin, it->second.bIsAlive, it->second.dwLoginStrategy );
//	}
}

void CLoginGroup::OnTimer5s()
{
	CenterDBCtrl::ReadNewGmCommand();
	CenterDBCtrl::UpdateGmCommand();
}

void CLoginGroup::OnCheckClearGmAuth()
{
	uint32 dwSecond = CUtil::GetSecondFrom0Clock();

	// 在 1:00 ~ 1:10 之间的，并且今天没清理过
	if(dwSecond >= LOGIN_CLEAR_GM_TIME &&
	   dwSecond <= LOGIN_CLEAR_GM_TIME + 600 &&
	   false == CUtil::IsToday(m_ClearGmTime))
	{
		//CenterDBCtrl::ClearPassportGmAuth();
		//CenterDBCtrl::ClearRoleGmAuth();

		m_ClearGmTime = time(NULL);

		//IME_LOG("OnCheckClearGmAuth end");
	}
}

void CLoginGroup::OnTimer1s()
{
//	sLog->CheckLogFileChange();
//	sLog->LogFileChange();
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
