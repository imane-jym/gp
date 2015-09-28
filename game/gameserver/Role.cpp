#include "Role.h"
#include "DatabaseMysql.h"
#include "PlayerSession.h"
#include "ShardingDB.h"
#include "CmdDefine.h"
#include "ErrorMsg.h"
#include "EnumDefines.h"
#include "GmCmdPro.h"
//#include "LuaScript.h"
//#include "LuaState.h"
//#include "LuaCtrl.h"
#include "ConfigData.h"
#include <cmath>
#include <new>
#include "NameChecker.h"
#include "World.h"
#include "GlobalConfig.h"
#include "CmdInnerDefine.h"
#include "md5.h"
#include "CardPacket.h"
#include "Log.h"
#include "Combine.h"
#include "Tujian.h"
#include "Tech.h"
#include "Mail.h"
#include "Arena.h"
#include "Task.h"
#include "Group.h"
#include "Master.h"
#include "Other.h"
#include "WorldBoss.h"
#include "CenterDBCtrl.h"
#include "Notice.h"
#include "Mission.h"
#include "Card.h"
#include "EnumDefines.h"
#include "Tower.h"
#include "Timer.h"

#define ERROR_NEW_TEST(t, name) \
    if (t == NULL)\
    {\
    IME_LOG("NEW A OBJECT FAIL %s", name);\
    abort();\
    }
#define ERROR_INIT_TEST(t, name) \
    if (t == NULL)\
    {\
    IME_LOG("A OBJECT INIT FAIL %s", name);\
    return false;\
    }
#define ERROR_LOAD_TEST(t, name) \
    if (t)\
    {\
    IME_LOG("A OBJECT[%s] LOAD DATA FAIL", name);\
	return false;\
    }
#define ERROR_SAVE_TEST(t, name) \
    if (t)\
    {\
    IME_LOG("A OBJECT[%s] SAVE DATA FAIL", name);\
    }

//extern CLuaState g_luaState;
extern CShardingDB WorldShardingDB;

CRole::CRole(CHandlerPlayersession * pSession)
:m_pSession(pSession)
{

	m_bIsInitOK = false;
	m_clsCardPacket = NULL;
	m_clsItemPacket = NULL;
	m_clsTech = NULL;
	m_clsTujian = NULL;
	m_clsArena = NULL;
	m_CShop = NULL;
	m_CMaster = NULL;
	m_CDMission = NULL;
	m_CActive = NULL;
	m_CRobotCtrl = NULL;
	m_CTimerGift = NULL;
	m_CMission = NULL;
	m_CTrial = NULL;
	m_CTower = NULL;
	
	m_dwUserOnlineTime = 0;
	m_byFirstLogin = 0;
	m_byDuration = DURATION_NORMAL;

	m_dwDelDMissionCnt = 0;
	m_dwDelDMissionCntTime = 0;
	m_dwEnergyUseCnt = 0;
	m_dwEnergyUseCntTime = 0;
	m_dwStrengthUseCnt = 0;
	m_dwStrengthUseCntTime = 0;

	m_dwGmFlushTime = 0;
	m_dwHighDelDMissionCnt = 0;
	m_dwHighDelDMissionCntTime = 0;
	m_dwLuck = 0;
	m_dwExtraStrengthPro = 0;
	m_dwLowMasterCount = 0;
	m_dwLowMasterTime = 0;

	m_wUseEnergy = 0;
	m_wUseStrength = 0;
	m_dwEnergyStrengthTime = 0;
	m_dwChargeLuck = 0;
	m_dwGiftDayTime = 0;
	m_wOtherDataversion = 0;
	m_dwStamina = 0;
	m_dwStaminaTime = 0;
	m_dwStaminaMax = 0;
	m_dwStaminaRate = 0;
	m_wUseStamina = 0;
}

CRole::~CRole()
{
	IME_LOG("RELEASE ROLE[UserId=%u]", GetdwAccountId());
	if (m_clsCardPacket != NULL)
		delete m_clsCardPacket;
	if (m_clsItemPacket != NULL)
		delete m_clsItemPacket;
	if (m_clsTech != NULL)
		delete m_clsTech;
	if (m_clsTujian != NULL)
		delete m_clsTujian;
	if (m_clsArena != NULL)
		delete m_clsArena;
	if (m_CShop != NULL)
		delete m_CShop;
	if (m_CMaster != NULL)
		delete m_CMaster;
	if (m_CDMission != NULL)
		delete m_CDMission;
	if (m_CActive != NULL)
		delete m_CActive;
	if (m_CRobotCtrl != NULL)
		delete m_CRobotCtrl;
	if (m_CTimerGift != NULL)
		delete m_CTimerGift;
	if (m_CMission != NULL)
		delete m_CMission;
	if (m_CTrial != NULL)
		delete m_CTrial;
	if (m_CTower != NULL)
		delete m_CTower;
}

bool CRole::LoadDataForDuration(uint32 dwAccountId)
{
	SetdwAccountId(dwAccountId);

	bool bIsSucc = CDBCtrl::GetUserBaseData(
//			m_wHeadPic,
			m_strUserName,
			m_wLevel,
			m_dwOneLevelExp,
			m_wVipLv,
			m_dwVipExp,
			m_dwDollar,
			m_dwCoin,
			m_dwEnergy,
			m_dwEnergyMax,
			m_dwEnergyRate,
			m_dwStrength,
			m_dwStrengthMax,
			m_dwStrengthRate,
//			m_dwCardAttributes,
//			m_dwAttackMin,
//			m_dwAttackMax,
//			m_dwDefenceMin,
//			m_dwDefenceMax,
//			m_dwAttackWin,
//			m_dwAttackTot,
//			m_dwDefenceWin,
//			m_dwDefenceTot,
			m_dwAccountId,
			m_dwInitNumber,
			m_byTaskLocation1,
			m_byTaskLocation2,
			m_byTaskLocation3,
			m_byTaskLocation4,
			m_byCardCapacity,
//			m_dwCardAttributes2,
//			m_byCurrentTeam,
			m_dwExplimit,
			m_dwEnergyCnt,
			m_dwStrengthCnt,
			m_byFlag,
			m_lFindGroupId,
			m_dwAvoidDurationTime,
			m_dwWorldHistory,
			m_dwWorldBroadTime,
			m_dwGameFlow,
			m_dwLuck,
			m_byGm,
			m_dwChargeLuck,
			m_dwStamina,
			m_dwStaminaTime,
			m_strSignature
			);
	/////

	if (!bIsSucc)
	{
		IME_ERROR("load data fail from user db user_id %u", dwAccountId);
		return false;
	}

//        SetwHeadPic(m_wHeadPic);
        SetstrUserName(m_strUserName);
        SetwLevel(m_wLevel);
        SetdwExp(0);
        SetwVipLv(m_wVipLv);
        SetdwVipExp(m_dwVipExp);
        SetdwDollar(m_dwDollar);
        SetdwCoin(m_dwCoin);
        SetdwEnergy(m_dwEnergy);
        SetdwEnergyMax(m_dwEnergyMax);
        SetdwEnergyRate(m_dwEnergyRate);
        SetdwStrength(m_dwStrength);
        SetdwStrengthMax(m_dwStrengthMax);
        SetdwStrengthRate(m_dwStrengthRate);
//        SetdwCardAttributes(m_dwCardAttributes);
//        SetdwAttackMin(m_dwAttackMin);
//        SetdwAttackMax(m_dwAttackMax);
//        SetdwDefenceMin(m_dwDefenceMin);
//        SetdwDefenceMax(m_dwDefenceMax);
//        SetdwAttackWin(m_dwAttackWin);
//        SetdwAttackTot(m_dwAttackTot);
//        SetdwDefenceWin(m_dwDefenceWin);
//        SetdwDefenceTot(m_dwDefenceTot);
        SetbyTaskLocation1(m_byTaskLocation1);
        SetbyTaskLocation2(m_byTaskLocation2);
        SetbyTaskLocation3(m_byTaskLocation3);
        SetbyTaskLocation4(m_byTaskLocation4);
        SetbyCardCapacity(m_byCardCapacity);
        SetdwExplimit(m_dwExplimit);
//        SetbyCurrentTeam(m_byCurrentTeam);
//		SetdwAttribute(GetRealAttribute());
		SetbyFlag(GetbyFlag());
		SetlFindGroupId(m_lFindGroupId);
		SetdwAvoidDurationTime(m_dwAvoidDurationTime);
		SetdwWorldHistory(m_dwWorldHistory);
		SetdwFriendLimit(0);
		SetdwLastExplimit(0);
		SetdwGameFlow(m_dwGameFlow);
		SetdwLuck(m_dwLuck);
		SetbyGm(m_byGm);
		SetwNextVipLv(0);
		SetdwNextVipExp(0);
		SetdwChargeLuck(m_dwChargeLuck);
		SetdwStamina(m_dwStamina);
//		SetdwStaminaTime(m_dwStaminaTime);
		SetdwStaminaMax(0);
		SetdwStaminaRate(0);
		SetstrSignature(m_strSignature);

	if (OtherDataGet())
	{
		IME_ERROR("get other data fail");
		return -1;
	}

	int ret;

	ret = CDBCtrl::GetStatusArena(m_dwAccountId);
	if (ret == 1)
	{
		SetbyFlag(OPEN_ARENA(GetbyFlag()));
	}
	else if (ret == 0)
	{
		SetbyFlag(CLOSE_ARENA(GetbyFlag()));
	}
	else
	{
		IME_ERROR("get status arena fail");
		return -1;
	}

	initMultipleCardExp();

	m_clsTech = new(std::nothrow) CTech();
	ERROR_NEW_TEST(m_clsTech, "Tech");

	m_clsCardPacket = new(std::nothrow) CCardPacket(this);
	ERROR_NEW_TEST(m_clsCardPacket, "CardPacket");

	m_clsItemPacket = new(std::nothrow) CItem();
	ERROR_NEW_TEST(m_clsItemPacket, "ItemPacket");

	m_CMaster = new(std::nothrow) CMaster();
	ERROR_NEW_TEST(m_CMaster, "CMaster");
	
	if (IS_THERE_ARENA(m_byFlag))
	{
		m_clsArena = new(std::nothrow) CArena();
		ERROR_NEW_TEST(m_clsArena, "Arena");
	}

	//todo loaddata
	ret = m_clsTech->Init(this);
	ERROR_LOAD_TEST(ret, "Tech");

	ret = m_clsCardPacket->LoadForOffLogin(GetdwAccountId());
	ERROR_LOAD_TEST(ret, "CardPacket");
        
	ret = m_clsItemPacket->Init(this);
	ERROR_LOAD_TEST(ret, "ItemPacket");

	ret = m_CMaster->Init(this);
	ERROR_LOAD_TEST(ret, "CMaster");


	if (IS_THERE_ARENA(m_byFlag))
	{
		ret = m_clsArena->Init(this);
		ERROR_LOAD_TEST(ret, "Arena");
	}


	//新账号初始化
	if (GetwLevel() == 0)
	{
		IME_ERROR("it is a uninit account!!!");
		return false;
	}
	else
	{
//		if (!CDBCtrl::GetUserRank(GetdwAccountId(), m_wMaxLevel, m_dwMaxAttack, m_dwMaxDefence))
//		{
//			IME_ERROR("get rank data fail userid %u", GetdwAccountId());
//			return false;
//		}

//		if (Calculate(false))
//		{
//			IME_ERROR("calculate fail");
//			return false;
//		}
		
		if (LoadProperty())
		{
			IME_ERROR("load PROPERTY fail, userid %u", m_dwAccountId);
			return false;
		}		

		SetdwExp(m_dwOneLevelExp + m_dwLastExplimit);
//		UpdateEnergyAndStrengthTime();
	}

	return true;
}

bool CRole::Init(uint32 dwAccountId, const std::string & strAccount)
{
	//初始化角色数据
	//这两个数据不会变
	SetdwAccountId(dwAccountId);
	SetstrAccount(strAccount);
	
	INIT_PROCESS_TIMER;	
	PROCESS_TIMER_START;
	if (dwAccountId == 0)
	{
		IME_ERROR("userid is 0");
		return false;
	}

	int is_ok = LoadRole();
	if (is_ok == E_SQL_RESULT_NO_ROLE)// new role, create him
	{
		CDBCtrl::NewUserData(dwAccountId, m_strAccount);
		int tmp_is_ok = LoadRole();
		if (tmp_is_ok != E_SQL_RESULT_SUCC)
		{
			IME_ERROR("ROLE INIT ERROR, DB GET NULL");
			return false;
		}
	}
	else if (is_ok == E_SQL_RESULT_SQL_ERROR)
	{
		IME_ERROR("Role Init sql error");
		return false;
	}
//	IME_DEBUG("init a role");

	if (OtherDataGet())
	{
		IME_ERROR("get other data fail");
		return -1;
	}
	PROCESS_TIMER_END("load role");

	int ret;
	PROCESS_TIMER_START;
	ret = CDBCtrl::GetStatusArena(m_dwAccountId);
	if (ret == 1)
	{
		IME_DEBUG("before open %x", GetbyFlag()); 
		SetbyFlag(OPEN_ARENA(GetbyFlag()));
		IME_DEBUG("after open %x", GetbyFlag()); 
	}
	else if (ret == 0)
	{
		IME_DEBUG("before close %x", GetbyFlag()); 
		SetbyFlag(CLOSE_ARENA(GetbyFlag()));
		IME_DEBUG("after close %x", GetbyFlag()); 
	}
	else
	{
		IME_ERROR("get status arena fail");
		return -1;
	}

	m_clsItemPacket = new(std::nothrow) CItem();
	ERROR_NEW_TEST(m_clsItemPacket, "ItemPacket");

	m_clsTujian = new(std::nothrow) CTujian();
	ERROR_NEW_TEST(m_clsTujian, "Tujian");

	m_clsTech = new(std::nothrow) CTech();
	ERROR_NEW_TEST(m_clsTech, "Tech");

	m_clsCardPacket = new(std::nothrow) CCardPacket(this);
	ERROR_NEW_TEST(m_clsCardPacket, "CardPacket");

	if (IS_THERE_ARENA(m_byFlag))
	{
		m_clsArena = new(std::nothrow) CArena();
		ERROR_NEW_TEST(m_clsArena, "Arena");
	}

	m_CShop = new(std::nothrow) CShop();
	ERROR_NEW_TEST(m_CShop, "CShop");

	m_CMaster = new(std::nothrow) CMaster();
	ERROR_NEW_TEST(m_CMaster, "CMaster");

	m_CDMission = new(std::nothrow) CDurationMission();
	ERROR_NEW_TEST(m_CDMission, "CDurationMission");

	m_CActive = new(std::nothrow) CActive();
	ERROR_NEW_TEST(m_CActive, "CActive");

	m_CRobotCtrl = new(std::nothrow) CRobotCtrl();
	ERROR_NEW_TEST(m_CRobotCtrl, "CRobotCtrl");

	m_CTimerGift = new(std::nothrow) CTimerGift();
	ERROR_NEW_TEST(m_CTimerGift, "CTimerGift");

	m_CMission = new(std::nothrow) CMission();
	ERROR_NEW_TEST(m_CMission, "CMission");

	m_CTrial = new(std::nothrow) CTrial();
	ERROR_NEW_TEST(m_CTrial, "CTrial");

	m_CTower = new(std::nothrow) CTower();
	ERROR_NEW_TEST(m_CTower, "CTower");

	PROCESS_TIMER_END("new class");

	//todo loaddata
	PROCESS_TIMER_START
	ret = m_CMission->Init(this);
	ERROR_LOAD_TEST(ret, "CMission");
	PROCESS_TIMER_END("mission init");

	PROCESS_TIMER_START
	ret = m_clsItemPacket->Init(this);
	ERROR_LOAD_TEST(ret, "ItemPacket");
	PROCESS_TIMER_END("item packet init");

	PROCESS_TIMER_START
	ret = m_clsTujian->Init(this);
	ERROR_LOAD_TEST(ret, "Tujian");

	ret = m_clsTech->Init(this);
	ERROR_LOAD_TEST(ret, "Tech");
	PROCESS_TIMER_END("tujian and tech init");

	PROCESS_TIMER_START
	ret = m_clsCardPacket->Load(GetdwAccountId());
	ERROR_LOAD_TEST(ret, "CardPacket");
	PROCESS_TIMER_END("cardpacket init");

	PROCESS_TIMER_START
	if (IS_THERE_ARENA(m_byFlag))
	{
		ret = m_clsArena->Init(this);
		ERROR_LOAD_TEST(ret, "Arena");
	}

	ret = m_CShop->Init(this);
	ERROR_LOAD_TEST(ret, "CShop");
	PROCESS_TIMER_END("arena and shop init");

	PROCESS_TIMER_START
//	IME_DEBUG("master now time %u", getMSTime());
	ret = m_CMaster->Init(this);
	ERROR_LOAD_TEST(ret, "CMaster");

//	IME_DEBUG("CDMission now time %u", getMSTime());
	ret = m_CDMission->Init(this);
	ERROR_LOAD_TEST(ret, "CDurationMission");
	PROCESS_TIMER_END("master and durationmission");

	PROCESS_TIMER_START
//	IME_DEBUG("CActive now time %u", getMSTime());
	ret = m_CActive->Init(this);
	ERROR_LOAD_TEST(ret, "CActive");
	PROCESS_TIMER_END("ACTIVE INIT");

	PROCESS_TIMER_START
//	IME_DEBUG("CRobotCtrl now time %u", getMSTime());
	ret = m_CRobotCtrl->Init(this);
	ERROR_LOAD_TEST(ret, "CRobotCtrl");
	PROCESS_TIMER_END("robot init");

	PROCESS_TIMER_START
//	IME_DEBUG("CTimerGift now time %u", getMSTime());
	ret = m_CTimerGift->Init(this);
	ERROR_LOAD_TEST(ret, "CTimerGift");
	PROCESS_TIMER_END("timergift");

	PROCESS_TIMER_START
	ret = m_CTrial->Init(this);
	ERROR_LOAD_TEST(ret, "CTrial");
	PROCESS_TIMER_END("trial init");

	PROCESS_TIMER_START
	ret = m_CTower->init(this);
	ERROR_LOAD_TEST(ret, "CTower");
	PROCESS_TIMER_END("tower init");


	//新账号初始化
	if (GetwLevel() == 0)
	{
		//新帐号
		//for test
//		SetdwExp(0);
//		SetdwCoin(1000);
//		SetdwDollar(1000);
//		m_clsCardPacket->InitCards(60001);
//
//		if (Calculate(false))
//		{
//			IME_ERROR("calculate fail");
//			return false;
//		}
//
//		m_dwEnergyCnt = time(NULL);
//		m_dwStrengthCnt = time(NULL);
//
//		SetdwEnergy(m_dwEnergyMax);
//		SetdwStrength(m_dwStrengthMax);
		// for test

	}
	else
	{
//		if (!CDBCtrl::GetUserRank(GetdwAccountId(), m_wMaxLevel, m_dwMaxAttack, m_dwMaxDefence))
//		{
//			IME_ERROR("get rank data fail userid %u", GetdwAccountId());
//			return false;
//		}

//		if (Calculate(false))
//		{
//			IME_ERROR("calculate fail");
//			return false;
//		}
		
//		IME_DEBUG("LoadProperty now time %u", getMSTime());
//		PROCESS_TIMER_START;
		if (LoadProperty())
		{
			IME_ERROR("load data fail userid %u", m_dwAccountId);
			return false;
		}
//		PROCESS_TIMER_END("LoadProperty");
//		PROCESS_TIMER_START;
		SetdwExp(m_dwOneLevelExp + m_dwLastExplimit);
//		IME_DEBUG("final now time %u", getMSTime());

		uint32_t now = time(NULL);
		uint32_t count = 0;
		if (now >= m_dwEnergyCnt)
		{
			count = (now - m_dwEnergyCnt) / m_dwEnergyRate;
			ChangeEnergy(count);
			m_dwEnergyCnt += m_dwEnergyRate * count;
		}
		
		if (now >= m_dwStrengthCnt)
		{
			count = (now - m_dwStrengthCnt) / m_dwStrengthRate;
			ChangeStrength(count);
			m_dwStrengthCnt += m_dwStrengthRate * count;
			IME_DEBUG("now %u, energy %u, strength %u", now, m_dwEnergyCnt, m_dwStrengthCnt); 
		}

		if (now >= m_dwStaminaTime)
		{
			count = (now - m_dwStaminaTime) / m_dwStaminaRate;
			ChangeStamina(count);
			m_dwStaminaTime += m_dwStaminaRate * count;
		}

		//group flag init
		CGroup *pCGroup = sWorld->GetCGroupByUserId(GetdwAccountId());
		if (pCGroup == NULL)
		{
			IME_DEBUG("before flag %u, group %llx", GetbyFlag(), GetlFindGroupId());
			SetbyFlag(CLOSE_GROUP(GetbyFlag()));
			SetlFindGroupId(0);
			IME_DEBUG("after flag %u, group %llx", GetbyFlag(), GetlFindGroupId());
		}
		else
		{
			IME_DEBUG("before flag %u, group %llx", GetbyFlag(), GetlFindGroupId());
			uint64 tmpid = MAKE_FIND_GROUPID(GetdwAccountId(), pCGroup->GetdwId());
			SetbyFlag(OPEN_GROUP(GetbyFlag()));
			SetlFindGroupId(tmpid);
			IME_DEBUG("after flag %u, group %llx", GetbyFlag(), GetlFindGroupId());
		}
		
		//card objid init id
		IME_DEBUG("before init id %u, update %u",m_dwInitNumber, m_clsCardPacket->GetMaxObjId());
		m_dwInitNumber = m_clsCardPacket->GetMaxObjId();
		IME_DEBUG("after init id %u", m_dwInitNumber);

		//add or del friend limit init calc
		if (!CUtil::IsToday(m_dwAddFriendTime))
		{
			m_dwAddFriend = 0;
			m_dwAddFriendTime = time(NULL);
		}
		if (!CUtil::IsToday(m_dwDelFriendTime))
		{
			m_dwDelFriend = 0;
			m_dwDelFriendTime = time(NULL);
		}
		
		if (!CUtil::IsToday(m_dwDelDMissionCntTime))
		{
			m_dwDelDMissionCnt = 0;
			m_dwDelDMissionCntTime = time(NULL);
		}

		if (!CUtil::IsToday(m_dwEnergyUseCntTime))
		{
			m_dwEnergyUseCnt = 0;
			m_dwEnergyUseCntTime = time(NULL);
		}

		if (!CUtil::IsToday(m_dwStrengthUseCntTime))
		{
			m_dwStrengthUseCnt = 0;
			m_dwStrengthUseCntTime = time(NULL);
		}

		if (!CUtil::IsToday(m_dwHighDelDMissionCntTime))
		{
			m_dwHighDelDMissionCnt = 0;
			m_dwHighDelDMissionCntTime = time(NULL);
		}
		
		if (!CUtil::IsToday(m_dwGiftDayTime))
		{
			std::map<int, SGift>::iterator it;
			for (it = m_mapGift.begin(); it != m_mapGift.end(); it++)
			{
				it->second.day = 0;
			}
			m_dwGiftDayTime = time(NULL);
		}
//		PROCESS_TIMER_END("time operate");
//		PROCESS_TIMER_START;

		m_dwGmFlushTime = CenterDBCtrl::HandlerGmCommandRole(this, m_dwGmFlushTime);
//		PROCESS_TIMER_END("centerdbctrl GM operate");
		PROCESS_TIMER_START;
		LoadFriendData();
		PROCESS_TIMER_END("loadfrienddata");
	}

	m_bIsInitOK = true;
	return true;
}

