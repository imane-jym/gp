/*
 * GmCmdPro.cpp
 *
 *  Created on: 2011-6-23
 *      Author: yq
 */

#include "GmCmdPro.h"
#include "Util.h"
#include "Role.h"
#include "LuaCtrl.h"
#include <string>
#include <vector>
#include <map>
#include "DBCtrl.h"
#include "CmdInnerDefine.h"
#include "World.h"
#include "ConfigData.h"
#include "GlobalConfig.h"
#include "md5.h"
#include "CardPacket.h"
#include "Arena.h"
#include "Group.h"
#include "Other.h"
#include "Mail.h"
#include "Master.h"
#include "Tujian.h"
#include "Task.h"
#include "WorldBoss.h"
#include "CenterDBCtrl.h"
#include "Mission.h"

extern std::string g_strConfigPath;

CGmCmdPro::MAP_FUNC_GM_CMD CGmCmdPro::m_mapGmFuncs;

void CGmCmdPro::Init()
{
	m_mapGmFuncs.clear();

	m_mapGmFuncs["$test"] = Test;
	m_mapGmFuncs["$lv"] = Level;
	m_mapGmFuncs["$addexp"] = AddExp;
	m_mapGmFuncs["$addenergy"] = AddEnergy;
	m_mapGmFuncs["$addstrength"] = AddStrength;
	m_mapGmFuncs["$addcoin"] = AddCoin;
	m_mapGmFuncs["$addcard"] = AddCard;
	m_mapGmFuncs["$additem"] = AddItem;
	m_mapGmFuncs["$addcardexp"] = AddCardExp;
	m_mapGmFuncs["$addfriend"] = AddFriend;
	m_mapGmFuncs["$addprestige"] = AddPrestige;
	m_mapGmFuncs["$addgroupcredit"] = AddGroupCredit;
	m_mapGmFuncs["$addgroupmembercredit"] = AddGroupMemberCredit;
//	m_mapGmFuncs["$cardskill"] = CardSkill;
	m_mapGmFuncs["$jumpmission"] = JumpMission;
	m_mapGmFuncs["$adddollar"] = AddDollar;
	m_mapGmFuncs["$updateanno"] = UpdateAnno;
	m_mapGmFuncs["$sendgift"] = SendGift;
	m_mapGmFuncs["$masteraddexp"] = MasterAddExp;
	m_mapGmFuncs["$openalltujian"] = OpenAllTujian;
	m_mapGmFuncs["$viplvup"] = VipLvUp;
	m_mapGmFuncs["$cardexpup"] = CardExpUp;
	m_mapGmFuncs["$worldexpup"] = WorldExpUp;
	m_mapGmFuncs["$addshop"] = AddShop;
	m_mapGmFuncs["$clearextrasign"] = ClearExtraSign;
	m_mapGmFuncs["$equipexpup"] = EquipExpUp;
	m_mapGmFuncs["$luckup"] = LuckUp;
	m_mapGmFuncs["$signclear"] = SignClear;
	m_mapGmFuncs["$addvipexp"] = AddVipexp;
	m_mapGmFuncs["$bossprize"] = BossPrize;
	m_mapGmFuncs["$udevote"] = UDevote;
	m_mapGmFuncs["$charge"] = Charge;
	m_mapGmFuncs["$cp1"] = CardPlusOne;
	m_mapGmFuncs["$sa"] = StaminaAdd;
	m_mapGmFuncs["$addvitality"] = AddVitality;
	m_mapGmFuncs["$limitrole"] = LimitRole;
}

void CGmCmdPro::PraseCmd(CRole *pRole, const std::string & strContent)
{
	IME_LOG("Gm Cmd[%s]", strContent.c_str());
	IME_CHAR_LOG("action [user %u press gm cmd %s]", pRole->GetdwAccountId(), strContent.c_str());
	if (!pRole->IsGM())
	{
		//todo
		IME_ERROR("GM:Role %s is not a gm",pRole->GetstrAccount().c_str());
		return ;
	}

	std::vector<std::string> vecData;
	CUtil::StrSplit(strContent," ",vecData);
	if (vecData.size() >= 1)
	{
		MAP_FUNC_GM_CMD::iterator itr = m_mapGmFuncs.find(vecData[0]);
		if (itr != m_mapGmFuncs.end())
		{
			(*(itr->second))(pRole, vecData);
		}
	}
}

