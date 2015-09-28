#include <new>
#include "World.h"
#include "UserSession.h"
#include "ShardingDB.h"
#include "WorldDb.h"
#include "DatabaseMysql.h"
#include "Log.h"
#include "RelayClientSession.h"
#include "CmdInnerDefine.h"
#include "PlayerSession.h"
#include "Role.h"
#include "CmdDefine.h"
#include "ConfigData.h"
#include "DBCtrl.h"
#include "google/malloc_extension.h"

#include "ConfigData.h"
#include "GlobalConfig.h"
#include "Arena.h"
#include "Group.h"
#include "ErrorMsg.h"
#include "Notice.h"
#include "Other.h"
#include "CenterDBCtrl.h"
#include "GmCommandExt.h"
#include "CardPacket.h"
#include "EnumDefines.h"
#include "Mail.h"


//extern DatabaseMysql LoginDatabase;        ///< Accessor to the login database
extern CShardingDB WorldDb;
extern DatabaseMysql OperateDatabase;       ///< Accessor to the Operation database

extern std::string g_strConfigPath;


CWorld * CWorld::m_pInstance = NULL;
//volatile bool CWorld::m_stopEvent = false;

CWorld::CWorld()
{
//	m_pRelayClientSession = new CRelayClientSession;
}

CWorld::~CWorld()
{
//	if (m_pRelayClientSession)
//	{
//		delete m_pRelayClientSession;
//	}

//	std::map<uint64_t, CGroup *>::iterator it = m_mapGroup.begin();
//
//	for (it = m_mapGroup.begin(); it != m_mapGroup.end(); it++)
//	{
//		delete it->second;
//	}
}

//void CWorld::Init()
//{
//	// call after mysql init.
//
//}

CWorld * CWorld::GetInstance()
{
	if (!m_pInstance)
	{
		m_pInstance = new CWorld();
	}
	return m_pInstance;
}
//void CWorld::AddSession(CUserSession *pSession)
//{
//	m_addSessQueue.add(pSession);
//}
//void CWorld::_AddSession(CUserSession *pSession)
//{
//	assert(pSession);
////	printf("new session added~\n");	
//	m_sessions[pSession->GetNetId()] = pSession;
//}

