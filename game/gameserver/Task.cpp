/*
 * =====================================================================================
 *
 *       Filename:  Task.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2012年09月06日 10时24分09秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  imane (), imanecr@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include "Task.h"
#include "WorldPacket.h"
#include "Role.h"
#include "ConfigData.h"
#include "RandomEvent.h"
#include "CmdDefine.h"
#include "ErrorMsg.h"
#include "Tech.h"
#include "CardPacket.h"
#include "EnumDefines.h"
#include "Other.h"
#include "Tujian.h"
#include "Mission.h"
#include "Mail.h"
#include "Friend.h"

//void ProcessMBonus(CRole *pCRole, STC_CONF_MISSION_BASE *p, STC_CONF_RANDOM_EVENT *q, int flag, int durationcmd)
//{
//	if (q != NULL)
//	{
//		if (q->wType == CRandomEvent::CARD_DROP)
//		{
//			if(pCRole->GetclsCardPacket()->IsFull())
//			{
//				IME_ERROR("user %u card packet already full", pCRole->GetdwAccountId());
//				pCRole->SendErrorMsg(ERRNO_MSG_CARD_PACKET_NOT_ENOUGH, CMD_SC_SIMPLE_MISSION_RESULT);
//				return;
//			}
//		}
//	}
////	if (p->dwBoss)
////	{
////		if(pCRole->GetclsCardPacket()->IsFull())
////		{
////			IME_ERROR("user %u card packet already full", pCRole->GetdwAccountId());
////			pCRole->SendErrorMsg(ERRNO_MSG_CARD_PACKET_NOT_ENOUGH, CMD_SC_SIMPLE_MISSION_RESULT);
////			return;
////		}
////	}
//
////	if (p->dwBoss != 0)
////	{
////		CBoss *pCBoss = new(std::nothrow) CBoss();
////		assert(pCBoss != NULL);
////		int ret = ProcessBoss(pCRole, pCBoss, p->dwIndex, CMD_SC_MISSION_DURATION_RESULT, durationcmd);
////		delete pCBoss;
////		if (ret == -1)
////		{
////			pCRole->SendErrorMsg(ERRNO_MSG_CSV_LOAD, CMD_SC_SIMPLE_MISSION_RESULT);
////			return;
////		}
////		else if (ret == 1)
////		{
////			pCRole->ChangeEnergy(-(p->wConsumeEnergy));
////			pCRole->SendProUpdate();
////			pCRole->SendErrorMsg(ERRNO_MSG_MISSION_DURATION_LOSE, CMD_SC_SIMPLE_MISSION_RESULT);
////			return;
////		}
////	}
//
//	STC_CONF_RANDOM_EVENT retemp;
//	retemp.para1 = 0;
//	retemp.para2 = 0;
//	retemp.para3 = 0;
//	retemp.para4 = 0;
////	if (!p->dwBoss)
////	{
//		if (q != NULL)
//		{
//			IME_DEBUG("mission event %u, random type %u", p->dwRandomEventIndex, q->wType);
//			if (CUtil::RandEither(p->byRate, 100)) // random event happen
//			{
//				memcpy(&retemp, q, sizeof(STC_CONF_RANDOM_EVENT));
//				CRandomEvent::initialization();
//				CRandomEvent::REventFunc HandlerRandomEventFunc;
//				HandlerRandomEventFunc = CRandomEvent::Find(q->wType);
//				if (HandlerRandomEventFunc == NULL)
//				{
//					IME_ERROR("no this random type %u", q->wType);
//					pCRole->SendErrorMsg(ERRNO_MSG_CSV_DATA, CMD_SC_SIMPLE_MISSION_RESULT);
//					return;
//				}
//				else
//				{
//					if (HandlerRandomEventFunc(pCRole, retemp.para1, retemp.para2, retemp.para3, retemp.para4))
//					{
//						IME_ERROR("random handler fail");
//						pCRole->SendErrorMsg(ERRNO_MSG_CSV_DATA, CMD_SC_SIMPLE_MISSION_RESULT);
//						return;
//					}
//				}
//			}	
//		}
////	}
////	else
////	{
////		STC_CONF_RANDOM_EVENT *pDurationEvent;
////		uint32_t DurationPro;
////		if (durationcmd == 0)
////		{
////			pDurationEvent = CConfRandomEvent::Find(p->dwAttackPrize);
////			DurationPro = p->byAttackPro;
////		}
////		else
////		{
////			pDurationEvent = CConfRandomEvent::Find(p->dwDefencePrize);
////			DurationPro = p->byDefencePro;
////		}
////		if (pDurationEvent != NULL)
////		{
////			IME_DEBUG("mission event %u, random type %u", p->dwRandomEventIndex, pDurationEvent->wType);
////			if (CUtil::RandEither(DurationPro, 100)) // random event happen
////			{
////				CRandomEvent::initialization();
////				CRandomEvent::REventFunc HandlerRandomEventFunc;
////				HandlerRandomEventFunc = CRandomEvent::Find(pDurationEvent->wType);
////				if (HandlerRandomEventFunc == NULL)
////				{
////					IME_ERROR("no this random type %u", pDurationEvent->wType);
////					pCRole->SendErrorMsg(ERRNO_MSG_CSV_DATA, CMD_SC_SIMPLE_MISSION_RESULT);
////					return;
////				}
////				else
////				{
////					if (HandlerRandomEventFunc(pCRole, retemp.para1, retemp.para2, retemp.para3, retemp.para4))
////					{
////						IME_ERROR("random handler fail");
////						pCRole->SendErrorMsg(ERRNO_MSG_CSV_DATA, CMD_SC_SIMPLE_MISSION_RESULT);
////						return;
////					}
////				}
////			}	
////		}
////	}
//
//	IME_DEBUG("flag %u energy %u energylimit %u, taskindex %u %u %u %u", flag, pCRole->GetdwEnergy(), pCRole->GetdwEnergyMax(), pCRole->GetbyTaskLocation1(), pCRole->GetbyTaskLocation2(), pCRole->GetbyTaskLocation3(), pCRole->GetbyTaskLocation4());
//
//	if (flag == 1)
//	{
//		pCRole->SetbyTaskLocation4(pCRole->GetbyTaskLocation4() + 1);
//	}
//	else if(flag == 2)
//	{
//		uint32_t location = pCRole->GetbyTaskLocation1() * 10000 + pCRole->GetbyTaskLocation2() * 100 +pCRole->GetbyTaskLocation3();
//		if (location == 0)
//		{
//			pCRole->SetbyTaskLocation1(1);
//			pCRole->SetbyTaskLocation2(1);
//			pCRole->SetbyTaskLocation3(1);
//			pCRole->SetbyTaskLocation4(1);
//
//		}
//		else
//		{
//			STC_CONF_MISSION_BASE *ptempMission;
//			CConfMissionBase::Findp1(location, &ptempMission);
//			pCRole->SetbyTaskLocation1(ptempMission->byChapter);
//			pCRole->SetbyTaskLocation2(ptempMission->byCheckpoint);
//			pCRole->SetbyTaskLocation3(ptempMission->byTask);
//			pCRole->SetbyTaskLocation4(1);
//		}
//
//	}
//	pCRole->ChangeEnergy(-(p->wConsumeEnergy));
//	
//	uint32_t tcoin = p->wGetCoin;
//	uint32_t texp = p->wGetExp;
//	pCRole->GetclsTech()->TechCoin(tcoin);
//	pCRole->GetclsTech()->TechExp(texp);
//	pCRole->ChangeCoin(tcoin);
//	pCRole->AddExp(texp);
//
//	IME_DEBUG("energy %u energylimit %u", pCRole->GetdwEnergy(), pCRole->GetdwEnergyMax());
//
//	if (pCRole->CalExp(true))
//	{
//		pCRole->SendErrorMsg(ERRNO_MSG_CALCULATE, CMD_SC_SIMPLE_MISSION_RESULT);
//		return;
//	}
//
//	WorldPacket info(CMD_SC_SIMPLE_MISSION_RESULT);
//	info << (uint16_t) 0;
//	info << retemp.para1;
//	info << retemp.para2;
//	info << retemp.para3;
//	info << retemp.para4;
//	IME_DEBUG("type %u, coin %u", retemp.para1, retemp.para2);
//	pCRole->SendPacket(&info);
//}


//void CTask::HandlerMissionInfo (CRole *pCRole, WorldPacket &pkg)
//{
//	uint32_t taskindex;
//	pkg >> taskindex;
//	if (taskindex == 0)
//	{
//		IME_ERROR("taskindex is unvalid 0");
//		pCRole->SendErrorMsg(ERRNO_MSG_DATA_ILLEGAL);
//		return;
//	}
//
//	STC_CONF_MISSION_BASE *p = CConfMissionBase::Find(taskindex);
//	if (p == NULL)
//	{
//		IME_ERROR("load mission conf fail taskindex %u", taskindex);
//		pCRole->SendErrorMsg(ERRNO_MSG_CSV_LOAD);
//		return;
//	}
//	WorldPacket info(CMD_SC_SIMPLE_MISSION_INFO);
//	info << p->byStepLimit;
//	info << p->byType;
//	info << p->wConsumeEnergy;
//	info << p->wGetExp;
//	info << p->wGetCoin;	
//	info << p->dwBoss;
//	pCRole->SendPacket(&info);
//	return ;
//}		/* -----  end of method CTask::HandlerMissionInfo  ----- */
//
//
//void CTask::HandlerMissionAccept (CRole *pCRole, WorldPacket &pkg)
//{
//	uint32_t taskindex;
//	uint8_t durationcmd;
//	pkg >> taskindex;
//	pkg >> durationcmd;
//
//	if (taskindex == 0)
//	{
//		IME_ERROR("taskindex is unvalid 0");
//		pCRole->SendErrorMsg(ERRNO_MSG_DATA_ILLEGAL, CMD_SC_SIMPLE_MISSION_RESULT);
//		return;
//	}
//
//	STC_CONF_MISSION_BASE *p = CConfMissionBase::Find(taskindex);
//	if (p == NULL)
//	{
//		IME_ERROR("load mission conf fail taskindex %u", taskindex);
//		pCRole->SendErrorMsg(ERRNO_MSG_CSV_LOAD, CMD_SC_SIMPLE_MISSION_RESULT);
//		return;
//	}
//
//	STC_CONF_RANDOM_EVENT *q = NULL;
////	if (p->dwRandomEventIndex != 0)
////	{
////		q = CConfRandomEvent::Find(p->dwRandomEventIndex);
////		if (q == NULL)
////		{
////			IME_ERROR("load random conf fail index %u", p->dwRandomEventIndex);
////			pCRole->SendErrorMsg(ERRNO_MSG_CSV_LOAD, CMD_SC_SIMPLE_MISSION_RESULT);
////			return;
////
////		}	
////	}
//
//	uint32_t tempenergy = pCRole->GetdwEnergy();
//	if (pCRole->GetdwEnergy() < p->wConsumeEnergy)
//	{
//		//todo something about resume
//		IME_ERROR("energy not enough taskindex %u", taskindex);
//		//todo format wrong
////		WorldPacket pkg(CMD_SC_SIMPLE_MISSION_RESULT);
////		pkg << (uint16_t) ERROR_MSG_NOT_ENOUGH_ENERGY;
////		pkg << (uint32_t) 0;
////		pkg << (uint32_t) 0;
////		pCRole->SendPacket(&pkg);
//
//		pCRole->SendEnergyNotEnough(CMD_SC_SIMPLE_MISSION_RESULT);
//		return;
//	}
//
//	uint32_t location = pCRole->GetbyTaskLocation1() * 10000 + pCRole->GetbyTaskLocation2() * 100 +pCRole->GetbyTaskLocation3();
//	IME_DEBUG("taskindex %u", taskindex);
//	if (location > taskindex)
//	{
//		ProcessMBonus(pCRole, p, q, 0, durationcmd);
//	}
//	else if (location == taskindex)
//	{
//		if (pCRole->GetbyTaskLocation4() >= p->byStepLimit) // have done
//		{
//			ProcessMBonus(pCRole, p, q, 0, durationcmd);
//		}
//		else
//		{
//			ProcessMBonus(pCRole, p, q, 1, durationcmd);
//		}
//	}
//	else
//	{
//		if (location == 0)
//		{
//			if (taskindex == 10101)
//			{
//				ProcessMBonus(pCRole, p, q, 2, durationcmd);
//			}
//			else
//			{
//				IME_ERROR("it doesnt allow accept after current task bigger than 1 task,  task id %u, location %u", taskindex, location);
//				pCRole->SendErrorMsg(ERRNO_MSG_MISSION_OVER_CURRENT, CMD_SC_SIMPLE_MISSION_RESULT);
//				return;
//			}
//		}
//		else
//		{
//			STC_CONF_MISSION_BASE *pp = CConfMissionBase::Find(location);
//			if (pp == NULL)
//			{
//				IME_ERROR("load mission conf fail taskindex %u", location);
//				pCRole->SendErrorMsg(ERRNO_MSG_CSV_LOAD, CMD_SC_SIMPLE_MISSION_RESULT);
//				return;
//			}
//			if (pCRole->GetbyTaskLocation4() >= pp->byStepLimit)
//			{
//				STC_CONF_MISSION_BASE *ptempMission;
//				if (CConfMissionBase::Findp1(location, &ptempMission))
//				{
//					IME_ERROR("current mission is already last one or not alive, %u", location);
//					pCRole->SendErrorMsg(ERRNO_MSG_DATA_ILLEGAL, CMD_SC_SIMPLE_MISSION_RESULT);
//					return;
//				}
//				if (ptempMission->dwIndex != taskindex)
//				{
//					IME_ERROR("it doesnt allow accept after current task bigger than 1 task,  task id %u, location %u, next mission %u", taskindex, location, ptempMission->dwIndex);
//					pCRole->SendErrorMsg(ERRNO_MSG_MISSION_OVER_CURRENT, CMD_SC_SIMPLE_MISSION_RESULT);
//					return;
//				}
//				ProcessMBonus(pCRole, p, q, 2, durationcmd);
//			}
//			else
//			{
//					IME_ERROR("current task is not finish,  step id %u, step limit %u, location %u", pCRole->GetbyTaskLocation4(), pp->byStepLimit, location);
//					pCRole->SendErrorMsg(ERRNO_MSG_MISSION_OVER_CURRENT, CMD_SC_SIMPLE_MISSION_RESULT);
//					return;
//
//			}
//		}
//	}
//
//	return ;
//}		/* -----  end of method CTask::HandlerMissionAccept  ----- */