void CRole::SendLoginData()
{
	IME_LOG("Send Login Data[UserId=%u]", GetdwAccountId());

	INIT_PROCESS_TIMER;
//	PROCESS_TIMER_START;
	SendPlayerBaseInfo();
	PROCESS_TIMER_START;
    SendPlayerCardPacketInfo();
	PROCESS_TIMER_END("cardpacket online");
//	m_clsCardPacket->HandlerItemQuipmentInfo();
	m_clsItemPacket->HandlerItemInfo();
	CCombine::HandlerCheckAndInfo(this);
	m_clsTujian->HandlerCardTujianInfo();
	m_clsTech->HandlerTechInfo();
	CMail::HandlerEmailInfo(this);
//	SendPlayerRank();
	if (IS_THERE_GROUP(GetbyFlag()))
	{
		CGroup *pCGroup = sWorld->GetCGroup(GetlFindGroupId());
		if (pCGroup != NULL)
		{
			pCGroup->SendAllGroupData(this);
		}
	}
	GlobalConfig::SendData(this);	
	m_CMaster->HandlerMasterInfo();
	m_CDMission->HandlerDurationMissionInfo();
	m_CDMission->HandlerMissionBranchInfo();
	m_CDMission->HandlerHighDurationMissionInfo();
	m_CDMission->HandlerActiveDurationMissionInfo();
	m_CDMission->HandlerMissionDurationAllStarInfo(0);
	sWorld->GetCAnnouncement()->HandlerInfo(this);
	m_CActive->HandlerInfo();
	HandlerInfoOpt();
//	PROCESS_TIMER_END("1 data push");
//	PROCESS_TIMER_START
	sWorld->GetWorldBoss()->DataDump();
	if (IsInitRoleInfo())
	{
		CFriendBoss *pFb = (CFriendBoss *)(sWorld->GetWorldBoss()->GetData(GetdwAccountId()));
		if (pFb != NULL)
		{
			pFb->HandlerBossInfo(this);
			pFb->HandlerBossHarmHistoryInfo(this);
			//		IME_LOG("before boss prize");
			pFb->HandlerBossPrizeInfo(this);
			//		IME_LOG("boss prize");
			pFb->HandlerBossAttackedInfo(this);
			//		IME_LOG("boss attack");
		}
		m_CTrial->HandlerDataInfo();
	}
	sWorld->GetWorldBoss()->DataDump();
//	PROCESS_TIMER_END("friend boss push");
//	PROCESS_TIMER_START
	HandlerWorldBroadCastInfo();
	m_CTimerGift->HandlerInfo();
	CActiveCtrl::HandlerActiveBase(this, 1);
	CActiveCtrl::HandlerActiveInfo(this, 1);
	m_CMission->HandlerMissionInfo(0);
	m_CMission->HandlerMissionBaseInfo();
	m_CMission->HandlerMissionActiveInfo();
	//推送塔信息
	m_CTower->handlerTowerInfo();
	//推送排行榜
	m_CTower->handlerTowerqueryRank();
//	PROCESS_TIMER_END("2 data push");
	//todo
}

int CRole::LoadRole()
{

	//操作太危险了!!!
	//先查看有没有角色
	//E_SQL_RESULT_NO_ROLE,			//新角色
//	bool bIsNew = CDBCtrl::IsNewRole(m_strAccount);
	bool bIsNew = CDBCtrl::IsNewRole(m_dwAccountId);
	if (bIsNew)
	{
		return E_SQL_RESULT_NO_ROLE;
	}
	bool bIsSucc = CDBCtrl::GetUserBaseData(
//			m_wHeadPic,
			m_strUserName,
			m_wLevel,
			m_dwOneLevelExp,
			m_wVipLv,
			m_dwVipExp,
			m_dwDollar,
			m_dwCoin,
			m_dwEnergy,
			m_dwEnergyMax,
			m_dwEnergyRate,
			m_dwStrength,
			m_dwStrengthMax,
			m_dwStrengthRate,
//			m_dwCardAttributes,
//			m_dwAttackMin,
//			m_dwAttackMax,
//			m_dwDefenceMin,
//			m_dwDefenceMax,
//			m_dwAttackWin,
//			m_dwAttackTot,
//			m_dwDefenceWin,
//			m_dwDefenceTot,
			m_dwAccountId,
			m_dwInitNumber,
			m_byTaskLocation1,
			m_byTaskLocation2,
			m_byTaskLocation3,
			m_byTaskLocation4,
			m_byCardCapacity,
//			m_dwCardAttributes2,
//			m_byCurrentTeam,
			m_dwExplimit,
			m_dwEnergyCnt,
			m_dwStrengthCnt,
			m_byFlag,
			m_lFindGroupId,
			m_dwAvoidDurationTime,
			m_dwWorldHistory,
			m_dwWorldBroadTime,
			m_dwGameFlow,
			m_dwLuck,
			m_byGm,
			m_dwChargeLuck,
			m_dwStamina,
			m_dwStaminaTime,
			m_strSignature
			);

	/////

	if (!bIsSucc)
	{
		return E_SQL_RESULT_SQL_ERROR;
	}

//        SetwHeadPic(m_wHeadPic);
        SetstrUserName(m_strUserName);
        SetwLevel(m_wLevel);
        SetdwExp(0);
        SetwVipLv(m_wVipLv);
        SetdwVipExp(m_dwVipExp);
        SetdwDollar(m_dwDollar);
        SetdwCoin(m_dwCoin);
        SetdwEnergy(m_dwEnergy);
        SetdwEnergyMax(m_dwEnergyMax);
        SetdwEnergyRate(m_dwEnergyRate);
        SetdwStrength(m_dwStrength);
        SetdwStrengthMax(m_dwStrengthMax);
        SetdwStrengthRate(m_dwStrengthRate);
//        SetdwCardAttributes(m_dwCardAttributes);
//        SetdwAttackMin(m_dwAttackMin);
//        SetdwAttackMax(m_dwAttackMax);
//        SetdwDefenceMin(m_dwDefenceMin);
//        SetdwDefenceMax(m_dwDefenceMax);
//        SetdwAttackWin(m_dwAttackWin);
//        SetdwAttackTot(m_dwAttackTot);
//        SetdwDefenceWin(m_dwDefenceWin);
//        SetdwDefenceTot(m_dwDefenceTot);
        SetbyTaskLocation1(m_byTaskLocation1);
        SetbyTaskLocation2(m_byTaskLocation2);
        SetbyTaskLocation3(m_byTaskLocation3);
        SetbyTaskLocation4(m_byTaskLocation4);
        SetbyCardCapacity(m_byCardCapacity);
//        SetdwCardAttributes2(m_dwCardAttributes2);
        SetdwExplimit(m_dwExplimit);
//        SetbyCurrentTeam(m_byCurrentTeam);
//		SetdwAttribute(GetRealAttribute());
		SetbyFlag(GetbyFlag());
		SetlFindGroupId(m_lFindGroupId);
		SetdwAvoidDurationTime(m_dwAvoidDurationTime);
		SetdwWorldHistory(m_dwWorldHistory);
		SetdwFriendLimit(0);
		SetdwLastExplimit(0);
		SetdwGameFlow(m_dwGameFlow);
		SetdwLuck(m_dwLuck);
		SetbyGm(m_byGm);
		SetwNextVipLv(0);
		SetdwNextVipExp(0);
		SetdwChargeLuck(m_dwChargeLuck);
		SetdwStamina(m_dwStamina);
//		SetdwStaminaTime(m_dwStaminaTime);
		SetdwStaminaMax(0);
		SetdwStaminaRate(0);
		SetstrSignature(m_strSignature);

	return E_SQL_RESULT_SUCC;
}

//客户端GM
void CRole::OnGMCmd(std::string & strContent)
{
	CGmCmdPro::PraseCmd(this,strContent);
}

//////////////////////////////////////
void CRole::OnTimer1s()
{
	uint32_t now = time(NULL);
	if (IsInitRoleInfo())
	{
		if (m_dwEnergyCnt + m_dwEnergyRate <= now)
		{
			IME_DEBUG("now %u, energy %u, rate %u", now, m_dwEnergyCnt, m_dwEnergyRate);
			ChangeEnergy(1);
			m_dwEnergyCnt += m_dwEnergyRate;
			SendProUpdate();
			HandlerInfoOpt();
		}
		if (m_dwStrengthCnt + m_dwStrengthRate <= now)
		{
			IME_DEBUG("now %u, strength %u, rate %u", now, m_dwStrengthCnt, m_dwStrengthRate);
			ChangeStrength(1);
			m_dwStrengthCnt += m_dwStrengthRate;
			SendProUpdate();
			HandlerInfoOpt();
		}
		if (m_dwStaminaTime + m_dwStaminaRate <= now)
		{
			IME_DEBUG("now %u, stamina %u, rate %u", now, m_dwStaminaTime, m_dwStaminaRate);
			ChangeStamina(1);
			m_dwStaminaTime += m_dwStaminaRate;
			SendProUpdate();
			HandlerInfoOpt();
		}
	}
}

void CRole::OnTimer5s()
{
	if (IsInitRoleInfo())
	{
		if (IS_THERE_ARENA(m_byFlag))
		{
			m_clsArena->OnTimer5s();
		}
		m_CMaster->OnTimer5s();
		m_CDMission->OnTimer5s();
		m_CActive->OnTimer5s();
		CFriendBoss *pFb = (CFriendBoss *)(sWorld->GetWorldBoss()->GetData(GetdwAccountId()));
		if (pFb != NULL)
		{
			pFb->OnTimer5s();
		}
		if (!CUtil::IsToday(m_dwAddFriendTime))
		{
			m_dwAddFriend = 0;
			m_dwAddFriendTime = time(NULL);
		}
		if (!CUtil::IsToday(m_dwDelFriendTime))
		{
			m_dwDelFriend = 0;
			m_dwDelFriendTime = time(NULL);
		}
		if (!CUtil::IsToday(m_dwDelDMissionCntTime))
		{
			m_dwDelDMissionCnt = 0;
			m_dwDelDMissionCntTime = time(NULL);
		}
		if (!CUtil::IsToday(m_dwHighDelDMissionCntTime))
		{
			m_dwHighDelDMissionCnt = 0;
			m_dwHighDelDMissionCntTime = time(NULL);
		}

		if (!CUtil::IsToday(m_dwEnergyUseCntTime))
		{
			m_dwEnergyUseCnt = 0;
			m_dwEnergyUseCntTime = time(NULL);
		}

		if (!CUtil::IsToday(m_dwStrengthUseCntTime))
		{
			m_dwStrengthUseCnt = 0;
			m_dwStrengthUseCntTime = time(NULL);
		}
		if (!CUtil::IsToday(m_dwLowMasterTime))
		{
			m_dwLowMasterCount = 0;
			m_dwLowMasterTime = time(NULL);
		}
		if (!CUtil::IsToday(m_dwEnergyStrengthTime))
		{
			m_wUseEnergy = 0;
			m_wUseStrength = 0;
			m_wUseStamina = 0;
			m_dwEnergyStrengthTime = time(NULL);
		}

		if (!CUtil::IsToday(m_dwGiftDayTime))
		{
			std::map<int, SGift>::iterator it;
			for (it = m_mapGift.begin(); it != m_mapGift.end(); it++)
			{
				it->second.day = 0;
			}
			m_dwGiftDayTime = time(NULL);
		}

		if (!CUtil::IsToday(m_dwJoinFlushTime))
		{
			m_mapJoin.clear();
			m_dwTotalJoin = 0;
			m_dwJoinFlushTime = time(NULL);
		}
		m_CMission->OnTimer5s();
		m_CTrial->OnTimer5s();
		m_CTimerGift->OnTimer5s();
		m_CTower->onTime5s();
	}
}

void CRole::OnTimer3min()
{
	// save data
	SaveAllData();
	if (IsInitRoleInfo())
	{
		m_dwGmFlushTime = CenterDBCtrl::HandlerGmCommandRole(this, m_dwGmFlushTime);
	}
	
	LoadFriendData();

	//每隔3分钟推送塔的排行榜
	this->m_CTower->handlerTowerqueryRank();
}

void CRole::SaveDataForDuration()
{
	int exp = (int)m_dwExp - m_dwLastExplimit;
	exp = exp < 0 ? 0 : exp;
	m_dwOneLevelExp = exp;
	CDBCtrl::SaveUserBaseData(
//			m_wHeadPic,
			m_strUserName,
			m_wLevel,
			m_dwOneLevelExp,
			m_wVipLv,
			m_dwVipExp,
			m_dwDollar,
			m_dwCoin,
			m_dwEnergy,
			m_dwEnergyMax,
			m_dwEnergyRate,
			m_dwStrength,
			m_dwStrengthMax,
			m_dwStrengthRate,
//			m_dwCardAttributes,
//			m_dwAttackMin,
//			m_dwAttackMax,
//			m_dwDefenceMin,
//			m_dwDefenceMax,
//			m_dwAttackWin,
//			m_dwAttackTot,
//			m_dwDefenceWin,
//			m_dwDefenceTot,
			m_dwAccountId,
			m_dwInitNumber,
			m_byTaskLocation1,
			m_byTaskLocation2,
			m_byTaskLocation3,
			m_byTaskLocation4,
			m_byCardCapacity,
//			m_dwCardAttributes2,
//			m_byCurrentTeam,
			m_dwExplimit,
			m_dwEnergyCnt,
			m_dwStrengthCnt,
			m_byFlag,
			m_lFindGroupId,
			m_dwAvoidDurationTime,
			m_dwWorldHistory,
			m_dwWorldBroadTime,
			m_dwGameFlow,
			m_dwLuck,
			m_byGm,
			m_dwChargeLuck,
			m_dwStamina,
			m_dwStaminaTime,
			m_strSignature
			);
        int ret;
//        ret = m_clsCardPacket->SaveData();
//        ERROR_SAVE_TEST(ret, "CardPacket");

        ret = m_clsItemPacket->SaveData();
        ERROR_SAVE_TEST(ret, "ItemPacket");

//        ret = m_clsTech->DirtySaveData();
//        ERROR_SAVE_TEST(ret, "Tech");

		if (IS_THERE_ARENA(m_byFlag))
		{
			ret = m_clsArena->SaveData();
	        ERROR_SAVE_TEST(ret, "Arena");
		}

		OtherDataSave();
}

void CRole::statisticsRole()
{
	CenterDBCtrl::UpdateRoleGmAuth(m_dwAccountId, m_byGm);
}