bool CWorld::InitSettings()
{
	m_timers[WUPDATE_PINGDB].SetInterval(1 * 60 * 1000);	// 1分钟
	m_timers[WUPDATE_30min].SetInterval(30 * 60 * 1000);
	m_timers[WUPDATE_LOGIN].SetInterval(30 * 1000);			// 30秒
	m_timers[WUPDATE_10min].SetInterval(10 * 60 * 1000);	// 
	m_timers[WUPDATE_1s].SetInterval(1 * 1000);	// 
	m_timers[WUPDATE_CHARGE].SetInterval(5 * 1000);	// 

	m_pCNotice = new CNotice();
	assert(m_pCNotice);
	if (!m_pCNotice->Init())
	{
		IME_ERROR("notice init fail");
		return false;
	}
	m_pCAnnouncement = new CAnnouncement();
	assert(m_pCAnnouncement);
	if (!m_pCAnnouncement->Init())
	{
		IME_ERROR("announcement init fail");
		return false;
	}

	m_RankTime = time(NULL);

//	m_PrestigeTime = time(NULL);
//	CConfArenaPrestige::FlushPrestigeCollect();	
	
//	uint32_t datestrformat = CUtil::GetDateTime(time(NULL));
//	uint32_t firsttimeofweek = CUtil::GetFirstWeekday(datestrformat, 0) + 24 * 3600;
//	uint8_t day = GlobalConfig::RankPrizeTime / 100;
//	uint8_t hour = GlobalConfig::RankPrizeTime % 100;
//	firsttimeofweek += day * 24 * 3600 + hour * 3600;
//	if(time(NULL) > firsttimeofweek)
//		firsttimeofweek += 7 * 24 * 3600;
//	m_RankTimeLimit = firsttimeofweek;

	// group date init
//	std::vector<CDBCtrl::SGroup> vec;
//	CDBCtrl::GetGroupInfoAll(vec);
//	for (int i = 0; i < vec.size(); i++)
//	{
//		CGroup *pGroup = new(std::nothrow) CGroup();
//		assert(pGroup != NULL);
//		if (!pGroup->Init(vec[i]))
//		{
//			IME_ERROR("group init fail");
//			return false;
//		}
//		uint64_t id;
//		id = MAKE_FIND_GROUPID(vec[i].dwUserId,vec[i].dwGroupId);
//		IME_DEBUG("%u groupid %llu", i, id);
//		if (AddCGroup(id, pGroup))
//		{
//			IME_ERROR("add group fail find id %lu", id);
//			return false;
//		}
//	}
	SServerData tmp;	
	for (int i = 0; i < m_vecServerId.size(); i++)
	{
//		IME_DEBUG("game serverid %u", m_vecServerId[i]);
		uint8_t CanLogin = 0;
		uint8_t Status = 0;
		uint8_t IsNew = 0;
		std::string ServerName;
		std::string Domain;
		std::string ResUrl;
		if (!CDBCtrl::GetLoginInfo(m_vecServerId[i], m_strIp, m_nPort, GetOnlineNum(), CanLogin, Status, IsNew, ServerName, Domain, ResUrl))
		{
			IME_ERROR("Get server info fail");
			return false;
		}
		tmp.serverid = m_vecServerId[i];
		tmp.canlogin = CanLogin;
		tmp.status = Status;
		tmp.isnew = IsNew;
		tmp.servername = ServerName;
		tmp.ip = m_strIp;
		tmp.port = m_nPort;
		tmp.domain = Domain;
		tmp.resurl = ResUrl;
		m_mapServer[m_vecServerId[i]] = tmp;
	}

	UpdateLoginInfo();

	m_pWorldBoss = new(std::nothrow) CWorldBoss();
	assert(m_pWorldBoss != NULL);
	if (m_pWorldBoss->Init(MAX_WORLD_BOSS_CAPACITY))
	{
		IME_ERROR("worldboss init fail");
		return false;
	}
	
	if (!CActiveCtrl::GetData())
	{
		IME_ERROR("get data fail");
		return false;
	}	

	if (!CenterDBCtrl::InitGmCommand())
	{
		IME_ERROR("init operate command fail");
		return false;
	}

	if (!CConfStore::Init())
	{
		IME_ERROR("init store fail");
		return false;
	}
	
	std::string strchat;
	if (!CDBCtrl::GetGlobalData(DB_GLOBAL_CHAT, strchat))
	{
		IME_ERROR("get global data fail");
		return false;
	}

	m_lstWorldBroadCast.clear();
	ByteBuffer by;
	by.append(strchat.data(), strchat.size());
	SWorldChat tchat;
	uint32_t len;
	by >> len;
	for (int i = 0; i < len; i++)
	{
		by >> tchat.choose;
		by >> tchat.userid;
		by >> tchat.username;
		by >> tchat.content;
		by >> tchat.viplv;
		m_lstWorldBroadCast.push_back(tchat);
	}

	std::string stra;
	if (!CDBCtrl::GetGlobalData(DB_GLOBAL_WHITE_LIST, stra))
	{
		IME_ERROR("get global data fail");
		return false;
	}
	CUtil::StrSplit(stra, ",", m_vecWhiteList);
	CDBCtrl::GetUserName(m_setAllUserName);

	m_GlobalDropTime = time(NULL);
	std::string strb;
	if (!CDBCtrl::GetGlobalData(DB_GLOBAL_DROP, strb))
	{
		IME_ERROR("get global data fail");
		return false;
	}
	uint32_t number = 0;
	uint32_t key,val;
	by.clear();
	by.append(strb.data(), strb.size());
	by >> number;
	for (int i = 0; i < number; i++)
	{
		by >> key;
		by >> val;
		m_mapGlobalDropCount[key] = val;
	}
	by >> m_GlobalDropTime;

	CConfRobRobot::MapData::iterator it;
	CConfRobRobot::MapData *p = CConfRobRobot::GetAllData();
	for (it = p->begin(); it != p->end(); it++)
	{
		if (it->second.dwFragment1 != 0 || it->second.dwFragment2 != 0 || it->second.dwFragment3 != 0 || it->second.dwFragment4 != 0 || it->second.dwFragment5 != 0 || it->second.dwFragment6 != 0)
		{
			if (m_mapRobRobot[it->first].Init(it->first))
			{
				IME_ERROR("init robot fail index %u", it->first);
				return false;
			}
		}
	}	

	IME_LOG("notice if server have some operations, please wait");
	std::string strc;
	if (!CDBCtrl::GetGlobalData(DB_GLOBAL_SERVER_OPERATIONS, strc))
	{
		IME_ERROR("get global data fail");
		return false;
	}

	//启动时更新包
	//this->rank.clear();
	CDBCtrl::GetRank(rank);
	//启动记录当天时间
	this->rowerRankTime = time(NULL);
	this->addRobotTime = time(NULL);

	std::vector<uint32_t> vecOpt;
	CUtil::StrSplit(strc, ",", vecOpt);
	if (std::find(vecOpt.begin(), vecOpt.end(), (uint32_t)ALL_ROLE_CARDPACKET) == vecOpt.end())
	{
		std::vector<uint32_t> vecUserId;
		if (!CDBCtrl::GetAllUserId(vecUserId))
		{
			IME_ERROR("get all user id fail");
			return false;
		}
		for (int i = 0; i < vecUserId.size(); i++)
		{
//			IME_LOG("%u userid %u", i, vecUserId[i]);
			CCardPacket *p = new (std::nothrow)CCardPacket(NULL);
			assert(p);
			if (p->FixData(vecUserId[i]))
			{
				IME_ERROR("role fix data error userid %u", vecUserId[i]);
				return false;
			}
			delete p;
		}
		vecOpt.push_back(ALL_ROLE_CARDPACKET);
		std::string stra;
		CUtil::StrMerge(stra, ",", vecOpt);
		CDBCtrl::SaveGlobalData(stra, DB_GLOBAL_SERVER_OPERATIONS);
	}
	IME_LOG("notice operations end");
	
	return true;
}

//void CWorld::Update(time_t diff)
//{
//    ///- Update the different timers
//    for (int i = 0; i < WUPDATE_COUNT; ++i)
//    {
//        if (m_timers[i].GetCurrent() >= 0)
//            m_timers[i].Update(diff);
//        else
//            m_timers[i].SetCurrent(0);
//    }
//	INIT_PROCESS_TIMER;
//
//    ///- Ping to keep MySQL connections alive
//    if (m_timers[WUPDATE_PINGDB].Passed())
//    {
//        m_timers[WUPDATE_PINGDB].Reset();
//		PROCESS_TIMER_START;
//		Update1min();
//		PROCESS_TIMER_END(NETWORK_CHECK_SEC, "1min");
//    }
//
//	if (m_timers[WUPDATE_30min].Passed())
//	{
//		m_timers[WUPDATE_30min].Reset();
//		PROCESS_TIMER_START;
//		Update30min();
//		PROCESS_TIMER_END(NETWORK_CHECK_SEC, "30min");
//	}
//	if (m_timers[WUPDATE_LOGIN].Passed())
//	{
//		m_timers[WUPDATE_LOGIN].Reset();
//		PROCESS_TIMER_START;
//		UpdateLoginInfo();
//		PROCESS_TIMER_END(NETWORK_CHECK_SEC, "30sec");
//	}
//	if (m_timers[WUPDATE_10min].Passed())
//	{
//		m_timers[WUPDATE_10min].Reset();
//		PROCESS_TIMER_START;
//		Update10min();
//		PROCESS_TIMER_END(NETWORK_CHECK_SEC, "10min");
//	}
//	if (m_timers[WUPDATE_1s].Passed())
//	{
//		m_timers[WUPDATE_1s].Reset();
//		PROCESS_TIMER_START;
////		IME_LOG("***now %u ***", interval);
//		Update1s();
//		PROCESS_TIMER_END(NETWORK_CHECK_SEC, "1sec");
//	}
//	if (m_timers[WUPDATE_CHARGE].Passed())
//	{
//		m_timers[WUPDATE_1s].Reset();
//		m_timers[WUPDATE_CHARGE].Reset();
//		PROCESS_TIMER_START;
//		UpdateCharge();
//		PROCESS_TIMER_END(NETWORK_CHECK_SEC, "5sec");
//	}
//
//	UpdateSessions(diff);
//
//	CenterDBCtrl::UpdateGmCommand();
//
//	//UpdateRelayClient(diff);
//}
//void CWorld::UpdateRelayClient(time_t diff)
//{
//	assert(m_pRelayClientSession);
//	//todo
//	m_pRelayClientSession->Update(diff);
//}
//
//void CWorld::UpdateSessions(time_t diff)
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
//			WorldPacket info(CMD_SC_CLOSE_CONNECT);
//			pSession->SendPacket(&info);
//			delete pSession;
//		}
//	}
//}

