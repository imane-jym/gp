/**
 * =====================================================================================
 *       @file  CardPacket.cpp
 *      @brief  
 *
 *  Detailed description starts here.
 *
 *   @internal
 *     Created  2012年08月28日 15时04分48秒 
 *    Revision  1.0.0.0
 *    Compiler  gcc/g++
 *     Company  
 *   Copyright  Copyright (c) 2012, 
 *
 *     @author  imane (), imanecr@gmail.com
 * =====================================================================================
 */

#include "CardPacket.h"
#include <new>
#include <stdio.h>
#include <algorithm>
#include "World.h"
#include "EnumDefines.h"
#include "DBCtrl.h"
#include "Util.h"
#include "Role.h"
#include "CmdDefine.h"
#include "ErrorMsg.h"
#include "skill.h"
#include "ConfigData.h"
#include "Tujian.h"
#include "Tech.h"
#include "Other.h"
#include "Mission.h"
#include "skill.h"
#include "Master.h"
#include "Tower.h"

using namespace std;

CCardPacket::CCardPacket()
{
    m_bIsInit = false;
	m_dwTotalSpace = GlobalConfig::RoleCardLimit + GlobalConfig::EquipCardLimit;
	m_dwCardNumberRole = 0;
	m_dwCardNumberEquip = 0;
	m_dwCardNumber = 0;
	m_vecLineUp.resize(MAX_CARD_CAPACITY, 0);
	m_MasterCard = 0;
	m_pCRole = NULL;
	addAttackTimes = 1;
	m_wMaxCardLevel = 0;
}

CCardPacket::CCardPacket(CRole *pCRole):m_bIsInit(false),m_dwTotalSpace(GlobalConfig::RoleCardLimit + GlobalConfig::EquipCardLimit),m_dwCardNumberRole(0),m_dwCardNumberEquip(0),m_dwCardNumber(0) 
{
	m_pCRole = pCRole;
	m_vecLineUp.resize(MAX_CARD_CAPACITY, 0);
	m_MasterCard = 0;
	addAttackTimes = 1;
	m_wMaxCardLevel = 0;
}

CCardPacket::~CCardPacket()
{
    std::map<uint32_t, CCard *>::const_iterator it = m_mapCards.begin();
    for (; it != m_mapCards.end(); it++)
    {
        delete it->second;
    }
}

void CCardPacket::StrSplit(const std::string & src, const std::string & sep, std::vector<uint32_t> & vecRes) 
{
	vecRes.clear();
	std::string s;
	for(std::string::const_iterator i = src.begin(); i != src.end(); i++)
	{                             
		if (sep.find(*i) != std::string::npos)
		{                                    
			if (s.length()) vecRes.push_back(atol(s.c_str()));
			s = "";
		}
		else          
		{   
			s += *i;                                                                          
		}
	}
	if (s.length()) vecRes.push_back(atol(s.c_str()));
}

void CCardPacket::StrMerge(std::string & src, const std::string & sep, const std::vector<uint32_t> & vecRes)  
{
	src.clear();
	char temp[50] = {0};
	std::vector<uint32_t>::const_iterator i = vecRes.begin();
	if (i != vecRes.end())
	{
		sprintf(temp, "%u", *i);
		src = temp;
		i++;
	}
	for(; i != vecRes.end(); i++)
	{
		src += sep;
		sprintf(temp, "%u", *i);
		src += temp;
	}
}

int CCardPacket::EquipCardCalForDuration(itemequip_t &tempM, itemequip_t &tempE, int i, int flag)
{
	uint32_t Attack = 0;
	uint32_t Defence = 0;
	uint32_t Health = 0;
	uint32_t Crit = 0;
	uint32_t Dodge = 0;
	uint32_t Action = 0;
	uint32_t SkillPower = 0;

	if (m_vecItemEquip1.size() < m_vecAttackTeam.size())
	{
		m_vecItemEquip1.resize(m_vecAttackTeam.size());
	}
	
	if (m_vecEquip1.size() < m_vecAttackTeam.size())
	{
		m_vecEquip1.resize(m_vecAttackTeam.size());
	}

	std::vector<int> vecRelationshipEffect;
	std::vector<int> vecCard;
	std::vector<int> vecRelationshipSkill;
	CCard *pCard;
	if (flag)
	{
		for (int k = 0; k < m_vecAttackTeam.size(); k++)
		{
			if (m_vecAttackTeam[k] != 0)
			{
				pCard = Find(m_vecAttackTeam[k]);
				if (pCard == NULL)
				{
					IME_ERROR("can not find this card objid %u", m_vecAttackTeam[k]);
					return false;
				}
//				vecCard.push_back(pCard->GetdwIndex());
				vecCard.push_back(pCard->GetdwCardGroup());
				STC_CONF_CARD_BASE *pConfCard = CConfCardBase::Find(pCard->GetdwIndex());
				if (pConfCard == NULL)
				{
					IME_ERROR("can not find card index %u", pCard->GetdwIndex());
					return false;
				}
				vecRelationshipSkill.push_back(pConfCard->dwRelation1);
				vecRelationshipSkill.push_back(pConfCard->dwRelation2);
				vecRelationshipSkill.push_back(pConfCard->dwRelation3);
				vecRelationshipSkill.push_back(pConfCard->dwRelation4);
			}
		}
		pCard = Find(m_MasterCard);
		if (pCard == NULL)
		{
			IME_ERROR("can not find this card objid %u", m_MasterCard);
			return false;
		}
		STC_CONF_CARD_BASE *pConfCard = CConfCardBase::Find(pCard->GetdwIndex());
		if (pConfCard == NULL)
		{
			IME_ERROR("can not find card index %u", pCard->GetdwIndex());
			return false;
		}
		vecRelationshipSkill.push_back(pConfCard->dwRelation1);
		vecRelationshipSkill.push_back(pConfCard->dwRelation2);
		vecRelationshipSkill.push_back(pConfCard->dwRelation3);
		vecRelationshipSkill.push_back(pConfCard->dwRelation4);
//		vecCard.push_back(pCard->GetdwIndex());
		vecCard.push_back(pCard->GetdwCardGroup());
		vecRelationshipEffect = CConfRelationShip::GetRelationshipIndex(vecCard, vecRelationshipSkill);
		for (int j = 0; j < vecRelationshipEffect.size(); j++)
		{
			IME_DEBUG("relation skill happen %u", vecRelationshipEffect[j]);
		}
	}

	if (i >= 0)
	{
		if (i >= m_vecAttackTeam.size())
		{
			return -1;
		}

		Attack = 0;
		Defence = 0;
		Health = 0;
		Crit = 0;
		Dodge = 0;
		Action = 0;

		uint32_t pet = m_vecAttackTeam[i];
		if (pet == 0)
		{
			return 0;
		}

		CCard *pPet = Find(pet);
		if (pPet == NULL)
		{
			IME_ERROR("can not find this card %u in packet", pet);
			return -1;
		}
		STC_CONF_CARD_BASE *pConfCard = CConfCardBase::Find(pPet->GetdwIndex());
		if (pConfCard == NULL)
		{
			IME_ERROR("can not find card %u in packet", pPet->GetdwIndex());
			return -1;
		}

		Attack += pPet->GetdwAttack();
		Defence += pPet->GetdwDefence();
		Health += pPet->GetdwHealth();
		Crit += pPet->GetdwCritRate();
//		Dodge += pPet->GetdwDodgeRate();
//		Action += pPet->GetdwAction();

		tempM.Attack = Attack;
		tempM.Defence = Defence;
		tempM.Health = Health;
		tempM.Crit = Crit;
//		tempM.Dodge = Dodge;
//		tempM.Action = Action;
		tempM.Attribute = pPet->GetbyAttribute(); 
		tempM.SkillPro = pPet->GetdwSkillPro();
		tempM.SkillPower = pPet->GetdwSkillPower();
		tempM.DurationExp = pPet->GetdwDurationExp();
		tempM.DurationCoin = pPet->GetdwDurationCoin();
		tempM.ObjDrop = pPet->GetdwObjDrop();
		tempM.StrengthRecover = pPet->GetdwStrengthRecover();
		tempM.EnergyRecover = pPet->GetdwEnergyRecover();
		tempM.AddHarm = pPet->GetdwAddHarm();
		tempM.ReduceHarm = pPet->GetdwReduceHarm();
		tempM.AddCritHarm = pPet->GetdwAddCritHarm();

		if (flag)
		{
			if (m_pCRole->GetbyDuration() == DURATION_FRIEND_BOSS)
			{
				uint32_t multi = 100;
				if (time(NULL) <= pPet->GetdwMultiTime())
				{
					multi = pPet->GetbyMultiPower() * 100;
				}

				STC_CONF_VIP *pConfVip = NULL;
				pConfVip = CConfVip::Find(m_pCRole->GetwVipLv());
				if (pConfVip == NULL)
				{
					IME_ERROR("vip lv %u can not find in vip csv", m_pCRole->GetwVipLv());
				}
				else
				{
					multi += pConfVip->dwVipBossAttack;
				}

				tempM.Attack = tempM.Attack * multi / 100.0f;
			}
		}
		
		uint32_t weapon = m_vecEquip1[i].dwWeaponObjId;
		uint32_t armor = m_vecEquip1[i].dwArmorObjId;
		uint32_t jewel = m_vecEquip1[i].dwJewelObjId;
		uint32_t skill = m_vecEquip1[i].dwSkillObjId; 
		uint32_t accessories = m_vecEquip1[i].dwAccessoriesObjId;
		uint32_t masterguide1 = m_vecEquip1[i].dwMasterGuide1;
		uint32_t masterguide2 = m_vecEquip1[i].dwMasterGuide2;
		uint32_t masterguide3 = m_vecEquip1[i].dwMasterGuide3;

		if (jewel != 0)
		{
			CCard *p = Find(jewel);
			if (p == NULL)
			{
				IME_ERROR("%u user have objid %u in first team but not in packet", m_pCRole->GetdwAccountId(), jewel); 
				return -1;
			}

			if (p->GetdwEffectIndex() != 0)
			{
				STC_CONF_JEWEL_SKILL *pJewelSkill = CConfJewelSkill::Find(p->GetdwEffectIndex());
				if (pJewelSkill == NULL)
				{
					IME_ERROR("can not find jewelskill index %u", p->GetdwEffectIndex());
					return -1;
				}
				CJewelSkill::Init();
				CJewelSkill::JewelSkillFunc pSkillFunc = CJewelSkill::Find(pJewelSkill->wType);
				if (pSkillFunc == NULL)
				{
					IME_ERROR("can not find jewelskill type %u, jewelskill index %u", pJewelSkill->wType, p->GetdwEffectIndex());
					return -1;
				}
				IME_DEBUG("jewel skill %u, base %u all %u", pJewelSkill->wType, pJewelSkill->para1, p->GetdwSkillPara());
				uint32_t para1;
//				if (p->GetdwCardGroup() == pConfCard->dwStrongSkill)
//				{
//					para1 = p->GetdwSkillPara() + pConfCard->bySkillFunc;
//				}
//				else
//				{
					para1 = p->GetdwSkillPara();
//				}

				if ((*pSkillFunc)(&tempM, para1))
				{
					IME_ERROR("jewel skill exec fail index %u", pJewelSkill->dwIndex);
					return -1;
				}
			}
		}

		if (flag)
		{
			std::vector<int> vecRelationship;
			if (find(vecRelationshipEffect.begin(), vecRelationshipEffect.end(), pConfCard->dwRelation1) != vecRelationshipEffect.end())
			{
				vecRelationship.push_back(pConfCard->dwRelation1);
			}
			if (find(vecRelationshipEffect.begin(), vecRelationshipEffect.end(), pConfCard->dwRelation2) != vecRelationshipEffect.end())
			{
				vecRelationship.push_back(pConfCard->dwRelation2);
			}
			if (find(vecRelationshipEffect.begin(), vecRelationshipEffect.end(), pConfCard->dwRelation3) != vecRelationshipEffect.end())
			{
				vecRelationship.push_back(pConfCard->dwRelation3);
			}
			if (find(vecRelationshipEffect.begin(), vecRelationshipEffect.end(), pConfCard->dwRelation4) != vecRelationshipEffect.end())
			{
				vecRelationship.push_back(pConfCard->dwRelation4);
			}
			IME_DEBUG("fellow before card index %u attack %u, defence %u, health %u, crit %u, attribute %u, skillpro %u, SkillPower %u, durationexp %u, DurationCoin %u, ObjDrop %u, StrengthRecover %u, EnergyRecover %u, AddHarm %u, ReduceHarm %u, AddCritHarm %u", pPet->GetdwIndex(), tempM.Attack, tempM.Defence, tempM.Health, tempM.Crit, tempM.Attribute, tempM.SkillPro, tempM.SkillPower, tempM.DurationExp, tempM.DurationCoin, tempM.ObjDrop, tempM.StrengthRecover, tempM.EnergyRecover, tempM.AddHarm, tempM.ReduceHarm, tempM.AddCritHarm);
//			CExSkill::ExecExSkillFuncByRelation(pPet->GetdwIndex(), vecRelationship, &tempM);
			CExSkill::ExecExSkillFuncByRelation(pPet->GetdwCardGroup(), vecRelationship, &tempM);
			IME_DEBUG("fellow after attack %u, defence %u, health %u, crit %u, attribute %u, skillpro %u, SkillPower %u, durationexp %u, DurationCoin %u, ObjDrop %u, StrengthRecover %u, EnergyRecover %u, AddHarm %u, ReduceHarm %u, AddCritHarm %u", tempM.Attack, tempM.Defence, tempM.Health, tempM.Crit, tempM.Attribute, tempM.SkillPro, tempM.SkillPower, tempM.DurationExp, tempM.DurationCoin, tempM.ObjDrop, tempM.StrengthRecover, tempM.EnergyRecover, tempM.AddHarm, tempM.ReduceHarm, tempM.AddCritHarm);
		}

		if (m_MasterCard != 0)
		{
			CCard *pMasterPet = Find(m_MasterCard);
			if (pMasterPet == NULL)
			{
				IME_ERROR("master card is null objid %u accountid %u", m_MasterCard, m_pCRole->GetdwAccountId());
				return -1;
			}
//		IME_ERROR("cal ~~~~~~~~~~~~ leader skill %u index %u", pMasterPet->GetdwLeaderIndex(), pMasterPet->GetdwIndex());
			if (pMasterPet->GetdwLeaderIndex() != 0)
			{
				STC_CONF_LEADER_SKILL *pConfLeader = CConfLeaderSkill::Find(pMasterPet->GetdwLeaderIndex());
				if (pConfLeader != NULL)
				{
					std::vector<int> attr;
					std::vector<int> type;
					std::vector<int> para;
					if (pConfLeader->dwStick)
						attr.push_back(STICK);
					if (pConfLeader->dwSword)
						attr.push_back(SWORD);
					if (pConfLeader->dwDark)
						attr.push_back(DARK);
					if (pConfLeader->dwPalm)
						attr.push_back(HAND);
					if (pConfLeader->dwInside)
						attr.push_back(INTERNAL_FORCE);
					
					if (pConfLeader->wtype1)
					{
						type.push_back(pConfLeader->wtype1);
						para.push_back(pConfLeader->dwNum1 + (pMasterPet->GetwLeaderLevel() - 1) * pConfLeader->dwUp1);
					}
					if (pConfLeader->wtype2)
					{
						type.push_back(pConfLeader->wtype2);
						para.push_back(pConfLeader->dwNum2 + (pMasterPet->GetwLeaderLevel() - 1) * pConfLeader->dwUp2);
					}
					if (pConfLeader->wtype3)
					{
						type.push_back(pConfLeader->wtype3);
						para.push_back(pConfLeader->dwNum3 + (pMasterPet->GetwLeaderLevel() - 1) * pConfLeader->dwUp3);
					}
					
//					IME_ERROR("attr %u own size %u", pPet->GetbyAttribute(), attr.size());	
					if (find(attr.begin(), attr.end(), pPet->GetbyAttribute()) != attr.end())
					{
						CExSkill::Init();
						for (int ii = 0; ii < type.size(); ii++)
						{
//							IME_ERROR("before skill type %u para %u attack %u defence %u health %u", type[ii], para[ii], tempM.Attack, tempM.Defence, tempM.Health);
							CExSkill::ExSkillFunc pFunc = CExSkill::Find(type[ii]);
							if (pFunc == NULL)
							{
								IME_ERROR("leader skill can not find skill func type %u", type[ii]);
								return -1;
							}

							if (pFunc(&tempM, para[ii]))
							{
								IME_ERROR("leader skill happen error");
								return -1;
							}
//							IME_ERROR("after skill type %u para %u attack %u defence %u health %u", type[ii], para[ii], tempM.Attack, tempM.Defence, tempM.Health);
						}
					}
				}	
			}
		}
		
		Attack = 0;
		Defence = 0;
		Health = 0;
		Crit = 0;
		Dodge = 0;
		Action = 0;
		SkillPower = 0;

		if (weapon != 0)
		{
			CCard *p = Find(weapon);
			if (p == NULL)
			{
				IME_ERROR("%u user have objid %u in first team but not in packet", m_pCRole->GetdwAccountId(), weapon); 
				return -1;
			}

			uint32_t para1 = 0;
			if (p->GetdwCardGroup() == pConfCard->dwStrongWeapon)
			{
				para1 = pConfCard->byWeaponFunc;
			}

			Attack += m_mapCards[weapon]->GetdwAttack() * ((100 + para1) / 100.0);
			Defence += m_mapCards[weapon]->GetdwDefence() * ((100 + para1) / 100.0);
			Health += m_mapCards[weapon]->GetdwHealth() * ((100 + para1) / 100.0);
			Crit += m_mapCards[weapon]->GetdwCritRate() * ((100 + para1) / 100.0);
//			Dodge += m_mapCards[weapon]->GetdwDodgeRate();
//			Action += m_mapCards[weapon]->GetdwAction();
		}
		if (armor != 0)
		{
			CCard *p = Find(armor);
			if (p == NULL)
			{
				IME_ERROR("%u user have objid %u in first team but not in packet", m_pCRole->GetdwAccountId(), armor); 
				return -1;
			}

			uint32_t para1 = 0;
			if (p->GetdwCardGroup() == pConfCard->dwStrongArmor)
			{
				para1 = pConfCard->byArmorFunc;
			}

			Attack += m_mapCards[armor]->GetdwAttack() * ((100 + para1) / 100.0);
			Defence += m_mapCards[armor]->GetdwDefence() * ((100 + para1) / 100.0);
			Health += m_mapCards[armor]->GetdwHealth() * ((100 + para1) / 100.0);
			Crit += m_mapCards[armor]->GetdwCritRate() * ((100 + para1) / 100.0);
//			Dodge += m_mapCards[armor]->GetdwDodgeRate();
//			Action += m_mapCards[armor]->GetdwAction();
		}
		if (accessories != 0)
		{
			CCard *p = Find(accessories);
			if (p == NULL)
			{
				IME_ERROR("%u user have objid %u in first team but not in packet", m_pCRole->GetdwAccountId(), armor); 
				return -1;
			}

			uint32_t para1 = 0;
			if (p->GetdwCardGroup() == pConfCard->dwStrongAccessories)
			{
				para1 = pConfCard->byAccessoriesFunc;
			}

			Attack += m_mapCards[accessories]->GetdwAttack() * ((100 + para1) / 100.0);
			Defence += m_mapCards[accessories]->GetdwDefence() * ((100 + para1) / 100.0);
			Health += m_mapCards[accessories]->GetdwHealth() * ((100 + para1) / 100.0);
			Crit += m_mapCards[accessories]->GetdwCritRate() * ((100 + para1) / 100.0);
//			Dodge += m_mapCards[accessories]->GetdwDodgeRate();
//			Action += m_mapCards[accessories]->GetdwAction();
		}
		if (skill != 0)
		{
			CCard *p = Find(skill);
			if (p == NULL)
			{
				IME_ERROR("%u user have objid %u in first team but not in packet", m_pCRole->GetdwAccountId(), skill); 
				return -1;
			}

			uint32_t para1 = 0;
			if (p->GetdwCardGroup() == pConfCard->dwStrongSkill)
			{
				para1 = pConfCard->bySkillFunc;
			}
			SkillPower += para1;
		}
		if (m_pCRole != NULL)
		{
			if (masterguide1 != 0)
			{
				CCard *p = Find(masterguide1);
				if (p == NULL)
				{
					IME_ERROR("%u user have objid %u in first team but not in packet", m_pCRole->GetdwAccountId(), masterguide1); 
					return -1;
				}
				uint32_t Rate = MASTER_FORCE_BASE + MASTER_FORCE_GROW * m_pCRole->GetCMaster()->GetMasterExp(p->GetdwMasterIndex()) / MASTER_EXP_MAX;
				IME_DEBUG("masterguide1 master rate %u", Rate);
				Attack += p->GetdwAttack() * (Rate / 10000.0);
				Defence += p->GetdwDefence() * (Rate / 10000.0);
				Health += p->GetdwHealth() * (Rate / 10000.0);
			}
			if (masterguide2 != 0)
			{
				CCard *p = Find(masterguide2);
				if (p == NULL)
				{
					IME_ERROR("%u user have objid %u in first team but not in packet", m_pCRole->GetdwAccountId(), masterguide2); 
					return -1;
				}
				uint32_t Rate = MASTER_FORCE_BASE + MASTER_FORCE_GROW * m_pCRole->GetCMaster()->GetMasterExp(p->GetdwMasterIndex()) / MASTER_EXP_MAX;
				IME_DEBUG("masterguide2 master rate %u", Rate);
				Attack += p->GetdwAttack() * (Rate / 10000.0);
				Defence += p->GetdwDefence() * (Rate / 10000.0);
				Health += p->GetdwHealth() * (Rate / 10000.0);
			}
			if (masterguide3 != 0)
			{
				CCard *p = Find(masterguide3);
				if (p == NULL)
				{
					IME_ERROR("%u user have objid %u in first team but not in packet", m_pCRole->GetdwAccountId(), masterguide3); 
					return -1;
				}
				uint32_t Rate = MASTER_FORCE_BASE + MASTER_FORCE_GROW * m_pCRole->GetCMaster()->GetMasterExp(p->GetdwMasterIndex()) / MASTER_EXP_MAX;
				IME_DEBUG("masterguide3 master rate %u", Rate);
				Attack += p->GetdwAttack() * (Rate / 10000.0);
				Defence += p->GetdwDefence() * (Rate / 10000.0);
				Health += p->GetdwHealth() * (Rate / 10000.0);
			}
		}

		tempE.Attack = Attack;
		tempE.Defence = Defence;
		tempE.Health = Health;
		tempE.Crit = Crit;
		tempE.SkillPower = SkillPower;
	}
	else
	{
		if (m_MasterCard != 0)
		{
			uint32_t pet = m_MasterCard;

			CCard *pPet = Find(pet);
			if (pPet == NULL)
			{
				IME_ERROR("can not find this card %u in packet", pet);
				return -1;
			}
			STC_CONF_CARD_BASE *pConfCard = CConfCardBase::Find(pPet->GetdwIndex());
			if (pConfCard == NULL)
			{
				IME_ERROR("can not find card %u in packet", pPet->GetdwIndex());
				return -1;
			}

			Attack += pPet->GetdwAttack();
			Defence += pPet->GetdwDefence();
			Health += pPet->GetdwHealth();
			Crit += pPet->GetdwCritRate();
			//		Dodge += pPet->GetdwDodgeRate();
			//		Action += pPet->GetdwAction();

//			m_pCRole->GetclsTech()->TechAttack(Attack);	
//			m_pCRole->GetclsTech()->TechDefence(Defence);	
//			m_pCRole->GetclsTech()->TechHealth(Health);	

			tempM.Attack = Attack;
			tempM.Defence = Defence;
			tempM.Health = Health;
			tempM.Crit = Crit;
			//		tempM.Dodge = Dodge;
			//		tempM.Action = Action;
			tempM.Attribute = pPet->GetbyAttribute(); 
			tempM.SkillPro = pPet->GetdwSkillPro();
			tempM.SkillPower = pPet->GetdwSkillPower();
			tempM.DurationExp = pPet->GetdwDurationExp();
			tempM.DurationCoin = pPet->GetdwDurationCoin();
			tempM.ObjDrop = pPet->GetdwObjDrop();
			tempM.StrengthRecover = pPet->GetdwStrengthRecover();
			tempM.EnergyRecover = pPet->GetdwEnergyRecover();
			tempM.AddHarm = pPet->GetdwAddHarm();
			tempM.ReduceHarm = pPet->GetdwReduceHarm();
			tempM.AddCritHarm = pPet->GetdwAddCritHarm();

			if (flag)
			{
				if (m_pCRole->GetbyDuration() == DURATION_FRIEND_BOSS)
				{
					uint32_t multi = 100;
					if (time(NULL) <= pPet->GetdwMultiTime())
					{
						multi = pPet->GetbyMultiPower() * 100;
					}

					STC_CONF_VIP *pConfVip = NULL;
					pConfVip = CConfVip::Find(m_pCRole->GetwVipLv());
					if (pConfVip == NULL)
					{
						IME_ERROR("vip lv %u can not find in vip csv", m_pCRole->GetwVipLv());
					}
					else
					{
						multi += pConfVip->dwVipBossAttack;
					}

					tempM.Attack = tempM.Attack * multi / 100.0f;
				}
			}
		
			uint32_t weapon = m_MasterEquip.dwWeaponObjId;
			uint32_t armor = m_MasterEquip.dwArmorObjId;
			uint32_t jewel = m_MasterEquip.dwJewelObjId;
			uint32_t skill = m_MasterEquip.dwSkillObjId; 
			uint32_t accessories = m_MasterEquip.dwAccessoriesObjId;
			uint32_t masterguide1 = m_MasterEquip.dwMasterGuide1;
			uint32_t masterguide2 = m_MasterEquip.dwMasterGuide2;
			uint32_t masterguide3 = m_MasterEquip.dwMasterGuide3;

			if (jewel != 0)
			{
				CCard *p = Find(jewel);
				if (p == NULL)
				{
					IME_ERROR("%u user have objid %u in first team but not in packet", m_pCRole->GetdwAccountId(), jewel); 
					return -1;
				}

				if (p->GetdwEffectIndex() != 0)
				{
					STC_CONF_JEWEL_SKILL *pJewelSkill = CConfJewelSkill::Find(p->GetdwEffectIndex());
					if (pJewelSkill == NULL)
					{
						IME_ERROR("can not find jewelskill index %u", p->GetdwEffectIndex());
						return -1;
					}
					CJewelSkill::Init();
					CJewelSkill::JewelSkillFunc pSkillFunc = CJewelSkill::Find(pJewelSkill->wType);
					if (pSkillFunc == NULL)
					{
						IME_ERROR("can not find jewelskill type %u, jewelskill index %u", pJewelSkill->wType, p->GetdwEffectIndex());
						return -1;
					}
					IME_DEBUG("jewel skill %u, %u", pJewelSkill->wType, p->GetdwSkillPara());

					uint32_t para1 = 0;
//					if (p->GetdwCardGroup() == pConfCard->dwStrongSkill)
//					{
//						para1 = p->GetdwSkillPara() * pConfCard->bySkillFunc;
//					}
//					else
//					{
						para1 = p->GetdwSkillPara(); 
//					}
			
					if ((*pSkillFunc)(&tempM, para1) )
					{
						IME_ERROR("jewel skill exec fail index %u", pJewelSkill->dwIndex);
						return -1;
					}
				}
			}

			if (flag)
			{
				std::vector<int> vecRelationship;
				if (find(vecRelationshipEffect.begin(), vecRelationshipEffect.end(), pConfCard->dwRelation1) != vecRelationshipEffect.end())
				{
					vecRelationship.push_back(pConfCard->dwRelation1);
				}
				if (find(vecRelationshipEffect.begin(), vecRelationshipEffect.end(), pConfCard->dwRelation2) != vecRelationshipEffect.end())
				{
					vecRelationship.push_back(pConfCard->dwRelation2);
				}
				if (find(vecRelationshipEffect.begin(), vecRelationshipEffect.end(), pConfCard->dwRelation3) != vecRelationshipEffect.end())
				{
					vecRelationship.push_back(pConfCard->dwRelation3);
				}
				if (find(vecRelationshipEffect.begin(), vecRelationshipEffect.end(), pConfCard->dwRelation4) != vecRelationshipEffect.end())
				{
					vecRelationship.push_back(pConfCard->dwRelation4);
				}
				IME_DEBUG("master before card index %u attack %u, defence %u, health %u, crit %u, attribute %u, skillpro %u, SkillPower %u, durationexp %u, DurationCoin %u, ObjDrop %u, StrengthRecover %u, EnergyRecover %u, AddHarm %u, ReduceHarm %u, AddCritHarm %u", pPet->GetdwIndex(), tempM.Attack, tempM.Defence, tempM.Health, tempM.Crit, tempM.Attribute, tempM.SkillPro, tempM.SkillPower, tempM.DurationExp, tempM.DurationCoin, tempM.ObjDrop, tempM.StrengthRecover, tempM.EnergyRecover, tempM.AddHarm, tempM.ReduceHarm, tempM.AddCritHarm);
				CExSkill::ExecExSkillFuncByRelation(pPet->GetdwCardGroup(), vecRelationship, &tempM);
				IME_DEBUG("master before attack %u, defence %u, health %u, crit %u, attribute %u, skillpro %u, SkillPower %u, durationexp %u, DurationCoin %u, ObjDrop %u, StrengthRecover %u, EnergyRecover %u, AddHarm %u, ReduceHarm %u, AddCritHarm %u", tempM.Attack, tempM.Defence, tempM.Health, tempM.Crit, tempM.Attribute, tempM.SkillPro, tempM.SkillPower, tempM.DurationExp, tempM.DurationCoin, tempM.ObjDrop, tempM.StrengthRecover, tempM.EnergyRecover, tempM.AddHarm, tempM.ReduceHarm, tempM.AddCritHarm);
			}

			Attack = 0;
			Defence = 0;
			Health = 0;
			Crit = 0;
			Dodge = 0;
			Action = 0;
			SkillPower = 0;

			if (weapon != 0)
			{
				CCard *p = Find(weapon);
				if (p == NULL)
				{
					IME_ERROR("%u user have objid %u in first team but not in packet", m_pCRole->GetdwAccountId(), weapon); 
					return -1;
				}

				uint32_t para1 = 0;
				if (p->GetdwCardGroup() == pConfCard->dwStrongWeapon)
				{
					para1 = pConfCard->byWeaponFunc;
				}

				Attack += m_mapCards[weapon]->GetdwAttack() * ((100 + para1) / 100.0);
				Defence += m_mapCards[weapon]->GetdwDefence() * ((100 + para1) / 100.0);
				Health += m_mapCards[weapon]->GetdwHealth() * ((100 + para1) / 100.0);
				Crit += m_mapCards[weapon]->GetdwCritRate() * ((100 + para1) / 100.0);
				//			Dodge += m_mapCards[weapon]->GetdwDodgeRate();
				//			Action += m_mapCards[weapon]->GetdwAction();
			}
			if (armor != 0)
			{
				CCard *p = Find(armor);
				if (p == NULL)
				{
					IME_ERROR("%u user have objid %u in first team but not in packet", m_pCRole->GetdwAccountId(), armor); 
					return -1;
				}

				uint32_t para1 = 0;
				if (p->GetdwCardGroup() == pConfCard->dwStrongArmor)
				{
					para1 = pConfCard->byArmorFunc;
				}

				Attack += m_mapCards[armor]->GetdwAttack() * ((100 + para1) / 100.0);
				Defence += m_mapCards[armor]->GetdwDefence() * ((100 + para1) / 100.0);
				Health += m_mapCards[armor]->GetdwHealth() * ((100 + para1) / 100.0);
				Crit += m_mapCards[armor]->GetdwCritRate() * ((100 + para1) / 100.0);
				//			Dodge += m_mapCards[armor]->GetdwDodgeRate();
				//			Action += m_mapCards[armor]->GetdwAction();
			}
			if (accessories != 0)
			{
				CCard *p = Find(accessories);
				if (p == NULL)
				{
					IME_ERROR("%u user have objid %u in first team but not in packet", m_pCRole->GetdwAccountId(), armor); 
					return -1;
				}

				uint32_t para1 = 0;
				if (p->GetdwCardGroup() == pConfCard->dwStrongAccessories)
				{
					para1 = pConfCard->byAccessoriesFunc;
				}

				Attack += m_mapCards[accessories]->GetdwAttack() * ((100 + para1) / 100.0);
				Defence += m_mapCards[accessories]->GetdwDefence() * ((100 + para1) / 100.0);
				Health += m_mapCards[accessories]->GetdwHealth() * ((100 + para1) / 100.0);
				Crit += m_mapCards[accessories]->GetdwCritRate() * ((100 + para1) / 100.0);
				//			Dodge += m_mapCards[accessories]->GetdwDodgeRate();
				//			Action += m_mapCards[accessories]->GetdwAction();
			}
			if (skill != 0)
			{
				CCard *p = Find(skill);
				if (p == NULL)
				{
					IME_ERROR("%u user have objid %u in first team but not in packet", m_pCRole->GetdwAccountId(), skill); 
					return -1;
				}

				uint32_t para1 = 0;
				if (p->GetdwCardGroup() == pConfCard->dwStrongSkill)
				{
					para1 = pConfCard->bySkillFunc;
				}
				SkillPower += para1;
			}
			if (m_pCRole->GetCMaster() != NULL)
			{
				if (masterguide1 != 0)
				{
					CCard *p = Find(masterguide1);
					if (p == NULL)
					{
						IME_ERROR("%u user have objid %u in first team but not in packet", m_pCRole->GetdwAccountId(), masterguide1); 
						return -1;
					}
					uint32_t Rate = MASTER_FORCE_BASE + MASTER_FORCE_GROW * m_pCRole->GetCMaster()->GetMasterExp(p->GetdwMasterIndex()) / MASTER_EXP_MAX;
					Attack += p->GetdwAttack() * (Rate / 10000.0);
					Defence += p->GetdwDefence() * (Rate / 10000.0);
					Health += p->GetdwHealth() * (Rate / 10000.0);
				}
				if (masterguide2 != 0)
				{
					CCard *p = Find(masterguide2);
					if (p == NULL)
					{
						IME_ERROR("%u user have objid %u in first team but not in packet", m_pCRole->GetdwAccountId(), masterguide2); 
						return -1;
					}
					uint32_t Rate = MASTER_FORCE_BASE + MASTER_FORCE_GROW * m_pCRole->GetCMaster()->GetMasterExp(p->GetdwMasterIndex()) / MASTER_EXP_MAX;
					Attack += p->GetdwAttack() * (Rate / 10000.0);
					Defence += p->GetdwDefence() * (Rate / 10000.0);
					Health += p->GetdwHealth() * (Rate / 10000.0);
				}
				if (masterguide3 != 0)
				{
					CCard *p = Find(masterguide3);
					if (p == NULL)
					{
						IME_ERROR("%u user have objid %u in first team but not in packet", m_pCRole->GetdwAccountId(), masterguide3); 
						return -1;
					}
					uint32_t Rate = MASTER_FORCE_BASE + MASTER_FORCE_GROW * m_pCRole->GetCMaster()->GetMasterExp(p->GetdwMasterIndex()) / MASTER_EXP_MAX;
					Attack += p->GetdwAttack() * (Rate / 10000.0);
					Defence += p->GetdwDefence() * (Rate / 10000.0);
					Health += p->GetdwHealth() * (Rate / 10000.0);
				}
			}

			tempE.Attack = Attack;
			tempE.Defence = Defence;
			tempE.Health = Health;
			tempE.Crit = Crit;
			tempE.SkillPower = SkillPower;
		}
		else
		{
		}
	}
	return 0;
}

