/*
 * =====================================================================================
 *
 *       Filename:  skill.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2012年09月26日 16时57分48秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  imane (), imanecr@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include <stdint.h>
#include <map>
#include "skill.h"
#include "EnumDefines.h"
#include "Role.h"
#include "Log.h"
#include "Card.h"
#include "CardPacket.h"
#include "ConfigData.h"
#include "Combat.h"

std::map<uint16_t, CJewelSkill::JewelSkillFunc> CJewelSkill::m_mapFunc;

void CJewelSkill::Init ()
{
	m_mapFunc[ATTACK] = JewelSkillAttack;
	m_mapFunc[DEFENCE] = JewelSkillDefence;
	m_mapFunc[HEALTH] = JewelSkillHealth;
	m_mapFunc[CRIT_RATE] = JewelSkillCritRate;
	m_mapFunc[DODGE_RATE] = JewelSkillDodgeRate;
//	m_mapFunc[ADD_BUFFER] = JewelSkillAddBuffer;
//	m_mapFunc[REDUCE_BUFFER] = JewelSkillReduceBuffer;
	m_mapFunc[SKILL_PRO] = JewelSkillSkillPro;
	m_mapFunc[SKILL_POWER] = JewelSkillSkillPower;
	return ;
}		/* -----  end of method CJewelSkill::init  ----- */

int CJewelSkill::JewelSkillAttack (CCardPacket::itemequip_t *pItemEquip, const int32_t &para1)
{
	if ( pItemEquip == NULL)
	{
		IME_ERROR("can not find item equip");
		return -1;
	}

	pItemEquip->Attack = pItemEquip->Attack * (100 + para1) / 100.0;

	return 0;
}		/* -----  end of method CJewelSkill::JewelSkillAttack  ----- */

int CJewelSkill::JewelSkillDefence(CCardPacket::itemequip_t *pItemEquip, const int32_t &para1)
{
	if ( pItemEquip == NULL)
	{
		IME_ERROR("can not find item equip");
		return -1;
	}

	IME_DEBUG("defence %u, para %u", pItemEquip->Defence, para1);
	pItemEquip->Defence = pItemEquip->Defence * (100 + para1) / 100.0;

	return 0;
}		/* -----  end of method CJewelSkill::JewelSkillDefence  ----- */

int CJewelSkill::JewelSkillHealth(CCardPacket::itemequip_t *pItemEquip, const int32_t &para1)
{
	if ( pItemEquip == NULL)
	{
		IME_ERROR("can not find item equip");
		return -1;
	}

	pItemEquip->Health = pItemEquip->Health * (100 + para1) / 100.0;

	return 0;
}

int CJewelSkill::JewelSkillCritRate(CCardPacket::itemequip_t *pItemEquip, const int32_t &para1)
{
		if ( pItemEquip == NULL)
		{
			IME_ERROR("can not find item equip");
			return -1;
		}

		pItemEquip->Crit += para1;
		return 0;
}

int CJewelSkill::JewelSkillDodgeRate(CCardPacket::itemequip_t *pItemEquip, const int32_t &para1)
{
	if ( pItemEquip == NULL)
	{
		IME_ERROR("can not find item equip");
		return -1;
	}

	pItemEquip->Dodge += para1;

	return 0;
}		

//int CJewelSkill::JewelSkillAddBuffer(CCardPacket::itemequip_t *pItemEquip, const int32_t &para1)
//{
//	if ( pItemEquip == NULL)
//	{
//		IME_ERROR("can not find item equip");
//		return -1;
//	}
//
//	pItemEquip->AddBuffer = pItemEquip->AddBuffer * (100 + para1) / 100.0;
//
//	return 0;
//}		
//
//int CJewelSkill::JewelSkillReduceBuffer(CCardPacket::itemequip_t *pItemEquip, const int32_t &para1)
//{
//	if ( pItemEquip == NULL)
//	{
//		IME_ERROR("can not find item equip");
//		return -1;
//	}
//
//	pItemEquip->ReduceBuffer = pItemEquip->ReduceBuffer * (100 + para1) / 100.0;
//
//	return 0;
//}		

int CJewelSkill::JewelSkillSkillPro(CCardPacket::itemequip_t *pItemEquip, const int32_t &para1)
{
	if ( pItemEquip == NULL)
	{
		IME_ERROR("can not find item equip");
		return -1;
	}

	pItemEquip->SkillPro += para1;

	return 0;
}		

int CJewelSkill::JewelSkillSkillPower(CCardPacket::itemequip_t *pItemEquip, const int32_t &para1)
{
	if ( pItemEquip == NULL)
	{
		IME_ERROR("can not find item equip");
		return -1;
	}

	pItemEquip->SkillPower += para1;

	return 0;
}		

std::map<uint16_t, CExSkill::ExSkillFunc> CExSkill::m_mapFunc;

void CExSkill::Init()
{
	m_mapFunc[ATTACK] = ExSkillAttack;
	m_mapFunc[DEFENCE] = ExSkillDefence;
	m_mapFunc[HEALTH] = ExSkillHealth;
	m_mapFunc[CRIT_RATE] = ExSkillCritRate;
	m_mapFunc[ADD_HARM] = ExSkillAddHarm;
	m_mapFunc[SKILL_PRO] = ExSkillSkillPro;
	m_mapFunc[ADD_CRIT_HARM] = ExSkillAddCritHarm;
	m_mapFunc[REDUCE_HARM] = ExSkillReduceHarm;

	m_mapFunc[DURATION_EXP] = ExSkillDurationExp;
	m_mapFunc[DURATION_COIN] = ExSkillDurationCoin;
	m_mapFunc[OBJ_DROP] = ExSkillObjDrop;
	m_mapFunc[STRENGTH_RECOVER] = ExSkillStrengthRecover;
	m_mapFunc[ENERGY_RECOVER] = ExSkillEnergyRecover;

	m_mapFunc[SKILL_POWER] = ExSkillSkillPower;
}

int CExSkill::ExecExSkillFunc(uint32_t index, CCardPacket::itemequip_t *p)
{
	STC_CONF_EXSKILL *pConf;
	pConf = CConfExskill::Find(index);
	if (pConf == NULL)
	{
		IME_ERROR("ex skill can not find skill index %u", index);
		return -1;
	}
	ExSkillFunc pFunc = Find(pConf->byType);
	if (pFunc == NULL)
	{
		IME_ERROR("ex skill can not find skill func type %u index %u", pConf->byType, index);
		return -1;
	}
	uint8_t type = pConf->byType;
	return pFunc(p, pConf->dwPara1);
}