void CRole::SaveAllData()
{
	if (!m_bIsInitOK)
		return ;
	int exp = (int)m_dwExp - m_dwLastExplimit;
	exp = exp < 0 ? 0 : exp;
	m_dwOneLevelExp = exp;
	CDBCtrl::SaveUserBaseData(
//			m_wHeadPic,
			m_strUserName,
			m_wLevel,
			m_dwOneLevelExp,
			m_wVipLv,
			m_dwVipExp,
			m_dwDollar,
			m_dwCoin,
			m_dwEnergy,
			m_dwEnergyMax,
			m_dwEnergyRate,
			m_dwStrength,
			m_dwStrengthMax,
			m_dwStrengthRate,
//			m_dwCardAttributes,
//			m_dwAttackMin,
//			m_dwAttackMax,
//			m_dwDefenceMin,
//			m_dwDefenceMax,
//			m_dwAttackWin,
//			m_dwAttackTot,
//			m_dwDefenceWin,
//			m_dwDefenceTot,
			m_dwAccountId,
			m_dwInitNumber,
			m_byTaskLocation1,
			m_byTaskLocation2,
			m_byTaskLocation3,
			m_byTaskLocation4,
			m_byCardCapacity,
//			m_dwCardAttributes2,
//			m_byCurrentTeam,
			m_dwExplimit,
			m_dwEnergyCnt,
			m_dwStrengthCnt,
			m_byFlag,
			m_lFindGroupId,
			m_dwAvoidDurationTime,
			m_dwWorldHistory,
			m_dwWorldBroadTime,
			m_dwGameFlow,
			m_dwLuck,
			m_byGm,
			m_dwChargeLuck,
			m_dwStamina,
			m_dwStaminaTime,
			m_strSignature
			);

	saveMultipleCardExp();//保存双倍经验丹

        int ret;
        ret = m_clsCardPacket->SaveData();
        ERROR_SAVE_TEST(ret, "CardPacket");
        ret = m_clsItemPacket->SaveData();
        ERROR_SAVE_TEST(ret, "ItemPacket");
        ret = m_clsTujian->DirtySaveData();
        ERROR_SAVE_TEST(ret, "Tujian");
        ret = m_clsTech->DirtySaveData();
        ERROR_SAVE_TEST(ret, "Tech");
		if (IS_THERE_ARENA(m_byFlag))
		{
			ret = m_clsArena->SaveData();
	        ERROR_SAVE_TEST(ret, "Arena");
		}
//		CDBCtrl::SaveUserRank(GetdwAccountId(), m_wMaxLevel, m_dwMaxAttack, m_dwMaxDefence);
        ret = m_CShop->SaveData();
        ERROR_SAVE_TEST(ret, "Shop");
        m_CMaster->SaveData();
		m_CDMission->SaveData();
		m_CRobotCtrl->SaveData();
		OtherDataSave();	
		m_CTimerGift->SaveData();
		m_CActive->SaveAllData();
		m_CMission->SaveData();
		m_CTrial->SaveData();
		//塔数据保存
		m_CTower->saveData();

}

void CRole::Offline()
{
	IME_LOG("PLAYER OFFLINE[UserId=%u]", GetdwAccountId());
	if (!m_bIsInitOK)
		return ;
	// 用户离开游戏
	sWorld->DelUserAuth(GetdwAccountId());

	// 保存所有用户数据
	SaveAllData();

	// 最近一次离开游戏时间记录
	if (m_dwUserOnlineTime != 0)
	{
		uint32_t last_login_time, last_logout_time, cur_login_time, login_day;
		std::string last_login_ip;
		if (CDBCtrl::GetUserLoginData(m_dwAccountId, last_login_time, last_logout_time, cur_login_time, login_day, last_login_ip))
		{
			last_logout_time = time(NULL);
			cur_login_time = time(NULL) - m_dwUserOnlineTime;
			CDBCtrl::SaveUserLoginData(m_dwAccountId, last_login_time, last_logout_time, cur_login_time, login_day, last_login_ip);
		}
	}
	//增加战斗力和师傅和弟子等级
	std::vector<CCard *> cardVector;
	GetclsCardPacket()->GetEquippCards(cardVector);
	std::string cardLevel = "";
	for(int i = 0; i < cardVector.size(); i++){
		char temp[10];
		sprintf(temp,"%d",cardVector[i]->GetwLevel());
		if("" == cardLevel){
			cardLevel += temp;
		}else{
			cardLevel.append(",").append(temp);
		}
	}
	for(int i = cardVector.size(); i < MAX_CARD_CAPACITY + 1; i++){
		if("" == cardLevel){
			cardLevel.append("0");
		}else{
			cardLevel.append(",").append("0");
		}
	}
	CenterDBCtrl::InsertOrUpdateRoleInfo(m_dwAccountId, m_strUserName, m_byGm, 0, m_dwGameFlow, m_wLevel, m_dwCoin, m_dwDollar, m_CDMission->GetMissionPosition() , 0, m_wVipLv, m_dwVipExp, m_dwStrength, m_dwEnergy, 0,GetPower(),cardLevel);
	CenterDBCtrl::InsertLogoutInfo(m_loginauto);
	//上次登录时间

}

void CRole::Online()
{
//	INIT_PROCESS_TIMER;
//	PROCESS_TIMER_START;
	if (!m_bIsInitOK)
		return ;
	// 用户登录游戏
	sWorld->AddUserAuth(GetdwAccountId(), GetSession());

	// 最近一次登录游戏时间
	uint32_t last_login_time, last_logout_time, cur_login_time, login_day;
	std::string last_login_ip;
	if (CDBCtrl::GetUserLoginData(m_dwAccountId, last_login_time, last_logout_time, cur_login_time, login_day, last_login_ip))
	{
		m_dwUserOnlineTime = time(NULL);
		m_dwLastLoginTime = m_dwUserOnlineTime; 
		if (last_login_time == 0)
		{
			login_day = 1;
			m_byFirstLogin = 1;
		}
		else
		{
			uint32_t lastt = CUtil::GetDateTime(last_login_time + 24 * 3600);
			uint32_t nextt = CUtil::GetDateTime(time(NULL));
//			IME_ERROR("lastt %u, nextt %u", lastt, nextt);

			if (lastt == nextt)
			{
				login_day += 1;
				m_byFirstLogin = 1;
			}
			else if (lastt < nextt)
			{
				m_byFirstLogin = 1;
				login_day = 0;
			}
			else
			{
				// now login	
				m_byFirstLogin = 0;
			}
		}
		last_login_time = time(NULL);
		last_logout_time = 0;
		last_login_ip = m_pSession->GetPlayersession()->GetRemoteIP();
		CDBCtrl::SaveUserLoginData(m_dwAccountId, last_login_time, last_logout_time, cur_login_time, login_day, last_login_ip);
	}
//	PROCESS_TIMER_END("get login data");

//	PROCESS_TIMER_START;
	m_CMission->ContinueLogin(0, login_day);
//	PROCESS_TIMER_END("mission continue login");
	
	// 用户登录
//	PROCESS_TIMER_START;
	SendLoginData();
//	PROCESS_TIMER_END("total data push");
//	PROCESS_TIMER_START;

	std::vector<CCard *> cardVector;
	GetclsCardPacket()->GetEquippCards(cardVector);
	std::string cardLevel = "";
//    if(cardVector.size() == 0){
//        cardLevel = "0,0,0,0,0,0,0";
//    }
	for (int i = 0; i < cardVector.size(); i++) {
		char temp[10];
		sprintf(temp, "%d", cardVector[i]->GetwLevel());
		if (cardLevel == "") {
			cardLevel += temp;
		}else{
			cardLevel.append(",").append(temp);
		}
	}
	for (int i = cardVector.size(); i < MAX_CARD_CAPACITY + 1; i++) {
		if ("" == cardLevel) {
			cardLevel.append("0");
		} else {
			cardLevel.append(",").append("0");
		}
	}
	CenterDBCtrl::InsertOrUpdateRoleInfo(m_dwAccountId, m_strUserName, m_byGm, 0, m_dwGameFlow, m_wLevel, m_dwCoin, m_dwDollar, m_CDMission->GetMissionPosition() , 0, m_wVipLv, m_dwVipExp, m_dwStrength, m_dwEnergy, 0,GetPower(),cardLevel);
	CenterDBCtrl::UpdateRoleLastLoginTime(m_dwAccountId);
	m_loginauto = CenterDBCtrl::InsertLoginInfo(m_dwAccountId, m_pSession->GetPlayersession()->GetRemoteIP(), m_pSession->GetLoginDevice(), m_pSession->GetLoginDeviceType());

	if (m_wVipLv >= 7)
	{
		sWorld->GetCNotice()->VipOnline(GetstrUserName(), GetdwAccountId(), GetwVipLv());
	}
	
	if (m_byFirstLogin)
	{
		CDBCtrl::stuMailDetail tmp = {0};
		uint32_t mail_id;
		CMail::SBaseAttr tmpAttr = {0};
		tmpAttr.coin = GlobalConfig::EverydayLoginCoin;
		tmpAttr.worldhistory = GlobalConfig::EverydayLoginWorldExp;
		CMail::AddEmailSysGift(m_dwAccountId, mail_id, tmp, 0, 0, 0, GlobalConfig::LoginRewardStr ,tmpAttr);  	
		CMail::EmailUpdate(this, CMail::ADD, &tmp, 0);
		STC_CONF_VIP *pVipConf = CConfVip::Find(GetwVipLv());
		if (pVipConf == NULL)
		{
			IME_ERROR("vip lv %u can not find in vip csv", GetwVipLv());
		}
		else
		{
			uint32_t everyluck = pVipConf->dwLuckEveryGet; 
			uint32_t maxluck = pVipConf->dwLuckMax;

			uint32_t tmpluck = m_dwLuck + everyluck;
			if (m_dwLuck < maxluck)
			{
				m_dwLuck = tmpluck;
				if (m_dwLuck > maxluck)
					m_dwLuck = maxluck;
				SetdwLuck(m_dwLuck);
			}
		}
	}

	//最后时候再发送验证成功，保证所有数据先到达客户端
//	if (GetwLevel())
//	{
	      m_pSession->SendAuthInfo(E_ERRNO_AUTH_SUCCESS);
//	}
	IME_LOG("user online useid %u", m_dwAccountId);
//	PROCESS_TIMER_END("calculate login data");
	m_CShop->HandlerShopInfo();
}

void CRole::TestScriptByName(const std::string & strName)
{
//	CLuaScript * pLuaScript = CLuaCtrl::FindScript("Role.lua");
//	if (!pLuaScript)
//		return ;
//	if (!pLuaScript->Call(strName.c_str(),"e","CRole",this))
//	{
//		IME_LOG("Role Script Func Name:%s call failed",strName.c_str());
//	}
}

////////////////////////////发送系列////////////////////////////////////////////
void CRole::SendPacket(WorldPacket * packet)
{
	m_pSession->SendPacket(packet);
}

void CRole::SendMsgStr(const std::string& Msg)
{
        WorldPacket data(CMD_SC_MSG_STR);
        data << Msg;
        SendPacket(&data);
}

void CRole::SendErrorMsg(uint16 ErrorMsgNum, uint16 wCmd)
{
	// 命令号存在，则返回包用该命令号，否则用错误码专用命令号
	uint16 wCurCmd;
	if (wCmd)
	{
		wCurCmd = wCmd;
	}
	else
	{
		wCurCmd = CMD_SC_ERROR_MSG;
	}
	WorldPacket pkg(wCurCmd);
	pkg << ErrorMsgNum;
	SendPacket(&pkg);
}

void CRole::SendProUpdate()
{
	WorldPacket data(CMD_SC_ROLE_PROPERTY);
	saveDataDirty(data);
//	IME_DEBUG("data size %u", (uint32_t)data.size());
	clearDirtyFlag();
	if (data.size() > 1)
	{
		SendPacket(&data);
	}
}

void CRole::SendErrorStr(const std::string& strMsg)
{
	WorldPacket pkg(CMD_SC_ERROR_STR);
	pkg << strMsg;
	SendPacket(&pkg);
}

void CRole::SendPlayerBaseInfo()
{
	//CMD_SC_ROLE_BASE_INFO

	std::vector<unsigned char> vecEnums;
	vecEnums.push_back(E_ROLE_dwAccountId);
	vecEnums.push_back(E_ROLE_strAccount);
	vecEnums.push_back(E_ROLE_strUserName);
	vecEnums.push_back(E_ROLE_wLevel);
	vecEnums.push_back(E_ROLE_dwExp);
	vecEnums.push_back(E_ROLE_wVipLv);
	vecEnums.push_back(E_ROLE_dwVipExp);
	vecEnums.push_back(E_ROLE_dwDollar);
	vecEnums.push_back(E_ROLE_dwCoin);
	vecEnums.push_back(E_ROLE_dwEnergy);
	vecEnums.push_back(E_ROLE_dwEnergyMax);
	vecEnums.push_back(E_ROLE_dwEnergyRate);
	vecEnums.push_back(E_ROLE_dwStrength);
	vecEnums.push_back(E_ROLE_dwStrengthMax);
	vecEnums.push_back(E_ROLE_dwStrengthRate);
	vecEnums.push_back(E_ROLE_byTaskLocation1);
	vecEnums.push_back(E_ROLE_byTaskLocation2);
	vecEnums.push_back(E_ROLE_byTaskLocation3);
	vecEnums.push_back(E_ROLE_byTaskLocation4);
	vecEnums.push_back(E_ROLE_byCardCapacity);
	vecEnums.push_back(E_ROLE_dwExplimit);
	vecEnums.push_back(E_ROLE_dwLastExplimit);
	vecEnums.push_back(E_ROLE_byFlag);
	vecEnums.push_back(E_ROLE_lFindGroupId);
	vecEnums.push_back(E_ROLE_dwAvoidDurationTime);
	vecEnums.push_back(E_ROLE_dwWorldHistory);
	vecEnums.push_back(E_ROLE_dwFriendLimit);
	vecEnums.push_back(E_ROLE_dwGameFlow);
	vecEnums.push_back(E_ROLE_dwLuck);
	vecEnums.push_back(E_ROLE_byGm);
	vecEnums.push_back(E_ROLE_wNextVipLv);
	vecEnums.push_back(E_ROLE_dwNextVipExp);
	vecEnums.push_back(E_ROLE_dwChargeLuck);
	vecEnums.push_back(E_ROLE_dwStamina);
	vecEnums.push_back(E_ROLE_dwStaminaMax);
	vecEnums.push_back(E_ROLE_dwStaminaRate);
	vecEnums.push_back(E_ROLE_strSignature);

	WorldPacket pkg(CMD_SC_ROLE_BASE_INFO);
	saveDataNeed(pkg, vecEnums);
	clearDirtyFlag();
	SendPacket(&pkg);
}

//void CRole::PackForTeam(WorldPacket &info)
//{
//	info << GetdwAttackMin();
//	info << GetdwAttackMax();
//	info << GetdwDefenceMin();
//	info << GetdwDefenceMax();
//}

void CRole::SendPlayerCardPacketInfo()
{
    m_clsCardPacket->HandlerBaseInfo();
}

int CRole::LoadProperty()
{
	STC_CONF_ROLE *p,*q;
	p = CConfRole::Find(m_wLevel);
	if (p == NULL)
		return -1;

	SetdwExplimit(p->dwExpLimit);
	if (m_wLevel - 1 <= 0)
	{
		SetdwLastExplimit(0);
	}
	else
	{
		q = CConfRole::Find(m_wLevel - 1);
		if (q == NULL)
			return -1;
		SetdwLastExplimit(q->dwExpLimit);
	}
	SetdwStaminaRate(p->dwStaminaRate);		
	SetdwStaminaMax(p->dwStaminaLimit);

	uint16_t friendLimit = p->dwFriendLimit;
	STC_CONF_VIP *pVip = CConfVip::Find(GetwVipLv());
	if (pVip == NULL)
	{
		IME_ERROR("can not find vip index %u record in csv", GetwVipLv());
	}
	else
	{
		friendLimit += pVip->dwVipFriendLimit;
	}
	SetdwFriendLimit(friendLimit);

	if (GetbyCardCapacity() >= MAX_CARD_CAPACITY)
	{
		SetbyCardCapacity(MAX_CARD_CAPACITY);
	}

	IME_DEBUG("vip lv %u, vip exp %u", GetwVipLv(), GetdwVipExp());
	STC_CONF_VIP *pConfVip = CConfVip::Find(GetwVipLv());
	if (pConfVip == NULL)
		return -1;
	if (GetwVipLv() >= CConfVip::MaxVipLv())
	{
		SetwNextVipLv(-1);
		SetdwNextVipExp(-1);
		m_dwCurrentDollarRate = pConfVip->dwDollarExtraRate;
		m_dwNextDollarRate = pConfVip->dwDollarExtraRate;
	}
	else
	{
		SetwNextVipLv(GetwVipLv() + 1);
		SetdwNextVipExp(pConfVip->dwVipExp);
		m_dwCurrentDollarRate = pConfVip->dwDollarExtraRate;
		m_dwNextDollarRate = pConfVip->dwDollarExtraRate;
		STC_CONF_VIP *pConfNextVip = CConfVip::Find(GetwVipLv());
		if (pConfNextVip != NULL)
		{
			m_dwNextDollarRate = pConfNextVip->dwDollarExtraRate;
		}
	}

	if (m_CMission != NULL)
	{
		m_CMission->RoleLevel(0, GetwLevel());
	}

	IME_DEBUG("after vip lv %u, vip exp %u", GetwNextVipLv(), GetdwNextVipExp());
	return 0;
}

int CRole::Calculate(bool IsUpdate)
{

//	todo some lv up 
	if (CalExp())
	{
		return -1;
	}

	while(1)
	{	
		if (GetwVipLv() >= CConfVip::MaxVipLv())
			break;
		STC_CONF_VIP *pConfVip = CConfVip::Find(GetwVipLv());
		if (pConfVip == NULL)
		{
			IME_ERROR("can not find vip lv %u", GetwVipLv());
			return -1;
		}
		IME_DEBUG("vip exp %u lv %u, addexp %u", GetdwVipExp(), GetwVipLv(), pConfVip->dwVipExp);
		if (GetdwVipExp() >= pConfVip->dwVipExp)
		{
			ChangeVipExp(-pConfVip->dwVipExp);
			SetwVipLv(GetwVipLv() + 1);			
			IME_DEBUG("vip exp %u lv %u, addexp %u", GetdwVipExp(), GetwVipLv(), pConfVip->dwVipExp);
		}
		else
		{
			IME_DEBUG("vip exp %u lv %u, addexp %u", GetdwVipExp(), GetwVipLv(), pConfVip->dwVipExp);
			break;
		}
	}

	STC_CONF_VIP *pConfVip = CConfVip::Find(GetwVipLv());
	if (pConfVip == NULL)
		return -1;
	if (GetwVipLv() >= CConfVip::MaxVipLv())
	{
		SetwNextVipLv(-1);
		SetdwNextVipExp(-1);
		m_dwCurrentDollarRate = pConfVip->dwDollarExtraRate;
		m_dwNextDollarRate = pConfVip->dwDollarExtraRate;
	}
	else
	{
		SetwNextVipLv(GetwVipLv() + 1);
		SetdwNextVipExp(pConfVip->dwVipExp);
		m_dwCurrentDollarRate = pConfVip->dwDollarExtraRate;
		m_dwNextDollarRate = pConfVip->dwDollarExtraRate;
		STC_CONF_VIP *pConfNextVip = CConfVip::Find(GetwVipLv());
		if (pConfNextVip != NULL)
		{
			m_dwNextDollarRate = pConfNextVip->dwDollarExtraRate;
		}
	}

//	if (m_wLevel != 0)
//	{
//		STC_CONF_ROLE *p;
//		p = CConfRole::Find(m_wLevel);
//		if (p == NULL)
//			return -1;
//		SetdwFriendLimit(p->dwFriendLimit);
//	}
//	if (CalTech())
//	{
//		return -1;
//	}

	if (GetbyCardCapacity() >= MAX_CARD_CAPACITY)
	{
		SetbyCardCapacity(MAX_CARD_CAPACITY);
	}

	IME_DEBUG("exp %u lv %u lastexplimit %u explimit %u", m_dwExp, m_wLevel, GetdwLastExplimit(), m_dwExplimit);	
	if (IsUpdate)
	{
		SendProUpdate();	
//		HandlerInfoOpt();
	}
	return 0;
}

int CRole::AddExp(uint32_t exp)
{
	if (m_dwExp < CConfRole::GetMaxExp())
	{
		if (m_dwExp + exp > CConfRole::GetMaxExp())
		{
			IME_DEBUG("exp overload max exp");
			m_dwExp = CConfRole::GetMaxExp();
		}
		else
		{
			m_dwExp += exp;
		}
	}
	else if (m_dwExp == CConfRole::GetMaxExp())
	{

	}
	else 
	{
		IME_ERROR("user exp %u is bigger than maxexp %u", m_dwExp, CConfRole::GetMaxExp());
		return -1;
	}
	SetdwExp(m_dwExp);
	return 0;
}