int CBoss::Init(uint32_t index)
{
	STC_CONF_DURATION_MISSION *pConfMission = CConfDurationMission::Find(index);
	if (pConfMission == NULL)
	{
		IME_ERROR("can not find data in duration mission csv index [%u]", index);
		return -1;
	}

//	STC_CONF_MISSION_BOSS *pConf;	
	STC_CONF_CARD_BASE *pConfCard = CConfCardBase::Find(pConfMission->dwMasterCardIndex);
	if (pConfCard == NULL)
	{
		IME_ERROR("can not find master card record index %u", pConfCard->dwIndex);
		return -1;
	}
	m_strUserName = pConfMission->strName;//pConfCard->strName;
	m_vecline.resize(MAX_CARD_CAPACITY);
	
	SBossCard temp;
	uint32_t initid = 1;
	
	std::vector<uint32_t> vecIndex;
	std::vector<uint16_t> vecLv;
//	std::vector<uint8_t> vecType;
	std::vector<uint32_t> vecSkillIndex;
	std::vector<uint16_t> vecSkillLevel;
	std::vector<uint32_t> vecAttackGrow;
	std::vector<uint32_t> vecDefenceGrow;
	std::vector<uint32_t> vecHealthGrow;
	std::vector<uint8_t> vecBreakCount;

	vecIndex.push_back(pConfMission->dwCardIndex1);
	vecIndex.push_back(pConfMission->dwCardIndex2);
	vecIndex.push_back(pConfMission->dwCardIndex3);
	vecIndex.push_back(pConfMission->dwCardIndex4);
	vecIndex.push_back(pConfMission->dwCardIndex5);
	vecIndex.push_back(pConfMission->dwCardIndex6);

	vecLv.push_back(pConfMission->wLevel1);
	vecLv.push_back(pConfMission->wLevel2);
	vecLv.push_back(pConfMission->wLevel3);
	vecLv.push_back(pConfMission->wLevel4);
	vecLv.push_back(pConfMission->wLevel5);
	vecLv.push_back(pConfMission->wLevel6);

	vecSkillIndex.push_back(pConfMission->dwSkillIndex1);
	vecSkillIndex.push_back(pConfMission->dwSkillIndex2);
	vecSkillIndex.push_back(pConfMission->dwSkillIndex3);
	vecSkillIndex.push_back(pConfMission->dwSkillIndex4);
	vecSkillIndex.push_back(pConfMission->dwSkillIndex5);
	vecSkillIndex.push_back(pConfMission->dwSkillIndex6);

	vecSkillLevel.push_back(pConfMission->wSkillLevel1);
	vecSkillLevel.push_back(pConfMission->wSkillLevel2);
	vecSkillLevel.push_back(pConfMission->wSkillLevel3);
	vecSkillLevel.push_back(pConfMission->wSkillLevel4);
	vecSkillLevel.push_back(pConfMission->wSkillLevel5);
	vecSkillLevel.push_back(pConfMission->wSkillLevel6);

//	vecType.push_back(pConfMission->byType1);
//	vecType.push_back(pConfMission->byType2);
//	vecType.push_back(pConfMission->byType3);
//	vecType.push_back(pConfMission->byType4);
//	vecType.push_back(pConfMission->byType5);
//	vecType.push_back(pConfMission->byType6);

	vecAttackGrow.push_back(pConfMission->dwAttack1);
	vecAttackGrow.push_back(pConfMission->dwAttack2);
	vecAttackGrow.push_back(pConfMission->dwAttack3);
	vecAttackGrow.push_back(pConfMission->dwAttack4);
	vecAttackGrow.push_back(pConfMission->dwAttack5);
	vecAttackGrow.push_back(pConfMission->dwAttack6);

	vecDefenceGrow.push_back(pConfMission->dwDefence1);
	vecDefenceGrow.push_back(pConfMission->dwDefence2);
	vecDefenceGrow.push_back(pConfMission->dwDefence3);
	vecDefenceGrow.push_back(pConfMission->dwDefence4);
	vecDefenceGrow.push_back(pConfMission->dwDefence5);
	vecDefenceGrow.push_back(pConfMission->dwDefence6);

	vecHealthGrow.push_back(pConfMission->dwHealth1);
	vecHealthGrow.push_back(pConfMission->dwHealth2);
	vecHealthGrow.push_back(pConfMission->dwHealth3);
	vecHealthGrow.push_back(pConfMission->dwHealth4);
	vecHealthGrow.push_back(pConfMission->dwHealth5);
	vecHealthGrow.push_back(pConfMission->dwHealth6);

	vecBreakCount.push_back(pConfMission->byBreakCount1);
	vecBreakCount.push_back(pConfMission->byBreakCount2);
	vecBreakCount.push_back(pConfMission->byBreakCount3);
	vecBreakCount.push_back(pConfMission->byBreakCount4);
	vecBreakCount.push_back(pConfMission->byBreakCount5);
	vecBreakCount.push_back(pConfMission->byBreakCount6);

	for (int i = 0; i < MAX_CARD_CAPACITY; i++)
	{
		if(vecIndex[i] != 0)
		{
			CCard Card;
			if (Card.CreateCard(vecIndex[i], vecLv[i], initid++))
			{
				IME_ERROR("create card fail index %u", vecIndex[i]);
				return -1; 
			}
			temp.dwObjId = Card.GetdwObjId();
			temp.dwIndex = Card.GetdwIndex();
			temp.byAttribute = Card.GetbyAttribute();
			temp.dwAttack = vecAttackGrow[i];
//			IME_DEBUG("attack %u vecAttackGrow %u, dwAttack %u", Card.GetdwAttack(), vecAttackGrow[i], temp.dwAttack);
			temp.dwDefence = vecDefenceGrow[i];
			temp.dwHealth = vecHealthGrow[i];
			temp.dwCritRate = Card.GetdwCritRate();
//			temp.dwDodgeRate = Card.GetdwDodgeRate();
//			temp.dwAction = Card.GetdwAction();
//			temp.dwInitSkill = Card.GetdwInitSkillIndex();
			temp.dwBigSkill = vecSkillIndex[i];
			temp.dwBigSkillLv = vecSkillLevel[i];
			if (temp.dwBigSkill == 0)
			{
				if (Card.GetbyType() == E_ITEM_MASTER_CARD)
				{
					temp.dwBigSkill = Card.GetdwMasterSkillIndex();
					temp.dwBigSkillLv = Card.GetwMasterSkillLv();
				}
			}
			temp.dwCardLv = vecLv[i];
			temp.byCardType = E_ITEM_PET_CARD;
			temp.byBreakCount = vecBreakCount[i];

			IME_DEBUG("init fellower card objid %u index %u attribute %u attack %u defence %u health %u critrate %u BigSkill %u BigSkillLv %u cardlv %u cardtype %u", temp.dwObjId, temp.dwIndex, temp.byAttribute, temp.dwAttack, temp.dwDefence, temp.dwHealth, temp.dwCritRate, temp.dwBigSkill, temp.dwBigSkillLv, temp.dwCardLv, temp.byCardType);
			m_vecline[i] = temp.dwObjId;
			m_vec.push_back(temp);
		}	
		else
		{
			m_vecline[i] = 0;
		}
	}

	m_masterid = 0;
	if (pConfMission->dwMasterCardIndex != 0)
	{
		CCard Card;
		if (Card.CreateCard(pConfMission->dwMasterCardIndex, pConfMission->wMasterLevel, initid++))
		{
			IME_ERROR("create card fail index %u", pConfMission->dwMasterCardIndex);
			return -1; 
		}
		temp.dwObjId = Card.GetdwObjId();
		temp.dwIndex = Card.GetdwIndex();
		temp.byAttribute = Card.GetbyAttribute();
		temp.dwAttack = pConfMission->dwMasterAttack;
		temp.dwDefence = pConfMission->dwMasterDefence;
		temp.dwHealth = pConfMission->dwMasterHealth * DURATION_MASTER_HEALTH_PARA;
		temp.dwCritRate = Card.GetdwCritRate();
		temp.dwBigSkill = pConfMission->dwMasterSkillIndex;
		temp.dwBigSkillLv = pConfMission->wMasterSkillLevel;
		if (temp.dwBigSkill == 0)
		{
			if (Card.GetbyType() == E_ITEM_MASTER_CARD)
			{
				temp.dwBigSkill = Card.GetdwMasterSkillIndex();
				temp.dwBigSkillLv = Card.GetwMasterSkillLv();
			}
		}
		temp.dwCardLv = pConfMission->wMasterLevel;
		temp.byCardType = E_ITEM_MASTER_CARD;
		temp.byBreakCount = pConfMission->byMasterBreakCount;
//		IME_DEBUG("NPC master objid %u index %u attack %u defence %u health %u", Card.GetdwObjId(), Card.GetdwIndex(), Card.GetdwAttack(), Card.GetdwDefence(), Card.GetdwHealth());
//
		IME_DEBUG("init fellower card objid %u index %u attribute %u attack %u defence %u health %u critrate %u BigSkill %u BigSkillLv %u cardlv %u cardtype %u", temp.dwObjId, temp.dwIndex, temp.byAttribute, temp.dwAttack, temp.dwDefence, temp.dwHealth, temp.dwCritRate, temp.dwBigSkill, temp.dwBigSkillLv, temp.dwCardLv, temp.byCardType);

		m_vec.push_back(temp);
		m_masterid = temp.dwObjId;	
	}
	m_vec.resize(MAX_CARD_CAPACITY + 1);
	
	if (m_vec.size() > MAX_CARD_CAPACITY + 1)
	{
		IME_ERROR("boss card number %u bigger than max card capacity", m_vec.size());
		return -1;
	}
	
	return 0;
}

