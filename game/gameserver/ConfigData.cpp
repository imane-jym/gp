/*
 * ConfigData.cpp
 *
 *  Created on: 2011-7-5
 *      Author: yq
 */

#include "ConfigData.h"
#include "ConfigReader.h"
#include "Log.h"
#include <stdlib.h>
#include <vector>
#include <map>
#include <algorithm>
#include <string>
#include "Role.h"
#include "Util.h"
#include "RandomEvent.h"
#include "EnumDefines.h"
#include "CommonConfig.h"
#include "DBCtrl.h"
#include "World.h"

#define READER_INT(value) \
		do{\
		value = atol(reader.cell(i, j++).c_str());\
		}while(0)

#define READER_FLOAT(value) \
	do{\
	value = atof(reader.cell(i, j++).c_str());\
	}while(0)

#define READER_STR(value) \
	do{\
		value = reader.cell(i, j++);\
	}while(0)

#define IS_READER_ERROR \
        do{\
            if (reader.is_error()) \
            {\
            IME_ERROR("LOAD CONF ERROR"); \
            return false;\
            }\
        }while(0)

bool ConfInit(std::string strpath)
{
//    if (!CConfTest::Init(strpath + "Test.csv"))
//    {
//        IME_ERROR("load test csv fail");
//        return false;
//    }
    if (!CConfCardBase::Init(strpath + "Card.csv"))
    {
        IME_ERROR("load cardbase csv fail");
        return false;
    }
	if (!CConfGoods::Init(strpath + "Item.csv"))
	{
		IME_ERROR("load Goods.csv fail");
		return false;
	}
//    if (!CConfMissionBase::Init(strpath + "Mission.csv"))
//    {
//        IME_ERROR("load missionbase csv fail");
//        return false;
//    }
//    if (!CConfRandomEvent::Init(strpath + "RandomEvent.csv"))
//    {
//        IME_ERROR("load randomevent csv fail");
//        return false;
//    }
    if (!CConfCardCollection::Init(strpath + "CardDrop.csv"))
    {
        IME_ERROR("load carddrop csv fail");
        return false;
    }
    if (!CConfRole::Init(strpath + "Exp.csv"))
    {
        IME_ERROR("load role csv fail");
        return false;
    }
    if (!CConfJewelSkill::Init(strpath + "ItemSkill.csv"))
    {
        IME_ERROR("load jewelskill csv fail");
        return false;
    }
//    if (!CConfSuitSkill::Init(strpath + "SuitSkill.csv"))
//    {
//        IME_ERROR("load Suitskill csv fail");
//        return false;
//    }
//    if (!CConfSuitCollection::Init(strpath + "SuitSkill2.csv"))
//    {
//        IME_ERROR("load SuitCollection csv fail");
//        return false;
//    }
    if (!CConfCardSkill::Init(strpath + "Skill.csv"))
    {
        IME_ERROR("load skill csv fail");
        return false;
    }
    if (!CConfAttributeRestriction::Init(strpath + "Nature.csv"))
    {
        IME_ERROR("load nature csv fail");
        return false;
    }
    if (!CConfItemCombine::Init(strpath + "Pieces.csv"))
    {
        IME_ERROR("load Pieces csv fail");
        return false;
    }
//    if (!CConfCombineCollection::Init(strpath + "PiecesCombine.csv"))
//    {
//        IME_ERROR("load item csv fail");
//        return false;
//    }
	if (!CConfCardUp::Init(strpath + "CardExp.csv"))
	{
		IME_ERROR("load card up csv fail");
		return false;
	}
//	if (!CConfCardUpConsume::Init(strpath + "Cost.csv"))
//	{
//		IME_ERROR("load card cost csv fail");
//		return false;
//	}
//	if (!CConfCardEvolution::Init(strpath + "Evolution.csv"))
//	{
//		IME_ERROR("load card evolution csv fail");
//		return false;
//	}
//	if (!CConfTech::Init(strpath + "TechType.csv"))
//	{
//		IME_ERROR("load techtype.csv csv fail");
//		return false;
//	}
//	if (!CConfTechRate::Init(strpath + "Tech.csv"))
//	{
//		IME_ERROR("load Tech.csv csv fail");
//		return false;
//	}
//	if (!CConfArenaPrestige::Init(strpath + "PrestigePrize.csv"))
//	{
//		IME_ERROR("load PrestigePrize.csv fail");
//		return false;
//	}
//	if (!CConfArenaRank::Init(strpath + "RankingPrize.csv"))
//	{
//		IME_ERROR("load RankingPrize.csv fail");
//		return false;
//	}
	if (!CConfItemCollection::Init(strpath + "PiecesDrop.csv"))
	{
		IME_ERROR("load PiecesEvent.csv fail");
		return false;
	}
//	if (!CConfMissionBoss::Init(strpath + "Boss.csv"))
//	{
//		IME_ERROR("load Boss.csv fail");
//		return false;
//	}
//	if (!CConfGlobal::Init(strpath + "Global.csv"))
//	{
//		IME_ERROR("load Global.csv fail");
//		return false;
//	}
//	if (!CConfGroupInfo::Init(strpath + "Group.csv"))
//	{
//		IME_ERROR("load Group.csv fail");
//		return false;
//	}
//	if (!CConfGroupAdmin::Init(strpath + "GroupPosition.csv"))
//	{
//		IME_ERROR("load GroupPosition.csv fail");
//		return false;
//	}
//	if (!CConfGroupMission::Init(strpath + "GroupMission.csv"))
//	{
//		IME_ERROR("load GroupMission.csv fail");
//		return false;
//	}
//	if (!CConfGroupPrize::Init(strpath + "GroupHonour.csv"))
//	{
//		IME_ERROR("load GroupHonour.csv fail");
//		return false;
//	}
//	if (!CConfStore::Init(strpath + "Store.csv"))
//	{
//		IME_ERROR("load Store.csv fail");
//		return false;
//	}
//	if (!CConfJewelSkillStrength::Init(strpath + "StoneStrengthen.csv"))
//	{
//		IME_ERROR("load StoneStrenghten.csv fail");
//		return false;
//	}
//	if (!CConfSkillCreate::Init(strpath + "SkillBirth.csv"))
//	{
//		IME_ERROR("load SkillBirth.csv fail");
//		return false;
//	}
//	if (!CConfWeaponRestriction::Init(strpath + "WeaponNature.csv"))
//	{
//		IME_ERROR("load SkillBirth.csv fail");
//		return false;
//	}
	if (!CConfUniqueCollection::Init(strpath + "ShopUnique.csv"))
	{
		IME_ERROR("load ShopUnique.csv fail");
		return false;
	}
	if (!CConfGiftCollection::Init(strpath + "ShopPresent.csv"))
	{
		IME_ERROR("load ShopPresent.csv fail");
		return false;
	}
	if (!CConfGoodsCollection::Init(strpath + "ItemDrop.csv"))
	{
		IME_ERROR("load ItemDrop.csv fail");
		return false;
	}
	if (!CConfMaster::Init(strpath))
	{
		IME_ERROR("load Master.csv fail");
		return false;
	}
	if (!CConfDurationMission::Init(strpath + "Area.csv"))
	{
		IME_ERROR("load Area.csv fail");
		return false;
	}
//	if (!CConfSkillLvUp::Init(strpath + "SkillCombine.csv"))
//	{
//		IME_ERROR("load Area.csv fail");
//		return false;
//	}
//	if (!CConfGiftPacket::Init(strpath + "Gift.csv"))
//	{
//		IME_ERROR("load gift.csv fail");
//		return false;
//	}
	if (!CConfMasterLevel::Init(strpath + "MasterLevel.csv"))
	{
		IME_ERROR("load MasterLevel.csv fail");
		return false;
	}
	if (!CConfRobRobot::Init(strpath + "RobRobot.csv"))
	{
		IME_ERROR("load RobRobot.csv fail");
		return false;
	}
	if (!CConfVip::Init(strpath + "Vip.csv"))
	{
		IME_ERROR("load Vip.csv fail");
		return false;
	}
	if (!CConfStrengthCritPro::Init(strpath + "WeaponStrengthen.csv"))
	{
		IME_ERROR("load WeaponStrenghten.csv fail");
		return false;
	}
	if (!CConfFriendBoss::Init(strpath + "Boss.csv"))
	{
		IME_ERROR("load Boss.csv fail");
		return false;
	}
	if (!CConfGacha::Init(strpath + "GaCha.csv"))
	{
		IME_ERROR("load Gacha.csv fail");
		return false;
	}
	if (!CConfTimerGift::Init(strpath + "TimerGift.csv"))
	{
		IME_ERROR("load TimerGift.csv fail");
		return false;
	}
	if (!CConfLoginSign::Init(strpath + "LoginReward.csv"))
	{
		IME_ERROR("load LoginReward.csv fail");
		return false;
	}
	if (!CConfExskill::Init(strpath + "ExSkill.csv"))
	{
		IME_ERROR("load ExSkill.csv fail");
		return false;
	}
	if (!CConfRelationShip::Init(strpath + "Yuan.csv"))
	{
		IME_ERROR("load Yuan.csv fail");
		return false;
	}
	if (!CConfStrengthConsume::Init(strpath + "Strengthen.csv"))
	{
		IME_ERROR("load Strengthen.csv fail");
		return false;
	}
	if (!CConfLeaderSkill::Init(strpath + "LeaderSkill.csv"))
	{
		IME_ERROR("load LeaderSkill.csv fail");
		return false;
	}
	if (!CConfActive::Init(strpath + "Active.csv"))
	{
		IME_ERROR("load Active.csv fail");
		return false;
	}
	if (!CConfAchieve::Init(strpath + "Achieve.csv"))
	{
		IME_ERROR("load Achieve.csv fail");
		return false;
	}
	if (!CConfUpgrade::Init(strpath + "Upgrade.csv"))
	{
		IME_ERROR("load Upgrade.csv fail");
		return false;
	}
	if (!CConfCycleLogin::Init(strpath + "CycleReward.csv"))
	{
		IME_ERROR("load CycleReward.csv fail");
		return false;
	}
	if (!CConfTrial::Init(strpath + "Trial.csv"))
	{
		IME_ERROR("load Trial.csv fail");
		return false;
	}
	if (!CConfVitalityPrize::Init(strpath + "VitalityPrize.csv"))
	{
		IME_ERROR("load VitalityPrize.csv fail");
		return false;
	}
	if(!CConfTower::init(strpath + "Tower.csv")){
		IME_ERROR("load Tower.csv fail");
		return false;
	}
	if(!CConfTowerRankPrize::Init(strpath + "TowerPrize.csv")){
		IME_ERROR("load TowerRankPrize.csv fail");
		return false;
	}
    return true;
}

bool ConfValid(std::string strpath, std::string strTable)
{
//    if (!CConfCardBase::Valid(strpath + "Card.csv"))
//    {
//        IME_ERROR("load cardbase csv fail");
//        return false;
//    }
	if (strTable == std::string("CardDrop.csv"))
	{
		if (!CConfCardCollection::Valid(strpath + "CardDrop.csv"))
		{
			IME_ERROR("load carddrop csv fail");
			return false;
		}
	}
	else if (strTable == std::string("Item.csv"))
	{
		if (!CConfGoods::Valid(strpath + "Item.csv"))
		{
			IME_ERROR("load Goods.csv fail");
			return false;
		}
	}
//    if (!CConfRole::Valid(strpath + "Exp.csv"))
//    {
//        IME_ERROR("load role csv fail");
//        return false;
//    }
	else if (strTable == std::string("ItemSkill.csv"))
	{
		if (!CConfJewelSkill::Valid(strpath + "ItemSkill.csv"))
		{
			IME_ERROR("load jewelskill csv fail");
			return false;
		}
	}
	else if (strTable == std::string("Skill.csv"))
	{
		if (!CConfCardSkill::Valid(strpath + "Skill.csv"))
		{
			IME_ERROR("load skill csv fail");
			return false;
		}
	}
	else if (strTable == std::string("Nature.csv"))
	{
		if (!CConfAttributeRestriction::Valid(strpath + "Nature.csv"))
		{
			IME_ERROR("load nature csv fail");
			return false;
		}
	}
	else if (strTable == std::string("Pieces.csv"))
	{
		if (!CConfItemCombine::Valid(strpath + "Pieces.csv"))
		{
			IME_ERROR("load Pieces csv fail");
			return false;
		}
	}
//	if (!CConfCardUp::Valid(strpath + "CardExp.csv"))
//	{
//		IME_ERROR("load card up csv fail");
//		return false;
//	}
	else if (strTable == std::string("PiecesDrop.csv"))
	{
		if (!CConfItemCollection::Valid(strpath + "PiecesDrop.csv"))
		{
			IME_ERROR("load PiecesEvent.csv fail");
			return false;
		}
	}
//	else if (strTable == std::string("Global.csv"))
//	{
//		if (!CConfGlobal::Valid(strpath + "Global.csv"))
//		{
//			IME_ERROR("load Global.csv fail");
//			return false;
//		}
//	}
//	if (!CConfStore::Valid(strpath + "Store.csv"))
//	{
//		IME_ERROR("load Store.csv fail");
//		return false;
//	}
	else if (strTable == std::string("ShopUnique.csv"))
	{
		if (!CConfUniqueCollection::Valid(strpath + "ShopUnique.csv"))
		{
			IME_ERROR("load ShopUnique.csv fail");
			return false;
		}
	}
	else if (strTable == std::string("ShopPresent.csv"))
	{
		if (!CConfGiftCollection::Valid(strpath + "ShopPresent.csv"))
		{
			IME_ERROR("load ShopPresent.csv fail");
			return false;
		}
	}
	else if (strTable == std::string("ItemDrop.csv"))
	{
		if (!CConfGoodsCollection::Valid(strpath + "ItemDrop.csv"))
		{
			IME_ERROR("load ItemDrop.csv fail");
			return false;
		}
	}
	else if (strTable == std::string("Master.csv"))
	{
		if (!CConfMaster::Valid(strpath))
		{
			IME_ERROR("load Master.csv fail");
			return false;
		}
	}
	else if (strTable == std::string("Area.csv"))
	{
		if (!CConfDurationMission::Valid(strpath + "Area.csv"))
		{
			IME_ERROR("load Area.csv fail");
			return false;
		}
	}
	else if (strTable == std::string("MasterLevel.csv"))
	{
		if (!CConfMasterLevel::Valid(strpath + "MasterLevel.csv"))
		{
			IME_ERROR("load MasterLevel.csv fail");
			return false;
		}
	}
//	else if (strTable == std::string("RobRobot.csv"))
//	{
//		if (!CConfRobRobot::Valid(strpath + "RobRobot.csv"))
//		{
//			IME_ERROR("load RobRobot.csv fail");
//			return false;
//		}
//	}
	else if (strTable == std::string("Vip.csv"))
	{
		if (!CConfVip::Valid(strpath + "Vip.csv"))
		{
			IME_ERROR("load Vip.csv fail");
			return false;
		}
	}
	else if (strTable == std::string("WeaponStrengthen.csv"))
	{
		if (!CConfStrengthCritPro::Valid(strpath + "WeaponStrengthen.csv"))
		{
			IME_ERROR("load WeaponStrenghten.csv fail");
			return false;
		}
	}
	else if (strTable == std::string("Boss.csv"))
	{
		if (!CConfFriendBoss::Valid(strpath + "Boss.csv"))
		{
			IME_ERROR("load Boss.csv fail");
			return false;
		}
	}
	else if (strTable == std::string("Gacha.csv"))
	{
		if (!CConfGacha::Valid(strpath + "GaCha.csv"))
		{
			IME_ERROR("load Gacha.csv fail");
			return false;
		}
	}
	else if (strTable == std::string("TimerGift.csv"))
	{
		if (!CConfTimerGift::Valid(strpath + "TimerGift.csv"))
		{
			IME_ERROR("load TimerGift.csv fail");
			return false;
		}
	}
	else if (strTable == std::string("LoginReward.csv"))
	{
		if (!CConfLoginSign::Valid(strpath + "LoginReward.csv"))
		{
			IME_ERROR("load LoginReward.csv fail");
			return false;
		}
	}
	else if (strTable == std::string("ExSkill.csv"))
	{
		if (!CConfExskill::Valid(strpath + "ExSkill.csv"))
		{
			IME_ERROR("load ExSkill.csv fail");
			return false;
		}
	}
	else if (strTable == std::string("Yuan.csv"))
	{
		if (!CConfRelationShip::Valid(strpath + "Yuan.csv"))
		{
			IME_ERROR("load Yuan.csv fail");
			return false;
		}
	}
	else if (strTable == std::string("Strengthen.csv"))
	{
		if (!CConfStrengthConsume::Valid(strpath + "Strengthen.csv"))
		{
			IME_ERROR("load Strengthen.csv fail");
			return false;
		}
	}
	else if (strTable == std::string("LeaderSkill.csv"))
	{
		if (!CConfLeaderSkill::Valid(strpath + "LeaderSkill.csv"))
		{
			IME_ERROR("load LeaderSkill.csv fail");
			return false;
		}
	}
	else if (strTable == std::string("Active.csv"))
	{
		if (!CConfActive::Valid(strpath + "Active.csv"))
		{
			IME_ERROR("load Active.csv fail");
			return false;
		}
	}
	else if (strTable == std::string("Achieve.csv"))
	{
		if (!CConfAchieve::Valid(strpath + "Achieve.csv"))
		{
			IME_ERROR("load Achieve.csv fail");
			return false;
		}
	}
	else if (strTable == std::string("Upgrade.csv"))
	{
		if (!CConfUpgrade::Valid(strpath + "Upgrade.csv"))
		{
			IME_ERROR("load Upgrade.csv fail");
			return false;
		}
	}
	else if (strTable == std::string("CycleReward.csv"))
	{
		if (!CConfCycleLogin::Valid(strpath + "CycleReward.csv"))
		{
			IME_ERROR("load CycleReward.csv fail");
			return false;
		}
	}
	else if (strTable == std::string("Trial.csv"))
	{
		if (!CConfTrial::Valid(strpath + "Trial.csv"))
		{
			IME_ERROR("load Trial.csv fail");
			return false;
		}
	}
	else if (strTable == std::string("VitalityPrize.csv"))
	{
		if (!CConfVitalityPrize::Valid(strpath + "VitalityPrize.csv"))
		{
			IME_ERROR("load VitalityPrize.csv fail");
			return false;
		}
	}
	else if (strTable == std::string("Tower.csv")) {
		if (!CConfTower::valid(strpath + "Tower.csv")) {
			IME_ERROR("load Tower.csv fail");
			return false;
		}
	}
	else if (strTable == std::string("TowerPrize.csv")) {
		if (!CConfTower::valid(strpath + "TowerPrize.csv")) {
			IME_ERROR("load TowerPrize.csv fail");
			return false;
		}
	}
	else
	{
		IME_ERROR("this csv table %s do not supported", strTable.c_str());
		return false;
	}
    return true;
}

