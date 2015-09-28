/*
 * =====================================================================================
 *
 *       Filename:  CommonConfig.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2013年05月24日 05时11分06秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  imane (), imanecr@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include "CommonConfig.h"

#include "ConfigReader.h"
#include <stdlib.h>
#include <vector>
#include <map>
#include <algorithm>
#include <string>
#include "EnumDefines.h"
#include "Log.h"

#if ( CC_PLATFORM_WIN32 == CC_TARGET_PLATFORM ) || ( CC_PLATFORM_IOS == CC_TARGET_PLATFORM ) || ( CC_PLATFORM_ANDROID == CC_TARGET_PLATFORM )
#else
#include "Util.h"
#endif

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

// duration mission
CConfDurationMission::MapData CConfDurationMission::m_mapData;
CConfDurationMission::VecData CConfDurationMission::ChapterVec;
CConfDurationMission::SectionData CConfDurationMission::SectionMap;
CConfDurationMission::SectionData CConfDurationMission::SectionJingyingMap;

bool CConfDurationMission::Init(const std::string& strFilePath )
{
	m_mapData.clear();
	ChapterVec.clear();
	SectionMap.clear();
	SectionJingyingMap.clear();

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
		STC_CONF_DURATION_MISSION info;
		READER_INT(info.dwIndex);
		READER_INT(info.dwArea);
		READER_INT(info.dwOrder);
		READER_STR(info.strName);
		READER_INT(info.byFlag);
		READER_INT(info.dwMasterCardIndex);
		READER_INT(info.wMasterLevel);
		READER_INT(info.dwMasterSkillIndex);
		READER_INT(info.wMasterSkillLevel);
		READER_INT(info.dwMasterAttack);
		READER_INT(info.dwMasterDefence);
		READER_INT(info.dwMasterHealth);
		READER_INT(info.byMasterBreakCount);

		READER_INT(info.dwCardIndex1);
		READER_INT(info.wLevel1);
		READER_INT(info.dwSkillIndex1);
		READER_INT(info.wSkillLevel1);
		READER_INT(info.dwAttack1);
		READER_INT(info.dwDefence1);
		READER_INT(info.dwHealth1);
		READER_INT(info.byBreakCount1);

		READER_INT(info.dwCardIndex2);
		READER_INT(info.wLevel2);
		READER_INT(info.dwSkillIndex2);
		READER_INT(info.wSkillLevel2);
		READER_INT(info.dwAttack2);
		READER_INT(info.dwDefence2);
		READER_INT(info.dwHealth2);
		READER_INT(info.byBreakCount2);

		READER_INT(info.dwCardIndex3);
		READER_INT(info.wLevel3);
		READER_INT(info.dwSkillIndex3);
		READER_INT(info.wSkillLevel3);
		READER_INT(info.dwAttack3);
		READER_INT(info.dwDefence3);
		READER_INT(info.dwHealth3);
		READER_INT(info.byBreakCount3);

		READER_INT(info.dwCardIndex4);
		READER_INT(info.wLevel4);
		READER_INT(info.dwSkillIndex4);
		READER_INT(info.wSkillLevel4);
		READER_INT(info.dwAttack4);
		READER_INT(info.dwDefence4);
		READER_INT(info.dwHealth4);
		READER_INT(info.byBreakCount4);

		READER_INT(info.dwCardIndex5);
		READER_INT(info.wLevel5);
		READER_INT(info.dwSkillIndex5);
		READER_INT(info.wSkillLevel5);
		READER_INT(info.dwAttack5);
		READER_INT(info.dwDefence5);
		READER_INT(info.dwHealth5);
		READER_INT(info.byBreakCount5);
		
		READER_INT(info.dwCardIndex6);
		READER_INT(info.wLevel6);
		READER_INT(info.dwSkillIndex6);
		READER_INT(info.wSkillLevel6);
		READER_INT(info.dwAttack6);
		READER_INT(info.dwDefence6);
		READER_INT(info.dwHealth6);
		READER_INT(info.byBreakCount6);

		READER_INT(info.wEnergyConsume);
		READER_INT(info.wChallengeNumber);
		READER_INT(info.dwExpUp);
		READER_INT(info.dwCardExpUp);
		READER_INT(info.dwCoinUp);
		READER_INT(info.dwDropRate1);
		READER_INT(info.dwDropCardIndex1);
		READER_INT(info.dwDropRate2);
		READER_INT(info.dwDropCardIndex2);
		READER_INT(info.dwBossRandomRate);
		READER_INT(info.dwBossRandomIndex1);
		READER_INT(info.dwBossRandomIndex2);
		READER_INT(info.dwBossRandomIndex3);
		READER_INT(info.dwBossRandomIndex4);
		READER_INT(info.dwBossRandomIndex5);
		READER_INT(info.dwBossRandomIndex6);
		READER_INT(info.dwBoss);
		READER_INT(info.dwNextTaskIndex);
		READER_STR(info.strContent);
		READER_INT(info.byType);
		READER_INT(info.wLevelLimit);
		READER_INT(info.dwCD);
		READER_INT(info.byBackGround);
		READER_INT(info.dwFBossPro);
		READER_INT(info.dwFBossId);
		READER_INT(info.dwStory);
		READER_INT(info.dwHighMission);

		READER_INT(info.byAllStarType);
		READER_INT(info.dwAllStarPara1);
		READER_INT(info.dwAllStarPara2);
		READER_INT(info.dwStoryBossPro);
		READER_INT(info.dwStoryBossId);

		READER_INT(info.dwSword);
		READER_INT(info.dwHand);
		READER_INT(info.dwInternalForce);
		READER_INT(info.dwDark);
		READER_INT(info.dwStick);
		READER_INT(info.byJoin);
		READER_INT(info.dwPlusPro);
		IS_READER_ERROR;
		
		std::vector<uint32> vectmp;
		vectmp.push_back(info.dwCardIndex1);
		vectmp.push_back(info.dwCardIndex2);
		vectmp.push_back(info.dwCardIndex3);
		vectmp.push_back(info.dwCardIndex4);
		vectmp.push_back(info.dwCardIndex5);
		vectmp.push_back(info.dwCardIndex6);
		for (int i = 0; i < vectmp.size(); i++)
		{
			if (vectmp[i] == 0)
				continue;
			if (CConfCardBase::Find(vectmp[i]) == NULL)
			{
				IME_ERROR("area index %u can not find card index %u", info.dwIndex, vectmp[i]);
				return false;
			}	
		}

//		IME_LOG("index %u, highmission %u", info.dwIndex, info.dwHighMission);
		if (m_mapData.insert(std::make_pair(info.dwIndex, info)).second == false)
		{
			IME_ERROR("ERROR CONF DUPLICATE [ChestId=%u]", info.dwIndex);
			return false;
		}
		if(!info.byFlag){
			ChapterVec.push_back(&m_mapData[info.dwIndex]);
		}else{
			if(info.byType==E_MISSION_HIGH){
				SectionJingyingMap[info.dwArea].push_back(&m_mapData[info.dwIndex]);
			}else if(info.byType==E_MISSION_MAIN){
				SectionMap[info.dwArea].push_back(&m_mapData[info.dwIndex]);
			}
		}
	}
	MapData::iterator it = m_mapData.begin();
	for (; it != m_mapData.end(); it++)
	{
		if (it->second.dwNextTaskIndex != 0)
		{
			if (Find(it->second.dwNextTaskIndex) == NULL)
			{
				IME_ERROR("can not find next task index, record index %u", it->first);
				return false;
			}
		}
		if (it->second.dwHighMission != 0)
		{
			if (Find(it->second.dwHighMission) == NULL)
			{
				IME_ERROR("can not find next highmission limit index, record index %u", it->first);
				return false;
			}
		}
	}

	IME_LOG("LOAD CONF duration mission para [Size=%d]", m_mapData.size());
	return true;
}

STC_CONF_DURATION_MISSION* CConfDurationMission::Find(uint32 dwIndex)
{
	MapData::iterator it = m_mapData.find(dwIndex);
	return it == m_mapData.end() ? NULL : &it->second;
}

STC_CONF_DURATION_MISSION*CConfDurationMission::FindChapter(uint32 aAreaID){
	if(aAreaID<1||aAreaID>ChapterVec.size()){
		return NULL;
	}else{
		return ChapterVec[aAreaID-1];
	}
}


#if ( CC_PLATFORM_WIN32 == CC_TARGET_PLATFORM ) || ( CC_PLATFORM_IOS == CC_TARGET_PLATFORM ) || ( CC_PLATFORM_ANDROID == CC_TARGET_PLATFORM )
const CConfDurationMission::VecData* CConfDurationMission::SectionVecData( uint32 areaId, bool jy )
{
	if ( !jy )
	{
		SectionData::iterator it = SectionMap.find( areaId );
		return it == SectionMap.end() ? NULL : &( it->second );
	}
	else
	{
		SectionData::iterator it = SectionJingyingMap.find( areaId );
		return it == SectionJingyingMap.end() ? NULL : &( it->second );
	}
}

uint32 CConfDurationMission::SectionCount( uint32 aAreaId, bool bJingyingFlag )
{
	if ( !bJingyingFlag )
	{
		SectionData::iterator it = SectionMap.find( aAreaId );
		return it == SectionMap.end() ? 0 : it->second.size();
	} 
	else 
	{
		SectionData::iterator it=SectionJingyingMap.find(aAreaId);
		return it == SectionJingyingMap.end() ? 0 :it->second.size();
	}
}

uint32 CConfDurationMission::SectionCountVisible(uint32 aAreaId,bool bJingyingFlag,uint32 dwTaskIndex,uint32 dwTaskIndexLimit){
	if(!bJingyingFlag){
		CC_UNUSED_PARAM(dwTaskIndexLimit);
		SectionData::iterator it=SectionMap.find(aAreaId);
		uint32 dwCount=0;
		if(it!=SectionMap.end()){
			VecData::iterator itt=it->second.begin();
			uint32 dwCount=0;
			for(;itt!=it->second.end();itt++){
				dwCount++;
				if((*itt)->dwIndex==dwTaskIndex){
					return dwCount;
				}
			}
		}
		return dwCount;
	}else{
		SectionData::iterator it=SectionJingyingMap.find(aAreaId);
		uint32 dwCount=0;
		if(it!=SectionJingyingMap.end()){
			VecData::iterator itt=it->second.begin();
			for(;itt!=it->second.end();itt++){
				uint32 dwTaskLimit=dwTaskIndexLimit;
				if((*itt)->dwIndex<=dwTaskLimit){
					dwCount++;
				}
				if((*itt)->dwIndex==dwTaskIndex){
					return dwCount;
				}
			}
		}
		return dwCount;
	}
}

uint32 CConfDurationMission::ChapterCount(){
	return ChapterVec.size();
}

STC_CONF_DURATION_MISSION*CConfDurationMission::FindSection(uint32 aAreaID,uint32 aIndex,bool bJingyingFlag){
	if(!bJingyingFlag){
		SectionData::iterator it = SectionMap.find( aAreaID );
		if ( it == SectionMap.end() ) {
			return NULL;
		}
		if ( aIndex >= it->second.size() ) {
			return NULL;
		}
		return it->second.at( aIndex );
	}else{
		SectionData::iterator it = SectionJingyingMap.find( aAreaID );
		if ( it == SectionJingyingMap.end() ) {
			return NULL;
		}
		if ( aIndex >= it->second.size() ) {
			return NULL;
		}
		return it->second.at( aIndex );
	}
}

#else
#endif
bool CConfDurationMission::Valid(const std::string& strFilePath )
{
    MapData m_mapData;
	VecData ChapterVec;
	SectionData SectionMap;
	SectionData SectionJingyingMap;

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
		STC_CONF_DURATION_MISSION info;
		READER_INT(info.dwIndex);
		READER_INT(info.dwArea);
		READER_INT(info.dwOrder);
		READER_STR(info.strName);
		READER_INT(info.byFlag);
		READER_INT(info.dwMasterCardIndex);
		READER_INT(info.wMasterLevel);
		READER_INT(info.dwMasterSkillIndex);
		READER_INT(info.wMasterSkillLevel);
		READER_INT(info.dwMasterAttack);
		READER_INT(info.dwMasterDefence);
		READER_INT(info.dwMasterHealth);
		READER_INT(info.byMasterBreakCount);

		READER_INT(info.dwCardIndex1);
		READER_INT(info.wLevel1);
		READER_INT(info.dwSkillIndex1);
		READER_INT(info.wSkillLevel1);
		READER_INT(info.dwAttack1);
		READER_INT(info.dwDefence1);
		READER_INT(info.dwHealth1);
		READER_INT(info.byBreakCount1);

		READER_INT(info.dwCardIndex2);
		READER_INT(info.wLevel2);
		READER_INT(info.dwSkillIndex2);
		READER_INT(info.wSkillLevel2);
		READER_INT(info.dwAttack2);
		READER_INT(info.dwDefence2);
		READER_INT(info.dwHealth2);
		READER_INT(info.byBreakCount2);

		READER_INT(info.dwCardIndex3);
		READER_INT(info.wLevel3);
		READER_INT(info.dwSkillIndex3);
		READER_INT(info.wSkillLevel3);
		READER_INT(info.dwAttack3);
		READER_INT(info.dwDefence3);
		READER_INT(info.dwHealth3);
		READER_INT(info.byBreakCount3);

		READER_INT(info.dwCardIndex4);
		READER_INT(info.wLevel4);
		READER_INT(info.dwSkillIndex4);
		READER_INT(info.wSkillLevel4);
		READER_INT(info.dwAttack4);
		READER_INT(info.dwDefence4);
		READER_INT(info.dwHealth4);
		READER_INT(info.byBreakCount4);

		READER_INT(info.dwCardIndex5);
		READER_INT(info.wLevel5);
		READER_INT(info.dwSkillIndex5);
		READER_INT(info.wSkillLevel5);
		READER_INT(info.dwAttack5);
		READER_INT(info.dwDefence5);
		READER_INT(info.dwHealth5);
		READER_INT(info.byBreakCount5);
		
		READER_INT(info.dwCardIndex6);
		READER_INT(info.wLevel6);
		READER_INT(info.dwSkillIndex6);
		READER_INT(info.wSkillLevel6);
		READER_INT(info.dwAttack6);
		READER_INT(info.dwDefence6);
		READER_INT(info.dwHealth6);
		READER_INT(info.byBreakCount6);

		READER_INT(info.wEnergyConsume);
		READER_INT(info.wChallengeNumber);
		READER_INT(info.dwExpUp);
		READER_INT(info.dwCardExpUp);
		READER_INT(info.dwCoinUp);
		READER_INT(info.dwDropRate1);
		READER_INT(info.dwDropCardIndex1);
		READER_INT(info.dwDropRate2);
		READER_INT(info.dwDropCardIndex2);
		READER_INT(info.dwBossRandomRate);
		READER_INT(info.dwBossRandomIndex1);
		READER_INT(info.dwBossRandomIndex2);
		READER_INT(info.dwBossRandomIndex3);
		READER_INT(info.dwBossRandomIndex4);
		READER_INT(info.dwBossRandomIndex5);
		READER_INT(info.dwBossRandomIndex6);
		READER_INT(info.dwBoss);
		READER_INT(info.dwNextTaskIndex);
		READER_STR(info.strContent);
		READER_INT(info.byType);
		READER_INT(info.wLevelLimit);
		READER_INT(info.dwCD);
		READER_INT(info.byBackGround);
		READER_INT(info.dwFBossPro);
		READER_INT(info.dwFBossId);
		READER_INT(info.dwStory);
		READER_INT(info.dwHighMission);

		READER_INT(info.byAllStarType);
		READER_INT(info.dwAllStarPara1);
		READER_INT(info.dwAllStarPara2);
		READER_INT(info.dwStoryBossPro);
		READER_INT(info.dwStoryBossId);

		READER_INT(info.dwSword);
		READER_INT(info.dwHand);
		READER_INT(info.dwInternalForce);
		READER_INT(info.dwDark);
		READER_INT(info.dwStick);
		READER_INT(info.byJoin);
		IS_READER_ERROR;
//		IME_LOG("index %u, highmission %u", info.dwIndex, info.dwHighMission);
		if (m_mapData.insert(std::make_pair(info.dwIndex, info)).second == false)
		{
			IME_ERROR("ERROR CONF DUPLICATE [ChestId=%u]", info.dwIndex);
			return false;
		}
		if(!info.byFlag){
			ChapterVec.push_back(&m_mapData[info.dwIndex]);
		}else{
			if(info.byType){
				SectionJingyingMap[info.dwArea].push_back(&m_mapData[info.dwIndex]);
			}else{
				SectionMap[info.dwArea].push_back(&m_mapData[info.dwIndex]);
			}
		}
	}

	IME_LOG("LOAD CONF duration mission para [Size=%d]", m_mapData.size());
	return true;
}


///////////////////////////////////////////////
CConfCardBase::MapData CConfCardBase::m_mapData;
std::vector<uint32> CConfCardBase::m_vecForDuration;
std::vector<uint32> CConfCardBase::m_vecOne;
std::vector<uint32> CConfCardBase::m_vecTwo;
std::vector<uint32> CConfCardBase::m_vecPetIndex;
bool CConfCardBase::Init( const std::string& strFilePath )
{
	m_mapData.clear();
	m_vecForDuration.clear();
	m_vecOne.clear();
	m_vecTwo.clear();
	m_vecPetIndex.clear();

	CConfigReader reader;
	if (!reader.init(strFilePath))
	{
		return false;
	}
	int nRow = reader.rowCount();
	for (int i = 1; i < nRow; ++i)
	{
		int j = 0;
		STC_CONF_CARD_BASE info;
		READER_INT(info.dwIndex);

		READER_STR(info.strName);
		READER_STR(info.strIntro);

		READER_INT(info.byType);
		READER_INT(info.byAttribute);
		READER_INT(info.byStarLevel);
		READER_INT(info.dwEffectIndex);	
		READER_INT(info.dwAttackBase);	
		READER_INT(info.dwAttackGrow);	
		READER_INT(info.dwDefenceBase);	
		READER_INT(info.dwDefenceGrow);	
		READER_INT(info.dwHealthBase);	
		READER_INT(info.dwHealthGrow);	
		READER_INT(info.dwExpGrow);	
		READER_INT(info.dwCritRate);	
		READER_INT(info.dwSuitIndex);	
		READER_INT(info.dwMasterSkillIndex);	
		READER_INT(info.dwSkillIndex);	
		READER_INT(info.dwGrowLimit);	
		READER_INT(info.dwLimitBreakNumber);	
		READER_INT(info.dwLimitBreakPro);	
		READER_INT(info.dwAwakePro);	

		READER_STR(info.strEffect);

		READER_INT(info.dwAddExp);	
		READER_INT(info.dwConsumeCoin);	
		READER_INT(info.dwGetCoin);	
		READER_INT(info.dwBreakExp);	
		READER_INT(info.byMultiPower);	
		std::string str;
		READER_STR(str);	
		str = str + " 23:59:59";
        struct tm tmptm;
#if CC_PLATFORM_WIN32 == CC_TARGET_PLATFORM
		int year, mon, day;
		int h, m, s;
		sscanf( str.c_str(), "%4d%2d%2d %2d:%2d:%2d", &year, &mon, &day, &h, &m, &s );
		tmptm.tm_year = year - 1900;
		tmptm.tm_mon = mon - 1;
		tmptm.tm_mday = day;
		tmptm.tm_hour = h;
		tmptm.tm_min = m;
		tmptm.tm_sec = s;
		tmptm.tm_isdst = 0;
#else
		strptime( str.c_str(), "%Y%m%d %H:%M:%S", &tmptm );
#endif
		info.dwMultiTime = mktime(&tmptm);
		READER_INT(info.dwStrongSkill);
		READER_INT(info.bySkillFunc);
		READER_INT(info.dwStrongWeapon);
		READER_INT(info.byWeaponFunc);
		READER_INT(info.dwStrongArmor);
		READER_INT(info.byArmorFunc);
		READER_INT(info.dwStrongAccessories);
		READER_INT(info.byAccessoriesFunc);
		READER_INT(info.dwBreakSkill1);
		READER_INT(info.dwBreakSkill2);
		READER_INT(info.dwBreakSkill3);
		READER_INT(info.dwBreakSkill4);
		READER_INT(info.dwBreakSkill5);

		READER_STR(info.strResPath);
		READER_STR(info.strFullBreakPath);
		READER_INT(info.byIsTujian);
		READER_INT(info.dwRelation1);
		READER_INT(info.dwRelation2);
		READER_INT(info.dwRelation3);
		READER_INT(info.dwRelation4);
		READER_INT(info.dwLeaderSkill);
		READER_INT(info.dwCardGroup);
		READER_INT(info.dwMasterIndex);

		IS_READER_ERROR;

		if ((info.byType == E_ITEM_WEAPON_CARD || info.byType == E_ITEM_ARMOR_CARD) && info.byStarLevel <= 3)
		{
			m_vecForDuration.push_back(info.dwIndex);	
		}
		if (info.byType == E_ITEM_PET_CARD && info.byStarLevel == 1)
		{
			m_vecOne.push_back(info.dwIndex);
		}
		if (info.byType == E_ITEM_PET_CARD && info.byStarLevel == 2)
		{
			m_vecTwo.push_back(info.dwIndex);
		}
		if (info.byType==E_ITEM_PET_CARD || info.byType==E_ITEM_MASTER_CARD){
			m_vecPetIndex.push_back(info.dwIndex);
		}

		if (m_mapData.insert(std::make_pair(info.dwIndex, info)).second == false)
		{
			IME_ERROR("ERROR CONF DUPLICATE [ChestId=%u]", info.dwIndex);
			return false;
		}
	}

	IME_LOG("LOAD CONF CARD [Size=%d]", m_mapData.size());

	return true;
}
#if ( CC_PLATFORM_WIN32 == CC_TARGET_PLATFORM ) || ( CC_PLATFORM_IOS == CC_TARGET_PLATFORM ) || ( CC_PLATFORM_ANDROID == CC_TARGET_PLATFORM )
uint32 CConfCardBase::GetRandomPetCardIndex(){
	time_t timeep;
	time(&timeep);
	srand(unsigned(timeep));
	uint32 dwIndex=rand()%m_vecPetIndex.size();
	if( dwIndex<m_vecPetIndex.size() ) {
		return m_vecPetIndex[dwIndex];
	}
    return m_vecPetIndex[0];
}
#endif
void CConfCardBase::GetHandbookData( std::vector<uint32>& pets, std::vector<uint32>& others, std::vector<uint32>& masters)
{
    pets.clear();
    others.clear();
	masters.clear();
    MapData::iterator it = m_mapData.begin();
    MapData::iterator itEnd = m_mapData.end();
    for ( ; it != itEnd; ++it )
    {
		if ( !it->second.byIsTujian ) {
			continue;
		}
        if ( it->second.byType == E_ITEM_PET_CARD )
        {
            pets.push_back( it->second.dwIndex );
        }
        else if( it->second.byType==E_ITEM_MASTER_CARD)
        {
            masters.push_back( it->second.dwIndex );
        }else{
			others.push_back( it->second.dwIndex );
		}
    }
}

STC_CONF_CARD_BASE* CConfCardBase::Find(uint32 dwIndex)
{
	MapData::iterator it = m_mapData.find(dwIndex);
	return it == m_mapData.end() ? NULL : &it->second;
}

const STC_CONF_CARD_BASE *CConfCardBase::FindByGroupId( uint32 dwGroupId ) {
    if ( dwGroupId == 0 ) {
        return NULL;
    }
	MapData::const_iterator it = m_mapData.begin();
	for ( ; it != m_mapData.end(); ++it ) {
		if ( it->second.dwCardGroup == dwGroupId ) {
			return &it->second;
		}
	}
	return NULL;
}

// card item combine 
CConfItemCombine::MapData CConfItemCombine::m_mapData;
CConfItemCombine::VecData CConfItemCombine::m_vecData;
//std::vector<STC_CONF_ITEM_COMBINE *> CConfItemCombine::m_vecData;
bool CConfItemCombine::Init( const std::string& strFilePath )
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
		STC_CONF_ITEM_COMBINE info;
		READER_INT(info.dwCollectionIndex);
		READER_INT(info.dwCardIndex);
		READER_INT(info.dwItemNumber);
		READER_STR(info.strPiecesDesc);
		READER_INT(info.dwOpenLevel);

		READER_INT(info.dwID1);
		READER_INT(info.dwID2);
		READER_INT(info.dwID3);
		READER_INT(info.dwID4);
		READER_INT(info.dwID5);
		READER_INT(info.dwID6);
		IS_READER_ERROR;

		m_mapData.insert(std::make_pair(info.dwCollectionIndex, info));
        m_vecData.push_back( &m_mapData[info.dwCollectionIndex] );
	}

	IME_LOG("LOAD CONF ITEM [Size=%d]", m_mapData.size());

	return true;
}

STC_CONF_ITEM_COMBINE* CConfItemCombine::Find(uint32 dwIndex)
{
	MapData::iterator it = m_mapData.find(dwIndex);
	return it == m_mapData.end() ? NULL : &it->second;
}

STC_CONF_ITEM_COMBINE* CConfItemCombine::At( uint32 dwIndex ) {
    if ( dwIndex >= m_vecData.size() ) {
        return NULL;
    }
    return m_vecData[dwIndex];
}

uint32 CConfItemCombine::Count(uint16 wLevel) {
	uint32 wNum=0;
	for( MapData::iterator ii=m_mapData.begin();ii!=m_mapData.end();ii++){
		if( wLevel >= ii->second.dwOpenLevel ){
			++wNum;
		}
	}
	return wNum;
    //	return m_vecData.size();
}

bool CConfItemCombine::Valid( const std::string& strFilePath )
{
    MapData m_mapData;
    VecData m_vecData;

	CConfigReader reader;
	if (!reader.init(strFilePath))
	{
		return false;
	}
	int nRow = reader.rowCount();
	for (int i = 1; i < nRow; ++i)
	{
		int j = 0;
		STC_CONF_ITEM_COMBINE info;
		READER_INT(info.dwCollectionIndex);
		READER_INT(info.dwCardIndex);
		READER_INT(info.dwItemNumber);
		READER_STR(info.strPiecesDesc);
		READER_INT(info.dwOpenLevel);

		READER_INT(info.dwID1);
		READER_INT(info.dwID2);
		READER_INT(info.dwID3);
		READER_INT(info.dwID4);
		READER_INT(info.dwID5);
		READER_INT(info.dwID6);
		IS_READER_ERROR;

		m_mapData.insert(std::make_pair(info.dwCollectionIndex, info));
        m_vecData.push_back( &m_mapData[info.dwCollectionIndex] );
	}

	IME_LOG("LOAD CONF ITEM [Size=%d]", m_mapData.size());

	return true;
}

//card skill conf
CConfCardSkill::MapData CConfCardSkill::m_mapData;

bool CConfCardSkill::Init(const std::string& strFilePath )
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
		STC_CONF_CARD_SKILL info;
		READER_INT(info.dwIndex);
		READER_INT(info.byStarLevel);
	
		READER_STR(info.strName);

		READER_INT(info.byAttribute);
		READER_INT(info.wType);
		READER_INT(info.wEffectType);
		READER_INT(info.dwPara1);
		READER_INT(info.dwPro);

		READER_STR(info.strEffect);
		READER_STR(info.strSpecialEffectPath);
		IS_READER_ERROR;

		if (m_mapData.insert(std::make_pair(info.dwIndex, info)).second == false)
		{
			IME_ERROR("ERROR CONF DUPLICATE [ChestId=%u]", info.dwIndex);
			return false;
		}
	}

	IME_LOG("LOAD CONF SKILL [Size=%d]", m_mapData.size());

	return true;
}

STC_CONF_CARD_SKILL* CConfCardSkill::Find(uint32 dwIndex)
{
	MapData::iterator it = m_mapData.find(dwIndex);
	return it == m_mapData.end() ? NULL : &it->second;
}

bool CConfCardSkill::Valid(const std::string& strFilePath )
{
	MapData mapData;

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
		STC_CONF_CARD_SKILL info;
		READER_INT(info.dwIndex);
		READER_INT(info.byStarLevel);
	
		READER_STR(info.strName);

		READER_INT(info.byAttribute);
		READER_INT(info.wType);
		READER_INT(info.wEffectType);
		READER_INT(info.dwPara1);
		READER_INT(info.dwPro);

		READER_STR(info.strEffect);
		READER_STR(info.strSpecialEffectPath);
		IS_READER_ERROR;

		if (mapData.insert(std::make_pair(info.dwIndex, info)).second == false)
		{
			IME_ERROR("ERROR CONF DUPLICATE [ChestId=%u]", info.dwIndex);
			return false;
		}
	}

	IME_LOG("LOAD CONF SKILL [Size=%d]", mapData.size());

	return true;
}


CConfMasterBase::MapData CConfMasterBase::m_mapData;

bool CConfMasterBase::Init(const std::string& strFilePath )
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
		STC_CONF_MASTER_BASE info;
		READER_INT(info.dwIndex);

		READER_STR(info.strName);

		READER_INT(info.dwStar);
		READER_INT(info.byType);

		READER_STR(info.strSkillName);

		READER_INT(info.dwSkillIndex);
		READER_INT(info.dwNumberMax);
		READER_INT(info.dwNeedLevel);
		READER_INT(info.dwAliveTime);
		READER_INT(info.dwSendRedCon);
		READER_INT(info.dwBlessCD);

		READER_INT(info.dwMasterCardIndex);
		READER_INT(info.dwFixPro);
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

void CConfMasterBase::GetHandbookData( std::vector<uint32>& masters ) {
	masters.clear();
	MapData::iterator it = m_mapData.begin();
	MapData::iterator itEnd = m_mapData.end();
	for ( ; it != itEnd; ++it )
	{
		masters.push_back( it->second.dwIndex );
	}
}


STC_CONF_MASTER_BASE* CConfMasterBase::Find(uint32 dwIndex)
{
	MapData::iterator it = m_mapData.find(dwIndex);
	return it == m_mapData.end() ? NULL : &it->second;
}

//jewel skill conf
CConfJewelSkill::MapData CConfJewelSkill::m_mapData;

bool CConfJewelSkill::Init(const std::string& strFilePath )
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
		STC_CONF_JEWEL_SKILL info;
		READER_INT(info.dwIndex);
		READER_INT(info.wType);
		READER_INT(info.para1);
		READER_INT(info.dwGrow);
		IS_READER_ERROR;

		if (m_mapData.insert(std::make_pair(info.dwIndex, info)).second == false)
		{
			IME_ERROR("ERROR CONF DUPLICATE [ChestId=%u]", info.dwIndex);
			return false;
		}
	}

	IME_LOG("LOAD CONF ITEMSKILL [Size=%d]", m_mapData.size());

	return true;
}

STC_CONF_JEWEL_SKILL* CConfJewelSkill::Find(uint32 dwIndex)
{
	MapData::iterator it = m_mapData.find(dwIndex);
	return it == m_mapData.end() ? NULL : &it->second;
}

bool CConfJewelSkill::Valid(const std::string& strFilePath )
{
	MapData mapData;

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
		STC_CONF_JEWEL_SKILL info;
		READER_INT(info.dwIndex);
		READER_INT(info.wType);
		READER_INT(info.para1);
		READER_INT(info.dwGrow);
		IS_READER_ERROR;

		if (mapData.insert(std::make_pair(info.dwIndex, info)).second == false)
		{
			IME_ERROR("ERROR CONF DUPLICATE [ChestId=%u]", info.dwIndex);
			return false;
		}
	}

	IME_LOG("LOAD CONF ITEMSKILL [Size=%d]", mapData.size());

	return true;
}

//relationship conf
CConfRelationShip::MapData CConfRelationShip::m_mapData;

bool CConfRelationShip::ReadConfig(const std::string& strFilePath, MapData& tmp)
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
		STC_CONF_RELATIONSHIP info;
		READER_INT(info.dwIndex);
		READER_STR(info.strName);
		READER_INT(info.dwID1);
		READER_INT(info.dwID2);
		READER_INT(info.dwID3);
		READER_INT(info.dwID4);
		READER_INT(info.dwID5);
		READER_INT(info.dwID6);
		READER_INT(info.wType);
		READER_INT(info.dwPara1);
		READER_STR(info.strContent);
		IS_READER_ERROR;

		std::vector<uint32_t> vectmp;
		vectmp.push_back(info.dwID1);
		vectmp.push_back(info.dwID2);
		vectmp.push_back(info.dwID3);
		vectmp.push_back(info.dwID4);
		vectmp.push_back(info.dwID5);
		vectmp.push_back(info.dwID6);
		for (int i = 0; i < vectmp.size(); i++)
		{
			if (vectmp[i] == 0)
				continue;
			if (find(vectmp.begin(), vectmp.end(), vectmp[i]) == vectmp.end())
			{
				IME_ERROR("relationship index %u can not find cardindex %u", info.dwIndex, vectmp[i]);
				return false;
			}
		}

		if (tmp.insert(std::make_pair(info.dwIndex, info)).second == false)
		{
			IME_ERROR("ERROR CONF DUPLICATE [ChestId=%u]", info.dwIndex);
			return false;
		}
	}

	IME_LOG("LOAD CONF RELATIONSHIP [Size=%d]", tmp.size());
	return true;
}

bool CConfRelationShip::Init(const std::string& strFilePath )
{
	m_mapData.clear();
	if (!ReadConfig(strFilePath, m_mapData))
	{
		IME_ERROR("init fail read config");
		return false;
	}

	return true;
}

STC_CONF_RELATIONSHIP* CConfRelationShip::Find(uint32 dwIndex)
{
	MapData::iterator it = m_mapData.find(dwIndex);
	return it == m_mapData.end() ? NULL : &it->second;
}

std::vector<int> CConfRelationShip::GetRelationshipIndex(const std::vector<int> &vecIndex, const std::vector<int> &vecSkill)
{
	std::vector<int> vecRes;
	MapData::iterator it;
	std::vector<int>::const_iterator itindex;
	for (int i = 0; i < vecSkill.size(); i++)
	{
		it = m_mapData.find(vecSkill[i]);	
		if (it == m_mapData.end())
			continue;

		if (it->second.dwID1 != 0)
		{
			itindex = find(vecIndex.begin(), vecIndex.end(), (int)(it->second.dwID1));
			if (itindex == vecIndex.end())
			{
				continue;
			}
		}
		if (it->second.dwID2 != 0)
		{
			itindex = std::find(vecIndex.begin(), vecIndex.end(), (int)(it->second.dwID2));
			if (itindex == vecIndex.end())
			{
				continue;
			}
		}
		if (it->second.dwID3 != 0)
		{
			itindex = std::find(vecIndex.begin(), vecIndex.end(), (int)(it->second.dwID3));
			if (itindex == vecIndex.end())
			{
				continue;
			}
		}
		if (it->second.dwID4 != 0)
		{
			itindex = std::find(vecIndex.begin(), vecIndex.end(), (int)(it->second.dwID4));
			if (itindex == vecIndex.end())
			{
				continue;
			}
		}
		if (it->second.dwID5 != 0)
		{
			itindex = std::find(vecIndex.begin(), vecIndex.end(), (int)(it->second.dwID5));
			if (itindex == vecIndex.end())
			{
				continue;
			}
		}
		if (it->second.dwID6 != 0)
		{
			itindex = std::find(vecIndex.begin(), vecIndex.end(), (int)(it->second.dwID6));
			if (itindex == vecIndex.end())
			{
				continue;
			}
		}
		itindex = std::find(vecRes.begin(), vecRes.end(), (int)(it->first));
		if (itindex == vecRes.end())
		{
			vecRes.push_back(it->first);
		}
	}
	return vecRes;
}

bool CConfRelationShip::Valid(const std::string& strFilePath )
{
    MapData m_mapData;

	if (!ReadConfig(strFilePath, m_mapData))
	{
		IME_ERROR("valid fail read config");
		return false;
	}

	return true;
}


// strength consume
CConfStrengthConsume::MapData CConfStrengthConsume::m_mapData;

bool CConfStrengthConsume::Init(const std::string& strFilePath )
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
	int ii = 1;
	for (int i = 1; i < nRow; ++i)
	{
		int j = 0;
		STC_CONF_STRENGTH_CONSUME info;
		READER_INT(info.wLevel);
		if (ii != info.wLevel)
		{
			IME_ERROR("ERROR CONF UNORDER now number %u [INDEX = %u]", ii, info.wLevel);
			return false;
		}

		READER_INT(info.dwConsume1);
		READER_INT(info.dwConsume2);
		READER_INT(info.dwConsume3);
		READER_INT(info.dwConsume4);
		READER_INT(info.dwConsume5);
		
		if (m_mapData.insert(std::make_pair(info.wLevel, info)).second == false)
		{
			IME_ERROR("ERROR CONF DUPLICATE [Level=%u]", info.wLevel);
			return false;
		}
		IS_READER_ERROR;

		ii++;
	}

	IME_LOG("LOAD CONF STRENGTHEN [Size=%d]", m_mapData.size());

	return true;
}

STC_CONF_STRENGTH_CONSUME* CConfStrengthConsume::Find(uint16 wLevel)
{
	std::map<uint32, STC_CONF_STRENGTH_CONSUME>::iterator it = m_mapData.find(wLevel);
	
	return it == m_mapData.end() ? NULL : &it->second;
}

uint32_t CConfStrengthConsume::GetConsume(uint32 dwStar, uint16 level)
{
	std::map<uint32, STC_CONF_STRENGTH_CONSUME>::iterator it = m_mapData.find(level);
	if (it == m_mapData.end())
	{
		IME_ERROR("can not find this level %u in card exp csv", level);
		return 0;
	}
	uint32 Exp;
	switch(dwStar)
	{
		case 1:
			Exp = it->second.dwConsume1;
			break;
		case 2:
			Exp = it->second.dwConsume2;
			break;
		case 3:
			Exp = it->second.dwConsume3;
			break;
		case 4:
			Exp = it->second.dwConsume4;
			break;
		case 5:
			Exp = it->second.dwConsume5;
			break;
		default:
			return 0;
	}
	return Exp;
}

bool CConfStrengthConsume::Valid(const std::string& strFilePath )
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
	int ii = 1;
	for (int i = 1; i < nRow; ++i)
	{
		int j = 0;
		STC_CONF_STRENGTH_CONSUME info;
		READER_INT(info.wLevel);
		if (ii != info.wLevel)
		{
			IME_ERROR("ERROR CONF UNORDER now number %u [INDEX = %u]", ii, info.wLevel);
			return false;
		}

		READER_INT(info.dwConsume1);
		READER_INT(info.dwConsume2);
		READER_INT(info.dwConsume3);
		READER_INT(info.dwConsume4);
		READER_INT(info.dwConsume5);
		
		if (m_mapData.insert(std::make_pair(info.wLevel, info)).second == false)
		{
			IME_ERROR("ERROR CONF DUPLICATE [Level=%u]", info.wLevel);
			return false;
		}
		IS_READER_ERROR;

		ii++;
	}

	IME_LOG("LOAD CONF STRENGTHEN [Size=%d]", m_mapData.size());

	return true;
}

//leadskill conf
CConfLeaderSkill::MapData CConfLeaderSkill::m_mapData;

bool CConfLeaderSkill::Init(const std::string& strFilePath )
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
		STC_CONF_LEADER_SKILL info;
		READER_INT(info.dwIndex);
		READER_STR(info.strName);
		READER_STR(info.strContent);

		READER_INT(info.dwStick);
		READER_INT(info.dwSword);
		READER_INT(info.dwDark);
		READER_INT(info.dwPalm);
		READER_INT(info.dwInside);
		READER_INT(info.wtype1);
		READER_INT(info.dwNum1);
		READER_INT(info.dwUp1);
		READER_INT(info.wtype2);
		READER_INT(info.dwNum2);
		READER_INT(info.dwUp2);
		READER_INT(info.wtype3);
		READER_INT(info.dwNum3);
		READER_INT(info.dwUp3);
		READER_INT(info.dwMaxlv);
		READER_INT(info.wPro1);
		READER_INT(info.wPro2);
		READER_INT(info.wPro3);
		READER_INT(info.wPro4);
		READER_INT(info.wPro5);
		READER_INT(info.wPro6);
		READER_INT(info.wPro7);
		READER_INT(info.wPro8);
		READER_INT(info.wPro9);
		IS_READER_ERROR;

		if (m_mapData.insert(std::make_pair(info.dwIndex, info)).second == false)
		{
			IME_ERROR("ERROR CONF DUPLICATE [ChestId=%u]", info.dwIndex);
			return false;
		}
	}

	IME_LOG("LOAD CONF SKILL [Size=%d]", m_mapData.size());

	return true;
}

STC_CONF_LEADER_SKILL* CConfLeaderSkill::Find(uint32 dwIndex)
{
	MapData::iterator it = m_mapData.find(dwIndex);
	return it == m_mapData.end() ? NULL : &it->second;
}

bool CConfLeaderSkill::Valid(const std::string& strFilePath )
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
		STC_CONF_LEADER_SKILL info;
		READER_INT(info.dwIndex);
		READER_STR(info.strName);
		READER_STR(info.strContent);

		READER_INT(info.dwStick);
		READER_INT(info.dwSword);
		READER_INT(info.dwDark);
		READER_INT(info.dwPalm);
		READER_INT(info.dwInside);
		READER_INT(info.wtype1);
		READER_INT(info.dwNum1);
		READER_INT(info.dwUp1);
		READER_INT(info.wtype2);
		READER_INT(info.dwNum2);
		READER_INT(info.dwUp2);
		READER_INT(info.wtype3);
		READER_INT(info.dwNum3);
		READER_INT(info.dwUp3);
		READER_INT(info.dwMaxlv);
		READER_INT(info.wPro1);
		READER_INT(info.wPro2);
		READER_INT(info.wPro3);
		READER_INT(info.wPro4);
		READER_INT(info.wPro5);
		READER_INT(info.wPro6);
		READER_INT(info.wPro7);
		READER_INT(info.wPro8);
		READER_INT(info.wPro9);
		IS_READER_ERROR;

		if (m_mapData.insert(std::make_pair(info.dwIndex, info)).second == false)
		{
			IME_ERROR("ERROR CONF DUPLICATE [ChestId=%u]", info.dwIndex);
			return false;
		}
	}

	IME_LOG("LOAD CONF SKILL [Size=%d]", m_mapData.size());

	return true;
}

#if ( CC_PLATFORM_WIN32 == CC_TARGET_PLATFORM ) || ( CC_PLATFORM_IOS == CC_TARGET_PLATFORM ) || ( CC_PLATFORM_ANDROID == CC_TARGET_PLATFORM )
#else
bool CConfLeaderSkill::IsLevelUp(uint32 Index, uint16 level)
{
	STC_CONF_LEADER_SKILL *pConf = Find(Index);
	if (pConf == NULL)
	{
		IME_ERROR("can not find leader skill index %u", Index);
		return false;
	}
	if (level >= pConf->dwMaxlv)
	{
		IME_ERROR("skill index %u level %u overload maxlevel %u", Index, level, pConf->dwMaxlv);
		return false;
	}
	if (level >= 10)
	{
		IME_ERROR("level bigger than 9 now level %u", level);
		return false;
	}
	std::vector<uint16_t> vecpro;
	vecpro.push_back(pConf->wPro1);
	vecpro.push_back(pConf->wPro2);
	vecpro.push_back(pConf->wPro3);
	vecpro.push_back(pConf->wPro4);
	vecpro.push_back(pConf->wPro5);
	vecpro.push_back(pConf->wPro6);
	vecpro.push_back(pConf->wPro7);
	vecpro.push_back(pConf->wPro8);
	vecpro.push_back(pConf->wPro9);
	return CUtil::RandEither(vecpro[level], 100);
}
#endif


//achieve conf
CConfAchieve::MapData CConfAchieve::m_mapData;
#if ( CC_PLATFORM_WIN32 == CC_TARGET_PLATFORM ) || ( CC_PLATFORM_IOS == CC_TARGET_PLATFORM ) || ( CC_PLATFORM_ANDROID == CC_TARGET_PLATFORM )
CConfAchieve::MapVecData CConfAchieve::m_mapVecData;
//CConfAchieve::MapVecData CConfAchieve::m_mapVecProcessData;
//CConfAchieve::MapVecData CConfAchieve::m_achievementDatas;
//CConfAchieve::VecData CConfAchieve::m_vecData;
#endif
bool CConfAchieve::Init(const std::string& strFilePath )
{
	m_mapData.clear();
#if ( CC_PLATFORM_WIN32 == CC_TARGET_PLATFORM ) || ( CC_PLATFORM_IOS == CC_TARGET_PLATFORM ) || ( CC_PLATFORM_ANDROID == CC_TARGET_PLATFORM )
	m_mapVecData.clear();
//	m_achievementDatas.clear();
//	m_vecData.clear();
#endif

	if (!ReadConfig(strFilePath, m_mapData))
	{
		IME_ERROR("read config fail");
		return false;
	}
	return true;
}

bool CConfAchieve::ReadConfig(const std::string &strFilePath, MapData &map)
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
		STC_CONF_ACHIEVE info;
		READER_INT( info.dwIndex );
		READER_STR( info.strTitle );
		READER_INT( info.wAchieveType );

		READER_INT( info.dwMissionEffect );
		READER_INT( info.dwMinLevel );
		READER_INT( info.dwMaxLevel );
		READER_INT( info.wCompeteType );
		READER_INT( info.dwCompeteObjId );
		READER_INT( info.dwStep );
		READER_INT( info.byDisplay );
		READER_INT( info.dwCoin );
		READER_INT( info.dwDollar );
		READER_INT( info.dwStrength );
		READER_INT( info.dwEnergy );
		READER_INT( info.dwPrestige );
		READER_INT( info.dwWorldExp );
		READER_INT( info.dwObjType );
		READER_INT( info.dwPara1 );
		READER_INT( info.dwPara2 );
		READER_STR( info.strContent );
		READER_INT( info.dwAchieveVal );
		READER_INT( info.dwNextAchieveIndex );
		READER_INT( info.dwGroupId );
		READER_STR( info.strGroupName );
		READER_INT( info.dwActive );
		IS_READER_ERROR;

		if (map.insert(std::make_pair(info.dwIndex, info)).second == false)
		{
			IME_ERROR("ERROR CONF DUPLICATE [ChestId=%u]", info.dwIndex);
			return false;
		}
#if ( CC_PLATFORM_WIN32 == CC_TARGET_PLATFORM ) || ( CC_PLATFORM_IOS == CC_TARGET_PLATFORM ) || ( CC_PLATFORM_ANDROID == CC_TARGET_PLATFORM )
        if ( info.wAchieveType == EVERYDAY_MISSION )
		{
            m_mapVecData[EVERYDAY_MISSION].push_back( &map[info.dwIndex] );
        }
        else if ( info.wAchieveType == PROCESS_MISSION )
		{
            m_mapVecData[PROCESS_MISSION].push_back( &map[info.dwIndex] );
        }
        else if ( info.wAchieveType == ACHIEVE )
		{
            m_mapVecData[ACHIEVE].push_back( &map[info.dwIndex] );
        }
		else if ( info.wAchieveType == SHARE )
		{
			m_mapVecData[SHARE].push_back( &map[info.dwIndex] );
		}
		else if ( info.wAchieveType == ACTIVE_MISSION )
		{
			m_mapVecData[ACTIVE_MISSION].push_back( &map[info.dwIndex] );
		}
#endif
    }

	IME_LOG("LOAD CONF SKILL [Size=%d]", map.size());
	return true;
}

STC_CONF_ACHIEVE* CConfAchieve::Find(uint32 dwIndex)
{
	MapData::iterator it = m_mapData.find(dwIndex);
	return it == m_mapData.end() ? NULL : &it->second;
}

bool CConfAchieve::Valid(const std::string& strFilePath )
{
    MapData maptmp;

	if (!ReadConfig(strFilePath, maptmp))
	{
		IME_ERROR("read config fail");
		return false;
	}
	return true;
}

CConfAchieve::MapData& CConfAchieve::GetData()
{
	return m_mapData;
}

// card up
CConfCardUp::MapData CConfCardUp::m_mapData;
std::map<uint32_t, std::vector<uint32_t> > CConfCardUp::m_mapExpToLevel;

bool CConfCardUp::Init(const std::string& strFilePath )
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
	uint32_t ExpLimit1 = 0;
	uint32_t ExpLimit2 = 0;
	uint32_t ExpLimit3 = 0;
	uint32_t ExpLimit4 = 0;
	uint32_t ExpLimit5 = 0;
	uint32_t ExpLimit6 = 0;
	uint32_t ExpLimit7 = 0;

	uint32_t val = 0;
	int ii = 1;
	for (int i = 1; i < nRow; ++i)
	{
		int j = 0;
		STC_CONF_CARD_UP info;
		READER_INT(info.wLevel);
//		READER_INT(info.dwStarExpLimit2);
//		READER_INT(info.dwStarExpLimit3);
//		READER_INT(info.dwStarExpLimit4);
//		READER_INT(info.dwStarExpLimit5);

//		if (star == 0 || star == info.dwStar)
//		{
//			if (star == 0)
//				star = info.dwStar;
//		}
//		else // change another star group
//		{
//			if (m_mapData.find(info.dwStar) != m_mapData.end())
//			{
//				IME_ERROR("ERROR CONF NO GROUP [STAR = %u] line %u", info.dwStar, i);
//				return false;
//			}
//			star = info.dwStar;
//			ExpLimit = 0;
//			ii = 1;
//		}
		if (ii != info.wLevel)
		{
			IME_ERROR("ERROR CONF UNORDER now number %u [INDEX = %u]", ii, info.wLevel);
			return false;
		}

		READER_INT(val);
		ExpLimit1 += val;
		info.dwExpLimit1 = ExpLimit1;
		m_mapExpToLevel[1].push_back(ExpLimit1);

		READER_INT(val);
		ExpLimit2 += val;
		info.dwExpLimit2 = ExpLimit2;
		m_mapExpToLevel[2].push_back(ExpLimit2);

		READER_INT(val);
		ExpLimit3 += val;
		info.dwExpLimit3 = ExpLimit3;
		m_mapExpToLevel[3].push_back(ExpLimit3);

		READER_INT(val);
		ExpLimit4 += val;
		info.dwExpLimit4 = ExpLimit4;
		m_mapExpToLevel[4].push_back(ExpLimit4);

		READER_INT(val);
		ExpLimit5 += val;
		info.dwExpLimit5 = ExpLimit5;
		m_mapExpToLevel[5].push_back(ExpLimit5);
		
		READER_INT(val);
		ExpLimit6 += val;
		info.dwExpLimit6 = ExpLimit6;
		m_mapExpToLevel[6].push_back(ExpLimit6);
		
		READER_INT(val);
		ExpLimit7 += val;
		info.dwExpLimit7 = ExpLimit7;
		m_mapExpToLevel[7].push_back(ExpLimit7);
		
		if (m_mapData.insert(std::make_pair(info.wLevel, info)).second == false)
		{
			IME_ERROR("ERROR CONF DUPLICATE [Level=%u]", info.wLevel);
			return false;
		}
		IS_READER_ERROR;

		ii++;
	}

	IME_LOG("LOAD CONF STRENGTHEN [Size=%d]", m_mapData.size());

	return true;
}

STC_CONF_CARD_UP* CConfCardUp::Find(uint16 wLevel)
{
	std::map<uint32, STC_CONF_CARD_UP>::iterator it = m_mapData.find(wLevel);
	
	return it == m_mapData.end() ? NULL : &it->second;
}

uint16_t CConfCardUp::FindLevel(uint32_t dwStar, uint32_t dwExp, uint32_t dwExpGrow)
{
	std::vector<uint32_t>::iterator low, it;
	std::vector<uint32_t> newExpVec;
	uint16_t level;

//	int i = 0;
	for (it = m_mapExpToLevel[dwStar].begin(); it != m_mapExpToLevel[dwStar].end(); it++)
	{
//		IME_LOG("lv %u row exp %u", i, *it);
		newExpVec.push_back(*it / 100.0 * dwExpGrow);
//		long long ttt = *it * dwExpGrow ;
//		IME_LOG("lv %u exp %lld, exp %lld", i, (long long int)(*it / 100.0 * dwExpGrow), newExpVec[newExpVec.size() - 1]);
//		i++;
	}

	low = lower_bound(newExpVec.begin(), newExpVec.end(), dwExp + 1);
	if (low == newExpVec.end())
	{
		level = newExpVec.size();
	}
	else
	{
		level = (int)(low - newExpVec.begin()) + 1;
	}
//	IME_LOG("level %u", level);
	return level;
}

uint32_t CConfCardUp::GetMaxExp(uint32_t dwStar, uint32_t grow, uint16_t maxlevel)
{
//	if (m_mapExpToLevel[dwStar].size() <= 1)
//		return 0;
//	std::vector<uint32_t>::iterator it = m_mapExpToLevel[dwStar].end();
//	it--;
//	it--;
//	return *it * grow / 100.0;
	return GetExp(dwStar, grow, maxlevel); 
}

uint32_t CConfCardUp::GetExp(uint32 dwStar, uint32 grow, uint16 level)
{
	std::map<uint32, STC_CONF_CARD_UP>::iterator it = m_mapData.find(level);
	if (it == m_mapData.end())
	{
		IME_ERROR("can not find this level %u in card exp csv", level);
		return 0;
	}
	uint32 Exp;
	switch(dwStar)
	{
		case 1:
			Exp = it->second.dwExpLimit1;
			break;
		case 2:
			Exp = it->second.dwExpLimit2;
			break;
		case 3:
			Exp = it->second.dwExpLimit3;
			break;
		case 4:
			Exp = it->second.dwExpLimit4;
			break;
		case 5:
			Exp = it->second.dwExpLimit5;
			break;
		case 6:
			Exp = it->second.dwExpLimit6;
			break;
		case 7:
			Exp = it->second.dwExpLimit7;
			break;
		default:
			return 0;
	}
	return Exp / 100.0 * grow;
}

//upgrade
CConfUpgrade::MapData CConfUpgrade::m_mapData;

bool CConfUpgrade::Init(const std::string& strFilePath )
{
	m_mapData.clear();
	
	if (!ReadConfig(strFilePath, m_mapData))
	{
		IME_LOG("read config fail");
		return false;
	}
	IME_LOG("LOAD CONF upgrade [Size=%d]", m_mapData.size());

	return true;
}

bool CConfUpgrade::ReadConfig(const std::string &strFilePath, MapData &map)
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
		STC_CONF_CARD_UPGRADE info;
		READER_INT(info.dwIndex);
		READER_INT(info.dwCardIndex);
		READER_INT(info.dwLevelLimit);

		READER_INT(info.wType[0]);
		READER_INT(info.dwPara1[0]);
		READER_INT(info.dwPara2[0]);
		READER_INT(info.wType[1]);
		READER_INT(info.dwPara1[1]);
		READER_INT(info.dwPara2[1]);
		READER_INT(info.wType[2]);
		READER_INT(info.dwPara1[2]);
		READER_INT(info.dwPara2[2]);
		READER_INT(info.wType[3]);
		READER_INT(info.dwPara1[3]);
		READER_INT(info.dwPara2[3]);
		READER_INT(info.wType[4]);
		READER_INT(info.dwPara1[4]);
		READER_INT(info.dwPara2[4]);

		READER_INT(info.dwCardIndexDesc);

		IS_READER_ERROR;
#if(CC_PLATFORM_ANDROID!=CC_TARGET_PLATFORM&&CC_PLATFORM_IOS!=CC_TARGET_PLATFORM&&CC_PLATFORM_WIN32!=CC_TARGET_PLATFORM)
		if (CConfCardBase::Find(info.dwCardIndex) == NULL)
		{
			IME_ERROR("can not find card index %u in upgrade csv index %u", info.dwCardIndex, info.dwIndex);
			return false;
		}
		if (CConfCardBase::Find(info.dwCardIndexDesc) == NULL)
		{
			IME_ERROR("can not find card index %u in upgrade csv index %u", info.dwCardIndexDesc, info.dwIndex);
			return false;
		}
		for (int i = 0; i < 5; i++)
		{
			if (info.wType[i] == E_OBJ_SHOP)
			{
				if (CConfGoods::Find(info.dwPara1[i]) == NULL)
				{
					IME_ERROR("can not recogntion this goods index %u", info.dwPara1);
					return false;
				}
			}
			else if (info.wType[i] == E_OBJ_CARD_MORE)
			{
				if (CConfCardBase::Find(info.dwPara1[i]) == NULL)
				{
					IME_ERROR("can not recognition this card index %u", info.dwPara1);
					return false;
				}
			}
			else if (info.wType[i] == 0)
			{
				// ok nothing to do
			}
			else
			{
				IME_ERROR("can not recognition type %u", info.wType[i]);
				return false;
			}
		}
#endif
		if (map.insert(std::make_pair(info.dwIndex, info)).second == false)
		{
			IME_ERROR("ERROR CONF DUPLICATE [ChestId=%u]", info.dwIndex);
			return false;
		}
	}
	return true;
}

STC_CONF_CARD_UPGRADE* CConfUpgrade::Find(uint32 dwIndex)
{
	MapData::iterator it = m_mapData.find(dwIndex);
	return it == m_mapData.end() ? NULL : &it->second;
}

STC_CONF_CARD_UPGRADE* CConfUpgrade::FindByCardIndex(uint32 dwCardIndex){
	for(MapData::iterator ii=m_mapData.begin();ii!=m_mapData.end();ii++){
		if(ii->second.dwCardIndex==dwCardIndex){
			return &(ii->second);
		}
	}
	return NULL;
}

bool CConfUpgrade::Valid(const std::string& strFilePath )
{
    MapData m_mapData;
	if (!ReadConfig(strFilePath, m_mapData))
	{
		IME_LOG("read config fail");
		return false;
	}

	IME_LOG("LOAD CONF SKILL [Size=%d]", m_mapData.size());
	return true;
}

bool CConfUpgrade::Check(std::map<uint32, std::vector<CConfUpgrade::SCard> > &mapCard, std::map<uint32_t, uint32_t> &mapItem, uint32 index)
{
	if (m_mapData.find(index) == m_mapData.end())
	{
		IME_ERROR("can not find index %u in upgrade csv", index);
		return false;
	}
	
	STC_CONF_CARD_UPGRADE &record = m_mapData[index];
	std::map<uint32, std::vector<SCard> > mapTempCard = mapCard;
	std::map<uint32_t, uint32_t> mapTempItem = mapItem;
	mapCard.clear();
	mapItem.clear();
	
	for (int i = 0; i < 5; i++)
	{
		if (record.wType[i] == E_OBJ_SHOP)
		{
			uint32_t itemindex = record.dwPara1[i];
			uint32_t number= record.dwPara2[i];
			if (mapTempItem.find(itemindex) == mapTempItem.end())
			{
				IME_ERROR("can not find item index %u", itemindex);
				return false;
			}
			if (mapTempItem[itemindex] < number)
			{
				IME_ERROR("item index %u number is not enough need %u now %u", itemindex, number, mapTempItem[itemindex]);
				return false;
			}
			mapTempItem[itemindex] -= number;
			if (mapItem.find(itemindex) != mapItem.end())
			{
				mapItem[itemindex] += number;
				IME_LOG("item index %u number %u", itemindex, number);
			}
			else
			{
				mapItem[itemindex] = number;
				IME_LOG("item index %u number %u", itemindex, number);
			}
		}
		else if (record.wType[i] == E_OBJ_CARD_MORE)
		{
			uint32_t cardindex = record.dwPara1[i];
			uint32_t number = record.dwPara2[i];
			if (mapTempCard.find(cardindex) == mapTempCard.end())
			{
				IME_ERROR("can not find card index %u", cardindex);
				return false;
			}
			if (mapTempCard[cardindex].size() < number)
			{
				IME_ERROR("item index %u number is not enough need %u now %u", cardindex, number, mapTempCard[cardindex].size());
				return false;
			}
			std::sort(mapTempCard[cardindex].begin(), mapTempCard[cardindex].end(), SCardComp);
			for (int j = 0; j < number; j++)
			{
				mapCard[cardindex].push_back(mapTempCard[cardindex][0]);
				mapTempCard[cardindex].erase(mapTempCard[cardindex].begin());
			}
			IME_LOG("card index %u number %u", cardindex, number);
		}
	}	
	return true;
}

//shop
CConfGoods::MapData CConfGoods::m_mapData;

bool CConfGoods::ReadConfig(const std::string& strFilePath, MapData &tmp)
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
		STC_CONF_GOODS info;
		READER_INT(info.dwIndex);
		READER_INT(info.dwFuncType);
		READER_INT(info.dwPara1);
		READER_INT(info.dwPara2);
		READER_INT(info.dwPara3);
		READER_INT(info.dwPara4);
		READER_INT(info.dwLevelLimit);
		READER_STR(info.strTitle);
		READER_STR(info.strContent);
		READER_STR(info.strPath);
		READER_INT(info.byIsSell);
		READER_INT(info.dwSellNumber);
		IS_READER_ERROR;
		if (info.dwFuncType == CHIP_EFFECT)
		{
			if (info.dwPara2 == E_OBJ_CARD && info.dwPara2 == E_OBJ_SKILL && info.dwPara2 == E_OBJ_AWAKE_CARD)
			{
				if (CConfCardBase::Find(info.dwPara3) == NULL)
				{
					IME_ERROR("goods index %u can not find card index %u", info.dwIndex, info.dwPara3);
					return false;
				}
			}
		}

		if (tmp.insert(std::make_pair(info.dwIndex, info)).second == false)
		{
			IME_ERROR("ERROR CONF DUPLICATE [ChestId=%u]", info.dwIndex);
			return false;
		}
	}

	IME_LOG("LOAD CONF SHOP PRIZE [Size=%d]", tmp.size());
	return true;
}

bool CConfGoods::Init(const std::string& strFilePath )
{
	m_mapData.clear();
	if (!ReadConfig(strFilePath, m_mapData))
	{
		IME_ERROR("init read config fail");
		return false;
	}
	return true;
}

STC_CONF_GOODS* CConfGoods::Find(uint32 dwIndex)
{
	MapData::iterator it = m_mapData.find(dwIndex);
	return it == m_mapData.end() ? NULL : &it->second;
}

CConfGoods::MapData* CConfGoods::GetVal()
{
	return &m_mapData;
}

bool CConfGoods::Valid(const std::string& strFilePath )
{
	MapData mapData;

	if (!ReadConfig(strFilePath, mapData))
	{
		IME_ERROR("valid read config fail");
		return false;
	}
	return true;
}

//trial
CConfTrial::MapData CConfTrial::m_mapData;

bool CConfTrial::ReadConfig(const std::string& strFilePath, MapData &tmp)
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
		STC_CONF_TRIAL info;
		READER_INT(info.dwIndex);
		READER_STR(info.strName);
		READER_INT(info.byFloor);
		READER_INT(info.wLevelLimit);
		READER_INT(info.dwChallengeNumber);
		READER_INT(info.wConsume);
		READER_INT(info.dwRoleExpNeed);
		READER_INT(info.dwCardExpNeed);
		READER_INT(info.dwCoinNeed);
		READER_INT(info.wPro);
		READER_INT(info.wPro1);
		READER_INT(info.dwBossId);
		READER_INT(info.wPro2);
		READER_INT(info.dwCoin);
		READER_INT(info.wPro3);
		READER_INT(info.dwRoleExp);
		READER_INT(info.wPro4);
		READER_INT(info.dwCardExp);
		READER_INT(info.wPro5);
		READER_INT(info.dwWorldExp);
		READER_INT(info.wPro6);
		READER_INT(info.dwItemCollection);
		READER_INT(info.wPro7);
		READER_INT(info.dwPiecesCollection);
		READER_INT(info.wPro8);
		READER_INT(info.dwCardCollection);
		READER_INT(info.wPro9);
		READER_INT(info.dwEnergy);
		READER_INT(info.wPro10);
		READER_INT(info.dwLotEnergy);
		READER_INT(info.wPro11);
		READER_INT(info.dwStrength);
		READER_INT(info.wPro12);
		READER_INT(info.dwLotStrength);
		READER_INT(info.wPro13);
		READER_INT(info.dwStamina);
		READER_INT(info.wPro14);
		READER_INT(info.dwLotStamina);
		READER_INT(info.dwTrieTime);
		READER_STR(info.strBackgroundPath);
		READER_INT(info.dwPro);
		READER_INT(info.dwNextTrial);
		READER_INT(info.wPlusPro);
		READER_INT(info.wPro15);

		IS_READER_ERROR;

		if (tmp.insert(std::make_pair(info.dwIndex, info)).second == false)
		{
			IME_ERROR("ERROR CONF DUPLICATE [ChestId=%u]", info.dwIndex);
			return false;
		}
	}
	if (tmp.size() == 0)
	{
		IME_ERROR("this csv must have a record");
		return false;
	}

	IME_LOG("LOAD CONF SHOP PRIZE [Size=%d]", tmp.size());
	return true;
}

bool CConfTrial::Init(const std::string& strFilePath )
{
	m_mapData.clear();
	if (!ReadConfig(strFilePath, m_mapData))
	{
		IME_ERROR("init read config fail");
		return false;
	}
	return true;
}

STC_CONF_TRIAL* CConfTrial::Find(uint32 dwIndex)
{
	MapData::iterator it = m_mapData.find(dwIndex);
	return it == m_mapData.end() ? NULL : &it->second;
}

CConfTrial::MapData* CConfTrial::GetVal()
{
	return &m_mapData;
}

bool CConfTrial::Valid(const std::string& strFilePath )
{
	MapData mapData;

	if (!ReadConfig(strFilePath, mapData))
	{
		IME_ERROR("valid read config fail");
		return false;
	}
	return true;
}

STC_CONF_TRIAL* CConfTrial::FindOfFirstTrial()
{
	MapData::iterator it = m_mapData.begin();
	if (it == m_mapData.end())
		return NULL;
	return &(it->second);
}

uint32 CConfJewelSkill::Count(uint16 lv) {
    return 0;
}

std::map<uint32, STC_CONF_TOWER> CConfTower::mapData;
std::map<uint32,std::vector<uint32> > CConfTower::layerMap;
//Tower global data
bool CConfTower::init(const std::string& strPath) {
	mapData.clear();
	if(!readConfig(strPath,mapData)){
		IME_ERROR("init read config fail");
		return false;
	}
	return true;
}

STC_CONF_TOWER* CConfTower::find(uint32 dwIndex) {
	std::map<uint32, STC_CONF_TOWER>::iterator it = mapData.find(dwIndex);
	if(it == mapData.end()){
		return NULL;
	}
	return &(it->second);
}

std::map<uint32, STC_CONF_TOWER>* CConfTower::getMap() {
	return &mapData;
}

bool CConfTower::valid(const std::string& strPath) {
	std::map<uint32, STC_CONF_TOWER> mapData;
	if(readConfig(strPath,mapData)){
		IME_ERROR("valid read config fail");
		return false;
	}
	mapData.clear();
	return true;
}

bool CConfTower::readConfig(const std::string& strPath,
		std::map<uint32, STC_CONF_TOWER>& mapData) {
	CConfigReader reader;
	if (!reader.init(strPath)) {
		return false;
	}
	int nRow = reader.rowCount();
	if (nRow <= 1) {
		IME_ERROR("the row is small than 1 rowCount %u", nRow);
		return false;
	}
	uint32 tmpLayer = 0;
	for (int i = 1; i < nRow; ++i)
		{
			int j = 0;
			STC_CONF_TOWER info;
			READER_INT(info.dwIndex);
			READER_INT(info.layer);
			READER_INT(info.objectId);
			READER_INT(info.dwLevel);
			READER_INT(info.dwSkill);
			READER_INT(info.dwSkillLevel);
			READER_INT(info.attack);
			READER_INT(info.defend);
			READER_INT(info.health);
			READER_INT(info.dollarReward);
			READER_INT(info.coinReward);
			READER_INT(info.propReward);//道具
			READER_INT(info.cardReward);
			READER_INT(info.worldExpReward);
			READER_INT(info.dwSwordEffect);
			READER_INT(info.dwHandEffect);
			READER_INT(info.dwInternalForceEffect);
			READER_INT(info.dwDarkEffect);
			READER_INT(info.dwStickEffect);
			READER_INT(info.nextLayer);
			READER_INT(info.failureCardReward);
			READER_INT(info.failureDollarReward);
			READER_INT(info.failureCoinReward);
			READER_INT(info.failurePropReward);
			READER_INT(info.failureWorldExpReward);
//			READER_INT(info.successCardReward);
//			READER_INT(info.successDollarReward);
//			READER_INT(info.successCoinReward);
//			READER_INT(info.successPropReward);
//			READER_INT(info.successWorldExpReward);

			//todo cyl
			//将每层中boss信息存入 layerMap中
			std::map<uint32, std::vector<uint32> >::iterator it = layerMap.find(info.layer);
			if(it == layerMap.end()){
				std::vector<uint32> index;
				index.push_back(info.dwIndex);
				layerMap.insert(std::make_pair(info.layer, index));
			}else{
				it->second.push_back(info.dwIndex);
			}

//			//将有物品掉落的层记录下来
//			if(info.cardReward || info.coinReward || info.dollarReward || info.propReward){
//				if(tmpLayer != info.layer){
//					dropLayer.push_back(info.layer);
//					tmpLayer = info.layer;
//				}
//			}else{
//				if(tmpLayer == info.layer){
//					IME_ERROR("Tower csv at %u layer may be error!", info.layer);
//					return false;
//				}
//			}

			IS_READER_ERROR;

			if (mapData.insert(std::make_pair(info.dwIndex, info)).second == false)
			{
				IME_ERROR("ERROR CONF DUPLICATE [ChestId=%u]", info.dwIndex);
				return false;
			}
		}
		if (mapData.size() == 0)
		{
			IME_ERROR("this csv must have a record");
			return false;
		}
	return true;
}

std::vector<uint32> * CConfTower::findLayerObjects(uint32 layer) {
	std::map<uint32, std::vector<uint32> >::iterator it = layerMap.find(layer);
	if(it == layerMap.end()){
		return  NULL;
	}
	return &(it->second);
}
//uint32 CConfTower::getNextDropLayer(uint32 layer){
//	for(std::vector<uint32>::iterator it = dropLayer.begin(); it != dropLayer.end(); it++){
//		if(*it >= layer){
//			return *it;
//		}
//	}
//	return 0;
//}


//CConfVitality::MapData CConfVitality::m_Datas;
//#if ( CC_PLATFORM_WIN32 == CC_TARGET_PLATFORM ) || ( CC_PLATFORM_IOS == CC_TARGET_PLATFORM ) || ( CC_PLATFORM_ANDROID == CC_TARGET_PLATFORM )
//CConfVitality::VecData CConfVitality::m_vecs; 
//#endif
//
//bool CConfVitality::Init( const std::string &strPath )
//{
//	m_Datas.clear();
//#if ( CC_PLATFORM_WIN32 == CC_TARGET_PLATFORM ) || ( CC_PLATFORM_IOS == CC_TARGET_PLATFORM ) || ( CC_PLATFORM_ANDROID == CC_TARGET_PLATFORM )
//	m_vecs.clear();
//#endif
//	
//	CConfigReader reader;
//	if ( !reader.init( strPath ) ) 
//	{
//		return false;
//	}
//
//	int nRow = reader.rowCount();
//	if ( nRow <= 1 ) 
//	{
//		IME_ERROR("the row is small than 1 rowCount %u", nRow);
//		return false;
//	}
//
//	for ( int i = 1; i < nRow; ++i )
//	{
//		int j = 0;
//		STC_CONF_VITALITY data;
//		READER_INT( data.dwIndex );
//		READER_STR( data.strName );
//		READER_INT( data.dwType );
//		READER_INT( data.dwNeedLevel );
//		READER_INT( data.dwNeedStep );
//		READER_STR( data.strDesc );
//		READER_INT( data.dwAwardVitality );
//
//		m_Datas.insert( std::make_pair( data.dwIndex, data ) );
//#if ( CC_PLATFORM_WIN32 == CC_TARGET_PLATFORM ) || ( CC_PLATFORM_IOS == CC_TARGET_PLATFORM ) || ( CC_PLATFORM_ANDROID == CC_TARGET_PLATFORM )
//		m_vecs.push_back( data );
//#endif
//	}
//
//	return true;
//}
//
//STC_CONF_VITALITY *CConfVitality::find( uint32 dwIndex )
//{
//	std::map<uint32,STC_CONF_VITALITY>::iterator it = m_Datas.find( dwIndex );
//	return it == m_Datas.end() ? NULL : &( it->second );
//}
//
//uint32 CConfVitality::count()
//{
//	return m_Datas.size();
//}
//
//#if ( CC_PLATFORM_WIN32 == CC_TARGET_PLATFORM ) || ( CC_PLATFORM_IOS == CC_TARGET_PLATFORM ) || ( CC_PLATFORM_ANDROID == CC_TARGET_PLATFORM )
//uint32 CConfVitality::VecCount()
//{
//	return m_vecs.size();
//}
//
//const STC_CONF_VITALITY *CConfVitality::at( uint32 aIndex )
//{
//	if ( aIndex >= m_vecs.size() )
//	{
//		return NULL;
//	}
//
//	return &m_vecs[aIndex];
//}
//#endif

