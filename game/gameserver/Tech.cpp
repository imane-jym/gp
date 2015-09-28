/*
 * =====================================================================================
 *
 *       Filename:  Tech.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2012年11月03日 02时30分11秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  imane (), imanecr@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include <map>
#include "Tech.h"
#include "Role.h"
#include "ErrorMsg.h"
#include "CmdDefine.h"
#include "ConfigData.h"
#include "EnumDefines.h"
#include "CardPacket.h"


CTech::CTech():IsUpdate(false),IsInit(false)
{
	return ;
}		/* -----  end of method CTech::CTECH  ----- */


int CTech::Init (CRole *pCRole)
{
	m_pCRole = pCRole;
	if (LoadData())
	{
		IME_ERROR("load tech data fail");
		return -1;
	}

	IsInit = true;
	return 0;
}		/* -----  end of method CTech::Init  ----- */


int CTech::LoadData ()
{
	std::string str;
//	if (!CDBCtrl::GetUserTechData(str, m_pCRole->GetdwAccountId()))
//		return -1;
	if (str.size() != 0)
	{
		ByteBuffer content;
		uint32_t number;
		uint32_t tempindexid;
		uint16_t level;
		content.append(str.data(), str.size());

		STC_CONF_TECH_RATE *pConf; 
		STC_CONF_TECH *p;

		content >> number;
		for (int i = 0; i < number; i++)
		{
			content >> tempindexid;
			content >> level;
			IME_LOG("indexid %u, level %u", tempindexid, level);
			if (level == 0)
				continue;
			m_mapTech[tempindexid] = level;

			p = CConfTech::Find(tempindexid);
			if (tempindexid < ATTACK || tempindexid > EXP)
			{
				IME_ERROR("tech id %u is unvalid", tempindexid);
				return -1;
			}
			if (p == NULL)
			{
				IME_ERROR("can not find this tech conf %u", tempindexid);
				return -1;
			}
			pConf = CConfTechRate::Find(level);
			if (pConf == NULL)
			{
				IME_ERROR("can not load csv in level %u of tech id %u", level, tempindexid);
				return -1;
			}

//			if (p->byType == 2)
//			{
//				m_mapEffectVal[tempindexid] = pConf->dwRate; 	
//			}
//			else if (p->byType == 3)
//			{
//				m_mapEffectVal[tempindexid] = pConf->dwVal; 	
//			}
//			else
//			{
//				m_mapEffectVal[tempindexid] = pConf->dwVal2; 
//			}
			if (p->byType >= TECH_TYPE_MAX)
			{
				IME_ERROR("type para %u is unvalid", p->byType);
				return -1;
			}	
			else
			{
				m_mapEffectVal[tempindexid] = pConf->dwVal[p->byType];
			}
			IME_LOG("tech id %u, val %u", tempindexid, m_mapEffectVal[tempindexid]);
		}
	}
	return 0;
}		/* -----  end of method CTech::LoadData  ----- */


int CTech::SaveData ()
{
	if (!IsInit)
		return -1;
	if (m_mapTech.size() == 0)
		return 0;
	ByteBuffer content;
	content << (uint32_t) m_mapTech.size();
	std::map<uint32_t, uint16_t>::iterator it = m_mapTech.begin();
	for(; it != m_mapTech.end(); it++)
	{
		content << it->first; 
		content << it->second;
	}	
	std::string temp;
	temp.assign((const char *)content.contents(), content.size());
	CDBCtrl::SaveUserTechData(temp, m_pCRole->GetdwAccountId());
	return 0;
}		/* -----  end of method CTech::SaveData  ----- */

int CTech::DirtySaveData ()
{
	if (!IsInit)
		return -1;

	if (IsUpdate)
	{
//		IME_LOG("save data tech");
		if (SaveData())
		{
			IME_ERROR("save data fail tujian");
			return -1;
		}
		IsUpdate = false;
	}
	return 0;
}		/* -----  end of method CTech::DirtySaveData  ----- */