bool ConfReload(std::string strpath, std::string strTable)
{
//    if (!CConfCardBase::Init(strpath + "Card.csv"))
//    {
//        IME_ERROR("load cardbase csv fail");
//        return false;
//    }
	if (strTable == std::string("CardDrop.csv"))
	{
		if (!CConfCardCollection::Init(strpath + "CardDrop.csv"))
		{
			IME_ERROR("load carddrop csv fail");
			return false;
		}
	}
	else if (strTable == std::string("Item.csv"))
	{
		if (!CConfGoods::Init(strpath + "Item.csv"))
		{
			IME_ERROR("load Goods.csv fail");
			return false;
		}
	}
//    if (!CConfRole::Init(strpath + "Exp.csv"))
//    {
//        IME_ERROR("load role csv fail");
//        return false;
//    }
	else if (strTable == std::string("ItemSkill.csv"))
	{
		if (!CConfJewelSkill::Init(strpath + "ItemSkill.csv"))
		{
			IME_ERROR("load jewelskill csv fail");
			return false;
		}
	}
	else if (strTable == std::string("Skill.csv"))
	{
		if (!CConfCardSkill::Init(strpath + "Skill.csv"))
		{
			IME_ERROR("load skill csv fail");
			return false;
		}
	}
	else if (strTable == std::string("Nature.csv"))
	{
		if (!CConfAttributeRestriction::Init(strpath + "Nature.csv"))
		{
			IME_ERROR("load nature csv fail");
			return false;
		}
	}
	else if (strTable == std::string("Pieces.csv"))
	{
		if (!CConfItemCombine::Init(strpath + "Pieces.csv"))
		{
			IME_ERROR("load Pieces csv fail");
			return false;
		}
	}
//	if (!CConfCardUp::Init(strpath + "CardExp.csv"))
//	{
//		IME_ERROR("load card up csv fail");
//		return false;
//	}
	else if (strTable == std::string("PiecesDrop.csv"))
	{
		if (!CConfItemCollection::Init(strpath + "PiecesDrop.csv"))
		{
			IME_ERROR("load PiecesEvent.csv fail");
			return false;
		}
	}
//	else if (strTable == std::string("Global.csv"))
//	{
//		if (!CConfGlobal::Init(strpath + "Global.csv"))
//		{
//			IME_ERROR("load Global.csv fail");
//			return false;
//		}
//	}
//	if (!CConfStore::Init(strpath + "Store.csv"))
//	{
//		IME_ERROR("load Store.csv fail");
//		return false;
//	}
	else if (strTable == std::string("ShopUnique.csv"))
	{
		if (!CConfUniqueCollection::Init(strpath + "ShopUnique.csv"))
		{
			IME_ERROR("load ShopUnique.csv fail");
			return false;
		}
	}
	else if (strTable == std::string("ShopPresent.csv"))
	{
		if (!CConfGiftCollection::Init(strpath + "ShopPresent.csv"))
		{
			IME_ERROR("load ShopPresent.csv fail");
			return false;
		}
	}
	else if (strTable == std::string("ItemDrop.csv"))
	{
		if (!CConfGoodsCollection::Init(strpath + "ItemDrop.csv"))
		{
			IME_ERROR("load ItemDrop.csv fail");
			return false;
		}
	}
	else if (strTable == std::string("Master.csv"))
	{
		if (!CConfMaster::Init(strpath))
		{
			IME_ERROR("load Master.csv fail");
			return false;
		}
	}
	else if (strTable == std::string("Area.csv"))
	{
		if (!CConfDurationMission::Init(strpath + "Area.csv"))
		{
			IME_ERROR("load Area.csv fail");
			return false;
		}
	}
	else if (strTable == std::string("MasterLevel.csv"))
	{
		if (!CConfMasterLevel::Init(strpath + "MasterLevel.csv"))
		{
			IME_ERROR("load MasterLevel.csv fail");
			return false;
		}
	}
//	else if (strTable == std::string("RobRobot.csv"))
//	{
//		if (!CConfRobRobot::Init(strpath + "RobRobot.csv"))
//		{
//			IME_ERROR("load RobRobot.csv fail");
//			return false;
//		}
//	}
	else if (strTable == std::string("Vip.csv"))
	{
		if (!CConfVip::Init(strpath + "Vip.csv"))
		{
			IME_ERROR("load Vip.csv fail");
			return false;
		}
	}
	else if (strTable == std::string("WeaponStrengthen.csv"))
	{
		if (!CConfStrengthCritPro::Init(strpath + "WeaponStrengthen.csv"))
		{
			IME_ERROR("load WeaponStrenghten.csv fail");
			return false;
		}
	}
	else if (strTable == std::string("Boss.csv"))
	{
		if (!CConfFriendBoss::Init(strpath + "Boss.csv"))
		{
			IME_ERROR("load Boss.csv fail");
			return false;
		}
	}
	else if (strTable == std::string("Gacha.csv"))
	{
		if (!CConfGacha::Init(strpath + "GaCha.csv"))
		{
			IME_ERROR("load Gacha.csv fail");
			return false;
		}
	}
	else if (strTable == std::string("TimerGift.csv"))
	{
		if (!CConfTimerGift::Init(strpath + "TimerGift.csv"))
		{
			IME_ERROR("load TimerGift.csv fail");
			return false;
		}
	}
	else if (strTable == std::string("LoginReward.csv"))
	{
		if (!CConfLoginSign::Init(strpath + "LoginReward.csv"))
		{
			IME_ERROR("load LoginReward.csv fail");
			return false;
		}
	}
	else if (strTable == std::string("ExSkill.csv"))
	{
		if (!CConfExskill::Init(strpath + "ExSkill.csv"))
		{
			IME_ERROR("load ExSkill.csv fail");
			return false;
		}
	}
	else if (strTable == std::string("Yuan.csv"))
	{
		if (!CConfRelationShip::Init(strpath + "Yuan.csv"))
		{
			IME_ERROR("load Yuan.csv fail");
			return false;
		}
	}
	else if (strTable == std::string("Strengthen.csv"))
	{
		if (!CConfStrengthConsume::Init(strpath + "Strengthen.csv"))
		{
			IME_ERROR("load Strengthen.csv fail");
			return false;
		}
	}
	else if (strTable == std::string("LeaderSkill.csv"))
	{
		if (!CConfLeaderSkill::Init(strpath + "LeaderSkill.csv"))
		{
			IME_ERROR("load LeaderSkill.csv fail");
			return false;
		}
	}
	else if (strTable == std::string("Active.csv"))
	{
		if (!CConfActive::Init(strpath + "Active.csv"))
		{
			IME_ERROR("load Active.csv fail");
			return false;
		}
	}
	else if (strTable == std::string("Upgrade.csv"))
	{
		if (!CConfUpgrade::Init(strpath + "Upgrade.csv"))
		{
			IME_ERROR("load Upgrade.csv fail");
			return false;
		}
	}
	else if (strTable == std::string("CycleReward.csv"))
	{
		if (!CConfCycleLogin::Init(strpath + "CycleReward.csv"))
		{
			IME_ERROR("load CycleReward.csv fail");
			return false;
		}
	}
	else if (strTable == std::string("Trial.csv"))
	{
		if (!CConfTrial::Init(strpath + "Trial.csv"))
		{
			IME_ERROR("load Trial.csv fail");
			return false;
		}
	}
	else if (strTable == std::string("VitalityPrize.csv"))
	{
		if (!CConfVitalityPrize::Init(strpath + "VitalityPrize.csv"))
		{
			IME_ERROR("load VitalityPrize.csv fail");
			return false;
		}
	}
	else if (strTable == std::string("Tower.csv")) {
		if (!CConfTrial::Init(strpath + "Trial.csv")) {
			IME_ERROR("load Tower.csv fail");
			return false;
		}
	} else if (strTable == std::string("TowerPrize.csv")) {
		if (!CConfTowerRankPrize::Init(strpath + "TowerPrize.csv")) {
			IME_ERROR("load TowerPrize.csv fail");
			return false;
		}
	}
	else
	{
		IME_ERROR("can not reload this csv table %s", strTable.c_str());
		return false;
	}
	return true;
}

/////////////////////////////////////////////////
CConfTest::MapData CConfTest::m_mapData;
bool CConfTest::Init( const std::string& strFilePath )
{
	m_mapData.clear();

	CConfigReader reader;
	if (!reader.init(strFilePath))
	{
		return false;
	}
	int nRow = reader.rowCount();
	for (int i = 1; i < nRow; ++i)
	{
		int j = 0;
		STC_CONF_TEST info;
		READER_INT(info.wTestId);
		if (!info.wTestId)
		{
			continue;
		}

		if (m_mapData.insert(std::make_pair(info.wTestId, info)).second == false)
		{
			IME_ERROR("ERROR CONF TEST DUPLICATE [ChestId=%u]", info.wTestId);
			return false;
		}
	}

	IME_LOG("LOAD CONF TEST [Size=%u]", m_mapData.size());

	return true;
}

STC_CONF_TEST* CConfTest::Find( uint16 wTestId )
{
	MapData::iterator it = m_mapData.find(wTestId);
	return it == m_mapData.end() ? NULL : &it->second;
}

//CConfMissionBase::MapData CConfMissionBase::m_mapData;
//bool CConfMissionBase::Init( const std::string& strFilePath )
//{
//	m_mapData.clear();
//
//	CConfigReader reader;
//	if (!reader.init(strFilePath))
//	{
//		return false;
//	}
//	int nRow = reader.rowCount();
//	for (int i = 1; i < nRow; ++i)
//	{
//		int j = 0;
//		STC_CONF_MISSION_BASE info;
//		READER_INT(info.dwIndex);
//		READER_INT(info.byChapter);
//		READER_INT(info.byCheckpoint);
//		READER_INT(info.byTask);
//		READER_INT(info.byType);
//		READER_INT(info.byStepLimit);
//		READER_INT(info.wConsumeEnergy);
//		READER_INT(info.wGetExp);
//		READER_INT(info.wGetCoin);
//		READER_INT(info.byRate);
//		READER_INT(info.dwRandomEventIndex);
//		READER_INT(info.dwBoss);
//		READER_INT(info.dwNextTaskIndex);
////		READER_INT(info.wLevelLimit);
////		READER_INT(info.dwCD);
////		READER_INT(info.dwSpecial);
////		READER_INT(info.byPro);
////		READER_INT(info.dwAttackPrize);
////		READER_INT(info.byAttackPro);
////		READER_INT(info.dwDefencePrize);
////		READER_INT(info.byDefencePro);
////		READER_INT(info.byDurationCmd);
//		IS_READER_ERROR;
//	
//		if (m_mapData.insert(std::make_pair(info.dwIndex, info)).second == false)
//		{
//			IME_ERROR("ERROR CONF DUPLICATE [ChestId=%u]", info.dwIndex);
//			return false;
//		}
//	}
//
//	IME_LOG("LOAD CONF MISSION [Size=%d]", m_mapData.size());
//
//	return true;
//}
//
//STC_CONF_MISSION_BASE* CConfMissionBase::Find(uint32 dwIndex)
//{
//	MapData::iterator it = m_mapData.find(dwIndex);
//	return it == m_mapData.end() ? NULL : &it->second;
//}
//
//int CConfMissionBase::Findp1(uint32 dwIndex, STC_CONF_MISSION_BASE **val)
//{
//	MapData::iterator it = m_mapData.find(dwIndex);
//	if (it == m_mapData.end())
//		return -1;
//	MapData::iterator it_next = m_mapData.find((it->second).dwNextTaskIndex);
//	if (it_next == m_mapData.end())                  /* not any task */
//		return 1;
//	if ((it_next->second).byType != E_MISSION_MAIN)
//		return 1;
//	*val = &it_next->second;
////	IME_LOG("index %u", it->second.dwIndex);
//	return 0;
//}

//CConfRandomEvent::MapData CConfRandomEvent::m_mapData;
//bool CConfRandomEvent::Init( const std::string& strFilePath )
//{
//	m_mapData.clear();
//
//	CConfigReader reader;
//	if (!reader.init(strFilePath))
//	{
//		return false;
//	}
//	int nRow = reader.rowCount();
//	for (int i = 1; i < nRow; ++i)
//	{
//		int j = 0;
//		STC_CONF_RANDOM_EVENT info;
//		READER_INT(info.dwIndex);
//		READER_INT(info.wType);
//		if (info.wType == CRandomEvent::EXP_RATE_UP || info.wType == CRandomEvent::COIN_RATE_UP)
//		{
//			float temp;
//			READER_FLOAT(temp);
//			info.para1 = int(temp * 10);
//		}	
//		else
//		{
//			READER_INT(info.para1);
//		}
//
//		READER_INT(info.para2);
//		READER_INT(info.para3);
//		READER_INT(info.para4);
//		IS_READER_ERROR;
//
//		if (m_mapData.insert(std::make_pair(info.dwIndex, info)).second == false)
//		{
//			IME_ERROR("ERROR CONF DUPLICATE [ChestId=%u]", info.dwIndex);
//			return false;
//		}
//	}
//
//	IME_LOG("LOAD CONF RANDOMEVENT [Size=%d]", m_mapData.size());
//
//	return true;
//}
//
//STC_CONF_RANDOM_EVENT* CConfRandomEvent::Find(uint32 dwIndex)
//{
//	MapData::iterator it = m_mapData.find(dwIndex);
//	return it == m_mapData.end() ? NULL : &it->second;
//}

CConfCardCollection::MapData CConfCardCollection::m_mapData;
std::vector<STC_CONF_CARD_COLLECTION *> CConfCardCollection::m_vecData;

bool CConfCardCollection::ReadConfig(const std::string& strFilePath, MapData &mapt)
{
	CConfigReader reader;
	if (!reader.init(strFilePath))
	{
		return false;
	}
	int nRow = reader.rowCount();
	for (int i = 1; i < nRow; ++i)
	{
		int j = 0;
		STC_CONF_CARD_COLLECTION info;
		READER_INT(info.dwIndex);
		READER_INT(info.dwCardindex);
		READER_INT(info.byWeight);
		READER_INT(info.byStar);
		READER_STR(info.strContent);
		IS_READER_ERROR;
		
		if (CConfCardBase::Find(info.dwCardindex) == NULL)
		{
			IME_ERROR("index %u can not find card index %u", info.dwIndex, info.dwCardindex);
			return false;
		}

		mapt.insert(std::make_pair(info.dwIndex, info));
	}

	IME_LOG("LOAD CONF CARDDROP [Size=%d]", mapt.size());
	return true;
}

bool CConfCardCollection::Init( const std::string& strFilePath )
{
	m_mapData.clear();
	m_vecData.clear();
	
	if (!ReadConfig(strFilePath, m_mapData))
	{
		IME_ERROR("read config fail");
		return false;
	}

	return true;
}

std::vector<STC_CONF_CARD_COLLECTION *>* CConfCardCollection::Find(uint32 dwIndex)
{
	MapData::iterator it;
	std::pair<MapData::iterator, MapData::iterator> ret;
	ret = m_mapData.equal_range(dwIndex);
	m_vecData.clear();
	for (it = ret.first; it != ret.second; it++)
	{
		m_vecData.push_back(&((*it).second));
	}
	return &m_vecData;
}

bool CConfCardCollection::Valid( const std::string& strFilePath )
{
	MapData mapData;
	
	if (!ReadConfig(strFilePath, mapData))
	{
		IME_ERROR("valid config fail");
		return false;
	}

	return true;
}

// role conf
CConfRole::MapData CConfRole::m_mapData;
std::vector<uint32_t> CConfRole::m_vecExpToLevel;

bool CConfRole::Init(const std::string& strFilePath )
{
	m_mapData.clear();

	CConfigReader reader;
	if (!reader.init(strFilePath))
	{
		return false;
	}
	int nRow = reader.rowCount();
	if (nRow <= 1)
	{
		IME_ERROR("the row is small than 1 rowCount %u", nRow);
		return false;
	}
	uint32_t ExpLimit = 0;
	for (int i = 1; i < nRow; ++i)
	{
		int j = 0;
		uint32_t val;
		STC_CONF_ROLE info;
		READER_INT(info.dwIndex);
		if (i != info.dwIndex)
		{
			IME_ERROR("ERROR CONF UNORDER [INDEX = %u]", info.dwIndex);
			return false;
		}
		READER_INT(val);
		ExpLimit += val;
		info.dwExpLimit = ExpLimit;
//		IME_ERROR("explimit %u", info.dwExpLimit);

		m_vecExpToLevel.push_back(ExpLimit);

		READER_INT(info.dwEnergyLimit);
		READER_INT(info.dwStrengthLimit);
		READER_INT(info.dwEnergyRate);
		READER_INT(info.dwStrengthRate);
		READER_INT(info.byCardCapacity);
		READER_INT(info.dwFriendLimit);
		READER_INT(info.dwStaminaLimit);
		READER_INT(info.dwStaminaRate);
		READER_INT(info.dwRoleExp);
		READER_INT(info.dwCardExp);
		READER_INT(info.dwCoin);
		IS_READER_ERROR;

		if (m_mapData.insert(std::make_pair(info.dwIndex, info)).second == false)
		{
			IME_ERROR("ERROR CONF DUPLICATE [ChestId=%u]", info.dwIndex);
			return false;
		}
	}

	IME_LOG("LOAD CONF EXP [Size=%d]", m_mapData.size());

	return true;
}

STC_CONF_ROLE* CConfRole::Find(uint32 dwIndex)
{
	MapData::iterator it = m_mapData.find(dwIndex);
	return it == m_mapData.end() ? NULL : &it->second;
}

uint32_t CConfRole::FindLevel(uint32_t dwExp)
{
	std::vector<uint32_t>::iterator low;
	uint32_t level;
	low = lower_bound(m_vecExpToLevel.begin(), m_vecExpToLevel.end(), dwExp + 1);
	if (low == m_vecExpToLevel.end())
	{
		level = m_vecExpToLevel.size();
	}
	else
	{
		level = (int)(low - m_vecExpToLevel.begin()) + 1;
	}
	return level;
}

uint32_t CConfRole::GetMaxExp()
{
	if (m_vecExpToLevel.size() == 0)
		return 0;
	std::vector<uint32_t>::iterator it = m_vecExpToLevel.end();
	it--;
	it--;
	return *it;
}

//suit skill conf
//CConfSuitSkill::MapData CConfSuitSkill::m_mapData;
//
//bool CConfSuitSkill::Init(const std::string& strFilePath )
//{
//	m_mapData.clear();
//
//	CConfigReader reader;
//	if (!reader.init(strFilePath))
//	{
//		return false;
//	}
//	int nRow = reader.rowCount();
//	if (nRow <= 1)
//	{
//		IME_ERROR("the row is small than 1 rowCount %u", nRow);
//		return false;
//	}
//	for (int i = 1; i < nRow; ++i)
//	{
//		int j = 0;
//		STC_CONF_SUIT_SKILL info;
//		READER_INT(info.dwIndex);
//		READER_INT(info.wType);
//		READER_INT(info.para1);
//		READER_INT(info.para2);
//		READER_INT(info.dwCollectionCard1);
//		READER_INT(info.dwCollectionCard2);
//		READER_INT(info.dwCollectionCard3);
//		READER_INT(info.dwCollectionCard4);
//		IS_READER_ERROR;
//
//		if (m_mapData.insert(std::make_pair(info.dwIndex, info)).second == false)
//		{
//			IME_ERROR("ERROR CONF DUPLICATE [ChestId=%u]", info.dwIndex);
//			return false;
//		}
//	}
//
//	IME_LOG("LOAD CONF SUITSKILL1 [Size=%d]", m_mapData.size());
//
//	return true;
//}
//
//STC_CONF_SUIT_SKILL* CConfSuitSkill::Find(uint32 dwIndex)
//{
//	MapData::iterator it = m_mapData.find(dwIndex);
//	return it == m_mapData.end() ? NULL : &it->second;
//}

////suitcollection conf
//CConfSuitCollection::MapData CConfSuitCollection::m_mapData;
//std::vector<uint32> CConfSuitCollection::m_vecData;
//bool CConfSuitCollection::Init( const std::string& strFilePath )
//{
//	m_mapData.clear();
//
//	CConfigReader reader;
//	if (!reader.init(strFilePath))
//	{
//		return false;
//	}
//	int nRow = reader.rowCount();
//	for (int i = 1; i < nRow; ++i)
//	{
//		int j = 0;
//		STC_CONF_SUIT_COLLECTION info;
//		READER_INT(info.dwIndex);
//		READER_INT(info.dwCardIndex);
//		IS_READER_ERROR;
//
//		m_mapData.insert(std::make_pair(info.dwIndex, info.dwCardIndex));
//	}
//
//	IME_LOG("LOAD CONF SUITSKILL2 [Size=%d]", m_mapData.size());
//
//	return true;
//}
//
//std::vector<uint32>* CConfSuitCollection::Find(uint32 dwIndex)
//{
//	MapData::iterator it;
//	std::pair<MapData::iterator, MapData::iterator> ret;
//	ret = m_mapData.equal_range(dwIndex);
//	m_vecData.clear();
//	for (it = ret.first; it != ret.second; it++)
//	{
//		m_vecData.push_back((*it).second);
//	}
//	return &m_vecData;
//}

//attributeRestriction conf
std::vector<std::vector<uint32> >  CConfAttributeRestriction::m_vecData;
bool CConfAttributeRestriction::Init( const std::string& strFilePath )
{
	m_vecData.clear();

	CConfigReader reader;
	if (!reader.init(strFilePath))
	{
		return false;
	}
	int nRow = reader.rowCount();
	std::vector<uint32> vecOneAttri;
	for (int i = 1; i < nRow; ++i)
	{
		vecOneAttri.clear();
		int j = 1;
		for (; j <= STICK + 1; )
		{
			uint32 info;
			READER_INT(info);
//			IME_LOG("i %u j %u %u", i, j, info);
			IS_READER_ERROR;

			vecOneAttri.push_back(info);
		}

		m_vecData.push_back(vecOneAttri);
	}

	IME_LOG("LOAD CONF NATURE [Size=%d]", m_vecData.size());

	return true;
}

uint32* CConfAttributeRestriction::Find(uint32 dwIndex, uint32 dwIndex2)
{
	if (dwIndex >= m_vecData.size())
		return NULL;
	if (dwIndex2 >= m_vecData[dwIndex].size())
		return NULL;
	return &(m_vecData[dwIndex][dwIndex2]);
}

bool CConfAttributeRestriction::Valid( const std::string& strFilePath )
{
	std::vector<std::vector<uint32> > m_vecData;

	CConfigReader reader;
	if (!reader.init(strFilePath))
	{
		return false;
	}
	int nRow = reader.rowCount();
	std::vector<uint32> vecOneAttri;
	for (int i = 1; i < nRow; ++i)
	{
		vecOneAttri.clear();
		int j = 1;
		for (; j <= STICK + 1; )
		{
			uint32 info;
			READER_INT(info);
//			IME_LOG("i %u j %u %u", i, j, info);
			IS_READER_ERROR;

			vecOneAttri.push_back(info);
		}

		m_vecData.push_back(vecOneAttri);
	}

	IME_LOG("LOAD CONF NATURE [Size=%d]", m_vecData.size());

	return true;
}



