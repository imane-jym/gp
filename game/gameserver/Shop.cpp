/*
 * =====================================================================================
 *
 *       Filename:  shop.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2012年12月09日 16时35分27秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  imane (), imanecr@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include "Shop.h"
#include "Role.h"
#include "CmdDefine.h"
#include "EnumDefines.h"
#include "ErrorMsg.h"
#include "ConfigData.h"
#include "CardPacket.h"
#include "RandomEvent.h"
#include "Arena.h"
#include "GlobalConfig.h"
#include "Task.h"
#include "Notice.h"
#include "Master.h"
#include "Other.h"
#include "Tujian.h"
#include "Mission.h"
#include "CommonType.h"
#include "Tower.h"

CShop::CShop()
{
	m_IsInit = false;
}

CShop::~CShop()
{

}

int CShop::Init(CRole *pCRole)
{
	m_pCRole = pCRole;
	if (!LoadData())
	{
		m_IsInit = false;
		IME_ERROR("load data fail");
		return -1;
	}
	m_IsInit = true;
	m_kind = 0;
	return 0;
}
bool CShop::LoadData()
{
	std::string temp;
	if (!CDBCtrl::GetUserShopPacketData(temp, m_pCRole->GetdwAccountId()))
	{
		IME_ERROR("get shop data fail, user_id %u", m_pCRole->GetdwAccountId());
		return false;
	}
	ByteBuffer content;
	content.append(temp.data(), temp.size());
	if (DeSerilizeDB(content))
	{
		IME_ERROR("serilizeDB fail");
		return false;
	}
	return true;
}

int CShop::SaveData()
{
	if (!m_IsInit)
		return -1;
	ByteBuffer byte;
	if (SerilizeDB(byte))
	{
		IME_ERROR("SerilizeDB fail");
		return -1;
	}
	std::string temp;
	temp.assign((const char *)byte.contents(), byte.size());
	CDBCtrl::SaveUserShopPacketData(temp, m_pCRole->GetdwAccountId());

	return 0;
}

int CShop::SerilizeDB ( ByteBuffer &byte)
{
	if (m_mapShop.size() == 0)
		return 0;
	byte << (uint32_t)m_mapShop.size();
	std::map<uint32_t, uint32_t>::iterator it;
	for (it = m_mapShop.begin(); it != m_mapShop.end(); it++)
	{
		byte << it->first;
		byte << it->second;
	}

	return 0;
}		

int CShop::DeSerilizeDB ( ByteBuffer &byte)
{
	if (byte.size() == 0)
		return 0;
	uint32_t production_number;
	uint32_t index;
	uint32_t number;
	byte >> production_number;
	for (int i = 0; i < production_number; i++)
	{
		byte >> index;
		byte >> number;
		m_mapShop[index] = number;
		IME_DEBUG("shop production index %u, number %u", index, number);
	}

	return 0;
}		

void CShop::HandlerShopInfo (uint32_t index)
{
//	CConfGoods::MapData *pConf = CConfGoods::GetVal();
//
//	WorldPacket info(CMD_SC_SHOP_PACKET_INFO);
//	uint32_t number = pConf->size();
//	info << number;
//
//	uint32_t shopnumber;	
//	CConfGoods::MapData::iterator it;
//	std::map<uint32_t, uint32_t>::iterator itShop;
//	for(it = pConf->begin(); it != pConf->end(); it++)
//	{
//		info << (it->second).dwIndex;
////		info << (it->second).byType;
//		itShop = m_mapShop.find((it->second).dwIndex);
//		if (itShop == m_mapShop.end())
//		{
//			shopnumber = (uint32_t) 0;
//		}
//		else
//		{
//			shopnumber = itShop->second;
//		}
//		info << shopnumber;
////		info << (it->second).dwConsumeCoin;
////		info << (it->second).dwConsumeDollar;
//		IME_DEBUG("shop id %u, number %u", it->second.dwIndex, shopnumber);
//	}

	if (index == 0)
	{
		WorldPacket info(CMD_SC_SHOP_PACKET_INFO);
		uint32_t number = 0;

		uint32_t shopnumber;	
		std::map<uint32_t, uint32_t>::iterator it;
		IME_DEBUG("total number %u", m_mapShop.size());
		for(it = m_mapShop.begin(); it != m_mapShop.end(); it++)
		{
			info << it->first;
			info << it->second;
			number++;
			if (number >= 300)
			{
				WorldPacket smallpkg(CMD_SC_SHOP_PACKET_INFO);
				smallpkg << (uint32_t) number;
				smallpkg.append(info.contents(), info.size());
				m_pCRole->SendPacket(&smallpkg);
				info.clear();
				number = 0;
			}
			IME_DEBUG("shop id %u, number %u", it->first, it->second);
		}

		if (number < 300)
		{
			WorldPacket smallpkg(CMD_SC_SHOP_PACKET_INFO);
			smallpkg << (uint32_t) number;
			smallpkg.append(info.contents(), info.size());
			m_pCRole->SendPacket(&smallpkg);
		}
		return;
	}
	else
	{
		if (m_mapShop.find(index) == m_mapShop.end())
		{
			WorldPacket info(CMD_SC_SHOP_PACKET_INFO);
			info << (uint32_t) 1;
			info << (uint32_t) index;
			info << 0;
			m_pCRole->SendPacket(&info);
		}
		else
		{
			WorldPacket info(CMD_SC_SHOP_PACKET_INFO);
			info << (uint32_t) 1;
			info << (uint32_t) index;
			info << m_mapShop[index];
			m_pCRole->SendPacket(&info);
		}
		return;
	}
}

void 
CShop::AddItem(uint32_t shopid, int source, uint32_t number)
{
	if (shopid == 0)
		return;
	if (m_mapShop.find(shopid) != m_mapShop.end())
	{
		m_mapShop[shopid] += number;
		IME_CHAR_LOG("resource [status=success,res=item,opt=add,character=%u,index=%u,number=%u,reason=ok,source=%d]", m_pCRole->GetdwAccountId(), shopid, number, source);
	}
	else
	{
		m_mapShop[shopid] = number;
		IME_CHAR_LOG("resource [status=success,res=item,opt=add,character=%u,index=%u,number=%u,reason=ok,source=%d]", m_pCRole->GetdwAccountId(), shopid, number, source);
	}
}

int 
CShop::DelItem(uint32_t shopid, int source, uint32_t number)
{
	if (m_mapShop.find(shopid) != m_mapShop.end())
	{
		if (m_mapShop[shopid] >= number)
		{
			m_mapShop[shopid] -= number;
			if (m_mapShop[shopid] == 0)
				m_mapShop.erase(shopid);
			IME_CHAR_LOG("resource [status=success,res=item,opt=del,character=%u,index=%u,number=%u,reason=ok,source=%d]", m_pCRole->GetdwAccountId(), shopid, number, source);
			return 0;
		}
		else
		{
			IME_ERROR("userid %u have not this item %u", m_pCRole->GetdwAccountId(), shopid);
			return -1;
		}
	}
	else
	{
		IME_ERROR("userid %u have not this item %u", m_pCRole->GetdwAccountId(), shopid);
		return -1;
	}
	return 0;
}

bool CShop::IsHave(uint32_t shopid, uint32_t number)
{
	if (m_mapShop.find(shopid) != m_mapShop.end())
	{
		if (m_mapShop[shopid] >= number)
		{
			return true;
		}
	}
	return false;
}

void
CShop::HandlerBuy (WorldPacket &pkg)
{
	uint32_t storeid;
	uint32_t number;
	pkg >> storeid;
	pkg >> number;
	IME_CHAR_LOG("action [userid %u buy shop %u number %u in store]", m_pCRole->GetdwAccountId(), storeid, number);
	uint32_t order_id;
	if (number > MAX_SHOP_BUY_NUMBER || number <= 0)
	{
		IME_ERROR("max bigger than 10");
		m_pCRole->SendErrorMsg(ERRNO_MSG_SHOP_MAX_NUMBER, CMD_SC_SHOP_BUY_RESULT);
		return;
	}

	STC_CONF_STORE *pConf = CConfStore::Find(storeid);
	if (pConf == NULL)
	{
		IME_ERROR("can not find this index %u in shop csv", storeid);
		m_pCRole->SendErrorMsg(ERRNO_MSG_CSV_LOAD, CMD_SC_SHOP_BUY_RESULT);
		return;
	}

	if (pConf->byObjType == STORE_OBJ_RMB)
	{
		IME_ERROR("this protocol can not accept this type buy request");
		m_pCRole->SendErrorMsg(ERRNO_MSG_DATA_ILLEGAL, CMD_SC_SHOP_BUY_RESULT);
		return;
	}

	if (pConf->byObjType == STORE_OBJ_DOLLAR)
	{
		if (!CDBCtrl::GetShopOrderId(m_pCRole->GetdwAccountId(), order_id))
		{
			IME_ERROR("get order id fail userid %u", m_pCRole->GetdwAccountId());
			m_pCRole->SendErrorMsg(ERRNO_MSG_SHOP_GET_ORDER_ID, CMD_SC_SHOP_BUY_RESULT);
			return;
		}
	}

	if (pConf->byIsBuy == 0)
	{
		IME_ERROR("this shop can not sell %u", storeid);
		m_pCRole->SendErrorMsg(ERRNO_MSG_SHOP_NOT_SELL, CMD_SC_SHOP_USE_RESULT);
		return;
	}

	
	if (pConf->byObjType == STORE_OBJ_COIN)
	{
		if (m_pCRole->GetdwCoin() < pConf->dwObjNumber * number)
		{
			IME_ERROR("user dollar is not enough now %u need %u", m_pCRole->GetdwDollar(), pConf->dwObjNumber * number);
			m_pCRole->SendErrorMsg(ERRNO_MSG_NOT_ENOUGH_COIN, CMD_SC_SHOP_BUY_RESULT);
			return;
		}
	}
	else if (pConf->byObjType == STORE_OBJ_DOLLAR)
	{
		if (m_pCRole->GetdwDollar() < pConf->dwObjNumber * number)
		{
			IME_ERROR("user coin is not enough now %u need %u", m_pCRole->GetdwCoin(), pConf->dwObjNumber * number);
			m_pCRole->SendErrorMsg(ERRNO_MSG_DOLLAR_NOT_ENGOUH, CMD_SC_SHOP_BUY_RESULT);
			return;
		}
	}
	else if (pConf->byObjType == STORE_OBJ_PRESTIGE)
	{
		if (!m_pCRole->IsOpenArena())
		{
			IME_ERROR("user prestige is closed userid %u", m_pCRole->GetdwAccountId());
			m_pCRole->SendErrorMsg(ERRNO_MSG_ARENA_NOT_ENOUGH_PRESTIGE, CMD_SC_SHOP_BUY_RESULT);
			return;
		}
		
//		IME_ERROR("user prestige now %u need %u", m_pCRole->GetclsArena()->GetdwPrestige(), pConf->dwObjNumber * number);
		if (m_pCRole->IsOpenArena() && (m_pCRole->GetclsArena()->GetdwPrestige() < pConf->dwObjNumber * number))
		{
			IME_ERROR("user prestige is not enough now %u need %u", m_pCRole->GetclsArena()->GetdwPrestige(), pConf->dwObjNumber * number);
			m_pCRole->SendErrorMsg(ERRNO_MSG_ARENA_NOT_ENOUGH_PRESTIGE, CMD_SC_SHOP_BUY_RESULT);
			return;
		}
	}
	else
	{
		IME_ERROR("store currency type is wrong type %u", pConf->byObjType);
		m_pCRole->SendErrorMsg(ERRNO_MSG_CSV_DATA, CMD_SC_SHOP_BUY_RESULT);
		return;
	}

	int VIPgift[VIP_MAX] = VIP_GIFT_ARR;
	std::vector<int> vecAllVip(VIPgift, VIPgift + VIP_MAX);
	if (std::find(vecAllVip.begin(), vecAllVip.end(), pConf->dwIndex) != vecAllVip.end())
	{
		STC_CONF_VIP *pVip = CConfVip::Find(m_pCRole->GetwVipLv());
		uint32_t VIPgiftlv = 0;
		if (pVip != NULL)
		{
			VIPgiftlv = pVip->dwVipGift;
		}
		if (VIPgiftlv > VIP_MAX)
			VIPgiftlv = VIP_MAX;

		std::vector<int> vecVip;
		for (int i = 0; i < VIPgiftlv; i++)
		{
			vecVip.push_back(VIPgift[i]);
		}

		if (std::find(vecVip.begin(), vecVip.end(), pConf->dwIndex) == vecVip.end())
		{
			m_pCRole->SendErrorMsg(ERRNO_MSG_VIP_GIFT_NOT_ALLOW, CMD_SC_SHOP_BUY_RESULT);
			return;	
		}
		if (number > 1)
		{
			m_pCRole->SendErrorMsg(ERRNO_MSG_VIP_GIFT_OVERLOAD_MAX_COUNT, CMD_SC_SHOP_BUY_RESULT);
			return;
		}

//		if (!m_pCRole->IsAccessGetVipGift(pConf->dwIndex))
//		{
//			m_pCRole->SendErrorMsg(ERRNO_MSG_VIP_GIFT_ALREADY, CMD_SC_SHOP_BUY_RESULT);
//			return;
//		}
//		m_pCRole->AddVipGift(pConf->dwIndex);
	}
	
	if (pConf->byGoodsType == E_OBJ_CARD)
	{
		if (m_pCRole->GetclsCardPacket()->IsFull(number, pConf->dwIndex))
		{
			m_pCRole->SendErrorMsg(ERRNO_MSG_CARD_PACKET_NOT_ENOUGH, CMD_SC_SHOP_BUY_RESULT);
			return;
		}
	}

	if (pConf->dwRoleLimitNumber != 0)
	{
		IME_ERROR("gift role %u, limit %u", m_pCRole->GetGiftRole(pConf->dwStoreIndex) + number, pConf->dwRoleLimitNumber);
		if (m_pCRole->GetGiftRole(pConf->dwStoreIndex) + number > pConf->dwRoleLimitNumber)
		{
			m_pCRole->SendErrorMsg(ERRNO_MSG_ALREADY_GOODS_ROLE, CMD_SC_SHOP_BUY_RESULT);
			return;
		}
	}
	if (pConf->dwLimitNumber != 0)
	{
		IME_ERROR("gift day %u, limit %u", m_pCRole->GetGiftDay(pConf->dwStoreIndex) + number, pConf->dwLimitNumber);
		if (m_pCRole->GetGiftDay(pConf->dwStoreIndex) + number > pConf->dwLimitNumber)
		{
			m_pCRole->SendErrorMsg(ERRNO_MSG_ALREADY_GOODS_DAY, CMD_SC_SHOP_BUY_RESULT);
			return;
		}
	}

	if (pConf->byObjType == STORE_OBJ_DOLLAR)
	{	
//		for (int i = 0; i < number; i++)
//		{
			if (m_pCRole->ChangeDollar(-(pConf->dwObjNumber * number), SOURCE_STORE_BUY))
			{
				IME_ERROR("del dollar fail");
				m_pCRole->SendErrorMsg(ERRNO_MSG_SHOP_DEL_DOLLAR, CMD_SC_SHOP_BUY_RESULT);
				return;
			}		

			CDBCtrl::LogShopRecord(m_pCRole->GetdwAccountId(), order_id, pConf->dwObjNumber * number, storeid, number, E_DC_SHOP);
			
			CShopEffect::SGiveProduct tmp;
			for (int m = 0; m < number; m++)
			{
				CShopEffect::GiveGameProductSpecial(m_pCRole, pConf->byGoodsType, pConf->dwIndex, 1, tmp, SOURCE_STORE_BUY);
			}
//			AddItem(pConf->dwIndex, SOURCE_STORE_BUY, number);
//			if (m_mapShop.find(pConf->dwIndex) != m_mapShop.end())
//			{
//				m_mapShop[pConf->dwIndex] += number;
//			}
//			else
//			{
//				m_mapShop[pConf->dwIndex] = number;
//			}

			CDBCtrl::LogShopProductGive(m_pCRole->GetdwAccountId(), order_id);
			order_id++;
//		}
//		HandlerShopInfo(pConf->dwIndex);
		m_pCRole->SendProUpdate();
	}
	else if (pConf->byObjType == STORE_OBJ_COIN || pConf->byObjType == STORE_OBJ_PRESTIGE)
	{
//		for (int i = 0; i < number; i++)
//		{
			if (pConf->byObjType == STORE_OBJ_COIN)
			{
				m_pCRole->ChangeCoin(-pConf->dwObjNumber * number, SOURCE_STORE_BUY);
				m_pCRole->SendProUpdate();	
			}
			else
			{
				uint32_t prestige = m_pCRole->GetclsArena()->GetdwPrestige();
				m_pCRole->GetclsArena()->SetdwPrestige(prestige - pConf->dwObjNumber * number);
				m_pCRole->GetclsArena()->HandlerProUpdate();
			}

			CShopEffect::SGiveProduct tmp;
			for (int m = 0; m < number; m++)
			{
				CShopEffect::GiveGameProductSpecial(m_pCRole, pConf->byGoodsType, pConf->dwIndex, 1, tmp, SOURCE_STORE_BUY);
			}
//			AddItem(pConf->dwIndex, SOURCE_STORE_BUY, number);
//			if (m_mapShop.find(pConf->dwIndex) != m_mapShop.end())
//			{
//				m_mapShop[pConf->dwIndex] += number;
//			}
//			else
//			{
//				m_mapShop[pConf->dwIndex] = number;
//			}
			order_id++;
//		}
//		HandlerShopInfo(pConf->dwIndex);
		m_pCRole->SendProUpdate();
	}
	else
	{
		IME_ERROR("get data store currency type fail %u", pConf->byObjType);
		m_pCRole->SendErrorMsg(ERRNO_MSG_CSV_DATA, CMD_SC_SHOP_BUY_RESULT);
		return;
	}

	if (pConf->dwRoleLimitNumber != 0)
	{
		m_pCRole->SetGiftRoleLimit(pConf->dwStoreIndex, m_pCRole->GetGiftRole(pConf->dwStoreIndex) + number);
	}

	if (pConf->dwLimitNumber != 0)
	{
		m_pCRole->SetGiftDayLimit(pConf->dwStoreIndex, m_pCRole->GetGiftDay(pConf->dwStoreIndex) + number);
	}

	WorldPacket storedata;
	storedata << (uint8_t) pConf->byStoreChannel;
	storedata << m_kind;
	HandlerStoreData(storedata);

	WorldPacket info(CMD_SC_SHOP_BUY_RESULT);
	info << (uint16_t) 0;
	info << (uint32_t) storeid;
	info << (uint32_t) number;
	m_pCRole->SendPacket(&info);
	m_pCRole->GetCMission()->EveryShop(0,1);
	return ;
}		/* -----  end of method CShop::HandlerBuy  ----- */

