/*
 * =====================================================================================
 *
 *       Filename:  Mission.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2012年09月03日 16时01分11秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  imane (), imanecr@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include <vector>
#include "stdint.h"
#include "Mission.h"
#include "Role.h"
#include "CommonConfig.h"
#include "EnumDefines.h"
#include "CmdDefine.h"
#include "ErrorMsg.h"
#include "CardPacket.h"
#include "Arena.h"
#include "Mail.h"
#include "Other.h"

#define MISSION_CHECK_SP(T, COMP, COMP2, KIND, CCOMP) \
CConfAchieve::MapData data = CConfAchieve::GetData();\
CConfAchieve::MapData::iterator it;\
for (it = data.begin(); it != data.end(); it++)\
{\
	STC_CONF_ACHIEVE &achieve = it->second;\
	uint8_t needupdate = 0;\
	if (achieve.dwMissionEffect == T)\
	{\
		if (achieve.dwMinLevel != 0)\
		{\
			if (m_pCRole->GetwLevel() < achieve.dwMinLevel)\
			{\
				continue;\
			}\
		}	\
		if (achieve.dwMaxLevel != 0)\
		{\
			if (m_pCRole->GetwLevel() > achieve.dwMaxLevel)\
			{\
				continue;\
			}\
		}\
		if (achieve.dwCompeteObjId != 0)\
		{\
			if (achieve.dwCompeteObjId != objid)\
			{\
				continue;\
			}\
		}\
		if (m_mapAchieve.find(achieve.dwIndex) == m_mapAchieve.end())\
		{\
			SMission tmp;\
			tmp.status = 0;\
			tmp.step = 0;\
			m_mapAchieve[achieve.dwIndex] = tmp; 	\
		}\
		if (m_mapAchieve[achieve.dwIndex].status == MISSION_NONE)	\
		{\
			if (achieve.dwStep != 0)\
			{\
				/* IME_DEBUG("before achieve index %u type %u status %u step %u val %u", achieve.dwIndex, T, m_mapAchieve[achieve.dwIndex].status, m_mapAchieve[achieve.dwIndex].step, val);*/\
				if (KIND == "plus")\
				{\
					m_mapAchieve[achieve.dwIndex].step COMP val;\
					needupdate = 1;\
				}\
				else if (KIND == "rank")\
				{\
					if (m_mapAchieve[achieve.dwIndex].step == 0)\
					{\
						m_mapAchieve[achieve.dwIndex].step COMP val;\
						needupdate = 1;\
					}\
					else\
					{\
						if (m_mapAchieve[achieve.dwIndex].step CCOMP val)\
						{\
							m_mapAchieve[achieve.dwIndex].step COMP val;\
							needupdate = 1;\
						}\
					}\
				}\
				else\
				{\
					if (m_mapAchieve[achieve.dwIndex].step CCOMP val)\
					{\
						m_mapAchieve[achieve.dwIndex].step COMP val;\
						needupdate = 1;\
					}\
				}\
				/* IME_DEBUG("after achieve index %u type %u status %u step %u val %u", achieve.dwIndex, T, m_mapAchieve[achieve.dwIndex].status, m_mapAchieve[achieve.dwIndex].step, val);*/\
			}\
			if (m_mapAchieve[achieve.dwIndex].step COMP2 achieve.dwStep)\
			{\
				if (achieve.wAchieveType == ACHIEVE)\
				{\
					m_mapAchieve[achieve.dwIndex].status = MISSION_COMPLETE;\
					needupdate = 1;\
					CMail::SBaseAttr tmpAttr = {0};\
					tmpAttr.coin = achieve.dwCoin;\
					tmpAttr.dollar = achieve.dwDollar;\
					tmpAttr.energy = achieve.dwEnergy;\
					tmpAttr.strength = achieve.dwStrength;\
					tmpAttr.worldhistory = achieve.dwWorldExp;\
					uint32_t mail_id = 0;\
					std::string con = CUtil::StrReplace(GlobalConfig::AchieveMailContent, std::string("%s"), achieve.strTitle);\
					CDBCtrl::stuMailDetail tmp = {0};\
					CMail::AddEmailSysDiy(m_pCRole->GetdwAccountId(), mail_id, tmp, achieve.dwObjType, achieve.dwPara1, achieve.dwPara2, con, tmpAttr, GlobalConfig::AchieveMailTitle);\
					CMail::EmailUpdate(m_pCRole, CMail::ADD, &tmp, 0);\
					m_dwAchieve += achieve.dwAchieveVal;\
					HandlerMissionBaseInfo();\
				}\
				else if (achieve.wAchieveType == SHARE)\
				{\
					std::vector<SProduct> vecp;\
					SProduct tmpobj;\
					tmpobj.type = E_OBJ_COIN;\
					tmpobj.para2 = achieve.dwCoin;\
					if (achieve.dwCoin != 0)\
					vecp.push_back(tmpobj);\
					tmpobj.type = E_OBJ_DOLLAR;\
					tmpobj.para2 = achieve.dwDollar;\
					if (achieve.dwDollar != 0)\
					vecp.push_back(tmpobj);\
					std::vector<CShopEffect::SGiveProduct> vec;\
					for (int i = 0; i < vecp.size(); i++)\
					{\
						CShopEffect::SGiveProduct ObjEffect = {0};\
						CShopEffect::GiveGameProductSpecial(m_pCRole, vecp[i].type, vecp[i].para1, vecp[i].para2, ObjEffect, SOURCE_SNS);\
						vec.push_back(ObjEffect);\
					}\
\
					m_pCRole->SendObjEffect(vec);\
					m_mapAchieve[achieve.dwIndex].status = MISSION_COMPLETE;\
					needupdate = 1;\
				}\
				else if (achieve.wAchieveType == ACTIVE_MISSION)\
				{\
					m_mapAchieve[achieve.dwIndex].status = MISSION_COMPLETE;\
					m_dwActive += achieve.dwActive;\
					needupdate = 1;\
					HandlerMissionBaseInfo();\
					HandlerMissionActiveInfo();\
				}\
				else\
				{\
					m_mapAchieve[achieve.dwIndex].status = MISSION_PRIZE;\
					needupdate = 1;\
				}\
				if (achieve.wAchieveType != SHARE && achieve.wAchieveType != ACTIVE_MISSION)\
				{\
					CDataPush push;\
					push.HandlerDataPushMissionComplete(m_pCRole, achieve.dwIndex);\
				}\
			}\
		}\
	}\
	if (needupdate)\
	{\
		HandlerMissionInfo(it->first);\
	}\
}