// item combine collection
//CConfCombineCollection::MapData CConfCombineCollection::m_mapData;
//
//bool CConfCombineCollection::Init(const std::string& strFilePath )
//{
//	m_mapData.clear();
//
//	CConfigReader reader;
//	if (!reader.init(strFilePath))
//	{
//		return false;
//	}
//	int nRow = reader.rowCount();
//	if (nRow <= 1)
//	{
//		IME_ERROR("the row is small than 1 rowCount %u", nRow);
//		return false;
//	}
//	for (int i = 1; i < nRow; ++i)
//	{
//		int j = 0;
//		STC_CONF_ITEM_COMBINE_COLLECTION info;
//		READER_INT(info.dwIndex);
//		READER_INT(info.dwID1);
//		READER_INT(info.dwID2);
//		READER_INT(info.dwID3);
//		READER_INT(info.dwID4);
//		READER_INT(info.dwID5);
//		READER_INT(info.dwID6);
//		IS_READER_ERROR;
//
//		if (m_mapData.insert(std::make_pair(info.dwIndex, info)).second == false)
//		{
//			IME_ERROR("ERROR CONF DUPLICATE [ChestId=%u]", info.dwIndex);
//			return false;
//		}
//	}
//
//	IME_LOG("LOAD CONF PIECESCOMBINE [Size=%d]", m_mapData.size());
//
//	return true;
//}
//
//STC_CONF_ITEM_COMBINE_COLLECTION* CConfCombineCollection::Find(uint32 dwIndex)
//{
//	MapData::iterator it = m_mapData.find(dwIndex);
//	return it == m_mapData.end() ? NULL : &it->second;
//}

// card up consume
//CConfCardUpConsume::MapData CConfCardUpConsume::m_mapData;
//
//bool CConfCardUpConsume::Init(const std::string& strFilePath )
//{
//	m_mapData.clear();
//
//	CConfigReader reader;
//	if (!reader.init(strFilePath))
//	{
//		return false;
//	}
//	int nRow = reader.rowCount();
//	if (nRow <= 1)
//	{
//		IME_ERROR("the row is small than 1 rowCount %u", nRow);
//		return false;
//	}
//	for (int i = 1; i < nRow; ++i)
//	{
//		int j = 0;
//		STC_CONF_CARD_UP_CONSUME info;
//		READER_INT(info.dwStar);
//		READER_INT(info.dwStrengthCon);
//		READER_INT(info.dWEvolutionCon);
//		IS_READER_ERROR;
//
//		if (m_mapData.insert(std::make_pair(info.dwStar, info)).second == false)
//		{
//			IME_ERROR("ERROR CONF DUPLICATE [ChestId=%u]", info.dwStar);
//			return false;
//		}
//	}
//
//	IME_LOG("LOAD CONF COST [Size=%d]", m_mapData.size());
//
//	return true;
//}
//
//STC_CONF_CARD_UP_CONSUME* CConfCardUpConsume::Find(uint32 dwIndex)
//{
//	MapData::iterator it = m_mapData.find(dwIndex);
//	return it == m_mapData.end() ? NULL : &it->second;
//}

// card evolution
//CConfCardEvolution::MapData CConfCardEvolution::m_mapData;
//
//bool CConfCardEvolution::Init(const std::string& strFilePath )
//{
//	m_mapData.clear();
//
//	CConfigReader reader;
//	if (!reader.init(strFilePath))
//	{
//		return false;
//	}
//	int nRow = reader.rowCount();
//	if (nRow <= 1)
//	{
//		IME_ERROR("the row is small than 1 rowCount %u", nRow);
//		return false;
//	}
//	for (int i = 1; i < nRow; ++i)
//	{
//		int j = 0;
//		STC_CONF_CARD_EVOLUTION info;
//		READER_INT(info.dwCardIndex1);
//		READER_INT(info.dwCardIndex2);
//		READER_INT(info.dWCondition);
//		IS_READER_ERROR;
//
//		if (m_mapData.insert(std::make_pair(info.dwCardIndex1, info)).second == false)
//		{
//			IME_ERROR("ERROR CONF DUPLICATE [ChestId=%u]", info.dwCardIndex1);
//			return false;
//		}
//	}
//
//	IME_LOG("LOAD CONF EVOLUTION [Size=%d]", m_mapData.size());
//
//	return true;
//}
//
//STC_CONF_CARD_EVOLUTION* CConfCardEvolution::Find(uint32 dwIndex)
//{
//	MapData::iterator it = m_mapData.find(dwIndex);
//	return it == m_mapData.end() ? NULL : &it->second;
//}

// tech
CConfTech::MapData CConfTech::m_mapData;

bool CConfTech::Init(const std::string& strFilePath )
{
	m_mapData.clear();

	CConfigReader reader;
	if (!reader.init(strFilePath))
	{
		return false;
	}
	int nRow = reader.rowCount();
	if (nRow <= 1)
	{
		IME_ERROR("the row is small than 1 rowCount %u", nRow);
		return false;
	}
	for (int i = 1; i < nRow; ++i)
	{
		int j = 0;
		STC_CONF_TECH info;
		READER_INT(info.dwIndex);
		READER_INT(info.byType);
		info.byType--;
//		IME_LOG("%u %u", info.dwIndex, info.byType);
		IS_READER_ERROR;

		if (m_mapData.insert(std::make_pair(info.dwIndex, info)).second == false)
		{
			IME_ERROR("ERROR CONF DUPLICATE [ChestId=%u]", info.dwIndex);
			return false;
		}
	}

	IME_LOG("LOAD CONF TECH [Size=%d]", m_mapData.size());

	return true;
}

STC_CONF_TECH* CConfTech::Find(uint32 dwIndex)
{
	MapData::iterator it = m_mapData.find(dwIndex);
	return it == m_mapData.end() ? NULL : &it->second;
}

CConfTech::MapData *CConfTech::GetMap()
{
	return &m_mapData;
}

 // tech_rate
CConfTechRate::MapData CConfTechRate::m_mapData;

bool CConfTechRate::Init(const std::string& strFilePath )
{
	m_mapData.clear();

	CConfigReader reader;
	if (!reader.init(strFilePath))
	{
		return false;
	}
	int nRow = reader.rowCount();
	if (nRow <= 1)
	{
		IME_ERROR("the row is small than 1 rowCount %u", nRow);
		return false;
	}
	for (int i = 1; i < nRow; ++i)
	{
		int j = 0;
		uint32 Val;
		uint32 Coin;
		uint32 CreditConsume;
		STC_CONF_TECH_RATE info;
		READER_INT(info.wLv);
		READER_INT(info.wRoleLv);
		READER_INT(info.wGroupLevelLimit);

		READER_INT(Val);
		READER_INT(Coin);
		READER_INT(CreditConsume);
		info.dwVal[0] = Val;
		info.dwCoin[0] = Coin;
		info.dwCreditConsume[0] = CreditConsume;
		READER_INT(Val);
		READER_INT(Coin);
		READER_INT(CreditConsume);
		info.dwVal[1] = Val;
		info.dwCoin[1] = Coin;
		info.dwCreditConsume[1] = CreditConsume;
		IS_READER_ERROR;

		if (m_mapData.insert(std::make_pair(info.wLv, info)).second == false)
		{
			IME_ERROR("ERROR CONF DUPLICATE [ChestId=%u]", info.wLv);
			return false;
		}
	}

	IME_LOG("LOAD CONF TECHRATE [Size=%d]", m_mapData.size());

	return true;
}

STC_CONF_TECH_RATE* CConfTechRate::Find(uint32 dwIndex)
{
	MapData::iterator it = m_mapData.find(dwIndex);
	return it == m_mapData.end() ? NULL : &it->second;
}

uint16 CConfTechRate::GetMaxLevel()
{
	MapData::iterator it = m_mapData.end();
	if (it != m_mapData.begin())
	{
		it--;
		return (it->second).wLv;
	}
	else
	{
		return 0;
	}
}

// arena PrestigePrize
//CConfArenaPrestige::MapData CConfArenaPrestige::m_mapData;
//std::map<uint32, STC_CONF_ARENA_PRESTIGE> CConfArenaPrestige::m_mapView;
//std::vector<uint32> CConfArenaPrestige::m_vec1;
//std::vector<uint32> CConfArenaPrestige::m_vec2;
//std::vector<uint32> CConfArenaPrestige::m_vec3;
//
//bool CConfArenaPrestige::Init(const std::string& strFilePath )
//{
//	m_mapData.clear();
//
//	CConfigReader reader;
//	if (!reader.init(strFilePath))
//	{
//		return false;
//	}
//	int nRow = reader.rowCount();
//	if (nRow <= 1)
//	{
//		IME_ERROR("the row is small than 1 rowCount %u", nRow);
//		return false;
//	}
//	for (int i = 1; i < nRow; ++i)
//	{
//		int j = 0;
//		STC_CONF_ARENA_PRESTIGE info;
//		READER_INT(info.dwIndex);
//		READER_INT(info.byLevel);
//		READER_INT(info.byType);
//		READER_INT(info.dwItemIndex);
//		READER_INT(info.byStarLevel);
//		READER_INT(info.dwPrestige);
//		IS_READER_ERROR;
//
//		if (m_mapData.insert(std::make_pair(info.dwIndex, info)).second == false)
//		{
//			IME_ERROR("ERROR CONF DUPLICATE [ChestId=%u]", info.dwIndex);
//			return false;
//		}
//		if (info.byType == 1)
//		{
//			m_vec1.push_back(info.dwIndex);
//		}
//		else if (info.byType == 2)
//		{
//			m_vec2.push_back(info.dwIndex);
//		}
//		else 
//		{
//			m_vec3.push_back(info.dwIndex);
//		}
//	}
//
//	IME_LOG("LOAD CONF ARENA PRESTIGE [Size=%d]", m_mapData.size());
//
//	return true;
//}
//
//STC_CONF_ARENA_PRESTIGE* CConfArenaPrestige::Find(uint32 dwIndex)
//{
//	MapData::iterator it = m_mapData.find(dwIndex);
//	return it == m_mapData.end() ? NULL : &it->second;
//}
//
//void CConfArenaPrestige::FlushPrestigeCollect()
//{
//	m_mapView.clear();
//	std::vector<int> vecRand;
//	vecRand = CUtil::RandByRangeMulti(0, m_vec3.size(), 2);
//	for (int i = 0; i < vecRand.size(); i++)
//	{
//		m_mapView[m_vec3[vecRand[i]]] = *Find(m_vec3[vecRand[i]]);	
//	}
//	vecRand = CUtil::RandByRangeMulti(0, m_vec2.size(), 2);
//	for (int i = 0; i < vecRand.size(); i++)
//	{
//		m_mapView[m_vec2[vecRand[i]]] = *Find(m_vec2[vecRand[i]]);	
//	}
//	vecRand = CUtil::RandByRangeMulti(0, m_vec1.size(), 2);
//	for (int i = 0; i < vecRand.size(); i++)
//	{
//		m_mapView[m_vec1[vecRand[i]]] = *Find(m_vec1[vecRand[i]]);	
//	}
//}
//
//void CConfArenaPrestige::GetPrestigeCollect(std::map<uint32, STC_CONF_ARENA_PRESTIGE> &mapPresitige)
//{
//	mapPresitige = m_mapView;
//}
//
//bool CConfArenaPrestige::CollectionFind(uint32 index)
//{
//	if (m_mapView.find(index) == m_mapView.end())
//		return false;
//	return true;
//}
//
//// arena rank
//CConfArenaRank::MapData CConfArenaRank::m_mapData;
//
//bool CConfArenaRank::Init(const std::string& strFilePath )
//{
//	m_mapData.clear();
//
//	CConfigReader reader;
//	if (!reader.init(strFilePath))
//	{
//		return false;
//	}
//	int nRow = reader.rowCount();
//	if (nRow <= 1)
//	{
//		IME_ERROR("the row is small than 1 rowCount %u", nRow);
//		return false;
//	}
//	for (int i = 1; i < nRow; ++i)
//	{
//		int j = 0;
//		STC_CONF_ARENA_RANK info;
//		READER_INT(info.dwIndex);
//		READER_INT(info.dwLow);
//		READER_INT(info.dwHigh);
//		READER_INT(info.byType);
//		READER_INT(info.dwItemIndex);
//		READER_INT(info.byStarLevel);
//		READER_INT(info.dwNumber);
//		READER_INT(info.byType2);
//		READER_INT(info.dwItemIndex2);
//		READER_INT(info.dwNumber2);
//		IS_READER_ERROR;
//
//		if (m_mapData.insert(std::make_pair(info.dwIndex, info)).second == false)
//		{
//			IME_ERROR("ERROR CONF DUPLICATE [ChestId=%u]", info.dwIndex);
//			return false;
//		}
//	}
//
//	IME_LOG("LOAD CONF ARENA RANK [Size=%d]", m_mapData.size());
//
//	return true;
//}
//
//STC_CONF_ARENA_RANK* CConfArenaRank::Find(uint32 dwIndex)
//{
//	MapData::iterator it = m_mapData.find(dwIndex);
//	return it == m_mapData.end() ? NULL : &it->second;
//}
//
//STC_CONF_ARENA_RANK* CConfArenaRank::GetRow(uint32 level)
//{
//	std::map<uint32, STC_CONF_ARENA_RANK>::iterator it;
//	for (it = m_mapData.begin(); it != m_mapData.end(); it++)
//	{
//		if (level >= (it->second).dwLow && level <= (it->second).dwHigh)
//		{
//			return &it->second;
//		}
//	}
//	return NULL;
//}

CConfItemCollection::MapData CConfItemCollection::m_mapData;
std::vector<STC_CONF_ITEM_COLLECTION *> CConfItemCollection::m_vecData;
bool CConfItemCollection::Init( const std::string& strFilePath )
{
	m_mapData.clear();
	m_vecData.clear();

	CConfigReader reader;
	if (!reader.init(strFilePath))
	{
		return false;
	}
	int nRow = reader.rowCount();
	for (int i = 1; i < nRow; ++i)
	{
		int j = 0;
		STC_CONF_ITEM_COLLECTION info;
		READER_INT(info.dwIndex);
		READER_INT(info.dwItemIndex);
		READER_INT(info.byWeight);
		IS_READER_ERROR;

		m_mapData.insert(std::make_pair(info.dwIndex, info));
	}

	IME_LOG("LOAD CONF CARDDROP [Size=%d]", m_mapData.size());

	return true;
}

std::vector<STC_CONF_ITEM_COLLECTION *>* CConfItemCollection::Find(uint32 dwIndex)
{
	MapData::iterator it;
	std::pair<MapData::iterator, MapData::iterator> ret;
	ret = m_mapData.equal_range(dwIndex);
	m_vecData.clear();
	for (it = ret.first; it != ret.second; it++)
	{
		m_vecData.push_back(&((*it).second));
	}
	return &m_vecData;
}

bool CConfItemCollection::Valid( const std::string& strFilePath )
{
	MapData mapData;
	std::vector<STC_CONF_ITEM_COLLECTION *> vecData;

	CConfigReader reader;
	if (!reader.init(strFilePath))
	{
		return false;
	}
	int nRow = reader.rowCount();
	for (int i = 1; i < nRow; ++i)
	{
		int j = 0;
		STC_CONF_ITEM_COLLECTION info;
		READER_INT(info.dwIndex);
		READER_INT(info.dwItemIndex);
		READER_INT(info.byWeight);
		IS_READER_ERROR;

		mapData.insert(std::make_pair(info.dwIndex, info));
	}

	IME_LOG("LOAD CONF CARDDROP [Size=%d]", mapData.size());

	return true;
}


//CConfMissionBoss::MapData CConfMissionBoss::m_mapData;
//bool CConfMissionBoss::Init( const std::string& strFilePath )
//{
//	m_mapData.clear();
//
//	CConfigReader reader;
//	if (!reader.init(strFilePath))
//	{
//		return false;
//	}
//	int nRow = reader.rowCount();
//	for (int i = 1; i < nRow; ++i)
//	{
//		int j = 0;
//		STC_CONF_MISSION_BOSS info;
//		READER_INT(info.dwIndex);
//		READER_INT(info.dwCardIndex);
//		READER_INT(info.byAttribute);
//		READER_INT(info.dwAttack);
//		READER_INT(info.dwDefence);
//		READER_INT(info.dwHealth);
//		READER_INT(info.dwCritRate);
//		READER_INT(info.dwDodgeRate);
//		READER_INT(info.dwAction);
//		READER_INT(info.dwInitSkill);
//		READER_INT(info.dwBigSkill);
//		IS_READER_ERROR;
//
//		if (m_mapData.insert(std::make_pair(info.dwIndex, info)).second == false)
//		{
//			IME_ERROR("ERROR CONF DUPLICATE [ChestId=%u]", info.dwIndex);
//			return false;
//		}
//	}
//
//	IME_LOG("LOAD CONF MISSION [Size=%d]", m_mapData.size());
//
//	return true;
//}
//
//STC_CONF_MISSION_BOSS* CConfMissionBoss::Find(uint32 dwIndex)
//{
//	MapData::iterator it = m_mapData.find(dwIndex);
//	return it == m_mapData.end() ? NULL : &it->second;
//}

CConfGlobal::MapData CConfGlobal::m_mapData;
bool CConfGlobal::Init( const std::string& strFilePath )
{
	m_mapData.clear();

	CConfigReader reader;
	if (!reader.init(strFilePath))
	{
		return false;
	}
	int nRow = reader.rowCount();
	for (int i = 1; i < nRow; ++i)
	{
		int j = 0;
		uint32_t key;
		int32_t val;
		READER_INT(key);
		j++;
		READER_INT(val);
		IS_READER_ERROR;

		if (m_mapData.insert(std::make_pair(key, val)).second == false)
		{
			IME_ERROR("ERROR CONF DUPLICATE [ChestId=%u]", key);
			return false;
		}
	}

	IME_LOG("LOAD CONF Global [Size=%d]", m_mapData.size());

	return true;
}

int32_t* CConfGlobal::Find(uint32 dwIndex, int32_t &val)
{
	MapData::iterator it = m_mapData.find(dwIndex);
	if (it != m_mapData.end())
	{
	   	val = it->second;
		return &it->second;
	}
	else
	{
		return NULL;
	}
}

bool CConfGlobal::Valid( const std::string& strFilePath )
{
	MapData mapData;

	CConfigReader reader;
	if (!reader.init(strFilePath))
	{
		return false;
	}
	int nRow = reader.rowCount();
	for (int i = 1; i < nRow; ++i)
	{
		int j = 0;
		uint32_t key;
		int32_t val;
		READER_INT(key);
		j++;
		READER_INT(val);
		IS_READER_ERROR;

		if (mapData.insert(std::make_pair(key, val)).second == false)
		{
			IME_ERROR("ERROR CONF DUPLICATE [ChestId=%u]", key);
			return false;
		}
	}

	IME_LOG("LOAD CONF Global [Size=%d]", mapData.size());

	return true;
}

//group info 
CConfGroupInfo::MapData CConfGroupInfo::m_mapData;

bool CConfGroupInfo::Init(const std::string& strFilePath )
{
	m_mapData.clear();

	CConfigReader reader;
	if (!reader.init(strFilePath))
	{
		return false;
	}
	int nRow = reader.rowCount();
	if (nRow <= 1)
	{
		IME_ERROR("the row is small than 1 rowCount %u", nRow);
		return false;
	}
	for (int i = 1; i < nRow; ++i)
	{
		int j = 0;
		STC_CONF_GROUP_INFO info;
		READER_INT(info.dwIndex);
		READER_INT(info.dwNeedCredit);
		READER_INT(info.dwCreditLimit);
		READER_INT(info.wPersionLimit);
		READER_INT(info.position1Persion);
		READER_INT(info.position2Persion);
		READER_INT(info.position3Persion);
		READER_INT(info.position4Persion);
		IS_READER_ERROR;

		if (m_mapData.insert(std::make_pair(info.dwIndex, info)).second == false)
		{
			IME_ERROR("ERROR CONF DUPLICATE [ChestId=%u]", info.dwIndex);
			return false;
		}
	}

	IME_LOG("LOAD CONF GROUP [Size=%d]", m_mapData.size());

	return true;
}

STC_CONF_GROUP_INFO* CConfGroupInfo::Find(uint32 dwIndex)
{
	MapData::iterator it = m_mapData.find(dwIndex);
	return it == m_mapData.end() ? NULL : &it->second;
}

//group admin
CConfGroupAdmin::MapData CConfGroupAdmin::m_mapData;