int CCardPacket::Calculate(bool IsUpdate)
{
	uint32_t Attack = 0;
	uint32_t Defence = 0;
	uint32_t Health = 0;
	uint32_t Crit = 0;
	uint32_t Dodge = 0;
	uint32_t Action = 0;
	
	m_vecItemEquip1.clear();
	if (m_vecItemEquip1.size() < m_vecAttackTeam.size())
	{
		m_vecItemEquip1.resize(m_vecAttackTeam.size());
	}
	
	if (m_vecEquip1.size() < m_vecAttackTeam.size())
	{
		m_vecEquip1.resize(m_vecAttackTeam.size());
	}

	for (int i = 0; i < m_vecAttackTeam.size(); i++)
	{	
		/* 
		Attack = 0;
		Defence = 0;
		Health = 0;
		Crit = 0;
		Dodge = 0;
		Action = 0;

		uint32_t pet = m_vecAttackTeam[i];
		if (pet == 0)
		{
			itemequip_t temp;
			temp.Attack = Attack;
			temp.Defence = Defence;
			temp.Health = Health;
			temp.Crit = Crit;
			temp.Dodge = Dodge;
			temp.Action = Action;
			temp.Attribute = 0;
			m_vecItemEquip1[i] = temp;
			continue;
		}

		CCard *pPet = Find(pet);
		if (pPet == NULL)
		{
			IME_ERROR("can not find this card %u in packet", pet);
			return -1;
		}

		Attack += pPet->GetdwAttack();
		Defence += pPet->GetdwDefence();
		Health += pPet->GetdwHealth();
		Crit += pPet->GetdwCritRate();

		m_pCRole->GetclsTech()->TechAttack(Attack);	
		m_pCRole->GetclsTech()->TechDefence(Defence);	
		m_pCRole->GetclsTech()->TechHealth(Health);	

		itemequip_t temp;
		temp.Attack = Attack;
		temp.Defence = Defence;
		temp.Health = Health;
		temp.Crit = Crit;
		temp.Attribute = pPet->GetbyAttribute(); 
		temp.SkillPro = pPet->GetdwSkillPro();
		temp.SkillPower = pPet->GetdwSkillPower();
		temp.DurationExp = pPet->GetdwDurationExp();
		temp.DurationCoin = pPet->GetdwDurationCoin();
		temp.ObjDrop = pPet->GetdwObjDrop();
		temp.StrengthRecover = pPet->GetdwStrengthRecover();
		temp.EnergyRecover = pPet->GetdwEnergyRecover();
		temp.AddHarm = pPet->GetdwAddHarm();
		temp.ReduceHarm = pPet->GetdwReduceHarm();
		temp.AddCritHarm = pPet->GetdwAddCritHarm();
		m_vecItemEquip1[i] = temp;

		uint32_t weapon = m_vecEquip1[i].dwWeaponObjId;
		uint32_t armor = m_vecEquip1[i].dwArmorObjId;
		uint32_t jewel = m_vecEquip1[i].dwJewelObjId;
		uint32_t accessories = m_vecEquip1[i].dwAccessoriesObjId;

		if (jewel != 0)
		{
			CCard *p = Find(jewel);
			if (p == NULL)
			{
				IME_ERROR("%u user have objid %u in first team but not in packet", m_pCRole->GetdwAccountId(), jewel); 
				return -1;
			}

			if (p->GetdwEffectIndex() != 0)
			{
				STC_CONF_JEWEL_SKILL *pJewelSkill = CConfJewelSkill::Find(p->GetdwEffectIndex());
				if (pJewelSkill == NULL)
				{
					IME_ERROR("can not find jewelskill index %u", p->GetdwEffectIndex());
					return -1;
				}
				CJewelSkill::Init();
				CJewelSkill::JewelSkillFunc pSkillFunc = CJewelSkill::Find(pJewelSkill->wType);
				if (pSkillFunc == NULL)
				{
					IME_ERROR("can not find jewelskill type %u, jewelskill index %u", pJewelSkill->wType, p->GetdwEffectIndex());
					return -1;
				}
				IME_DEBUG("jewel skill %u, base %u all %u", pJewelSkill->wType, pJewelSkill->para1, p->GetdwSkillPara());
				if ((*pSkillFunc)(&m_vecItemEquip1[i], p->GetdwSkillPara()) )
				{
					IME_ERROR("jewel skill exec fail index %u", pJewelSkill->dwIndex);
					return -1;
				}
			}
		}

		Attack = m_vecItemEquip1[i].Attack;
		Defence = m_vecItemEquip1[i].Defence;
		Health = m_vecItemEquip1[i].Health;
		Crit = m_vecItemEquip1[i].Crit;

		if (weapon != 0)
		{
			CCard *p = Find(weapon);
			if (p == NULL)
			{
				IME_DEBUG("%u user have objid %u in first team but not in packet", m_pCRole->GetdwAccountId(), weapon); 
				return -1;
			}

			Attack += m_mapCards[weapon]->GetdwAttack();
			Defence += m_mapCards[weapon]->GetdwDefence();
			Health += m_mapCards[weapon]->GetdwHealth();
			Crit += m_mapCards[weapon]->GetdwCritRate();
		}
		if (armor != 0)
		{
			CCard *p = Find(armor);
			if (p == NULL)
			{
				IME_DEBUG("%u user have objid %u in first team but not in packet", m_pCRole->GetdwAccountId(), armor); 
				return -1;
			}

			Attack += m_mapCards[armor]->GetdwAttack();
			Defence += m_mapCards[armor]->GetdwDefence();
			Health += m_mapCards[armor]->GetdwHealth();
			Crit += m_mapCards[armor]->GetdwCritRate();
		}
		if (accessories != 0)
		{
			CCard *p = Find(accessories);
			if (p == NULL)
			{
				IME_DEBUG("%u user have objid %u in first team but not in packet", m_pCRole->GetdwAccountId(), armor); 
				return -1;
			}

			Attack += m_mapCards[accessories]->GetdwAttack();
			Defence += m_mapCards[accessories]->GetdwDefence();
			Health += m_mapCards[accessories]->GetdwHealth();
			Crit += m_mapCards[accessories]->GetdwCritRate();
		}

		m_vecItemEquip1[i].Attack = Attack;
		m_vecItemEquip1[i].Defence = Defence;
		m_vecItemEquip1[i].Health = Health;
		m_vecItemEquip1[i].Crit = Crit;
		*/
		itemequip_t M,E;
		if (EquipCardCalForDuration(M, E, i, 0))
		{
			IME_ERROR("cal data fail position %u accountid %u", i, m_pCRole->GetdwAccountId());
			return -1;
		}
		m_vecItemEquip1[i].Attack = M.Attack + E.Attack;
		m_vecItemEquip1[i].Defence = M.Defence + E.Defence;
		m_vecItemEquip1[i].Health = M.Health + E.Health;
		m_vecItemEquip1[i].Crit = M.Crit + E.Crit;
		m_vecItemEquip1[i].Dodge = M.Dodge + E.Dodge;
		m_vecItemEquip1[i].Action = M.Action + E.Action;
		m_vecItemEquip1[i].Attribute = M.Attribute;
		m_vecItemEquip1[i].SkillPro = M.SkillPro + E.SkillPro;
		m_vecItemEquip1[i].SkillPower = M.SkillPower + E.SkillPower;
		m_vecItemEquip1[i].DurationExp = M.DurationExp + E.DurationExp;
		m_vecItemEquip1[i].DurationCoin = M.DurationCoin + E.DurationCoin;
		m_vecItemEquip1[i].ObjDrop = M.ObjDrop + E.ObjDrop;
		m_vecItemEquip1[i].StrengthRecover = M.StrengthRecover + E.StrengthRecover;
		m_vecItemEquip1[i].EnergyRecover = M.EnergyRecover + E.EnergyRecover;
		m_vecItemEquip1[i].AddHarm = M.AddHarm + E.AddHarm;
		m_vecItemEquip1[i].ReduceHarm = M.ReduceHarm + E.ReduceHarm;
		m_vecItemEquip1[i].AddCritHarm = M.AddCritHarm + E.AddCritHarm;
	}

	Attack = 0;
	Defence = 0;
	Health = 0;
	Crit = 0;
	Dodge = 0;
	Action = 0;
	if (m_MasterCard != 0)
	{
		itemequip_t M,E;
		if (EquipCardCalForDuration(M, E, -1, 0))
		{
			IME_ERROR("cal data fail position -1 accountid %u", m_pCRole->GetdwAccountId());
			return -1;
		}
		m_MasterItemEquip.Attack = M.Attack + E.Attack;
		m_MasterItemEquip.Defence = M.Defence + E.Defence;
		m_MasterItemEquip.Health = M.Health + E.Health;
		m_MasterItemEquip.Crit = M.Crit + E.Crit;
		m_MasterItemEquip.Dodge = M.Dodge + E.Dodge;
		m_MasterItemEquip.Action = M.Action + E.Action;
		m_MasterItemEquip.Attribute = M.Attribute;
		m_MasterItemEquip.SkillPro = M.SkillPro + E.SkillPro;
		m_MasterItemEquip.SkillPower = M.SkillPower + E.SkillPower;
		m_MasterItemEquip.DurationExp = M.DurationExp + E.DurationExp;
		m_MasterItemEquip.DurationCoin = M.DurationCoin + E.DurationCoin;
		m_MasterItemEquip.ObjDrop = M.ObjDrop + E.ObjDrop;
		m_MasterItemEquip.StrengthRecover = M.StrengthRecover + E.StrengthRecover;
		m_MasterItemEquip.EnergyRecover = M.EnergyRecover + E.EnergyRecover;
		m_MasterItemEquip.AddHarm = M.AddHarm + E.AddHarm;
		m_MasterItemEquip.ReduceHarm = M.ReduceHarm + E.ReduceHarm;
		m_MasterItemEquip.AddCritHarm = M.AddCritHarm + E.AddCritHarm;
	}
	else
	{
		m_MasterItemEquip.Attack = Attack;
		m_MasterItemEquip.Defence = Defence;
		m_MasterItemEquip.Health = Health;
		m_MasterItemEquip.Crit = Crit;
		m_MasterItemEquip.Dodge = Dodge;
		m_MasterItemEquip.Action = Action;
		m_MasterItemEquip.Attribute = 0;
	}
//	IME_DEBUG("master objid %u attack %u defence %u health %u", m_MasterCard, m_MasterItemEquip.Attack, m_MasterItemEquip.Defence, m_MasterItemEquip.Health);	
		
    if (IsUpdate)
	{
		HandlerEquipCalInfo(0);
	}

	return 0;
}

//int CCardPacket::GetEquipCards(std::vector<uint32_t> &vecEquip, std::vector<uint32_t> &vecIndex)
//{
//	int i;
//		for (i = 0; i < m_vecAttackTeam.size(); i++)
//		{
//			if (m_vecAttackTeam[i] != 0)
//			{
//				vecEquip.push_back(m_vecAttackTeam[i]);
//				if (m_mapCards.find(m_vecAttackTeam[i]) == m_mapCards.end())
//				{
//					return -1;
//				}
//				vecIndex.push_back(m_mapCards[m_vecAttackTeam[i]]->GetdwIndex());
//			}
//			
//		}
//		return 0;
//}
//
//
int CCardPacket::GetEquippCards(std::vector<CCard *> &vecEquip)
{
	int i;
		CCard *p = Find(m_MasterCard);
		if (p == NULL)
		{
			return -1;
		}
		vecEquip.push_back(p);
		for (i = 0; i < m_vecAttackTeam.size(); i++)
		{
			if (m_vecAttackTeam[i] != 0)
			{
				CCard *p = Find(m_vecAttackTeam[i]);
				if (p == NULL)
				{
					return -1;
				}
				vecEquip.push_back(p);
			}
			
		}
		return 0;
}

//int CCardPacket::LoadOnlyEquip(uint32_t userid, uint8_t currentteam, std::vector<uint32_t> &vecEquip, std::vector<uint32_t> &vecIndex)
//{
//    m_bIsInit = false;
//    std::string attack_team;
//    std::string defense_team;
//    std::string suit_attribute;
//    std::string cards;
//	std::string equip1;
//	std::string equip2;
//    if (!CDBCtrl::GetUserCardPacketData(attack_team, defense_team, suit_attribute, cards, userid, equip1, equip2))
//    {
//		IME_DEBUG("load data fail from db");
//        return -1;
//    }
//
//	{
//		if(cards.size() == 0)
//			return 0;
//		ByteBuffer temp;
//		temp.append(cards.data(), cards.size());
//		temp >> m_dwCardNumber;
//		uint32_t tempnumber = 0;
//		for( ;tempnumber < m_dwCardNumber; tempnumber++)
//		{
//			CCard *pCard = new(std::nothrow) CCard();          
//			//todo log?
//			assert(pCard != NULL);
//			pCard->SetCRole(m_pCRole);
//			pCard->DeserializeDB(temp); 
//			m_mapCards[pCard->GetdwObjId()] = pCard;
//		}
//
//	}
//
//    StrSplit(attack_team, ",", m_vecAttackTeam);
//    StrSplit(defense_team, ",", m_vecDefenseTeam);
//    StrSplit(suit_attribute, ",", m_vecSuitAttribute);
//
//	std::vector<std::string> vectemp;
//	std::vector<uint32_t> vectempint;
//	int i;
//	if (currentteam == 0)
//	{
//		for (; i < m_vecAttackTeam.size(); i++)
//		{
//			if (m_vecAttackTeam[i] != 0)
//			{
//				vecEquip.push_back(m_vecAttackTeam[i]);
//				if (m_mapCards.find(m_vecAttackTeam[i]) == m_mapCards.end())
//				{
//					return -1;
//				}
//				vecIndex.push_back(m_mapCards[m_vecAttackTeam[i]]->GetdwIndex());
//			}
//			
//		}
//		return 0;
//	}
//	else if (currentteam == 1)
//	{
//		for (; i < m_vecDefenseTeam.size(); i++)
//		{
//			if (m_vecDefenseTeam[i] != 0)
//			{
//				vecEquip.push_back(m_vecDefenseTeam[i]);
//				if (m_mapCards.find(m_vecDefenseTeam[i]) == m_mapCards.end())
//				{
//					return -1;
//				}
//				vecIndex.push_back(m_mapCards[m_vecDefenseTeam[i]]->GetdwIndex());
//			}
//		}
//		return 0;
//	}
//	else
//	{
//		IME_DEBUG("unknow currentteam %u", currentteam);
//		return -1;
//	}
//}

// must be careful  this method only load part of data. must not call savedata function after this function
int CCardPacket::LoadForOffLogin(uint32_t userid)
{
    std::string attack_team;
    std::string defense_team;
//    std::string suit_attribute;
    std::string lineup;
    std::string cards;
	std::string equip1;
//	std::string equip2;
	std::string master;
	std::string equip_master;
    if (!CDBCtrl::GetUserCardPacketDataOffOnline(attack_team, lineup, cards, userid, equip1, master, equip_master))
    {
        m_bIsInit = false;
		IME_ERROR("load data fail from db");
        return -1;
    }
    StrSplit(lineup, ",", m_vecLineUp);
	m_vecLineUp.resize(MAX_CARD_CAPACITY);

	std::vector<uint32_t> vecMaster;
	StrSplit(master, ",", vecMaster);
	if (vecMaster.size() == 0)
	{
		m_MasterCard = 0;
	}
	else
	{
		m_MasterCard = vecMaster[0];
	}

    StrSplit(attack_team, ",", m_vecAttackTeam);
//    StrSplit(defense_team, ",", m_vecDefenseTeam);
//    StrSplit(suit_attribute, ",", m_vecSuitAttribute);
	for (int k = 0; k < m_vecAttackTeam.size(); k++)
	{
		IME_DEBUG("1st team %u objid [%u]", k, m_vecAttackTeam[k]);
		m_setobjid.insert(m_vecAttackTeam[k]);
	}
	m_setobjid.insert(m_MasterCard);
	m_setobjid.erase(0);
//	for (int k = 0; k < m_vecDefenseTeam.size(); k++)
//	{
//		IME_DEBUG("2nd team %u objid [%u]", k, m_vecDefenseTeam[k]);
//	}

	std::vector<std::string> vectemp;
	std::vector<uint32_t> vectempint;
	int i;
	CUtil::StrSplit(equip1, "|", vectemp);	
	for (i = 0; i < vectemp.size(); i++)
	{
		StrSplit(vectemp[i], ",", vectempint);
		if (vectempint.size() < 8)
			vectempint.resize(8, 0);
		if (vectempint.size() != 8)
		{
			IME_ERROR("cardpacket equip column is not 8");
			return -1;
		}
		equip_t tempequip;
		tempequip.dwWeaponObjId = vectempint[0];
	   	tempequip.dwArmorObjId = vectempint[1];
	   	tempequip.dwJewelObjId = vectempint[2];
	   	tempequip.dwAccessoriesObjId = vectempint[3];
	   	tempequip.dwSkillObjId = vectempint[4];
		tempequip.dwMasterGuide1 = vectempint[5];
		tempequip.dwMasterGuide2 = vectempint[6];
		tempequip.dwMasterGuide3 = vectempint[7];

		m_vecEquip1.push_back(tempequip);
//		if (vectempint[0] != 0)
//		m_setForEquip1.insert(vectempint[0]);
//		if (vectempint[1] != 0)
//		m_setForEquip1.insert(vectempint[1]);
//		if (vectempint[2] != 0)
//		m_setForEquip1.insert(vectempint[2]);
		m_setForEquip1.insert(vectempint[0]);
		m_setForEquip1.insert(vectempint[1]);
		m_setForEquip1.insert(vectempint[2]);
		m_setForEquip1.insert(vectempint[3]);
		m_setForEquip1.insert(vectempint[4]);
		m_setForEquip1.insert(vectempint[5]);
		m_setForEquip1.insert(vectempint[6]);
		m_setForEquip1.insert(vectempint[7]);
		m_setForEquip1.erase(0);

		IME_DEBUG("1st %u weapon armor jewel %u %u %u", i, vectempint[0], vectempint[1], vectempint[2]);
	}
	StrSplit(equip_master, ",", vectempint); 
	if (vectempint.size() != 0)
	{
		if (vectempint.size() < 8)
			vectempint.resize(8, 0);
		if (vectempint.size() != 8)
		{
			IME_ERROR("cardpacket equip column is not 8");
			return -1;
		}
		m_MasterEquip.dwWeaponObjId = vectempint[0];
		m_MasterEquip.dwArmorObjId = vectempint[1];
		m_MasterEquip.dwJewelObjId = vectempint[2];
		m_MasterEquip.dwAccessoriesObjId = vectempint[3];
		m_MasterEquip.dwSkillObjId = vectempint[4];
		m_MasterEquip.dwMasterGuide1 = vectempint[5];
		m_MasterEquip.dwMasterGuide2 = vectempint[6];
		m_MasterEquip.dwMasterGuide3 = vectempint[7];
		m_setForEquip1.insert(vectempint[0]);
		m_setForEquip1.insert(vectempint[1]);
		m_setForEquip1.insert(vectempint[2]);
		m_setForEquip1.insert(vectempint[3]);
		m_setForEquip1.insert(vectempint[4]);
		m_setForEquip1.insert(vectempint[5]);
		m_setForEquip1.insert(vectempint[6]);
		m_setForEquip1.insert(vectempint[7]);
		m_setForEquip1.erase(0);
	}

//	CUtil::StrSplit(equip2, "|", vectemp);	
//	for (i = 0; i < vectemp.size(); i++)
//	{
//		StrSplit(vectemp[i], ",", vectempint);
//		if (vectempint.size() != 3)
//		{
//			IME_ERROR("cardpacket equip column is not 3");
//			return -1;
//		}
//		equip_t tempequip(vectempint[0], vectempint[1], vectempint[2]);
//		m_vecEquip2.push_back(tempequip);
//		if (vectempint[0] != 0)
//		m_setForEquip2.insert(vectempint[0]);
//		if (vectempint[1] != 0)
//		m_setForEquip2.insert(vectempint[1]);
//		if (vectempint[2] != 0)
//		m_setForEquip2.insert(vectempint[2]);
//		IME_DEBUG("2nd %u weapon armor jewel %u %u %u", i, vectempint[0], vectempint[1], vectempint[2]);
//	}
    
    
    deserialize(cards);

	m_vecView = m_vecLineUp;
	for (int i = 0; i < m_vecView.size(); )
	{
		if (m_vecView[i] == 0)
		{
			m_vecView.erase(m_vecView.begin() + i);
		}
		else
		{
			i++;
		}
	}

	if (EquipCardInfo())
	{
		IME_ERROR("card equip fail");
		return false;
	}


    if (Calculate(false))
	{
		IME_ERROR("card packet calculate fail");
		return false;
	}

    return 0;
}

