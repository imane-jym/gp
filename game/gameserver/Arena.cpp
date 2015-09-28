/*
 * =====================================================================================
 *
 *       Filename:  Arena.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2012年11月16日 20时33分47秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  imane (), imanecr@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdint.h>
#include "Arena.h"
#include "DBCtrl.h"
#include "ErrorMsg.h"
#include "EnumDefines.h"
#include "CmdDefine.h"
#include "Role.h"
#include "Item.h"
#include "CardPacket.h"
#include "WorldPacket.h"
#include "Combat.h"
#include "ConfigData.h"
#include "Mission.h"

CArena::CArena ()
:m_IsData(false)
{
	return ;
}		/* -----  end of method CArena::CArena  ----- */


int CArena::CreateInitData (CRole *pCRole)
{
	uint32_t rank = CDBCtrl::GetArenaRank(pCRole->GetdwAccountId());
	if (rank == 0)
	{
		IME_ERROR("database get fail");
		return -1;
	}
	if (!CDBCtrl::InsertArena(pCRole->GetdwAccountId(), 0, rank, 0, GlobalConfig::ChallengeTimes))
	{
		IME_ERROR("create arena fail");
		return -1;
	}		
	m_pCRole = pCRole;

	m_dwUserid = pCRole->GetdwAccountId();
	m_dwPrestige = 0;
	m_dwRank = rank;
	m_dwLastTime = 0;

	uint32_t extranumber = 0;
//	STC_CONF_VIP *pVip = CConfVip::Find(pCRole->GetwVipLv());
//	if (pVip != NULL)
//		extranumber = pVip->dwArenaChallengeNumber;
//	m_dwChallengeNumber = GlobalConfig::ChallengeTimes + extranumber;
	m_dwChallengeNumber = 0;
	
	m_dwRanked = 0;
	m_dwGetPrestige = 0;
//	m_dwGetPrizeTime = 0;

	m_IsData = true;
	return 0;
}		/* -----  end of method CArena::CreateInitData  ----- */

int CArena::Init(CRole *pCRole)
{
	m_dwUserid = pCRole->GetdwAccountId();
	if (!CDBCtrl::GetUserArena(m_dwUserid, m_dwPrestige, m_dwRank, m_dwLastTime, m_dwChallengeNumber, m_dwRanked, m_dwGetPrestige))
	{
		IME_ERROR("load data fail");
		return -1;
	}
//	if (m_dwGetPrizeTime < sWorld->GetRankTime())
//	{
//		m_dwNumber = 0;
//		m_dwGetPrizeTime = time(NULL);
//	}
	if (!CUtil::IsToday(m_dwLastTime))
	{
//		uint32_t extranumber = 0;
//		STC_CONF_VIP *pVip = CConfVip::Find(pCRole->GetwVipLv());
//		if (pVip != NULL)
//			extranumber = pVip->dwArenaChallengeNumber;
//		m_dwChallengeNumber = GlobalConfig::ChallengeTimes + extranumber;
		m_dwChallengeNumber = 0;
	}
	m_pCRole = pCRole;
	m_IsData = true;
	return 0;
}

int CArena::SaveData ()
{
	if (!m_IsData)
		return -1;
	CDBCtrl::SaveUserArena(m_dwUserid, m_dwPrestige, m_dwRank, m_dwLastTime, m_dwChallengeNumber, m_dwRanked, m_dwGetPrestige);
	return 0;
}		/* -----  end of method CArena::SaveData  ----- */

void CArena::HandlerArenaTeam(WorldPacket &pkg)
{
	uint32_t userid;
	pkg >> userid;

	IME_CHAR_LOG("action [userid %u look up player %u team]", m_pCRole->GetdwAccountId(), userid);
	bool IsOnline = true;
	CRole *pPeerCRole = sWorld->FindUserRole(userid);
	CRole temp(NULL);
	if (pPeerCRole == NULL)
	{
		pPeerCRole = &temp;
		assert(pPeerCRole != NULL);
		if (!pPeerCRole->LoadDataForDuration(userid))
		{
			IME_ERROR("load peer role from DB fail userid %u", userid);
			m_pCRole->SendErrorMsg(ERRNO_MSG_DB_NO_DATA_OR_ERROR , CMD_SC_ARENA_PLAYER_TEAM_RESULT);
			return;
		}
		IsOnline = false;
	}
	
	WorldPacket info(CMD_SC_ARENA_PLAYER_TEAM_RESULT);
	info << (uint16_t) 0;
	pPeerCRole->GetclsCardPacket()->PacketForShowProtocol(info);
	m_pCRole->SendPacket(&info);
//	IME_ERROR("no used protocol");
	return;
}