bool CConfGroupAdmin::Init(const std::string& strFilePath )
{
	m_mapData.clear();

	CConfigReader reader;
	if (!reader.init(strFilePath))
	{
		return false;
	}
	int nRow = reader.rowCount();
	if (nRow <= 1)
	{
		IME_ERROR("the row is small than 1 rowCount %u", nRow);
		return false;
	}
	for (int i = 1; i < nRow; ++i)
	{
		int j = 0;
		STC_CONF_GROUP_ADMIN info;
		READER_INT(info.dwIndex);
		READER_INT(info.byAppoint);
		READER_INT(info.byLevelUp);
		READER_INT(info.byMemberIO);
		READER_INT(info.byUpdateNotice);
		READER_INT(info.byFlushTask);
		READER_INT(info.byMakeAlliance);
		READER_INT(info.byFight);
		READER_INT(info.byCredit);
		IS_READER_ERROR;

		if (m_mapData.insert(std::make_pair(info.dwIndex, info)).second == false)
		{
			IME_ERROR("ERROR CONF DUPLICATE [ChestId=%u]", info.dwIndex);
			return false;
		}
	}

	IME_LOG("LOAD CONF GROUP ADMIN [Size=%d]", m_mapData.size());

	return true;
}

STC_CONF_GROUP_ADMIN* CConfGroupAdmin::Find(uint32 dwIndex)
{
	MapData::iterator it = m_mapData.find(dwIndex);
	return it == m_mapData.end() ? NULL : &it->second;
}

uint32 CConfGroupAdmin::GetVal(uint32 posion, uint32 auth)
{
	STC_CONF_GROUP_ADMIN *pConf = CConfGroupAdmin::Find(posion);
	if (pConf != NULL)
	{
		switch (auth)
		{
			case E_GA_APPOINT:
				return pConf->byAppoint;
			case E_GA_LEVELUP:
				return pConf->byLevelUp;
			case E_GA_MEMBER_IO:
				return pConf->byMemberIO;
			case E_GA_UPDATE_NOTICE:
				return pConf->byUpdateNotice;
			case E_GA_FLUSH_TASK:
				return pConf->byFlushTask;
			case E_GA_MAKE_ALLIANCE:
				return pConf->byMakeAlliance;
			case E_GA_FIGHT:
				return pConf->byFight;
			case E_GA_CREDIT:
				return pConf->byCredit;
		}
	}
	return 0;
}

//group mission
CConfGroupMission::MapData CConfGroupMission::m_mapData;

bool CConfGroupMission::Init(const std::string& strFilePath )
{
	m_mapData.clear();

	CConfigReader reader;
	if (!reader.init(strFilePath))
	{
		return false;
	}
	int nRow = reader.rowCount();
	if (nRow <= 1)
	{
		IME_ERROR("the row is small than 1 rowCount %u", nRow);
		return false;
	}
	for (int i = 1; i < nRow; ++i)
	{
		int j = 0;
		STC_CONF_GROUP_MISSION info;
		READER_INT(info.dwIndex);
		READER_INT(info.wLevelLimit);
		READER_INT(info.byStepLimit);
		READER_INT(info.wConsumeEnergy);
		READER_INT(info.wGetCredit);
		READER_INT(info.wGetCoin);
		READER_INT(info.byRate);
		READER_INT(info.dwRandomEventIndex);
		READER_INT(info.dwBoss);
		READER_INT(info.byDurationCmd);
		IS_READER_ERROR;

		if (m_mapData.insert(std::make_pair(info.dwIndex, info)).second == false)
		{
			IME_ERROR("ERROR CONF DUPLICATE [ChestId=%u]", info.dwIndex);
			return false;
		}
	}

	IME_LOG("LOAD CONF GROUP MISSION [Size=%d]", m_mapData.size());

	return true;
}

STC_CONF_GROUP_MISSION* CConfGroupMission::Find(uint32 dwIndex)
{
	MapData::iterator it = m_mapData.find(dwIndex);
	return it == m_mapData.end() ? NULL : &it->second;
}

//group prize

CConfGroupPrize::MapData CConfGroupPrize::m_mapData;

bool CConfGroupPrize::Init(const std::string& strFilePath )
{
	m_mapData.clear();

	CConfigReader reader;
	if (!reader.init(strFilePath))
	{
		return false;
	}
	int nRow = reader.rowCount();
	if (nRow <= 1)
	{
		IME_ERROR("the row is small than 1 rowCount %u", nRow);
		return false;
	}
	for (int i = 1; i < nRow; ++i)
	{
		int j = 0;
		STC_CONF_GROUP_PRIZE info;
		READER_INT(info.dwIndex);
		READER_INT(info.byType);
		READER_INT(info.dwCombineId);
		READER_INT(info.dwConsumeCredit);
		IS_READER_ERROR;

		if (m_mapData.insert(std::make_pair(info.dwIndex, info)).second == false)
		{
			IME_ERROR("ERROR CONF DUPLICATE [ChestId=%u]", info.dwIndex);
			return false;
		}
	}

	IME_LOG("LOAD CONF GROUP PRIZE [Size=%d]", m_mapData.size());

	return true;
}

STC_CONF_GROUP_PRIZE* CConfGroupPrize::Find(uint32 dwIndex)
{
	MapData::iterator it = m_mapData.find(dwIndex);
	return it == m_mapData.end() ? NULL : &it->second;
}

//jewelskill strength
//CConfJewelSkillStrength::MapData CConfJewelSkillStrength::m_mapData;
//
//bool CConfJewelSkillStrength::Init(const std::string& strFilePath )
//{
//	m_mapData.clear();
//
//	CConfigReader reader;
//	if (!reader.init(strFilePath))
//	{
//		return false;
//	}
//	int nRow = reader.rowCount();
//	if (nRow <= 1)
//	{
//		IME_ERROR("the row is small than 1 rowCount %u", nRow);
//		return false;
//	}
//	for (int i = 1; i < nRow; ++i)
//	{
//		int j = 0;
//		STC_CONF_JEWEL_SKILL_STRENGTH info;
//		READER_INT(info.dwIndex);
//		READER_INT(info.dwSkillPara);
//		IS_READER_ERROR;
//
//		if (m_mapData.insert(std::make_pair(info.dwIndex, info)).second == false)
//		{
//			IME_ERROR("ERROR CONF DUPLICATE [ChestId=%u]", info.dwIndex);
//			return false;
//		}
//	}
//
//	IME_LOG("LOAD CONF JewelSkill para [Size=%d]", m_mapData.size());
//
//	return true;
//}
//
//STC_CONF_JEWEL_SKILL_STRENGTH* CConfJewelSkillStrength::Find(uint32 dwIndex)
//{
//	MapData::iterator it = m_mapData.find(dwIndex);
//	return it == m_mapData.end() ? NULL : &it->second;
//}

//skillcreate strength
//std::vector<int> CConfSkillCreate::m_vec2Star;
//std::vector<int> CConfSkillCreate::m_vec2InitPro;
//
//std::vector<int> CConfSkillCreate::m_vec3Star;
//std::vector<int> CConfSkillCreate::m_vec3InitPro;
//
//std::vector<int> CConfSkillCreate::m_vec4Star;
//std::vector<int> CConfSkillCreate::m_vec4InitPro;
//
//std::vector<int> CConfSkillCreate::m_vec5Star;
//std::vector<int> CConfSkillCreate::m_vec5InitPro;
//
//CConfSkillCreate::MapData CConfSkillCreate::m_mapData2;
//CConfSkillCreate::MapData CConfSkillCreate::m_mapData3;
//CConfSkillCreate::MapData CConfSkillCreate::m_mapData4;
//CConfSkillCreate::MapData CConfSkillCreate::m_mapData5;
//
//bool CConfSkillCreate::Init(const std::string& strFilePath )
//{
////	m_mapData.clear();
//
//	CConfigReader reader;
//	if (!reader.init(strFilePath))
//	{
//		return false;
//	}
//	int nRow = reader.rowCount();
//	if (nRow <= 1)
//	{
//		IME_ERROR("the row is small than 1 rowCount %u", nRow);
//		return false;
//	}
//	for (int i = 1; i < nRow; ++i)
//	{
//		int j = 0;
//		STC_CONF_SKILL_CREATE info;
//		READER_INT(info.byStar);
//		READER_INT(info.dwSkillIndex);
//		READER_INT(info.dwInitPro);
//		READER_INT(info.dwLowPro);
//		READER_INT(info.dwHighPro);
//		IS_READER_ERROR;
//
//		switch(info.byStar)
//		{
//			case 2:
//				m_vec2Star.push_back(info.dwSkillIndex);
//				m_vec2InitPro.push_back(info.dwInitPro);
//				if (m_mapData2.insert(std::make_pair(info.dwSkillIndex, info)).second == false)
//				{
//					IME_ERROR("ERROR CONF DUPLICATE SKILL CREATE[SkillBirth Id=%u]", info.dwSkillIndex);
//					return false;
//				}
//				break;
//			case 3:
//				m_vec3Star.push_back(info.dwSkillIndex);
//				m_vec3InitPro.push_back(info.dwInitPro);
//				if (m_mapData3.insert(std::make_pair(info.dwSkillIndex, info)).second == false)
//				{
//					IME_ERROR("ERROR CONF DUPLICATE SKILL CREATE[SkillBirth Id=%u]", info.dwSkillIndex);
//					return false;
//				}
//				break;
//			case 4:
//				m_vec4Star.push_back(info.dwSkillIndex);
//				m_vec4InitPro.push_back(info.dwInitPro);
//				if (m_mapData4.insert(std::make_pair(info.dwSkillIndex, info)).second == false)
//				{
//					IME_ERROR("ERROR CONF DUPLICATE SKILL CREATE[SkillBirth Id=%u]", info.dwSkillIndex);
//					return false;
//				}
//				break;
//			case 5:
//				m_vec5Star.push_back(info.dwSkillIndex);
//				m_vec5InitPro.push_back(info.dwInitPro);
//				if (m_mapData5.insert(std::make_pair(info.dwSkillIndex, info)).second == false)
//				{
//					IME_ERROR("ERROR CONF DUPLICATE SKILL CREATE[SkillBirth Id=%u]", info.dwSkillIndex);
//					return false;
//				}
//				break;
//			default:
//				IME_ERROR("star [%u] is not valid", info.byStar);
//				return false;
//		}
//
////		if (m_mapData.insert(std::make_pair(info.dwSkillIndex, info)).second == false)
////		{
////			IME_ERROR("ERROR CONF DUPLICATE SKILL CREATE[ChestId=%u]", info.dwSkillIndex);
////			return false;
////		}
//		
//	}
//
////	IME_LOG("LOAD CONF skill create para [Size=%d]", m_mapData.size());
//
//	return true;
//}
//
//STC_CONF_SKILL_CREATE* CConfSkillCreate::Find(uint32 dwIndex, uint8 star)
//{
//	if (star == 2)
//	{
//		MapData::iterator it = m_mapData2.find(dwIndex);
//		return it == m_mapData2.end() ? NULL : &it->second;
//	}
//	else if (star == 3)
//	{
//		MapData::iterator it = m_mapData3.find(dwIndex);
//		return it == m_mapData3.end() ? NULL : &it->second;
//	}
//	else if (star == 4)
//	{
//		MapData::iterator it = m_mapData4.find(dwIndex);
//		return it == m_mapData4.end() ? NULL : &it->second;
//	}
//	else if (star == 5)
//	{
//		MapData::iterator it = m_mapData5.find(dwIndex);
//		return it == m_mapData5.end() ? NULL : &it->second;
//	}
//	else
//	{
//		return NULL;
//	}
//}
//
//uint32 CConfSkillCreate::GetSkillIndex(uint8 star)
//{
//	int number;
//	switch(star)
//	{
//		case 2:
//			if (m_vec2InitPro.size() == 0)
//				return 0;
//			number = CUtil::RandFactor(m_vec2InitPro);
//			if (number < 0)
//			{
//				return 0;
//			}
//			else
//			{
//				return m_vec2Star[number];
//			}
//			break;
//		case 3:
//			if (m_vec3InitPro.size() == 0)
//				return 0;
//			number = CUtil::RandFactor(m_vec3InitPro);
//			if (number < 0)
//			{
//				return 0;
//			}
//			else
//			{
//				return m_vec3Star[number];
//			}
//			break;
//		case 4:
//			if (m_vec4InitPro.size() == 0)
//				return 0;
//			number = CUtil::RandFactor(m_vec4InitPro);
//			if (number < 0)
//			{
//				return 0;
//			}
//			else
//			{
//				return m_vec4Star[number];
//			}
//			break;
//		case 5:
//			if (m_vec5InitPro.size() == 0)
//				return 0;
//			number = CUtil::RandFactor(m_vec5InitPro);
//			if (number < 0)
//			{
//				return 0;
//			}
//			else
//			{
//				return m_vec5Star[number];
//			}
//			break;
//		default:
//			IME_ERROR("star [%u] is not valid", star);
//			return 0;
//	}
//}

//WeaponRestriction conf
//std::vector<std::vector<uint32> >  CConfWeaponRestriction::m_vecData;
//bool CConfWeaponRestriction::Init( const std::string& strFilePath )
//{
//	m_vecData.clear();
//
//	CConfigReader reader;
//	if (!reader.init(strFilePath))
//	{
//		return false;
//	}
//	int nRow = reader.rowCount();
//	std::vector<uint32> vecOneAttri;
//	for (int i = 1; i < nRow; ++i)
//	{
//		vecOneAttri.clear();
//		int j = 1;
//		for (; j < 11; )
//		{
//			uint32 info;
//			READER_INT(info);
////			IME_LOG("i %u j %u %u", i, j, info);
//			IS_READER_ERROR;
//
//			vecOneAttri.push_back(info);
//		}
//
//		m_vecData.push_back(vecOneAttri);
//	}
//
//	IME_LOG("LOAD CONF NATURE [Size=%d]", m_vecData.size());
//
//	return true;
//}
//
//uint32* CConfWeaponRestriction::Find(uint32 dwIndex, uint32 dwIndex2)
//{
//	if (dwIndex >= m_vecData.size())
//		return NULL;
//	if (dwIndex2 >= m_vecData[dwIndex].size())
//		return NULL;
//	return &(m_vecData[dwIndex][dwIndex2]);
//}

// egg collection
CConfUniqueCollection::MapData CConfUniqueCollection::m_mapData;
std::vector<STC_CONF_UNIQUE_COLLECTION *> CConfUniqueCollection::m_vecData;

bool CConfUniqueCollection::ReadConfig(const std::string& strFilePath, MapData &tmp)
{
	CConfigReader reader;
	if (!reader.init(strFilePath))
	{
		return false;
	}
	int nRow = reader.rowCount();
	for (int i = 1; i < nRow; ++i)
	{
		int j = 0;
		STC_CONF_UNIQUE_COLLECTION info;
//		j++;
		READER_INT(info.dwIndex);
		READER_INT(info.wWeight);
		READER_INT(info.byType);
		READER_INT(info.dwPara1);
		READER_INT(info.dwPara2);
		READER_STR(info.strContent);
		IS_READER_ERROR;
//		IME_LOG("%u %u %u ", info.dwIndex, info.dwRandomEventIndex, info.wWeight);
		
		if (info.byType == E_OBJ_SHOP)
		{
			if (CConfGoods::Find(info.dwPara1) == NULL)
			{
				IME_ERROR("unique collection index %u can not find item index %u", info.dwIndex, info.dwPara1);
				return false;
			}
		}
		if (info.byType == E_OBJ_CARD || info.byType == E_OBJ_SKILL || info.byType == E_OBJ_AWAKE_CARD)
		{
			if (CConfCardBase::Find(info.dwPara1) == NULL)
			{
				IME_ERROR("unique collection index %u can not find card index %u", info.dwIndex, info.dwPara2);
				return false;
			}
		}

		tmp.insert(std::make_pair(info.dwIndex, info));
	}

	IME_LOG("LOAD CONF CARDDROP [Size=%d]", tmp.size());
	return true;
}

bool CConfUniqueCollection::Init( const std::string& strFilePath )
{
	m_mapData.clear();
	m_vecData.clear();
	if (!ReadConfig(strFilePath, m_mapData))
	{
		IME_ERROR("init read config fail");
		return false;
	}
	return true;
}

std::vector<STC_CONF_UNIQUE_COLLECTION *>* CConfUniqueCollection::Find(uint32 dwIndex)
{
	MapData::iterator it;
	std::pair<MapData::iterator, MapData::iterator> ret;
	ret = m_mapData.equal_range(dwIndex);
	m_vecData.clear();
	for (it = ret.first; it != ret.second; it++)
	{
		m_vecData.push_back(&((*it).second));
	}
	return &m_vecData;
}

STC_CONF_UNIQUE_COLLECTION* CConfUniqueCollection::GetRandResult(uint32 index)
{
	Find(index);
	std::vector<int> vecRand;
	for (int i = 0; i < m_vecData.size(); i++)
	{
		vecRand.push_back(m_vecData[i]->wWeight);
	}
	if (vecRand.empty())
	{
		return 0;
	}
	int ret = CUtil::RandFactor(vecRand);
	return m_vecData[ret]; 
}

bool CConfUniqueCollection::Valid( const std::string& strFilePath )
{
	MapData mapData;
	std::vector<STC_CONF_UNIQUE_COLLECTION *> vecData;
	if (!ReadConfig(strFilePath, mapData))
	{
		IME_ERROR("init read config fail");
		return false;
	}
	return true;
}


// skill collection
//CConfSkillCollection::MapData CConfSkillCollection::m_mapData;
//std::vector<STC_CONF_SKILL_COLLECTION *> CConfSkillCollection::m_vecData;
//bool CConfSkillCollection::Init( const std::string& strFilePath )
//{
//	m_mapData.clear();
//
//	CConfigReader reader;
//	if (!reader.init(strFilePath))
//	{
//		return false;
//	}
//	int nRow = reader.rowCount();
//	for (int i = 1; i < nRow; ++i)
//	{
//		int j = 0;
//		STC_CONF_SKILL_COLLECTION info;
//		j++;
//		READER_INT(info.dwIndex);
//		READER_INT(info.dwSkillIndex);
//		READER_INT(info.wWeight);
//		IS_READER_ERROR;
//
//		m_mapData.insert(std::make_pair(info.dwIndex, info));
//	}
//
//	IME_LOG("LOAD CONF CARDDROP [Size=%d]", m_mapData.size());
//
//	return true;
//}
//
//std::vector<STC_CONF_SKILL_COLLECTION *>* CConfSkillCollection::Find(uint32 dwIndex)
//{
//	MapData::iterator it;
//	std::pair<MapData::iterator, MapData::iterator> ret;
//	ret = m_mapData.equal_range(dwIndex);
//	m_vecData.clear();
//	for (it = ret.first; it != ret.second; it++)
//	{
//		m_vecData.push_back(&((*it).second));
//	}
//	return &m_vecData;
//}
//
//STC_CONF_SKILL_COLLECTION* CConfSkillCollection::GetRandResult(uint32 index)
//{
//	Find(index);
//	std::vector<int> vecRand;
//	for (int i = 0; i < m_vecData.size(); i++)
//	{
//		vecRand.push_back(m_vecData[i]->wWeight);
//	}
//	if (vecRand.empty())
//	{
//		return 0;
//	}
//	int ret = CUtil::RandFactor(vecRand);
//	return m_vecData[ret]; 
//}

//gift collection 
CConfGiftCollection::MapData CConfGiftCollection::m_mapData;
std::vector<STC_CONF_GIFT_COLLECTION *> CConfGiftCollection::m_vecData;

bool CConfGiftCollection::ReadConfig(const std::string& strFilePath, MapData &tmp)
{
	CConfigReader reader;
	if (!reader.init(strFilePath))
	{
		return false;
	}
	int nRow = reader.rowCount();
	if (nRow <= 1)
	{
		IME_ERROR("the row is small than 1 rowCount %u", nRow);
		return false;
	}
	for (int i = 1; i < nRow; ++i)
	{
		int j = 0;
		STC_CONF_GIFT_COLLECTION info;
//		j++;
		READER_INT(info.dwIndex);
		READER_INT(info.wType1);
		READER_INT(info.dwProductIndex1);
		READER_INT(info.dwNumber1);
		READER_INT(info.wType2);
		READER_INT(info.dwProductIndex2);
		READER_INT(info.dwNumber2);
		READER_INT(info.wType3);
		READER_INT(info.dwProductIndex3);
		READER_INT(info.dwNumber3);
		READER_INT(info.wType4);
		READER_INT(info.dwProductIndex4);
		READER_INT(info.dwNumber4);
		READER_INT(info.wWeight);
		IS_READER_ERROR;
		std::vector<uint32_t> vecitem;
		std::vector<uint32_t> vecCard;
		if (info.wType1 == E_OBJ_SHOP)
		{
			vecitem.push_back(info.dwProductIndex1);
		}
		if (info.wType2 == E_OBJ_SHOP)
		{
			vecitem.push_back(info.dwProductIndex2);
		}
		if (info.wType3 == E_OBJ_SHOP)
		{
			vecitem.push_back(info.dwProductIndex3);
		}
		if (info.wType4 == E_OBJ_SHOP)
		{
			vecitem.push_back(info.dwProductIndex4);
		}	
		if (info.wType1 == E_OBJ_CARD || info.wType1 == E_OBJ_SKILL || info.wType1 == E_OBJ_AWAKE_CARD)
			vecCard.push_back(info.dwProductIndex1);
		if (info.wType2 == E_OBJ_CARD || info.wType2 == E_OBJ_SKILL || info.wType2 == E_OBJ_AWAKE_CARD)
			vecCard.push_back(info.dwProductIndex2);
		if (info.wType3 == E_OBJ_CARD || info.wType3 == E_OBJ_SKILL || info.wType3 == E_OBJ_AWAKE_CARD)
			vecCard.push_back(info.dwProductIndex3);
		if (info.wType4 == E_OBJ_CARD || info.wType4 == E_OBJ_SKILL || info.wType4 == E_OBJ_AWAKE_CARD)
			vecCard.push_back(info.dwProductIndex4);

		for (int i = 0; i < vecitem.size(); i++)
		{
			if (vecitem[i] == 0)
				continue;
			if (CConfGoods::Find(vecitem[i]) == NULL)
			{
				IME_ERROR("gift collection index %u can not find item index %u", info.dwIndex, vecitem[i]);
				return false;
			}
		}
		for (int i = 0; i < vecCard.size(); i++)
		{
			if (vecCard[i] == 0)
				continue;
			if (CConfCardBase::Find(vecCard[i]) == NULL)
			{
				IME_ERROR("gift collection index %u can not find card index %u", info.dwIndex, vecCard[i]);
				return false;
			}
		}

		tmp.insert(std::make_pair(info.dwIndex, info));
	}

	IME_LOG("LOAD CONF GROUP MISSION [Size=%d]", tmp.size());
	return true;
}