int CCardPacket::Load(uint32_t userid)
{
    std::string attack_team;
//    std::string defense_team;
//    std::string suit_attribute;
	std::string lineup;
    std::string cards;
	std::string equip1;
//	std::string equip2;
	std::string master;
	std::string equip_master;
    if (!CDBCtrl::GetUserCardPacketData(attack_team, lineup, cards, userid, equip1, master, equip_master))
    {
        m_bIsInit = false;
		IME_ERROR("load data fail from db");
        return -1;
    }
    StrSplit(attack_team, ",", m_vecAttackTeam);
//    StrSplit(defense_team, ",", m_vecDefenseTeam);
//    StrSplit(suit_attribute, ",", m_vecSuitAttribute);

	std::vector<uint32_t> vecMaster;
	StrSplit(master, ",", vecMaster);
	if (vecMaster.size() == 0)
	{
		m_MasterCard = 0;
	}
	else
	{
		m_MasterCard = vecMaster[0];
	}

    StrSplit(lineup, ",", m_vecLineUp);
	m_vecLineUp.resize(MAX_CARD_CAPACITY);
	for (int k = 0; k < m_vecAttackTeam.size(); k++)
	{
		m_setobjid.insert(m_vecAttackTeam[k]);
		IME_DEBUG("1st team %u objid [%u]", k, m_vecAttackTeam[k]);
	}
	m_setobjid.insert(m_MasterCard);
	m_setobjid.erase(0);
	
//	std::set<uint32_t>::iterator it = m_setobjid.begin();
//	IME_ERROR("set data dump");
//	for ( ; it != m_setobjid.end(); it++)
//	{
//		IME_ERROR("set objid %u", *it);
//	}	

//	for (int k = 0; k < m_vecDefenseTeam.size(); k++)
//	{
//		IME_DEBUG("2nd team %u objid [%u]", k, m_vecDefenseTeam[k]);
//	}

	std::vector<std::string> vectemp;
	std::vector<uint32_t> vectempint;
	int i;
	CUtil::StrSplit(equip1, "|", vectemp);	
	for (i = 0; i < vectemp.size(); i++)
	{
		StrSplit(vectemp[i], ",", vectempint);
		if (vectempint.size() < 8)
			vectempint.resize(8, 0);
		if (vectempint.size() != 8)
		{
			IME_ERROR("cardpacket equip column is not 8");
			return -1;
		}
//		equip_t tempequip(vectempint[0], vectempint[1], vectempint[2]);
		equip_t tempequip;
		tempequip.dwWeaponObjId = vectempint[0];
		tempequip.dwArmorObjId = vectempint[1];
		tempequip.dwJewelObjId = vectempint[2];
		tempequip.dwAccessoriesObjId = vectempint[3];
		tempequip.dwSkillObjId = vectempint[4];
		tempequip.dwMasterGuide1 = vectempint[5];
		tempequip.dwMasterGuide2 = vectempint[6];
		tempequip.dwMasterGuide3 = vectempint[7];

		m_vecEquip1.push_back(tempequip);
//		if (vectempint[0] != 0)
//		m_setForEquip1.insert(vectempint[0]);
//		if (vectempint[1] != 0)
//		m_setForEquip1.insert(vectempint[1]);
//		if (vectempint[2] != 0)
//		m_setForEquip1.insert(vectempint[2]);
		m_setForEquip1.insert(vectempint[0]);
		m_setForEquip1.insert(vectempint[1]);
		m_setForEquip1.insert(vectempint[2]);
		m_setForEquip1.insert(vectempint[3]);
		m_setForEquip1.insert(vectempint[4]);
		m_setForEquip1.insert(vectempint[5]);
		m_setForEquip1.insert(vectempint[6]);
		m_setForEquip1.insert(vectempint[7]);
		m_setForEquip1.erase(0);
		IME_DEBUG("1st %u weapon armor jewel %u %u %u", i, vectempint[0], vectempint[1], vectempint[2]);
	}
		StrSplit(equip_master, ",", vectempint);
		if (vectempint.size() != 0)
		{
			if (vectempint.size() < 8)
				vectempint.resize(8, 0);
			if (vectempint.size() != 8)
			{
				IME_ERROR("cardpacket equip column is not 8");
				return -1;
			}
			m_MasterEquip.dwWeaponObjId = vectempint[0];
			m_MasterEquip.dwArmorObjId = vectempint[1];
			m_MasterEquip.dwJewelObjId = vectempint[2];
			m_MasterEquip.dwAccessoriesObjId = vectempint[3];
			m_MasterEquip.dwSkillObjId = vectempint[4];
			m_MasterEquip.dwMasterGuide1 = vectempint[5];
			m_MasterEquip.dwMasterGuide2 = vectempint[6];
			m_MasterEquip.dwMasterGuide3 = vectempint[7];

			m_setForEquip1.insert(vectempint[0]);
			m_setForEquip1.insert(vectempint[1]);
			m_setForEquip1.insert(vectempint[2]);
			m_setForEquip1.insert(vectempint[3]);
			m_setForEquip1.insert(vectempint[4]);
			m_setForEquip1.insert(vectempint[5]);
			m_setForEquip1.insert(vectempint[6]);
			m_setForEquip1.insert(vectempint[7]);
			m_setForEquip1.erase(0);
		}
//	CUtil::StrSplit(equip2, "|", vectemp);	
//	for (i = 0; i < vectemp.size(); i++)
//	{
//		StrSplit(vectemp[i], ",", vectempint);
//		if (vectempint.size() != 3)
//		{
//			IME_ERROR("cardpacket equip column is not 3");
//			return -1;
//		}
//		equip_t tempequip(vectempint[0], vectempint[1], vectempint[2]);
//		m_vecEquip2.push_back(tempequip);
//		if (vectempint[0] != 0)
//		m_setForEquip2.insert(vectempint[0]);
//		if (vectempint[1] != 0)
//		m_setForEquip2.insert(vectempint[1]);
//		if (vectempint[2] != 0)
//		m_setForEquip2.insert(vectempint[2]);
//		IME_DEBUG("2nd %u weapon armor jewel %u %u %u", i, vectempint[0], vectempint[1], vectempint[2]);
//	}

    deserialize(cards);

	m_vecView = m_vecLineUp;
	for (int i = 0; i < m_vecView.size(); )
	{
		if (m_vecView[i] == 0)
		{
			m_vecView.erase(m_vecView.begin() + i);
		}
		else
		{
			i++;
		}
	}
    

	if (EquipCardInfo())
	{
		IME_ERROR("card equip fail");
		return -1;
	}


    if (Calculate(false))
	{
		IME_ERROR("card packet calculate fail");
		return -1;
	}

    m_bIsInit = true;
    return 0;
}

int CCardPacket::SingleCardAddUpdate(const uint32_t &objid)
{
	CCard *p = Find(objid);
	if (p == NULL)
	{
		IME_ERROR("objid %u is not in packet", objid);
		return -1;
	}

    WorldPacket pkg(CMD_SC_ROLE_CARD_PACKET_CHANGE);
    pkg << (uint32_t)1;
    pkg << (uint8_t)ADD;
    p->SerializeClient(pkg);     
    m_pCRole->SendPacket(&pkg);
	return 0;
}

int CCardPacket::SingleCardDelUpdate(uint32_t objid)
{
	CCard *p = Find(objid);
//	if (p == NULL)
//	{
//		IME_DEBUG("objid %u is not in packet", objid);
//		return -1;
//	}

    WorldPacket pkg(CMD_SC_ROLE_CARD_PACKET_CHANGE);
    pkg << (uint32_t)1;
    pkg << (uint8_t)DEL;
	pkg << (uint32_t)objid;
    m_pCRole->SendPacket(&pkg);
	return 0;
}

//int CCardPacket::HandlerCardAdd(uint32_t cardindex)
//{
//    WorldPacket pkg(CMD_SC_ROLE_CARD_PACKET_CHANGE);
//    pkg << (uint32_t)1;
//    pkg << (uint8_t)ADD;
//    uint32_t objid;
//    if (CardAddByPara(cardindex, objid))
//		return -1;
//	CCard *p = Find(objid);
//	if (p == NULL)
//	{
//		IME_DEBUG("objid %u is not in packet", objid);
//		return -1;
//	}
//    p->SerializeClient(pkg);
//    m_pCRole->SendPacket(&pkg);
//	return 0;
//}
int CCardPacket::CardAddByParaNoAutoAwake(uint32_t cardindex, uint32_t &objid, uint32_t &opt, int source, bool IsUpdate, int Awake)
{
	SAddCardStu stu;
	stu.cardindex = cardindex;
	stu.Awake = Awake;
	stu.IsAutoAwake = false;
	stu.IsUpdate = IsUpdate;
	stu.exp = 0;
	stu.breaknumber = 0;
	stu.attackplus = 0;
	stu.defenceplus = 0;
	stu.healthplus = 0; 
	stu.source = source;

	stu.objid = objid;
	stu.opt = opt;
	if (CardAdd(stu))
		return -1;
	objid = stu.objid;
	opt = stu.opt;
    return 0;
}

int CCardPacket::CardAddByPara(uint32_t cardindex, uint32_t &objid, uint32_t &opt, int source, bool IsUpdate, int Awake)
{
	SAddCardStu stu;
	stu.cardindex = cardindex;
	stu.Awake = Awake;
	stu.IsAutoAwake = true;
	stu.IsUpdate = IsUpdate;
	stu.exp = 0;
	stu.breaknumber = 0;
	stu.attackplus = 0;
	stu.defenceplus = 0;
	stu.healthplus = 0; 
	stu.source = source;

	stu.objid = objid;
	stu.opt = opt;
	if (CardAdd(stu))
		return -1;
	objid = stu.objid;
	opt = stu.opt;
    return 0;
}

int CCardPacket::CardAddByParaWithExp(uint32_t cardindex, uint32_t &objid, uint32_t exp, uint32_t breaknumber, uint32_t awake, int source, bool IsUpdate)
{
	SAddCardStu stu;
	stu.cardindex = cardindex;
	stu.Awake = awake;
	stu.IsAutoAwake = false;
	stu.IsUpdate = IsUpdate;
	stu.exp = exp;
	stu.breaknumber = breaknumber;
	stu.attackplus = 0;
	stu.defenceplus = 0;
	stu.healthplus = 0; 
	stu.source = source;

	stu.objid = objid;
	stu.opt = 0;
	if (CardAdd(stu))
		return -1;
	objid = stu.objid;
    return 0;
}

int CCardPacket::CardAddByParaWithPlus(uint32_t cardindex, uint32_t &objid, int &attack, int &defence, int &health, int source, bool IsUpdate)
{
	SAddCardStu stu;
	stu.cardindex = cardindex;
	stu.Awake = E_CR_NORMAL;
	stu.IsAutoAwake = true; 
	stu.IsUpdate = IsUpdate;
	stu.exp = 0;
	stu.breaknumber = 0;
	stu.attackplus = attack;
	stu.defenceplus = defence;
	stu.healthplus = health;
	stu.source = source;

	stu.objid = objid;
	stu.opt = 0;
	if (CardAdd(stu))
		return -1;
	objid = stu.objid;
	attack = stu.attackplus;
	defence = stu.defenceplus;
	health = stu.healthplus;
    return 0;
}

int CCardPacket::CardAddByParaWithRandPlus(uint32_t cardindex, uint32_t &objid, int pluspro, int source, bool IsUpdate)
{
	SAddCardStu stu;
	stu.cardindex = cardindex;
	stu.Awake = E_CR_NORMAL;
	stu.IsAutoAwake = true; 
	stu.IsUpdate = IsUpdate;
	stu.exp = 0;
	stu.breaknumber = 0;
	stu.attackplus = 0;
	stu.defenceplus = 0;
	stu.healthplus = 0;
	stu.source = source;

	if (CUtil::RandEither(pluspro, 10000))
	{
		int ret = CUtil::RandByRange(0, 3);
		if (ret == 0)
		{
			stu.attackplus = 1;
		}
		else if (ret == 1)
		{
			stu.defenceplus = 1;
		}
		else
		{
			stu.healthplus = 1;
		}
	}	

	stu.objid = objid;
	stu.opt = 0;
	if (CardAdd(stu))
		return -1;
	objid = stu.objid;
    return 0;
}

int CCardPacket::CardAdd(SAddCardStu &stu)
{
	uint32_t cardindex = stu.cardindex;
	int Awake = stu.Awake;
	bool IsAutoAwake = stu.IsAutoAwake;
	bool IsUpdate = stu.IsUpdate;
	uint32_t exp = stu.exp;
	uint32_t breakcount = stu.breaknumber;

	uint32_t objid = stu.objid;
	uint32_t opt = stu.opt;

	opt = 0;
	STC_CONF_CARD_BASE *pCardConf = CConfCardBase::Find(cardindex);
	if (pCardConf == NULL)
	{
		IME_ERROR("can not find cardindex %u in csv", cardindex);
		IME_CHAR_LOG("resource [status=fail,res=card,opt=add,character=%u,index=%u,reason=cannot find index in csv,source=%d]", m_pCRole->GetdwAccountId(), cardindex, stu.source);
		return -1;
	}
	
	if (IsFull(1, cardindex))
	{
		IME_ERROR("card capacity is full %u, role limit %u, equip limit %u", m_dwCardNumberRole, GlobalConfig::RoleCardLimit, GlobalConfig::EquipCardLimit); 
		IME_CHAR_LOG("resource [status=fail,res=card,opt=add,character=%u,index=%u,reason=overload capacity,source=%d]", m_pCRole->GetdwAccountId(), cardindex, stu.source);
		return -1;
	}

	CCard *pCard = new(std::nothrow) CCard();          
	//todo log?
	assert(pCard != NULL);
	pCard->SetCRole(m_pCRole);

	uint32_t dwObjId = m_pCRole->GetNewObjId();                             
	IME_DEBUG("objid %u index %u", dwObjId, cardindex);
	pCard->SetdwObjId(dwObjId);
	pCard->SetdwIndex(cardindex);
	if (pCard->LoadConf())
	{
		IME_ERROR("card load conf fail");
		IME_CHAR_LOG("resource [status=fail,res=card,opt=add,character=%u,index=%u,reason=loadconf,source=%d]", m_pCRole->GetdwAccountId(), cardindex, stu.source);
		return -1;
	}

	if (pCard->GetbyType() == E_ITEM_PET_CARD || pCard->GetbyType() == E_ITEM_MASTER_CARD)
	{
		if (Awake)
		{
			pCard->SetbyRare(E_CR_AWAKE);
		}
		else
		{
			if (IsAutoAwake)
			{
				if (CUtil::RandEither(pCard->GetdwAwakePro(), 100))
				{
					pCard->SetbyRare(E_CR_AWAKE);
				}
			}
		}
	}

	pCard->AddExp(exp);
	uint32_t tmpbreaknumber = std::min(pCard->GetdwLimitBreakNumber(), breakcount);
	pCard->SetdwLimitBreak(tmpbreaknumber);

	if (pCard->Calculate())
	{
		IME_ERROR("card calculate fail");
		IME_CHAR_LOG("resource [status=fail,res=card,opt=add,character=%u,index=%u,reason=add calculate,source=%d]", m_pCRole->GetdwAccountId(), cardindex, stu.source);
		return -1;
	}
	m_mapCards[dwObjId] = pCard;
	objid = dwObjId;

	if (pCardConf->byType == E_ITEM_PET_CARD || pCardConf->byType == E_ITEM_MASTER_CARD)
	{
		m_dwCardNumberRole++;
	}
	else
	{
		m_dwCardNumberEquip++;
	}
	m_dwCardNumber++;

	m_pCRole->GetclsTujian()->AddTujian(cardindex, pCard->GetbyRare(), pCard->GetdwLimitBreak(), TUJIAN_GET);
	m_pCRole->GetclsTujian()->CardTujianUpdate(cardindex, TUJIAN_CARD);
	IME_CHAR_LOG("resource [status=success,res=card,opt=add,character=%u,index=%u,reason=success,source=%d]", m_pCRole->GetdwAccountId(), cardindex, stu.source);

//	if (stu.attackplus == 0 && stu.defenceplus == 0 && stu.healthplus == 0)
//	{
//		if (CUtil::RandEither(GlobalConfig::PlusProbability, 10000))
//		{
//			int ret = CUtil::RandByRange(0, 3);
//			if (ret == 0)
//			{
//				pCard->SetwAttackPlus(1);
//				stu.attackplus = 1;
//			}
//			else if (ret == 1)
//			{
//				pCard->SetwDefencePlus(1);
//				stu.defenceplus = 1;
//			}
//			else
//			{
//				pCard->SetwHealthPlus(1);
//				stu.healthplus = 1;
//			}
//		}	
//	}
//	else
//	{
	if (pCard->GetbyType() == E_ITEM_PET_CARD || pCard->GetbyType() == E_ITEM_MASTER_CARD)
	{
		pCard->ChangeAttackPlus(stu.attackplus);
		pCard->ChangeDefencePlus(stu.defenceplus);
		pCard->ChangeHealthPlus(stu.healthplus);
	}
//	}

	opt = 1;

	if (pCard->GetbyType() == E_ITEM_PET_CARD || pCard->GetbyType() == E_ITEM_MASTER_CARD)
	{
		if (pCard->GetbyRare() == E_CR_AWAKE)
		{
			m_pCRole->GetCMission()->CardAwakeNumber(0, 1);
		}
		m_pCRole->GetCMission()->HaveCard(0, 1);
		m_pCRole->GetCMission()->GetCard(cardindex, 1);
		if (pCard->GetbyAttribute() == SWORD)
		{
			m_pCRole->GetCMission()->HaveSwordCard(0, 1);
		}
		else if (pCard->GetbyAttribute() == HAND)
		{
			m_pCRole->GetCMission()->HaveHandCard(0, 1);
		}
		else if (pCard->GetbyAttribute() == INTERNAL_FORCE)
		{
			m_pCRole->GetCMission()->HaveInternalForceCard(0, 1);
		}
		else if (pCard->GetbyAttribute() == DARK)
		{
			m_pCRole->GetCMission()->HaveDarkCard(0, 1);
		}
		else if (pCard->GetbyAttribute() == STICK)
		{
			m_pCRole->GetCMission()->HaveStickCard(0, 1);
		}
	}

	WorldPacket info(CMD_SC_CARD_BREAK_AWAKE);
	info << (uint8_t) 1;
	if (IsUpdate)
	{
		pCard->SerializeClient(info);
		m_pCRole->SendPacket(&info);
	}

	stu.objid = objid;
	stu.opt = opt;
    return 0;
}

int CCardPacket::CardDel(uint32_t objid, int source)
{
	if (find(m_vecAttackTeam.begin(), m_vecAttackTeam.end(), objid) != m_vecAttackTeam.end())
	{
		IME_ERROR("delete objid[%u] fail", objid);
		return -1;
	}

//	if (find(m_vecDefenseTeam.begin(), m_vecDefenseTeam.end(), objid) != m_vecDefenseTeam.end())
//	{
//		IME_ERROR("delete objid[%u] fail", objid);
//		return -1;
//	}

	for (int i = 0; i < m_vecEquip1.size(); i++)
	{
		if (m_vecEquip1[i].dwWeaponObjId == objid)
			return -1;
		if (m_vecEquip1[i].dwArmorObjId == objid)
			return -1;
		if (m_vecEquip1[i].dwJewelObjId == objid)
			return -1;
		if (m_vecEquip1[i].dwAccessoriesObjId == objid)
			return -1;
		if (m_vecEquip1[i].dwSkillObjId == objid)
			return -1;
		if (m_vecEquip1[i].dwMasterGuide1 == objid)
			return -1;
		if (m_vecEquip1[i].dwMasterGuide2 == objid)
			return -1;
		if (m_vecEquip1[i].dwMasterGuide3 == objid)
			return -1;
	}

	if (objid == m_MasterCard)
		return -1;

	if (m_MasterEquip.dwWeaponObjId == objid)
		return -1;
	if (m_MasterEquip.dwArmorObjId == objid)
		return -1;
	if (m_MasterEquip.dwJewelObjId == objid)
		return -1;
	if (m_MasterEquip.dwAccessoriesObjId == objid)
		return -1;
	if (m_MasterEquip.dwSkillObjId == objid)
		return -1;
	if (m_MasterEquip.dwMasterGuide1 == objid)
		return -1;
	if (m_MasterEquip.dwMasterGuide2 == objid)
		return -1;
	if (m_MasterEquip.dwMasterGuide3 == objid)
		return -1;

//	for (int i = 0; i < m_vecEquip2.size(); i++)
//	{
//		if (m_vecEquip2[i].dwWeaponObjId == objid)
//			return -1;
//		if (m_vecEquip2[i].dwArmorObjId == objid)
//			return -1;
//		if (m_vecEquip2[i].dwJewelObjId == objid)
//			return -1;
//	}
	CCard *pCard = Find(objid);
	if (pCard == NULL)
	{
		IME_ERROR("cannot find objid %u in packet userid = %u", objid, m_pCRole->GetdwAccountId());
		IME_CHAR_LOG("resource [status=fail,res=card,opt=del,character=%u,index=0,reason=cannot find objid,source=%d]", m_pCRole->GetdwAccountId(), source);
		return -1;
	}
	else
	{
		IME_CHAR_LOG("resource [status=success,res=card,opt=del,character=%u,index=%u,reason=success,source=%d]", m_pCRole->GetdwAccountId(), pCard->GetdwIndex(), source);
	}
	if (pCard->GetbyType() == E_ITEM_MASTER_CARD || pCard->GetbyType() == E_ITEM_PET_CARD)
	{
		m_dwCardNumberRole--;
	}
	else
	{
		m_dwCardNumberEquip--;
	}
	m_mapCards.erase(objid);		
	return 0;
}

bool CCardPacket::IsInEquip(CCard *p, uint32_t &location, uint8_t &team)
{
	std::vector<uint32_t>::iterator it;
	std::vector<equip_t>::iterator itequip;

	uint32_t objid = p->GetdwObjId();

	it = find(m_vecAttackTeam.begin(), m_vecAttackTeam.end(), p->GetdwObjId());
	if (it != m_vecAttackTeam.end())
	{
		location = it - m_vecAttackTeam.begin();
		team = 1;
		return true;
	}
	for (int i = 0; i < m_vecEquip1.size(); i++)
	{
		if (m_vecEquip1[i].dwWeaponObjId == objid || m_vecEquip1[i].dwArmorObjId == objid || m_vecEquip1[i].dwJewelObjId == objid || m_vecEquip1[i].dwAccessoriesObjId == objid || m_vecEquip1[i].dwSkillObjId == objid || m_vecEquip1[i].dwMasterGuide1 == objid || m_vecEquip1[i].dwMasterGuide2 == objid || m_vecEquip1[i].dwMasterGuide3 == objid)
		{
			location = i;
			team = 1;
			return true;
		}
	}

	if (p->GetdwObjId() == m_MasterCard)
	{
		location = 0;
		team = 0;
		return true;
	}
	if (m_MasterEquip.dwWeaponObjId == objid || m_MasterEquip.dwArmorObjId == objid || m_MasterEquip.dwJewelObjId == objid || m_MasterEquip.dwAccessoriesObjId == objid || m_MasterEquip.dwSkillObjId == objid || m_MasterEquip.dwMasterGuide1 == objid || m_MasterEquip.dwMasterGuide2 == objid || m_MasterEquip.dwMasterGuide3 == objid)
	{
		location =0;
		team = 0;
		return true;
	}

	return false;
}


int CCardPacket::InitCards(uint32_t masterindex)
{
    if (m_mapCards.size() != 0)
        return -1;
    uint32_t tempobj,obj1,obj2,obj3;
    //todo add 增加初始卡牌
	uint32_t opt;
	uint32_t CardIndex1;
	int randcard = 0;
//	std::vector<STC_CONF_CARD_COLLECTION *> *pOneArr = CConfCardCollection::Find(INIT_CARD_ONE_STAR);	
//	if (pOneArr->size() == 0)
//		return -1;
	
//	int randcard = CUtil::RandByRange(0, pOneArr->size());
//	uint32_t opt;
//	uint32_t CardIndex1;
//	CardIndex1 = (*pOneArr)[randcard]->dwCardindex;

	CardIndex1 = INIT_CARD_ONE_STAR;

//	std::vector<STC_CONF_CARD_COLLECTION *> *pTwoArr = CConfCardCollection::Find(INIT_CARD_TWO_STAR);	
//	if (pTwoArr->size() == 0)
//		return -1;
//	randcard = CUtil::RandByRange(0, pTwoArr->size());
//	uint32_t CardIndex2;
//	CardIndex2 = (*pTwoArr)[randcard]->dwCardindex;
	
	uint32_t CardIndex2;
	CardIndex2 = INIT_CARD_TWO_STAR;

	STC_CONF_CARD_BASE *pConf = CConfCardBase::Find(CardIndex1);
	if (pConf == NULL)
	{
		IME_ERROR("can not find card record in card csv index %u", CardIndex1); 
		return -1;
	}
	if (pConf->byStarLevel != 1)
	{
		IME_ERROR("it is not one star card in one star card collection");
		return -1;
	}

	pConf = CConfCardBase::Find(CardIndex2);
	if (pConf == NULL)
	{
		IME_ERROR("can not find card record in card csv index %u", CardIndex2); 
		return -1;
	}
	if (pConf->byStarLevel != 2)
	{
		IME_ERROR("it is not two star card in one star card collection");
		return -1;
	}
	int ThreeArr[INIT_CARD_MAX] = INIT_THREE_CARD;
	randcard = CUtil::RandByRange(0, INIT_CARD_MAX);
	pConf = CConfCardBase::Find(ThreeArr[randcard]);
	if (pConf == NULL)
	{
		IME_ERROR("not find in card csv index %u", ThreeArr[randcard]);
		return -1;
	}
	pConf = CConfCardBase::Find(masterindex);
	if (pConf == NULL)
	{
		IME_ERROR("can not find card csv index %u", masterindex);
		return -1;
	}
//	if (pConf->byType != E_ITEM_MASTER_CARD)
//	{
//		IME_ERROR("request masterindex %u is not master card", masterindex);
//		return -1;
//	}

    if (CardAddByParaNoAutoAwake(CardIndex1, obj1, opt, SOURCE_INIT, false))
		return -1;
	if (opt)
	SingleCardAddUpdate(obj1);	

    if (CardAddByParaNoAutoAwake(CardIndex2, obj2, opt, SOURCE_INIT, false))
		return -1;
	if (opt)
	SingleCardAddUpdate(obj2);

	if (CardAddByParaNoAutoAwake(ThreeArr[randcard], obj3, opt, SOURCE_INIT, false))
		return -1;
	if (opt)
	SingleCardAddUpdate(obj3);

//	if (CardAddByPara(10001,tempobj))
//		return;
//	if (CardAddByPara(10002,tempobj))
//		return;
//	if (CardAddByPara(20001,tempobj))
//		return;
//	if (CardAddByPara(20002,tempobj))
//		return;
//	if (CardAddByPara(20003,tempobj))
//		return;
//	if (CardAddByPara(30001,tempobj))
//		return;
//	if (CardAddByPara(30002,tempobj))
//		return;
//	if (CardAddByPara(30003,tempobj))
//		return;
	for (int i = 0; i < GlobalConfig::InitSkillCard.size(); i++)
	{
		CardAddByParaNoAutoAwake(GlobalConfig::InitSkillCard[i], tempobj, opt, SOURCE_INIT, false);
		SingleCardAddUpdate(tempobj);
	}

    m_vecAttackTeam.push_back(obj1);
	Find(obj1)->CardEquip1(true);
    m_vecAttackTeam.push_back(obj2);
	Find(obj2)->CardEquip1(true);
    m_vecAttackTeam.push_back(obj3);
	Find(obj3)->CardEquip1(true);

	uint32_t eobj1,eobj2,eobj3;
	uint32_t ArrE[] = INIT_EQUIP_CARD;
    CardAddByPara(ArrE[0], eobj1, opt, SOURCE_INIT, false);
	if (opt)
	SingleCardAddUpdate(eobj1);
    CardAddByPara(ArrE[1], eobj2, opt, SOURCE_INIT, false);
	if (opt)
	SingleCardAddUpdate(eobj2);
    CardAddByPara(ArrE[2], eobj3, opt, SOURCE_INIT, false);
	if (opt)
	SingleCardAddUpdate(eobj3);
	
	CCard *pCard = Find(obj1);
	m_mapEquipCards[pCard->GetdwObjId()] = pCard;
	pCard = Find(obj2);
	m_mapEquipCards[pCard->GetdwObjId()] = pCard;
	pCard = Find(obj3);
	m_mapEquipCards[pCard->GetdwObjId()] = pCard;

	m_vecLineUp[0] = obj1;
	m_vecLineUp[1] = obj2;
	m_vecLineUp[2] = obj3;
	
	m_vecView = m_vecLineUp;
	for (int i = 0; i < m_vecView.size(); )
	{
		if (m_vecView[i] == 0)
		{
			m_vecView.erase(m_vecView.begin() + i);
		}
		else
		{
			i++;
		}
	}

	HandlerLineUpInfo();
	HandlerViewInfo();

	if (CardAddByParaNoAutoAwake(masterindex, tempobj, opt, SOURCE_INIT, false))
		return -1;
	m_MasterCard = tempobj;
	if (opt)
	SingleCardAddUpdate(tempobj);
	Find(tempobj)->CardEquip1(true);

    Calculate(true);

	//todo attack team notice
	HandlerCardQuipmentInfo();	
	m_setobjid.insert(eobj1);
	m_setobjid.insert(eobj2);
	m_setobjid.insert(eobj3);
	m_setobjid.insert(tempobj);

	return 0;
}