void
CShop::HandlerShopUse(WorldPacket &pkg)
{
	uint32_t shopid;
	pkg >> shopid;
	IME_CHAR_LOG("action [userid %u use item %u]", m_pCRole->GetdwAccountId(), shopid);

	uint32_t order_id;
	if (!CDBCtrl::GetShopOrderId(m_pCRole->GetdwAccountId(), order_id))
	{
		IME_ERROR("get order id fail userid %u", m_pCRole->GetdwAccountId());
		m_pCRole->SendErrorMsg(ERRNO_MSG_SHOP_GET_ORDER_ID, CMD_SC_SHOP_USE_RESULT);
		return;
	}

	STC_CONF_GOODS *pConf = CConfGoods::Find(shopid);
	if (pConf == NULL)
	{
		IME_ERROR("can not find this index %u in goods csv", shopid);
		m_pCRole->SendErrorMsg(ERRNO_MSG_CSV_LOAD, CMD_SC_SHOP_USE_RESULT);
		return;
	}

	IME_DEBUG("shop item use func %u, shopid %u", pConf->dwFuncType, shopid);
	if (pConf->dwFuncType == EGG_EFFECT)
	{
		if (m_pCRole->GetclsCardPacket()->IsFull(1, 0))
		{
			IME_ERROR("userid %u card packet is not enough", m_pCRole->GetdwAccountId());
			m_pCRole->SendErrorMsg(ERRNO_MSG_CARD_PACKET_NOT_ENOUGH, CMD_SC_SHOP_USE_RESULT);
			return;
		}
	}
	if (pConf->dwFuncType == GIFT_EFFECT)
	{
		if (CShopEffect::IsFullCardPacketGift(m_pCRole, pConf->dwPara1))
		{
			IME_ERROR("userid %u card packet is not enough", m_pCRole->GetdwAccountId());
			m_pCRole->SendErrorMsg(ERRNO_MSG_CARD_PACKET_NOT_ENOUGH, CMD_SC_SHOP_USE_RESULT);
			return;
		}	
	}
	if (pConf->dwFuncType == CHIP_EFFECT)
	{
		if (m_pCRole->GetclsCardPacket()->IsFull(1, 0))
		{
			IME_ERROR("userid %u card packet is not enough", m_pCRole->GetdwAccountId());
			m_pCRole->SendErrorMsg(ERRNO_MSG_CARD_PACKET_NOT_ENOUGH, CMD_SC_SHOP_USE_RESULT);
			return;
		}

		if (!IsHave(pConf->dwIndex, pConf->dwPara1))
		{
			IME_ERROR("use have not this goods %u", pConf->dwIndex);
			m_pCRole->SendErrorMsg(ERRNO_MSG_GOODS_NOT_ENOUGH, CMD_SC_SHOP_USE_RESULT);
			return;
		}	
	}
	if (pConf->dwFuncType == ENERGY_ITEM_EFFECT || pConf->dwFuncType == ENERGY_ITEM_COUNT_EFFECT)
	{
		if (m_pCRole->GetdwEnergy() >= m_pCRole->GetdwEnergyMax())
		{
			IME_ERROR("userid %u energy is full", m_pCRole->GetdwAccountId());
			m_pCRole->SendErrorMsg(ERRNO_MSG_ENERGY_FULL, CMD_SC_SHOP_USE_RESULT);
			return;
		}
		STC_CONF_VIP *pVip = CConfVip::Find(m_pCRole->GetwVipLv());
		uint32_t number = 0;
		if (pVip != NULL)
			number = pVip->dwEnergyStrengthNumber;
		if (m_pCRole->GetdwEnergyUseCnt() >= number)
		{
			IME_ERROR("already use %u, limit %u", m_pCRole->GetdwEnergyUseCnt(), number);
			m_pCRole->SendErrorMsg(ERRNO_MSG_ENERGY_USE_MAX, CMD_SC_SHOP_USE_RESULT);
			return;
		}
		m_pCRole->SetdwEnergyUseCnt(m_pCRole->GetdwEnergyUseCnt() + 1);
	}
	if (pConf->dwFuncType == STRENGTH_ITEM_EFFECT || pConf->dwFuncType == STRENGTH_ITEM_COUNT_EFFECT)
	{
		if (m_pCRole->GetdwStrength() >= m_pCRole->GetdwStrengthMax())
		{
			IME_ERROR("userid %u strength is full", m_pCRole->GetdwAccountId());
			m_pCRole->SendErrorMsg(ERRNO_MSG_STRENGTH_FULL, CMD_SC_SHOP_USE_RESULT);
			return;
		}
		STC_CONF_VIP *pVip = CConfVip::Find(m_pCRole->GetwVipLv());
		uint32_t number = 0;
		if (pVip != NULL)
			number = pVip->dwEnergyStrengthNumber;
		if (m_pCRole->GetdwStrengthUseCnt() >= number)
		{
			IME_ERROR("already use %u, limit %u", m_pCRole->GetdwStrengthUseCnt(), number);
			m_pCRole->SendErrorMsg(ERRNO_MSG_STRENGTH_USE_MAX, CMD_SC_SHOP_USE_RESULT);
			return;
		}
		m_pCRole->SetdwStrengthUseCnt(m_pCRole->GetdwStrengthUseCnt() + 1);
	}
	if (pConf->dwFuncType == STAMINA_ITEM_EFFECT || pConf->dwFuncType == STAMINA_ITEM_COUNT_EFFECT)
	{
		if (m_pCRole->GetdwStamina() >= m_pCRole->GetdwStaminaMax())
		{
			IME_ERROR("userid %u stamina is full", m_pCRole->GetdwAccountId());
			m_pCRole->SendErrorMsg(ERRNO_MSG_STAMINA_FULL, CMD_SC_SHOP_USE_RESULT);
			return;
		}
		STC_CONF_VIP *pVip = CConfVip::Find(m_pCRole->GetwVipLv());
		uint32_t number = 0;
		if (pVip != NULL)
			number = pVip->dwStaminaNumber;
		if (m_pCRole->GetwUseStamina() >= number)
		{
			IME_ERROR("already use %u, limit %u", m_pCRole->GetwUseStamina(), number);
			m_pCRole->SendErrorMsg(ERRNO_MSG_STAMINA_USE_MAX, CMD_SC_SHOP_USE_RESULT);
			return;
		}
		m_pCRole->AddwUseStamina();
	}
	if (pConf->dwFuncType == BOX_EFFECT || pConf->dwFuncType == KEY_EFFECT)
	{
		if (!m_pCRole->GetCShop()->IsHaveBoxAndKey(pConf->dwPara1))
		{
			IME_ERROR("have not all box and key");
			m_pCRole->SendErrorMsg(ERRNO_MSG_NO_BOX_KEY, CMD_SC_SHOP_USE_RESULT);
			return;
		}
		if (m_pCRole->GetclsCardPacket()->IsFull(1, 0))
		{
			IME_ERROR("userid %u card packet is not enough", m_pCRole->GetdwAccountId());
			m_pCRole->SendErrorMsg(ERRNO_MSG_CARD_PACKET_NOT_ENOUGH, CMD_SC_SHOP_USE_RESULT);
			return;
		}
	}
	if (pConf->dwFuncType == UNIQUE_GIFT_EFFECT)
	{
		if (m_pCRole->GetclsCardPacket()->IsFull(1, 0))
		{
			IME_ERROR("userid %u card packet is not enough", m_pCRole->GetdwAccountId());
			m_pCRole->SendErrorMsg(ERRNO_MSG_CARD_PACKET_NOT_ENOUGH, CMD_SC_SHOP_USE_RESULT);
			return;
		}
	}
	if (pConf->dwFuncType == TRIAL_ENTER_EFFECT)
	{
		if (!m_pCRole->GetCTrial()->ValidChangeTrial(pConf->dwPara1))
		{
			m_pCRole->SendErrorMsg(ERRNO_MSG_TRIAL_LEVEL, CMD_SC_SHOP_USE_RESULT);
			return;
		}
	}
	if (pConf->dwLevelLimit != 0)
	{
		if (m_pCRole->GetwLevel() < pConf->dwLevelLimit)
		{
			m_pCRole->SendErrorMsg(ERRNO_MSG_SHOP_LEVEL_LIMIT, CMD_SC_SHOP_USE_RESULT);
			return;
		}
	}

	CShopEffect::Init();	
	CShopEffect::ItemUseEffectFunc fn = CShopEffect::Find(pConf->dwFuncType);	
	if (fn == NULL)
	{
		IME_ERROR("this shop id cannot be used in here %u", shopid);
		m_pCRole->SendErrorMsg(ERRNO_MSG_SHOP_ITEM_UNALLOW_USE, CMD_SC_SHOP_USE_RESULT);
		return;
	}

	if (!IsHave(shopid))
	{
		m_pCRole->SendErrorMsg(ERRNO_MSG_GOODS_NOT_ENOUGH, CMD_SC_SHOP_USE_RESULT);
		return;
	}

	CShopEffect::SEffect data;
	data.para1 = pConf->dwPara1;
	data.para2 = pConf->dwPara2;
	data.para3 = pConf->dwPara3;
	data.para4 = pConf->dwPara4;

	if (fn(m_pCRole, data))
	{
		IME_ERROR("use fail %u", shopid);	
		m_pCRole->SendErrorMsg(ERRNO_MSG_SHOP_ITEM_USE_FAIL, CMD_SC_SHOP_USE_RESULT);
		return;
	}
	
	if (pConf->dwFuncType == BOX_EFFECT || pConf->dwFuncType == KEY_EFFECT)
	{
		uint32_t delgoods;
		DelItemByFuncType(BOX_EFFECT, pConf->dwPara1, delgoods);
		HandlerShopInfo(delgoods);
		DelItemByFuncType(KEY_EFFECT, pConf->dwPara1, delgoods);
		HandlerShopInfo(delgoods);
	}
	else if (pConf->dwFuncType == CHIP_EFFECT)
	{
		for (int i = 0; i < pConf->dwPara1; i++)
		{
			DelItem(pConf->dwIndex, SOURCE_ITEM_USE);
		}
	}
	else
	{
		DelItem(shopid, SOURCE_ITEM_USE);
	}

	HandlerShopInfo(shopid);

	WorldPacket info(CMD_SC_SHOP_USE_RESULT);
	info << (uint16_t) 0;
	info << shopid;
	info << pConf->dwFuncType;
	if (pConf->dwFuncType == GIFT_EFFECT)
	{
		info << (uint32_t) data.out.size();
		for (int i = 0; i < data.out.size(); i++)
		{
			info << data.out[i].effect;
			info << data.out[i].para1;
			info << data.out[i].para2;
		}
	}
	else
	{
		info << (uint32_t) 1;
		info << data.para1;
		info << data.para2;
		info << data.para3;
	}
	m_pCRole->SendPacket(&info);
	return ;
}		/* -----  end of method CShop::HandlerBuy  ----- */

bool CShop::IsHaveBoxAndKey(uint32_t CombineId)
{
	std::map<uint32_t, uint32_t>::iterator it;
	STC_CONF_GOODS *pConf = NULL;
	bool IsBox = false;
	bool IsKey = false;
	for (it = m_mapShop.begin(); it != m_mapShop.end(); it++)
	{
		pConf = CConfGoods::Find(it->first);
		if (pConf == NULL)
		{
			IME_ERROR("can not find this shopid %u in shop csv", it->first);
			return false;
		}
		if (pConf->dwFuncType == BOX_EFFECT && pConf->dwPara1 == CombineId)
		{
			if (it->second > 0)
				IsBox = true;
		}
		if (pConf->dwFuncType == KEY_EFFECT && pConf->dwPara1 == CombineId)
		{
			if (it->second > 0)
				IsKey = true;
		}
	}

	if (IsBox && IsKey)
	{
		return true;
	}
	return false;
}

int CShop::DelItemByFuncType(uint32_t Functype, uint32_t CombineId, uint32_t &shopid)
{
	std::map<uint32_t, uint32_t>::iterator it;
	bool IsOk = false;
	STC_CONF_GOODS *pConf = NULL;
	for (it = m_mapShop.begin(); it != m_mapShop.end(); it++)
	{
		pConf = CConfGoods::Find(it->first);
		if (pConf == NULL)
		{
			IME_ERROR("can not find this index %u in shop csv", it->first);
			return -1;
		}
		
		if (Functype == BOX_EFFECT || Functype == KEY_EFFECT)
		{
			if (pConf->dwFuncType == Functype && pConf->dwPara1 == CombineId)
			{
				IsOk = true;
			}
			else
			{
				IsOk = false;
			}
		}
		else
		{
			if (pConf->dwFuncType == Functype)
			{
				IsOk = true;
			}
			else
			{
				IsOk = false;
			}
		}

		if (IsOk)
		{
			shopid = it->first;
			if (it->second > 0)
			{
				it->second--;
				if (it->second == 0)
				{
					m_mapShop.erase(it);
				}
				return 0;
			}
			else
			{
				IME_ERROR("this index %u shop item now %u = 0", it->first, it->second);
				return -1;
			}
		}
	}

//	std::map<uint32_t, uint32_t>::iterator itdel;
//	for (it = m_mapShop.begin(); it != m_mapShop.end(); )
//	{
//		if (it->second == 0)
//		{
//			itdel = it;
//			it++;
//			m_mapShop.erase(itdel);
//		}
//		else
//		{
//			it++;
//		}
//			
//	}

	IME_ERROR("can not find this func type %u combineid %u in shop", Functype, CombineId);
	return -1;
}

void CShop::HandlerShopUseCancel(WorldPacket &pkg)
{
	IME_CHAR_LOG("action [userid %u shop use item cancel]", m_pCRole->GetdwAccountId());
	uint32_t shopid;
	pkg >> shopid;
	if (shopid == AVOID_DURATION_SHOPID)
	{
		m_pCRole->SetdwAvoidDurationTime(0);
		m_pCRole->HandlerInfoOpt();
	}
	WorldPacket info(CMD_SC_SHOP_USE_CANCEL_RESULT);
	info << (uint16_t) 0;
	m_pCRole->SendPacket(&info);
}