bool CConfGiftCollection::Init(const std::string& strFilePath )
{
	m_mapData.clear();
	m_vecData.clear();
	if (!ReadConfig(strFilePath, m_mapData))
	{
		IME_ERROR("init read config fail");
		return false;
	}
	return true;
}

std::vector<STC_CONF_GIFT_COLLECTION*>* CConfGiftCollection::Find(uint32 dwIndex)
{
	MapData::iterator it;
	std::pair<MapData::iterator, MapData::iterator> ret;
	ret = m_mapData.equal_range(dwIndex);
	m_vecData.clear();
	for (it = ret.first; it != ret.second; it++)
	{
		m_vecData.push_back(&((*it).second));
	}
	return &m_vecData;
}

STC_CONF_GIFT_COLLECTION* CConfGiftCollection::GetRandResult(uint32 index)
{
	Find(index);
	std::vector<int> vecRand;
	for (int i = 0; i < m_vecData.size(); i++)
	{
		vecRand.push_back(m_vecData[i]->wWeight);
	}
	if (vecRand.empty())
	{
		return 0;
	}
	int ret = CUtil::RandFactor(vecRand);
	return m_vecData[ret]; 
}

bool CConfGiftCollection::Valid(const std::string& strFilePath )
{
	MapData mapData;
	if (!ReadConfig(strFilePath, mapData))
	{
		IME_ERROR("valid read config fail");
		return false;
	}
	return true;
}


// shop collection
CConfGoodsCollection::MapData CConfGoodsCollection::m_mapData;
std::vector<STC_CONF_GOODS_COLLECTION *> CConfGoodsCollection::m_vecData;

bool CConfGoodsCollection::ReadConfig(const std::string& strFilePath, MapData &map)
{
	CConfigReader reader;
	if (!reader.init(strFilePath))
	{
		return false;
	}
	int nRow = reader.rowCount();
	for (int i = 1; i < nRow; ++i)
	{
		int j = 0;
		STC_CONF_GOODS_COLLECTION info;
		READER_INT(info.dwIndex);
		READER_INT(info.dwItemIndex);
		READER_INT(info.byWeight);
		READER_INT(info.dwNumber);
		READER_STR(info.strContent);
		IS_READER_ERROR;
		if (CConfGoods::Find(info.dwItemIndex) == NULL)
		{
			IME_ERROR("goods collection index %u can not find item %u", info.dwIndex, info.dwItemIndex);
			return false;
		}

		map.insert(std::make_pair(info.dwIndex, info));
	}

	IME_LOG("LOAD CONF CARDDROP [Size=%d]", map.size());

	return true;
}

bool CConfGoodsCollection::Init( const std::string& strFilePath )
{
	m_mapData.clear();
	m_vecData.clear();
	if (!ReadConfig(strFilePath, m_mapData))
	{
		IME_ERROR("init read config fail");
		return false;
	}
	return true;
}

std::vector<STC_CONF_GOODS_COLLECTION *>* CConfGoodsCollection::Find(uint32 dwIndex)
{
	MapData::iterator it;
	std::pair<MapData::iterator, MapData::iterator> ret;
	ret = m_mapData.equal_range(dwIndex);
	m_vecData.clear();
	for (it = ret.first; it != ret.second; it++)
	{
		m_vecData.push_back(&((*it).second));
	}
	return &m_vecData;
}

bool CConfGoodsCollection::Valid( const std::string& strFilePath )
{
	MapData m_mapData;
	std::vector<STC_CONF_GOODS_COLLECTION> m_vecData;

	if (!ReadConfig(strFilePath, m_mapData))
	{
		IME_ERROR("valid read config fail");
		return false;
	}
	return true;
}


//master
CConfMaster::MapData CConfMaster::m_mapData;

bool CConfMaster::Init(const std::string& strFilePath )
{
	m_mapData.clear();
	
	if (!CConfMasterBase::Init(strFilePath + "Master.csv"))
	{
		IME_ERROR("master base init fail");
		return false;
	}
	if (!CConfMasterPro::Init(strFilePath + "MasterProbability.csv"))
	{
		IME_ERROR("master pro init fail");
		return false;
	}
	CConfMasterBase::MapData *pbase = CConfMasterBase::GetVal();
	CConfMasterPro::MapData *ppro = CConfMasterPro::GetVal();
	CConfMasterBase::MapData::iterator it;
	CConfMasterPro::MapData::iterator itpro;

	for (it = pbase->begin(); it != pbase->end(); it++)
	{
		int j = 0;
		STC_CONF_MASTER info;
		info.dwIndex = it->second.dwIndex;
		info.strName = it->second.strName;
		info.dwStar = it->second.dwStar;
		info.byType = it->second.byType;
		info.strSkillName = it->second.strSkillName;
		info.dwSkillIndex = it->second.dwSkillIndex;
		info.dwNumberMax = it->second.dwNumberMax;
		info.dwNeedLevel = it->second.dwNeedLevel;
		info.dwAliveTime = it->second.dwAliveTime;
		info.dwSendRedCon = it->second.dwSendRedCon;
		info.dwBlessCD = it->second.dwBlessCD;
		info.dwMasterCardIndex = it->second.dwMasterCardIndex;
		info.dwFixPro = it->second.dwFixPro;		

		itpro = ppro->find(info.dwIndex);
		if (itpro != ppro->end())
		{
			info.dwLowPro = itpro->second.dwLowPro;
			info.dwMiddlePro = itpro->second.dwMiddlePro;
			info.dwHighPro = itpro->second.dwHighPro;
			info.dwFirstLowPro = itpro->second.dwFirstLowPro;
			info.dwFirstMiddlePro = itpro->second.dwMiddlePro;
			info.dwFirstHighPro = itpro->second.dwFirstHighPro;	
		}
		else
		{
			info.dwLowPro = 0;
			info.dwMiddlePro = 0;
			info.dwHighPro = 0;
			info.dwFirstLowPro = 0;
			info.dwFirstMiddlePro = 0;
			info.dwFirstHighPro = 0;
		}
//		IME_LOG("index %u strname %s star %u type %u strSkillName %s skillindex %u numbermax %u, needlevel %u, alivetime %u, sendredcon %u, blessCd %u, mastercardindex %u, fixpro %u, lowpro %u, dwMiddlePro %u, highpro %u, first low pro %u, first middle pro %u, first high pro %u", info.dwIndex, info.strName.c_str(), info.dwStar, info.byType, info.strSkillName.c_str(), info.dwSkillIndex, info.dwNumberMax, info.dwNeedLevel, info.dwAliveTime, info.dwSendRedCon, info.dwBlessCD, info.dwMasterCardIndex, info.dwFixPro, info.dwLowPro, info.dwMiddlePro, info.dwHighPro, info.dwFirstLowPro, info.dwFirstMiddlePro, info.dwFirstHighPro); 

		if (m_mapData.insert(std::make_pair(info.dwIndex, info)).second == false)
		{
			IME_ERROR("ERROR CONF DUPLICATE [ChestId=%u]", info.dwIndex);
			return false;
		}
	}

	IME_LOG("LOAD CONF JewelSkill para [Size=%d]", m_mapData.size());

	return true;
}

STC_CONF_MASTER* CConfMaster::Find(uint32 dwIndex)
{
	MapData::iterator it = m_mapData.find(dwIndex);
	return it == m_mapData.end() ? NULL : &it->second;
}

STC_CONF_MASTER* CConfMaster::RandGet(uint32 level, uint32 star_s, uint32 star_e, int pro)
{
	MapData::iterator it;
	std::vector<int> vec;
	std::vector<int> vecPro;
	for (it = m_mapData.begin(); it != m_mapData.end(); it++)
	{
		if ((it->second).dwStar >= star_s && (it->second).dwStar <= star_e && (it->second).dwNeedLevel <= level)
		{
			vec.push_back(it->first);
			switch(pro)
			{
				case 1:
					vecPro.push_back((it->second).dwLowPro);
					break;
				case 2:
					vecPro.push_back((it->second).dwMiddlePro);
					break;
				case 3:
					vecPro.push_back((it->second).dwHighPro);
					break;
				default:
					return NULL;
			}
		}	
	}
	
	if (vec.size() == 0)
	{
		return NULL;
	}
	else
	{
		uint32 val = CUtil::RandFactor(vecPro);
		return &m_mapData[vec[val]];
	}
}

STC_CONF_MASTER* CConfMaster::RandGetFirst(uint32 level, uint32 star_s, uint32 star_e, int pro)
{
	MapData::iterator it;
	std::vector<int> vec;
	std::vector<int> vecPro;
	for (it = m_mapData.begin(); it != m_mapData.end(); it++)
	{
		if ((it->second).dwStar >= star_s && (it->second).dwStar <= star_e && (it->second).dwNeedLevel <= level)
		{
			vec.push_back(it->first);
			switch(pro)
			{
				case 1:
					vecPro.push_back((it->second).dwFirstLowPro);
					break;
				case 2:
					vecPro.push_back((it->second).dwFirstMiddlePro);
					break;
				case 3:
					vecPro.push_back((it->second).dwFirstHighPro);
					break;
				default:
					return NULL;
			}
		}	
	}
	
	if (vec.size() == 0)
	{
		return NULL;
	}
	else
	{
		uint32 val = CUtil::RandFactor(vecPro);
		return &m_mapData[vec[val]];
	}
}

bool CConfMaster::Valid(const std::string& strFilePath )
{
	MapData m_mapData;
	
	if (!CConfMasterBase::Init(strFilePath + "Master.csv"))
	{
		IME_ERROR("master base init fail");
		return false;
	}
	if (!CConfMasterPro::Init(strFilePath + "MasterProbability.csv"))
	{
		IME_ERROR("master pro init fail");
		return false;
	}
	CConfMasterBase::MapData *pbase = CConfMasterBase::GetVal();
	CConfMasterPro::MapData *ppro = CConfMasterPro::GetVal();
	CConfMasterBase::MapData::iterator it;
	CConfMasterPro::MapData::iterator itpro;

	for (it = pbase->begin(); it != pbase->end(); it++)
	{
		int j = 0;
		STC_CONF_MASTER info;
		info.dwIndex = it->second.dwIndex;
		info.strName = it->second.strName;
		info.dwStar = it->second.dwStar;
		info.byType = it->second.byType;
		info.strSkillName = it->second.strSkillName;
		info.dwSkillIndex = it->second.dwSkillIndex;
		info.dwNumberMax = it->second.dwNumberMax;
		info.dwNeedLevel = it->second.dwNeedLevel;
		info.dwAliveTime = it->second.dwAliveTime;
		info.dwSendRedCon = it->second.dwSendRedCon;
		info.dwBlessCD = it->second.dwBlessCD;
		info.dwMasterCardIndex = it->second.dwMasterCardIndex;
		info.dwFixPro = it->second.dwFixPro;		

		itpro = ppro->find(info.dwIndex);
		if (itpro != ppro->end())
		{
			info.dwLowPro = itpro->second.dwLowPro;
			info.dwMiddlePro = itpro->second.dwMiddlePro;
			info.dwHighPro = itpro->second.dwHighPro;
			info.dwFirstLowPro = itpro->second.dwFirstLowPro;
			info.dwFirstMiddlePro = itpro->second.dwMiddlePro;
			info.dwFirstHighPro = itpro->second.dwFirstHighPro;	
		}
		else
		{
			info.dwLowPro = 0;
			info.dwMiddlePro = 0;
			info.dwHighPro = 0;
			info.dwFirstLowPro = 0;
			info.dwFirstMiddlePro = 0;
			info.dwFirstHighPro = 0;
		}
//		IME_LOG("index %u strname %s star %u type %u strSkillName %s skillindex %u numbermax %u, needlevel %u, alivetime %u, sendredcon %u, blessCd %u, mastercardindex %u, fixpro %u, lowpro %u, dwMiddlePro %u, highpro %u, first low pro %u, first middle pro %u, first high pro %u", info.dwIndex, info.strName.c_str(), info.dwStar, info.byType, info.strSkillName.c_str(), info.dwSkillIndex, info.dwNumberMax, info.dwNeedLevel, info.dwAliveTime, info.dwSendRedCon, info.dwBlessCD, info.dwMasterCardIndex, info.dwFixPro, info.dwLowPro, info.dwMiddlePro, info.dwHighPro, info.dwFirstLowPro, info.dwFirstMiddlePro, info.dwFirstHighPro); 

		if (m_mapData.insert(std::make_pair(info.dwIndex, info)).second == false)
		{
			IME_ERROR("ERROR CONF DUPLICATE [ChestId=%u]", info.dwIndex);
			return false;
		}
	}

	IME_LOG("LOAD CONF JewelSkill para [Size=%d]", m_mapData.size());
	return true;
}


// master probability
CConfMasterPro::MapData CConfMasterPro::m_mapData;

bool CConfMasterPro::Init(const std::string& strFilePath )
{
	m_mapData.clear();

	CConfigReader reader;
	if (!reader.init(strFilePath))
	{
		return false;
	}
	int nRow = reader.rowCount();
	if (nRow <= 1)
	{
		IME_ERROR("the row is small than 1 rowCount %u", nRow);
		return false;
	}
	for (int i = 1; i < nRow; ++i)
	{
		int j = 0;
		STC_CONF_MASTER_PRO info;
		READER_INT(info.dwIndex);
		READER_INT(info.dwLowPro);
		READER_INT(info.dwMiddlePro);
		READER_INT(info.dwHighPro);
		READER_INT(info.dwFirstLowPro);
		READER_INT(info.dwFirstMiddlePro);
		READER_INT(info.dwFirstHighPro);
		IS_READER_ERROR;

		if (m_mapData.insert(std::make_pair(info.dwIndex, info)).second == false)
		{
			IME_ERROR("ERROR CONF DUPLICATE [ChestId=%u]", info.dwIndex);
			return false;
		}
	}

	IME_LOG("LOAD CONF JewelSkill para [Size=%d]", m_mapData.size());

	return true;
}

STC_CONF_MASTER_PRO* CConfMasterPro::Find(uint32 dwIndex)
{
	MapData::iterator it = m_mapData.find(dwIndex);
	return it == m_mapData.end() ? NULL : &it->second;
}

// skill up
//CConfSkillLvUp::MapData CConfSkillLvUp::m_mapData;
//
//bool CConfSkillLvUp::Init(const std::string& strFilePath )
//{
//	m_mapData.clear();
//
//	CConfigReader reader;
//	if (!reader.init(strFilePath))
//	{
//		return false;
//	}
//	int nRow = reader.rowCount();
//	if (nRow <= 1)
//	{
//		IME_ERROR("the row is small than 1 rowCount %u", nRow);
//		return false;
//	}
//	for (int i = 1; i < nRow; ++i)
//	{
//		int j = 0;
//		STC_CONF_SKILL_LVUP info;
//		READER_INT(info.dwIndex);
//		READER_INT(info.dwMaxLv);
//		IS_READER_ERROR;
//
//		if (m_mapData.insert(std::make_pair(info.dwIndex, info)).second == false)
//		{
//			IME_ERROR("ERROR CONF DUPLICATE [ChestId=%u]", info.dwIndex);
//			return false;
//		}
//	}
//
//	IME_LOG("LOAD CONF SKILLLVUP para [Size=%d]", m_mapData.size());
//
//	return true;
//}
//
//STC_CONF_SKILL_LVUP* CConfSkillLvUp::Find(uint32 dwIndex)
//{
//	MapData::iterator it = m_mapData.find(dwIndex);
//	return it == m_mapData.end() ? NULL : &it->second;
//}

// gift packet 
CConfGiftPacket::MapData CConfGiftPacket::m_mapData;

bool CConfGiftPacket::Init(const std::string& strFilePath )
{
	m_mapData.clear();

	CConfigReader reader;
	if (!reader.init(strFilePath))
	{
		return false;
	}
	int nRow = reader.rowCount();
	if (nRow <= 1)
	{
		IME_ERROR("the row is small than 1 rowCount %u", nRow);
		return false;
	}
	for (int i = 1; i < nRow; ++i)
	{
		int j = 0;
		STC_CONF_GIFT_PACKET info;
		READER_INT(info.dwIndex);
		READER_INT(info.dwRandomEventIndex);
		READER_INT(info.dwCD);
		READER_INT(info.dwNumber);
		IS_READER_ERROR;

		if (m_mapData.insert(std::make_pair(info.dwIndex, info)).second == false)
		{
			IME_ERROR("ERROR CONF DUPLICATE [ChestId=%u]", info.dwIndex);
			return false;
		}
	}

	IME_LOG("LOAD CONF GIFT PACKET para [Size=%d]", m_mapData.size());
	return true;
}

STC_CONF_GIFT_PACKET* CConfGiftPacket::Find(uint32 dwIndex)
{
	MapData::iterator it = m_mapData.find(dwIndex);
	return it == m_mapData.end() ? NULL : &it->second;
}

//master level 
CConfMasterLevel::MapData CConfMasterLevel::m_mapData;
std::map<uint32, std::vector<uint32> > CConfMasterLevel::m_mapExpToLevel;

bool CConfMasterLevel::Init(const std::string& strFilePath )
{
	m_mapData.clear();
	m_mapExpToLevel.clear();

	CConfigReader reader;
	if (!reader.init(strFilePath))
	{
		return false;
	}
	int nRow = reader.rowCount();
	if (nRow <= 1)
	{
		IME_ERROR("the row is small than 1 rowCount %u", nRow);
		return false;
	}
	uint32_t ExpLimit = 0;
	uint32_t type = 0;
	uint32_t val = 0;
	int ii = 1;
	for (int i = 1; i < nRow; ++i)
	{
		int j = 0;
		STC_CONF_MASTER_LEVEL info;
		READER_INT(info.wLevel);
		READER_INT(info.dwType);
		READER_INT(info.dwNumber);
		if (type == 0 || type == info.dwType)
		{
			if (type == 0)
				type = info.dwType;
		}
		else // change another type group
		{
			if (m_mapData.find(info.dwType) != m_mapData.end())
			{
				IME_ERROR("ERROR CONF NO GROUP [type = %u] line %u", info.dwType, i);
				return false;
			}
			type = info.dwType;
			ExpLimit = 0;
			ii = 1;
		}
		if (ii != info.wLevel)
		{
			IME_ERROR("ERROR CONF UNORDER now number %u [INDEX = %u]", ii, info.wLevel);
			return false;
		}

		READER_INT(val);
		ExpLimit += val;
		info.dwExp = ExpLimit;
		m_mapExpToLevel[info.dwType].push_back(ExpLimit);
//		IME_LOG("type %u explimit %u", info.dwType, ExpLimit);

		info.dwAddExp = val;
		READER_INT(info.dwPro);
		READER_INT(info.dwPrizePro);
		IS_READER_ERROR;
//		IME_LOG("type %u, level %u, pro %u", info.dwType, info.wLevel, info.dwPrizePro);

		if (m_mapData[info.dwType].insert(std::make_pair(info.wLevel, info)).second == false)
		{
			IME_ERROR("ERROR CONF DUPLICATE [type=%u Level=%u]", info.dwType, info.wLevel);
			return false;
		}

		ii++;
	}

	IME_LOG("LOAD CONF STRENGTHEN [Size=%d]", m_mapData.size());

	return true;
}

STC_CONF_MASTER_LEVEL* CConfMasterLevel::Find(uint32 dwType, uint16 wLevel)
{
	std::map<uint32, STC_CONF_MASTER_LEVEL>::iterator it = m_mapData[dwType].find(wLevel);
	
	return it == m_mapData[dwType].end() ? NULL : &it->second;
}

uint16_t CConfMasterLevel::FindLevel(uint32_t dwType, uint32_t dwExp)
{
	std::vector<uint32_t>::iterator low;
	uint16_t level;
	std::vector<uint32_t> &vec = m_mapExpToLevel[dwType];

	low = lower_bound(vec.begin(), vec.end(), dwExp + 1);
	if (low == vec.end())
	{
		level = vec.size();
	}
	else
	{
		level = (int)(low - vec.begin()) + 1;
	}
	return level;
}

