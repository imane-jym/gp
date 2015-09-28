/*
 * =====================================================================================
 *
 *       Filename:  Master.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2013年01月04日 10时04分51秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  imane (), imanecr@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include "Master.h"
#include "Role.h"
#include "EnumDefines.h"
#include "CmdDefine.h"
#include "DBCtrl.h"
#include "WorldPacket.h"
#include "ErrorMsg.h"
#include "CardPacket.h"
#include "ConfigData.h"
#include "Master.h"
#include "Notice.h"
#include "Tujian.h"
#include "Mission.h"

/*
 *--------------------------------------------------------------------------------------
 *       Class:  CMaster
 *      Method:  CMaster
 * Description:  constructor
 *--------------------------------------------------------------------------------------
 */
CMaster::CMaster ()
{
//	m_dwMaster = 0;
//	m_dwLowStartTime = 0;
//	m_dwLowBlessTime = 0;
//	m_dwLowBlessNumber = 0;
//	m_dwLowNumberMax = 0;
//	m_dwLowTimeCD = 0;
	m_dwLowFirstTime = 0;

//	m_dwMiddleStartTime = 0;
//	m_dwMiddleBlessTime = 0;
//	m_dwMiddleBlessNumber = 0;
//	m_dwMiddleNumberMax = 0;
//	m_dwMiddleTimeCD = 0;
	m_dwMiddleFirstTime = 0;

//	m_dwHighStartTime = 0;
//	m_dwHighBlessTime = 0;
//	m_dwHighBlessNumber = 0;
//	m_dwHighNumberMax = 0;
//	m_dwHighTimeCD = 0;
	m_dwHighFirstTime = 0;
	
	m_IsInit = false;

	m_mapMaster.clear();
}  /* -----  end of method CMaster::CMaster  (constructor)  ----- */


int
CMaster::Init (CRole *pCRole)
{
	m_pCRole = pCRole;
	std::string temp;
	ByteBuffer by;
	if (!CDBCtrl::GetUserCardMasterData(temp, m_pCRole->GetdwAccountId()))
	{
		IME_ERROR("can not get master data userid %u", m_pCRole->GetdwAccountId());
		return -1;
	}
	by.append(temp.data(), temp.size());
	SeriliazeDB(by);
	if (LoadConf())
	{
		return -1;
	}
	if (Calculate())
	{
		return -1;
	}

	uint32_t now = time(NULL);
//	if (m_dwLowStartTime + m_dwLowTimeCD <= now)
//	{
//		m_dwLowMaster = 0;
//		m_dwLowStartTime = 0;
//		m_dwLowBlessTime = 0;
//		m_dwLowBlessNumber = 0;
//		m_dwLowNumberMax = 0;
//		m_dwLowTimeCD = 0;
//	}
//	if (m_dwMiddleStartTime + m_dwMiddleTimeCD <= now)
//	{
//		m_dwMiddleMaster = 0;
//		m_dwMiddleStartTime = 0;
//		m_dwMiddleBlessTime = 0;
//		m_dwMiddleBlessNumber = 0;
//		m_dwMiddleNumberMax = 0;
//		m_dwMiddleTimeCD = 0;
//	}
//	if (m_dwHighStartTime + m_dwHighTimeCD <= now)
//	{
//		m_dwHighMaster = 0;
//		m_dwHighStartTime = 0;
//		m_dwHighBlessTime = 0;
//		m_dwHighBlessNumber = 0;
//		m_dwHighNumberMax = 0;
//		m_dwHighTimeCD = 0;
//	}
	std::map<uint32_t, SMaster>::iterator it = m_mapMaster.begin();
	for (; it != m_mapMaster.end(); it++)
	{
		if (it->second.IsAppr && it->second.StartTime + it->second.TimeCD <= now)
		{
			it->second.StartTime = 0;
			it->second.BlessTime = 0;
			it->second.BlessNumber = 0;
			it->second.NumberMax = 0;
			it->second.TimeCD = 0;
			it->second.IsAppr = 0;
		}
	}

	m_IsInit = true;
	return 0;
}		/* -----  end of method CMaster::Init  ----- */

int CMaster::LoadConf(uint32_t index)
{
//	if (m_dwLowMaster != 0)
//	{
//		STC_CONF_MASTER *pConf = CConfMaster::Find(m_dwLowMaster);
//		if (pConf == NULL)
//		{
//			IME_ERROR("can not find index %u in master csv", m_dwLowMaster);
//			return -1;
//		}
//		m_dwLowNumberMax = pConf->dwNumberMax;
//		m_dwLowTimeCD = pConf->dwAliveTime;	 
//	}
//	if (m_dwMiddleMaster != 0)
//	{
//		STC_CONF_MASTER *pConf = CConfMaster::Find(m_dwMiddleMaster);
//		if (pConf == NULL)
//		{
//			IME_ERROR("can not find index %u in master csv", m_dwMiddleMaster);
//			return -1;
//		}
//		m_dwMiddleNumberMax = pConf->dwNumberMax;
//		m_dwMiddleTimeCD = pConf->dwAliveTime;	 
//	}
//	if (m_dwHighMaster != 0)
//	{
//		STC_CONF_MASTER *pConf = CConfMaster::Find(m_dwHighMaster);
//		if (pConf == NULL)
//		{
//			IME_ERROR("can not find index %u in master csv", m_dwHighMaster);
//			return -1;
//		}
//		m_dwHighNumberMax = pConf->dwNumberMax;
//		m_dwHighTimeCD = pConf->dwAliveTime;	 
//	}
	if (index == 0)
	{
		std::map<uint32_t, SMaster>::iterator it;
		for (it = m_mapMaster.begin(); it != m_mapMaster.end(); it++)
		{
			if (it->second.IsAppr)
			{
				STC_CONF_MASTER *pConf = CConfMaster::Find(it->second.Master);
				if (pConf == NULL)
				{
					IME_ERROR("can not find this data in master csv index %u", it->second.Master);
					return -1;
				}
				it->second.NumberMax = pConf->dwNumberMax;
				it->second.TimeCD = pConf->dwAliveTime;
			}
		}
	}
	else
	{
		std::map<uint32_t, SMaster>::iterator it;
		it = m_mapMaster.find(index);
		if (it == m_mapMaster.end())
			return -1;
		if (it->second.IsAppr)
		{
			STC_CONF_MASTER *pConf = CConfMaster::Find(it->second.Master);
			if (pConf == NULL)
			{
				IME_ERROR("can not find this data in master csv index %u", it->second.Master);
				return -1;
			}
			it->second.NumberMax = pConf->dwNumberMax;
			it->second.TimeCD = pConf->dwAliveTime;
		}
	}
	return 0;	
}