int CExSkill::ExecExSkillFuncByRelation(uint32_t cardgroupindex, std::vector<int> vecRelationship, CCardPacket::itemequip_t *p)
{
	for (int i = 0; i < vecRelationship.size(); i++)
	{
		uint32_t index = vecRelationship[i];
		STC_CONF_RELATIONSHIP *pConf;
		pConf = CConfRelationShip::Find(index);
		if (pConf == NULL)
		{
			IME_ERROR("relationship skill can not find skill index %u", index);
			return -1;
		}
		std::vector<uint32_t> vecID;
		if (pConf->dwID1 != 0)
			vecID.push_back(pConf->dwID1);
		if (pConf->dwID2 != 0)
			vecID.push_back(pConf->dwID2);
		if (pConf->dwID3 != 0)
			vecID.push_back(pConf->dwID3);
		if (pConf->dwID4 != 0)
			vecID.push_back(pConf->dwID4);
		if (pConf->dwID5 != 0)
			vecID.push_back(pConf->dwID5);
		if (pConf->dwID6 != 0)
			vecID.push_back(pConf->dwID6);

		if (vecID.size() == 0)
			continue;
		
		int IsIn = 0;
		for (int j = 0; j < vecID.size(); j++)
		{
			if (cardgroupindex == vecID[j])
				IsIn = 1;
		}

		if (!IsIn)
		{
			continue;	
		}
		
		if (pConf->wType == ATTACK || pConf->wType == DEFENCE || pConf->wType == HEALTH || pConf->wType == CRIT_RATE || pConf->wType == ADD_HARM || pConf->wType == SKILL_PRO || pConf->wType == ADD_CRIT_HARM || pConf->wType == DURATION_EXP || pConf->wType == SKILL_POWER)
		{
			ExSkillFunc pFunc = Find(pConf->wType);
			if (pFunc == NULL)
			{
				IME_ERROR("relationship skill can not find skill func type %u index %u", pConf->wType, index);
				return -1;
			}

			if (pFunc(p, pConf->dwPara1))
			{
				IME_ERROR("relationship skill happen error");
				return -1;
			}
		}
		else
		{
			if (cardgroupindex == vecID[0])
			{
				ExSkillFunc pFunc = Find(pConf->wType);
				if (pFunc == NULL)
				{
					IME_ERROR("relationship skill can not find skill func type %u index %u", pConf->wType, index);
					return -1;
				}

				if (pFunc(p, pConf->dwPara1))
				{
					IME_ERROR("relationship skill happen error");
					return -1;
				}
			}
		}
	}
	return 0;
}

int CExSkill::ExSkillAttack(CCardPacket::itemequip_t *p, const int32_t &para1)
{
	p->Attack = p->Attack * (100 + para1) / 100.0;
	return 0;
}
int CExSkill::ExSkillDefence(CCardPacket::itemequip_t *p, const int32_t &para1)
{
	p->Defence = p->Defence * (100 + para1) / 100.0;
	return 0;
}
int CExSkill::ExSkillHealth(CCardPacket::itemequip_t *p, const int32_t &para1)
{
	p->Health = p->Health * (100 + para1) / 100.0;
	return 0;
}
int CExSkill::ExSkillCritRate(CCardPacket::itemequip_t *p, const int32_t &para1)
{
	p->Crit = p->Crit * (100 + para1) / 100.0;
	return 0;
}
int CExSkill::ExSkillAddHarm(CCardPacket::itemequip_t *p, const int32_t &para1)
{
	p->AddHarm += para1;
	return 0;
}
int CExSkill::ExSkillSkillPro(CCardPacket::itemequip_t *p, const int32_t &para1)
{
	p->SkillPro += para1;
	return 0;
}
int CExSkill::ExSkillAddCritHarm(CCardPacket::itemequip_t *p, const int32_t &para1)
{
	p->AddCritHarm += para1;
	return 0;
}
int CExSkill::ExSkillReduceHarm(CCardPacket::itemequip_t *p, const int32_t &para1)
{
	p->ReduceHarm += para1;
	return 0;
}

int CExSkill::ExSkillDurationExp(CCardPacket::itemequip_t *p, const int32_t &para1)
{
	p->DurationExp += para1;
	return 0;
}

int CExSkill::ExSkillDurationCoin(CCardPacket::itemequip_t *p, const int32_t &para1)
{
	p->DurationCoin += para1;
	return 0;
}

int CExSkill::ExSkillObjDrop(CCardPacket::itemequip_t *p, const int32_t &para1)
{
	p->ObjDrop += para1;
	return 0;
}

int CExSkill::ExSkillStrengthRecover(CCardPacket::itemequip_t *p, const int32_t &para1)
{
	p->StrengthRecover += para1;
	return 0;
}

int CExSkill::ExSkillEnergyRecover(CCardPacket::itemequip_t *p, const int32_t &para1)
{
	p->EnergyRecover += para1;
	return 0;
}

int CExSkill::ExSkillSkillPower(CCardPacket::itemequip_t *p, const int32_t &para1)
{
	p->SkillPower += para1;
	return 0;
}

std::map<uint16_t, CCardSkill::CardSkillFunc> CCardSkill::m_mapFuncSkill;
//std::map<uint16_t, CCardSkill::SkillConditionFunc> CCardSkill::m_mapFuncCondition;
//std::map<uint16_t, CCardSkill::BufferEffectFunc> CCardSkill::m_mapFuncBuffer;

void CCardSkill::Init()
{
	m_mapFuncSkill[ATTACK] = CardSkillAttack;
	m_mapFuncSkill[DEFENCE] = CardSkillDefence;

//	m_mapFuncCondition[HEALTH_30] = SkillConditionHealth30;
//	m_mapFuncCondition[HEALTH_40] = SkillConditionHealth40;
//	m_mapFuncCondition[HEALTH_50] = SkillConditionHealth50;
//	m_mapFuncCondition[HEALTH_70] = SkillConditionHealth70;
//	m_mapFuncCondition[HEALTH_100] = SkillConditionHealth100;
//	m_mapFuncCondition[ROUND_1] = SkillConditionRound1;
//	m_mapFuncCondition[ROUND_2] = SkillConditionRound2;
//	m_mapFuncCondition[ROUND_2_DOWN] = SkillConditionRound2Down;
//	m_mapFuncCondition[ROUND_3_DOWN] = SkillConditionRound3Down;
//
//	m_mapFuncBuffer[S_POISON] = BufferEffectPosition;
//	m_mapFuncBuffer[S_ADD_HEALTH] = BufferEffectAddHealth;
//	m_mapFuncBuffer[S_ADD_ATTACK] = BufferEffectAddAttack;
//	m_mapFuncBuffer[S_ADD_DEFENCE] = BufferEffectAddDefence;
//	m_mapFuncBuffer[S_REDUCE_ATTACK] = BufferEffectReduceAttack;
//	m_mapFuncBuffer[S_REDUCE_DEFENCE] = BufferEffectReduceDefence;
}