#define MISSION_CHECK(T, COMP) MISSION_CHECK_SP(T, COMP, >=, "plus", =);

/*
 *--------------------------------------------------------------------------------------
 *       Class:  CMission
 *      Method:  CMission
 * Description:  constructor
 *--------------------------------------------------------------------------------------
 */

CMission::CMission ():m_IsInit(false),m_dwAchieve(0),m_dwFlushTime(0),m_dwActive(0)
{
	m_pCRole = NULL;
}  /* -----  end of method CMission::CMission  (constructor)  ----- */

CMission::~CMission()
{

}

int CMission::Init(CRole *pCRole)
{
	m_pCRole = pCRole;
	if (LoadData())
	{
		m_IsInit = false;
		IME_ERROR("load data fail");
		return -1;
	}
	m_IsInit = true;
	//this->m_dwFlushTime = time(NULL) - 86400;
	//如果不是当天就直接把活跃度清空
	OnTimer5s();
	return 0;

}

void CMission::SaveData()
{
	if (!m_IsInit)
		return;
//	if (!m_IsUpdate)
//		return;
	ByteBuffer byte;
	if (SerializeDB(byte))
	{
		IME_ERROR("SerilizeDB fail");
		return;
	}
	std::string temp;
	temp.assign((const char *)byte.contents(), byte.size());
	CDBCtrl::SaveUserDataAchieve(temp, m_pCRole->GetdwAccountId());
}