void CShop::HandlerStoreData(WorldPacket &pkg)
{
	uint8_t storechannel;
	uint32_t kind;
	pkg >> storechannel;
	pkg >> kind;
	IME_CHAR_LOG("action [userid %u request store list store channel %u kind %u]", m_pCRole->GetdwAccountId(), storechannel, kind);
	m_kind = kind;
	WorldPacket info(CMD_SC_STORE_REQUEST_RESULT);
	ByteBuffer tmp;
	uint32_t number = 0;
	info << storechannel;
	CConfStore::MapData *pData = CConfStore::GetVal();
	CConfStore::MapData::iterator it;
//	if (storechannel == STORE_DOLLAR)
//	{
		for (it = pData->begin(); it != pData->end(); it++)
		{
			if (it->second.byStoreChannel == storechannel && it->second.byIsBuy && it->second.dwKind == kind)
			{
				tmp << it->second.dwStoreIndex;
				tmp << it->second.byStoreChannel;
				tmp << it->second.dwIndex;
				tmp << it->second.byType;
				tmp << it->second.byObjType;
				tmp << it->second.dwObjNumber;
				tmp << it->second.byIsBuy;
				tmp << it->second.dwViewOrder;
				tmp << it->second.strContent;
				tmp << it->second.dwIndexNumber;
				tmp << it->second.dwSendPrize;
				uint32_t rolelimit = m_pCRole->GetGiftRole(it->second.dwStoreIndex);
				uint32_t daylimit = m_pCRole->GetGiftDay(it->second.dwStoreIndex);
				if (it->second.dwRoleLimitNumber != 0)
				{
					int32_t val = (int32_t)(it->second.dwRoleLimitNumber) - (int32_t)rolelimit; 
					val = val < 0 ? 0 : val;
					tmp << val;
//					IME_DEBUG("role limit %d", val);
				}
				else
				{
					tmp << (int32_t) -1;
				}

				if (it->second.dwLimitNumber != 0)
				{
					int32_t val = (int32_t)(it->second.dwLimitNumber) - (int32_t)daylimit; 
					val = val < 0 ? 0 : val;
					tmp << val;
//					IME_DEBUG("role limit %d", val);
				}	
				else
				{
					tmp << (int32_t) -1;
				}
				tmp << it->second.dwViewPrize;
				tmp << it->second.dwMaxBuy;
				tmp << it->second.byGoodsType;

				IME_DEBUG("store %u, shopid %u objtype %u objnumber %u rolelimit %d daylimit %d, csv role %d csv day limit %d", it->second.dwStoreIndex, it->second.byStoreChannel, it->second.byObjType, it->second.dwObjNumber, rolelimit, daylimit, it->second.dwRoleLimitNumber, it->second.dwLimitNumber);
				number++;
			}
		}
		info << number;
		info.append(tmp.contents(), tmp.size());
		m_pCRole->SendPacket(&info);
//	}
//	else if (storechannel == STORE_PRESTIGE) 
//	{
//		for (it = pData->begin(); it != pData->end(); it++)
//		{
//			if (it->second.byStoreChannel == STORE_PRESTIGE && it->second.byIsBuy)
//			{
//				tmp << it->second.dwStoreIndex;
//				tmp << it->second.byStoreChannel;
//				tmp << it->second.dwIndex;
//				tmp << it->second.byType;
//				tmp << it->second.byObjType;
//				tmp << it->second.dwObjNumber;
//				tmp << it->second.byIsBuy;
//				tmp << it->second.dwViewOrder;
//				tmp << (uint32_t) 1;
//				IME_DEBUG("store %u, shopid %u objtype %u objnumber %u", it->second.dwStoreIndex, it->second.byStoreChannel, it->second.byObjType, it->second.dwObjNumber);
//				number++;
//			}
//		}
//		info << number;
//		info.append(tmp.contents(), tmp.size());
//		m_pCRole->SendPacket(&info);
//	}
//	else if (storechannel == STORE_CHARGE) 
//	{
//		for (it = pData->begin(); it != pData->end(); it++)
//		{
//			if (it->second.byStoreChannel == STORE_PRESTIGE && it->second.byIsBuy)
//			{
//				tmp << it->second.dwStoreIndex;
//				tmp << it->second.byStoreChannel;
//				tmp << it->second.dwIndex;
//				tmp << it->second.byType;
//				tmp << it->second.byObjType;
//				tmp << it->second.dwObjNumber;
//				tmp << it->second.byIsBuy;
//				tmp << it->second.dwViewOrder;
//				tmp << it->second.dwIndexNumber;
//				IME_DEBUG("store %u, shopid %u objtype %u objnumber %u", it->second.dwStoreIndex, it->second.byStoreChannel, it->second.byObjType, it->second.dwObjNumber);
//				number++;
//			}
//		}
//		info << number;
//		info.append(tmp.contents(), tmp.size());
//		m_pCRole->SendPacket(&info);
//	}
//	else
//	{
//		IME_ERROR("store channel is unvalid %u", storechannel);
//		m_pCRole->SendErrorMsg(ERRNO_MSG_DATA_ILLEGAL, CMD_SC_SHOP_BUY_RESULT);
//		return;
//	}
	return;
}



std::map<uint16_t, CShopEffect::ItemUseEffectFunc> CShopEffect::m_mapFunc;
void CShopEffect::Init()
{
//	m_mapFunc[EGG_EFFECT] = EggEffect;
	m_mapFunc[GIFT_EFFECT] = GiftEffect;
	m_mapFunc[ENERGY_ITEM_EFFECT] = EnergyItemEffect;
	m_mapFunc[STRENGTH_ITEM_EFFECT] = StrengthItemEffect;
	m_mapFunc[MISS_DURATION_EFFECT] = MissDurationEffect;
	m_mapFunc[COIN_PACKET_EFFECT] = CoinPacketEffect;
	m_mapFunc[COIN_BOX_EFFECT] = CoinBoxEffect;
	m_mapFunc[BOX_EFFECT] = BoxEffect;
	m_mapFunc[KEY_EFFECT] = KeyEffect;
	m_mapFunc[MASTER_EFFECT] = MasterEffect;
	m_mapFunc[UNIQUE_GIFT_EFFECT] = UniqueGiftEffect;
	m_mapFunc[CHIP_EFFECT] = ChipEffect;
	m_mapFunc[VIP_EFFECT] = VipEffect;
	m_mapFunc[ENERGY_ITEM_COUNT_EFFECT] = EnergyItemCountEffect;
	m_mapFunc[STRENGTH_ITEM_COUNT_EFFECT] = StrengthItemEffect;
	m_mapFunc[STAMINA_ITEM_EFFECT] = StaminaItemEffect;
	m_mapFunc[STAMINA_ITEM_COUNT_EFFECT] = StaminaItemCountEffect;
	m_mapFunc[TRIAL_ENTER_EFFECT] = TrialEnterEffect;
	m_mapFunc[ROLE_EXP_EFFECT] = RoleExpEffect;
	m_mapFunc[CARD_EXP_EFFECT] = CardExpEffect;
}

int CShopEffect::UniqueGiftEffect(CRole *pCRole, SEffect &data)
{
	STC_CONF_UNIQUE_COLLECTION* pRand = CConfUniqueCollection::GetRandResult(data.para1);
	if (pRand == NULL)
	{
		IME_ERROR("box rand index %u is not in collection",data.para1);
		return -1;
	}

	SGiveProduct tmp;
	CShopEffect::GiveGameProduct(pCRole, pRand->byType, pRand->dwPara1, pRand->dwPara2, tmp, SOURCE_UNIQUE_GIFT);
	data.para1 = tmp.effect;
	data.para2 = tmp.para1;
	data.para3 = tmp.para2;

	if (tmp.effect == E_EFFECT_CARD)
	{
		STC_CONF_CARD_BASE *pConf = CConfCardBase::Find(pRand->dwPara1);
		if (pConf == NULL)
		{
			IME_ERROR("can not find card unique index %u, type %u, para1 %u, para2 %u", pRand->dwIndex, pRand->byType, pRand->dwPara1, pRand->dwPara2);
			return 0;
		}
		std::string targetid = CUtil::Uint32ToString(pConf->dwIndex);
		if (GlobalConfig::NoticeSpecificCard.find(targetid) != std::string::npos)
		{
			std::string m = pRand->strContent;
			m = CUtil::StrReplace(m, "%s", pCRole->GetstrUserName());
			if (m != "0")
			{
				sWorld->GetCNotice()->DiyNotice(m);
			}
			else
			{
				IME_ERROR("content is 0 unique index %u", pRand->dwIndex);
			}
//			IME_ERROR("notice card");
		}
	}
	else if (tmp.effect == E_EFFECT_DOLLAR || tmp.effect == E_EFFECT_COL_DOLLAR)
	{
		if (tmp.para1 >= GlobalConfig::NoticeSpecificDollarLimit)
		{
			std::string m = pRand->strContent;
			m = CUtil::StrReplace(m, "%s", pCRole->GetstrUserName());
			if (m != "0")
			{
				sWorld->GetCNotice()->DiyNotice(m);
			}
			else
			{
				IME_ERROR("content is 0 unique index %u", pRand->dwIndex);
			}
//			IME_ERROR("notice dollar");
		}
	}
	else if (tmp.effect == E_EFFECT_SHOP)
	{
		std::string targetid = CUtil::Uint32ToString(tmp.para1);
//		IME_ERROR("targetid %s NoticeSpecificItem %s", targetid.c_str(), GlobalConfig::NoticeSpecificItem.c_str());
		if (GlobalConfig::NoticeSpecificItem.find(targetid) != std::string::npos)
		{
			std::string m = pRand->strContent;
			m = CUtil::StrReplace(m, "%s", pCRole->GetstrUserName());
			if (m != "0")
			{
				sWorld->GetCNotice()->DiyNotice(m);
			}
			else
			{
				IME_ERROR("content is 0 unique index %u", pRand->dwIndex);
			}
//			IME_ERROR("notice item");
		}
	}
	return 0;
}

typedef struct 
{
	uint16_t wType;
	uint32_t dwIndex;
	uint32_t dwNumber;
}SShopItem;

int CShopEffect::GiftEffect(CRole *pCRole, SEffect &data)
{
	STC_CONF_GIFT_COLLECTION *pConf = CConfGiftCollection::GetRandResult(data.para1);
	if (pConf == NULL)
	{
		IME_ERROR("index %u can not in gift collection", data.para1);
		return -1;
	}
	
	data.out.clear();
	SShopItem temp;
	std::vector<SShopItem> vec;
	
	if (pConf->wType1 != 0)
	{
		temp.wType = pConf->wType1;
		temp.dwIndex = pConf->dwProductIndex1;
		temp.dwNumber = pConf->dwNumber1;
		vec.push_back(temp);
	}

	if (pConf->wType2 != 0)
	{
		temp.wType = pConf->wType2;
		temp.dwIndex = pConf->dwProductIndex2;
		temp.dwNumber = pConf->dwNumber2;
		vec.push_back(temp);
	}

	if (pConf->wType3 != 0)
	{
		temp.wType = pConf->wType3;
		temp.dwIndex = pConf->dwProductIndex3;
		temp.dwNumber = pConf->dwNumber3;
		vec.push_back(temp);
	}

	if (pConf->wType4 != 0)
	{
		temp.wType = pConf->wType4;
		temp.dwIndex = pConf->dwProductIndex4;
		temp.dwNumber = pConf->dwNumber4;
		vec.push_back(temp);
	}

	for (int i = 0; i < vec.size(); i++)
	{
//		switch(vec[i].wType)
//		{
//			case E_OBJ_CARD:
//				for (int j = 0; j < vec[i].dwNumber; j++)
//				{
//				uint32_t objid, opt;
//				pCRole->GetclsCardPacket()->CardAddByPara(vec[i].dwIndex, objid, opt); 
//				if (opt)
//				pCRole->GetclsCardPacket()->SingleCardAddUpdate(objid);
//				}
//				break;
//			case E_OBJ_ITEM:
//				for (int j = 0; j < vec[i].dwNumber; j++)
//				{
//				pCRole->GetclsItemPacket()->AddItem(vec[i].dwIndex);
//				pCRole->GetclsItemPacket()->ItemUpdate(vec[i].dwIndex, CItem::ADD);
//				}
//				break;
//			case E_OBJ_SHOP:
//				for (int j = 0; j < vec[i].dwNumber; j++)
//				{
//				pCRole->GetCShop()->AddItem(vec[i].dwIndex);
//				pCRole->GetCShop()->HandlerShopInfo();
//				}
//				break;
//			case E_OBJ_COIN:
//				pCRole->ChangeCoin(vec[i].dwNumber);
//				pCRole->SendProUpdate();
//				break;
//			case E_OBJ_DOLLAR:
//				
//				uint32_t order_id;
//				if (!CDBCtrl::GetShopOrderId(pCRole->GetdwAccountId(), order_id))
//				{
//					IME_ERROR("user %u give dollar %u fail in EggEffect", pCRole->GetdwAccountId(), vec[i].dwNumber);
//					return -1;
//				}
//
//				pCRole->ChangeDollar(vec[i].dwNumber);
//				pCRole->SendProUpdate();
//				CDBCtrl::LogShopRecord(pCRole->GetdwAccountId(), order_id, vec[i].dwNumber, 0, 0, E_DC_GIVE);	
//				break;
//			default:
//				IME_ERROR("unvalid type %u", vec[i].wType);
//				return -1;
//		}
		SGiveProduct tmp;
		if (vec[i].wType < E_OBJ_MAX || vec[i].wType == E_OBJ_CARD_MORE)
		{
			CShopEffect::GiveGameProductSpecial(pCRole, vec[i].wType, vec[i].dwIndex, vec[i].dwNumber, tmp, SOURCE_GIFT);
		}
		data.out.push_back(tmp);
	}
//	memset(&data, 0, sizeof(SEffect));
	return 0;
}

int CShopEffect::EnergyItemEffect(CRole *pCRole, SEffect &data)
{
	uint32_t val = pCRole->GetdwEnergyMax();
	val = val * data.para1 / 100.0;
	IME_DEBUG("shop use now %u, %u, %u", pCRole->GetdwEnergy(), pCRole->GetdwEnergyMax(), val);
	pCRole->ChangeEnergy(val);
	pCRole->SendProUpdate();
	pCRole->HandlerInfoOpt();
//	memset(&data, 0, sizeof(SEffect));
	
	pCRole->GetCMission()->MeatCount(ENERGY_SHOPID, 1);

	data.para1 = E_EFFECT_ENERGY;
	data.para2 = val;

	return 0;
}

int CShopEffect::StrengthItemEffect(CRole *pCRole, SEffect &data)
{
	uint32_t val = pCRole->GetdwStrengthMax();
	val = val * data.para1 / 100.0;
	pCRole->ChangeStrength(val);
	pCRole->SendProUpdate();
	pCRole->HandlerInfoOpt();
//	memset(&data, 0, sizeof(SEffect));

	pCRole->GetCMission()->WineCount(STRENGTH_SHOPID, 1);

	data.para1 = E_EFFECT_STRENGTH;
	data.para2 = val;
	return 0;
}

int CShopEffect::MissDurationEffect(CRole *pCRole, SEffect &data)
{
	time_t now = time(NULL);
	uint32_t result_time;
	if (pCRole->GetdwAvoidDurationTime() > now)
	{
		result_time = pCRole->GetdwAvoidDurationTime() + data.para1;
	}
	else
	{
		result_time = now + data.para1;
	}
	pCRole->SetdwAvoidDurationTime(result_time);
	pCRole->HandlerInfoOpt();

	pCRole->GetCMission()->AvoidTime(AVOID_DURATION_SHOPID, 1);
//	memset(&data, 0, sizeof(SEffect));

	return 0;
}

int CShopEffect::CoinPacketEffect(CRole *pCRole, SEffect &data)
{
	pCRole->ChangeCoin(data.para1, SOURCE_COIN_PACKET);
	pCRole->SendProUpdate();
	uint32_t coin = data.para1;
	data.para1 = coin;
	data.para2 = 0;
//	memset(&data, 0, sizeof(SEffect));
	return 0;
}

int CShopEffect::CoinBoxEffect(CRole *pCRole, SEffect &data)
{
	pCRole->ChangeCoin(data.para1, SOURCE_BOX_PACKET);
	pCRole->SendProUpdate();
//	memset(&data, 0, sizeof(SEffect));
	return 0;
}