//int CCardSkill::CardAttack(CDuration *pCRole, CDuration *pPeerCRole, ByteBuffer &by)
//{
//	CDuration::DurationData_t* pCard, *pPeerCard;	
//	pCard = pCRole->GetCardData(pCRole->GetLaunchObjid());
//	if (pCard == NULL)
//		return -1;
//
//	by << (uint8_t) ACTION_ATTACK;
//	by << (uint32_t) pCard->CardObjId;
//	by << (uint8_t) pCard->Team;
//
//	by << (uint32_t) 0;
//	CDuration::BufferData_t *pBufferTemp;
//	pBufferTemp = pCRole->GetBuffer(pCard->CardObjId, S_TARGET);
//	if (pBufferTemp)
//	{
//		by << (uint32_t) pBufferTemp->index;
//	}
//	else
//	{
//		by << (uint32_t) 0;
//	}
//
//	std::vector<uint32_t> vecTarget;
//	GetTargetAttackOne(pCRole, pPeerCRole, vecTarget);
//
//	by << (uint32_t)vecTarget.size();
//	for (int i = 0; i < vecTarget.size(); i++)
//	{
//		uint8_t IsTarget = 0;
//		uint32_t RecognizeIndex = 0;
//		uint8_t IsCrit = 0;
//		uint8_t IsAttr = 0;
//		uint32_t AttackHarm = 0;
//		uint32_t PeerHealth = 0;
//		uint8_t BufferOpt = 0;
//		uint32_t UnvalidIndex = 0;
//		uint32_t SkillOldIndex = 0;
//		uint32_t SkillNewIndex = 0;
//
//		pPeerCard = pPeerCRole->GetCardData(vecTarget[i]);
//		if (pPeerCard == NULL)
//			return -1;
//
//		int HitPro = CARD_NORAML_ATTACK_HIT - pPeerCard->Dodge;
//		if (HitPro < 0 ) HitPro = 0;
//		int CritPro = pCard->Crit;
//		if (CritPro < 0 ) CritPro = 0;
//		uint32_t *pAttrVal = CConfAttributeRestriction::Find(pCard->Attribute, pPeerCard->Attribute);
//		int32_t AttrVal;
//		int32_t CritVal = 100;
//		int SkillHarm = 0;
//
//		if (pAttrVal == NULL)
//		{
//			IME_ERROR("can not find attribure restriction %u vs %u", pCard->Attribute, pPeerCard->Attribute);
//			return -1;
//		}
//		else
//		{
//			AttrVal = *pAttrVal;
//		}
//		IME_ERROR("attribure restriction %u vs %u val %u", pCard->Attribute, pPeerCard->Attribute, AttrVal);
//		if (AttrVal > 100)
//			IsAttr = 1;
//
//		if (CUtil::RandEither(HitPro, 100))
//		{
//			IsTarget = 1;
//			if (CUtil::RandEither(CritPro, 100))
//			{
//				IsCrit = 1;
//				CritVal =  SKILL_CRIT_FACTOR;
//			}	
//			SkillHarm = (int32_t)((pCard->Attack * 1000) / (pPeerCard->Defence + 1000)) * CritVal / 100.0 * AttrVal / 100.0;
//			IME_DEBUG("obj %u team %u -> obj %u team %u harm %d",pCard->CardObjId, pCard->Team, pPeerCard->CardObjId, pPeerCard->Team, SkillHarm);
//			if (SkillHarm < 0)
//				SkillHarm = 0;
//			if (SkillHarm > pPeerCard->Health)
//			{
//				pPeerCard->Health = 0;		
//			}
//			else
//			{
//				pPeerCard->Health -= SkillHarm;
//			}
//
//			AttackHarm = SkillHarm;
//			PeerHealth = pPeerCard->Health;	
//		}
//		else // skill miss
//		{
//			IsTarget = 0;
//		}
//
//		by << vecTarget[i];
//		by << pPeerCRole->GetTeam();
//		by << IsTarget;
//		by << RecognizeIndex;
//		by << IsCrit;
//		by << IsAttr;
//		by << AttackHarm;
//		by << PeerHealth;
//		by << BufferOpt;
//		by << UnvalidIndex;
//		by << SkillOldIndex;
//		by << SkillNewIndex;	
//	}
//	return 0;
//}