void CArena::HandlerArenaChallenge(WorldPacket &pkg)
{
	uint32_t userid;
	pkg >> userid;
	
	IME_CHAR_LOG("action [userid %u chanlleng player %u in arena]", m_pCRole->GetdwAccountId(), userid);
	uint32_t extranumber = 0;
	STC_CONF_VIP *pVip = CConfVip::Find(m_pCRole->GetwVipLv());
	if (pVip != NULL)
		extranumber = pVip->dwArenaChallengeNumber;
	uint32_t allowchallengenumber = GlobalConfig::ChallengeTimes + extranumber;
	if (GetdwChallengeNumber() >= allowchallengenumber)
	{
		IME_ERROR("userid [%u] have not enough chanllenge number", m_pCRole->GetdwAccountId());
		m_pCRole->SendErrorMsg(ERRNO_MSG_ARENA_NOT_ENOUGH_NUMBER, CMD_SC_ARENA_DURATION_RESULT);
		return;
	}
	if (m_pCRole->GetdwAccountId() == userid)
	{
		IME_ERROR("request userid is same");
		m_pCRole->SendErrorMsg(ERRNO_MSG_DATA_ILLEGAL, CMD_SC_ARENA_DURATION_RESULT);
		return;
	}
//	if ((int32_t)(time(NULL) - GetdwLastTime()) < (int32_t)GlobalConfig::ChallengeCD - GetdwDelCD())
//	{
//		IME_ERROR("userid [%u] chanllenge is in CD", m_pCRole->GetdwAccountId());
//		m_pCRole->SendErrorMsg(ERRNO_MSG_ARENA_IN_CD, CMD_SC_ARENA_DURATION_RESULT);
//		return;
//	}	

	if (!m_pCRole->IsOpenArena())
	{
		m_pCRole->SendErrorMsg(ERRNO_MSG_ARENA_NOT_OPEN, CMD_SC_ARENA_DURATION_RESULT);
		return;
	}

	uint8_t IsLevelLow = 0;
	bool IsOnline = false;
	CRole temp(NULL);
	CRole *pPeerCRole = NULL;
	uint8_t IsRobort = 0;
	CRobot *pCRobot = NULL;
	if (!IS_ROBORT(userid))
	{
		pPeerCRole = sWorld->FindUserRole(userid);
		if (pPeerCRole == NULL)
		{
			pPeerCRole = &temp;
			assert(pPeerCRole != NULL);
			if (!pPeerCRole->LoadDataForDuration(userid))
			{
				IME_ERROR("load peer role from DB fail userid %u", userid);
				m_pCRole->SendErrorMsg(ERRNO_MSG_DB_NO_DATA_OR_ERROR , CMD_SC_ARENA_DURATION_RESULT);
				return;
			}
			IsOnline = false;
		}
		else
		{
			IsOnline = true;
		}

//		if (m_pCRole->GetclsArena()->GetdwRank() <= pPeerCRole->GetclsArena()->GetdwRank())
//		{
//			IME_ERROR("userid %u rank small than peeruserid %u", m_pCRole->GetdwAccountId(), pPeerCRole->GetdwAccountId());
//			m_pCRole->SendErrorMsg(ERRNO_MSG_ARENA_LEVEL_SMALL, CMD_SC_ARENA_DURATION_RESULT);
//			return;
//		}
		if (m_pCRole->GetclsArena()->GetdwRank() <= pPeerCRole->GetclsArena()->GetdwRank())
		{
			IsLevelLow = 1;
		}
		if (!pPeerCRole->IsOpenArena())
		{
			m_pCRole->SendErrorMsg(ERRNO_MSG_ARENA_NOT_OPEN, CMD_SC_ARENA_DURATION_RESULT);
			return;
		}
	}
	else
	{
		pCRobot = new (std::nothrow)CRobot();
		if (pCRobot == NULL)
		{
			IME_ERROR("new a obj fail %u");
			m_pCRole->SendErrorMsg(ERRNO_MSG_DB_NO_DATA_OR_ERROR, CMD_SC_ARENA_DURATION_RESULT);
			return;
		}
		if (pCRobot->Init(userid))
		{
			IME_ERROR("init robot fail %u", userid);
			m_pCRole->SendErrorMsg(ERRNO_MSG_CSV_DATA, CMD_SC_ARENA_DURATION_RESULT);
			return;
		}
		if (pCRobot->InitDataForArena())
		{
			IME_ERROR("init data for arena fail robotid %u", pCRobot->GetdwRobotId());
			m_pCRole->SendErrorMsg(ERRNO_MSG_CSV_DATA, CMD_SC_ARENA_DURATION_RESULT);
			return;
		}
//		if (m_pCRole->GetclsArena()->GetdwRank() <= pCRobot->GetdwRank())
//		{
//			IME_ERROR("userid %u rank small than robot %u", m_pCRole->GetdwAccountId(), pCRobot->GetdwRobotId());
//			m_pCRole->SendErrorMsg(ERRNO_MSG_ARENA_LEVEL_SMALL, CMD_SC_ARENA_DURATION_RESULT);
//			return;
//		}
		if (m_pCRole->GetclsArena()->GetdwRank() <= pCRobot->GetdwRank())
		{
			IsLevelLow = 1;
		}
		IsRobort = 1;
	}

	WorldPacket Dpkg(CMD_SC_DURATION_PKG);
	uint8_t eva;
	SDurationIn in;
	int ret;
	if (IsRobort)
	{
		ret = DurationJudgeWin(m_pCRole, pCRobot, Dpkg, eva, in); 
	}
	else
	{
		ret = DurationJudgeWin(m_pCRole, pPeerCRole, Dpkg, eva, in); 
	}
	if (ret == -1)
	{
		m_pCRole->SendErrorMsg(ERRNO_MSG_CSV_LOAD, CMD_SC_ARENA_DURATION_RESULT);
		return;
	}

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
		m_pCRole->SendPacket(&DpkgSplit);
	}

	uint32_t rankmyself,rankpeer;
	uint32_t prestige = 0;
	if (ret == 0)
	{
		if (!IsLevelLow)
		{
			if (IsRobort)
			{
				rankpeer = pCRobot->GetdwRank();	
				rankmyself = m_dwRank;
				if (!CDBCtrl::UpdateUserRank(rankpeer, rankmyself, pCRobot->GetdwRobotId(), m_pCRole->GetdwAccountId()))
				{
					IME_ERROR("data save fail");
					m_pCRole->SendErrorMsg(ERRNO_MSG_DB, CMD_SC_ARENA_DURATION_RESULT);
					return;
				}
				pCRobot->SetdwRank(m_dwRank);
				m_dwRank = rankpeer;
				//		CDBCtrl::InsertArenaLose(pPeerCRole->GetdwAccountId(), m_pCRole->GetdwAccountId());
				m_pCRole->GetclsArena()->HandlerChallengeUpdate();
				if (m_pCRole->GetclsArena()->GetdwRank() <= MAX_TOP_RANK_FLUSH)
				{
					m_pCRole->GetclsArena()->HandlerTopUpdate();
				}

				if (m_pCRole != NULL && m_pCRole->GetCMission() != NULL)
				{
					//			m_pCRole->GetCMission()->EveryMatch(0, 1);
					m_pCRole->GetCMission()->MatchRank(0, m_dwRank);
				}
			}
			else
			{
				rankpeer = pPeerCRole->GetclsArena()->GetdwRank();	
				rankmyself = m_dwRank;
				if (!CDBCtrl::UpdateUserRank(rankpeer, rankmyself, pPeerCRole->GetdwAccountId(), m_pCRole->GetdwAccountId()))
				{
					IME_ERROR("data save fail");
					m_pCRole->SendErrorMsg(ERRNO_MSG_DB, CMD_SC_ARENA_DURATION_RESULT);
					return;
				}
				pPeerCRole->GetclsArena()->SetdwRank(m_dwRank);
				m_dwRank = rankpeer;
				//		CDBCtrl::InsertArenaLose(pPeerCRole->GetdwAccountId(), m_pCRole->GetdwAccountId());
				m_pCRole->GetclsArena()->HandlerChallengeUpdate();
				if (m_pCRole->GetclsArena()->GetdwRank() <= MAX_TOP_RANK_FLUSH)
				{
					m_pCRole->GetclsArena()->HandlerTopUpdate();
				}

				if (m_pCRole != NULL && m_pCRole->GetCMission() != NULL)
				{
					//			m_pCRole->GetCMission()->EveryMatch(0, 1);
					m_pCRole->GetCMission()->MatchRank(0, m_dwRank);
				}
			}
		}
		prestige = GlobalConfig::ArenaWinPrestigeGet;
	}
	else 
	{
		prestige = GlobalConfig::ArenaLosePrestigeGet;
	}

	if (m_pCRole != NULL && m_pCRole->GetCMission() != NULL)
	{
		m_pCRole->GetCMission()->EveryMatch(0, 1);
	}
	
	m_dwLastTime = time(NULL);
	m_dwChallengeNumber += 1;
	m_dwPrestige += prestige;

