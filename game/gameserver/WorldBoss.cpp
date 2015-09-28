/*
 * =====================================================================================
 *
 *       Filename:  WorldBoss.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2013年03月21日 05时00分53秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  imane (), imanecr@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include "WorldBoss.h"
#include "DBCtrl.h"
#include "EnumDefines.h"
#include "ErrorMsg.h"
#include "CardPacket.h"
#include "GlobalConfig.h"
#include "Role.h"
#include "CmdDefine.h"
#include "CardPacket.h"
#include "math.h"
#include "Other.h"
#include "Mission.h"

CWorldBoss::CWorldBoss()
{
}

CWorldBoss::~CWorldBoss()
{
}

CStu* CWorldBoss::GetInnerData(uint32_t key)
{
	CFriendBoss *pf = new(std::nothrow) CFriendBoss();
	assert(pf != NULL);
	if (pf->Init(key))
	{
		IME_ERROR("init fail key %u", key);
		return NULL;
	}
	return pf;
}

bool CWorldBoss::SaveInnerData(CStu *p)
{
	CFriendBoss *pf = (CFriendBoss *)p;
	if (pf == NULL)
	{
		IME_ERROR("para is unvalid key %u", pf->GetUserId());
		return false;	
	}
	if (pf->SaveData())
	{
		IME_ERROR("save data fail %u", pf->GetUserId());
	   	return false;	
	}		
	return true;
}

CFriendBoss::CFriendBoss():m_BossIndex(0),IsInit(false),m_UserId(0),m_Type(0)
{
	m_StartTime = 0;
	m_BossHealth = 0;
	m_Level = 0;
	m_ActiveFlushTime = 0;

	m_FirstPrize = 0;
	m_BossAliveTime = 0;
	m_BossLvUpPara = 0;
}

CFriendBoss::~CFriendBoss()
{

}

int CFriendBoss::Init(uint32_t userid)
{
	m_UserId = userid;
	
//	CRole *pOwner = sWorld->FindUserRole(m_UserId);
//	CRole temp(NULL);
//	if (pOwner == NULL)
//	{
//		pOwner = &temp;
//		assert(pOwner != NULL);
//		if (!pOwner->LoadDataForDuration(m_UserId))
//		{
//			IME_ERROR("load peer role from DB fail userid %u", m_UserId);
//			return -1;
//		}
//	}
//	IME_DEBUG("username %s get string %s, userid %d", m_username.c_str(), pOwner->GetstrUserName().c_str(), m_UserId);
	std::vector<CDBCtrl::stuPlayer> vecPlayer;
	CDBCtrl::GetUserBaseDataById(userid, vecPlayer);
	if (vecPlayer.size() == 0)
	{
		IME_ERROR("load peer role from DB fail userid %u", m_UserId);
		return -1;
	}
	m_username = vecPlayer[0].strName;

	std::string str;
	if (!CDBCtrl::GetUserFriendBoss(str, userid))
		return -1;
	if (str.size() != 0)
	{
		ByteBuffer content;
		content.append(str.data(), str.size());
		if (DeSeriliazeDB(content))
		{
			IME_ERROR("DeSeriliazeDB fail");
			return -1;
		}
	}
	
	if (m_BossIndex != 0)
	{
		STC_CONF_FRIEND_BOSS *pConf = CConfFriendBoss::Find(m_BossIndex);
		if (pConf == NULL)
		{
			IME_ERROR("can not find record in csv index %u", m_BossIndex);
			return -1;
		}
		m_Type = pConf->byType;
		m_BossAliveTime = pConf->dwBossTime;
		m_BossLvUpPara = pConf->dwBossLvUpPara;
	}

	OnTimer5s();

	IME_DEBUG("bossindex %u, starttime %u, bosshealth %u, level %u", m_BossIndex, m_StartTime, m_BossHealth, m_Level);

	IsInit = true;
	return 0;
}

int CFriendBoss::SaveData()
{
	if (!IsInit)
		return -1;
	ByteBuffer content;
	
	if (SeriliazeDB(content))
	{
		IME_ERROR("SeriliazeDB fail");
		return -1;
	}

	std::string temp;
	temp.assign((const char *)content.contents(), content.size());
	CDBCtrl::SaveUserFriendBoss(temp, m_UserId);
	return 0;
}

int CFriendBoss::SeriliazeDB(ByteBuffer &by)
{
	by << m_BossIndex;
	by << m_StartTime;
	by << m_BossHealth;
	by << m_Level;	

	by << (uint32_t) m_mapActiveBoss.size();
	std::map<uint32_t, uint16_t>::iterator it;
	for (it = m_mapActiveBoss.begin(); it != m_mapActiveBoss.end(); it++)
	{
		by << (uint32_t)it->first;
		by << (uint32_t)it->second;
	}
	by << (uint32_t) m_listRecord.size();
	std::list<SRecord>::iterator itl;
	for (itl = m_listRecord.begin(); itl != m_listRecord.end(); itl++)
	{
		by << itl->userid;
		by << itl->username;
		by << itl->time;
		by << itl->skillindex;
		by << itl->skilllevel;
		by << itl->totalharm;
	}	
	by << (uint32_t) m_mapPrize.size();
	std::map<uint32_t, Prize>::iterator itp;
	for (itp = m_mapPrize.begin(); itp != m_mapPrize.end(); itp++)
	{
		by << itp->second.key;
		by << itp->second.userid;
		by << itp->second.username;
		by << itp->second.masterindex;
		by << itp->second.viewlv;
		by << itp->second.lv;
		by << itp->second.t;
		by << itp->second.number;
		by << itp->second.time;
	}
	by << (uint32_t) m_mapAttacked.size();
	std::map<uint32_t, Attacked>::iterator ita;
	for (ita = m_mapAttacked.begin(); ita != m_mapAttacked.end(); ita++)
	{
		by << ita->second.key;
		by << ita->second.userid;
		by << ita->second.username;
		by << ita->second.masterindex;
		by << ita->second.lv;
		by << ita->second.time;
	}
	by << (uint32_t) m_vecUserId.size();
	for (int i = 0; i < m_vecUserId.size(); i++)
	{
		by << m_vecUserId[i];
	}
	by << m_ActiveFlushTime;
	by << m_FirstPrize;
	return 0;
}

int CFriendBoss::DeSeriliazeDB(ByteBuffer &by)
{
	if (by.size() == 0)
		return 0;
	by >> m_BossIndex;
	by >> m_StartTime;
	by >> m_BossHealth;
	by >> m_Level;

	uint32_t number, key, val;
	SRecord tmp;
	by >> number;
	for (int i = 0; i < number; i++)
	{
		by >> key;
		by >> val;
		m_mapActiveBoss[key] = val;	
	}
	by >> number;
	for (int i = 0; i < number; i++)
	{
		by >> tmp.userid;
		by >> tmp.username;
		by >> tmp.time;
		by >> tmp.skillindex; 
		by >> tmp.skilllevel;
		by >> tmp.totalharm;
		m_listRecord.push_back(tmp);
	}
	Prize tmpp;
	by >> number;
	for (int i = 0; i < number; i++)
	{
		by >> key;
		tmpp.key = key;
		by >> tmpp.userid;
		by >> tmpp.username;
		by >> tmpp.masterindex;
		by >> tmpp.viewlv;
		by >> tmpp.lv;
		by >> tmpp.t;
		by >> tmpp.number;
		by >> tmpp.time;
		m_mapPrize[key] = tmpp;
	}
	Attacked tmpa;
	by >> number;
	for (int i = 0; i < number; i++)
	{
		by >> key;
		tmpa.key = key;
		by >> tmpa.userid;
		by >> tmpa.username;
		by >> tmpa.masterindex;
		by >> tmpa.lv;
		by >> tmpa.time;
		m_mapAttacked[key] = tmpa;
	}
	by >> number;
	for (int i = 0; i < number; i++)
	{
		by >> val;
		m_vecUserId.push_back(val);
	}
	by >> m_ActiveFlushTime;
	by >> m_FirstPrize;
	return 0;
}

void CFriendBoss::OnTimer5s()
{
	if (m_BossIndex != 0)
	{
		int cd = 0;
//		if (m_Type == FRIEND_BOSS_STORY)
//		{
//			cd = m_StartTime + GlobalConfig::StoryFBossAliveTime - time(NULL);
//		}
//		else
//		{
//			cd = m_StartTime + GlobalConfig::ActiveFBossAliveTime - time(NULL);
//		}
		cd = m_StartTime + m_BossAliveTime * 60 - time(NULL);
		if (cd <= 0)
		{
			m_BossIndex = 0;
			m_StartTime = 0;
			m_BossHealth = 0;
			m_Level = 0;
			m_listRecord.clear();
			m_Type = 0;

			m_BossAliveTime = 0;
			m_BossLvUpPara = 0;

			CRole *pCRole = sWorld->FindUserRole(m_UserId);
			if (pCRole != NULL)
			{
				WorldPacket pkg(CMD_SC_FRIEND_BOSS_INFO);
				PkgBossInfo(pkg);
				pCRole->SendPacket(&pkg);
			}
		}
	}

	if (m_ActiveFlushTime < CActiveCtrl::FriendBossRealStartTime)
	{
		m_ActiveFlushTime = CActiveCtrl::FriendBossRealStartTime;
		m_mapActiveBoss.clear();
	}
	
	uint8_t IsUpdate = false;
//	std::map<uint32_t, Prize>::iterator it,itdel;
//	for (it = m_mapPrize.begin(); it != m_mapPrize.end();)
//	{
//		if (it->second.t + GlobalConfig::FBossPrizeCD <= time(NULL))
//		{
//			itdel = it;
//			it++;
//			m_mapPrize.erase(itdel);
//			IsUpdate = true;
//		}
//		else
//		{
//			it++;
//		}
//	}
//
//	if (IsUpdate)
//	{
//		CRole *pCRole = sWorld->FindUserRole(m_UserId);
//		if (pCRole != NULL)
//		{
//			HandlerBossPrizeInfo(pCRole);
//		}
//	}
	return;
}

void CFriendBoss::HandlerBossInfo(CRole *pCRole)
{
	WorldPacket pkg(CMD_SC_FRIEND_BOSS_INFO);
//	pkg << m_UserId;
//	pkg << m_BossIndex;
//	int cd = 0;
//	if (m_Type == FRIEND_BOSS_STORY)
//	{
//		cd = m_StartTime + GlobalConfig::StoryFBossAliveTime - time(NULL);
//		cd = cd < 0 ? 0 : cd;
//	}
//	else
//	{
//		cd = m_StartTime + GlobalConfig::ActiveFBossAliveTime - time(NULL);
//		cd = cd < 0 ? 0 : cd;
//	}
//	pkg << (uint32_t) cd;
//	pkg << m_BossHealth;
//	pkg << m_Level;
//	pkg << (uint32_t) m_listRecord.size();
//	std::list<SRecord>::iterator it;
//	for (it = m_listRecord.begin(); it != m_listRecord.end(); it++)
//	{
//		pkg << it->username;
//		pkg << it->time;
//		pkg << it->skillindex;
//		pkg << it->skilllevel;
//		pkg << it->totalharm;
//	}
//	if (m_BossIndex == 0)
//		return;
	PkgBossInfo(pkg);
	pCRole->SendPacket(&pkg);
}

void CFriendBoss::HandlerBossHarmHistoryInfo(CRole *pCRole)
{
//	if (m_BossIndex == 0)
//		return;
	WorldPacket pkg(CMD_SC_FRIEND_BOSS_HARM_HISTORY_INFO);
//	pkg << (uint32_t) m_listRecord.size();
//	std::list<SRecord>::iterator it;
//	for (it = m_listRecord.begin(); it != m_listRecord.end(); it++)
//	{
//		pkg << it->username;
//		pkg << it->time;
//		pkg << it->skillindex;
//		pkg << it->skilllevel;
//		pkg << it->totalharm;
//	}
	PkgBossHarmHistoryInfo(pkg);	
	pCRole->SendPacket(&pkg);
	return;
}

void CFriendBoss::PkgBossHarmHistoryInfo(WorldPacket &pkg)
{
	pkg << (uint32_t) m_listRecord.size();
	std::list<SRecord>::iterator it;
	for (it = m_listRecord.begin(); it != m_listRecord.end(); it++)
	{
		pkg << it->userid;
		pkg << it->username;
		pkg << it->time;
		pkg << it->skillindex;
		pkg << it->skilllevel;
		pkg << it->totalharm;
//		IME_DEBUG("harm history ==== userid %u time %u totalharm %u ====", it->userid, it->time, it->totalharm);
	}
	return;
}

bool CFriendBoss::IsAlreadyPrize(uint32_t id)
{
	return m_mapPrize.find(id) != m_mapPrize.end();
}

void CFriendBoss::AddPrize(Prize &p)
{
	p.time = time(NULL);
	std::map<uint32_t, Prize>::iterator it, itdel;
	if (m_mapPrize.size() + 1 > MAX_WORLD_BOSS_PRIZE_NUMBER)
	{
		uint32_t tmp;
		itdel = m_mapPrize.begin();
		if (itdel != m_mapPrize.end())
		{
			tmp = itdel->second.time;
			for (it = m_mapPrize.begin(); it != m_mapPrize.end(); it++)
			{
				if (it->second.time  < tmp)
				{
					tmp = it->second.time;
					itdel = it;
				}
			}
			m_mapPrize.erase(itdel);
		}
		else
		{
			return;
		}
	}

	int i = 1;
	for (it = m_mapPrize.begin(); it != m_mapPrize.end(); it++)
	{
		if (it->first != i)
		{
			p.key = i;
			m_mapPrize[i] = p;
			return;
		}
		i++;
	}
	if (it == m_mapPrize.end())
	{
		p.key = i;
		m_mapPrize[i] = p;
	}
	return;
}

void CFriendBoss::AddAttacked(Attacked &p)
{
//	std::map<uint32_t, Attacked>::iterator it;
//	m_mapAttacked[p.userid] = p;
//	if (m_mapAttacked.size() > MAX_WORLD_BOSS_ATTACKED_NUMBER)
//	{
//		for (it = m_mapAttacked.begin(); it != m_mapAttacked.end(); it++)
//		{
//			if (it->first != p.userid)
//			{
//				m_mapAttacked.erase(it);
//				break;
//			}
//		}
//	}
	
	p.time = time(NULL);
	std::map<uint32_t, Attacked>::iterator it,itdel;
	if (m_mapAttacked.size() + 1 > MAX_WORLD_BOSS_ATTACKED_NUMBER)
	{
//		it = m_mapAttacked.begin();
//		if (it != m_mapAttacked.end())
//		{
//			m_mapAttacked.erase(it);
//		}
		uint32_t tmp;
		itdel = m_mapAttacked.begin();
		if (itdel != m_mapAttacked.end())
		{
			tmp = itdel->second.time;
			for (it = m_mapAttacked.begin(); it != m_mapAttacked.end(); it++)
			{
				if (it->second.time  < tmp)
				{
					tmp = it->second.time;
					itdel = it;
				}
			}
			m_mapAttacked.erase(itdel);
		}
		else
		{
			return;
		}
	}

	int i = 1;
	for (it = m_mapAttacked.begin(); it != m_mapAttacked.end(); it++)
	{
		if (it->first != i)
		{
			p.key = i;
			m_mapAttacked[i] = p;
			return;
		}
		i++;
	}
	if (it == m_mapAttacked.end())
	{
		p.key = i;
		m_mapAttacked[i] = p;
	}
	return;
}

uint16_t CFriendBoss::GetBossPrizeLv(uint32_t bossindex, CRole *pCRole)
{
	STC_CONF_FRIEND_BOSS *pConf = CConfFriendBoss::Find(bossindex);
	if (pConf == NULL)
	{
		IME_ERROR("Can not find bossindex %u in friendboss csv", bossindex);
		return 0;
	}
	
	if(pConf->byType == FRIEND_BOSS_ACTIVE)
	{
		if (m_mapActiveBoss.find(bossindex) != m_mapActiveBoss.end())
		{
			return m_mapActiveBoss[bossindex];
		}
		else
		{
			return 1;
		}
	}
	else
	{
		return pCRole->GetwLevel();
	}
}

void CFriendBoss::ChangeBossLv(uint32_t bossindex)
{
	STC_CONF_FRIEND_BOSS *pConf = CConfFriendBoss::Find(bossindex);
	if (pConf == NULL)
	{
		IME_ERROR("Can not find bossindex %u in friendboss csv", bossindex);
		return;
	}
	
	if(pConf->byType == FRIEND_BOSS_ACTIVE)
	{
		if (m_mapActiveBoss.find(bossindex) == m_mapActiveBoss.end())
		{
			m_mapActiveBoss[bossindex] = 2;
		}
		else
		{
			m_mapActiveBoss[bossindex] += 1;
		}
		if (m_mapActiveBoss[bossindex] > MAX_FRIEND_BOSS_LV)
			m_mapActiveBoss[bossindex] = MAX_FRIEND_BOSS_LV;
	}
}

void CFriendBoss::HandlerBossHarmAdd(CRole *pCRole, SRecord &re)
{
	WorldPacket info(CMD_SC_FRIEND_BOSS_HARM_ADD);
	info << re.userid;
	info << re.username;
	info << re.time;
	info << re.skillindex;
	info << re.skilllevel;
	info << re.totalharm;
	pCRole->SendPacket(&info);
	return;
}

void CFriendBoss::HandlerHappenBoss(CRole *pCRole, uint32_t BossIndex)
{
	if (m_BossIndex != 0)
	{
		IME_ERROR("already have boss index %u", m_BossIndex);
		return;	
	}
	if (pCRole->GetdwAccountId() != m_UserId)
	{
		IME_ERROR("find fail friend boss data userid %u, request userid %u", m_UserId, pCRole->GetdwAccountId());
		return;
	}
	STC_CONF_FRIEND_BOSS *pConf = CConfFriendBoss::Find(BossIndex);
	if (pConf == NULL)
	{
		IME_ERROR("can not find record in csv index %u", BossIndex);
		return;
	}
	if (pConf->byType == FRIEND_BOSS_ACTIVE)
	{
//		if (CActiveCtrl::IsActiveStart(FRIEND_BOSS_ACTIVE_ID))
//		{
//		}
//		else
//		{
//			IME_ERROR("friend boss active not start");
//			return;
//		}
	}

//	uint16_t level;
//	if (m_Type == FRIEND_BOSS_STORY)
//	{
//		level = pCRole->GetwLevel();	
//	}
//	else
//	{
//		if (m_mapActiveBoss.find(BossIndex) == m_mapActiveBoss.end())
//		{
//			level = 1;	
//		}	
//		else
//		{
//			level = m_mapActiveBoss[BossIndex] + 1;
//		}
//	}

	m_BossIndex = BossIndex;
	m_StartTime = time(NULL);
	m_BossHealth = 0;
	m_Level = GetBossPrizeLv(m_BossIndex, pCRole);

	m_BossAliveTime = pConf->dwBossTime;
	m_BossLvUpPara = pConf->dwBossLvUpPara;

	m_listRecord.clear();
	m_vecUserId.clear();

	m_Type = pConf->byType;

//	if (m_Type == FRIEND_BOSS_ACTIVE)
//	{
//		m_mapActiveBoss[BossIndex] = level;
//	}
	HandlerBossInfo(pCRole);
	HandlerBossHarmHistoryInfo(pCRole);

	WorldPacket info(CMD_SC_FRIEND_BOSS_HAPPEN);
	info << m_BossIndex;
	pCRole->SendPacket(&info);
	return;
}

void CFriendBoss::HandlerBossPrizeInfo(CRole *pCRole)
{
	WorldPacket info(CMD_SC_FRIEND_BOSS_PRIZE_INFO);
	info << (uint32_t) m_mapPrize.size();
	std::map<uint32_t, Prize>::iterator it;
	std::list<Prize *> listtmp;
	std::list<Prize *>::iterator itlist;
	for (it = m_mapPrize.begin(); it != m_mapPrize.end(); it++)
	{
		for (itlist = listtmp.begin(); itlist != listtmp.end(); itlist++)
		{
			if (it->second.time > (*itlist)->time)
			{
				listtmp.insert(itlist, &(it->second));
				break;
			}	
		}	
		if (itlist == listtmp.end())
		{
			listtmp.push_back(&(it->second));
		}
	}
	for (itlist = listtmp.begin(); itlist != listtmp.end(); itlist++)
	{
		info << (uint32_t) (*itlist)->key;
		info << (*itlist)->userid;
		info << (*itlist)->username;
		info << (*itlist)->masterindex;
		info << (*itlist)->viewlv;
		int64_t cd = (int64_t)((*itlist)->t) + GlobalConfig::StoryFBossAliveTime - time(NULL);
		cd = cd < 0 ? 0 : cd;
		info << (uint32_t) cd;
		info << (*itlist)->number;
//		IME_DEBUG("userid %u username %s masterindex %u", it->second.userid, it->second.username.c_str(), it->second.masterindex);
	}
	pCRole->SendPacket(&info);
}

void CFriendBoss::HandlerBossAttackedInfo(CRole *pCRole)
{
	WorldPacket info(CMD_SC_FRIEND_BOSS_ATTACKED_INFO);
	info << (uint32_t) m_mapAttacked.size();
	std::map<uint32_t, Attacked>::iterator it;
	std::list<Attacked *> listtmp;
	std::list<Attacked *>::iterator itlist;
	for (it = m_mapAttacked.begin(); it != m_mapAttacked.end(); it++)
	{
		for (itlist = listtmp.begin(); itlist != listtmp.end(); itlist++)
		{
			if (it->second.time > (*itlist)->time)
			{
				listtmp.insert(itlist, &(it->second));
				break;
			}	
		}	
		if (itlist == listtmp.end())
		{
			listtmp.push_back(&(it->second));
		}
	}

	for (itlist = listtmp.begin(); itlist != listtmp.end(); itlist++)
	{
		info << (*itlist)->key;
		info << (*itlist)->userid;
		info << (*itlist)->username;
		info << (*itlist)->masterindex;
		info << (*itlist)->lv;
	}
	pCRole->SendPacket(&info);
}

void CFriendBoss::HandlerBossGacha(CRole *pCRole, WorldPacket &pkg)
{
	uint8_t all;
	uint32_t one_prize_id;
	pkg >> all;
	pkg >> one_prize_id;
	if (pCRole->GetdwAccountId() != m_UserId)
	{
		IME_ERROR("para is unvalid, prize_id %u, Role userid %u", one_prize_id, pCRole->GetdwAccountId());
		pCRole->SendErrorMsg(ERRNO_MSG_DATA_ILLEGAL, CMD_SC_FRIEND_BOSS_GACHA_REQUEST_RESULT);
		return;
	}

	std::vector<uint32_t> vecPrize;
	if (!all)
	{
		if (m_mapPrize.find(one_prize_id) == m_mapPrize.end())
		{
			IME_ERROR("userid have not this prize id %u, role userid %u", one_prize_id, m_UserId);
			pCRole->SendErrorMsg(ERRNO_MSG_FRIEND_BOSS_GACHA_NO_PRIZE, CMD_SC_FRIEND_BOSS_GACHA_REQUEST_RESULT);
			return;	
		}	
		vecPrize.push_back(one_prize_id);
	}
	else
	{
		std::map<uint32_t, Prize>::iterator it;
		for (it = m_mapPrize.begin(); it != m_mapPrize.end(); it++)
		{
			vecPrize.push_back(it->first);
		}
	}

	std::vector<CShopEffect::SGiveProduct> vecgive;
	for (int i = 0; i < vecPrize.size(); i++)
	{
		uint32_t prize_id = vecPrize[i];
		if (pCRole->GetclsCardPacket()->IsFull(m_mapPrize[prize_id].number, 0))
		{
			if (i == 0)
			{
				IME_ERROR("card packet is full userid %u", pCRole->GetdwAccountId());
				pCRole->SendErrorMsg(ERRNO_MSG_CARD_PACKET_NOT_ENOUGH, CMD_SC_FRIEND_BOSS_GACHA_REQUEST_RESULT);
			}
			else
			{
				HandlerBossPrizeInfo(pCRole);

				WorldPacket info(CMD_SC_FRIEND_BOSS_GACHA_REQUEST_RESULT);
				info << (uint16_t) 0;
				info << (uint8_t) vecgive.size();
				for (int i = 0; i < vecgive.size(); i++)
				{
					info << vecgive[i].effect;
					info << vecgive[i].para1;
					info << vecgive[i].para2;
				}
				pCRole->SendPacket(&info);
			}
			return;
		}

		STC_CONF_FRIEND_BOSS *pConf = CConfFriendBoss::Find(m_mapPrize[prize_id].masterindex);
		if (pConf == NULL)
		{
			IME_ERROR("can not find master index %u in friend boss csv", m_mapPrize[prize_id].masterindex);
			pCRole->SendErrorMsg(ERRNO_MSG_CSV_LOAD, CMD_SC_FRIEND_BOSS_GACHA_REQUEST_RESULT);
			return;
		}

//		std::vector<uint8_t> vecOpt;
//		std::vector<uint32_t> vecObj;
		int32_t tpro = (int32_t)((m_mapPrize[prize_id].lv + m_mapPrize[prize_id].viewlv) / 2) * m_BossLvUpPara + pConf->dwDropPro;
		uint32_t pro = tpro < 0 ? 0 : tpro;
		uint32_t objid;
		uint32_t opt;

		int32_t tpro2 = (int32_t)((m_mapPrize[prize_id].lv + m_mapPrize[prize_id].viewlv) / 2)* m_BossLvUpPara + pConf->dwDropPro;
		uint32_t pro2 = tpro2 < 0 ? 0 : tpro2;

//		IME_LOG("worldboss gacha pro %d", pro);
		for (int i = 0; i < m_mapPrize[prize_id].number; i++)
		{
			if (m_FirstPrize == 0)
			{
				CShopEffect::SGiveProduct tmpgive;
				CShopEffect::GiveGameProduct(pCRole, E_OBJ_SHOP, INIT_FRIEND_BOSS_PIECES_DROP, 1, tmpgive, SOURCE_WORLDBOSS_PRIZE);
				vecgive.push_back(tmpgive);
				m_FirstPrize = 1;
			}

//			IME_LOG("boss drop item %u card %u itemcol %u cardcol %u", pro, pro2, pConf->dwDropItemColPro, pConf->dwDropCardColPro);
//			IME_LOG("boss drop itemid %u cardid %u itemcolid %u cardcolid %u", pConf->dwDropItemId, pConf->dwCardId, pConf->dwItemCol, pConf->dwCardCol);
			if (pConf->dwDropItemId != 0 && CUtil::RandEither(pro, 10000))
			{
				//			if (CUtil::RandEither(pConf->dwGoldCardPro, 10000))
				//			{
				//				pCRole->GetclsCardPacket()->CardAddByParaNoAutoAwake(pConf->dwDropCardId, objid, opt, true, 1);
				//				if (opt)
				//					pCRole->GetclsCardPacket()->SingleCardAddUpdate(objid);
				//				vecObj.push_back(objid);
				//				vecOpt.push_back(opt);
				//			}
				//			else
				//			{
				//				pCRole->GetclsCardPacket()->CardAddByParaNoAutoAwake(pConf->dwDropCardId, objid, opt, true);
				//				if (opt)
				//					pCRole->GetclsCardPacket()->SingleCardAddUpdate(objid);
				//				vecObj.push_back(objid);
				//				vecOpt.push_back(opt);
				//			}

				CShopEffect::SGiveProduct tmpgive;
				CShopEffect::GiveGameProduct(pCRole, E_OBJ_SHOP, pConf->dwDropItemId, 1, tmpgive, SOURCE_WORLDBOSS_PRIZE);
				vecgive.push_back(tmpgive);
			}	
			else if (pConf->dwCardId != 0 && CUtil::RandEither(pro2, 10000))
			{
				CShopEffect::SGiveProduct tmpgive;
				CShopEffect::GiveGameProduct(pCRole, E_OBJ_CARD, pConf->dwCardId, 1, tmpgive, SOURCE_WORLDBOSS_PRIZE);
				vecgive.push_back(tmpgive);
			}
			else
			{
//				if (pConf->byBossType == FRIEND_BOSS_NORMAL)
//				{
//					STC_CONF_GACHA *pConf = CConfGacha::GetByNormal(0);	
//					if (pConf != NULL)
//					{
//						pCRole->GetclsCardPacket()->CardAddByPara(pConf->dwIndex, objid, opt, true);
//						if (opt)
//							pCRole->GetclsCardPacket()->SingleCardAddUpdate(objid);
////						vecObj.push_back(objid);
////						vecOpt.push_back(opt);
//					}
//					else
//					{
//						IME_ERROR("can not get card");
//						pCRole->SendErrorMsg(ERRNO_MSG_CSV_DATA, CMD_SC_FRIEND_BOSS_GACHA_REQUEST_RESULT);
//						return;
//					}
//				}
//				else
//				{
//					STC_CONF_GACHA *pConf = CConfGacha::GetByAwakeBoss(0);	
//					if (pConf != NULL)
//					{
//						pCRole->GetclsCardPacket()->CardAddByPara(pConf->dwIndex, objid, opt, true);
//						if (opt)
//							pCRole->GetclsCardPacket()->SingleCardAddUpdate(objid);
////						vecObj.push_back(objid);
////						vecOpt.push_back(opt);
//					}
//					else
//					{
//						IME_ERROR("can not get card");
//						pCRole->SendErrorMsg(ERRNO_MSG_CSV_DATA, CMD_SC_FRIEND_BOSS_GACHA_REQUEST_RESULT);
//						return;
//					}
//				}
//				IME_ERROR("itemcol id %u", pConf->dwItemCol);
				if (pConf->dwItemCol != 0 && CUtil::RandEither(pConf->dwDropItemColPro, 10000))
				{
					CShopEffect::SGiveProduct tmpgive;
					CShopEffect::GiveGameProduct(pCRole, E_OBJ_COL_SHOP, pConf->dwItemCol, 0, tmpgive, SOURCE_WORLDBOSS_PRIZE);
					vecgive.push_back(tmpgive);
				}
				else
				{
					CShopEffect::SGiveProduct tmpgive;
					CShopEffect::GiveGameProduct(pCRole, E_OBJ_COL_CARD, pConf->dwCardCol, 0, tmpgive, SOURCE_WORLDBOSS_PRIZE);
					vecgive.push_back(tmpgive);
				}
			}
		}
		m_mapPrize.erase(prize_id);
	}
	HandlerBossPrizeInfo(pCRole);

	WorldPacket info(CMD_SC_FRIEND_BOSS_GACHA_REQUEST_RESULT);
	info << (uint16_t) 0;
	info << (uint8_t) vecgive.size();
	for (int i = 0; i < vecgive.size(); i++)
	{
		info << vecgive[i].effect;
		info << vecgive[i].para1;
		info << vecgive[i].para2;
	}
//	info << (uint8_t) vecOpt.size();
//	for (int i = 0; i < vecOpt.size(); i++)
//	{
//		info << vecOpt[i];
//		info << vecObj[i];
//	}
//	info << (uint8_t) itemnumber;
//	info << (uint32_t) pConf->dwDropCardId;
	pCRole->SendPacket(&info);
}

void CFriendBoss::PkgBossInfo(WorldPacket &pkg)
{
	pkg << m_UserId;
	pkg << m_BossIndex;
	int64_t cd = 0;
//	if (m_Type == FRIEND_BOSS_STORY)
//	{
//		cd = (int64_t)m_StartTime + GlobalConfig::StoryFBossAliveTime - time(NULL);
//		cd = cd < 0 ? 0 : cd;
//	}
//	else
//	{
//		cd = (int64_t)m_StartTime + GlobalConfig::ActiveFBossAliveTime - time(NULL);
//		cd = cd < 0 ? 0 : cd;
//	}

	cd = (int64_t)m_StartTime + m_BossAliveTime * 60 - time(NULL);
	cd = cd < 0 ? 0 : cd;

	pkg << (uint32_t) cd;
	if (m_BossIndex != 0)
	{
		CFBoss Fboss;
		if (Fboss.Init(m_BossIndex, m_Level))
		{
			IME_ERROR("fboss init fail bossindex %u lv %u", m_BossIndex, m_Level);
			return;
		}	
		IME_DEBUG("health %u, max %u, m_BossHealth %u", Fboss.GetAllHealth(), Fboss.GetAllHealthMax(), m_BossHealth);
		Fboss.OptAllHarmHealth(m_BossHealth);
		pkg << Fboss.GetAllHealth();
		pkg << Fboss.GetAllHealthMax();
		IME_DEBUG("health %u, max %u, m_BossHealth %u", Fboss.GetAllHealth(), Fboss.GetAllHealthMax(), m_BossHealth);
	}
	else
	{
		pkg << (uint32_t) 0;
		pkg << (uint32_t) 0;
	}
	pkg << m_Level;
	pkg << m_username;
	return;
}

void CFriendBoss::HandlerDurationBoss(CRole *pCRole)
{
	bool IsMyself = false;
	CFriendBoss *pFb = NULL;
	STC_CONF_FRIEND_BOSS *pConf = NULL;

//	if (pCRole->GetdwAccountId() != userid)
//	{
//		IME_ERROR("para is unvalid, userid %u, Role userid %u", userid, pCRole->GetdwAccountId());
//		pCRole->SendPacket(ERRNO_MSG_DATA_ILLEGAL, CMD_SC_FRIEND_BOSS_DURATION_REQUEST_RESULT);
//		return;
//	}
	if (pCRole->GetdwAccountId() != m_UserId)
	{	
		if (!CDBCtrl::IsFriend(m_UserId, pCRole->GetdwAccountId()))
		{
			IME_ERROR("you have been not his friend userid %u request %u", m_UserId, pCRole->GetdwAccountId());
			pCRole->SendErrorMsg(ERRNO_MSG_ALREADY_NOT_FRIEND, CMD_SC_FRIEND_BOSS_DURATION_REQUEST_RESULT);
			return;
		}
	}

	if (m_UserId == pCRole->GetdwAccountId())
	{
		IsMyself = true;
	}
	pFb = (CFriendBoss *)(sWorld->GetWorldBoss()->GetData(pCRole->GetdwAccountId()));
	if (pFb == NULL)
	{
		IME_ERROR("can not find this user %u friend boss data", pCRole->GetdwAccountId());
		pCRole->SendErrorMsg(ERRNO_MSG_FRIEND_BOSS_NOT_FIND, CMD_SC_FRIEND_BOSS_DURATION_REQUEST_RESULT);
		return;
	}

	if (m_BossIndex == 0)
	{
		IME_ERROR("this userid %u have not boss", m_UserId);
		pCRole->SendErrorMsg(ERRNO_MSG_FRIEND_BOSS_LEAVE, CMD_SC_FRIEND_BOSS_DURATION_REQUEST_RESULT);
		return;
	}

	pConf = CConfFriendBoss::Find(m_BossIndex);
	if (pConf == NULL)
	{
		IME_ERROR("can not find boss index %u info in friend boss csv", m_BossIndex);
		pCRole->SendErrorMsg(ERRNO_MSG_CSV_LOAD, CMD_SC_FRIEND_BOSS_DURATION_REQUEST_RESULT);
		return;
	}

  	if (pCRole->GetdwStrength() < GlobalConfig::FBossStrengthConsume)
  	{
  		pCRole->SendStrengthNotEnough(CMD_SC_FRIEND_BOSS_DURATION_REQUEST_RESULT);
  		return;
  	}
	
	CFBoss Fboss;
	//del health
	if (Fboss.Init(m_BossIndex, m_Level))
	{
		IME_ERROR("Friend boss init fail index %u", m_BossIndex);
		pCRole->SendErrorMsg(ERRNO_MSG_FRIEND_BOSS_INIT_FAIL, CMD_SC_FRIEND_BOSS_DURATION_REQUEST_RESULT);
		return;
	}	
	Fboss.OptAllHarmHealth(m_BossHealth);
	IME_DEBUG("opt harm %u", m_BossHealth);

	uint8_t IsFirstAttack = 0;
	if (m_BossHealth == 0)
		IsFirstAttack = 1;

	uint8_t eva;
	WorldPacket Dpkg(CMD_SC_DURATION_PKG);
	SDurationIn in;
	in.sword = pConf->dwSword;
	in.hand = pConf->dwHand;
	in.internalforce = pConf->dwInternalForce;
	in.dark = pConf->dwDark;
	in.stick = pConf->dwStick;
	in.type = DURATION_FRIEND_BOSS;
	pCRole->SetbyDuration(DURATION_FRIEND_BOSS);
	int ret = DurationJudgeWin(pCRole, &Fboss, Dpkg, eva, in); 
	if (ret == -1)
	{
		pCRole->SendErrorMsg(ERRNO_MSG_CSV_LOAD, CMD_SC_FRIEND_BOSS_DURATION_REQUEST_RESULT);
		return;
	}
	pCRole->SetbyDuration(DURATION_NORMAL);

	uint32_t len = Dpkg.size();
	const char *pStart = (const char *)Dpkg.contents();
	uint32_t number;
	while(len)
	{
		WorldPacket DpkgSplit(CMD_SC_DURATION_PKG);
		if (len > PACKET_SPLIT_NUMBER)
		{
			number = PACKET_SPLIT_NUMBER;
			len -= PACKET_SPLIT_NUMBER;
		}
		else
		{
			number = len;
			len = 0;
		}
		
		DpkgSplit.append(pStart, number);
		pStart += number;
		pCRole->SendPacket(&DpkgSplit);
	}

	if (m_vecUserId.size() < MAX_FRIEND_BOSS_PRIZE_RECORD && std::find(m_vecUserId.begin(), m_vecUserId.end(), pCRole->GetdwAccountId()) == m_vecUserId.end())
	{
		m_vecUserId.push_back(pCRole->GetdwAccountId());
	}
	
	if (ret == 0) //win
	{

		//send gacha with mail sys
		//send 2 gacha to pCRole
//		for(int i = 0; i < 2; i++)
//		{
//			uint32_t mail_id;
//			CDBCtrl::stuMailDetail temp;
//			CMail::AddEmailSysFriendBoss(pCRole->GetdwAccountId(), mail_id, temp, pFb->GetBossId(), pFb->GetBossLv()); 
//			CMail::EmailUpdate(pCRole, CMail::ADD, &temp, 0);
//		}
		pFb = (CFriendBoss *)(sWorld->GetWorldBoss()->GetData(pCRole->GetdwAccountId()));
		if (pFb == NULL)
		{
			IME_ERROR("can not find this user %u friend boss data", m_UserId);
			pCRole->SendErrorMsg(ERRNO_MSG_FRIEND_BOSS_NOT_FIND, CMD_SC_FRIEND_BOSS_DURATION_REQUEST_RESULT);
			return;
		}
		Prize tmp;
		tmp.userid = m_UserId;
		tmp.username = m_username;
		tmp.masterindex = m_BossIndex;
		tmp.viewlv = m_Level; 
		tmp.lv = pFb->GetBossPrizeLv(m_BossIndex, pCRole);
		tmp.t = time(NULL);
		tmp.number = 2;
		pFb->AddPrize(tmp);
		pFb->HandlerBossPrizeInfo(pCRole);

//		Attacked Atktmp;
//		Atktmp.userid = m_UserId;
//		Atktmp.username = m_username;
//		Atktmp.masterindex = m_BossIndex;
//		Atktmp.lv = m_Level;
//		pFb->AddAttacked(Atktmp);
		
		if (!IsMyself)
		{
			//send 2 gacha to boss owner: this m_UserId
			pFb = (CFriendBoss *)(sWorld->GetWorldBoss()->GetData(m_UserId));
			if (pFb == NULL)
			{
				IME_ERROR("can not find this user %u friend boss data", m_UserId);
				pCRole->SendErrorMsg(ERRNO_MSG_FRIEND_BOSS_NOT_FIND, CMD_SC_FRIEND_BOSS_DURATION_REQUEST_RESULT);
				return;
			}

			//				uint32_t mail_id;
			//				CDBCtrl::stuMailDetail temp;
			//				CMail::AddEmailSysFriendBoss(m_UserId, mail_id, temp, m_BossIndex, m_Level); 

			CRole *pOwner = sWorld->FindUserRole(m_UserId);
			CRole temp(NULL);
			bool IsOnline = true;
			if (pOwner == NULL)
			{
				IsOnline = false;
				pOwner = &temp;
				assert(pOwner != NULL);
				if (!pOwner->LoadDataForDuration(m_UserId))
				{
					IME_ERROR("load peer role from DB fail userid %u", m_UserId);
					pCRole->SendErrorMsg(ERRNO_MSG_FRIEND_BOSS_NOT_FIND, CMD_SC_FRIEND_BOSS_DURATION_REQUEST_RESULT);
					return;
				}
			}

			Prize tmp;
			tmp.userid = m_UserId;
			tmp.username = m_username;
			tmp.masterindex = m_BossIndex;
			tmp.viewlv = m_Level;
			tmp.lv = pFb->GetBossPrizeLv(m_BossIndex, pOwner);
			tmp.t = time(NULL);
			tmp.number = 2;
			pFb->AddPrize(tmp);
			if (IsOnline)
			{
				pFb->HandlerBossPrizeInfo(pOwner);
			}
		}	

		std::vector<uint32_t>::iterator it;
		for (it = m_vecUserId.begin(); it != m_vecUserId.end(); it++)
		{
			if (*it != pCRole->GetdwAccountId() && *it != m_UserId) //send 1 gacha to harm this boss unless finally harm and owner
			{
				pFb = (CFriendBoss *)(sWorld->GetWorldBoss()->GetData(*it));
				if (pFb == NULL)
				{
					IME_ERROR("can not find this user %u friend boss data", *it);
					pCRole->SendErrorMsg(ERRNO_MSG_FRIEND_BOSS_NOT_FIND, CMD_SC_FRIEND_BOSS_DURATION_REQUEST_RESULT);
					return;
				}

//				uint32_t mail_id;
//				CDBCtrl::stuMailDetail temp;
//				CMail::AddEmailSysFriendBoss(it->userid, mail_id, temp, m_BossIndex, m_Level); 
//				CRole *pOwner = sWorld->FindUserRole(it->userid);
//				if (pOwner != NULL)
//				{
//					CMail::EmailUpdate(pCRole, CMail::ADD, &temp, 0);
//				}

				CRole *pOwner = sWorld->FindUserRole(*it);
				CRole temp(NULL);
				bool IsOnline = true;
				if (pOwner == NULL)
				{
					IsOnline = false;
					pOwner = &temp;
					assert(pOwner != NULL);
					if (!pOwner->LoadDataForDuration(*it))
					{
						IME_ERROR("load peer role from DB fail userid %u", *it);
						pCRole->SendErrorMsg(ERRNO_MSG_FRIEND_BOSS_NOT_FIND, CMD_SC_FRIEND_BOSS_DURATION_REQUEST_RESULT);
						return;
					}
				}

				Prize tmp;
				tmp.userid = m_UserId;
				tmp.username = m_username;
				tmp.masterindex = m_BossIndex;
				tmp.viewlv = m_Level;
				tmp.lv = pFb->GetBossPrizeLv(m_BossIndex, pOwner);
				tmp.t = time(NULL);
				tmp.number = 1;
				pFb->AddPrize(tmp);
				if (IsOnline)
				{
					pFb->HandlerBossPrizeInfo(pOwner);
				}
			}
		}

		//friend add attacked boss
		std::vector<CDBCtrl::stuPlayerPlus> vecTmp;
		CDBCtrl::GetUserBaseDataInFriend(m_UserId, vecTmp);
		CDBCtrl::stuPlayerPlus OwnerP;
		OwnerP.dwUserid = m_UserId;
		vecTmp.push_back(OwnerP);
		for(int i = 0; i < vecTmp.size(); i++)
		{
			pFb = (CFriendBoss *)(sWorld->GetWorldBoss()->GetData(vecTmp[i].dwUserid));
			if (pFb == NULL)
			{
				IME_ERROR("can not find this user %u friend boss data");
				pCRole->SendErrorMsg(ERRNO_MSG_FRIEND_BOSS_NOT_FIND, CMD_SC_FRIEND_BOSS_DURATION_REQUEST_RESULT);
				return;
			}

			Attacked tmp;
			tmp.userid = m_UserId;
			tmp.username = m_username;
			tmp.masterindex = m_BossIndex;
			tmp.lv = m_Level;
			pFb->AddAttacked(tmp);

			CRole *pOwner = sWorld->FindUserRole(vecTmp[i].dwUserid);
			if (pOwner != NULL)
			{
				pFb->HandlerBossAttackedInfo(pOwner);
			}
		}	
		
		if (m_Type == FRIEND_BOSS_ACTIVE)
		{
			uint32_t devote;
			int awakepara = 1;
			if (pConf->byBossType == FRIEND_BOSS_AWAKE)
			{
				awakepara = BOSS_DEVOTE_AWAKE_PARA;
			}
			devote = m_Level * BOSS_DEVOTE_PARA * awakepara * 1.0 * pCRole->GetTotalHarm() / Fboss.GetAllHealthMax(); 
			if (devote == 0 ) devote = 1;
			IME_DEBUG("level %u totalharm %u, allhealthmax %u, devote %u", m_Level, pCRole->GetTotalHarm(), Fboss.GetAllHealthMax(), devote);
			pCRole->GetCActive()->ChangeDevote(devote);
		}	
		
		ChangeBossLv(m_BossIndex);
		//clear data and update to client
		m_BossIndex = 0;
		m_StartTime = 0;
		m_BossHealth = 0;
		m_Level = 0;
		m_Type = 0;
		m_vecUserId.clear();

		SRecord rtmp;	
		rtmp.userid = pCRole->GetdwAccountId();
		rtmp.username = pCRole->GetstrUserName();
		rtmp.time = time(NULL);
		pCRole->GetMasterSkill(rtmp.skillindex, rtmp.skilllevel);
		rtmp.totalharm = pCRole->GetTotalHarm();
		m_listRecord.push_front(rtmp);	

		CRole *pOwner = sWorld->FindUserRole(m_UserId);
		if (pOwner != NULL)
		{
			HandlerBossInfo(pOwner);
//			HandlerBossHarmHistoryInfo(pOwner);	
			HandlerBossHarmAdd(pOwner, rtmp);
		}
		m_listRecord.clear();

		if (pCRole->GetwLevel() >= GlobalConfig::FriendBossHappenLevel)
		{
			pFb = (CFriendBoss *)(sWorld->GetWorldBoss()->GetData(pCRole->GetdwAccountId()));
			if (pFb == NULL)
			{
				IME_ERROR("can not find this user %u friend boss data", m_UserId);
				pCRole->SendErrorMsg(ERRNO_MSG_FRIEND_BOSS_NOT_FIND, CMD_SC_FRIEND_BOSS_DURATION_REQUEST_RESULT);
				return;
			}
			if (pFb->GetBossId() == 0)
			{
				if (CUtil::RandEither(pConf->dwAwakePro, 10000))
				{
					if (pConf->byType == FRIEND_BOSS_STORY)
					{
						pFb->HandlerHappenBoss(pCRole, pConf->dwAwakeBossId);
					}
					else
					{
						if (CActiveCtrl::IsActiveStart(FRIEND_BOSS_ACTIVE_ID))
						{
//							if (CActiveCtrl::IsFriendBossActive(pConf->dwAwakeBossId))
//							{
								pFb->HandlerHappenBoss(pCRole, pConf->dwAwakeBossId);
//							}
						}

					}
				}
			}
		}

		pCRole->GetCMission()->BossAttack(0, 1);
	}
	else
	{
		//CDuration class add harm total info and skill get  
		SRecord tmp;	
		tmp.userid = pCRole->GetdwAccountId();
		tmp.username = pCRole->GetstrUserName();
		tmp.time = time(NULL);
		pCRole->GetMasterSkill(tmp.skillindex, tmp.skilllevel);
		tmp.totalharm = pCRole->GetTotalHarm();
		m_listRecord.push_front(tmp);	
		m_BossHealth += pCRole->GetTotalHarm();
		IME_DEBUG("already harm %u", m_BossHealth);

		CRole *pOwner = sWorld->FindUserRole(m_UserId);
		if (pOwner != NULL)
		{
			HandlerBossInfo(pOwner);
			HandlerBossHarmAdd(pOwner, tmp);
		}

		if (m_Type == FRIEND_BOSS_ACTIVE)
		{
			uint32_t devote;
			int awakepara = 1;
			if (pConf->byBossType == FRIEND_BOSS_AWAKE)
			{
				awakepara = BOSS_DEVOTE_AWAKE_PARA;
			}
			devote = m_Level * BOSS_DEVOTE_PARA * awakepara * 1.0 * pCRole->GetTotalHarm() / Fboss.GetAllHealthMax(); 
			if (devote == 0) devote = 1;
			IME_DEBUG("level %u totalharm %u, allhealthmax %u, devote %u", m_Level, pCRole->GetTotalHarm(), Fboss.GetAllHealthMax(), devote);
			pCRole->GetCActive()->ChangeDevote(devote);
		}	
	}

	if (IsFirstAttack && ret != 0)
	{
		uint8_t IsOk = 1;
		CRole *pOwner = sWorld->FindUserRole(m_UserId);
		if (pOwner == NULL)
		{
			IsOk = 0;
		}
//		IME_ERROR("userid %u, IsOk %u", m_UserId, IsOk);			

		if (IsOk)
		{
			if (pConf->byBossType == FRIEND_BOSS_AWAKE)
			{
				sWorld->GetCNotice()->VipHappenBoss(pOwner->GetstrUserName(), pOwner->GetdwAccountId(), pOwner->GetwVipLv(), pConf->strName);
			}

			std::vector<SPlayer> friendplayer = pOwner->GetFriend();
			for (int i = 0; i < friendplayer.size(); i++)
			{
				CRole *pFriend = sWorld->FindUserRole(friendplayer[i].userid);
				if (pFriend != NULL)
				{
					CDataPush push;
					push.HandlerDataPushBossAttack(pFriend, pOwner->GetstrUserName(), pOwner->GetdwAccountId(), pConf->strName); 
				}
			}
		}
	}

	uint32_t exp = pConf->dwExp;
	uint32_t coin = pConf->dwCoin;	
//	IME_DEBUG("totalharm %u, allhealthmax %u, exp %u", pCRole->GetTotalHarm(), Fboss.GetAllHealthMax(), pConf->dwExp);

	std::vector<CCard *> vec;
	if (pCRole->GetclsCardPacket()->GetEquippCards(vec))
	{
		IME_ERROR("get card property fail, userid %u", pCRole->GetdwAccountId());
		pCRole->SendErrorMsg(ERRNO_MSG_SYS, CMD_SC_FRIEND_BOSS_DURATION_REQUEST_RESULT);
		return;
	}
	pCRole->ChangeCoin(coin, SOURCE_WORLDBOSS_DURATION);
	pCRole->SendProUpdate();

	pCRole->ChangeStrength(-GlobalConfig::FBossStrengthConsume);

	int addenergy = pCRole->GetExtraEnergy();
	int addstrength = pCRole->GetExtraStrength(); 
	IME_DEBUG("get extra energy %u strength %u", addenergy, addstrength);
	pCRole->ChangeEnergyNoLimit(addenergy);
	pCRole->ChangeStrengthNoLimit(addstrength);

	pCRole->HandlerInfoOpt();
	pCRole->SendProUpdate();

	pCRole->GetCMission()->EveryWorldBoss(0, 1);	

	WorldPacket info(CMD_SC_FRIEND_BOSS_DURATION_REQUEST_RESULT);
	info << (uint16_t) 0;
	info << pCRole->GetdwAccountId();
	info << (uint8_t) ret;
	
	info << coin;
	info << (uint32_t) vec.size();
	for (int i = 0; i < vec.size(); i++)
	{
		info << vec[i]->GetdwObjId();
		info << vec[i]->GetwLevel();
		info << vec[i]->GetdwExp();
		info << vec[i]->GetdwLastExpLimit();
		info << vec[i]->GetdwExpLimit();
		IME_DEBUG("%u flag %u, exp %d, add exp %u, level %u", i, vec[i]->GetbyFlag(), vec[i]->GetdwExp(), exp, vec[i]->GetwLevel());
		if (pCRole->GetwLevel() * STRENGTH_LEVEL_LIMIT_PARA > vec[i]->GetwLevel())
		{
			vec[i]->AddExp(exp);
			vec[i]->Calculate(true);
		}
		IME_DEBUG("%u flag %u, exp %d", i, vec[i]->GetbyFlag(), vec[i]->GetdwExp());
		info << vec[i]->GetwLevel();
		info << vec[i]->GetdwExp();
		info << vec[i]->GetdwLastExpLimit();
		info << vec[i]->GetdwExpLimit();
	}
	pCRole->SendPacket(&info);
	return;
}

int CFBoss::Init(uint32_t index, uint16_t level)
{
	STC_CONF_FRIEND_BOSS *pConf = CConfFriendBoss::Find(index);
	if (pConf == NULL)
	{
		IME_ERROR("can not find data in friend boss csv index [%u]", index);
		return -1;
	}

	m_strUserName = pConf->strName;
	m_vecline.resize(MAX_CARD_CAPACITY);
	
	SBossCard temp;
	uint32_t initid = 1;

	m_masterid = 0;
	if (pConf->dwMasterCardIndex != 0)
	{
		if (level == 0)
			level = 1;

		CCard Card;
		if (Card.CreateCard(pConf->dwMasterCardIndex, level, initid++))
		{
			IME_ERROR("create card fail index %u", pConf->dwMasterCardIndex);
			return -1; 
		}

		temp.dwObjId = Card.GetdwObjId();
		temp.dwIndex = Card.GetdwIndex();
		temp.byAttribute = Card.GetbyAttribute();
		temp.dwAttack = pConf->dwMasterAttackBase + (level - 1) * pConf->dwMasterAttackGrow;
		temp.dwDefence = pConf->dwMasterDefenceBase + (level - 1) * pConf->dwMasterDefenceGrow;
		temp.dwHealth = pConf->dwMasterHealthBase + (level - 1) * pConf->dwMasterHealthGrow;
		temp.dwHealthMax = pConf->dwMasterHealthBase + (level - 1) * pConf->dwMasterHealthGrow;
		temp.dwCritRate = Card.GetdwCritRate();
		temp.dwBigSkill = pConf->dwMasterSkillIndex;
		temp.dwBigSkillLv = pConf->wMasterSkillLevel;
		temp.dwCardLv = level;
		temp.byCardType = E_ITEM_MASTER_CARD;
		if (pConf->byRare == E_CR_AWAKE || pConf->byRare == E_CR_NORMAL)
		{
			temp.byRare = pConf->byRare;
		}
		else
		{
			temp.byRare = E_CR_NORMAL;
		}
		temp.byBreakCount = pConf->byBreakCount;
//
		IME_DEBUG("init fellower card objid %u index %u attribute %u attack %u defence %u health %u critrate %u BigSkill %u BigSkillLv %u cardlv %u cardtype %u", temp.dwObjId, temp.dwIndex, temp.byAttribute, temp.dwAttack, temp.dwDefence, temp.dwHealth, temp.dwCritRate, temp.dwBigSkill, temp.dwBigSkillLv, temp.dwCardLv, temp.byCardType);

		m_vec.push_back(temp);
		m_masterid = temp.dwObjId;
	}
	SBossCard tmp2;
	m_vec.resize(MAX_CARD_CAPACITY + 1, tmp2);
	
	if (m_vec.size() > MAX_CARD_CAPACITY + 1)
	{
		IME_ERROR("boss card number %u bigger than max card capacity", m_vec.size());
		return -1;
	}
	return 0;
}

void CFBoss::OptAllHarmHealth(uint32_t harm)
{
	for (int i = 0; i < m_vec.size(); i++)
	{
//		IME_DEBUG("id %u health %u healthmax %u", m_vec[i].dwObjId, m_vec[i].dwHealth, m_vec[i].dwHealthMax);
		if (m_vec[i].dwObjId != 0)
		{
			int64_t val = (int64_t)m_vec[i].dwHealthMax - harm;
			m_vec[i].dwHealth = val < 0 ? 0 : val;
		}
//		IME_DEBUG("id %u health %u healthmax %u", m_vec[i].dwObjId, m_vec[i].dwHealth, m_vec[i].dwHealthMax);
	}
}

uint32_t CFBoss::GetAllHealthMax()
{
	uint32_t healthmax = 0;
	for (int i = 0; i < m_vec.size(); i++)
	{
		healthmax += m_vec[i].dwHealthMax;
	}
	return healthmax;
}

uint32_t CFBoss::GetAllHealth()
{
	uint32_t health = 0;
	for (int i = 0; i < m_vec.size(); i++)
	{
		health += m_vec[i].dwHealth;
	}
	return health;
}

int CFBoss::PrepareDurationData(std::vector<cardforduration_t> &vec, std::vector<uint32_t> &vecline)
{
	vecline = m_vecline; 
	vecline.resize(MAX_CARD_CAPACITY, 0);
	vec.clear();
	vec.resize(MAX_CARD_CAPACITY + 1);
	for(int i = 0; i < m_vec.size(); i++)
	{
		vec[i].CardObjId = m_vec[i].dwObjId;
		vec[i].CardIndex = m_vec[i].dwIndex;
		vec[i].BigSkill = m_vec[i].dwBigSkill;
		vec[i].BigSkillLv = m_vec[i].dwBigSkillLv;
		vec[i].AttackM = m_vec[i].dwAttack;
		vec[i].AttackE = 0;
		vec[i].DefenceM = m_vec[i].dwDefence;
		vec[i].DefenceE = 0;
		vec[i].Health = m_vec[i].dwHealth;
		vec[i].HealthMaxM = m_vec[i].dwHealthMax;
		vec[i].HealthMaxE = 0;
		vec[i].Crit = m_vec[i].dwCritRate;
		vec[i].Attribute = m_vec[i].byAttribute;
		vec[i].SkillPro = 0;
		vec[i].SkillPower = 0;
		vec[i].CardLv = m_vec[i].dwCardLv;
		vec[i].CardType = m_vec[i].byCardType;
		vec[i].Rare = m_vec[i].byRare;
		vec[i].BreakCount = m_vec[i].byBreakCount; 
		IME_DEBUG("objid %u bigskill %u bigksilllv %u", m_vec[i].dwObjId, m_vec[i].dwBigSkill, m_vec[i].dwBigSkillLv);
	}
	CDuration::SetStrUserName(m_strUserName);
	vecline.insert(vecline.begin(), m_masterid);
	return 0;
}