int CMission::LoadData()
{
	std::string temp;
	if (!CDBCtrl::GetUserDataAchieve(temp, m_pCRole->GetdwAccountId()))
	{
		IME_ERROR("get achieve data fail, user_id %u", m_pCRole->GetdwAccountId());
		return -1;
	}
	ByteBuffer content;
	content.append(temp.data(), temp.size());
	if (DeserializeDB(content))
	{
		IME_ERROR("serilizeDB fail");
		return -1;
	}
	return 0;
}

int CMission::SerializeDB (ByteBuffer &pkg)
{
	pkg << m_dwAchieve;
	pkg << m_dwFlushTime;
	pkg << (uint32_t)m_mapAchieve.size();
	std::map<uint32_t, SMission>::iterator it;
	for (it = m_mapAchieve.begin(); it != m_mapAchieve.end(); it++)
	{
		pkg << it->first;
		pkg << it->second.status;
		pkg << it->second.step;
	}
	pkg << m_dwActive;
	pkg << (uint32_t) m_setActive.size();
	std::set<uint32_t>::iterator ita;
	for (ita = m_setActive.begin(); ita != m_setActive.end(); ita++)
	{
		pkg << (uint32_t)*ita;
	}
	return 0;
}               /*  -----  end of method CMission::SerializeDB  ----- */

int CMission::DeserializeDB (ByteBuffer &pkg)
{
	pkg >> m_dwAchieve;
	pkg >> m_dwFlushTime;
	uint32_t number;
	pkg >> number;
	uint32_t index;
	SMission tmp;
	for (int i = 0; i < number; i++)
	{
		pkg >> index;
		pkg >> tmp.status;
		pkg >> tmp.step;
		m_mapAchieve[index] = tmp;
	}
	pkg >> m_dwActive;
	m_setActive.clear();
	pkg >> number;
	std::set<uint32_t>::iterator it;
	uint32_t val;
	for (int i = 0; i < number; i++)
	{
		pkg >> val;
		m_setActive.insert(val);
	}
	return 0;
}               /*  -----  end of method CMission::DeserializeDB  ----- */

void CMission::OnTimer5s()
{
//	IME_DEBUG("mission time %u", m_dwFlushTime);
	if (!CUtil::IsToday(m_dwFlushTime))
	{
		std::map<uint32_t, SMission>::iterator it;
		STC_CONF_ACHIEVE *pConf = NULL;
		for (it = m_mapAchieve.begin(); it != m_mapAchieve.end(); it++)
		{
			pConf = CConfAchieve::Find(it->first);
			if (pConf == NULL)
			{
				IME_ERROR("can not find achieve index %u userid %u", it->first, m_pCRole->GetdwAccountId());
				continue;
			}
			if (pConf->wAchieveType == EVERYDAY_MISSION || pConf->wAchieveType == SHARE )
			{
				m_mapAchieve[it->first].status = MISSION_NONE;
				m_mapAchieve[it->first].step = 0;
			}
			//每日刷新活跃度列表
			if (pConf->wAchieveType == ACTIVE_MISSION )
			{
				m_mapAchieve[it->first].status = MISSION_NONE;
				m_mapAchieve[it->first].step = 0;
			}

		}
		m_dwFlushTime = time(NULL);
		m_setActive.clear();
		m_dwActive = 0;
	}
}

void CMission::EveryWorld(uint32_t objid, uint32_t val)
{
	MISSION_CHECK(EVERY_WORLD, +=);
}