//	SetdwDelCD(0);
//	m_pCRole->GetclsArena()->SaveData();
	m_pCRole->GetclsArena()->HandlerProUpdate();

	WorldPacket info(CMD_SC_ARENA_DURATION_RESULT);
	info << (uint16_t) 0;
//	IME_DEBUG("attack %d attack %d, defence %d, defence %d", AttackMinSelf, AttackMaxSelf, DefenceMin, DefenceMax);
	if (ret == 0)
	{
		info << (uint8_t) 0;
	}
	else
	{
		info << (uint8_t) 1;
	}
	info << (uint32_t) prestige;
	m_pCRole->SendPacket(&info);

	if (IsOnline && !IsRobort)
	{
		pPeerCRole->GetclsArena()->HandlerProUpdate();
		pPeerCRole->GetclsArena()->HandlerChallengeUpdate();
		if (pPeerCRole->GetclsArena()->GetdwRank() <= MAX_TOP_RANK_FLUSH)
		{
			pPeerCRole->GetclsArena()->HandlerTopUpdate();
		}
	}

//	else
//	{
//		pPeerCRole->SaveDataForDuration();
//	}
}

void CArena::HandlerTopUpdate()
{
	std::vector <CDBCtrl::stuPlayerPlus> vecPlayer;
	CDBCtrl::GetUserBaseDataArenaTop(m_pCRole->GetdwAccountId(), vecPlayer);

	WorldPacket info(CMD_SC_ARENA_TOP_PLAYER);
	info << (uint8_t) vecPlayer.size();
	for (int i = 0; i < vecPlayer.size(); i++)
	{
		info << vecPlayer[i].dwUserid;
		uint32_t userid = vecPlayer[i].dwUserid;
		if (!IS_ROBORT(userid))
		{
			info << vecPlayer[i].wLevel;
			info << vecPlayer[i].strName;
			info << vecPlayer[i].dwRank;
			CRole *pCRole = sWorld->FindUserRole(userid);
			CRole temp(NULL);
			if (pCRole == NULL)
			{
				pCRole = &temp;
				assert(pCRole != NULL);
				if (!pCRole->LoadDataForDuration(userid))
				{
					IME_ERROR("load peer role from DB fail userid %u", userid);
//					m_pCRole->SendErrorMsg(ERRNO_MSG_DB_NO_DATA_OR_ERROR, CMD_SC_ARENA_TOP_PLAYER);
					return;
				}
			}
			if (pCRole->GetclsCardPacket()->PreDataTeamCardDataForArena(info))
			{
				IME_ERROR("role prepare data fail userid %u", userid);
//				m_pCRole->SendErrorMsg(ERRNO_MSG_DB_NO_DATA_OR_ERROR, CMD_SC_ARENA_TOP_PLAYER);
				return;
			}
		}
		else
		{
			CRobot *pCRobot = new (std::nothrow)CRobot();
			if (pCRobot == NULL)
			{
				IME_ERROR("new a obj fail %u");
//				m_pCRole->SendErrorMsg(ERRNO_MSG_DB_NO_DATA_OR_ERROR, CMD_SC_ARENA_TOP_PLAYER);
				return;
			}
			if (pCRobot->Init(userid))
			{
				IME_ERROR("init robot fail %u", userid);
//				m_pCRole->SendErrorMsg(ERRNO_MSG_CSV_DATA, CMD_SC_ARENA_TOP_PLAYER);
				return;
			}

			info << pCRobot->GetRoleLv();
			info << pCRobot->GetstrUserName();
			info << vecPlayer[i].dwRank;

			if (pCRobot->PreDataTeamCardDataForArena(info))
			{
				IME_ERROR("robot prepare data fail userid %u", userid);
//				m_pCRole->SendErrorMsg(ERRNO_MSG_DB_NO_DATA_OR_ERROR, CMD_SC_ARENA_TOP_PLAYER);
				return;
			}
		}
	}
	m_pCRole->SendPacket(&info);
}