int CCardPacket::SaveData()
{
    if (!m_bIsInit)
        return -1;
    std::string attack_team;
//    std::string defense_team;
//    std::string suit_attribute;
	std::string lineup;
    std::string cards;
	std::string master;
    StrMerge(attack_team, ",", m_vecAttackTeam);
//    StrMerge(defense_team, ",", m_vecDefenseTeam);
//    StrMerge(suit_attribute, ",", m_vecSuitAttribute);
	StrMerge(lineup, ",", m_vecLineUp);

	std::vector<uint32_t> vecMaster;
	vecMaster.push_back(m_MasterCard);
	StrMerge(master, ",", vecMaster);
   	
	std::string equip1;
//	std::string equip2;
	int i;
	std::vector<uint32_t> vectemp;
	std::vector<std::string> vectempstr;
	std::string tempstr;
	std::string equip_master;
//	IME_DEBUG("equip1 %u size,equip2 %u size",m_vecEquip1.size(), m_vecEquip2.size());

	for (i = 0; i < m_vecEquip1.size(); i++)
	{
		vectemp.clear();
		vectemp.push_back(m_vecEquip1[i].dwWeaponObjId);
		vectemp.push_back(m_vecEquip1[i].dwArmorObjId);
		vectemp.push_back(m_vecEquip1[i].dwJewelObjId);	
		vectemp.push_back(m_vecEquip1[i].dwAccessoriesObjId);	
		vectemp.push_back(m_vecEquip1[i].dwSkillObjId);	
		vectemp.push_back(m_vecEquip1[i].dwMasterGuide1);
		vectemp.push_back(m_vecEquip1[i].dwMasterGuide2);
		vectemp.push_back(m_vecEquip1[i].dwMasterGuide3);
		StrMerge(tempstr, ",", vectemp);
		vectempstr.push_back(tempstr);
	}

	CUtil::StrMerge(equip1 , "|", vectempstr);

	vectemp.clear();
	vectemp.push_back(m_MasterEquip.dwWeaponObjId);
	vectemp.push_back(m_MasterEquip.dwArmorObjId);
	vectemp.push_back(m_MasterEquip.dwJewelObjId);	
	vectemp.push_back(m_MasterEquip.dwAccessoriesObjId);	
	vectemp.push_back(m_MasterEquip.dwSkillObjId);	
	vectemp.push_back(m_MasterEquip.dwMasterGuide1);	
	vectemp.push_back(m_MasterEquip.dwMasterGuide2);	
	vectemp.push_back(m_MasterEquip.dwMasterGuide3);	
	StrMerge(equip_master, ",", vectemp);

//	vectemp.clear();
//	vectempstr.clear();
//	tempstr.clear();
//	for (i = 0; i < m_vecEquip2.size(); i++)
//	{
//		vectemp.clear();
//		vectemp.push_back(m_vecEquip2[i].dwWeaponObjId);
//		vectemp.push_back(m_vecEquip2[i].dwArmorObjId);
//		vectemp.push_back(m_vecEquip2[i].dwJewelObjId);	
//		StrMerge(tempstr, ",", vectemp);
//		vectempstr.push_back(tempstr);
//	}
//	CUtil::StrMerge(equip2 , "|", vectempstr);
	
    serialize(cards);

	UpdateEquipCards();
	std::string equipcards;
	serializeEquipCard(equipcards);
	std::string base;
	ByteBuffer by;
	by << m_dwTotalForce;
	CCard *pCard = Find(m_MasterCard);
	if (pCard != NULL)
	{
		by << (uint32_t) 1;
		by << pCard->GetdwObjId();
		by << pCard->GetdwIndex();
		by << pCard->GetwLevel();
		by << pCard->GetdwLimitBreak();
		by << pCard->GetdwLimitBreakNumber();
		by << pCard->GetbyRare();
	}
	else
	{
		by << (uint32_t) 0;
	}
	base.assign((const char *)by.contents(), by.size());

    CDBCtrl::SaveUserCardPacketData(attack_team, lineup, cards, m_pCRole->GetdwAccountId(), equip1, equipcards, master, equip_master, base);
    return 0;
}

void CCardPacket::UpdateEquipCards()
{
	m_mapEquipCards.clear();
	std::map<uint32_t, CCard *>::iterator it;
    for(it = m_mapCards.begin() ;it != m_mapCards.end() ;it++)
    {
        CCard *pCard = it->second;          
		uint32_t location; uint8_t team;
//		if (IsInEquip(pCard, location, 0) || IsInEquip(pCard, location, 1))
		if (IsInEquip(pCard, location, team))
		{
			m_mapEquipCards[pCard->GetdwObjId()] = pCard;
		}
	}
}

void CCardPacket::HandlerBaseInfo()
{

    WorldPacket info(CMD_SC_ROLE_CARD_TEAM_EQUIP_INFO, 20000);    
    uint8_t teamnumber = 0;
    teamnumber = (uint8_t)m_vecAttackTeam.size();
    info << teamnumber;
    for(int i = 0; i < teamnumber; i++)
    {
        info << m_vecAttackTeam[i];
		IME_DEBUG("up team %u", m_vecAttackTeam[i]);
    }
//    teamnumber = (uint8_t)m_vecDefenseTeam.size();
//    info << teamnumber;
//    for(int i = 0; i < teamnumber; i++)
//    {
//        info << m_vecDefenseTeam[i];
//    }
	info << m_MasterCard;
	IME_DEBUG("up team master %u", m_MasterCard);
	m_pCRole->SendPacket(&info);

	int i = 0;
	info.clear();

    for(std::map<uint32_t, CCard *>::const_iterator it = m_mapCards.begin(); it != m_mapCards.end(); it++)
    {
        it->second->SerializeClient(info);
		i++;
		if (i >= 60)
		{
			WorldPacket smallpkg(CMD_SC_ROLE_CARD_PACKET_INFO);
			smallpkg << (uint32_t)i;
			smallpkg.append(info.contents(), info.size());
			m_pCRole->SendPacket(&smallpkg);
			info.clear();
			i = 0;
		}
    }     

	if (i < 60)
	{
			WorldPacket smallpkg(CMD_SC_ROLE_CARD_PACKET_INFO);
			smallpkg << (uint32_t)i;
			smallpkg.append(info.contents(), info.size());
			m_pCRole->SendPacket(&smallpkg);
	}
	HandlerEquipCalInfo(0);
	HandlerLineUpInfo();
	HandlerViewInfo();
	HandlerItemQuipmentInfo();
    return;
}

int CCardPacket::EquipCardInfo()
{
	for(int i = 0; i < m_vecAttackTeam.size(); i++)
	{
		if (m_vecAttackTeam[i] != 0)
		{
			if(Find(m_vecAttackTeam[i]) != NULL)
			{
				Find(m_vecAttackTeam[i])->CardEquip1();
//				IME_DEBUG("objid %u, flag %u", m_vecAttackTeam[i], Find(m_vecAttackTeam[i])->GetbyFlag());
			}
			else
			{
				IME_ERROR("can not find this card objid [%u]", m_vecAttackTeam[i]);
				return -1; 
			}
		}
	}	
	if (m_MasterCard != 0)
	{
		Find(m_MasterCard)->CardEquip1();
	}
	
	uint32_t objid;
	for (int i = 0; i < m_vecEquip1.size(); i++)
	{
		objid = m_vecEquip1[i].dwWeaponObjId;
		if (objid != 0)
		{
			if (Find(objid) != NULL)
			{
				Find(objid)->CardEquip1();
			}
			else
			{
				IME_ERROR("can not find this card objid [%u]", objid);
				return -1;
			}
		}
		objid = m_vecEquip1[i].dwArmorObjId;
		if (objid != 0)
		{
			if (Find(objid) != NULL)
			{
				Find(objid)->CardEquip1();
			}
			else
			{
				IME_ERROR("can not find this card objid [%u]", objid);
				return -1;
			}
		}
		objid = m_vecEquip1[i].dwJewelObjId;
		if (objid != 0)
		{
			if (Find(objid) != NULL)
			{
				Find(objid)->CardEquip1();
			}
			else
			{
				IME_ERROR("can not find this card objid [%u]", objid);
				return -1;
			}
		}
		objid = m_vecEquip1[i].dwAccessoriesObjId;
		if (objid != 0)
		{
			if (Find(objid) != NULL)
			{
				Find(objid)->CardEquip1();
			}
			else
			{
				IME_ERROR("can not find this card objid [%u]", objid);
				return -1;
			}
		}
		objid = m_vecEquip1[i].dwSkillObjId;
		if (objid != 0)
		{
			if (Find(objid) != NULL)
			{
				Find(objid)->CardEquip1();
			}
			else
			{
				IME_ERROR("can not find this card objid [%u]", objid);
				return -1;
			}
		}
		objid = m_vecEquip1[i].dwMasterGuide1;
		if (objid != 0)
		{
			if (Find(objid) != NULL)
			{
				Find(objid)->CardEquip1();
			}
			else
			{
				IME_ERROR("can not find this card objid [%u]", objid);
				return -1;
			}
		}
		objid = m_vecEquip1[i].dwMasterGuide2;
		if (objid != 0)
		{
			if (Find(objid) != NULL)
			{
				Find(objid)->CardEquip1();
			}
			else
			{
				IME_ERROR("can not find this card objid [%u]", objid);
				return -1;
			}
		}
		objid = m_vecEquip1[i].dwMasterGuide3;
		if (objid != 0)
		{
			if (Find(objid) != NULL)
			{
				Find(objid)->CardEquip1();
			}
			else
			{
				IME_ERROR("can not find this card objid [%u]", objid);
				return -1;
			}
		}
	}

		objid = m_MasterEquip.dwWeaponObjId;
		if (objid != 0)
		{
			if (Find(objid) != NULL)
			{
				Find(objid)->CardEquip1();
			}
			else
			{
				IME_ERROR("can not find this card objid [%u]", objid);
				return -1;
			}
		}
		objid = m_MasterEquip.dwArmorObjId;
		if (objid != 0)
		{
			if (Find(objid) != NULL)
			{
				Find(objid)->CardEquip1();
			}
			else
			{
				IME_ERROR("can not find this card objid [%u]", objid);
				return -1;
			}
		}
		objid = m_MasterEquip.dwJewelObjId;
		if (objid != 0)
		{
			if (Find(objid) != NULL)
			{
				Find(objid)->CardEquip1();
			}
			else
			{
				IME_ERROR("can not find this card objid [%u]", objid);
				return -1;
			}
		}
		objid = m_MasterEquip.dwAccessoriesObjId;
		if (objid != 0)
		{
			if (Find(objid) != NULL)
			{
				Find(objid)->CardEquip1();
			}
			else
			{
				IME_ERROR("can not find this card objid [%u]", objid);
				return -1;
			}
		}
		objid = m_MasterEquip.dwSkillObjId;
		if (objid != 0)
		{
			if (Find(objid) != NULL)
			{
				Find(objid)->CardEquip1();
			}
			else
			{
				IME_ERROR("can not find this card objid [%u]", objid);
				return -1;
			}
		}
		objid = m_MasterEquip.dwMasterGuide1;
		if (objid != 0)
		{
			if (Find(objid) != NULL)
			{
				Find(objid)->CardEquip1();
			}
			else
			{
				IME_ERROR("can not find this card objid [%u]", objid);
				return -1;
			}
		}
		objid = m_MasterEquip.dwMasterGuide2;
		if (objid != 0)
		{
			if (Find(objid) != NULL)
			{
				Find(objid)->CardEquip1();
			}
			else
			{
				IME_ERROR("can not find this card objid [%u]", objid);
				return -1;
			}
		}
		objid = m_MasterEquip.dwMasterGuide3;
		if (objid != 0)
		{
			if (Find(objid) != NULL)
			{
				Find(objid)->CardEquip1();
			}
			else
			{
				IME_ERROR("can not find this card objid [%u]", objid);
				return -1;
			}
		}
//	for (int i = 0; i < m_vecEquip2.size(); i++)
//	{
//		objid = m_vecEquip2[i].dwWeaponObjId;
//		if (objid != 0)
//		{
//			if (Find(objid) != NULL)
//			{
//				Find(objid)->CardEquip2();
//			}
//			else
//			{
//				IME_ERROR("can not find this card objid [%u]", objid);
//				return -1;
//			}
//		}
//		objid = m_vecEquip2[i].dwArmorObjId;
//		if (objid != 0)
//		{
//			if (Find(objid) != NULL)
//			{
//				Find(objid)->CardEquip2();
//			}
//			else
//			{
//				IME_ERROR("can not find this card objid [%u]", objid);
//				return -1;
//			}
//		}
//		objid = m_vecEquip2[i].dwJewelObjId;
//		if (objid != 0)
//		{
//			if (Find(objid) != NULL)
//			{
//				Find(objid)->CardEquip2();
//			}
//			else
//			{
//				IME_ERROR("can not find this card objid [%u]", objid);
//				return -1;
//			}
//		}
//	}
	return 0;
}

void CCardPacket::deserialize(std::string &str)
{
    if(str.size() == 0)
        return;
	m_dwCardNumberRole = 0;
	m_dwCardNumberEquip = 0;
    ByteBuffer temp;
    temp.append(str.data(), str.size());
    temp >> m_dwCardNumber;
    uint32_t tempnumber = 0;
    for( ;tempnumber < m_dwCardNumber; tempnumber++)
    {
        CCard *pCard = new(std::nothrow) CCard();          
        //todo log?
        assert(pCard != NULL);
        pCard->SetCRole(m_pCRole);
        pCard->DeserializeDB(temp); 
        if (pCard->LoadConf())
			return;
        if (pCard->Calculate(false, false, false))
			return;
        m_mapCards[pCard->GetdwObjId()] = pCard;
		uint32_t location; uint8_t team;
		if (IsInEquip(pCard, location, team))
		{
			m_mapEquipCards[pCard->GetdwObjId()] = pCard;
		}
		if (pCard->GetbyType() == E_ITEM_PET_CARD || pCard->GetbyType() == E_ITEM_MASTER_CARD)
		{
			m_dwCardNumberRole++;
		}
		else
		{
			m_dwCardNumberEquip++;
		}

		if (pCard->GetwLevel() > m_wMaxCardLevel)
			m_wMaxCardLevel = pCard->GetwLevel();
//		IME_DEBUG("result card objid %u, index %u, type %u, level %u, exp %u, explimit %u, nature %u, lastexplimit %u rare %u", pCard->GetdwObjId(), pCard->GetdwIndex(), pCard->GetbyType(), pCard->GetwLevel(), pCard->GetdwExp(), pCard->GetdwExpLimit(), pCard->GetbyAttribute(), pCard->GetdwLastExpLimit(), pCard->GetbyRare() );
    }

	IME_LOG("total card number %u, limit %u", m_dwCardNumber, m_dwTotalSpace);
}

void CCardPacket::serializeEquipCard(std::string &str)
{
    if (m_mapEquipCards.size() == 0)
        return;
    ByteBuffer temp(20000);
    uint32_t tempnumber = 0;
    tempnumber = (uint32_t)m_mapEquipCards.size(); 
    temp << tempnumber;
    map<uint32_t, CCard *>::const_iterator it = m_mapEquipCards.begin();
    for(; it != m_mapEquipCards.end(); it++)
    {
		IME_DEBUG("equip objid %u", it->first);
        it->second->SerializeDB(temp);
    }
    str.assign((const char *)temp.contents(), temp.size());
}

void CCardPacket::serialize(std::string &str)
{
    if (m_mapCards.size() == 0)
        return;
    ByteBuffer temp(40000);
    uint32_t tempnumber = 0;
    tempnumber = (uint32_t)m_mapCards.size(); 
    temp << tempnumber;
    map<uint32_t, CCard *>::const_iterator it = m_mapCards.begin();
    for(; it != m_mapCards.end(); it++)
    {
        it->second->SerializeDB(temp);
    }
    str.assign((const char *)temp.contents(), temp.size());
}

//void CCardPacket::HandlerCardQuipmentChange(WorldPacket &info)
//{
//    uint8_t number = 0;
//    uint32_t tempobjid = 0;
//    std::vector<uint32_t> vecobjid;
////    std::vector<uint32_t> vecobjid2;
//    std::set<uint32_t> setobjid;
//    info >> number;
//    map<uint32_t, CCard *>::const_iterator it = m_mapCards.begin();
//
//    if (number > m_pCRole->GetbyCardCapacity())
//    {
//        m_pCRole->SendErrorMsg(ERRNO_MSG_EQUIP_OVERLOAD,CMD_SC_CARD_QUIPMENT_CHANGE_RESULT);
//        return;
//    }
//	if (m_vecAttackTeam.size() < m_pCRole->GetbyCardCapacity())
//	{
//		m_vecAttackTeam.resize(m_pCRole->GetbyCardCapacity(), 0);
//	}
//	if (m_vecEquip1.size() < m_pCRole->GetbyCardCapacity())
//	{
//		m_vecEquip1.resize(m_pCRole->GetbyCardCapacity());
//	}
//	
//	uint8_t IsAllUnpacket = 1;
//    for(int i = 0; i < number; i++)
//    {
//        info >> tempobjid;
//		if (tempobjid != 0)
//		{
//			CCard *p = Find(tempobjid);
//			if (p == NULL)
//			{
//				IME_ERROR("pet card is not in packet objid %u", tempobjid);
//				m_pCRole->SendErrorMsg(ERRNO_MSG_EQUIP_NOT_IN_PACKET, CMD_SC_CARD_QUIPMENT_CHANGE_RESULT);
//				return;
//			}
//			if (p->GetbyType() != E_ITEM_PET_CARD)
//			{
//				IME_ERROR("pet card type is not pet type %u", p->GetbyType());
//				m_pCRole->SendErrorMsg(ERRNO_MSG_DATA_ILLEGAL, CMD_SC_CARD_QUIPMENT_CHANGE_RESULT);
//				return;
//			}
//
//			if (m_mapCards.find(tempobjid) == m_mapCards.end())
//			{
//				m_pCRole->SendErrorMsg(ERRNO_MSG_EQUIP_NOT_IN_PACKET,CMD_SC_CARD_QUIPMENT_CHANGE_RESULT);
//				return;
//			}
//			if (setobjid.find(tempobjid) == setobjid.end())
//			{
//				setobjid.insert(tempobjid);
//			}
//			else
//			{
//				m_pCRole->SendErrorMsg(ERRNO_MSG_EQUIP_DUPLICATE_OBJID,CMD_SC_CARD_QUIPMENT_CHANGE_RESULT);
//				return;
//			}
//			IsAllUnpacket = 0;
//		}
//		vecobjid.push_back(tempobjid); 
//    }
//	info >> tempobjid;
//	if (IsAllUnpacket || tempobjid == 0)
//	{
//		m_pCRole->SendErrorMsg(ERRNO_MSG_NOT_ALL_EMPTY, CMD_SC_CARD_QUIPMENT_CHANGE_RESULT);
//		return;
//	}
//	CCard *p = Find(tempobjid);
//	if (p == NULL)
//	{
//		IME_ERROR("pet card is not in packet objid %u", tempobjid);
//		m_pCRole->SendErrorMsg(ERRNO_MSG_EQUIP_NOT_IN_PACKET, CMD_SC_CARD_QUIPMENT_CHANGE_RESULT);
//		return;
//	}
//	if (p->GetbyType() != E_ITEM_MASTER_CARD)
//	{
//		IME_ERROR("pet card type is not master type %u", p->GetbyType());
//		m_pCRole->SendErrorMsg(ERRNO_MSG_DATA_ILLEGAL, CMD_SC_CARD_QUIPMENT_CHANGE_RESULT);
//		return;
//	}
//
//	if (m_MasterCard != 0)
//	{
//		CCard *pOld = Find(m_MasterCard);
//		if (pOld == NULL)
//		{
//			IME_ERROR("pet card is not in packet objid %u", m_MasterCard);
//			m_pCRole->SendErrorMsg(ERRNO_MSG_EQUIP_NOT_IN_PACKET, CMD_SC_CARD_QUIPMENT_CHANGE_RESULT);
//			return;
//		}
//		pOld->CardUnEquip1(true);
//	}
//	
//	setobjid.insert(tempobjid);
//	m_setobjid = setobjid;
//	m_MasterCard = tempobjid;
//	p->CardEquip1(true);
//
//	if (number < m_vecAttackTeam.size())
//		vecobjid.resize(m_vecAttackTeam.size(), 0);
//	
//	for(int i = 0; i < m_vecAttackTeam.size(); i++)
//	{
//		if (m_vecAttackTeam[i] != 0)
//		{
//			Find(m_vecAttackTeam[i])->CardUnEquip1(true);
//		}
//	}	
//	for(int i = 0; i < vecobjid.size(); i++)
//	{
//		if (vecobjid[i] != 0)
//		{
//			Find(vecobjid[i])->CardEquip1(true);
//		}
//	}
//
//	for (int i = 0; i < m_vecAttackTeam.size(); i++)
//	{
//		if (m_vecAttackTeam[i] != 0 && vecobjid[i] == 0)
//		{
//			if (m_vecEquip1[i].dwWeaponObjId != 0)
//			{
//				Find(m_vecEquip1[i].dwWeaponObjId)->CardUnEquip1(true);
//				m_setForEquip1.erase(m_vecEquip1[i].dwWeaponObjId);
//				m_vecEquip1[i].dwWeaponObjId = 0;
//			}
//			if (m_vecEquip1[i].dwArmorObjId != 0)
//			{
//				Find(m_vecEquip1[i].dwArmorObjId)->CardUnEquip1(true);
//				m_setForEquip1.erase(m_vecEquip1[i].dwArmorObjId);
//				m_vecEquip1[i].dwArmorObjId = 0;
//			}
//			if (m_vecEquip1[i].dwJewelObjId != 0)
//			{
//				Find(m_vecEquip1[i].dwJewelObjId)->CardUnEquip1(true);
//				m_setForEquip1.erase(m_vecEquip1[i].dwJewelObjId);
//				m_vecEquip1[i].dwJewelObjId = 0;
//			}
//			if (m_vecEquip1[i].dwAccessoriesObjId != 0)
//			{
//				Find(m_vecEquip1[i].dwAccessoriesObjId)->CardUnEquip1(true);
//				m_setForEquip1.erase(m_vecEquip1[i].dwAccessoriesObjId);
//				m_vecEquip1[i].dwAccessoriesObjId = 0;
//			}
//			if (m_vecEquip1[i].dwSkillObjId != 0)
//			{
//				Find(m_vecEquip1[i].dwSkillObjId)->CardUnEquip1(true);
//				m_setForEquip1.erase(m_vecEquip1[i].dwSkillObjId);
//				m_vecEquip1[i].dwSkillObjId = 0;
//			}
//		}
//		HandlerItemQuipmentInfo();	
//		HandlerEquipCalInfo(0);
//	}	
//	
//    m_vecAttackTeam = vecobjid;
//	for (int i = 0; i < m_vecLineUp.size(); i++)
//	{
//		m_vecLineUp[i] = 0;
//	}
//
//	for (int i = 0; i < m_vecAttackTeam.size(); i++)
//	{
//		if (m_vecAttackTeam[i] != 0)
//		{
//			m_vecLineUp[i] = m_vecAttackTeam[i];
//		}
//	}
//
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
//	
//	HandlerLineUpInfo();
//	HandlerViewInfo();
//
////    m_vecDefenseTeam = vecobjid2;
//	if (Calculate(true))
//	{
//		IME_ERROR("calculate fail");
//		m_pCRole->SendErrorMsg(ERRNO_MSG_EQUIP_DATA, CMD_SC_CARD_QUIPMENT_CHANGE_RESULT);
//		return;
//	}
//    
//	HandlerCardQuipmentInfo();
//
//    WorldPacket pkg(CMD_SC_CARD_QUIPMENT_CHANGE_RESULT);
//    pkg << (uint16_t)0;
//    m_pCRole->SendPacket(&pkg); 
//}

//void CCardPacket::HandlerChangeTeam(WorldPacket &info)
//{
//    uint8_t team;
//    info >> team;
//	IME_DEBUG("handlerChangeTeam %u", team);
//    m_pCRole->SetbyCurrentTeam(team);
//	CalChangeTeam(true);
//	IME_DEBUG("handler attack min %u max %u defence min %u max %u", m_pCRole->GetdwAttackMin(), m_pCRole->GetdwAttackMax(), m_pCRole->GetdwDefenceMin(), m_pCRole->GetdwDefenceMax());
//}

#define UNPACK_EQUIP_FELLOWER(item,e) \
m_setForEquip1.erase(m_vecEquip1[location].item);\
if (e != 0)\
m_setForEquip1.insert(e);\
if (m_vecEquip1[location].item != e)\
{\
	if (m_vecEquip1[location].item != 0)\
	{\
		Find(m_vecEquip1[location].item)->CardUnEquip1(true);\
	}\
	if (e != 0)\
	{\
		Find(e)->CardEquip1(true);\
	}\
}\
m_vecEquip1[location].item = e;