void
CMaster::SeriliazeDB (ByteBuffer &by)
{
	if(by.size() == 0)
		return;
//	by >> m_dwLowMaster;
//	by >> m_dwLowStartTime;
//	by >> m_dwLowBlessTime;
//	by >> m_dwLowBlessNumber;
	by >> m_dwLowFirstTime; 
//	by >> m_dwMiddleMaster;
//	by >> m_dwMiddleStartTime;
//	by >> m_dwMiddleBlessTime;
//	by >> m_dwMiddleBlessNumber;
	by >> m_dwMiddleFirstTime; 
//	by >> m_dwHighMaster;
//	by >> m_dwHighStartTime;
//	by >> m_dwHighBlessTime;
//	by >> m_dwHighBlessNumber;
	by >> m_dwHighFirstTime; 
	uint32_t number;
	by >> number;
	for(int i = 0; i < number; i++)
	{
		SMaster tmp = {0};
		by >> tmp.Master;
		IME_DEBUG("%u master index %u", i, tmp.Master);
		by >> tmp.StartTime;
		by >> tmp.BlessTime;
		by >> tmp.BlessNumber;
		by >> tmp.IsAppr;
		by >> tmp.Exp;
		m_mapMaster[tmp.Master] = tmp;
	}
	return ;
}		/* -----  end of method CMaster::SeriliazeDB  ----- */

void
CMaster::DeSeriliazeDB (ByteBuffer &by)
{
//	by << m_dwLowMaster;
//	by << m_dwLowStartTime;
//	by << m_dwLowBlessTime;
//	by << m_dwLowBlessNumber;
	by << m_dwLowFirstTime;
//	by << m_dwMiddleMaster;
//	by << m_dwMiddleStartTime;
//	by << m_dwMiddleBlessTime;
//	by << m_dwMiddleBlessNumber;
	by << m_dwMiddleFirstTime;
//	by << m_dwHighMaster;
//	by << m_dwHighStartTime;
//	by << m_dwHighBlessTime;
//	by << m_dwHighBlessNumber;
	by << m_dwHighFirstTime;
	by << (uint32_t)m_mapMaster.size();
	std::map<uint32_t, SMaster>::iterator it;
	for (it = m_mapMaster.begin(); it != m_mapMaster.end(); it++)
	{
		by << it->second.Master;
		by << it->second.StartTime;
		by << it->second.BlessTime;
		by << it->second.BlessNumber;	
		by << it->second.IsAppr;
		by << it->second.Exp;
	}
	return ;
}		/* -----  end of method CMaster::DeSeriliazeDB  ----- */

void
CMaster::SaveData ()
{
	if (!m_IsInit)
		return;
	ByteBuffer by;
	DeSeriliazeDB(by);
	std::string temp;
	temp.assign((const char *)by.contents(), by.size());
	CDBCtrl::SaveUserCardMasterData(temp, m_pCRole->GetdwAccountId());
	return ;
}		/* -----  end of method CMaster::SaveData  ----- */


void
CMaster::HandlerMasterInfo (uint32_t index)
{
	int64_t startime, blesstime;
	int64_t tmp;
	WorldPacket info(CMD_SC_MASTER_INFO);
	tmp = (int64_t)m_dwLowFirstTime + GlobalConfig::MasterCD1 - time(NULL);
	tmp = tmp < 0 ? 0 : tmp;
	info << (uint32_t) tmp;
	tmp = (int64_t)m_dwMiddleFirstTime + GlobalConfig::MasterCD2 - time(NULL);
	tmp = tmp < 0 ? 0 : tmp;
	info << (uint32_t) tmp;
	tmp = (int64_t)m_dwHighFirstTime + GlobalConfig::MasterCD3 - time(NULL);
	tmp = tmp < 0 ? 0 : tmp;
	info << (uint32_t) tmp;

	if (m_mapMaster.find(index) != m_mapMaster.end())
	{
		info << (uint32_t) 1;
		std::map<uint32_t, SMaster>::iterator it = m_mapMaster.find(index);
		info << it->second.Master;
		startime = (int64_t)it->second.StartTime + it->second.TimeCD - time(NULL);
		startime = startime < 0 ? 0 : startime;
		info << (uint32_t) startime;
		STC_CONF_MASTER *pConf = CConfMaster::Find(index);
		if (pConf == NULL)
		{
			IME_ERROR("can not find master csv in csv index %u", index);
			blesstime = 0;
		}
		else
		{
			blesstime = (int64_t)it->second.BlessTime + pConf->dwBlessCD - time(NULL);
			blesstime = blesstime < 0 ? 0 : blesstime;
		}
		info << (uint32_t) blesstime;
		info << it->second.BlessNumber;
		info << it->second.IsAppr;
		info << it->second.Exp;
		info << it->second.Level;
	}
	else
	{
		if (index == 0)
		{
			info << (uint32_t) m_mapMaster.size();
			std::map<uint32_t, SMaster>::iterator it;
			for (it = m_mapMaster.begin(); it != m_mapMaster.end(); it++)
			{
				info << it->second.Master;
				startime = (int64_t)it->second.StartTime + it->second.TimeCD - time(NULL);
				startime = startime < 0 ? 0 : startime;
				info << (uint32_t) startime;
				STC_CONF_MASTER *pConf = CConfMaster::Find(it->second.Master);
				if (pConf == NULL)
				{
					IME_ERROR("can not find master csv in csv index %u", index);
					blesstime = 0;
				}
				else
				{
					blesstime = (int64_t)it->second.BlessTime + pConf->dwBlessCD - time(NULL);
					blesstime = blesstime < 0 ? 0 : blesstime;
				}

				info << (uint32_t) blesstime;
//				IME_ERROR("master index %u, bless time %u, bless %u", it->second.Master, blesstime, it->second.BlessTime);
				info << it->second.BlessNumber;
				info << it->second.IsAppr;
				info << it->second.Exp;
				info << it->second.Level;
			}
		}
		else
		{
			IME_ERROR("can not find this index %u in master data", index);
			return;
		}
	}
	m_pCRole->SendPacket(&info);
	return ;
}		/* -----  end of method CMaster::HandlerMasterInfo  ----- */