void CWorld::AddUserAuth( uint32 dwUserId, CHandlerPlayersession*  pSession)
{
	m_mapUserAuth[dwUserId] = pSession;

	// send relay server
//	WorldPacket pkg(CMD_RELAY_CS_LOGIN);
//	pkg << dwUserId;
//	sSessCtrl->SendToRelay(pkg);
}

void CWorld::DelUserAuth( uint32 dwUserId )
{
	m_mapUserAuth.erase(dwUserId);

	//send relay server
//	WorldPacket pkg(CMD_RELAY_CS_LOGOUT);
//	pkg << dwUserId;
//	sSessCtrl->SendToRelay(pkg);
}

CRole* CWorld::FindUserRole( uint32 dwUserId )
{
	MapSessionByUserId::iterator it = m_mapUserAuth.find(dwUserId);
	return it == m_mapUserAuth.end() ? NULL : it->second->GetRole();
}

CHandlerPlayersession* CWorld::FindSess(uint32 dwUserId)
{
	MapSessionByUserId::iterator it = m_mapUserAuth.find(dwUserId);
	return it == m_mapUserAuth.end() ? NULL : it->second;
}

void CWorld::AllOffline()
{
//	IME_ERROR("OFFLINE SESSION NUM[Num=%u]", (uint32)m_sessions.size());
	IME_ERROR("OFFLINE ROLE NUM[Num=%u]", (uint32)m_mapUserAuth.size());
	WorldPacket info(CMD_SC_CLOSE_CONNECT);
	sSessCtrl->SendPkgToAll(info);

//	SessionMap::iterator it = m_sessions.begin();
//	for ( ; it !=m_sessions.end(); ++it)
//	{
//		CUserSession* pSess = it->second;
//		pSess->Offline();
//		delete pSess;
//	}
	// save some group data
//	UpdateLoginInfo();
	SaveAllData();

}

//bool CWorld::SendToRelay( WorldPacket& pkg )
//{
//	if (!m_pRelayClientSession)
//	{
//		return false;
//	}
//	m_pRelayClientSession->SendPacket(&pkg);
//}
//
//void CWorld::SendPkgToAll( WorldPacket& pkg )
//{
//	MapSessionByUserId::iterator it = m_mapUserAuth.begin();
//	for ( ; it != m_mapUserAuth.end(); ++it)
//	{
////		IME_DEBUG("before send data to userid %u", it->first);
//		CPlayerSession* pSess = it->second;
//		pSess->SendPacket(&pkg);
////		IME_DEBUG("after send data to userid %u", it->first);
//	}
//}
//
//void CWorld::SendPkgToPart( WorldPacket& pkg, uint32_t off, uint32_t number)
//{
//	uint32_t i = 0;
//	MapSessionByUserId::iterator it = m_mapUserAuth.begin();
//	for ( ; it != m_mapUserAuth.end(); ++it)
//	{
//		if (i >= off && i < off + number)
//		{
//			CPlayerSession* pSess = it->second;
//			pSess->SendPacket(&pkg);
//			if (pSess->GetRole() != NULL)
//			{
//				IME_LOG("send data to userid %u", pSess->GetRole()->GetdwAccountId());
//			}
//			else
//			{
//				IME_LOG("it is impossible");
//			}
//		}
//		i++;
//	}
//}

void CWorld::Update1min()
{
//	IME_DEBUG("Ping MySQL to keep connection alive");
	//LoginDatabase.Ping();
	//CenterDatabase.Ping();
	WorldDb.Ping();
	CenterDBCtrl::ReadNewGmCommand();
	//中午12点插入机器人闯塔排行
	if(!CUtil::IsTodayOffset(addRobotTime,TWELVE_CLOCK_SECOND)){
		addRobotTime = time(NULL);
		std::map<uint32, uint32> *layerMap = CConfRobRobot::GetRobotLayerMap();
		for (std::map<uint32, uint32>::iterator it = layerMap->begin();	it != layerMap->end(); it++) {
			uint32 userId = it->first;
			uint32 layer = it->second;
			STC_CONF_ROB_ROBOT *robot = CConfRobRobot::Find(userId);
			std::string name = robot->strName;
			CDBCtrl::InsertUserRank(userId, name.c_str(), layer, 0, robot->wLevel, 0, robot->dwMasterCardIndex);
		}
	}
	this->rank.clear();
	CDBCtrl::GetRank(rank);
	if(!CUtil::IsToday(rowerRankTime)){
		//重新置
		rowerRankTime = time(NULL);
		//重置排行信息
		CDBCtrl::ClearRankData();
		//发送排行领奖邮件
		CMail::SBaseAttr tmpAttr = {0};
		tmpAttr.coin = 0;
		tmpAttr.dollar = 0;
		tmpAttr.energy = 0;
		tmpAttr.strength = 0;
		tmpAttr.worldhistory = 0;
		uint32_t mail_id = 0;
		CDBCtrl::stuMailDetail tmp = {0};
		std::string con;
		uint32 max = PLAYER_INIT_ID << 8;
		uint32 min = ROBOT_INIT_ID << 8;
		for(int i = 0; i < rank.size(); i++){
			//不处理机器人
			if(rank[i].userId < max && rank[i].userId > min){
				continue;
			}
			CConfTowerRankPrize::MapData rankArray= CConfTowerRankPrize::GetData();
			STC_TOWER_RANK_PRIZE * prize = &rankArray[i + 1];
			con = CUtil::StrReplace(GlobalConfig::TowerMailContent, std::string("%s"), CUtil::Uint32ToString(i + 1));
			CMail::AddEmailSysDiy(rank[i].userId, mail_id, tmp, prize->type, prize->param1, prize->param2, con, tmpAttr, GlobalConfig::TowerMailTitle);
			CRole *pPeerCRole = sWorld->FindUserRole(rank[i].userId);
			if (pPeerCRole != NULL)
			{
				CMail::EmailUpdate(pPeerCRole, CMail::ADD, &tmp, 0);
			}
		}
	}
}

