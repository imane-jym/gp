/*
 * =====================================================================================
 *
 *       Filename:  RandomEvent.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2012年09月11日 04时57分05秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  imane (), imanecr@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include "RandomEvent.h"
#include "Role.h"
#include "ConfigData.h"
#include "CardPacket.h"
#include "Tech.h"
#include "EnumDefines.h"

std::map<uint16_t, CRandomEvent::REventFunc> CRandomEvent::m_mapFunc;

void CRandomEvent::initialization()
{
	m_mapFunc[EXP_RATE_UP] = REventExpRateUp;
	m_mapFunc[COIN_RATE_UP] = REventCoinRateUp;
	m_mapFunc[CARD_DROP] = REventCardDrop;
	m_mapFunc[ITEM_DROP] = REventItemDrop;
	m_mapFunc[SHOP_DROP] = REventShopDrop;
	m_mapFunc[WORLD_EXP_UP] = REventWorldExpUp;
}

int CRandomEvent::REventExpRateUp(
		CRole *pCRole,
		int32_t &para1,
		int32_t &para2,
		int32_t &para3,
		int32_t &para4		
		)
{
//	para2 = (para1 * para2);
//	uint32_t texp = para2;
//	uint32_t tcardexp = (para1 * para3);
//	pCRole->GetclsTech()->TechExp(texp);
//	para2 = texp;
//	pCRole->AddExp(para2);
//	para1 = EXP_UP;
//	pCRole->CalExp(true);
//	para3 = tcardexp;

	int exp = CUtil::RandByRange(para1, para2 + 1); 
	if (exp < 0)
		exp = 0;
	pCRole->AddExp(exp);
	pCRole->CalExp(true);

	std::vector<CCard *> vec;
	if (pCRole->GetclsCardPacket()->GetEquippCards(vec))
	{
		IME_ERROR("get card property fail, userid %u", pCRole->GetdwAccountId());
		return -1;
	}
	for (int i = 0; i < vec.size(); i++)
	{
		vec[i]->AddExp(exp);
		vec[i]->Calculate(true);
	}

//	IME_DEBUG("get exp type %u", EXP_UP);
	pCRole->SendProUpdate();
	para1 = EXP_UP;
	para2 = exp;
	para3 = exp;
	return 0;
}

int CRandomEvent::REventCoinRateUp(
		CRole *pCRole,
		int32_t &para1,
		int32_t &para2,
		int32_t &para3,
		int32_t &para4		
		)
{
//	para2 = (para1 * para3);
//	uint32_t tcoin = para2;
//	pCRole->GetclsTech()->TechCoin(tcoin);
//	para2 = tcoin;

	int coin = CUtil::RandByRange(para1, para2 + 1); 
	if (coin < 0)
		coin = 0;
	pCRole->ChangeCoin(coin, SOURCE_OTHER);
	para1 = COIN_UP;
	para2 = coin;
	pCRole->SendProUpdate();
//	IME_DEBUG("get coin event %u", COIN_UP);
	return 0;
}


int CRandomEvent::REventCardDrop(
		CRole *pCRole,
		int32_t &para1,
		int32_t &para2,
		int32_t &para3,
		int32_t &para4
		)
{
//	IME_DEBUG("card drop event %u", CARD_GET);
	std::vector<int> vecrate;
	std::vector<STC_CONF_CARD_COLLECTION *> *p;
	p = CConfCardCollection::Find(para1);
	if (p->size() == 0)
	{
		IME_ERROR("can not find cardcollection index %u", para1);
		para1 = 0;
		para2 = 0;
		para3 = 0;
		return 0;
	}
	for (int i = 0; i < p->size(); i++)
	{
		vecrate.push_back((*p)[i]->byWeight);
	}	
	int rel;
	if ((rel = CUtil::RandFactor(vecrate)) == -1)
	{
		para1 = 0;
		para2 = 0;
		para3 = 0;
		return 0;
	}
	if ((*p)[rel]->dwCardindex == 0)
	{
		para1 = 0;
		para2 = 0;
		para3 = 0;
		return 0;
	}
	para1 = CARD_GET; 
	para2 = 0;
	para3 = (*p)[rel]->dwCardindex;
	uint32_t objid, opt;
	pCRole->GetclsCardPacket()->CardAddByPara(para3, objid, opt, SOURCE_OTHER); 
	if (opt)
	pCRole->GetclsCardPacket()->SingleCardAddUpdate(objid);
	CCard *pCard = pCRole->GetclsCardPacket()->Find(objid);
	if (pCard == NULL)
	{
		IME_ERROR("add this card fail %u", para3);
		return -1;
	}
	para2 = pCard->GetbyStarLevel();
	para3 = objid;
	return 0;
}

int CRandomEvent::REventItemDrop(
		CRole *pCRole,
		int32_t &para1,
		int32_t &para2,
		int32_t &para3,		
		int32_t &para4	
		)
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
	para1 = ITEM_GET; 
	para2 = (*p)[rel]->dwItemIndex;
	pCRole->GetclsItemPacket()->AddItem(para2, SOURCE_OTHER); 
	pCRole->GetclsItemPacket()->ItemUpdate(para2, CItem::ADD);
	return 0;
}

int CRandomEvent::REventShopDrop(
		CRole *pCRole,
		int32_t &para1,
		int32_t &para2,
		int32_t &para3,		
		int32_t &para4	
		)
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
	if ((rel = CUtil::RandFactor(vecrate)) == -1)
	{
		return -1;
	}
	para1 = SHOP_GET; 
	para2 = (*p)[rel]->dwItemIndex;
	for (int i = 0; i < (*p)[rel]->dwNumber; i++)
	{
		pCRole->GetCShop()->AddItem(para2, SOURCE_OTHER); 
	}
	pCRole->GetCShop()->HandlerShopInfo(para2);
	para3 = (*p)[rel]->dwNumber;
	return 0;
}

//int CRandomEvent::ExecRandomEvent(CRole *pCRole, uint8_t rate, STC_CONF_RANDOM_EVENT &retemp)
//{
//	if (CUtil::RandEither(rate, 100))
//	{
//		initialization();
//		REventFunc HandlerRandomEventFunc;
//		HandlerRandomEventFunc = CRandomEvent::Find(retemp.wType);
//		if (HandlerRandomEventFunc == NULL)
//		{
//			IME_ERROR("no this random type %u", retemp.wType);
//			return -1;
//		}
//		else
//		{
//			if (HandlerRandomEventFunc(pCRole, retemp.para1, retemp.para2, retemp.para3, retemp.para4))
//			{
//				IME_ERROR("random handler fail");
//				return -1;
//			}
//		}
//	}	
//	return 0;
//}

int CRandomEvent::REventWorldExpUp(
		CRole *pCRole,
		int32_t &para1,
		int32_t &para2,
		int32_t &para3,
		int32_t &para4		
		)
{
//	para2 = (para1 * para3);
//	uint32_t tcoin = para2;
//	pCRole->GetclsTech()->TechCoin(tcoin);
//	para2 = tcoin;

	int exp = CUtil::RandByRange(para1, para2 + 1); 
	if (exp < 0)
		exp = 0;
	pCRole->SetdwWorldHistory(pCRole->GetdwWorldHistory() + exp);
	para1 = WORLD_EXP_UP;
	para2 = exp;
	pCRole->SendProUpdate();
//	IME_DEBUG("get coin event %u", COIN_UP);
	return 0;
}

