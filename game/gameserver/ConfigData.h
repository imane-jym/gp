/*
 * ConfigData.h
 *
 *  Created on: 2011-7-5
 *      Author: yq
 */

#ifndef CONFIGDATA_H_
#define CONFIGDATA_H_

#include "Defines.h"
#include "CommonConfig.h"
#include "EnumDefines.h"
#include <map>
#include <vector>
#include <string>
#include <stdint.h>

#define TECH_TYPE_MAX 2

//////////////////////////////////
bool ConfInit(std::string strpath);
bool ConfReload(std::string strpath, std::string strTable);
bool ConfValid(std::string strpath, std::string strTable);

typedef struct _STC_CONF_TEST
{
	uint16 wTestId;

}STC_CONF_TEST;

class CConfTest
{
public:
	typedef std::map<uint16, STC_CONF_TEST> MapData;

public:
	static bool Init(const std::string& strFilePath);

	static STC_CONF_TEST* Find(uint16 wTestId);

private:
	static MapData m_mapData;
};

//typedef struct _STC_CONF_CARD_BASE
//{
//    uint32 dwIndex;
//    uint8 byType;
//    uint8 byAttribute; //卡牌属性
//	uint8 byStarLevel;
//	uint32 dwEffectIndex;
////	uint8 byRare;
//	uint32 dwAttackBase;
//	uint32 dwAttackGrow;
//	uint32 dwDefenceBase;
//	uint32 dwDefenceGrow;
//	uint32 dwHealthBase;
//	uint32 dwHealthGrow;
//	uint32 dwExpGrow;
//	uint32 dwCritRate;
////	uint32 dwDodgeRate;
////	uint32 dwAction;
//	uint32 dwSuitIndex;
//	uint32 dwMasterSkillIndex;
//	uint32 dwSkillIndex;
//	uint32 dwGrowLimit;
//	uint32 dwLimitBreakNumber;
//	uint32 dwLimitBreakPro;
//	uint32 dwAwakePro;
//	uint32 dwAddExp;
//	uint32 dwConsumeCoin;
//	uint32 dwGetCoin;
//	uint32 dwBreakExp;
//	uint8 byMultiPower;
//	uint32 dwMultiTime;
//	uint32 dwStrongSkill;
//	uint8 bySkillFunc;
//	uint32 dwStrongWeapon;
//	uint8 byWeaponFunc;
//	uint32 dwStrongArmor;
//	uint8 byArmorFunc;
//	uint32 dwStrongAccessories;
//	uint8 byAccessoriesFunc;
//	uint32 dwBreakSkill1;
//	uint32 dwBreakSkill2;
//	uint32 dwBreakSkill3;
//	uint32 dwBreakSkill4;
//	uint32 dwBreakSkill5;
//}STC_CONF_CARD_BASE;
//    
//class CConfCardBase
//{
//public:
//    typedef std::map<uint32, STC_CONF_CARD_BASE> MapData;
//    static bool Init(const std::string& strFilePath);
//    static STC_CONF_CARD_BASE* Find(uint32 dwIndex);
//	static std::vector<uint32>* GetVecForDuration() { return &m_vecForDuration;}
//	static std::vector<uint32>* GetVecOne() {return &m_vecOne;}
//	static std::vector<uint32>* GetVecTwo() {return &m_vecTwo;}
//	static MapData* GetAll() {return &m_mapData;}
//
//private:
//    static MapData m_mapData;
//	static std::vector<uint32> m_vecForDuration;
//	static std::vector<uint32> m_vecOne;
//	static std::vector<uint32> m_vecTwo;
//};

//struct STC_CONF_MISSION_BASE {
//	uint32 dwIndex;
//	uint8 byChapter;
//	uint8 byCheckpoint;
//	uint8 byTask;
//	uint8 byType;
//	uint8 byStepLimit;
//	uint16 wConsumeEnergy;
//	uint16 wGetExp;
//	uint16 wGetCoin;
//	uint8 byRate;
//	uint32 dwRandomEventIndex;
//	uint32 dwBoss;
//	uint32 dwNextTaskIndex;
////	uint16 wLevelLimit;
////	uint32 dwCD;
////	uint32 dwSpecial;
////	uint8 byPro;
////	uint32 dwAttackPrize;
////	uint8 byAttackPro;
////	uint32 dwDefencePrize;
////	uint8 byDefencePro;
////	uint8 byDurationCmd;
//};				/* ----------  end of struct STC_CONF_MISSION_BASE  ---------- */
//
//typedef struct STC_CONF_MISSION_BASE STC_CONF_MISSION_BASE;
//
//class CConfMissionBase
//{
//public:
//    typedef std::map<uint32, STC_CONF_MISSION_BASE> MapData;
//    static bool Init(const std::string& strFilePath);
//    static STC_CONF_MISSION_BASE* Find(uint32 dwIndex);
//	static int Findp1(uint32 dwIndex, STC_CONF_MISSION_BASE **val);
//
//private:
//    static MapData m_mapData;
//};

//struct STC_CONF_RANDOM_EVENT {
//	uint32 dwIndex;
//	uint16 wType;
//	int32 para1;
//	int32 para2;
//	int32 para3;
//	int32 para4;
//};				/* ----------  end of struct STC_CONF_MISSION_BASE  ---------- */
//typedef struct STC_CONF_RANDOM_EVENT STC_CONF_RANDOM_EVENT;
//
//class CConfRandomEvent
//{
//public:
//    typedef std::map<uint32, STC_CONF_RANDOM_EVENT> MapData;
//    static bool Init(const std::string& strFilePath);
//    static STC_CONF_RANDOM_EVENT* Find(uint32 dwIndex);
//
//private:
//    static MapData m_mapData;
//};


struct STC_CONF_CARD_COLLECTION {
	uint32 dwIndex;
	uint32 dwCardindex;
	uint8 byWeight;
	uint8 byStar;
	std::string strContent;
};				/* ----------  end of struct STC_CONF_CARD_COLLECTION  ---------- */

