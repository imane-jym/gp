/*
 * =====================================================================================
 *
 *       Filename:  tujian.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2012年11月01日 23时36分00秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  imane (), imanecr@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include <set>
#include "Role.h"
#include "DBCtrl.h"
#include "Tujian.h"
#include "ConfigData.h"
#include "CmdDefine.h"

CTujian::CTujian():IsInit(false),IsUpdate(false)
{
}

int CTujian::Init (CRole *pCRole)
{
	m_pCRole = pCRole;
	if (!LoadData())
	{
		IME_ERROR("load data fail");
		return -1;
	}
	IsInit = true;
	return 0;
}		/* -----  end of method CTujian::Init  ----- */

bool CTujian::LoadData ()
{
	std::string str;
	if (!CDBCtrl::GetUserCardTujianData(str, m_pCRole->GetdwAccountId()))
		return false;
	if (str.size() != 0)
	{
		ByteBuffer content;
		uint32_t number;
		uint32_t tempindexid;
		SDataTujian tmp;
		content.append(str.data(), str.size());
		content >> number;
		for (int i = 0; i < number; i++)
		{
			content >> tempindexid;
			content >> tmp.Rare;
			content >> tmp.BreakCount[E_CR_NORMAL];
			content >> tmp.BreakCount[E_CR_AWAKE];
			content >> tmp.Status;
//			IME_DEBUG("number %u index %u", i, tempindexid);
			m_mapTujian[tempindexid] = tmp;
		}
	}
	return true;
}		/* -----  end of method CTujian::LoadData  ----- */


bool CTujian::SaveData (  )
{
	if (!IsInit)
		return false;
	if (m_mapTujian.size() == 0)
		return true;
	ByteBuffer content;
	content << (uint32_t) m_mapTujian.size();
	std::map<uint32_t, SDataTujian>::iterator it = m_mapTujian.begin();
	for(; it != m_mapTujian.end(); it++)
	{
		content << it->first; 
		content << it->second.Rare;
		content << it->second.BreakCount[E_CR_NORMAL];
		content << it->second.BreakCount[E_CR_AWAKE];
		content << it->second.Status;
	}	
	std::string temp;
	temp.assign((const char *)content.contents(), content.size());
	CDBCtrl::SaveUserCardTujianData(temp, m_pCRole->GetdwAccountId());
	return true;
}		/* -----  end of method CTujian::SaveData  ----- */


int CTujian::DirtySaveData ()
{
	if (!IsInit)
		return -1;

	if (IsUpdate)
	{
		if (!SaveData())
		{
			IME_ERROR("save data fail tujian");
			return -1;
		}
		IsUpdate = false;
	}
	return 0;
}		/* -----  end of method CTujian::DirtySaveData  ----- */


bool CTujian::AddTujian (uint32_t index, uint8_t rare, uint32_t breaknumber, uint8_t status)
{
//	if (type == TUJIAN_CARD)
//	{
		STC_CONF_CARD_BASE *ptemp = CConfCardBase::Find(index);
		if (ptemp == NULL)
		{
			IME_ERROR("add card tujian fail [%u]", index);
			return false;
		}
		if (!ptemp->byIsTujian)
		{
//			IME_DEBUG("not access tujian");
			return false;
		}
//	}
//	else if (type == TUJIAN_MASTER)
//	{
//		STC_CONF_MASTER *ptemp = CConfMaster::Find(objid);
//		if (ptemp == NULL)
//		{
//			IME_ERROR("add master tujian fail [%u]", objid);
//			return false;
//		}
//	}
//	else
//	{
//		IME_ERROR("type %u is unvalid", type);
//		return false;
//	}

	if (rare >= E_CR_MAX)
	{
		return false;
	}
	if (status >= TUJIAN_MAX)
	{
		return false;
	}
	
//	IME_ERROR("index %u, rare %u, breaknumber %u, status %u", index, rare, breaknumber, status);	
	std::map<uint32_t, SDataTujian>::iterator it;
	it = m_mapTujian.find(index);
	if (it != m_mapTujian.end())
	{
		if (it->second.Rare <= rare && it->second.BreakCount[rare] < breaknumber)
		{
			it->second.Rare = rare;
			it->second.BreakCount[rare] = breaknumber;
			for (int i = 0; i < rare; i++)
			{
				if (it->second.BreakCount[i] < breaknumber)
					it->second.BreakCount[i] = breaknumber;
			}
			IsUpdate = true;
		}

//		IME_ERROR("pre status %u, status %u", it->second.Status, status);	
		if (status > it->second.Status)
		{
			it->second.Status = status;
			IsUpdate = true;
		}
//		IME_ERROR("pre status %u, status %u", it->second.Status, status);	
	}
	else
	{
		m_mapTujian[index].Rare = rare;
		for (int i = 0; i <= rare; i++)
		{
			m_mapTujian[index].BreakCount[i] = breaknumber;
		}
		for (int i = rare + 1; i < E_CR_MAX; i++)
		{
			m_mapTujian[index].BreakCount[i] = 0;
		}
		m_mapTujian[index].Status = status;
		IsUpdate = true;
	}
	
	return true;
}		/* -----  end of method CTujian::AddTujian  ----- */