void CTech::HandlerTechUp (WorldPacket &pkg)
{
	uint32_t techid;
	pkg >> techid;
	int16_t level;
	STC_CONF_TECH_RATE *pConfNNext, *pConfNext; 
	STC_CONF_TECH *p;

	p = CConfTech::Find(techid);
	if (techid < ATTACK || techid > EXP)
	{
		IME_ERROR("tech id %u is unvalid", techid);
        m_pCRole->SendErrorMsg(ERRNO_MSG_DATA_ILLEGAL, CMD_SC_TECH_UPDATE_RESULT);
		return;
	}
	if (p == NULL)
	{
		IME_ERROR("can not find this tech conf %u", techid);
        m_pCRole->SendErrorMsg(ERRNO_MSG_CSV_DATA, CMD_SC_TECH_UPDATE_RESULT);
		return;
	}

	if (m_mapTech.find(p->dwIndex) == m_mapTech.end())
	{
		level = 0;
	}
	else
	{
		level = m_mapTech[p->dwIndex];
	}

	if (level >= CConfTechRate::GetMaxLevel())
	{
		IME_ERROR("tech id %u have been max level", techid);
        m_pCRole->SendErrorMsg(ERRNO_MSG_DATA_ILLEGAL, CMD_SC_TECH_UPDATE_RESULT);
		return;
	}	

	pConfNext = CConfTechRate::Find(level + 1);
	if (pConfNext == NULL)
	{
		IME_ERROR("can not load csv in level %u of tech id %u", level + 1, techid);
        m_pCRole->SendErrorMsg(ERRNO_MSG_CSV_LOAD, CMD_SC_TECH_UPDATE_RESULT);
		return;
	}
	
	if (p->byType >= TECH_TYPE_MAX)
	{
		IME_ERROR("type para %u is unvalid", p->byType);
		m_pCRole->SendErrorMsg(ERRNO_MSG_DATA_ILLEGAL, CMD_SC_TECH_UPDATE_RESULT);
		return;
	}

	if (m_pCRole->GetdwCoin() < pConfNext->dwCoin[p->byType])
	{
		IME_ERROR("tech id %u need consume %u now user have %u", techid, pConfNext->dwCoin, m_pCRole->GetdwCoin());
		m_pCRole->SendErrorMsg(ERRNO_MSG_NOT_ENOUGH_COIN, CMD_SC_TECH_UPDATE_RESULT);
		return;
	}

	if (m_pCRole->GetwLevel() < pConfNext->wRoleLv)
	{
		IME_ERROR("tech id %u need lv %u now user have %u", techid, pConfNext->wRoleLv, m_pCRole->GetwLevel());
//		m_pCRole->SendErrorMsg(ERRNO_MSG_TECH_LEVEL_NOT_ARRIVE, CMD_SC_TECH_UPDATE_RESULT);
		WorldPacket errpkg(CMD_SC_TECH_UPDATE_RESULT);
		errpkg << (uint16_t) ERRNO_MSG_TECH_LEVEL_NOT_ARRIVE;
		errpkg << techid;
		m_pCRole->SendPacket(&errpkg);
		return;
	}
	
	m_pCRole->ChangeCoin(-(pConfNext->dwCoin[p->byType]), SOURCE_OTHER);
	m_mapTech[techid] = level + 1;	
//	if (p->byType == 2)
//	{
//		m_mapEffectVal[techid] = pConfNext->dwRate; 	
//	}
//	else if (p->byType = 4)
//	{
//		m_mapEffectVal[techid] = pConfNext->dwVal; 	
//	}
//	else
//	{
//		m_mapEffectVal[techid] = pConfNext->dwVal2;
//	}
	m_mapEffectVal[techid] = pConfNext->dwVal[p->byType];
	m_pCRole->SendProUpdate();

	if (techid <= HEALTH)
	{
		m_pCRole->GetclsCardPacket()->Calculate(true);	
	}
//	if (techid == ENERGY)
//	{
//		m_pCRole->CalTech(true);
//	}
	
	WorldPacket info(CMD_SC_TECH_UPDATE_RESULT);
	if (level + 1 < CConfTechRate::GetMaxLevel())
	{
		pConfNNext = CConfTechRate::Find(level + 2);
		if (pConfNNext != NULL)
		{
			info << (uint16_t) 0;
			info << (uint32_t) techid;
			info << pConfNNext->wLv;
			info << pConfNNext->wRoleLv;
			info << (uint32_t) pConfNNext->dwVal[p->byType];
			info << pConfNNext->dwCoin[p->byType];
			m_pCRole->SendPacket(&info);
			IsUpdate = true;
			return;
		}
	}
	info << (uint16_t) 0;
	info << (uint32_t) techid;
	info << (uint16_t) -1;
	info << (uint16_t) 0;
	info << (uint32_t) 0;
	info << (uint32_t) 0;
	info << (uint32_t) 0;
	m_pCRole->SendPacket(&info);
	IsUpdate = true;
	return ;
}		/* -----  end of method CTech::HandlerTechUp  ----- */