#define UNPACK_EQUIP_MASTER(item,e) \
m_setForEquip1.erase(m_MasterEquip.item);\
if (e != 0)\
m_setForEquip1.insert(e);\
if (m_MasterEquip.item != e)\
{\
	if (m_MasterEquip.item != 0)\
	{\
		Find(m_MasterEquip.item)->CardUnEquip1(true);\
	}\
	if (e != 0)\
	{\
		Find(e)->CardEquip1(true);\
	}\
}\
m_MasterEquip.item = e;

void CCardPacket::HandlerItemQuipmentChange (WorldPacket &info)
{
	uint8_t team;
	uint8_t location;
	uint32_t weapon;
	uint32_t armor;
	uint32_t jewel;	
	uint32_t accessories;
	uint32_t skill;
	uint32_t master1;
	uint32_t master2;
	uint32_t master3;
	info >> team;
	info >> location;
	info >> weapon;
	info >> armor;
	info >> jewel;
	info >> accessories;
	info >> skill;
	info >> master1;
	info >> master2;
	info >> master3;
	IME_CHAR_LOG("action [userid %u equipment change]", m_pCRole->GetdwAccountId());
	if ( team != 0 && team != 1)
	{
		IME_ERROR("unknow team %u", team);
     	m_pCRole->SendErrorMsg(ERRNO_MSG_DATA_ILLEGAL, CMD_SC_ITEM_QUIPMENT_CHANGE_RESULT);
		return;
	}

	if (location >= m_pCRole->GetbyCardCapacity())
	{
		IME_ERROR("overload card capacity location %u", location);
		m_pCRole->SendErrorMsg(ERRNO_MSG_DATA_ILLEGAL, CMD_SC_ITEM_QUIPMENT_CHANGE_RESULT);
		return;
	}
	if (weapon != 0)
	{
		CCard *p = Find(weapon);
		if (p == NULL)
		{
			IME_ERROR("weapon card is not in packet objid %u", weapon);
			m_pCRole->SendErrorMsg(ERRNO_MSG_EQUIP_NOT_IN_PACKET, CMD_SC_ITEM_QUIPMENT_CHANGE_RESULT);
			return;
		}
		if (p->GetbyType() != E_ITEM_WEAPON_CARD)
		{
			IME_ERROR("weapon card type is not weapon type %u", p->GetbyType());
			m_pCRole->SendErrorMsg(ERRNO_MSG_DATA_ILLEGAL, CMD_SC_ITEM_QUIPMENT_CHANGE_RESULT);
			return;
		}
	}
	if (armor != 0)
	{
		CCard *p = Find(armor);
		if (p == NULL)
		{
			IME_ERROR("armor card is not in packet objid %u", armor);
			m_pCRole->SendErrorMsg(ERRNO_MSG_EQUIP_NOT_IN_PACKET, CMD_SC_ITEM_QUIPMENT_CHANGE_RESULT);
			return;
		}
		if (p->GetbyType() != E_ITEM_ARMOR_CARD)
		{
			IME_ERROR("armor card type is not armor type %u", p->GetbyType());
			m_pCRole->SendErrorMsg(ERRNO_MSG_DATA_ILLEGAL, CMD_SC_ITEM_QUIPMENT_CHANGE_RESULT);
			return;
		}
	}
	if (jewel != 0)
	{
		CCard *p = Find(jewel);
		if (p == NULL)
		{
			IME_ERROR("jewel card is not in packet objid %u", jewel);
			m_pCRole->SendErrorMsg(ERRNO_MSG_EQUIP_NOT_IN_PACKET, CMD_SC_ITEM_QUIPMENT_CHANGE_RESULT);
			return;
		}
		if (p->GetbyType() != E_ITEM_JEWEL_CARD)
		{
			IME_ERROR("jewel card type is not jewel type %u", p->GetbyType());
			m_pCRole->SendErrorMsg(ERRNO_MSG_DATA_ILLEGAL, CMD_SC_ITEM_QUIPMENT_CHANGE_RESULT);
			return;
		}
	}
	if (accessories != 0)
	{
		CCard *p = Find(accessories);
		if (p == NULL)
		{
			IME_ERROR("accessories card is not in packet objid %u", accessories);
			m_pCRole->SendErrorMsg(ERRNO_MSG_EQUIP_NOT_IN_PACKET, CMD_SC_ITEM_QUIPMENT_CHANGE_RESULT);
			return;
		}
		if (p->GetbyType() != E_ITEM_ACCESSORIES_CARD)
		{
			IME_ERROR("accessories card type is not accessories type %u", p->GetbyType());
			m_pCRole->SendErrorMsg(ERRNO_MSG_DATA_ILLEGAL, CMD_SC_ITEM_QUIPMENT_CHANGE_RESULT);
			return;
		}
	}
	std::vector<uint32_t> vecmaster;
	vecmaster.push_back(master1);
	vecmaster.push_back(master2);
	vecmaster.push_back(master3);
	std::vector<uint32_t> vecExcept;
	if (team == 0)
	{
		if (location + 1 > m_pCRole->GetbyCardCapacity())
		{
			IME_ERROR("card capacity arrive limit");
			m_pCRole->SendErrorMsg(ERRNO_MSG_EQUIP_FIRST_CARD, CMD_SC_ITEM_QUIPMENT_CHANGE_RESULT);
			return;	
		}
		m_vecEquip1.resize(m_pCRole->GetbyCardCapacity());

		vecExcept.push_back(m_vecEquip1[location].dwMasterGuide1);
		vecExcept.push_back(m_vecEquip1[location].dwMasterGuide2);
		vecExcept.push_back(m_vecEquip1[location].dwMasterGuide3);
	}
	else
	{
		vecExcept.push_back(m_MasterEquip.dwMasterGuide1);
		vecExcept.push_back(m_MasterEquip.dwMasterGuide2);
		vecExcept.push_back(m_MasterEquip.dwMasterGuide3);
	}

	std::vector<uint32_t> vecmastergroupindex;
	for(std::set<uint32_t>::iterator it = m_setForEquip1.begin(); it != m_setForEquip1.end(); it++)
	{
		if (find(vecExcept.begin(), vecExcept.end(), *it) != vecExcept.end())
			continue;
		CCard *p = Find(*it);
		if (p == NULL)
		{
			IME_ERROR("card objid can not find in packet objid %u", *it);
			m_pCRole->SendErrorMsg(ERRNO_MSG_EQUIP_NOT_IN_PACKET, CMD_SC_ITEM_QUIPMENT_CHANGE_RESULT);
			return;
		}
		vecmastergroupindex.push_back(p->GetdwCardGroup());
	}	
	for (std::set<uint32_t>::iterator it = m_setobjid.begin(); it != m_setobjid.end(); it++)
	{
		CCard *p = Find(*it);
		if (p == NULL)
		{
			IME_ERROR("card objid can not find in packet objid %u", *it);
			m_pCRole->SendErrorMsg(ERRNO_MSG_EQUIP_NOT_IN_PACKET, CMD_SC_ITEM_QUIPMENT_CHANGE_RESULT);
			return;
		}
		vecmastergroupindex.push_back(p->GetdwCardGroup());
	}

	for (int i = 0; i < vecmaster.size(); i++)
	{
		if (vecmaster[i] != 0)
		{
			CCard *p = Find(vecmaster[i]);
			if (p == NULL)
			{
				IME_ERROR("master guide card is not in packet objid %u", vecmaster[i]);
				m_pCRole->SendErrorMsg(ERRNO_MSG_EQUIP_NOT_IN_PACKET, CMD_SC_ITEM_QUIPMENT_CHANGE_RESULT);
				return;
			}
			if (std::find(vecmastergroupindex.begin(), vecmastergroupindex.end(), p->GetdwCardGroup()) != vecmastergroupindex.end())
			{
				IME_ERROR("master guide card is same group");
				m_pCRole->SendErrorMsg(ERRNO_MSG_CARD_GROUP_IN_TEAM, CMD_SC_ITEM_QUIPMENT_CHANGE_RESULT);
				return;
			}
			vecmastergroupindex.push_back(p->GetdwCardGroup());
			if (p->GetbyType() != E_ITEM_MASTER_CARD)
			{
				IME_ERROR("master guide card type is not master type %u", p->GetbyType());
				m_pCRole->SendErrorMsg(ERRNO_MSG_DATA_ILLEGAL, CMD_SC_ITEM_QUIPMENT_CHANGE_RESULT);
				return;
			}
		}
	}

//	if (team == 1)
//	{
//		if (skill != 0)
//		{
//			IME_ERROR("master card can not equip skill card index");
//			m_pCRole->SendErrorMsg(ERRNO_MSG_DATA_ILLEGAL, CMD_SC_ITEM_QUIPMENT_CHANGE_RESULT);
//			return;
//		}
//	}

	if (skill != 0)
	{
		CCard *p = Find(skill);
		if (p == NULL)
		{
			IME_ERROR("skill card is not in packet objid %u", skill);
			m_pCRole->SendErrorMsg(ERRNO_MSG_EQUIP_NOT_IN_PACKET, CMD_SC_ITEM_QUIPMENT_CHANGE_RESULT);
			return;
		}
		if (p->GetbyType() != E_ITEM_SKILL_CARD)
		{
			IME_ERROR("skill card type is not skill type %u", p->GetbyType());
			m_pCRole->SendErrorMsg(ERRNO_MSG_DATA_ILLEGAL, CMD_SC_ITEM_QUIPMENT_CHANGE_RESULT);
			return;
		}
	}

	if (team == 0)
	{
//		if (location + 1 > m_vecAttackTeam.size())
//		{
//			IME_ERROR("first equip card equip then opt item equip");
//			m_pCRole->SendErrorMsg(ERRNO_MSG_EQUIP_FIRST_CARD, CMD_SC_ITEM_QUIPMENT_CHANGE_RESULT);
//			return;
//		}
//		else
//		{
//			if (m_vecAttackTeam[location] == 0)
//			{
//				IME_ERROR("first equip card equip then opt item equip");
//				m_pCRole->SendErrorMsg(ERRNO_MSG_EQUIP_FIRST_CARD, CMD_SC_ITEM_QUIPMENT_CHANGE_RESULT);
//				return;
//			}	
//		}

//		todo some item disboard quipment
//		if (location + 1 > m_vecEquip1.size())
//		{
//			if (m_setForEquip1.find(weapon) != m_setForEquip1.end() ||
//				m_setForEquip1.find(armor) != m_setForEquip1.end() ||
//				m_setForEquip1.find(jewel) != m_setForEquip1.end() ||
//				m_setForEquip1.find(accessories) != m_setForEquip1.end() ||
//				m_setForEquip1.find(skill) != m_setForEquip1.end())
//			{
//				IME_ERROR("equip is already in quip");
//				m_pCRole->SendErrorMsg(ERRNO_MSG_EQUIP_DUPLICATE_OBJID, CMD_SC_ITEM_QUIPMENT_CHANGE_RESULT);
//				return;
//			}
//		}
//		else
//		{
			if (m_vecEquip1[location].dwWeaponObjId != weapon)
			{

				if (m_setForEquip1.find(weapon) != m_setForEquip1.end())
				{
					IME_ERROR("equip is already in quip");
					m_pCRole->SendErrorMsg(ERRNO_MSG_EQUIP_DUPLICATE_OBJID, CMD_SC_ITEM_QUIPMENT_CHANGE_RESULT);
					return;
				}
			}
			if (m_vecEquip1[location].dwArmorObjId != armor)
			{
				if (m_setForEquip1.find(armor) != m_setForEquip1.end())
				{
					IME_ERROR("equip is already in quip");
					m_pCRole->SendErrorMsg(ERRNO_MSG_EQUIP_DUPLICATE_OBJID, CMD_SC_ITEM_QUIPMENT_CHANGE_RESULT);
					return;
				}
			}
			if (m_vecEquip1[location].dwJewelObjId != jewel)
			{
				if (m_setForEquip1.find(jewel) != m_setForEquip1.end())
				{
					IME_ERROR("equip is already in quip");
					m_pCRole->SendErrorMsg(ERRNO_MSG_EQUIP_DUPLICATE_OBJID, CMD_SC_ITEM_QUIPMENT_CHANGE_RESULT);
					return;
				}
			}
			if (m_vecEquip1[location].dwAccessoriesObjId != accessories)
			{
				if (m_setForEquip1.find(accessories) != m_setForEquip1.end())
				{
					IME_ERROR("equip is already in quip");
					m_pCRole->SendErrorMsg(ERRNO_MSG_EQUIP_DUPLICATE_OBJID, CMD_SC_ITEM_QUIPMENT_CHANGE_RESULT);
					return;
				}
			}
			if (m_vecEquip1[location].dwSkillObjId != skill)
			{
				if (m_setForEquip1.find(skill) != m_setForEquip1.end())
				{
					IME_ERROR("equip is already in quip");
					m_pCRole->SendErrorMsg(ERRNO_MSG_EQUIP_DUPLICATE_OBJID, CMD_SC_ITEM_QUIPMENT_CHANGE_RESULT);
					return;
				}
			}
			if (m_vecEquip1[location].dwMasterGuide1 != master1)
			{
				if (m_setForEquip1.find(master1) != m_setForEquip1.end())
				{
					IME_ERROR("equip is already in quip");
					m_pCRole->SendErrorMsg(ERRNO_MSG_EQUIP_DUPLICATE_OBJID, CMD_SC_ITEM_QUIPMENT_CHANGE_RESULT);
					return;
				}
			}
			if (m_vecEquip1[location].dwMasterGuide2 != master2)
			{
				if (m_setForEquip1.find(master2) != m_setForEquip1.end())
				{
					IME_ERROR("equip is already in quip");
					m_pCRole->SendErrorMsg(ERRNO_MSG_EQUIP_DUPLICATE_OBJID, CMD_SC_ITEM_QUIPMENT_CHANGE_RESULT);
					return;
				}
			}
			if (m_vecEquip1[location].dwMasterGuide3 != master3)
			{
				if (m_setForEquip1.find(master3) != m_setForEquip1.end())
				{
					IME_ERROR("equip is already in quip");
					m_pCRole->SendErrorMsg(ERRNO_MSG_EQUIP_DUPLICATE_OBJID, CMD_SC_ITEM_QUIPMENT_CHANGE_RESULT);
					return;
				}
			}
//		}

		m_setForEquip1.erase(m_vecEquip1[location].dwWeaponObjId);
		m_setForEquip1.erase(m_vecEquip1[location].dwArmorObjId);
		m_setForEquip1.erase(m_vecEquip1[location].dwJewelObjId);
		m_setForEquip1.erase(m_vecEquip1[location].dwAccessoriesObjId);
		m_setForEquip1.erase(m_vecEquip1[location].dwSkillObjId);

		if (weapon != 0)
		m_setForEquip1.insert(weapon);
		if (armor != 0)
		m_setForEquip1.insert(armor);
		if (jewel != 0)
		m_setForEquip1.insert(jewel);
		if (accessories != 0)
		m_setForEquip1.insert(accessories);
		if (skill != 0)
		m_setForEquip1.insert(skill);

//		if (location + 1 > m_vecEquip1.size())
//			m_vecEquip1.resize(location + 1);
		
		if (m_vecEquip1[location].dwWeaponObjId != weapon)
		{
			if (m_vecEquip1[location].dwWeaponObjId != 0)
			{
//				CCard *pCard = Find(m_vecEquip1[location].dwWeaponObjId);
//				IME_DEBUG("flag %u index %u objid %u", pCard->GetbyFlag(), pCard->GetdwIndex(), pCard->GetdwObjId()); 
				Find(m_vecEquip1[location].dwWeaponObjId)->CardUnEquip1(true); 
//				IME_DEBUG("flag %u index %u objid %u", pCard->GetbyFlag(), pCard->GetdwIndex(), pCard->GetdwObjId()); 
			}	
			if (weapon != 0)
			{
				Find(weapon)->CardEquip1(true);
			}
		}
		if (m_vecEquip1[location].dwArmorObjId != armor)
		{
			if (m_vecEquip1[location].dwArmorObjId != 0)
			{
				Find(m_vecEquip1[location].dwArmorObjId)->CardUnEquip1(true);
			}
			if (armor != 0)
			{
				Find(armor)->CardEquip1(true);
			}
		}
		if (m_vecEquip1[location].dwJewelObjId != jewel)
		{
			if (m_vecEquip1[location].dwJewelObjId != 0)
			{
				Find(m_vecEquip1[location].dwJewelObjId)->CardUnEquip1(true);
			}
			if (jewel != 0)
			{
				Find(jewel)->CardEquip1(true);
			}
		}
		if (m_vecEquip1[location].dwAccessoriesObjId != accessories)
		{
			if (m_vecEquip1[location].dwAccessoriesObjId != 0)
			{
				Find(m_vecEquip1[location].dwAccessoriesObjId)->CardUnEquip1(true);
			}
			if (accessories != 0)
			{
				Find(accessories)->CardEquip1(true);
			}
		}
		if (m_vecEquip1[location].dwSkillObjId != skill)
		{
			if (m_vecEquip1[location].dwSkillObjId != 0)
			{
				Find(m_vecEquip1[location].dwSkillObjId)->CardUnEquip1(true);
			}
			if (skill != 0)
			{
				Find(skill)->CardEquip1(true);
			}
		}

		m_vecEquip1[location].dwWeaponObjId = weapon;
		m_vecEquip1[location].dwArmorObjId = armor;
		m_vecEquip1[location].dwJewelObjId = jewel;
		m_vecEquip1[location].dwAccessoriesObjId = accessories;
		m_vecEquip1[location].dwSkillObjId = skill;

		UNPACK_EQUIP_FELLOWER(dwMasterGuide1, master1);
		UNPACK_EQUIP_FELLOWER(dwMasterGuide2, master2);
		UNPACK_EQUIP_FELLOWER(dwMasterGuide3, master3);

		IME_DEBUG("veceuip1 size %ld", m_vecEquip1.size());
		if (Calculate(true))
		{
			IME_ERROR("calculate fail");
			m_pCRole->SendErrorMsg(ERRNO_MSG_EQUIP_DATA, CMD_SC_ITEM_QUIPMENT_CHANGE_RESULT);
			return;
		}

	}
	else
	{
		if (m_MasterEquip.dwWeaponObjId != weapon)
		{
			if (m_setForEquip1.find(weapon) != m_setForEquip1.end())
			{
				IME_ERROR("equip is already in quip");
				m_pCRole->SendErrorMsg(ERRNO_MSG_EQUIP_DUPLICATE_OBJID, CMD_SC_ITEM_QUIPMENT_CHANGE_RESULT);
				return;
			}
		}
		if (m_MasterEquip.dwArmorObjId != armor)
		{
			IME_DEBUG("armor %u, up %u", armor, m_MasterEquip.dwArmorObjId);
			if (m_setForEquip1.find(armor) != m_setForEquip1.end())
			{
				IME_ERROR("equip is already in quip");
				m_pCRole->SendErrorMsg(ERRNO_MSG_EQUIP_DUPLICATE_OBJID, CMD_SC_ITEM_QUIPMENT_CHANGE_RESULT);
				return;
			}
		}
		if (m_MasterEquip.dwJewelObjId != jewel)
		{
			if (m_setForEquip1.find(jewel) != m_setForEquip1.end())
			{
				IME_ERROR("equip is already in quip");
				m_pCRole->SendErrorMsg(ERRNO_MSG_EQUIP_DUPLICATE_OBJID, CMD_SC_ITEM_QUIPMENT_CHANGE_RESULT);
				return;
			}
		}
		if (m_MasterEquip.dwAccessoriesObjId != accessories)
		{
			if (m_setForEquip1.find(accessories) != m_setForEquip1.end())
			{
				IME_ERROR("equip is already in quip");
				m_pCRole->SendErrorMsg(ERRNO_MSG_EQUIP_DUPLICATE_OBJID, CMD_SC_ITEM_QUIPMENT_CHANGE_RESULT);
				return;
			}
		}
		if (m_MasterEquip.dwSkillObjId != skill)
		{
			if (m_setForEquip1.find(skill) != m_setForEquip1.end())
			{
				IME_ERROR("equip is already in quip");
				m_pCRole->SendErrorMsg(ERRNO_MSG_EQUIP_DUPLICATE_OBJID, CMD_SC_ITEM_QUIPMENT_CHANGE_RESULT);
				return;
			}
		}
		if (m_MasterEquip.dwMasterGuide1 != master1)
		{
			if (m_setForEquip1.find(master1) != m_setForEquip1.end())
			{
				IME_ERROR("equip is already in quip");
				m_pCRole->SendErrorMsg(ERRNO_MSG_EQUIP_DUPLICATE_OBJID, CMD_SC_ITEM_QUIPMENT_CHANGE_RESULT);
				return;
			}
		}
		if (m_MasterEquip.dwMasterGuide2 != master2)
		{
			if (m_setForEquip1.find(master2) != m_setForEquip1.end())
			{
				IME_ERROR("equip is already in quip");
				m_pCRole->SendErrorMsg(ERRNO_MSG_EQUIP_DUPLICATE_OBJID, CMD_SC_ITEM_QUIPMENT_CHANGE_RESULT);
				return;
			}
		}
		if (m_MasterEquip.dwMasterGuide3 != master3)
		{
			if (m_setForEquip1.find(master3) != m_setForEquip1.end())
			{
				IME_ERROR("equip is already in quip");
				m_pCRole->SendErrorMsg(ERRNO_MSG_EQUIP_DUPLICATE_OBJID, CMD_SC_ITEM_QUIPMENT_CHANGE_RESULT);
				return;
			}
		}

		m_setForEquip1.erase(m_MasterEquip.dwWeaponObjId);
		m_setForEquip1.erase(m_MasterEquip.dwArmorObjId);
		m_setForEquip1.erase(m_MasterEquip.dwJewelObjId);
		m_setForEquip1.erase(m_MasterEquip.dwAccessoriesObjId);
		m_setForEquip1.erase(m_MasterEquip.dwSkillObjId);

		if (weapon != 0)
		m_setForEquip1.insert(weapon);
		if (armor != 0)
		m_setForEquip1.insert(armor);
		if (jewel != 0)
		m_setForEquip1.insert(jewel);
		if (accessories != 0)
		m_setForEquip1.insert(accessories);
		if (skill != 0)
		m_setForEquip1.insert(skill);

		
		if (m_MasterEquip.dwWeaponObjId != weapon)
		{
			if (m_MasterEquip.dwWeaponObjId != 0)
			{
//				CCard *pCard = Find(m_MasterEquip.dwWeaponObjId);
//				IME_DEBUG("flag %u index %u objid %u", pCard->GetbyFlag(), pCard->GetdwIndex(), pCard->GetdwObjId()); 
				Find(m_MasterEquip.dwWeaponObjId)->CardUnEquip1(true); 
//				IME_DEBUG("flag %u index %u objid %u", pCard->GetbyFlag(), pCard->GetdwIndex(), pCard->GetdwObjId()); 
			}	
			if (weapon != 0)
			{
				Find(weapon)->CardEquip1(true);
			}
		}
		if (m_MasterEquip.dwArmorObjId != armor)
		{
			if (m_MasterEquip.dwArmorObjId != 0)
			{
				Find(m_MasterEquip.dwArmorObjId)->CardUnEquip1(true);
			}
			if (armor != 0)
			{
				Find(armor)->CardEquip1(true);
			}
		}
		if (m_MasterEquip.dwJewelObjId != jewel)
		{
			if (m_MasterEquip.dwJewelObjId != 0)
			{
				Find(m_MasterEquip.dwJewelObjId)->CardUnEquip1(true);
			}
			if (jewel != 0)
			{
				Find(jewel)->CardEquip1(true);
			}
		}
		if (m_MasterEquip.dwAccessoriesObjId != accessories)
		{
			if (m_MasterEquip.dwAccessoriesObjId != 0)
			{
				Find(m_MasterEquip.dwAccessoriesObjId)->CardUnEquip1(true);
			}
			if (accessories != 0)
			{
				Find(accessories)->CardEquip1(true);
			}
		}
		if (m_MasterEquip.dwSkillObjId != skill)
		{
			if (m_MasterEquip.dwSkillObjId != 0)
			{
				Find(m_MasterEquip.dwSkillObjId)->CardUnEquip1(true);
			}
			if (skill != 0)
			{
				Find(skill)->CardEquip1(true);
			}
		}

		m_MasterEquip.dwWeaponObjId = weapon;
		m_MasterEquip.dwArmorObjId = armor;
		m_MasterEquip.dwJewelObjId = jewel;
		m_MasterEquip.dwAccessoriesObjId = accessories;
		m_MasterEquip.dwSkillObjId = skill;

		UNPACK_EQUIP_MASTER(dwMasterGuide1, master1);
		UNPACK_EQUIP_MASTER(dwMasterGuide2, master2);
		UNPACK_EQUIP_MASTER(dwMasterGuide3, master3);
//		IME_DEBUG("veceuip1 size %u", m_vecEquip1.size());
		if (Calculate(true))
		{
			IME_ERROR("calculate fail");
			m_pCRole->SendErrorMsg(ERRNO_MSG_EQUIP_DATA, CMD_SC_ITEM_QUIPMENT_CHANGE_RESULT);
			return;
		}
	}

//		if (location + 1 > m_vecDefenseTeam.size())
//		{
//			IME_ERROR("first equip card equip then opt item equip");
//			m_pCRole->SendErrorMsg(ERRNO_MSG_EQUIP_FIRST_CARD, CMD_SC_ITEM_QUIPMENT_CHANGE_RESULT);
//			return;
//		}
//		else
//		{
//			if (m_vecDefenseTeam[location] == 0)
//			{
//				IME_ERROR("first equip card equip then opt item equip");
//				m_pCRole->SendErrorMsg(ERRNO_MSG_EQUIP_FIRST_CARD, CMD_SC_ITEM_QUIPMENT_CHANGE_RESULT);
//				return;
//			}	
//		}
//
//		if (location + 1 > m_vecEquip2.size())
//		{
//			if (m_setForEquip2.find(weapon) != m_setForEquip2.end() ||
//				m_setForEquip2.find(armor) != m_setForEquip2.end() ||
//				m_setForEquip2.find(jewel) != m_setForEquip2.end() )
//			{
//				IME_ERROR("equip is already in quip");
//				m_pCRole->SendErrorMsg(ERRNO_MSG_EQUIP_DUPLICATE_OBJID, CMD_SC_ITEM_QUIPMENT_CHANGE_RESULT);
//				return;
//			}
//		}
//		else
//		{
//			if (m_vecEquip2[location].dwWeaponObjId != weapon)
//			{
//				if (m_setForEquip2.find(weapon) != m_setForEquip2.end())
//				{
//					IME_ERROR("equip is already in quip");
//					m_pCRole->SendErrorMsg(ERRNO_MSG_EQUIP_DUPLICATE_OBJID, CMD_SC_ITEM_QUIPMENT_CHANGE_RESULT);
//					return;
//				}
//			}
//			if (m_vecEquip2[location].dwArmorObjId != armor)
//			{
//				if (m_setForEquip2.find(armor) != m_setForEquip2.end())
//				{
//					IME_ERROR("equip is already in quip");
//					m_pCRole->SendErrorMsg(ERRNO_MSG_EQUIP_DUPLICATE_OBJID, CMD_SC_ITEM_QUIPMENT_CHANGE_RESULT);
//					return;
//				}
//			}
//			if (m_vecEquip2[location].dwJewelObjId != jewel)
//			{
//				if (m_setForEquip2.find(jewel) != m_setForEquip2.end())
//				{
//					IME_ERROR("equip is already in quip");
//					m_pCRole->SendErrorMsg(ERRNO_MSG_EQUIP_DUPLICATE_OBJID, CMD_SC_ITEM_QUIPMENT_CHANGE_RESULT);
//					return;
//				}
//			}
//		}
//
//		m_setForEquip2.erase(m_vecEquip2[location].dwWeaponObjId);
//		m_setForEquip2.erase(m_vecEquip2[location].dwArmorObjId);
//		m_setForEquip2.erase(m_vecEquip2[location].dwJewelObjId);
//
//		if (weapon != 0)
//		m_setForEquip2.insert(weapon);
//		if (armor != 0)
//		m_setForEquip2.insert(armor);
//		if (jewel != 0)
//		m_setForEquip2.insert(jewel);
//
//		if (location + 1 > m_vecEquip2.size())
//			m_vecEquip2.resize(location + 1);
//		
//		if (m_vecEquip2[location].dwWeaponObjId != weapon)
//		{
//			if (m_vecEquip2[location].dwWeaponObjId != 0)
//			{
//				Find(m_vecEquip2[location].dwWeaponObjId)->CardUnEquip2(true); 
//			}	
//			if (weapon != 0)
//			{
//				Find(weapon)->CardEquip2(true);
//			}
//		}
//		if (m_vecEquip2[location].dwArmorObjId != armor)
//		{
//			if (m_vecEquip2[location].dwArmorObjId != 0)
//			{
//				Find(m_vecEquip2[location].dwArmorObjId)->CardUnEquip2(true);
//			}
//			if (armor != 0)
//			{
//				Find(armor)->CardEquip2(true);
//			}
//		}
//		if (m_vecEquip2[location].dwJewelObjId != jewel)
//		{
//			if (m_vecEquip2[location].dwJewelObjId != 0)
//			{
//				Find(m_vecEquip2[location].dwJewelObjId)->CardUnEquip2(true);
//			}
//			if (jewel != 0)
//			{
//				Find(jewel)->CardEquip2(true);
//			}
//		}
//		m_vecEquip2[location].dwWeaponObjId = weapon;
//		m_vecEquip2[location].dwArmorObjId = armor;
//		m_vecEquip2[location].dwJewelObjId = jewel;
//		if (Cal2ndTeam(true))
//		{
//			IME_ERROR("calculate fail");
//			m_pCRole->SendErrorMsg(ERRNO_MSG_EQUIP_DATA, CMD_SC_ITEM_QUIPMENT_CHANGE_RESULT);
//			return;
//		}
//	}
	
	WorldPacket pkg(CMD_SC_ITEM_QUIPMENT_CHANGE_RESULT);
	pkg	<< (uint16_t) 0;
	pkg << team;
	pkg << location;
	pkg << weapon;
	pkg << armor;
	pkg << jewel;
	pkg << accessories;
	pkg << skill;
	pkg << master1;
	pkg << master2;
	pkg << master3;
	m_pCRole->SendPacket(&pkg);
	return;
}		/* -----  end of method CCardPacket::HandlerItemQuipmentChange  ----- */


