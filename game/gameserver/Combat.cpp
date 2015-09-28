/*
 * =====================================================================================
 *
 *       Filename:  Combat.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2012年09月14日 03时10分02秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  imane (), imanecr@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include "Role.h"
#include "stdint.h"
#include "CmdDefine.h"
#include "ErrorMsg.h"
#include "EnumDefines.h"
#include <vector>
#include "Combat.h" 
#include "Log.h"
#include "CardPacket.h"
#include "ConfigData.h"
#include "skill.h"
#include "ErrorMsg.h"
#include "Tech.h"
#include "Mail.h"
#include "Combine.h"
#include "Mission.h"

void hexdump(const char *p, int len);

void CCombat::HandlerAutoPlayer(CRole *pCRole, WorldPacket &pkg)
{
	uint8_t type = 0;	
	pkg >> type;
	IME_CHAR_LOG("action [userid %u auto search player type %u", pCRole->GetdwAccountId(), type);
	
	std::vector <CDBCtrl::stuPlayer> vecPlayer;
	CDBCtrl::GetUserBaseDataAuto10(pCRole->GetdwAccountId(), pCRole->GetwLevel(), vecPlayer);

	WorldPacket info(CMD_SC_AUTO_PLAYER_RESULT);
	info << (uint16_t)0;
	info << (uint8_t) vecPlayer.size();
	for (int i = 0; i < vecPlayer.size(); i++)
	{
		info << vecPlayer[i].dwUserid;
		info << vecPlayer[i].wLevel;
		info << vecPlayer[i].strName;
		uint32_t userid = vecPlayer[i].dwUserid;
		CRole *pCRole = sWorld->FindUserRole(userid);
		CRole temp(NULL);
		if (pCRole == NULL)
		{
			pCRole = &temp;
			assert(pCRole != NULL);
			if (!pCRole->LoadDataForDuration(userid))
			{
				IME_ERROR("load peer role from DB fail userid %u", userid);
				pCRole->SendErrorMsg(ERRNO_MSG_DB_NO_DATA_OR_ERROR, CMD_SC_AUTO_PLAYER_RESULT);
				return;
			}
		}
		if (pCRole->GetclsCardPacket()->PreDataTeamCardData(info, 3))
		{
			pCRole->SendErrorMsg(ERRNO_MSG_DB_NO_DATA_OR_ERROR, CMD_SC_AUTO_PLAYER_RESULT);
			return;
		}
		IME_DEBUG("userid %u, wLevel %u", vecPlayer[i].dwUserid, vecPlayer[i].wLevel);
	}
	pCRole->SendPacket(&info);
}

// 0: equal 1: bigger 2 : small
//int comparebetweenskill(uint8_t skill1, uint8_t skill2, const std::map<uint8_t, uint8_t> &SkillOrder)
//{
//	std::map<uint8_t, uint8_t>::const_iterator it;
//	uint8_t seq1,seq2;
//	if ((it = SkillOrder.find(skill1)) == SkillOrder.end())
//	{
//		IME_ERROR("skill1[%u] is not in skill order", skill1);
//		return 1;
//	}
//	seq1 = it->second;
//
//	if ((it = SkillOrder.find(skill2)) == SkillOrder.end())
//	{
//		IME_ERROR("skill2[%u] is not in skill order", skill2);
//		return 2;
//	}
//
//	seq2 = it->second;
//	if (seq1 == seq2)
//		return 0;
//	return ((seq1 > seq2) ? 1 : 2);
//}

CDuration::~CDuration()
{
}

CDuration::cardforduration_t* CDuration::GetCardBaseData(uint32_t objid)
{
	for (int i = 0; i < m_vecForDuration.size(); i++)
	{
		if (objid == m_vecForDuration[i].CardObjId)
		{
			return &m_vecForDuration[i];
		}
	}
	return NULL;
}

CDuration::DurationData_t* CDuration::GetCardData(uint32_t objid)
{
	for (int i = 0; i < m_vecDurationData.size(); i++)
	{
		if (objid == m_vecDurationData[i].CardObjId)
		{
			return &m_vecDurationData[i];
		}
	}
	if (objid == m_DurationMaster.CardObjId)
		return &m_DurationMaster;
	return NULL;
}

int CDuration::CheckData()
{
	for (int i = 0; i < m_vecLineUp.size(); i++)
	{
		if (m_vecLineUp[i] != 0)
		{
			if (GetCardData(m_vecLineUp[i]) == NULL)
			{
				return -1;
			}
		}
	}
	if (m_vecLineUp.size() != MAX_CARD_CAPACITY || m_vecForDuration.size() != MAX_CARD_CAPACITY + 1)
	{
		return -1;
	}
	return 0;
}

int CDuration::DurationDataCreate()
{
	m_vecDurationData.clear();	
	memset(&m_TotalData, 0, sizeof(m_TotalData));
	memset(&m_DurationMaster, 0, sizeof(m_DurationMaster));
	uint32_t masterobjid = m_vecLineUp[0];
	m_vecLineUp.erase(m_vecLineUp.begin());
	for(int i = 0; i < m_vecForDuration.size(); i++)
	{
		DurationData_t temp = {0};
		temp.CardObjId = m_vecForDuration[i].CardObjId;
		temp.CardIndex = m_vecForDuration[i].CardIndex;
//		temp.TalentSkill = m_vecForDuration[i].TalentSkill;
		temp.BigSkill = m_vecForDuration[i].BigSkill;
		temp.BigSkillLv = m_vecForDuration[i].BigSkillLv;
		temp.AttackM = m_vecForDuration[i].AttackM;
		temp.AttackE = m_vecForDuration[i].AttackE;
		temp.Crit = m_vecForDuration[i].Crit;
//		temp.Dodge = m_vecForDuration[i].Dodge;
//		temp.Action = m_vecForDuration[i].Action;
		temp.Attribute = m_vecForDuration[i].Attribute;
//		temp.AddBuffer = m_vecForDuration[i].AddBuffer;
//		temp.ReduceBuffer = m_vecForDuration[i].ReduceBuffer;
		temp.SkillPro = m_vecForDuration[i].SkillPro;
		temp.SkillPower = m_vecForDuration[i].SkillPower;
//		temp.Team = m_Team; 
//		temp.BigSkillNumber = 0;
		STC_CONF_CARD_SKILL *pConf;
		if (temp.BigSkill != 0)
		{
			pConf = CConfCardSkill::Find(temp.BigSkill * 100 + temp.BigSkillLv);
			if (pConf == NULL)
			{
				IME_ERROR("can not find big skill %u in skill csv", temp.BigSkill * 100 + temp.BigSkillLv);
				return -1;
			}
//			temp.BigSkillNumber = pConf->dwNumber;
		}
		
//		temp.TalentSkillNumber = 0;
//		if (temp.TalentSkill != 0)
//		{
//			pConf = CConfCardSkill::Find(temp.TalentSkill);
//			if (pConf == NULL)
//			{
//				IME_ERROR("can not find TalentSkill skill %u in skill csv", temp.TalentSkill);
//				return -1;
//			}
//			temp.TalentSkillNumber = pConf->dwNumber;
//		}
//		temp.CardLv = m_vecForDuration[i].CardLv; 
//		IME_ERROR("objid %u, cardlv %u", temp.CardObjId, temp.CardLv);
		
		m_TotalData.Defence += m_vecForDuration[i].DefenceM + m_vecForDuration[i].DefenceE;
		m_TotalData.Health += m_vecForDuration[i].Health;
		m_TotalData.HealthMax += m_vecForDuration[i].HealthMaxM + m_vecForDuration[i].HealthMaxE;
		m_TotalData.ReduceHarm += m_vecForDuration[i].ReduceHarm;	

		IME_DEBUG("health %u healthmax %u CardObjId %u", m_vecForDuration[i].Health, m_TotalData.HealthMax, m_vecForDuration[i].CardObjId);
//		if (m_vecForDuration[i].CardType == E_ITEM_MASTER_CARD)
		if (m_vecForDuration[i].CardObjId == masterobjid)
		{
			m_DurationMaster = temp;	
		}	
		else
		{
			m_vecDurationData.push_back(temp);
		}
	}
	return 0;
}

//CDuration::BufferData_t *CDuration::GetBuffer(uint32_t objid, uint32_t index)
//{
//	DurationData_t *ptemp = GetCardData(objid); 
//	STC_CONF_CARD_SKILL *pConf;
//	if (ptemp == NULL)
//		return NULL;
//	for (int i = 0; i < MAX_STATUS; i++)
//	{
//		if (ptemp->Buffer[i] != NULL)
//		{
//			pConf = CConfCardSkill::Find(ptemp->Buffer[i]->index);
//			if (pConf == NULL)
//				return NULL;
//			if(pConf->byStatusType == index)
//				return ptemp->Buffer[i];	
//		}
//		if (ptemp->Debuffer[i] != NULL)
//		{
//			pConf = CConfCardSkill::Find(ptemp->Debuffer[i]->index);
//			if (pConf == NULL)
//				return NULL;
//			if(pConf->byStatusType == index)
//				return ptemp->Debuffer[i];
//		}
//	}
//	return NULL;
//}

void CDuration::HexDump()
{
	cardforduration_t *pCardBaseTemp;
	for (int i = 0; i < m_vecLineUp.size(); i++)
	{
		pCardBaseTemp = GetCardBaseData(m_vecLineUp[i]);
		if (pCardBaseTemp != NULL && m_vecLineUp[i] != 0)
		{
			IME_DEBUG("%u objid %u CardIndex %u, BigSkill %u, BigSkillLv %u, AttackM %u, AttackE %u, SkillPro %u, SkillPower %u, lv %u, defence %u, Crit %u ", i, pCardBaseTemp->CardObjId, pCardBaseTemp->CardIndex, pCardBaseTemp->BigSkill, pCardBaseTemp->BigSkillLv, pCardBaseTemp->AttackM, pCardBaseTemp->AttackE, pCardBaseTemp->SkillPro, pCardBaseTemp->SkillPower, pCardBaseTemp->CardLv, pCardBaseTemp->DefenceM, pCardBaseTemp->Crit);
		}

	}
		pCardBaseTemp = GetCardBaseData(m_DurationMaster.CardObjId);
		if (pCardBaseTemp != NULL)
		{
			IME_DEBUG("master objid %u CardIndex %u, BigSkill %u, BigSkillLv %u, AttackM %u, AttackE %u, SkillPro %u, SkillPower %u, CardLv %u, defence %u, Crit %u ", pCardBaseTemp->CardObjId, pCardBaseTemp->CardIndex, pCardBaseTemp->BigSkill, pCardBaseTemp->BigSkillLv, pCardBaseTemp->AttackM, pCardBaseTemp->AttackE, pCardBaseTemp->SkillPro, pCardBaseTemp->SkillPower, pCardBaseTemp->CardLv, pCardBaseTemp->DefenceM, pCardBaseTemp->Crit);
		}

	IME_DEBUG("total defence %u, health %u, healthmax %u", m_TotalData.Defence, m_TotalData.Health, m_TotalData.HealthMax);
}

int DurationJudgeWin(CDuration *pCRole, CDuration *pPeerCRole, ByteBuffer &by, uint8_t &eva, SDurationIn in)
{

	std::vector<std::vector<ByteBuffer> > pkgBody;

	if (pCRole->PrepareData(0) || pPeerCRole->PrepareData(1))
	{
		IME_ERROR("prepare data fail");
		return -1;
	}

	CCardSkill::Init();
	CCardSkill::CardSkillFunc pSkillFunc;

	if (pCRole->DurationDataCreate())
	{
		IME_ERROR("create duration data fail");
		return -1;
	}
	if (pPeerCRole->DurationDataCreate())
	{
		IME_ERROR("create duration data fail");
		return -1;
	}

	if (pCRole->CheckData() || pPeerCRole->CheckData())
	{
		IME_ERROR("one check data fail in two sides of duration");
		return -1;
	}
	
	std::vector<uint32_t> vecOrder;
//	std::vector<uint8_t> vecTeam;
	STC_CONF_CARD_SKILL *pConf = NULL;
	CDuration::cardforduration_t *pCardBaseTemp;
	CDuration::DurationData_t *pCardTemp;
	CDuration *pTempRole;
	CDuration *pPeerTempRole;
	int Success = 0;
	by << pCRole->GetstrUserName();
	by << pPeerCRole->GetstrUserName();

	for (int i = 0; i < pCRole->m_vecLineUp.size(); i++)
	{
		pCardBaseTemp = pCRole->GetCardBaseData(pCRole->m_vecLineUp[i]);
//		pCardTemp = pCRole->GetData(pCRole->m_vecLineUp[i]);
		if (pCardBaseTemp != NULL && pCRole->m_vecLineUp[i] != 0)
		{
			by << pCardBaseTemp->CardObjId;
			by << pCardBaseTemp->Health;
			by << (uint32_t)(pCardBaseTemp->HealthMaxM + pCardBaseTemp->HealthMaxE);
			by << pCardBaseTemp->CardLv;
			by << (uint32_t)(pCardBaseTemp->AttackM + pCardBaseTemp->AttackE);
			by << pCardBaseTemp->Rare;
			STC_CONF_CARD_BASE *pCardConf = CConfCardBase::Find(pCardBaseTemp->CardIndex);
			if (pCardConf == NULL)
			{
				IME_ERROR("can not find card in card csv index %u", pCardBaseTemp->CardIndex);
				return -1;
			}
			by << (uint16_t) pCardBaseTemp->BreakCount;
			by << (uint16_t) (pCardBaseTemp->BreakCount >= pCardConf->dwLimitBreakNumber ? 1 : 0);
		}
		else
		{
			by << (uint32_t) 0;
			by << (uint32_t) 0;
			by << (uint32_t) 0;
			by << (uint16_t) 0;
			by << (uint32_t) 0;
			by << (uint8_t) 0;
			by << (uint16_t) 0;
			by << (uint16_t) 0;
		}
	}
	pCardBaseTemp = pCRole->GetCardBaseData(pCRole->m_DurationMaster.CardObjId);
	if (pCardBaseTemp != NULL)
	{
		by << pCardBaseTemp->CardObjId;
		by << pCardBaseTemp->Health;
		by << (uint32_t)(pCardBaseTemp->HealthMaxM + pCardBaseTemp->HealthMaxE);
		by << (uint32_t)pCardBaseTemp->CardIndex;
		by << pCardBaseTemp->CardLv;
		by << (uint32_t)(pCardBaseTemp->AttackM + pCardBaseTemp->AttackE);
		by << pCardBaseTemp->Rare;
		STC_CONF_CARD_BASE *pCardConf = CConfCardBase::Find(pCardBaseTemp->CardIndex);
		if (pCardConf == NULL)
		{
			IME_ERROR("can not find card in card csv index %u", pCardBaseTemp->CardIndex);
			return -1;
		}
		by << (uint16_t) pCardBaseTemp->BreakCount;
		by << (uint16_t) (pCardBaseTemp->BreakCount >= pCardConf->dwLimitBreakNumber ? 1 : 0);
	}
	else
	{
		by << (uint32_t) 0;
		by << (uint32_t) 0;
		by << (uint32_t) 0;
		by << (uint32_t) 0;
		by << (uint16_t) 0;
		by << (uint32_t) 0;
		by << (uint8_t) 0;
		by << (uint16_t) 0;
		by << (uint16_t) 0;
	}
	IME_DEBUG("Role");
	pCRole->HexDump();
	
	for (int i = 0; i < pPeerCRole->m_vecLineUp.size(); i++)
	{
		pCardBaseTemp = pPeerCRole->GetCardBaseData(pPeerCRole->m_vecLineUp[i]);
		if (pCardBaseTemp != NULL && pPeerCRole->m_vecLineUp[i] != 0)
		{
			by << pCardBaseTemp->CardObjId;
			by << pCardBaseTemp->Health;
			by << (uint32_t)(pCardBaseTemp->HealthMaxM + pCardBaseTemp->HealthMaxE);
			by << pCardBaseTemp->CardIndex;
			by << pCardBaseTemp->CardLv;
			by << (uint32_t)(pCardBaseTemp->AttackM + pCardBaseTemp->AttackE);			
			by << pCardBaseTemp->Rare;			
			STC_CONF_CARD_BASE *pCardConf = CConfCardBase::Find(pCardBaseTemp->CardIndex);
			if (pCardConf == NULL)
			{
				IME_ERROR("can not find card in card csv index %u", pCardBaseTemp->CardIndex);
				return -1;
			}
			by << (uint16_t) pCardBaseTemp->BreakCount;
			by << (uint16_t) (pCardBaseTemp->BreakCount >= pCardConf->dwLimitBreakNumber ? 1 : 0);
		}
		else
		{
			by << (uint32_t) 0;
			by << (uint32_t) 0;
			by << (uint32_t) 0;
			by << (uint32_t) 0;
			by << (uint16_t) 0;
			by << (uint32_t) 0;
			by << (uint8_t) 0;
			by << (uint16_t) 0;
			by << (uint16_t) 0;
		}
	}
	pCardBaseTemp = pPeerCRole->GetCardBaseData(pPeerCRole->m_DurationMaster.CardObjId);
	if (pCardBaseTemp != NULL)
	{
		by << pCardBaseTemp->CardObjId;
		by << pCardBaseTemp->Health;
		by << (uint32_t)(pCardBaseTemp->HealthMaxM + pCardBaseTemp->HealthMaxE);
		by << pCardBaseTemp->CardIndex;
		by << pCardBaseTemp->CardLv;
		by << (uint32_t)(pCardBaseTemp->AttackM + pCardBaseTemp->AttackE);
		by << pCardBaseTemp->Rare;
		STC_CONF_CARD_BASE *pCardConf = CConfCardBase::Find(pCardBaseTemp->CardIndex);
		if (pCardConf == NULL)
		{
			IME_ERROR("can not find card in card csv index %u", pCardBaseTemp->CardIndex);
			return -1;
		}
		by << (uint16_t) pCardBaseTemp->BreakCount;
		by << (uint16_t) (pCardBaseTemp->BreakCount >= pCardConf->dwLimitBreakNumber ? 1 : 0);
	}
	else
	{
		by << (uint32_t) 0;
		by << (uint32_t) 0;
		by << (uint32_t) 0;
		by << (uint32_t) 0;
		by << (uint16_t) 0;
		by << (uint32_t) 0;
		by << (uint8_t) 0;
		by << (uint16_t) 0;
		by << (uint16_t) 0;
	}

	IME_DEBUG("PeerRole");
	pPeerCRole->HexDump();
	
	by << (uint32_t) pCRole->m_TotalData.Health;
	by << (uint32_t) pCRole->m_TotalData.HealthMax;
	by << (uint32_t) pPeerCRole->m_TotalData.Health;
	by << (uint32_t) pPeerCRole->m_TotalData.HealthMax;

	int RoundForEvaluate = 0;
	int HealthEvaluate = 0;

	std::vector<uint32_t> tmp;	
	tmp.push_back(CDuration::MASTER_MYSELF);
	tmp.push_back(CDuration::MASTER_PEER);
	tmp.push_back(CDuration::FIRST_MYSELF);
	tmp.push_back(CDuration::FIRST_PEER);
	tmp.push_back(CDuration::SECOND_MYSELF);
	tmp.push_back(CDuration::SECOND_PEER);
	int choose[CDuration::MAX_ORDER][3] = {
		{CDuration::MASTER_MYSELF, CDuration::FIRST_MYSELF, CDuration::SECOND_MYSELF},
		{CDuration::MASTER_PEER, CDuration::FIRST_PEER, CDuration::SECOND_PEER},
		{CDuration::FIRST_MYSELF, CDuration::SECOND_MYSELF, CDuration::MASTER_MYSELF},
		{CDuration::FIRST_PEER, CDuration::SECOND_PEER, CDuration::MASTER_PEER},
		{CDuration::SECOND_MYSELF, CDuration::FIRST_MYSELF, CDuration::MASTER_MYSELF},
		{CDuration::SECOND_PEER, CDuration::FIRST_PEER, CDuration::MASTER_PEER}
	};

	for(int i = 0; i < tmp.size(); i++)
	{
		int j;
		for (j = 0; j < 3; j++)
		{
			int line = choose[i][j];
			if (line <= CDuration::MASTER_MYSELF)
			{
				pTempRole = pCRole;
			}
			else
			{
				pTempRole = pPeerCRole;
			}
			line = choose[i][j] % (CDuration::MASTER_MYSELF + 1);
			if (pTempRole->IsEmpty(line))
			{
				continue;
			}
			else
			{
				tmp[i] = choose[i][j];
				break;
			}
		}

		if (j >= 3) // all is in empty, delete
		{
			tmp.erase(tmp.begin() + i);
			i--;
		}
	}

	for (int i = 0; i < tmp.size(); i++)
	{
		IME_DEBUG("%u order %u", i, tmp[i]);
	}
	vecOrder = tmp;

//	vecOrder.push_back(CDuration::MASTER_MYSELF);
//	vecOrder.push_back(CDuration::MASTER_PEER);
//	vecOrder.push_back(CDuration::FIRST_MYSELF);
//	vecOrder.push_back(CDuration::FIRST_PEER);
//	vecOrder.push_back(CDuration::SECOND_MYSELF);
//	vecOrder.push_back(CDuration::SECOND_PEER);	

	uint32_t MaxRound = 0;
	if (in.type == DURATION_NORMAL)
	{
		MaxRound = GlobalConfig::MaxRound;
	}
	else
	{
		MaxRound = GlobalConfig::FriendBossMaxRound;
	}

	for (int roundcnt = 0; roundcnt < MaxRound; roundcnt++)
	{
		RoundForEvaluate = roundcnt;

		pkgBody.push_back(std::vector<ByteBuffer>());
		std::vector<ByteBuffer> *ppkgRound = &pkgBody[pkgBody.size() - 1];
		ByteBuffer pkgAction;

		for (int i = 0; i < vecOrder.size(); i++)
		{
//			IME_ERROR("%u %u %u",vecOrder.size(),  i,  vecOrder[i]);
			// for every card action
			if (vecOrder[i] <= CDuration::MASTER_MYSELF)
			{
				pTempRole = pCRole;
				pPeerTempRole = pPeerCRole;
//				pSetHaltBuffer = &setHaltBuffer;
			}
			else
			{
				pTempRole = pPeerCRole;
				pPeerTempRole = pCRole;
//				pSetHaltBuffer = &setPeerHaltBuffer;
			}

//			pCardTemp = pTempRole->GetCardData(vecOrder[i]);
//			if (pCardTemp->Health <= 0)
//				continue;
//
//			if (pSetHaltBuffer->find(vecOrder[i]) != pSetHaltBuffer->end())
//			{
//				pkgAction << (uint8_t)ACTION_HALT_BUFFER << vecOrder[i] << vecTeam[i];
//				pkgAction << pTempRole->GetBuffer(vecOrder[i], S_HALT)->index;
//				ppkgRound->push_back(pkgAction);	
//				IME_DEBUG("============action=============");
//				::hexdump((const char *)pkgAction.contents(), pkgAction.size());
//				IME_DEBUG("============action=============");
//				pkgAction.clear();
//				continue;
//			}
//
//			CCardSkill::SCondition SCon;
//
//			if (pCardTemp->BigSkill != 0)
//			{
//				pConf = CConfCardSkill::Find(pCardTemp->BigSkill * 100 + pCardTemp->BigSkillLv);	
//				pConditionFunc = CCardSkill::FindCondition(pConf->dwCondition);	
//				SCon.round = roundcnt;	
//				uint8_t IsCondition = 1;
//
//				if (pConf->dwCondition != 0)
//				{
//					if (pConditionFunc == NULL)
//					{
//						IME_ERROR("can not find this condition func %u skill index %u", pConf->dwCondition, pConf->dwIndex);
//						return -1;
//					}
//					pTempRole->SetLaunchObjid(vecOrder[i]);
//					if (pConditionFunc(pTempRole, &SCon))
//					{
//						IsCondition = 0;
//					}
//				}
//				
//				if (IsCondition)
//				{
//					if (pCardTemp->BigSkillNumber > 0)
//					{
				uint8_t line = vecOrder[i] % (MASTER_LINE + 1);	
				uint8_t IsLaunchSkill = 0;
				int SkillSeq = 0;
				if (line == MASTER_LINE)
				{
					if (pTempRole->m_DurationMaster.CardObjId == 0)
					{
						continue;
					}

					if (pTempRole->m_DurationMaster.BigSkill != 0)
					{
						pConf = CConfCardSkill::Find(pTempRole->m_DurationMaster.BigSkill * 100 + pTempRole->m_DurationMaster.BigSkillLv);
						int totalpro = pConf->dwPro * (100 + pTempRole->m_DurationMaster.SkillPro) / 100.0;
						if (CUtil::RandEither(totalpro , 100))
						{
							IsLaunchSkill = 1;
						}					
						IME_DEBUG("master skill pro total %u, skill base pro %u, extra rate pro %u, happen %u", totalpro, pConf->dwPro, pTempRole->m_DurationMaster.SkillPro, IsLaunchSkill);
					}
				}
				else
				{
					pConf = NULL;
					std::vector<int> vecWeight;
					std::vector<int> vecSkill;
					int number = 0;
					int RemainWeigth = 0;
					uint32_t linebase = line * ONE_TIME_NUMBER;
					for (int i = 0; i < ONE_TIME_NUMBER; i++)
					{
						if (pTempRole->m_vecLineUp[i + linebase] != 0)
						{
							pCardTemp = pTempRole->GetCardData(pTempRole->m_vecLineUp[i + linebase]);
							pConf = CConfCardSkill::Find(pCardTemp->BigSkill * 100 + pCardTemp->BigSkillLv);
							if (pConf != NULL)
							{
								int totalpro = pConf->dwPro * (100 + pCardTemp->SkillPro) / 100.0; 
								IME_DEBUG("fellow skill pro total %u, skill base pro %u, extra rate pro %u", totalpro, pConf->dwPro, pCardTemp->SkillPro);
								if (totalpro  <= 100)
								{
									vecWeight.push_back(totalpro);
									vecSkill.push_back(pConf->dwIndex);
									RemainWeigth += pConf->dwPro;
								}
								else
								{
									vecWeight.push_back(100);
									vecSkill.push_back(pConf->dwIndex);
									RemainWeigth += 100;
								}
							}
							else
							{
								vecWeight.push_back(0);
								vecSkill.push_back(0);
							}
							number++;
						}
						else
						{
							vecWeight.push_back(0);
							vecSkill.push_back(0);
						}
					}
					if (number == 0)
					{
						continue;
					}
					RemainWeigth = number * 100 - RemainWeigth;	
					vecWeight.push_back(RemainWeigth);
					vecSkill.push_back(0);
					SkillSeq = CUtil::RandFactor(vecWeight);
					uint32_t SkillIndex = vecSkill[SkillSeq];
					if (SkillIndex != 0)
					{
						IsLaunchSkill = 1;
						pConf = CConfCardSkill::Find(SkillIndex);
					}
				}
						// big skill
				if (IsLaunchSkill)
				{
					pSkillFunc = CCardSkill::FindSkill(pConf->wEffectType);
					if (pSkillFunc == NULL)
					{
						IME_ERROR("can not find big skill type %u", pConf->wEffectType);
						return -1;
					}
					CCardSkill::SSkillStu Stu;
					Stu.pConf = pConf;
					Stu.IsLaunchSkill = IsLaunchSkill;
					Stu.line = line;
					Stu.SkillSeq = SkillSeq;
//					Stu.type = in.type;
					Stu.ismyself = (pTempRole == pCRole ? 1 : 0);
					//						pTempRole->SetLaunchObjid(vecOrder[i]);
					int ret = pSkillFunc(pTempRole, pPeerTempRole, &Stu, pkgAction, in);
					if (ret == -1)
					{
						IME_ERROR("skill func exec fail index %u", pConf->dwIndex);
						return -1;
					}
				}
				else
				{
					CCardSkill::SSkillStu Stu;
					Stu.pConf = NULL;
					Stu.IsLaunchSkill = IsLaunchSkill;
					Stu.line = line;
					Stu.SkillSeq = 0;
//					Stu.type = in.type;
					Stu.ismyself = (pTempRole == pCRole ? 1 : 0);
					int ret = CCardSkill::CardSkillAttack(pTempRole, pPeerTempRole, &Stu, pkgAction, in);
					if (ret == -1)
					{
						IME_ERROR("skill func exec fail index %u", pConf->dwIndex);
						return -1;
					}
					
				}
//				else if (ret == 0)
//				{
//					pCardTemp->BigSkillNumber--;
//					goto ACTION_END;
//				}
//				pkgAction.clear();
//					}
//				}
//			}
//
//			if (pCardTemp->TalentSkill != 0)
//			{
//				pConf = CConfCardSkill::Find(pCardTemp->TalentSkill);	
//				pConditionFunc = CCardSkill::FindCondition(pConf->dwCondition);	
//				SCon.round = roundcnt;	
//				uint8_t IsCondition = 1;
//
//				if (pConf->dwCondition != 0)
//				{
//					if (pConditionFunc == NULL)
//					{
//						IME_ERROR("can not find this condition func %u skill index %u", pConf->dwCondition, pConf->dwIndex);
//						return -1;
//					}
//					pTempRole->SetLaunchObjid(vecOrder[i]);
//					if (!pConditionFunc(pTempRole, &SCon))
//					{
//						IsCondition = 0;
//					}
//				}
//
//				if (IsCondition)
//				{
//					if (pCardTemp->TalentSkillNumber > 0)
//					{
//						// Talent skill
//						pSkillFunc = CCardSkill::FindSkill(pConf->wType);
//						if (pSkillFunc == NULL)
//						{
//							IME_ERROR("can not find talent skill type %u", pConf->wType);
//							return -1;
//						}
//						CCardSkill::SSkillStu Stu;
//						Stu.pBufferlist = &listBuffer;
//						Stu.pConf = pConf;
//						pTempRole->SetLaunchObjid(vecOrder[i]);
//						int ret = pSkillFunc(pTempRole, pPeerTempRole, &Stu, pkgAction);
//						if (ret == -1)
//						{
//							IME_ERROR("skill func exec fail index %u", pConf->dwIndex);
//							return -1;
//						}
//						else if(ret == 0)
//						{
//							pCardTemp->TalentSkillNumber--;
//							goto ACTION_END;
//						}
//						pkgAction.clear();
//					}
//				}
//			}
//			
//			pTempRole->SetLaunchObjid(vecOrder[i]);
//			if (CCardSkill::CardAttack(pTempRole, pPeerTempRole, pkgAction))
//			{
//				IME_ERROR("card normal attack fail");
//				return -1;
//			}
			
			// judge win or lose	
//ACTION_END:
			IME_DEBUG("=========after one action========");
			IME_DEBUG("Role");
			pCRole->HexDump();
			IME_DEBUG("PeerRole");
			pPeerCRole->HexDump();

			IME_DEBUG("============action=============");
			::hexdump((const char *)pkgAction.contents(), pkgAction.size());
			IME_DEBUG("============action=============");
			ppkgRound->push_back(pkgAction);	
			pkgAction.clear();

			if (pPeerCRole->m_TotalData.Health == 0)
			{
				Success = 0;
				HealthEvaluate = pCRole->m_TotalData.Health * 100.0 / pCRole->m_TotalData.HealthMax;
				goto DURATION_END;
			}
			if (pCRole->m_TotalData.Health == 0)
			{
				Success = 1;
				HealthEvaluate = pPeerCRole->m_TotalData.Health * 100.0 / pPeerCRole->m_TotalData.HealthMax;
				goto DURATION_END;
			}
		}		

	}

	Success = 2;

DURATION_END:
	uint8_t evaluation = 1;
	IME_DEBUG("round %u", RoundForEvaluate);
	IME_DEBUG("health %u", HealthEvaluate);

	pCRole->HexDump();	
	if (RoundForEvaluate < DURATION_ROUND_CONDITION)
		evaluation++;
	if (HealthEvaluate > DURATION_HEALTH_CONDITION)
		evaluation++;

	if (Success == 0)
	{
		eva = evaluation;
	}
	else
	{
		eva = 0;
	}
	IME_DEBUG("evaluation %u", eva);

	by << eva;
	by << (uint32_t) pkgBody.size();
	IME_DEBUG("======total round %u======", pkgBody.size());
	for (int i = 0; i < pkgBody.size(); i++)
	{
		IME_DEBUG("round seq %u", i);
		IME_DEBUG("--------total action %u----------", pkgBody[i].size());
		by << (uint32_t) pkgBody[i].size();
		for (int j = 0; j < pkgBody[i].size(); j++)
		{
			IME_DEBUG("action seqno %u", j);
			by.append(pkgBody[i][j]);
			::hexdump((const char *)pkgBody[i][j].contents(), pkgBody[i][j].size());
		}
	}	
	by << (uint32_t)pCRole->m_vecRelationIndex.size();
	for (int i = 0; i < pCRole->m_vecRelationIndex.size(); i++)
	{
		by << pCRole->m_vecRelationIndex[i];
	}
	by << (uint32_t)pPeerCRole->m_vecRelationIndex.size();
	for (int i = 0; i < pPeerCRole->m_vecRelationIndex.size(); i++)
	{
		by << pPeerCRole->m_vecRelationIndex[i];
	}
	IME_DEBUG("======================");
	return Success;
}

//void CDuration::GetFreeBufferPosition(uint32_t objid, uint32_t index, uint8_t bufferflag, int32_t &position)
//{
//	position = -1;
//	DurationData_t *pCard = GetCardData(objid);
//	if (pCard == NULL)
//		return;
//
//	for (int i = 0; i < MAX_STATUS; i++)
//	{
//		if (bufferflag == 0)
//		{
//			if (pCard->Buffer[i] == NULL)
//				position = i;
//			break;
//		}
//		else
//		{
//			if (pCard->Debuffer[i] == NULL)
//				position = i;
//			break;
//		}
//	}
//}

/* void CCombat::HandlerDuration(CRole *pCRole, WorldPacket &pkg)
 * {
 *  	uint32_t userid;
 *   	pkg >> userid;	
 * 
 *   	if (pCRole->GetdwStrength() < GlobalConfig::StrengthConsume)
 *   	{
 *   		pCRole->SendStrengthNotEnough(CMD_SC_DURATION_RESULT);
 *   		return;
 *   	}
 *   	
 *   	bool IsOnline = true;
 *   	CRole *pPeerCRole = sWorld->FindUserRole(userid);
 *   	CRole temp(NULL);
 *   	if (pPeerCRole == NULL)
 *   	{
 *   		pPeerCRole = &temp;
 *   		assert(pPeerCRole != NULL);
 *   		if (!pPeerCRole->LoadDataForDuration(userid))
 *   		{
 *   			IME_ERROR("load peer role from DB fail userid %u", userid);
 *   			pCRole->SendErrorMsg(ERRNO_MSG_DB_NO_DATA_OR_ERROR , CMD_SC_DURATION_RESULT);
 *   			return;
 *   		}
 *   		IsOnline = false;
 *   	}
 * 
 *   	if (pPeerCRole->GetdwAvoidDurationTime() > time(NULL))
 *   	{
 *   		pCRole->SendErrorMsg(ERRNO_MSG_AVOID_DURATION, CMD_SC_DURATION_RESULT);
 *   		return;
 *   	}	
 * 
 * 	WorldPacket Dpkg(CMD_SC_DURATION_PKG);
 * 	uint8_t eva;
 * 	int ret = DurationJudgeWin(pCRole, pPeerCRole, Dpkg, eva);
 * 	if (ret == -1)
 * 	{
 * 		IME_ERROR("Calculate win condition fail");
 * 		pCRole->SendErrorMsg(ERRNO_MSG_DB_NO_DATA_OR_ERROR , CMD_SC_DURATION_RESULT);
 * 		return;
 * 	}
 * 
 * 	uint32_t len = Dpkg.size();
 * 	const char *pStart = (const char *)Dpkg.contents();
 * 	uint32_t number;
 * 	while(len)
 * 	{
 * 		WorldPacket DpkgSplit(CMD_SC_DURATION_PKG);
 * 		if (len > PACKET_SPLIT_NUMBER)
 * 		{
 * 			number = PACKET_SPLIT_NUMBER;
 * 			len -= PACKET_SPLIT_NUMBER;
 * 		}
 * 		else
 * 		{
 * 			number = len;
 * 			len = 0;
 * 		}
 * 		
 * 		DpkgSplit.append(pStart, number);
 * 		pStart += number;
 * 		pCRole->SendPacket(&DpkgSplit);
 * 	}
 * 
 *   	STC_CONF_CARD_BASE *pConfCard = NULL;
 *   	int32_t tempcoin;
 *   	uint32_t tempExp;
 *   	uint32_t objid;
 *   	if (ret == 0) // win
 *   	{
 *   		//todo some notice
 *   		std::vector<uint32_t> *pWeaponArr = CConfCardBase::GetVecForDuration();	
 *   		if (pWeaponArr == NULL)
 *   		{
 *   			IME_ERROR("get weapon index fail");
 *   			pCRole->SendErrorMsg(ERRNO_MSG_CSV_DATA, CMD_SC_DURATION_RESULT);
 *   			return;
 *   		}
 *   		int weaponindex = CUtil::RandByRange(0, pWeaponArr->size());
 *   		pConfCard = CConfCardBase::Find((*pWeaponArr)[weaponindex]);	
 *   		if (pConfCard == NULL)
 *   		{
 *   			IME_ERROR("get weapon conf fail");
 *   			pCRole->SendErrorMsg(ERRNO_MSG_CSV_DATA, CMD_SC_DURATION_RESULT);
 *   			return;
 *   		}
 * 		uint32_t opt;
 *   		pCRole->GetclsCardPacket()->CardAddByPara(pConfCard->dwIndex, objid, opt, false); 
 * 		if (opt)
 *   		pCRole->GetclsCardPacket()->SingleCardAddUpdate(objid);
 *   
 *   		tempExp = uint32_t(pCRole->GetwLevel() * 0.5 + 0.5);
 *   		pCRole->GetclsTech()->TechExp(tempExp);
 *   		pCRole->AddExp(tempExp);
 *   
 *   		tempcoin = int32_t(pPeerCRole->GetdwCoin() * 0.1 + 0.5);
 *   		if (tempcoin > 1000) tempcoin = 1000;
 *   		uint32_t tcoin = tempcoin; 
 *   		pCRole->GetclsTech()->TechCoin(tcoin);
 *   		pCRole->ChangeCoin(tcoin);
 *   
 *   		pCRole->CalExp(true);
 *   
 *   		pPeerCRole->ChangeCoin(-tempcoin);
 *   
 *   		CDBCtrl::InsertDurationLose(pPeerCRole->GetdwAccountId(), pCRole->GetdwAccountId());
 *   		uint32_t mail_id;
 *   		CDBCtrl::stuMailDetail temp;
 *   		if (!CMail::AddEmailDurationWin(pCRole, pPeerCRole->GetdwAccountId(), mail_id, temp, tempcoin))
 *   		{
 *   			if (IsOnline)
 *   			{
 *   				CMail::EmailUpdate(pPeerCRole, CMail::ADD, &temp, 0); 
 *   			}
 *   		}
 *   	}
 *   	else // lose 
 *   	{
 *   		//todo some notice
 * 		ret = 1;
 *   		tempExp = 1;
 *   		pCRole->GetclsTech()->TechExp(tempExp);
 *   		pCRole->AddExp(tempExp);
 *   
 *   		tempcoin = int32_t(pCRole->GetdwCoin() * 0.1 + 0.5);
 *   		if (tempcoin > 1000) tempcoin = 1000;
 *   		pCRole->ChangeCoin(-tempcoin);
 *   		pCRole->CalExp(true);
 *   
 *   		pPeerCRole->ChangeCoin(tempcoin);
 *   
 *   		uint32_t mail_id;
 *   		CDBCtrl::stuMailDetail temp;
 *   		if (!CMail::AddEmailDurationLose(pCRole, pPeerCRole->GetdwAccountId(), mail_id, temp, tempcoin))
 *   		{
 *   			if (IsOnline)
 *   			{
 *   				CMail::EmailUpdate(pPeerCRole, CMail::ADD, &temp, 0); 
 *   			}
 *   		}
 *   	}
 *   	
 *   	// consume strength after every duration
 *   		
 *   	pCRole->ChangeStrength(-GlobalConfig::StrengthConsume);
 *   	pCRole->SendProUpdate();
 * 
 * 	WorldPacket info(CMD_SC_DURATION_RESULT);
 *  	info << (uint16_t) 0;
 * 	info << (uint8_t) ret;	
 *   	info << tempExp;
 *     info << tempcoin;	
 *   	if (ret == 0) //win
 *   	{
 *   	info << (uint32_t) WEAPON_GET;
 *   	info << (uint32_t) objid;
 *   	info << (uint32_t) 0;
 *   	info << (uint32_t) 0;
 *   	}
 *   	else  // lose
 *   	{
 *   	info << (uint32_t) 0;
 *   	info << (uint32_t) 0;
 *   	info << (uint32_t) 0;
 *   	info << (uint32_t) 0;
 *   	}
 * 	pCRole->SendPacket(&info);
 * 
 *   	if (IsOnline)
 *   	{
 *   		pPeerCRole->SendProUpdate();
 *   	}
 *   	else
 *   	{
 *   		pPeerCRole->SaveDataForDuration();
 *   	}
 * }
 */