typedef struct STC_CONF_CARD_COLLECTION STC_CONF_CARD_COLLECTION;
class CConfCardCollection
{
public:
    typedef std::multimap<uint32, STC_CONF_CARD_COLLECTION> MapData;
    static bool Init(const std::string& strFilePath);
    static std::vector<STC_CONF_CARD_COLLECTION *>* Find(uint32 dwIndex);
	static bool Valid(const std::string& strFilePath); 
	static bool ReadConfig(const std::string& strFilePath, MapData &map);

private:
    static MapData m_mapData;
	static std::vector<STC_CONF_CARD_COLLECTION *> m_vecData;
};

struct STC_CONF_ROLE {
	uint32 dwIndex;
	uint32 dwExpLimit;
	uint32 dwEnergyLimit;
	uint32 dwStrengthLimit;
	uint32 dwEnergyRate;
	uint32 dwStrengthRate;
	uint8 byCardCapacity;
	uint32 dwFriendLimit;
	uint32 dwStaminaLimit;
	uint32 dwStaminaRate;
	uint32 dwRoleExp;
	uint32 dwCardExp;
	uint32 dwCoin;
};			

typedef struct STC_CONF_ROLE STC_CONF_ROLE;

class CConfRole
{
public:
    typedef std::map<uint32, STC_CONF_ROLE> MapData;
    static bool Init(const std::string& strFilePath);
    static STC_CONF_ROLE* Find(uint32 dwIndex);
	static uint32 FindLevel(uint32 dwExp);
	static uint32 GetMaxExp();

private:
    static MapData m_mapData;
	static std::vector<uint32> m_vecExpToLevel;
};

//
//typedef struct STC_CONF_SUIT_SKILL
//{
//	uint32 dwIndex;
//	uint16 wType;
//	int32 para1;
//	int32 para2;
//	uint32 dwCollectionCard1;
//	uint32 dwCollectionCard2;
//	uint32 dwCollectionCard3;
//	uint32 dwCollectionCard4;
//} STC_CONF_SUIT_SKILL;
//
//class CConfSuitSkill
//{
//public:
//    typedef std::map<uint32, STC_CONF_SUIT_SKILL> MapData;
//    static bool Init(const std::string& strFilePath);
//    static STC_CONF_SUIT_SKILL* Find(uint32 dwIndex);
//
//private:
//    static MapData m_mapData;
//};

//typedef struct STC_CONF_SUIT_COLLECTION
//{
//	uint32 dwIndex;
//	uint32 dwCardIndex;
//} STC_CONF_SUIT_COLLECTION;
//
//class CConfSuitCollection
//{
//public:
//    typedef std::multimap<uint32, uint32> MapData;
//    static bool Init(const std::string& strFilePath);
//    static std::vector<uint32>* Find(uint32 dwIndex);
//
//private:
//    static MapData m_mapData;
//	static std::vector<uint32> m_vecData;
//};

class CConfAttributeRestriction
{
public:
    static bool Init(const std::string& strFilePath);
    static uint32* Find(uint32 dwIndex, uint32 dwIndex2);
	static bool Valid(const std::string& strFilePath);

private:
	static std::vector<std::vector<uint32> > m_vecData;
};

//typedef struct STC_CONF_CARD_SKILL
//{
//	uint32 dwIndex;
//	uint8 byStarLevel;
//	uint8 byAttribute;
//	uint16 wType;
//	uint16 wEffectType;
//	uint32 dwPara1;
//	uint32 dwPro;
//} STC_CONF_CARD_SKILL;
//
//class CConfCardSkill
//{
//public:
//    typedef std::map<uint32, STC_CONF_CARD_SKILL> MapData;
//    static bool Init(const std::string& strFilePath);
//    static STC_CONF_CARD_SKILL* Find(uint32 dwIndex);
//	static MapData& GetVal()
//	{
//		return m_mapData;
//	}
//
//private:
//    static MapData m_mapData;
//};

//typedef struct STC_CONF_ITEM_COMBINE
//{
//	uint32 dwCardIndex;
//	uint32 dwItemNumber;
//	uint32 dWCollectionIndex;
//	uint32 dwProbability;
//} STC_CONF_ITEM_COMBINE;
//
//class CConfItemCombine
//{
//public:
//    typedef std::multimap<uint32, STC_CONF_ITEM_COMBINE> MapData;
//    static bool Init(const std::string& strFilePath);
//    static std::vector<STC_CONF_ITEM_COMBINE *>* Find(uint32 dwIndex);
//
//private:
//    static MapData m_mapData;
//	static std::vector<STC_CONF_ITEM_COMBINE *> m_vecData;
//};

//typedef struct STC_CONF_ITEM_COMBINE_COLLECTION
//{
//	uint32 dwIndex;
//	uint32 dwID1;
//	uint32 dwID2;
//	uint32 dwID3;
//	uint32 dwID4;
//	uint32 dwID5;
//	uint32 dwID6;
//} STC_CONF_ITEM_COMBINE_COLLECTION;
//
//class CConfCombineCollection
//{
//public:
//    typedef std::map<uint32, STC_CONF_ITEM_COMBINE_COLLECTION> MapData;
//    static bool Init(const std::string& strFilePath);
//    static STC_CONF_ITEM_COMBINE_COLLECTION* Find(uint32 dwIndex);
//
//private:
//    static MapData m_mapData;
//};

//typedef struct STC_CONF_CARD_UP_CONSUME
//{
//	uint32 dwStar;
//	uint32 dwStrengthCon;
//	uint32 dWEvolutionCon;
//} STC_CONF_CARD_UP_CONSUME;
//
//class CConfCardUpConsume
//{
//public:
//    typedef std::map<uint32, STC_CONF_CARD_UP_CONSUME> MapData;
//    static bool Init(const std::string& strFilePath);
//    static STC_CONF_CARD_UP_CONSUME* Find(uint32 dwIndex);
//
//private:
//    static MapData m_mapData;
//};

//typedef struct STC_CONF_CARD_EVOLUTION
//{
//	uint32 dwCardIndex1;
//	uint32 dwCardIndex2;
//	uint32 dWCondition;
//} STC_CONF_CARD_EVOLUTION;
//
//class CConfCardEvolution
//{
//public:
//    typedef std::map<uint32, STC_CONF_CARD_EVOLUTION> MapData;
//    static bool Init(const std::string& strFilePath);
//    static STC_CONF_CARD_EVOLUTION* Find(uint32 dwIndex);
//
//private:
//    static MapData m_mapData;
//};