int CBoss::PrepareDurationData(std::vector<cardforduration_t> &vec, std::vector<uint32_t> &vecline)
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
		vec[i].BreakCount = m_vec[i].byBreakCount;
//		IME_DEBUG("objid %u attack %u", m_vec[i].dwObjId, m_vec[i].dwAttack);
	}
	CDuration::SetStrUserName(m_strUserName);
	vecline.insert(vecline.begin(), m_masterid);
	return 0;
}

void CBoss::AddTujian(CRole *pCRole)
{
	for (int i = 0; i < m_vec.size(); i++)
	{
		if (m_vec[i].dwIndex != 0)
		{
			pCRole->GetclsTujian()->AddTujian(m_vec[i].dwIndex, E_CR_NORMAL, 0, TUJIAN_HAPPEN);
			pCRole->GetclsTujian()->CardTujianUpdate(m_vec[i].dwIndex, TUJIAN_CARD);
		}
	}
}

struct STC_CONF_RANDOM_EVENT {
	int32 para1;
	int32 para2;
	int32 para3;
	int32 para4;
};

uint32_t CDurationMission::GetMissionPosition()
{
	return m_MissionPosition;
}

CDurationMission::CDurationMission()
{
	m_MissionPosition = 0;
	m_HighMissionPosition = 0;
	m_HighMissionPositionLimit = 0;
}

int CDurationMission::Init (CRole *pCRole)
{
	m_pCRole = pCRole;
	std::string temp;
	ByteBuffer by;
	if (!CDBCtrl::GetUserDurationMission(temp, m_pCRole->GetdwAccountId()))
	{
		IME_ERROR("can not get duration mission data userid %u", m_pCRole->GetdwAccountId());
		return -1;
	}
	by.append(temp.data(), temp.size());
	SeriliazeDB(by);
//	uint32_t now = time(NULL);
//	std::map<uint32_t, DMission_t>::iterator it;
//	for (it = m_mapMission.begin(); it != m_mapMission.end(); it++)
//	{
//		if (!CUtil::IsToday(it->second.FlushTime))
//		{
//			it->second.Number = 0;
//			it->second.FlushTime = now;
//		}
//	}
	OnTimer5s();
	STC_CONF_DURATION_MISSION *pConf = NULL;
	if (m_HighMissionPosition != 0)
	{
		pConf = CConfDurationMission::Find(m_HighMissionPosition);
		if (pConf == NULL)
		{
			IME_ERROR("can not find high mission %u", m_HighMissionPosition);
			m_HighMissionPosition = 0;
		}
	}

	if (m_HighMissionPositionLimit != 0)
	{
		pConf = CConfDurationMission::Find(m_HighMissionPositionLimit);
		if (pConf == NULL)
		{
			IME_ERROR("can not find high mission %u", m_HighMissionPositionLimit);
			m_HighMissionPositionLimit = m_HighMissionPosition;
		}
	}
	
	if (m_mapAllStar.size() == 0 && m_mapMission.size() != 0)
	{
		CConfDurationMission::MapData data = CConfDurationMission::GetAllData();
		CConfDurationMission::MapData::iterator it = data.begin();
		std::vector<uint32_t> FullStar;
		std::vector<uint32_t> FullStarArea;
		for (; it != data.end(); it++)
		{
			if (it->second.byFlag == 0 && it->second.byType == E_MISSION_MAIN)
			{
				FullStar.push_back(it->second.dwIndex);
				FullStarArea.push_back(it->second.dwArea);
			}
		}

		for (int i = 0; i < FullStar.size(); i++)
		{
			bool isOK = true;
			uint32_t targetarea = FullStarArea[i];
			uint32_t targetindex = FullStar[i];
			for (; it != data.end(); it++)
			{
				if (it->second.byFlag == 1 && it->second.byType == E_MISSION_MAIN && it->second.dwArea == targetarea)
				{
					if (m_mapMission.find(it->first) == m_mapMission.end() || m_mapMission[it->first].MaxEva != DURATION_MISSION_FULL_STAR)
					{
						isOK = false;
						break;
					}
				}
			}

			if (isOK)
			{
				m_mapAllStar[targetindex] = ALL_STAR_RECV;
			}
		}
	}	

	m_IsInit = true;
	return 0;
}		/* -----  end of method CDurationMission::Init  ----- */

void
CDurationMission::SaveData ()
{
	if (!m_IsInit)
		return;
	ByteBuffer by;
	DeSeriliazeDB(by);
	std::string temp;
	temp.assign((const char *)by.contents(), by.size());
	CDBCtrl::SaveUserDurationMission(temp, m_pCRole->GetdwAccountId());
	return;
}		/* -----  end of method CDurationMission::SaveData  ----- */

void
CDurationMission::SeriliazeDB (ByteBuffer &by)
{
	if (by.size() == 0)
		return;
	m_mapMission.clear();
	m_mapHighMission.clear();
	uint32_t number;
	by >> m_MissionPosition;
	by >> number;
	DMission_t temp;
	for (int i = 0; i < number; i++)
	{
		by >> temp.Index;
		by >> temp.Number;
		by >> temp.FlushTime;
		by >> temp.MaxEva;
		m_mapMission[temp.Index] = temp;
	}

	BMission_t temp2;
	by >> number;
	for (int i = 0; i < number; i++)
	{
		by >> temp2.dwTaskId;
		by >> temp2.dwTime;
		m_mapBranchMission[temp2.dwTaskId] = temp2;
	}

	DMission_t temp3;
	by >> m_HighMissionPosition;
	by >> m_HighMissionPositionLimit;
	by >> number;
	for (int i = 0; i < number; i++)
	{
		by >> temp3.Index;
		by >> temp3.Number;
		by >> temp3.FlushTime;
		by >> temp3.MaxEva;
		m_mapHighMission[temp3.Index] = temp3;
	}

	by >> number;
	uint32_t key;
   	uint8_t val;
	for (int i = 0; i < number; i++)
	{
		by >> key;
		by >> val;
		m_mapAllStar[key] = val;
	}
	return ;
}		/* -----  end of method CDurationMission::SeriliazeDB  ----- */

void
CDurationMission::DeSeriliazeDB (ByteBuffer &by)
{
	by << m_MissionPosition;
	by << (uint32_t) m_mapMission.size();
	std::map<uint32_t, DMission_t>::iterator it;
	for (it = m_mapMission.begin(); it != m_mapMission.end(); it++)
	{
		by << it->second.Index;
		by << it->second.Number;
		by << it->second.FlushTime;
		by << it->second.MaxEva;
	}
	by << (uint32_t) m_mapBranchMission.size();
	std::map<uint32_t, BMission_t>::iterator it2;
	for (it2 = m_mapBranchMission.begin(); it2 != m_mapBranchMission.end(); it2++)
	{
		by << it2->second.dwTaskId;
		by << it2->second.dwTime;
	}

	by << m_HighMissionPosition;
	by << m_HighMissionPositionLimit;
	by << (uint32_t) m_mapHighMission.size();
//	std::map<uint32_t, DMission_t>::iterator it;
	for (it = m_mapHighMission.begin(); it != m_mapHighMission.end(); it++)
	{
		by << it->second.Index;
		by << it->second.Number;
		by << it->second.FlushTime;
		by << it->second.MaxEva;
	}
	by << (uint32_t) m_mapAllStar.size();
	std::map<uint32_t, uint8_t>::iterator it3;
	for (it3 = m_mapAllStar.begin(); it3 != m_mapAllStar.end(); it3++)
	{
		by << it3->first;
		by << it3->second;
	}
	return ;
}		/* -----  end of method CDurationMission::DeSeriliazeDB  ----- */

void CDurationMission::FlushMissionChallenge(uint32_t index, uint8_t type)
{
	std::map<uint32_t, DMission_t>::iterator it;
	if (type == E_MISSION_MAIN)
	{
		it = m_mapMission.find(index);
		if (it != m_mapMission.end()) 
		{
			it->second.Number = 0;
		}
	}
	
	if (type == E_MISSION_HIGH)
	{
		it = m_mapHighMission.find(index);
		if (it != m_mapHighMission.end())
		{
			it->second.Number = 0;
		}
	}

	if (type == E_MISSION_ACTIVE)
	{
		it = m_mapActiveMission.find(index);
		if (it != m_mapActiveMission.end())
		{
			it->second.Number = 0;
		}
	}
}

void CDurationMission::OnTimer5s()
{
	uint32_t now = time(NULL);
	std::map<uint32_t, DMission_t>::iterator it;
	for (it = m_mapMission.begin(); it != m_mapMission.end(); it++)
	{
		if (!CUtil::IsToday(it->second.FlushTime))
		{
			it->second.Number = 0;
			it->second.FlushTime = now;
		}
	}

	for (it = m_mapHighMission.begin(); it != m_mapHighMission.end(); it++)
	{
		if (!CUtil::IsToday(it->second.FlushTime))
		{
			it->second.Number = 0;
			it->second.FlushTime = now;
		}
	}

	for (it = m_mapActiveMission.begin(); it != m_mapActiveMission.end(); it++)
	{
		if (!CUtil::IsToday(it->second.FlushTime))
		{
			it->second.Number = 0;
			it->second.FlushTime = now;
		}
	}
}

void
CDurationMission::HandlerDurationMissionInfo (uint32_t index)
{
	if (m_mapMission.size() == 0)
		return;
	if (index == 0)
	{
		WorldPacket pkg(CMD_SC_DURATION_MISSION_INFO);
		pkg << m_MissionPosition;
		pkg << (uint32_t) m_mapMission.size();
		std::map<uint32_t, DMission_t>::iterator it;
		for (it = m_mapMission.begin(); it != m_mapMission.end(); it++)
		{
			pkg << it->second.Index;
			pkg << it->second.Number;
			pkg << it->second.FlushTime;
			pkg << it->second.MaxEva;
		}
		m_pCRole->SendPacket(&pkg);
		return ;
	}
	else
	{
		std::map<uint32_t, DMission_t>::iterator it;
		it = m_mapMission.find(index);
		if (it != m_mapMission.end())
		{
			WorldPacket pkg(CMD_SC_DURATION_MISSION_INFO);
			pkg << m_MissionPosition;
			pkg << (uint32_t) 1;
			pkg << it->second.Index;
			pkg << it->second.Number;
			pkg << it->second.FlushTime;
			pkg << it->second.MaxEva;
			m_pCRole->SendPacket(&pkg);
		}	
	}
}		/* -----  end of method CDurationMission::HandlerDurationMissionInfo  ----- */

void
CDurationMission::HandlerHighDurationMissionInfo (uint32_t index)
{
//	if (m_mapHighMission.size() == 0)
//		return;

	if (index == 0)
	{
		WorldPacket pkg(CMD_SC_HIGH_DURATION_MISSION_INFO);
		pkg << m_HighMissionPosition;
		pkg << m_HighMissionPositionLimit;
		pkg << (uint32_t) m_mapHighMission.size();
		std::map<uint32_t, DMission_t>::iterator it;
		for (it = m_mapHighMission.begin(); it != m_mapHighMission.end(); it++)
		{
			pkg << it->second.Index;
			pkg << it->second.Number;
			pkg << it->second.FlushTime;
			pkg << it->second.MaxEva;
		}
		m_pCRole->SendPacket(&pkg);
		return;
	}
	else
	{
		std::map<uint32_t, DMission_t>::iterator it;
		it = m_mapHighMission.find(index);
		if (it != m_mapHighMission.end())
		{
			WorldPacket pkg(CMD_SC_HIGH_DURATION_MISSION_INFO);
			pkg << m_HighMissionPosition;
			pkg << m_HighMissionPositionLimit;
			pkg << (uint32_t) 1;
			pkg << it->second.Index;
			pkg << it->second.Number;
			pkg << it->second.FlushTime;
			pkg << it->second.MaxEva;
			m_pCRole->SendPacket(&pkg);
		}	
		return;
	}
	return;
}		/* -----  end of method CDurationMission::HandlerDurationMissionInfo  ----- */