void CCombat::HandlerSearchPlayer(CRole *pCRole, WorldPacket &pkg)
{
	std::string strPlayer;
	pkg >> strPlayer;
	IME_CHAR_LOG("action [userid %u search player %s]", pCRole->GetdwAccountId(), strPlayer.c_str());

	std::vector <CDBCtrl::stuPlayer> vecPlayer;
	CDBCtrl::GetUserBaseDataByName(pCRole->GetdwAccountId(), strPlayer, vecPlayer);

	WorldPacket info(CMD_SC_SEARCH_PLAYER_RESULT);
	info << (uint16_t)0;
	info << (uint8_t) vecPlayer.size();
	for (int i = 0; i < vecPlayer.size(); i++)
	{
		info << vecPlayer[i].dwUserid;
		info << vecPlayer[i].wLevel;
		info << vecPlayer[i].strName;
		uint32_t userid = vecPlayer[i].dwUserid;
		CRole *pCRole = sWorld->FindUserRole(userid);
		CRole temp(NULL);
		if (pCRole == NULL)
		{
			pCRole = &temp;
			assert(pCRole != NULL);
			if (!pCRole->LoadDataForDuration(userid))
			{
				IME_ERROR("load peer role from DB fail userid %u", userid);
				pCRole->SendErrorMsg(ERRNO_MSG_DB_NO_DATA_OR_ERROR, CMD_SC_SEARCH_PLAYER_RESULT);
				return;
			}
		}
		if (pCRole->GetclsCardPacket()->PreDataTeamCardData(info, 3))
		{
			pCRole->SendErrorMsg(ERRNO_MSG_DB_NO_DATA_OR_ERROR, CMD_SC_SEARCH_PLAYER_RESULT);
			return;
		}
	}
	pCRole->SendPacket(&info);
}