//void CArena::HandlerLoseUpdate()
//{
//	std::vector <CDBCtrl::stuPlayerPlus> vecPlayer;
//	CDBCtrl::GetUserBaseDataArenaLose(m_pCRole->GetdwAccountId(), GetdwRank(), vecPlayer);
//
//	WorldPacket info(CMD_SC_ARENA_GET_ENEMY);
//	info << (uint8_t) vecPlayer.size();
//	for (int i = 0; i < vecPlayer.size(); i++)
//	{
//		info << vecPlayer[i].dwUserid;
//		info << vecPlayer[i].wLevel;
//		info << vecPlayer[i].strName;
//		info << vecPlayer[i].dwRank;
//		uint32_t userid = vecPlayer[i].dwUserid;
//		CRole *pCRole = sWorld->FindUserRole(userid);
//		CRole temp(NULL);
//		if (pCRole == NULL)
//		{
//			pCRole = &temp;
//			assert(pCRole != NULL);
//			if (!pCRole->LoadDataForDuration(userid))
//			{
//				IME_ERROR("load peer role from DB fail userid %u", userid);
//				m_pCRole->SendErrorMsg(ERRNO_MSG_DB_NO_DATA_OR_ERROR, CMD_SC_ARENA_GET_ENEMY);
//				return;
//			}
//		}
//		if (pCRole->GetclsCardPacket()->PreDataTeamCardData(info, 3))
//		{
//			m_pCRole->SendErrorMsg(ERRNO_MSG_DB_NO_DATA_OR_ERROR, CMD_SC_ARENA_GET_ENEMY);
//			return;
//		}
//	}
//	m_pCRole->SendPacket(&info);
//}