void
CMaster::HandlerMasterRequest (WorldPacket &pkg)
{
	uint32_t choose;
   	pkg >> choose;	
	IME_CHAR_LOG("action [userid %u master request choose %u]", m_pCRole->GetdwAccountId(), choose);
	uint32_t order_id;
	if (!CDBCtrl::GetShopOrderId(m_pCRole->GetdwAccountId(), order_id))
	{
		m_pCRole->SendErrorMsg(ERRNO_MSG_SHOP_GET_ORDER_ID, CMD_SC_MASTER_REQUEST_RESULT);
		return;
	}

	WorldPacket info(CMD_SC_MASTER_REQUEST_RESULT);
	STC_CONF_MASTER *pConf = NULL;
	STC_CONF_MASTER_LEVEL *pConfUpPrize = NULL;
	std::map<uint32_t, SMaster>::iterator it;
	uint8_t isfree = 0;
	switch(choose)
	{
		case 1:
			if (m_dwLowFirstTime + GlobalConfig::MasterCD1 <= time(NULL) && m_pCRole->GetdwLowMasterCount() < GlobalConfig::MasterLimit1)
			{
				pConf = CConfMaster::RandGetFirst(m_pCRole->GetwLevel(), MAX_MASTER_LOW_S, MAX_MASTER_LOW_E, choose);
				if (pConf == NULL)
				{
					IME_ERROR("can not find master level %u star 0 3 choose %u in master csv", m_pCRole->GetwLevel(), choose);
					m_pCRole->SendErrorMsg(ERRNO_MSG_CSV_DATA, CMD_SC_MASTER_REQUEST_RESULT);
					return;
				}
				m_dwLowFirstTime = time(NULL);
				isfree = 1;
				IME_DEBUG("master count %u, limit %u", m_pCRole->GetdwLowMasterCount(), GlobalConfig::MasterLimit1);
			}
			else
			{
				if (m_pCRole->GetdwDollar() < GlobalConfig::MasterLowCon)
				{
					m_pCRole->SendErrorMsg(ERRNO_MSG_DOLLAR_NOT_ENGOUH, CMD_SC_MASTER_REQUEST_RESULT);
					return;
				}
				if (m_pCRole->ChangeDollar(-GlobalConfig::MasterLowCon, SOURCE_MASTER_REQUEST))
				{
					m_pCRole->SendErrorMsg(ERRNO_MSG_DOLLAR_CONSUME_FAIL, CMD_SC_MASTER_REQUEST_RESULT);
					return;
				}
				if (m_pCRole->GetclsCardPacket()->IsFull(1, 0))
				{
					m_pCRole->SendErrorMsg(ERRNO_MSG_CARD_PACKET_NOT_ENOUGH, CMD_SC_MASTER_REQUEST_RESULT);
					return;
				}
				CDBCtrl::LogShopRecord(m_pCRole->GetdwAccountId(), order_id, GlobalConfig::MasterLowCon, choose, 1, E_DC_MASTER);
				pConf = CConfMaster::RandGet(m_pCRole->GetwLevel(), MAX_MASTER_LOW_S, MAX_MASTER_LOW_E, choose);
				if (pConf == NULL)
				{
					IME_ERROR("can not find master level %u star 3 5 choose %u in master csv", m_pCRole->GetwLevel(), choose);
					m_pCRole->SendErrorMsg(ERRNO_MSG_CSV_DATA, CMD_SC_MASTER_REQUEST_RESULT);
					return;
				}

				uint16_t level = 1;
				it = m_mapMaster.find(pConf->dwIndex);
				if (it != m_mapMaster.end())
				{
					level = it->second.Level;
				}

				pConfUpPrize = CConfMasterLevel::GetRandPrize(pConf->byType, level);
				if (pConfUpPrize == NULL)
				{
					IME_ERROR("master rand fail master index %u", it->first);
					m_pCRole->SendErrorMsg(ERRNO_MSG_CSV_DATA, CMD_SC_MASTER_BLESS_RESULT);
					return;
				}

				CDBCtrl::LogShopProductGive(m_pCRole->GetdwAccountId(), order_id);
//				m_dwLowFirstTime = time(NULL);
			}
			break;			
		case 2:
			if (m_dwMiddleFirstTime + GlobalConfig::MasterCD2 <= time(NULL))
			{
				pConf = CConfMaster::RandGetFirst(m_pCRole->GetwLevel(), MAX_MASTER_MIDDLE_S, MAX_MASTER_MIDDLE_E, choose);
				if (pConf == NULL)
				{
					IME_ERROR("can not find master level %u star 2 4 choose %u in master csv", m_pCRole->GetwLevel(), choose);
					m_pCRole->SendErrorMsg(ERRNO_MSG_CSV_DATA, CMD_SC_MASTER_REQUEST_RESULT);
					return;
				}
				m_dwMiddleFirstTime = time(NULL);
				isfree = 1;
			}
			else
			{
				if (m_pCRole->GetdwDollar() < GlobalConfig::MasterMiddleCon)
				{
					m_pCRole->SendErrorMsg(ERRNO_MSG_DOLLAR_NOT_ENGOUH, CMD_SC_MASTER_REQUEST_RESULT);
					return;
				}
				if (m_pCRole->ChangeDollar(-GlobalConfig::MasterMiddleCon, SOURCE_MASTER_REQUEST))
				{
					m_pCRole->SendErrorMsg(ERRNO_MSG_DOLLAR_CONSUME_FAIL, CMD_SC_MASTER_REQUEST_RESULT);
					return;
				}
				if (m_pCRole->GetclsCardPacket()->IsFull(1, 0))
				{
					m_pCRole->SendErrorMsg(ERRNO_MSG_CARD_PACKET_NOT_ENOUGH, CMD_SC_MASTER_REQUEST_RESULT);
					return;
				}
				CDBCtrl::LogShopRecord(m_pCRole->GetdwAccountId(), order_id, GlobalConfig::MasterMiddleCon, choose, 1, E_DC_MASTER);
				pConf = CConfMaster::RandGet(m_pCRole->GetwLevel(), MAX_MASTER_MIDDLE_S, MAX_MASTER_MIDDLE_E, choose);
				if (pConf == NULL)
				{
					IME_ERROR("can not find master level %u star 3 5 choose %u in master csv", m_pCRole->GetwLevel(), choose);
					m_pCRole->SendErrorMsg(ERRNO_MSG_CSV_DATA, CMD_SC_MASTER_REQUEST_RESULT);
					return;
				}

				uint16_t level = 1;
				it = m_mapMaster.find(pConf->dwIndex);
				if (it != m_mapMaster.end())
				{
					level = it->second.Level;
				}

				pConfUpPrize = CConfMasterLevel::GetRandPrize(pConf->byType, level);
				if (pConfUpPrize == NULL)
				{
					IME_ERROR("master rand fail master index %u", it->first);
					m_pCRole->SendErrorMsg(ERRNO_MSG_CSV_DATA, CMD_SC_MASTER_BLESS_RESULT);
					return;
				}

				CDBCtrl::LogShopProductGive(m_pCRole->GetdwAccountId(), order_id);
//				m_dwMiddleFirstTime = time(NULL);
			}
			break;
		case 3:
			if (m_dwHighFirstTime + GlobalConfig::MasterCD3 <= time(NULL))
			{
				pConf = CConfMaster::RandGetFirst(m_pCRole->GetwLevel(), MAX_MASTER_HIGH_S, MAX_MASTER_HIGH_E, choose);
				if (pConf == NULL)
				{
					IME_ERROR("can not find master level %u star 3 5 choose %u in master csv", m_pCRole->GetwLevel(), choose);
					m_pCRole->SendErrorMsg(ERRNO_MSG_CSV_DATA, CMD_SC_MASTER_REQUEST_RESULT);
					return;
				}
				m_dwHighFirstTime = time(NULL);
				isfree = 1;
			}
			else
			{
				if (m_pCRole->GetdwDollar() < GlobalConfig::MasterHighCon)
				{
					m_pCRole->SendErrorMsg(ERRNO_MSG_DOLLAR_NOT_ENGOUH, CMD_SC_MASTER_REQUEST_RESULT);
					return;
				}
				if (m_pCRole->ChangeDollar(-GlobalConfig::MasterHighCon, SOURCE_MASTER_REQUEST))
				{
					m_pCRole->SendErrorMsg(ERRNO_MSG_DOLLAR_CONSUME_FAIL, CMD_SC_MASTER_REQUEST_RESULT);
					return;
				}
				if (m_pCRole->GetclsCardPacket()->IsFull(1, 0))
				{
					m_pCRole->SendErrorMsg(ERRNO_MSG_CARD_PACKET_NOT_ENOUGH, CMD_SC_MASTER_REQUEST_RESULT);
					return;
				}
				CDBCtrl::LogShopRecord(m_pCRole->GetdwAccountId(), order_id, GlobalConfig::MasterHighCon, choose, 1, E_DC_MASTER);
				pConf = CConfMaster::RandGet(m_pCRole->GetwLevel(), MAX_MASTER_HIGH_S, MAX_MASTER_HIGH_E, choose);
				if (pConf == NULL)
				{
					IME_ERROR("can not find master level %u star 3 5 choose %u in master csv", m_pCRole->GetwLevel(), choose);
					m_pCRole->SendErrorMsg(ERRNO_MSG_CSV_DATA, CMD_SC_MASTER_REQUEST_RESULT);
					return;
				}

				uint16_t level = 1;
				it = m_mapMaster.find(pConf->dwIndex);
				if (it != m_mapMaster.end())
				{
					level = it->second.Level;
				}

				pConfUpPrize = CConfMasterLevel::GetRandPrize(pConf->byType, level);
				if (pConfUpPrize == NULL)
				{
					IME_ERROR("master rand fail master index %u", it->first);
					m_pCRole->SendErrorMsg(ERRNO_MSG_CSV_DATA, CMD_SC_MASTER_BLESS_RESULT);
					return;
				}

				CDBCtrl::LogShopProductGive(m_pCRole->GetdwAccountId(), order_id);
//				m_dwHighFirstTime = time(NULL);
			}
			break;
		default:
			IME_ERROR("unvalid choose %u", choose);
			m_pCRole->SendErrorMsg(ERRNO_MSG_DATA_ILLEGAL, CMD_SC_MASTER_REQUEST_RESULT);
			return;
	}
	
	it = m_mapMaster.find(pConf->dwIndex);
	IME_DEBUG("request master index %u", pConf->dwIndex);
	if (it != m_mapMaster.end())
	{
		if (it->second.IsAppr) // already have and appr status
		{
			if (it->second.Exp + 1 <= CConfMasterLevel::GetMaxExp(pConf->byType))
			{
				it->second.Exp += 2;
				Calculate(it->first);
				HandlerMasterInfo(it->first);
			}

			if (choose == 1 && isfree)
			{
				if (m_pCRole->GetdwLowMasterCount() < GlobalConfig::MasterLimit1)
				{
					m_pCRole->SetdwLowMasterCount(m_pCRole->GetdwLowMasterCount() + 1);
					IME_ERROR("master count %u", m_pCRole->GetdwLowMasterCount());
					m_pCRole->HandlerInfoOpt();
				}
			}	
			m_pCRole->GetCMission()->EveryMaster(0, 1);
			m_pCRole->GetCMission()->LearnSkill(0, 1);
			
			CShopEffect::SGiveProduct tmp = {0};
			if (!isfree)
			{
				CShopEffect::GiveGameProduct(m_pCRole, pConf->byType, pConf->dwSkillIndex, pConfUpPrize->dwNumber, tmp, SOURCE_MASTER_REQUEST);
				if (pConf->dwStar >= 6 && pConf->byType == E_OBJ_SKILL)
				{
					sWorld->GetCNotice()->MasterGetSkill(m_pCRole->GetstrUserName(), pConf->dwSkillIndex * 100 + pConfUpPrize->dwNumber);
				}
			}

			m_pCRole->SendProUpdate();
			info << (uint16_t) 0;
			info << choose;
			info << it->first;
			info << (uint8_t) 1;
			info << tmp.effect;
			info << tmp.para1;
		   	info << tmp.para2;
			m_pCRole->SendPacket(&info);
			return;
		}
		// already have but it isn't in appr status
		if (it->second.Exp + 1 <= CConfMasterLevel::GetMaxExp(pConf->byType))
		{
			it->second.Exp++;
			Calculate(it->first);
			HandlerMasterInfo(it->first);
		}
	}
	else // have not this master
	{
		SMaster tmp = {0};
		m_mapMaster[pConf->dwIndex] = tmp;
		it = m_mapMaster.find(pConf->dwIndex);
		it->second.Master = pConf->dwIndex;
		it->second.Exp = 1;
	}

	if (choose == 1 && isfree)
	{
		if (m_pCRole->GetdwLowMasterCount() < GlobalConfig::MasterLimit1)
		{
			m_pCRole->SetdwLowMasterCount(m_pCRole->GetdwLowMasterCount() + 1);
			IME_DEBUG("master count %u", m_pCRole->GetdwLowMasterCount());
			m_pCRole->HandlerInfoOpt();
		}
	}	

	m_pCRole->GetCMission()->EveryMaster(0, 1);
	m_pCRole->GetCMission()->LearnSkill(0, 1);
	
	it->second.IsAppr = 1;
	it->second.StartTime = time(NULL);
	it->second.BlessTime = time(NULL);
	it->second.BlessNumber = 0;
	it->second.NumberMax = pConf->dwNumberMax;
	it->second.TimeCD = pConf->dwAliveTime;
	Calculate(it->first);

	HandlerMasterInfo(it->first);
//	m_pCRole->GetclsTujian()->AddTujian(pConf->dwIndex, TUJIAN_MASTER);
//	m_pCRole->GetclsTujian()->CardTujianUpdate(pConf->dwIndex, TUJIAN_MASTER); 
	m_pCRole->SendProUpdate();

	info.clear();
	info << (uint16_t) 0;
	info << choose;
	info << it->first;
	info << (uint8_t) 0;
	info << (uint32_t) 0;
	info << (uint32_t) 0;
	info << (uint32_t) 0;
	m_pCRole->SendPacket(&info);
	return ;
}		/* -----  end of method CMaster::HandlerMasterRequest  ----- */