int CRole::ChangeEnergy(int32_t energy)
{
//	if (m_dwEnergy == m_dwEnergyMax)
//		return 0;
//	if (m_dwEnergy <= m_dwEnergyMax)
//	{
		int32_t temp = m_dwEnergy + energy;
		if (energy < 0)
		{
			if (temp < 0)
				m_dwEnergy = 0;
			else
				m_dwEnergy = temp;
		}
		else
		{
			if (m_dwEnergy < m_dwEnergyMax)
			{
				if (temp > m_dwEnergyMax)
				{
					m_dwEnergy = m_dwEnergyMax;
				}
				else
				{
					m_dwEnergy = m_dwEnergy + energy;
				}
			}
		}
//	}
//	IME_LOG("energy %u del %d", GetdwEnergy(), energy);
	SetdwEnergy(m_dwEnergy);
	return 0;
}

int CRole::ChangeStrength(int32_t strength)
{
//	if (m_dwStrength == m_dwStrengthMax)
//		return 0;
//	if (m_dwStrength < m_dwStrengthMax)
//	{
		int32_t temp = m_dwStrength + strength;
		if (strength < 0)
		{
			if (temp < 0)
				m_dwStrength = 0;
			else
				m_dwStrength = temp;

		}
		else
		{
			if (m_dwStrength < m_dwStrengthMax)
			{
				if (temp > m_dwStrengthMax)
				{
					m_dwStrength = m_dwStrengthMax;
				}
				else
				{
					m_dwStrength = m_dwStrength + strength;
				}
			}
		}
//	}
	SetdwStrength(m_dwStrength);
	return 0;
}

int CRole::CalExp(bool IsUpdate)
{
//	IME_ERROR("cal before role explimit %u, lastexplimit %u, exp %u", m_dwExplimit, m_dwLastExplimit, m_dwExp);
	uint16_t wLevel = 0;
	wLevel = CConfRole::FindLevel(m_dwExp);
//	IME_ERROR("level %u", wLevel);
	if (wLevel < m_wLevel)
	{
		IME_ERROR("level up reach level small than now level it is csv error");
		return -1;
	}
	STC_CONF_ROLE *p,*q;
	p = CConfRole::Find(wLevel);
	if (p == NULL)
		return -1;

	if (wLevel - 1 <= 0)
	{
		SetdwLastExplimit(0);
	}
	else
	{
		q = CConfRole::Find(wLevel - 1);
		if (q == NULL)
			return -1;
		SetdwLastExplimit(q->dwExpLimit);
	}
	uint16_t friendLimit = p->dwFriendLimit;
	STC_CONF_VIP *pVip = CConfVip::Find(GetwVipLv());
	if (pVip == NULL)
	{
		IME_ERROR("can not find vip index %u record in csv", GetwVipLv());
	}
	else
	{
		friendLimit += pVip->dwVipFriendLimit;
	}
	SetdwFriendLimit(friendLimit);
//	IME_ERROR("role explimit %u, lastexplimit %u, exp %u", m_dwExplimit, m_dwLastExplimit, m_dwExp);

	if (wLevel != m_wLevel)
	{
		SetwLevel(wLevel);
		SetdwExplimit(p->dwExpLimit);
		uint32_t energymax = p->dwEnergyLimit;
//		GetclsTech()->TechEnergy(energymax);
		SetdwEnergyMax(energymax);
		if (GetdwEnergy() < GetdwEnergyMax())
		{
			SetdwEnergy(GetdwEnergyMax());
		}
		SetdwStrengthMax(p->dwStrengthLimit);
		if (GetdwStrength() < GetdwStrengthMax())
		{
			SetdwStrength(GetdwStrengthMax());
		}
		SetdwEnergyRate(p->dwEnergyRate);
		SetdwStrengthRate(p->dwStrengthRate);
		SetbyCardCapacity(p->byCardCapacity);
		IME_DEBUG("energy %u, energylimit %u friend limit %u", m_dwEnergy, m_dwEnergyMax, m_dwFriendLimit);
		SetdwStaminaMax(p->dwStaminaLimit);
		SetdwStaminaRate(p->dwStaminaRate);

//		SetRankLevel(wLevel);

//		m_dwAddFriend = 0;
//		m_dwDelFriend = 0;

	}

	if (m_CMission != NULL)
	{
		m_CMission->RoleLevel(0, GetwLevel());
	}

	if (IsUpdate)
		SendProUpdate();
	return 0;
}

//int CRole::CalTech(bool IsUpdate)
//{
//	uint16_t wLevel;
//	wLevel = CConfRole::FindLevel(m_dwExp);
//	STC_CONF_ROLE *p;
//	p = CConfRole::Find(wLevel);
//	if (p == NULL)
//		return -1;
//
//	uint32_t energymax = p->dwEnergyLimit;
//	GetclsTech()->TechEnergy(energymax);
//	SetdwEnergyMax(energymax);
//
//	if (IsUpdate)
//		SendProUpdate();
//	return 0;
//}

int CRole::ChangeCoin(int32_t coin, int source)
{
	if (coin == 0) return 0;
	if (m_dwCoin + coin < 0 )
	{
		SetdwCoin(0);
	}
	else
	{
		SetdwCoin(m_dwCoin + coin);
	}
	if (coin >= 0)
	{
		IME_CHAR_LOG("resource [status=success,res=coin,opt=add,character=%u,index=%u,number=%d,reason=ok,source=%d]", GetdwAccountId(), 0, coin, source);
	}
	else
	{
		IME_CHAR_LOG("resource [status=success,res=coin,opt=del,character=%u,index=%u,number=%d,reason=ok,source=%d]", GetdwAccountId(), 0, -coin, source);
	}
	return 0;
}

int CRole::ChangeWorldHistory(int32_t exp, int source)
{
	if (exp == 0) return 0;
	if (m_dwWorldHistory + exp < 0 )
	{
		SetdwWorldHistory(0);
	}
	else
	{
		SetdwWorldHistory(m_dwWorldHistory + exp);
	}
	if (exp >= 0)
	{
		IME_CHAR_LOG("resource [status=success,res=worldexp,opt=add,character=%u,index=%u,number=%d,reason=ok,source=%d]", GetdwAccountId(), 0, exp, source);
	}
	else
	{
		IME_CHAR_LOG("resource [status=success,res=worldexp,opt=del,character=%u,index=%u,number=%d,reason=ok,source=%d]", GetdwAccountId(), 0, -exp, source);
	}
	return 0;
}

int CRole::ChangeDollar(int32_t dollar, int source)
{
	if (dollar == 0) return 0;
//	IME_LOG("add dollar %u", dollar);
	if (dollar < 0)
	{
		if (GetdwDollar() < -dollar)
		{
			IME_CHAR_LOG("resource [status=fail,res=dollar,opt=del,character=%u,index=%u,number=%d,reason=not enough,source=%d]", GetdwAccountId(), 0, dollar, source);
			return -1;
		}
		SetdwDollar(GetdwDollar() + dollar);
	}
	else
	{
		SetdwDollar(GetdwDollar() + dollar);
	}
	//todo some DB record
	if (dollar >= 0)
	{
		IME_CHAR_LOG("resource [status=success,res=dollar,opt=add,character=%u,index=%u,number=%d,reason=ok],source=%d", GetdwAccountId(), 0, dollar, source);
	}
	else
	{
		IME_CHAR_LOG("resource [status=success,res=dollar,opt=del,character=%u,index=%u,number=%d,reason=ok],source=%d", GetdwAccountId(), 0, -dollar, source);
	}
	return 0;
}

int CRole::ChangeDollarWithExtra(int32_t &dollar, int source)
{
	if (dollar == 0) return 0;
//	IME_LOG("add dollar %u", dollar);
	if (dollar < 0)
	{
		if (GetdwDollar() < -dollar)
		{
			IME_CHAR_LOG("resource [status=fail,res=dollar,opt=del,character=%u,index=%u,number=%d,reason=not enough],source=%d", GetdwAccountId(), 0, dollar, source);
			return -1;
		}
		SetdwDollar(GetdwDollar() + dollar);
	}
	else
	{
		STC_CONF_VIP *pConf = CConfVip::Find(GetwVipLv());
		if (pConf != NULL)
		{
			IME_DEBUG("after dollar %u, vip lv %u, get extra dollar %u, dot dollar %u", dollar, GetwVipLv(), GetdwGetExtraDollar(), GetdwDotDollar());
			uint32_t orgin = dollar * 100;
			uint32_t expanddollar = orgin;
			expanddollar = (pConf->dwDollarExtraRate + 100) / 100.0 * expanddollar;
			SetdwGetExtraDollar(GetdwGetExtraDollar() + (expanddollar - orgin) / 100);
			dollar = (expanddollar + GetdwDotDollar()) / 100;
			SetdwDotDollar((expanddollar + GetdwDotDollar()) % 100);
			IME_DEBUG("after dollar %u, vip lv %u, get extra dollar %u, dot dollar %u", dollar, GetwVipLv(), GetdwGetExtraDollar(), GetdwDotDollar());
		}
		else
		{
			IME_ERROR("can not find vip level index %u in vip csv", GetwVipLv()); 
		}
		SetdwDollar(GetdwDollar() + dollar);
	}
	//todo some DB record
	if (dollar >= 0)
	{
		IME_CHAR_LOG("resource [status=success,res=dollar,opt=add,character=%u,index=%u,number=%d,reason=ok],source=%d", GetdwAccountId(), 0, dollar, source);
	}
	else
	{
		IME_CHAR_LOG("resource [status=success,res=dollar,opt=del,character=%u,index=%u,number=%d,reason=ok],source=%d", GetdwAccountId(), 0, -dollar, source);
	}
	return 0;
}

int CRole::ChangeVipExp(int32_t exp)
{
	if (exp < 0)
	{
		if (GetdwVipExp() < -exp)
		{
			return -1;
		}
		SetdwVipExp(GetdwVipExp() + exp);
		return 0;
	}
	else
	{
		uint32_t maxexp = CConfVip::MaxVipExp(GetwVipLv());
		if (GetdwVipExp() < maxexp) 
		{
			uint32_t vipexp = GetdwVipExp() + exp;
			if (vipexp > maxexp)
				vipexp = maxexp;
			SetdwVipExp(vipexp);
		}
		return 0;
	}
	//todo some DB record
}

int CRole::PrepareDurationData(std::vector<cardforduration_t> &vec, std::vector<uint32_t> &vecline)
{
//	if (GetclsCardPacket()->CopyDataForDuration())
//	{
//		IME_ERROR("copy data for duration fail");
//		return -1;
//	}
//	std::vector<CDuration::cardforduration_t> *p = GetclsCardPacket()->GetVecForDuration();
//
//	vec.clear();
//	vec = *p;
//	if (CDuration::SetVecForDuration(*p))
//	{
//		IME_ERROR("set vec forduration fail %u", GetdwAccountId());
//		return -1;
//	}
//	CDuration::SetAttributeDuration(GetRealAttribute());
	std::vector<int> vecRelation;
	int ret = GetclsCardPacket()->PrepareDurationData(vec, vecline, vecRelation);
	if (ret)
	{
		return -1;
	}
	CDuration::SetStrUserName(GetstrUserName());
	uint32_t vipharm = 0;
	STC_CONF_VIP *pVip = CConfVip::Find(GetwVipLv());
	if (pVip != NULL)
		vipharm = pVip->dwAttackBuff;
	CDuration::SetVipExtraHarm(vipharm);
	CDuration::SetRelationIndex(vecRelation);
	return 0;
}

void CRole::HandlerArenaOpen(WorldPacket &pkg)
{
	IME_CHAR_LOG("action [userid %u arena open and get data]", GetdwAccountId());
	if (!IS_THERE_ARENA(m_byFlag))
	{
//		if (GetwLevel() < GlobalConfig::ArenaOpen )
//		{
//			WorldPacket info(CMD_SC_ARENA_OPEN_RESULT);
//			info << (uint16_t) ERRNO_MSG_ARENA_LEVEL_LIMIT;
//			info << (uint16_t) GlobalConfig::ArenaOpen;
//			SendPacket(&info);
//			return;
//		}
		
		m_clsArena = new CArena();
		assert(m_clsArena != NULL);
		if (m_clsArena->CreateInitData(this))
		{
			SendErrorMsg(ERRNO_MSG_ARENA_INIT, CMD_SC_ARENA_OPEN_RESULT);
			delete m_clsArena;
			m_clsArena = NULL;
			return;
		}
		m_clsArena->SetdwPrestige(GlobalConfig::InitPrestige);
		SetbyFlag(OPEN_ARENA(m_byFlag));
	}

	m_clsArena->HandlerProUpdate();
	m_clsArena->HandlerTopUpdate();
//	m_clsArena->HandlerLoseUpdate();
	m_clsArena->HandlerChallengeUpdate();
//	m_clsArena->HandlerArenaRankProduct();
//	m_clsArena->HandlerArenaPrestigeProduct();

	WorldPacket info(CMD_SC_ARENA_OPEN_RESULT);
	info << (uint16) 0;
	SendPacket(&info);
}

//void CRole::SendPlayerRank()
//{
//	std::vector<CDBCtrl::SRank> vec;
//	WorldPacket info(CMD_SC_RANK_RESULT);
//	if (!CDBCtrl::GetUserRankLevel(GetdwAccountId(), vec))
//	{
//		IME_ERROR("get level fail userid %u", GetdwAccountId());
//		return;
//	}
//	info << (uint8_t)vec.size();
//	for(int i = 0; i < vec.size(); i++)
//	{
//		info << (uint32_t) vec[i].dwUserId;
//		info << vec[i].strName;
//		info << (uint32_t) vec[i].dwVal;
//	}
//	if (!CDBCtrl::GetUserRankAttack(GetdwAccountId(), vec))
//	{
//		IME_ERROR("get level fail userid %u", GetdwAccountId());
//		return;
//	}
//	info << (uint8_t)vec.size();
//	for(int i = 0; i < vec.size(); i++)
//	{
//		info << (uint32_t) vec[i].dwUserId;
//		info << vec[i].strName;
//		info << (uint32_t) vec[i].dwVal;
//	}
//	if (!CDBCtrl::GetUserRankDefence(GetdwAccountId(), vec))
//	{
//		IME_ERROR("get level fail userid %u", GetdwAccountId());
//		return;
//	}
//	info << (uint8_t)vec.size();
//	for(int i = 0; i < vec.size(); i++)
//	{
//		info << (uint32_t) vec[i].dwUserId;
//		info << vec[i].strName;
//		info << (uint32_t) vec[i].dwVal;
//	}
//	SendPacket(&info);
//}


//void
//CRole::HandlerGroupAdd (WorldPacket &pkg)
//{
//	return ;
//}		/* -----  end of method CRole::HandlerGroupAdd  ----- */


void
CRole::HandlerGroupCreate (WorldPacket &pkg)
{
	std::string groupname;
	pkg >> groupname;
	if (IS_THERE_GROUP(GetbyFlag()))
	{
		SendErrorMsg(ERRNO_MSG_ALREADY_IN_GROUP, CMD_SC_GROUP_CREATE_RESULT);			
		return;
	}
	if (GetwLevel() < GlobalConfig::CreateGroupLimit)
	{
		SendErrorMsg(ERRNO_MSG_GROUP_LEVEL_LIMIT, CMD_SC_GROUP_CREATE_RESULT);			
		return;
	}

	uint32_t order_id;
	if (!CDBCtrl::GetShopOrderId(GetdwAccountId(), order_id))
	{
		SendErrorMsg(ERRNO_MSG_SHOP_GET_ORDER_ID, CMD_SC_GROUP_CREATE_RESULT);
		return;
	}

	if (GetdwDollar() < GlobalConfig::ConsumeDollarCreate)
	{
		SendErrorMsg(ERRNO_MSG_DOLLAR_NOT_ENGOUH, CMD_SC_GROUP_CREATE_RESULT);
		return;
	}

	if (ChangeDollar(-GlobalConfig::ConsumeDollarCreate, SOURCE_GROUP_CREATE_CONSUME))
	{
		SendErrorMsg(ERRNO_MSG_DOLLAR_CONSUME_FAIL, CMD_SC_GROUP_CREATE_RESULT);
		return;
	}
	
	CDBCtrl::LogShopRecord(GetdwAccountId(), order_id, GlobalConfig::ConsumeDollarCreate, 0, 1, E_DC_CREATE_GROUP);	
	uint32_t  groupid = CDBCtrl::GetGroupId(GetdwAccountId());
	if (!CDBCtrl::InsertGroup(GetdwAccountId(), groupid, GetstrUserName(), groupname))
	{
		IME_ERROR("insert into group fail");
		SendErrorMsg(ERRNO_MSG_DB, CMD_SC_GROUP_CREATE_RESULT);
		return;
	}

	CGroup *pGroup = new(std::nothrow) CGroup();
	assert(pGroup != NULL);
	CDBCtrl::SGroup tmp;
	tmp.byFlag = 0;
	tmp.dwCredit = 0;
	tmp.dwGroupId = groupid;
	tmp.dwUserId = GetdwAccountId();
	tmp.strContent = "";
	tmp.strCreateName = GetstrUserName();
	tmp.strGroupName = groupname;
	tmp.wLevel = 1;
	tmp.strMember = "";
	if (!pGroup->Init(tmp))
	{
		IME_ERROR("group init fail");
		SendErrorMsg(ERRNO_MSG_GROUP_INIT, CMD_SC_GROUP_CREATE_RESULT);
		return;
	}
	uint64 id;
	id = MAKE_FIND_GROUPID(GetdwAccountId(), groupid);
	if (sWorld->AddCGroup(id, pGroup))
	{
		IME_ERROR("add group fail find userid %u, groupid %u id %llu",GetdwAccountId(), groupid,  id);
		SendErrorMsg(ERRNO_MSG_GROUP_INIT, CMD_SC_GROUP_CREATE_RESULT);
		return;
	}

	CDBCtrl::LogShopProductGive(GetdwAccountId(), order_id);

	SetlFindGroupId(id);
	SetbyFlag(OPEN_GROUP(GetbyFlag()));

	SendProUpdate();
	
	pGroup->SendAllGroupData(this);

	WorldPacket info(CMD_SC_GROUP_CREATE_RESULT);
	info << (uint16_t) 0;
	info << (uint32_t) groupid;
	SendPacket(&info);
	return; 
}		/* -----  end of method CRole::HandlerGroupCreate  ----- */

void
CRole::HandlerGroupAutoInfo (WorldPacket &pkg)
{
	uint32_t off;
	pkg >> off;
	if (IS_THERE_GROUP(GetbyFlag()))
	{
		SendErrorMsg(ERRNO_MSG_ALREADY_IN_GROUP, CMD_SC_GROUP_AUTO_RESULT);			
		return;
	}
	std::vector<CGroup *> vec;
	sWorld->GroupAuto10(vec, off);

	WorldPacket info(CMD_SC_GROUP_AUTO_RESULT);
	info << (uint16_t) 0;
	info << (uint32_t) vec.size();

	for(int i = 0; i < vec.size(); i++)
	{
		uint64 tmpid;
		tmpid = (uint64) MAKE_FIND_GROUPID(vec[i]->GetdwCreateId(), vec[i]->GetdwId());
		IME_ERROR("userid %u, groupid %u, %llx",vec[i]->GetdwCreateId(), vec[i]->GetdwId(), tmpid);
		info << tmpid;
		info << (uint16_t) vec[i]->GetwLevel();
		info << vec[i]->GetstrName();
		info << vec[i]->GetstrCreateName();
		info << vec[i]->GetwPersionNumber();
		info << vec[i]->GetwPersionLimit();
	}
	SendPacket(&info);

	return;
}