void CArena::HandlerChallengeUpdate()
{
	int interval[ARENA_USER_GET_INTERVAL_MAX][2] = ARENA_USER_GET_INTERVAL;
	int valarr[ARENA_USER_GET_INTERVAL_MAX] = ARENA_USER_GET_INTERVAL_VAL;
	int val = valarr[ARENA_USER_GET_INTERVAL_MAX - 1];
	int valpre = valarr[ARENA_USER_GET_INTERVAL_MAX -1];
	int rankline = 1;
	int k = 0;
	for (k = 0; k < ARENA_USER_GET_INTERVAL_MAX - 1; k++)
	{
		if (m_dwRank >= interval[k][0] && m_dwRank <= interval[k][1])
		{
			val = valarr[k];
			break;
		}	
	}
	if (k == 0)
	{
		valpre = val;
	}
	else
	{
		valpre = valarr[k - 1];
		rankline = interval[k - 1][1];
	}

	int intval = val;
	int start = m_dwRank + 3 * intval;
	std::vector<uint32_t> vecRank;
	int IsOpr = 0;
	for (int i = 0; i < 11; i++)
	{
		vecRank.push_back(start);

//		IME_DEBUG("start %u intval %u rankline %u", start, intval, rankline);
		if (!IsOpr)
		{
			if (start - intval <= rankline)
			{
				intval = valpre;
				start = rankline + intval;
//				IME_DEBUG("start %u intval %u", start, intval);
				IsOpr = 1;
			}
		}

		start -= intval;

		if (start <= 0)
			break;
	}

	std::vector <CDBCtrl::stuPlayerPlus> vecPlayer;
	CDBCtrl::GetUserBaseDataArenaChallenge(m_pCRole->GetdwAccountId(), GetdwRank(), vecPlayer, vecRank);

	WorldPacket info(CMD_SC_ARENA_HIGH_RANK_PLAYER);
	info << (uint8_t) vecPlayer.size();
	for (int i = 0; i < vecPlayer.size(); i++)
	{
		info << vecPlayer[i].dwUserid;
		uint32_t userid = vecPlayer[i].dwUserid;
		if (!IS_ROBORT(userid))
		{
			info << vecPlayer[i].wLevel;
			info << vecPlayer[i].strName;
			info << vecPlayer[i].dwRank;
			CRole *pCRole = sWorld->FindUserRole(userid);
			CRole temp(NULL);
			if (pCRole == NULL)
			{
				pCRole = &temp;
				assert(pCRole != NULL);
				if (!pCRole->LoadDataForDuration(userid))
				{
					IME_ERROR("load peer role from DB fail userid %u", userid);
//					m_pCRole->SendErrorMsg(ERRNO_MSG_DB_NO_DATA_OR_ERROR, CMD_SC_ARENA_HIGH_RANK_PLAYER);
					return;
				}
			}
			if (pCRole->GetclsCardPacket()->PreDataTeamCardDataForArena(info))
			{
				IME_ERROR("role prepare data fail userid %u", userid);
//				m_pCRole->SendErrorMsg(ERRNO_MSG_DB_NO_DATA_OR_ERROR, CMD_SC_ARENA_HIGH_RANK_PLAYER);
				return;
			}
		}
		else
		{
		
			CRobot *pCRobot = new (std::nothrow)CRobot();
			if (pCRobot == NULL)
			{
				IME_ERROR("new a obj fail %u");
//				m_pCRole->SendErrorMsg(ERRNO_MSG_DB_NO_DATA_OR_ERROR, CMD_SC_ARENA_HIGH_RANK_PLAYER);
				return;
			}
			if (pCRobot->Init(userid))
			{
				IME_ERROR("init robot fail %u", userid);
//				m_pCRole->SendErrorMsg(ERRNO_MSG_CSV_DATA, CMD_SC_ARENA_HIGH_RANK_PLAYER);
				return;
			}
			info << pCRobot->GetRoleLv();
			info << pCRobot->GetstrUserName();
			info << vecPlayer[i].dwRank;
			if (pCRobot->PreDataTeamCardDataForArena(info))
			{
				IME_ERROR("robot prepare data fail userid %u", userid);
//				m_pCRole->SendErrorMsg(ERRNO_MSG_DB_NO_DATA_OR_ERROR, CMD_SC_ARENA_HIGH_RANK_PLAYER);
				return;
			}
		}
	}
	m_pCRole->SendPacket(&info);
}

uint32_t CArena::GetPrizePrestige(int rank)
{
	int PrizeInterval[ARENA_USER_PRIZE_INTERVAL_MAX][2] = ARENA_USER_PRIZE_INTERVAL;
	int Prize[ARENA_USER_PRIZE_INTERVAL_MAX] = ARENA_USER_PRIZE_INTERVAL_VAL;
	int prestige = Prize[ARENA_USER_PRIZE_INTERVAL_MAX - 1];
	for (int i = 0; i < ARENA_USER_PRIZE_INTERVAL_MAX - 1; i++)
	{
		if (rank >= PrizeInterval[i][0] && rank <= PrizeInterval[i][1])
		{
			prestige = Prize[i];
			break;
		}
	}
	return prestige;
}