void CMission::EveryMatch(uint32_t objid, uint32_t val)
{
	MISSION_CHECK(EVERY_MATCH, +=);
}
void CMission::EveryMaster(uint32_t objid, uint32_t val)
{
	MISSION_CHECK(EVERY_MASTER, +=);
}
void CMission::EveryRob(uint32_t objid, uint32_t val)
{
	MISSION_CHECK(EVERY_ROB, +=);
}
void CMission::EveryStrength(uint32_t objid, uint32_t val)
{
	MISSION_CHECK(EVERY_STRENGTH, +=);
}
void CMission::EveryWorldBoss(uint32_t objid, uint32_t val)
{
	MISSION_CHECK(EVERY_WORLDBOSS, +=);
}
void CMission::GetFourCard(uint32_t objid, uint32_t val)
{
	MISSION_CHECK(GET_FOUR_CARD, +=);
}
void CMission::GetFiveCard(uint32_t objid, uint32_t val)
{
	MISSION_CHECK(GET_FIVE_CARD, +=);
}
void CMission::NormalTask(uint32_t objid, uint32_t val)
{
	MISSION_CHECK(NORMAL_TASK, +=);
}
void CMission::HighTask(uint32_t objid, uint32_t val)
{
	MISSION_CHECK(HIGH_TASK, +=);
}
void CMission::RoleLevel(uint32_t objid, uint32_t val)
{
	MISSION_CHECK_SP(ROLE_LEVEL, =, >=, "equal", <);
}
void CMission::CardLevel(uint32_t objid, uint32_t val)
{
	MISSION_CHECK_SP(CARD_LEVEL, =, >=, "equal", <);
}
void CMission::CardBreakNumber(uint32_t objid, uint32_t val)
{
	MISSION_CHECK(CARD_BREAK_NUMBER, +=);
}
void CMission::ContinueLogin(uint32_t objid, uint32_t val)
{
	MISSION_CHECK_SP(CONTINUE_LOGIN, =, >=, "equal", <);
}
void CMission::RobSuccess(uint32_t objid, uint32_t val)
{
	MISSION_CHECK(ROB_SUCCESS, +=);
}
void CMission::LearnSkill(uint32_t objid, uint32_t val)
{
	MISSION_CHECK(LEARN_SKILL, +=);
}
void CMission::CardAwakeNumber(uint32_t objid, uint32_t val)
{
	MISSION_CHECK(CARD_AWAKE_NUMBER, +=);
}
void CMission::BossAttack(uint32_t objid, uint32_t val)
{
	MISSION_CHECK(BOSS_ATTACK, +=);
}
void CMission::MatchRank(uint32_t objid, uint32_t val)
{
	MISSION_CHECK_SP(MATCH_RANK, =, <=, "rank", >);
}
void CMission::FriendNumber(uint32_t objid, uint32_t val)
{
	MISSION_CHECK_SP(FRIEND_NUMBER, =, >=, "equal", <);
}
void CMission::AvoidTime(uint32_t objid, uint32_t val)
{
	MISSION_CHECK(AVOID_TIME, +=);
}
void CMission::MeatCount(uint32_t objid, uint32_t val)
{
	MISSION_CHECK(MEAT_COUNT, +=);
}
void CMission::WineCount(uint32_t objid, uint32_t val)
{
	MISSION_CHECK(WINE_COUNT, +=);
}
void CMission::HaveCard(uint32_t objid, uint32_t val)
{
	MISSION_CHECK(HAVE_CARD, +=);
}
void CMission::HaveSwordCard(uint32_t objid, uint32_t val)
{
	MISSION_CHECK(HAVE_SWORD_CARD, +=);
}
void CMission::HaveHandCard(uint32_t objid, uint32_t val)
{
	MISSION_CHECK(HAVE_HAND_CARD, +=);
}
void CMission::HaveInternalForceCard(uint32_t objid, uint32_t val)
{
	MISSION_CHECK(HAVE_INTERNAL_FORCE_CARD, +=);
}
void CMission::HaveDarkCard(uint32_t objid, uint32_t val)
{
	MISSION_CHECK(HAVE_DARK_CARD, +=);
}
void CMission::HaveStickCard(uint32_t objid, uint32_t val)
{
	MISSION_CHECK(HAVE_STICK_CARD, +=);
}
void CMission::HaveFullBreakCard(uint32_t objid, uint32_t val)
{
	MISSION_CHECK(HAVE_FULL_BREAK_CARD, +=);
}
void CMission::Sign(uint32_t objid, uint32_t val)
{
	MISSION_CHECK(SIGN, +=);
}
void CMission::ForceValue(uint32_t objid, uint32_t val)
{
	MISSION_CHECK_SP(FORCE_VALUE, =, >=, "equal", <);
}
void CMission::WorldThreeStar(uint32_t objid, uint32_t val)
{
	MISSION_CHECK(WORLD_THREE_STAR, +=);
}
void CMission::GetCard(uint32_t objid, uint32_t val)
{
	MISSION_CHECK(GET_CARD, +=);
}
void CMission::SnsShare(uint32_t objid, uint32_t val)
{
	objid = 0;
	CConfAchieve::MapData::iterator itspecial;
	CConfAchieve::MapData dataspecial = CConfAchieve::GetData();
	for (itspecial = dataspecial.begin(); itspecial != dataspecial.end(); itspecial++)
	{
		STC_CONF_ACHIEVE &achieve = itspecial->second;
		if (achieve.dwMissionEffect == SNS_SHARE && (m_mapAchieve.find(achieve.dwIndex) != m_mapAchieve.end() && m_mapAchieve[achieve.dwIndex].status != 0) )
		{
			objid = achieve.dwCompeteObjId;
		}
	}
	objid++;

	MISSION_CHECK(SNS_SHARE, +=);
}