void CGmCmdPro::Test(CRole *pRole, const std::vector<std::string> & vecPara)
{
	//echo test
	if (vecPara.size() > 1)
	{
		std::string strContent;
		for(int i = 0; i < vecPara.size(); ++i)
			strContent += vecPara[i] + "---";
		//todo
                pRole->SendMsgStr(strContent);
	}
}

void CGmCmdPro::Level(CRole *pRole, const std::vector<std::string> &vecPara)
{
//    int lv = atol(vecPara[1].c_str());
//    pRole->SetwLevel(lv);
//    pRole->SendProUpdate();
}

void CGmCmdPro::AddExp(CRole *pRole, const std::vector<std::string> &vecPara)
{
	if (vecPara.size() < 2)
		return;
    int val = atol(vecPara[1].c_str());
//    unsigned int Exp = pRole->GetdwExp(); 
//    Exp += val;
//    pRole->SetdwExp(Exp);
	pRole->AddExp(val);
	pRole->Calculate();
    pRole->SendProUpdate();
}

void CGmCmdPro::AddEnergy(CRole *pRole, const std::vector<std::string> &vecPara)
{
	if (vecPara.size() < 2)
		return;
    int val = atol(vecPara[1].c_str());
//    unsigned int Energy = pRole->GetdwEnergy(); 
//    Energy += val;
//    pRole->SetdwEnergy(Energy);
	pRole->ChangeEnergyNoLimit(val);
    pRole->SendProUpdate();
	pRole->HandlerInfoOpt();
}

void CGmCmdPro::AddStrength(CRole *pRole, const std::vector<std::string> &vecPara)
{
	if (vecPara.size() < 2)
		return;
    int val = atol(vecPara[1].c_str());
//    unsigned int Strength = pRole->GetdwStrength(); 
//    Strength += val;
//    pRole->SetdwStrength(Strength);
    pRole->ChangeStrengthNoLimit(val);
    pRole->SendProUpdate();
	pRole->HandlerInfoOpt();
}

void CGmCmdPro::AddCoin(CRole *pRole, const std::vector<std::string> &vecPara)
{
	if (vecPara.size() < 2)
		return;
    int val = atol(vecPara[1].c_str());
//    unsigned int Coin = pRole->GetdwCoin(); 
//    Coin += val;
//    pRole->SetdwCoin(Coin);
	pRole->ChangeCoin(val, SOURCE_GM);
    pRole->SendProUpdate();

}

void CGmCmdPro::AddCard(CRole *pRole, const std::vector<std::string> &vecPara)
{
	if (vecPara.size() < 2)
		return;
	uint32_t val = atol(vecPara[1].c_str());
	uint32_t star = 1;
	if (vecPara.size() == 3)
		star = atol(vecPara[2].c_str());
	uint32_t objid,opt;
	pRole->GetclsCardPacket()->CardAddByPara(val, objid, opt, SOURCE_GM);
	if (opt)
	pRole->GetclsCardPacket()->SingleCardAddUpdate(objid);
}

void CGmCmdPro::AddItem(CRole *pRole, const std::vector<std::string> &vecPara)
{
	if (vecPara.size() < 3)
	{
		return;
	}
	uint32_t val = atol(vecPara[1].c_str());
	uint32_t number = atol(vecPara[2].c_str());
	if (pRole->GetclsItemPacket()->AddItem(val, SOURCE_GM, number))
	{
		IME_ERROR("[GM]add item fail index %u number %u", val, number); 	
		return;
	}
	pRole->GetclsItemPacket()->ItemUpdate(val, CItem::ADD, number);
}

void CGmCmdPro::AddCardExp(CRole *pRole, const std::vector<std::string> &vecPara)
{
	if (vecPara.size() > 2)
	{
    	uint32_t val = atol(vecPara[1].c_str());
		uint32_t number = atol(vecPara[2].c_str());
		CCard *p = pRole->GetclsCardPacket()->Find(val);
    	if (p == NULL)
		{
			IME_ERROR("[GM]find card fail objid [%u]", val); 	
			return;
		}
		IME_DEBUG("[GM] add card objid [%u] exp [%u]", val, number);
		p->AddExp(number);
		p->Calculate(true);
	}
}