void CArena::HandlerProUpdate()
{
	WorldPacket info(CMD_SC_ARENA_INFO);
	info << m_dwUserid;
	info << m_dwPrestige;
	info << m_dwRank;
	info << (uint32_t) sWorld->GetRankCDTime();

	uint32_t extranumber = 0;
	STC_CONF_VIP *pVip = CConfVip::Find(m_pCRole->GetwVipLv());
	if (pVip != NULL)
		extranumber = pVip->dwArenaChallengeNumber;
	uint32_t allowchallengenumber = GlobalConfig::ChallengeTimes + extranumber;
	int remain = (int)allowchallengenumber - (int)m_dwChallengeNumber;
	remain = remain < 0 ? 0 : remain;
	info << remain;

	info << m_dwRanked;
	info << m_dwGetPrestige;
	
	uint32_t lastprestige = GetPrizePrestige(m_dwRanked);
	uint32_t nowprestie = GetPrizePrestige(m_dwRank);
	info << lastprestige;
	info << nowprestie;
	
	m_pCRole->SendPacket(&info);
}

//void CArena::HandlerArenaPrestigeProduct()
//{
//	std::map<uint32_t, STC_CONF_ARENA_PRESTIGE> mapPrestige;
//	std::map<uint32_t, STC_CONF_ARENA_PRESTIGE>::iterator it;
//	CConfArenaPrestige::GetPrestigeCollect(mapPrestige);
//	WorldPacket info(CMD_SC_ARENA_PRESTIGE_PRODUCT_LIST);	
//	info << (uint32_t) mapPrestige.size();
//	for (it = mapPrestige.begin(); it != mapPrestige.end() ; it++)
//	{
//		info << (it->second).dwIndex;
//		IME_DEBUG("prestie prize index %u", it->second.dwIndex);
//	}
//	int32_t remain;
//	remain = GlobalConfig::PrestigePrizeTime - (time(NULL) - sWorld->GetPrestigeTime());
//	if (remain < 0)
//		remain = 0;
//	info << remain;
//	m_pCRole->SendPacket(&info);
//}

//void CArena::HandlerArenaRankProduct()
//{
//	WorldPacket info(CMD_SC_ARENA_RANK_PRODUCT_LIST);
//	uint32_t rank = sWorld->GetSortRank(m_pCRole->GetdwAccountId());
//	STC_CONF_ARENA_RANK *p = CConfArenaRank::GetRow(rank);
//
//	uint32_t rankindex;
//
//	if (p == NULL)
//	{
//		rankindex = 0;
//		info << (uint32_t) 0; 
//	}
//	else
//	{
//		rankindex = p->dwIndex;
//		info << p->dwIndex;
//	}
//	info << (uint8_t)m_dwNumber;
//	int32_t remain;
//
//	uint32_t datestrformat = CUtil::GetDateTime(time(NULL));
//	uint32_t firsttimeofweek = CUtil::GetFirstWeekday(datestrformat, 0) + 24 * 3600;
//	uint8_t day = GlobalConfig::RankPrizeTime / 100;
//	uint8_t hour = GlobalConfig::RankPrizeTime % 100;
//	firsttimeofweek += day * 24 * 3600 + hour * 3600;
//
//	remain = firsttimeofweek - time(NULL);
//	
//	// for test
////	remain = sWorld->GetRankTime() + 20 - time(NULL);
//
//	if (remain < 0)
//		remain += 7 * 24 * 3600;
//
//	IME_DEBUG("index %u rank %u remain time %u", rankindex, rank, remain);
//	info << remain;
//	m_pCRole->SendPacket(&info);
//}

//void CArena::HandlerArenaRankFlush()
//{
//	m_dwNumber = 0;
//	m_dwGetPrizeTime = time(NULL);
//}