void CDurationMission::HandlerMissionBranchInfo(uint32_t index)
{
	std::map<uint32_t, BMission_t>::iterator it;

	STC_CONF_DURATION_MISSION *p = NULL;
	
	if (index == 0)
	{
		WorldPacket info(CMD_SC_MISSION_BRANCH_INFO);
		info << (uint16_t) m_mapBranchMission.size();
		for (it = m_mapBranchMission.begin(); it != m_mapBranchMission.end(); it++)
		{
			p = CConfDurationMission::Find(it->second.dwTaskId);
			if (p == NULL)
			{
				IME_ERROR("load mission conf fail taskindex %u", it->second.dwTaskId);
				return;
			}
			info << it->second.dwTaskId;
			uint32_t tmp = 0;
			uint32_t now = time(NULL);
			if (it->second.dwTime + p->dwCD > now)
				tmp = it->second.dwTime + p->dwCD - now;
			info << tmp;
		}
		m_pCRole->SendPacket(&info);
		return;
	}
	else
	{
		it = m_mapBranchMission.find(index);
		if (it != m_mapBranchMission.end())
		{
			WorldPacket info(CMD_SC_MISSION_BRANCH_INFO);
			info << (uint16_t) 1;
			p = CConfDurationMission::Find(it->second.dwTaskId);
			if (p == NULL)
			{
				IME_ERROR("load mission conf fail taskindex %u", it->second.dwTaskId);
				return;
			}
			info << it->second.dwTaskId;
			uint32_t tmp = 0;
			uint32_t now = time(NULL);
			if (it->second.dwTime + p->dwCD > now)
				tmp = it->second.dwTime + p->dwCD - now;
			info << tmp;
			m_pCRole->SendPacket(&info);
			return;
		}
	}
}

void CDurationMission::HandlerActiveDurationMissionInfo(uint32_t index)
{
	if (index == 0)
	{
		WorldPacket pkg(CMD_SC_ACTIVE_DURATION_MISSION_ACTIVE_INFO);
		pkg << (uint32_t) m_mapActiveMission.size();
		std::map<uint32_t, DMission_t>::iterator it;
		for (it = m_mapActiveMission.begin(); it != m_mapActiveMission.end(); it++)
		{
			pkg << it->second.Index;
			pkg << it->second.Number;
			pkg << it->second.FlushTime;
			pkg << it->second.MaxEva;
		}
		m_pCRole->SendPacket(&pkg);
		return;
	}
	else
	{
		std::map<uint32_t, DMission_t>::iterator it;
		it = m_mapActiveMission.find(index);
		if (it != m_mapActiveMission.end())
		{
			WorldPacket pkg(CMD_SC_ACTIVE_DURATION_MISSION_ACTIVE_INFO);
			pkg << (uint32_t) 1;
			pkg << it->second.Index;
			pkg << it->second.Number;
			pkg << it->second.FlushTime;
			pkg << it->second.MaxEva;
			m_pCRole->SendPacket(&pkg);
		}	
		return;
	}
}

void CDurationMission::HandlerMissionBranchAccept(WorldPacket &pkg)
{
//	uint32_t taskindex;
//	pkg >> taskindex;
//
//	STC_CONF_DURATION_MISSION *p = CConfDurationMission::Find(taskindex);
//	if (p == NULL)
//	{
//		IME_ERROR("load mission conf fail taskindex %u", taskindex);
//		m_pCRole->SendErrorMsg(ERRNO_MSG_CSV_LOAD, CMD_SC_MISSION_BRANCH_ACCEPT_RESULT);
//		return;
//	}
//
//	STC_CONF_RANDOM_EVENT *q = NULL;
//	if (p->dwRandomIndex != 0)
//	{
//		q = CConfRandomEvent::Find(p->dwRandomIndex);
//		if (q == NULL)
//		{
//			IME_ERROR("load random conf fail index %u", p->dwRandomIndex);
//			m_pCRole->SendErrorMsg(ERRNO_MSG_CSV_LOAD, CMD_SC_MISSION_BRANCH_ACCEPT_RESULT);
//			return;
//
//		}	
//	}
//
//	uint32_t tempenergy = m_pCRole->GetdwEnergy();
//	if (m_pCRole->GetdwEnergy() < p->wEnergyConsume)
//	{
//		m_pCRole->SendEnergyNotEnough(CMD_SC_MISSION_BRANCH_ACCEPT_RESULT);
//		return;
//	}
//
//	STC_CONF_RANDOM_EVENT *qBoss = NULL;
//	if (p->dwBossRandomIndex != 0)
//	{
//		qBoss = CConfRandomEvent::Find(p->dwBossRandomIndex);
//		if (qBoss == NULL)
//		{
//			IME_ERROR("load random conf fail index %u", p->dwBossRandomIndex);
//			m_pCRole->SendErrorMsg(ERRNO_MSG_CSV_LOAD, CMD_SC_MISSION_BRANCH_ACCEPT_RESULT);
//			return;
//
//		}	
//	}
//
//	if (p->dwRandomIndex != 0)
//	{
//		if (q->wType == CRandomEvent::CARD_DROP)
//		{
//			if(m_pCRole->GetclsCardPacket()->IsFull())
//			{
//				IME_ERROR("user %u card packet already full", m_pCRole->GetdwAccountId());
//				m_pCRole->SendErrorMsg(ERRNO_MSG_CARD_PACKET_NOT_ENOUGH, CMD_SC_MISSION_BRANCH_ACCEPT_RESULT);
//				return;
//			}
//		}
//	}
////	if (p->dwBoss)
////	{
//		if(m_pCRole->GetclsCardPacket()->IsFull(2))
//		{
//			IME_ERROR("user %u card packet already full", m_pCRole->GetdwAccountId());
//			m_pCRole->SendErrorMsg(ERRNO_MSG_CARD_PACKET_NOT_ENOUGH, CMD_SC_MISSION_BRANCH_ACCEPT_RESULT);
//			return;
//		}
////	}
//
//	if (p->byType != E_MISSION_BRANCH)
//	{
//		IME_ERROR("this mission taskindex %u then type %u", taskindex, p->byType);
//		m_pCRole->SendErrorMsg(ERRNO_MSG_MISSION_TYPE_UNVALID, CMD_SC_MISSION_BRANCH_ACCEPT_RESULT);
//		return;
//	}
//	if (m_pCRole->GetwLevel() < p->wLevelLimit)
//	{
//		IME_ERROR("this mission taskindex %u then levellimit %u, now role level", taskindex, p->wLevelLimit, m_pCRole->GetwLevel());
//		m_pCRole->SendErrorMsg(ERRNO_MSG_MISSION_LEVEL_NOT_ARRIVE, CMD_SC_MISSION_BRANCH_ACCEPT_RESULT);
//		return;
//	}
//	
//	if (m_mapBranchMission.find(taskindex) == m_mapBranchMission.end())
//	{
//		BMission_t temp;
//		temp.dwTaskId = taskindex;
//		temp.dwTime = 0;	
//		m_mapBranchMission[taskindex] = temp;
//	}
//
//	uint32_t mytime;
//	mytime = m_mapBranchMission[taskindex].dwTime;
//
//	if (mytime + p->dwCD > time(NULL))
//	{
//		m_pCRole->SendErrorMsg(ERRNO_MSG_MISSION_IN_CD, CMD_SC_MISSION_BRANCH_ACCEPT_RESULT);
//		return;
//	}
//
//	CBoss tmpNPC;
//	if (tmpNPC.Init(p->dwIndex))
//	{
//		IME_ERROR("NPC init fail %u", p->dwIndex);
//		m_pCRole->SendErrorMsg(ERRNO_MSG_CSV_LOAD, CMD_SC_MISSION_BRANCH_ACCEPT_RESULT);
//		return;
//	}
//
//	WorldPacket Dpkg(CMD_SC_DURATION_PKG);
//	uint8_t eva;
//	int ret = DurationJudgeWin(m_pCRole, &tmpNPC, Dpkg, eva); 
//	if (ret == -1)
//	{
//		m_pCRole->SendErrorMsg(ERRNO_MSG_CSV_LOAD, CMD_SC_MISSION_BRANCH_ACCEPT_RESULT);
//		return;
//	}
//
//	uint32_t len = Dpkg.size();
//	const char *pStart = (const char *)Dpkg.contents();
//	uint32_t number;
//	while(len)
//	{
//		WorldPacket DpkgSplit(CMD_SC_DURATION_PKG);
//		if (len > PACKET_SPLIT_NUMBER)
//		{
//			number = PACKET_SPLIT_NUMBER;
//			len -= PACKET_SPLIT_NUMBER;
//		}
//		else
//		{
//			number = len;
//			len = 0;
//		}
//		
//		DpkgSplit.append(pStart, number);
//		pStart += number;
//		m_pCRole->SendPacket(&DpkgSplit);
//	}
//
//	STC_CONF_RANDOM_EVENT retemp;
//	STC_CONF_RANDOM_EVENT retempBoss;
//	retemp.para1 = 0;
//	retemp.para2 = 0;
//	retemp.para3 = 0;
//	retemp.para4 = 0;
//	retempBoss.para1 = 0;
//	retempBoss.para2 = 0;
//	retempBoss.para3 = 0;
//	retempBoss.para4 = 0;
//
//	uint32_t tcoin = 0;
//	uint32_t texp = 0;
//	uint32_t tcardexp = 0;
//	uint16_t status = 0;
//	uint32_t exp = m_pCRole->GetdwExp();
//	uint32_t lastexplimit = m_pCRole->GetdwLastExplimit();
//	uint16_t level = m_pCRole->GetwLevel();
//	uint32_t explimit = m_pCRole->GetdwExplimit();
//
//	if (ret == 0) // win
//	{
//
////		if (p->dwBoss)
////		{
//			if (qBoss != NULL)
//			{
//				IME_DEBUG("mission event %u, random type %u", p->dwBossRandomIndex, qBoss->wType);
//				if (CUtil::RandEither(p->dwBossRandomRate, 100)) // random event happen
//				{
//					memcpy(&retempBoss, qBoss, sizeof(STC_CONF_RANDOM_EVENT));
//					CRandomEvent::initialization();
//					CRandomEvent::REventFunc HandlerRandomEventFunc;
//					HandlerRandomEventFunc = CRandomEvent::Find(qBoss->wType);
//					if (HandlerRandomEventFunc == NULL)
//					{
//						IME_ERROR("no this random type %u", qBoss->wType);
//						m_pCRole->SendErrorMsg(ERRNO_MSG_CSV_DATA, CMD_SC_MISSION_BRANCH_ACCEPT_RESULT);
//						return;
//					}
//					else
//					{
//						if (HandlerRandomEventFunc(m_pCRole, retempBoss.para1, retempBoss.para2, retempBoss.para3, retempBoss.para4))
//						{
//							IME_ERROR("random handler fail");
//							m_pCRole->SendErrorMsg(ERRNO_MSG_CSV_DATA, CMD_SC_MISSION_BRANCH_ACCEPT_RESULT);
//							return;
//						}
//					}
//				}	
//			}
//
////		}
//
//		if (q != NULL)
//		{
//			IME_DEBUG("mission event %u, random type %u", p->dwRandomIndex, q->wType);
//			if (CUtil::RandEither(p->dwRandomRate, 100)) // random event happen
//			{
//				memcpy(&retemp, q, sizeof(STC_CONF_RANDOM_EVENT));
//				CRandomEvent::initialization();
//				CRandomEvent::REventFunc HandlerRandomEventFunc;
//				HandlerRandomEventFunc = CRandomEvent::Find(q->wType);
//				if (HandlerRandomEventFunc == NULL)
//				{
//					IME_ERROR("no this random type %u", q->wType);
//					m_pCRole->SendErrorMsg(ERRNO_MSG_CSV_DATA, CMD_SC_MISSION_BRANCH_ACCEPT_RESULT);
//					return;
//				}
//				else
//				{
//					if (HandlerRandomEventFunc(m_pCRole, retemp.para1, retemp.para2, retemp.para3, retemp.para4))
//					{
//						IME_ERROR("random handler fail");
//						m_pCRole->SendErrorMsg(ERRNO_MSG_CSV_DATA, CMD_SC_MISSION_BRANCH_ACCEPT_RESULT);
//						return;
//					}
//				}
//			}	
//		}
//		
//		
//		m_pCRole->ChangeEnergy(-(p->wEnergyConsume));
//
//		tcoin = p->dwCoinUp;
//		texp = p->dwExpUp;
//		tcardexp = p->dwCardExpUp;
//
//		m_pCRole->GetclsTech()->TechCoin(tcoin);
//		m_pCRole->GetclsTech()->TechExp(texp);
//
//		m_pCRole->ChangeCoin(tcoin);
//		m_pCRole->AddExp(texp);
//		m_pCRole->CalExp(true);
//
//		HandlerMissionBranchInfo(taskindex);	
//
//	}
//	else //lose 
//	{
//		m_pCRole->ChangeEnergy(-(p->wEnergyConsume));
//		m_pCRole->SendProUpdate();
//		status = ERRNO_MSG_MISSION_DURATION_LOSE;
//
//		memset(&retemp, 0, sizeof(STC_CONF_RANDOM_EVENT));
//		memset(&retempBoss, 0, sizeof(STC_CONF_RANDOM_EVENT));
//	}
//
//	std::vector<CCard *> vec;
//	if (m_pCRole->GetclsCardPacket()->GetEquippCards(vec))
//	{
//		IME_ERROR("get card property fail, userid %u", m_pCRole->GetdwAccountId());
//		m_pCRole->SendErrorMsg(ERRNO_MSG_CSV_DATA, CMD_SC_MISSION_BRANCH_ACCEPT_RESULT);
//		return;
//	}
//
//	WorldPacket info(CMD_SC_MISSION_BRANCH_ACCEPT_RESULT);
//	info << (uint16_t) status;
//	info << (uint32_t) taskindex;
//	info << exp;
//	info << lastexplimit;
//	info << explimit;
//	info << level;
//	info << m_pCRole->GetdwExp();
//	info << m_pCRole->GetdwLastExplimit();
//	info << m_pCRole->GetdwExplimit();
//	info << m_pCRole->GetwLevel();
//	info << tcoin;
//	info << texp;
//
//	info << (uint32_t) vec.size(); 	
//
//	for (int i = 0; i < vec.size(); i++)
//	{
//		info << vec[i]->GetdwObjId();
//		info << vec[i]->GetwLevel();
//		info << vec[i]->GetdwExp();
//		info << vec[i]->GetdwLastExpLimit();
//		info << vec[i]->GetdwExpLimit();
//		if (tcardexp != 0)
//		{
//			vec[i]->AddExp(tcardexp);
//			vec[i]->Calculate(true);
//		}
//		info << vec[i]->GetwLevel();
//		info << vec[i]->GetdwExp();
//		info << vec[i]->GetdwLastExpLimit();
//		info << vec[i]->GetdwExpLimit();
//	}
//
//	info << retemp.para1;
//	info << retemp.para2;
//	info << retemp.para3;
//	info << retemp.para4;
//	info << retempBoss.para1;
//	info << retempBoss.para2;
//	info << retempBoss.para3;
//	info << retempBoss.para4;
//
//	m_pCRole->SendPacket(&info);
//	return;
}