void CMaster::RequestByShop(uint32_t choose, uint32_t &master, uint32_t &flag)
{
	STC_CONF_MASTER *pConf = NULL;
	std::map<uint32_t, SMaster>::iterator it;
	switch(choose)
	{
		case 1:
			pConf = CConfMaster::RandGet(m_pCRole->GetwLevel(), MAX_MASTER_LOW_S, MAX_MASTER_LOW_E, choose);
			if (pConf == NULL)
			{
				IME_ERROR("can not find master level %u star 0 3 choose %u in master csv", m_pCRole->GetwLevel(), choose);
				return;
			}

			break;			
		case 2:
			pConf = CConfMaster::RandGet(m_pCRole->GetwLevel(), MAX_MASTER_MIDDLE_S, MAX_MASTER_MIDDLE_E, choose);
			if (pConf == NULL)
			{
				IME_ERROR("can not find master level %u star 0 3 choose %u in master csv", m_pCRole->GetwLevel(), choose);
				return;
			}
			break;
		case 3:
			pConf = CConfMaster::RandGet(m_pCRole->GetwLevel(), MAX_MASTER_HIGH_S, MAX_MASTER_HIGH_E, choose);
			if (pConf == NULL)
			{
				IME_ERROR("can not find master level %u star 0 3 choose %u in master csv", m_pCRole->GetwLevel(), choose);
				return;
			}
			break;
		default:
			IME_ERROR("unvalid choose %u", choose);
			return;
	}
	
	it = m_mapMaster.find(pConf->dwIndex);
	IME_DEBUG("request master index %u", pConf->dwIndex);
	if (it != m_mapMaster.end())
	{
		if (it->second.IsAppr) // already have and appr status
		{
			if (it->second.Exp + 1 <= CConfMasterLevel::GetMaxExp(pConf->byType))
			{
				it->second.Exp++;
				Calculate(it->first);
				HandlerMasterInfo(it->first);
			}

			m_pCRole->SendProUpdate();
			master = it->first;
			flag = 1;
			return;
		}
		// already have but it isn't in appr status
	}
	else // have not this master
	{
		SMaster tmp = {0};
		m_mapMaster[pConf->dwIndex] = tmp;
		it = m_mapMaster.find(pConf->dwIndex);
		it->second.Master = pConf->dwIndex;
	}
	
	it->second.IsAppr = 1;
	it->second.StartTime = time(NULL);
	it->second.BlessTime = time(NULL);
	it->second.BlessNumber = 0;
	it->second.NumberMax = pConf->dwNumberMax;
	it->second.TimeCD = pConf->dwAliveTime;
	Calculate(it->first);

	HandlerMasterInfo(it->first);
//	m_pCRole->GetclsTujian()->AddTujian(pConf->dwIndex, TUJIAN_MASTER);
//	m_pCRole->GetclsTujian()->CardTujianUpdate(pConf->dwIndex, TUJIAN_MASTER); 
	m_pCRole->SendProUpdate();

	master = it->first;
	flag = 0;
	return ;
}