void CArena::HandlerArenaRequestPrestigeProduct(WorldPacket &pkg)
{
	uint32_t itemindex;
	pkg >> itemindex;

	IME_CHAR_LOG("action [userid %u get prestige]", m_pCRole->GetdwAccountId());
	if (m_dwGetPrestige <= 0)
	{
		IME_ERROR("no prestige for getting");
		m_pCRole->SendErrorMsg(ERRNO_MSG_NO_PRESIGE, CMD_SC_ARENA_REQUEST_PRESTIGE_PRODUCT_RESULT);
		return;
	}

	IME_CHAR_LOG("resource [status=success,res=prestige,opt=add,character=%u,index=%u,number=%d,reason=ok]", m_pCRole->GetdwAccountId(), 0, m_dwGetPrestige);

	m_dwPrestige += m_dwGetPrestige;
	uint32_t get = m_dwGetPrestige;
	m_dwGetPrestige = 0;
	HandlerProUpdate();

//	uint32_t itemindex;
//	pkg >> itemindex;
//	STC_CONF_ARENA_PRESTIGE *p = CConfArenaPrestige::Find(itemindex);
//	if (p == NULL)
//	{
//		IME_ERROR("can not find this item [%u] in arena csv", itemindex);
//		m_pCRole->SendErrorMsg(ERRNO_MSG_CSV_LOAD ,CMD_SC_ARENA_REQUEST_PRESTIGE_PRODUCT_RESULT);
//		return;
//	}
//
//	if (m_dwPrestige < p->dwPrestige)
//	{
//		m_pCRole->SendErrorMsg(ERRNO_MSG_ARENA_NOT_ENOUGH_PRESTIGE, CMD_SC_ARENA_REQUEST_PRESTIGE_PRODUCT_RESULT);
//		return;
//	}
//
//	if (!CConfArenaPrestige::CollectionFind(p->dwIndex))
//	{
//		m_pCRole->SendErrorMsg(ERRNO_MSG_ARENA_ITEM_DOWN, CMD_SC_ARENA_REQUEST_PRESTIGE_PRODUCT_RESULT);
//		return;
//	}
//	
//	if (p->byType == E_OBJ_CARD)
//	{
//		if (m_pCRole->GetclsCardPacket()->IsFull())
//		{
//			IME_ERROR("user %u card packet already full", m_pCRole->GetdwAccountId());
//			m_pCRole->SendErrorMsg(ERRNO_MSG_CARD_PACKET_NOT_ENOUGH, CMD_SC_ARENA_REQUEST_PRESTIGE_PRODUCT_RESULT);
//			return;
//		}
//	}
//
//	if (p->byType == E_OBJ_CARD)
//	{
//		uint32_t objid, opt;
//		m_pCRole->GetclsCardPacket()->CardAddByPara(p->dwItemIndex, objid, opt); 
//		if (opt)
//		{
//			m_pCRole->GetclsCardPacket()->SingleCardAddUpdate(objid);
//		}
//	}
//	else if (p->byType == E_OBJ_ITEM)
//	{
//		m_pCRole->GetclsItemPacket()->AddItem(p->dwItemIndex);
//		m_pCRole->GetclsItemPacket()->ItemUpdate(p->dwItemIndex, CItem::ADD);
//	}
//	else if (p->byType == E_OBJ_SHOP)
//	{
//		m_pCRole->GetCShop()->AddItem(p->dwItemIndex);
//		m_pCRole->GetCShop()->HandlerShopInfo();
//	}
//	else
//	{
//		m_pCRole->SendErrorMsg(ERRNO_MSG_DATA_ILLEGAL, CMD_SC_ARENA_REQUEST_PRESTIGE_PRODUCT_RESULT);
//	}
//
//	m_dwPrestige -= p->dwPrestige;
//	HandlerProUpdate();

	WorldPacket info(CMD_SC_ARENA_REQUEST_PRESTIGE_PRODUCT_RESULT);
	info << (uint16_t) 0;
	info << get;
	m_pCRole->SendPacket(&info);
	return;
}