void CRole::HandlerGroupSearch(WorldPacket &pkg)
{
	std::string name;
	pkg >> name;

	if (IS_THERE_GROUP(GetbyFlag()))
	{
		SendErrorMsg(ERRNO_MSG_ALREADY_IN_GROUP, CMD_SC_GROUP_SEARCH_RESULT);			
		return;
	}
	std::vector<CGroup *> vec;
	sWorld->GroupSearchByName(vec, name);

	WorldPacket info(CMD_SC_GROUP_SEARCH_RESULT);
	info << (uint16_t) 0;
	info << (uint32_t) vec.size();

	for(int i = 0; i < vec.size(); i++)
	{
		uint64 tmpid;
		tmpid = (uint64) MAKE_FIND_GROUPID(vec[i]->GetdwCreateId(), vec[i]->GetdwId());
		IME_ERROR("userid %u, groupid %u, %llx",vec[i]->GetdwCreateId(), vec[i]->GetdwId(), tmpid);
		info << tmpid;
		info << (uint16_t) vec[i]->GetwLevel();
		info << vec[i]->GetstrName();
		info << vec[i]->GetstrCreateName();
		info << vec[i]->GetwPersionNumber();
		info << vec[i]->GetwPersionLimit();
	}
	SendPacket(&info);

	return;

}

void CRole::HandlerGroupRequestEnter(WorldPacket &pkg)
{
	uint64 groupid;
	std::string str;
	pkg >> groupid;
	pkg >> str;

	if (IS_THERE_GROUP(GetbyFlag()))
	{
		SendErrorMsg(ERRNO_MSG_ALREADY_IN_GROUP, CMD_SC_GROUP_REQUSET_ENTER_RESULT);			
		return;
	}
	CGroup *pCGroup = sWorld->GetCGroup(groupid);
	if (pCGroup == NULL)
	{
		SendErrorMsg(ERRNO_MSG_GROUP_NOT_LIVE, CMD_SC_GROUP_REQUSET_ENTER_RESULT);			
		return;
	}

	uint32_t mailid = CDBCtrl::GetGroupMailId(groupid);
	CDBCtrl::SGroupMail SGMtmp;
	SGMtmp.lGroupId = groupid;
	SGMtmp.dwTime = time(NULL);
	SGMtmp.dwMailId = mailid;
	SGMtmp.content = str;
	SGMtmp.byType = E_GT_MAIL_SYSTEM;
	SGMtmp.byDoflag = 0;
	SGMtmp.byDetailType = E_GDT_MAIL_REQUEST_ENTER;

	ByteBuffer blob;
	blob << GetdwAccountId();
	blob << E_GT_MAIL_REQUEST_ENTER_INDEX;
	blob << GetstrUserName();
	SGMtmp.blob.assign((const char *)blob.contents(), blob.size());

	if (!CDBCtrl::InsertGroupMail(SGMtmp))
	{
		IME_ERROR("insert data fail groupid %llu, %u", groupid, GetdwAccountId());
		SendErrorMsg(ERRNO_MSG_DB, CMD_SC_GROUP_REQUSET_ENTER_RESULT);			
		return;
	}

	WorldPacket info(CMD_SC_GROUP_REQUSET_ENTER_RESULT);
	info << (uint16_t) 0;
	SendPacket(&info);
	return;
}

//void CRole::UpdateEnergyAndStrengthTime()
//{
//	int32_t next_point_time;
//	int32_t all_time;
//	
//	uint32_t now = time(NULL);
//	next_point_time = (m_dwEnergyCnt + m_dwEnergyRate) - now;	
//	if (next_point_time < 0)
//		next_point_time = 0;
//	uint32_t need = GetdwEnergyMax() - GetdwEnergy();
//	all_time = (m_dwEnergyCnt + m_dwEnergyRate * need) - now;
//	if (all_time < 0)
//		all_time = 0;
//	SetdwNextEnergy(next_point_time);
//	SetdwAllEnergy(all_time);
//
//	next_point_time = (m_dwStrengthCnt + m_dwStrengthRate) - now;	
//	if (next_point_time < 0)
//		next_point_time = 0;
//	uint32_t need = GetdwStrengthMax() - GetdwStrength();
//	all_time = (m_dwStrengthCnt + m_dwStrengthRate * need) - now;
//	if (all_time < 0)
//		all_time = 0;
//	SetdwNextStrength(next_point_time);
//	SetdwAllStrength(all_time);
//}

void CRole::SendEnergyNotEnough(uint16 wCmd)
{
	WorldPacket info(wCmd);
	info << (uint16) ERROR_MSG_NOT_ENOUGH_ENERGY;
	int32_t next_point_time;
	int32_t all_time;
	
	uint32_t now = time(NULL);
	next_point_time = (m_dwEnergyCnt + m_dwEnergyRate) - now;	
	if (next_point_time < 0)
		next_point_time = 0;
	uint32_t need = GetdwEnergyMax() - GetdwEnergy();
	all_time = (m_dwEnergyCnt + m_dwEnergyRate * need) - now;
	if (all_time < 0)
		all_time = 0;
	info << next_point_time;
   	info << all_time;	
	SendPacket(&info);
}

void CRole::SendStrengthNotEnough(uint16 wCmd)
{
	WorldPacket info(wCmd);
	info << (uint16) ERRNO_MSG_NOT_ENOUGH_STRENGTH;
	int32_t next_point_time;
	int32_t all_time;
	
	uint32_t now = time(NULL);
	next_point_time = (m_dwStrengthCnt + m_dwStrengthRate) - now;	
	if (next_point_time < 0)
		next_point_time = 0;
	uint32_t need = GetdwStrengthMax() - GetdwStrength();
	all_time = (m_dwStrengthCnt + m_dwStrengthRate * need) - now;
	if (all_time < 0)
		all_time = 0;
	info << next_point_time;
   	info << all_time;	
	SendPacket(&info);
}

void CRole::SendStaminaNotEnough(uint16 wCmd)
{
	WorldPacket info(wCmd);
	info << (uint16) ERRNO_MSG_NOT_ENOUGH_STAMINA;
	int32_t next_point_time;
	int32_t all_time;

	uint32_t now = time(NULL);
	next_point_time = (m_dwStaminaTime + m_dwStaminaRate) - now;
	if (next_point_time < 0)
		next_point_time = 0;
	uint32_t need = GetdwStaminaMax() - GetdwStamina();
	all_time = (m_dwStaminaTime + m_dwStaminaRate * need) - now;
	if (all_time < 0)
		all_time = 0;
	info << next_point_time;
	info << all_time;
	SendPacket(&info);
}

void CRole::HandlerSellCard(WorldPacket &pkg)
{
	uint32_t CardId;
	std::vector<CCard*> vecCard;
	uint16_t number;
	uint32_t coin = 0;
	uint8_t type = -1;
	pkg >> number;

	IME_CHAR_LOG("action [userid %u sell card and item]", GetdwAccountId());
	std::vector<uint32_t> vecItem;
	std::vector<uint32_t> vecItemNumber;
	uint16_t itemnumber = 0;

	for (int i = 0; i < number; i++)
	{
		pkg >> CardId;
		CCard *pCard = m_clsCardPacket->Find(CardId);
		if (pCard == NULL)
		{
			IME_ERROR("can not find this card index %u", CardId);
			SendErrorMsg(ERRNO_MSG_CARD_NOT_IN_PACKET, CMD_SC_SELL_CARD_RESULT);
			return;
		}
		if (i == 0)
		{
			type = pCard->GetbyType();
		}
		if (!(pCard->GetbyType() == E_ITEM_PET_CARD || pCard->GetbyType() == E_ITEM_WEAPON_CARD || pCard->GetbyType() == E_ITEM_ARMOR_CARD || pCard->GetbyType() == E_ITEM_JEWEL_CARD || pCard->GetbyType() == E_ITEM_ACCESSORIES_CARD || pCard->GetbyType() == E_ITEM_MASTER_CARD || pCard->GetbyType() == E_ITEM_SKILL_CARD))
		{
			IME_ERROR("this card type is not unvalid objid %u type %u", CardId, pCard->GetbyType());
			SendErrorMsg(ERRNO_MSG_DATA_ILLEGAL, CMD_SC_SELL_CARD_RESULT);
			return;
		}

		uint32_t location;
		uint8_t team;
		if (m_clsCardPacket->IsInEquip(pCard, location, team))
		{
			IME_ERROR("the card objid [%u] location [%u] is in equip", CardId, location);
			SendErrorMsg(ERRNO_MSG_CONSUME_CARD_IN_EQUIP, CMD_SC_SELL_CARD_RESULT);	
			return;
		}	

		vecCard.push_back(pCard);
//		STC_CONF_CARD_UP *pConfUp = CConfCardUp::Find(pCard->GetbyStarLevel(), pCard->GetwLevel());
//		if (pConfUp == NULL)
//		{
//			IME_ERROR("this card consume conf csv can not find this record index star %u level %u", pCard->GetbyStarLevel(), pCard->GetwLevel());
//			SendErrorMsg(ERRNO_MSG_CSV_LOAD, CMD_SC_SELL_CARD_RESULT);
//			return;
//
//		}
		STC_CONF_CARD_BASE *pConf = CConfCardBase::Find(pCard->GetdwIndex());
		if (pConf == NULL)
		{
			IME_ERROR("this card consume conf csv can not find this record index %u", pCard->GetdwIndex());
			SendErrorMsg(ERRNO_MSG_CSV_LOAD, CMD_SC_SELL_CARD_RESULT);
			return;
		}
		//todo coin
		uint32 tmpcoin = pConf->dwGetCoin + pCard->GetdwTotalConsumeCoin() * SELL_CARD_COIN / 100.0 ;	
		coin += tmpcoin;	
	}

	pkg >> itemnumber;
	for (int i = 0; i < itemnumber; i++)
	{
		uint32_t val;
		pkg >> val;
		vecItem.push_back(val);
		pkg >> val;
		vecItemNumber.push_back(val);
		if (!m_CShop->IsHave(vecItem[i], vecItemNumber[i]))
		{
			IME_ERROR("shop item index %u is not enough %u", vecItem[i], vecItemNumber[i]);
			SendErrorMsg(ERRNO_MSG_GOODS_NOT_ENOUGH, CMD_SC_SELL_CARD_RESULT);
			return;
		}
		STC_CONF_GOODS *pConf = CConfGoods::Find(vecItem[i]);
		if (pConf == NULL)
		{
			IME_ERROR("this good index %u can not find in csv", vecItem[i]);
			SendErrorMsg(ERRNO_MSG_CSV_DATA, CMD_SC_SELL_CARD_RESULT);
			return;
		}
		if (!pConf->byIsSell)
		{
			IME_ERROR("this good index %u can not sell", pConf->dwIndex);
			SendErrorMsg(ERRNO_MSG_SELL_UNALLOW, CMD_SC_SELL_CARD_RESULT);
			return;
		}
		uint32 tmpcoin = pConf->dwSellNumber * vecItemNumber[i];
		coin += tmpcoin;
	}


	for (int i = 0; i < number; i++)
	{
		m_clsCardPacket->CardDel(vecCard[i]->GetdwObjId(), SOURCE_SELL_CARD);
		m_clsCardPacket->SingleCardDelUpdate(vecCard[i]->GetdwObjId());
	}
	for (int i = 0; i < itemnumber; i++)
	{
		m_CShop->DelItem(vecItem[i], SOURCE_SELL_CARD, vecItemNumber[i]);
		m_CShop->HandlerShopInfo(vecItem[i]);
	}

	ChangeCoin(coin, SOURCE_SELL_CARD);
	SendProUpdate();

	WorldPacket info(CMD_SC_SELL_CARD_RESULT);
	info << (uint16_t) 0;
	info << type;
	SendPacket(&info);
}

void CRole::HandlerInfoOpt()
{
	long int t = GetdwAvoidDurationTime() - time(NULL);
	if (t < 0)
		t = 0;
	WorldPacket info(CMD_SC_ROLE_INFO_OPT);
	info << (uint32_t) t;
	info << m_byFirstLogin;
//	IME_ERROR("is first login %u", m_byFirstLogin);
	info << (uint32_t)time(NULL);
	info << m_CActive->IsSigned();

	int32_t next_point_time;
	int32_t all_time;
	
	uint32_t now = time(NULL);
	next_point_time = (m_dwEnergyCnt + m_dwEnergyRate) - now;	
	if (next_point_time < 0)
		next_point_time = 0;
	if (m_dwEnergy >= m_dwEnergyMax)
	{
		next_point_time = 0;
	}

	uint32_t need = GetdwEnergyMax() - GetdwEnergy();
	all_time = (m_dwEnergyCnt + m_dwEnergyRate * need) - now;
	if (all_time < 0)
		all_time = 0;
	info << next_point_time;
	info << all_time;

	next_point_time = (m_dwStrengthCnt + m_dwStrengthRate) - now;	
	if (next_point_time < 0)
		next_point_time = 0;
	if (m_dwStrength >= m_dwStrengthMax)
	{
		next_point_time = 0;
	}
	need = GetdwStrengthMax() - GetdwStrength();
	all_time = (m_dwStrengthCnt + m_dwStrengthRate * need) - now;
	if (all_time < 0)
		all_time = 0;
	info << next_point_time;
	info << all_time;
	info << m_dwExtraStrengthPro;
	info << (uint32_t)(GlobalConfig::MasterLimit1 - m_dwLowMasterCount);
	if (m_dwLowMasterCount >= LOW_MASTER_EVERVY_LIMIT)
	{
		info << (uint8_t) 1;
	}
	else
	{
		info << (uint8_t) 0;
	}
	info << (uint32_t)m_mapFlow.size();
	std::map<uint32_t, uint32_t>::iterator it;
	for (it = m_mapFlow.begin(); it != m_mapFlow.end(); it++)
	{
		info << (uint32_t) it->first;
		info << (uint32_t) it->second;
	}
	info << m_dwFlowCurrent;
	info << m_wUseEnergy;
	info << m_wUseStrength;
	uint32_t energyworth = 0;
	uint32_t strengthworth = 0;
	energyworth = GetdwEnergyMax() * (GlobalConfig::EnergyWorth + m_wUseEnergy * GlobalConfig::EnergyWorthGrow) / 100;
	energyworth = energyworth <= 0 ? 1 : energyworth; 
	strengthworth = GetdwStrengthMax() * (GlobalConfig::StrengthWorth + m_wUseStrength * GlobalConfig::StrengthWorth) / 100;
	strengthworth = strengthworth <= 0 ? 1 : strengthworth;
	info << energyworth;
	info << strengthworth;
	info << (uint32_t)(GlobalConfig::RoleCardLimit + m_dwExtraRolePacketLimit);
	info << (uint32_t)(GlobalConfig::EquipCardLimit + m_dwExtraEquipPacketLimit);
	info << (uint32_t)((m_dwExtraRolePacketLimit / GlobalConfig::RolePacketNumber + 1) * GlobalConfig::RolePacketConsume);
	info << (uint32_t)((m_dwExtraEquipPacketLimit / GlobalConfig::EquipPacketNumber + 1) * GlobalConfig::EquipPacketConsume);

	int64_t cd = (int64_t)(m_dwWorldBroadTime + GlobalConfig::WorldBroadCD) - now;	
	cd = cd < 0 ? 0 : cd;
	info << (uint32_t)cd;

	STC_CONF_VIP *pVip = CConfVip::Find(GetwVipLv());
	uint32_t number = 0;
	if (pVip != NULL)
		number = pVip->dwEnergyStrengthNumber;
	if (GetdwEnergyUseCnt() >= number)
	{
		info << (uint8_t) 1;
	}
	else
	{
		info << (uint8_t) 0;
	}
	if (GetdwStrengthUseCnt() >= number)
	{
		info << (uint8_t) 1;
	}
	else
	{
		info << (uint8_t) 0;
	}
	info << m_dwGetExtraDollar;
	info << m_dwCurrentDollarRate;
	info << m_dwNextDollarRate;

	info << m_dwStamina;
	next_point_time = (m_dwStaminaTime + m_dwStaminaRate) - now;	
	if (next_point_time < 0)
		next_point_time = 0;
	if (m_dwStamina >= m_dwStaminaMax)
	{
		next_point_time = 0;
	}
	need = GetdwStaminaMax() - GetdwStamina();
	all_time = (m_dwStaminaTime + m_dwStaminaRate * need) - now;
	if (all_time < 0)
		all_time = 0;
	info << next_point_time;
	info << all_time;

	uint32_t Staminaworth = 0;
	Staminaworth = GetdwStaminaMax() * (GlobalConfig::StaminaWorth + m_wUseStamina * GlobalConfig::StaminaWorthGrow) / 100;
	Staminaworth = Staminaworth <= 0 ? 1 : Staminaworth; 
	info << m_wUseStamina;
	info << Staminaworth;
	info << m_byFirstChangeName;
	info << m_byFirstSendRed;
	SendPacket(&info);
}

bool CRole::IsInitRoleInfo()
{
	return GetwLevel();
}	