/* void CCombat::HandlerDurationPreview(CRole *pCRole, WorldPacket &pkg)
 * {
 * 	uint32_t userid;
 * 	pkg >> userid;	
 * 	uint32_t Coin;
 * 	uint16_t level;
 * 	std::string strName;
 * 	bool bIsSucc = CDBCtrl::GetUserBaseDataForDurationPreview(             
 * 		strName,                               
 * 		Coin,                                    
 * 		level,
 * 		userid
 * 		);                                           
 * 	if (!bIsSucc)
 * 	{
 * 		pCRole->SendErrorMsg(ERRNO_MSG_DB_NO_DATA_OR_ERROR , CMD_SC_DURATION_PREVIEW_RESULT);
 * 		return;
 * 	}
 * 
 * 	int32_t wincoin = int32_t(Coin * 0.1 + 0.5);
 * 	if (wincoin > 1000) wincoin = 1000;
 * 	int32_t losecoin = int32_t(pCRole->GetdwCoin() * 0.1 + 0.5);
 * 	if (losecoin > 1000) losecoin = 1000;
 * 	WorldPacket info(CMD_SC_DURATION_PREVIEW_RESULT);
 * 	info << (uint16_t) 0;
 * 	info << (uint32_t)(level * 0.5 + 0.5);
 * 	info << (int32_t)wincoin;
 * 	info << (uint32_t) 1;
 * 	info << -(int32_t)losecoin;
 * 	info << strName;
 * 	pCRole->SendPacket(&info);
 * }
 */