//int CShopEffect::SkillCombineBookEffect(CRole *pCRole, uint32_t objid, uint8_t position, SEffect &data)
//{
//	STC_CONF_SHOP *pConf1 = CConfShop::Find(data.para1);
//	STC_CONF_SHOP *pConf2 = CConfShop::Find(data.para2);
//	STC_CONF_SHOP *pConf3 = CConfShop::Find(data.para3);
//	STC_CONF_SHOP *pConf4 = CConfShop::Find(data.para4);
//
//	std::set<uint32_t> attr;
//	std::set<uint32_t> team;
//	uint32_t proindex = E_COMBINE_LOW;
//	attr.insert(GRASS);
//	attr.insert(FIRE);
//	attr.insert(WATER);
//	attr.insert(FLY);
//	attr.insert(NORMAL);
//	attr.insert(GROUND);
//	attr.insert(ELECTRICITY);
//	attr.insert(ICE);
//	attr.insert(DRAGON);
//	team.insert(E_ST_ATTACK);
//	team.insert(E_ST_DEFENCE);
//	team.insert(E_ST_ASSIST);
//
//	if (data.para1 != 0)
//	{
//		if (pConf1 == NULL)
//		{
//			IME_ERROR("index %u not in shop csv", data.para1);
//			return -1;
//		}
//		if (pConf1->dwFuncType != SKILL_COMBINE_ATTR)
//		{
//			IME_ERROR("index %u is not skill combine attr", data.para1);
//			return -1;
//		}
//		attr.clear();
//		attr.insert(pConf1->dwPara1);
//		attr.insert(pConf1->dwPara2);
//		attr.insert(pConf1->dwPara3);
//		attr.insert(pConf1->dwPara4);
//		attr.erase(0);
//	}
//
//	if (data.para2 != 0)
//	{
//		if (pConf2 == NULL)
//		{
//			IME_ERROR("index %u not in shop csv", data.para2);
//			return -1;
//		}
//		if (pConf2->dwFuncType != SKILL_COMBINE_TEAM)
//		{
//			IME_ERROR("index %u is not skill combine team", data.para2);
//			return -1;
//		}
//		team.clear();
//		team.insert(pConf2->dwPara1);
//		team.insert(pConf2->dwPara2);
//		team.insert(pConf2->dwPara3);
//		team.insert(pConf2->dwPara4);
//		team.erase(0);
//	}
//
//	if (data.para3 != 0)
//	{
//		if (pConf3 == NULL && data.para3 != 0)
//		{
//			IME_ERROR("index %u not in shop csv", data.para3);
//			return -1;
//		}
//		if (pConf3->dwFuncType != SKILL_COMBINE_PRO)
//		{
//			IME_ERROR("index %u is not skill combine pro", data.para3);
//			return -1;
//		}
//		proindex = pConf3->dwPara1;
//	}
//
//	if (pConf4 == NULL && data.para4 != 0)
//	{
//		IME_ERROR("index %u not in shop csv", data.para4);
//		return -1;
//	}
//	if (pConf4->dwFuncType != SKILL_COMBINE_BOOK)
//	{
//		IME_ERROR("index %u is not skill combine book", data.para4);
//		return -1;
//	}
//
//	CCard *pCard = pCRole->GetclsCardPacket()->Find(objid);
//	if (pCard == NULL)
//	{
//		IME_ERROR("card objid %u is not in user %u packet", data.para2, pCRole->GetdwAccountId());
//		return -1;
//	}
//	IME_DEBUG("card star %u", pCard->GetbyStarLevel());
//
//	std::map<uint32_t, STC_CONF_CARD_SKILL> mapSkill = CConfCardSkill::GetVal();
//	std::map<uint32_t, STC_CONF_CARD_SKILL>::iterator it;
//	STC_CONF_CARD_SKILL *pConfSkill;
//	std::vector<uint32_t> vecSkillIndex;
//	std::vector<int> vecPro;
//	STC_CONF_SKILL_CREATE *pConfCreate;
//	for (it = mapSkill.begin(); it != mapSkill.end(); it++)
//	{
//		pConfSkill = &(it->second);	
//		if (attr.find(pConfSkill->para1) != attr.end() && team.find(pConfSkill->byTeam) != team.end())
//		{
//			if (proindex == E_COMBINE_LOW)
//			{
//				pConfCreate = CConfSkillCreate::Find(pConfSkill->dwIndex, pCard->GetbyStarLevel());
//				if (pConfCreate == NULL)
//					continue;
//				vecSkillIndex.push_back(pConfSkill->dwIndex);
//				vecPro.push_back(pConfCreate->dwLowPro);
//				IME_DEBUG("low skill index %u, pro %u", pConfSkill->dwIndex, pConfCreate->dwLowPro);
//			}
//			else
//			{
//				pConfCreate = CConfSkillCreate::Find(pConfSkill->dwIndex, pCard->GetbyStarLevel());
//				if (pConfCreate == NULL)
//					continue;
//				vecSkillIndex.push_back(pConfSkill->dwIndex);
//				vecPro.push_back(pConfCreate->dwHighPro);
//				IME_DEBUG("high skill index %u, pro %u", pConfSkill->dwIndex, pConfCreate->dwHighPro);
//			}
//		}
//	}
//	std::set<uint32_t>::iterator itset;
//	for (itset = attr.begin(); itset != attr.end(); itset++)
//	{
//		IME_DEBUG("attr val %u", *itset); 
//	}
//	for (itset = team.begin(); itset != team.end(); itset++)
//	{
//		IME_DEBUG("team val %u", *itset); 
//	}
//
//	if (vecPro.size() == 0)
//		return -1;
//	uint32_t rel = CUtil::RandFactor(vecPro);
//
//	if (position == E_CSP_ONE)
//	{
//		pCard->SetdwAttackSkillIndex(vecSkillIndex[rel]);
//	}
//	else if (position == E_CSP_TWO)
//	{
//		pCard->SetdwDeffenseSkillIndex(vecSkillIndex[rel]);
//	}
//	else
//	{
//		IME_ERROR("unvalid skill position %u", position);
//		return -1;
//	}
//	pCard->SendProUpdate();
//	memset(&data, 0, sizeof(SEffect));
//	data.para1 = SKILL_GET;
//	data.para2 = vecSkillIndex[rel];
//	return 0;
//}

int CShopEffect::VipEffect(CRole *pCRole, SEffect &data)
{
	pCRole->ChangeVipExp(data.para1);
	uint16_t viplv = pCRole->GetwVipLv();
	pCRole->Calculate(true);
	pCRole->HandlerInfoOpt();
	IME_ERROR("new vip lv %u, lv %u", pCRole->GetwVipLv(), viplv); 
	if (pCRole->GetwVipLv() != viplv)
	{
		CDataPush push;
		push.HandlerDataPushVip(pCRole, pCRole->GetwVipLv());
	}
	uint32_t vipexp = data.para1;
	data.para1 = E_EFFECT_VIP_EXP;
	data.para2 = vipexp; 
//	memset(&data, 0, sizeof(SEffect));
	return 0;
}