int CCardSkill::CardSkillAttack(CDuration *pCRole, CDuration *pPeerCRole, SSkillStu *pConf, ByteBuffer &by, SDurationIn in)
{
	CDuration::DurationData_t *pCard;	
	CDuration::cardforduration_t *pCardBase;
	if (pConf == NULL)
	{
		return -1;
	}

	uint32_t skillindex = 0;
	int CritVal = 100;
	int AttrVal = 100;
	int TotalHarm = 0;
	uint8_t IsCrit = 0;
	uint8_t IsAttr = 0;
	int harmarr[ONE_TIME_NUMBER] = {0};
	uint8_t IsCritarr[ONE_TIME_NUMBER] = {0};
	uint32_t AfterAttack = 0;
	if (pConf->line == MASTER_LINE)
	{
		pCard = pCRole->GetCardData(pCRole->m_DurationMaster.CardObjId);
		pCardBase = pCRole->GetCardBaseData(pCRole->m_DurationMaster.CardObjId);
		if (CUtil::RandEither(pCard->Crit, 100))
		{
			CritVal =  SKILL_CRIT_FACTOR + pCardBase->AddCritHarm;
			IsCrit = 1;
		}	

		if (pConf->IsLaunchSkill)
		{
			if (pConf->pConf->byAttribute == pCard->Attribute)
			{
				IsAttr = 1;
				AttrVal = SKILL_ATTR_FACTOR;
			}
			int Attack = pCard->AttackM * (pConf->pConf->dwPara1 + pCard->SkillPower + 100) / 100.0 * AttrVal / 100.0 + pCard->AttackE;
			AfterAttack = pCard->AttackM * (pConf->pConf->dwPara1 + pCard->SkillPower + 100) / 100.0 + pCard->AttackE;
//			IME_DEBUG("attack %u, para1 %u, skillpower %u, AfterAttack %u", pCard->Attack, pConf->pConf->dwPara1, pCard->SkillPower, AfterAttack);
			int64_t tmp = (int64_t)Attack * (1 - DURATION_HARM_FIXED_RATE) - pPeerCRole->m_TotalData.Defence / pPeerCRole->GetCardNumber();
			TotalHarm = Attack * DURATION_HARM_FIXED_RATE + (tmp < 0 ? 0 : tmp);
			TotalHarm *= CritVal / 100.0;
			TotalHarm *= DURATION_MASTER_HARM_PARA;

//			TotalHarm += (CritVal == 100 ? 0 : pCardBase->AddCritHarm);
//			IME_DEBUG("0 TotalHarm %d addharm %d, reduceharm %d", TotalHarm, pCardBase->AddHarm, pPeerCRole->GetTotalReduceHarm());
//			TotalHarm = TotalHarm * (100 + pCardBase->AddHarm) / 100.0;
			IME_DEBUG("1 TotalHarm %d addharm %d, reduceharm %d", TotalHarm, pCardBase->AddHarm, pPeerCRole->GetTotalReduceHarm());
		   	TotalHarm = TotalHarm * (100 + (int)pCardBase->AddHarm - (int)pPeerCRole->GetTotalReduceHarm()) / 100.0; 
			IME_DEBUG("2 TotalHarm %d addharm %d, reduceharm %d", TotalHarm, pCardBase->AddHarm, pPeerCRole->GetTotalReduceHarm());

			// for vipharm
			TotalHarm = TotalHarm * (100 + (int)pCRole->m_VipExtra) / 100.0;
			//
//			IME_DEBUG("3 TotalHarm %d", TotalHarm);
			int FloatRare = CUtil::RandByRange(-GlobalConfig::HarmFloat, GlobalConfig::HarmFloat);
			TotalHarm = TotalHarm * (FloatRare + 100) / 100.0;
//			IME_DEBUG("4 TotalHarm %d", TotalHarm);
			
			if (pConf->ismyself)
			{	
				uint32_t restriction[MAX_ATTRIBUTE] = {0};	
				restriction[SWORD] = in.sword;
				restriction[HAND] = in.hand;
				restriction[INTERNAL_FORCE] = in.internalforce;
				restriction[DARK] = in.dark;
				restriction[STICK] = in.stick;

				TotalHarm = TotalHarm * restriction[pCardBase->Attribute] / 100.0;
			}

			if (TotalHarm <= 0)
				TotalHarm = 1;

//			IME_DEBUG("5 TotalHarm %d", TotalHarm);

			IME_DEBUG("Attack %u 2part %lld totalharm %u Totaldefence %u card number %u", Attack, tmp, TotalHarm, pPeerCRole->m_TotalData.Defence, pPeerCRole->GetCardNumber());
			skillindex = pConf->pConf->dwIndex;
		}
		else
		{
			int Attack = pCard->AttackM + pCard->AttackE; 
			int64_t tmp = (int64_t)Attack * (1 - DURATION_HARM_FIXED_RATE) - pPeerCRole->m_TotalData.Defence / pPeerCRole->GetCardNumber();
			TotalHarm = Attack * DURATION_HARM_FIXED_RATE + (tmp < 0 ? 0 : tmp);
			TotalHarm *= CritVal / 100.0;
			TotalHarm *= DURATION_MASTER_HARM_PARA;

//			TotalHarm += (CritVal == 100 ? 0 : pCardBase->AddCritHarm);
//			IME_DEBUG("0 TotalHarm %d addharm %d, reduceharm %d", TotalHarm, pCardBase->AddHarm, pPeerCRole->GetTotalReduceHarm());
//			TotalHarm = TotalHarm * (100 + pCardBase->AddHarm) / 100.0;
			IME_DEBUG("1 TotalHarm %d addharm %d, reduceharm %d", TotalHarm, pCardBase->AddHarm, pPeerCRole->GetTotalReduceHarm());
		   	TotalHarm = TotalHarm * (100 + (int)pCardBase->AddHarm - (int)pPeerCRole->GetTotalReduceHarm()) / 100.0; 
			IME_DEBUG("2 TotalHarm %d addharm %d, reduceharm %d", TotalHarm, pCardBase->AddHarm, pPeerCRole->GetTotalReduceHarm());
			// for vipharm
			TotalHarm = TotalHarm * (100 + (int)pCRole->m_VipExtra) / 100.0;
			//
//			IME_DEBUG("3 TotalHarm %d", TotalHarm);
			int FloatRare = CUtil::RandByRange(-GlobalConfig::HarmFloat, GlobalConfig::HarmFloat);
			TotalHarm = TotalHarm * (FloatRare + 100) / 100.0;
//			IME_DEBUG("4 TotalHarm %d", TotalHarm);

			if (pConf->ismyself)
			{	
				uint32_t restriction[MAX_ATTRIBUTE] = {0};	
				restriction[SWORD] = in.sword;
				restriction[HAND] = in.hand;
				restriction[INTERNAL_FORCE] = in.internalforce;
				restriction[DARK] = in.dark;
				restriction[STICK] = in.stick;

				TotalHarm = TotalHarm * restriction[pCardBase->Attribute] / 100.0;
			}

			if (TotalHarm <= 0)
				TotalHarm = 1;

		}

		if (TotalHarm > MOST_MAX_HARM)
			TotalHarm = MOST_MAX_HARM;

		if (pPeerCRole->m_TotalData.Health <= TotalHarm)
		{
			pCRole->m_harm += pPeerCRole->m_TotalData.Health;	
			pPeerCRole->m_TotalData.Health = 0;
		}
		else
		{
			pPeerCRole->m_TotalData.Health -= TotalHarm;
			pCRole->m_harm += TotalHarm;	
		}
		IME_DEBUG("=== team %u master line %u harm %u ===", pCRole->GetTeam(), pConf->line, TotalHarm);
	}
	else
	{
		uint8_t line = pConf->line;
		int SkillSeq = pConf->SkillSeq;	
		uint32_t linebase = line * ONE_TIME_NUMBER;
		for (int i = 0; i < ONE_TIME_NUMBER; i++)
		{
			if (pCRole->m_vecLineUp[linebase + i] != 0)
			{
				if (pConf->IsLaunchSkill && i == SkillSeq)
				{
					CritVal = 100;
					AttrVal = 100;
					pCard = pCRole->GetCardData(pCRole->m_vecLineUp[linebase + i]);
					pCardBase = pCRole->GetCardBaseData(pCRole->m_vecLineUp[linebase + i]);
					if (CUtil::RandEither(pCard->Crit, 100))
					{
						CritVal =  SKILL_CRIT_FACTOR + pCardBase->AddCritHarm;
						IsCritarr[i] = 1;
					}	
					if (pConf->pConf->byAttribute == pCard->Attribute)
					{
						IsAttr = 1;
						AttrVal = SKILL_ATTR_FACTOR;
					}
					int Attack = pCard->AttackM * (pConf->pConf->dwPara1 + pCard->SkillPower + 100) / 100.0 * AttrVal / 100.0 + pCard->AttackE;
					AfterAttack = pCard->AttackM * (pConf->pConf->dwPara1 + pCard->SkillPower + 100) / 100.0 + pCard->AttackE;
//					IME_DEBUG("attack %u, para1 %u, skillpower %u, AfterAttack %u", pCard->Attack, pConf->pConf->dwPara1, pCard->SkillPower, AfterAttack);
					int64_t tmp = (int64_t)Attack * (1 - DURATION_HARM_FIXED_RATE) - pPeerCRole->m_TotalData.Defence / pPeerCRole->GetCardNumber();
					harmarr[i] = Attack * DURATION_HARM_FIXED_RATE + (tmp < 0 ? 0 : tmp);
					harmarr[i] *= CritVal / 100.0;

//					harmarr[i] += (CritVal == 100 ? 0 : pCardBase->AddCritHarm);
					
//					harmarr[i] += (int) pCardBase->AddHarm - pPeerCRole->GetTotalReduceHarm(); 
//					harmarr[i] = harmarr[i] * (100 + (int)pCardBase->AddHarm) / 100.0;
					harmarr[i] = harmarr[i] * (100 + (int)pCardBase->AddHarm - (int)pPeerCRole->GetTotalReduceHarm()) / 100.0; 

					// for vipharm
					harmarr[i] = harmarr[i] * (100 + (int)pCRole->m_VipExtra) / 100.0;
					//
					int FloatRare = CUtil::RandByRange(-GlobalConfig::HarmFloat, GlobalConfig::HarmFloat);
					harmarr[i] = harmarr[i] * (FloatRare + 100) / 100.0;
					
					if (pConf->ismyself)
					{	
						uint32_t restriction[MAX_ATTRIBUTE] = {0};	
						restriction[SWORD] = in.sword;
						restriction[HAND] = in.hand;
						restriction[INTERNAL_FORCE] = in.internalforce;
						restriction[DARK] = in.dark;
						restriction[STICK] = in.stick;

						harmarr[i] = harmarr[i] * restriction[pCardBase->Attribute] / 100.0;
					}

					if (harmarr[i] <= 0)
						harmarr[i] = 1;
					TotalHarm += harmarr[i];
					skillindex = pConf->pConf->dwIndex;
					IME_DEBUG("follower line %u, obj %u, harm %u", pConf->line, pCRole->m_vecLineUp[linebase + i], harmarr[i]);
				}
				else
				{
					CritVal = 100;
					pCard = pCRole->GetCardData(pCRole->m_vecLineUp[linebase + i]);
					pCardBase = pCRole->GetCardBaseData(pCRole->m_vecLineUp[linebase + i]);
					if (CUtil::RandEither(pCard->Crit, 100))
					{
						CritVal =  SKILL_CRIT_FACTOR + pCardBase->AddCritHarm;
						IsCritarr[i] = 1;
					}	
					int Attack = pCard->AttackM + pCard->AttackE;
					int64_t tmp = (int64_t)Attack * (1 - DURATION_HARM_FIXED_RATE) - pPeerCRole->m_TotalData.Defence / pPeerCRole->GetCardNumber();
					harmarr[i] = Attack * DURATION_HARM_FIXED_RATE + (tmp < 0 ? 0 : tmp);
					harmarr[i] *= CritVal / 100.0;

//					harmarr[i] += (CritVal == 100 ? 0 : pCardBase->AddCritHarm);
//					harmarr[i] = harmarr[i] * (100 + pCardBase->AddHarm) / 100.0;
					harmarr[i] = harmarr[i] * (100 + (int)pCardBase->AddHarm - (int)pPeerCRole->GetTotalReduceHarm()) / 100.0; 

					// for vipharm
					harmarr[i] = harmarr[i] * (100 + (int)pCRole->m_VipExtra) / 100.0;
					//
					int FloatRare = CUtil::RandByRange(-GlobalConfig::HarmFloat, GlobalConfig::HarmFloat);
					harmarr[i] = harmarr[i] * (FloatRare + 100) / 100.0;
					
					if (pConf->ismyself)
					{
						uint32_t restriction[MAX_ATTRIBUTE] = {0};	
						restriction[SWORD] = in.sword;
						restriction[HAND] = in.hand;
						restriction[INTERNAL_FORCE] = in.internalforce;
						restriction[DARK] = in.dark;
						restriction[STICK] = in.stick;

						harmarr[i] = harmarr[i] * restriction[pCardBase->Attribute] / 100.0;
					}
					
					if (harmarr[i] <= 0)
						harmarr[i] = 1;
					TotalHarm += harmarr[i];
					IME_DEBUG("follower line %u, obj %u, harm %u", pConf->line, pCRole->m_vecLineUp[linebase + i], harmarr[i]);
				}
			}
		}
		IME_DEBUG("=== team %u total line %u harm %u ===", pCRole->GetTeam(), pConf->line, TotalHarm);
		if (TotalHarm > MOST_MAX_HARM)
			TotalHarm = MOST_MAX_HARM;
		if (pPeerCRole->m_TotalData.Health <= TotalHarm)
		{
			pCRole->m_harm += pPeerCRole->m_TotalData.Health;	
			pPeerCRole->m_TotalData.Health = 0;
		}
		else
		{
			pPeerCRole->m_TotalData.Health -= TotalHarm;
			pCRole->m_harm += TotalHarm;	
		}
	}

	by << (uint8_t) ACTION_ATTACK;
	by << (uint8_t) pConf->line;
	by << (uint8_t) pCRole->GetTeam();
	by << (uint32_t) (skillindex / 100);
	by << (uint16_t) (skillindex % 100);
	by << pConf->SkillSeq;
	by << (uint8_t) IsAttr;
	by << (uint32_t) AfterAttack;
	for (int i = 0; i < ONE_TIME_NUMBER; i++)
	{
		by << harmarr[i];
		by << IsCritarr[i];
		IME_DEBUG("harm %u, IsCrit %u", harmarr[i], IsCritarr[i]);
	}
	by << TotalHarm;
	by << IsCrit;
	IME_DEBUG("action %u, line %u, team %u, SkillSeq %u, IsAttr %u, AfterAttack %u, totalHarm %u, IsCrit %u, history_harm %u", ACTION_ATTACK, pConf->line, pCRole->GetTeam(), pConf->SkillSeq, IsAttr, AfterAttack, TotalHarm, IsCrit, pCRole->m_harm);
	return 0;
}