typedef struct STC_CONF_TECH
{
	uint32 dwIndex;
	uint8 byType;
} STC_CONF_CARD_TECH;

class CConfTech
{
public:
    typedef std::map<uint32, STC_CONF_TECH> MapData;
    static bool Init(const std::string& strFilePath);
    static STC_CONF_TECH* Find(uint32 dwIndex);
	static MapData *GetMap();

private:
    static MapData m_mapData;
};

typedef struct STC_CONF_TECH_RATE
{
	uint16 wLv;
	uint16 wRoleLv;
	uint16 wGroupLevelLimit;

//	uint32 dwRate;
	uint32 dwVal[TECH_TYPE_MAX];
	uint32 dwCoin[TECH_TYPE_MAX];
//	uint32 dwVal2;
	uint32 dwCreditConsume[TECH_TYPE_MAX];
} STC_CONF_TECH_RATE;

class CConfTechRate
{
public:
    typedef std::map<uint32, STC_CONF_TECH_RATE> MapData;
    static bool Init(const std::string& strFilePath);
    static STC_CONF_TECH_RATE* Find(uint32 dwIndex);
	static uint16 GetMaxLevel();

private:
    static MapData m_mapData;
};

//typedef struct STC_CONF_ARENA_PRESTIGE
//{
//	uint32 dwIndex;
//	uint8 byLevel;
//	uint8 byType;
//	uint32 dwItemIndex;
//	uint8 byStarLevel;
//	uint32 dwPrestige;
//} STC_CONF_ARENA_PRESTIGE;
//
//class CConfArenaPrestige
//{
//public:
//    typedef std::map<uint32, STC_CONF_ARENA_PRESTIGE> MapData;
//    static bool Init(const std::string& strFilePath);
//    static STC_CONF_ARENA_PRESTIGE* Find(uint32 dwIndex);
//	static void FlushPrestigeCollect();
//	static bool CollectionFind(uint32 index);
//	static void GetPrestigeCollect(std::map<uint32, STC_CONF_ARENA_PRESTIGE> &pre);
//
//private:
//    static MapData m_mapData;
//	static std::map<uint32, STC_CONF_ARENA_PRESTIGE> m_mapView;
//	static std::vector<uint32> m_vec1;
//	static std::vector<uint32> m_vec2;
//	static std::vector<uint32> m_vec3;
//};
//
//typedef struct STC_CONF_ARENA_RANK
//{
//	uint32 dwIndex;
//	uint32 dwLow;
//	uint32 dwHigh;
//	uint8 byType;
//	uint32 dwItemIndex;
//	uint8 byStarLevel;
//	uint32 dwNumber;
//	uint8 byType2;
//	uint32 dwItemIndex2;
//	uint32 dwNumber2;
//} STC_CONF_ARENA_RANK;
//
//class CConfArenaRank
//{
//public:
//    typedef std::map<uint32, STC_CONF_ARENA_RANK> MapData;
//    static bool Init(const std::string& strFilePath);
//    static STC_CONF_ARENA_RANK* Find(uint32 dwIndex);
//	static STC_CONF_ARENA_RANK* GetRow(uint32 level);
//
//private:
//    static MapData m_mapData;
//};

struct STC_CONF_ITEM_COLLECTION {
	uint32 dwIndex;
	uint32 dwItemIndex;
	uint8 byWeight;
};				/* ----------  end of struct STC_CONF_ITEM_COLLECTION  ---------- */

typedef struct STC_CONF_ITEM_COLLECTION STC_CONF_ITEM_COLLECTION;
class CConfItemCollection
{
public:
    typedef std::multimap<uint32, STC_CONF_ITEM_COLLECTION> MapData;
    static bool Init(const std::string& strFilePath);
    static std::vector<STC_CONF_ITEM_COLLECTION *>* Find(uint32 dwIndex);
	static bool Valid(const std::string& strFilePath);

private:
    static MapData m_mapData;
	static std::vector<STC_CONF_ITEM_COLLECTION *> m_vecData;
};

//typedef struct STC_CONF_MISSION_BOSS
//{
//	uint32 dwIndex;
//
//	uint32 dwCardIndex;
//	uint8 byAttribute;
//	uint32 dwAttack;
//	uint32 dwDefence;
//	uint32 dwHealth;
//	uint32 dwCritRate;
//	uint32 dwDodgeRate;
//	uint32 dwAction;
//	uint32 dwInitSkill;
//	uint32 dwBigSkill;
//	uint8 byPosition;
//} STC_CONF_MISSION_BOSS;
//
//class CConfMissionBoss
//{
//public:
//    typedef std::map<uint32, STC_CONF_MISSION_BOSS> MapData;
//    static bool Init(const std::string& strFilePath);
//    static STC_CONF_MISSION_BOSS* Find(uint32 dwIndex);
//
//private:
//    static MapData m_mapData;
//};

class CConfGlobal
{
public:
    typedef std::map<uint32, int32> MapData;
    static bool Init(const std::string& strFilePath);
    static int32* Find(uint32 dwIndex, int32 &val);
	static bool Valid(const std::string& strFilePath);

private:
    static MapData m_mapData;
};

typedef struct STC_CONF_GROUP_INFO
{
	uint32 dwIndex;
	uint32 dwNeedCredit;
	uint32 dwCreditLimit;
	uint16 wPersionLimit;
	uint32 position1Persion;
	uint32 position2Persion;
	uint32 position3Persion;
	uint32 position4Persion;
} STC_CONF_GROUP_INFO;

class CConfGroupInfo
{
public:
    typedef std::map<uint32, STC_CONF_GROUP_INFO> MapData;
    static bool Init(const std::string& strFilePath);
    static STC_CONF_GROUP_INFO* Find(uint32 dwIndex);
private:
    static MapData m_mapData;
};

typedef struct STC_CONF_GROUP_ADMIN
{
	uint32 dwIndex;
	uint8 byAppoint;
	uint8 byLevelUp;
	uint8 byMemberIO;
	uint8 byUpdateNotice;
	uint8 byFlushTask;
	uint8 byMakeAlliance;
	uint8 byFight;
	uint8 byCredit;
} STC_CONF_GROUP_ADMIN;