void
CMaster::HandlerMasterYesOrNo (WorldPacket &pkg)
{
	uint32_t flag;
//	uint8_t choose;
	uint32_t master;
	pkg >> flag >> master;
	if (flag == 0) //agree
	{
	}
	else //refuse
	{
		std::map<uint32_t, SMaster>::iterator it;
//		switch(choose)
//		{
//			case 1:
				it = m_mapMaster.find(master);
				if (it == m_mapMaster.end())
				{
					IME_ERROR("can not find previous master info index %u", master);
					m_pCRole->SendErrorMsg(ERRNO_MSG_PREVIOUS_MASTER_NONE, CMD_SC_MASTER_YES_OR_NO_RESULT);
					return;
				}
				STC_CONF_MASTER *pConf = CConfMaster::Find(master);
				if (pConf == NULL)
				{
					IME_ERROR("can not find this record in  master csv index %u", master);
					m_pCRole->SendErrorMsg(ERRNO_MSG_CSV_LOAD, CMD_SC_MASTER_YES_OR_NO_RESULT);
					return;
				}
				m_pCRole->ChangeCoin(GlobalConfig::MasterBack * pConf->dwStar * it->second.Level,SOURCE_OTHER);
				it->second.StartTime = 0;
				it->second.BlessTime = 0;
				it->second.BlessNumber = 0;
				it->second.NumberMax = 0;
				it->second.TimeCD = 0;
				it->second.IsAppr = 0;
				m_pCRole->SendProUpdate();
				HandlerMasterInfo(it->first);
//				break;
//			case 2:
//				it = m_mapMaster.find(master);
//				if (it == m_mapMaster.end())
//				{
//					IME_ERROR("can not find previous master info index %u", master);
//					m_pCRole->SendErrorMsg(ERRNO_MSG_PREVIOUS_MASTER_NONE, CMD_SC_MASTER_YES_OR_NO_RESULT);
//					return;
//				}
//				it->second.StartTime = 0;
//				it->second.BlessTime = 0;
//				it->second.BlessNumber = 0;
//				it->second.NumberMax = 0;
//				it->second.TimeCD = 0;
//				it->second.IsAppr = 0;
//				m_pCRole->ChangeCoin(GlobalConfig::MasterMiddleBack);
//				m_pCRole->SendProUpdate();
//				HandlerMasterInfo(it->first);
//				break;
//			case 3:
//				it = m_mapMaster.find(master);
//				if (it == m_mapMaster.end())
//				{
//					IME_ERROR("can not find previous master info index %u", master);
//					m_pCRole->SendErrorMsg(ERRNO_MSG_PREVIOUS_MASTER_NONE, CMD_SC_MASTER_YES_OR_NO_RESULT);
//					return;
//				}
//				it->second.StartTime = 0;
//				it->second.BlessTime = 0;
//				it->second.BlessNumber = 0;
//				it->second.NumberMax = 0;
//				it->second.TimeCD = 0;
//				it->second.IsAppr = 0;
//				m_pCRole->ChangeCoin(GlobalConfig::MasterHighBack);
//				m_pCRole->SendProUpdate();
//				HandlerMasterInfo(it->first);
//				break;
//			default:
//				IME_ERROR("unvalid choose %u", choose);
//				m_pCRole->SendErrorMsg(ERRNO_MSG_DATA_ILLEGAL, CMD_SC_MASTER_YES_OR_NO_RESULT);
//				return;
//		}	
	}

	WorldPacket info(CMD_SC_MASTER_YES_OR_NO_RESULT);
	info << (uint16_t) 0;
	info << (uint32_t) flag;
	info << master;
	m_pCRole->SendPacket(&info);
	return;
}		/* -----  end of method CMaster::HandlerMasterYesOrNo  ----- */


