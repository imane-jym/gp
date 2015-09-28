/*
 * =====================================================================================
 *
 *       Filename:  CardUp.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2012年10月16日 11时01分55秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  imane (), imanecr@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include "CardUp.h"
#include "Card.h"
#include "CardPacket.h"
#include "ErrorMsg.h"
#include "EnumDefines.h"
#include "CmdDefine.h"
#include "ConfigData.h"
#include "Role.h"
#include "WorldPacket.h"
#include "Notice.h"
#include <algorithm>
#include "Mission.h"

void CCardUp::HandlerSpecialCardStrength(CRole *pCRole, WorldPacket &pkg)
{
	uint32_t objid;
	uint16_t conNumber;

	uint32_t val32;
	uint32_t AddExp = 0;
	uint32_t location;
	uint32_t conCoin = 0;

	IME_CHAR_LOG("action [userid %u strengthen card contain skill and jewel]", pCRole->GetdwAccountId());
	std::vector<CCard *> vecconCard;
	pkg >> objid;
	pkg >> conNumber;
	if (conNumber == 0)
	{
		IME_ERROR("consume card number is zero");
		pCRole->SendErrorMsg(ERRNO_MSG_DATA_ILLEGAL, CMD_SC_SPECIAL_CARD_STRENGTH_RESULT);
		return;
	}
	CCard *p = pCRole->GetclsCardPacket()->Find(objid);
	CCard *tempq;
	if (p == NULL)
	{
		IME_ERROR("can not find the objid [%u] in packet", objid);
		pCRole->SendErrorMsg(ERRNO_MSG_CSV_DATA, CMD_SC_SPECIAL_CARD_STRENGTH_RESULT);
		return;
	}

	int maxlevel = p->GetdwGrowLimit() + p->GetdwLimitBreak() * BREAK_LIMIT_PARA + (p->GetbyRare() == E_CR_AWAKE ? AWAKE_LEVEL_PARA : 0); 
	if (p->GetdwExp() >= CConfCardUp::GetMaxExp(p->GetbyStarLevel(), p->GetdwExpGrow(), maxlevel - 1))
	{
		pCRole->SendErrorMsg(ERRNO_MSG_CARD_FULL_LEVEL, CMD_SC_SPECIAL_CARD_STRENGTH_RESULT);
		return;
	}
//	if (p->GetwLevel() >= maxlevel)
//	{
//		pCRole->SendErrorMsg(ERRNO_MSG_CARD_FULL_LEVEL, CMD_SC_SPECIAL_CARD_STRENGTH_RESULT);
//		return;
//	}

	if (pCRole->GetwLevel() * STRENGTH_LEVEL_LIMIT_PARA <= p->GetwLevel())
	{
		pCRole->SendErrorMsg(ERRNO_MSG_CARD_LEVEL_OVERLOAD_ROLE, CMD_SC_SPECIAL_CARD_STRENGTH_RESULT);
		return;
	}

	STC_CONF_CARD_BASE *pConf;
	if (p->GetbyType() == E_ITEM_JEWEL_CARD)
	{
		for (int i = 0; i < conNumber; i++)
		{
			pkg >> val32;	
			if (val32 == objid)
			{
				IME_ERROR("must not strength the card by itself");
				pCRole->SendErrorMsg(ERRNO_MSG_NOT_ALLOW_SAME_CARD, CMD_SC_SPECIAL_CARD_STRENGTH_RESULT);
				return;
			}
			tempq = pCRole->GetclsCardPacket()->Find(val32);
			if (tempq == NULL)
			{
				IME_ERROR("can not find the objid [%u] in packet", val32);
				pCRole->SendErrorMsg(ERRNO_MSG_CSV_DATA, CMD_SC_SPECIAL_CARD_STRENGTH_RESULT);
				return;
			}
			vecconCard.push_back(tempq);
//			pConfCardUp = CConfCardUp::Find(tempq->GetbyStarLevel(), tempq->GetwLevel());
//			if (pConfCardUp == NULL)
//			{
//				IME_ERROR("can not find the index [%u] in cardup csv", tempq->GetbyStarLevel());
//				pCRole->SendErrorMsg(ERRNO_MSG_CSV_DATA, CMD_SC_SPECIAL_CARD_STRENGTH_RESULT);
//				return;
//			}
//			AddExp += pConfCardUp->dwAddExp;
			pConf = CConfCardBase::Find(tempq->GetdwIndex());
			if (pConf == NULL)
			{
				IME_ERROR("can not find the index [%u] in card csv", tempq->GetdwIndex());
				pCRole->SendErrorMsg(ERRNO_MSG_CSV_DATA, CMD_SC_SPECIAL_CARD_STRENGTH_RESULT);
				return;
			}
			AddExp += pConf->dwAddExp + tempq->GetdwExp() * GlobalConfig::StrengthJewelPara / 100.0;

			if (tempq->GetbyType() != E_ITEM_JEWEL_CARD)
			{
				IME_ERROR("consume card type is unvalid: card type[%u] consume card type[%u]", p->GetbyType(), tempq->GetbyType());
				pCRole->SendErrorMsg(ERRNO_MSG_CSV_DATA, CMD_SC_SPECIAL_CARD_STRENGTH_RESULT);
				return;
			}

			uint8_t team;
			if (pCRole->GetclsCardPacket()->IsInEquip(tempq, location, team))
			{
				IME_ERROR("the consume card's objid[%u] location [%u] is in equip", tempq->GetdwObjId(), location);
				pCRole->SendErrorMsg(ERRNO_MSG_CONSUME_CARD_IN_EQUIP, CMD_SC_SPECIAL_CARD_STRENGTH_RESULT);
				return;
			}

		}

//		STC_CONF_CARD_UP_CONSUME *pConfCardUpCon = CConfCardUpConsume::Find(p->GetbyStarLevel());
//		if (pConfCardUpCon == NULL)
//		{
//			IME_ERROR("can not find the index [%u] in cardupconsume csv", p->GetbyStarLevel());
//			pCRole->SendErrorMsg(ERRNO_MSG_CSV_DATA, CMD_SC_SPECIAL_CARD_STRENGTH_RESULT);
//			return;
//		}
		conCoin = p->GetdwConsumeCoin() * p->GetwLevel() * conNumber;
		if (pCRole->GetdwCoin() < conCoin)
		{
			IME_ERROR("it have not enough coin userid %u", pCRole->GetdwAccountId());
			pCRole->SendErrorMsg(ERRNO_MSG_NOT_ENOUGH_COIN, CMD_SC_SPECIAL_CARD_STRENGTH_RESULT);
			return;
		}

		pCRole->ChangeCoin(-conCoin, SOURCE_CARDUP);
		pCRole->SendProUpdate();

	}
	else if (p->GetbyType() == E_ITEM_SKILL_CARD)
	{
		uint32_t proup = 0;
		if (conNumber > 5 || conNumber < 1)
		{
			IME_ERROR("consume card number is not equal 1~5 range");
			pCRole->SendErrorMsg(ERRNO_MSG_DATA_ILLEGAL, CMD_SC_SPECIAL_CARD_STRENGTH_RESULT);
			return;
		}
//		STC_CONF_SKILL_LVUP *pConfSkillLvUp = CConfSkillLvUp::Find(p->GetdwIndex());
//		if (pConfSkillLvUp == NULL)
//		{
//			IME_ERROR("can not find skill card lv up csv index %u", p->GetdwIndex());
//			pCRole->SendErrorMsg(ERRNO_MSG_CSV_LOAD, CMD_SC_SPECIAL_CARD_STRENGTH_RESULT);
//			return;
//		}
		if (p->GetdwGrowLimit() <= p->GetwLevel())
		{
			IME_ERROR("this skill card have been full level obji %u, index %u, level %u",p->GetdwObjId(), p->GetdwIndex(), p->GetwLevel()); 
			pCRole->SendErrorMsg(ERRNO_MSG_CARD_FULL_LEVEL, CMD_SC_SPECIAL_CARD_STRENGTH_RESULT);
			return;
		}

		conCoin = p->GetdwConsumeCoin() * p->GetwLevel() * conNumber;
		if (pCRole->GetdwCoin() < conCoin)
		{
			IME_ERROR("it have not enough coin userid %u", pCRole->GetdwAccountId());
			pCRole->SendErrorMsg(ERRNO_MSG_NOT_ENOUGH_COIN, CMD_SC_SPECIAL_CARD_STRENGTH_RESULT);
			return;
		}
		
		int numeratorstar = 0;	
		int numeratorlv = 0;	
		for (int i = 0; i < conNumber; i++)
		{
			pkg >> val32;	
			if (val32 == objid)
			{
				IME_ERROR("must not strength the card by itself");
				pCRole->SendErrorMsg(ERRNO_MSG_NOT_ALLOW_SAME_CARD, CMD_SC_SPECIAL_CARD_STRENGTH_RESULT);
				return;
			}
			tempq = pCRole->GetclsCardPacket()->Find(val32);
			if (tempq == NULL)
			{
				IME_ERROR("can not find the objid [%u] in packet", val32);
				pCRole->SendErrorMsg(ERRNO_MSG_CSV_DATA, CMD_SC_SPECIAL_CARD_STRENGTH_RESULT);
				return;
			}
			proup += tempq->GetbyStarLevel() * SKILL_CARD_LEVELUP_STAR_UP + tempq->GetwLevel() * SKILL_CARD_LEVELUP_LEVEL_UP;

//			if (tempq->GetdwIndex() != p->GetdwIndex() || tempq->GetwLevel() != p->GetwLevel())
//			{
//				IME_ERROR("consume card index is unvalid: card type[%u] consume card type[%u]", p->GetdwIndex(), tempq->GetdwIndex());
//				pCRole->SendErrorMsg(ERRNO_MSG_DATA_ILLEGAL, CMD_SC_SPECIAL_CARD_STRENGTH_RESULT);
//				return;
//			}
			int IsSame = 0;
			if (tempq->GetdwIndex() == p->GetdwIndex())
				IsSame = 1;

			uint8_t team;
			if (pCRole->GetclsCardPacket()->IsInEquip(tempq, location, team))
			{
				IME_ERROR("the consume card's objid[%u] location [%u] is in equip", tempq->GetdwObjId(), location);
				pCRole->SendErrorMsg(ERRNO_MSG_CONSUME_CARD_IN_EQUIP, CMD_SC_SPECIAL_CARD_STRENGTH_RESULT);
				return;
			}
			vecconCard.push_back(tempq);
			numeratorstar += tempq->GetbyStarLevel();
			numeratorlv += tempq->GetwLevel();

		}
		uint16_t pro = numeratorstar * 1.0 / (p->GetbyStarLevel() * std::max((int)p->GetbyStarLevel(), SP_STAR_LEVEL)) * numeratorlv * 1.0 / (p->GetwLevel() * std::max((int)p->GetwLevel(), SP_LEVEL)) * 100;
		uint32_t tpro = pCRole->GetdwExtraStrengthPro();
		tpro = tpro / SKILL_CARD_LEVELUP_RATE_PARA;
		tpro = tpro > SKILL_CARD_LEVELUP_FAIL_PRO_MAX ? SKILL_CARD_LEVELUP_FAIL_PRO_MAX : tpro;
		pro += tpro;
		if (pro > SKILL_CARD_LEVELUP_PRO_MAX) pro = SKILL_CARD_LEVELUP_PRO_MAX;
		IME_DEBUG("pro %u, extra up %u", pro, pCRole->GetdwExtraStrengthPro());
		if (CUtil::RandEither(pro, 100))
		{
			if (pCRole->GetdwExtraStrengthPro() != 0)
			{
				pCRole->SetdwExtraStrengthPro(0);
				pCRole->HandlerInfoOpt();
			}
			AddExp = p->GetdwExpLimit() - p->GetdwLastExpLimit();
		}
		else
		{
			pCRole->SetdwExtraStrengthPro(pCRole->GetdwExtraStrengthPro() + proup);
			pCRole->HandlerInfoOpt();
			AddExp = 0;
		}
		pCRole->ChangeCoin(-conCoin, SOURCE_CARDUP);
		pCRole->SendProUpdate();
	}
	else
	{
		IME_ERROR("card type is unvalid %u", p->GetbyType());
		pCRole->SendErrorMsg(ERRNO_MSG_DATA_ILLEGAL, CMD_SC_SPECIAL_CARD_STRENGTH_RESULT);
		return;
	}

	for(int i = 0; i < vecconCard.size(); i++)
	{
		pCRole->GetclsCardPacket()->CardDel(vecconCard[i]->GetdwObjId(), SOURCE_CARDUP);
		pCRole->GetclsCardPacket()->SingleCardDelUpdate(vecconCard[i]->GetdwObjId());
	}		
	
	uint8_t IsSuccess = 1;
	if (AddExp != 0)
	{
		IsSuccess = 0;
	}

	p->AddExp(AddExp);
	p->Calculate(true);
	if (p->GetbyType() == E_ITEM_JEWEL_CARD)
	{	
		if (p->GetwLevel() >= 3)
		{
			sWorld->GetCNotice()->StrengthCard(pCRole->GetstrUserName(), p->GetdwIndex(), p->GetwLevel());
		}
	}
	else
	{
		if (AddExp != 0)
		{
			sWorld->GetCNotice()->SkillLvUp(pCRole->GetstrUserName(), p->GetdwIndex() * 100 + p->GetwLevel());
		}
	}

	uint8_t team;
	if (pCRole->GetclsCardPacket()->IsInEquip(p, location, team))
	{
		pCRole->GetclsCardPacket()->Calculate(true);
	}

	WorldPacket info(CMD_SC_SPECIAL_CARD_STRENGTH_RESULT);
	info << (uint16_t) 0;
	info << (uint32_t) conCoin;
	info << (uint32_t) objid;
	info << (uint8_t) IsSuccess;
	pCRole->SendPacket(&info);
}

//void CCardUp::HandlerCardSkillFlush(CRole *pCRole, WorldPacket &pkg)
//{
//	uint32_t cardobjid;
//	uint8_t position;
//	uint32_t attr;
//	uint32_t team;
//	uint32_t pro;
//	uint32_t skillbook;
//	pkg >> cardobjid;
//	pkg >> position;
//	pkg >> attr;
//	pkg >> team;
//	pkg >> pro;
//	pkg >> skillbook;
//	
//	CShopEffect::SEffect data;
//	data.para1 = attr;
//	data.para2 = team;
//	data.para3 = pro;
//	data.para4 = skillbook;
//	
//	if (attr != 0 && !pCRole->GetCShop()->IsHave(attr))
//	{
//		IME_ERROR("userid %u have not this shop item id %u", pCRole->GetdwAccountId(), attr);
//		pCRole->SendErrorMsg(ERRNO_MSG_SHOP_ITEM_HAVE_NOT, CMD_SC_SKILL_FLUSH_RESULT);	
//		return;	
//	}
//
//	if (team != 0 && !pCRole->GetCShop()->IsHave(team))
//	{
//		IME_ERROR("userid %u have not this shop item id %u", pCRole->GetdwAccountId(), team);
//		pCRole->SendErrorMsg(ERRNO_MSG_SHOP_ITEM_HAVE_NOT, CMD_SC_SKILL_FLUSH_RESULT);	
//		return;	
//	}
//
//	if (pro != 0 && !pCRole->GetCShop()->IsHave(pro))
//	{
//		IME_ERROR("userid %u have not this shop item id %u", pCRole->GetdwAccountId(), pro);
//		pCRole->SendErrorMsg(ERRNO_MSG_SHOP_ITEM_HAVE_NOT, CMD_SC_SKILL_FLUSH_RESULT);	
//		return;
//	}
//	
//	if (skillbook == 0 || !pCRole->GetCShop()->IsHave(skillbook))
//	{
//		IME_ERROR("userid %u have not this shop item id %u", pCRole->GetdwAccountId(), skillbook);
//		pCRole->SendErrorMsg(ERRNO_MSG_SHOP_ITEM_HAVE_NOT, CMD_SC_SKILL_FLUSH_RESULT);	
//		return;
//	}
//
//	if (CShopEffect::SkillCombineBookEffect(pCRole, cardobjid, position, data))
//	{
//		IME_ERROR("skill flush fail user_id=%u cardobjid=%u position=%u attr=%u team=%u pro=%u skillbook=%u",pCRole->GetdwAccountId(), cardobjid, position, attr, team, pro, skillbook);
//		pCRole->SendErrorMsg(ERRNO_MSG_SKILL_COMBINE_FAIL, CMD_SC_SKILL_FLUSH_RESULT);
//		return;
//	}	
//
//	pCRole->GetCShop()->DelItem(attr);
//	pCRole->GetCShop()->DelItem(team);
//	pCRole->GetCShop()->DelItem(pro);
//	pCRole->GetCShop()->DelItem(skillbook);
//
//	WorldPacket info(CMD_SC_SKILL_FLUSH_RESULT);
//	info << (uint16_t) 0;
//	info << (uint32_t)data.para1;
//	info << data.para2;
//	info << data.para3;
//	info << data.para4;
//	pCRole->SendPacket(&info);
//}

void CCardUp::HandlerNormalCardStrength(CRole *pCRole, WorldPacket &pkg)
{
	uint32_t objid;
	uint32_t conNumber;
	uint32_t conCoin;

	uint32_t AddExp = 0;
	uint32_t location;

	pkg >> objid;
	pkg >> conNumber;
	IME_CHAR_LOG("action [userid %u strengthen normal card such as equip]", pCRole->GetdwAccountId());
	
	if (conNumber != 0)
	{
		IME_ERROR("can not support consume coin is not 0");
		pCRole->SendErrorMsg(ERRNO_MSG_DATA_ILLEGAL, CMD_SC_NORMAL_CARD_STRENGTH_RESULT);
		return;
	}

	CCard *p = pCRole->GetclsCardPacket()->Find(objid);
	if (p == NULL)
	{
		IME_ERROR("can not find the objid [%u] in packet", objid);
		pCRole->SendErrorMsg(ERRNO_MSG_CSV_DATA, CMD_SC_NORMAL_CARD_STRENGTH_RESULT);
		return;
	}

	if (pCRole->GetwLevel() * STRENGTH_LEVEL_LIMIT_PARA <= p->GetwLevel())
	{
		pCRole->SendErrorMsg(ERRNO_MSG_CARD_LEVEL_OVERLOAD_ROLE, CMD_SC_NORMAL_CARD_STRENGTH_RESULT);
		return;
	}

	int maxlevel = p->GetdwGrowLimit() + p->GetdwLimitBreak() * BREAK_LIMIT_PARA + (p->GetbyRare() == E_CR_AWAKE ? AWAKE_LEVEL_PARA : 0); 
	if (p->GetdwExp() >= CConfCardUp::GetMaxExp(p->GetbyStarLevel(), p->GetdwExpGrow(), maxlevel - 1))
	{
		pCRole->SendErrorMsg(ERRNO_MSG_CARD_FULL_LEVEL, CMD_SC_NORMAL_CARD_STRENGTH_RESULT);
		return;
	}
//	if (p->GetwLevel() >= maxlevel)
//	{
//		pCRole->SendErrorMsg(ERRNO_MSG_CARD_FULL_LEVEL, CMD_SC_NORMAL_CARD_STRENGTH_RESULT);
//		return;
//	}

	STC_CONF_CARD_BASE *pConf;
	pConf = CConfCardBase::Find(p->GetdwIndex());
	if (pConf == NULL)
	{
		IME_ERROR("can not find the index [%u] in cardup csv", p->GetbyStarLevel());
		pCRole->SendErrorMsg(ERRNO_MSG_CSV_DATA, CMD_SC_NORMAL_CARD_STRENGTH_RESULT);
		return;
	}

	if (p->GetbyType() == E_ITEM_WEAPON_CARD || p->GetbyType() == E_ITEM_ARMOR_CARD || p->GetbyType() == E_ITEM_ACCESSORIES_CARD)
	{
	}
	else
	{
		IME_ERROR("consume card type is unvalid: card type[%u]", p->GetbyType());
		pCRole->SendErrorMsg(ERRNO_MSG_CSV_DATA, CMD_SC_NORMAL_CARD_STRENGTH_RESULT);
		return;
	}

	STC_CONF_STRENGTH_CRIT_PRO *pConfCrit = CConfStrengthCritPro::Find(p->GetwLevel());
	if (pConfCrit == NULL)
	{
		IME_ERROR("can not find index %u in strength crit csv", p->GetwLevel());
		pCRole->SendErrorMsg(ERRNO_MSG_CSV_LOAD, CMD_SC_NORMAL_CARD_STRENGTH_RESULT);
		return;
	}

	uint32_t ExtraExp = 0;
	uint32_t CritNumber = 0;
	if (conNumber == 0)
	{
//		if (pCRole->GetwVipLv() != 0)
//		{
			STC_CONF_VIP *pVip = CConfVip::Find(pCRole->GetwVipLv());
			if (pVip == NULL)
			{
				IME_ERROR("user vip lv cannot record in vip csv %u", pCRole->GetdwAccountId());
			}
			else
			{
				std::vector<int> wtmp;
				wtmp.push_back(pConfCrit->dwVip1);
				wtmp.push_back(pConfCrit->dwVip2);
				wtmp.push_back(pConfCrit->dwVip3);
				wtmp.push_back(pConfCrit->dwVip4);
				wtmp.push_back(pConfCrit->dwVip5);
				wtmp.push_back(pConfCrit->dwVip6);
				for(int i = pVip->dwCritExtraLevel; i < wtmp.size(); i++)
				{
					wtmp[i] = 0;
				}
				int choose = CUtil::RandFactor(wtmp);

				CritNumber = choose;	
				int tmplevel = p->GetwLevel() + CritNumber;
				tmplevel = tmplevel > maxlevel ? maxlevel : tmplevel;
				IME_DEBUG("up to level %u", tmplevel);
				ExtraExp = CConfCardUp::GetExp(p->GetbyStarLevel(), p->GetdwExpGrow(), tmplevel);
			}
//		}
	}
	
	if (conNumber == 0)
	{
		AddExp = p->GetdwExpLimit() - p->GetdwExp();
//		conCoin = pConf->dwConsumeCoin * AddExp;
		conCoin = CConfStrengthConsume::GetConsume(p->GetbyStarLevel(), p->GetwLevel());
		if (ExtraExp != 0)
		{
			long int tmp = (long int)ExtraExp - p->GetdwExp();
			AddExp = tmp < 0 ? 0 : tmp;
		}
	}
	else
	{
		AddExp = conNumber / pConf->dwConsumeCoin; 
		conCoin = conNumber;
	}

	IME_DEBUG("con %u limit %u lastlimit %u exp %u, addexp %u, conCoin %u, star %u", conNumber, p->GetdwExpLimit(), p->GetdwLastExpLimit(), p->GetdwExp(), AddExp, conCoin, p->GetbyStarLevel());
	
	if (pCRole->GetdwCoin() < conCoin)
	{
		IME_ERROR("it have not enough coin userid %u", pCRole->GetdwAccountId());
		pCRole->SendErrorMsg(ERRNO_MSG_NOT_ENOUGH_COIN, CMD_SC_NORMAL_CARD_STRENGTH_RESULT);
		return;
	}
	
	p->SetdwTotalConsumeCoin(p->GetdwTotalConsumeCoin() + conCoin);
	pCRole->ChangeCoin(-conCoin, SOURCE_CARDUP);
	pCRole->SendProUpdate();

	IME_DEBUG("strength now level %u, crit number %u, ExtraExp %u", p->GetwLevel(), CritNumber, ExtraExp);
	p->AddExp(AddExp);
	p->Calculate(true);
	IME_DEBUG("after add exp strength now level %u", p->GetwLevel());

	uint8_t team;
	if (pCRole->GetclsCardPacket()->IsInEquip(p, location, team))
	{
		pCRole->GetclsCardPacket()->Calculate(true);
	}

	if (p->GetwLevel() >= 80 && p->GetwLevel() % 10 <= 3)
	{
		sWorld->GetCNotice()->StrengthCard(pCRole->GetstrUserName(), p->GetdwIndex(), p->GetwLevel());
	}

	pCRole->GetCMission()->EveryStrength(0, 1);

	WorldPacket info(CMD_SC_NORMAL_CARD_STRENGTH_RESULT);
	info << (uint16_t) 0;
	info << (uint32_t) conCoin;
	info << (uint32_t) objid;
	info << (uint32_t) CritNumber;
	pCRole->SendPacket(&info);
}

void CCardUp::HandlerSwitchCardExp(CRole *pCRole, WorldPacket &pkg)
{
	uint32_t objid;
	uint32_t val;
//	uint32_t conobjid;
	std::vector<uint32_t> vecconCard;
	uint8_t flag;
	uint32_t AddExp;
	uint32_t location;
	uint32_t switchnumber;

	uint8_t IsSame = 1;
	uint32_t CostCoin = 0;

	uint16_t attackplus = 0;
	uint16_t defenceplus = 0;
	uint16_t healthplus = 0;

	uint16_t leaderskillnumber = 0;

	IME_CHAR_LOG("action [userid %u switch card exp]", pCRole->GetdwAccountId());
	pkg >> objid;
	pkg >> switchnumber;
	for (int i = 0; i < switchnumber; i++)
	{
		pkg >> val;
		vecconCard.push_back(val);
	}
	pkg >> flag;

	if (switchnumber > MAX_SWITCH_CARD_NUMBER)
	{
		IME_ERROR("switch card number %u is so big", switchnumber);
		pCRole->SendErrorMsg(ERRNO_MSG_DATA_ILLEGAL, CMD_SC_SWITCH_CARD_EXP_RESULT);
		return;
	}

	CCard *p = pCRole->GetclsCardPacket()->Find(objid);
//	CCard *pP = pCRole->GetclsCardPacket()->Find(conobjid);
	if (p == NULL)
	{
		IME_ERROR("can not find the objid [%u] in packet", objid);
		pCRole->SendErrorMsg(ERRNO_MSG_CSV_DATA, CMD_SC_SWITCH_CARD_EXP_RESULT);
		return;
	}
	if (!(p->GetbyType() == E_ITEM_PET_CARD || p->GetbyType() == E_ITEM_MASTER_CARD))
	{
		IME_ERROR("card type is unvalid %u", p->GetbyType());
		pCRole->SendErrorMsg(ERRNO_MSG_DATA_ILLEGAL, CMD_SC_SWITCH_CARD_EXP_RESULT);
		return;
	}
	STC_CONF_LEADER_SKILL *pConfLeader = CConfLeaderSkill::Find(p->GetdwLeaderIndex());
	if (p->GetdwLeaderIndex() != 0)
	{
		if (pConfLeader == NULL)
		{
			IME_ERROR("leader skill is unvalid %u", p->GetdwLeaderIndex());
			pCRole->SendErrorMsg(ERRNO_MSG_CSV_LOAD, CMD_SC_SWITCH_CARD_EXP_RESULT);
			return;
		}
	}

//	if (pP->GetwLevel() < 2)
//	{
//		IME_ERROR("switch exp consume card level is 1");
//		pCRole->SendErrorMsg(ERRNO_MSG_CARD_SWITCH_LEVEL_ONE, CMD_SC_SWITCH_CARD_EXP_RESULT);
//		return;
//	}
	
	for (int i = 0; i < vecconCard.size(); i++)
	{
		CCard *pP = pCRole->GetclsCardPacket()->Find(vecconCard[i]);
		if (pP == NULL)
		{
			IME_ERROR("can not find the objid [%u] in packet", vecconCard[i]);
			pCRole->SendErrorMsg(ERRNO_MSG_CSV_LOAD, CMD_SC_SWITCH_CARD_EXP_RESULT);
			return;
		}
		uint8_t team;
		if (pCRole->GetclsCardPacket()->IsInEquip(pP, location, team))
		{
			IME_ERROR("the consume card's objid[%u] location [%u] is in equip", pP->GetdwObjId(), location);
			pCRole->SendErrorMsg(ERRNO_MSG_CONSUME_CARD_IN_EQUIP, CMD_SC_SWITCH_CARD_EXP_RESULT);
			return;
		}
		if (!(pP->GetbyType() == E_ITEM_PET_CARD || pP->GetbyType() == E_ITEM_MASTER_CARD))
		{
			IME_ERROR("card type is unvalid %u", pP->GetbyType());
			pCRole->SendErrorMsg(ERRNO_MSG_DATA_ILLEGAL, CMD_SC_SWITCH_CARD_EXP_RESULT);
			return;
		}

//		if (p->GetdwIndex() != pP->GetdwIndex())
//		{
//			IsSame = 0;
//		}

		if (p->GetdwCardGroup() != pP->GetdwCardGroup())
		{
			IsSame = 0;
		}

		attackplus += pP->GetwAttackPlus();
		defenceplus += pP->GetwDefencePlus();
		healthplus += pP->GetwHealthPlus();
	}

	STC_CONF_GOODS *pConfShop;
	if (flag == 0)
	{
//		pConfShop = CConfGoods::Find(CARD_SWITCH_EXP_NORMAL);
	}
	else
	{
		pConfShop = CConfGoods::Find(CARD_SWITCH_EXP_HIGH);
	

		if (pConfShop == NULL)
		{
			IME_ERROR("can not find this index switch normal or high flag %u in shop csv", flag);
			pCRole->SendErrorMsg(ERRNO_MSG_CSV_LOAD, CMD_SC_SWITCH_CARD_EXP_RESULT);
			return;
		}
		if (pConfShop->dwLevelLimit != 0)
		{
			if (pCRole->GetwLevel() < pConfShop->dwLevelLimit)
			{
				pCRole->SendErrorMsg(ERRNO_MSG_SHOP_LEVEL_LIMIT, CMD_SC_SWITCH_CARD_EXP_RESULT);
				return;
			}
		}
	}

	int maxlevel = p->GetdwGrowLimit() + p->GetdwLimitBreak() * BREAK_LIMIT_PARA + (p->GetbyRare() == E_CR_AWAKE ? AWAKE_LEVEL_PARA : 0); 
	if (p->GetdwExp() >= CConfCardUp::GetMaxExp(p->GetbyStarLevel(), p->GetdwExpGrow(), maxlevel - 1))
	{
		if (!IsSame)
		{
			pCRole->SendErrorMsg(ERRNO_MSG_SWITCH_SAME_CARD, CMD_SC_SWITCH_CARD_EXP_RESULT);
			return;
		}
		
		if (p->GetbyRare() == E_CR_AWAKE && p->GetdwLimitBreak() >= p->GetdwLimitBreakNumber())
		{
			pCRole->SendErrorMsg(ERRNO_MSG_SWITCH_CARD_FULL, CMD_SC_SWITCH_CARD_EXP_RESULT);
			return;
		}
	}

//	if (p->GetwLevel() >= maxlevel)
//	{
//		pCRole->SendErrorMsg(ERRNO_MSG_CARD_FULL_LEVEL, CMD_SC_SWITCH_CARD_EXP_RESULT);
//		return;
//	}

	if (pCRole->GetwLevel() * STRENGTH_LEVEL_LIMIT_PARA <= p->GetwLevel())
	{
		pCRole->SendErrorMsg(ERRNO_MSG_CARD_ROLE_LEVEL_OVERLOAD_ROLE, CMD_SC_SWITCH_CARD_EXP_RESULT);
		return;
	}

	if (flag == 0)
	{
//		if (!pCRole->GetCShop()->IsHave(CARD_SWITCH_EXP_NORMAL))
//		{
//			IME_ERROR("have not this shop item index %u", CARD_SWITCH_EXP_NORMAL);
//			pCRole->SendErrorMsg(ERRNO_MSG_SHOP_ITEM_HAVE_NOT, CMD_SC_SWITCH_CARD_EXP_RESULT);
//			return;
//		}
	}
	else
	{
		if (!pCRole->GetCShop()->IsHave(CARD_SWITCH_EXP_HIGH))
		{
			IME_ERROR("have not this shop item index %u", CARD_SWITCH_EXP_NORMAL);
			pCRole->SendErrorMsg(ERRNO_MSG_SHOP_ITEM_HAVE_NOT, CMD_SC_SWITCH_CARD_EXP_RESULT);
			return;
		}

	}

	STC_CONF_CARD_BASE *pConf;
	CCard *pP = NULL;
	AddExp = 0;
	std::vector<CCard *> vecCCard;
	for (int i = 0; i < vecconCard.size(); i++)
	{
		pP = pCRole->GetclsCardPacket()->Find(vecconCard[i]);

		if (flag == 0)
		{
			AddExp += (pP->GetdwExp() + pP->GetdwAddExp()) * CARD_SWITCH_EXP_NORMAL_EFFECT / 100.0;
//			pCRole->GetCShop()->DelItem(CARD_SWITCH_EXP_NORMAL);
//			pCRole->GetCShop()->HandlerShopInfo(CARD_SWITCH_EXP_NORMAL);
		}
		else
		{
			AddExp += (pP->GetdwExp() + pP->GetdwAddExp()) * CARD_SWITCH_EXP_HIGH_EFFECT / 100.0;
		}
		vecCCard.push_back(pP);

		if (p->GetdwLeaderIndex() != 0 && p->GetdwLeaderIndex() == pP->GetdwLeaderIndex())
		{
			leaderskillnumber++;	
		}
	}

//	CostCoin = (GlobalConfig::SwitchCost / 100.0) * AddExp;
	CostCoin = (GlobalConfig::SwitchCost / 100.0) * p->GetwLevel() * (uint32_t)vecCCard.size();
	if (pCRole->GetdwCoin() < CostCoin)
	{
		IME_ERROR("userid %u coin is not enough, cost coin %u", pCRole->GetdwCoin(), CostCoin);
		pCRole->SendErrorMsg(ERRNO_MSG_NOT_ENOUGH_COIN, CMD_SC_SWITCH_CARD_EXP_RESULT);
		return;
	}
	if (flag != 0)
	{
		pCRole->GetCShop()->DelItem(CARD_SWITCH_EXP_HIGH, SOURCE_CARDUP);
		pCRole->GetCShop()->HandlerShopInfo(CARD_SWITCH_EXP_HIGH);
	}

	if (p->GetwLeaderLevel() != 0)
	{
		for (int j = leaderskillnumber; j > 0; j--)
		{
			if (p->GetwLeaderLevel() < pConfLeader->dwMaxlv)
			{
				if (CConfLeaderSkill::IsLevelUp(pConfLeader->dwIndex, p->GetwLeaderLevel()))
				{
					p->SetwLeaderLevel(p->GetwLeaderLevel() + 1);
				}
			}
		}
	}

//	attackplus += p->GetwAttackPlus();
//	attackplus = attackplus > GlobalConfig::PlusMaxlimit ? GlobalConfig::PlusMaxlimit : attackplus;
//	defenceplus += p->GetwDefencePlus();
//	defenceplus = defenceplus > GlobalConfig::PlusMaxlimit ? GlobalConfig::PlusMaxlimit : defenceplus;
//	healthplus += p->GetwHealthPlus();
//	healthplus = healthplus > GlobalConfig::PlusMaxlimit ? GlobalConfig::PlusMaxlimit : healthplus;
//	p->SetwAttackPlus(attackplus);
//	p->SetwDefencePlus(defenceplus);
//	p->SetwHealthPlus(healthplus);
	p->ChangeAttackPlus(attackplus);
	p->ChangeDefencePlus(defenceplus);
	p->ChangeHealthPlus(healthplus);
	p->Calculate();

	SSwitchCard SwitchPara;
	SwitchPara.AddExp = AddExp;
	pCRole->GetclsCardPacket()->CardBreak(p, vecCCard, SwitchPara);
	for (int i = 0; i < vecCCard.size(); i++)
	{
		pCRole->GetclsCardPacket()->CardDel(vecCCard[i]->GetdwObjId(), SOURCE_CARDUP);
		pCRole->GetclsCardPacket()->SingleCardDelUpdate(vecCCard[i]->GetdwObjId());
	}

	pCRole->ChangeCoin(-CostCoin, SOURCE_CARDUP);

	uint8_t team;
	if (pCRole->GetclsCardPacket()->IsInEquip(p, location, team))
	{
		pCRole->GetclsCardPacket()->Calculate(true);
	}
	pCRole->SendProUpdate();

	WorldPacket info(CMD_SC_SWITCH_CARD_EXP_RESULT);
	info << (uint16_t) 0;
	info << (uint8_t) flag;
	info << (uint32_t) objid;
	pCRole->SendPacket(&info);
}

void CCardUp::HandlerSkillStrengthReview(CRole *pCRole, WorldPacket &pkg)
{
	uint32_t objid;
	pkg >> objid;
	uint32_t number;
	pkg >> number;
	int numeratorstar = 0;	
	int numeratorlv = 0;	
	uint32_t val32;
	uint32_t proup = 0;
	IME_CHAR_LOG("action [userid %u skill strength preview]", pCRole->GetdwAccountId());
	CCard *p = pCRole->GetclsCardPacket()->Find(objid);
	if (p == NULL)
	{
		IME_ERROR("can not find the objid [%u] in packet", objid);
		pCRole->SendErrorMsg(ERRNO_MSG_CSV_DATA, CMD_SC_SKILL_STRENGTH_REVIEW_RESULT);
		return;
	}

	for (int i = 0; i < number; i++)
	{
		pkg >> val32;	
		CCard *tempq = pCRole->GetclsCardPacket()->Find(val32);
		if (tempq == NULL)
		{
			IME_ERROR("can not find the objid [%u] in packet", val32);
			pCRole->SendErrorMsg(ERRNO_MSG_CSV_DATA, CMD_SC_SKILL_STRENGTH_REVIEW_RESULT);
			return;
		}
		proup += tempq->GetbyStarLevel() * SKILL_CARD_LEVELUP_STAR_UP + tempq->GetwLevel() * SKILL_CARD_LEVELUP_LEVEL_UP;

		numeratorstar += tempq->GetbyStarLevel();
		numeratorlv += tempq->GetwLevel();

	}

	uint16_t pro = numeratorstar * 1.0 / (p->GetbyStarLevel() * std::max((int)p->GetbyStarLevel(), SP_STAR_LEVEL)) * numeratorlv * 1.0 / (p->GetwLevel() * std::max((int)p->GetwLevel(), SP_LEVEL)) * 100;
	uint32_t tpro = pCRole->GetdwExtraStrengthPro();
	tpro = tpro / SKILL_CARD_LEVELUP_RATE_PARA;
	tpro = tpro > SKILL_CARD_LEVELUP_FAIL_PRO_MAX ? SKILL_CARD_LEVELUP_FAIL_PRO_MAX : tpro;
	pro += tpro;
	if (pro > SKILL_CARD_LEVELUP_PRO_MAX) pro = SKILL_CARD_LEVELUP_PRO_MAX;
//	IME_DEBUG("pro %u, extra up %u", pro, pCRole->GetdwExtraStrengthPro());
	WorldPacket info(CMD_SC_SKILL_STRENGTH_REVIEW_RESULT);
	info << (uint16_t)0;
	info << pro;
	pCRole->SendPacket(&info);
}