void CRole::HandlerInitRole(WorldPacket &pkg)
{
	std::string username;
	uint32_t masterindex;
	pkg >> username;
	pkg >> masterindex;

	if (masterindex == 0)
		return;

	std::vector<int>::iterator ita;
	std::vector<SProduct> vproduct;
	std::string t;
	std::string c;
	SProduct duct;
	ita = std::find(GlobalConfig::ChannelSpecial.begin(), GlobalConfig::ChannelSpecial.end(), (int)GetSession()->GetwChannel());
   	if (ita != GlobalConfig::ChannelSpecial.end())
	{
		int number = ita - GlobalConfig::ChannelSpecial.begin();
		duct.type = E_OBJ_SHOP;
		duct.para1 = GlobalConfig::ChannelSpecialGift[number];
		duct.para2 = 1;
		vproduct.push_back(duct);
		t = GlobalConfig::ChannelSpecialMailTitle[number];
		c = GlobalConfig::ChannelSpecialMailContent[number];
	}
	if (CShopEffect::IsObjValid(vproduct))
	{
		IME_ERROR("channel gift type para unvalid");
		SendErrorMsg(ERRNO_MSG_CSV_DATA, CMD_SC_INIT_ROLE_RESULT);
		return;
	}

	if (m_clsCardPacket->InitCards(masterindex))
	{
		IME_ERROR("role character id %u init fail", m_dwAccountId);
		SendErrorMsg(ERRNO_MSG_INIT_ROLE, CMD_SC_INIT_ROLE_RESULT);
		return;
	}

	CNameChecker::ReplaceString(username);	
	if (sWorld->IsExists(username))
	{
		IME_ERROR("user name already have been chosen name %s", username.c_str());
		SendErrorMsg(ERRNO_MSG_USER_NAME_ALREADY_USE, CMD_SC_INIT_ROLE_RESULT);
		return;
	}

	SetdwExp(0);
	SetdwCoin(GlobalConfig::InitCoin);
	SetdwDollar(GlobalConfig::InitDollar);

	m_CShop->AddItem(INIT_SHOP_ITEM, SOURCE_INIT_ROLE);
	m_CShop->HandlerShopInfo(INIT_SHOP_ITEM);

	SetstrUserName(username);
	sWorld->InsertUserName(username);

	if (Calculate(false))
	{
		IME_ERROR("calculate fail");
		SendErrorMsg(ERRNO_MSG_CALCULATE, CMD_SC_INIT_ROLE_RESULT);
		return;
	}

   	if (ita != GlobalConfig::ChannelSpecial.end())
	{
		uint32_t mail_id = 0;
		CDBCtrl::stuMailDetail tmp = {0};
		CMail::SBaseAttr a = {0};
		CMail::AddEmailSysDiy(GetdwAccountId(), mail_id, tmp, duct.type, duct.para1, duct.para2, c, a, t);   

		CMail::EmailUpdate(this, CMail::ADD, &tmp, 0);
	}

	//		m_dwEnergyCnt = m_dwEnergyRate;
	//		m_dwStrengthCnt = m_dwStrengthRate;

	m_dwEnergyCnt = time(NULL);
	m_dwStrengthCnt = time(NULL);

	SetdwEnergy(m_dwEnergyMax);
	SetdwStrength(m_dwStrengthMax);
	SetdwStamina(m_dwStaminaMax);
	SendProUpdate();

	uint32_t objid1 = 0;
	uint32_t objid2 = 0;
	uint32_t objid3 = 0;
	std::vector<CCard *> tempvec;
	m_clsCardPacket->GetEquippCards(tempvec);
	for (int i = 0; i < tempvec.size(); i++)
	{
		if (tempvec[i]->GetbyStarLevel() == 1 && tempvec[i]->GetbyType() == E_ITEM_PET_CARD)
			objid1 = tempvec[i]->GetdwObjId();
		if (tempvec[i]->GetbyStarLevel() == 2 && tempvec[i]->GetbyType() == E_ITEM_PET_CARD)
			objid3 = tempvec[i]->GetdwObjId();
		if (tempvec[i]->GetbyStarLevel() == 3 && tempvec[i]->GetbyType() == E_ITEM_PET_CARD)
			objid2 = tempvec[i]->GetdwObjId();
	}
	
	m_CTimerGift->StartTimer();

	WorldPacket info(CMD_SC_INIT_ROLE_INFO);
	info << objid1;
	info << objid2;
	info << objid3;
	SendPacket(&info);
	
	WorldPacket info2(CMD_SC_INIT_ROLE_RESULT);
	info2 << (uint16_t) 0;
	SendPacket(&info2);

	m_dwAddFriend = 0;
	m_dwAddFriendTime = time(NULL);
	m_dwDelFriend = 0;
	m_dwDelFriendTime = time(NULL);
	m_dwDelDMissionCnt = 0;
	m_dwDelDMissionCntTime = time(NULL);
	m_dwEnergyUseCnt = 0;
	m_dwEnergyUseCntTime = time(NULL);
	m_dwStrengthUseCnt = 0;
	m_dwStrengthUseCntTime = time(NULL);
	m_dwHighDelDMissionCnt = 0;
	m_dwHighDelDMissionCntTime = time(NULL);
	m_dwLuck = CUtil::RandByRange(GlobalConfig::LuckInitLow, GlobalConfig::LuckInitHigh + 1);
	SetdwLuck(m_dwLuck);

	m_wUseEnergy = 0;
	m_wUseStrength = 0;
	m_wUseStamina = 0;

	m_dwEnergyStrengthTime = time(NULL);
	
	m_dwExtraRolePacketLimit = 0;
	m_dwExtraEquipPacketLimit = 0;

	m_dwGmFlushTime = CenterDBCtrl::HandlerGmCommandRole(this, m_dwGmFlushTime);
	m_dwGiftDayTime = time(NULL);

	m_dwStaminaTime = time(NULL);

	m_CTrial->HandlerDataInfo();

	SaveAllData();
	return;
}

void CRole::HandlerFriendBossListRequset(WorldPacket &pkg)
{
	CFriendBoss *pFb = NULL;
	std::vector<CDBCtrl::stuPlayerPlus> vecTmp;
	CDBCtrl::GetUserBaseDataInFriend(GetdwAccountId(), vecTmp);
	WorldPacket info(CMD_SC_FRIEND_BOSS_LIST_REQUEST_RESULT);
	info << (uint16_t) 0;
	int number = 0;;
	WorldPacket by;

	m_vecFriend.clear();
	IME_CHAR_LOG("action [userid %u request friend boss list]", GetdwAccountId());

	for(int i = 0; i < vecTmp.size(); i++)
	{
		pFb = (CFriendBoss *)(sWorld->GetWorldBoss()->GetData(vecTmp[i].dwUserid));
		if (pFb == NULL)
		{
			IME_ERROR("can not find this user %u friend boss data", vecTmp[i].dwUserid);
			SendErrorMsg(ERRNO_MSG_FRIEND_BOSS_NOT_FIND, CMD_SC_FRIEND_BOSS_LIST_REQUEST_RESULT);
			return;
		}
		pFb->OnTimer5s();
		
		if (pFb->GetBossId())
		{
			pFb->PkgBossInfo(by);
//			by << vecTmp[i].strName;
			number++;
		}
		SPlayer tmp;
		tmp.userid = vecTmp[i].dwUserid;
		tmp.level = vecTmp[i].wLevel;
		tmp.username = vecTmp[i].strName;
		m_vecFriend.push_back(tmp);
	}	
	info << (uint16_t) number;
	info.append(by.contents(), by.size());
	SendPacket(&info);
	return;
}

void CRole::HandlerFriendBossHistoryRequest(WorldPacket &pkg)
{
	uint32 requestuserid;
	pkg >> requestuserid;
	IME_CHAR_LOG("action [userid %u request friend %u boss history]", GetdwAccountId(), requestuserid);

	CFriendBoss *pFb = NULL;
	pFb = (CFriendBoss *)(sWorld->GetWorldBoss()->GetData(requestuserid));
	if (pFb == NULL)
	{
		IME_ERROR("can not find this user %u friend boss data", requestuserid);
		SendErrorMsg(ERRNO_MSG_FRIEND_BOSS_NOT_FIND, CMD_SC_FRIEND_BOSS_HISTORY_REQUEST_RESULT);
		return;
	}
	WorldPacket info(CMD_SC_FRIEND_BOSS_HISTORY_REQUEST_RESULT);
	pFb->PkgBossHarmHistoryInfo(info);
	SendPacket(&info);
}

void CRole::HandlerFriendBossDurationRequest(WorldPacket &packet)
{
	uint32 userid;
	packet >> userid;
	IME_CHAR_LOG("action [userid %u request friend boss duration]", GetdwAccountId());
	CFriendBoss *pFb = NULL;
	pFb = (CFriendBoss *)(sWorld->GetWorldBoss()->GetData(userid));
	if (pFb == NULL)
	{
		IME_ERROR("can not find this user %u friend boss data", userid);
		SendErrorMsg(ERRNO_MSG_FRIEND_BOSS_NOT_FIND, CMD_SC_FRIEND_BOSS_DURATION_REQUEST_RESULT);
		return;
	}
	sWorld->GetWorldBoss()->AddResidentKey(userid);
	//user peer user relation
	pFb->HandlerDurationBoss(this);
	sWorld->GetWorldBoss()->ClearResidentKey();
	return;
}

void CRole::HandlerFriendBossGacha(WorldPacket &packet)
{
	IME_CHAR_LOG("action [userid %u friend boss gacha]", GetdwAccountId());
	CFriendBoss *pFb = NULL;
	pFb = (CFriendBoss *)(sWorld->GetWorldBoss()->GetData(GetdwAccountId()));
	if (pFb == NULL)
	{
		IME_ERROR("can not find this user %u friend boss data", GetdwAccountId());
		SendErrorMsg(ERRNO_MSG_FRIEND_BOSS_NOT_FIND, CMD_SC_FRIEND_BOSS_DURATION_REQUEST_RESULT);
		return;
	}
	pFb->HandlerBossGacha(this, packet);
	return;
}

void CRole::HandlerWorldBroadCastInfo()
{
//	WorldPacket info(CMD_SC_WORLD_BROADCAST_INFO);
//	
//	PkgWorldBroadCastInfo(0, 0, "", info);	
//	SendPacket(&info);
	sWorld->HandlerWorldChatInfo(this);
	sWorld->GetCNotice()->HandlerBroadCast(this);
}

void CRole::PkgWorldBroadCastInfo(uint8_t choose, std::string content, WorldPacket &info)
{
//	uint32_t now = time(NULL);
//	int64_t cd = (int64_t)(m_dwWorldBroadTime + GlobalConfig::WorldBroadCD) - now;	
//	cd = cd < 0 ? 0 : cd;
//	info << (uint32_t)cd;
//	info << cmd;
	info << choose;
	info << GetdwAccountId();
	info << GetstrUserName();
	info << content;
	info << GetwVipLv();

	SWorldChat tmp;
	tmp.choose = choose;
	tmp.userid = GetdwAccountId();
	tmp.username = GetstrUserName();
	tmp.content = content;
	tmp.viplv = GetwVipLv();
	sWorld->AddWorldChat(tmp);
//	SendPacket(&info);
}

void CRole::HandlerWorldBroadCast(WorldPacket &pkg)
{
	uint8_t choose;
	std::string content;
	IME_CHAR_LOG("action [userid %u chat content %s]", GetdwAccountId(), content.c_str());
	pkg >> choose;
	pkg >> content;
	if (content.size() > MAX_BROADCAST_LENGTH)
	{
		SendErrorMsg(ERRNO_MSG_BROADCAST_OVER_MAX_LENGTH, CMD_SC_WORLD_BROADCAST_REQUEST_RESULT);
		return;
	}

	if (choose == 0)
	{
		if (m_dwWorldBroadTime + GlobalConfig::WorldBroadCD >= time(NULL))
		{
			SendErrorMsg(ERRNO_MSG_CHAT_IN_CD, CMD_SC_WORLD_BROADCAST_REQUEST_RESULT);
			return;
		}
		CNameChecker::ReplaceString(content);

		WorldPacket info(CMD_SC_WORLD_BROADCAST_INFO);
		PkgWorldBroadCastInfo(choose, content, info);
		sSessCtrl->SendPkgToAll(info);	
		m_dwWorldBroadTime = time(NULL);
		HandlerInfoOpt();
	}	
	else
	{
		STC_CONF_GOODS *pConf = CConfGoods::Find(TRUMPET_SHOPID);
		if (pConf == NULL)
		{
			IME_ERROR("can not find this index %u in shop csv", TRUMPET_SHOPID);
			SendErrorMsg(ERRNO_MSG_CSV_LOAD, CMD_SC_WORLD_BROADCAST_REQUEST_RESULT);
			return;
		}
		if (pConf->dwLevelLimit != 0)
		{
			if (GetwLevel() < pConf->dwLevelLimit)
			{
				SendErrorMsg(ERRNO_MSG_SHOP_LEVEL_LIMIT, CMD_SC_WORLD_BROADCAST_REQUEST_RESULT);
				return;
			}
		}

		if (!m_CShop->IsHave(TRUMPET_SHOPID, 1))
		{
			SendErrorMsg(ERRNO_MSG_SHOP_ITEM_HAVE_NOT, CMD_SC_WORLD_BROADCAST_REQUEST_RESULT);
			return;
		}
		m_CShop->DelItem(TRUMPET_SHOPID, SOURCE_ITEM_USE);
		m_CShop->HandlerShopInfo(TRUMPET_SHOPID);

		CNameChecker::ReplaceString(content);

		WorldPacket info(CMD_SC_WORLD_BROADCAST_INFO);
		PkgWorldBroadCastInfo(choose, content, info);
		sSessCtrl->SendPkgToAll(info);	
		HandlerInfoOpt();
	}

	WorldPacket res(CMD_SC_WORLD_BROADCAST_REQUEST_RESULT);
	res << (uint16_t) 0;
	SendPacket(&res);
	return;
}

void CRole::HandlerGameFlow(WorldPacket &packet)
{
	uint32_t gameflow = 0;
	uint32_t key = 0;
	uint32_t currentkey = 0;
	packet >> key;
	packet >> gameflow;
	packet >> currentkey;
	m_dwFlowCurrent = currentkey;
	m_mapFlow[key] = gameflow;
	SetdwGameFlow(key * 10000 + gameflow);

	SendProUpdate();
	HandlerInfoOpt();

	WorldPacket info(CMD_SC_GAME_FLOW_REQUEST_RESULT);
	info << (uint16_t) 0;
	info << key;
	info << gameflow;
	info << currentkey;
	SendPacket(&info);
}

int CRole::OtherDataGet()
{
	std::string tmp;
	m_mapFlow.clear();
	if (!CDBCtrl::GetUserDataOther(tmp, m_dwAccountId))
	{
		IME_ERROR("get role other data fail");
		return -1;
	}
	ByteBuffer by;
	by.append(tmp.c_str(), tmp.size());
	by >> m_wOtherDataversion;
	IME_DEBUG("other data version %u", m_wOtherDataversion);
	if (m_wOtherDataversion == 0 || m_wOtherDataversion == 1)
	{
		by >> m_dwAddFriend;
		by >> m_dwDelFriend;
		by >> m_dwAddFriendTime;
		by >> m_dwDelFriendTime;
		by >> m_dwDelDMissionCnt;
		by >> m_dwDelDMissionCntTime;
		by >> m_dwEnergyUseCnt;
		by >> m_dwEnergyUseCntTime;
		by >> m_dwStrengthUseCnt;
		by >> m_dwStrengthUseCntTime;
		uint32_t number;
		by >> number;
		int val;
		for (int i = 0; i < number; i++)
		{
			by >> val; 
			m_setVipGift.insert(val);
		}
		by >> m_dwGmFlushTime;
		by >> m_dwHighDelDMissionCnt;
		by >> m_dwHighDelDMissionCntTime;
		by >> m_dwExtraStrengthPro;
		by >> m_dwLowMasterCount;
		by >> m_dwLowMasterTime;
		by >> number;
		uint32_t key,value;
		for (int i = 0; i < number; i++)
		{
			by >> key;
			by >> value;
			m_mapFlow[key] = value;
		}	
		by >> m_dwFlowCurrent;
		by >> m_byFirstGacha;
		by >> m_wUseEnergy;
		by >> m_wUseStrength;
		by >> m_dwEnergyStrengthTime;
		by >> m_dwExtraRolePacketLimit;
		by >> m_dwExtraEquipPacketLimit;
		by >> number;
		uint8_t vipset;
		for (int i = 0; i < number; i++)
		{
			by >> vipset;
			m_setVipFirstGacha.insert(vipset);
		}

		by >> number;
		for (int i = 0; i < number; i++)
		{
			by >> key;
			SGift tmp;
			by >> tmp.role;
			by >> tmp.day;
			m_mapGift[key] = tmp;
		}
		by >> m_dwGiftDayTime;
		by >> m_dwGetExtraDollar;
		by >> m_dwDotDollar;
		by >> m_dwTotalJoin;
		by >> number;
		uint8_t valjoin;
		for (int i = 0; i < number; i++)
		{
			by >> key;
			by >> valjoin;
			m_mapJoin[key] = valjoin;
			//		IME_ERROR("join key %u val %u", key, valjoin);
		}
		by >> m_dwJoinFlushTime;
		by >> m_wUseStamina;
		by >> m_byFirstChangeName;
		by >> m_byFirstSendRed;
		by >> m_bySecondGacha;
		IME_DEBUG("addFriend %u delFriend %u, energyUseCnt %u, strengthUseCnt %u", m_dwAddFriend, m_dwDelFriend, m_dwEnergyUseCnt, m_dwStrengthUseCnt);
	}
	else
	{
		IME_ERROR("server version is too old than DB, other data version %u", m_wOtherDataversion);
		return -1;
	}

	if(m_wOtherDataversion == 0)
	{
		m_wOtherDataversion = 1;
		std::map<int, SGift>::iterator it = m_mapGift.begin();
		for (; it != m_mapGift.end(); it++)
		{
			it->second.role = 0;
		}
	}

	uint32_t last_login_time, last_logout_time, cur_login_time, login_day;
	std::string last_login_ip;
	if (CDBCtrl::GetUserLoginData(m_dwAccountId, last_login_time, last_logout_time, cur_login_time, login_day, last_login_ip))
	{
		m_dwLastLoginTime = last_login_time;
	}

	return 0;
}

void CRole::OtherDataSave()
{
	ByteBuffer by;
	by << m_wOtherDataversion;
	by << m_dwAddFriend;
	by << m_dwDelFriend;
	by << m_dwAddFriendTime;
	by << m_dwDelFriendTime;
	by << m_dwDelDMissionCnt;
	by << m_dwDelDMissionCntTime;
	by << m_dwEnergyUseCnt;
	by << m_dwEnergyUseCntTime;
	by << m_dwStrengthUseCnt;
	by << m_dwStrengthUseCntTime;
	by << (uint32_t)m_setVipGift.size();
	std::set<int>::iterator it;
	for (it = m_setVipGift.begin(); it != m_setVipGift.end(); it++)
	{
		by << *it;
	}
	by << m_dwGmFlushTime;
	by << m_dwHighDelDMissionCnt;
	by << m_dwHighDelDMissionCntTime;
	by << m_dwExtraStrengthPro;
	by << m_dwLowMasterCount;
	by << m_dwLowMasterTime;
	by << (uint32_t)m_mapFlow.size();
	std::map<uint32_t, uint32_t>::iterator itm;
	for (itm = m_mapFlow.begin(); itm != m_mapFlow.end(); itm++)
	{
		by << (uint32_t) itm->first;
		by << (uint32_t) itm->second;
	}
	by << m_dwFlowCurrent;
	by << m_byFirstGacha;
	by << m_wUseEnergy;
	by << m_wUseStrength;
	by << m_dwEnergyStrengthTime;
	by << m_dwExtraRolePacketLimit;
	by << m_dwExtraEquipPacketLimit;
	by << (uint32_t)m_setVipFirstGacha.size();
	std::set<uint8_t>::iterator its = m_setVipFirstGacha.begin();
	for (its = m_setVipFirstGacha.begin(); its != m_setVipFirstGacha.end(); its++)
	{
		by << *its;
	}

	by << (uint32_t)m_mapGift.size();
	std::map<int, SGift>::iterator ita = m_mapGift.begin();
	for (ita = m_mapGift.begin(); ita != m_mapGift.end(); ita++)
	{
		by << ita->first;
		by << ita->second.role;
		by << ita->second.day;
	}
	by << m_dwGiftDayTime;
	by << m_dwGetExtraDollar;
	by << m_dwDotDollar;

	by << m_dwTotalJoin;
	by << (uint32_t) m_mapJoin.size();
	std::map<uint32_t, uint8_t>::iterator itj = m_mapJoin.begin();
	for (itj = m_mapJoin.begin(); itj != m_mapJoin.end(); itj++)
	{
		by << itj->first;
		by << itj->second;
	}
	by << m_dwJoinFlushTime;
	by << m_wUseStamina;
	by << m_byFirstChangeName;
	by << m_byFirstSendRed;
	by << m_bySecondGacha;
	std::string tmp;
	tmp.append((const char *)by.contents(), by.size());

	CDBCtrl::SaveUserDataOther(tmp, m_dwAccountId);
}

int CRole::ChangeEnergyNoLimit(int32_t energy)
{
	int32_t temp = m_dwEnergy + energy;
	if (temp < 0)
	{
		temp = 0;
	}
//	IME_LOG("energy %u del %d", GetdwEnergy(), energy);
	SetdwEnergy(temp);
	return 0;
}

int CRole::ChangeStrengthNoLimit(int32_t strength)
{
	int32_t temp = m_dwStrength + strength;
	if (temp < 0)
	{
		temp = 0;
	}
	SetdwStrength(temp);
	return 0;
}

int CRole::ChangeStamina(int32_t stamina)
{
	int32_t temp = m_dwStamina + stamina;
	if (stamina < 0)
	{
		if (temp < 0)
			m_dwStamina = 0;
		else
			m_dwStamina = temp;
	}
	else
	{
		if (m_dwStamina < m_dwStaminaMax)
		{
			if (temp > m_dwStaminaMax)
			{
				m_dwStamina = m_dwStaminaMax;
			}
			else
			{
				m_dwStamina = temp;
			}
		}
	}
	SetdwStamina(m_dwStamina);
	return 0;
}