void CShop::HandlerBuyDelCD(WorldPacket &pkg)
{
//	IME_ERROR("this function is no use");
//	return;
	uint32_t shopid;
	int32_t para1;
	int32_t para2;
	pkg >> shopid;	
	pkg >> para1;
	pkg >> para2;
	IME_CHAR_LOG("action [userid %u del cd shopid %u para1 %u para2 %u]", m_pCRole->GetdwAccountId(), shopid, para1, para2);
	uint32_t order_id;
	if (!CDBCtrl::GetShopOrderId(m_pCRole->GetdwAccountId(), order_id))
	{
		m_pCRole->SendErrorMsg(ERRNO_MSG_SHOP_GET_ORDER_ID, CMD_SC_SHOP_BUY_DEL_CD_RESULT);
		return;
	}
	
	if (shopid == E_DCD_DURATION_MISSION)
	{

//		if (!m_pCRole->IsOpenArena())
//		{
//			IME_ERROR("user id %u can not open arena", m_pCRole->GetdwAccountId());
//			m_pCRole->SendErrorMsg(ERRNO_MSG_ARENA_NOT_OPEN, CMD_SC_SHOP_BUY_DEL_CD_RESULT);
//			return;
//		}

		if (m_pCRole->GetdwDollar() < GlobalConfig::DurationMissionDelConsume)
		{
			m_pCRole->SendErrorMsg(ERRNO_MSG_DOLLAR_NOT_ENGOUH, CMD_SC_SHOP_BUY_DEL_CD_RESULT);
			return;
		}

		uint32_t delnumber;
		STC_CONF_VIP *pVip = CConfVip::Find(m_pCRole->GetwVipLv());
		if (pVip == NULL)
		{
			delnumber = 0;
		}
		else
		{
			delnumber = pVip->dwDelDMissionNumber;
		}
		if (m_pCRole->GetdwDelDMissionCnt() >= delnumber)
		{
			m_pCRole->SendErrorMsg(ERRNO_MSG_DEL_DURATION_CHALLENGE_MAX, CMD_SC_SHOP_BUY_DEL_CD_RESULT);
			return;
		}

		if (m_pCRole->ChangeDollar(-GlobalConfig::DurationMissionDelConsume, SOURCE_DCD_NORMAL_TASK))
		{
			m_pCRole->SendErrorMsg(ERRNO_MSG_DOLLAR_CONSUME_FAIL, CMD_SC_SHOP_BUY_DEL_CD_RESULT);
			return;
		}

		CDBCtrl::LogShopRecord(m_pCRole->GetdwAccountId(), order_id, GlobalConfig::DurationMissionDelConsume, E_DCD_DURATION_MISSION, 1, E_DC_DEL_CD);	
		
		m_pCRole->GetCDurationMission()->FlushMissionChallenge(para1, E_MISSION_MAIN);
		m_pCRole->GetCDurationMission()->HandlerDurationMissionInfo(para1);
		m_pCRole->SetdwDelDMissionCnt(m_pCRole->GetdwDelDMissionCnt() + 1);
		m_pCRole->SendProUpdate();
		CDBCtrl::LogShopProductGive(m_pCRole->GetdwAccountId(), order_id);
	}
	else if (shopid == E_DCD_HIGH_DURATION_MISSION)
	{
		if (m_pCRole->GetdwDollar() < GlobalConfig::HighDurationMissionDelConsume)
		{
			m_pCRole->SendErrorMsg(ERRNO_MSG_DOLLAR_NOT_ENGOUH, CMD_SC_SHOP_BUY_DEL_CD_RESULT);
			return;
		}

		uint32_t delnumber;
		STC_CONF_VIP *pVip = CConfVip::Find(m_pCRole->GetwVipLv());
		if (pVip == NULL)
		{
			delnumber = 0;
		}
		else
		{
			delnumber = pVip->dwHighDelDMissionNumber;
		}
		if (m_pCRole->GetdwHighDelDMissionCnt() >= delnumber)
		{
			m_pCRole->SendErrorMsg(ERRNO_MSG_DEL_DURATION_CHALLENGE_MAX, CMD_SC_SHOP_BUY_DEL_CD_RESULT);
			return;
		}

		if (m_pCRole->ChangeDollar(-GlobalConfig::HighDurationMissionDelConsume, SOURCE_DCD_HIGH_TASK))
		{
			m_pCRole->SendErrorMsg(ERRNO_MSG_DOLLAR_CONSUME_FAIL, CMD_SC_SHOP_BUY_DEL_CD_RESULT);
			return;
		}

		CDBCtrl::LogShopRecord(m_pCRole->GetdwAccountId(), order_id, GlobalConfig::HighDurationMissionDelConsume, E_DCD_DURATION_MISSION, 1, E_DC_DEL_CD);	
		
		m_pCRole->GetCDurationMission()->FlushMissionChallenge(para1, E_MISSION_HIGH);
		m_pCRole->GetCDurationMission()->HandlerHighDurationMissionInfo(para1);
		m_pCRole->SetdwHighDelDMissionCnt(m_pCRole->GetdwHighDelDMissionCnt() + 1);
		m_pCRole->SendProUpdate();
		CDBCtrl::LogShopProductGive(m_pCRole->GetdwAccountId(), order_id);

	}
	else if (shopid == E_DCD_BRANCH_MISSION)
	{
		STC_CONF_DURATION_MISSION *p = CConfDurationMission::Find(para1);
		if (p == NULL)
		{
			IME_ERROR("load mission conf fail taskindex %u", para1);
			m_pCRole->SendErrorMsg(ERRNO_MSG_CSV_LOAD, CMD_SC_SHOP_BUY_DEL_CD_RESULT);
			return;
		}

		if (m_pCRole->GetdwDollar() < GlobalConfig::BMCDComsume)
		{
			m_pCRole->SendErrorMsg(ERRNO_MSG_DOLLAR_NOT_ENGOUH, CMD_SC_SHOP_BUY_DEL_CD_RESULT);
			return;
		}

		if (m_pCRole->ChangeDollar(-GlobalConfig::BMCDComsume, SOURCE_DCD_BRANCH_TASK))
		{
			m_pCRole->SendErrorMsg(ERRNO_MSG_DOLLAR_CONSUME_FAIL, CMD_SC_SHOP_BUY_DEL_CD_RESULT);
			return;
		}

		CDBCtrl::LogShopRecord(m_pCRole->GetdwAccountId(), order_id, GlobalConfig::BMCDComsume, E_DCD_BRANCH_MISSION, 1, E_DC_DEL_CD);	
		//todo delete CD
//		CDBCtrl::SaveUserMission(m_pCRole->GetdwAccountId(), para1, 0, 0);
//		CTask::HandlerMissionBranchInfo(m_pCRole);
		m_pCRole->GetCDurationMission()->DelBranchMissionCD(para1);
		m_pCRole->GetCDurationMission()->HandlerMissionBranchInfo();
		m_pCRole->SendProUpdate();
		CDBCtrl::LogShopProductGive(m_pCRole->GetdwAccountId(), order_id);
	}
	else if (shopid == E_RECOVER_ENERGY)
	{
		uint32_t costdollar = m_pCRole->GetdwEnergyMax() * (GlobalConfig::EnergyWorth + m_pCRole->GetwUseEnergy() * GlobalConfig::EnergyWorthGrow) / 100;
		if (m_pCRole->GetdwDollar() < costdollar)
		{
			m_pCRole->SendErrorMsg(ERRNO_MSG_DOLLAR_NOT_ENGOUH, CMD_SC_SHOP_BUY_DEL_CD_RESULT);
			return;
		}

		if (m_pCRole->ChangeDollar(-costdollar, SOURCE_ENERGY_RECOVER))
		{
			m_pCRole->SendErrorMsg(ERRNO_MSG_DOLLAR_CONSUME_FAIL, CMD_SC_SHOP_BUY_DEL_CD_RESULT);
			return;
		}

		CDBCtrl::LogShopRecord(m_pCRole->GetdwAccountId(), order_id, costdollar, E_RECOVER_ENERGY, 1, E_DC_DEL_CD);	
		m_pCRole->ChangeEnergyNoLimit(m_pCRole->GetdwEnergyMax());
		m_pCRole->AddwUseEnergy();
		m_pCRole->HandlerInfoOpt();
		m_pCRole->SendProUpdate();
		m_pCRole->GetCActive()->HandlerInfoRecover();
		CDBCtrl::LogShopProductGive(m_pCRole->GetdwAccountId(), order_id);
	}
	else if (shopid == E_RECOVER_STRENGTH)
	{
		uint32_t costdollar = m_pCRole->GetdwStrengthMax() * (GlobalConfig::StrengthWorth + m_pCRole->GetwUseStrength() * GlobalConfig::StrengthWorthGrow) / 100;
		if (m_pCRole->GetdwDollar() < costdollar)
		{
			m_pCRole->SendErrorMsg(ERRNO_MSG_DOLLAR_NOT_ENGOUH, CMD_SC_SHOP_BUY_DEL_CD_RESULT);
			return;
		}

		if (m_pCRole->ChangeDollar(-costdollar, SOURCE_STRENGTH_RECOVER))
		{
			m_pCRole->SendErrorMsg(ERRNO_MSG_DOLLAR_CONSUME_FAIL, CMD_SC_SHOP_BUY_DEL_CD_RESULT);
			return;
		}

		CDBCtrl::LogShopRecord(m_pCRole->GetdwAccountId(), order_id, costdollar, E_RECOVER_ENERGY, 1, E_DC_DEL_CD);	
		m_pCRole->ChangeStrengthNoLimit(m_pCRole->GetdwStrengthMax());
		m_pCRole->AddwUseStrength();
		m_pCRole->HandlerInfoOpt();
		m_pCRole->SendProUpdate();
		m_pCRole->GetCActive()->HandlerInfoRecover();
		CDBCtrl::LogShopProductGive(m_pCRole->GetdwAccountId(), order_id);
	}
	else if (shopid == E_RECOVER_STAMINA)
	{
		uint32_t costdollar = m_pCRole->GetdwStaminaMax() * (GlobalConfig::StaminaWorth + m_pCRole->GetwUseStamina() * GlobalConfig::StaminaWorthGrow) / 100;
		if (m_pCRole->GetdwDollar() < costdollar)
		{
			m_pCRole->SendErrorMsg(ERRNO_MSG_DOLLAR_NOT_ENGOUH, CMD_SC_SHOP_BUY_DEL_CD_RESULT);
			return;
		}

		if (m_pCRole->ChangeDollar(-costdollar, SOURCE_STAMINA_RECOVER))
		{
			m_pCRole->SendErrorMsg(ERRNO_MSG_DOLLAR_CONSUME_FAIL, CMD_SC_SHOP_BUY_DEL_CD_RESULT);
			return;
		}

		CDBCtrl::LogShopRecord(m_pCRole->GetdwAccountId(), order_id, costdollar, E_RECOVER_STAMINA, 1, E_DC_DEL_CD);	
		m_pCRole->ChangeStaminaNoLimit(m_pCRole->GetdwStaminaMax());
		m_pCRole->AddwUseStamina();
		m_pCRole->HandlerInfoOpt();
		m_pCRole->SendProUpdate();
		m_pCRole->GetCActive()->HandlerInfoRecover();
		CDBCtrl::LogShopProductGive(m_pCRole->GetdwAccountId(), order_id);
	}
	else if (shopid == E_INCREASE_ROLE_PACKET)
	{
		uint32_t costdollar = (m_pCRole->GetExtraRolePacketLimit() / GlobalConfig::RolePacketNumber + 1) * GlobalConfig::RolePacketConsume;
		if (m_pCRole->GetExtraRolePacketLimit() + GlobalConfig::RoleCardLimit >= ROLE_CARD_PACKET_LIMIT)
		{
			m_pCRole->SendErrorMsg(ERRNO_MSG_ROLE_MAX_PACKET_LIMIT, CMD_SC_SHOP_BUY_DEL_CD_RESULT);
			return;
		}

		if (m_pCRole->GetdwDollar() < costdollar)
		{
			m_pCRole->SendErrorMsg(ERRNO_MSG_DOLLAR_NOT_ENGOUH, CMD_SC_SHOP_BUY_DEL_CD_RESULT);
			return;
		}

		if (m_pCRole->ChangeDollar(-costdollar, SOURCE_ROLE_PACKET))
		{
			m_pCRole->SendErrorMsg(ERRNO_MSG_DOLLAR_CONSUME_FAIL, CMD_SC_SHOP_BUY_DEL_CD_RESULT);
			return;
		}

		CDBCtrl::LogShopRecord(m_pCRole->GetdwAccountId(), order_id, costdollar, E_INCREASE_ROLE_PACKET, 1, E_DC_DEL_CD);	
		m_pCRole->ChangeExtraRolePacketLimit(GlobalConfig::RolePacketNumber);
		m_pCRole->HandlerInfoOpt();
		m_pCRole->SendProUpdate();
		CDBCtrl::LogShopProductGive(m_pCRole->GetdwAccountId(), order_id);
	}
	else if (shopid == E_INCREASE_EQUIP_PACKET)
	{
		uint32_t costdollar = (m_pCRole->GetExtraEquipPacketLimit() / GlobalConfig::EquipPacketNumber + 1) * GlobalConfig::EquipPacketConsume;
		if (m_pCRole->GetExtraEquipPacketLimit() + GlobalConfig::EquipCardLimit >= EQUIP_CARD_PACKET_LIMIT)
		{
			m_pCRole->SendErrorMsg(ERRNO_MSG_EQUIP_MAX_PACKET_LIMIT, CMD_SC_SHOP_BUY_DEL_CD_RESULT);
			return;
		}

		if (m_pCRole->GetdwDollar() < costdollar)
		{
			m_pCRole->SendErrorMsg(ERRNO_MSG_DOLLAR_NOT_ENGOUH, CMD_SC_SHOP_BUY_DEL_CD_RESULT);
			return;
		}

		if (m_pCRole->ChangeDollar(-costdollar, SOURCE_EQUIP_PACKET))
		{
			m_pCRole->SendErrorMsg(ERRNO_MSG_DOLLAR_CONSUME_FAIL, CMD_SC_SHOP_BUY_DEL_CD_RESULT);
			return;
		}

		CDBCtrl::LogShopRecord(m_pCRole->GetdwAccountId(), order_id, costdollar, E_INCREASE_EQUIP_PACKET, 1, E_DC_DEL_CD);	
		m_pCRole->ChangeExtraEquipPacketLimit(GlobalConfig::RolePacketNumber);
		m_pCRole->HandlerInfoOpt();
		m_pCRole->SendProUpdate();
		CDBCtrl::LogShopProductGive(m_pCRole->GetdwAccountId(), order_id);
	}
	else if (shopid == E_DCD_ACTIVE_DURATION_MISSION)
	{

//		if (!m_pCRole->IsOpenArena())
//		{
//			IME_ERROR("user id %u can not open arena", m_pCRole->GetdwAccountId());
//			m_pCRole->SendErrorMsg(ERRNO_MSG_ARENA_NOT_OPEN, CMD_SC_SHOP_BUY_DEL_CD_RESULT);
//			return;
//		}

		if (m_pCRole->GetdwDollar() < GlobalConfig::ActiveDurationMissionDelConsume)
		{
			m_pCRole->SendErrorMsg(ERRNO_MSG_DOLLAR_NOT_ENGOUH, CMD_SC_SHOP_BUY_DEL_CD_RESULT);
			return;
		}

		uint32_t delnumber;
		STC_CONF_VIP *pVip = CConfVip::Find(m_pCRole->GetwVipLv());
		if (pVip == NULL)
		{
			delnumber = 0;
		}
		else
		{
			delnumber = pVip->dwDelDMissionNumber;
		}
		if (m_pCRole->GetdwDelDMissionCnt() >= delnumber)
		{
			m_pCRole->SendErrorMsg(ERRNO_MSG_DEL_DURATION_CHALLENGE_MAX, CMD_SC_SHOP_BUY_DEL_CD_RESULT);
			return;
		}

		if (m_pCRole->ChangeDollar(-GlobalConfig::DurationMissionDelConsume, SOURCE_DCD_ACTIVE_TASK))
		{
			m_pCRole->SendErrorMsg(ERRNO_MSG_DOLLAR_CONSUME_FAIL, CMD_SC_SHOP_BUY_DEL_CD_RESULT);
			return;
		}

		CDBCtrl::LogShopRecord(m_pCRole->GetdwAccountId(), order_id, GlobalConfig::ActiveDurationMissionDelConsume, E_DCD_ACTIVE_DURATION_MISSION, 1, E_DC_DEL_CD);	
		
		m_pCRole->GetCDurationMission()->FlushMissionChallenge(para1, E_MISSION_ACTIVE);
		m_pCRole->GetCDurationMission()->HandlerActiveDurationMissionInfo(para1);
		m_pCRole->SetdwDelDMissionCnt(m_pCRole->GetdwDelDMissionCnt() + 1);
		m_pCRole->SendProUpdate();
		CDBCtrl::LogShopProductGive(m_pCRole->GetdwAccountId(), order_id);
	}
	else if (shopid == E_DCD_ARENA_CHALLENGES)
	{
		if (!IS_THERE_ARENA(m_pCRole->GetbyFlag()))
		{
			m_pCRole->SendErrorMsg(ERRNO_MSG_ARENA_NOT_OPEN, CMD_SC_SHOP_BUY_DEL_CD_RESULT);
			return;
		}

		if (m_pCRole->GetdwDollar() < GlobalConfig::RecoverChallengesConsume)
		{
			m_pCRole->SendErrorMsg(ERRNO_MSG_DOLLAR_NOT_ENGOUH, CMD_SC_SHOP_BUY_DEL_CD_RESULT);
			return;
		}

		if (m_pCRole->ChangeDollar(-GlobalConfig::RecoverChallengesConsume, SOURCE_ARENA_CHALLENGES))
		{
			m_pCRole->SendErrorMsg(ERRNO_MSG_DOLLAR_CONSUME_FAIL, CMD_SC_SHOP_BUY_DEL_CD_RESULT);
			return;
		}

		CDBCtrl::LogShopRecord(m_pCRole->GetdwAccountId(), order_id, GlobalConfig::ActiveDurationMissionDelConsume, E_DCD_ARENA_CHALLENGES, 1, E_DC_DEL_CD);	
		
		m_pCRole->GetclsArena()->SetdwChallengeNumber(0);
		m_pCRole->GetclsArena()->HandlerProUpdate();
		CDBCtrl::LogShopProductGive(m_pCRole->GetdwAccountId(), order_id);
	}
	//TODO
	else if (shopid == E_TOWER_THREE_ATTACK) {
		//判断元宝够不够扣
		if(m_pCRole->GetdwDollar() < GlobalConfig::ThreeAttackCost){
			m_pCRole->SendErrorMsg(ERRNO_MSG_DOLLAR_NOT_ENGOUH, CMD_SC_SHOP_BUY_DEL_CD_RESULT);
			return;
		}
		CDBCtrl::LogShopRecord(m_pCRole->GetdwAccountId(), order_id, GlobalConfig::ThreeAttackCost, E_TOWER_THREE_ATTACK, 1, E_DC_DEL_CD);
		//扣元宝
		if(m_pCRole->ChangeDollar(-GlobalConfig::ThreeAttackCost,SOURCE_TOWER)){
			m_pCRole->SendErrorMsg(ERRNO_MSG_DOLLAR_CONSUME_FAIL,CMD_SC_SHOP_BUY_DEL_CD_RESULT);
			return;
		}

		//增加三倍攻击
		m_pCRole->getCTower()->handlerTowerThreeAttack();

		//发送更新角色元宝
		m_pCRole->SendProUpdate();


	}
	else if (shopid == E_TOWER_TWO_ATTACK) {
		if (m_pCRole->GetdwDollar() < GlobalConfig::TwoAttackCost) {
			m_pCRole->SendErrorMsg(ERRNO_MSG_DOLLAR_NOT_ENGOUH,CMD_SC_SHOP_BUY_DEL_CD_RESULT);
			return;
		}
		//处理订单信息
		CDBCtrl::LogShopRecord(m_pCRole->GetdwAccountId(), order_id, GlobalConfig::TwoAttackCost, E_TOWER_TWO_ATTACK, 1, E_DC_DEL_CD);
		//处理如何扣钱
		if(m_pCRole->ChangeDollar(-GlobalConfig::TwoAttackCost,SOURCE_TOWER)){
			m_pCRole->SendErrorMsg(ERRNO_MSG_DOLLAR_CONSUME_FAIL,CMD_SC_SHOP_BUY_DEL_CD_RESULT);
			return;
		}
		//增加二倍攻击
		m_pCRole->getCTower()->handlerTowerTwoAttack();

		//发送更新角色元宝
		m_pCRole->SendProUpdate();

	}
	else if (shopid == E_TOWER_PURCHASE_BREAK_TIMES) {
		//判断购买闯关次数是达到限制
		if(m_pCRole->getCTower()->getBuytimes() + 1 > GlobalConfig::BuyBreakBlockadeTimesLimit){
			m_pCRole->SendErrorMsg(ERRNO_MSG_BUY_BREAK_BLOCKADE_TIMES_LIMIT, CMD_SC_SHOP_BUY_DEL_CD_RESULT);
			return;
		}
		//判断钱是不是够扣
		if (m_pCRole->GetdwDollar() < GlobalConfig::BuyBreakBlockadeTimesCost * para1) {
			m_pCRole->SendErrorMsg(ERRNO_MSG_DOLLAR_NOT_ENGOUH, CMD_SC_SHOP_BUY_DEL_CD_RESULT);
			return;
		}
		//处理订单信息
		CDBCtrl::LogShopRecord(m_pCRole->GetdwAccountId(), order_id, GlobalConfig::BuyBreakBlockadeTimesCost * para1, E_TOWER_PURCHASE_BREAK_TIMES, 1, E_DC_DEL_CD);
		//处理如何扣钱
		if(m_pCRole->ChangeDollar(-GlobalConfig::BuyBreakBlockadeTimesCost * para1,SOURCE_TOWER)){
			m_pCRole->SendErrorMsg(ERRNO_MSG_DOLLAR_CONSUME_FAIL,CMD_SC_SHOP_BUY_DEL_CD_RESULT);
			return;
		}
		//增加闯关次数
		m_pCRole->getCTower()->handlerTowerBreakBlockadeTimes(para1);
		//发送更新角色元宝
		m_pCRole->SendProUpdate();

	}
	else if (shopid == E_TOWER_REFRESH_ATTRIBUTE) {
		if (m_pCRole->GetdwDollar() < GlobalConfig::BossNatureCost) {
			m_pCRole->SendErrorMsg(ERRNO_MSG_DOLLAR_NOT_ENGOUH,CMD_SC_SHOP_BUY_DEL_CD_RESULT);
			return;
		}
		//处理订单信息
		CDBCtrl::LogShopRecord(m_pCRole->GetdwAccountId(), order_id, GlobalConfig::BossNatureCost, E_TOWER_REFRESH_ATTRIBUTE, 1, E_DC_DEL_CD);
		//处理如何扣钱
		if(m_pCRole->ChangeDollar(-GlobalConfig::BossNatureCost,SOURCE_TOWER)){
			m_pCRole->SendErrorMsg(ERRNO_MSG_DOLLAR_CONSUME_FAIL,CMD_SC_SHOP_BUY_DEL_CD_RESULT);
			return;
		}

		//更改随机塔属性
		m_pCRole->getCTower()->handlerTowerRefreshAttribute();

		//发送更新角色元宝
		m_pCRole->SendProUpdate();
	}
	else
	{
		IME_ERROR("shopid %u is not valid shop", shopid);
		m_pCRole->SendErrorMsg(ERRNO_MSG_DATA_ILLEGAL, CMD_SC_SHOP_BUY_DEL_CD_RESULT);
		return;
	}

	WorldPacket info(CMD_SC_SHOP_BUY_DEL_CD_RESULT);
	info << (uint16_t) 0;
	info << shopid; 
	info << para1;
	info << para2;
	m_pCRole->SendPacket(&info);
	return;
}

bool CShopEffect::IsFullCardPacketGift(CRole *pCRole, uint32_t giftid)
{
	std::vector<STC_CONF_GIFT_COLLECTION*> *vectmp = CConfGiftCollection::Find(giftid);
	if (vectmp == NULL)
	{
		IME_ERROR("index %u can not in gift collection", giftid);
		return true;
	}
	uint32_t result_number = 0;

	for (int i = 0; i < vectmp->size(); i++)
	{
		uint32_t number = 0;
		STC_CONF_GIFT_COLLECTION *pConf = (*vectmp)[i];
		SShopItem temp;
		std::vector<SShopItem> vec;

		if (pConf->wType1 != 0)
		{
			temp.wType = pConf->wType1;
			temp.dwIndex = pConf->dwProductIndex1;
			temp.dwNumber = pConf->dwNumber1;
			vec.push_back(temp);
		}

		if (pConf->wType2 != 0)
		{
			temp.wType = pConf->wType2;
			temp.dwIndex = pConf->dwProductIndex2;
			temp.dwNumber = pConf->dwNumber2;
			vec.push_back(temp);
		}

		if (pConf->wType3 != 0)
		{
			temp.wType = pConf->wType3;
			temp.dwIndex = pConf->dwProductIndex3;
			temp.dwNumber = pConf->dwNumber3;
			vec.push_back(temp);
		}

		if (pConf->wType4 != 0)
		{
			temp.wType = pConf->wType4;
			temp.dwIndex = pConf->dwProductIndex4;
			temp.dwNumber = pConf->dwNumber4;
			vec.push_back(temp);
		}

		for (int i = 0; i < vec.size(); i++)
		{
			if (vec[i].wType == E_OBJ_CARD)
			{
				number += vec[i].dwNumber;
			}
			else if (vec[i].wType == E_OBJ_CARD_MORE)
			{
				number += vec[i].dwNumber;
			}
		}

		if (number > result_number) result_number = number;
	}
	
	return pCRole->GetclsCardPacket()->IsFull(result_number, 0);
}