void
CMaster::HandlerMasterSendRed (WorldPacket &pkg)
{
	uint32_t masterindex;
	pkg >> masterindex;
	IME_CHAR_LOG("action [userid %u master send red masterindex %u]", m_pCRole->GetdwAccountId(), masterindex);

	uint32_t order_id;
	if (!CDBCtrl::GetShopOrderId(m_pCRole->GetdwAccountId(), order_id))
	{
		m_pCRole->SendErrorMsg(ERRNO_MSG_SHOP_GET_ORDER_ID, CMD_SC_MASTER_REQUEST_RESULT);
		return;
	}

	STC_CONF_MASTER *pConf = NULL;
	STC_CONF_MASTER_LEVEL *pConfUp = NULL;
	std::map<uint32_t, SMaster>::iterator it;
	pConf = CConfMaster::Find(masterindex);
	if (pConf == NULL)
	{
		IME_ERROR("can not find this index %u in csv", masterindex);
		m_pCRole->SendErrorMsg(ERRNO_MSG_CSV_LOAD, CMD_SC_MASTER_SEND_RED_RESULT);
		return;
	}

	if (m_pCRole->GetbyFirstSendRed())
	{
		if (m_pCRole->GetdwDollar() < pConf->dwSendRedCon)
		{
			m_pCRole->SendErrorMsg(ERRNO_MSG_DOLLAR_NOT_ENGOUH, CMD_SC_MASTER_SEND_RED_RESULT);
			return;
		}
		if (m_pCRole->ChangeDollar(-pConf->dwSendRedCon, SOURCE_SEND_RED))
		{
			m_pCRole->SendErrorMsg(ERRNO_MSG_DOLLAR_CONSUME_FAIL, CMD_SC_MASTER_SEND_RED_RESULT);
			return;
		}
	}

	it = m_mapMaster.find(masterindex);
	if (it == m_mapMaster.end())
	{
		IME_ERROR("can not find this master info index %u", masterindex);
		m_pCRole->SendErrorMsg(ERRNO_MSG_MASTER_NOT_FOUND, CMD_SC_MASTER_SEND_RED_RESULT);
		return;
	}
	if (!(it->second.IsAppr)) // already have and appr status
	{
		IME_ERROR("this master index %u", masterindex);
	  	m_pCRole->SendErrorMsg(ERRNO_MSG_MASTER_ALREADY_BLESS, CMD_SC_MASTER_BLESS_RESULT);
		return;	
	}
	pConfUp = CConfMasterLevel::Find(pConf->byType, it->second.Level);
	STC_CONF_MASTER_LEVEL *pConfUpPrize = CConfMasterLevel::GetRandPrize(pConf->byType, it->second.Level);
	if (pConfUpPrize == NULL)
	{
		IME_ERROR("master rand fail master index %u", it->first);
		m_pCRole->SendErrorMsg(ERRNO_MSG_CSV_DATA, CMD_SC_MASTER_SEND_RED_RESULT);
		return;
	}
	if (pConfUp == NULL)
	{
		IME_ERROR("load master level csv fail type %u, level %u", pConf->byType, it->second.Level);
		m_pCRole->SendErrorMsg(ERRNO_MSG_CSV_LOAD, CMD_SC_MASTER_SEND_RED_RESULT);
		return;
	}
	if (!(pConf->byType == E_OBJ_CARD_EXP || pConf->byType == E_OBJ_COIN || pConf->byType == E_OBJ_SKILL))
	{
		IME_ERROR("type unvalid %u", pConf->byType);
		m_pCRole->SendErrorMsg(ERRNO_MSG_DATA_ILLEGAL, CMD_SC_MASTER_SEND_RED_RESULT);
		return;
	}

	if (m_pCRole->GetclsCardPacket()->IsFull(1, pConf->dwSkillIndex))
	{
		IME_ERROR("card packet is full userid %u", m_pCRole->GetdwAccountId());
		m_pCRole->SendErrorMsg(ERRNO_MSG_CARD_EQUIP_PACKET_NOT_ENOUGH, CMD_SC_MASTER_SEND_RED_RESULT);
		return;
	}

	if (m_pCRole->GetbyFirstSendRed())
	{
		CDBCtrl::LogShopRecord(m_pCRole->GetdwAccountId(), order_id, pConf->dwSendRedCon, 0, 1, E_DC_MASTER_RED);
	}
	if (!m_pCRole->GetbyFirstSendRed())
	{
		m_pCRole->SetbyFirstSendRed(1);
	}

	it->second.StartTime = 0;
	it->second.BlessTime = 0;
	it->second.BlessNumber = 0;
	it->second.NumberMax = 0;
	it->second.TimeCD = 0;
	it->second.IsAppr = 0;

	CShopEffect::SGiveProduct tmp = {0};
	uint32_t pro = pConf->dwFixPro / 100.0 * pConfUp->dwPro;
	if (CUtil::RandEither(pro, 10000))
	{
		//todo
		uint32_t objid, opt;
		m_pCRole->GetclsCardPacket()->CardAddByPara(pConf->dwMasterCardIndex, objid, opt, SOURCE_SEND_RED);
		if (opt)
		{
			m_pCRole->GetclsCardPacket()->SingleCardAddUpdate(objid);
		}
		sWorld->GetCNotice()->MasterGetMaster(m_pCRole->GetstrUserName(), pConf->dwMasterCardIndex);
		tmp.effect = E_EFFECT_CARD;
		tmp.para1 = objid;
		tmp.para2 = 1;
	}
	else
	{
		CShopEffect::GiveGameProduct(m_pCRole, pConf->byType, pConf->dwSkillIndex, pConfUpPrize->dwNumber, tmp, SOURCE_SEND_RED);
		if (pConf->dwStar >= 6 && pConf->byType == E_OBJ_SKILL)
		{
			sWorld->GetCNotice()->MasterGetSkill(m_pCRole->GetstrUserName(), pConf->dwSkillIndex * 100 + pConfUpPrize->dwNumber);
		}
	}

	CDBCtrl::LogShopProductGive(m_pCRole->GetdwAccountId(), order_id);
	HandlerMasterInfo(it->first);
	m_pCRole->SendProUpdate();
	m_pCRole->HandlerInfoOpt();

	WorldPacket info(CMD_SC_MASTER_SEND_RED_RESULT);
	info << (uint16_t) 0;
	info << masterindex;
	info << tmp.effect;
	info << (uint32_t)tmp.para1;
	info << (uint32_t)tmp.para2;
	m_pCRole->SendPacket(&info);
	return;
}		/* -----  end of method CMaster::HandlerMasterSendRed  ----- */