int CRole::ChangeStaminaNoLimit(int32_t stamina)
{
	int32_t temp = m_dwStamina + stamina;
	if (temp < 0)
	{
		temp = 0;
	}
	SetdwStamina(temp);
	return 0;
}

void CRole::HandlerTurntable(WorldPacket &packet)
{
	uint8_t choose;
	uint32_t number;
	packet >> choose;
	packet >> number;

	IME_CHAR_LOG("action [userid %u open turntable choose %u number %u]", GetdwAccountId(), choose, number);
//	int consumedollar = 0;
	uint32_t turnconsume = 0;
//	uint32_t order_id;
//	if (!CDBCtrl::GetShopOrderId(m_pCRole->GetdwAccountId(), order_id))
//	{
//		m_pCRole->SendErrorMsg(ERRNO_MSG_SHOP_GET_ORDER_ID, CMD_SC_TURNTABLE_TURN_RESULT);
//		return;
//	}

	if (number != TURNTABLE_TURN_COUNT_1 && number != TURNTABLE_TURN_COUNT_10 && number != TURNTABLE_TURN_COUNT_100)
	{
		IME_ERROR("send para unvalid number %u", number);
		SendErrorMsg(ERRNO_MSG_DATA_ILLEGAL, CMD_SC_TURNTABLE_TURN_RESULT);
		return;
	}
	if (choose == TURNTABLE_NORMAL_CHOOSE)
	{
//		if (GetdwDollar() < number * GlobalConfig::NormalTurntableConsume)
//		{
//			SendErrorMsg(ERRNO_MSG_DOLLAR_NOT_ENGOUH, CMD_SC_TURNTABLE_TURN_RESULT);
//			return;
//		}
		if (!GetCShop()->IsHave(TURNTABLE_SHOPID, GlobalConfig::NormalTurntableConsume * number))
		{
			WorldPacket pkg(CMD_SC_TURNTABLE_TURN_RESULT);
			pkg << (uint16_t) ERRNO_MSG_GOODS_NOT_ENOUGH;
			pkg << (uint32_t) GlobalConfig::NormalTurntableConsume * number;
			pkg << (uint8_t) choose;
			SendPacket(&pkg);
			return;
		}
		if (!CActiveCtrl::IsActiveStart(TURN_NORMAL_TABLE_ACTIVE_ID))
		{
			SendErrorMsg(ERRNO_MSG_ACTIVE_NOT_OPEN, CMD_SC_TURNTABLE_TURN_RESULT);
			return;
		}
//		consumedollar = - (number * GlobalConfig::NormalTurntableConsume);
		turnconsume = GlobalConfig::NormalTurntableConsume * number;
	}
	else if(choose == TURNTABLE_SUPER_CHOOSE)
	{
		if (!GetCShop()->IsHave(SUPER_TURNTABLE_SHOPID, GlobalConfig::SuperTurntableConsume * number))
		{
			WorldPacket pkg(CMD_SC_TURNTABLE_TURN_RESULT);
			pkg << (uint16_t) ERRNO_MSG_GOODS_NOT_ENOUGH;
			pkg << (uint32_t) GlobalConfig::SuperTurntableConsume * number;
			pkg << (uint8_t) choose;
			SendPacket(&pkg);
			return;
		}
		if (!CActiveCtrl::IsActiveStart(TURN_SUPER_TABLE_ACTIVE_ID))
		{
			SendErrorMsg(ERRNO_MSG_ACTIVE_NOT_OPEN, CMD_SC_TURNTABLE_TURN_RESULT);
			return;
		}
//		consumedollar = - (number * GlobalConfig::SuperTurntableConsume);
		turnconsume = GlobalConfig::SuperTurntableConsume * number;
	}
	else
	{
		IME_ERROR("send para unvalid choose %u", choose);
		SendErrorMsg(ERRNO_MSG_DATA_ILLEGAL, CMD_SC_TURNTABLE_TURN_RESULT);
		return;
	}

	if (number == TURNTABLE_TURN_COUNT_1)
	{
		if (GetclsCardPacket()->IsFull(1, 0))
		{
			SendErrorMsg(ERRNO_MSG_CARD_PACKET_NOT_ENOUGH, CMD_SC_TURNTABLE_TURN_RESULT);
			return;
		}
	}
	else
	{
		if (GetclsCardPacket()->IsFull(TURNTABLE_MAX_CARD_GET, 0))
		{
			IME_ERROR("cardpacket is full");
			SendErrorMsg(ERRNO_MSG_CARD_PACKET_NOT_ENOUGH, CMD_SC_TURNTABLE_TURN_RESULT);
			return;
		}
	}
	
	std::vector<int> Stmp;
	std::vector<int> StmpIndex;
	std::vector<int> StmpCount;
	CActiveCtrl::GetTurnResult(this, choose, number, Stmp);
	for (int i = 0; i < Stmp.size(); i++)
	{
		if (Stmp[i] != 0)
		{
			StmpIndex.push_back(i);
			StmpCount.push_back(Stmp[i]);
		}
	}
	if (StmpIndex.size() == 0)
	{
		IME_ERROR("turn result fail");
		SendErrorMsg(ERRNO_MSG_CSV_DATA, CMD_SC_TURNTABLE_TURN_RESULT);
		return;
	}
	int random = CUtil::RandByRange(0, StmpIndex.size());
//	int tindex = StmpIndex[random];
//	int tcount = StmpCount[random];
//	StmpIndex.erase(StmpIndex.begin() + random);
//	StmpCount.erase(StmpCount.begin() + random);
//	StmpIndex.push_back(tindex);
//	StmpCount.push_back(tcount);

	if (choose == TURNTABLE_NORMAL_CHOOSE)
	{
		GetCShop()->DelItem(TURNTABLE_SHOPID, SOURCE_ITEM_USE, turnconsume);
		GetCShop()->HandlerShopInfo(TURNTABLE_SHOPID);
	}
	else
	{
		GetCShop()->DelItem(SUPER_TURNTABLE_SHOPID, SOURCE_ITEM_USE, turnconsume);
		GetCShop()->HandlerShopInfo(SUPER_TURNTABLE_SHOPID);
	}

	WorldPacket info(CMD_SC_TURNTABLE_TURN_RESULT);
	info << (uint16) 0;
	info << (uint8_t) choose;
	info << number;
	info << (uint32_t) StmpIndex.size();
	for (int i = 0; i < StmpIndex.size(); i++)
	{
			info << (uint32_t)StmpCount[i];
			info << (uint32_t)StmpIndex[i];
	}
	info << (uint32_t) StmpIndex[random];
	SendPacket(&info);
}

void CRole::HandlerFirstPrize(WorldPacket &packet)
{
	if (!IS_FIRST_PRIZE(GetbyFlag()))
	{
		SendErrorMsg(ERRNO_MSG_FIRST_PRIZE, CMD_SC_FIRST_CHARGE_RESULT);
		return;
	}

	IME_CHAR_LOG("action [userid %u get first prize]", GetdwAccountId());
	uint32_t number = 0;
	uint16_t type = 0; 
	for (int i = 0; i < GlobalConfig::vecFirstPrizeType.size(); i++)
	{
		type = GlobalConfig::vecFirstPrizeType[i];
		if (type == E_OBJ_CARD_MORE)
		{
			number = GlobalConfig::vecFirstPrizePara2[i];
		}
		else
		{
			number += 1;
		}
	}

	if (GetclsCardPacket()->IsFull(number, 0))
	{
		SendErrorMsg(ERRNO_MSG_CARD_PACKET_NOT_ENOUGH, CMD_SC_FIRST_CHARGE_RESULT);
		return;
	}


	std::vector<CShopEffect::SGiveProduct> vect;
	for (int i = 0; i < GlobalConfig::vecFirstPrizeType.size(); i++)
	{
		CShopEffect::SGiveProduct tmp = {0};
		uint16_t type = GlobalConfig::vecFirstPrizeType[i];
		if (type < E_OBJ_MAX || type == E_OBJ_CARD_MORE)
		{
			CShopEffect::GiveGameProductSpecial(this, GlobalConfig::vecFirstPrizeType[i], GlobalConfig::vecFirstPrizePara1[i], GlobalConfig::vecFirstPrizePara2[i], tmp, SOURCE_FIRST_PRIZE);
		}
		vect.push_back(tmp);
	}

	SetbyFlag(CLOSE_FIRST_PRIZE(GetbyFlag()));
	SendProUpdate();
	SendObjEffect(vect);

	WorldPacket info(CMD_SC_FIRST_CHARGE_RESULT);
	info << (uint16_t) 0;
	SendPacket(&info);
}

void CRole::HandlerUpgradeReview(WorldPacket &packet)
{
	uint32_t upgradeindex;
	uint32_t objid;
	packet >> upgradeindex;
	packet >> objid;

	IME_CHAR_LOG("action [userid %u upgrade preview upgradeindex %u]", GetdwAccountId(), upgradeindex);
	CCard *pCard = m_clsCardPacket->Find(objid);
	if (pCard == NULL)
	{
		SendErrorMsg(ERRNO_MSG_CARD_NOT_IN_PACKET, CMD_SC_UPGRADE_REVIEW_RESULT);
		return;
	}
	STC_CONF_CARD_UPGRADE *pConf = CConfUpgrade::Find(upgradeindex);
	if (pConf == NULL)
	{
		IME_ERROR("upgrade index %u not in upgrade csv", upgradeindex);
		SendErrorMsg(ERRNO_MSG_CARD_NOT_IN_PACKET, CMD_SC_UPGRADE_REVIEW_RESULT);
		return;
	}
	if (pConf->dwCardIndex != pCard->GetdwIndex())
	{
		IME_ERROR("upgrade index %u upgrade card index %u but get card index %u", upgradeindex, pConf->dwCardIndex, pCard->GetdwIndex());
		SendErrorMsg(ERRNO_MSG_CSV_LOAD, CMD_SC_UPGRADE_REVIEW_RESULT);
		return;
	}
	
	CCard tcard;
	tcard.SetdwObjId(1);
	tcard.SetdwIndex(pConf->dwCardIndexDesc);
	if (tcard.LoadConf())
	{
		IME_ERROR("loadcard fail index %u", tcard.GetdwIndex());
		SendErrorMsg(ERRNO_MSG_CSV_DATA, CMD_SC_UPGRADE_REVIEW_RESULT);
		return;
	}
	uint32_t bnumber = std::min(pCard->GetdwLimitBreak(), tcard.GetdwLimitBreakNumber());
	tcard.SetdwLimitBreak(bnumber);
	tcard.SetbyRare(pCard->GetbyRare());
	tcard.SetdwExp(pCard->GetdwExp());
	tcard.Calculate();
	
	WorldPacket info(CMD_SC_UPGRADE_REVIEW_RESULT);
	info << (uint16_t) 0;
	tcard.SerializeClient(info);
	SendPacket(&info);
	return;
}

void CRole::HandlerUpgrade(WorldPacket &packet)
{
	uint32_t upgradeindex;
	uint32_t objid;
	packet >> upgradeindex;
	packet >> objid;

	IME_CHAR_LOG("action [userid %u upgrade card upgradeindex %u]", GetdwAccountId(), upgradeindex);

	CCard *pCard = m_clsCardPacket->Find(objid);
	if (pCard == NULL)
	{
		SendErrorMsg(ERRNO_MSG_CARD_NOT_IN_PACKET, CMD_SC_UPGRADE_RESULT);
		return;
	}
	STC_CONF_CARD_UPGRADE *pConf = CConfUpgrade::Find(upgradeindex);
	if (pConf == NULL)
	{
		IME_ERROR("upgrade index %u not in upgrade csv", upgradeindex);
		SendErrorMsg(ERRNO_MSG_CSV_LOAD, CMD_SC_UPGRADE_RESULT);
		return;
	}
	if (pConf->dwCardIndex != pCard->GetdwIndex())
	{
		IME_ERROR("upgrade index %u upgrade card index %u but get card index %u", upgradeindex, pConf->dwCardIndex, pCard->GetdwIndex());
		SendErrorMsg(ERRNO_MSG_CSV_DATA, CMD_SC_UPGRADE_RESULT);
		return;
	}

	STC_CONF_CARD_BASE *pDescConf = CConfCardBase::Find(pConf->dwCardIndexDesc);
	if (pDescConf == NULL)
	{
		IME_ERROR("desc card index %u can not find in card csv", pConf->dwCardIndexDesc);
		SendErrorMsg(ERRNO_MSG_CSV_DATA, CMD_SC_UPGRADE_RESULT);
		return;
	}
	if (pCard->GetbyType() != pDescConf->byType)
	{
		IME_ERROR("upgrade card type is not same upgradeindex %u", pConf->dwIndex);
		SendErrorMsg(ERRNO_MSG_CSV_DATA, CMD_SC_UPGRADE_RESULT);
		return;
	}

//	if (pCard->GetbyType() != E_ITEM_PET_CARD && pCard->GetbyType() != E_ITEM_MASTER_CARD)
//	{
//		IME_ERROR("upgrade card must be pet or master index %u", pCard->GetdwIndex());
//		SendErrorMsg(ERRNO_MSG_CSV_LOAD, CMD_SC_UPGRADE_RESULT);
//		return;
//	}

	std::map<uint32, std::vector<CConfUpgrade::SCard> > mapCard, mapCardTemp;
	std::map<uint32_t, uint32_t> mapItem, mapItemTemp;
	std::map<uint32, std::vector<CConfUpgrade::SCard> >::iterator itCard;
	std::map<uint32_t, uint32_t>::iterator itItem;

	if (pCard->GetbyType() == E_ITEM_PET_CARD || pCard->GetbyType() == E_ITEM_MASTER_CARD || pCard->GetbyType() == E_ITEM_WEAPON_CARD || pCard->GetbyType() == E_ITEM_ARMOR_CARD || pCard->GetbyType() == E_ITEM_JEWEL_CARD || pCard->GetbyType() == E_ITEM_ACCESSORIES_CARD)
	{
		m_clsCardPacket->PreDataForUpgrade(mapCard, objid);
		if (mapCard.size() == 0)
		{
			IME_DEBUG("card number is zero");
			SendErrorMsg(ERRNO_MSG_UPGRADE_NOT_ENOUGH, CMD_SC_UPGRADE_RESULT);
			return;
		}
		m_CShop->PreDataForUpgrade(mapItem);
		if (mapItem.size() == 0)
		{
			IME_DEBUG("item number is zero");
			SendErrorMsg(ERRNO_MSG_UPGRADE_NOT_ENOUGH, CMD_SC_UPGRADE_RESULT);
			return;
		}	
		if (pCard->GetwLevel() < pConf->dwLevelLimit)
		{
			SendErrorMsg(ERRNO_MSG_UPGRADE_LEVEL_LIMIT, CMD_SC_UPGRADE_RESULT);
			return;
		}

		mapCardTemp = mapCard;
		mapItemTemp = mapItem;
		if (!CConfUpgrade::Check(mapCardTemp, mapItemTemp, upgradeindex))
		{
			IME_ERROR("upgrade stuff is not enough, index %u", upgradeindex);
			SendErrorMsg(ERRNO_MSG_UPGRADE_NOT_ENOUGH, CMD_SC_UPGRADE_RESULT);
			return;
		}
	}
	else
	{
		IME_ERROR("card type %u is not valid", pCard->GetbyType());
		SendErrorMsg(ERRNO_MSG_CSV_DATA, CMD_SC_UPGRADE_RESULT);
		return;
	}

	for (itCard = mapCardTemp.begin(); itCard != mapCardTemp.end(); itCard++)
	{
		for (int i = 0; i < itCard->second.size(); i++)
		{
			m_clsCardPacket->CardDel(itCard->second[i].objid, SOURCE_UPGRADE);
			m_clsCardPacket->SingleCardDelUpdate(itCard->second[i].objid);
		}
	}

//	m_clsCardPacket->CardDel(objid);
//	m_clsCardPacket->SingleCardDelUpdate(objid);
	for (itItem = mapItemTemp.begin(); itItem != mapItemTemp.end(); itItem++)
	{
		m_CShop->DelItem(itItem->first, SOURCE_UPGRADE, itItem->second);
		m_CShop->HandlerShopInfo(itItem->first);
	}
//	uint32_t objiddesc;
//	m_clsCardPacket->CardAddByParaWithExp(pConf->dwCardIndexDesc, objiddesc, pCard->GetdwExp(), pCard->GetdwLimitBreak(), pCard->GetbyRare(), true); 
//	m_clsCardPacket->SingleCardAddUpdate(objiddesc);
	pCard->Upgrade(pConf->dwCardIndexDesc);
	pCard->SendProUpdate();
	uint32_t location;
	uint8_t team;
	if (m_clsCardPacket->IsInEquip(pCard, location, team))
	{
		m_clsCardPacket->Calculate(true);
	}
	
	WorldPacket infobreak(CMD_SC_CARD_BREAK_AWAKE);
	infobreak << (uint8_t) 1;
	pCard->SerializeClient(infobreak);
	SendPacket(&infobreak);

	WorldPacket info(CMD_SC_UPGRADE_RESULT);
	info << (uint16_t) 0;
	info << upgradeindex;
	info << objid;
	SendPacket(&info);
	GetCMission()->EveryCardUpgrade(0,1);
	return;
}

bool mylevelbig(SJoin a, SJoin b)
{
	return a.level > b.level;
}

void CRole::HandlerJoinPlayerInfo(WorldPacket &packet)
{
	IME_CHAR_LOG("action [userid %u join request]", GetdwAccountId());
	std::vector<SJoin> m_vecF,m_vecU;	
	int number = 0;	
	for (int i = 0; i < m_vecFJoin.size(); i++)
	{
		uint32_t userid = m_vecFJoin[i].userid;
		if (m_mapJoin.find(userid) == m_mapJoin.end() ||  m_mapJoin[userid] < JOIN_EVERY_COUNT)
		{
			m_vecF.push_back(m_vecFJoin[i]);
			number++;
			if (number >= 4)
				break;
		}
	}
	
	int unfamilynumber = 0;	
	for (int i = 0; i < m_vecUJoin.size(); i++)
	{
		uint32_t userid = m_vecUJoin[i].userid;
		if (m_mapJoin.find(userid) == m_mapJoin.end() || m_mapJoin[userid] < JOIN_EVERY_COUNT)
		{
			m_vecU.push_back(m_vecUJoin[i]);
			unfamilynumber++;
			if (number + unfamilynumber >= 8)
				break;
		}
	}

	WorldPacket info(CMD_SC_JOIN_PLAYER_RESULT);
	info << (uint16_t) 0;
	info << (uint32_t) (m_vecF.size() + m_vecU.size());
	for (int i = 0; i < m_vecF.size(); i++)
	{
		info << m_vecF[i].userid;
		info << m_vecF[i].level;
		info << m_vecF[i].username;
		info << m_vecF[i].masterindex;
		info << (uint8_t) 1;
		IME_DEBUG("friend id %u", m_vecF[i].userid);
	}
	for (int i = 0; i < m_vecU.size(); i++)
	{
		info << m_vecU[i].userid;
		info << m_vecU[i].level;
		info << m_vecU[i].username;
		info << m_vecU[i].masterindex;
		info << (uint8_t) 0;
		IME_DEBUG("user id %u", m_vecU[i].userid);
	}
	info << m_dwTotalJoin;
	uint16_t maxnumber = 0;
	STC_CONF_VIP *pVipConf = CConfVip::Find(GetwVipLv());
	if (pVipConf == NULL)
	{
		IME_ERROR("vip lv %u can not find in vip csv", GetwVipLv());
	}
	else
	{
		maxnumber = pVipConf->dwJoinMax;
	}
	info << maxnumber;
//	IME_ERROR("now %u, max %u, get %u", m_dwTotalJoin, maxnumber, m_vecFJoin.size() + m_vecUJoin.size());
	SendPacket(&info);
	return;
}