class CConfGroupAdmin
{
public:
    typedef std::map<uint32, STC_CONF_GROUP_ADMIN> MapData;
    static bool Init(const std::string& strFilePath);
    static STC_CONF_GROUP_ADMIN* Find(uint32 dwIndex);
    static uint32 GetVal(uint32 posion, uint32 auth);
private:
    static MapData m_mapData;
};

typedef struct STC_CONF_GROUP_MISSION
{
	uint32 dwIndex;
	uint16 wLevelLimit;
	uint8 byStepLimit;
	uint16 wConsumeEnergy;
	uint16 wGetCredit;
	uint16 wGetCoin;
	uint8 byRate;
	uint32 dwRandomEventIndex;
	uint32 dwBoss;
	uint8 byDurationCmd;
} STC_CONF_GROUP_MISSION;

class CConfGroupMission
{
public:
    typedef std::map<uint32, STC_CONF_GROUP_MISSION> MapData;
    static bool Init(const std::string& strFilePath);
    static STC_CONF_GROUP_MISSION* Find(uint32 dwIndex);
private:
    static MapData m_mapData;
};

typedef struct STC_CONF_GROUP_PRIZE
{
	uint32 dwIndex;
	uint16 byType;
	uint32 dwCombineId;
	uint32 dwConsumeCredit;
} STC_CONF_GROUP_PRIZE;

class CConfGroupPrize
{
public:
    typedef std::map<uint32, STC_CONF_GROUP_PRIZE> MapData;
    static bool Init(const std::string& strFilePath);
    static STC_CONF_GROUP_PRIZE* Find(uint32 dwIndex);
private:
    static MapData m_mapData;
};

//typedef struct STC_CONF_GOODS
//{
//	uint32 dwIndex;
//	uint32 dwFuncType;
//	uint32 dwPara1;
//	uint32 dwPara2;
//	uint32 dwPara3;
//	uint32 dwPara4;
//	uint32 dwLevelLimit;
//} STC_CONF_GOODS;
//
//class CConfGoods
//{
//public:
//    typedef std::map<uint32, STC_CONF_GOODS> MapData;
//	static bool ReadConfig(const std::string& strFilePath, MapData &map);
//    static bool Init(const std::string& strFilePath);
//    static STC_CONF_GOODS* Find(uint32 dwIndex);
//    static MapData* GetVal();
//	static bool Valid(const std::string& strFilePath);
//private:
//    static MapData m_mapData;
//};

typedef struct STC_CONF_STORE
{
	uint32 dwStoreIndex;
	uint8 byStoreChannel;
	uint32 dwIndex;
	uint16 byType;
	uint8 byObjType;
	uint32 dwObjNumber;
	uint8 byIsBuy;
	uint32 dwViewOrder;
	std::string strContent;
	uint32 dwLimitNumber;
	uint32 dwRoleLimitNumber;
	uint32 dwIndexNumber;
	uint32 dwSendPrize;
	uint32 dwViewPrize;
	uint32 dwMaxBuy;
	uint32 dwKind;
	uint8 byGoodsType;
} STC_CONF_STORE;

class CConfStore
{
public:
    typedef std::map<uint32, STC_CONF_STORE> MapData;
	static bool ReadConfig(MapData &map);
    static bool Init();
    static STC_CONF_STORE* Find(uint32 dwIndex);
	static STC_CONF_STORE* FindByRMB(uint32 dollar, uint32 kind);
	static STC_CONF_STORE* FindByRMBWithRange(uint32 dollar, uint32 kind);
    static MapData* GetVal();
	static bool Valid();
private:
    static MapData m_mapData;
};

//typedef struct STC_CONF_JEWEL_SKILL_STRENGTH
//{
//	uint32 dwIndex;
//	uint32 dwSkillPara;
//} STC_CONF_JEWEL_SKILL_STRENGTH;
//
//class CConfJewelSkillStrength
//{
//public:
//    typedef std::map<uint32, STC_CONF_JEWEL_SKILL_STRENGTH> MapData;
//    static bool Init(const std::string& strFilePath);
//    static STC_CONF_JEWEL_SKILL_STRENGTH* Find(uint32 dwIndex);
//private:
//    static MapData m_mapData;
//};

//typedef struct STC_CONF_SKILL_CREATE
//{
//	uint32 byStar;
//	uint32 dwSkillIndex;
//	uint32 dwInitPro;
//	uint32 dwLowPro;
//	uint32 dwHighPro;
//} STC_CONF_SKILL_CREATE;
//
//class CConfSkillCreate
//{
//public:
//    typedef std::map<uint32, STC_CONF_SKILL_CREATE> MapData;
//    static bool Init(const std::string& strFilePath);
//	static uint32 GetSkillIndex(uint8 star);
//	static STC_CONF_SKILL_CREATE *Find(uint32 dwSkillIndex, uint8 star);
//private:
////    static std::vector<STC_CONF_SKILL_CREATE> m_mapData;
//	static std::vector<int> m_vec2Star;
//	static std::vector<int> m_vec2InitPro;
//	static MapData m_mapData2;
//
//	static std::vector<int> m_vec3Star;
//	static std::vector<int> m_vec3InitPro;
//	static MapData m_mapData3;
//
//	static std::vector<int> m_vec4Star;
//	static std::vector<int> m_vec4InitPro;
//	static MapData m_mapData4;
//	
//	static std::vector<int> m_vec5Star;
//	static std::vector<int> m_vec5InitPro;
//	static MapData m_mapData5;
//
//};

//class CConfWeaponRestriction
//{
//public:
//    static bool Init(const std::string& strFilePath);
//    static uint32* Find(uint32 dwIndex, uint32 dwIndex2);
//
//private:
//	static std::vector<std::vector<uint32> > m_vecData;
//};

typedef struct STC_CONF_UNIQUE_COLLECTION {
	uint32 dwIndex;
//	uint32 dwRandomEventIndex;
//	uint32 wWeight;
	uint32 wWeight;
	uint16 byType;
	int32 dwPara1;
	int32 dwPara2;
	std::string strContent;
}STC_CONF_UNIQUE_COLLECTION;