void
CMaster::HandlerMasterBless (WorldPacket &pkg)
{
	uint32_t masterindex;
	pkg >> masterindex;
	IME_CHAR_LOG("action [userid %u master %u bless request]", m_pCRole->GetdwAccountId(), masterindex);
	if (m_pCRole->GetclsCardPacket()->IsFull(1, 0))
	{
		m_pCRole->SendErrorMsg(ERRNO_MSG_CARD_PACKET_NOT_ENOUGH, CMD_SC_MASTER_BLESS_RESULT);
		return;
	}

	STC_CONF_MASTER *pConf = NULL;
	STC_CONF_MASTER_LEVEL *pConfUp = NULL;
	std::map<uint32_t, SMaster>::iterator it;
	pConf = CConfMaster::Find(masterindex);
	if (pConf == NULL)
	{
		IME_ERROR("can not find this data in master csv index %u", masterindex);
		return;
	}	
	it = m_mapMaster.find(masterindex);
	if (it == m_mapMaster.end())
	{
		IME_ERROR("can not find this master info index %u", masterindex);
		m_pCRole->SendErrorMsg(ERRNO_MSG_MASTER_NOT_FOUND, CMD_SC_MASTER_BLESS_RESULT);
		return;
	}
	if (!(it->second.IsAppr)) // already have and appr status
	{
		IME_ERROR("this master index %u", masterindex);
	  	m_pCRole->SendErrorMsg(ERRNO_MSG_MASTER_ALREADY_BLESS, CMD_SC_MASTER_BLESS_RESULT);
		return;	
	}
	if (it->second.BlessTime + pConf->dwBlessCD > time(NULL))
	{
		m_pCRole->SendErrorMsg(ERRNO_MSG_MASTER_BLESS_IN_CD, CMD_SC_MASTER_BLESS_RESULT);
		return;
	}
	STC_CONF_MASTER_LEVEL *pConfUpPrize = CConfMasterLevel::GetRandPrize(pConf->byType, it->second.Level);
	if (pConfUpPrize == NULL)
	{
		IME_ERROR("master rand fail master index %u", it->first);
		m_pCRole->SendErrorMsg(ERRNO_MSG_CSV_DATA, CMD_SC_MASTER_BLESS_RESULT);
		return;
	}

	pConfUp = CConfMasterLevel::Find(pConf->byType, it->second.Level);
	if (pConfUp == NULL)
	{
		IME_ERROR("load master level csv fail type %u, level %u", pConf->byType, it->second.Level);
		m_pCRole->SendErrorMsg(ERRNO_MSG_CSV_LOAD, CMD_SC_MASTER_BLESS_RESULT);
		return;
	}
	if (!(pConf->byType == E_OBJ_CARD_EXP || pConf->byType == E_OBJ_COIN || pConf->byType == E_OBJ_SKILL))
	{
		IME_ERROR("type unvalid %u", pConf->byType);
		m_pCRole->SendErrorMsg(ERRNO_MSG_DATA_ILLEGAL, CMD_SC_MASTER_BLESS_RESULT);
		return;
	}

	CShopEffect::SGiveProduct tmp = {0};
	if (it->second.BlessNumber + 1 >= it->second.NumberMax)
	{
		uint32_t pro = pConf->dwFixPro + pConfUp->dwPro;
		if (CUtil::RandEither(pro, 10000))
		{
			//todo
			uint32_t objid, opt;
			m_pCRole->GetclsCardPacket()->CardAddByPara(pConf->dwMasterCardIndex, objid, opt, SOURCE_MASTER_BLESS);
			if (opt)
			{
				m_pCRole->GetclsCardPacket()->SingleCardAddUpdate(objid);
			}
			sWorld->GetCNotice()->MasterGetMaster(m_pCRole->GetstrUserName(), pConf->dwMasterCardIndex);
			tmp.effect = E_EFFECT_CARD;
			tmp.para1 = objid;
			tmp.para2 = 1;
		}
		else
		{
			CShopEffect::GiveGameProduct(m_pCRole, pConf->byType, pConf->dwSkillIndex, pConfUpPrize->dwNumber, tmp, SOURCE_MASTER_BLESS);
			if (pConf->dwStar >= 6 && pConf->byType == E_OBJ_SKILL)
			{
				sWorld->GetCNotice()->MasterGetSkill(m_pCRole->GetstrUserName(), pConf->dwSkillIndex * 100 + pConfUpPrize->dwNumber);
			}
		}

		it->second.StartTime = 0;
		it->second.BlessTime = 0;
		it->second.BlessNumber = 0;
		it->second.NumberMax = 0;
		it->second.TimeCD = 0;
		it->second.IsAppr = 0;
	}
	else
	{
		it->second.BlessNumber++ ;
		it->second.BlessTime = time(NULL);
	}

	HandlerMasterInfo(it->first);
	WorldPacket info(CMD_SC_MASTER_BLESS_RESULT);
	info << (uint16_t) 0;
	info << masterindex;
	info << tmp.effect;
	info << (uint32_t)tmp.para1;
	info << (uint32_t)tmp.para2;
	m_pCRole->SendPacket(&info);
	return;
}		/* -----  end of method CMaster::HandlerMasterBless  ----- */