int CCardSkill::CardSkillDefence(CDuration *pCRole, CDuration *pPeerCRole, SSkillStu *pConf, ByteBuffer &by, SDurationIn in)
{
	CDuration::DurationData_t *pCard;	
	CDuration::cardforduration_t *pCardBase;
	if (pConf == NULL)
	{
		return -1;
	}

	uint32_t skillindex = 0;
	int CritVal = 100;
	int TotalHarm = 0;
	uint8_t IsCrit = 0;
	int harmarr[ONE_TIME_NUMBER] = {0};
	uint8_t IsCritarr[ONE_TIME_NUMBER] = {0};
	int health = 0;
	if (pConf->line == MASTER_LINE)
	{
		pCard = pCRole->GetCardData(pCRole->m_DurationMaster.CardObjId);
		pCardBase = pCRole->GetCardBaseData(pCRole->m_DurationMaster.CardObjId);
		if (pConf->IsLaunchSkill)
		{
			health = pCardBase->Health * pConf->pConf->dwPara1 / 100.0;
			skillindex = pConf->pConf->dwIndex;
		}
		if (pCRole->m_TotalData.Health + health > pCRole->m_TotalData.HealthMax)
		{
			pCRole->m_TotalData.Health = pCRole->m_TotalData.HealthMax;
		}
		else
		{
			pCRole->m_TotalData.Health += health;
		}
		IME_DEBUG("=== team %u master total health %u ===", pCRole->GetTeam(), health);

		if (CUtil::RandEither(pCard->Crit, 100))
		{
			CritVal =  SKILL_CRIT_FACTOR + pCardBase->AddCritHarm;
			IsCrit = 1;
		}	
		int Attack = pCard->AttackM + pCard->AttackE;
		int64_t tmp = (int64_t)Attack * (1 - DURATION_HARM_FIXED_RATE) - pPeerCRole->m_TotalData.Defence / pPeerCRole->GetCardNumber();
		TotalHarm = Attack * DURATION_HARM_FIXED_RATE + (tmp < 0 ? 0 : tmp);
		TotalHarm *= CritVal / 100.0 ;
		TotalHarm *= DURATION_MASTER_HARM_PARA;

//		TotalHarm += (CritVal == 100 ? 0 : pCardBase->AddCritHarm);
//		TotalHarm += (int) pCardBase->AddHarm - pPeerCRole->GetTotalReduceHarm(); 

//		TotalHarm = TotalHarm * (100 + pCardBase->AddHarm) / 100.0;
		TotalHarm = TotalHarm * (100 + (int)pCardBase->AddHarm - (int)pPeerCRole->GetTotalReduceHarm()) / 100.0; 

		// for vipharm
		TotalHarm = TotalHarm * (100 + (int)pCRole->m_VipExtra) / 100.0;
		//
		int FloatRare = CUtil::RandByRange(-GlobalConfig::HarmFloat, GlobalConfig::HarmFloat);
		TotalHarm = TotalHarm * (FloatRare + 100) / 100.0;

		if (pConf->ismyself)
		{
			uint32_t restriction[MAX_ATTRIBUTE] = {0};	
			restriction[SWORD] = in.sword;
			restriction[HAND] = in.hand;
			restriction[INTERNAL_FORCE] = in.internalforce;
			restriction[DARK] = in.dark;
			restriction[STICK] = in.stick;

			TotalHarm = TotalHarm * restriction[pCardBase->Attribute] / 100.0;
		}

		if (TotalHarm <= 0)
			TotalHarm = 1;

		if (TotalHarm > MOST_MAX_HARM)
			TotalHarm = MOST_MAX_HARM;

		if (pPeerCRole->m_TotalData.Health <= TotalHarm)
		{
			pCRole->m_harm += pPeerCRole->m_TotalData.Health;	
			pPeerCRole->m_TotalData.Health = 0;
		}
		else
		{
			pPeerCRole->m_TotalData.Health -= TotalHarm;
			pCRole->m_harm += TotalHarm;	
		}
		IME_DEBUG("=== team %u master total harm %u ===", pCRole->GetTeam(), TotalHarm);
	}
	else
	{
		uint8_t line = pConf->line;
		int SkillSeq = pConf->SkillSeq;	
		uint32_t linebase = line * ONE_TIME_NUMBER;
		for (int i = 0; i < ONE_TIME_NUMBER; i++)
		{
			if (pCRole->m_vecLineUp[linebase + i] != 0)
			{
				CritVal = 100;
				pCard = pCRole->GetCardData(pCRole->m_vecLineUp[linebase + i]);
				pCardBase = pCRole->GetCardBaseData(pCRole->m_vecLineUp[linebase + i]);
				if (CUtil::RandEither(pCard->Crit, 100))
				{
					CritVal =  SKILL_CRIT_FACTOR + pCardBase->AddCritHarm;
					IsCritarr[i] = 1;
				}	
				int Attack = pCard->AttackM + pCard->AttackE;
				int64_t tmp = (int64_t)Attack * (1 - DURATION_HARM_FIXED_RATE) - pPeerCRole->m_TotalData.Defence / pPeerCRole->GetCardNumber();
				harmarr[i] = Attack * DURATION_HARM_FIXED_RATE + (tmp < 0 ? 0 : tmp);
				harmarr[i] *= CritVal / 100.0;

//				harmarr[i] += (CritVal == 100 ? 0 : pCardBase->AddCritHarm);

//				harmarr[i] = harmarr[i] * (100 + pCardBase->AddHarm) / 100.0;
				harmarr[i] = harmarr[i] * (100 + (int)pCardBase->AddHarm - (int)pPeerCRole->GetTotalReduceHarm()) / 100.0; 

				// for vipharm
				harmarr[i] = harmarr[i] * (100 + (int)pCRole->m_VipExtra) / 100.0;
				//
				int FloatRare = CUtil::RandByRange(-GlobalConfig::HarmFloat, GlobalConfig::HarmFloat);
				harmarr[i] = harmarr[i] * (FloatRare + 100) / 100.0;
				
				if (pConf->ismyself)
				{
					uint32_t restriction[MAX_ATTRIBUTE] = {0};	
					restriction[SWORD] = in.sword;
					restriction[HAND] = in.hand;
					restriction[INTERNAL_FORCE] = in.internalforce;
					restriction[DARK] = in.dark;
					restriction[STICK] = in.stick;

					harmarr[i] = harmarr[i] * restriction[pCardBase->Attribute] / 100.0;
				}

				if (harmarr[i] <= 0)
					harmarr[i] = 1;
				TotalHarm += harmarr[i];
				IME_DEBUG("follower harm %u", harmarr[i]);
			}
			IME_DEBUG("follower line %u, obj %u, harm %u", pConf->line, pCRole->m_vecLineUp[linebase + i], harmarr[i]);
		}
		IME_DEBUG("===== team %u total harm %u ====", pCRole->GetTeam(), TotalHarm);

		if (TotalHarm > MOST_MAX_HARM)
			TotalHarm = MOST_MAX_HARM;

		if (pPeerCRole->m_TotalData.Health <= TotalHarm)
		{
			pCRole->m_harm += pPeerCRole->m_TotalData.Health;	
			pPeerCRole->m_TotalData.Health = 0;
		}
		else
		{
			pPeerCRole->m_TotalData.Health -= TotalHarm;
			pCRole->m_harm += TotalHarm;	
		}

		if (pConf->IsLaunchSkill)
		{
			pCardBase = pCRole->GetCardBaseData(pCRole->m_vecLineUp[linebase + SkillSeq]);
			health = pCardBase->Health * pConf->pConf->dwPara1 / 100.0;
			skillindex = pConf->pConf->dwIndex;
			IME_DEBUG("===== team %u total health %u ====", pCRole->GetTeam(), health);
		}
		if (pCRole->m_TotalData.Health + health > pCRole->m_TotalData.HealthMax)
		{
			pCRole->m_TotalData.Health = pCRole->m_TotalData.HealthMax;
		}
		else
		{
			pCRole->m_TotalData.Health += health;
		}
	}

	by << (uint8_t) ACTION_DEFENCE;
	by << (uint8_t) pConf->line;
	by << (uint8_t) pCRole->GetTeam();
	by << (uint32_t) (skillindex / 100);
	by << (uint16_t) (skillindex % 100);
	by << pConf->SkillSeq;
	by << health;
	for (int i = 0; i < ONE_TIME_NUMBER; i++)
	{
		by << harmarr[i];
		by << IsCritarr[i];
		IME_DEBUG("harm %u, IsCrit %u", harmarr[i], IsCritarr[i]);
	}
	by << TotalHarm;
	by << IsCrit;
	IME_DEBUG("action %u, line %u, team %u, SkillSeq %u, health %u, totalHarm %u, IsCrit %u, totalharm %u", ACTION_DEFENCE, pConf->line, pCRole->GetTeam(), pConf->SkillSeq, health, TotalHarm, IsCrit, pCRole->m_harm);
	return 0;
}