void CWorld::Update30min()
{
	MallocExtension::instance()->ReleaseFreeMemory();
	m_pWorldBoss->OnTimer();
}

void CWorld::Update10min()
{
	SaveAllData();		
}

void CWorld::UpdateLoginInfo()
{
	m_pCNotice->HandlerSendSysBroadCast();
	for (int i=0; i< m_vecServerId.size(); i++)
	{
		uint32_t serverid = m_vecServerId[i];
		CDBCtrl::UpdateLoginInfo(serverid, m_strIp, m_nPort, GetOnlineNum(), m_mapServer[serverid].canlogin, m_mapServer[serverid].status, m_mapServer[serverid].isnew, m_mapServer[serverid].servername, m_mapServer[serverid].domain, m_mapServer[serverid].resurl);
		CenterDBCtrl::UpdateGameServerInfo(serverid, m_mapServer[serverid].servername, m_mapServer[serverid].domain, m_mapServer[serverid].ip, m_mapServer[serverid].port, m_mapServer[serverid].resurl, GetOnlineNum(), CUtil::Uint32ToString(INNER_VERSION).c_str(), "", m_mapServer[serverid].canlogin, m_mapServer[serverid].status, 0); 
	}
	// check alive connect
//	SessionMap::iterator itr = m_sessions.begin();
//	SessionMap::iterator next;
//	for(; itr != m_sessions.end(); itr = next)
//	{
//		next = itr;
//		++next;
//		CPlayerSession *pSession = (CPlayerSession *)(itr->second);
//		if (pSession->GetdwActiveTime() + CLIENT_ACTIVE_TIME <= time(NULL))
//		{
//			IME_ERROR("Session account id %u not alive close it alivetime %u, now %u", pSession->GetAccountId(), pSession->GetdwActiveTime(), time(NULL));
//			WorldPacket info(CMD_SC_CLOSE_CONNECT);
//			pSession->SendPacket(&info);
//			m_sessions.erase(itr);
//			pSession->Offline();
//			delete pSession;
//		}
//	}
}

void CWorld::Update1s()
{

	time_t temp = time(NULL);
//	IME_DEBUG("now %u rank limit %u", temp, m_RankTimeLimit);
//	if (temp >= m_RankTimeLimit)
//	{
//		m_RankTime = temp;
//		CDBCtrl::GetUserArenaTop500(m_ArenaUser);
//		FlushAllUserRank();
//
//		uint32_t datestrformat = CUtil::GetDateTime(temp);
//		uint32_t firsttimeofweek = CUtil::GetFirstWeekday(datestrformat, 0) + 24 * 3600;
//		uint8_t day = GlobalConfig::RankPrizeTime / 100;
//		uint8_t hour = GlobalConfig::RankPrizeTime % 100;
//		firsttimeofweek += day * 24 * 3600 + hour * 3600;
//		if(temp > firsttimeofweek)
//			firsttimeofweek += 7 * 24 * 3600;
//		m_RankTimeLimit = firsttimeofweek;
//
//		IME_DEBUG("****rank flush time %u*****", temp);
//	}
//	if (m_PrestigeTime + GlobalConfig::PrestigePrizeTime <= temp)
//	{
//		CConfArenaPrestige::FlushPrestigeCollect();
//		m_PrestigeTime = temp;
//		FlushAllUserPrestige();	
//		IME_DEBUG("****Prestige flush time %u*****", temp);
//	}
//	IME_ERROR("arena cd %u, RankTime %u", GetRankCDTime(), m_RankTime);
	if (GetRankCDTime() <= 0)
	{
		m_RankTime = time(NULL);
		std::vector<CDBCtrl::SArena> vecTmp;
		if (CDBCtrl::GetUserArenaAll(vecTmp))
		{
			for (int i = 0; i < vecTmp.size(); i++)
			{
				IME_DEBUG("arena flush userid %u rank %u", vecTmp[i].userid, vecTmp[i].rank);
				if (!IS_ROBORT(vecTmp[i].userid))
				{
					CRole *pCRole = FindUserRole(vecTmp[i].userid);		
					uint32_t prestige = CArena::GetPrizePrestige(vecTmp[i].rank); 
					if (pCRole != NULL)
					{
						pCRole->GetclsArena()->SetdwRanked(vecTmp[i].rank);
						pCRole->GetclsArena()->SetdwGetPrestige(prestige);
						pCRole->GetclsArena()->HandlerProUpdate();
					}	
					else
					{
						if (!CDBCtrl::UpdateUserArenaForRankPrestige(vecTmp[i].rank, prestige, vecTmp[i].userid))
						{
							IME_ERROR("update arena data fail userid %u", vecTmp[i].userid);
						}
					}
				}
			}
		}
	}

	CActiveCtrl::OnTimer1s();
	CActiveCtrl::HandlerActiveBase(NULL, 0);
	CActiveCtrl::HandlerActiveInfo(NULL, 0);
	CActiveCtrl::IsUpdate = 0;

	if (!CUtil::IsToday(m_GlobalDropTime))
	{
		m_mapGlobalDropCount.clear();
		m_GlobalDropTime = time(NULL);
	}
//	std::map<uint64_t, CGroup *>::iterator it;
//	for (it = m_mapGroup.begin(); it != m_mapGroup.end(); it++)
//	{
//		it->second->OnTimer1s();
//	}
	CenterDBCtrl::UpdateGmCommand();
}