void CGmCmdPro::AddFriend(CRole *pRole, const std::vector<std::string> &vecPara)
{
	if (vecPara.size() > 1)
	{
    	uint32_t val = atol(vecPara[1].c_str());
		if (pRole->GetdwAccountId() == val)
			return;
		if (CDBCtrl::IsHaveRole(pRole->GetdwAccountId()) || CDBCtrl::IsHaveRole(val))
			return;
		std::string str;
		uint32 coin;
		uint16 level;
		uint32 accounid;
		accounid = val;
		CDBCtrl::GetUserBaseDataForDurationPreview(str, coin, level, accounid);
		if (pRole->GetwLevel() == 0 || level == 0)
			return;
		CDBCtrl::InsertFriend(pRole->GetdwAccountId(), val);
		CDBCtrl::InsertFriend(val, pRole->GetdwAccountId());
	}
}

void CGmCmdPro::AddPrestige(CRole *pRole, const std::vector<std::string> &vecPara)
{
	if (vecPara.size() > 1)
	{
    	uint32_t val = atol(vecPara[1].c_str());
		if (pRole->IsOpenArena())
		{
			uint32_t prestige = pRole->GetclsArena()->GetdwPrestige();
			prestige += val;
			pRole->GetclsArena()->SetdwPrestige(prestige);
			pRole->GetclsArena()->HandlerProUpdate();
			IME_CHAR_LOG("resource [status=success,res=prestige,opt=add,character=%u,index=%u,number=%d,reason=ok]", pRole->GetdwAccountId(), 0, val);
		}
	}
}

void CGmCmdPro::AddGroupCredit(CRole *pRole, const std::vector<std::string> &vecPara)
{
	if (vecPara.size() > 2)
	{
    	uint64 val = atoll(vecPara[1].c_str());
		uint32_t valCredit = atol(vecPara[2].c_str());
//		IME_ERROR("%llu", val);
		CGroup *pCGroup = sWorld->GetCGroup(val);
		if (pCGroup != NULL)
		{
			pCGroup->ChangeCredit(valCredit);
			WorldPacket info;
			pCGroup->PackProUpdate(info);
			pCGroup->SendDataToAllMember(&info);
		}
	}
}

void CGmCmdPro::AddGroupMemberCredit(CRole *pRole, const std::vector<std::string> &vecPara)
{
	if (vecPara.size() > 2)
	{
    	uint32 val = atol(vecPara[1].c_str());
		uint32_t valCredit = atol(vecPara[2].c_str());
//		IME_ERROR("%llu", val);
		CGroup *pCGroup = sWorld->GetCGroupByUserId(val);
		if (pCGroup != NULL)
		{
			SGroupMember *pSGM = pCGroup->GetGroupMember(val);
			if (pSGM == NULL)
				return;
			IME_DEBUG("now %u, add %u", pSGM->dwCredit, valCredit);
			pSGM->dwCredit += valCredit;
			WorldPacket info;
			pCGroup->PackGroupMemberInfo(pRole->GetdwAccountId(), info);
			pRole->SendPacket(&info);
		}
	}
}

//void CGmCmdPro::CardSkill(CRole *pRole, const std::vector<std::string> &vecPara)
//{
//	if (vecPara.size() > 3)
//	{
//    	uint32 val = atol(vecPara[1].c_str());
//		uint32_t pos = atol(vecPara[2].c_str());
//		uint32_t skillindex = atol(vecPara[3].c_str());
//
//		CCard *pCard = pRole->GetclsCardPacket()->Find(val);
//		if (pCard == NULL)
//		{
//			IME_ERROR("can not find card objid %u in csv", val);
//			return;
//		}
//		
////		if (pos = 0)
////		{
//			pCard->SetdwStudySkillIndex(skillindex);
////		}
//		pCard->SendProUpdate();
//	}
//}

void CGmCmdPro::JumpMission(CRole *pRole, const std::vector<std::string> &vecPara)
{
	if (vecPara.size() >= 3)
	{
    	uint32_t type  = atol(vecPara[1].c_str());
		uint32_t index = atol(vecPara[2].c_str());
		
		if (type == 0)
		{
			pRole->GetCDurationMission()->SetMissionPosition(index);
			pRole->GetCDurationMission()->HandlerDurationMissionInfo(index);
		}
		else
		{
			pRole->GetCDurationMission()->SetHighMissionPosition(index);
			pRole->GetCDurationMission()->HandlerHighDurationMissionInfo(index);
		}
		
	}
}