int CShopEffect::GiveGameProductSpecial(CRole *pCRole, uint16_t type, uint32_t index, uint32_t number, SGiveProduct &Rel, int source)
{
	if (type == E_OBJ_CARD_MORE)
	{
		for (int j = 0; j < number; j++)
		{
			uint32_t objid,opt;
			STC_CONF_CARD_BASE *pConf = CConfCardBase::Find(index);
			if (pConf == NULL)
			{
				IME_ERROR("can not find this card index %u", index);
				return -1;
			}
			pCRole->GetclsCardPacket()->CardAddByPara(index, objid, opt, source); 
			if (opt)
				pCRole->GetclsCardPacket()->SingleCardAddUpdate(objid);
		}
		Rel.effect = E_EFFECT_CARD_MORE;
		Rel.para1 = index;
		Rel.para2 = number;	
		return 0;
	}
	else
	{
		return GiveGameProduct(pCRole, type, index, number, Rel, source);
	}
}	

int CShopEffect::GiveGameProduct(CRole *pCRole, uint16_t type, uint32_t index, uint32_t number, SGiveProduct &Rel, int source)
{
	Rel.para1 = index;
	Rel.para2 = number;
	int32_t para1 = (int32_t) index;
	int32_t para2 = (int32_t) number;
	switch(type)
	{
		case 0:
			break;
		case E_OBJ_CARD:
			{
//			for (int j = 0; j < number; j++)
//			{
				uint32_t objid,opt;
				STC_CONF_CARD_BASE *pConf = CConfCardBase::Find(index);
				if (pConf == NULL)
				{
					IME_ERROR("can not find this card index %u", index);
					return -1;
				}
//				if (pConf->byType == E_ITEM_SKILL_CARD)
//				{
//					IME_ERROR("type is not unvalid E_OBJ_CARD, send a skill card should use E_OBJ_SKILL");
//					return -1;
//				}
				pCRole->GetclsCardPacket()->CardAddByPara(index, objid, opt, source); 
				if (opt)
				pCRole->GetclsCardPacket()->SingleCardAddUpdate(objid);
				Rel.para1 = objid;
//			}
			}
			Rel.effect = E_EFFECT_CARD;
			break;
		case E_OBJ_ITEM:
//			for (int j = 0; j < number; j++)
//			{
				pCRole->GetclsItemPacket()->AddItem(index, source, number);
//				if (number < 10)
//				{
					pCRole->GetclsItemPacket()->ItemUpdate(index, CItem::ADD, number);
//				}
//			}
//			if (number >= 10)
//				pCRole->GetclsItemPacket()->HandlerItemInfo();
			Rel.effect = E_EFFECT_ITEM;
			break;
		case E_OBJ_SHOP:
			{
				if (!sWorld->IsAllowDrop(index, number))
				{
					type = GlobalConfig::DefaultDropType;
					index = GlobalConfig::DefaultDropPara1;
					number = GlobalConfig::DefaultDropPara2;
					GiveGameProduct(pCRole, type, index, number, Rel, source);
					break;
				}
				STC_CONF_GOODS *pConf = CConfGoods::Find(index);
				if (pConf != NULL)
				{
//					for (int j = 0; j < number; j++)
//					{
						pCRole->GetCShop()->AddItem(index, source, number);
//					}
				}
				else
				{
					IME_ERROR("goods can not find this goods %u", index);
					return -1;
				}
				pCRole->GetCShop()->HandlerShopInfo(index);
				Rel.effect = E_EFFECT_SHOP;
				sWorld->AddDropItem(index, number);
			}
			break;
		case E_OBJ_COIN:
			pCRole->ChangeCoin(number, source);
			pCRole->SendProUpdate();
			Rel.effect = E_EFFECT_COIN;
			Rel.para1 = number;
			break;
		case E_OBJ_DOLLAR:
			{
				uint32_t order_id;
				if (!CDBCtrl::GetShopOrderId(pCRole->GetdwAccountId(), order_id))
				{
					IME_ERROR("user %u give dollar %u fail in EggEffect", pCRole->GetdwAccountId(), number);
					return -1;
				}
				
				int32_t getdollar = number;
				pCRole->ChangeDollarWithExtra(getdollar, source);
				pCRole->SendProUpdate();
				pCRole->HandlerInfoOpt();
				CDBCtrl::LogShopRecord(pCRole->GetdwAccountId(), order_id, number, 0, 0, E_DC_GIVE);	
			
			Rel.para1 = getdollar;
			Rel.effect = E_EFFECT_DOLLAR;
			}
			break;
		case E_OBJ_CARD_EXP:
			{
				std::vector<CCard *> vec;
				if (pCRole->GetclsCardPacket()->GetEquippCards(vec))
				{
					IME_ERROR("get card property fail, userid %u", pCRole->GetdwAccountId());
					return -1;
				}
				for (int i = 0; i < vec.size(); i++)
				{
					if (pCRole->GetwLevel() * STRENGTH_LEVEL_LIMIT_PARA > vec[i]->GetwLevel())
					{
						vec[i]->AddExp(number);
						vec[i]->Calculate(true);
					}
				}
			}
			Rel.effect = E_EFFECT_CARD_EXP;
			Rel.para1 = number;
			break;
		case E_OBJ_SKILL:
			{
				uint32_t objid, opt;
				pCRole->GetclsCardPacket()->CardAddByPara(index, objid, opt, source); 
				if (opt)
				{
					CCard *pCard = pCRole->GetclsCardPacket()->Find(objid);
					if (pCard == NULL)
					{
						IME_ERROR("can not find this card %u", objid);
						return -1;
					}
					if (number != 0)
					{
						pCard->SetRealLv(number);
//						pCard->SetwLevel(number);
					}
					pCRole->GetclsCardPacket()->SingleCardAddUpdate(objid);
				}
				Rel.para1 = objid;
			}
			Rel.effect = E_EFFECT_SKILL;
			break;
		case E_OBJ_AWAKE_CARD:
			{
				uint32_t objid, opt;
				pCRole->GetclsCardPacket()->CardAddByParaNoAutoAwake(index, objid, opt, source, true, number); 
				if (opt)
				{
					CCard *pCard = pCRole->GetclsCardPacket()->Find(objid);
					if (pCard == NULL)
					{
						IME_ERROR("can not find this card %u", objid);
						return -1;
					}
					pCRole->GetclsCardPacket()->SingleCardAddUpdate(objid);
				}
				Rel.para1 = objid;
			}
			Rel.effect = E_EFFECT_AWAKE_CARD;
			break;
		case E_OBJ_WORLD_EXP:
			{
				pCRole->SetdwWorldHistory(pCRole->GetdwWorldHistory() + number);
				pCRole->SendProUpdate();
				Rel.effect = E_EFFECT_WORLD_EXP;
				Rel.para1 = number;
				Rel.para2 = 0;
			}
			break;
		case E_OBJ_ROLE_EXP:
			{
				pCRole->AddExp(para2);
				pCRole->Calculate();
				pCRole->SendProUpdate();
				pCRole->HandlerInfoOpt();
				Rel.effect = E_EFFECT_ROLE_EXP;
				Rel.para1 = para2;
				Rel.para2 = 0;
			}
			break;
		case E_OBJ_ENERGY:
			{
				pCRole->ChangeEnergy(para2);
				pCRole->Calculate();
				pCRole->SendProUpdate();
				pCRole->HandlerInfoOpt();
				Rel.effect = E_EFFECT_ENERGY;
				Rel.para1 = para2;
				Rel.para2 = 0;
			}
			break;
		case E_OBJ_STRENGTH:
			{
				pCRole->ChangeStrength(para2);
				pCRole->Calculate();
				pCRole->SendProUpdate();
				pCRole->HandlerInfoOpt();
				Rel.effect = E_EFFECT_STRENGTH;
				Rel.para1 = para2;
				Rel.para2 = 0;
			}
			break;
		case E_OBJ_STAMINA:
			{
				pCRole->ChangeStamina(para2);
				pCRole->Calculate();
				pCRole->SendProUpdate();
				pCRole->HandlerInfoOpt();
				Rel.effect = E_EFFECT_STAMINA;
				Rel.para1 = para2;
				Rel.para2 = 0;
			}
			break;
		case E_OBJ_VIP_EXP:
			{
				pCRole->ChangeVipExp(para2);
				uint16_t viplv = pCRole->GetwVipLv();
				pCRole->Calculate(true);
				pCRole->HandlerInfoOpt();
				IME_ERROR("new vip lv %u, lv %u", pCRole->GetwVipLv(), viplv); 
				if (pCRole->GetwVipLv() != viplv)
				{
					CDataPush push;
					push.HandlerDataPushVip(pCRole, pCRole->GetwVipLv());
				}
				Rel.effect = E_EFFECT_VIP_EXP;
				Rel.para1 = para2;
				Rel.para2 = 0; 
			}
			break;
		case E_OBJ_COL_CARD_EXP:
			{
				int exp = CUtil::RandByRange(para1, para2 + 1);
				if (exp < 0)
					exp = 0;
				std::vector<CCard *> vec;
				if (pCRole->GetclsCardPacket()->GetEquippCards(vec))
				{
					IME_ERROR("get card property fail, userid %u", pCRole->GetdwAccountId());
					return -1;
				}
				for (int i = 0; i < vec.size(); i++)
				{
					if (pCRole->GetwLevel() * STRENGTH_LEVEL_LIMIT_PARA > vec[i]->GetwLevel())
					{
						vec[i]->AddExp(exp);
						vec[i]->Calculate(true);
					}
				}
				pCRole->SendProUpdate();
				Rel.effect = E_EFFECT_COL_CARD_EXP;
				Rel.para1 = exp;
				Rel.para2 = 0;
			}
			break;
		case E_OBJ_COL_COIN:
			{
				int coin = CUtil::RandByRange(para1, para2 + 1); 
				if (coin < 0)
					coin = 0;
				pCRole->ChangeCoin(coin, source);
				pCRole->SendProUpdate();
				Rel.effect = E_EFFECT_COL_COIN;
				Rel.para1 = coin;
				Rel.para2 = 0;
			}
			break;
		case E_OBJ_COL_CARD:
			{
				std::vector<int> vecrate;
				std::vector<STC_CONF_CARD_COLLECTION *> *p;
				p = CConfCardCollection::Find(para1);
				if (p->size() == 0)
				{
					IME_ERROR("can not find cardcollection index %u", para1);
					return -1;
				}
				for (int i = 0; i < p->size(); i++)
				{
					vecrate.push_back((*p)[i]->byWeight);
				}	
				int rel;
				if ((rel = CUtil::RandFactor(vecrate)) == -1)
				{
					return -1;
				}
				if ((*p)[rel]->dwCardindex == 0)
				{
					return -1;
				}
				uint32_t cardindex = (*p)[rel]->dwCardindex;
				uint32_t objid, opt;
				pCRole->GetclsCardPacket()->CardAddByPara(cardindex, objid, opt, source); 
				if (opt)
					pCRole->GetclsCardPacket()->SingleCardAddUpdate(objid);
				Rel.effect = E_EFFECT_CARD;
				Rel.para1 = objid;
				Rel.para2 = 0;

				if (objid != 0)
				{
					std::string targetid = CUtil::Uint32ToString(cardindex);
					if (GlobalConfig::NoticeSpecificCard.find(targetid) != std::string::npos)
					{
						std::string m = (*p)[rel]->strContent;
						m = CUtil::StrReplace(m, "%s", pCRole->GetstrUserName());
						if (m != "0")
						{
							sWorld->GetCNotice()->DiyNotice(m);
						}
						else
						{
							IME_ERROR("notice content is 0 index %u", (*p)[rel]->dwIndex);
						}
					}
				}
			}
			break;
		case E_OBJ_COL_ITEM:
			{
				std::vector<int> vecrate;
				std::vector<STC_CONF_ITEM_COLLECTION *> *p;
				p = CConfItemCollection::Find(para1);
				if (p->size() == 0)
				{
					IME_ERROR("can not find itemcollection index %u", para1);
					return -1;
				}
				for (int i = 0; i < p->size(); i++)
				{
					vecrate.push_back((*p)[i]->byWeight);
				}	
				int rel;
				if ((rel = CUtil::RandFactor(vecrate)) == -1)
				{
					return -1;
				}
				uint32_t itemindex = (*p)[rel]->dwItemIndex;
				pCRole->GetclsItemPacket()->AddItem(itemindex, source); 
				pCRole->GetclsItemPacket()->ItemUpdate(itemindex, CItem::ADD);
				Rel.effect = E_EFFECT_COL_ITEM;
				Rel.para1 = itemindex;
				Rel.para2 = 1;
			}
			break;
		case E_OBJ_COL_SHOP:
			{
				std::vector<int> vecrate;
				std::vector<STC_CONF_GOODS_COLLECTION *> *p;
				p = CConfGoodsCollection::Find(para1);
				if (p->size() == 0)
				{
					IME_ERROR("can not find shopcollection index %u", para1);
					return -1;
				}
				for (int i = 0; i < p->size(); i++)
				{
					vecrate.push_back((*p)[i]->byWeight);
				}	
				int rel;
				uint32_t shopid = 0;
				uint32_t shopnumber = 0;
				
				if ((rel = CUtil::RandFactor(vecrate)) == -1)
				{
					return -1;
				}
				shopid = (*p)[rel]->dwItemIndex;
				shopnumber = (*p)[rel]->dwNumber;
				if (!sWorld->IsAllowDrop(shopid, shopnumber))
				{
					type = GlobalConfig::DefaultDropType;
					index = GlobalConfig::DefaultDropPara1;
					number = GlobalConfig::DefaultDropPara2;
					GiveGameProduct(pCRole, type, index, number, Rel, source);
					break;
				}

				STC_CONF_GOODS *pConfGoods = CConfGoods::Find(shopid);
				if (pConfGoods == NULL)
				{
					IME_ERROR("can not find shop index %u", shopid);
					return -1;
				}
//				for (int i = 0; i < (*p)[rel]->dwNumber; i++)
//				{
					pCRole->GetCShop()->AddItem(shopid, source, shopnumber); 
//				}
				pCRole->GetCShop()->HandlerShopInfo(shopid);

				Rel.effect = E_EFFECT_SHOP;
				Rel.para1 = shopid;
				Rel.para2 = shopnumber;

				std::string targetid = CUtil::Uint32ToString(shopid);
				if (GlobalConfig::NoticeSpecificItem.find(targetid) != std::string::npos)
				{
					std::string m = (*p)[rel]->strContent;
					m = CUtil::StrReplace(m, "%s", pCRole->GetstrUserName());
					if (m != "0")
					{
						sWorld->GetCNotice()->DiyNotice(m);
					}
					else
					{
						IME_ERROR("notice content is 0 index %u", (*p)[rel]->dwIndex);
					}
				}
				sWorld->AddDropItem(shopid, shopnumber);
			}
			break;
		case E_OBJ_COL_DOLLAR:
			{
				uint32_t order_id;
				if (!CDBCtrl::GetShopOrderId(pCRole->GetdwAccountId(), order_id))
				{
					IME_ERROR("user %u give dollar %u fail in EggEffect", pCRole->GetdwAccountId(), number);
					return -1;
				}

				int dollar = CUtil::RandByRange(para1, para2 + 1); 
				if (dollar < 0)
					dollar = 0;

				pCRole->ChangeDollarWithExtra(dollar, source);
				pCRole->SendProUpdate();
				pCRole->HandlerInfoOpt();
				CDBCtrl::LogShopRecord(pCRole->GetdwAccountId(), order_id, dollar, 0, 0, E_DC_GIVE);	
				Rel.effect = E_EFFECT_DOLLAR;
				Rel.para1 = dollar;
				Rel.para2 = 0;
			}
			break;
		case E_OBJ_COL_WORLD_EXP:
			{
				int exp = CUtil::RandByRange(para1, para2 + 1); 
				if (exp < 0)
					exp = 0;
				pCRole->SetdwWorldHistory(pCRole->GetdwWorldHistory() + exp);
				pCRole->SendProUpdate();
				Rel.effect = E_EFFECT_COL_WORLD_EXP;
				Rel.para1 = exp;
				Rel.para2 = 0;
			}
			break;
		case E_OBJ_COL_CARD_PLUS:
			{
				std::vector<int> vecrate;
				std::vector<STC_CONF_CARD_COLLECTION *> *p;
				p = CConfCardCollection::Find(para1);
				if (p->size() == 0)
				{
					IME_ERROR("can not find cardcollection index %u", para1);
					return -1;
				}
				for (int i = 0; i < p->size(); i++)
				{
					vecrate.push_back((*p)[i]->byWeight);
				}	
				int rel;
				if ((rel = CUtil::RandFactor(vecrate)) == -1)
				{
					return -1;
				}
				if ((*p)[rel]->dwCardindex == 0)
				{
					return -1;
				}
				uint32_t cardindex = (*p)[rel]->dwCardindex;
				uint32_t objid, opt;
				pCRole->GetclsCardPacket()->CardAddByParaWithRandPlus(cardindex, objid, para2, source); 
				if (opt)
					pCRole->GetclsCardPacket()->SingleCardAddUpdate(objid);
				Rel.effect = E_EFFECT_COL_CARD_PLUS;
				Rel.para1 = objid;
				Rel.para2 = 0;
			}
			break;
		default:
			IME_ERROR("unvalid type %u", type);
			return 0;
	}
	return 0;
}