void CCombat::HandlerRobPreview(CRole *pCRole, WorldPacket &pkg)
{
	uint32_t userid;
	pkg >> userid;	
	uint32_t Coin;
	uint16_t level;
	std::string strName;
	bool bIsSucc = CDBCtrl::GetUserBaseDataForDurationPreview(             
		strName,                               
		Coin,                                    
		level,
		userid
		);                                           
	if (!bIsSucc)
	{
		pCRole->SendErrorMsg(ERRNO_MSG_DB_NO_DATA_OR_ERROR , CMD_SC_ROB_PREVIEW_RESULT);
		return;
	}

	WorldPacket info(CMD_SC_ROB_PREVIEW_RESULT);
	info << (uint16_t) 0;
	info << (uint32_t)(level * 0.5 + 0.5);
	info << (uint32_t) 1;
	info << strName;
	pCRole->SendPacket(&info);
}

void CCombat::HandlerSearchPlayerByItem(CRole *pCRole, WorldPacket &pkg)
{
	uint32_t itemid;
	pkg >> itemid;
	IME_CHAR_LOG("action [userid %u search player by item index %u in rob system]", pCRole->GetdwAccountId(), itemid);
	std::vector <CDBCtrl::stuPlayer> vecPlayer;
	CDBCtrl::GetUserbyItem(pCRole->GetdwAccountId(), itemid, pCRole->GetwLevel(), vecPlayer);
	
	std::vector<int> tmp;
	tmp = CUtil::RandByRangeMulti(0, vecPlayer.size(), MAX_SEARCH_ITEM_NUMBER * 2 / 3);
	WorldPacket info(CMD_SC_SEARCH_PLAYER_ITEM_RESULT);
	info << (uint16_t)0;
	info << (uint8_t) tmp.size();
	for (int i = 0; i < tmp.size(); i++)
	{
		info << vecPlayer[tmp[i]].dwUserid;
		info << vecPlayer[tmp[i]].wLevel;
		info << vecPlayer[tmp[i]].strName;
		uint32_t userid = vecPlayer[tmp[i]].dwUserid;
		CRole *pPeerCRole = sWorld->FindUserRole(userid);
		CRole temp(NULL);
		if (pPeerCRole == NULL)
		{
			pPeerCRole = &temp;
			if (!pPeerCRole->LoadDataForDuration(userid))
			{
				IME_ERROR("load peer role from DB fail userid %u", userid);
				pCRole->SendErrorMsg(ERRNO_MSG_DB_NO_DATA_OR_ERROR, CMD_SC_SEARCH_PLAYER_ITEM_RESULT);
				return;
			}
		}

		if (pPeerCRole->GetclsCardPacket()->PreDataTeamCardDataForRob(info, 3))
		{
			pCRole->SendErrorMsg(ERRNO_MSG_DB_NO_DATA_OR_ERROR, CMD_SC_SEARCH_PLAYER_ITEM_RESULT);
			IME_ERROR("prepare data fail");
			return;
		}
		IME_DEBUG("userid %u, wLevel %u", vecPlayer[tmp[i]].dwUserid, vecPlayer[tmp[i]].wLevel);
	}
	int robotnumber = MAX_SEARCH_ITEM_NUMBER - tmp.size();
	if (robotnumber < 0)
		robotnumber = 0;
	pCRole->GetCRobotCtrl()->PacketRandRobotTeamCardData(info, robotnumber, itemid);
	pCRole->SendPacket(&info);
}