void CWorld::UpdateCharge()
{
//	IME_DEBUG("charge get record");
	uint32_t order_id = 0;
	std::list<STC_CHARGE_INFO> listtmp;
	std::list<STC_CHARGE_INFO>::iterator it;
	if (CenterDBCtrl::GetUnhandledCharge(listtmp))
	{
		for(it = listtmp.begin(); it != listtmp.end(); it++)
		{
			uint8_t IsOnline = 1;
//			IME_ERROR("role id %u", it->dwRoleId);
			CRole *pCRole = sWorld->FindUserRole(it->dwRoleId);
			if (pCRole == NULL)
			{
				IsOnline = 0;
				continue;
			}

			if (!(pCRole->IsInitRoleInfo()))
			{
				continue;
			}

			if (!CDBCtrl::GetShopOrderId(pCRole->GetdwAccountId(), order_id))
			{
				IME_ERROR("Get shopid orderid fail user_id %u", pCRole->GetdwAccountId());
				continue;
			}

			uint32_t totalnumber = it->dwGoodsQuantity;
			uint8_t method = 0;
			if (it->dwGoodsId == 0)
			{
				totalnumber = 1;
				method = 1;
			}

			STC_CONF_STORE *pConfStore = NULL;
			if (method == 0)
			{
				pConfStore = CConfStore::Find(it->dwGoodsId);
				if (pConfStore == NULL)
				{
					IME_ERROR("can not recognition this goods index %u", it->dwGoodsId);
					continue;
				}
			}
			else
			{
//				pConfStore = CConfStore::FindByDollar(it->dwGoodsQuantity / 100, it->dwKind);
				pConfStore = CConfStore::FindByRMBWithRange(it->dwGoodsQuantity / 100, it->dwKind);
				if (pConfStore == NULL)
				{
					IME_ERROR("can not recognition this goods dollar index %u, kind %u", it->dwGoodsQuantity, it->dwKind);
					continue;
				}
			}

			if (pConfStore->byObjType != STORE_OBJ_RMB)
			{
				IME_ERROR("store index %u goods id %u is not rmb record", pConfStore->dwIndex, it->dwGoodsId);
				continue;
			}

			IME_LOG("role id %u charge %u", it->dwRoleId, pConfStore->dwIndexNumber);
			
			uint32_t dollar = 0;
			uint8_t flag = pCRole->GetbyFlag();
//			uint32_t vipexp = pConfStore->dwObjNumber;
//			uint32_t rmb = pConfStore->dwObjNumber;
			uint32_t vipexp = 0;
			uint32_t rmb = 0;

			uint8_t IsDouble = 0;
			
			if (IS_ALREADY_DOUBLE_CHARGE(flag))
			{
				if (method == 0)
				{
					dollar = pConfStore->dwIndexNumber * it->dwGoodsQuantity;
					vipexp = pConfStore->dwObjNumber * it->dwGoodsQuantity;
					rmb = pConfStore->dwObjNumber * it->dwGoodsQuantity;
				}
				else
				{
					dollar = it->dwGoodsQuantity / 100.0 / pConfStore->dwObjNumber * pConfStore->dwIndexNumber;
					vipexp = it->dwGoodsQuantity / 100.0;
					rmb = it->dwGoodsQuantity / 100.0;
				}
			}
			else
			{
				if (method == 0)
				{
					vipexp = pConfStore->dwObjNumber * it->dwGoodsQuantity;
					rmb = pConfStore->dwObjNumber * it->dwGoodsQuantity;
					if (vipexp >= CHARGE_DOUBLE_LIMIT)
					{
						dollar = pConfStore->dwIndexNumber * it->dwGoodsQuantity * FIRST_CHARGE_MULTI_PARA / 100.0;
						IsDouble = 1;
					}
					else
					{
						dollar = pConfStore->dwIndexNumber * it->dwGoodsQuantity;
					}
				}
				else
				{
					vipexp = it->dwGoodsQuantity / 100.0;
					rmb = it->dwGoodsQuantity / 100.0;
					if (vipexp >= CHARGE_DOUBLE_LIMIT)
					{
						dollar = it->dwGoodsQuantity / 100.0 / pConfStore->dwObjNumber * pConfStore->dwIndexNumber * FIRST_CHARGE_MULTI_PARA / 100.0;
//						dollar = pConfStore->dwIndexNumber * FIRST_CHARGE_MULTI_PARA / 100.0;
						IsDouble = 1;
					}
					else
					{
						dollar = it->dwGoodsQuantity / 100.0 / pConfStore->dwObjNumber * pConfStore->dwIndexNumber;
					}
				}
			}

			if (!CenterDBCtrl::ChargeHandled(it->dwAutoId, dollar, pCRole->GetSession()->GetRemoteIP(), pCRole->GetSession()->GetLoginDevice(), pCRole->GetSession()->GetLoginDeviceType(), pCRole->GetSession()->GetLoginDeviceUid()))
			{
				IME_ERROR("update charge fail");
				continue;
			}
			CDBCtrl::LogShopRecord(pCRole->GetdwAccountId(), order_id, dollar, pConfStore->dwStoreIndex, totalnumber, E_DC_CHARGE);	

			SCommonActivePara chargepara;
			chargepara.para1 = rmb;
			chargepara.para2 = 0;
			pCRole->GetCActive()->ChangeCommonActive(CHARGE_SINGLE_ACTIVE_ID, chargepara, 1);	
			pCRole->GetCActive()->ChangeCommonActive(CHARGE_ACCUMULATE_ACTIVE_ID, chargepara, 1);	
			pCRole->GetCActive()->ChangeCommonActive(CHARGE_CYCLE_ACTIVE_ID, chargepara, 1);	

			if (!IS_ALREADY_FIRST_CHARGE(flag))
			{
				flag = OPEN_FIRST_PRIZE(flag);
				flag = FIRST_CHARGE(flag);
				pCRole->SetbyFlag(flag);
			}

			if (IsDouble)
			{
				flag = OPEN_DOUBLE_CHARGE(flag);
				pCRole->SetbyFlag(flag);
			}

			uint16_t oldviplv = pCRole->GetwVipLv();

			pCRole->ChangeDollar(dollar, SOURCE_CHARGE);
			pCRole->ChangeVipExp(vipexp);
			pCRole->Calculate(false);
			pCRole->HandlerInfoOpt();

			uint32_t luck = pCRole->GetdwLuck() + vipexp * DOLLAR_LUCK_PARA;
			uint16_t newviplv = pCRole->GetwVipLv();
			STC_CONF_VIP *pVipConf = CConfVip::Find(newviplv);
			if (newviplv > oldviplv)
			{
				if (pVipConf == NULL)
				{
					IME_ERROR("vip lv %u can not find in vip csv", newviplv);
				}
				else
				{
					luck = pVipConf->dwLuckMax + vipexp * DOLLAR_LUCK_PARA;
				}
				CDataPush dpush;
				dpush.HandlerDataPushVip(pCRole, newviplv);
			}

			pCRole->SetdwLuck(luck);
			pCRole->SetdwChargeLuck(pCRole->GetdwChargeLuck() + vipexp * DOLLAR_LUCK_PARA);

			pCRole->SaveAllData();
			order_id++;
			if (IsOnline)
			{
				pCRole->SendProUpdate();
				CDataPush dpush;
				dpush.m_byCharge = 1;
				dpush.m_dwChargeNumber = dollar;
				dpush.HandlerDataPush(pCRole);	
			}
		}
	}
}