void CGmCmdPro::AddDollar(CRole *pRole, const std::vector<std::string> &vecPara)
{
	if (vecPara.size() > 1)
	{
		int32_t val  = atol(vecPara[1].c_str());

		uint32_t order_id;
		if (!CDBCtrl::GetShopOrderId(pRole->GetdwAccountId(), order_id))
		{
			IME_ERROR("user %u give dollar %u fail in GM", pRole->GetdwAccountId(), val);
			return;
		}

		pRole->ChangeDollarWithExtra(val, SOURCE_GM);
		pRole->SendProUpdate();
		pRole->HandlerInfoOpt();
		CDBCtrl::LogShopRecord(pRole->GetdwAccountId(), order_id, val, 0, 0, E_DC_GM);	
	}
}

std::string &replace_all(std::string &str, const std::string strold, const std::string strnew)
{
	while(true)
	{
		size_t pos;
		if ((pos = str.find(strold)) != std::string::npos)
			str.replace(pos, strold.size(), strnew);
		else
			break;
	}
	return str;
}

void CGmCmdPro::UpdateAnno(CRole *pRole, const std::vector<std::string> &vecPara)
{
	if (vecPara.size() > 2)
	{
		uint32_t val  = atol(vecPara[1].c_str());
		std::string temp = vecPara[2];
		replace_all(temp, "&nbsp;", " ");
		replace_all(temp, "\\n", "\n");
		sWorld->GetCAnnouncement()->HandlerUpdateContent(temp.c_str(), val);
//		CDBCtrl::SaveAnnouncementData(temp.c_str(), val);
	}
}

void CGmCmdPro::SendGift(CRole *pRole, const std::vector<std::string> &vecPara)
{
	if (vecPara.size() > 10)
	{
		IME_DEBUG("send gift");
		uint32_t userid = atol(vecPara[1].c_str());
		std::string type = vecPara[2];
		uint32_t producttype = atol(vecPara[3].c_str());
		uint32_t para1 = atol(vecPara[4].c_str());
		uint32_t para2 = atol(vecPara[5].c_str());

		uint32_t coin = atol(vecPara[6].c_str());
		uint32_t dollar = atol(vecPara[7].c_str());
		uint32_t energy = atol(vecPara[8].c_str());
		uint32_t strength = atol(vecPara[9].c_str());
		uint32_t worldhistory = atol(vecPara[10].c_str());

		CMail::SBaseAttr tmpAttr = {0};
		tmpAttr.coin = coin;
		tmpAttr.dollar = dollar;
		tmpAttr.energy = energy;
		tmpAttr.strength = strength;
		tmpAttr.worldhistory = worldhistory;

		uint32_t mail_id = 0;
		CDBCtrl::stuMailDetail tmp = {0};
		if (type == "pay")
		{
			CMail::AddEmailSysPay(userid, mail_id, tmp, producttype, para1, para2, tmpAttr);   
		}
		else
		{
			CMail::AddEmailSysGift(userid, mail_id, tmp, producttype, para1, para2, type, tmpAttr);   
		}
		
		CRole *pPeerCRole = sWorld->FindUserRole(userid);
		if (pPeerCRole != NULL)
		{
			CMail::EmailUpdate(pPeerCRole, CMail::ADD, &tmp, 0);
		}

//		sWorld->GetCAnnouncement()->HandlerUpdateContent(temp.c_str(), val);
//		CDBCtrl::SaveAnnouncementData(temp.c_str(), val);
	}
}

void CGmCmdPro::MasterAddExp(CRole *pRole, const std::vector<std::string> &vecPara)
{
	if (vecPara.size() > 2)
	{
		uint32_t masterindex = atol(vecPara[1].c_str());
		uint32_t exp = atol(vecPara[2].c_str());
		pRole->GetCMaster()->AddMasterExp(masterindex, exp);
	}
}

void CGmCmdPro::OpenAllTujian(CRole *pRole, const std::vector<std::string> &vecPara)
{
	if (vecPara.size() > 1)
	{
		uint32_t flag = atol(vecPara[1].c_str());
		CConfCardBase::MapData *p = CConfCardBase::GetAll();
		CConfCardBase::MapData::iterator it;
		for (it = p->begin(); it != p->end(); it++)
		{
			if (flag == 0)
			{
				pRole->GetclsTujian()->AddTujian(it->first, E_CR_AWAKE, 0, TUJIAN_GET);
				pRole->GetclsTujian()->CardTujianUpdate(it->first, TUJIAN_CARD);
			}
			else
			{
				pRole->GetclsTujian()->AddTujian(it->first, E_CR_AWAKE, (it->second).dwLimitBreakNumber, TUJIAN_GET);
				pRole->GetclsTujian()->CardTujianUpdate(it->first, TUJIAN_CARD);
			}
		}
	}
}