void CCombat::HandlerRob(CRole *pCRole, WorldPacket &pkg)
{
/* 	uint32_t userid;
 * 	uint32_t itemid;
 * 	pkg >> userid;	
 * 	pkg >> itemid;
 * 
 * 	if (pCRole->GetdwStrength() < GlobalConfig::StrengthConsume)
 * 	{
 * //		pCRole->SendErrorMsg(ERRNO_MSG_NOT_ENOUGH_STRENGTH, CMD_SC_ROB_RESULT);
 * 		pCRole->SendStrengthNotEnough(CMD_SC_ROB_RESULT);
 * 		return;
 * 	}
 * 
 * 	
 * 	bool IsOnline = true;
 * 	CRole *pPeerCRole = sWorld->FindUserRole(userid);
 * 	CRole temp(NULL);
 * 	if (pPeerCRole == NULL)
 * 	{
 * 		pPeerCRole = &temp;
 * 		assert(pPeerCRole != NULL);
 * 		if (!pPeerCRole->LoadDataForDuration(userid))
 * 		{
 * 			IME_ERROR("load peer role from DB fail userid %u", userid);
 * 			pCRole->SendErrorMsg(ERRNO_MSG_DB_NO_DATA_OR_ERROR , CMD_SC_ROB_RESULT);
 * 			return;
 * 		}
 * 		IsOnline = false;
 * 	}
 * 
 * 	if (pPeerCRole->GetdwAvoidDurationTime() > time(NULL))
 * 	{
 * 		pCRole->SendErrorMsg(ERRNO_MSG_AVOID_DURATION, CMD_SC_ROB_RESULT);
 * 		return;
 * 	}	
 * 
 * 	if (pPeerCRole->GetclsItemPacket()->JudgeItemNumber(itemid))
 * 	{
 * 		pCRole->SendErrorMsg(ERRNO_MSG_PEER_NOT_ENOUGH_ITEM, CMD_SC_ROB_RESULT);
 * 		return;
 * 	}
 * 	
 * 	uint32_t AttributesSelf, Attributes;
 * 
 * 	std::vector<uint32_t> vecCardsSelf;
 * 	std::vector<uint32_t> vecIndexsSelf;
 * 	std::vector<sCSEffect> vecEffect;
 * //	std::vector<uint32_t> vecCards;
 * //	std::vector<uint32_t> vecIndexs;
 * 	std::vector<CCard *> vecpCards;
 * 	std::vector<sCSEffect> vecPeerEffect;
 * 	sCSEffect sfactor;
 * 
 * 	uint32_t initattack, initdefence;
 * 
 * 	if (pCRole->GetclsCardPacket()->GetEquipCards(vecCardsSelf, vecIndexsSelf))
 * 	{
 * 		IME_ERROR("Get card data fail");
 * 		pCRole->SendErrorMsg(ERRNO_MSG_DB_NO_DATA_OR_ERROR , CMD_SC_ROB_RESULT);
 * 		return;
 * 	}
 * //	if (pCRole->GetbyCurrentTeam() == 0)
 * //	{
 * //		AttributesSelf = pCRole->GetdwCardAttributes();
 * //	}
 * //	else
 * //	{
 * //		AttributesSelf = pCRole->GetdwCardAttributes2();
 * //	}
 * 
 * 	AttributesSelf = 1;
 * 
 * 	if (pPeerCRole->GetclsCardPacket()->GetEquippCards(vecpCards))
 * 	{
 * 		IME_ERROR("Get card data fail");
 * 		pCRole->SendErrorMsg(ERRNO_MSG_DB_NO_DATA_OR_ERROR , CMD_SC_ROB_RESULT);
 * 		return;
 * 	}
 * //	if (pPeerCRole->GetbyCurrentTeam() == 0)
 * //	{
 * //		Attributes = pPeerCRole->GetdwCardAttributes();
 * //	}
 * //	else
 * //	{
 * //		Attributes = pPeerCRole->GetdwCardAttributes2();
 * //	}
 * 	
 * 	Attributes = 1;
 * 
 * 	int ret = DurationJudgeWin(pCRole, pPeerCRole, vecEffect, vecPeerEffect, sfactor, initattack, initdefence);
 * 	if (ret == -1)
 * 	{
 * 		IME_ERROR("Calculate win condition fail");
 * 		pCRole->SendErrorMsg(ERRNO_MSG_DB_NO_DATA_OR_ERROR , CMD_SC_ROB_RESULT);
 * 		return;
 * 	}
 * 	// process bonus and punish
 * 	STC_CONF_CARD_BASE *pConfCard = NULL;
 * 	uint32_t tempExp;
 * 	bool IsRobItemSucc = false;
 * 	uint8_t RobPro = 0;
 * 	if (ret == 0) // win
 * 	{
 * 		//todo some notice
 * 		std::vector<uint32_t> *pWeaponArr = CConfCardBase::GetVecForDuration();	
 * 		if (pWeaponArr == NULL)
 * 		{
 * 			IME_ERROR("get weapon index fail");
 * 			pCRole->SendErrorMsg(ERRNO_MSG_CSV_DATA, CMD_SC_ROB_RESULT);
 * 			return;
 * 		}
 * 		int weaponindex = CUtil::RandByRange(0, pWeaponArr->size());
 * 		pConfCard = CConfCardBase::Find((*pWeaponArr)[weaponindex]);	
 * 		if (pConfCard == NULL)
 * 		{
 * 			IME_ERROR("get weapon conf fail");
 * 			pCRole->SendErrorMsg(ERRNO_MSG_CSV_DATA, CMD_SC_ROB_RESULT);
 * 			return;
 * 		}
 * 		uint32_t objid;
 * 		pCRole->GetclsCardPacket()->CardAddByPara(pConfCard->dwIndex, pConfCard->byStarLevel, objid); 
 * 		pCRole->GetclsCardPacket()->SingleCardAddUpdate(objid);
 * 
 * 		tempExp = uint32_t(pCRole->GetwLevel() * 0.5 + 0.5);
 * 		pCRole->GetclsTech()->TechExp(tempExp);
 * 		pCRole->AddExp(tempExp);
 * 
 * 
 * 		int32_t leveldiff = pCRole->GetwLevel() - pPeerCRole->GetwLevel();
 * 		if (leveldiff < -20)
 * 		{
 * 			RobPro = 80;
 * 		}
 * 		else if (leveldiff >= -20 && leveldiff < -10)
 * 		{
 * 			RobPro = 70;
 * 		}
 * 		else if ( leveldiff >= -10 && leveldiff < 0)
 * 		{
 * 			RobPro = 60;
 * 		}
 * 		else if (leveldiff == 0)
 * 		{
 * 			RobPro = 50;
 * 		}
 * 		else if (leveldiff > 0 && leveldiff <= 10)
 * 		{
 * 			RobPro = 40;
 * 		}
 * 		else if (leveldiff > 10 && leveldiff <= 20)
 * 		{
 * 			RobPro = 30;
 * 		}
 * 		else 
 * 		{
 * 			RobPro = 20;
 * 		}
 * 
 * 		IME_DEBUG("rob pro %u", RobPro);
 * 		if (CUtil::RandEither(RobPro, 100))
 * 		{
 * 			pCRole->GetclsItemPacket()->AddItem(itemid);
 * 			pPeerCRole->GetclsItemPacket()->DelItem(itemid);	
 * 			IsRobItemSucc = true;
 * 			pCRole->GetclsItemPacket()->ItemUpdate(itemid, CItem::ADD);
 * 			if (IsOnline)
 * 			{
 * 				pPeerCRole->GetclsItemPacket()->ItemUpdate(itemid, CItem::DEL);
 * 			}
 * 
 * 			uint32_t mail_id;
 * 			CDBCtrl::stuMailDetail temp;
 * 			if (!CMail::AddEmailRobGet(pCRole, pPeerCRole->GetdwAccountId(), mail_id, temp, itemid))
 * 			{
 * 				if (IsOnline)
 * 				{
 * 					CMail::EmailUpdate(pPeerCRole, CMail::ADD, &temp, 0); 
 * 				}
 * 			}
 * 
 * 		}
 * 		else
 * 		{
 * 			uint32_t mail_id;
 * 			CDBCtrl::stuMailDetail temp;
 * 			if (!CMail::AddEmailRobLose(pCRole, pPeerCRole->GetdwAccountId(), mail_id, temp, itemid))
 * 			{
 * 				if (IsOnline)
 * 				{
 * 					CMail::EmailUpdate(pPeerCRole, CMail::ADD, &temp, 0); 
 * 				}
 * 			}
 * 
 * 		}
 * 
 * 		pCRole->CalExp(true);
 * 
 * 	}
 * 	else // lose 
 * 	{
 * 		//todo some notice
 * 		tempExp = 1;
 * 		pCRole->GetclsTech()->TechExp(tempExp);
 * 		pCRole->AddExp(tempExp);
 * 
 * 		pCRole->CalExp(true);
 * 	}
 * 	
 * 	// consume strength after every duration
 * 	pCRole->ChangeStrength(-GlobalConfig::StrengthConsume);
 * 	pCRole->SendProUpdate();
 * 
 * 	WorldPacket info(CMD_SC_ROB_RESULT);
 * 	info << (uint16_t) 0;
 * //	IME_DEBUG("attack %d attack %d, defence %d, defence %d", AttackMinSelf, AttackMaxSelf, DefenceMin, DefenceMax);
 * 	if (ret == 0)
 * 	{
 * 		info << (uint8_t) 0;	
 * 		IME_DEBUG("0");
 * 	}
 * 	else
 * 	{
 * 		info << (uint8_t) 1;
 * 		IME_DEBUG("1");
 * 	}
 * 
 * 	info << pCRole->GetdwAccountId();
 * 	info << initattack;
 * 	info << AttributesSelf;
 * 	IME_DEBUG("userid %u, initattack %u, initcardattribute %u", pCRole->GetdwAccountId(), initattack, AttributesSelf);
 * 
 * 	info << (uint8_t) vecEffect.size();
 * 	for (int i = 0; i < vecEffect.size(); i++)
 * 	{
 * 		info << (uint32_t) vecEffect[i].ObjId;
 * 		info << (uint32_t) vecEffect[i].SkillIndex;
 * 		info << (uint32_t) vecEffect[i].SkillEffect;
 * 		info << (int32_t) vecEffect[i].Para1;
 * 		info << (int32_t) vecEffect[i].Para2;
 * 		info << (uint8_t) vecEffect[i].Order;
 * 		info << (uint32_t) vecEffect[i].attack;
 * 		info << (uint32_t) vecEffect[i].defence;
 * 		info << (uint32_t) vecEffect[i].attackattribute;
 * 		info << (uint32_t) vecEffect[i].defenceattribute;
 * 		IME_DEBUG("ojbid %u, skillindex %u, skilleffect %u, para1 %d para2 %d, order %u, attack %u, defence %u, attackattribute %u, defenceattribute %u", vecEffect[i].ObjId, vecEffect[i].SkillIndex, vecEffect[i].SkillEffect, vecEffect[i].Para1, vecEffect[i].Para2, vecEffect[i].Order, vecEffect[i].attack, vecEffect[i].defence, vecEffect[i].attackattribute, vecEffect[i].defenceattribute);
 * 	}
 * 
 * 	info << (uint8_t) vecCardsSelf.size();
 * 	for (int i = 0; i < vecCardsSelf.size(); i++)
 * 	{
 * 		info << vecCardsSelf[i];
 * 		info << vecIndexsSelf[i];
 * 	}
 * 
 * 	info << userid;
 * 	info << initdefence;
 *    	info << Attributes;	
 * 	IME_DEBUG("userid %u, initdefence %u, attribute %u", userid, initdefence, Attributes);
 * 
 * 	info << (uint8_t) vecPeerEffect.size();
 * 	for (int i = 0; i < vecPeerEffect.size(); i++)
 * 	{
 * 		info << (uint32_t) vecPeerEffect[i].ObjId;
 * 		info << (uint32_t) vecPeerEffect[i].SkillIndex;
 * 		info << (uint32_t) vecPeerEffect[i].SkillEffect;
 * 		info << (int32_t) vecPeerEffect[i].Para1;
 * 		info << (int32_t) vecPeerEffect[i].Para2;
 * 		info << (uint8_t) vecPeerEffect[i].Order;
 * 		info << (uint32_t) vecPeerEffect[i].attack;
 * 		info << (uint32_t) vecPeerEffect[i].defence;
 * 		info << (uint32_t) vecPeerEffect[i].attackattribute;
 * 		info << (uint32_t) vecPeerEffect[i].defenceattribute;
 * 		IME_DEBUG("ojbid %u, skillindex %u, skilleffect %u, para1 %d para2 %d, order %u, attack %u, defence %u, attackattribute %u, defenceattribute %u", vecPeerEffect[i].ObjId, vecPeerEffect[i].SkillIndex, vecPeerEffect[i].SkillEffect, vecPeerEffect[i].Para1, vecPeerEffect[i].Para2, vecPeerEffect[i].Order, vecPeerEffect[i].attack, vecPeerEffect[i].defence, vecPeerEffect[i].attackattribute, vecPeerEffect[i].defenceattribute);
 * 	}
 * 	info << (uint8_t) vecpCards.size();
 * 	for (int i = 0; i < vecpCards.size(); i++)
 * 	{
 * 		info << vecpCards[i]->GetdwObjId();
 * 		info << vecpCards[i]->GetdwIndex();
 * 		info << vecpCards[i]->GetbyAttribute();
 * 		info << vecpCards[i]->GetdwExp();
 * 		info << vecpCards[i]->GetdwExpLimit();
 * 		info << vecpCards[i]->GetwLevel();
 * 		info << vecpCards[i]->GetbyStarLevel();
 * 		info << vecpCards[i]->GetdwAttackMin();
 * 		info << vecpCards[i]->GetdwAttackMax();
 * 		info << vecpCards[i]->GetdwDefenseMin();
 * 		info << vecpCards[i]->GetdwDefenseMax();
 * 		info << vecpCards[i]->GetdwLastExpLimit();
 * 		IME_DEBUG("objid %u index %u exp %u, explimit %u, explastlimit %u", vecpCards[i]->GetdwObjId(), vecpCards[i]->GetdwIndex(), vecpCards[i]->GetdwExp(), vecpCards[i]->GetdwExpLimit(), vecpCards[i]->GetdwLastExpLimit());
 * 	}
 * 	
 * 	info << sfactor.attack;
 * 	info << sfactor.defence;
 * 	info << sfactor.attackattribute;
 * 	info << sfactor.defenceattribute;	
 * 	info << (uint8_t)sfactor.Para1;
 * 
 * 	IME_DEBUG("attack %u, defence %u, attackattr %u, defencearr %u", sfactor.attack, sfactor.defence, sfactor.attackattribute, sfactor.defenceattribute);
 * 
 * 	info << tempExp;
 * 	if (IsRobItemSucc)
 * 	{
 * 		info << (uint32_t) STUFF_GET;
 * 		info << (uint32_t) itemid;
 * 	}
 * 	else
 * 	{
 * 		info << (uint32_t)0;
 * 		info << (uint32_t)0;
 * 	}
 * 	info << (uint32_t) 0;
 * 	info << (uint32_t) 0;
 * 	pCRole->SendPacket(&info);
 * 
 * 	if (IsOnline)
 * 	{
 * 		pPeerCRole->SendProUpdate();
 * 	}
 * 	else
 * 	{
 * 		pPeerCRole->SaveDataForDuration();
 * 	}
 */
 	uint32_t userid;
 	uint32_t itemid;
	uint8_t flag;
 	pkg >> userid;	
 	pkg >> itemid;
	pkg >> flag;

	IME_CHAR_LOG("action [userid %u request rob]", pCRole->GetdwAccountId());
 
 	if (pCRole->GetdwStrength() < GlobalConfig::StrengthConsume)
 	{
 		pCRole->SendStrengthNotEnough(CMD_SC_ROB_RESULT);
 		return;
 	}
 
 	bool IsOnline = true;
	CDuration *pPeerDuration;
 	CRole *pPeerCRole = sWorld->FindUserRole(userid);
 	CRole temp(NULL);
	if (itemid == 0)
	{
		IME_ERROR("itemid is zero");
		pCRole->SendErrorMsg(ERRNO_MSG_DATA_ILLEGAL, CMD_SC_ROB_RESULT);
		return;
	}
	if (pCRole->GetdwAvoidDurationTime() > time(NULL))
	{
		pCRole->SendErrorMsg(ERRNO_MSG_MYSELF_AVOID_DURATION, CMD_SC_ROB_RESULT);
		return;
	}

	if (flag == 0)
	{
		if (pPeerCRole == NULL)
		{
			pPeerCRole = &temp;
			assert(pPeerCRole != NULL);
			if (!pPeerCRole->LoadDataForDuration(userid))
			{
				IME_ERROR("load peer role from DB fail userid %u", userid);
				pCRole->SendErrorMsg(ERRNO_MSG_DB_NO_DATA_OR_ERROR , CMD_SC_ROB_RESULT);
				return;
			}
			IsOnline = false;
		}

		if (pPeerCRole->GetdwAvoidDurationTime() > time(NULL))
		{
			pCRole->SendErrorMsg(ERRNO_MSG_AVOID_DURATION, CMD_SC_ROB_RESULT);
			return;
		}	

		if (pPeerCRole->GetclsItemPacket()->JudgeItemNumber(itemid))
		{
			pCRole->SendErrorMsg(ERRNO_MSG_PEER_NOT_ENOUGH_ITEM, CMD_SC_ROB_RESULT);
			return;
		}
		pPeerDuration = pPeerCRole;
	}
	else
	{
		if (!pCRole->GetCRobotCtrl()->CheckRobot(userid, itemid))
		{
			IME_ERROR("Robotid %u, itemid %u", userid, itemid);
			pCRole->SendErrorMsg(ERRNO_MSG_PEER_NOT_ENOUGH_ITEM, CMD_SC_ROB_RESULT);
			return;
		}
		pPeerDuration = pCRole->GetCRobotCtrl()->GetRobot(userid); 
	}

	WorldPacket Dpkg(CMD_SC_DURATION_PKG);
	uint8_t eva;
	SDurationIn in;
	int ret = DurationJudgeWin(pCRole, pPeerDuration, Dpkg, eva, in);
	if (ret == -1)
	{
		IME_ERROR("Calculate win condition fail");
		pCRole->SendErrorMsg(ERRNO_MSG_DB_NO_DATA_OR_ERROR , CMD_SC_DURATION_RESULT);
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
		pCRole->SendPacket(&DpkgSplit);
	}

  	// process bonus and punish
  	STC_CONF_CARD_BASE *pConfCard = NULL;
  	uint32_t tempExp = 0;
  	bool IsRobItemSucc = false;
  	uint8_t RobPro = 0;
  	if (ret == 0) // win
  	{
//  		tempExp = uint32_t(pCRole->GetwLevel() * 0.5 + 0.5);
//  		pCRole->GetclsTech()->TechExp(tempExp);
//  		pCRole->AddExp(tempExp);
  
		if (flag == 0)
		{
			int32_t leveldiff = pCRole->GetwLevel() - pPeerCRole->GetwLevel();
			if (leveldiff < -20)
			{
				RobPro = 80;
			}
			else if (leveldiff >= -20 && leveldiff < -10)
			{
				RobPro = 70;
			}
			else if ( leveldiff >= -10 && leveldiff < 0)
			{
				RobPro = 60;
			}
			else if (leveldiff == 0)
			{
				RobPro = 50;
			}
			else if (leveldiff > 0 && leveldiff <= 10)
			{
				RobPro = 40;
			}
			else if (leveldiff > 10 && leveldiff <= 20)
			{
				RobPro = 30;
			}
			else 
			{
				RobPro = 20;
			}
		}
		else
		{
			STC_CONF_ROB_ROBOT *pConf = CConfRobRobot::Find(userid);
			RobPro = pConf->dwPro;
		}

  		IME_DEBUG("rob pro %u", RobPro);
  		if (CUtil::RandEither(RobPro, 100))
  		{
			if (flag == 0)
			{
				pCRole->GetclsItemPacket()->AddItem(itemid, SOURCE_ROB);
				pPeerCRole->GetclsItemPacket()->DelItem(itemid, SOURCE_ROB);	
				IsRobItemSucc = true;
				pCRole->GetclsItemPacket()->ItemUpdate(itemid, CItem::ADD);
				if (IsOnline)
				{
					pPeerCRole->GetclsItemPacket()->ItemUpdate(itemid, CItem::DEL);
				}

				uint32_t mail_id;
				CDBCtrl::stuMailDetail temp;
				if (!CMail::AddEmailRobGet(pCRole, pPeerCRole->GetdwAccountId(), mail_id, temp, itemid))
				{
					if (IsOnline)
					{
						CMail::EmailUpdate(pPeerCRole, CMail::ADD, &temp, 0); 
					}
				}
			}
			else
			{
				pCRole->GetclsItemPacket()->AddItem(itemid, SOURCE_ROB);
				IsRobItemSucc = true;
				pCRole->GetclsItemPacket()->ItemUpdate(itemid, CItem::ADD);
				pCRole->GetCRobotCtrl()->SetUpadetime(userid);
			}
			pCRole->GetCMission()->RobSuccess(0, 1);
  		}
  		else
  		{
			if (flag == 0)
			{
				uint32_t mail_id;
				CDBCtrl::stuMailDetail temp;
				if (!CMail::AddEmailRobLose(pCRole, pPeerCRole->GetdwAccountId(), mail_id, temp, itemid))
				{
					if (IsOnline)
					{
						CMail::EmailUpdate(pPeerCRole, CMail::ADD, &temp, 0); 
					}
				}
			}
  
  		}
//  		pCRole->CalExp(true);
 		pCRole->GetCMission()->EveryRob(0, 1); 
  	}
  	else // lose 
  	{
  		//todo some notice
		ret = 1;	
//  		tempExp = 1;
//  		pCRole->GetclsTech()->TechExp(tempExp);
//  		pCRole->AddExp(tempExp);
//  
//  		pCRole->CalExp(true);
  	}
  	
  	// consume strength after every duration
	int addenergy = pCRole->GetExtraEnergy();
	int addstrength = pCRole->GetExtraStrength(); 
	IME_DEBUG("get extra energy %u strength %u", addenergy, addstrength);
	pCRole->ChangeEnergyNoLimit(addenergy);
	pCRole->ChangeStrengthNoLimit(addstrength);
  	pCRole->ChangeStrength(-GlobalConfig::StrengthConsume);
  	pCRole->SendProUpdate();
	pCRole->HandlerInfoOpt();

  	WorldPacket info(CMD_SC_ROB_RESULT);
  	info << (uint16_t) 0;
	info << (uint8_t) ret;	
  	info << tempExp;
  	if (IsRobItemSucc)
  	{
  		info << (uint32_t) STUFF_GET;
  		info << (uint32_t) itemid;
  	}
  	else
  	{
  		info << (uint32_t)0;
  		info << (uint32_t)0;
  	}
  	info << (uint32_t) 0;
  	info << (uint32_t) 0;
  	pCRole->SendPacket(&info);

	if (flag == 0)
	{
		if (IsOnline)
		{
			pPeerCRole->SendProUpdate();
			CCombine::HandlerCheckAndInfo(pPeerCRole);
		}
		else
		{
			pPeerCRole->SaveDataForDuration();
		}
	}
}