void CMission::BreakTower(uint32_t objid,uint32_t val){
	MISSION_CHECK_SP(BREAK_TOWER,=, >=, "equal", <);
}

void CMission::EveryShop(uint32_t objid,uint32_t val){
	MISSION_CHECK(EVERY_SHOP,+=);
}

void CMission::EveryGacha(uint32_t objid, uint32_t val){
	//每日招募
	MISSION_CHECK(EVERY_GACHA,+=);
}

void CMission::EveryCardUpgrade(uint32_t objid, uint32_t val) {
	//每日转生
	MISSION_CHECK(EVERY_CARD_UP,+=);
}

void CMission::HandlerMissionInfo(uint32_t index)
{
	if (index == 0)
	{
		uint32_t number = 0;
		WorldPacket info(CMD_SC_ACHIEVE_INFO);
		ByteBuffer pkg;
		std::map<uint32_t, SMission>::iterator it;
		for (it = m_mapAchieve.begin(); it != m_mapAchieve.end(); it++)
		{
			pkg << it->first;
			pkg << it->second.status;
			pkg << it->second.step;
//			IME_DEBUG("index %u, status %u, step %u", it->first, it->second.status, it->second.step);
			number++;
			if (number >= 200)
			{
				info.clear();
				info << number;
				info.append(pkg.contents(), pkg.size());
				m_pCRole->SendPacket(&info);
				number = 0;
				pkg.clear();
			}
		}
		if (number != 0)
		{
			info.clear();
			info << number;
			info.append(pkg.contents(), pkg.size());
			m_pCRole->SendPacket(&info);
		}
		return;
	}
	else
	{
		WorldPacket info(CMD_SC_ACHIEVE_INFO);
		std::map<uint32_t, SMission>::iterator it = m_mapAchieve.find(index);
		if (it != m_mapAchieve.end())
		{
			info << (uint32_t) 1;
			info << it->first;
			info << it->second.status;
			info << it->second.step;
//			IME_DEBUG("index %u, status %u, step %u", it->first, it->second.status, it->second.step);
			m_pCRole->SendPacket(&info);
		}
		else
		{
			IME_ERROR("can not find achieve index %u", index);
		}
		return;
	}
}

void CMission::HandlerMissionBaseInfo()
{
	WorldPacket info(CMD_SC_ACHIEVE_BASE_INFO);
	info << m_dwAchieve;
	info << m_dwActive;
	m_pCRole->SendPacket(&info);
}