//int CCardSkill::SkillConditionHealth(CDuration *pCRole, SCondition *pStu, int rate)
//{
//	CDuration::DurationData_t* ptemp;	
//	ptemp = pCRole->GetCardData(pCRole->GetLaunchObjid());
//	if (ptemp == NULL)
//		return 1;
//	if (ptemp->Health < ptemp->HealthMax * rate / 100.0)
//	{
//		return 0;
//	}
//	return 1;
//}
//
//int CCardSkill::SkillConditionRound(CDuration *pCRole, SCondition *pStu, int round)
//{
//	if (pStu->round == round)
//		return 0;
//	return 1;
//}
//
//int CCardSkill::SkillConditionRoundDown(CDuration *pCRole, SCondition *pStu, int rounddown)
//{
//	if (pStu->round <= rounddown)
//		return 0;
//	return 1;
//}
//
//int CCardSkill::SkillConditionHealth30(CDuration *pCRole, SCondition *pStu)
//{
//	return SkillConditionHealth(pCRole, pStu, 30);
//}
//
//int CCardSkill::SkillConditionHealth40(CDuration *pCRole, SCondition *pStu)
//{
//	return SkillConditionHealth(pCRole, pStu, 40);
//}
//
//int CCardSkill::SkillConditionHealth50(CDuration *pCRole, SCondition *pStu)
//{
//	return SkillConditionHealth(pCRole, pStu, 50);
//}
//
//int CCardSkill::SkillConditionHealth70(CDuration *pCRole, SCondition *pStu)
//{
//	return SkillConditionHealth(pCRole, pStu, 70);
//}
//
//int CCardSkill::SkillConditionHealth100(CDuration *pCRole, SCondition *pStu)
//{
//	return SkillConditionHealth(pCRole, pStu, 100);
//}
//
//int CCardSkill::SkillConditionRound1(CDuration *pCRole, SCondition *pStu)
//{
//	return SkillConditionRound(pCRole, pStu, 1);
//}
//
//int CCardSkill::SkillConditionRound2(CDuration *pCRole, SCondition *pStu)
//{
//	return SkillConditionRound(pCRole, pStu, 2);
//}
//
//int CCardSkill::SkillConditionRound2Down(CDuration *pCRole, SCondition *pStu)
//{
//	return SkillConditionRoundDown(pCRole, pStu, 2);
//}
//
//int CCardSkill::SkillConditionRound3Down(CDuration *pCRole, SCondition *pStu)
//{
//	return SkillConditionRoundDown(pCRole, pStu, 3);
//}
//
//int CCardSkill::BufferEffectPosition(CDuration *pCRole, STC_CONF_CARD_SKILL *pConf, ByteBuffer &by)
//{
//	if (pConf == NULL)
//		return -1;	
//	CDuration::DurationData_t *ptemp;
//	ptemp = pCRole->GetCardData(pCRole->GetLaunchObjid());
//	if (ptemp == NULL)
//		return -1;
//	uint32_t harm;
//	harm = ptemp->Health * pConf->dwStatusPower / 100.0;
//	by << harm;
//	ptemp->Health -= harm;
//	return 0;
//}
//
//int CCardSkill::BufferEffectAddHealth(CDuration *pCRole, STC_CONF_CARD_SKILL *pConf, ByteBuffer &by)
//{
//	if (pConf == NULL)
//		return -1;
//	CDuration::DurationData_t *ptemp;
//	ptemp = pCRole->GetCardData(pCRole->GetLaunchObjid());
//	if (ptemp == NULL)
//		return -1;
//	uint32_t add;
//	add = ptemp->HealthMax * pConf->dwStatusPower / 100.0;
//	if (add > ptemp->HealthMax - ptemp->Health)
//	{
//		add = ptemp->HealthMax - ptemp->Health;
//	}
//	by << add;
//	ptemp->Health += add;
//	return 0;
//}
//
//int CCardSkill::BufferEffectAddAttack(CDuration *pCRole, STC_CONF_CARD_SKILL *pConf, ByteBuffer &by)
//{
//	CDuration::DurationData_t* ptemp;	
//	ptemp = pCRole->GetCardData(pCRole->GetLaunchObjid());
//	if (ptemp == NULL)
//		return -1;
//	ptemp->Attack += ptemp->Attack * pConf->dwStatusPower / 100.0;
//	return 0;
//}
//
//int CCardSkill::BufferEffectAddDefence(CDuration *pCRole, STC_CONF_CARD_SKILL *pConf, ByteBuffer &by)
//{
//	CDuration::DurationData_t* ptemp;	
//	ptemp = pCRole->GetCardData(pCRole->GetLaunchObjid());
//	if (ptemp == NULL)
//		return -1;
//	ptemp->Defence += ptemp->Defence * pConf->dwStatusPower / 100.0;
//	return 0;
//}
//
//int CCardSkill::BufferEffectReduceAttack(CDuration *pCRole, STC_CONF_CARD_SKILL *pConf, ByteBuffer &by)
//{
//	CDuration::DurationData_t* ptemp;	
//	ptemp = pCRole->GetCardData(pCRole->GetLaunchObjid());
//	if (ptemp == NULL)
//		return -1;
//	ptemp->Attack += ptemp->Attack * pConf->dwStatusPower / 100.0;
//	return 0;
//}
//
//int CCardSkill::BufferEffectReduceDefence(CDuration *pCRole, STC_CONF_CARD_SKILL *pConf, ByteBuffer &by)
//{
//	CDuration::DurationData_t* ptemp;	
//	ptemp = pCRole->GetCardData(pCRole->GetLaunchObjid());
//	if (ptemp == NULL)
//		return -1;
//	ptemp->Defence += ptemp->Defence * pConf->dwStatusPower / 100.0;
//	return 0;
//}
//
//void CCardSkill::GetTargetAttackSpecialOne(CDuration *pCRole, CDuration *pPeerCRole, std::vector<uint32_t> &vecTargetObjId)
//{
//	vecTargetObjId.clear();
//	uint32_t objid = 0;
//	uint32_t health = 0;
//	CDuration::DurationData_t* ptemp;	
//	for (int i = 0; i < MAX_CARD_CAPACITY; i++)
//	{
//		if (pPeerCRole->m_vecLineUp[i] != 0)
//		{
//			ptemp = pPeerCRole->GetCardData(pPeerCRole->m_vecLineUp[i]);
//			if (ptemp == NULL)
//				return;
//			if (ptemp->Health == 0)
//			{
//				continue;
//			}
//
//			if (health == 0)
//			{
//				objid = ptemp->CardObjId;
//				health = ptemp->Health;	
//			}
//			else
//			{
//				if (health > ptemp->Health)
//				{
//					objid = ptemp->CardObjId;
//					health = ptemp->Health;
//				}
//			}
//		}
//	}
//	if (objid != 0)
//		vecTargetObjId.push_back(objid);
//}
//
//void CCardSkill::GetTargetAttackOne(CDuration *pCRole, CDuration *pPeerCRole, std::vector<uint32_t> &vecTargetObjId)
//{
//	vecTargetObjId.clear();
//	if (pCRole->GetBuffer(pCRole->GetLaunchObjid(), S_TARGET))
//	{
//		return GetTargetAttackSpecialOne(pCRole, pPeerCRole, vecTargetObjId);
//	}
//	else
//	{
//		int position;
//		CDuration::DurationData_t* ptemp;	
//		position = find(pCRole->m_vecLineUp.begin(), pCRole->m_vecLineUp.end(), pCRole->GetLaunchObjid()) - pCRole->m_vecLineUp.begin();
//		int target[6][6] = {
//			{1, 3, 5, 2, 4, 6},
//			{1, 3, 5, 2, 4, 6},
//			{3, 1, 5, 2, 4, 6},
//			{3, 1, 5, 2, 4, 6},
//			{5, 1, 3, 2, 4, 6},
//			{5, 1, 3, 2, 4, 6}
//		};
//		uint32_t objid;
//		for (int j = 0; j < 6; j++)
//		{
//			objid = pPeerCRole->m_vecLineUp[target[position][j] - 1];
//			if (objid != 0)
//			{
//				ptemp = pPeerCRole->GetCardData(objid);
//				if (ptemp == NULL)
//					return;
//				if (ptemp->Health > 0)
//				{
//					vecTargetObjId.push_back(objid);
//					break;
//				}
//			}
//		}
//		return;
//	}
//	return;
//}
//
//void CCardSkill::GetTargetAttackColumn(CDuration *pCRole, CDuration *pPeerCRole, std::vector<uint32_t> &vecTargetObjId)
//{
//	vecTargetObjId.clear();
//	std::vector<uint32_t> vectemp;
//
//	int position;
//	CDuration::DurationData_t* ptemp;	
//	position = find(pCRole->m_vecLineUp.begin(), pCRole->m_vecLineUp.end(), pCRole->GetLaunchObjid()) - pCRole->m_vecLineUp.begin();
//	int target[6][3][2] = {
//		{{1, 2}, {3, 4}, {5, 6}},
//		{{1, 2}, {3, 4}, {5, 6}},
//		{{3, 4}, {1, 2}, {5, 6}},
//		{{3, 4}, {1, 2}, {5, 6}},
//		{{5, 6}, {1, 2}, {3, 4}},
//		{{5, 6}, {1, 2}, {3, 4}},
//	};
//	uint32_t objid;
//	for (int j = 0; j < 3; j++)
//	{
//		vectemp.clear();
//
//		objid = pPeerCRole->m_vecLineUp[target[position][j][0] - 1];
//		if (objid != 0)
//		{
//			ptemp = pPeerCRole->GetCardData(objid);
//			if (ptemp == NULL)
//				return;
//			if (ptemp->Health > 0)
//			{
//				vectemp.push_back(objid);
//			}
//		}
//
//		objid = pPeerCRole->m_vecLineUp[target[position][j][1] - 1];
//		if (objid != 0)
//		{
//			ptemp = pPeerCRole->GetCardData(objid);
//			if (ptemp == NULL)
//				return;
//			if (ptemp->Health > 0)
//			{
//				vectemp.push_back(objid);
//			}
//		}
//
//		if (vectemp.size() != 0)
//		{
//			vecTargetObjId = vectemp;
//			return;			
//		}	
//
//	}
//	return;
//}
//
//void CCardSkill::GetTargetAttackOneTrough(CDuration *pCRole, CDuration *pPeerCRole, std::vector<uint32_t> &vecTargetObjId)
//{
//	vecTargetObjId.clear();
//	std::vector<uint32_t> vectemp;
//
//	int position;
//	CDuration::DurationData_t* ptemp;	
//	position = find(pCRole->m_vecLineUp.begin(), pCRole->m_vecLineUp.end(), pCRole->GetLaunchObjid()) - pCRole->m_vecLineUp.begin();
//	int target[6][3][2] = {
//		{{1, 2}, {3, 4}, {5, 6}},
//		{{1, 2}, {3, 4}, {5, 6}},
//		{{3, 4}, {1, 2}, {5, 6}},
//		{{3, 4}, {1, 2}, {5, 6}},
//		{{5, 6}, {1, 2}, {3, 4}},
//		{{5, 6}, {1, 2}, {3, 4}},
//	};
//	uint32_t objid;
//	for (int j = 0; j < 3; j++)
//	{
//		vectemp.clear();
//
//		objid = pPeerCRole->m_vecLineUp[target[position][j][0] - 1];
//		if (objid != 0)
//		{
//			ptemp = pPeerCRole->GetCardData(objid);
//			if (ptemp == NULL)
//				return;
//			if (ptemp->Health > 0)
//			{
//				vectemp.push_back(objid);
//			}
//		}
//
//		objid = pPeerCRole->m_vecLineUp[target[position][j][1] - 1];
//		if (objid != 0)
//		{
//			ptemp = pPeerCRole->GetCardData(objid);
//			if (ptemp == NULL)
//				return;
//			if (ptemp->Health > 0)
//			{
//				vectemp.push_back(objid);
//			}
//		}
//
//		if (vectemp.size() != 0)
//		{
//			vecTargetObjId = vectemp;
//			return;			
//		}	
//
//	}
//	return;
//}
//
//void CCardSkill::GetTargetAttackFrontLine(CDuration *pCRole, CDuration *pPeerCRole, std::vector<uint32_t> &vecTargetObjId)
//{
//	vecTargetObjId.clear();
//
//	int postion;
//	CDuration::DurationData_t* ptemp;	
//	int target[2][3] = {
//		{3, 1, 5},
//		{4, 2, 6}
//	};
//	uint32_t objid;
//	std::vector<uint32_t> vectemp;
//	for (int j = 0; j < 2; j++)
//	{
//		vectemp.clear();
//
//		objid = pPeerCRole->m_vecLineUp[target[j][0] - 1];
//		if (objid != 0)
//		{
//			ptemp = pPeerCRole->GetCardData(objid);
//			if (ptemp == NULL)
//				return;
//			if (ptemp->Health > 0)
//			{
//				vectemp.push_back(objid);
//			}
//		}
//
//		objid = pPeerCRole->m_vecLineUp[target[j][1] - 1];
//		if (objid != 0)
//		{
//			ptemp = pPeerCRole->GetCardData(objid);
//			if (ptemp == NULL)
//				return;
//			if (ptemp->Health > 0)
//			{
//				vectemp.push_back(objid);
//			}
//		}
//
//		objid = pPeerCRole->m_vecLineUp[target[j][2] - 1];
//		if (objid != 0)
//		{
//			ptemp = pPeerCRole->GetCardData(objid);
//			if (ptemp == NULL)
//				return;
//			if (ptemp->Health > 0)
//			{
//				vectemp.push_back(objid);
//			}
//		}
//
//		if (vectemp.size() != 0)
//		{
//			vecTargetObjId = vectemp;
//			return;			
//		}	
//
//	}
//	return;
//}
//
//void CCardSkill::GetTargetAttackBehindLine(CDuration *pCRole, CDuration *pPeerCRole, std::vector<uint32_t> &vecTargetObjId)
//{
//	vecTargetObjId.clear();
//
//	int postion;
//	CDuration::DurationData_t* ptemp;	
//	int target[2][3] = {
//		{4, 2, 6},
//		{3, 1, 5}
//	};
//	uint32_t objid;
//	std::vector<uint32_t> vectemp;
//	for (int j = 0; j < 2; j++)
//	{
//		vectemp.clear();
//
//		objid = pPeerCRole->m_vecLineUp[target[j][0] - 1];
//		if (objid != 0)
//		{
//			ptemp = pPeerCRole->GetCardData(objid);
//			if (ptemp == NULL)
//				return;
//			if (ptemp->Health > 0)
//			{
//				vectemp.push_back(objid);
//			}
//		}
//
//		objid = pPeerCRole->m_vecLineUp[target[j][1] - 1];
//		if (objid != 0)
//		{
//			ptemp = pPeerCRole->GetCardData(objid);
//			if (ptemp == NULL)
//				return;
//			if (ptemp->Health > 0)
//			{
//				vectemp.push_back(objid);
//			}
//		}
//
//		objid = pPeerCRole->m_vecLineUp[target[j][2] - 1];
//		if (objid != 0)
//		{
//			ptemp = pPeerCRole->GetCardData(objid);
//			if (ptemp == NULL)
//				return;
//			if (ptemp->Health > 0)
//			{
//				vectemp.push_back(objid);
//			}
//		}
//
//		if (vectemp.size() != 0)
//		{
//			vecTargetObjId = vectemp;
//			return;			
//		}	
//
//	}
//	return;
//
//}
//
//void CCardSkill::GetTargetAttackAll(CDuration *pCRole, CDuration *pPeerCRole, std::vector<uint32_t> &vecTargetObjId)
//{
//	vecTargetObjId.clear();
//
//	CDuration::DurationData_t* ptemp;	
//	uint32_t objid;
//	for (int i = 0; i < pPeerCRole->m_vecLineUp.size(); i++)
//	{
//		objid = pPeerCRole->m_vecLineUp[i];
//		if (objid != 0)
//		{
//			ptemp = pPeerCRole->GetCardData(objid);
//			if (ptemp == NULL)
//				return;
//			if (ptemp->Health > 0)
//			{
//				vecTargetObjId.push_back(objid);
//			}
//		}
//	}	
//	return;
//}
//void CCardSkill::GetTargetDefenceOne(CDuration *pCRole, CDuration *pPeerCRole, std::vector<uint32_t> &vecTargetObjId)
//{
//
//	vecTargetObjId.clear();
//	uint32_t objid = 0;
//	uint32_t health = 0;
//	CDuration::DurationData_t* ptemp;	
//	for (int i = 0; i < MAX_CARD_CAPACITY; i++)
//	{
//		if (pCRole->m_vecLineUp[i] != 0)
//		{
//			ptemp = pCRole->GetCardData(pCRole->m_vecLineUp[i]);
//			if (ptemp == NULL)
//				return;
//			if (ptemp->Health == 0)
//			{
//				continue;
//			}
//			
//			uint32_t healthrate = ptemp->Health * 100.0 / ptemp->HealthMax;
//			if (health == 0)
//			{
//				objid = ptemp->CardObjId;
//				health = healthrate;	
//			}
//			else
//			{
//				if (health > healthrate)
//				{
//					objid = ptemp->CardObjId;
//					health = healthrate;
//				}
//			}
//		}
//	}
//	if (objid != 0)
//		vecTargetObjId.push_back(objid);
//
//}
//
//void CCardSkill::GetTargetDefenceAll(CDuration *pCRole, CDuration *pPeerCRole, std::vector<uint32_t> &vecTargetObjId)
//{
//	vecTargetObjId.clear();
//
//	CDuration::DurationData_t* ptemp;	
//	uint32_t objid;
//	for (int i = 0; i < pCRole->m_vecLineUp.size(); i++)
//	{
//		objid = pCRole->m_vecLineUp[i];
//		if (objid != 0)
//		{
//			ptemp = pCRole->GetCardData(objid);
//			if (ptemp == NULL)
//				return;
//			if (ptemp->Health > 0)
//			{
//				vecTargetObjId.push_back(objid);
//			}
//		}
//	}	
//	return;
//}
//
//void CCardSkill::GetTargetDefenceMyself(CDuration *pCRole, CDuration *pPeerCRole, std::vector<uint32_t> &vecTargetObjId)
//{
//	uint32_t objid = pCRole->GetLaunchObjid();
//	if (objid != 0)
//	{
//		CDuration::DurationData_t *ptemp = pCRole->GetCardData(objid);
//		if (ptemp == NULL)
//			return;
//		if (ptemp->Health > 0)
//		{
//			vecTargetObjId.push_back(objid);
//			return;
//		}
//	}
//	return;
//}