//void CWorld::FlushAllUserRank()
//{
//	MapSessionByUserId::iterator it = m_mapUserAuth.begin();
//	for ( ; it != m_mapUserAuth.end(); ++it)
//	{
//		CPlayerSession* pSess = it->second;
//		CRole *pCRole = pSess->GetRole();
//		if (pCRole == NULL)
//		{
//			IME_ERROR("prole is NULL in mapUserAuth");
//			return;
//		}
//		if (!pCRole->IsOpenArena())
//		{
//			return;
//		}
//		pCRole->GetclsArena()->HandlerArenaRankFlush();
//		pCRole->GetclsArena()->HandlerArenaRankProduct();
//	}
//}
//
//void CWorld::FlushAllUserPrestige()
//{
//	MapSessionByUserId::iterator it = m_mapUserAuth.begin();
//	for ( ; it != m_mapUserAuth.end(); ++it)
//	{
//		CPlayerSession* pSess = it->second;
//		CRole *pCRole = pSess->GetRole();
//		if (pCRole == NULL)
//		{
//			IME_ERROR("prole is NULL in mapUserAuth");
//			return;
//		}
//		if (!pCRole->IsOpenArena())
//		{
//			return;
//		}
//		pCRole->GetclsArena()->HandlerArenaPrestigeProduct();
//	}
//}

void CWorld::SetServerId(std::string strServerId)
{
	CUtil::StrSplit(strServerId, "'", m_vecServerId);
}
void CWorld::SetIp(std::string &strIp)
{
	m_strIp = strIp;
}
void CWorld::SetPort(int nPort)
{
	m_nPort = nPort;
}
uint32 CWorld::GetOnlineNum()
{
//	return m_sessions.size();
	return m_mapUserAuth.size();
}

bool CWorld::ReloadConfig( const std::string& strConfigName )
{
	bool bSucc = true;
	if (strConfigName == "global")
	{

		if (!GlobalConfig::Init(g_strConfigPath + "GlobalConfig.ini"))
		{
			IME_ERROR("GlobalConfig::Init failed!");
		}
		else
		{
			IME_LOG("GlobalConfig::Init succ!!");
		}
	}
	else if (strConfigName == "test")
	{
		CConfTest::Init(g_strConfigPath + "Test.csv");
	}
	else
	{
		IME_ERROR("UNKNOW CONFIG NAME [%s]", strConfigName.c_str());
		bSucc = false;
	}
	return bSucc;
}

void CWorld::GetRoleList( std::vector<CRole*>& vecRole )
{
	MapSessionByUserId::iterator it = m_mapUserAuth.begin();
	for ( ; it != m_mapUserAuth.end(); ++it)
	{
		vecRole.push_back(it->second->GetRole());
	}
}

CGroup *CWorld::GetCGroup(uint64_t groupid)
{
	if (m_mapGroup.find(groupid) == m_mapGroup.end())
	{
		return NULL;
	}	
	return m_mapGroup[groupid];
}

int CWorld::AddCGroup(uint64_t groupid, CGroup *p)
{
	if (m_mapGroup.find(groupid) != m_mapGroup.end())
	{
		return -1;
	}
	m_mapGroup[groupid] = p;
	return 0;
}

void CWorld::SaveAllData()
{
//	std::map<uint64_t, CGroup *>::iterator it;
//	for (it = m_mapGroup.begin(); it != m_mapGroup.end(); it++)
//	{
//		it->second->SaveData();
//	}
	m_pWorldBoss->SaveAllData();
	CActiveCtrl::SaveData();
	m_pCNotice->SaveData();

	ByteBuffer by;
	by << (uint32_t) m_lstWorldBroadCast.size();
	std::list<SWorldChat>::iterator it;
	for (it = m_lstWorldBroadCast.begin(); it != m_lstWorldBroadCast.end(); it++)
	{
		by << it->choose;
		by << it->userid;
		by << it->username;
		by << it->content;
		by << it->viplv;
	}
	std::string strt;
	strt.assign(by.contents(), by.size());
	CDBCtrl::SaveGlobalData(strt, DB_GLOBAL_CHAT); 

	std::string stra;
	CUtil::StrMerge(stra, ",", m_vecWhiteList);
	CDBCtrl::SaveGlobalData(stra, DB_GLOBAL_WHITE_LIST);

	std::string strb;
	by.clear();	
	by << (uint32_t)m_mapGlobalDropCount.size();
	std::map<uint32_t, uint32_t>::iterator ita;
	for (ita = m_mapGlobalDropCount.begin(); ita != m_mapGlobalDropCount.end(); ita++)
	{
		by << ita->first;
		by << ita->second;
	}
	by << m_GlobalDropTime;
	strb.assign(by.contents(), by.size());
	CDBCtrl::SaveGlobalData(strb, DB_GLOBAL_DROP);
}