int CShopEffect::BoxEffect(CRole *pCRole, SEffect &data)
{
	STC_CONF_UNIQUE_COLLECTION* pRand = CConfUniqueCollection::GetRandResult(data.para2);
	if (pRand == NULL)
	{
		IME_ERROR("box rand index %u is not in collection",data.para2);
		return -1;
	}

	SGiveProduct tmp;
	CShopEffect::GiveGameProduct(pCRole, pRand->byType, pRand->dwPara1, pRand->dwPara2, tmp, SOURCE_UNIQUE_GIFT);
	data.para1 = tmp.effect;
	data.para2 = tmp.para1;
	data.para3 = tmp.para2;
	return 0;
}

int CShopEffect::KeyEffect(CRole *pCRole, SEffect &data)
{
	STC_CONF_UNIQUE_COLLECTION* pRand = CConfUniqueCollection::GetRandResult(data.para2);
	if (pRand == NULL)
	{
		IME_ERROR("box rand index %u is not in collection",data.para2);
		return -1;
	}

	SGiveProduct tmp;
	CShopEffect::GiveGameProduct(pCRole, pRand->byType, pRand->dwPara1, pRand->dwPara2, tmp, SOURCE_UNIQUE_GIFT);
	data.para1 = tmp.effect;
	data.para2 = tmp.para1;
	data.para3 = tmp.para2;
	return 0;
}

int CShopEffect::MasterEffect(CRole *pCRole, SEffect &data)
{
	if (data.para1 < 1 || data.para1 > 3)
	{
		IME_ERROR("para unvalid must 1-3 now %u", data.para1);
		return -1;
	}
	uint32_t master = 0;
	uint32_t flag = 0;
	pCRole->GetCMaster()->RequestByShop(data.para1, master, flag);
	if (master == 0 )
	{
		IME_ERROR("request master fail choose %u", data.para1);
		return -1;
	}
	data.para2 = master;
	data.para3 = flag;	
	return 0;
}

int CShopEffect::ChipEffect(CRole *pCRole, SEffect &data)
{
//	if (data.para1 > 0)
//	{
//		IME_ERROR("para unvalid must bigger than 1 now %u", data.para1);
//		return -1;
//	}

	SGiveProduct tmp;
	CShopEffect::GiveGameProduct(pCRole, data.para2, data.para3, data.para4, tmp, SOURCE_CHIP);
	data.para1 = tmp.effect;
	data.para2 = tmp.para1;
	data.para3 = tmp.para2;
	return 0;
}

void CShopEffect::HandlerGacha(CRole *pCRole, WorldPacket &pkg)
{
	uint8_t action;
	pkg >> action;
	uint32_t order_id;
	uint32_t objid = 0;
	uint32_t opt;
	IME_CHAR_LOG("action [userid %u single gacha]", pCRole->GetdwAccountId());
//	IME_ERROR("***********gacha first %u second %u", pCRole->IsFirstGacha(), pCRole->IsSecondGacha()); 

	STC_CONF_GACHA tmp;
	STC_CONF_CARD_BASE *pConfCardBase = NULL;
	if (action == 0) //gacha by hero item
	{
		if (pCRole->GetclsCardPacket()->IsFull(1, 0))
		{
			IME_ERROR("card capacity is full");
			pCRole->SendErrorMsg(ERRNO_MSG_CARD_PACKET_NOT_ENOUGH, CMD_SC_GACHA_RESULT);
			return;
		}

		if (pCRole->GetCShop()->IsHave(HERO_REQUEST_SHOPID, HERO_REQUEST_SHOPID_COUNT_EVERY))
		{
			STC_CONF_GOODS *pConfShop = CConfGoods::Find(HERO_REQUEST_SHOPID);
			if (pConfShop == NULL)
			{
				IME_ERROR("can not find this index %u in shop csv", HERO_REQUEST_SHOPID);
				pCRole->SendErrorMsg(ERRNO_MSG_CSV_LOAD, CMD_SC_GACHA_RESULT);
				return;
			}
			if (pConfShop->dwLevelLimit != 0)
			{
				if (pCRole->GetwLevel() < pConfShop->dwLevelLimit)
				{
					pCRole->SendErrorMsg(ERRNO_MSG_SHOP_LEVEL_LIMIT, CMD_SC_GACHA_RESULT);
					return;
				}
			}

			STC_CONF_GACHA *pConf = NULL;
			std::vector<uint32_t> vecCardIndex;
			uint32_t mastercardindex = 0;
			pCRole->GetclsCardPacket()->GetCardIndex(mastercardindex, vecCardIndex);
			if (mastercardindex != 0)
				vecCardIndex.push_back(mastercardindex);

			if (!(pCRole->IsFirstGacha()))
			{
				pConf = CConfGacha::GetByFirstGacha(pCRole->GetdwLuck(), vecCardIndex);	
			}
			else
			{
				if (!pCRole->IsSecondGacha())
				{
					tmp.dwIndex = GlobalConfig::SecondChargeCardIndex;
					tmp.dwDelLuck = 0;
					pConf = &tmp;
				}
				else
				{
					pConf = CConfGacha::GetByItem(pCRole->GetdwLuck());	
				}
			}
			
			if (pConf != NULL)
			{
				pConfCardBase = CConfCardBase::Find(pConf->dwIndex);
				if (pConfCardBase == NULL)
				{
					IME_ERROR("can not find card index %u in card base csv", pConf->dwIndex);
					pCRole->SendErrorMsg(ERRNO_MSG_CSV_DATA, CMD_SC_GACHA_RESULT);
					return;
				}
//				pCRole->GetclsCardPacket()->CardAddByPara(pConf->dwIndex, objid, opt, SOURCE_GACHA, true);
				pCRole->GetclsCardPacket()->CardAddByParaWithRandPlus(pConf->dwIndex, objid, GlobalConfig::HeroGachaPro, SOURCE_HERO_GACHA, true);
//				if (opt)
				pCRole->GetclsCardPacket()->SingleCardAddUpdate(objid);
				pCRole->SetdwLuck(pCRole->GetdwLuck() - pConf->dwDelLuck);
			}
			else
			{
				IME_ERROR("can not get card");
				pCRole->SendErrorMsg(ERRNO_MSG_CSV_DATA, CMD_SC_GACHA_RESULT);
				return;
			}

			for (int i = 0; i < HERO_REQUEST_SHOPID_COUNT_EVERY; i++)
			{
				pCRole->GetCShop()->DelItem(HERO_REQUEST_SHOPID, SOURCE_ITEM_USE);
			}
			pCRole->GetCShop()->HandlerShopInfo(HERO_REQUEST_SHOPID);

		}
		else
		{
			if (!CDBCtrl::GetShopOrderId(pCRole->GetdwAccountId(), order_id))
			{
				IME_ERROR("get order id fail userid %u", pCRole->GetdwAccountId());
				pCRole->SendErrorMsg(ERRNO_MSG_SHOP_GET_ORDER_ID, CMD_SC_GACHA_RESULT);
				return;
			}

			if (pCRole->GetdwDollar() < GlobalConfig::DollarGacha)
			{
				IME_LOG("user dollar is not enough now %u need %u", pCRole->GetdwDollar(), GlobalConfig::DollarGacha);
				pCRole->SendErrorMsg(ERRNO_MSG_DOLLAR_NOT_ENGOUH, CMD_SC_GACHA_RESULT);
				return;	
			}
			
			STC_CONF_GACHA *pConf = NULL;
			uint8_t choose = 0;
			std::vector<uint32_t> vecCardIndex;
			uint32_t mastercardindex = 0;
			pCRole->GetclsCardPacket()->GetCardIndex(mastercardindex, vecCardIndex);
			if (mastercardindex != 0)
				vecCardIndex.push_back(mastercardindex);

			if (!(pCRole->IsFirstGacha()))
			{
				pConf = CConfGacha::GetByFirstGacha(pCRole->GetdwLuck(), vecCardIndex);	
			}
			else
			{
				if (!(pCRole->IsSecondGacha()))
				{
					tmp.dwIndex = GlobalConfig::SecondChargeCardIndex;
					tmp.dwDelLuck = 0;
					pConf = &tmp;
				}
				else
				{
					pConf = SelectGacha(pCRole, choose);
				}
			}
//			IME_ERROR("gacha first %u second %u, index %u", pCRole->IsFirstGacha(), pCRole->IsSecondGacha(), pConf->dwIndex); 

			if (pConf != NULL)
			{
				pConfCardBase = CConfCardBase::Find(pConf->dwIndex);
				if (pConfCardBase == NULL)
				{
					IME_ERROR("can not find card index %u in card base csv", pConf->dwIndex);
					pCRole->SendErrorMsg(ERRNO_MSG_CSV_DATA, CMD_SC_GACHA_RESULT);
					return;
				}

//				pCRole->GetclsCardPacket()->CardAddByPara(pConf->dwIndex, objid, opt, SOURCE_GACHA, true);
				pCRole->GetclsCardPacket()->CardAddByParaWithRandPlus(pConf->dwIndex, objid, GlobalConfig::DollarGachaPro, SOURCE_DOLLAR_GACHA, true);
//				if (opt)
					pCRole->GetclsCardPacket()->SingleCardAddUpdate(objid);
				SelectGachaOpt(pCRole, pConf, choose);
			}
			else
			{
				IME_ERROR("can not get card");
				pCRole->SendErrorMsg(ERRNO_MSG_CSV_DATA, CMD_SC_GACHA_RESULT);
				return;
			}

			if (pCRole->ChangeDollar(-GlobalConfig::DollarGacha, SOURCE_DOLLAR_GACHA))
			{
				IME_ERROR("del dollar fail");
				pCRole->SendErrorMsg(ERRNO_MSG_SHOP_DEL_DOLLAR, CMD_SC_GACHA_RESULT);
				return;
			}
			pCRole->SendProUpdate();
			CDBCtrl::LogShopRecord(pCRole->GetdwAccountId(), order_id, GlobalConfig::DollarGacha, 1, 1, E_DC_GACHA);

		}
//		IME_ERROR("gacha first %u second %u", pCRole->IsFirstGacha(), pCRole->IsSecondGacha()); 
		if (!(pCRole->IsFirstGacha()))
		{
			pCRole->SetFirstGacha(1);
		}
		else
		{
			if (!(pCRole->IsSecondGacha()))
				pCRole->SetSecondGacha(1);
		}
//		IME_ERROR("gacha first %u second %u", pCRole->IsFirstGacha(), pCRole->IsSecondGacha()); 


		if (pConfCardBase->byStarLevel >= 5)
		{
			sWorld->GetCNotice()->AddUseEgg(pCRole->GetstrUserName(), pConfCardBase->dwIndex);
		}

		if (pConfCardBase->byStarLevel == 4)
		{
			pCRole->GetCMission()->GetFourCard(0, 1);
		}
		if (pConfCardBase->byStarLevel == 5)
		{
			pCRole->GetCMission()->GetFiveCard(0, 1);
		}

		WorldPacket info(CMD_SC_GACHA_RESULT);
		info << (uint16_t) 0;
		info << action;
		info << (uint8_t) 1;
		info << (uint8_t)opt;
		info << objid;
		pCRole->SendPacket(&info);
		pCRole->GetCMission()->EveryGacha(0,1);
		return;
	}
	else // gacha by world exp
	{
		if (pCRole->GetclsCardPacket()->IsFull(GACHA_ONE_TIMES + 1, 0))
		{
			IME_ERROR("card capacity is full");
			pCRole->SendErrorMsg(ERRNO_MSG_CARD_PACKET_NOT_ENOUGH, CMD_SC_GACHA_RESULT);
			return;
		}
	
		if (pCRole->GetdwWorldHistory() < GlobalConfig::NormalGacha)
		{
			IME_LOG("user world history is not enough now %u need %u", pCRole->GetdwWorldHistory(), GlobalConfig::NormalGacha);
			pCRole->SendErrorMsg(ERRNO_MSG_NOT_ENOUGH_WORLD_EXP, CMD_SC_GACHA_RESULT);
			return;
		}

		uint8_t TenSeq = 0;
		if (pCRole->GetdwWorldHistory() > GlobalConfig::NormalGacha * GACHA_ONE_TIMES)
		{
			TenSeq = 1;
		}
		std::vector<STC_CONF_GACHA *> vecRes;
		std::vector<uint32_t> vecObj;
		std::vector<uint8_t> vecOpt;
		uint32_t IsTujian = 0;

		if (TenSeq)
		{
			for (int i = 0; i < GACHA_ONE_TIMES + 1; i++)
			{
				STC_CONF_GACHA *pConf = CConfGacha::GetByNormal(pCRole->GetdwLuck());	
				if (pConf != NULL)
				{
					pConfCardBase = CConfCardBase::Find(pConf->dwIndex);
					if (pConfCardBase == NULL)
					{
						IME_ERROR("can not find card index %u in card base csv", pConf->dwIndex);
					}
					else
					{
						if (pConfCardBase->byStarLevel >= 5)
						{
							sWorld->GetCNotice()->AddUseEgg(pCRole->GetstrUserName(), pConfCardBase->dwIndex);
						}
					}

					if (pCRole->GetclsTujian()->IsTujianGet(pConf->dwIndex))
						IsTujian = 0;
					else
						IsTujian = 1;

//					pCRole->GetclsCardPacket()->CardAddByPara(pConf->dwIndex, objid, opt, SOURCE_GACHA, true);
					pCRole->GetclsCardPacket()->CardAddByParaWithRandPlus(pConf->dwIndex, objid, GlobalConfig::WorldHistoryGachaPro, SOURCE_WORLD_GACHA, true);
//					if (opt)
						pCRole->GetclsCardPacket()->SingleCardAddUpdate(objid);
					vecObj.push_back(objid);
					vecOpt.push_back(IsTujian);
					vecRes.push_back(pConf);
					pCRole->SetdwLuck(pCRole->GetdwLuck() - pConf->dwDelLuck);
				}
				else
				{
					IME_ERROR("can not get card");
					pCRole->SendErrorMsg(ERRNO_MSG_CSV_DATA, CMD_SC_GACHA_RESULT);
					return;
				}
			}

			pCRole->SetdwWorldHistory(pCRole->GetdwWorldHistory() - GlobalConfig::NormalGacha * GACHA_ONE_TIMES, true);
		}
		else
		{
			STC_CONF_GACHA *pConf = CConfGacha::GetByNormal(pCRole->GetdwLuck());	
			if (pConf != NULL)
			{
//				pCRole->GetclsCardPacket()->CardAddByPara(pConf->dwIndex, objid, opt, SOURCE_GACHA, true);
				pCRole->GetclsCardPacket()->CardAddByParaWithRandPlus(pConf->dwIndex, objid, GlobalConfig::WorldHistoryGachaPro, SOURCE_WORLD_GACHA, true);
//				if (opt)
					pCRole->GetclsCardPacket()->SingleCardAddUpdate(objid);

				if (pCRole->GetclsTujian()->IsTujianGet(pConf->dwIndex))
					IsTujian = 0;
				else
					IsTujian = 1;

				vecObj.push_back(objid);
				vecOpt.push_back(IsTujian);
				vecRes.push_back(pConf);
				pCRole->SetdwLuck(pCRole->GetdwLuck() - pConf->dwDelLuck);
			}
			else
			{
				IME_ERROR("can not get card");
				pCRole->SendErrorMsg(ERRNO_MSG_CSV_DATA, CMD_SC_GACHA_RESULT);
				return;
			}
			pCRole->SetdwWorldHistory(pCRole->GetdwWorldHistory() - GlobalConfig::NormalGacha, true);
			pConfCardBase = CConfCardBase::Find(pConf->dwIndex);
			if (pConfCardBase == NULL)
			{
				IME_ERROR("can not find card index %u in card base csv", pConf->dwIndex);
				pCRole->SendErrorMsg(ERRNO_MSG_CSV_DATA, CMD_SC_GACHA_RESULT);
				return;
			}
			if (pConfCardBase->byStarLevel >= 5)
			{
				sWorld->GetCNotice()->AddUseEgg(pCRole->GetstrUserName(), pConfCardBase->dwIndex);
			}
		}

		WorldPacket info(CMD_SC_GACHA_RESULT);
		info << (uint16_t) 0;
		info << action;
		info << (uint8_t) vecObj.size();
		for (int i = 0; i < vecObj.size(); i++)
		{
			info << vecOpt[i];
			info << vecObj[i];
		}
		pCRole->SendPacket(&info);
	}
	return;
}