void CDurationMission::HandlerDurationAccept(WorldPacket &pkg)
{
	uint32_t taskindex;
	uint32_t joinuserid;
	pkg >> taskindex;
	pkg >> joinuserid;
	IME_CHAR_LOG("action [userid %u enter task %u join userid %u]", m_pCRole->GetdwAccountId(), taskindex, joinuserid);

	uint8_t IsSkip = 0;
	uint8_t IsBoss = 0;
	uint8_t IsStartBoss = 0;

	STC_CONF_DURATION_MISSION *p = CConfDurationMission::Find(taskindex);
	if (p == NULL)
	{
		IME_ERROR("load mission conf fail taskindex %u", taskindex);
		m_pCRole->SendErrorMsg(ERRNO_MSG_CSV_LOAD, CMD_SC_DURATION_MISSION_ACCEPT_RESULT);
		return;
	}
	if (p->dwHighMission != 0 && CConfDurationMission::Find(p->dwHighMission) == NULL)
	{
		IME_ERROR("load high mission limit conf fail taskindex %u", p->dwIndex);
		m_pCRole->SendErrorMsg(ERRNO_MSG_CSV_LOAD, CMD_SC_DURATION_MISSION_ACCEPT_RESULT);
		return;
	}

	if (!p->byJoin && joinuserid != 0)
	{
		m_pCRole->SendErrorMsg(ERRNO_MSG_JOIN_UNALLOW, CMD_SC_DURATION_MISSION_ACCEPT_RESULT);
		return;
	}

	std::map<uint32_t, uint8_t> &mapJoin = m_pCRole->GetMapJoin();
//	IME_ERROR("================join user %u", mapJoin.size());
	if (joinuserid != 0)
	{
		if (mapJoin.find(joinuserid) != mapJoin.end() && mapJoin[joinuserid] >= JOIN_EVERY_COUNT)
		{
			IME_ERROR("fellow used overload max count %u", joinuserid);
			m_pCRole->SendErrorMsg(ERRNO_MSG_ALREADY_OVERLOAD_MAX, CMD_SC_DURATION_MISSION_ACCEPT_RESULT);
			return;
		}

		uint16_t maxnumber = 0;
		STC_CONF_VIP *pVipConf = CConfVip::Find(m_pCRole->GetwVipLv());
		if (pVipConf == NULL)
		{
			IME_ERROR("vip lv %u can not find in vip csv", m_pCRole->GetwVipLv());
		}
		else
		{
			maxnumber = pVipConf->dwJoinMax;
		}
		if (m_pCRole->GetTotalJoin() >= maxnumber)
		{
			m_pCRole->SendErrorMsg(ERRNO_MSG_ALREADY_OVERLOAD_MAX, CMD_SC_DURATION_MISSION_ACCEPT_RESULT);
			return;
		}
	}

//	STC_CONF_RANDOM_EVENT *q = NULL;
//	if (p->dwRandomIndex != 0)
//	{
//		q = CConfRandomEvent::Find(p->dwRandomIndex);
//		if (q == NULL)
//		{
//			IME_ERROR("load random conf fail index %u", p->dwRandomIndex);
//			m_pCRole->SendErrorMsg(ERRNO_MSG_CSV_LOAD, CMD_SC_DURATION_MISSION_ACCEPT_RESULT);
//			return;
//
//		}	
//	}
	
//	if (p->byType != E_MISSION_MAIN && p->byType != E_MISSION_HIGH)
//	{
//		IME_ERROR("request mission is not main and high, mission type %u", p->byType);
//		m_pCRole->SendErrorMsg(ERRNO_MSG_DATA_ILLEGAL, CMD_SC_DURATION_MISSION_ACCEPT_RESULT);
//		return;
//	}

	if (p->byType == E_MISSION_MAIN)
	{
		IME_DEBUG("now %u, requset %u", m_MissionPosition, taskindex);
		if (m_MissionPosition == 0)
		{
			if (taskindex != INIT_DURATION_MISSION)
			{
				IME_ERROR("request mission index %u", taskindex, m_MissionPosition);
				m_pCRole->SendErrorMsg(ERRNO_MSG_DURATION_MISSION_OVER_CURRENT, CMD_SC_DURATION_MISSION_ACCEPT_RESULT);
				return;
			}
		}
		else
		{
			STC_CONF_DURATION_MISSION *q = CConfDurationMission::Find(m_MissionPosition);
			if (q == NULL)
			{
				IME_ERROR("load mission conf fail taskindex %u", m_MissionPosition);
				m_pCRole->SendErrorMsg(ERRNO_MSG_CSV_LOAD, CMD_SC_DURATION_MISSION_ACCEPT_RESULT);
				return;
			}
			if (taskindex > q->dwNextTaskIndex && q->dwNextTaskIndex != 0)
			{
				IME_ERROR("request mission index %u over current postion %u", taskindex, m_MissionPosition);
				m_pCRole->SendErrorMsg(ERRNO_MSG_DURATION_MISSION_OVER_CURRENT, CMD_SC_DURATION_MISSION_ACCEPT_RESULT);
				return;
			}
		}

		if (taskindex <= m_MissionPosition)
		{
			IsSkip = 1;
		}

		if (taskindex > m_MissionPosition && taskindex == INIT_WORLDBOSS_TASK)
		{
			IsBoss = 1;
		}
	}
	else if (p->byType == E_MISSION_HIGH)
	{
		IME_DEBUG("branch now %u, requset %u", m_HighMissionPosition, taskindex);
		if (m_HighMissionPosition == 0)
		{
			if (taskindex != INIT_HIGH_DURATION_MISSION)
			{
				IME_ERROR("request mission index %u", taskindex);
				m_pCRole->SendErrorMsg(ERRNO_MSG_DURATION_MISSION_OVER_CURRENT, CMD_SC_DURATION_MISSION_ACCEPT_RESULT);
				return;
			}
		}
		else
		{
			STC_CONF_DURATION_MISSION *q = CConfDurationMission::Find(m_HighMissionPosition);
			if (q == NULL)
			{
				IME_ERROR("load mission conf fail taskindex %u", m_HighMissionPosition);
				m_pCRole->SendErrorMsg(ERRNO_MSG_CSV_LOAD, CMD_SC_DURATION_MISSION_ACCEPT_RESULT);
				return;
			}
			if (taskindex > q->dwNextTaskIndex && q->dwNextTaskIndex != 0)
			{
				IME_ERROR("request mission index %u over current postion %u, next mission %u", taskindex, m_HighMissionPosition, q->dwNextTaskIndex);
				m_pCRole->SendErrorMsg(ERRNO_MSG_DURATION_MISSION_OVER_CURRENT, CMD_SC_DURATION_MISSION_ACCEPT_RESULT);
				return;
			}
		}

		if (taskindex > m_HighMissionPositionLimit)
		{
			IME_ERROR("request mission index %u over current postion %u", taskindex, m_HighMissionPositionLimit);
			m_pCRole->SendErrorMsg(ERRNO_MSG_DURATION_MISSION_OVER_CURRENT, CMD_SC_DURATION_MISSION_ACCEPT_RESULT);
			return;
		}

		if (taskindex <= m_HighMissionPositionLimit)
		{
			IsSkip = 1;
		}
	}
	else if (p->byType == E_MISSION_ACTIVE)
	{
		IME_DEBUG("active now %u", taskindex);
		if (!(CActiveCtrl::TaskCheck(taskindex)) )
		{
			IME_ERROR("active mission can not start index %u", taskindex);
			m_pCRole->SendErrorMsg(ERRNO_MSG_ACTIVE_NOT_OPEN, CMD_SC_DURATION_MISSION_ACCEPT_RESULT);
			return;
		}
	}
	else
	{
		IME_ERROR("request mission is not main and high, mission type %u", p->byType);
		m_pCRole->SendErrorMsg(ERRNO_MSG_DATA_ILLEGAL, CMD_SC_DURATION_MISSION_ACCEPT_RESULT);
		return;
	}

	if (m_MissionPosition >= INIT_WORLDBOSS_TASK)
	{
		IsStartBoss = 1;
	}

	if (m_pCRole->GetwVipLv() >= 4)
	{
		IsSkip = 1;
	}


	uint32_t tempenergy = m_pCRole->GetdwEnergy();
	if (m_pCRole->GetdwEnergy() < p->wEnergyConsume)
	{
		//todo something about resume
		IME_ERROR("energy not enough taskindex %u", taskindex);
		m_pCRole->SendEnergyNotEnough(CMD_SC_DURATION_MISSION_ACCEPT_RESULT);
		return;
	}

//	STC_CONF_RANDOM_EVENT *qBoss = NULL;
	std::vector<int> vecTmp;
	std::vector<int> vecW;
	std::vector<int> vecRandomType;
	if (p->dwBossRandomIndex1 != 0)
	{
		vecTmp.push_back(p->dwBossRandomIndex1);
		vecW.push_back(GlobalConfig::DMissionW1);
		vecRandomType.push_back(E_OBJ_COL_COIN);
	}
	if (p->dwBossRandomIndex2 != 0)
	{
		vecTmp.push_back(p->dwBossRandomIndex2);
		vecW.push_back(GlobalConfig::DMissionW2);
		vecRandomType.push_back(E_OBJ_COL_CARD_EXP);
	}
	if (p->dwBossRandomIndex3 != 0)
	{
		vecTmp.push_back(p->dwBossRandomIndex3);
		vecW.push_back(GlobalConfig::DMissionW3);
		vecRandomType.push_back(E_OBJ_COL_WORLD_EXP);
	}
	if (p->dwBossRandomIndex4 != 0)
	{
		vecTmp.push_back(p->dwBossRandomIndex4);
		vecW.push_back(GlobalConfig::DMissionW4);
		vecRandomType.push_back(E_OBJ_COL_SHOP);
	}
	if (p->dwBossRandomIndex5 != 0)
	{
		vecTmp.push_back(p->dwBossRandomIndex5);
		vecW.push_back(GlobalConfig::DMissionW5);
		vecRandomType.push_back(E_OBJ_COL_ITEM);
	}
	if (p->dwBossRandomIndex6 != 0)
	{
		vecTmp.push_back(p->dwBossRandomIndex6);
		vecW.push_back(GlobalConfig::DMissionW6);
		vecRandomType.push_back(E_OBJ_COL_CARD_PLUS);
	}

	uint32_t randomret = CUtil::RandFactor(vecW);
//	if (randomret < vecTmp.size())
//	{
//		qBoss = CConfRandomEvent::Find(vecTmp[randomret]);
//		if (qBoss == NULL)
//		{
//			IME_ERROR("load random conf fail index %u", vecTmp[randomret]);
//			m_pCRole->SendErrorMsg(ERRNO_MSG_CSV_LOAD, CMD_SC_DURATION_MISSION_ACCEPT_RESULT);
//			return;
//		}	
//	}

//	if (p->dwRandomIndex != 0)
//	{
//		if (q->wType == CRandomEvent::CARD_DROP)
//		{
//			if(m_pCRole->GetclsCardPacket()->IsFull())
//			{
//				IME_ERROR("user %u card packet already full", m_pCRole->GetdwAccountId());
//				m_pCRole->SendErrorMsg(ERRNO_MSG_CARD_PACKET_NOT_ENOUGH, CMD_SC_DURATION_MISSION_ACCEPT_RESULT);
//				return;
//			}
//		}
//	}
//	if (p->dwBoss)
//	{
		if(m_pCRole->GetclsCardPacket()->IsFull(2, 0))
		{
			IME_ERROR("user %u card packet already full", m_pCRole->GetdwAccountId());
			m_pCRole->SendErrorMsg(ERRNO_MSG_CARD_PACKET_NOT_ENOUGH, CMD_SC_DURATION_MISSION_ACCEPT_RESULT);
			return;
		}
//	}

		if (p->byType == E_MISSION_MAIN)
		{
			if (m_mapMission.find(taskindex) != m_mapMission.end())
			{
				if (m_mapMission[taskindex].Number >= p->wChallengeNumber)
				{
					IME_ERROR("mission %u challeng number %u is full", p->wChallengeNumber);
					//					m_pCRole->SendErrorMsg(ERRNO_MSG_DURATION_MISSION_OVER, CMD_SC_DURATION_MISSION_ACCEPT_RESULT);
					WorldPacket info(CMD_SC_DURATION_MISSION_ACCEPT_RESULT);
					info << (uint16_t) ERRNO_MSG_DURATION_MISSION_OVER;
					info << (uint32_t) taskindex;
					info << (uint8_t) p->byType;
					m_pCRole->SendPacket(&info);
					return;
				}
			}
			else
			{
				m_mapMission[taskindex].Index = taskindex;
				m_mapMission[taskindex].Number = 0;
				m_mapMission[taskindex].FlushTime = time(NULL);
				m_mapMission[taskindex].MaxEva = 0; 
			}

			IME_DEBUG("taskindex %u star %u", taskindex, m_mapMission[taskindex].MaxEva);
//			if(m_mapMission[taskindex].MaxEva >= DURATION_MISSION_FULL_STAR)
//			{
//				IsSkip = 1;
//			}	

		}
		else if (p->byType == E_MISSION_HIGH)
		{
			if (m_mapHighMission.find(taskindex) != m_mapHighMission.end())
			{
				if (m_mapHighMission[taskindex].Number >= p->wChallengeNumber)
				{
					IME_ERROR("mission %u challeng number %u is full", p->wChallengeNumber);
					WorldPacket info(CMD_SC_DURATION_MISSION_ACCEPT_RESULT);
					info << (uint16_t) ERRNO_MSG_DURATION_MISSION_OVER;
					info << (uint32_t) taskindex;
					info << (uint8_t) p->byType;
					m_pCRole->SendPacket(&info);
					return;
				}
			}
			else
			{
				m_mapHighMission[taskindex].Index = taskindex;
				m_mapHighMission[taskindex].Number = 0;
				m_mapHighMission[taskindex].FlushTime = time(NULL);
				m_mapHighMission[taskindex].MaxEva = 0; 
			}

			IME_DEBUG("taskindex %u star %u", taskindex, m_mapHighMission[taskindex].MaxEva);
//			if(m_mapHighMission[taskindex].MaxEva >= DURATION_MISSION_FULL_STAR)
//			{
//				IsSkip = 1;
//			}	
		}
		else
		{
			if (m_mapActiveMission.find(taskindex) != m_mapActiveMission.end())
			{
				if (m_mapActiveMission[taskindex].Number >= p->wChallengeNumber)
				{
					IME_ERROR("mission %u challeng number %u is full", p->wChallengeNumber);
					WorldPacket info(CMD_SC_DURATION_MISSION_ACCEPT_RESULT);
					info << (uint16_t) ERRNO_MSG_DURATION_MISSION_OVER;
					info << (uint32_t) taskindex;
					info << (uint8_t) p->byType;
					m_pCRole->SendPacket(&info);
					return;
				}
			}
			else
			{
				m_mapActiveMission[taskindex].Index = taskindex;
				m_mapActiveMission[taskindex].Number = 0;
				m_mapActiveMission[taskindex].FlushTime = time(NULL);
				m_mapActiveMission[taskindex].MaxEva = 0;
			}
		}

	CBoss tmpNPC;
	if (tmpNPC.Init(p->dwIndex))
	{
		IME_ERROR("NPC init fail %u", p->dwIndex);
		m_pCRole->SendErrorMsg(ERRNO_MSG_CSV_LOAD, CMD_SC_DURATION_MISSION_ACCEPT_RESULT);
		return;
	}

	uint8_t eva;
	WorldPacket Dpkg(CMD_SC_DURATION_PKG);
	SDurationIn in;
	in.sword = p->dwSword;
	in.hand = p->dwHand;
	in.internalforce = p->dwInternalForce;
	in.dark = p->dwDark;
	in.stick = p->dwStick;
	
	uint8_t IsOnlineJoin = 0;
	CRole *pJoin;
	CRole temp(NULL);
	if (joinuserid != 0)
	{
		IsOnlineJoin = 1;
		pJoin = sWorld->FindUserRole(joinuserid);
		if (pJoin == NULL)
		{
			IsOnlineJoin = 0;
			pJoin = &temp;
			assert(pJoin != NULL);
			if (!pJoin->LoadDataForDuration(joinuserid))
			{
				IME_ERROR("load peer role from DB fail userid %u", joinuserid);
				m_pCRole->SendErrorMsg(ERRNO_MSG_DB_NO_DATA_OR_ERROR, CMD_SC_DURATION_MISSION_ACCEPT_RESULT);
				return;
			}
		}
	}
	if (joinuserid != 0)
	{
		uint32_t rate = 50;
		if (m_pCRole->IsFriend(joinuserid))
			rate = 100;
		m_pCRole->GetclsCardPacket()->SetMasterEquipForJoin(pJoin->GetclsCardPacket()->CreateMasterEquipForJoin(m_pCRole->GetCurrentMaxObjId() + 1, rate));
	}
	int ret = DurationJudgeWin(m_pCRole, &tmpNPC, Dpkg, eva, in); 
	if (joinuserid != 0)
	{
		m_pCRole->GetclsCardPacket()->SetMasterEquipForJoin(CDuration::cardforduration_t());
	}

	// for test
	// ret = rand() % 2;
	if (ret == -1)
	{
		m_pCRole->SendErrorMsg(ERRNO_MSG_CSV_LOAD, CMD_SC_DURATION_MISSION_ACCEPT_RESULT);
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
	
	if (ret == 0) // win
	{
		STC_CONF_RANDOM_EVENT retemp;
		STC_CONF_RANDOM_EVENT retempBoss;
		retemp.para1 = 0;
		retemp.para2 = 0;
		retemp.para3 = 0;
		retemp.para4 = 0;
		retempBoss.para1 = 0;
		retempBoss.para2 = 0;
		retempBoss.para3 = 0;
		retempBoss.para4 = 0;

		CShopEffect::SGiveProduct givetmp2 = {0};	
		uint32_t min;
		uint32_t max;
		uint32_t para2;

		uint32_t objdroppro1 = p->dwDropRate1 * (100 + m_pCRole->GetExtrObjDrop()) / 100.0;
		uint32_t objdroppro2 = p->dwDropRate2 * (100 + m_pCRole->GetExtrObjDrop()) / 100.0;
		if (CActiveCtrl::IsActiveStart(OBJ_DROP_ACTIVE_ID))
		{
			objdroppro1 = CActiveCtrl::GetMultiObjDrop(taskindex, objdroppro1);
			objdroppro2 = CActiveCtrl::GetMultiObjDrop(taskindex, objdroppro2);
		}

		if (p->dwDropCardIndex1 != 0 && CUtil::RandEither(objdroppro1, 100))
		{
			CShopEffect::GiveGameProduct(m_pCRole, E_OBJ_CARD, p->dwDropCardIndex1, para2, givetmp2, SOURCE_TASK);
		}	
		else
		{
			if (p->dwDropCardIndex2 != 0 && CUtil::RandEither(objdroppro2, 100))
			{
				CShopEffect::GiveGameProduct(m_pCRole, E_OBJ_CARD, p->dwDropCardIndex2, para2, givetmp2, SOURCE_TASK);
			}
		}
		retemp.para1 = givetmp2.effect;
		retemp.para2 = givetmp2.para1;
		retemp.para3 = givetmp2.para2;

		CShopEffect::SGiveProduct fullstargive = {0};	
		if (p->byType == E_MISSION_MAIN)
		{
			if (taskindex > m_MissionPosition)
			{
				m_MissionPosition = taskindex;
			}

			if (m_mapMission[taskindex].MaxEva != DURATION_MISSION_FULL_STAR && eva == DURATION_MISSION_FULL_STAR)
			{
				CShopEffect::GiveGameProduct(m_pCRole, p->byAllStarType, p->dwAllStarPara1, p->dwAllStarPara2, fullstargive, SOURCE_TASK);
				m_pCRole->GetCMission()->WorldThreeStar(0, 1);

			}

		}
		else if (p->byType == E_MISSION_HIGH)
		{
			if (taskindex > m_HighMissionPosition)
			{
				m_HighMissionPosition = taskindex;
			}

			if (m_mapHighMission[taskindex].MaxEva != DURATION_MISSION_FULL_STAR && eva == DURATION_MISSION_FULL_STAR)
			{
				CShopEffect::GiveGameProduct(m_pCRole, p->byAllStarType, p->dwAllStarPara1, p->dwAllStarPara2, fullstargive, SOURCE_TASK);
				m_pCRole->GetCMission()->WorldThreeStar(0, 1);
			}
		}
		else
		{
			if (m_mapActiveMission[taskindex].MaxEva != DURATION_MISSION_FULL_STAR && eva == DURATION_MISSION_FULL_STAR)
			{
				CShopEffect::GiveGameProduct(m_pCRole, p->byAllStarType, p->dwAllStarPara1, p->dwAllStarPara2, fullstargive, SOURCE_TASK);
			}
		}

		IME_DEBUG("boss event pro %u", p->dwBossRandomRate);
		if (CUtil::RandEither(p->dwBossRandomRate * (100 + m_pCRole->GetExtrObjDrop()) / 100.0, 100))
		{
			if (randomret < vecTmp.size())
			{
				CShopEffect::SGiveProduct givetmp = {0};	
				uint32_t min;
				uint32_t max;
				uint32_t para2 = p->dwPlusPro;
				if (vecRandomType[randomret] == E_OBJ_COL_COIN || vecRandomType[randomret] == E_OBJ_COL_CARD_EXP || vecRandomType[randomret] == E_OBJ_COL_WORLD_EXP)
				{
					min = vecTmp[randomret] * (100 - DURATION_MISSION_BOSS_RANDOM_PARA) / 100.0;
					max = vecTmp[randomret] * (100 + DURATION_MISSION_BOSS_RANDOM_PARA) / 100.0;
					CShopEffect::GiveGameProduct(m_pCRole, vecRandomType[randomret], min, max, givetmp, SOURCE_TASK); 
				}
				else
				{
					CShopEffect::GiveGameProduct(m_pCRole, vecRandomType[randomret], vecTmp[randomret], para2, givetmp, SOURCE_TASK);
				}
				retempBoss.para1 = givetmp.effect;
				retempBoss.para2 = givetmp.para1;
				retempBoss.para3 = givetmp.para2;
			}
		}
		
		m_pCRole->ChangeEnergy(-(p->wEnergyConsume));

		uint32_t tcoin = p->dwCoinUp;
		uint32_t texp = p->dwExpUp;

		//计算成功之后增加的卡牌经验
		uint32 cardExpTime;
		uint32_t tcardexp = 0;
		if(p->byType != E_MISSION_ACTIVE){
			cardExpTime= m_pCRole->getMultipleCardExp();//卡牌经验倍数
			tcardexp = p->dwCardExpUp * cardExpTime;
		}else
			tcardexp = p->dwCardExpUp ;

		m_pCRole->GetclsTech()->TechCoin(tcoin);
		m_pCRole->GetclsTech()->TechExp(texp);

		if (p->byType != E_MISSION_ACTIVE)
		{
			if (CActiveCtrl::IsActiveStart(EXP_ADD_ACTIVE_ID))
			{
				tcardexp = CActiveCtrl::GetMultiExp(taskindex, tcardexp);
			}

			if (CActiveCtrl::IsActiveStart(COIN_ADD_ACTIVE_ID))
			{
				tcoin = CActiveCtrl::GetMultiCoin(taskindex, tcoin);
			}
		}

		tcoin = tcoin * (100 + m_pCRole->GetExtrCoin()) / 100.0;
		IME_DEBUG("get total coin %u, extracoin %u", tcoin, m_pCRole->GetExtrCoin());
		int addenergy = m_pCRole->GetExtraEnergy();
		int addstrength = m_pCRole->GetExtraStrength(); 
		IME_DEBUG("get extra energy %u strength %u", addenergy, addstrength);
		m_pCRole->ChangeEnergyNoLimit(addenergy);
		m_pCRole->ChangeStrengthNoLimit(addstrength);

		uint32_t exp = m_pCRole->GetdwExp();
		uint32_t lastexplimit = m_pCRole->GetdwLastExplimit();
		uint16_t level = m_pCRole->GetwLevel();
		uint32_t explimit = m_pCRole->GetdwExplimit();
		m_pCRole->ChangeCoin(tcoin, SOURCE_TASK);
		m_pCRole->AddExp(texp);

		tmpNPC.AddTujian(m_pCRole);

		std::vector<CCard *> vec;
		if (m_pCRole->GetclsCardPacket()->GetEquippCards(vec))
		{
			IME_ERROR("get card property fail, userid %u", m_pCRole->GetdwAccountId());
			m_pCRole->SendErrorMsg(ERRNO_MSG_DATA_ILLEGAL, CMD_SC_DURATION_MISSION_ACCEPT_RESULT);
			return;
		}
		m_pCRole->CalExp(true);
		m_pCRole->HandlerInfoOpt();

		if (p->byType == E_MISSION_MAIN)
		{
			if (taskindex > m_MissionPosition)
			{
				m_MissionPosition = taskindex;
			}

//			if (m_mapMission[taskindex].MaxEva != DURATION_MISSION_FULL_STAR && eva == DURATION_MISSION_FULL_STAR)
//			{
//				CShopEffect::GiveGameProduct(m_pCRole, p->byAllStarType, p->dwAllStarPara1, p->dwAllStarPara2, fullstargive);
//			}
			
//			uint8_t a = 0;
//			if (m_mapMission[taskindex].MaxEva != DURATION_MISSION_FULL_STAR && eva == DURATION_MISSION_FULL_STAR)
//			{
//				a = 1;
//			}

			m_mapMission[taskindex].Number += 1;
			if (m_mapMission[taskindex].MaxEva < eva)
				m_mapMission[taskindex].MaxEva = eva;

//			if (a)
//			{
			STC_CONF_DURATION_MISSION* Chapter = CConfDurationMission::FindChapter(p->dwArea);
			if (Chapter != 0)
			{
				if (m_mapAllStar.find(Chapter->dwIndex) == m_mapAllStar.end())
				{
					m_mapAllStar[Chapter->dwIndex] = ALL_STAR_NO;
				}

				CConfDurationMission::MapData data = CConfDurationMission::GetAllData();
				CConfDurationMission::MapData::iterator it = data.begin();
				bool isOK = true;
				for (; it != data.end(); it++)
				{
					if (it->second.byFlag == 1 && it->second.byType == E_MISSION_MAIN && it->second.dwArea == p->dwArea)
					{
						if (m_mapMission.find(it->first) == m_mapMission.end() || m_mapMission[it->first].MaxEva != DURATION_MISSION_FULL_STAR)
						{
							isOK = false;
							break;
						}
					}
				}
				if (isOK)
				{
					if (m_mapAllStar[Chapter->dwIndex] == ALL_STAR_NO)
					{
						m_mapAllStar[Chapter->dwIndex] = ALL_STAR_RECV;
						HandlerMissionDurationAllStarInfo(0);
					}
				}
			}
//			}

			HandlerDurationMissionInfo(taskindex);	

			m_HighMissionPositionLimit = std::max(m_HighMissionPositionLimit, p->dwHighMission);
			IME_DEBUG("high position limit %u", m_HighMissionPositionLimit);
			HandlerHighDurationMissionInfo();

			m_pCRole->GetCMission()->NormalTask(taskindex, 1);
		}
		else if (p->byType == E_MISSION_HIGH)
		{
			if (taskindex > m_HighMissionPosition)
			{
				m_HighMissionPosition = taskindex;
			}

//			if (m_mapHighMission[taskindex].MaxEva != DURATION_MISSION_FULL_STAR && eva == DURATION_MISSION_FULL_STAR)
//			{
//				CShopEffect::GiveGameProduct(m_pCRole, p->byAllStarType, p->dwAllStarPara1, p->dwAllStarPara2, fullstargive);
//			}

			m_mapHighMission[taskindex].Number += 1;
			if (m_mapHighMission[taskindex].MaxEva < eva)
				m_mapHighMission[taskindex].MaxEva = eva;
			HandlerHighDurationMissionInfo(taskindex);	

			m_pCRole->GetCMission()->HighTask(taskindex, 1);
		}
		else
		{
			m_mapActiveMission[taskindex].Number += 1;
			if (m_mapActiveMission[taskindex].MaxEva < eva)
				m_mapActiveMission[taskindex].MaxEva = eva;
			HandlerActiveDurationMissionInfo(taskindex);	
		}


		// happen boss
		if (m_pCRole->GetwLevel() >= GlobalConfig::FriendBossHappenLevel)
		{
			uint32_t BossIdarr[2],BossProarr[2];
			uint32_t BossId,BossPro;
//			BossIdarr[0] = p->dwFBossId;
			BossIdarr[0] = CActiveCtrl::FriendBossIndex;
			BossIdarr[1] = p->dwStoryBossId;
//			BossProarr[0] = p->dwFBossPro;
			BossProarr[0] = CActiveCtrl::FriendBossPro;
			BossProarr[1] = p->dwStoryBossPro;
			
			for (int j = 0; j < 2; j++)
			{
				BossId = BossIdarr[j];
				BossPro = BossProarr[j];
//				IME_LOG("bossid %u pro %u IsStartBoss %u IsBoss %u", BossId, BossPro, IsStartBoss, IsBoss);

				//					STC_CONF_FRIEND_BOSS *pConfFB = CConfFriendBoss::Find(BossId);
				//					if (pConfFB == NULL)
				//					{
				//						IME_ERROR("can not find friend boss index %u in csv", BossId);
				//						m_pCRole->SendErrorMsg(ERRNO_MSG_CSV_DATA, CMD_SC_DURATION_MISSION_ACCEPT_RESULT);
				//						return;
				//					}
				//
				//					IME_LOG("happen boss pro %u", BossPro);
				//					sWorld->GetWorldBoss()->DataDump();
				if (!IsStartBoss)
				{
					if (IsBoss)
					{
						CFriendBoss *pFb = (CFriendBoss *)(sWorld->GetWorldBoss()->GetData(m_pCRole->GetdwAccountId()));
						if (pFb == NULL)
						{
							IME_ERROR("can not find this user %u friend boss data", m_pCRole->GetdwAccountId());
						}
						else
						{
							if (pFb->GetBossId() == 0)
							{
								pFb->HandlerHappenBoss(m_pCRole, INIT_FRIEND_BOSS_HAPPEN);
								break;
							}
						}
					}
				}
				else
				{
					if (BossId == 0)
						continue;

					STC_CONF_FRIEND_BOSS *pConfFB = CConfFriendBoss::Find(BossId);
					if (pConfFB == NULL)
					{
						IME_ERROR("can not find friend boss index %u in csv", BossId);
						m_pCRole->SendErrorMsg(ERRNO_MSG_CSV_DATA, CMD_SC_DURATION_MISSION_ACCEPT_RESULT);
						return;
					}

					IME_DEBUG("happen boss pro %u", BossPro);

					if (CUtil::RandEither(BossPro, 10000))
					{
						IME_DEBUG("happen boss");
						if (pConfFB->byType == FRIEND_BOSS_STORY)
						{
							IME_DEBUG("story happen boss");
							CFriendBoss *pFb = (CFriendBoss *)(sWorld->GetWorldBoss()->GetData(m_pCRole->GetdwAccountId()));
							if (pFb == NULL)
							{
								IME_ERROR("can not find this user %u friend boss data", m_pCRole->GetdwAccountId());
							}
							else
							{
								IME_DEBUG("now boss index %u", pFb->GetBossId());
								if (pFb->GetBossId() == 0)
								{
									pFb->HandlerHappenBoss(m_pCRole, BossId);
									IME_ERROR("happen boss bossindex %u", BossId);
									break;
								}
							}
						}
						else
						{
							if (CActiveCtrl::IsActiveStart(FRIEND_BOSS_ACTIVE_ID))
							{
								//							if (CActiveCtrl::IsFriendBossActive(p->dwFBossId))
								//							{
								IME_DEBUG("active happen boss");
								CFriendBoss *pFb = (CFriendBoss *)(sWorld->GetWorldBoss()->GetData(m_pCRole->GetdwAccountId()));
								if (pFb == NULL)
								{
									IME_ERROR("can not find this user %u friend boss data", m_pCRole->GetdwAccountId());
								}
								else
								{
									IME_DEBUG("now boss index %u", pFb->GetBossId());
									if (pFb->GetBossId() == 0)
									{
										pFb->HandlerHappenBoss(m_pCRole, BossId);
										IME_ERROR("happen boss bossindex %u", BossId);
										break;
									}
								}
								//							}
							}
						}
					}
				}

				sWorld->GetWorldBoss()->DataDump();
				//					break;
			}

		}

		CMail::SBaseAttr tmpAttr = {0};
		
		if (joinuserid != 0)
		{
			if (m_pCRole->IsFriend(joinuserid))
			{
				tmpAttr.worldhistory = GlobalConfig::JoinWorldHistroy;
			}
			else
			{
				tmpAttr.worldhistory = GlobalConfig::JoinStrangerWorldHistroy;
			}

			if (mapJoin.find(joinuserid) == mapJoin.end())
			{
				if (mapJoin.size() < JOIN_LIMIT_MAX_COUNT)
					mapJoin[joinuserid] = 1;
//				IME_ERROR("join userid %u, val %u", joinuserid, mapJoin[joinuserid]);
			}
			else
			{
				mapJoin[joinuserid]++;
//				IME_ERROR("join userid %u, val %u", joinuserid, mapJoin[joinuserid]);
			}
			m_pCRole->SetTotalJoin(m_pCRole->GetTotalJoin() + 1);

			std::string con = CUtil::StrReplace(GlobalConfig::JoinMailContent1, std::string("%s"), m_pCRole->GetstrUserName()); 
			uint32_t mail_id = 0;
			CDBCtrl::stuMailDetail tmp = {0};
			CMail::AddEmailSysDiy(joinuserid, mail_id, tmp, 0, 0, 0, con, tmpAttr, GlobalConfig::JoinMailTitle);   
			if (IsOnlineJoin)
			{
				CMail::EmailUpdate(pJoin, CMail::ADD, &tmp, 0);
			}
			con = CUtil::StrReplace(GlobalConfig::JoinMailContent2, std::string("%s"), pJoin->GetstrUserName());
			CMail::AddEmailSysDiy(m_pCRole->GetdwAccountId(), mail_id, tmp, 0, 0, 0, con, tmpAttr, GlobalConfig::JoinMailTitle);   
			CMail::EmailUpdate(m_pCRole, CMail::ADD, &tmp, 0);

			WorldPacket parapkg;
			parapkg << joinuserid;
			CFriend::HandlerAddFriend(m_pCRole, parapkg, 1);
		}

		m_pCRole->GetCMission()->EveryWorld(0, 1);

		if (level != m_pCRole->GetwLevel())
		{
			CDataPush push;
			push.HandlerDataPushLevelUp(m_pCRole, level, m_pCRole->GetwLevel());
		}

		WorldPacket info(CMD_SC_DURATION_MISSION_ACCEPT_RESULT);
		info << (uint16_t) 0;
		info << (uint8_t) 0;
		info << (uint32_t) taskindex;
		info << exp;
		info << lastexplimit;
		info << explimit;
		info << level;
		info << m_pCRole->GetdwExp();
		info << m_pCRole->GetdwLastExplimit();
		info << m_pCRole->GetdwExplimit();
		info << m_pCRole->GetwLevel();
		info << texp;
		info << tcoin;

		info << (uint32_t) vec.size(); 	

		for (int i = 0; i < vec.size(); i++)
		{
			info << vec[i]->GetdwObjId();
			info << vec[i]->GetwLevel();
			info << vec[i]->GetdwExp();
			info << vec[i]->GetdwLastExpLimit();
			info << vec[i]->GetdwExpLimit();
			IME_DEBUG("%u flag %u, exp %d", i, vec[i]->GetbyFlag(), vec[i]->GetdwExp());
			if (m_pCRole->GetwLevel() * STRENGTH_LEVEL_LIMIT_PARA > vec[i]->GetwLevel())
			{
//				if (vec[i]->GetbyType() != E_ITEM_MASTER_CARD || joinuserid == 0)
				if (vec[i]->GetdwObjId() != m_pCRole->GetclsCardPacket()->GetMasterObjId() || joinuserid == 0)
				{
					uint32_t tmpcardexp = tcardexp * (100 + m_pCRole->GetExtraExp(vec[i]->GetdwObjId())) / 100.0;
					vec[i]->AddExp(tmpcardexp);
					IME_DEBUG("orign exp %u, extra %u, total %u", tcardexp, m_pCRole->GetExtraExp(vec[i]->GetdwObjId()), tmpcardexp);
					vec[i]->Calculate(true);
				}
			}
			IME_DEBUG("%u flag %u, exp %d", i, vec[i]->GetbyFlag(), vec[i]->GetdwExp());
			info << vec[i]->GetwLevel();
			info << vec[i]->GetdwExp();
			info << vec[i]->GetdwLastExpLimit();
			info << vec[i]->GetdwExpLimit();
		}
		m_pCRole->GetclsCardPacket()->Calculate(true);

		info << retemp.para1;
		info << retemp.para2;
		info << retemp.para3;
		info << retemp.para4;
		info << retempBoss.para1;
		info << retempBoss.para2;
		info << retempBoss.para3;
		info << retempBoss.para4;
		info << (uint8_t)p->byType;
		info << fullstargive.effect;
		info << fullstargive.para1;
		info << fullstargive.para2;
		info << (uint8_t) IsSkip;
		
		m_pCRole->SendPacket(&info);
		return;
	}
	else //lose 
	{
		if (m_pCRole->GetwLevel() >= CONSUME_MAX_LEVEL_CONDITION)
		{
			m_pCRole->ChangeEnergy(-(p->wEnergyConsume));
			m_pCRole->SendProUpdate();
		}

		uint32_t exp = m_pCRole->GetdwExp();
		uint32_t lastexplimit = m_pCRole->GetdwLastExplimit();
		uint16_t level = m_pCRole->GetwLevel();
		uint32_t explimit = m_pCRole->GetdwExplimit();

		std::vector<CCard *> vec;
		if (m_pCRole->GetclsCardPacket()->GetEquippCards(vec))
		{
			IME_ERROR("get card property fail, userid %u", m_pCRole->GetdwAccountId());
			m_pCRole->SendErrorMsg(ERRNO_MSG_CSV_DATA, CMD_SC_DURATION_MISSION_ACCEPT_RESULT);
			return;
		}

		WorldPacket info(CMD_SC_DURATION_MISSION_ACCEPT_RESULT);
		info << (uint16_t) 0;
		info << (uint8_t) 1;
		info << (uint32_t) taskindex;
		info << exp;
		info << lastexplimit;
		info << explimit;
		info << level;
		info << exp;
		info << lastexplimit;
		info << explimit;
		info << level;
		info << (uint32_t)0;
		info << (uint32_t)0;

		info << (uint32_t) vec.size(); 	

		for (int i = 0; i < vec.size(); i++)
		{
			info << vec[i]->GetdwObjId();
			info << vec[i]->GetwLevel();
			info << vec[i]->GetdwExp();
			info << vec[i]->GetdwLastExpLimit();
			info << vec[i]->GetdwExpLimit();
			info << vec[i]->GetwLevel();
			info << vec[i]->GetdwExp();
			info << vec[i]->GetdwLastExpLimit();
			info << vec[i]->GetdwExpLimit();
		}

		info << (uint32_t) 0;
		info << (int32_t) 0;
		info << (int32_t) 0;
		info << (int32_t) 0;
		info << (uint32_t) 0;
		info << (int32_t) 0;
		info << (int32_t) 0;
		info << (int32_t) 0;
		info << (uint8_t)p->byType;
		info << (uint32_t) 0;
		info << (int32_t) 0;
		info << (int32_t) 0;
		info << (uint8_t) IsSkip;
		
		m_pCRole->SendPacket(&info);
		return;
	}
	return;
}

void CDurationMission::DelBranchMissionCD(uint32_t index)
{
	std::map<uint32_t, BMission_t>::iterator it;
	it = m_mapBranchMission.find(index);
	if (it != m_mapBranchMission.end())
	{
		it->second.dwTime = 0;
	}	
}	

void CDurationMission::HandlerMissionDurationAllStarGet(WorldPacket &pkg)
{
	uint32_t taskindex;
	pkg >> taskindex;
	STC_CONF_DURATION_MISSION *p = CConfDurationMission::Find(taskindex);
	if (p == NULL)
	{
		IME_ERROR("can not find duration mission %u", taskindex);
		m_pCRole->SendErrorMsg(ERRNO_MSG_CSV_LOAD, CMD_SC_DURATION_MISSION_ALL_STAR_RESULT);
		return;
	}

	if (m_mapAllStar.find(taskindex) == m_mapAllStar.end() || m_mapAllStar[taskindex] != ALL_STAR_RECV)
	{
		IME_ERROR("this all star prize do not conform the conditions");
		m_pCRole->SendErrorMsg(ERRNO_MSG_ALL_STAR_PRIZE_NOT_CONFORM, CMD_SC_DURATION_MISSION_ALL_STAR_RESULT);
		return;
	}
	
	SProduct duct;
	duct.type = p->byAllStarType;
	duct.para1 = p->dwAllStarPara1;
	duct.para2 = p->dwAllStarPara2;
	std::vector<SProduct> vecDuct;
	vecDuct.push_back(duct);

	if (m_pCRole->GetclsCardPacket()->IsFullSpecial(vecDuct))
	{
		IME_ERROR("card packet is full");
		m_pCRole->SendErrorMsg(ERRNO_MSG_CARD_PACKET_NOT_ENOUGH, CMD_SC_DURATION_MISSION_ALL_STAR_RESULT);
		return;
	}
	if (CShopEffect::IsObjValid(vecDuct))
	{
		IME_ERROR("para is not valid");
		m_pCRole->SendErrorMsg(ERRNO_MSG_CSV_DATA, CMD_SC_DURATION_MISSION_ALL_STAR_RESULT);
		return;
	}

	CConfDurationMission::MapData data = CConfDurationMission::GetAllData();
	CConfDurationMission::MapData::iterator it = data.begin();
	bool isOK = true;
	for (; it != data.end(); it++)
	{
		if (it->second.byFlag == 1 && it->second.byType == E_MISSION_MAIN && it->second.dwArea == p->dwArea)
		{
			if (m_mapMission.find(it->first) == m_mapMission.end() || m_mapMission[it->first].MaxEva != DURATION_MISSION_FULL_STAR)
			{
				isOK = false;
				break;
			}
		}
	}

	if (!isOK)
	{
		IME_ERROR("all three star prize can not meet the condition");
		m_pCRole->SendErrorMsg(ERRNO_MSG_ALL_STAR_PRIZE_NOT_CONFORM, CMD_SC_DURATION_MISSION_ALL_STAR_RESULT);
		return;
	}

	CShopEffect::SGiveProduct ObjEffect = {0};
	CShopEffect::GiveGameProductSpecial(m_pCRole, duct.type, duct.para1, duct.para2, ObjEffect, SOURCE_ALL_STAR);
	std::vector<CShopEffect::SGiveProduct> vect;
	vect.push_back(ObjEffect);
	m_pCRole->SendObjEffect(vect);
	m_mapAllStar[taskindex] =  ALL_STAR_ALREADY_GET;
	HandlerMissionDurationAllStarInfo(taskindex);

	WorldPacket info(CMD_SC_DURATION_MISSION_ALL_STAR_RESULT);
	info << (uint16_t) 0;
	info << (uint32_t) taskindex;
	m_pCRole->SendPacket(&info);
}

void CDurationMission::HandlerMissionDurationAllStarInfo(uint32_t index)
{
	std::map<uint32_t, uint8_t>::iterator it;
	if (index != 0)
	{
		it = m_mapAllStar.find(index);
		if (it != m_mapAllStar.end())
		{
			WorldPacket info(CMD_SC_DURATION_MISSION_ALL_STAR_INFO);
			info << (uint32_t) 1;
			info << it->first;
			info << it->second;
			m_pCRole->SendPacket(&info);
		}
		else
		{
			IME_ERROR("can not find all star mission %u", index);
		}
		return;
	}
	else
	{
		WorldPacket info(CMD_SC_DURATION_MISSION_ALL_STAR_INFO);
		info << (uint32_t)m_mapAllStar.size();
		for (it = m_mapAllStar.begin(); it != m_mapAllStar.end(); it++)
		{
			info << it->first;
			info << it->second;
		}
		m_pCRole->SendPacket(&info);
		return;
	}
	return;
}