void CCombat::HandlerSearchWin(CRole *pCRole, WorldPacket &pkg)
{
	std::vector <uint32_t> vecPlayer;
	CDBCtrl::GetUserBaseDataAuto10ForLose(pCRole->GetdwAccountId(), vecPlayer);

	WorldPacket info(CMD_SC_SEARCH_WIN_RESULT);
	info << (uint16_t)0;
	info << (uint8_t) vecPlayer.size();
	for (int i = 0; i < vecPlayer.size(); i++)
	{
		info << vecPlayer[i];
		uint32_t userid = vecPlayer[i];
		CRole *pCRole = sWorld->FindUserRole(userid);
		CRole temp(NULL);
		if (pCRole == NULL)
		{
			pCRole = &temp;
			assert(pCRole != NULL);
			if (!pCRole->LoadDataForDuration(userid))
			{
				IME_ERROR("load peer role from DB fail userid %u", userid);
				pCRole->SendErrorMsg(ERRNO_MSG_DB_NO_DATA_OR_ERROR, CMD_SC_SEARCH_WIN_RESULT);
				return;
			}
		}
		info << pCRole->GetwLevel();
		info << pCRole->GetstrUserName();
		if (pCRole->GetclsCardPacket()->PreDataTeamCardData(info, 3))
		{
			pCRole->SendErrorMsg(ERRNO_MSG_DB_NO_DATA_OR_ERROR, CMD_SC_SEARCH_WIN_RESULT);
			return;
		}
	}
	pCRole->SendPacket(&info);

}