class CConfUniqueCollection
{
public:
    typedef std::multimap<uint32, STC_CONF_UNIQUE_COLLECTION> MapData;
	static bool ReadConfig(const std::string& strFilePath, MapData &tmp);
    static bool Init(const std::string& strFilePath);
    static std::vector<STC_CONF_UNIQUE_COLLECTION *>* Find(uint32 dwIndex);
	static STC_CONF_UNIQUE_COLLECTION* GetRandResult(uint32 index);
	static bool Valid(const std::string& strFilePath);

private:
    static MapData m_mapData;
	static std::vector<STC_CONF_UNIQUE_COLLECTION *> m_vecData;
};

//typedef struct STC_CONF_SKILL_COLLECTION {
//	uint32 dwIndex;
//	uint32 dwSkillIndex;
//	uint16 wWeight;
//}STC_CONF_SKILL_COLLECTION;
//
//class CConfSkillCollection
//{
//public:
//    typedef std::multimap<uint32, STC_CONF_SKILL_COLLECTION> MapData;
//    static bool Init(const std::string& strFilePath);
//    static std::vector<STC_CONF_SKILL_COLLECTION *>* Find(uint32 dwIndex);
//	static STC_CONF_SKILL_COLLECTION* GetRandResult(uint32 index);
//
//private:
//    static MapData m_mapData;
//	static std::vector<STC_CONF_SKILL_COLLECTION *> m_vecData;
//};

typedef struct STC_CONF_GIFT_COLLECTION {
	uint32 dwIndex;
	uint16 wType1;
	uint32 dwProductIndex1;
	uint32 dwNumber1;
	uint16 wType2;
	uint32 dwProductIndex2;
	uint32 dwNumber2;
	uint16 wType3;
	uint32 dwProductIndex3;
	uint32 dwNumber3;
	uint16 wType4;
	uint32 dwProductIndex4;
	uint32 dwNumber4;
	uint32 wWeight;
}STC_CONF_GIFT_COLLECTION;

class CConfGiftCollection
{
public:
    typedef std::multimap<uint32, STC_CONF_GIFT_COLLECTION> MapData;
	static bool ReadConfig(const std::string& strFilePath, MapData &tmp);
    static bool Init(const std::string& strFilePath);
    static std::vector<STC_CONF_GIFT_COLLECTION*>* Find(uint32 dwIndex);
    static bool Valid(const std::string& strFilePath);
	static STC_CONF_GIFT_COLLECTION* GetRandResult(uint32 index);

private:
    static MapData m_mapData;
	static std::vector<STC_CONF_GIFT_COLLECTION *> m_vecData;
};

typedef struct STC_CONF_GOODS_COLLECTION {
	uint32 dwIndex;
	uint32 dwItemIndex;
	uint8 byWeight;
	uint32 dwNumber;
	std::string strContent;
}STC_CONF_GOODS_COLLECTION;

class CConfGoodsCollection
{
public:
    typedef std::multimap<uint32, STC_CONF_GOODS_COLLECTION> MapData;
	static bool ReadConfig(const std::string& strFilePath, MapData &map);
    static bool Init(const std::string& strFilePath);
    static std::vector<STC_CONF_GOODS_COLLECTION *>* Find(uint32 dwIndex);
	static bool Valid(const std::string& strFilePath);

private:
    static MapData m_mapData;
	static std::vector<STC_CONF_GOODS_COLLECTION *> m_vecData;
};

typedef struct STC_CONF_MASTER
{
	uint32 dwIndex;

	std::string strName;

	uint32 dwStar;
	uint8 byType;

	std::string strSkillName;

	uint32 dwSkillIndex;
	uint32 dwNumberMax;
	uint32 dwNeedLevel;
	uint32 dwAliveTime;
	uint32 dwSendRedCon;
	uint32 dwBlessCD;
	uint32 dwMasterCardIndex;
	uint32 dwFixPro;

	uint32 dwLowPro;
	uint32 dwMiddlePro;
	uint32 dwHighPro;
	uint32 dwFirstLowPro;
	uint32 dwFirstMiddlePro;
	uint32 dwFirstHighPro;
} STC_CONF_MASTER;

class CConfMaster
{
public:
    typedef std::map<uint32, STC_CONF_MASTER> MapData;
    static bool Init(const std::string& strFilePath);
    static STC_CONF_MASTER* Find(uint32 dwIndex);
	static STC_CONF_MASTER* RandGet(uint32 level, uint32 star_s, uint32 star_e, int pro);
	static STC_CONF_MASTER* RandGetFirst(uint32 level, uint32 star_s, uint32 star_e, int pro);
	static bool Valid(const std::string& strFilePath);
private:
    static MapData m_mapData;
};

typedef struct STC_CONF_MASTER_PRO
{
	uint32 dwIndex;
	uint32 dwLowPro;
	uint32 dwMiddlePro;
	uint32 dwHighPro;
	uint32 dwFirstLowPro;
	uint32 dwFirstMiddlePro;
	uint32 dwFirstHighPro;
} STC_CONF_MASTER_PRO;

class CConfMasterPro
{
public:
    typedef std::map<uint32, STC_CONF_MASTER_PRO> MapData;
    static bool Init(const std::string& strFilePath);
    static STC_CONF_MASTER_PRO* Find(uint32 dwIndex);
	static MapData *GetVal() {return &m_mapData;};
private:
    static MapData m_mapData;
};