void CCardPacket::HandlerItemQuipmentInfo ()
{
	WorldPacket pkg(CMD_SC_ROLE_CARD_EQUIP_INFO);	
	uint8_t number;
	number = (uint8_t)m_vecEquip1.size();
	int i;
	pkg << number;
	IME_DEBUG("size %u, %ld", number, m_vecEquip1.size());
	for ( i = 0; i < number; i++)
	{
		pkg << (uint8_t) i;
		pkg << m_vecEquip1[i].dwWeaponObjId;
		pkg << m_vecEquip1[i].dwArmorObjId;
		pkg << m_vecEquip1[i].dwJewelObjId;
		pkg << m_vecEquip1[i].dwAccessoriesObjId;
		pkg << m_vecEquip1[i].dwSkillObjId;
		pkg << m_vecEquip1[i].dwMasterGuide1;
		pkg << m_vecEquip1[i].dwMasterGuide2;
		pkg << m_vecEquip1[i].dwMasterGuide3;
		IME_DEBUG("equip position %u, weapon %u armor %u jewel %u accessories %u skill %u", i, m_vecEquip1[i].dwWeaponObjId, m_vecEquip1[i].dwArmorObjId, m_vecEquip1[i].dwJewelObjId, m_vecEquip1[i].dwAccessoriesObjId, m_vecEquip1[i].dwSkillObjId);
	}
	pkg << (uint8_t) 0;
	pkg << m_MasterEquip.dwWeaponObjId;
	pkg << m_MasterEquip.dwArmorObjId;
	pkg << m_MasterEquip.dwJewelObjId;
	pkg << m_MasterEquip.dwAccessoriesObjId;
	pkg << m_MasterEquip.dwSkillObjId;
	pkg << m_MasterEquip.dwMasterGuide1;
	pkg << m_MasterEquip.dwMasterGuide2;
	pkg << m_MasterEquip.dwMasterGuide3;
	IME_DEBUG("master equip weapon %u armor %u jewel %u accessories %u skill %u", m_MasterEquip.dwWeaponObjId, m_MasterEquip.dwArmorObjId, m_MasterEquip.dwJewelObjId, m_MasterEquip.dwAccessoriesObjId, m_MasterEquip.dwSkillObjId);

//	number = (uint8_t)m_vecEquip2.size();
//	pkg << number;
//	for ( i = 0; i < number; i++)
//	{
//		pkg << (uint8_t) i;
//		pkg << m_vecEquip2[i].dwWeaponObjId;
//		pkg << m_vecEquip2[i].dwArmorObjId;
//		pkg << m_vecEquip2[i].dwJewelObjId;
//	}
	m_pCRole->SendPacket(&pkg);
	
//	HandlerEquipCalInfo(0);
//	HandlerLineUpInfo();
//	HandlerViewInfo();
	return;
}		/* -----  end of method CCardPacket::HandlerItemQuipmentInfo  ----- */

void CCardPacket::HandlerEquipCalInfo(uint8_t team)
{
	uint32_t attack = 0;
	uint32_t defence = 0;
	uint32_t health = 0;
	uint32_t Att = 0;
	WorldPacket pkg(CMD_SC_EQUIP_CARD_CAL_INFO);
//	if (team == 0)
//	{
		pkg << (uint16_t) m_vecItemEquip1.size();
		for (int i = 0; i < m_vecItemEquip1.size(); i++)
		{
			pkg << (uint8_t)i;
			pkg << m_vecItemEquip1[i].Attack;
			pkg << m_vecItemEquip1[i].Defence;
			pkg << m_vecItemEquip1[i].Health;
			pkg << m_vecItemEquip1[i].Crit;
			pkg << m_vecItemEquip1[i].Dodge;
			pkg << m_vecItemEquip1[i].Action;
			pkg << m_vecItemEquip1[i].Attribute;
			IME_DEBUG("cal equip val position %u attack %u", i, m_vecItemEquip1[i].Attack);
			attack += m_vecItemEquip1[i].Attack;
			defence += m_vecItemEquip1[i].Defence;
			health += m_vecItemEquip1[i].Health;
		}

		pkg << m_MasterItemEquip.Attack;
		pkg << m_MasterItemEquip.Defence;
		pkg << m_MasterItemEquip.Health;
		pkg << m_MasterItemEquip.Crit;
		pkg << m_MasterItemEquip.Dodge;
		pkg << m_MasterItemEquip.Action;
		pkg << m_MasterItemEquip.Attribute;
		IME_DEBUG("cal equip master attack %u", m_MasterItemEquip.Attack);
		attack += m_MasterItemEquip.Attack;
		defence += m_MasterItemEquip.Defence;
		health += m_MasterItemEquip.Health;
		Att = POWER_CALCULATE(attack,defence,health);
		pkg << Att;
		m_pCRole->SendPacket(&pkg);

//		IME_ERROR("weapon value %u", Att);
		if (m_pCRole != NULL && m_pCRole->GetCMission() != NULL)
		{
			m_pCRole->GetCMission()->ForceValue(0, Att);
		}

		m_dwTotalForce = Att;
//	}
//	else if (team == 1)
//	{
//		pkg << (uint8_t)1;
//		pkg << (uint16_t) m_vecItemEquip2.size();
//		for (int i = 0; i < m_vecItemEquip2.size(); i++)
//		{
//			pkg << (uint8_t)i;
//			pkg << m_vecItemEquip2[i].si_attack_min;
//			pkg << m_vecItemEquip2[i].si_attack_max;
//			pkg << m_vecItemEquip2[i].si_defence_min;
//		   	pkg << m_vecItemEquip2[i].si_defence_max;	
//		}
//		m_pCRole->SendPacket(&pkg);
//	}
//	else
//	{
//		IME_ERROR("team %u is not unvalid");
//	}
}

//int CCardPacket::CopyDataForDuration()
//{
//	int i = 0;
//	CDuration::cardforduration_t temp;
//	m_vecForDuration.clear();
//	if (m_pCRole->GetbyCurrentTeam() == 0)
//	{
//		for (; i < m_vecAttackTeam.size(); i++)
//		{
//			if (i >= m_vecItemEquip1.size())
//			{
//				IME_ERROR("1st team number != item equip number");
//				return -1;
//			}		
//
//			if (m_vecAttackTeam[i] != 0)
//			{
//
//				temp.cardobjid = m_vecAttackTeam[i];
//				if (m_vecItemEquip1[i].attack_high_probability != 0)
//				{
//					if (CUtil::RandEither(m_vecItemEquip1[i].attack_high_probability, 100))
//					{
//						temp.attack = m_vecItemEquip1[i].si_attack_max;
//					}
//					else
//					{
//						temp.attack = CUtil::RandByRange(m_vecItemEquip1[i].si_attack_min, m_vecItemEquip1[i].si_attack_max - 1);
//					}
//				}
//				else
//				{
//					temp.attack = CUtil::RandByRange(m_vecItemEquip1[i].si_attack_min, m_vecItemEquip1[i].si_attack_max);
//				}
//
//				if (m_vecItemEquip1[i].defence_high_probability != 0)
//				{
//					if (CUtil::RandEither(m_vecItemEquip1[i].defence_high_probability, 100))
//					{
//						temp.defence = m_vecItemEquip1[i].si_defence_max;
//					}
//					else
//					{
//						temp.defence = CUtil::RandByRange(m_vecItemEquip1[i].si_defence_min, m_vecItemEquip1[i].si_defence_max - 1);
//					}
//				}
//				else
//				{
//					temp.defence = CUtil::RandByRange(m_vecItemEquip1[i].si_defence_min, m_vecItemEquip1[i].si_defence_max);
//				}
//				temp.skill_probability = m_vecItemEquip1[i].skill_probability;
//				CCard *p = Find(temp.cardobjid);
//				if (p == NULL)
//				{
//					IME_DEBUG("%u user have objid %u in first team but not in packet", m_pCRole->GetdwAccountId(), temp.cardobjid); 
//					return -1;
//				}
//				temp.attribute = p->GetbyAttribute();
//				temp.attackskill = p->GetdwAttackSkillIndex();
//				temp.defenceskill = p->GetdwDeffenseSkillIndex();
//				STC_CONF_CARD_SKILL *pConfSkill;
//
//				temp.attackskilltype = 0;
//				if (temp.attackskill != 0)
//				{
//					pConfSkill = CConfCardSkill::Find(temp.attackskill);
//					if (pConfSkill == NULL)
//					{
//						IME_ERROR("this skill index is not in csv index %u", temp.attackskill);
//						return -1;
//					}
//					temp.attackskilltype = pConfSkill->wType;
//				}
//
//				temp.defenceskilltype = 0;
//				if (temp.defenceskill != 0)
//				{
//					pConfSkill = CConfCardSkill::Find(temp.defenceskill);
//					if (pConfSkill == NULL)
//					{
//						IME_ERROR("this skill index is not in csv index %u", temp.defenceskill);
//						return -1;
//					}
//					temp.defenceskilltype = pConfSkill->wType;
//				}
//
//				m_vecForDuration.push_back(temp);
//				SetAttributeDuration(m_pCRole->GetRealAttribute());				
////				IME_DEBUG("number %u, objid %u attribute %u attackskill %u defence skill %u attack %u defence %u skill_probability %u", i, m_vecForDuration[i].cardobjid, m_vecForDuration[i].attribute, m_vecForDuration[i].attackskill, m_vecForDuration[i].defenceskill, m_vecForDuration[i].attack, m_vecForDuration[i].defence, m_vecForDuration[i].skill_probability);
//			}
//		}	
//	}
//	else
//	{
//		for (; i < m_vecDefenseTeam.size(); i++)
//		{
//			if (i >= m_vecItemEquip2.size())
//			{
//				IME_ERROR("1st team number != item equip number");
//				return -1;
//			}		
//
//			if (m_vecDefenseTeam[i] != 0)
//			{
//				temp.cardobjid = m_vecDefenseTeam[i];
//				if (m_vecItemEquip2[i].attack_high_probability != 0)
//				{
//					if (CUtil::RandEither(m_vecItemEquip2[i].attack_high_probability, 100))
//					{
//						temp.attack = m_vecItemEquip2[i].si_attack_max;
//					}
//					else
//					{
//						temp.attack = CUtil::RandByRange(m_vecItemEquip2[i].si_attack_min, m_vecItemEquip2[i].si_attack_max - 1);
//					}
//				}
//				else
//				{
//					temp.attack = CUtil::RandByRange(m_vecItemEquip2[i].si_attack_min, m_vecItemEquip2[i].si_attack_max);
//				}
//				if (m_vecItemEquip2[i].defence_high_probability != 0)
//				{
//					if (CUtil::RandEither(m_vecItemEquip2[i].defence_high_probability, 100))
//					{
//						temp.defence = m_vecItemEquip2[i].si_defence_max;
//					}
//					else
//					{
//						temp.defence = CUtil::RandByRange(m_vecItemEquip2[i].si_defence_min, m_vecItemEquip2[i].si_defence_max - 1);
//					}
//				}
//				else
//				{
//					temp.defence = CUtil::RandByRange(m_vecItemEquip2[i].si_defence_min, m_vecItemEquip2[i].si_defence_max);
//				}
//
//				temp.skill_probability = m_vecItemEquip2[i].skill_probability;
//				CCard *p = Find(temp.cardobjid);
//				if (p == NULL)
//				{
//					IME_DEBUG("%u user have objid %u in first team but not in packet", m_pCRole->GetdwAccountId(), temp.cardobjid); 
//					return -1;
//				}
//				temp.attribute = p->GetbyAttribute();
//				temp.attackskill = p->GetdwAttackSkillIndex();
//				temp.defenceskill = p->GetdwDeffenseSkillIndex();
//				STC_CONF_CARD_SKILL *pConfSkill;
//				temp.attackskilltype = 0;
//				if (temp.attackskill != 0)
//				{
//					pConfSkill = CConfCardSkill::Find(temp.attackskill);
//					if (pConfSkill == NULL)
//					{
//						IME_ERROR("this skill index is not in csv index %u", temp.attackskill);
//						return -1;
//					}
//					temp.attackskilltype = pConfSkill->wType;
//				}
//				
//				temp.defenceskilltype = 0;
//				if (temp.defenceskill != 0)
//				{
//					pConfSkill = CConfCardSkill::Find(temp.defenceskill);
//					if (pConfSkill == NULL)
//					{
//						IME_ERROR("this skill index is not in csv index %u", temp.defenceskill);
//						return -1;
//					}
//					temp.defenceskilltype = pConfSkill->wType;
//				}
//				m_vecForDuration.push_back(temp);
//				SetAttributeDuration(m_pCRole->GetRealAttribute());				
//			}
//		}	
//
//	}
//	return 0;
//}


void
CCardPacket::PacketForShowProtocol(WorldPacket &info)
{
    uint8_t teamnumber = 0;
    teamnumber = (uint8_t)m_vecAttackTeam.size();
    info << teamnumber;
    for(int i = 0; i < teamnumber; i++)
    {
        info << m_vecAttackTeam[i];
		IME_DEBUG("%u %u",i, m_vecAttackTeam[i]);
    }

	teamnumber = (uint8_t)m_vecView.size();
	info << teamnumber;
	for (int i = 0; i < teamnumber; i++)
	{
		info << m_vecView[i];
	}
	uint8_t number;
	number = (uint8_t)m_vecEquip1.size();
	info << number;
	for (int i = 0; i < number; i++)
	{
		info << (uint8_t) i;
		info << m_vecEquip1[i].dwWeaponObjId;
		info << m_vecEquip1[i].dwArmorObjId;
		info << m_vecEquip1[i].dwJewelObjId;
		info << m_vecEquip1[i].dwAccessoriesObjId;
		info << m_vecEquip1[i].dwSkillObjId;
		info << m_vecEquip1[i].dwMasterGuide1;
		info << m_vecEquip1[i].dwMasterGuide2;
		info << m_vecEquip1[i].dwMasterGuide3;
		IME_DEBUG("%u %u %u %u", i, m_vecEquip1[i].dwWeaponObjId, m_vecEquip1[i].dwArmorObjId, m_vecEquip1[i].dwJewelObjId);
	}

    info << (uint32_t)m_mapEquipCards.size();
    for(std::map<uint32_t, CCard *>::const_iterator it = m_mapEquipCards.begin(); it != m_mapEquipCards.end(); it++)
    {
        it->second->SerializeClient(info);
    }     
	info << m_pCRole->GetbyCardCapacity(); 

	uint32_t attack,defence,health,Att;
	info << (uint16_t) m_vecItemEquip1.size();
	for (int i = 0; i < m_vecItemEquip1.size(); i++)
	{
		info << (uint8_t)i;
		info << m_vecItemEquip1[i].Attack;
		info << m_vecItemEquip1[i].Defence;
		info << m_vecItemEquip1[i].Health;
		info << m_vecItemEquip1[i].Crit;
		info << m_vecItemEquip1[i].Attribute;
		IME_DEBUG("cal equip val position %u attack %u", i, m_vecItemEquip1[i].Attack);
		attack += m_vecItemEquip1[i].Attack;
		defence += m_vecItemEquip1[i].Defence;
		health += m_vecItemEquip1[i].Health;
	}

	info << m_MasterItemEquip.Attack;
	info << m_MasterItemEquip.Defence;
	info << m_MasterItemEquip.Health;
	info << m_MasterItemEquip.Crit;
	info << m_MasterItemEquip.Attribute;
	IME_DEBUG("cal equip master attack %u", m_MasterItemEquip.Attack);
	attack += m_MasterItemEquip.Attack;
	defence += m_MasterItemEquip.Defence;
	health += m_MasterItemEquip.Health;
	Att = POWER_CALCULATE(attack,defence,health);
	info << Att;
	return ;
}		/* -----  end of method CCardPacket::PacketForFriendProtocol(WorldPacket &info)  ----- */

void CCardPacket::HandlerLineUpInfo()
{
	WorldPacket info(CMD_SC_CARD_LINEUP_INFO);
	for (int i = 0; i < MAX_CARD_CAPACITY; i++)
	{
		info << m_vecLineUp[i];
		IME_DEBUG("team lineup %u", m_vecLineUp[i]);
	}		
	m_pCRole->SendPacket(&info);
}

void CCardPacket::HandlerLineUpChange(WorldPacket &info)
{
	IME_CHAR_LOG("action [userid %u change line team]", m_pCRole->GetdwAccountId());
	uint32_t objid;
	std::vector<uint32_t> vec;
	for (int i = 0; i < MAX_CARD_CAPACITY; i++)
	{
		info >> objid;
		if (objid != 0)
		{
			if (find(vec.begin(), vec.end(), objid) != vec.end())
			{
				m_pCRole->SendErrorMsg(ERRNO_MSG_CARD_DUPLICATE_LINEUP, CMD_SC_CARD_LINEUP_CHANGE_RESULT); 
				return;
			}
			if (find(m_vecAttackTeam.begin(), m_vecAttackTeam.end(), objid) == m_vecAttackTeam.end())
			{
				m_pCRole->SendErrorMsg(ERRNO_MSG_CARD_LINEUP_NOT_IN_EQUIPMENT, CMD_SC_CARD_LINEUP_CHANGE_RESULT); 
				return;
			}

			if (Find(objid) == NULL)
			{
				m_pCRole->SendErrorMsg(ERRNO_MSG_CARD_NOT_IN_PACKET, CMD_SC_CARD_LINEUP_CHANGE_RESULT);
				return;
			}
			vec.push_back(objid);
		}
		else
		{
			vec.push_back(objid);
		}
	}
	m_vecLineUp = vec;
	
	m_vecView = m_vecLineUp;
	for (int i = 0; i < m_vecView.size(); )
	{
		if (m_vecView[i] == 0)
		{
			m_vecView.erase(m_vecView.begin() + i);
		}
		else
		{
			i++;
		}
	}

	HandlerLineUpInfo();
	HandlerViewInfo();

	WorldPacket pkg(CMD_SC_CARD_LINEUP_CHANGE_RESULT);
	pkg << (uint16_t) 0;
	m_pCRole->SendPacket(&pkg);
}

int CCardPacket::PrepareDurationData(std::vector<CDuration::cardforduration_t> &vec, std::vector<uint32_t> &vecline, std::vector<int> &vecRelationshipEffect)
{
	vecline = m_vecLineUp;
	vec.clear();
	vec.resize(MAX_CARD_CAPACITY + 1);
	CCard *pCard = NULL;
	CCard *pSkillCard = NULL;

	int i = 0;
	for (i = 0; i < m_vecAttackTeam.size(); i++)
	{
		if (m_vecAttackTeam[i] == 0)
		{
			continue;
		}
		else
		{
			if (find(m_vecLineUp.begin(), m_vecLineUp.end(), m_vecAttackTeam[i]) == m_vecLineUp.end())
			{
				continue;
			}

			vec[i].CardObjId = m_vecAttackTeam[i];
			pCard = Find(m_vecAttackTeam[i]);
			if (pCard == NULL)
			{
				IME_ERROR("can not find this objid %u in packet", m_vecAttackTeam[i]);
				return -1;
			}
			vec[i].CardIndex = pCard->GetdwIndex();
//			vec[i].TalentSkill = pCard->GetdwInitSkillIndex();
			if (m_vecEquip1[i].dwSkillObjId != 0)
			{
				pSkillCard = Find(m_vecEquip1[i].dwSkillObjId);
				if (pSkillCard == NULL)
				{
					IME_ERROR("can not find this objid %u in packet", m_vecEquip1[i].dwSkillObjId);
					return -1;
				}
				vec[i].BigSkill = pSkillCard->GetdwSkillIndex();
				vec[i].BigSkillLv = pSkillCard->GetwLevel();
			}
			else
			{
				vec[i].BigSkill = 0;
				vec[i].BigSkillLv = 0;
			}
			itemequip_t tempM, tempE;
			if (EquipCardCalForDuration(tempM, tempE, i, 1))
			{
				IME_ERROR("cal for duration fail position %u", i);
				return -1;
			}

			vec[i].AttackM = tempM.Attack * this->addAttackTimes;
			vec[i].AttackE = tempE.Attack;
			vec[i].DefenceM = tempM.Defence;
			vec[i].DefenceE = tempE.Defence;
			vec[i].HealthMaxM = tempM.Health;
			vec[i].HealthMaxE = tempE.Health;
			vec[i].Health = tempM.Health + tempE.Health;
			vec[i].Crit = tempM.Crit + tempE.Crit;
			vec[i].Attribute = m_vecItemEquip1[i].Attribute;
			vec[i].SkillPro = tempM.SkillPro + tempE.SkillPro;
			vec[i].SkillPower = tempM.SkillPower + tempE.SkillPower;
			vec[i].CardLv = pCard->GetwLevel();
			vec[i].CardType = pCard->GetbyType();
			vec[i].Rare = pCard->GetbyRare();

			vec[i].DurationExp = tempM.DurationExp;
			vec[i].DurationCoin = tempM.DurationCoin;
			vec[i].ObjDrop = tempM.ObjDrop;
			vec[i].StrengthRecover = tempM.StrengthRecover;
			vec[i].EnergyRecover = tempM.EnergyRecover;

			vec[i].AddHarm = tempM.AddHarm;
			vec[i].ReduceHarm = tempM.ReduceHarm;
			vec[i].AddCritHarm = tempM.AddCritHarm;
			vec[i].BreakCount = pCard->GetdwLimitBreak();

			IME_DEBUG("objid %u Cardlv %u", pCard->GetdwObjId(), pCard->GetwLevel());
		}
	}
	
	uint32_t masterid = 0;
	if (m_masterForJoin.CardObjId != 0)
	{
		vec[i] = m_masterForJoin;
		masterid = m_masterForJoin.CardObjId;
	}
	else
	{
		IME_DEBUG("master objid %u", m_MasterCard);
		if (m_MasterCard != 0)
		{
			itemequip_t tempM, tempE;
			if (EquipCardCalForDuration(tempM, tempE, -1, 1))
			{
				IME_ERROR("cal for duration fail master");
				return -1;
			}

			vec[i].CardObjId = m_MasterCard;
			pCard = Find(m_MasterCard);
			if (pCard == NULL)
			{
				IME_ERROR("can not find this objid %u in packet", m_MasterCard);
				return -1;
			}
			vec[i].CardIndex = pCard->GetdwIndex();

			if (m_MasterEquip.dwSkillObjId != 0)
			{
				pSkillCard = Find(m_MasterEquip.dwSkillObjId);
				if (pSkillCard == NULL)
				{
					IME_ERROR("can not find this objid %u in packet", m_MasterEquip.dwSkillObjId);
					return -1;
				}
				vec[i].BigSkill = pSkillCard->GetdwSkillIndex();
				vec[i].BigSkillLv = pSkillCard->GetwLevel();
			}
			else
			{
				vec[i].BigSkill = 0;
				vec[i].BigSkillLv = 0;
			}

			vec[i].AttackM = tempM.Attack * this->addAttackTimes;
			vec[i].AttackE = tempE.Attack;
			vec[i].DefenceM = tempM.Defence;
			vec[i].DefenceE = tempE.Defence;
			vec[i].Health = (tempM.Health + tempE.Health) * DURATION_MASTER_HEALTH_PARA;
			vec[i].HealthMaxM = tempM.Health * DURATION_MASTER_HEALTH_PARA;
			vec[i].HealthMaxE = tempE.Health * DURATION_MASTER_HEALTH_PARA;
			vec[i].Crit = tempM.Crit + tempE.Crit;
			vec[i].Attribute = m_MasterItemEquip.Attribute;
			vec[i].SkillPro = tempM.SkillPro + tempE.SkillPro;
			vec[i].SkillPower = tempM.SkillPower + tempE.SkillPower;
			vec[i].CardLv = pCard->GetwLevel();
			vec[i].CardType = pCard->GetbyType();
			vec[i].Rare = pCard->GetbyRare();

			vec[i].DurationExp = tempM.DurationExp;
			vec[i].DurationCoin = tempM.DurationCoin;
			vec[i].ObjDrop = tempM.ObjDrop;
			vec[i].StrengthRecover = tempM.StrengthRecover;
			vec[i].EnergyRecover = tempM.EnergyRecover;

			vec[i].AddHarm = tempM.AddHarm;
			vec[i].ReduceHarm = tempM.ReduceHarm;
			vec[i].AddCritHarm = tempM.AddCritHarm;
			vec[i].BreakCount = pCard->GetdwLimitBreak();
		}
		masterid = m_MasterCard;
	}
	vec.resize(MAX_CARD_CAPACITY + 1);

	IME_DEBUG("===================================");
	for (int j = 0; j < vecline.size(); j++)
	{
		IME_DEBUG("%u, objid %u", j, vecline[j]);
	}
	IME_DEBUG("******");
	for (int j = 0; j < vec.size(); j++)
	{
		IME_DEBUG("%u, objid %u", j, vec[j].CardObjId);
	}
	IME_DEBUG("===================================");

	std::vector<int> vecCard;
	std::vector<int> vecRelationshipSkill;
	for (int k = 0; k < m_vecAttackTeam.size(); k++)
	{
		if (m_vecAttackTeam[k] != 0)
		{
			pCard = Find(m_vecAttackTeam[k]);
			if (pCard == NULL)
			{
				IME_ERROR("can not find this card objid %u", m_vecAttackTeam[k]);
				return false;
			}
			vecCard.push_back(pCard->GetdwCardGroup());
			STC_CONF_CARD_BASE *pConfCard = CConfCardBase::Find(pCard->GetdwIndex());
			if (pConfCard == NULL)
			{
				IME_ERROR("can not find card index %u", pCard->GetdwIndex());
				return false;
			}
			vecRelationshipSkill.push_back(pConfCard->dwRelation1);
			vecRelationshipSkill.push_back(pConfCard->dwRelation2);
			vecRelationshipSkill.push_back(pConfCard->dwRelation3);
			vecRelationshipSkill.push_back(pConfCard->dwRelation4);
		}
	}
	pCard = Find(m_MasterCard);
	if (pCard == NULL)
	{
		IME_ERROR("can not find this card objid %u", m_MasterCard);
		return false;
	}
	STC_CONF_CARD_BASE *pConfCard = CConfCardBase::Find(pCard->GetdwIndex());
	if (pConfCard == NULL)
	{
		IME_ERROR("can not find card index %u", pCard->GetdwIndex());
		return false;
	}
	vecRelationshipSkill.push_back(pConfCard->dwRelation1);
	vecRelationshipSkill.push_back(pConfCard->dwRelation2);
	vecRelationshipSkill.push_back(pConfCard->dwRelation3);
	vecRelationshipSkill.push_back(pConfCard->dwRelation4);
	vecCard.push_back(pCard->GetdwCardGroup());
	vecRelationshipEffect = CConfRelationShip::GetRelationshipIndex(vecCard, vecRelationshipSkill);
	vecline.insert(vecline.begin(), masterid);
	return 0;
}