void CWorld::GroupSearchByName(std::vector<CGroup *> &vec,const std::string &name)
{
		std::map<uint64_t, CGroup *>::iterator it;
		int j = 10;
		for (it = m_mapGroup.begin(); it != m_mapGroup.end(); it++)
		{
			if (j > 0)
			{	
				if (it->second->GetstrName() == name)
				{
					vec.push_back(it->second);
				}
				j--;
			}
		}
}

CGroup* CWorld::GetCGroupByUserId(uint32_t userid)
{
	std::map<uint64_t, CGroup *>::iterator it;
	for (it = m_mapGroup.begin(); it != m_mapGroup.end(); it++)
	{
		if (it->second->GetGroupMember(userid))
		{
			return it->second;
		}
	}
	return NULL;
}

bool mycompare (CGroup *p, CGroup *q) 
{ 
	if (p->GetwLevel() != q->GetwLevel())
	{
		return (p->GetwLevel() > q->GetwLevel());
	}
	else
	{
		return (p->GetwPersionNumber() > q->GetwPersionNumber());
	}
}

void CWorld::HandlerGetGroupList(CRole *pCRole, WorldPacket &pkg)
{
	uint32_t off;
	pkg >> off;

	std::map<uint64_t, CGroup *>::iterator it = m_mapGroup.begin();
	std::vector<CGroup *> sorteddata;
	for (; it != m_mapGroup.end(); it++)
	{
		sorteddata.push_back(it->second);
	}

	std::sort(sorteddata.begin(), sorteddata.end(), mycompare);
	std::vector<CGroup *> vec;

	int number = 10;
	for (int i = 0; i < sorteddata.size(); i++)
	{
		if (i >= off)
		{
			vec.push_back(sorteddata[i]);

			number--;
			if (number <= 0)
				break;
		}	
	}

	WorldPacket info(CMD_SC_GROUP_INFO_RESULT);
	info << (uint16_t) 0;
	info << (uint32_t) vec.size();
	for (int i = 0; i < vec.size(); i++)
	{
		uint64 tmpid = (uint64) MAKE_FIND_GROUPID(vec[i]->GetdwCreateId(), vec[i]->GetdwId()); 
		info << tmpid;
		info << vec[i]->GetstrName();
		info << vec[i]->GetwLevel();
		info << vec[i]->GetstrCreateName();
		info << vec[i]->GetwPersionNumber();
		info << vec[i]->GetwPersionLimit();
		IME_ERROR("%x %llx %s %x %s %x %x", vec.size(), tmpid, vec[i]->GetstrName().c_str(), vec[i]->GetwLevel(), vec[i]->GetstrCreateName().c_str(), vec[i]->GetwPersionNumber(), vec[i]->GetwPersionLimit());
	}
	pCRole->SendPacket(&info);
}

void CWorld::HandlerGroupRankSearch(CRole *pCRole, WorldPacket &pkg)
{
	std::string str;
	pkg >> str;

	std::map<uint64_t, CGroup *>::iterator it = m_mapGroup.begin();
	std::vector<CGroup *> sorteddata;
	for (; it != m_mapGroup.end(); it++)
	{
		sorteddata.push_back(it->second);
	}

	std::sort(sorteddata.begin(), sorteddata.end(), mycompare);
	std::vector<CGroup *> vec;
	std::vector<int> vecrank;

	int number = 10;
	for (int i = 0; i < sorteddata.size(); i++)
	{
		if (sorteddata[i]->GetstrName() == str)
		{
			vec.push_back(sorteddata[i]);
			vecrank.push_back(i + 1);

			number--;
			if (number <= 0)
				break;
		}	
	}

	WorldPacket info(CMD_SC_GROUP_RANK_SEARCH_RESULT);
	info << (uint16_t) 0;
	info << (uint32_t) vec.size();
	for (int i = 0; i < vec.size(); i++)
	{
		uint64 tmpid = (uint64) MAKE_FIND_GROUPID(vec[i]->GetdwCreateId(), vec[i]->GetdwId());
		info << (uint32_t)vecrank[i];
		info << tmpid; 
		info << vec[i]->GetstrName();
		info << vec[i]->GetwLevel();
		info << vec[i]->GetstrCreateName();
		info << vec[i]->GetwPersionNumber();
		info << vec[i]->GetwPersionLimit();
//		IME_ERROR("%x %llx %s %x %s %x %x", vecrank[i], tmpid, vec[i]->GetstrName().c_str(), vec[i]->GetwLevel(), vec[i]->GetstrCreateName().c_str(), vec[i]->GetwPersionNumber(), vec[i]->GetwPersionLimit());
	}
	pCRole->SendPacket(&info);
}

void CWorld::HandlerGetGroupDetail(CRole *pCRole, WorldPacket &pkg)
{
	uint64 groupid;
	pkg >> groupid;

	CGroup *pCGroup = GetCGroup(groupid);

	WorldPacket info(CMD_SC_GROUP_DETAIL_INFO_RESULT);
	info << (uint16_t) 0;
	info << groupid;
	info << pCGroup->GetstrName();
	info << pCGroup->GetwLevel();
	info << pCGroup->GetstrCreateName();
	info << pCGroup->GetwPersionNumber();
	info << pCGroup->GetwPersionLimit();
	info << pCGroup->GetstrNotice();
	pCRole->SendPacket(&info);
}