//typedef struct STC_CONF_DURATION_MISSION
//{
//	uint32 dwIndex;
//	uint32 dwArea;
//	uint32 dwOrder;
//
//	uint32 dwMasterCardIndex;
//	uint16 wMasterLevel;
//	uint32 dwMasterSkillIndex;
//	uint16 wMasterSkillLevel;
//	uint32 dwMasterAttack;
//	uint32 dwMasterDefence;
//	uint32 dwMasterHealth;
//
//	uint32 dwCardIndex1;
//	uint16 wLevel1;
//	uint32 dwSkillIndex1;
//	uint16 wSkillLevel1;
//	uint32 dwAttack1;
//	uint32 dwDefence1;
//	uint32 dwHealth1;
//
//	uint32 dwCardIndex2;
//	uint16 wLevel2;
//	uint32 dwSkillIndex2;
//	uint16 wSkillLevel2;
//	uint32 dwAttack2;
//	uint32 dwDefence2;
//	uint32 dwHealth2;
//
//	uint32 dwCardIndex3;
//	uint16 wLevel3;
//	uint32 dwSkillIndex3;
//	uint16 wSkillLevel3;
//	uint32 dwAttack3;
//	uint32 dwDefence3;
//	uint32 dwHealth3;
//
//	uint32 dwCardIndex4;
//	uint16 wLevel4;
//	uint32 dwSkillIndex4;
//	uint16 wSkillLevel4;
//	uint32 dwAttack4;
//	uint32 dwDefence4;
//	uint32 dwHealth4;
//
//	uint32 dwCardIndex5;
//	uint16 wLevel5;
//	uint32 dwSkillIndex5;
//	uint16 wSkillLevel5;
//	uint32 dwAttack5;
//	uint32 dwDefence5;
//	uint32 dwHealth5;
//
//	uint32 dwCardIndex6;
//	uint16 wLevel6;
//	uint32 dwSkillIndex6;
//	uint16 wSkillLevel6;
//	uint32 dwAttack6;
//	uint32 dwDefence6;
//	uint32 dwHealth6;
//	
//	uint16 wEnergyConsume;
//	uint16 wChallengeNumber;
//	uint32 dwExpUp;
//	uint32 dwCardExpUp;
//	uint32 dwCoinUp;
//	uint32 dwDropRate1; 
//	uint32 dwDropCardIndex1;
//	uint32 dwDropRate2; 
//	uint32 dwDropCardIndex2;
//	uint32 dwBossRandomRate;
//	uint32 dwBossRandomIndex1;
//	uint32 dwBossRandomIndex2;
//	uint32 dwBossRandomIndex3;
//	uint32 dwBossRandomIndex4;
//	uint32 dwBossRandomIndex5;
//	uint32 dwBossRandomIndex6;
//	uint32 dwBoss;
//	uint32 dwNextTaskIndex;
//
//	uint8 byType;
//	uint32 wLevelLimit;
//	uint32 dwCD;
//	uint32 dwFBossPro;
//	uint32 dwFBossId;
//
//	std::string strName;
//	uint32 dwHighMission;
//} STC_CONF_DURATION_MISSION;
//
//class CConfDurationMission
//{
//public:
//    typedef std::map<uint32, STC_CONF_DURATION_MISSION> MapData;
//    static bool Init(const std::string& strFilePath);
//    static STC_CONF_DURATION_MISSION* Find(uint32 dwIndex);
//private:
//    static MapData m_mapData;
//};

typedef struct STC_CONF_GIFT_PACKET {
	uint32 dwIndex;
	uint32 dwRandomEventIndex;
	uint32 dwCD;
	uint32 dwNumber;
}STC_CONF_GIFT_PACKET;

class CConfGiftPacket
{
public:
    typedef std::map<uint32, STC_CONF_GIFT_PACKET> MapData;
    static bool Init(const std::string& strFilePath);
    static STC_CONF_GIFT_PACKET* Find(uint32 dwIndex);

private:
    static MapData m_mapData;
};

struct STC_CONF_MASTER_LEVEL {
	uint16 wLevel;
	uint32 dwType;
	uint32 dwNumber;
	uint32 dwExp;
	uint32 dwAddExp;
	uint32 dwPro;
	uint32 dwPrizePro;
};			

typedef struct STC_CONF_MASTER_LEVEL STC_CONF_MASTER_LEVEL;

class CConfMasterLevel
{
public:
    typedef std::map<uint8, std::map<uint32, STC_CONF_MASTER_LEVEL> > MapData;
    static bool Init(const std::string& strFilePath);
    static STC_CONF_MASTER_LEVEL* Find(uint32 dwType, uint16 wLevel);
	static uint16 FindLevel(uint32 dwType, uint32 dwExp);
	static uint32 GetMaxExp(uint32 dwType);
	static STC_CONF_MASTER_LEVEL *GetRandPrize(uint32 type, uint32 level);
	static bool Valid(const std::string& strFilePath);

private:
    static MapData m_mapData;
	static std::map<uint32, std::vector<uint32> > m_mapExpToLevel;
};

struct STC_CONF_ROB_ROBOT {
	uint32 dwIndex;
	uint16 wLevel;
	std::string strName;

	uint32 dwMasterCardIndex;
	uint16 wMasterLevel;
	uint32 dwMasterSkillIndex;
	uint16 wMasterSkillLevel;
	uint32 dwMasterAttackGrow;
	uint32 dwMasterDefenceGrow;
	uint32 dwMasterHealthGrow;

	uint32 dwCardIndex1;
	uint16 wLevel1;
	uint32 dwSkillIndex1;
	uint16 wSkillLevel1;
	uint32 dwAttackGrow1;
	uint32 dwDefenceGrow1;
	uint32 dwHealthGrow1;

	uint32 dwCardIndex2;
	uint16 wLevel2;
	uint32 dwSkillIndex2;
	uint16 wSkillLevel2;
	uint32 dwAttackGrow2;
	uint32 dwDefenceGrow2;
	uint32 dwHealthGrow2;

	uint32 dwCardIndex3;
	uint16 wLevel3;
	uint32 dwSkillIndex3;
	uint16 wSkillLevel3;
	uint32 dwAttackGrow3;
	uint32 dwDefenceGrow3;
	uint32 dwHealthGrow3;

	uint32 dwCardIndex4;
	uint16 wLevel4;
	uint32 dwSkillIndex4;
	uint16 wSkillLevel4;
	uint32 dwAttackGrow4;
	uint32 dwDefenceGrow4;
	uint32 dwHealthGrow4;

	uint32 dwCardIndex5;
	uint16 wLevel5;
	uint32 dwSkillIndex5;
	uint16 wSkillLevel5;
	uint32 dwAttackGrow5;
	uint32 dwDefenceGrow5;
	uint32 dwHealthGrow5;

	uint32 dwCardIndex6;
	uint16 wLevel6;
	uint32 dwSkillIndex6;
	uint16 wSkillLevel6;
	uint32 dwAttackGrow6;
	uint32 dwDefenceGrow6;
	uint32 dwHealthGrow6;
	