void CShopEffect::HandlerTenGacha(CRole *pCRole, WorldPacket &pkg)
{
	uint32_t order_id;
	IME_CHAR_LOG("action [userid %u ten gacha]", pCRole->GetdwAccountId());
	if (!CDBCtrl::GetShopOrderId(pCRole->GetdwAccountId(), order_id))
	{
		IME_ERROR("get order id fail userid %u", pCRole->GetdwAccountId());
		pCRole->SendErrorMsg(ERRNO_MSG_SHOP_GET_ORDER_ID, CMD_SC_TEN_GACHA_RESULT);
		return;
	}

	if (pCRole->GetdwDollar() < GlobalConfig::DollarGacha * GACHA_ONE_TIMES)
	{
		IME_LOG("user dollar is not enough now %u need %u", pCRole->GetdwDollar(), GlobalConfig::DollarGacha * GACHA_ONE_TIMES);
		pCRole->SendErrorMsg(ERRNO_MSG_DOLLAR_NOT_ENGOUH, CMD_SC_TEN_GACHA_RESULT);
		return;	
	}
	
	if (pCRole->GetclsCardPacket()->IsFull(GACHA_ONE_TIMES + 1, 0))
	{
		IME_ERROR("card capacity is full");
		pCRole->SendErrorMsg(ERRNO_MSG_CARD_PACKET_NOT_ENOUGH, CMD_SC_TEN_GACHA_RESULT);
		return;
	}

	std::vector<STC_CONF_GACHA *> vecRes;
	std::vector<uint32_t> vecObj;
	std::vector<uint8_t> vecOpt;
	uint32_t opt;
	uint32_t objid;
	uint32_t IsTujian = 0;
	STC_CONF_CARD_BASE *pConfCardBase = NULL;

	uint16_t step4number = 0;
	uint16_t step5number = 0;
	for (int i = 0; i < GACHA_ONE_TIMES; i++)
	{
//		STC_CONF_GACHA *pConf = CConfGacha::GetByDollar(pCRole->GetdwLuck());	
		uint8_t choose = 0;
		STC_CONF_GACHA *pConf = SelectGacha(pCRole, choose);
		if (pConf != NULL)
		{
			pConfCardBase = CConfCardBase::Find(pConf->dwIndex);
			if (pConfCardBase == NULL)
			{
				IME_ERROR("can not find card index %u in card base csv", pConf->dwIndex);
				pCRole->SendErrorMsg(ERRNO_MSG_CSV_DATA, CMD_SC_TEN_GACHA_RESULT);
				return;
			}
			else
			{
				if (pConfCardBase->byStarLevel >= 5)
				{
					sWorld->GetCNotice()->AddUseEgg(pCRole->GetstrUserName(), pConfCardBase->dwIndex);
				}
			}

			if (pCRole->GetclsTujian()->IsTujianGet(pConf->dwIndex))
				IsTujian = 0;
			else
				IsTujian = 1;

			pCRole->GetclsCardPacket()->CardAddByPara(pConf->dwIndex, objid, opt, SOURCE_DOLLAR_GACHA, true);
			if (opt)
				pCRole->GetclsCardPacket()->SingleCardAddUpdate(objid);
			vecObj.push_back(objid);
			vecOpt.push_back(IsTujian);
			vecRes.push_back(pConf);
//			pCRole->SetdwLuck(pCRole->GetdwLuck() - pConf->dwDelLuck);
			SelectGachaOpt(pCRole, pConf, choose);
		}
		else
		{
			IME_ERROR("can not get card");
			pCRole->SendErrorMsg(ERRNO_MSG_CSV_DATA, CMD_SC_TEN_GACHA_RESULT);
			return;
		}

		if (pConfCardBase->byStarLevel == 4)
		{
			step4number++;
		}
		if (pConfCardBase->byStarLevel == 5)
		{
			step5number++;
		}
	}

	STC_CONF_GACHA *pConf = CConfGacha::GetByFinalGacha(pCRole->GetdwLuck());	
	if (pConf != NULL)
	{
		if (pCRole->GetclsTujian()->IsTujianGet(pConf->dwIndex))
			IsTujian = 0;
		else
			IsTujian = 1;

		pCRole->GetclsCardPacket()->CardAddByPara(pConf->dwIndex, objid, opt, SOURCE_DOLLAR_GACHA, true);
		if (opt)
			pCRole->GetclsCardPacket()->SingleCardAddUpdate(objid);
		vecObj.push_back(objid);
		vecOpt.push_back(IsTujian);
		vecRes.push_back(pConf);
//		pCRole->SetdwLuck(pCRole->GetdwLuck() - pConf->dwDelLuck);
	}
	else
	{
		IME_ERROR("can not get card");
		pCRole->SendErrorMsg(ERRNO_MSG_CSV_DATA, CMD_SC_TEN_GACHA_RESULT);
		return;
	}

	if (pCRole->ChangeDollar(-GlobalConfig::DollarGacha * GACHA_ONE_TIMES, SOURCE_DOLLAR_GACHA))
	{
		IME_ERROR("del dollar fail");
		pCRole->SendErrorMsg(ERRNO_MSG_SHOP_DEL_DOLLAR, CMD_SC_TEN_GACHA_RESULT);
		return;
	}
	pCRole->SendProUpdate();

	CDBCtrl::LogShopRecord(pCRole->GetdwAccountId(), order_id, GlobalConfig::DollarGacha* GACHA_ONE_TIMES, 2, 1, E_DC_GACHA);

	pConfCardBase = CConfCardBase::Find(pConf->dwIndex);
	if (pConfCardBase == NULL)
	{
		IME_ERROR("can not find card index %u in card base csv", pConf->dwIndex);
		pCRole->SendErrorMsg(ERRNO_MSG_CSV_DATA, CMD_SC_TEN_GACHA_RESULT);
		return;
	}
	if (pConfCardBase->byStarLevel >= 5)
	{
		sWorld->GetCNotice()->AddUseEgg(pCRole->GetstrUserName(), pConfCardBase->dwIndex);
	}

	if (pConfCardBase->byStarLevel == 4)
	{
		step4number++;
	}
	if (pConfCardBase->byStarLevel == 5)
	{
		step5number++;
	}

	if (step4number != 0)
	{
		pCRole->GetCMission()->GetFourCard(0, step4number);
	}
	if (step5number != 0)
	{
		pCRole->GetCMission()->GetFiveCard(0, step5number);
	}


	WorldPacket info(CMD_SC_TEN_GACHA_RESULT);
	info << (uint16_t) 0;
	info << (uint8_t) vecRes.size();
//	IME_ERROR("total gacha card number %u", vecRes.size());
	for (int i = 0; i < vecRes.size(); i++)
	{
		info << vecOpt[i];
		info << vecObj[i];
	}
	pCRole->SendPacket(&info);
	return;
}

STC_CONF_GACHA* CShopEffect::SelectGacha(CRole *pCRole, uint8_t &choose)
{
	STC_CONF_GACHA *pConf;
	if (pCRole->GetwVipLv() == 0)
	{
		pConf = CConfGacha::GetByDollar(pCRole->GetdwLuck());
		if (pConf == NULL)
			IME_ERROR("get by dollar is null luck %u", pCRole->GetdwLuck());
		choose = 1;
		return pConf;
	}	
	else
	{
		if (pCRole->GetdwChargeLuck() >= GlobalConfig::VipChargeGachaFloors)
		{
			pConf = CConfGacha::GetByChargeGacha();
			if (pConf == NULL)
				IME_ERROR("get by charge is null");
			choose = 2;
			return pConf;
		}

		for (int i = pCRole->GetwVipLv(); i > 0; i--)
		{
			if (pCRole->IsVipFirstGacha(i))
			{
//				IME_ERROR("first gacha vip lv %u", i);
				pConf = CConfGacha::GetByVipFirstGacha(i);
				if (pConf == NULL)
					IME_ERROR("get vip first gacha is null vip lv %u", i);
				choose = 3;
				return pConf;
			}
		}

		pConf = CConfGacha::GetByVipNormalGacha(pCRole->GetwVipLv(), pCRole->GetdwLuck());
		if (pConf == NULL)
			IME_ERROR("get by vip normal gacha viplv %u, luck %u", pCRole->GetwVipLv(), pCRole->GetdwLuck());
		choose = 4;
		return pConf;
	}
}

void CShopEffect::SelectGachaOpt(CRole *pCRole, STC_CONF_GACHA *pConf, uint8_t choose)
{
	if (choose == 1)
	{
		pCRole->SetdwLuck(pCRole->GetdwLuck() - pConf->dwDelLuck);
		return;
	}	
	else if (choose == 2)
	{
			pCRole->SetdwChargeLuck(pCRole->GetdwChargeLuck() - GlobalConfig::VipChargeGachaFloors);
			return;
	}
	else if (choose == 3)
	{
		for (int i = pCRole->GetwVipLv(); i > 0; i--)
		{
			if (pCRole->IsVipFirstGacha(i))
			{
//				IME_ERROR("clear first gacha viplv %u", i);
				pCRole->AddVipFirstGacha(i);
				return;
			}
		}
	}
	else if (choose == 4)
	{
		pCRole->SetdwLuck(pCRole->GetdwLuck() - pConf->dwDelLuck);
		return;
	}
	else if (choose == 0)
	{
		//nothing to do 
		return;
	}
	return;
}

int CShopEffect::EnergyItemCountEffect(CRole *pCRole, SEffect &data)
{
	uint32_t val = pCRole->GetdwEnergyMax();
	val = data.para1;
	IME_DEBUG("shop use now %u, %u, %u", pCRole->GetdwEnergy(), pCRole->GetdwEnergyMax(), val);
	pCRole->ChangeEnergy(val);
	pCRole->SendProUpdate();
	pCRole->HandlerInfoOpt();
//	memset(&data, 0, sizeof(SEffect));
	
	pCRole->GetCMission()->MeatCount(ENERGY_SHOPID, 1);
	data.para1 = E_EFFECT_ENERGY;
	data.para2 = val;

	return 0;
}

int CShopEffect::StrengthItemCountEffect(CRole *pCRole, SEffect &data)
{
	uint32_t val = pCRole->GetdwStrengthMax();
	val = data.para1;
	pCRole->ChangeStrength(val);
	pCRole->SendProUpdate();
	pCRole->HandlerInfoOpt();
//	memset(&data, 0, sizeof(SEffect));

	pCRole->GetCMission()->WineCount(STRENGTH_SHOPID, 1);
	data.para1 = E_EFFECT_STRENGTH;
	data.para2 = val;

	return 0;
}

int CShopEffect::StaminaItemEffect(CRole *pCRole, SEffect &data)
{
	uint32_t val = pCRole->GetdwStaminaMax();
	val = val * data.para1 / 100.0;
	pCRole->ChangeStamina(val);
	pCRole->SendProUpdate();
	pCRole->HandlerInfoOpt();
//	memset(&data, 0, sizeof(SEffect));

//	pCRole->GetCMission()->WineCount(STRENGTH_SHOPID, 1);
	data.para1 = E_EFFECT_STAMINA;
	data.para2 = val;

	return 0;
}

int CShopEffect::StaminaItemCountEffect(CRole *pCRole, SEffect &data)
{
	uint32_t val = pCRole->GetdwStaminaMax();
	val = data.para1;
	pCRole->ChangeStamina(val);
	pCRole->SendProUpdate();
	pCRole->HandlerInfoOpt();
//	memset(&data, 0, sizeof(SEffect));

//	pCRole->GetCMission()->WineCount(STRENGTH_SHOPID, 1);
	data.para1 = E_EFFECT_STAMINA;
	data.para2 = val;

	return 0;
}

int CShopEffect::TrialEnterEffect(CRole *pCRole, SEffect &data)
{
	pCRole->GetCTrial()->ChangeTrial(data.para1);
	pCRole->GetCTrial()->HandlerDataInfo();

	return 0;
}

int CShopEffect::RoleExpEffect(CRole *pCRole, SEffect &data)
{
	pCRole->AddExp(data.para1);
	pCRole->Calculate(true);
	uint32_t val = data.para1;
	data.para1 = E_EFFECT_ROLE_EXP;
	data.para2 = val;

	return 0;
}

int CShopEffect::CardExpEffect(CRole *pCRole, SEffect &data){
	pCRole->dealMultipleCardExp(data.para1,data.para2);
	return 0;
}

int CShopEffect::IsObjValid(std::vector<SProduct> vproduct)
{
	for (int i = 0; i< vproduct.size(); i++)
	{
		int32_t para1 = (int32_t) vproduct[i].para1;
		int32_t para2 = (int32_t) vproduct[i].para2;
		switch(vproduct[i].type)
		{
			case 0:
				break;
			case E_OBJ_CARD:
			case E_OBJ_CARD_MORE:
			case E_OBJ_SKILL:
			case E_OBJ_AWAKE_CARD:
				{
					STC_CONF_CARD_BASE *pConf = CConfCardBase::Find(para1);
					if (pConf == NULL)
					{
						IME_ERROR("can not find this card index %u", para1);
						return -1;
					}
				}
				break;
			case E_OBJ_ITEM:
				break;
			case E_OBJ_SHOP:
				{
					STC_CONF_GOODS *pConf = CConfGoods::Find(para1);
					if (pConf == NULL)
					{
						IME_ERROR("goods can not find this goods %u", para1);
						return -1;
					}
				}
				break;
			case E_OBJ_COIN:
			case E_OBJ_DOLLAR:
			case E_OBJ_CARD_EXP:
			case E_OBJ_WORLD_EXP:
			case E_OBJ_ROLE_EXP:
			case E_OBJ_ENERGY:
			case E_OBJ_STRENGTH:
			case E_OBJ_STAMINA:
			case E_OBJ_VIP_EXP:
			case E_OBJ_COL_CARD_EXP:
			case E_OBJ_COL_COIN:
				break;
			case E_OBJ_COL_CARD:
				{
					std::vector<STC_CONF_CARD_COLLECTION *> *p;
					p = CConfCardCollection::Find(para1);
					if (p->size() == 0)
					{
						IME_ERROR("can not find cardcollection index %u", para1);
						return -1;
					}
				}
				break;
			case E_OBJ_COL_ITEM:
				{
					std::vector<STC_CONF_ITEM_COLLECTION *> *p;
					p = CConfItemCollection::Find(para1);
					if (p->size() == 0)
					{
						IME_ERROR("can not find itemcollection index %u", para1);
						return -1;
					}
				}
				break;
			case E_OBJ_COL_SHOP:
				{
					std::vector<STC_CONF_GOODS_COLLECTION *> *p;
					p = CConfGoodsCollection::Find(para1);
					if (p->size() == 0)
					{
						IME_ERROR("can not find shopcollection index %u", para1);
						return -1;
					}
				}
				break;
			case E_OBJ_COL_DOLLAR:
			case E_OBJ_COL_WORLD_EXP:
				break;
			case E_OBJ_COL_CARD_PLUS:
				{
					std::vector<STC_CONF_CARD_COLLECTION *> *p;
					p = CConfCardCollection::Find(para1);
					if (p->size() == 0)
					{
						IME_ERROR("can not find cardcollection index %u", para1);
						return -1;
					}
				}
				break;
			default:
				IME_ERROR("unvalid type %u", vproduct[i].type);
				return -1;
		}
	}
	return 0;
}