int CCardPacket::PreDataTeamCardData(WorldPacket &by, int number)
{
	by << (uint32_t) min((int)m_vecView.size(), number);
	for (int i = 0; i < min((int)m_vecView.size(), number); i++)
	{
		if (m_vecView[i] != 0)
		{
			CCard *pCard = Find(m_vecView[i]);
			if (pCard == NULL)
			{
				IME_ERROR("can not find this card index %u", m_vecView[i]);
				return -1;
			}
			by << pCard->GetdwIndex();
		}
		else
		{
			by << (uint32_t) 0;
		}
	}
	if (m_MasterCard != 0)
	{
		CCard *pCard = Find(m_MasterCard);
		if (pCard == NULL)
		{
			IME_ERROR("can not find this card index %u", m_MasterCard);
			return -1;
		}
		by << pCard->GetdwIndex();
	}
	else
	{
		by << (uint32_t) 0;
	}
	return 0;
}

int CCardPacket::PreDataTeamCardDataForRob(WorldPacket &by, int number)
{
	by << (uint32_t) min((int)m_vecView.size(), number);
	for (int i = 0; i < min((int)m_vecView.size(), number); i++)
	{
		if (m_vecView[i] != 0)
		{
			CCard *pCard = Find(m_vecView[i]);
			if (pCard == NULL)
			{
				IME_ERROR("can not find this card index %u", m_vecView[i]);
				return -1;
			}
			by << pCard->GetdwIndex();
		}
		else
		{
			by << (uint32_t) 0;
		}
	}
	if (m_MasterCard != 0)
	{
		CCard *pCard = Find(m_MasterCard);
		if (pCard == NULL)
		{
			IME_ERROR("can not find this card index %u", m_MasterCard);
			return -1;
		}
		by << pCard->GetdwIndex();
	}
	else
	{
		by << (uint32_t) 0;
	}

	uint32_t attack = 0;
	uint32_t defence = 0;
	uint32_t health = 0;
	uint32_t Att = 0;
	for (int i = 0; i < m_vecItemEquip1.size(); i++)
	{
		attack += m_vecItemEquip1[i].Attack;
		defence += m_vecItemEquip1[i].Defence;
		health += m_vecItemEquip1[i].Health;
	}
	if (m_MasterCard != 0)
	{
		CCard *pCard = Find(m_MasterCard);
		if (pCard == NULL)
		{
			IME_ERROR("can not find this card index %u", m_MasterCard);
			return -1;
		}
		attack += m_MasterItemEquip.Attack;
		defence += m_MasterItemEquip.Defence;
		health += m_MasterItemEquip.Health;
	}
	Att = POWER_CALCULATE(attack,defence,health);
	by << (uint32_t) Att;

	return 0;
}

int CCardPacket::GetCardIndex(uint32_t &masterindex, std::vector<uint32_t> &vecCardIndex)
{
	vecCardIndex.clear();
	for (int i = 0; i < m_vecView.size(); i++)
	{
		if (m_vecView[i] != 0)
		{
			CCard *pCard = Find(m_vecView[i]);
			if (pCard == NULL)
			{
				IME_ERROR("can not find this card index %u", m_vecView[i]);
				return -1;
			}
			vecCardIndex.push_back(pCard->GetdwIndex());
		}
	}

	if (m_MasterCard != 0)
	{
		CCard *pCard = Find(m_MasterCard);
		if (pCard == NULL)
		{
			IME_ERROR("can not find this card index %u", m_MasterCard);
			return -1;
		}
		masterindex = pCard->GetdwIndex();
	}
	else
	{
		masterindex = 0;
	}
	return 0;
}

int CCardPacket::PreDataTeamCardDataForFriend(WorldPacket &by)
{
	uint32_t attack = 0;
	uint32_t defence = 0;
	uint32_t health = 0;
	uint32_t Att = 0;
//	for (int i = 0; i < m_vecView.size(); i++)
//	{
//		if (m_vecView[i] != 0)
//		{
//			CCard *pCard = Find(m_vecView[i]);
//			if (pCard == NULL)
//			{
//				IME_ERROR("can not find this card index %u", m_vecView[i]);
//				return -1;
//			}
//			attack += pCard->GetdwAttack();
//			defence += pCard->GetdwDefence();
//			health += pCard->GetdwHealth();
//		}
//	}
//
//	if (m_MasterCard != 0)
//	{
//		CCard *pCard = Find(m_MasterCard);
//		if (pCard == NULL)
//		{
//			IME_ERROR("can not find this card index %u", m_MasterCard);
//			return -1;
//		}
//		attack += pCard->GetdwAttack();
//		defence += pCard->GetdwDefence();
//		health += pCard->GetdwHealth();
//		Att = attack / 100 * 1.2 + defence / 100 * 0.6 + health / 100 * 0.8;
//
//		by << (uint32_t) Att;
//		by << pCard->GetdwIndex();
//	}
//	else
//	{
//		Att = attack / 100 * 1.2 + defence / 100 * 0.6 + health / 100 * 0.8;
//
//		by << (uint32_t) Att;
//		by << (uint32_t) 0;
//	}

	for (int i = 0; i < m_vecItemEquip1.size(); i++)
	{
		attack += m_vecItemEquip1[i].Attack;
		defence += m_vecItemEquip1[i].Defence;
		health += m_vecItemEquip1[i].Health;
	}
	if (m_MasterCard != 0)
	{
		CCard *pCard = Find(m_MasterCard);
		if (pCard == NULL)
		{
			IME_ERROR("can not find this card index %u", m_MasterCard);
			return -1;
		}
		attack += m_MasterItemEquip.Attack;
		defence += m_MasterItemEquip.Defence;
		health += m_MasterItemEquip.Health;
		Att = POWER_CALCULATE(attack,defence,health);

		by << (uint32_t) Att;
		by << pCard->GetdwIndex();
	}
	else
	{
		Att = POWER_CALCULATE(attack,defence,health);

		by << (uint32_t) Att;
		by << (uint32_t) 0;
	}
	by << m_pCRole->GetdwLastLoginTime();
	return 0;
}

int CCardPacket::PreDataTeamCardDataForArena(WorldPacket &by)
{
	uint32_t attack = 0;
	uint32_t defence = 0;
	uint32_t health = 0;
	uint32_t Att = 0;
	for (int i = 0; i < m_vecItemEquip1.size(); i++)
	{
		attack += m_vecItemEquip1[i].Attack;
		defence += m_vecItemEquip1[i].Defence;
		health += m_vecItemEquip1[i].Health;
	}

	if (m_MasterCard != 0)
	{
		CCard *pCard = Find(m_MasterCard);
		if (pCard == NULL)
		{
			IME_ERROR("can not find this card index %u", m_MasterCard);
			return -1;
		}
		attack += m_MasterItemEquip.Attack;
		defence += m_MasterItemEquip.Defence;
		health += m_MasterItemEquip.Health;
		Att = POWER_CALCULATE(attack, defence, health);

		by << (uint32_t) Att;
		by << pCard->GetdwIndex();
	}
	else
	{
		Att = POWER_CALCULATE(attack,defence,health);

		by << (uint32_t) Att;
		by << (uint32_t) 0;
	}
	return 0;
}

void CCardPacket::HandlerCardQuipmentInfo()
{
    WorldPacket info(CMD_SC_ROLE_CARD_TEAM_EQUIP_INFO);    
    uint8_t teamnumber = 0;
    teamnumber = (uint8_t)m_vecAttackTeam.size();
    info << teamnumber;
    for(int i = 0; i < teamnumber; i++)
    {
        info << m_vecAttackTeam[i];
		IME_DEBUG("up team %u", m_vecAttackTeam[i]);
    }
	info << m_MasterCard;
	IME_DEBUG("up team master %u", m_MasterCard);
	m_pCRole->SendPacket(&info);
	return;
}

void CCardPacket::HandlerViewInfo()
{
	WorldPacket info(CMD_SC_VIEW_CARD_TEAM_INFO);
	info << (uint16_t)m_vecView.size();
	for (int i = 0; i < m_vecView.size(); i++)
	{
		info << (uint32_t)m_vecView[i];
		IME_DEBUG("up team view %u", m_vecView[i]);
	}
	m_pCRole->SendPacket(&info);
	return;
}

void CCardPacket::HandlerCardChangeSingle(WorldPacket &info)
{
	uint8_t cmd;
	uint32_t position;
	uint32_t cardobjid;
	uint32_t oldcard;
	uint32_t newcard;

	uint32_t clientneedobjid = 0;

	info >> cmd;
	IME_CHAR_LOG("action [userid %u equipment change single cmd %u]", m_pCRole->GetdwAccountId(), cmd);
	if (m_vecAttackTeam.size() < m_pCRole->GetbyCardCapacity())
	{
		m_vecAttackTeam.resize(m_pCRole->GetbyCardCapacity(), 0);
	}
	if (m_vecEquip1.size() < m_pCRole->GetbyCardCapacity())
	{
		m_vecEquip1.resize(m_pCRole->GetbyCardCapacity());
	}

	if (cmd == CARD_CHANGE_ADD_LINE)
	{
		info >> position;
		info >> cardobjid;

		clientneedobjid = cardobjid;
		CCard *p = Find(cardobjid);
		if (p == NULL)
		{
			IME_ERROR("pet card is not in packet objid %u", cardobjid);
			m_pCRole->SendErrorMsg(ERRNO_MSG_EQUIP_NOT_IN_PACKET, CMD_SC_CARD_QUIPMENT_CHANGE_SINGLE_RESULT);
			return;
		}
		if (p->GetbyType() != E_ITEM_PET_CARD)
		{
			IME_ERROR("pet card type is not pet type %u", p->GetbyType());
			m_pCRole->SendErrorMsg(ERRNO_MSG_DATA_ILLEGAL, CMD_SC_CARD_QUIPMENT_CHANGE_SINGLE_RESULT);
			return;
		}

		if (m_mapCards.find(cardobjid) == m_mapCards.end())
		{
			m_pCRole->SendErrorMsg(ERRNO_MSG_EQUIP_NOT_IN_PACKET,CMD_SC_CARD_QUIPMENT_CHANGE_SINGLE_RESULT);
			return;
		}
		if (m_setobjid.find(cardobjid) != m_setobjid.end())
		{
			m_pCRole->SendErrorMsg(ERRNO_MSG_EQUIP_DUPLICATE_OBJID,CMD_SC_CARD_QUIPMENT_CHANGE_SINGLE_RESULT);
			return;
		}
		for (int i = 0; i < m_vecAttackTeam.size(); i++)
		{
			if (m_vecAttackTeam[i] != 0)
			{
//				if (Find(m_vecAttackTeam[i])->GetdwIndex() == p->GetdwIndex())
//				{
//					m_pCRole->SendErrorMsg(ERRNO_MSG_CARD_LINEUP_SAME, CMD_SC_CARD_QUIPMENT_CHANGE_SINGLE_RESULT);
//					return;
//				}
				if (Find(m_vecAttackTeam[i])->GetdwCardGroup() == p->GetdwCardGroup())
				{
					m_pCRole->SendErrorMsg(ERRNO_MSG_CARD_LINEUP_SAME, CMD_SC_CARD_QUIPMENT_CHANGE_SINGLE_RESULT);
					return;
				}
			}
		}
		if (Find(m_MasterCard)->GetdwCardGroup() == p->GetdwCardGroup())
		{
			m_pCRole->SendErrorMsg(ERRNO_MSG_CARD_LINEUP_SAME, CMD_SC_CARD_QUIPMENT_CHANGE_SINGLE_RESULT);
			return;
		}
		std::vector<uint32_t> vecmastergroupindex;
		for(std::set<uint32_t>::iterator it = m_setForEquip1.begin(); it != m_setForEquip1.end(); it++)
		{
			CCard *p = Find(*it);
			if (p == NULL)
			{
				IME_ERROR("card objid can not find in packet objid %u", *it);
				m_pCRole->SendErrorMsg(ERRNO_MSG_EQUIP_NOT_IN_PACKET, CMD_SC_CARD_QUIPMENT_CHANGE_SINGLE_RESULT);
				return;
			}
			vecmastergroupindex.push_back(p->GetdwCardGroup());
		}	
		if (std::find(vecmastergroupindex.begin(), vecmastergroupindex.end(), p->GetdwCardGroup()) != vecmastergroupindex.end())
		{
			IME_ERROR("master guide card is same group");
			m_pCRole->SendErrorMsg(ERRNO_MSG_CARD_LINEUP_SAME, CMD_SC_CARD_QUIPMENT_CHANGE_SINGLE_RESULT);
			return;
		}

		int pos = -1;
		for (int i = 0; i < m_vecAttackTeam.size(); i++)
		{
			if (m_vecAttackTeam[i] == 0)
			{
				pos = i;	
				break;
			}
		}
		if (pos == -1)
		{
			IME_ERROR("user attackteam arriver max limit");
        	m_pCRole->SendErrorMsg(ERRNO_MSG_EQUIP_OVERLOAD,CMD_SC_CARD_QUIPMENT_CHANGE_SINGLE_RESULT);
			return;
		}
		if (position >= MAX_CARD_CAPACITY)
		{
			IME_ERROR("position is unvalid %u", position);
			m_pCRole->SendErrorMsg(ERRNO_MSG_DATA_ILLEGAL, CMD_SC_CARD_QUIPMENT_CHANGE_SINGLE_RESULT);
			return;
		}
		if (m_vecLineUp[position] != 0)
		{
			IME_ERROR("position already card objid %u", m_vecLineUp[position]);	
			m_pCRole->SendErrorMsg(ERRNO_MSG_LINEUP_ALREADY_CARD, CMD_SC_CARD_QUIPMENT_CHANGE_SINGLE_RESULT);
			return;
		}
		m_vecAttackTeam[pos] = cardobjid; 
		m_vecLineUp[position] = cardobjid;

		m_vecView = m_vecLineUp;
		for (int i = 0; i < m_vecView.size(); )
		{
			if (m_vecView[i] == 0)
			{
				m_vecView.erase(m_vecView.begin() + i);
			}
			else
			{
				i++;
			}
		}

		m_setobjid.insert(cardobjid);
		p->CardEquip1(true);
		HandlerLineUpInfo();
		HandlerViewInfo();
		Calculate(true);
		HandlerCardQuipmentInfo();
	}
	else if (cmd == CARD_CHANGE_SWITCH)
	{
		info >> oldcard;
		info >> newcard;
		
		clientneedobjid = newcard;

		CCard *p = Find(oldcard);
		if (p == NULL)
		{
			IME_ERROR("pet card is not in packet objid %u", oldcard);
			m_pCRole->SendErrorMsg(ERRNO_MSG_EQUIP_NOT_IN_PACKET, CMD_SC_CARD_QUIPMENT_CHANGE_SINGLE_RESULT);
			return;
		}
		CCard *pN = Find(newcard);
		if (pN == NULL)
		{
			IME_ERROR("pet card is not in packet objid %u", newcard);
			m_pCRole->SendErrorMsg(ERRNO_MSG_EQUIP_NOT_IN_PACKET, CMD_SC_CARD_QUIPMENT_CHANGE_SINGLE_RESULT);
			return;
		}
		if (pN->GetbyType() != E_ITEM_PET_CARD && pN->GetbyType() != E_ITEM_MASTER_CARD)
		{
			IME_ERROR("pet card type is not pet type %u", pN->GetbyType());
			m_pCRole->SendErrorMsg(ERRNO_MSG_DATA_ILLEGAL, CMD_SC_CARD_QUIPMENT_CHANGE_SINGLE_RESULT);
			return;
		}
		if (oldcard == newcard)
		{
			IME_ERROR("two cards is the same");
			m_pCRole->SendErrorMsg(ERRNO_MSG_DATA_ILLEGAL, CMD_SC_CARD_QUIPMENT_CHANGE_SINGLE_RESULT);
			return;
		}
//		if (p->GetbyType() != pN->GetbyType())
//		{
//			IME_ERROR("this two card type is not same");
//			m_pCRole->SendErrorMsg(ERRNO_MSG_DATA_ILLEGAL, CMD_SC_CARD_QUIPMENT_CHANGE_SINGLE_RESULT);
//			return;
//		}

		if (m_mapCards.find(oldcard) == m_mapCards.end() || m_mapCards.find(newcard) == m_mapCards.end())
		{
			m_pCRole->SendErrorMsg(ERRNO_MSG_EQUIP_NOT_IN_PACKET,CMD_SC_CARD_QUIPMENT_CHANGE_SINGLE_RESULT);
			return;
		}

		
//		std::set<uint32_t>::iterator it = m_setobjid.begin();
//		IME_ERROR("set dump");
//		for ( ; it != m_setobjid.end(); it++)
//		{
//			IME_ERROR("set objid %u", *it);
//		}	

		if (m_setobjid.find(newcard) != m_setobjid.end())
		{
			m_pCRole->SendErrorMsg(ERRNO_MSG_EQUIP_DUPLICATE_OBJID,CMD_SC_CARD_QUIPMENT_CHANGE_SINGLE_RESULT);
			return;
		}

		for (int i = 0; i < m_vecAttackTeam.size(); i++)
		{
			if (m_vecAttackTeam[i] != 0)
			{
				//					if (Find(m_vecAttackTeam[i])->GetdwIndex() == pN->GetdwIndex() && m_vecAttackTeam[i] != oldcard)
				//					{
				//						m_pCRole->SendErrorMsg(ERRNO_MSG_CARD_LINEUP_SAME, CMD_SC_CARD_QUIPMENT_CHANGE_SINGLE_RESULT);
				//						return;
				//					}

				if (Find(m_vecAttackTeam[i])->GetdwCardGroup() == pN->GetdwCardGroup() && m_vecAttackTeam[i] != oldcard)
				{
					m_pCRole->SendErrorMsg(ERRNO_MSG_CARD_LINEUP_SAME, CMD_SC_CARD_QUIPMENT_CHANGE_SINGLE_RESULT);
					return;
				}
			}
		}
		if (Find(m_MasterCard)->GetdwCardGroup() == pN->GetdwCardGroup() && m_MasterCard != oldcard)
		{
			m_pCRole->SendErrorMsg(ERRNO_MSG_CARD_LINEUP_SAME, CMD_SC_CARD_QUIPMENT_CHANGE_SINGLE_RESULT);
			return;
		}

		std::vector<uint32_t> vecmastergroupindex;
		for(std::set<uint32_t>::iterator it = m_setForEquip1.begin(); it != m_setForEquip1.end(); it++)
		{
			CCard *p = Find(*it);
			if (p == NULL)
			{
				IME_ERROR("card objid can not find in packet objid %u", *it);
				m_pCRole->SendErrorMsg(ERRNO_MSG_EQUIP_NOT_IN_PACKET, CMD_SC_CARD_QUIPMENT_CHANGE_SINGLE_RESULT);
				return;
			}
			vecmastergroupindex.push_back(p->GetdwCardGroup());
		}	
		if (std::find(vecmastergroupindex.begin(), vecmastergroupindex.end(), pN->GetdwCardGroup()) != vecmastergroupindex.end())
		{
			IME_ERROR("master guide card is same group");
			m_pCRole->SendErrorMsg(ERRNO_MSG_CARD_LINEUP_SAME, CMD_SC_CARD_QUIPMENT_CHANGE_SINGLE_RESULT);
			return;
		}

//		if (p->GetbyType() == E_ITEM_PET_CARD)
		if (oldcard != m_MasterCard)
		{
			int pos = -1;
			for (int i = 0; i < m_vecAttackTeam.size(); i++)
			{
				if (m_vecAttackTeam[i] == oldcard)
				{
					pos = i;	
					break;
				}
			}
			if (pos == -1)
			{
				IME_ERROR("can not find card objid %u in attack team", oldcard);
				m_pCRole->SendErrorMsg(ERRNO_MSG_DATA_ILLEGAL,CMD_SC_CARD_QUIPMENT_CHANGE_SINGLE_RESULT);
				return;
			}

			int Lpos = -1;
			for (int i = 0; i < m_vecLineUp.size(); i++)
			{
				if (m_vecLineUp[i] == oldcard)
				{
					Lpos = i;
				}
			}
			if (Lpos == -1)
			{
				IME_ERROR("can not find card objid %u in lineup", oldcard);
				m_pCRole->SendErrorMsg(ERRNO_MSG_DATA_ILLEGAL, CMD_SC_CARD_QUIPMENT_CHANGE_SINGLE_RESULT);
				return;
			}

			Find(oldcard)->CardUnEquip1(true);
			Find(newcard)->CardEquip1(true);

			m_vecAttackTeam[pos] = newcard; 
			m_vecLineUp[Lpos] = newcard;

			m_vecView = m_vecLineUp;
			for (int i = 0; i < m_vecView.size(); )
			{
				if (m_vecView[i] == 0)
				{
					m_vecView.erase(m_vecView.begin() + i);
				}
				else
				{
					i++;
				}
			}
			HandlerLineUpInfo();
			HandlerViewInfo();
		}
		else
		{
			Find(oldcard)->CardUnEquip1(true);
			Find(newcard)->CardEquip1(true);
			m_MasterCard = newcard;
		}

		m_setobjid.erase(oldcard);
		m_setobjid.insert(newcard);
		Calculate(true);
		HandlerCardQuipmentInfo();
	}
	else if (cmd == CARD_CHANGE_DISBOARD)
	{
		info >> cardobjid;

		CCard *p = Find(cardobjid);
		if (p == NULL)
		{
			IME_ERROR("pet card is not in packet objid %u", cardobjid);
			m_pCRole->SendErrorMsg(ERRNO_MSG_EQUIP_NOT_IN_PACKET, CMD_SC_CARD_QUIPMENT_CHANGE_SINGLE_RESULT);
			return;
		}

//		if (p->GetbyType() != E_ITEM_PET_CARD)
//		{
//			IME_ERROR("pet card type is not pet type %u", p->GetbyType());
//			m_pCRole->SendErrorMsg(ERRNO_MSG_DATA_ILLEGAL, CMD_SC_CARD_QUIPMENT_CHANGE_SINGLE_RESULT);
//			return;
//		}

		if (m_mapCards.find(cardobjid) == m_mapCards.end())
		{
			m_pCRole->SendErrorMsg(ERRNO_MSG_EQUIP_NOT_IN_PACKET, CMD_SC_CARD_QUIPMENT_CHANGE_SINGLE_RESULT);
			return;
		}

		int ownattackteam = 0;

		int pos = -1;
		for (int i = 0; i < m_vecAttackTeam.size(); i++)
		{
			if (m_vecAttackTeam[i] == cardobjid)
			{
				pos = i;	
//				break;
			}

			if (m_vecAttackTeam[i] != 0)
			{
				ownattackteam++;
			}
		}
		if (pos == -1)
		{
			IME_ERROR("can not find this card in attackteam %u", cardobjid);
        	m_pCRole->SendErrorMsg(ERRNO_MSG_CARD_NOT_IN_ATTACKTEAM, CMD_SC_CARD_QUIPMENT_CHANGE_SINGLE_RESULT);
			return;
		}

		if (ownattackteam <= 1)
		{
			IME_ERROR("this last attack card, must not down");
			m_pCRole->SendErrorMsg(ERRNO_MSG_EQUIP_DATA, CMD_SC_CARD_QUIPMENT_CHANGE_SINGLE_RESULT);
			return;
		}

		int Lpos = -1;
		for (int i = 0; i < m_vecLineUp.size(); i++)
		{
			if (m_vecLineUp[i] == cardobjid)
			{
				Lpos = i;
				break;
			}
		}
		if (Lpos == -1)
		{
			IME_ERROR("can not find card objid %u in lineup", oldcard);
			m_pCRole->SendErrorMsg(ERRNO_MSG_DATA_ILLEGAL, CMD_SC_CARD_QUIPMENT_CHANGE_SINGLE_RESULT);
			return;
		}

		Find(m_vecAttackTeam[pos])->CardUnEquip1(true);

		m_vecAttackTeam[pos] = 0; 
		m_vecLineUp[Lpos] = 0; 

		if (m_vecEquip1[pos].dwWeaponObjId != 0)
		{
			Find(m_vecEquip1[pos].dwWeaponObjId)->CardUnEquip1(true);
			m_setForEquip1.erase(m_vecEquip1[pos].dwWeaponObjId);
			m_vecEquip1[pos].dwWeaponObjId = 0;
		}
		if (m_vecEquip1[pos].dwArmorObjId != 0)
		{
			Find(m_vecEquip1[pos].dwArmorObjId)->CardUnEquip1(true);
			m_setForEquip1.erase(m_vecEquip1[pos].dwArmorObjId);
			m_vecEquip1[pos].dwArmorObjId = 0;
		}
		if (m_vecEquip1[pos].dwJewelObjId != 0)
		{
			Find(m_vecEquip1[pos].dwJewelObjId)->CardUnEquip1(true);
			m_setForEquip1.erase(m_vecEquip1[pos].dwJewelObjId);
			m_vecEquip1[pos].dwJewelObjId = 0;
		}
		if (m_vecEquip1[pos].dwAccessoriesObjId != 0)
		{
			Find(m_vecEquip1[pos].dwAccessoriesObjId)->CardUnEquip1(true);
			m_setForEquip1.erase(m_vecEquip1[pos].dwAccessoriesObjId);
			m_vecEquip1[pos].dwAccessoriesObjId = 0;
		}
		if (m_vecEquip1[pos].dwSkillObjId != 0)
		{
			Find(m_vecEquip1[pos].dwSkillObjId)->CardUnEquip1(true);
			m_setForEquip1.erase(m_vecEquip1[pos].dwSkillObjId);
			m_vecEquip1[pos].dwSkillObjId = 0;
		}
		if (m_vecEquip1[pos].dwMasterGuide1 != 0)
		{
			Find(m_vecEquip1[pos].dwMasterGuide1)->CardUnEquip1(true);
			m_setForEquip1.erase(m_vecEquip1[pos].dwMasterGuide1);
			m_vecEquip1[pos].dwMasterGuide1 = 0;
		}
		if (m_vecEquip1[pos].dwMasterGuide2 != 0)
		{
			Find(m_vecEquip1[pos].dwMasterGuide2)->CardUnEquip1(true);
			m_setForEquip1.erase(m_vecEquip1[pos].dwMasterGuide2);
			m_vecEquip1[pos].dwMasterGuide2 = 0;
		}
		if (m_vecEquip1[pos].dwMasterGuide3 != 0)
		{
			Find(m_vecEquip1[pos].dwMasterGuide3)->CardUnEquip1(true);
			m_setForEquip1.erase(m_vecEquip1[pos].dwMasterGuide3);
			m_vecEquip1[pos].dwMasterGuide3 = 0;
		}

		m_vecView = m_vecLineUp;
		for (int i = 0; i < m_vecView.size(); )
		{
			if (m_vecView[i] == 0)
			{
				m_vecView.erase(m_vecView.begin() + i);
			}
			else
			{
				i++;
			}
		}

		m_setobjid.erase(cardobjid);
		HandlerLineUpInfo();
		HandlerViewInfo();
		Calculate(true);
		HandlerCardQuipmentInfo();
		HandlerItemQuipmentInfo();
	}
	else
	{
		info >> cardobjid;

		clientneedobjid = cardobjid;

		CCard *p = Find(cardobjid);
		if (p == NULL)
		{
			IME_ERROR("pet card is not in packet objid %u", cardobjid);
			m_pCRole->SendErrorMsg(ERRNO_MSG_EQUIP_NOT_IN_PACKET, CMD_SC_CARD_QUIPMENT_CHANGE_SINGLE_RESULT);
			return;
		}
		if (p->GetbyType() != E_ITEM_PET_CARD && p->GetbyType() != E_ITEM_MASTER_CARD)
		{
			IME_ERROR("pet card type is not pet type %u", p->GetbyType());
			m_pCRole->SendErrorMsg(ERRNO_MSG_DATA_ILLEGAL, CMD_SC_CARD_QUIPMENT_CHANGE_SINGLE_RESULT);
			return;
		}

		if (m_mapCards.find(cardobjid) == m_mapCards.end())
		{
			m_pCRole->SendErrorMsg(ERRNO_MSG_EQUIP_NOT_IN_PACKET,CMD_SC_CARD_QUIPMENT_CHANGE_SINGLE_RESULT);
			return;
		}
		if (m_setobjid.find(cardobjid) != m_setobjid.end())
		{
			m_pCRole->SendErrorMsg(ERRNO_MSG_EQUIP_DUPLICATE_OBJID,CMD_SC_CARD_QUIPMENT_CHANGE_SINGLE_RESULT);
			return;
		}

		for (int i = 0; i < m_vecAttackTeam.size(); i++)
		{
			if (m_vecAttackTeam[i] != 0)
			{
//				if (Find(m_vecAttackTeam[i])->GetdwIndex() == p->GetdwIndex())
//				{
//					m_pCRole->SendErrorMsg(ERRNO_MSG_CARD_LINEUP_SAME, CMD_SC_CARD_QUIPMENT_CHANGE_SINGLE_RESULT);
//					return;
//				}

				if (Find(m_vecAttackTeam[i])->GetdwCardGroup() == p->GetdwCardGroup())
				{
					m_pCRole->SendErrorMsg(ERRNO_MSG_CARD_LINEUP_SAME, CMD_SC_CARD_QUIPMENT_CHANGE_SINGLE_RESULT);
					return;
				}
			}
		}
		if (Find(m_MasterCard)->GetdwCardGroup() == p->GetdwCardGroup())
		{
			m_pCRole->SendErrorMsg(ERRNO_MSG_CARD_LINEUP_SAME, CMD_SC_CARD_QUIPMENT_CHANGE_SINGLE_RESULT);
			return;
		}

		std::vector<uint32_t> vecmastergroupindex;
		for(std::set<uint32_t>::iterator it = m_setForEquip1.begin(); it != m_setForEquip1.end(); it++)
		{
			CCard *p = Find(*it);
			if (p == NULL)
			{
				IME_ERROR("card objid can not find in packet objid %u", *it);
				m_pCRole->SendErrorMsg(ERRNO_MSG_EQUIP_NOT_IN_PACKET, CMD_SC_CARD_QUIPMENT_CHANGE_SINGLE_RESULT);
				return;
			}
			vecmastergroupindex.push_back(p->GetdwCardGroup());
		}	
		if (std::find(vecmastergroupindex.begin(), vecmastergroupindex.end(), p->GetdwCardGroup()) != vecmastergroupindex.end())
		{
			IME_ERROR("master guide card is same group");
			m_pCRole->SendErrorMsg(ERRNO_MSG_CARD_LINEUP_SAME, CMD_SC_CARD_QUIPMENT_CHANGE_SINGLE_RESULT);
			return;
		}

		int pos = -1;
		for (int i = 0; i < m_vecAttackTeam.size(); i++)
		{
			if (m_vecAttackTeam[i] == 0)
			{
				pos = i;	
				break;
			}
		}
		if (pos == -1)
		{
			IME_ERROR("user attackteam arriver max limit");
        	m_pCRole->SendErrorMsg(ERRNO_MSG_EQUIP_OVERLOAD,CMD_SC_CARD_QUIPMENT_CHANGE_SINGLE_RESULT);
			return;
		}
		int Lpos = -1;
		for (int i = 0; i < m_vecLineUp.size(); i++)
		{
			if (m_vecLineUp[i] == 0)
			{
				Lpos = i;
				break;
			}
		}
		if (Lpos == -1)
		{
			IME_ERROR("can not find free in lineup");
			m_pCRole->SendErrorMsg(ERRNO_MSG_DATA_ILLEGAL, CMD_SC_CARD_QUIPMENT_CHANGE_SINGLE_RESULT);
			return;
		}

		m_vecAttackTeam[pos] = cardobjid; 
		m_vecLineUp[Lpos] = cardobjid;

		m_vecView = m_vecLineUp;
		for (int i = 0; i < m_vecView.size(); )
		{
			if (m_vecView[i] == 0)
			{
				m_vecView.erase(m_vecView.begin() + i);
			}
			else
			{
				i++;
			}
		}

		m_setobjid.insert(cardobjid);
		p->CardEquip1(true);
		HandlerLineUpInfo();
		HandlerViewInfo();
		Calculate(true);
		HandlerCardQuipmentInfo();
	}

	WorldPacket pkg(CMD_SC_CARD_QUIPMENT_CHANGE_SINGLE_RESULT);
	pkg << (uint16_t) 0;
	pkg << cmd;
	pkg << clientneedobjid;
	m_pCRole->SendPacket(&pkg);
	return;
}