uint32_t CConfMasterLevel::GetMaxExp(uint32_t dwType)
{
	if (m_mapExpToLevel[dwType].size() <= 1)
		return 0;
	std::vector<uint32_t>::iterator it = m_mapExpToLevel[dwType].end();
	it--;
	it--;
	return *it;
}

STC_CONF_MASTER_LEVEL *CConfMasterLevel::GetRandPrize(uint32 type, uint32 level)
{
	std::vector<int> vecpro;
//	IME_LOG("type %u, level %u", type, level);
	for (int i = 1; i <= level; i++)
	{
//		IME_LOG("prize pro %u", m_mapData[type][i].dwPrizePro);
		if (m_mapData[type].find(i) != m_mapData[type].end())
		{
//			IME_LOG("find");
			vecpro.push_back(m_mapData[type][i].dwPrizePro);
		}
		else
		{
//			IME_LOG("not find");
			vecpro.push_back(0);
		}
//		IME_LOG("level %u, pro %u", i, vecpro[i - 1]);
	}
	int lv = CUtil::RandFactor(vecpro);
	if (m_mapData[type].find(lv + 1) != m_mapData[type].end())
	{
		return &m_mapData[type][lv + 1];
	}
	else
	{
		return NULL;
	}
}

bool CConfMasterLevel::Valid(const std::string& strFilePath )
{
	MapData m_mapData;
	std::map<uint32, std::vector<uint32> > m_mapExpToLevel;

	CConfigReader reader;
	if (!reader.init(strFilePath))
	{
		return false;
	}
	int nRow = reader.rowCount();
	if (nRow <= 1)
	{
		IME_ERROR("the row is small than 1 rowCount %u", nRow);
		return false;
	}
	uint32_t ExpLimit = 0;
	uint32_t type = 0;
	uint32_t val = 0;
	int ii = 1;
	for (int i = 1; i < nRow; ++i)
	{
		int j = 0;
		STC_CONF_MASTER_LEVEL info;
		READER_INT(info.wLevel);
		READER_INT(info.dwType);
		READER_INT(info.dwNumber);
		if (type == 0 || type == info.dwType)
		{
			if (type == 0)
				type = info.dwType;
		}
		else // change another type group
		{
			if (m_mapData.find(info.dwType) != m_mapData.end())
			{
				IME_ERROR("ERROR CONF NO GROUP [type = %u] line %u", info.dwType, i);
				return false;
			}
			type = info.dwType;
			ExpLimit = 0;
			ii = 1;
		}
		if (ii != info.wLevel)
		{
			IME_ERROR("ERROR CONF UNORDER now number %u [INDEX = %u]", ii, info.wLevel);
			return false;
		}

		READER_INT(val);
		ExpLimit += val;
		info.dwExp = ExpLimit;
		m_mapExpToLevel[info.dwType].push_back(ExpLimit);
//		IME_LOG("type %u explimit %u", info.dwType, ExpLimit);

		info.dwAddExp = val;
		READER_INT(info.dwPro);
		READER_INT(info.dwPrizePro);
		IS_READER_ERROR;
//		IME_LOG("type %u, level %u, pro %u", info.dwType, info.wLevel, info.dwPrizePro);

		if (m_mapData[info.dwType].insert(std::make_pair(info.wLevel, info)).second == false)
		{
			IME_ERROR("ERROR CONF DUPLICATE [type=%u Level=%u]", info.dwType, info.wLevel);
			return false;
		}

		ii++;
	}

	IME_LOG("LOAD CONF STRENGTHEN [Size=%d]", m_mapData.size());

	return true;
}


// Rob Robot
CConfRobRobot::MapData CConfRobRobot::m_mapData;
std::multimap<uint32, STC_CONF_ROB_ROBOT*> CConfRobRobot::m_mapFragment;
std::vector<STC_CONF_ROB_ROBOT *> CConfRobRobot::m_vec;
std::map<uint32, uint32> CConfRobRobot::robotLayMap;

bool CConfRobRobot::Init(const std::string& strFilePath )
{
	m_mapData.clear();
	m_mapFragment.clear();
	m_vec.clear();

	CConfigReader reader;
	if (!reader.init(strFilePath))
	{
		return false;
	}
	int nRow = reader.rowCount();
	if (nRow <= 1)
	{
		IME_ERROR("the row is small than 1 rowCount %u", nRow);
		return false;
	}
//	IME_ERROR("rob total size %u", nRow);
	uint32_t robotbase = 1001;
	for (int i = 1; i < nRow; ++i)
	{
		int j = 0;
		STC_CONF_ROB_ROBOT info;
		READER_INT(info.dwIndex);
//		IME_ERROR("rob index %u", info.dwIndex);
		READER_INT(info.wLevel);
		READER_STR(info.strName);

		READER_INT(info.dwMasterCardIndex);
		READER_INT(info.wMasterLevel);
		READER_INT(info.dwMasterSkillIndex);
		READER_INT(info.wMasterSkillLevel);
		READER_INT(info.dwMasterAttackGrow);
		READER_INT(info.dwMasterDefenceGrow);
		READER_INT(info.dwMasterHealthGrow);

		READER_INT(info.dwCardIndex1);
		READER_INT(info.wLevel1);
		READER_INT(info.dwSkillIndex1);
		READER_INT(info.wSkillLevel1);
		READER_INT(info.dwAttackGrow1);
		READER_INT(info.dwDefenceGrow1);
		READER_INT(info.dwHealthGrow1);

		READER_INT(info.dwCardIndex2);
		READER_INT(info.wLevel2);
		READER_INT(info.dwSkillIndex2);
		READER_INT(info.wSkillLevel2);
		READER_INT(info.dwAttackGrow2);
		READER_INT(info.dwDefenceGrow2);
		READER_INT(info.dwHealthGrow2);

		READER_INT(info.dwCardIndex3);
		READER_INT(info.wLevel3);
		READER_INT(info.dwSkillIndex3);
		READER_INT(info.wSkillLevel3);
		READER_INT(info.dwAttackGrow3);
		READER_INT(info.dwDefenceGrow3);
		READER_INT(info.dwHealthGrow3);

		READER_INT(info.dwCardIndex4);
		READER_INT(info.wLevel4);
		READER_INT(info.dwSkillIndex4);
		READER_INT(info.wSkillLevel4);
		READER_INT(info.dwAttackGrow4);
		READER_INT(info.dwDefenceGrow4);
		READER_INT(info.dwHealthGrow4);
		
		READER_INT(info.dwCardIndex5);
		READER_INT(info.wLevel5);
		READER_INT(info.dwSkillIndex5);
		READER_INT(info.wSkillLevel5);
		READER_INT(info.dwAttackGrow5);
		READER_INT(info.dwDefenceGrow5);
		READER_INT(info.dwHealthGrow5);

		READER_INT(info.dwCardIndex6);
		READER_INT(info.wLevel6);
		READER_INT(info.dwSkillIndex6);
		READER_INT(info.wSkillLevel6);
		READER_INT(info.dwAttackGrow6);
		READER_INT(info.dwDefenceGrow6);
		READER_INT(info.dwHealthGrow6);

		READER_INT(info.dwCD);
		READER_INT(info.dwFragment1);
		READER_INT(info.dwFragment2);
		READER_INT(info.dwFragment3);
		READER_INT(info.dwFragment4);
		READER_INT(info.dwFragment5);
		READER_INT(info.dwFragment6);
		READER_INT(info.dwPro);
		READER_INT(info.dwDevote);
		READER_INT(info.dwDevoteGrow);
		READER_INT(info.dwTowerLayer);
		IS_READER_ERROR;

		if ((robotbase << 8 | robotbase % 256) != info.dwIndex)
		{
			IME_ERROR("robot index %u is not valid should be %u", info.dwIndex, (robotbase << 8 | robotbase % 256));
			return false;
		}
		robotbase++;

		if (!IS_ROBORT(info.dwIndex))
		{
			IME_ERROR("robort index %u big than 10000", info.dwIndex);
			return false;
		}
		if (info.dwMasterCardIndex == 0)
		{
			IME_ERROR("master card index == 0", info.dwMasterCardIndex);
			return false;
		}

		if (m_mapData.insert(std::make_pair(info.dwIndex, info)).second == false)
		{
			IME_ERROR("ERROR CONF DUPLICATE [ChestId=%u]", info.dwIndex);
			return false;
		}
		//保存机器人闯塔信息
		if(info.dwTowerLayer != 0){
			robotLayMap.insert(std::make_pair(info.dwIndex,info.dwTowerLayer));
		}
		std::vector<uint32_t> vectmp;
		vectmp.push_back(0);
		
		uint32_t dwIndex = info.dwIndex;

		if (info.dwFragment1 != 0)
		{
			m_mapFragment.insert(std::pair<uint32_t, STC_CONF_ROB_ROBOT*>(info.dwFragment1, &m_mapData[dwIndex]));
//			IME_LOG("pieces %u, index %u", info.dwFragment1, dwIndex);
			vectmp.push_back(info.dwFragment1);
		}
		if (info.dwFragment2 != 0 && std::find(vectmp.begin(), vectmp.end(), info.dwFragment2) == vectmp.end())
		{
			m_mapFragment.insert(std::pair<uint32_t, STC_CONF_ROB_ROBOT*>(info.dwFragment2, &m_mapData[dwIndex]));
//			IME_LOG("pieces %u, index %u", info.dwFragment2, dwIndex);
			vectmp.push_back(info.dwFragment2);
		}
		if (info.dwFragment3 != 0 && std::find(vectmp.begin(), vectmp.end(), info.dwFragment3) == vectmp.end())
		{
			m_mapFragment.insert(std::pair<uint32_t, STC_CONF_ROB_ROBOT*>(info.dwFragment3, &m_mapData[dwIndex]));
//			IME_LOG("pieces %u, index %u", info.dwFragment3, dwIndex);
			vectmp.push_back(info.dwFragment3);
		}
		if (info.dwFragment4 != 0 && std::find(vectmp.begin(), vectmp.end(), info.dwFragment4) == vectmp.end())
		{
			m_mapFragment.insert(std::pair<uint32_t, STC_CONF_ROB_ROBOT*>(info.dwFragment4, &m_mapData[dwIndex]));
//			IME_LOG("pieces %u, index %u", info.dwFragment4, dwIndex);
			vectmp.push_back(info.dwFragment4);
		}
		if (info.dwFragment5 != 0 && std::find(vectmp.begin(), vectmp.end(), info.dwFragment5) == vectmp.end())
		{
			m_mapFragment.insert(std::pair<uint32_t, STC_CONF_ROB_ROBOT*>(info.dwFragment5, &m_mapData[dwIndex]));
//			IME_LOG("pieces %u, index %u", info.dwFragment5, dwIndex);
			vectmp.push_back(info.dwFragment5);
		}
		if (info.dwFragment6 != 0 && std::find(vectmp.begin(), vectmp.end(), info.dwFragment6) == vectmp.end())
		{
			m_mapFragment.insert(std::pair<uint32_t, STC_CONF_ROB_ROBOT*>(info.dwFragment6, &m_mapData[dwIndex]));
//			IME_LOG("pieces %u, index %u", info.dwFragment6, dwIndex);
			vectmp.push_back(info.dwFragment6);
		}
	}

	IME_LOG("LOAD CONF robot [Size=%d]", m_mapData.size());

	return true;
}

STC_CONF_ROB_ROBOT* CConfRobRobot::Find(uint32 dwIndex)
{
	MapData::iterator it = m_mapData.find(dwIndex);
	return it == m_mapData.end() ? NULL : &it->second;
}

std::vector<STC_CONF_ROB_ROBOT *>* CConfRobRobot::GetData(uint32 dwFragment)
{
	m_vec.clear();
	std::multimap<uint32, STC_CONF_ROB_ROBOT *>::iterator it;
	std::pair<std::multimap<uint32, STC_CONF_ROB_ROBOT *>::iterator, std::multimap<uint32, STC_CONF_ROB_ROBOT *>::iterator > ret;
	ret = m_mapFragment.equal_range(dwFragment);
	for (it = ret.first; it != ret.second; ++it)
	{
		m_vec.push_back(it->second);	
		IME_LOG("index %u", it->second->dwIndex);
	}	
	return &m_vec;
}

bool CConfRobRobot::Valid(const std::string& strFilePath )
{
    MapData m_mapData;
	std::multimap<uint32, STC_CONF_ROB_ROBOT*> m_mapFragment;
	std::vector<STC_CONF_ROB_ROBOT *> m_vec;

	CConfigReader reader;
	if (!reader.init(strFilePath))
	{
		return false;
	}
	int nRow = reader.rowCount();
	if (nRow <= 1)
	{
		IME_ERROR("the row is small than 1 rowCount %u", nRow);
		return false;
	}
	uint32_t robotbase = 1001;
	for (int i = 1; i < nRow; ++i)
	{
		int j = 0;
		STC_CONF_ROB_ROBOT info;
		READER_INT(info.dwIndex);
		READER_INT(info.wLevel);
		READER_STR(info.strName);

		READER_INT(info.dwMasterCardIndex);
		READER_INT(info.wMasterLevel);
		READER_INT(info.dwMasterSkillIndex);
		READER_INT(info.wMasterSkillLevel);
		READER_INT(info.dwMasterAttackGrow);
		READER_INT(info.dwMasterDefenceGrow);
		READER_INT(info.dwMasterHealthGrow);

		READER_INT(info.dwCardIndex1);
		READER_INT(info.wLevel1);
		READER_INT(info.dwSkillIndex1);
		READER_INT(info.wSkillLevel1);
		READER_INT(info.dwAttackGrow1);
		READER_INT(info.dwDefenceGrow1);
		READER_INT(info.dwHealthGrow1);

		READER_INT(info.dwCardIndex2);
		READER_INT(info.wLevel2);
		READER_INT(info.dwSkillIndex2);
		READER_INT(info.wSkillLevel2);
		READER_INT(info.dwAttackGrow2);
		READER_INT(info.dwDefenceGrow2);
		READER_INT(info.dwHealthGrow2);

		READER_INT(info.dwCardIndex3);
		READER_INT(info.wLevel3);
		READER_INT(info.dwSkillIndex3);
		READER_INT(info.wSkillLevel3);
		READER_INT(info.dwAttackGrow3);
		READER_INT(info.dwDefenceGrow3);
		READER_INT(info.dwHealthGrow3);

		READER_INT(info.dwCardIndex4);
		READER_INT(info.wLevel4);
		READER_INT(info.dwSkillIndex4);
		READER_INT(info.wSkillLevel4);
		READER_INT(info.dwAttackGrow4);
		READER_INT(info.dwDefenceGrow4);
		READER_INT(info.dwHealthGrow4);
		
		READER_INT(info.dwCardIndex5);
		READER_INT(info.wLevel5);
		READER_INT(info.dwSkillIndex5);
		READER_INT(info.wSkillLevel5);
		READER_INT(info.dwAttackGrow5);
		READER_INT(info.dwDefenceGrow5);
		READER_INT(info.dwHealthGrow5);

		READER_INT(info.dwCardIndex6);
		READER_INT(info.wLevel6);
		READER_INT(info.dwSkillIndex6);
		READER_INT(info.wSkillLevel6);
		READER_INT(info.dwAttackGrow6);
		READER_INT(info.dwDefenceGrow6);
		READER_INT(info.dwHealthGrow6);

		READER_INT(info.dwCD);
		READER_INT(info.dwFragment1);
		READER_INT(info.dwFragment2);
		READER_INT(info.dwFragment3);
		READER_INT(info.dwFragment4);
		READER_INT(info.dwFragment5);
		READER_INT(info.dwFragment6);
		READER_INT(info.dwPro);
		READER_INT(info.dwDevote);
		READER_INT(info.dwDevoteGrow);
		IS_READER_ERROR;

		if ((robotbase << 8 | robotbase % 256) != info.dwIndex)
		{
			IME_ERROR("robot index %u is not valid should be %u", info.dwIndex, (robotbase << 8 | robotbase % 256));
			return false;
		}
		robotbase++;

		if (!IS_ROBORT(info.dwIndex))
		{
			IME_ERROR("robort index %u big than 10000", info.dwIndex);
			return false;
		}
		if (info.dwMasterCardIndex == 0)
		{
			IME_ERROR("master card index == 0", info.dwMasterCardIndex);
			return false;
		}

		if (m_mapData.insert(std::make_pair(info.dwIndex, info)).second == false)
		{
			IME_ERROR("ERROR CONF DUPLICATE [ChestId=%u]", info.dwIndex);
			return false;
		}
		std::vector<uint32_t> vectmp;
		vectmp.push_back(0);
		
		uint32_t dwIndex = info.dwIndex;

		if (info.dwFragment1 != 0)
		{
			m_mapFragment.insert(std::pair<uint32_t, STC_CONF_ROB_ROBOT*>(info.dwFragment1, &m_mapData[dwIndex]));
//			IME_LOG("pieces %u, index %u", info.dwFragment1, dwIndex);
			vectmp.push_back(info.dwFragment1);
		}
		if (info.dwFragment2 != 0 && std::find(vectmp.begin(), vectmp.end(), info.dwFragment2) == vectmp.end())
		{
			m_mapFragment.insert(std::pair<uint32_t, STC_CONF_ROB_ROBOT*>(info.dwFragment2, &m_mapData[dwIndex]));
//			IME_LOG("pieces %u, index %u", info.dwFragment2, dwIndex);
			vectmp.push_back(info.dwFragment2);
		}
		if (info.dwFragment3 != 0 && std::find(vectmp.begin(), vectmp.end(), info.dwFragment3) == vectmp.end())
		{
			m_mapFragment.insert(std::pair<uint32_t, STC_CONF_ROB_ROBOT*>(info.dwFragment3, &m_mapData[dwIndex]));
//			IME_LOG("pieces %u, index %u", info.dwFragment3, dwIndex);
			vectmp.push_back(info.dwFragment3);
		}
		if (info.dwFragment4 != 0 && std::find(vectmp.begin(), vectmp.end(), info.dwFragment4) == vectmp.end())
		{
			m_mapFragment.insert(std::pair<uint32_t, STC_CONF_ROB_ROBOT*>(info.dwFragment4, &m_mapData[dwIndex]));
//			IME_LOG("pieces %u, index %u", info.dwFragment4, dwIndex);
			vectmp.push_back(info.dwFragment4);
		}
		if (info.dwFragment5 != 0 && std::find(vectmp.begin(), vectmp.end(), info.dwFragment5) == vectmp.end())
		{
			m_mapFragment.insert(std::pair<uint32_t, STC_CONF_ROB_ROBOT*>(info.dwFragment5, &m_mapData[dwIndex]));
//			IME_LOG("pieces %u, index %u", info.dwFragment5, dwIndex);
			vectmp.push_back(info.dwFragment5);
		}
		if (info.dwFragment6 != 0 && std::find(vectmp.begin(), vectmp.end(), info.dwFragment6) == vectmp.end())
		{
			m_mapFragment.insert(std::pair<uint32_t, STC_CONF_ROB_ROBOT*>(info.dwFragment6, &m_mapData[dwIndex]));
//			IME_LOG("pieces %u, index %u", info.dwFragment6, dwIndex);
			vectmp.push_back(info.dwFragment6);
		}
	}

	IME_LOG("LOAD CONF robot para [Size=%d]", m_mapData.size());

	return true;
}

//vip 
CConfVip::MapData CConfVip::m_mapData;

bool CConfVip::Init(const std::string& strFilePath )
{
	m_mapData.clear();
	
	if (!ReadConfig(strFilePath, m_mapData))
	{
		return false;
	}

	IME_LOG("LOAD CONF VIP para [Size=%d]", m_mapData.size());
	return true;
}

STC_CONF_VIP* CConfVip::Find(uint32 dwIndex)
{
	MapData::iterator it = m_mapData.find(dwIndex);
	return it == m_mapData.end() ? NULL : &it->second;
}

bool CConfVip::Valid(const std::string& strFilePath )
{
    MapData m_mapData;
	
	if (!ReadConfig(strFilePath, m_mapData))
	{
		return false;
	}	

	IME_LOG("LOAD CONF VIP para [Size=%d]", m_mapData.size());
	return true;
}

uint32 CConfVip::MaxVipExp(uint16_t lv)
{
	MapData::iterator it;
//	it = m_mapData.find();
//	if (it != m_mapData.begin())
//	{
//		it--;
//		return it->second.dwIndex;
//	}
//	else
//	{
//		return 0;
//	}
	uint32 vipexp = 0;
	uint32 lastexp = 0;
	for(it = m_mapData.begin(); it != m_mapData.end(); it++)
	{
		if (it->first >= lv)
		{
			vipexp += it->second.dwVipExp;
			lastexp = it->second.dwVipExp;
		}
	}
	return vipexp - lastexp;
}

uint32 CConfVip::MaxVipLv()
{
	MapData::iterator it;
	it = m_mapData.end();
	if (it != m_mapData.begin())
	{
		it--;
		return it->second.dwIndex;
	}
	else
	{
		return 0;
	}
}

