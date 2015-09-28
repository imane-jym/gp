/*
 * =====================================================================================
 *
 *       Filename:  CommonConfig.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2013年05月24日 05时11分37秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  imane (), imanecr@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef _COMMONCONFIG_H_
#define	_COMMONCONFIG_H_ 

#include "Defines.h"
#include <map>
#include <vector>
#include <string>
#include <stdint.h>

typedef struct STC_CONF_DURATION_MISSION
{
	uint32 dwIndex;
	uint32 dwArea;
	uint32 dwOrder;

	std::string strName;
	uint8 byFlag;

	uint32 dwMasterCardIndex;
	uint16 wMasterLevel;
	uint32 dwMasterSkillIndex;
	uint16 wMasterSkillLevel;
	uint32 dwMasterAttack;
	uint32 dwMasterDefence;
	uint32 dwMasterHealth;
	uint8 byMasterBreakCount;

	uint32 dwCardIndex1;
	uint16 wLevel1;
	uint32 dwSkillIndex1;
	uint16 wSkillLevel1;
	uint32 dwAttack1;
	uint32 dwDefence1;
	uint32 dwHealth1;
	uint8 byBreakCount1;

	uint32 dwCardIndex2;
	uint16 wLevel2;
	uint32 dwSkillIndex2;
	uint16 wSkillLevel2;
	uint32 dwAttack2;
	uint32 dwDefence2;
	uint32 dwHealth2;
	uint8 byBreakCount2;

	uint32 dwCardIndex3;
	uint16 wLevel3;
	uint32 dwSkillIndex3;
	uint16 wSkillLevel3;
	uint32 dwAttack3;
	uint32 dwDefence3;
	uint32 dwHealth3;
	uint8 byBreakCount3;

	uint32 dwCardIndex4;
	uint16 wLevel4;
	uint32 dwSkillIndex4;
	uint16 wSkillLevel4;
	uint32 dwAttack4;
	uint32 dwDefence4;
	uint32 dwHealth4;
	uint8 byBreakCount4;

	uint32 dwCardIndex5;
	uint16 wLevel5;
	uint32 dwSkillIndex5;
	uint16 wSkillLevel5;
	uint32 dwAttack5;
	uint32 dwDefence5;
	uint32 dwHealth5;
	uint8 byBreakCount5;

	uint32 dwCardIndex6;
	uint16 wLevel6;
	uint32 dwSkillIndex6;
	uint16 wSkillLevel6;
	uint32 dwAttack6;
	uint32 dwDefence6;
	uint32 dwHealth6;
	uint8 byBreakCount6;
	
	uint16 wEnergyConsume;
	uint16 wChallengeNumber;
	uint32 dwExpUp;
	uint32 dwCardExpUp;
	uint32 dwCoinUp;
	uint32 dwDropRate1; 
	uint32 dwDropCardIndex1;
	uint32 dwDropRate2; 
	uint32 dwDropCardIndex2;
	uint32 dwBossRandomRate;
	uint32 dwBossRandomIndex1;
	uint32 dwBossRandomIndex2;
	uint32 dwBossRandomIndex3;
	uint32 dwBossRandomIndex4;
	uint32 dwBossRandomIndex5;
	uint32 dwBossRandomIndex6;
	uint32 dwBoss;
	uint32 dwNextTaskIndex;

	std::string strContent;

	uint8 byType;
	uint32 wLevelLimit;
	uint32 dwCD;

	uint8  byBackGround;

	uint32 dwFBossPro;
	uint32 dwFBossId;

	uint32 dwStory;
	uint32 dwHighMission;
	uint16 byAllStarType;
	uint32 dwAllStarPara1;
	uint32 dwAllStarPara2;
	
	uint32 dwStoryBossPro;
	uint32 dwStoryBossId;
	uint32 dwSword;
	uint32 dwHand;
	uint32 dwInternalForce;	
	uint32 dwDark;
	uint32 dwStick;
	uint8 byJoin;
	uint32 dwPlusPro;
} STC_CONF_DURATION_MISSION;

class CConfDurationMission
{
public:
    typedef std::map<uint32, STC_CONF_DURATION_MISSION> MapData;
	typedef std::vector<STC_CONF_DURATION_MISSION*> VecData;
	typedef std::map<uint32,VecData> SectionData;
    static bool Init(const std::string& strFilePath);
    static STC_CONF_DURATION_MISSION* Find(uint32 dwIndex);
	static MapData& GetAllData() { return m_mapData; }
	static STC_CONF_DURATION_MISSION *FindChapter( uint32 aAreaID );
#if ( CC_PLATFORM_WIN32 == CC_TARGET_PLATFORM ) || ( CC_PLATFORM_IOS == CC_TARGET_PLATFORM ) || ( CC_PLATFORM_ANDROID == CC_TARGET_PLATFORM )
	static uint32 ChapterCount();
	static uint32 SectionCount( uint32 aAreaId, bool bJingyingFlag );
	static const VecData* SectionVecData( uint32 areaId, bool jy );
	static uint32 SectionCountVisible( uint32 aAreaId, bool bJingyingFlag,uint32 dwTaskIndex,uint32 dwTaskIndexLimit = 0 );
	static STC_CONF_DURATION_MISSION *FindSection( uint32 aAreaID, uint32 aIndex, bool bJingyingFlag );
#else
#endif
	static bool Valid(const std::string& strFilePath);
private:
    static MapData m_mapData;
	static VecData ChapterVec;
	static SectionData SectionMap;
	static SectionData SectionJingyingMap;
};

typedef struct _STC_CONF_CARD_BASE
{
    uint32 dwIndex;
	std::string strName;
	std::string strIntro;
    uint8 byType;
    uint8 byAttribute; 
	uint8 byStarLevel;
	uint32 dwEffectIndex;
	uint32 dwAttackBase;
	uint32 dwAttackGrow;
	uint32 dwDefenceBase;
	uint32 dwDefenceGrow;
	uint32 dwHealthBase;
	uint32 dwHealthGrow;
	uint32 dwExpGrow;
	uint32 dwCritRate;
	uint32 dwSuitIndex;
	uint32 dwMasterSkillIndex;
	uint32 dwSkillIndex;
	uint32 dwGrowLimit;
	uint32 dwLimitBreakNumber;
	uint32 dwLimitBreakPro;
	uint32 dwAwakePro;
	std::string strEffect;
	uint32 dwAddExp;
	uint32 dwConsumeCoin;
	uint32 dwGetCoin;
	uint32 dwBreakExp;
	uint8 byMultiPower;
	uint32 dwMultiTime;
	uint32 dwStrongSkill;
	uint8 bySkillFunc;
	uint32 dwStrongWeapon;
	uint8 byWeaponFunc;
	uint32 dwStrongArmor;
	uint8 byArmorFunc;
	uint32 dwStrongAccessories;
	uint8 byAccessoriesFunc;
	uint32 dwBreakSkill1;
	uint32 dwBreakSkill2;
	uint32 dwBreakSkill3;
	uint32 dwBreakSkill4;
	uint32 dwBreakSkill5;
	std::string strResPath;
	std::string strFullBreakPath; 
	uint8 byIsTujian;
	uint32 dwRelation1;
	uint32 dwRelation2;
	uint32 dwRelation3;
	uint32 dwRelation4;
	uint32 dwLeaderSkill;
	uint32 dwCardGroup;
	uint32 dwMasterIndex;
}STC_CONF_CARD_BASE;
    
class CConfCardBase
{
public:
    typedef std::map<uint32, STC_CONF_CARD_BASE> MapData;
    static bool Init(const std::string& strFilePath);
    static STC_CONF_CARD_BASE* Find(uint32 dwIndex);
	static std::vector<uint32>* GetVecForDuration() { return &m_vecForDuration;}
	static std::vector<uint32>* GetVecOne() {return &m_vecOne;}
	static std::vector<uint32>* GetVecTwo() {return &m_vecTwo;}
    static void GetHandbookData( std::vector<uint32>& pets, std::vector<uint32>& others, std::vector<uint32>& masters );
	static MapData* GetAll() {return &m_mapData;}
	static const STC_CONF_CARD_BASE *FindByGroupId( uint32 dwGroupId );
#if ( CC_PLATFORM_WIN32 == CC_TARGET_PLATFORM ) || ( CC_PLATFORM_IOS == CC_TARGET_PLATFORM ) || ( CC_PLATFORM_ANDROID == CC_TARGET_PLATFORM )
	static uint32 GetRandomPetCardIndex();
#endif
private:
    static MapData m_mapData;
	static std::vector<uint32> m_vecForDuration;
	static std::vector<uint32> m_vecOne;
	static std::vector<uint32> m_vecTwo; 
	static std::vector<uint32> m_vecPetIndex;
};

typedef struct STC_CONF_ITEM_COMBINE
{
	uint32 dwCollectionIndex;
	uint32 dwCardIndex;
	uint32 dwItemNumber;
	std::string strPiecesDesc;
	uint32 dwOpenLevel;

	uint32 dwID1;
	uint32 dwID2;
	uint32 dwID3;
	uint32 dwID4;
	uint32 dwID5;
	uint32 dwID6;
} STC_CONF_ITEM_COMBINE;

class CConfItemCombine
{
public:
    typedef std::map<uint32, STC_CONF_ITEM_COMBINE> MapData;
    typedef std::vector<STC_CONF_ITEM_COMBINE*> VecData;
    static bool Init(const std::string& strFilePath);
//    static std::vector<STC_CONF_ITEM_COMBINE *>* Find(uint32 dwIndex);
    static STC_CONF_ITEM_COMBINE* Find(uint32 dwIndex);
    static uint32 Count( uint16 lv );
    static STC_CONF_ITEM_COMBINE* At( uint32 dwIndex );
	static bool Valid(const std::string& strFilePath);
private:
    static MapData m_mapData;
    static VecData m_vecData;
//	static std::vector<STC_CONF_ITEM_COMBINE *> m_vecData;
};

typedef struct STC_CONF_CARD_SKILL
{
	uint32 dwIndex;
	uint8 byStarLevel;
	std::string strName;
	uint8 byAttribute;
	uint16 wType;
	uint16 wEffectType;
	uint32 dwPara1;
	uint32 dwPro;
	std::string strEffect;	
	std::string strSpecialEffectPath;
} STC_CONF_CARD_SKILL;

class CConfCardSkill
{
public:
    typedef std::map<uint32, STC_CONF_CARD_SKILL> MapData;
    static bool Init(const std::string& strFilePath);
    static STC_CONF_CARD_SKILL* Find(uint32 dwIndex);
	static MapData& GetVal()
	{
		return m_mapData;
	}
	static bool Valid(const std::string& strFilePath);

private:
    static MapData m_mapData;
};

typedef struct STC_CONF_MASTER_BASE
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
} STC_CONF_MASTER_BASE;

class CConfMasterBase
{
public:
    typedef std::map<uint32, STC_CONF_MASTER_BASE> MapData;
    static bool Init(const std::string& strFilePath);
    static STC_CONF_MASTER_BASE* Find(uint32 dwIndex);
	static MapData *GetVal() {return &m_mapData;};
    static void GetHandbookData( std::vector<uint32>& masters );
private:
    static MapData m_mapData;
};

typedef struct STC_CONF_JEWEL_SKILL
{
	uint32 dwIndex;
	uint16 wType;
	int32 para1;
	uint32 dwGrow;
} STC_CONF_JEWEL_SKILL;

class CConfJewelSkill
{
public:
    typedef std::map<uint32, STC_CONF_JEWEL_SKILL> MapData;
    static bool Init(const std::string& strFilePath);
    static STC_CONF_JEWEL_SKILL* Find(uint32 dwIndex);
    static uint32 Count( uint16 lv );
	static bool Valid(const std::string& strFilePath);
private:
    static MapData m_mapData;
};

typedef struct STC_CONF_RELATIONSHIP
{
	uint32 dwIndex;
	std::string strName;
	uint32 dwID1;
	uint32 dwID2;
	uint32 dwID3;
	uint32 dwID4;
	uint32 dwID5;
	uint32 dwID6;
	uint16 wType;
	int32 dwPara1;
	std::string strContent;
} STC_CONF_RELATIONSHIP;

class CConfRelationShip
{
public:
    typedef std::map<uint32, STC_CONF_RELATIONSHIP> MapData;
    static bool Init(const std::string& strFilePath);
	static bool ReadConfig(const std::string& strFilePath, MapData& tmp);
    static STC_CONF_RELATIONSHIP* Find(uint32 dwIndex);
	static std::vector<int> GetRelationshipIndex(const std::vector<int> &vecIndex, const std::vector<int> &vecSkill);
    static bool Valid(const std::string& strFilePath);
private:
    static MapData m_mapData;
};

struct STC_CONF_STRENGTH_CONSUME {
	uint16 wLevel;
	uint32 dwConsume1;
	uint32 dwConsume2;
	uint32 dwConsume3;
	uint32 dwConsume4;
	uint32 dwConsume5;
};			

typedef struct STC_CONF_STRENGTH_CONSUME STC_CONF_STRENGTH_CONSUME;

class CConfStrengthConsume
{
public:
    typedef std::map<uint32, STC_CONF_STRENGTH_CONSUME> MapData;
    static bool Init(const std::string& strFilePath);
    static STC_CONF_STRENGTH_CONSUME* Find(uint16 wLevel);
	static uint32 GetConsume(uint32 dwStar, uint16 level);
    static bool Valid(const std::string& strFilePath);

private:
    static MapData m_mapData;
};

struct STC_CONF_LEADER_SKILL {
	uint32 dwIndex;
	std::string strName;
	std::string strContent;
	uint32 dwStick;
	uint32 dwSword;
	uint32 dwDark;
	uint32 dwPalm;
	uint32 dwInside;
	uint16 wtype1;
	uint32 dwNum1;
	uint32 dwUp1;
	uint16 wtype2;
	uint32 dwNum2;
	uint32 dwUp2;
	uint16 wtype3;
	uint32 dwNum3;
	uint32 dwUp3;
	uint32 dwMaxlv;
	uint16 wPro1;
	uint16 wPro2;
	uint16 wPro3;
	uint16 wPro4;
	uint16 wPro5;
	uint16 wPro6;
	uint16 wPro7;
	uint16 wPro8;
	uint16 wPro9;
};			

typedef struct STC_CONF_LEADER_SKILL STC_CONF_LEADER_SKILL;

class CConfLeaderSkill
{
public:
    typedef std::map<uint32, STC_CONF_LEADER_SKILL> MapData;
    static bool Init(const std::string& strFilePath);
    static STC_CONF_LEADER_SKILL* Find(uint32 dwIndex);
    static bool Valid(const std::string& strFilePath);
#if ( CC_PLATFORM_WIN32 == CC_TARGET_PLATFORM ) || ( CC_PLATFORM_IOS == CC_TARGET_PLATFORM ) || ( CC_PLATFORM_ANDROID == CC_TARGET_PLATFORM )
#else
	static bool IsLevelUp(uint32 index, uint16 level);
#endif
private:
    static MapData m_mapData;
};

struct STC_CONF_ACHIEVE {
	uint32 dwIndex;
	std::string strTitle;
	uint16 wAchieveType;
	uint32 dwMissionEffect;
	uint32 dwMinLevel;
	uint32 dwMaxLevel;
	uint16 wCompeteType;
	uint32 dwCompeteObjId;
	uint32 dwStep;
	uint8 byDisplay;
	uint32 dwCoin;
	uint32 dwDollar;
	uint32 dwStrength;
	uint32 dwEnergy;
	uint32 dwPrestige;
	uint32 dwWorldExp;
	uint32 dwObjType;
	uint32 dwPara1;
	uint32 dwPara2;
	std::string strContent;
	uint32 dwAchieveVal;
	uint32 dwNextAchieveIndex;
	uint32 dwGroupId;
	std::string strGroupName;
	uint32 dwActive;
};			

typedef struct STC_CONF_ACHIEVE STC_CONF_ACHIEVE;

class CConfAchieve
{
public:
    typedef std::map<uint32, STC_CONF_ACHIEVE> MapData;
    static bool Init(const std::string& strFilePath);
	static bool ReadConfig(const std::string &strFilePath, MapData &map);
    static STC_CONF_ACHIEVE* Find(uint32 dwIndex);
	static bool Valid(const std::string& strFilePath);
	static MapData& GetData();
private:
    static MapData m_mapData;
#if ( CC_PLATFORM_WIN32 == CC_TARGET_PLATFORM ) || ( CC_PLATFORM_IOS == CC_TARGET_PLATFORM ) || ( CC_PLATFORM_ANDROID == CC_TARGET_PLATFORM )
public:
	typedef std::map<uint8,std::vector<STC_CONF_ACHIEVE*> > MapVecData;
	static MapVecData& GetAllAchieveData() { return m_mapVecData; }
private:
	static MapVecData m_mapVecData;
#endif
};

struct STC_CONF_CARD_UP {
	uint16 wLevel;
	uint32 dwExpLimit1;
	uint32 dwExpLimit2;
	uint32 dwExpLimit3;
	uint32 dwExpLimit4;
	uint32 dwExpLimit5;
	uint32 dwExpLimit6;
	uint32 dwExpLimit7;
};			

typedef struct STC_CONF_CARD_UP STC_CONF_CARD_UP;

class CConfCardUp
{
public:
    typedef std::map<uint32, STC_CONF_CARD_UP> MapData;
    static bool Init(const std::string& strFilePath);
    static STC_CONF_CARD_UP* Find(uint16 wLevel);
	static uint16 FindLevel(uint32 dwStar, uint32 dwExp, uint32 dwExpGrow);
	static uint32 GetMaxExp(uint32 dwStar, uint32 grow, uint16 maxlevel);
	static uint32 GetExp(uint32 dwStar, uint32 grow, uint16 level);

private:
    static MapData m_mapData;
	static std::map<uint32, std::vector<uint32> > m_mapExpToLevel;
};

struct STC_CONF_CARD_UPGRADE
{
	uint32 dwIndex;
	uint32 dwCardIndex;
	uint32 dwLevelLimit;
	uint16 wType[5];
	uint32 dwPara1[5];
	uint32 dwPara2[5];
	uint32 dwCardIndexDesc;
};
typedef struct STC_CONF_CARD_UPGRADE STC_CONF_CARD_UPGRADE;


class CConfUpgrade
{
public:
    typedef std::map<uint32, STC_CONF_CARD_UPGRADE> MapData;
	typedef struct SCard
	{
		int objid;
		int lv;
	} SCard;
	static bool SCardComp(SCard i, SCard j) {return i.lv < j.lv;};

    static bool Init(const std::string& strFilePath);
    static bool Valid(const std::string& strFilePath);
    static bool ReadConfig(const std::string& strFilePath, MapData &map);
    static STC_CONF_CARD_UPGRADE* Find(uint32 dwIndex);
	static bool Check(std::map<uint32, std::vector<SCard> > &mapCard, std::map<uint32_t, uint32_t> &mapItem, uint32 index); 
	static MapData &GetMapData(){return m_mapData;}
	static STC_CONF_CARD_UPGRADE*FindByCardIndex(uint32 dwCardIndex);
private:
    static MapData m_mapData;
};

typedef struct STC_CONF_GOODS
{
	uint32 dwIndex;
	uint32 dwFuncType;
	uint32 dwPara1;
	uint32 dwPara2;
	uint32 dwPara3;
	uint32 dwPara4;
	uint32 dwLevelLimit;
	std::string strTitle;
	std::string strContent;
	std::string strPath;
	uint8 byIsSell;
	uint32 dwSellNumber;
} STC_CONF_GOODS;

class CConfGoods
{
public:
    typedef std::map<uint32, STC_CONF_GOODS> MapData;
	static bool ReadConfig(const std::string& strFilePath, MapData &map);
    static bool Init(const std::string& strFilePath);
    static STC_CONF_GOODS* Find(uint32 dwIndex);
    static MapData* GetVal();
	static bool Valid(const std::string& strFilePath);
private:
    static MapData m_mapData;
};

typedef struct STC_CONF_TRIAL
{
	uint32 dwIndex;
	std::string strName;
	uint8 byFloor;
	uint16 wLevelLimit;
	uint32 dwChallengeNumber;
	uint16 wConsume;
	uint32 dwRoleExpNeed;
	uint32 dwCardExpNeed;
	uint32 dwCoinNeed;
	uint16 wPro;
	uint16 wPro1;
	uint32 dwBossId;
	uint16 wPro2;
	uint32 dwCoin;
	uint16 wPro3;
	uint32 dwRoleExp;
	uint16 wPro4;
	uint32 dwCardExp;
	uint16 wPro5;
	uint32 dwWorldExp;
	uint16 wPro6;
	uint32 dwItemCollection;
	uint16 wPro7;
	uint32 dwPiecesCollection;
	uint16 wPro8;
	uint32 dwCardCollection;
	uint16 wPro9;
	uint32 dwEnergy;
	uint16 wPro10;
	uint32 dwLotEnergy;
	uint16 wPro11;
	uint32 dwStrength;
	uint16 wPro12;
	uint32 dwLotStrength;
	uint16 wPro13;
	uint32 dwStamina;
	uint16 wPro14;
	uint32 dwLotStamina;
	uint32 dwTrieTime;
	std::string strBackgroundPath;
	uint32 dwPro;
	uint32 dwNextTrial;
	uint16 wPlusPro;
	uint32 wPro15;
} STC_CONF_TRIAL;

class CConfTrial
{
public:
    typedef std::map<uint32, STC_CONF_TRIAL> MapData;
	static bool ReadConfig(const std::string& strFilePath, MapData &map);
    static bool Init(const std::string& strFilePath);
    static STC_CONF_TRIAL* Find(uint32 dwIndex);
    static MapData* GetVal();
	static bool Valid(const std::string& strFilePath);
	static STC_CONF_TRIAL* FindOfFirstTrial();
private:
    static MapData m_mapData;
};

typedef struct STC_CONF_TOWER{
	uint32 dwIndex;
	uint32 layer;//层
	uint32 objectId;//侠客id
	uint32 dwLevel;//侠客等级
	uint32 dwSkill;//侠客技能
	uint32 dwSkillLevel;//侠客技能等级
	uint32 attack;//攻击加成
	uint32 defend;//防御加成
	uint32 health;//生命加成
	uint32 dollarReward;//元宝奖励
	uint32 coinReward;//金钱奖励
	uint32 propReward;//道具包
	uint32 cardReward;//卡牌包
	uint32 worldExpReward;//阅历奖励
	uint32 dwSwordEffect;//剑系特效
	uint32 dwHandEffect;//掌系特效
	uint32 dwInternalForceEffect;//内系特效
	uint32 dwDarkEffect;//暗系特效
	uint32 dwStickEffect;//棍系特效
	uint32 nextLayer;//下一层ID
	uint32 failureCardReward;
	uint32 failureDollarReward;
	uint32 failureCoinReward;
	uint32 failurePropReward;
	uint32 failureWorldExpReward;
//	uint32 successCardReward;
//	uint32 successDollarReward;
//	uint32 successCoinReward;
//	uint32 successPropReward;
//	uint32 successWorldExpReward;
}STC_CONF_TOWER;

class CConfTower{
public :
	static bool init(const std::string &strPath);
	static STC_CONF_TOWER* find(uint32 dwIndex);
	static std::map<uint32,STC_CONF_TOWER>* getMap();
	static bool valid(const std::string &strPath);
	static std::vector<uint32> *  findLayerObjects(uint32 layer);//返回里存放index
	//static uint32  getNextDropLayer(uint32 layer);//获取下个掉落物品的层数
private:
	static std::map<uint32,std::vector<uint32> > layerMap;
	static std::map<uint32,STC_CONF_TOWER> mapData;
	static bool readConfig(const std::string &strPath,std::map<uint32,STC_CONF_TOWER>& mapData);
	//static std::vector<uint32> dropLayer;
};

//typedef struct STC_CONF_VITALITY
//{
//	uint32 dwIndex;
//	std::string strName;
//	uint32 dwType;
//	uint32 dwNeedLevel;
//	uint32 dwNeedStep;
//	std::string strDesc;
//	uint32 dwAwardVitality;
//} STC_CONF_VITALITY;
//
//class CConfVitality
//{
//public:
//	static bool Init( const std::string &strPath );
//	static STC_CONF_VITALITY *find( uint32 dwIndex );
//	typedef std::map<uint32, STC_CONF_VITALITY> MapData;
//	static uint32 count();
//	static MapData m_Datas;
//#if ( CC_PLATFORM_WIN32 == CC_TARGET_PLATFORM ) || ( CC_PLATFORM_IOS == CC_TARGET_PLATFORM ) || ( CC_PLATFORM_ANDROID == CC_TARGET_PLATFORM )
//	typedef std::vector<STC_CONF_VITALITY> VecData;
//	static uint32 VecCount();
//	static const STC_CONF_VITALITY *at( uint32 aIndex );
//	static VecData m_vecs; 
//#endif
//};
#endif
