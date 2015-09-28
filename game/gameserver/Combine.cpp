/*
 * =====================================================================================
 *
 *       Filename:  Combine.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2012年10月12日 14时03分34秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  imane (), imanecr@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include <stdint.h>
#include <vector>
#include "Combine.h"
#include "Log.h"
#include "DBCtrl.h"
#include "CmdDefine.h"
#include "EnumDefines.h"
#include "ErrorMsg.h"
#include "Role.h"
#include "ConfigData.h"
#include "Util.h"
#include "CardPacket.h"


void CCombine::HandlerCardCombineRequest(CRole *pCRole, WorldPacket &pkg)
{
	uint32_t ComIndex;
	pkg >> ComIndex;
	
	IME_CHAR_LOG("action [userid %u combine request]", pCRole->GetdwAccountId());	
	IME_DEBUG("combine request %u", ComIndex);
	uint32_t Timestamp;
	if (CDBCtrl::GetItemCombine(pCRole->GetdwAccountId(), ComIndex, Timestamp))
	{
		IME_ERROR("get record from db userid %u, combine %u", pCRole->GetdwAccountId(), ComIndex);
		pCRole->SendErrorMsg(ERRNO_MSG_ALREADY_ITEM_COMBINE, CMD_SC_CARD_COMBINE_REQUEST_RESULT);
		return;
	}

	STC_CONF_ITEM_COMBINE *p = CConfItemCombine::Find(ComIndex);
	if (p == NULL)
	{
		IME_ERROR("cannot find conf in combine csv index %u", ComIndex);
	  	pCRole->SendErrorMsg(ERRNO_MSG_CSV_DATA, CMD_SC_CARD_COMBINE_REQUEST_RESULT);
		return;	
	}

	bool IsHave = true;
	std::vector<uint32_t> vecID;
	if (p->dwID1 != 0 ) vecID.push_back(p->dwID1);
	if (p->dwID2 != 0 ) vecID.push_back(p->dwID2);
	if (p->dwID3 != 0 ) vecID.push_back(p->dwID3);
	if (p->dwID4 != 0 ) vecID.push_back(p->dwID4);
	if (p->dwID5 != 0 ) vecID.push_back(p->dwID5);
	if (p->dwID6 != 0 ) vecID.push_back(p->dwID6);

	for (int i = 0; i < vecID.size(); i++)
	{
		if (pCRole->GetclsItemPacket()->JudgeItemNumber(vecID[i]))
		{
			IsHave = false;
			break;
		}
	}

	if (!IsHave)
	{
		IME_ERROR("the user have not enough item for combining index %u", ComIndex);
		pCRole->SendErrorMsg(ERRNO_MSG_NOT_ENOUGH_ITEM_COMBINE, CMD_SC_CARD_COMBINE_REQUEST_RESULT);
		return;
	}

	if (!CDBCtrl::InsertItemCombine(pCRole->GetdwAccountId(), ComIndex, time(NULL)))
	{
		IME_ERROR("insert db fail userid %u, combine %u", pCRole->GetdwAccountId(), ComIndex);
		pCRole->SendErrorMsg(ERRNO_MSG_DB, CMD_SC_CARD_COMBINE_REQUEST_RESULT);
		return;
	}
	
	WorldPacket info(CMD_SC_CARD_COMBINE_REQUEST_RESULT);
	info << (uint16_t) 0;
	info << (uint32_t) ComIndex;
	info << (uint32_t) GlobalConfig::CombineTime;
	pCRole->SendPacket(&info);
}

void CCombine::HandlerCardCombineGet(CRole *pCRole, WorldPacket &pkg)
{
	uint32_t ComIndex;
	pkg >> ComIndex;
	uint32_t Timestamp;

	IME_CHAR_LOG("action [userid %u combine index %u get]", pCRole->GetdwAccountId(), ComIndex); 
	IME_DEBUG("combine get %u", ComIndex);
	if (!CDBCtrl::GetItemCombine(pCRole->GetdwAccountId(), ComIndex, Timestamp))
	{
		IME_ERROR("get db fail userid %u, combine %u", pCRole->GetdwAccountId(), ComIndex);
		pCRole->SendErrorMsg(ERRNO_MSG_DB_NO_DATA_OR_ERROR, CMD_SC_CARD_COMBINE_GET_RESULT);
		return;
	}
	if (Timestamp + GlobalConfig::CombineTime > time(NULL))
	{
		IME_ERROR("cannot get the item , time is not to, time is up will in %u, now %u", Timestamp + GlobalConfig::CombineTime, time(NULL));
		pCRole->SendErrorMsg(ERRNO_MSG_COMBINE_TIME_NOT, CMD_SC_CARD_COMBINE_GET_RESULT);
		return;
	}

	STC_CONF_ITEM_COMBINE *p = CConfItemCombine::Find(ComIndex);
	if (p == NULL)
	{
		IME_ERROR("cannot find conf in combine csv index %u", ComIndex);
	  	pCRole->SendErrorMsg(ERRNO_MSG_CSV_DATA, CMD_SC_CARD_COMBINE_GET_RESULT);
		return;	
	}

	bool IsHave = true;
	std::vector<uint32_t> vecID;
	if (p->dwID1 != 0 ) vecID.push_back(p->dwID1);
	if (p->dwID2 != 0 ) vecID.push_back(p->dwID2);
	if (p->dwID3 != 0 ) vecID.push_back(p->dwID3);
	if (p->dwID4 != 0 ) vecID.push_back(p->dwID4);
	if (p->dwID5 != 0 ) vecID.push_back(p->dwID5);
	if (p->dwID6 != 0 ) vecID.push_back(p->dwID6);

	for (int i = 0; i < vecID.size(); i++)
	{
		if (pCRole->GetclsItemPacket()->JudgeItemNumber(vecID[i]))
		{
			IsHave = false;
			break;
		}
	}

	STC_CONF_ITEM_COMBINE *pConfCombine;
//	std::vector<STC_CONF_ITEM_COMBINE *> *pConfCombine;
	pConfCombine = CConfItemCombine::Find(ComIndex);
	if (pConfCombine == NULL )
	{
		IME_ERROR("cannot find this index [%u] in Combine csv STC_CONF_ITEM_COMBINE", ComIndex);
	  	pCRole->SendErrorMsg(ERRNO_MSG_CSV_DATA, CMD_SC_CARD_COMBINE_REQUEST_RESULT);
		return;	
	}

	if(pCRole->GetclsCardPacket()->IsFull(1, pConfCombine->dwCardIndex))
	{
		IME_ERROR("user %u card packet already full", pCRole->GetdwAccountId());
		pCRole->SendErrorMsg(ERRNO_MSG_CARD_EQUIP_PACKET_NOT_ENOUGH, CMD_SC_CARD_COMBINE_GET_RESULT);
		return;
	}
	
//	if (pConfCombine->size() == 0)
//	{
//	  	pCRole->SendErrorMsg(ERRNO_MSG_CSV_DATA, CMD_SC_CARD_COMBINE_REQUEST_RESULT);
//		return;	
//	}

	if (!CDBCtrl::DelItemCombine(pCRole->GetdwAccountId(), ComIndex))
	{
		IME_ERROR("del db fail userid %u, combine %u", pCRole->GetdwAccountId(), ComIndex);
		pCRole->SendErrorMsg(ERRNO_MSG_DB, CMD_SC_CARD_COMBINE_REQUEST_RESULT);
		return;
	}

	if (!IsHave)
	{
		IME_ERROR("the user have not enough item for combining index %u", ComIndex);
		pCRole->SendErrorMsg(ERRNO_MSG_NOT_ENOUGH_ITEM_COMBINE, CMD_SC_CARD_COMBINE_GET_RESULT);
		return;
	}

	//todo create card  del item , combine record
	

//	std::vector<int> vecCardIndex;
//	for (int i = 0; i < pConfCombine->size(); i++)
//	{
//		vecCardIndex.push_back((*pConfCombine)[i]->dwProbability);
//	}	

//	int ret = CUtil::RandFactor(vecCardIndex);

	for (int i = 0; i < vecID.size(); i++)
	{
		pCRole->GetclsItemPacket()->DelItem(vecID[i], SOURCE_COMBINE);
		pCRole->GetclsItemPacket()->ItemUpdate(vecID[i], CItem::DEL);
	}
	
	uint32_t objid,opt;
	pCRole->GetclsCardPacket()->CardAddByPara(pConfCombine->dwCardIndex, objid, opt, SOURCE_COMBINE);
	if (opt)
	pCRole->GetclsCardPacket()->SingleCardAddUpdate(objid);
//	HandlerCardCombineInfo(pCRole);

	WorldPacket info(CMD_SC_CARD_COMBINE_GET_RESULT);
	info << (uint16_t) 0;
	info << (uint32_t) objid;
	info << (uint32_t) ComIndex;
	pCRole->SendPacket(&info);
	return;
	
}

void CCombine::HandlerCardCombineInfo(CRole *pCRole)
{
	std::vector<sCombRecord> temp;
	if (!CDBCtrl::GetMultiItemCombine(pCRole->GetdwAccountId(), temp))
	{
//		IME_ERROR("get record from db userid %u", pCRole->GetdwAccountId());
//		pCRole->SendErrorMsg(ERRNO_MSG_DB_NO_DATA_OR_ERROR, CMD_SC_CARD_COMBINE_INFO);
		WorldPacket info(CMD_SC_CARD_COMBINE_INFO);
		info << (uint16_t) 0;
		pCRole->SendPacket(&info);
		return;
	}

	WorldPacket info(CMD_SC_CARD_COMBINE_INFO);
	info << (uint16_t) temp.size();
	uint32_t arrivetime;
	for (int i = 0; i < temp.size(); i++)
	{
		arrivetime = temp[i].Timestamp + GlobalConfig::CombineTime;
		info << temp[i].ItemIndex;
		info << (uint32_t)(((int64_t)arrivetime - time(NULL)) < 0 ? 0 : (arrivetime - time(NULL)));
	}
	pCRole->SendPacket(&info);
	return;
}

void CCombine::TimerCheck(CRole *pCRole)
{
	std::vector<sCombRecord> temp;
	if (!CDBCtrl::GetMultiItemCombine(pCRole->GetdwAccountId(), temp))
	{
		IME_ERROR("insert db fail userid %u", pCRole->GetdwAccountId());
		return;
	}

	for (int i = 0; i < temp.size(); i++)
	{	
		uint32_t ComIndex = temp[i].ItemIndex;
		STC_CONF_ITEM_COMBINE *p = CConfItemCombine::Find(ComIndex);
		if (p == NULL)
		{
			IME_ERROR("cannot find conf in combine csv index %u", ComIndex);
			return;	
		}

		bool IsHave = true;
		std::vector<uint32_t> vecID;
		if (p->dwID1 != 0 ) vecID.push_back(p->dwID1);
		if (p->dwID2 != 0 ) vecID.push_back(p->dwID2);
		if (p->dwID3 != 0 ) vecID.push_back(p->dwID3);
		if (p->dwID4 != 0 ) vecID.push_back(p->dwID4);
		if (p->dwID5 != 0 ) vecID.push_back(p->dwID5);
		if (p->dwID6 != 0 ) vecID.push_back(p->dwID6);

		for (int i = 0; i < vecID.size(); i++)
		{
			if (pCRole->GetclsItemPacket()->JudgeItemNumber(vecID[i]))
			{
				IsHave = false;
				break;
			}
		}

		if (!IsHave)
		{
			CDBCtrl::DelItemCombine(pCRole->GetdwAccountId(), ComIndex);
		}
	}
}

void CCombine::HandlerCheckAndInfo(CRole *pCRole)
{
	IME_DEBUG("combine update %u", pCRole->GetdwAccountId());
	std::vector<sCombRecord> temp;
	if (!CDBCtrl::GetMultiItemCombine(pCRole->GetdwAccountId(), temp))
	{
//		IME_ERROR("get item data fail userid %u", pCRole->GetdwAccountId());
		return;
	}

	for (int i = 0; i < temp.size(); i++)
	{	
		uint32_t ComIndex = temp[i].ItemIndex;
		STC_CONF_ITEM_COMBINE *p = CConfItemCombine::Find(ComIndex);
		if (p == NULL)
		{
			IME_ERROR("cannot find conf in combine csv index %u", ComIndex);
			return;	
		}

		IME_DEBUG("data combine index %u", ComIndex);

		bool IsHave = true;
		std::vector<uint32_t> vecID;
		if (p->dwID1 != 0 ) vecID.push_back(p->dwID1);
		if (p->dwID2 != 0 ) vecID.push_back(p->dwID2);
		if (p->dwID3 != 0 ) vecID.push_back(p->dwID3);
		if (p->dwID4 != 0 ) vecID.push_back(p->dwID4);
		if (p->dwID5 != 0 ) vecID.push_back(p->dwID5);
		if (p->dwID6 != 0 ) vecID.push_back(p->dwID6);

		for (int j = 0; j < vecID.size(); j++)
		{
			if (pCRole->GetclsItemPacket()->JudgeItemNumber(vecID[j]))
			{
				IsHave = false;
				break;
			}
		}

		if (!IsHave)
		{
			CDBCtrl::DelItemCombine(pCRole->GetdwAccountId(), ComIndex);
			temp.erase(temp.begin() + i);
			i--;
		}

	}

	WorldPacket info(CMD_SC_CARD_COMBINE_INFO);
	info << (uint16_t) temp.size();
	uint32_t arrivetime;
	for (int i = 0; i < temp.size(); i++)
	{
		arrivetime = temp[i].Timestamp + GlobalConfig::CombineTime;
		info << temp[i].ItemIndex;
		info << (uint32_t)(((int64_t)arrivetime - time(NULL)) < 0 ? 0 : (arrivetime - time(NULL)));
		IME_DEBUG("combine index %u, cd time %d", temp[i].ItemIndex, (int32_t)(arrivetime - time(NULL)) < 0 ? 0 : (arrivetime - time(NULL)) );
	}
	pCRole->SendPacket(&info);
	return;
}