int CCardPacket::CardBreak(CCard *ped, std::vector<CCard *> vecp, const SSwitchCard &swt)
{
	uint8_t fullbreak = 0;
	uint8_t IsFullBreak = 0;
	if (ped->GetdwLimitBreak() < ped->GetdwLimitBreakNumber())
		fullbreak = 1;	

	WorldPacket info(CMD_SC_CARD_BREAK_AWAKE);
	info << (uint8_t) 0;
	ped->SerializeClient(info);
	info << (uint32_t) vecp.size();
	for (int i = 0; i < vecp.size(); i++)
	{
		CCard *p = vecp[i];
		p->SerializeClient(info);
//		if (ped->GetdwIndex() == p->GetdwIndex())
		if (ped->GetdwCardGroup() == p->GetdwCardGroup())
		{

			uint32_t breakNumber = ped->GetdwLimitBreak() + p->GetdwLimitBreak();
			if (breakNumber > ped->GetdwLimitBreakNumber())
			{
				breakNumber = ped->GetdwLimitBreakNumber();
				if (fullbreak)
					IsFullBreak = 1;
			}

			ped->SetdwLimitBreak(breakNumber);

			if (ped->GetdwLimitBreak() < ped->GetdwLimitBreakNumber())
			{
				if (CUtil::RandEither(ped->GetdwLimitBreakPro(), 100))
				{
					ped->SetdwLimitBreak(ped->GetdwLimitBreak() + 1);
				}	
			}

			if (ped->GetbyRare() == E_CR_NORMAL)
			{
				if (p->GetbyRare() == E_CR_AWAKE)
				{
					ped->SetbyRare(E_CR_AWAKE);
				}
			}

//			if (m_pCRole->GetwLevel() * STRENGTH_LEVEL_LIMIT_PARA > ped->GetwLevel())
//			{
				ped->AddExp(p->GetdwBreakExp());
//			}
			if (ped->Calculate(true))
			{
				IME_ERROR("card break calculate fail");
				return -1;
			}

			m_pCRole->GetCMission()->CardBreakNumber(0, 1);
		}
	}
	if (IsFullBreak)
		m_pCRole->GetCMission()->HaveFullBreakCard(0, 1);
	ped->AddExp(swt.AddExp);
	ped->Calculate(true);
	ped->SerializeClient(info);

	m_pCRole->SendPacket(&info);
	return 0;
}

bool CCardPacket::IsFull(uint32_t number, uint32_t cardindex)
{
	//IME_DEBUG("now %u, limit %u", m_dwCardNumber, m_dwTotalSpace);
	//return (m_dwCardNumber + number > m_dwTotalSpace ? true : false);
	if (cardindex != 0)
	{
		STC_CONF_CARD_BASE *pConf = CConfCardBase::Find(cardindex);
		if (pConf == NULL)
			return false;
		if (pConf->byType == E_ITEM_PET_CARD || pConf->byType == E_ITEM_MASTER_CARD)
		{
			return (m_dwCardNumberRole + number > GlobalConfig::RoleCardLimit + m_pCRole->GetExtraRolePacketLimit() ? true : false);
		}
		else
		{
			return (m_dwCardNumberEquip + number > GlobalConfig::EquipCardLimit + m_pCRole->GetExtraEquipPacketLimit() ? true : false);
		}
	}
	bool ok = false;
	if (m_dwCardNumberRole + number > GlobalConfig::RoleCardLimit + m_pCRole->GetExtraRolePacketLimit())
		ok = true;
	if (m_dwCardNumberEquip + number > GlobalConfig::EquipCardLimit + m_pCRole->GetExtraEquipPacketLimit())
		ok = true;
	return ok;
}

bool CCardPacket::IsFullSpecial(std::vector<SProduct> tmp)
{
	uint32_t number = 0;
	for(int i = 0; i < tmp.size(); i++)
	{ 
		if (tmp[i].type == E_OBJ_CARD_MORE)
		{
			number += tmp[i].para2;
		}
		else if (tmp[i].type == E_OBJ_CARD || tmp[i].type == E_OBJ_SKILL || tmp[i].type == E_OBJ_AWAKE_CARD || tmp[i].type == E_OBJ_COL_CARD )
		{
			number += 1;
		}
	}
	if (IsFull(number, 0))
	{
		return true;
	}
	return false;
}

void CCardPacket::PreDataForUpgrade(std::map<uint32, std::vector<CConfUpgrade::SCard> > &map, uint32_t objid)
{
	CConfUpgrade::SCard c;
	std::map<uint32_t, CCard *>::iterator it;
	for (it = m_mapCards.begin(); it != m_mapCards.end(); it++)
	{
		IME_DEBUG("before card index %u", it->second->GetdwIndex());
		if (m_setobjid.find(it->first) != m_setobjid.end())
			continue;
		if (m_setForEquip1.find(it->first) != m_setForEquip1.end())
			continue;
		if (it->first == objid)
			continue;
		c.lv = it->second->GetwLevel();
		c.objid = it->second->GetdwObjId();
		map[it->second->GetdwIndex()].push_back(c);
		IME_DEBUG("after card index %u objid %u lv %u", it->second->GetdwIndex(), c.objid, c.lv);
	}
}

void CCardPacket::PreDataForRank(SFBRankRole &tmp, uint32_t devote)
{
	tmp.userid = m_pCRole->GetdwAccountId();
	tmp.level = m_pCRole->GetwLevel();
	tmp.name = m_pCRole->GetstrUserName();
	for (int i = 0; i < m_vecView.size(); i++)
	{
		tmp.cardindex.push_back(Find(m_vecView[i])->GetdwIndex());
	}
	tmp.masterindex = Find(m_MasterCard)->GetdwIndex();
	tmp.devote = devote;
}

CDuration::cardforduration_t CCardPacket::CreateMasterEquipForJoin(uint32_t objid, uint32_t rate)
{
		itemequip_t tempM, tempE;
		CDuration::cardforduration_t d;
		if (EquipCardCalForDuration(tempM, tempE, -1, 1))
		{
			IME_ERROR("cal for duration fail master");
			return d;
		}
		
		d.CardObjId = objid;
		CCard *pCard = Find(m_MasterCard);
		if (pCard == NULL)
		{
			IME_ERROR("can not find this objid %u in packet", m_MasterCard);
			return d;
		}
		d.CardIndex = pCard->GetdwIndex();

		if (m_MasterEquip.dwSkillObjId != 0)
		{
			CCard *pSkillCard = Find(m_MasterEquip.dwSkillObjId);
			if (pSkillCard == NULL)
			{
				IME_ERROR("can not find this objid %u in packet", m_MasterEquip.dwSkillObjId);
				return d;
			}
			d.BigSkill = pSkillCard->GetdwSkillIndex();
			d.BigSkillLv = pSkillCard->GetwLevel();
		}
		else
		{
			d.BigSkill = 0;
			d.BigSkillLv = 0;
		}

		d.AttackM = tempM.Attack * rate / 100.0;
		d.AttackE = tempE.Attack * rate / 100.0;
		d.DefenceM = tempM.Defence * rate / 100.0;
		d.DefenceE = tempE.Defence * rate / 100.0;
		d.Health = (tempM.Health + tempE.Health) * DURATION_MASTER_HEALTH_PARA * rate / 100.0;
		d.HealthMaxM = tempM.Health * DURATION_MASTER_HEALTH_PARA * rate / 100.0;
		d.HealthMaxE = tempE.Health * DURATION_MASTER_HEALTH_PARA * rate / 100.0;
		d.Crit = (tempM.Crit + tempE.Crit) * rate / 100.0;
		d.Attribute = m_MasterItemEquip.Attribute;
		d.SkillPro = tempM.SkillPro + tempE.SkillPro;
		d.SkillPower = tempM.SkillPower + tempE.SkillPower;
		d.CardLv = pCard->GetwLevel();
		d.CardType = pCard->GetbyType();
		d.Rare = pCard->GetbyRare();

		d.DurationExp = tempM.DurationExp;
		d.DurationCoin = tempM.DurationCoin;
		d.ObjDrop = tempM.ObjDrop;
		d.StrengthRecover = tempM.StrengthRecover;
		d.EnergyRecover = tempM.EnergyRecover;

		d.AddHarm = tempM.AddHarm;
		d.ReduceHarm = tempM.ReduceHarm;
		d.AddCritHarm = tempM.AddCritHarm;
		d.BreakCount = pCard->GetdwLimitBreak();
		return d;
}

void CCardPacket::PreDataRole(SJoin &tmp)
{
	tmp.userid = 0;
	tmp.masterindex = 0;

	tmp.userid = m_pCRole->GetdwAccountId();
	tmp.level = m_pCRole->GetwLevel();
	tmp.username = m_pCRole->GetstrUserName();
	CCard *pCard = Find(m_MasterCard);
	if (pCard == NULL)
	{
		IME_ERROR("can not find this card index %u", m_MasterCard);
		return;
	}
	tmp.masterindex = pCard->GetdwIndex();
	tmp.breakcount = pCard->GetdwLimitBreak();
	tmp.maxbreakcount = pCard->GetdwLimitBreakNumber();
	tmp.awake = pCard->GetbyRare();
	tmp.sigature = m_pCRole->GetstrSignature();
	return;
}

int CCardPacket::FixData(uint32_t userid)
{
	{
    std::string attack_team;
//    std::string defense_team;
//    std::string suit_attribute;
	std::string lineup;
    std::string cards;
	std::string equip1;
//	std::string equip2;
	std::string master;
	std::string equip_master;
    if (!CDBCtrl::GetUserCardPacketData(attack_team, lineup, cards, userid, equip1, master, equip_master))
    {
        m_bIsInit = false;
		IME_ERROR("load data fail from db");
        return -1;
    }
    StrSplit(attack_team, ",", m_vecAttackTeam);
//    StrSplit(defense_team, ",", m_vecDefenseTeam);
//    StrSplit(suit_attribute, ",", m_vecSuitAttribute);

	std::vector<uint32_t> vecMaster;
	StrSplit(master, ",", vecMaster);
	if (vecMaster.size() == 0)
	{
		m_MasterCard = 0;
	}
	else
	{
		m_MasterCard = vecMaster[0];
	}

    StrSplit(lineup, ",", m_vecLineUp);
	m_vecLineUp.resize(MAX_CARD_CAPACITY);
	for (int k = 0; k < m_vecAttackTeam.size(); k++)
	{
		m_setobjid.insert(m_vecAttackTeam[k]);
		IME_DEBUG("1st team %u objid [%u]", k, m_vecAttackTeam[k]);
	}
	m_setobjid.insert(m_MasterCard);
	m_setobjid.erase(0);
	
//	std::set<uint32_t>::iterator it = m_setobjid.begin();
//	IME_ERROR("set data dump");
//	for ( ; it != m_setobjid.end(); it++)
//	{
//		IME_ERROR("set objid %u", *it);
//	}	

//	for (int k = 0; k < m_vecDefenseTeam.size(); k++)
//	{
//		IME_DEBUG("2nd team %u objid [%u]", k, m_vecDefenseTeam[k]);
//	}

	std::vector<std::string> vectemp;
	std::vector<uint32_t> vectempint;
	int i;
	CUtil::StrSplit(equip1, "|", vectemp);	
	for (i = 0; i < vectemp.size(); i++)
	{
		StrSplit(vectemp[i], ",", vectempint);
		if (vectempint.size() < 8)
			vectempint.resize(8, 0);
		if (vectempint.size() != 8)
		{
			IME_ERROR("cardpacket equip column is not 8");
			return -1;
		}
//		equip_t tempequip(vectempint[0], vectempint[1], vectempint[2]);
		equip_t tempequip;
		tempequip.dwWeaponObjId = vectempint[0];
		tempequip.dwArmorObjId = vectempint[1];
		tempequip.dwJewelObjId = vectempint[2];
		tempequip.dwAccessoriesObjId = vectempint[3];
		tempequip.dwSkillObjId = vectempint[4];
		tempequip.dwMasterGuide1 = vectempint[5];
		tempequip.dwMasterGuide2 = vectempint[6];
		tempequip.dwMasterGuide3 = vectempint[7];

		m_vecEquip1.push_back(tempequip);
//		if (vectempint[0] != 0)
//		m_setForEquip1.insert(vectempint[0]);
//		if (vectempint[1] != 0)
//		m_setForEquip1.insert(vectempint[1]);
//		if (vectempint[2] != 0)
//		m_setForEquip1.insert(vectempint[2]);
		m_setForEquip1.insert(vectempint[0]);
		m_setForEquip1.insert(vectempint[1]);
		m_setForEquip1.insert(vectempint[2]);
		m_setForEquip1.insert(vectempint[3]);
		m_setForEquip1.insert(vectempint[4]);
		m_setForEquip1.insert(vectempint[5]);
		m_setForEquip1.insert(vectempint[6]);
		m_setForEquip1.insert(vectempint[7]);
		m_setForEquip1.erase(0);
		IME_DEBUG("1st %u weapon armor jewel %u %u %u", i, vectempint[0], vectempint[1], vectempint[2]);
	}
		StrSplit(equip_master, ",", vectempint);
		if (vectempint.size() != 0)
		{
			if (vectempint.size() < 8)
				vectempint.resize(8, 0);
			if (vectempint.size() != 8)
			{
				IME_ERROR("cardpacket equip column is not 8");
				return -1;
			}
			m_MasterEquip.dwWeaponObjId = vectempint[0];
			m_MasterEquip.dwArmorObjId = vectempint[1];
			m_MasterEquip.dwJewelObjId = vectempint[2];
			m_MasterEquip.dwAccessoriesObjId = vectempint[3];
			m_MasterEquip.dwSkillObjId = vectempint[4];
			m_MasterEquip.dwMasterGuide1 = vectempint[5];
			m_MasterEquip.dwMasterGuide2 = vectempint[6];
			m_MasterEquip.dwMasterGuide3 = vectempint[7];

			m_setForEquip1.insert(vectempint[0]);
			m_setForEquip1.insert(vectempint[1]);
			m_setForEquip1.insert(vectempint[2]);
			m_setForEquip1.insert(vectempint[3]);
			m_setForEquip1.insert(vectempint[4]);
			m_setForEquip1.insert(vectempint[5]);
			m_setForEquip1.insert(vectempint[6]);
			m_setForEquip1.insert(vectempint[7]);
			m_setForEquip1.erase(0);
		}
//	CUtil::StrSplit(equip2, "|", vectemp);	
//	for (i = 0; i < vectemp.size(); i++)
//	{
//		StrSplit(vectemp[i], ",", vectempint);
//		if (vectempint.size() != 3)
//		{
//			IME_ERROR("cardpacket equip column is not 3");
//			return -1;
//		}
//		equip_t tempequip(vectempint[0], vectempint[1], vectempint[2]);
//		m_vecEquip2.push_back(tempequip);
//		if (vectempint[0] != 0)
//		m_setForEquip2.insert(vectempint[0]);
//		if (vectempint[1] != 0)
//		m_setForEquip2.insert(vectempint[1]);
//		if (vectempint[2] != 0)
//		m_setForEquip2.insert(vectempint[2]);
//		IME_DEBUG("2nd %u weapon armor jewel %u %u %u", i, vectempint[0], vectempint[1], vectempint[2]);
//	}

    deserialize(cards);

	m_vecView = m_vecLineUp;
	for (int i = 0; i < m_vecView.size(); )
	{
		if (m_vecView[i] == 0)
		{
			m_vecView.erase(m_vecView.begin() + i);
		}
		else
		{
			i++;
		}
	}
    

//	if (EquipCardInfo())
//	{
//		IME_ERROR("card equip fail");
//		return -1;
//	}
//
//
//    if (Calculate(false))
//	{
//		IME_ERROR("card packet calculate fail");
//		return -1;
//	}
	uint32_t attack = 0;
	uint32_t defence = 0;
	uint32_t health = 0;
	uint32_t Att = 0;
	for (int i = 0; i < m_vecAttackTeam.size(); i++)
	{
		CCard *pCard = Find(m_vecAttackTeam[i]);
		if (pCard == NULL)
		{
			continue;
		}
		attack += pCard->GetdwAttack();
		defence += pCard->GetdwDefence();
		health += pCard->GetdwHealth();
	}
	CCard *pCard = Find(m_MasterCard);
	if (pCard != NULL)
	{
		attack = pCard->GetdwAttack();
		defence = pCard->GetdwDefence();
		health = pCard->GetdwHealth();
	}

	Att = POWER_CALCULATE(attack,defence,health);

//		IME_ERROR("weapon value %u", Att);
	if (m_pCRole != NULL && m_pCRole->GetCMission() != NULL)
	{
		m_pCRole->GetCMission()->ForceValue(0, Att);
	}

	m_dwTotalForce = Att;

	}
	{
    std::string attack_team;
//    std::string defense_team;
//    std::string suit_attribute;
	std::string lineup;
    std::string cards;
	std::string master;
    StrMerge(attack_team, ",", m_vecAttackTeam);
//    StrMerge(defense_team, ",", m_vecDefenseTeam);
//    StrMerge(suit_attribute, ",", m_vecSuitAttribute);
	StrMerge(lineup, ",", m_vecLineUp);

	std::vector<uint32_t> vecMaster;
	vecMaster.push_back(m_MasterCard);
	StrMerge(master, ",", vecMaster);
   	
	std::string equip1;
//	std::string equip2;
	int i;
	std::vector<uint32_t> vectemp;
	std::vector<std::string> vectempstr;
	std::string tempstr;
	std::string equip_master;
//	IME_DEBUG("equip1 %u size,equip2 %u size",m_vecEquip1.size(), m_vecEquip2.size());

	for (i = 0; i < m_vecEquip1.size(); i++)
	{
		vectemp.clear();
		vectemp.push_back(m_vecEquip1[i].dwWeaponObjId);
		vectemp.push_back(m_vecEquip1[i].dwArmorObjId);
		vectemp.push_back(m_vecEquip1[i].dwJewelObjId);	
		vectemp.push_back(m_vecEquip1[i].dwAccessoriesObjId);	
		vectemp.push_back(m_vecEquip1[i].dwSkillObjId);	
		vectemp.push_back(m_vecEquip1[i].dwMasterGuide1);
		vectemp.push_back(m_vecEquip1[i].dwMasterGuide2);
		vectemp.push_back(m_vecEquip1[i].dwMasterGuide3);
		StrMerge(tempstr, ",", vectemp);
		vectempstr.push_back(tempstr);
	}

	CUtil::StrMerge(equip1 , "|", vectempstr);

	vectemp.clear();
	vectemp.push_back(m_MasterEquip.dwWeaponObjId);
	vectemp.push_back(m_MasterEquip.dwArmorObjId);
	vectemp.push_back(m_MasterEquip.dwJewelObjId);	
	vectemp.push_back(m_MasterEquip.dwAccessoriesObjId);	
	vectemp.push_back(m_MasterEquip.dwSkillObjId);	
	vectemp.push_back(m_MasterEquip.dwMasterGuide1);	
	vectemp.push_back(m_MasterEquip.dwMasterGuide2);	
	vectemp.push_back(m_MasterEquip.dwMasterGuide3);	
	StrMerge(equip_master, ",", vectemp);

//	vectemp.clear();
//	vectempstr.clear();
//	tempstr.clear();
//	for (i = 0; i < m_vecEquip2.size(); i++)
//	{
//		vectemp.clear();
//		vectemp.push_back(m_vecEquip2[i].dwWeaponObjId);
//		vectemp.push_back(m_vecEquip2[i].dwArmorObjId);
//		vectemp.push_back(m_vecEquip2[i].dwJewelObjId);	
//		StrMerge(tempstr, ",", vectemp);
//		vectempstr.push_back(tempstr);
//	}
//	CUtil::StrMerge(equip2 , "|", vectempstr);
	
    serialize(cards);

	std::string base;
	ByteBuffer by;
	by << m_dwTotalForce;
	CCard *pCard = Find(m_MasterCard);
	if (pCard != NULL)
	{
		by << (uint32_t) 1;
		by << pCard->GetdwObjId();
		by << pCard->GetdwIndex();
		by << pCard->GetwLevel();
		by << pCard->GetdwLimitBreak();
		by << pCard->GetdwLimitBreakNumber();
		by << pCard->GetbyRare();
	}
	else
	{
		by << (uint32_t) 0;
	}
	base.assign((const char *)by.contents(), by.size());

	CDBCtrl::SaveUserCardPacketDataOnlyForBase(userid, base);
	}
    return 0;
}

int CCardPacket::GetPower(){
	uint32 attack = 0;
	uint32 defence = 0;
	uint32 health = 0;
	uint32 att = 0;
	for (int i = 0; i < m_vecItemEquip1.size(); i++) {
		attack += m_vecItemEquip1[i].Attack;
		defence += m_vecItemEquip1[i].Defence;
		health += m_vecItemEquip1[i].Health;
	}

	attack += m_MasterItemEquip.Attack;
	defence += m_MasterItemEquip.Defence;
	health += m_MasterItemEquip.Health;
	att = POWER_CALCULATE(attack,defence,health);
	return att;
}

void CCardPacket::setAttack(uint32 times){
	this->addAttackTimes = times;
}