//void CArena::HandlerArenaRequestRankProduct(WorldPacket &pkg)
//{
//	uint32_t itemindex;
//	pkg >> itemindex;
//
//	uint32_t rank = sWorld->GetSortRank(m_pCRole->GetdwAccountId());
//
//	STC_CONF_ARENA_RANK *p = CConfArenaRank::Find(itemindex);
//	if (p == NULL)
//	{
//		IME_ERROR("can not find this item [%u] in arena csv", itemindex);
//		m_pCRole->SendErrorMsg(ERRNO_MSG_CSV_LOAD ,CMD_SC_ARENA_REQUEST_RANK_PRODUCT_RESULT);
//		return;
//	}
//
//	if (!(p->dwLow <= rank && rank <= p->dwHigh))
//	{
//		IME_ERROR("user rank prize data unvalid level %u, then prizes low %u, high %u", rank, p->dwLow, p->dwHigh);
//		m_pCRole->SendErrorMsg(ERRNO_MSG_ARENA_RANK_PRIZE ,CMD_SC_ARENA_REQUEST_RANK_PRODUCT_RESULT);
//		return;
//	}
//
//	IME_DEBUG("number %u limit %u", m_dwNumber, GlobalConfig::RankingPrizeTimes);
//	if (m_dwNumber >= GlobalConfig::RankingPrizeTimes)
//	{
//		m_pCRole->SendErrorMsg(ERRNO_MSG_ARENA_OVERLOAD_NUMBER ,CMD_SC_ARENA_REQUEST_RANK_PRODUCT_RESULT);
//		return;
//	}
//
//	if (p->byType == E_OBJ_CARD)
//	{
//		if (m_pCRole->GetclsCardPacket()->IsFull())
//		{
//			IME_ERROR("user %u card packet already full", m_pCRole->GetdwAccountId());
//			m_pCRole->SendErrorMsg(ERRNO_MSG_CARD_PACKET_NOT_ENOUGH, CMD_SC_ARENA_REQUEST_RANK_PRODUCT_RESULT);
//			return;
//		}
//	}
//
//	if (p->byType == E_OBJ_CARD)
//	{
//		uint32_t objid, opt;
//		m_pCRole->GetclsCardPacket()->CardAddByPara(p->dwItemIndex, objid, opt); 
//		if (opt)
//		{
//			m_pCRole->GetclsCardPacket()->SingleCardAddUpdate(objid);
//		}
//	}
//	else if (p->byType == E_OBJ_ITEM)
//	{
//		m_pCRole->GetclsItemPacket()->AddItem(p->dwItemIndex);
//		m_pCRole->GetclsItemPacket()->ItemUpdate(p->dwItemIndex, CItem::ADD);
//	}
//	else if (p->byType == E_OBJ_SHOP)
//	{
//		m_pCRole->GetCShop()->AddItem(p->dwItemIndex);
//		m_pCRole->GetCShop()->HandlerShopInfo();
//	}
//	else if (p->byType == E_OBJ_DOLLAR)
//	{
//		uint32_t order_id;
//		if (!CDBCtrl::GetShopOrderId(m_pCRole->GetdwAccountId(), order_id))
//		{
//			IME_ERROR("user %u give dollar %u fail in EggEffect", m_pCRole->GetdwAccountId(), p->dwNumber);
//			return;
//		}
//
//		m_pCRole->ChangeDollar(p->dwNumber);
//		m_pCRole->SendProUpdate();
//		CDBCtrl::LogShopRecord(m_pCRole->GetdwAccountId(), order_id, p->dwNumber, 0, 0, E_DC_GIVE);	
//	}
//	else if (p->byType == E_OBJ_COIN)
//	{
//		m_pCRole->ChangeCoin(p->dwNumber);
//		m_pCRole->SendProUpdate();
//	}
//	else
//	{
//		m_pCRole->SendErrorMsg(ERRNO_MSG_DATA_ILLEGAL, CMD_SC_ARENA_REQUEST_RANK_PRODUCT_RESULT);
//	}
//
//	if (p->byType2 == E_OBJ_ITEM)
//	{
//		m_pCRole->GetclsItemPacket()->AddItem(p->dwItemIndex2);
//		m_pCRole->GetclsItemPacket()->ItemUpdate(p->dwItemIndex2, CItem::ADD);
//	}
//	else if (p->byType2 == E_OBJ_SHOP)
//	{
//		m_pCRole->GetCShop()->AddItem(p->dwItemIndex2);
//		m_pCRole->GetCShop()->HandlerShopInfo();
//	}
//	else if (p->byType2 == E_OBJ_DOLLAR)
//	{
//		uint32_t order_id;
//		if (!CDBCtrl::GetShopOrderId(m_pCRole->GetdwAccountId(), order_id))
//		{
//			IME_ERROR("user %u give dollar %u fail in EggEffect", m_pCRole->GetdwAccountId(), p->dwNumber2);
//			return;
//		}
//
//		m_pCRole->ChangeDollar(p->dwNumber2);
//		m_pCRole->SendProUpdate();
//		CDBCtrl::LogShopRecord(m_pCRole->GetdwAccountId(), order_id, p->dwNumber2, 0, 0, E_DC_GIVE);	
//	}
//	else if (p->byType2 == E_OBJ_COIN)
//	{
//		m_pCRole->ChangeCoin(p->dwNumber2);
//		m_pCRole->SendProUpdate();
//	}
//	else
//	{
//		m_pCRole->SendErrorMsg(ERRNO_MSG_DATA_ILLEGAL, CMD_SC_ARENA_REQUEST_RANK_PRODUCT_RESULT);
//	}
//
//	m_dwNumber++;
////	m_dwGetPrizeTime = time(NULL);
////  HandlerArenaRankProduct();
//
//	WorldPacket info(CMD_SC_ARENA_REQUEST_RANK_PRODUCT_RESULT);
//	info << (uint16_t) 0;
//	m_pCRole->SendPacket(&info);
//	return;
//}

void CArena::OnTimer5s()
{
	if (!CUtil::IsToday(m_dwLastTime))
	{
//		uint32_t extranumber = 0;
//		STC_CONF_VIP *pVip = CConfVip::Find(m_pCRole->GetwVipLv());
//		if (pVip != NULL)
//			extranumber = pVip->dwArenaChallengeNumber;
//		m_dwChallengeNumber = GlobalConfig::ChallengeTimes + extranumber;
		m_dwChallengeNumber = 0;
	}
}