void CTech::HandlerGroupTechUp(WorldPacket &pkg, CGroup *pCGroup)
{
	uint32_t techid;
	pkg >> techid;
	int16_t level;
	STC_CONF_TECH_RATE *pConfNNext, *pConfNext; 
	STC_CONF_TECH *p;


	p = CConfTech::Find(techid);
	if (techid < ATTACK || techid > EXP)
	{
		IME_ERROR("tech id %u is unvalid", techid);
        m_pCRole->SendErrorMsg(ERRNO_MSG_DATA_ILLEGAL, CMD_SC_GROUP_TECH_UPDATE_RESULT);
		return;
	}
	if (p == NULL)
	{
		IME_ERROR("can not find this tech conf %u", techid);
        m_pCRole->SendErrorMsg(ERRNO_MSG_CSV_DATA, CMD_SC_GROUP_TECH_UPDATE_RESULT);
		return;
	}

	if (m_mapTech.find(p->dwIndex) == m_mapTech.end())
	{
		level = 0;
	}
	else
	{
		level = m_mapTech[p->dwIndex];
	}

	if (level >= CConfTechRate::GetMaxLevel())
	{
		IME_ERROR("tech id %u have been max level", techid);
        m_pCRole->SendErrorMsg(ERRNO_MSG_DATA_ILLEGAL, CMD_SC_GROUP_TECH_UPDATE_RESULT);
		return;
	}	

	pConfNext = CConfTechRate::Find(level + 1);
	if (pConfNext == NULL)
	{
		IME_ERROR("can not load csv in level %u of tech id %u", level + 1, techid);
        m_pCRole->SendErrorMsg(ERRNO_MSG_CSV_LOAD, CMD_SC_GROUP_TECH_UPDATE_RESULT);
		return;
	}

	if (pCGroup->GetwLevel() < pConfNext->wGroupLevelLimit) 
	{
		IME_ERROR("group level %u is not enough", pCGroup->GetwLevel());
        m_pCRole->SendErrorMsg(ERRNO_MSG_GROUP_LEVEL_NOT_ARRIVAL, CMD_SC_GROUP_TECH_UPDATE_RESULT);
		return;
	}

	if (m_pCRole->GetwLevel() < pConfNext->wRoleLv)
	{
		IME_ERROR("tech id %u need lv %u now user have %u", techid, pConfNext->wRoleLv, m_pCRole->GetwLevel());
//		m_pCRole->SendErrorMsg(ERRNO_MSG_TECH_LEVEL_NOT_ARRIVE, CMD_SC_GROUP_TECH_UPDATE_RESULT);
		WorldPacket errpkg(CMD_SC_GROUP_TECH_UPDATE_RESULT);
		errpkg << (uint16_t) ERRNO_MSG_TECH_LEVEL_NOT_ARRIVE;
		errpkg << techid;
		m_pCRole->SendPacket(&errpkg);
		return;
	}

	if (p->byType >= TECH_TYPE_MAX)
	{
		IME_ERROR("type para %u is unvalid", p->byType);
		m_pCRole->SendErrorMsg(ERRNO_MSG_DATA_ILLEGAL, CMD_SC_TECH_UPDATE_RESULT);
		return;
	}

	SGroupMember *pSGM = pCGroup->GetGroupMember(m_pCRole->GetdwAccountId());	
	if (pSGM->dwCredit < pConfNext->dwCreditConsume[p->byType])
	{
		IME_ERROR("userid %u credit is not enough now %u need %u", m_pCRole->GetdwAccountId(), pSGM->dwCredit, pConfNext->dwCreditConsume[p->byType]);
		m_pCRole->SendErrorMsg(ERRNO_MSG_GROUP_CREDIT_NOT_ENGOUH, CMD_SC_GROUP_TECH_UPDATE_RESULT);
		return;
	}
	pSGM->dwCredit -= pConfNext->dwCreditConsume[p->byType];
	WorldPacket GroupMemberpkg;
	pCGroup->PackGroupMemberInfo(m_pCRole->GetdwAccountId(), GroupMemberpkg);
	m_pCRole->SendPacket(&GroupMemberpkg);

	m_mapTech[techid] = level + 1;	
//	if (p->byType == 2)
//	{
//		m_mapEffectVal[techid] = pConfNext->dwRate; 	
//	}
//	else if (p->byType = 4)
//	{
//		m_mapEffectVal[techid] = pConfNext->dwVal; 	
//	}
//	else
//	{
//		m_mapEffectVal[techid] = pConfNext->dwVal2;
//	}
	m_mapEffectVal[techid] = pConfNext->dwVal[p->byType];	

	if (techid <= HEALTH)
	{
		m_pCRole->GetclsCardPacket()->Calculate(true);	
	}
//	if (techid == ENERGY)
//	{
//		m_pCRole->CalTech(true);
//	}
	
	WorldPacket info(CMD_SC_GROUP_TECH_UPDATE_RESULT);
	if (level + 1 < CConfTechRate::GetMaxLevel())
	{
		pConfNNext = CConfTechRate::Find(level + 2);
		if (pConfNNext != NULL)
		{
			info << (uint16_t) 0;
			info << (uint32_t) techid;
			info << pConfNNext->wLv;
			info << pConfNNext->wRoleLv;
			info << (uint32_t) pConfNNext->dwVal[p->byType];
			info << pConfNNext->dwCreditConsume[p->byType];
			m_pCRole->SendPacket(&info);
			IsUpdate = true;
			return;
		}
	}
	info << (uint16_t) 0;
	info << (uint32_t) techid;
	info << (uint16_t) -1;
	info << (uint16_t) 0;
	info << (uint32_t) 0;
	info << (uint32_t) 0;
	info << (uint32_t) 0;
	m_pCRole->SendPacket(&info);
	IsUpdate = true;
	return ;

}