	uint32 dwCD;
	uint32 dwFragment1;
	uint32 dwFragment2;
	uint32 dwFragment3;
	uint32 dwFragment4;
	uint32 dwFragment5;
	uint32 dwFragment6;
	uint32 dwPro;
	uint32 dwDevote;
	uint32 dwDevoteGrow;
	uint32 dwTowerLayer;
};			

typedef struct STC_CONF_ROB_ROBOT STC_CONF_ROB_ROBOT;

class CConfRobRobot
{
public:
    typedef std::map<uint32, STC_CONF_ROB_ROBOT> MapData;
    static bool Init(const std::string& strFilePath);
    static STC_CONF_ROB_ROBOT* Find(uint32 dwIndex);
	static std::vector<STC_CONF_ROB_ROBOT *>* GetData(uint32 dwFragment);
	static MapData *GetAllData() {return &m_mapData;}
	static bool Valid(const std::string& strFilePath);
	static std::map<uint32, uint32> * GetRobotLayerMap(){return &robotLayMap;}

private:
    static MapData m_mapData;
	static std::multimap<uint32, STC_CONF_ROB_ROBOT*> m_mapFragment;
	static std::vector<STC_CONF_ROB_ROBOT *> m_vec;
	static std::map<uint32, uint32> robotLayMap;
};

struct STC_CONF_VIP {
	uint32 dwIndex;
	uint32 dwCritExtraLevel;
	uint32 dwDelDMissionNumber;
	uint32 dwEnergyStrengthNumber;
	uint32 dwVipGift;
	uint32 dwAttackBuff;
	uint32 dwExtraSign;
	uint32 dwArenaChallengeNumber;
	uint32 dwHighDelDMissionNumber;
	uint32 dwVipExp;
	uint32 dwLuckMax;
	uint32 dwLuckEveryGet;
	uint32 dwVipRecover;
	uint32 dwVipFriendLimit;
	uint32 dwVipBossAttack;
	uint32 dwDollarExtraRate;
	uint32 dwJoinMax;
	uint32 dwStaminaNumber;
};			

typedef struct STC_CONF_VIP STC_CONF_VIP;

class CConfVip
{
public:
    typedef std::map<uint32, STC_CONF_VIP> MapData;
    static bool Init(const std::string& strFilePath);
    static STC_CONF_VIP* Find(uint32 dwIndex);
    static bool Valid(const std::string& strFilePath);
	static bool ReadConfig(const std::string& strFilePath, MapData &map);
	static uint32 MaxVipExp(uint16_t lv);
	static uint32 MaxVipLv();

private:
    static MapData m_mapData;
};

struct STC_CONF_STRENGTH_CRIT_PRO {
	uint32 dwIndex;
	uint32 dwVip1;
	uint32 dwVip2;
	uint32 dwVip3;
	uint32 dwVip4;
	uint32 dwVip5;
	uint32 dwVip6;
};			

typedef struct STC_CONF_STRENGTH_CRIT_PRO STC_CONF_STRENGTH_CRIT_PRO;

class CConfStrengthCritPro
{
public:
    typedef std::map<uint32, STC_CONF_STRENGTH_CRIT_PRO> MapData;
    static bool Init(const std::string& strFilePath);
    static STC_CONF_STRENGTH_CRIT_PRO* Find(uint32 dwIndex);
    static bool Valid(const std::string& strFilePath);

private:
    static MapData m_mapData;
};

struct STC_CONF_FRIEND_BOSS {
	uint32 dwIndex;
	std::string strName;
	uint8 byType;

	uint32 dwMasterCardIndex;
	uint32 dwMasterSkillIndex;
	uint16 wMasterSkillLevel;
	uint32 dwMasterAttackBase;
	uint32 dwMasterDefenceBase;
	uint32 dwMasterHealthBase;
	uint32 dwMasterAttackGrow;
	uint32 dwMasterDefenceGrow;
	uint32 dwMasterHealthGrow;

	int32 dwDropPro;
	uint32 dwDropItemId;
	uint32 dwGoldCardPro;
	uint32 dwAwakePro;
	uint32 dwAwakeBossId;
	uint32 byBossType;
	uint32 dwBossExp;
	uint32 dwExp;
	uint32 dwCoin;
	uint8 byRare;
	uint8 byBreakCount;

	int32 dwDropCardPro;
	uint32 dwCardId;
	int32 dwDropItemColPro;
	uint32 dwItemCol;
	int32 dwDropCardColPro;
	uint32 dwCardCol;
	uint32 dwBossTime;
	uint32 dwBossLvUpPara;

	uint32 dwSword;
	uint32 dwHand;
	uint32 dwInternalForce;	
	uint32 dwDark;
	uint32 dwStick;
};			

typedef struct STC_CONF_FRIEND_BOSS STC_CONF_FRIEND_BOSS;

class CConfFriendBoss
{
public:
    typedef std::map<uint32, STC_CONF_FRIEND_BOSS> MapData;
    static bool Init(const std::string& strFilePath);
    static STC_CONF_FRIEND_BOSS* Find(uint32 dwIndex);
    static bool Valid(const std::string& strFilePath);
	static bool ReadConfig(const std::string& strFilePath, MapData &map);

private:
    static MapData m_mapData;
};

struct STC_CONF_GACHA {
	uint32 dwIndex;
	uint32 dwNormalPro;
	uint32 dwItemPro;
	uint32 dwDollarPro;
	uint32 dwAwakeBossPro;
	uint32 dwDelLuck;
	uint32 dwFirstGachaPro;
	uint32 dwFinalGachaPro;
	
	uint32 dwVipNormalPro[VIP_MAX];
	uint32 dwVipFirstPro[VIP_MAX];
	uint32 dwChargePro;
};			

typedef struct STC_CONF_GACHA STC_CONF_GACHA;

class CConfGacha
{
public:
    typedef std::map<uint32, STC_CONF_GACHA> MapData;
    static bool Init(const std::string& strFilePath);
	static bool ReadConfig(const std::string &strFilePath, MapData &map);
    static STC_CONF_GACHA* Find(uint32 dwIndex);
	static STC_CONF_GACHA* GetByNormal(uint32 luck);
	static STC_CONF_GACHA* GetByItem(uint32 luck);
	static STC_CONF_GACHA* GetByDollar(uint32 luck);
	static STC_CONF_GACHA* GetByAwakeBoss(uint32 luck);
	static STC_CONF_GACHA* GetByFirstGacha(uint32 luck, std::vector<uint32_t> extraindex);
	static STC_CONF_GACHA* GetByFinalGacha(uint32 luck);