void
CTujian::HandlerCardTujianInfo ()
{
	WorldPacket info(CMD_SC_CARD_TUJIAN_INFO);
	uint32_t number = m_mapTujian.size();
	std::map<uint32_t, SDataTujian>::iterator it;
	int j = 0;
	for(it = m_mapTujian.begin(); it != m_mapTujian.end(); it++)
	{
//		ptemp = CConfCardBase::Find(*it);
//		if (ptemp == NULL)
//		{	
//			IME_ERROR("add card tujian fail [%u]", *it);
//			return;
//		}
//		info << ptemp->dwIndex;
//		info << ptemp->byType;
//		info << ptemp->byAttribute;
//		info << ptemp->byStarLevel;
//		info << ptemp->dwAttackMin;
//		info << ptemp->dwAttackMax;
//		info << ptemp->dwDefenseMin;
//		info << ptemp->dwDefenseMax;
//		info << ptemp->dwEffectIndex;
		info << it->first;
		info << it->second.Rare;
		info << it->second.BreakCount[E_CR_NORMAL];
		info << it->second.BreakCount[E_CR_AWAKE];
		info << it->second.Status;
		j++;
		if (j >= 1500)
		{
			WorldPacket SmallPkg(CMD_SC_CARD_TUJIAN_INFO);
			SmallPkg << (uint32_t) j;
			SmallPkg.append(info.contents(), info.size());
			m_pCRole->SendPacket(&SmallPkg);

			info.clear();
			j = 0;
		}
	}
	if (j < 1500)
	{
		WorldPacket SmallPkg(CMD_SC_CARD_TUJIAN_INFO);
		SmallPkg << (uint32_t) j;
		SmallPkg.append(info.contents(), info.size());
		m_pCRole->SendPacket(&SmallPkg);
	}
	return;
}		/* -----  end of method CTujian::HandlerCardTujianInfo  ----- */


void
CTujian::CardTujianUpdate (uint32_t index, uint8_t type)
{
	WorldPacket info(CMD_SC_CARD_TUJIAN_CHANGE);
//    STC_CONF_CARD_BASE *ptemp;
//	ptemp = CConfCardBase::Find(index);
//	if (ptemp == NULL)
//	{	
//		IME_ERROR("add card tujian fail [%u]", index);
//		return;
//	}
//	info << ptemp->dwIndex;
//	info << ptemp->byType;
//	info << ptemp->byAttribute;
//	info << ptemp->byStarLevel;
//	info << ptemp->dwAttackMin;
//	info << ptemp->dwAttackMax;
//	info << ptemp->dwDefenseMin;
//	info << ptemp->dwDefenseMax;
//	info << ptemp->dwEffectIndex;
	if (m_mapTujian.find(index) == m_mapTujian.end())
		return;
	info << index;
	info << m_mapTujian[index].Rare;
	info << m_mapTujian[index].BreakCount[E_CR_NORMAL];
	info << m_mapTujian[index].BreakCount[E_CR_AWAKE];
	info << m_mapTujian[index].Status;
	m_pCRole->SendPacket(&info);	
}		/* -----  end of method CTujian::CardTujianUpdate  ----- */

bool
CTujian::IsTujianGet(uint32_t index)
{
	if (m_mapTujian.find(index) != m_mapTujian.end())
	{
		if (m_mapTujian[index].Status == TUJIAN_GET)
		{
			return true;
		}
		{
			return false;
		}
	}	
	else
	{
		return false;
	}
}