bool CConfVip::ReadConfig(const std::string& strFilePath, MapData &map)
{
	CConfigReader reader;
	if (!reader.init(strFilePath))
	{
		return false;
	}
	int nRow = reader.rowCount();
	if (nRow <= 1)
	{
		IME_ERROR("the row is small than 1 rowCount %u", nRow);
		return false;
	}
	for (int i = 1; i < nRow; ++i)
	{
		int j = 0;
		STC_CONF_VIP info;
		READER_INT(info.dwIndex);
		READER_INT(info.dwCritExtraLevel);
		READER_INT(info.dwDelDMissionNumber);
		READER_INT(info.dwEnergyStrengthNumber);
		READER_INT(info.dwVipGift);
		READER_INT(info.dwAttackBuff);
		READER_INT(info.dwExtraSign);
		READER_INT(info.dwArenaChallengeNumber);
		READER_INT(info.dwHighDelDMissionNumber);
		READER_INT(info.dwVipExp);
		READER_INT(info.dwLuckMax);
		READER_INT(info.dwLuckEveryGet);
		READER_INT(info.dwVipRecover);
		READER_INT(info.dwVipFriendLimit);
		READER_INT(info.dwVipBossAttack);
		READER_INT(info.dwDollarExtraRate);
		READER_INT(info.dwJoinMax);
		READER_INT(info.dwStaminaNumber);
		
		IS_READER_ERROR;

		if (map.insert(std::make_pair(info.dwIndex, info)).second == false)
		{
			IME_ERROR("ERROR CONF DUPLICATE [ChestId=%u]", info.dwIndex);
			return false;
		}
	}
	return true;
}


//strength Crit pro 
CConfStrengthCritPro::MapData CConfStrengthCritPro::m_mapData;

bool CConfStrengthCritPro::Init(const std::string& strFilePath )
{
	m_mapData.clear();

	CConfigReader reader;
	if (!reader.init(strFilePath))
	{
		return false;
	}
	int nRow = reader.rowCount();
	if (nRow <= 1)
	{
		IME_ERROR("the row is small than 1 rowCount %u", nRow);
		return false;
	}
	for (int i = 1; i < nRow; ++i)
	{
		int j = 0;
		STC_CONF_STRENGTH_CRIT_PRO info;
		READER_INT(info.dwIndex);
		READER_INT(info.dwVip1);
		READER_INT(info.dwVip2);
		READER_INT(info.dwVip3);
		READER_INT(info.dwVip4);
		READER_INT(info.dwVip5);
		READER_INT(info.dwVip6);
		IS_READER_ERROR;

		if (m_mapData.insert(std::make_pair(info.dwIndex, info)).second == false)
		{
			IME_ERROR("ERROR CONF DUPLICATE [ChestId=%u]", info.dwIndex);
			return false;
		}
	}

	IME_LOG("LOAD CONF STRENGTH CRIT PRO para [Size=%d]", m_mapData.size());
	return true;
}

STC_CONF_STRENGTH_CRIT_PRO* CConfStrengthCritPro::Find(uint32 dwIndex)
{
	MapData::iterator it = m_mapData.find(dwIndex);
	return it == m_mapData.end() ? NULL : &it->second;
}

bool CConfStrengthCritPro::Valid(const std::string& strFilePath )
{
    MapData m_mapData;

	CConfigReader reader;
	if (!reader.init(strFilePath))
	{
		return false;
	}
	int nRow = reader.rowCount();
	if (nRow <= 1)
	{
		IME_ERROR("the row is small than 1 rowCount %u", nRow);
		return false;
	}
	for (int i = 1; i < nRow; ++i)
	{
		int j = 0;
		STC_CONF_STRENGTH_CRIT_PRO info;
		READER_INT(info.dwIndex);
		READER_INT(info.dwVip1);
		READER_INT(info.dwVip2);
		READER_INT(info.dwVip3);
		READER_INT(info.dwVip4);
		READER_INT(info.dwVip5);
		READER_INT(info.dwVip6);
		IS_READER_ERROR;

		if (m_mapData.insert(std::make_pair(info.dwIndex, info)).second == false)
		{
			IME_ERROR("ERROR CONF DUPLICATE [ChestId=%u]", info.dwIndex);
			return false;
		}
	}

	IME_LOG("LOAD CONF STRENGTH CRIT PRO para [Size=%d]", m_mapData.size());
	return true;
}


// friend boss
CConfFriendBoss::MapData CConfFriendBoss::m_mapData;

bool CConfFriendBoss::Init(const std::string& strFilePath )
{
	m_mapData.clear();

	if (!ReadConfig(strFilePath, m_mapData))
	{
		return false;
	}
	IME_LOG("LOAD CONF duration mission para [Size=%d]", m_mapData.size());
	return true;
}

STC_CONF_FRIEND_BOSS* CConfFriendBoss::Find(uint32 dwIndex)
{
	MapData::iterator it = m_mapData.find(dwIndex);
	return it == m_mapData.end() ? NULL : &it->second;
}

bool CConfFriendBoss::Valid(const std::string& strFilePath )
{
    MapData m_mapData;
	if (!ReadConfig(strFilePath, m_mapData))
	{
		return false;
	}

	IME_LOG("LOAD CONF duration mission para [Size=%d]", m_mapData.size());
	return true;
}

bool CConfFriendBoss::ReadConfig(const std::string& strFilePath, CConfFriendBoss::MapData &map)
{
	CConfigReader reader;
	if (!reader.init(strFilePath))
	{
		return false;
	}

	int nRow = reader.rowCount();
	if (nRow <= 1)
	{
		IME_ERROR("the row is small than 1 rowCount %u", nRow);
		return false;
	}
	for (int i = 1; i < nRow; ++i)
	{
		int j = 0;
		STC_CONF_FRIEND_BOSS info;
		READER_INT(info.dwIndex);
		READER_STR(info.strName);
		READER_INT(info.byType);

		READER_INT(info.dwMasterCardIndex);
		READER_INT(info.dwMasterSkillIndex);
		READER_INT(info.wMasterSkillLevel);
		READER_INT(info.dwMasterAttackBase);
		READER_INT(info.dwMasterDefenceBase);
		READER_INT(info.dwMasterHealthBase);
		READER_INT(info.dwMasterAttackGrow);
		READER_INT(info.dwMasterDefenceGrow);
		READER_INT(info.dwMasterHealthGrow);

		READER_INT(info.dwDropPro);
		READER_INT(info.dwDropItemId);
		READER_INT(info.dwGoldCardPro);
		READER_INT(info.dwAwakePro);
		READER_INT(info.dwAwakeBossId);
		READER_INT(info.byBossType);
		READER_INT(info.dwExp);
		READER_INT(info.dwCoin);
		READER_INT(info.byRare);

		READER_INT(info.byBreakCount);
		
		READER_INT(info.dwDropCardPro);
		READER_INT(info.dwCardId);
		READER_INT(info.dwDropItemColPro);
		READER_INT(info.dwItemCol);
		READER_INT(info.dwDropCardColPro);
		READER_INT(info.dwCardCol);
		READER_INT(info.dwBossTime);
		READER_INT(info.dwBossLvUpPara);

		READER_INT(info.dwSword);
		READER_INT(info.dwHand);
		READER_INT(info.dwInternalForce);
		READER_INT(info.dwDark);
		READER_INT(info.dwStick);
		IS_READER_ERROR;
		
		if (info.dwDropItemId != 0 && CConfGoods::Find(info.dwDropItemId) == NULL)
		{
			IME_ERROR("friend boss drop item index %u is null", info.dwDropItemId);
			return false;
		}
		if (info.dwCardId != 0 && CConfCardBase::Find(info.dwCardId) == NULL)
		{
			IME_ERROR("friend boss drop card index %u is null", info.dwCardId);
			return false;
		}	
		if (info.dwItemCol != 0 && CConfGoodsCollection::Find(info.dwItemCol) == NULL)
		{
			IME_ERROR("friend boss item collection drop index %u is null", info.dwItemCol);
			return false;
		}	

		if (CConfCardCollection::Find(info.dwCardCol) == NULL)
		{
			IME_ERROR("friend boss drop card col index %u is null", info.dwCardCol);
			return false;
		}
		if (map.insert(std::make_pair(info.dwIndex, info)).second == false)
		{
			IME_ERROR("ERROR CONF DUPLICATE [ChestId=%u]", info.dwIndex);
			return false;
		}
	}
	return true;
}

//gacha pro 
CConfGacha::MapData CConfGacha::m_mapData;

bool CConfGacha::ReadConfig(const std::string &strFilePath, MapData &map)
{
	uint8_t NormalOk = 0;
	uint8_t ItemOk = 0;
	uint8_t DOllarOk = 0;
	uint8_t AwakeBossOk = 0;
	uint8_t FirstGacha = 0;
	uint8_t FinalGacha = 0;
	uint8_t VipGacha[VIP_MAX] = {0};
	uint8_t FirstGachaNumber = 0;

	CConfigReader reader;
	if (!reader.init(strFilePath))
	{
		return false;
	}
	int nRow = reader.rowCount();
	if (nRow <= 1)
	{
		IME_ERROR("the row is small than 1 rowCount %u", nRow);
		return false;
	}
	for (int i = 1; i < nRow; ++i)
	{
		int j = 0;
		STC_CONF_GACHA info;
		READER_INT(info.dwIndex);
		READER_INT(info.dwNormalPro);
		READER_INT(info.dwItemPro);
		READER_INT(info.dwDollarPro);
		READER_INT(info.dwAwakeBossPro);
		READER_INT(info.dwDelLuck);
		READER_INT(info.dwFirstGachaPro);
		READER_INT(info.dwFinalGachaPro);

		READER_INT(info.dwVipFirstPro[0]);
		READER_INT(info.dwVipNormalPro[0]);
		READER_INT(info.dwVipFirstPro[1]);
		READER_INT(info.dwVipNormalPro[1]);
		READER_INT(info.dwVipFirstPro[2]);
		READER_INT(info.dwVipNormalPro[2]);
		READER_INT(info.dwVipFirstPro[3]);
		READER_INT(info.dwVipNormalPro[3]);
		READER_INT(info.dwVipFirstPro[4]);
		READER_INT(info.dwVipNormalPro[4]);
		READER_INT(info.dwVipFirstPro[5]);
		READER_INT(info.dwVipNormalPro[5]);
		READER_INT(info.dwVipFirstPro[6]);
		READER_INT(info.dwVipNormalPro[6]);
		READER_INT(info.dwVipFirstPro[7]);
		READER_INT(info.dwVipNormalPro[7]);
		READER_INT(info.dwVipFirstPro[8]);
		READER_INT(info.dwVipNormalPro[8]);
		READER_INT(info.dwVipFirstPro[9]);
		READER_INT(info.dwVipNormalPro[9]);
		READER_INT(info.dwVipFirstPro[10]);
		READER_INT(info.dwVipNormalPro[10]);

		READER_INT(info.dwChargePro);
		
		IS_READER_ERROR;
		if (info.dwNormalPro != 0 && info.dwDelLuck == 0)
			NormalOk = 1;
		if (info.dwItemPro != 0 && info.dwDelLuck == 0)
			ItemOk = 1;
		if (info.dwDollarPro != 0 && info.dwDelLuck == 0)
			DOllarOk = 1;
		if (info.dwAwakeBossPro != 0 && info.dwDelLuck == 0)
			AwakeBossOk = 1;
		if (info.dwFirstGachaPro != 0 && info.dwDelLuck == 0)
			FirstGacha = 1;
		if (info.dwFirstGachaPro != 0 && info.dwDelLuck <= 300) 
			FirstGachaNumber++;
//		if (info.dwFinalGachaPro != 0 && info.dwDelLuck == 0)
//			FinalGacha = 1;
		if (info.dwFinalGachaPro != 0)
			FinalGacha = 1;

		for (int ii = 0; ii < VIP_MAX; ii++)
		{
			if (info.dwVipNormalPro[ii] != 0 && info.dwDelLuck == 0)
				VipGacha[ii] = 1;
		}

		if (map.insert(std::make_pair(info.dwIndex, info)).second == false)
		{
			IME_ERROR("ERROR CONF DUPLICATE [ChestId=%u]", info.dwIndex);
			return false;
		}
		if (CConfCardBase::Find(info.dwIndex) == NULL)
		{
			IME_ERROR("card can not find index %u", info.dwIndex);
			return false;
		}
	}
	if (!(NormalOk && ItemOk && DOllarOk && AwakeBossOk && FirstGacha && FinalGacha))
	{
		IME_ERROR("gacha normal %u, item %u, dollar %u, awakeboss %u firstgacha %u and finalgacha %u pro have any one pro != 0 luck == 0 record. it must not", NormalOk, ItemOk, DOllarOk, AwakeBossOk, FirstGacha, FinalGacha);
		return false;
	}

//	if (FirstGachaNumber < 5)
//	{
//		IME_ERROR("first gacha number small than 5");
//		return false;
//	}

	for (int ii = 0; ii < VIP_MAX; ii++)
	{
		if (!VipGacha[ii])
		{
			IME_ERROR("vip %u normal gacha have any one pro != 0 luck == 0 record. it must not", ii + 1);
			return false;
		}
	}

	IME_LOG("LOAD CONF GIFT PACKET para [Size=%d]", map.size());
	return true;

}

bool CConfGacha::Init(const std::string& strFilePath )
{
	m_mapData.clear();
	if (!ReadConfig(strFilePath, m_mapData))
	{
		IME_ERROR("read config fail");
		return false;
	}
	return true;
}

STC_CONF_GACHA* CConfGacha::Find(uint32 dwIndex)
{
	MapData::iterator it = m_mapData.find(dwIndex);
	return it == m_mapData.end() ? NULL : &it->second;
}

STC_CONF_GACHA* CConfGacha::GetByNormal(uint32_t luck)
{
	MapData::iterator it;
	std::vector<int> vecPro;
	std::vector<STC_CONF_GACHA *> vecRes;
	for (it = m_mapData.begin(); it != m_mapData.end(); it++)
	{
		if (it->second.dwDelLuck <= luck)
		{
			vecPro.push_back(it->second.dwNormalPro);
			vecRes.push_back(&(it->second));
		}
	}
	int ret = CUtil::RandFactor(vecPro);
	if (ret >= vecRes.size())
		return NULL;
	return vecRes[ret];
}

STC_CONF_GACHA* CConfGacha::GetByItem(uint32_t luck)
{
	MapData::iterator it;
	std::vector<int> vecPro;
	std::vector<STC_CONF_GACHA *> vecRes;
	for (it = m_mapData.begin(); it != m_mapData.end(); it++)
	{
		if (it->second.dwDelLuck <= luck)
		{
			vecPro.push_back(it->second.dwItemPro);
			vecRes.push_back(&(it->second));
		}
	}
	int ret = CUtil::RandFactor(vecPro);
	if (ret >= vecRes.size())
		return NULL;
	return vecRes[ret];
}

STC_CONF_GACHA* CConfGacha::GetByDollar(uint32_t luck)
{
	MapData::iterator it;
	std::vector<int> vecPro;
	std::vector<STC_CONF_GACHA *> vecRes;
	for (it = m_mapData.begin(); it != m_mapData.end(); it++)
	{
		if (it->second.dwDelLuck <= luck)
		{
			vecPro.push_back(it->second.dwDollarPro);
			vecRes.push_back(&(it->second));
		}
	}
	int ret = CUtil::RandFactor(vecPro);
	if (ret >= vecRes.size())
		return NULL;
	return vecRes[ret];
}

STC_CONF_GACHA* CConfGacha::GetByAwakeBoss(uint32_t luck)
{
	MapData::iterator it;
	std::vector<int> vecPro;
	std::vector<STC_CONF_GACHA *> vecRes;
	for (it = m_mapData.begin(); it != m_mapData.end(); it++)
	{
		if (it->second.dwDelLuck <= luck)
		{
			vecPro.push_back(it->second.dwAwakeBossPro);
			vecRes.push_back(&(it->second));
		}
	}
	int ret = CUtil::RandFactor(vecPro);
	if (ret >= vecRes.size())
		return NULL;
	return vecRes[ret];
}

STC_CONF_GACHA* CConfGacha::GetByFirstGacha(uint32_t luck, std::vector<uint32_t> extraindex)
{
	MapData::iterator it;
	std::vector<int> vecPro;
	std::vector<STC_CONF_GACHA *> vecRes;
	for (it = m_mapData.begin(); it != m_mapData.end(); it++)
	{
		if (it->second.dwDelLuck <= luck && std::find(extraindex.begin(), extraindex.end(), it->second.dwIndex) == extraindex.end())
		{
			vecPro.push_back(it->second.dwFirstGachaPro);
			vecRes.push_back(&(it->second));
		}
	}
	int ret = CUtil::RandFactor(vecPro);
	if (ret >= vecRes.size())
		return NULL;
	return vecRes[ret];
}

STC_CONF_GACHA* CConfGacha::GetByFinalGacha(uint32_t luck)
{
	MapData::iterator it;
	std::vector<int> vecPro;
	std::vector<STC_CONF_GACHA *> vecRes;
	for (it = m_mapData.begin(); it != m_mapData.end(); it++)
	{
//		if (it->second.dwDelLuck <= luck)
//		{
			vecPro.push_back(it->second.dwFinalGachaPro);
			vecRes.push_back(&(it->second));
//		}
	}
	int ret = CUtil::RandFactor(vecPro);
	if (ret >= vecRes.size())
		return NULL;
	return vecRes[ret];
}

STC_CONF_GACHA* CConfGacha::GetByChargeGacha()
{
	MapData::iterator it;
	std::vector<int> vecPro;
	std::vector<STC_CONF_GACHA *> vecRes;
	for (it = m_mapData.begin(); it != m_mapData.end(); it++)
	{
//		IME_ERROR("charge gacha index %u, pro %u", it->second.dwIndex, it->second.dwChargePro);
		vecPro.push_back(it->second.dwChargePro);
		vecRes.push_back(&(it->second));
	}
	int ret = CUtil::RandFactor(vecPro);
	if (ret >= vecRes.size())
		return NULL;
	return vecRes[ret];
}

STC_CONF_GACHA* CConfGacha::GetByVipFirstGacha(uint16 viplevel)
{
	if (viplevel > VIP_MAX || viplevel <= 0)
	{
		IME_ERROR("para vip %u pass vipmax %u or small than 0", viplevel, VIP_MAX);
		return NULL;
	}

	MapData::iterator it;
	std::vector<int> vecPro;
	std::vector<STC_CONF_GACHA *> vecRes;
	for (it = m_mapData.begin(); it != m_mapData.end(); it++)
	{
//		IME_ERROR("vip first gacha index %u, pro %u", it->second.dwIndex, it->second.dwVipFirstPro[viplevel - 1]);
		vecPro.push_back(it->second.dwVipFirstPro[viplevel - 1]);
		vecRes.push_back(&(it->second));
	}
	int ret = CUtil::RandFactor(vecPro);
	if (ret >= vecRes.size())
		return NULL;
	return vecRes[ret];
}

STC_CONF_GACHA* CConfGacha::GetByVipNormalGacha(uint16 viplevel, uint32 luck)
{
	if (viplevel > VIP_MAX || viplevel <= 0)
	{
		IME_ERROR("para vip %u pass vipmax %u or small than 0", viplevel, VIP_MAX);
		return NULL;
	}

	MapData::iterator it;
	std::vector<int> vecPro;
	std::vector<STC_CONF_GACHA *> vecRes;
	for (it = m_mapData.begin(); it != m_mapData.end(); it++)
	{
		if (it->second.dwDelLuck <= luck)
		{
//			IME_ERROR("vip normal gacha index %u, pro %u", it->second.dwIndex, it->second.dwVipNormalPro[viplevel - 1]);
			vecPro.push_back(it->second.dwVipNormalPro[viplevel - 1]);
			vecRes.push_back(&(it->second));
		}
	}
	int ret = CUtil::RandFactor(vecPro);
	if (ret >= vecRes.size())
		return NULL;
	return vecRes[ret];
}

bool CConfGacha::Valid(const std::string& strFilePath )
{
    MapData m_mapData;

	if (!ReadConfig(strFilePath, m_mapData))
	{
		IME_ERROR("read config fail");
		return false;
	}
	return true;
}

//timer gift
CConfTimerGift::MapData CConfTimerGift::m_mapData;

bool CConfTimerGift::Init(const std::string& strFilePath )
{
	m_mapData.clear();

	CConfigReader reader;
	if (!reader.init(strFilePath))
	{
		return false;
	}
	int nRow = reader.rowCount();
	if (nRow <= 1)
	{
		IME_ERROR("the row is small than 1 rowCount %u", nRow);
		return false;
	}
	for (int i = 1; i < nRow; ++i)
	{
		int j = 0;
		STC_CONF_TIMER_GIFT info;
		READER_INT(info.dwIndex);
		READER_INT(info.dwCD);
		READER_INT(info.byType);
		READER_INT(info.dwPara1);
		READER_INT(info.dwPara2);
		IS_READER_ERROR;
//		IME_LOG("index %u, CD %u", info.dwIndex, info.dwCD);

		if (m_mapData.insert(std::make_pair(info.dwIndex, info)).second == false)
		{
			IME_ERROR("ERROR CONF DUPLICATE [ChestId=%u]", info.dwIndex);
			return false;
		}
	}

	IME_LOG("LOAD CONF GIFT PACKET para [Size=%d]", m_mapData.size());
	return true;
}