CRobot::~CRobot()
{
	if (m_pCard != NULL)
		delete m_pCard;
}

int CRobot::Init(uint32_t index)
{
	m_pCard = new (std::nothrow)CCard();
	if (m_pCard == NULL)
	{
		IME_ERROR("new a card fail");
		return -1;
	}
	STC_CONF_ROB_ROBOT *pConfRobot = CConfRobRobot::Find(index);
	if (pConfRobot == NULL)
	{
		IME_ERROR("can not find data in robot csv index [%u]", index);
		return -1;
	}

	m_strUserName = pConfRobot->strName;
	m_vecline.resize(MAX_CARD_CAPACITY);
	
	m_MasterId = 0;
	m_MasterLv = 0;
	m_RoleLv = pConfRobot->wLevel;
	m_RobotId = index;
	
	SBossCard temp;
	uint32_t initid = 1;
	
	std::vector<uint32_t> vecIndex;
	std::vector<uint16_t> vecLv;
	std::vector<uint32_t> vecSkillIndex;
	std::vector<uint16_t> vecSkillLevel;
	std::vector<uint32_t> vecAttackGrow;
	std::vector<uint32_t> vecDefenceGrow;
	std::vector<uint32_t> vecHealthGrow;

	vecIndex.push_back(pConfRobot->dwCardIndex1);
	vecIndex.push_back(pConfRobot->dwCardIndex2);
	vecIndex.push_back(pConfRobot->dwCardIndex3);
	vecIndex.push_back(pConfRobot->dwCardIndex4);
	vecIndex.push_back(pConfRobot->dwCardIndex5);
	vecIndex.push_back(pConfRobot->dwCardIndex6);

	vecLv.push_back(pConfRobot->wLevel1);
	vecLv.push_back(pConfRobot->wLevel2);
	vecLv.push_back(pConfRobot->wLevel3);
	vecLv.push_back(pConfRobot->wLevel4);
	vecLv.push_back(pConfRobot->wLevel5);
	vecLv.push_back(pConfRobot->wLevel6);

	vecSkillIndex.push_back(pConfRobot->dwSkillIndex1);
	vecSkillIndex.push_back(pConfRobot->dwSkillIndex2);
	vecSkillIndex.push_back(pConfRobot->dwSkillIndex3);
	vecSkillIndex.push_back(pConfRobot->dwSkillIndex4);
	vecSkillIndex.push_back(pConfRobot->dwSkillIndex5);
	vecSkillIndex.push_back(pConfRobot->dwSkillIndex6);

	vecSkillLevel.push_back(pConfRobot->wSkillLevel1);
	vecSkillLevel.push_back(pConfRobot->wSkillLevel2);
	vecSkillLevel.push_back(pConfRobot->wSkillLevel3);
	vecSkillLevel.push_back(pConfRobot->wSkillLevel4);
	vecSkillLevel.push_back(pConfRobot->wSkillLevel5);
	vecSkillLevel.push_back(pConfRobot->wSkillLevel6);

	vecAttackGrow.push_back(pConfRobot->dwAttackGrow1);
	vecAttackGrow.push_back(pConfRobot->dwAttackGrow2);
	vecAttackGrow.push_back(pConfRobot->dwAttackGrow3);
	vecAttackGrow.push_back(pConfRobot->dwAttackGrow4);
	vecAttackGrow.push_back(pConfRobot->dwAttackGrow5);
	vecAttackGrow.push_back(pConfRobot->dwAttackGrow6);

	vecDefenceGrow.push_back(pConfRobot->dwDefenceGrow1);
	vecDefenceGrow.push_back(pConfRobot->dwDefenceGrow2);
	vecDefenceGrow.push_back(pConfRobot->dwDefenceGrow3);
	vecDefenceGrow.push_back(pConfRobot->dwDefenceGrow4);
	vecDefenceGrow.push_back(pConfRobot->dwDefenceGrow5);
	vecDefenceGrow.push_back(pConfRobot->dwDefenceGrow6);

	vecHealthGrow.push_back(pConfRobot->dwHealthGrow1);
	vecHealthGrow.push_back(pConfRobot->dwHealthGrow2);
	vecHealthGrow.push_back(pConfRobot->dwHealthGrow3);
	vecHealthGrow.push_back(pConfRobot->dwHealthGrow4);
	vecHealthGrow.push_back(pConfRobot->dwHealthGrow5);
	vecHealthGrow.push_back(pConfRobot->dwHealthGrow6);

	for (int i = 0; i < MAX_CARD_CAPACITY; i++)
	{
		if(vecIndex[i] != 0)
		{
			if (m_pCard->CreateCard(vecIndex[i], vecLv[i], initid++))
			{
				IME_ERROR("create card fail index %u", vecIndex[i]);
				return -1; 
			}
			temp.dwObjId = m_pCard->GetdwObjId();
			temp.dwIndex = m_pCard->GetdwIndex();
			temp.byAttribute = m_pCard->GetbyAttribute();
//			temp.dwAttack = m_pCard->GetdwAttack() * vecAttackGrow[i] / 100.0;
//			temp.dwDefence = m_pCard->GetdwDefence() * vecDefenceGrow[i] / 100.0;
//			temp.dwHealth = m_pCard->GetdwHealth() * vecHealthGrow[i] / 100.0;
			temp.dwAttack = vecAttackGrow[i];
			temp.dwDefence = vecDefenceGrow[i];
			temp.dwHealth = vecHealthGrow[i];
			temp.dwCritRate = m_pCard->GetdwCritRate();
			temp.dwBigSkill = vecSkillIndex[i];
			temp.dwBigSkillLv = vecSkillLevel[i];
			temp.dwCardLv = vecLv[i];
			temp.byCardType = E_ITEM_PET_CARD;
//			IME_DEBUG("init fellower card objid %u index %u attribute %u attack %u defence %u health %u critrate %u BigSkill %u BigSkillLv %u cardlv %u cardtype %u", temp.dwObjId, temp.dwIndex, temp.byAttribute, temp.dwAttack, temp.dwDefence, temp.dwHealth, temp.dwCritRate, temp.dwBigSkill, temp.dwBigSkillLv, temp.dwCardLv, temp.byCardType);

			m_vecline[i] = temp.dwObjId;
			m_vec.push_back(temp);
		}	
		else
		{
			m_vecline[i] = 0;
		}
	}

	if (pConfRobot->dwMasterCardIndex != 0)
	{
		if (m_pCard->CreateCard(pConfRobot->dwMasterCardIndex, pConfRobot->wMasterLevel, initid++))
		{
			IME_ERROR("create card fail index %u", pConfRobot->dwMasterCardIndex);
			return -1; 
		}
		temp.dwObjId = m_pCard->GetdwObjId();
		temp.dwIndex = m_pCard->GetdwIndex();
		temp.byAttribute = m_pCard->GetbyAttribute();
//		temp.dwAttack = m_pCard->GetdwAttack() * pConfRobot->dwMasterAttackGrow / 100.0;
//		temp.dwDefence = m_pCard->GetdwDefence() * pConfRobot->dwMasterDefenceGrow / 100.0;
//		temp.dwHealth = m_pCard->GetdwHealth() * pConfRobot->dwMasterHealthGrow / 100.0 * DURATION_MASTER_HEALTH_PARA;
		temp.dwAttack = pConfRobot->dwMasterAttackGrow;
		temp.dwDefence = pConfRobot->dwMasterDefenceGrow;
		temp.dwHealth = pConfRobot->dwMasterHealthGrow * DURATION_MASTER_HEALTH_PARA;
		temp.dwCritRate = m_pCard->GetdwCritRate();
		temp.dwBigSkill = pConfRobot->dwMasterSkillIndex;
		temp.dwBigSkillLv = pConfRobot->wMasterSkillLevel;
		temp.dwCardLv = pConfRobot->wMasterLevel;
		temp.byCardType = E_ITEM_MASTER_CARD;
//		IME_DEBUG("NPC master objid %u index %u attack %u defence %u health %u", m_pCard->GetdwObjId(), m_pCard->GetdwIndex(), m_pCard->GetdwAttack(), m_pCard->GetdwDefence(), m_pCard->GetdwHealth());
//		IME_DEBUG("init fellower card objid %u index %u attribute %u attack %u defence %u health %u critrate %u BigSkill %u BigSkillLv %u cardlv %u cardtype %u", temp.dwObjId, temp.dwIndex, temp.byAttribute, temp.dwAttack, temp.dwDefence, temp.dwHealth, temp.dwCritRate, temp.dwBigSkill, temp.dwBigSkillLv, temp.dwCardLv, temp.byCardType);
		
		m_vec.push_back(temp);
		m_MasterId = temp.dwObjId;
		m_MasterLv = pConfRobot->wMasterLevel;
	}
	else
	{
		IME_ERROR("master cardindex == 0 robort index %u", pConfRobot->dwIndex);
		return -1;
	}
	m_vec.resize(MAX_CARD_CAPACITY + 1);
	
	if (m_vec.size() > MAX_CARD_CAPACITY + 1)
	{
		IME_ERROR("boss card number %u bigger than max card capacity", m_vec.size());
		return -1;
	}

	m_dwDevote = pConfRobot->dwDevote;

	return 0;
}