void CTech::HandlerTechInfo ()
{
	CConfTech::MapData *p = CConfTech::GetMap();			
	CConfTech::MapData::iterator it;
	int16_t level;
	STC_CONF_TECH_RATE *pConf;
	WorldPacket info(CMD_SC_TECH_INFO);
	
	info << (uint32_t)p->size();
//	IME_LOG("size %u", p->size());

	for (it = p->begin(); it != p->end(); it++ )
	{
		if (m_mapTech.find(it->first) == m_mapTech.end())
		{
			level = 0;
		}
		else
		{
			level = m_mapTech[it->first];
		}

		if (level >= CConfTechRate::GetMaxLevel())
		{
			level = -1;
		}	

		if (it->second.byType >= TECH_TYPE_MAX)
		{
			IME_ERROR("type para %u is unvalid", it->second.byType);
			return;
		}
		
		info << (uint32_t) it->first;
		if (level >= 0)
		{
			pConf = CConfTechRate::Find(level + 1);	
			if (pConf == NULL)
			{
				IME_ERROR("can not find this tech %u", level + 1);
				return;
			}
			info << (int16_t) pConf->wLv;
			info << (uint16_t) pConf->wRoleLv;
//			if (it->second.byType == 2)
//			{
//				info << (uint32_t) pConf->dwRate;
//			}
//			else 
//			{
				info << (uint32_t) pConf->dwVal[it->second.byType];
//			}
			info << (uint32_t) pConf->dwCoin[it->second.byType];
			info << (uint32_t) pConf->dwCreditConsume[it->second.byType];
		}
		else
		{
			info << (int16_t) level;
			info << (uint16_t) 0;
			info << (uint32_t) 0;
			info << (uint32_t) 0;
			info << (uint32_t) 0;
		}

	}

//	IME_LOG("size %u", p->size());
	m_pCRole->SendPacket(&info);
	return ;
}		/* -----  end of method CTech::HandlerTechInfo  ----- */


void CTech::TechAttack (uint32_t &val)
{
	if (m_mapEffectVal.find(ATTACK) != m_mapEffectVal.end()) 
		val += m_mapEffectVal[ATTACK];
}		/* -----  end of method CTech::TechAttrEffect  ----- */


void
CTech::TechDefence (uint32_t &val)
{
	if (m_mapEffectVal.find(DEFENCE) != m_mapEffectVal.end()) 
		val += m_mapEffectVal[DEFENCE];
}		/* -----  end of method CTech::TechAttrDefence  ----- */


void
CTech::TechHealth (uint32_t &val)
{
	if (m_mapEffectVal.find(HEALTH) != m_mapEffectVal.end()) 
		val += m_mapEffectVal[HEALTH];
	return;
}		/* -----  end of method CTech::TechEnergy  ----- */


void
CTech::TechCoin (uint32_t &val)
{
	if (m_mapEffectVal.find(COIN) != m_mapEffectVal.end()) 
	val += m_mapEffectVal[COIN];
	return ;
}		/* -----  end of method CTech::TechCoin  ----- */


void
CTech::TechExp (uint32_t &val)
{
	if (m_mapEffectVal.find(EXP) != m_mapEffectVal.end()) 
	val += m_mapEffectVal[EXP];
	return ;
}		/* -----  end of method CTech::TechExp  ----- */