void
CMaster::OnTimer5s ()
{
	uint32_t now = time(NULL);
//	if (m_dwLowStartTime + m_dwLowTimeCD <= now)
//	{
//		m_dwLowMaster = 0;
//		m_dwLowStartTime = 0;
//		m_dwLowBlessTime = 0;
//		m_dwLowBlessNumber = 0;
//		m_dwLowNumberMax = 0;
//		m_dwLowTimeCD = 0;
//	}
//	if (m_dwMiddleStartTime + m_dwMiddleTimeCD <= now)
//	{
//		m_dwMiddleMaster = 0;
//		m_dwMiddleStartTime = 0;
//		m_dwMiddleBlessTime = 0;
//		m_dwMiddleBlessNumber = 0;
//		m_dwMiddleNumberMax = 0;
//		m_dwMiddleTimeCD = 0;
//	}
//	if (m_dwHighStartTime + m_dwHighTimeCD <= now)
//	{
//		m_dwHighMaster = 0;
//		m_dwHighStartTime = 0;
//		m_dwHighBlessTime = 0;
//		m_dwHighBlessNumber = 0;
//		m_dwHighNumberMax = 0;
//		m_dwHighTimeCD = 0;
//	}
	std::map<uint32_t, SMaster>::iterator it = m_mapMaster.begin();
	for (; it != m_mapMaster.end(); it++)
	{
		if (it->second.IsAppr && it->second.StartTime + it->second.TimeCD <= now)
		{
			it->second.StartTime = 0;
			it->second.BlessTime = 0;
			it->second.BlessNumber = 0;
			it->second.NumberMax = 0;
			it->second.TimeCD = 0;
			it->second.IsAppr = 0;
			HandlerMasterInfo(it->second.Master);
		}
	}
	return ;
}		/* -----  end of method CMaster::OnTimer5s  ----- */

int CMaster::Calculate(uint32_t index)
{
	if (index == 0)
	{
		std::map<uint32_t, SMaster>::iterator it;
		for (it = m_mapMaster.begin(); it != m_mapMaster.end(); it++)
		{
			if (it == m_mapMaster.end())
			{
				IME_ERROR("can not find this master index %u", index);
				return -1;
			}
//			if (it->second.IsAppr)
//			{
				STC_CONF_MASTER *pConf = CConfMaster::Find(it->second.Master);
				if (pConf == NULL)
				{
					IME_ERROR("can not find this data in master csv index %u", it->second.Master);
					return -1;
				}
				it->second.Level = CConfMasterLevel::FindLevel(pConf->byType, it->second.Exp);
				IME_DEBUG("index %u level %u", it->second.Master, it->second.Level);
//			}
			
		}
	}
	else
	{
		std::map<uint32_t, SMaster>::iterator it;
		it = m_mapMaster.find(index);
		if (it == m_mapMaster.end())
		{
			IME_ERROR("can not find this master index %u", index);
			return -1;
		}
//		if (it->second.IsAppr)
//		{
			STC_CONF_MASTER *pConf = CConfMaster::Find(it->second.Master);
			if (pConf == NULL)
			{
				IME_ERROR("can not find this data in master csv index %u", it->second.Master);
				return -1;
			}
			it->second.Level = CConfMasterLevel::FindLevel(pConf->byType, it->second.Exp);
//		}
	}
	return 0;
}

int CMaster::AddMasterExp(uint32_t masterindex, uint32_t exp)
{
	STC_CONF_MASTER *pConf = NULL;
	pConf = CConfMaster::Find(masterindex);
	if (pConf == NULL)
	{
		IME_ERROR("master can not find in csv %u", masterindex);
		return -1;
	}
	std::map<uint32_t, SMaster>::iterator it;
	it = m_mapMaster.find(pConf->dwIndex);
	if (it != m_mapMaster.end())
	{
		if (it->second.IsAppr) // already have and appr status
		{
			if (it->second.Exp + exp <= CConfMasterLevel::GetMaxExp(pConf->byType))
			{
				it->second.Exp += exp;
			}
			else if (it->second.Exp < CConfMasterLevel::GetMaxExp(pConf->byType) && it->second.Exp + exp > CConfMasterLevel::GetMaxExp(pConf->byType) )
			{
				it->second.Exp = CConfMasterLevel::GetMaxExp(pConf->byType);
			}

			Calculate(it->first);
			HandlerMasterInfo(it->first);
			return 0;
		}
		// already have but it isn't in appr status
	}
	else // have not this master
	{
		SMaster tmp = {0};
		m_mapMaster[pConf->dwIndex] = tmp;
		it = m_mapMaster.find(pConf->dwIndex);
		it->second.Master = pConf->dwIndex;
	}
	
	it->second.IsAppr = 1;
	it->second.StartTime = time(NULL);
	it->second.BlessTime = time(NULL);
	it->second.BlessNumber = 0;
	it->second.NumberMax = pConf->dwNumberMax;
	it->second.TimeCD = pConf->dwAliveTime;
	it->second.Exp = std::min(exp, CConfMasterLevel::GetMaxExp(pConf->byType));
	Calculate(it->first);

	HandlerMasterInfo(it->first);
	m_pCRole->SendProUpdate();
	return 0;
}

uint32_t CMaster::GetMasterExp(uint32_t masterindex)
{
	if (m_mapMaster.find(masterindex) != m_mapMaster.end())
	{
		return m_mapMaster[masterindex].Exp;
	}
	return 0;
}