uint32_t CRobot::GetTotalForce()
{
	uint32_t attack = 0;
	uint32_t defence = 0;
	uint32_t health = 0;
	uint32_t Att = 0;
	for (int i = 0; i < m_vec.size(); i++)
	{
		attack += m_vec[i].dwAttack;
		defence += m_vec[i].dwDefence;
		health += m_vec[i].dwHealth;
	}
	Att = POWER_CALCULATE(attack,defence,health);
	return Att;
}

int CRobot::InitDataForArena()
{
	uint32_t prestige;
	uint32_t lasttime;
	uint32_t challenge_number;
	uint32_t ranked;
	uint32_t number;
	bool ret = CDBCtrl::GetUserArena(m_RobotId, prestige, m_dwRank, lasttime, challenge_number, ranked, number);
	if (ret)
	{
		return 0;
	}
	else
	{
		return -1;
	}
}

int CRobot::PrepareDurationData(std::vector<cardforduration_t> &vec, std::vector<uint32_t> &vecline)
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
		vec[i].HealthMaxM = m_vec[i].dwHealth;
		vec[i].HealthMaxE = 0;
		vec[i].Crit = m_vec[i].dwCritRate;
		vec[i].Attribute = m_vec[i].byAttribute;
		vec[i].SkillPro = 0;
		vec[i].SkillPower = 0;
		vec[i].CardLv = m_vec[i].dwCardLv;
		vec[i].CardType = m_vec[i].byCardType;
		vec[i].Rare = E_CR_NORMAL;
	}
	CDuration::SetStrUserName(m_strUserName);
	vecline.insert(vecline.begin(), m_MasterId);
	return 0;
}

CRobot::SBossCard* CRobot::GetMaster()
{
	std::vector<SBossCard>::iterator it;
	for (it = m_vec.begin(); it != m_vec.end(); it++)
	{
//		if (it->byCardType == E_ITEM_MASTER_CARD)
//			return &(*it);
		if (it->dwObjId == m_MasterId)
			return &(*it);
	}
	return NULL;
}

int CRobot::PreDataTeamCardDataForArena(WorldPacket &by)
{
	uint32_t attack = 0;
	uint32_t defence = 0;
	uint32_t health = 0;
	uint32_t Att = 0;
	int masterpos = -1;
	for (int i = 0; i < m_vec.size(); i++)
	{
		if (m_vec[i].dwObjId == m_MasterId)
		{
			masterpos = i;
		}
		else
		{
			attack += m_vec[i].dwAttack;
			defence += m_vec[i].dwDefence;
			health += m_vec[i].dwHealth;
		}
	}

	if (masterpos != -1)
	{
		attack += m_vec[masterpos].dwAttack;
		defence += m_vec[masterpos].dwDefence;
		health += m_vec[masterpos].dwHealth;
		Att = POWER_CALCULATE(attack, defence, health);

		by << (uint32_t) Att;
		by << (uint32_t) m_vec[masterpos].dwIndex;
	}
	else
	{
		Att = POWER_CALCULATE(attack,defence,health);

		by << (uint32_t) Att;
		by << (uint32_t) 0;
	}
	return 0;
}

int CRobot::PreDataForRank(SFBRankRole &tmp)
{
	tmp.userid = m_RobotId;
	tmp.level = m_RoleLv;
	tmp.name = m_strUserName;
	for (int i = 0; i < m_vec.size(); i++)
	{
		if (m_vec[i].dwIndex != 0)
		{
//			if (m_vec[i].byCardType == E_ITEM_MASTER_CARD)
			if (m_vec[i].dwObjId == m_MasterId)
			{
				tmp.masterindex = m_vec[i].dwIndex;
			}
			else
			{
				tmp.cardindex.push_back(m_vec[i].dwIndex);
			}
		}
	}
	tmp.devote = m_dwDevote;
	return 0;
}

CRobotCtrl::CRobotCtrl():m_IsInit(false),m_pCRole(NULL)
{

}

int CRobotCtrl::Init(CRole *pCRole)
{
//	IME_ERROR("1 now time %u", getMSTime());
	m_pCRole = pCRole;
	std::string temp;
	ByteBuffer by;
	if (!CDBCtrl::GetUserRob(temp, m_pCRole->GetdwAccountId()))
	{
		IME_ERROR("can not get rob data userid %u", m_pCRole->GetdwAccountId());
		return -1;
	}
	by.append(temp.data(), temp.size());
	SeriliazeDB(by);
	
////	IME_ERROR("2 now time %u", getMSTime());
//	CConfRobRobot::MapData::iterator it;
//	CConfRobRobot::MapData *p = CConfRobRobot::GetAllData();
////	IME_DEBUG("total robot number %u", p->size());
//	for (it = p->begin(); it != p->end(); it++)
//	{
//		if (it->second.dwFragment1 != 0 || it->second.dwFragment2 != 0 || it->second.dwFragment3 != 0 || it->second.dwFragment4 != 0 || it->second.dwFragment5 != 0 || it->second.dwFragment6 != 0)
//		{
//			if (m_mapRobot[it->first].Init(it->first))
//			{
//				IME_ERROR("init robot fail index %u", it->first);
//				return -1;
//			}
////			IME_DEBUG("init robort id %u", it->first);
//		}
//	}	
	std::map<uint32_t, CRobot>& m_mapRobot = sWorld->GetRobRobot();
//	IME_ERROR("3 now time %u", getMSTime());
	std::map<uint32_t, uint32_t> maptmp;
	std::map<uint32_t, CRobot>::iterator ita;
	for (ita = m_mapRobot.begin(); ita != m_mapRobot.end(); ita++)
	{
		if (m_mapRob.find(ita->first) != m_mapRob.end())
		{
			maptmp[ita->first] = m_mapRob[ita->first];
		}	
		else
		{
			maptmp[ita->first] = 0;
		}
	}
	m_mapRob = maptmp;

//	IME_ERROR("4 now time %u", getMSTime());
	m_IsInit = true;
	return 0;
}

void CRobotCtrl::SaveData()
{
	if (!m_IsInit)
		return;
	ByteBuffer by;
	DeSeriliazeDB(by);
	std::string temp;
	temp.assign((const char *)by.contents(), by.size());
	CDBCtrl::SaveUserRob(temp, m_pCRole->GetdwAccountId());

	return;
}

void CRobotCtrl::SeriliazeDB(ByteBuffer &by)
{
	if (by.size() == 0)
		return;
	uint32_t number;
	uint32_t key,val;
	by >> number;
	for (int i = 0; i < number; i++)
	{
		by >> key;
		by >> val;
		m_mapRob[key] = val;
	}
}

void CRobotCtrl::DeSeriliazeDB(ByteBuffer &by)
{
	by << (uint32_t) m_mapRob.size();
	std::map<uint32_t, uint32_t>::iterator it;
	for (it = m_mapRob.begin(); it != m_mapRob.end(); it++)
	{
		by << it->first;
		by << it->second;
	}
}

void CRobotCtrl::PacketRandRobotTeamCardData(WorldPacket &pkg, uint8_t randnumber, uint32_t itemindex)
{
	std::map<uint32_t, CRobot> &m_mapRobot = sWorld->GetRobRobot();
	std::vector<STC_CONF_ROB_ROBOT *>* pVec;	
	std::vector<STC_CONF_ROB_ROBOT *>::iterator it;
	std::vector<int> vecRobotId;
	pVec = CConfRobRobot::GetData(itemindex);
	for (it = pVec->begin(); it != pVec->end(); it++)
	{
		uint32_t robotid = (*it)->dwIndex;
		if (time(NULL) > m_mapRob[robotid] + (*it)->dwCD)
		{
			vecRobotId.push_back(robotid);
			IME_DEBUG("robotid %u", robotid);
		}	
	}
	std::vector<int> tmp;
	STC_CONF_ROB_ROBOT *pConf;
	tmp = CUtil::RandByRangeMulti(0, vecRobotId.size(), randnumber);
	pkg << (uint8_t) tmp.size();
	for (int i = 0; i < tmp.size(); i++)
	{
		pConf = CConfRobRobot::Find(vecRobotId[tmp[i]]);
		IME_DEBUG("robotid %u", vecRobotId[tmp[i]]);
		if (pConf == NULL)
			return;
		pkg << vecRobotId[tmp[i]];
		pkg << pConf->wLevel;
		pkg << pConf->strName;
		pkg << (uint32_t) 3; 
		for (int j = 0; j < 3; j++)
		{
			pkg << m_mapRobot[vecRobotId[tmp[i]]].m_vec[j].dwIndex;
			IME_DEBUG("%u fellower %u", j, m_mapRobot[vecRobotId[tmp[i]]].m_vec[j].dwIndex);
		}
		pkg << m_mapRobot[vecRobotId[tmp[i]]].GetMaster()->dwIndex;
		pkg << m_mapRobot[vecRobotId[tmp[i]]].GetTotalForce();
		IME_DEBUG("master %u", m_mapRobot[vecRobotId[tmp[i]]].GetMaster()->dwIndex);
	}
	return;
}

bool CRobotCtrl::CheckRobot(uint32_t robotid, uint32_t itemid)
{
	if (m_mapRob.find(robotid) == m_mapRob.end())
	{
		IME_ERROR("can not find robotid in map %u", robotid);
		return false;
	}
	STC_CONF_ROB_ROBOT *pConf = CConfRobRobot::Find(robotid);
	if (pConf == NULL)
		return false;
	if (time(NULL) < m_mapRob[robotid] + pConf->dwCD)
	{
		IME_ERROR("the robot have been robed index %u", robotid);
		return false;
	}
	std::vector<int> tmp;
	tmp.push_back(pConf->dwFragment1);
	tmp.push_back(pConf->dwFragment2);
	tmp.push_back(pConf->dwFragment3);
	tmp.push_back(pConf->dwFragment4);
	tmp.push_back(pConf->dwFragment5);
	tmp.push_back(pConf->dwFragment6);
	if (std::find(tmp.begin(), tmp.end(), itemid) == tmp.end())
	{
		IME_ERROR("the robot have not this piece index %u, itemid %u", robotid, itemid);
		return false;
	}

//	std::vector<STC_CONF_ROB_ROBOT *>* pVec;	
//	pVec = CConfRobRobot::GetData(itemid);
//	if (std::find(pVec->begin(), pVec->end(), itemid) == pVec->end())
//	{
//		IME_ERROR("the robot have not this piece index %u", robotid);
//		return false;
//	}
	return true;
}

CRobot *CRobotCtrl::GetRobot(uint32_t robotid)
{
	std::map<uint32_t, CRobot> &m_mapRobot = sWorld->GetRobRobot();
	if (m_mapRob.find(robotid) == m_mapRob.end())
		return NULL;
	if (m_mapRobot.find(robotid) == m_mapRobot.end())
		return NULL;
	return &m_mapRobot[robotid];
}

void CRobotCtrl::SetUpadetime(uint32_t robotid)
{
	m_mapRob[robotid] = time(NULL);
}