void CWorld::HandlerGroupCancel(CRole *pCRole, WorldPacket &pkg)
{
	uint64 groupid = pCRole->GetlFindGroupId();

	CGroup *pCGroup = GetCGroup(groupid);
	if (pCGroup == NULL)
	{
		IME_ERROR("userid %u can not join group", pCRole->GetdwAccountId());
		WorldPacket info(CMD_SC_GROUP_CANCEL_RESULT);
		info << (uint16_t) ERRNO_MSG_ALREADY_NOT_IN_GROUP;
		pCRole->SendPacket(&info);
		return;
	}
	
	if (pCGroup->GroupCancel(pCRole, pkg))
	{
		IME_ERROR("cancel group fail %llu", groupid);
		return;
	}

	pCGroup->SaveData();
	m_mapGroup.erase(groupid);
}

//void CWorld::HandlerSysBroadContent(std::string str)
//{
//	WorldPacket info(CMD_SC_BROAD_CAST);
//	info << str;
//	SendPkgToAll(info);
//}

uint32_t CWorld::GetRankCDTime()
{
	struct tm t;
	time_t tmp = m_RankTime;
	localtime_r(&tmp, &t);
	uint16_t hour1 = GlobalConfig::ArenaFirstFlush % 24;
	uint16_t hour2 = GlobalConfig::ArenaSecondFlush % 24;
	uint16_t hour3 = GlobalConfig::ArenaThirdFlush % 24;
	uint16_t hour4 = GlobalConfig::ArenaFourthFlush % 24;
	uint16_t hour5 = GlobalConfig::ArenaFifthFlush % 24;
	uint16_t hour6 = GlobalConfig::ArenaSixthFlush % 24;
	uint16_t hour7 = GlobalConfig::ArenaSeventhFlush % 24;
	uint16_t hour8 = GlobalConfig::ArenaEighthFlush % 24;
	uint16_t hour = t.tm_hour; 
	uint32_t sec = t.tm_hour * 3600 + t.tm_min * 60 + t.tm_sec;
	uint32_t now = time(NULL);
//	IME_DEBUG("hour %u", hour);
	if (hour < hour1)
	{
		int64_t cd = (hour1 * 3600 - sec) - ((int64_t)now - m_RankTime);
		return cd < 0 ? 0 : cd;
	}
	else if (hour < hour2)
	{
		int64_t cd = (hour2 * 3600 - sec) - ((int64_t)now - m_RankTime);
		return cd < 0 ? 0 : cd;
	}
	else if (hour < hour3)
	{
		int64_t cd = (hour3 * 3600 - sec) - ((int64_t)now - m_RankTime);
//		IME_DEBUG("cd %ld, now %ld, m_RankTime %ld, para1 %ld, para2 %ld sec %ld",cd, now, m_RankTime, (int64_t)now - m_RankTime, (hour3 * 3600 - sec), sec);
		return cd < 0 ? 0 : cd;
	}
	else if (hour < hour4) {
		int64_t cd = (hour4 * 3600 - sec) - ((int64_t) now - m_RankTime);
		return cd < 0 ? 0 : cd;
	} else if (hour < hour5) {
		int64_t cd = (hour5 * 3600 - sec) - ((int64_t) now - m_RankTime);
		return cd < 0 ? 0 : cd;
	} else if (hour < hour6) {
		int64_t cd = (hour6 * 3600 - sec) - ((int64_t) now - m_RankTime);
		return cd < 0 ? 0 : cd;
	} else if (hour < hour7) {
		int64_t cd = (hour7 * 3600 - sec) - ((int64_t) now - m_RankTime);
		return cd < 0 ? 0 : cd;
	} else if (hour < hour8) {
		int64_t cd = (hour8 * 3600 - sec) - ((int64_t) now - m_RankTime);
		return cd < 0 ? 0 : cd;
	}
	else
	{
		int64_t cd = (hour1 * 3600 + 24 * 3600 - sec) - ((int64_t)now - m_RankTime);
		return cd < 0 ? 0 : cd;
	}
}

void CWorld::AddWorldChat(const SWorldChat &tmp)
{
	m_lstWorldBroadCast.push_back(tmp);
	if (m_lstWorldBroadCast.size() > MAX_WORLD_CHAT_NUMBER)
	{
		m_lstWorldBroadCast.pop_front();
	}
}

void CWorld::HandlerWorldChatInfo(CRole *pCRole)
{
	std::list<SWorldChat>::iterator it;
	for (it = m_lstWorldBroadCast.begin(); it != m_lstWorldBroadCast.end(); it++)
	{
		WorldPacket info(CMD_SC_WORLD_BROADCAST_INFO);
		info << it->choose;
		info << it->userid;
		info << it->username;
		info << it->content;
		info << it->viplv;
		pCRole->SendPacket(&info);
	}
}

void CWorld::PkgWorldChat(SWorldChat tmp, WorldPacket &info)
{
	info.clear();
	info.SetOpcode(CMD_SC_WORLD_BROADCAST_INFO);
	info << tmp.choose;
	info << tmp.userid;
	info << tmp.username;
	info << tmp.content;
	info << tmp.viplv;

	AddWorldChat(tmp);
}

bool CWorld::IsAllowDrop(uint32_t index, uint32_t number)
{
	std::vector<int>::iterator it = std::find(GlobalConfig::DropIndex.begin(), GlobalConfig::DropIndex.end(), index);
	if (it == GlobalConfig::DropIndex.end())
	{
		return true;
	}
	int pos = it - GlobalConfig::DropIndex.begin();

	if (m_mapGlobalDropCount.find(index) == m_mapGlobalDropCount.end())
	{
		m_mapGlobalDropCount[index] = 0;
	}
	if (number + m_mapGlobalDropCount[index] > GlobalConfig::DropLimit[pos])
		return false;
	return true;
}

void CWorld::AddDropItem(uint32_t index, uint32_t number)
{
	std::vector<int>::iterator it = std::find(GlobalConfig::DropIndex.begin(), GlobalConfig::DropIndex.end(), index);
	if (it == GlobalConfig::DropIndex.end())
	{
		return;
	}
	int pos = it - GlobalConfig::DropIndex.begin();

	if (m_mapGlobalDropCount.find(index) == m_mapGlobalDropCount.end())
	{
		m_mapGlobalDropCount[index] = 0;
	}
	m_mapGlobalDropCount[index] += number;
}

std::vector<STowerRank>* CWorld::getRank(){
	return &rank;

}