void CGmCmdPro::VipLvUp(CRole *pRole, const std::vector<std::string> &vecPara)
{
	if (vecPara.size() > 1)
	{
		uint32_t lev = atol(vecPara[1].c_str());
		pRole->SetwVipLv(lev);
		pRole->LoadProperty();
		pRole->SendProUpdate();
	}
}

void CGmCmdPro::CardExpUp(CRole *pRole, const std::vector<std::string> &vecPara)
{
	if (vecPara.size() > 2)
	{
		uint32_t pos = atol(vecPara[1].c_str());
		uint32_t exp = atol(vecPara[2].c_str());
		if (pos != 0)
		{
			uint32_t obj = pRole->GetclsCardPacket()->GetViewObjId(pos - 1);
			if (obj != 0)
			{
				IME_DEBUG("add card objid %u", obj);
				pRole->GetclsCardPacket()->Find(obj)->AddExp(exp);
				pRole->GetclsCardPacket()->Find(obj)->Calculate(true);
				pRole->GetclsCardPacket()->Calculate(true);
			}
		}
		else
		{
			uint32_t obj = pRole->GetclsCardPacket()->GetMasterObjId();
			if (obj != 0)
			{
				IME_DEBUG("add card objid %u", obj);
				pRole->GetclsCardPacket()->Find(obj)->AddExp(exp);
				pRole->GetclsCardPacket()->Find(obj)->Calculate(true);
				pRole->GetclsCardPacket()->Calculate(true);
			}
		}
	}
}

void CGmCmdPro::WorldExpUp(CRole *pRole, const std::vector<std::string> &vecPara)
{
	if (vecPara.size() > 1)
	{
		int exp = atol(vecPara[1].c_str());
		pRole->SetdwWorldHistory(pRole->GetdwWorldHistory() + exp, true);
	}
}

void CGmCmdPro::AddShop(CRole *pRole, const std::vector<std::string> &vecPara)
{
	if (vecPara.size() > 2)
	{
		int id = atol(vecPara[1].c_str());
		int number = atol(vecPara[2].c_str());
		STC_CONF_GOODS *pConf = CConfGoods::Find(id);
		if (pConf == NULL)
		{
			IME_ERROR("can not find this shop index %u in shop csv", id);
			return;
		}

//		for (int i = 0; i < number; i++)
//		{
			pRole->GetCShop()->AddItem(id, SOURCE_GM, number);
//		}
		pRole->GetCShop()->HandlerShopInfo(id);
	}
}

void CGmCmdPro::ClearExtraSign(CRole *pRole, const std::vector<std::string> &vecPara)
{
	pRole->GetCActive()->SetExtraSign(0);
	pRole->GetCActive()->HandlerInfoSign();
}

void CGmCmdPro::EquipExpUp(CRole *pRole, const std::vector<std::string> &vecPara)
{
	if (vecPara.size() > 3)
	{
		uint32_t pos = atol(vecPara[1].c_str());
		uint32_t exp = atol(vecPara[2].c_str());
		uint32_t type = atol(vecPara[3].c_str());
		if (pos != 0)
		{
			uint32_t obj = pRole->GetclsCardPacket()->GetEquipObjId(pos - 1, type);
			if (obj != 0)
			{
//				IME_LOG("add card objid %u", obj);
				pRole->GetclsCardPacket()->Find(obj)->AddExp(exp);
				pRole->GetclsCardPacket()->Find(obj)->Calculate(true);
				pRole->GetclsCardPacket()->Calculate(true);
			}
		}
		else
		{
			uint32_t obj = pRole->GetclsCardPacket()->GetMasterEquipObjId(type);
			if (obj != 0)
			{
//				IME_LOG("add card objid %u", obj);
				pRole->GetclsCardPacket()->Find(obj)->AddExp(exp);
				pRole->GetclsCardPacket()->Find(obj)->Calculate(true);
				pRole->GetclsCardPacket()->Calculate(true);
			}
		}
	}
}

void CGmCmdPro::LuckUp(CRole *pRole, const std::vector<std::string> &vecPara)
{
	if (vecPara.size() > 1)
	{
		int number = atol(vecPara[1].c_str());
		pRole->SetdwLuck(pRole->GetdwLuck() + number);
	}
}