STC_CONF_TIMER_GIFT* CConfTimerGift::Find(uint32 dwIndex)
{
	MapData::iterator it = m_mapData.find(dwIndex);
	return it == m_mapData.end() ? NULL : &it->second;
}

bool CConfTimerGift::Valid(const std::string& strFilePath )
{
    MapData m_mapData;

	CConfigReader reader;
	if (!reader.init(strFilePath))
	{
		return false;
	}
	int nRow = reader.rowCount();
	if (nRow <= 1)
	{
		IME_ERROR("the row is small than 1 rowCount %u", nRow);
		return false;
	}
	for (int i = 1; i < nRow; ++i)
	{
		int j = 0;
		STC_CONF_TIMER_GIFT info;
		READER_INT(info.dwIndex);
		READER_INT(info.dwCD);
		READER_INT(info.byType);
		READER_INT(info.dwPara1);
		READER_INT(info.dwPara2);
		IS_READER_ERROR;
//		IME_LOG("index %u, CD %u", info.dwIndex, info.dwCD);

		if (m_mapData.insert(std::make_pair(info.dwIndex, info)).second == false)
		{
			IME_ERROR("ERROR CONF DUPLICATE [ChestId=%u]", info.dwIndex);
			return false;
		}
	}

	IME_LOG("LOAD CONF GIFT PACKET para [Size=%d]", m_mapData.size());
	return true;
}


//login active
CConfLoginSign::MapData CConfLoginSign::m_mapData;

bool CConfLoginSign::Init(const std::string& strFilePath )
{
	m_mapData.clear();

	CConfigReader reader;
	if (!reader.init(strFilePath))
	{
		return false;
	}
	int nRow = reader.rowCount();
	if (nRow <= 1)
	{
		IME_ERROR("the row is small than 1 rowCount %u", nRow);
		return false;
	}
	for (int i = 1; i < nRow; ++i)
	{
		int j = 0;
		STC_CONF_LOGIN_SIGN info;
		READER_INT(info.dwMonth);
		READER_INT(info.dwCount);
		READER_INT(info.byType);
		READER_INT(info.dwPara1);
		READER_INT(info.dwPara2);
		info.dwIndex = info.dwMonth * 100 + info.dwCount;
		IS_READER_ERROR;

		if (m_mapData.insert(std::make_pair(info.dwIndex, info)).second == false)
		{
			IME_ERROR("ERROR CONF DUPLICATE [ChestId=%u]", info.dwIndex);
			return false;
		}
	}

	IME_LOG("LOAD CONF login sign para [Size=%d]", m_mapData.size());
	return true;
}

STC_CONF_LOGIN_SIGN* CConfLoginSign::Find(uint32 dwIndex)
{
	MapData::iterator it = m_mapData.find(dwIndex);
	return it == m_mapData.end() ? NULL : &it->second;
}

bool CConfLoginSign::Valid(const std::string& strFilePath )
{
    MapData m_mapData;

	CConfigReader reader;
	if (!reader.init(strFilePath))
	{
		return false;
	}
	int nRow = reader.rowCount();
	if (nRow <= 1)
	{
		IME_ERROR("the row is small than 1 rowCount %u", nRow);
		return false;
	}
	for (int i = 1; i < nRow; ++i)
	{
		int j = 0;
		STC_CONF_LOGIN_SIGN info;
		READER_INT(info.dwMonth);
		READER_INT(info.dwCount);
		READER_INT(info.byType);
		READER_INT(info.dwPara1);
		READER_INT(info.dwPara2);
		info.dwIndex = info.dwMonth * 100 + info.dwCount;
		IS_READER_ERROR;

		if (m_mapData.insert(std::make_pair(info.dwIndex, info)).second == false)
		{
			IME_ERROR("ERROR CONF DUPLICATE [ChestId=%u]", info.dwIndex);
			return false;
		}
	}

	IME_LOG("LOAD CONF login sign para [Size=%d]", m_mapData.size());
	return true;
}


//store
CConfStore::MapData CConfStore::m_mapData;

bool CConfStore::ReadConfig(MapData &map)
{
//	CConfigReader reader;
//	if (!reader.init(strFilePath))
//	{
//		return false;
//	}
//	int nRow = reader.rowCount();
//	if (nRow <= 1)
//	{
//		IME_ERROR("the row is small than 1 rowCount %u", nRow);
//		return false;
//	}
//	for (int i = 1; i < nRow; ++i)
//	{
//		int j = 0;
//		STC_CONF_STORE info;
//		READER_INT(info.dwStoreIndex);
//		READER_INT(info.byStoreChannel);
//		READER_INT(info.dwIndex);
//		READER_INT(info.byType);
//		READER_INT(info.byObjType);
//		READER_INT(info.dwObjNumber);
//		READER_INT(info.byIsBuy);
//		READER_INT(info.dwViewOrder);
//		READER_STR(info.strContent);
//		READER_INT(info.dwLimitNumber);
//		READER_INT(info.dwRoleLimitNumber);
//		READER_INT(info.dwIndexNumber);
//		READER_INT(info.dwSendPrize);
//		READER_INT(info.dwViewPrize);
//		READER_INT(info.dwMaxBuy);
//		READER_INT(info.dwKind);
//		IS_READER_ERROR;
//		if (info.byType == E_OBJ_SHOP)
//		{
//			if (CConfGoods::Find(info.dwIndex) == NULL)
//			{
//				IME_ERROR("store index %u can not find item index %u", info.dwStoreIndex, info.dwIndex);
//				return false;
//			}
//		}
//
//		if (map.insert(std::make_pair(info.dwStoreIndex, info)).second == false)
//		{
//			IME_ERROR("ERROR CONF DUPLICATE [ChestId=%u]", info.dwIndex);
//			return false;
//		}
//	}
//
//	IME_LOG("LOAD CONF SHOP PRIZE [Size=%d]", map.size());
//	return true;
	
	std::vector<uint32_t> vectmp = sWorld->GetServerId();
	std::vector<STC_CONF_STORE> vec;
	if (CDBCtrl::GetStoreData(vectmp[0], vec))
	{
		for (int i = 0; i < vec.size(); ++i)
		{
			STC_CONF_STORE info;
			info = vec[i];
			if (info.byGoodsType == E_OBJ_SHOP)
			{
				if (CConfGoods::Find(info.dwIndex) == NULL)
				{
					IME_ERROR("store index %u can not find item index %u", info.dwStoreIndex, info.dwIndex);
					return false;
				}
			}
			else if (info.byGoodsType == E_OBJ_CARD)
			{
				if (CConfCardBase::Find(info.dwIndex) == NULL)
				{
					IME_ERROR("store index %u can not find card index %u", info.dwStoreIndex, info.dwIndex);
					return false;
				}
			}
			else if (info.byGoodsType == 0)
			{
			}
			else
			{
				IME_ERROR("this type cannot be support %u", info.byGoodsType);
				return false;
			}

			if (map.insert(std::make_pair(info.dwStoreIndex, info)).second == false)
			{
				IME_ERROR("ERROR CONF DUPLICATE [ChestId=%u]", info.dwIndex);
				return false;
			}
		}

		IME_LOG("LOAD CONF SHOP PRIZE [Size=%d]", map.size());
		return true;
	}
	return true;
}

bool CConfStore::Init()
{
	m_mapData.clear();
	if (!ReadConfig(m_mapData))
	{
		IME_ERROR("init fail read config");
		return false;
	}
	return true;
}

STC_CONF_STORE* CConfStore::Find(uint32 dwIndex)
{
	MapData::iterator it = m_mapData.find(dwIndex);
	return it == m_mapData.end() ? NULL : &it->second;
}

STC_CONF_STORE* CConfStore::FindByRMB(uint32 rmb, uint32 kind)
{
//	IME_ERROR("the dollar is %u", dollar);
	MapData::iterator it = m_mapData.begin();
	for ( ; it != m_mapData.end(); it++)
	{
//		IME_ERROR("store index %u buy %u store channel %u objnumber %u", it->first, it->second.byIsBuy, it->second.byStoreChannel, it->second.dwObjNumber);
		if (it->second.byIsBuy && it->second.byStoreChannel == STORE_CHARGE && it->second.dwObjNumber == rmb && it->second.dwKind == kind)
		{
//			IME_ERROR("store index %u", it->first);
			return &(it->second);
		}
	}
	return NULL;
}

STC_CONF_STORE* CConfStore::FindByRMBWithRange(uint32 rmb, uint32 kind)
{
	std::map<uint32_t, STC_CONF_STORE*> mapa;
	MapData::iterator it = m_mapData.begin();
	for ( ; it !=  m_mapData.end(); it++)
	{
		if (it->second.byIsBuy && it->second.byStoreChannel == STORE_CHARGE && it->second.dwKind == kind)	
		{
//			IME_ERROR("rmb need %u", it->second.dwObjNumber);
			mapa[it->second.dwObjNumber] = &(it->second);
//			if (rmb >= it->second.dwObjNumber)
//			{
//				return &(it->second);
//			}
		}
	}

	std::map<uint32_t, STC_CONF_STORE*>::iterator itb, afterit;
	for (itb = mapa.begin(); itb != mapa.end(); itb++)
	{
		afterit = itb;
		afterit++;
		if (afterit == mapa.end())
		{
			if (itb->second->dwObjNumber <= rmb)
			{
				return itb->second;
			}
			break;
		}
		else
		{
			if (itb->second->dwObjNumber <= rmb && rmb < afterit->second->dwObjNumber)
			{
				return itb->second;
			}
			continue;
		}
	}
	return NULL;
}

CConfStore::MapData* CConfStore::GetVal()
{
	return &m_mapData;
}

bool CConfStore::Valid()
{
	MapData mapData;
	if (!ReadConfig(mapData))
	{
		IME_ERROR("valid fail read config");
		return false;
	}
	return true;
}

// exskill
CConfExskill::MapData CConfExskill::m_mapData;

bool CConfExskill::Init(const std::string& strFilePath )
{
	m_mapData.clear();

	CConfigReader reader;
	if (!reader.init(strFilePath))
	{
		return false;
	}
	int nRow = reader.rowCount();
	if (nRow <= 1)
	{
		IME_ERROR("the row is small than 1 rowCount %u", nRow);
		return false;
	}
	for (int i = 1; i < nRow; ++i)
	{
		int j = 0;
		STC_CONF_EXSKILL info;
		READER_INT(info.dwIndex);
		j++;
		READER_INT(info.byType);
		READER_INT(info.dwPara1);
		IS_READER_ERROR;

		if (m_mapData.insert(std::make_pair(info.dwIndex, info)).second == false)
		{
			IME_ERROR("ERROR CONF DUPLICATE [ChestId=%u]", info.dwIndex);
			return false;
		}
	}

	IME_LOG("LOAD CONF SHOP PRIZE [Size=%d]", m_mapData.size());

	return true;
}

STC_CONF_EXSKILL* CConfExskill::Find(uint32 dwIndex)
{
	MapData::iterator it = m_mapData.find(dwIndex);
	return it == m_mapData.end() ? NULL : &it->second;
}

bool CConfExskill::Valid(const std::string& strFilePath )
{
    MapData m_mapData;

	CConfigReader reader;
	if (!reader.init(strFilePath))
	{
		return false;
	}
	int nRow = reader.rowCount();
	if (nRow <= 1)
	{
		IME_ERROR("the row is small than 1 rowCount %u", nRow);
		return false;
	}
	for (int i = 1; i < nRow; ++i)
	{
		int j = 0;
		STC_CONF_EXSKILL info;
		READER_INT(info.dwIndex);
		j++;
		READER_INT(info.byType);
		READER_INT(info.dwPara1);
		IS_READER_ERROR;

		if (m_mapData.insert(std::make_pair(info.dwIndex, info)).second == false)
		{
			IME_ERROR("ERROR CONF DUPLICATE [ChestId=%u]", info.dwIndex);
			return false;
		}
	}

	IME_LOG("LOAD CONF SHOP PRIZE [Size=%d]", m_mapData.size());

	return true;
}

// active
CConfActive::MapData CConfActive::m_mapData;

bool CConfActive::Init(const std::string& strFilePath )
{
	m_mapData.clear();

	CConfigReader reader;
	if (!reader.init(strFilePath))
	{
		return false;
	}
	int nRow = reader.rowCount();
	if (nRow <= 1)
	{
		IME_ERROR("the row is small than 1 rowCount %u", nRow);
		return false;
	}
	for (int i = 1; i < nRow; ++i)
	{
		int j = 0;
		STC_CONF_ACTIVE info;
		READER_INT(info.dwIndex);
		READER_INT(info.byType);
		READER_STR(info.strName);
		READER_STR(info.strDesc);
		READER_INT(info.wOpr);
		READER_INT(info.dwPara);
		READER_STR(info.strIcon);
		READER_INT(info.dwSort);
		IS_READER_ERROR;

		if (m_mapData.insert(std::make_pair(info.dwIndex, info)).second == false)
		{
			IME_ERROR("ERROR CONF DUPLICATE [ChestId=%u]", info.dwIndex);
			return false;
		}
	}

	IME_LOG("LOAD CONF ACTIVE [Size=%d]", m_mapData.size());

	return true;
}

STC_CONF_ACTIVE* CConfActive::Find(uint32 dwIndex)
{
	MapData::iterator it = m_mapData.find(dwIndex);
	return it == m_mapData.end() ? NULL : &it->second;
}

bool CConfActive::Valid(const std::string& strFilePath )
{
    MapData m_mapData;

	CConfigReader reader;
	if (!reader.init(strFilePath))
	{
		return false;
	}
	int nRow = reader.rowCount();
	if (nRow <= 1)
	{
		IME_ERROR("the row is small than 1 rowCount %u", nRow);
		return false;
	}
	for (int i = 1; i < nRow; ++i)
	{
		int j = 0;
		STC_CONF_ACTIVE info;
		READER_INT(info.dwIndex);
		READER_INT(info.byType);
		READER_STR(info.strName);
		READER_STR(info.strDesc);
		READER_INT(info.wOpr);
		READER_INT(info.dwPara);
		READER_STR(info.strIcon);
		READER_INT(info.dwSort);
		IS_READER_ERROR;

		if (m_mapData.insert(std::make_pair(info.dwIndex, info)).second == false)
		{
			IME_ERROR("ERROR CONF DUPLICATE [ChestId=%u]", info.dwIndex);
			return false;
		}
	}

	IME_LOG("LOAD CONF ACTIVE [Size=%d]", m_mapData.size());

	return true;
}

CConfActive::MapData& CConfActive::GetData()
{
	return m_mapData;
}

// card collection
CConfCycleLogin::MapData CConfCycleLogin::m_mapData;

bool CConfCycleLogin::ReadConfig(const std::string& strFilePath, MapData &mapt)
{
	CConfigReader reader;
	if (!reader.init(strFilePath))
	{
		return false;
	}
	int nRow = reader.rowCount();
	for (int i = 1; i < nRow; ++i)
	{
		int j = 0;
		STC_CONF_CYCLE_LOGIN info;
		READER_INT(info.dwIndex);
		READER_INT(info.wObj1Type);
		READER_INT(info.dwObj1Para1);
		READER_INT(info.dwObj1Para2);
		READER_INT(info.wObj2Type);
		READER_INT(info.dwObj2Para1);
		READER_INT(info.dwObj2Para2);
		READER_INT(info.wObj3Type);
		READER_INT(info.dwObj3Para1);
		READER_INT(info.dwObj3Para2);
		READER_INT(info.dwNextIndex);
		READER_STR(info.strDesc);
		IS_READER_ERROR;
		
		mapt.insert(std::make_pair(info.dwIndex, info));
	}

	IME_LOG("LOAD CONF CARDDROP [Size=%d]", mapt.size());
	return true;
}

bool CConfCycleLogin::Init( const std::string& strFilePath )
{
	m_mapData.clear();	

	if (!ReadConfig(strFilePath, m_mapData))
	{
		IME_ERROR("read config fail");
		return false;
	}

	return true;
}

STC_CONF_CYCLE_LOGIN* CConfCycleLogin::Find(uint32 dwIndex)
{
	MapData::iterator it = m_mapData.find(dwIndex);
	return it == m_mapData.end() ? NULL : &it->second;
}

bool CConfCycleLogin::Valid( const std::string& strFilePath )
{
	MapData mapData;
	
	if (!ReadConfig(strFilePath, mapData))
	{
		IME_ERROR("valid config fail");
		return false;
	}

	return true;
}

uint32_t CConfCycleLogin::GetFirstRecord()
{
	if (m_mapData.size() == 0)
		return 0;
	return (m_mapData.begin()->first);
}

// vitality prize
CConfVitalityPrize::MapData CConfVitalityPrize::m_mapData;

bool CConfVitalityPrize::ReadConfig(const std::string& strFilePath, MapData &mapt)
{
	CConfigReader reader;
	if (!reader.init(strFilePath))
	{
		return false;
	}
	int nRow = reader.rowCount();
	for (int i = 1; i < nRow; ++i)
	{
		int j = 0;
		STC_CONF_VITALITY_PRIZE info;
		READER_INT(info.dwIndex);
		READER_INT(info.dwNeedActive);
		READER_INT(info.wObj1Type);
		READER_INT(info.dwObj1Para1);
		READER_INT(info.dwObj1Para2);
		READER_INT(info.wObj2Type);
		READER_INT(info.dwObj2Para1);
		READER_INT(info.dwObj2Para2);
		IS_READER_ERROR;
		
		mapt.insert(std::make_pair(info.dwIndex, info));
	}
	if (mapt.size() > MAX_ACTIVE_PRIZE_NUMBER)
	{
		IME_ERROR("active prize number big than max");
		return false;
	}

	IME_LOG("LOAD CONF CARDDROP [Size=%d]", mapt.size());
	return true;
}

bool CConfVitalityPrize::Init( const std::string& strFilePath )
{
	m_mapData.clear();	

	if (!ReadConfig(strFilePath, m_mapData))
	{
		IME_ERROR("read config fail");
		return false;
	}

	return true;
}

STC_CONF_VITALITY_PRIZE* CConfVitalityPrize::Find(uint32 dwIndex)
{
	MapData::iterator it = m_mapData.find(dwIndex);
	return it == m_mapData.end() ? NULL : &it->second;
}

bool CConfVitalityPrize::Valid( const std::string& strFilePath )
{
	MapData mapData;
	
	if (!ReadConfig(strFilePath, mapData))
	{
		IME_ERROR("valid config fail");
		return false;
	}

	return true;
}


CConfTowerRankPrize::MapData CConfTowerRankPrize::m_mapData;
bool CConfTowerRankPrize::Init(const std::string& strFilePath) {
	m_mapData.clear();
	if (!ReadConfig(strFilePath, m_mapData)) {
		IME_ERROR("read config fail");
		return false;
	}
	return true;
}

STC_TOWER_RANK_PRIZE* CConfTowerRankPrize::Find(uint32 dwIndex) {
	MapData::iterator it = m_mapData.find(dwIndex);
	return it == m_mapData.end() ? NULL : &it->second;
}

bool CConfTowerRankPrize::Valid(const std::string& strFilePath) {
	MapData mapData;
	if (!ReadConfig(strFilePath, mapData)) {
		IME_ERROR("valid config fail");
		return false;
	}
	return true;
}

bool CConfTowerRankPrize::ReadConfig(const std::string& strFilePath,
		MapData& mapt) {
	CConfigReader reader;
	if (!reader.init(strFilePath)) {
		return false;
	}
	int nRow = reader.rowCount();
	for (int i = 1; i < nRow; ++i) {
		int j=0;
		uint32 index;
		READER_INT(index);
		uint32 start;
		uint32 end;
		uint32 type;
		uint32 param1;
		uint32 param2;
		READER_INT(start);
		READER_INT(end);
		READER_INT(type);
		READER_INT(param1);
		READER_INT(param2);
		IS_READER_ERROR;
		for (int k = start; k <= end; k++) {
			STC_TOWER_RANK_PRIZE info;
			info.rank = k;
			info.type = type;
			info.param1 = param1;
			info.param2 = param2;
			mapt.insert(std::make_pair(k, info));
		}
	}
//	int i=1;
//	for(MapData::iterator it = mapt.begin(); it != mapt.end(); it++,i++){
//		IME_LOG("LOAD CONF TOWER_RANK rank %d  type %d, param1 %d, param2 %d", i,it->second.type,it->second.param1,it->second.param2);
//	}

	if(mapt.size() != 20){
		IME_ERROR("Config TowerRankPrize.csv Error!");
	}
	IME_LOG("LOAD CONF TOWER_RANK [Size=%d]", mapt.size());
	return true;
}