void CRole::SendObjEffect(std::vector<CShopEffect::SGiveProduct> tmp)
{
	WorldPacket info(CMD_SC_PRODUCT_GET_INFO);
	info << (uint16_t) PRODUCT_GET;
	info << (uint32_t) tmp.size();
	for (int i = 0; i < tmp.size(); i++)
	{
		info << tmp[i].effect;
		info << tmp[i].para1;
		info << tmp[i].para2;
	}
	SendPacket(&info);
}

void CRole::SendObjEffectByHappenBoss()
{
	WorldPacket info(CMD_SC_PRODUCT_GET_INFO);
	info << (uint16_t) HAPPEN_BOSS;
	SendPacket(&info);
}

void CRole::SendObjEffectByHappenRole()
{
	if (m_vecUJoin.size() == 0)
		return;
	int ret = CUtil::RandByRange(0, m_vecUJoin.size());
	
	WorldPacket info(CMD_SC_PRODUCT_GET_INFO);
	info << (uint16_t) HAPPEN_ROLE;
	info << m_vecUJoin[ret].userid;
	info << m_vecUJoin[ret].level;
	info << m_vecUJoin[ret].username;
	info << m_vecUJoin[ret].masterindex;
	info << m_vecUJoin[ret].breakcount;
	info << m_vecUJoin[ret].maxbreakcount;
	info << m_vecUJoin[ret].awake;
	info << m_vecUJoin[ret].sigature;
	SendPacket(&info);
	return;
}

void CRole::LoadFriendData()
{
//	uint32_t interval = getMSTime();
//	uint32_t before = interval;

	m_vecFJoin.clear();
	m_vecUJoin.clear();
	std::vector<CDBCtrl::stuPlayerPlus> vecTmp, vecTmp2;
	CDBCtrl::GetUserBaseDataInFriend(GetdwAccountId(), vecTmp2);

	std::vector<int> vecVal2;
	if (vecTmp2.size() <= MAX_JOIN_NUMBER)
	{
		vecTmp = vecTmp2;
	}
	else
	{
		vecVal2 = CUtil::RandByRangeMulti(0, vecTmp2.size(), MAX_JOIN_NUMBER);
		for (int i = 0; i < vecVal2.size(); i++)
		{
			vecTmp.push_back(vecTmp2[vecVal2[i]]);
		}
	}

	for(int i = 0; i < vecTmp.size(); i++)
	{
		uint32_t userid = vecTmp[i].dwUserid;
		CBaseRoleData *pCRole = NULL;
//		CRole *pCRole = sWorld->FindUserRole(userid);
		CBaseRoleData temp;
//		CRole temp(NULL);
		if (pCRole == NULL)
		{
			pCRole = &temp;
			assert(pCRole != NULL);
			if (!pCRole->Init(userid))
//			if (!pCRole->LoadDataForBaseData(userid))
			{
				IME_ERROR("load peer role from DB fail userid %u", userid);
				SendErrorMsg(ERRNO_MSG_DB_NO_DATA_OR_ERROR, CMD_SC_JOIN_PLAYER_RESULT);
				return;
			}
		}
//		std::vector<uint32_t> a;
//		uint32_t masterindex = 0;
//		if (pCRole->GetclsCardPacket()->GetCardIndex(masterindex, a))
//		{
//			pCRole->SendErrorMsg(ERRNO_MSG_DB_NO_DATA_OR_ERROR, CMD_SC_JOIN_PLAYER_RESULT);
//			return;
//		}
		SJoin tmp;
		pCRole->PreDataRole(tmp);
//		pCRole->GetclsCardPacket()->PreDataRole(tmp);
//		tmp.userid = vecTmp[i].dwUserid;
//		tmp.level = vecTmp[i].wLevel;
//		tmp.username = vecTmp[i].strName;
//		tmp.masterindex = masterindex;
		m_vecFJoin.push_back(tmp);
	}	
//	before = interval;
//	interval = getMSTime() - interval;
//	if (interval >= NETWORK_CHECK_SEC)
//	IME_LOG("===now fjoin loadfrienddata before %u total %u usercount %u select %u===", before, interval, vecTmp.size(), vecTmp2.size());

//	interval = getMSTime();
	std::stable_sort(m_vecFJoin.begin(), m_vecFJoin.end(), mylevelbig);
//	before = interval;
//	interval = getMSTime() - interval;
//	if (interval >= NETWORK_CHECK_SEC)
//	IME_LOG("===now fjoin sort before %u total %u usercount %u===", before, interval, vecTmp.size());

//	interval = getMSTime();
	std::vector<CDBCtrl::stuPlayer> vecTmpU, vecTmpU2;
	CDBCtrl::GetUserBaseDataAuto10ForFriend(GetdwAccountId(), GetwLevel(), vecTmpU2);

	std::vector<int> vecVal;
	if (vecTmpU2.size() <= MAX_JOIN_NUMBER)
	{
		vecTmpU = vecTmpU2;
	}
	else
	{
		vecVal = CUtil::RandByRangeMulti(0, vecTmpU2.size(), MAX_JOIN_NUMBER);
		for (int i = 0; i < vecVal.size(); i++)
		{
			vecTmpU.push_back(vecTmpU2[vecVal[i]]);
		}
	}

	for(int i = 0; i < vecTmpU.size(); i++)
	{
		uint32_t userid = vecTmpU[i].dwUserid;
		if (m_mapJoin.find(userid) == m_mapJoin.end())
		{
			CBaseRoleData *pCRole = NULL;
//			CRole *pCRole = sWorld->FindUserRole(userid);
			CBaseRoleData temp;
//			CRole *pCRole = NULL;
//			CRole temp(NULL);
			if (pCRole == NULL)
			{
				pCRole = &temp;
				assert(pCRole != NULL);
				if (!pCRole->Init(userid))
//				if (!pCRole->LoadDataForBaseData(userid))
				{
					IME_ERROR("load peer role from DB fail userid %u", userid);
					SendErrorMsg(ERRNO_MSG_DB_NO_DATA_OR_ERROR, CMD_SC_JOIN_PLAYER_RESULT);
					return;
				}
			}
//			std::vector<uint32_t> a;
//			uint32_t masterindex = 0;
//			if (pCRole->GetclsCardPacket()->GetCardIndex(masterindex, a))
//			{
//				pCRole->SendErrorMsg(ERRNO_MSG_DB_NO_DATA_OR_ERROR, CMD_SC_JOIN_PLAYER_RESULT);
//				return;
//			}
			SJoin tmp;
			pCRole->PreDataRole(tmp);
//			pCRole->GetclsCardPacket()->PreDataRole(tmp);
//			tmp.userid = vecTmpU[i].dwUserid;
//			tmp.level = vecTmpU[i].wLevel;
//			tmp.username = vecTmpU[i].strName;
//			tmp.masterindex = masterindex;
			m_vecUJoin.push_back(tmp);
		}
	}	
//	before = interval;
//	interval = getMSTime() - interval;
//	if (interval >= NETWORK_CHECK_SEC)
//	IME_LOG("===now ujoin loadfrienddata before %u total %u usercount %u select %u==", before, interval, vecTmpU.size(), vecTmpU2.size());

//	interval = getMSTime();
	std::stable_sort(m_vecUJoin.begin(), m_vecUJoin.end(), mylevelbig);
//	before = interval;
//	interval = getMSTime() - interval;
//	if (interval >= NETWORK_CHECK_SEC)
//	IME_LOG("===now ujoin sort before %u total %u usercount %u===", before, interval, vecTmpU.size());
}

void CRole::HandlerRoleInfoUpdate(WorldPacket &packet)
{
	std::string name;
	std::string sigature;
	packet >> name;
	packet >> sigature;
	uint32_t order_id;
	IME_CHAR_LOG("action [userid %u change name %s and sigature %s]", GetdwAccountId(), name.c_str(), sigature.c_str());
	
	if (m_byFirstChangeName && name != "")
	{
		if (GetdwDollar() < GlobalConfig::ChangeConsume)
		{
			SendErrorMsg(ERRNO_MSG_DOLLAR_NOT_ENGOUH, CMD_SC_ROLE_INFO_UPDATE_RESULT);
			return;
		}

		if (!CDBCtrl::GetShopOrderId(GetdwAccountId(), order_id))
		{
			SendErrorMsg(ERRNO_MSG_SHOP_GET_ORDER_ID, CMD_SC_ROLE_INFO_UPDATE_RESULT);
			return;
		}
	}

	if (m_byFirstChangeName && name != "")
	{
		if (ChangeDollar(-GlobalConfig::ChangeConsume, SOURCE_CHANGE_ROLE_INFO))
		{
			SendErrorMsg(ERRNO_MSG_DOLLAR_CONSUME_FAIL, CMD_SC_ROLE_INFO_UPDATE_RESULT);
			return;
		}

		CDBCtrl::LogShopRecord(GetdwAccountId(), order_id, GlobalConfig::ChangeConsume, 1, 1, E_DC_CHANGE_ROLE);	
	}
	
	if (name != "")
	{
		if (sWorld->IsExists(name))
		{
			SendErrorMsg(ERRNO_MSG_USER_NAME_ALREADY_USE, CMD_SC_ROLE_INFO_UPDATE_RESULT);
			return;
		}
		sWorld->DelUserName(GetstrUserName());
//		IME_DEBUG("name %s", name.c_str());
//		IME_DEBUG("before name variable %s", m_strUserName.c_str());
		CNameChecker::ReplaceString(name);
		SetstrUserName(name);
//		IME_DEBUG("now name %s", GetstrUserName().c_str());
//		IME_DEBUG("after name variable %s", m_strUserName.c_str());
		if (!m_byFirstChangeName)
			m_byFirstChangeName = 1;
		sWorld->InsertUserName(name);
	}
	if (sigature != "")
	{
		IME_DEBUG("signature %s", sigature.c_str());
		CNameChecker::ReplaceString(sigature);
		SetstrSignature(sigature);
	}
	SendProUpdate();
	HandlerInfoOpt();

	WorldPacket info(CMD_SC_ROLE_INFO_UPDATE_RESULT);
	info << (uint16_t) 0;
	SendPacket(&info);
}

bool CBaseRoleData::Init(uint32 dwAccountId)
{
	
	m_dwAccountId = dwAccountId;
	bool bIsSucc = CDBCtrl::GetUserBaseData(
			m_strUserName,
			m_wLevel,
			m_dwOneLevelExp,
			m_wVipLv,
			m_dwVipExp,
			m_dwDollar,
			m_dwCoin,
			m_dwEnergy,
			m_dwEnergyMax,
			m_dwEnergyRate,
			m_dwStrength,
			m_dwStrengthMax,
			m_dwStrengthRate,
			m_dwAccountId,
			m_dwInitNumber,
			m_byTaskLocation1,
			m_byTaskLocation2,
			m_byTaskLocation3,
			m_byTaskLocation4,
			m_byCardCapacity,
			m_dwExplimit,
			m_dwEnergyCnt,
			m_dwStrengthCnt,
			m_byFlag,
			m_lFindGroupId,
			m_dwAvoidDurationTime,
			m_dwWorldHistory,
			m_dwWorldBroadTime,
			m_dwGameFlow,
			m_dwLuck,
			m_byGm,
			m_dwChargeLuck,
			m_dwStamina,
			m_dwStaminaTime,
			m_strSignature
			);

	if (!bIsSucc)
	{
		IME_ERROR("load data fail from user db user_id %u", dwAccountId);
		return false;
	}
	m_dwFriendLimit = 0;

	int ret;

    std::string attack_team;
    std::string defense_team;
    std::string lineup;
    std::string cards;
	std::string base;
	std::string equip1;
	std::string master;
	std::string equip_master;
    if (!CDBCtrl::GetUserCardPacketBaseData(attack_team, lineup, GetdwAccountId(), equip1, master, equip_master, base))
    {
		IME_ERROR("load data fail from db");
        return false;
    }
//  StrSplit(lineup, ",", m_vecLineUp);
//	m_vecLineUp.resize(MAX_CARD_CAPACITY);

	std::vector<uint32_t> vecMaster;
	CUtil::StrSplit(master, ",", vecMaster);
	if (vecMaster.size() == 0)
	{
		m_MasterCard = 0;
	}
	else
	{
		m_MasterCard = vecMaster[0];
	}

//    StrSplit(attack_team, ",", m_vecAttackTeam);

//	std::vector<std::string> vectemp;
//	std::vector<uint32_t> vectempint;
//	int i;
//	CUtil::StrSplit(equip1, "|", vectemp);	
//	for (i = 0; i < vectemp.size(); i++)
//	{
//		StrSplit(vectemp[i], ",", vectempint);
//		if (vectempint.size() < 8)
//			vectempint.resize(8, 0);
//		if (vectempint.size() != 8)
//		{
//			IME_ERROR("cardpacket equip column is not 8");
//			return -1;
//		}
//		equip_t tempequip;
//		tempequip.dwWeaponObjId = vectempint[0];
//	   	tempequip.dwArmorObjId = vectempint[1];
//	   	tempequip.dwJewelObjId = vectempint[2];
//	   	tempequip.dwAccessoriesObjId = vectempint[3];
//	   	tempequip.dwSkillObjId = vectempint[4];
//		tempequip.dwMasterGuide1 = vectempint[5];
//		tempequip.dwMasterGuide2 = vectempint[6];
//		tempequip.dwMasterGuide3 = vectempint[7];
//
//		m_vecEquip1.push_back(tempequip);
//
//		IME_DEBUG("1st %u weapon armor jewel %u %u %u", i, vectempint[0], vectempint[1], vectempint[2]);
//	}
//	StrSplit(equip_master, ",", vectempint); 
//	if (vectempint.size() != 0)
//	{
//		if (vectempint.size() < 8)
//			vectempint.resize(8, 0);
//		if (vectempint.size() != 8)
//		{
//			IME_ERROR("cardpacket equip column is not 8");
//			return -1;
//		}
//		m_MasterEquip.dwWeaponObjId = vectempint[0];
//		m_MasterEquip.dwArmorObjId = vectempint[1];
//		m_MasterEquip.dwJewelObjId = vectempint[2];
//		m_MasterEquip.dwAccessoriesObjId = vectempint[3];
//		m_MasterEquip.dwSkillObjId = vectempint[4];
//		m_MasterEquip.dwMasterGuide1 = vectempint[5];
//		m_MasterEquip.dwMasterGuide2 = vectempint[6];
//		m_MasterEquip.dwMasterGuide3 = vectempint[7];
//	}
	
//    deserialize(cards);

//	m_vecView = m_vecLineUp;
//	for (int i = 0; i < m_vecView.size(); )
//	{
//		if (m_vecView[i] == 0)
//		{
//			m_vecView.erase(m_vecView.begin() + i);
//		}
//		else
//		{
//			i++;
//		}
//	}

//	if (EquipCardInfo())
//	{
//		IME_ERROR("card equip fail");
//		return false;
//	}


//    if (Calculate(false))
//	{
//		IME_ERROR("card packet calculate fail");
//		return false;
//	}
	
	m_mapCards.clear();
	ByteBuffer by;
	by.append((const char *)base.data(), base.size());
	by >> m_dwTotalForce;
	uint32_t number;
	by >> number;
	CARD_BASE c;
	uint32_t objid;
	for (int i = 0; i < number; i++)
	{
		by >> objid;
		by >> c.index;
		by >> c.lv;
		by >> c.breaknumber;
		by >> c.breakmax;
		by >> c.rare;
		m_mapCards[objid] = c;
	}
        
//	m_dwExp = m_dwOneLevelExp + m_dwLastExplimit;

	uint32_t last_login_time, last_logout_time, cur_login_time, login_day;
	std::string last_login_ip;
	if (CDBCtrl::GetUserLoginData(m_dwAccountId, last_login_time, last_logout_time, cur_login_time, login_day, last_login_ip))
	{
		m_dwLastLoginTime = last_login_time; 
	}

	return true;

}

void CBaseRoleData::PreDataRole(SJoin &tmp)
{
	tmp.userid = 0;
	tmp.masterindex = 0;

	tmp.userid = GetdwAccountId();
	tmp.level = GetwLevel();
	tmp.username = GetstrUserName();
	if (m_mapCards.find(m_MasterCard) == m_mapCards.end())
	{
		IME_ERROR("can not find this card index %u", m_MasterCard);
		return;
	}
	CARD_BASE *pCard = &m_mapCards[m_MasterCard];
	tmp.masterindex = pCard->index;
	tmp.breakcount = pCard->breaknumber;
	tmp.maxbreakcount = pCard->breakmax;
	tmp.awake = pCard->rare;
	tmp.sigature = GetstrSignature();
}

void CBaseRoleData::PreDataTeamCardDataForFriend(ByteBuffer &by)
{
	uint32 index = 0;
	if (m_mapCards.find(m_MasterCard) == m_mapCards.end())
	{
		IME_ERROR("can not find this card index %u", m_MasterCard);
//		return;
	}
	else
	{
		CARD_BASE *pCard = &m_mapCards[m_MasterCard];
		index = pCard->index;
	}
	by << m_dwTotalForce;
	by << index;
	by << m_dwLastLoginTime;
}

uint32 CRole::GetPower(){
	return this->GetclsCardPacket()->GetPower();
}

void CRole::SetAttackTimes(uint32 times){
	this->GetclsCardPacket()->setAttack(times);
}

uint32 CRole::getMultipleCardExp(){
	if(multipleCardExp.size() == 0){
		return 1;
	}
	uint32 num = multipleCardExp.back()[0];
	if(num > 0 && num -1 == 0){
		uint32 times = multipleCardExp.back()[1];
		multipleCardExp.pop_back();
		return times;
	}else if(num - 1 > 0){
		uint32 times = multipleCardExp.back()[1];
		multipleCardExp.back()[0] = multipleCardExp.back()[0]-1;
		return times;
	}
	return 1;
}
void CRole::dealMultipleCardExp(uint32 num,uint32 times){
	std::vector<uint32> multriple;
	multriple.push_back(num);
	multriple.push_back(times);
	if (multipleCardExp.size() == 0) {
		multipleCardExp.push_back(multriple);
	} else {
		multipleCardExp.insert(multipleCardExp.begin(), multriple);
	}
	std::cout << multipleCardExp.size() << std::endl;
}

void CRole::initMultipleCardExp(){
	//取到双倍经验丹数据
	std::string str;
	if (!CDBCtrl::GetUserMultipleCardExp(m_dwAccountId, str)) {
		IME_ERROR("get multriple card exp data fail, user_id %u",m_dwAccountId);
		return;
	}
	ByteBuffer multipleExp;
	multipleExp.append(str.data(), str.size());
	if (multipleExp.size() == 0)
		return;
	uint32_t number;
	multipleExp >> number;
	for (int i = 0; i < number; i++) {
		std::vector<uint32> exp;
		uint32 multiple;
		uint32 multipleNum;

		multipleExp >> multipleNum;
		exp.push_back(multipleNum);

		multipleExp >> multiple;
		exp.push_back(multiple);

		multipleCardExp.push_back(exp);
	}
}
void CRole::saveMultipleCardExp(){
	ByteBuffer by;
	by << (uint32_t)multipleCardExp.size();
	for(int i = 0; i < multipleCardExp.size(); i++){
		for(int j = 0; j < multipleCardExp[i].size(); j++){
			by << (uint32_t)multipleCardExp[i][j];
		}
	}
	std::string tempByte;
	tempByte.assign((const char *)by.contents(), by.size());
	CDBCtrl::SaveUserMultipleCardExp(m_dwAccountId,tempByte);
}