void CGmCmdPro::SignClear(CRole *pRole, const std::vector<std::string> &vecPara)
{
	pRole->GetCActive()->HandlerSignClear();
}

void CGmCmdPro::AddVipexp(CRole *pRole, const std::vector<std::string> &vecPara)
{
	if (vecPara.size() > 1)
	{
		int exp = atol(vecPara[1].c_str());
		pRole->ChangeVipExp(exp);
//		IME_LOG("vip exp %u lv %u", pRole->GetdwVipExp(), pRole->GetwVipLv());
		pRole->Calculate(true);
	}
}

void CGmCmdPro::BossPrize(CRole *pRole, const std::vector<std::string> &vecPara)
{
	if (vecPara.size() >= 4)
	{
		CFriendBoss::Prize tmp;
		tmp.userid = pRole->GetdwAccountId();
		tmp.username = pRole->GetstrUserName();
		tmp.masterindex = atol(vecPara[1].c_str());
		tmp.viewlv = atol(vecPara[2].c_str()); 
		tmp.lv = atol(vecPara[3].c_str());
		tmp.t = time(NULL);
		tmp.number = 2;
		CFriendBoss *pFb = (CFriendBoss *)(sWorld->GetWorldBoss()->GetData(pRole->GetdwAccountId()));
		if (pFb == NULL)
		{
			IME_ERROR("can not find this user %u friend boss data", pRole->GetdwAccountId());
			return;
		}
		pFb->AddPrize(tmp);	
		pFb->HandlerBossPrizeInfo(pRole);
	}
}

void CGmCmdPro::UDevote(CRole *pCRole, const std::vector<std::string> &vecPara)
{
	if (vecPara.size() >= 1)
	{
		int devote = atol(vecPara[1].c_str());
		pCRole->GetCActive()->ChangeDevote(devote);
	}
}

void CGmCmdPro::Charge(CRole *pCRole, const std::vector<std::string> &vecPara)
{
	if (vecPara.size() >= 1)
	{
		int goodsid = atol(vecPara[1].c_str());
		STC_CONF_STORE *pConf = CConfStore::Find(goodsid);
		if (pConf == NULL)
		{
			IME_ERROR("can not find this goods %u", goodsid);
			return;
		}
		if (pConf->byObjType != STORE_OBJ_RMB)
		{
			IME_ERROR("this goodsid %u is not charge goods", goodsid);
			return;
		}
		CenterDBCtrl::InsertCharge(goodsid, pCRole->GetdwAccountId(), 1);
	}
}

void CGmCmdPro::CardPlusOne(CRole *pCRole, const std::vector<std::string> &vecPara)
{
	if (vecPara.size() < 2)
		return;
	uint32_t val = atol(vecPara[1].c_str());
	int attack = atol(vecPara[2].c_str());
	int defence = atol(vecPara[3].c_str());
	int health = atol(vecPara[4].c_str());
	uint32_t objid;
	pCRole->GetclsCardPacket()->CardAddByParaWithPlus(val, objid, attack, defence, health, SOURCE_GM);
	IME_DEBUG("add plus card");
	pCRole->GetclsCardPacket()->SingleCardAddUpdate(objid);
}

void CGmCmdPro::StaminaAdd(CRole *pRole, const std::vector<std::string> &vecPara)
{
	if (vecPara.size() < 2)
		return;
    int val = atol(vecPara[1].c_str());
//    unsigned int Energy = pRole->GetdwEnergy(); 
//    Energy += val;
//    pRole->SetdwEnergy(Energy);
	pRole->ChangeStaminaNoLimit(val);
    pRole->SendProUpdate();
	pRole->HandlerInfoOpt();
}
void CGmCmdPro::AddVitality(CRole *pRole, const std::vector<std::string> &vecPara){
	if (vecPara.size() < 2)
		return;
	pRole->GetCMission()->GMAddActive(atol(vecPara[1].c_str()));
	pRole->GetCMission()->HandlerMissionBaseInfo();
	pRole->GetCMission()->HandlerMissionActiveInfo();
}

void CGmCmdPro::LimitRole(CRole *pRole, const std::vector<std::string> &vecPara){
	if (vecPara.size() < 2)
			return;
	uint32 userId = atol(vecPara[1].c_str());
	uint32 isLimit = atol(vecPara[2].c_str());
	CDBCtrl::setLimitRolelogin(userId,isLimit);

}