void CMission::HandlerGetPrize(WorldPacket &packet)
{
	uint32_t index;
	packet >> index;
	IME_CHAR_LOG("action [userid %u get achieve index %u]", m_pCRole->GetdwAccountId(), index);
	if (m_mapAchieve.find(index) == m_mapAchieve.end())
	{
		m_pCRole->SendErrorMsg(ERRNO_MSG_ACHIEVE_NOT_COMPLETE, CMD_SC_ACHIEVE_RESULT);
		return;
	}
	if (m_mapAchieve[index].status != MISSION_PRIZE)
	{
		m_pCRole->SendErrorMsg(ERRNO_MSG_ACHIEVE_NOT_COMPLETE, CMD_SC_ACHIEVE_RESULT);
		return;
	}
	uint32_t order_id;
	if (!CDBCtrl::GetShopOrderId(m_pCRole->GetdwAccountId(), order_id))
	{
		IME_ERROR("user %u give dollar get order fail", m_pCRole->GetdwAccountId());
		m_pCRole->SendErrorMsg(ERRNO_MSG_DB, CMD_SC_ACHIEVE_RESULT);
		return;
	}

	uint32_t number = 1;
	STC_CONF_ACHIEVE *pConf = CConfAchieve::Find(index);
	if (pConf == NULL)
	{
		IME_ERROR("can not find achieve index %u", index);
		m_pCRole->SendErrorMsg(ERRNO_MSG_CSV_LOAD, CMD_SC_ACHIEVE_RESULT);
		return;
	}
	if (pConf->dwObjType == E_OBJ_CARD_MORE)
	{
		number = pConf->dwPara2;
	}	
	if (m_pCRole->GetclsCardPacket()->IsFull(number, 0))
	{
		m_pCRole->SendErrorMsg(ERRNO_MSG_CARD_PACKET_NOT_ENOUGH, CMD_SC_ACHIEVE_RESULT);
		return;
	}
	CShopEffect::SGiveProduct tmp = {0};
	m_pCRole->ChangeCoin(pConf->dwCoin, SOURCE_ACHIEVE);
	int32_t getdollar = pConf->dwDollar;
	m_pCRole->ChangeDollarWithExtra(getdollar, SOURCE_ACHIEVE);
	m_pCRole->ChangeEnergyNoLimit(pConf->dwEnergy);
	m_pCRole->ChangeStrengthNoLimit(pConf->dwStrength);
	if (m_pCRole->IsOpenArena())
	{
		uint32_t prestige = m_pCRole->GetclsArena()->GetdwPrestige();
		m_pCRole->GetclsArena()->SetdwPrestige(prestige + pConf->dwPrestige);
		IME_CHAR_LOG("resource [status=success,res=prestige,opt=add,character=%u,index=%u,number=%d,reason=ok]", m_pCRole->GetdwAccountId(), 0, pConf->dwPrestige);
	}
	m_pCRole->ChangeWorldHistory(pConf->dwWorldExp, SOURCE_ACHIEVE);
	m_pCRole->SendProUpdate();
	m_pCRole->HandlerInfoOpt();

	SMissionObj ObjType = {0};
	ObjType.coin = pConf->dwCoin;
	ObjType.dollar = getdollar;
	ObjType.energy = pConf->dwEnergy;
	ObjType.strength = pConf->dwStrength;
	ObjType.worldhistory = pConf->dwWorldExp;
	ObjType.prestige = pConf->dwPrestige;
	ObjType.obj.type = pConf->dwObjType;
	ObjType.obj.para1 = pConf->dwPara1;
	ObjType.obj.para2 = pConf->dwPara2;

	if (pConf->dwDollar > 0)
	{
		CDBCtrl::LogShopRecord(m_pCRole->GetdwAccountId(), order_id, pConf->dwDollar, 0, 0, E_DC_GIVE);	
	}
	if (pConf->dwObjType < E_OBJ_MAX || pConf->dwObjType == E_OBJ_CARD_MORE)
	{
		CShopEffect::GiveGameProductSpecial(m_pCRole, pConf->dwObjType, pConf->dwPara1, pConf->dwPara2, tmp, SOURCE_ACHIEVE);
	}
	m_dwAchieve += pConf->dwAchieveVal;		
	m_mapAchieve[index].status = MISSION_COMPLETE;
	HandlerMissionInfo(index);
	HandlerMissionBaseInfo();
	WorldPacket info(CMD_SC_ACHIEVE_RESULT);
	info << (uint16_t) 0;
	info << (uint32_t) index;
	info << ObjType.coin;
	info << ObjType.dollar;
	info << ObjType.energy;
	info << ObjType.strength;
	info << ObjType.worldhistory;
	info << ObjType.prestige;
	info << tmp.effect;
	info << tmp.para1;
	info << tmp.para2;
	m_pCRole->SendPacket(&info);
}