	static STC_CONF_GACHA* GetByChargeGacha();
	static STC_CONF_GACHA* GetByVipFirstGacha(uint16 viplevel);
	static STC_CONF_GACHA* GetByVipNormalGacha(uint16 viplevel, uint32 luck);

    static bool Valid(const std::string& strFilePath);

private:
    static MapData m_mapData;
};

struct STC_CONF_TIMER_GIFT {
	uint32 dwIndex;
	uint32 dwCD;
	uint8 byType;
	uint32 dwPara1;
	uint32 dwPara2;
};			

typedef struct STC_CONF_TIMER_GIFT STC_CONF_TIMER_GIFT;

class CConfTimerGift
{
public:
    typedef std::map<uint32, STC_CONF_TIMER_GIFT> MapData;
    static bool Init(const std::string& strFilePath);
    static STC_CONF_TIMER_GIFT* Find(uint32 dwIndex);
    static bool Valid(const std::string& strFilePath);

private:
    static MapData m_mapData;
};

struct STC_CONF_LOGIN_SIGN {
	uint32 dwIndex;
	uint8 dwMonth;
	uint8 dwCount;
	uint8 byType;
	uint32 dwPara1;
	uint32 dwPara2;
};			

typedef struct STC_CONF_LOGIN_SIGN STC_CONF_LOGIN_SIGN;

class CConfLoginSign
{
public:
    typedef std::map<uint32, STC_CONF_LOGIN_SIGN> MapData;
    static bool Init(const std::string& strFilePath);
    static STC_CONF_LOGIN_SIGN* Find(uint32 dwIndex);
    static bool Valid(const std::string& strFilePath);

private:
    static MapData m_mapData;
};

struct STC_CONF_EXSKILL {
	uint32 dwIndex;
	uint8 byType;
	uint32 dwPara1;
};			

typedef struct STC_CONF_EXSKILL STC_CONF_EXSKILL;

class CConfExskill
{
public:
    typedef std::map<uint32, STC_CONF_EXSKILL> MapData;
    static bool Init(const std::string& strFilePath);
    static STC_CONF_EXSKILL* Find(uint32 dwIndex);
    static bool Valid(const std::string& strFilePath);

private:
    static MapData m_mapData;
};

struct STC_CONF_ACTIVE {
	uint32 dwIndex;
	uint8 byType;
	std::string strName;
	std::string strDesc;
	uint16 wOpr;
	uint32 dwPara;
	std::string strIcon;
	uint32 dwSort;
};			
typedef struct STC_CONF_ACTIVE STC_CONF_ACTIVE;

class CConfActive
{
public:
    typedef std::map<uint32, STC_CONF_ACTIVE> MapData;
    static bool Init(const std::string& strFilePath);
    static STC_CONF_ACTIVE* Find(uint32 dwIndex);
    static bool Valid(const std::string& strFilePath);
	static MapData& GetData();

private:
    static MapData m_mapData;
};

struct STC_CONF_CYCLE_LOGIN {
	uint32 dwIndex;
	uint16 wObj1Type;
	uint32 dwObj1Para1;
	uint32 dwObj1Para2;
	uint16 wObj2Type;
	uint32 dwObj2Para1;
	uint32 dwObj2Para2;
	uint16 wObj3Type;
	uint32 dwObj3Para1;
	uint32 dwObj3Para2;
	uint32 dwNextIndex;
	std::string strDesc;
};				/* ----------  end of struct STC_CONF_CARD_COLLECTION  ---------- */

typedef struct STC_CONF_CYCLE_LOGIN STC_CONF_CYCLE_LOGIN;
class CConfCycleLogin
{
public:
    typedef std::map<uint32, STC_CONF_CYCLE_LOGIN> MapData;
    static bool Init(const std::string& strFilePath);
    static STC_CONF_CYCLE_LOGIN* Find(uint32 dwIndex);
	static bool Valid(const std::string& strFilePath); 
	static bool ReadConfig(const std::string& strFilePath, MapData &map);
	static uint32_t GetFirstRecord();
	static MapData* GetMapData() { return &m_mapData;}

private:
    static MapData m_mapData;
};

struct STC_CONF_VITALITY_PRIZE {
	uint32 dwIndex;
	uint32 dwNeedActive;
	uint16 wObj1Type;
	uint32 dwObj1Para1;
	uint32 dwObj1Para2;
	uint16 wObj2Type;
	uint32 dwObj2Para1;
	uint32 dwObj2Para2;
};			
typedef struct STC_CONF_VITALITY_PRIZE STC_CONF_VITALITY_PRIZE;

class CConfVitalityPrize
{
public:
    typedef std::map<uint32, STC_CONF_VITALITY_PRIZE> MapData;
    static bool Init(const std::string& strFilePath);
	static bool ReadConfig(const std::string& strFilePath, MapData &mapt);
    static STC_CONF_VITALITY_PRIZE* Find(uint32 dwIndex);
    static bool Valid(const std::string& strFilePath);
	static MapData& GetData() { return m_mapData;}

private:
    static MapData m_mapData;
};

struct STC_TOWER_RANK_PRIZE{
	uint32 rank;
	uint32 type;
	uint32 param1;
	uint32 param2;
};

typedef struct STC_TOWER_RANK_PRIZE STC_TOWER_RANK_PRIZE;

class CConfTowerRankPrize{
public :
	typedef std::map<uint32, STC_TOWER_RANK_PRIZE> MapData;
	static bool Init(const std::string& strFilePath);
	static STC_TOWER_RANK_PRIZE* Find(uint32 dwIndex);
	static bool Valid(const std::string& strFilePath);
	static MapData& GetData() {
		return m_mapData;
	}
	private:
	    static MapData m_mapData;
	    static bool ReadConfig(const std::string& strFilePath, MapData &mapt);
};



#endif /* CONFIGDATA_H_ */