void CMission::HandlerGetActivePrize(WorldPacket &packet)
{
	uint32_t index;
	packet >> index;

	STC_CONF_VITALITY_PRIZE *p = CConfVitalityPrize::Find(index);
	if (p == NULL)
	{
		IME_ERROR("can not find this active prize index %u", index);
		m_pCRole->SendErrorMsg(ERRNO_MSG_CSV_LOAD, CMD_SC_ACTIVE_PRIZE_GET_RESULT);
		return;
	}

	if (m_setActive.find(index) == m_setActive.end())
	{
		if (p->dwNeedActive > m_dwActive)
		{
			IME_ERROR("your active val is not enough");
			m_pCRole->SendErrorMsg(ERRNO_MSG_ACTIVE_NOT_ENOUGH, CMD_SC_ACTIVE_PRIZE_GET_RESULT);
			return;
		}
	}
	else
	{
		IME_ERROR("you have already get this prize %u", index);
		m_pCRole->SendErrorMsg(ERRNO_MSG_HAVE_ALREADY_GET_PRIZE, CMD_SC_ACTIVE_PRIZE_GET_RESULT);
		return;
	}

	SProduct duct;
	duct.type = p->wObj1Type;
	duct.para1 = p->dwObj1Para1;
	duct.para2 = p->dwObj1Para2;
	std::vector<SProduct> vecDuct;
	vecDuct.push_back(duct);
	duct.type = p->wObj2Type;
	duct.para1 = p->dwObj2Para1;
	duct.para2 = p->dwObj2Para2;
	vecDuct.push_back(duct);

	if (m_pCRole->GetclsCardPacket()->IsFullSpecial(vecDuct))
	{
		IME_ERROR("card packet is full");
		m_pCRole->SendErrorMsg(ERRNO_MSG_CARD_PACKET_NOT_ENOUGH, CMD_SC_ACTIVE_PRIZE_GET_RESULT);
		return;
	}
	if (CShopEffect::IsObjValid(vecDuct))
	{
		IME_ERROR("para is not valid");
		m_pCRole->SendErrorMsg(ERRNO_MSG_CSV_DATA, CMD_SC_ACTIVE_PRIZE_GET_RESULT);
		return;
	}

	std::vector<CShopEffect::SGiveProduct> vect;
	for (int i = 0; i < vecDuct.size(); i++)
	{
		CShopEffect::SGiveProduct ObjEffect = {0};
		CShopEffect::GiveGameProductSpecial(m_pCRole, vecDuct[i].type, vecDuct[i].para1, vecDuct[i].para2, ObjEffect, SOURCE_ACTIVE_MISSION);
		vect.push_back(ObjEffect);
	}
	m_pCRole->SendObjEffect(vect);
	m_setActive.insert(index);
	HandlerMissionActiveInfo();

	WorldPacket info(CMD_SC_ACTIVE_PRIZE_GET_RESULT);
	info << (uint16_t) 0;
	info << (uint32_t) index;
	m_pCRole->SendPacket(&info);
}

void CMission::HandlerMissionActiveInfo()
{
	CConfVitalityPrize::MapData &map = CConfVitalityPrize::GetData();
	CConfVitalityPrize::MapData::iterator it;
	WorldPacket info(CMD_SC_MISSION_ACTIVE_PRIZE_INFO);	
	info << (uint32_t) map.size();
	for(it = map.begin(); it != map.end(); it++)
	{
		info << it->first;
		info << it->second.dwNeedActive;
		if (m_setActive.find(it->first) == m_setActive.end())
		{
			if (it->second.dwNeedActive > m_dwActive)
			{
				info << (uint8_t) ACTIVE_PRIZE_NO;
			}
			else
			{
				info << (uint8_t) ACTIVE_PRIZE_RECV;
			}
		}
		else
		{
			info << (uint8_t) ACTIVE_PRIZE_ALREADY_GET;
		}
		info << it->second.wObj1Type;
		info << it->second.dwObj1Para1;
		info << it->second.dwObj1Para2;
		info << it->second.wObj2Type;
		info << it->second.dwObj2Para1;
		info << it->second.dwObj2Para2;
	}
	m_pCRole->SendPacket(&info);
}

void CMission::GMAddActive(uint32 active){
	this->m_dwActive = this->m_dwActive + active;
}
