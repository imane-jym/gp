/**
 * =====================================================================================
 *       @file  Card.cpp
 *      @brief  
 *
 *  Detailed description starts here.
 *
 *   @internal
 *     Created  2012年08月29日 12时10分31秒 
 *    Revision  1.0.0.0
 *    Compiler  gcc/g++
 *     Company  
 *   Copyright  Copyright (c) 2012, 
 *
 *     @author  imane (), imanecr@gmail.com
 * =====================================================================================
 */
#include <algorithm>
#include "Card.h"
#include "ConfigData.h"
#include "Role.h"
#include "CardPacket.h"
#include "EnumDefines.h"
#include "CmdDefine.h"
#include "skill.h"
#include "Mission.h"

using namespace std;

CCard::CCard()
{
    //SetdwObjId(m_dwObjId);
    //SetdwIndex(m_dwIndex);
    //SetbyType(m_byType);
    //SetbyAttribute(m_byAttribute);
    //SetbyStarLevel(m_byStarLevel);
    //SetwLevel(m_wLevel);
    //SetdwExp(m_dwExp);
    //SetdwExpLimit(m_dwExpLimit);
    //SetdwAttackMin(m_dwAttackMin);
    //SetdwAttackMax(m_dwAttackMax);
    //SetdwDefenseMin(m_dwDefenseMin);
    //SetdwDefenseMax(m_dwDefenseMax);
    //SetdwEquipObjId(m_dwEquipObjId);
    //SetdwEffectIndex(m_dwEffectIndex);
    //SetdwWeaponObjId(m_dwWeaponObjId);
    //SetdwArmorObjId(m_dwArmorObjId);
    //SetdwJewelObjId(m_dwJewelObjId);
    //SetdwAttackSkillIndex(m_dwAttackSkillIndex);
    //SetdwAttackSkillLevel(m_dwAttackSkillLevel);
    //SetdwAttackSkillExp(m_dwAttackSkillExp);
    //SetdwDeffenseSkillIndex(m_dwDeffenseSkillIndex);
    //SetdwDeffenseSkillLevel(m_dwDeffenseSkillLevel);
    //SetdwDeffenseSkillExp(m_dwDeffenseSkillExp);
    //SetbyRare(m_byRare);
    //SetdwSuit1Index(m_dwSuit1Index);
    //SetdwSuit2Index(m_dwSuit2Index);
    //SetdwSuit3Index(m_dwSuit3Index);
    //SetdwSuit4Index(m_dwSuit4Index); 

    SetdwObjId(0);
    SetdwIndex(0);
    SetbyType(0);
    SetbyAttribute(0);
    SetbyStarLevel(0);
    SetwLevel(0);
    SetdwExp(0);
    SetdwExpLimit(0);
    SetdwEffectIndex(0);
    SetbyRare(E_CR_NORMAL);
	SetdwLastExpLimit(0);
	SetbyFlag(0);
	SetdwSkillPara(0);
	SetdwAttack(0);
	SetdwDefence(0);
	SetdwHealth(0);
	SetdwAttackGrow(0);
	SetdwDefenceGrow(0);
	SetdwHealthGrow(0);
	SetdwExpGrow(0);
	SetdwCritRate(0);
//	SetdwDodgeRate(0);
//	SetdwAction(0);
	SetdwSuitIndex(0);
//	SetdwInitSkillIndex(0);
//	SetdwStudySkillIndex(0);
	SetdwMasterSkillIndex(0);
	SetdwSkillIndex(0);
	SetdwGrowLimit(0);
	SetdwLimitBreakNumber(0);
	SetdwLimitBreak(0);
	SetdwLimitBreakPro(0);
	SetdwAwakePro(0);
	SetdwAddExp(0);
	SetdwConsumeCoin(0);
	SetdwGetCoin(0);
	SetwMasterSkillLv(0);
	SetdwBreakExp(0);
	SetbyMultiPower(0);
	SetdwMultiTime(0);
	SetdwOneLevelExp(0);
	SetdwTotalConsumeCoin(0);
	SetdwSkillPro(0);
	SetdwSkillPower(0);
	SetdwDurationExp(0);
	SetdwDurationCoin(0);
	SetdwObjDrop(0);
	SetdwStrengthRecover(0);
	SetdwEnergyRecover(0);
	SetdwAddHarm(0);
	SetdwReduceHarm(0);
	SetdwAddCritHarm(0);
	SetwLeaderLevel(0);
	SetdwLeaderIndex(0);
	SetdwCardGroup(0);
	SetdwMasterIndex(0);
	SetwAttackPlus(0);
	SetwDefencePlus(0);
	SetwHealthPlus(0);

	m_pCRole = NULL;
}

CCard::~CCard()
{
}

void CCard::PacketData(ByteBuffer &by)
{
	uint8_t arrenum[] = {
    E_CARD_dwObjId,
    E_CARD_dwIndex,
    E_CARD_byType,
    E_CARD_byAttribute,
    E_CARD_byStarLevel,
    E_CARD_wLevel,
    E_CARD_dwExp,
    E_CARD_dwExpLimit,
    E_CARD_dwEffectIndex,
//  E_CARD_dwAttackSkillIndex,
//  E_CARD_dwDeffenseSkillIndex,
    E_CARD_byRare,
    E_CARD_dwLastExpLimit,
    E_CARD_byFlag,
    E_CARD_dwSkillPara,
	E_CARD_dwAttack,
	E_CARD_dwDefence,
	E_CARD_dwHealth,
	E_CARD_dwAttackGrow,
	E_CARD_dwDefenceGrow,
	E_CARD_dwHealthGrow,
	E_CARD_dwExpGrow,
	E_CARD_dwCritRate,
//	E_CARD_dwDodgeRate,
//	E_CARD_dwAction,
	E_CARD_dwSuitIndex,
//	E_CARD_dwInitSkillIndex,
//	E_CARD_dwStudySkillIndex,
	E_CARD_dwMasterSkillIndex,
	E_CARD_wMasterSkillLv,
	E_CARD_dwSkillIndex,
	E_CARD_dwGrowLimit,
	E_CARD_dwLimitBreakNumber,
	E_CARD_dwLimitBreak,
	E_CARD_dwLimitBreakPro,
	E_CARD_dwAwakePro,
	E_CARD_dwAddExp,
	E_CARD_dwConsumeCoin,
	E_CARD_dwGetCoin,
	E_CARD_dwBreakExp,
	E_CARD_byMultiPower,
	E_CARD_dwMultiTime,
	E_CARD_dwOneLevelExp,
	E_CARD_dwTotalConsumeCoin,
	E_CARD_dwSkillPro,
	E_CARD_dwSkillPower,
	E_CARD_dwDurationExp,
	E_CARD_dwDurationCoin,
	E_CARD_dwObjDrop,
	E_CARD_dwStrengthRecover,
	E_CARD_dwEnergyRecover,
	E_CARD_dwAddHarm,
	E_CARD_dwReduceHarm,
	E_CARD_dwAddCritHarm,
	E_CARD_wLeaderLevel,
	E_CARD_dwLeaderIndex,
	E_CARD_dwCardGroup,
	E_CARD_dwMasterIndex,
	E_CARD_wAttackPlus,
	E_CARD_wDefencePlus,
	E_CARD_wHealthPlus,
	};

	by << (uint8_t) (sizeof(arrenum) / sizeof(uint8_t));

    by << (uint8_t) E_CARD_dwObjId << (uint8_t)eAnyTypeUInt32 << GetdwObjId();
    by << (uint8_t) E_CARD_dwIndex << (uint8_t)eAnyTypeUInt32 << GetdwIndex();
    by << (uint8_t) E_CARD_byType << (uint8_t)eAnyTypeUInt8 << GetbyType();
    by << (uint8_t) E_CARD_byAttribute << (uint8_t)eAnyTypeUInt8 << GetbyAttribute();
    by << (uint8_t) E_CARD_byStarLevel << (uint8_t)eAnyTypeUInt8 << GetbyStarLevel();
    by << (uint8_t) E_CARD_wLevel << (uint8_t)eAnyTypeUInt16 << GetwLevel();
    by << (uint8_t) E_CARD_dwExp << (uint8_t)eAnyTypeUInt32 << GetdwExp();
    by << (uint8_t) E_CARD_dwExpLimit << (uint8_t)eAnyTypeUInt32 << GetdwExpLimit();
    by << (uint8_t) E_CARD_dwEffectIndex << (uint8_t)eAnyTypeUInt32 << GetdwEffectIndex();
    by << (uint8_t) E_CARD_byRare << (uint8_t)eAnyTypeUInt32 << GetbyRare();
    by << (uint8_t) E_CARD_dwLastExpLimit << (uint8_t)eAnyTypeUInt32 << GetdwLastExpLimit();
    by << (uint8_t) E_CARD_byFlag << (uint8_t)eAnyTypeUInt8 << GetbyFlag();
    by << (uint8_t) E_CARD_dwSkillPara << (uint8_t)eAnyTypeUInt32 << GetdwSkillPara();
	by << (uint8_t) E_CARD_dwAttack << (uint8_t)eAnyTypeUInt32 << GetdwAttack();
	by << (uint8_t) E_CARD_dwDefence << (uint8_t)eAnyTypeUInt32 << GetdwDefence();
	by << (uint8_t) E_CARD_dwHealth << (uint8_t)eAnyTypeUInt32 << GetdwHealth();
	by << (uint8_t) E_CARD_dwAttackGrow << (uint8_t)eAnyTypeUInt32 << GetdwAttackGrow();
	by << (uint8_t) E_CARD_dwDefenceGrow << (uint8_t)eAnyTypeUInt32 << GetdwDefenceGrow();
	by << (uint8_t) E_CARD_dwHealthGrow << (uint8_t)eAnyTypeUInt32 << GetdwHealthGrow();
	by << (uint8_t) E_CARD_dwExpGrow << (uint8_t)eAnyTypeUInt32 << GetdwExpGrow();
	by << (uint8_t) E_CARD_dwCritRate << (uint8_t)eAnyTypeUInt32 << GetdwCritRate();
	by << (uint8_t) E_CARD_dwSuitIndex << (uint8_t)eAnyTypeUInt32 << GetdwSuitIndex();
	by << (uint8_t) E_CARD_dwMasterSkillIndex << (uint8_t)eAnyTypeUInt32 << GetdwMasterSkillIndex();
	by << (uint8_t) E_CARD_wMasterSkillLv << (uint8_t)eAnyTypeUInt16 << GetwMasterSkillLv();
	by << (uint8_t) E_CARD_dwSkillIndex << (uint8_t)eAnyTypeUInt32 << GetdwSkillIndex();
	by << (uint8_t) E_CARD_dwGrowLimit << (uint8_t)eAnyTypeUInt32 << GetdwGrowLimit();
	by << (uint8_t) E_CARD_dwLimitBreakNumber << (uint8_t)eAnyTypeUInt32 << GetdwLimitBreakNumber();
	by << (uint8_t) E_CARD_dwLimitBreak << (uint8_t)eAnyTypeUInt32 << GetdwLimitBreak();
	by << (uint8_t) E_CARD_dwLimitBreakPro << (uint8_t)eAnyTypeUInt32 << GetdwLimitBreakPro();
	by << (uint8_t) E_CARD_dwAwakePro << (uint8_t)eAnyTypeUInt32 << GetdwAwakePro();
	by << (uint8_t) E_CARD_dwAddExp << (uint8_t)eAnyTypeUInt32 << GetdwAddExp();
	by << (uint8_t) E_CARD_dwConsumeCoin << (uint8_t)eAnyTypeUInt32 << GetdwConsumeCoin();
	by << (uint8_t) E_CARD_dwGetCoin << (uint8_t)eAnyTypeUInt32 << GetdwGetCoin();
	by << (uint8_t) E_CARD_dwBreakExp << (uint8_t)eAnyTypeUInt32 << GetdwBreakExp();
	by << (uint8_t) E_CARD_byMultiPower << (uint8_t)eAnyTypeUInt8 << GetbyMultiPower();
	by << (uint8_t) E_CARD_dwMultiTime << (uint8_t)eAnyTypeUInt32 << GetdwMultiTime();
	by << (uint8_t) E_CARD_dwOneLevelExp << (uint8_t)eAnyTypeUInt32 << GetdwOneLevelExp();
	by << (uint8_t) E_CARD_dwTotalConsumeCoin << (uint8_t)eAnyTypeUInt32 << GetdwTotalConsumeCoin();
	by << (uint8_t) E_CARD_dwSkillPro << (uint8_t)eAnyTypeUInt32 << GetdwSkillPro();
	by << (uint8_t) E_CARD_dwSkillPower << (uint8_t)eAnyTypeUInt32 << GetdwSkillPower();
	by << (uint8_t) E_CARD_dwDurationExp << (uint8_t)eAnyTypeUInt32 << GetdwDurationExp();
	by << (uint8_t) E_CARD_dwDurationCoin << (uint8_t)eAnyTypeUInt32 << GetdwDurationCoin();
	by << (uint8_t) E_CARD_dwObjDrop << (uint8_t)eAnyTypeUInt32 << GetdwObjDrop();
	by << (uint8_t) E_CARD_dwStrengthRecover << (uint8_t)eAnyTypeUInt32 << GetdwStrengthRecover();
	by << (uint8_t) E_CARD_dwEnergyRecover << (uint8_t)eAnyTypeUInt32 << GetdwEnergyRecover();
	by << (uint8_t) E_CARD_dwAddHarm << (uint8_t)eAnyTypeUInt32 << GetdwAddHarm();
	by << (uint8_t) E_CARD_dwReduceHarm << (uint8_t)eAnyTypeUInt32 << GetdwReduceHarm();
	by << (uint8_t) E_CARD_dwAddCritHarm << (uint8_t)eAnyTypeUInt32 << GetdwAddCritHarm();
	by << (uint8_t) E_CARD_wLeaderLevel << (uint8_t)eAnyTypeUInt16 << GetwLeaderLevel();
	by << (uint8_t) E_CARD_dwLeaderIndex << (uint8_t)eAnyTypeUInt32 << GetdwLeaderIndex();
	by << (uint8_t) E_CARD_dwCardGroup << (uint8_t)eAnyTypeUInt32 << GetdwCardGroup();
	by << (uint8_t) E_CARD_dwMasterIndex << (uint8_t)eAnyTypeUInt32 << GetdwMasterIndex();
	by << (uint8_t) E_CARD_wAttackPlus << (uint8_t)eAnyTypeUInt16 << GetwAttackPlus();
	by << (uint8_t) E_CARD_wDefencePlus << (uint8_t)eAnyTypeUInt16 << GetwDefencePlus();
	by << (uint8_t) E_CARD_wHealthPlus << (uint8_t)eAnyTypeUInt16 << GetwHealthPlus();
//	IME_DEBUG("objid %u index %u exp %u explimit %u lastexplimit %u onelevelexp %u", GetdwObjId(), GetdwIndex(), GetdwExp(), GetdwExpLimit(), GetdwLastExpLimit(), GetdwOneLevelExp());  */
// 	
}

void CCard::SerializeClient(ByteBuffer &by)
{
//	IME_DEBUG("objid %u index %u", GetdwObjId(), GetdwIndex());
/*  
    std::vector<uint8_t> vecEnum;
    vecEnum.push_back(E_CARD_dwObjId);
    vecEnum.push_back(E_CARD_dwIndex);
    vecEnum.push_back(E_CARD_byType);
    vecEnum.push_back(E_CARD_byAttribute);
    vecEnum.push_back(E_CARD_byStarLevel);
    vecEnum.push_back(E_CARD_wLevel);
    vecEnum.push_back(E_CARD_dwExp);
    vecEnum.push_back(E_CARD_dwExpLimit);
    vecEnum.push_back(E_CARD_dwEffectIndex);
//    vecEnum.push_back(E_CARD_dwAttackSkillIndex);
//    vecEnum.push_back(E_CARD_dwDeffenseSkillIndex);
    vecEnum.push_back(E_CARD_byRare);
    vecEnum.push_back(E_CARD_dwLastExpLimit);
    vecEnum.push_back(E_CARD_byFlag);
    vecEnum.push_back(E_CARD_dwSkillPara);
	vecEnum.push_back(E_CARD_dwAttack);
	vecEnum.push_back(E_CARD_dwDefence);
	vecEnum.push_back(E_CARD_dwHealth);
	vecEnum.push_back(E_CARD_dwAttackGrow);
	vecEnum.push_back(E_CARD_dwDefenceGrow);
	vecEnum.push_back(E_CARD_dwHealthGrow);
	vecEnum.push_back(E_CARD_dwExpGrow);
	vecEnum.push_back(E_CARD_dwCritRate);
//	vecEnum.push_back(E_CARD_dwDodgeRate);
//	vecEnum.push_back(E_CARD_dwAction);
	vecEnum.push_back(E_CARD_dwSuitIndex);
//	vecEnum.push_back(E_CARD_dwInitSkillIndex);
//	vecEnum.push_back(E_CARD_dwStudySkillIndex);
	vecEnum.push_back(E_CARD_dwMasterSkillIndex);
	vecEnum.push_back(E_CARD_wMasterSkillLv);
	vecEnum.push_back(E_CARD_dwSkillIndex);
	vecEnum.push_back(E_CARD_dwGrowLimit);
	vecEnum.push_back(E_CARD_dwLimitBreakNumber);
	vecEnum.push_back(E_CARD_dwLimitBreak);
	vecEnum.push_back(E_CARD_dwLimitBreakPro);
	vecEnum.push_back(E_CARD_dwAwakePro);
	vecEnum.push_back(E_CARD_dwAddExp);
	vecEnum.push_back(E_CARD_dwConsumeCoin);
	vecEnum.push_back(E_CARD_dwGetCoin);
	vecEnum.push_back(E_CARD_dwBreakExp);
	vecEnum.push_back(E_CARD_byMultiPower);
	vecEnum.push_back(E_CARD_dwMultiTime);
	vecEnum.push_back(E_CARD_dwOneLevelExp);
	vecEnum.push_back(E_CARD_dwTotalConsumeCoin);
	vecEnum.push_back(E_CARD_dwSkillPro);
	vecEnum.push_back(E_CARD_dwSkillPower);
	vecEnum.push_back(E_CARD_dwDurationExp);
	vecEnum.push_back(E_CARD_dwDurationCoin);
	vecEnum.push_back(E_CARD_dwObjDrop);
	vecEnum.push_back(E_CARD_dwStrengthRecover);
	vecEnum.push_back(E_CARD_dwEnergyRecover);
	vecEnum.push_back(E_CARD_dwAddHarm);
	vecEnum.push_back(E_CARD_dwReduceHarm);
	vecEnum.push_back(E_CARD_dwAddCritHarm);
	vecEnum.push_back(E_CARD_wLeaderLevel);
	vecEnum.push_back(E_CARD_dwLeaderIndex);
	vecEnum.push_back(E_CARD_dwCardGroup);
	vecEnum.push_back(E_CARD_dwMasterIndex);
	vecEnum.push_back(E_CARD_wAttackPlus);
	vecEnum.push_back(E_CARD_wDefencePlus);
	vecEnum.push_back(E_CARD_wHealthPlus);
    saveDataNeed(by, vecEnum);
    clearDirtyFlag();
	*/
	
	PacketData(by);	
/*
	uint8_t arrenum[] = {
    E_CARD_dwObjId,
    E_CARD_dwIndex,
    E_CARD_byType,
    E_CARD_byAttribute,
    E_CARD_byStarLevel,
    E_CARD_wLevel,
    E_CARD_dwExp,
    E_CARD_dwExpLimit,
    E_CARD_dwEffectIndex,
//  E_CARD_dwAttackSkillIndex,
//  E_CARD_dwDeffenseSkillIndex,
    E_CARD_byRare,
    E_CARD_dwLastExpLimit,
    E_CARD_byFlag,
    E_CARD_dwSkillPara,
	E_CARD_dwAttack,
	E_CARD_dwDefence,
	E_CARD_dwHealth,
	E_CARD_dwAttackGrow,
	E_CARD_dwDefenceGrow,
	E_CARD_dwHealthGrow,
	E_CARD_dwExpGrow,
	E_CARD_dwCritRate,
//	E_CARD_dwDodgeRate,
//	E_CARD_dwAction,
	E_CARD_dwSuitIndex,
//	E_CARD_dwInitSkillIndex,
//	E_CARD_dwStudySkillIndex,
	E_CARD_dwMasterSkillIndex,
	E_CARD_wMasterSkillLv,
	E_CARD_dwSkillIndex,
	E_CARD_dwGrowLimit,
	E_CARD_dwLimitBreakNumber,
	E_CARD_dwLimitBreak,
	E_CARD_dwLimitBreakPro,
	E_CARD_dwAwakePro,
	E_CARD_dwAddExp,
	E_CARD_dwConsumeCoin,
	E_CARD_dwGetCoin,
	E_CARD_dwBreakExp,
	E_CARD_byMultiPower,
	E_CARD_dwMultiTime,
	E_CARD_dwOneLevelExp,
	E_CARD_dwTotalConsumeCoin,
	E_CARD_dwSkillPro,
	E_CARD_dwSkillPower,
	E_CARD_dwDurationExp,
	E_CARD_dwDurationCoin,
	E_CARD_dwObjDrop,
	E_CARD_dwStrengthRecover,
	E_CARD_dwEnergyRecover,
	E_CARD_dwAddHarm,
	E_CARD_dwReduceHarm,
	E_CARD_dwAddCritHarm,
	E_CARD_wLeaderLevel,
	E_CARD_dwLeaderIndex,
	E_CARD_dwCardGroup,
	E_CARD_dwMasterIndex,
	E_CARD_wAttackPlus,
	E_CARD_wDefencePlus,
	E_CARD_wHealthPlus,
	};

	by << (uint8_t) (sizeof(arrenum) / sizeof(uint8_t));

    by << (uint8_t) E_CARD_dwObjId << (uint8_t)eAnyTypeUInt32 << GetdwObjId();
    by << (uint8_t) E_CARD_dwIndex << (uint8_t)eAnyTypeUInt32 << GetdwIndex();
    by << (uint8_t) E_CARD_byType << (uint8_t)eAnyTypeUInt8 << GetbyType();
    by << (uint8_t) E_CARD_byAttribute << (uint8_t)eAnyTypeUInt8 << GetbyAttribute();
    by << (uint8_t) E_CARD_byStarLevel << (uint8_t)eAnyTypeUInt8 << GetbyStarLevel();
    by << (uint8_t) E_CARD_wLevel << (uint8_t)eAnyTypeUInt16 << GetwLevel();
    by << (uint8_t) E_CARD_dwExp << (uint8_t)eAnyTypeUInt32 << GetdwExp();
    by << (uint8_t) E_CARD_dwExpLimit << (uint8_t)eAnyTypeUInt32 << GetdwExpLimit();
    by << (uint8_t) E_CARD_dwEffectIndex << (uint8_t)eAnyTypeUInt32 << GetdwEffectIndex();
    by << (uint8_t) E_CARD_byRare << (uint8_t)eAnyTypeUInt32 << GetbyRare();
    by << (uint8_t) E_CARD_dwLastExpLimit << (uint8_t)eAnyTypeUInt32 << GetdwLastExpLimit();
    by << (uint8_t) E_CARD_byFlag << (uint8_t)eAnyTypeUInt8 << GetbyFlag();
    by << (uint8_t) E_CARD_dwSkillPara << (uint8_t)eAnyTypeUInt32 << GetdwSkillPara();
	by << (uint8_t) E_CARD_dwAttack << (uint8_t)eAnyTypeUInt32 << GetdwAttack();
	by << (uint8_t) E_CARD_dwDefence << (uint8_t)eAnyTypeUInt32 << GetdwDefence();
	by << (uint8_t) E_CARD_dwHealth << (uint8_t)eAnyTypeUInt32 << GetdwHealth();
	by << (uint8_t) E_CARD_dwAttackGrow << (uint8_t)eAnyTypeUInt32 << GetdwAttackGrow();
	by << (uint8_t) E_CARD_dwDefenceGrow << (uint8_t)eAnyTypeUInt32 << GetdwDefenceGrow();
	by << (uint8_t) E_CARD_dwHealthGrow << (uint8_t)eAnyTypeUInt32 << GetdwHealthGrow();
	by << (uint8_t) E_CARD_dwExpGrow << (uint8_t)eAnyTypeUInt32 << GetdwExpGrow();
	by << (uint8_t) E_CARD_dwCritRate << (uint8_t)eAnyTypeUInt32 << GetdwCritRate();
	by << (uint8_t) E_CARD_dwSuitIndex << (uint8_t)eAnyTypeUInt32 << GetdwSuitIndex();
	by << (uint8_t) E_CARD_dwMasterSkillIndex << (uint8_t)eAnyTypeUInt32 << GetdwMasterSkillIndex();
	by << (uint8_t) E_CARD_wMasterSkillLv << (uint8_t)eAnyTypeUInt16 << GetwMasterSkillLv();
	by << (uint8_t) E_CARD_dwSkillIndex << (uint8_t)eAnyTypeUInt32 << GetdwSkillIndex();
	by << (uint8_t) E_CARD_dwGrowLimit << (uint8_t)eAnyTypeUInt32 << GetdwGrowLimit();
	by << (uint8_t) E_CARD_dwLimitBreakNumber << (uint8_t)eAnyTypeUInt32 << GetdwLimitBreakNumber();
	by << (uint8_t) E_CARD_dwLimitBreak << (uint8_t)eAnyTypeUInt32 << GetdwLimitBreak();
	by << (uint8_t) E_CARD_dwLimitBreakPro << (uint8_t)eAnyTypeUInt32 << GetdwLimitBreakPro();
	by << (uint8_t) E_CARD_dwAwakePro << (uint8_t)eAnyTypeUInt32 << GetdwAwakePro();
	by << (uint8_t) E_CARD_dwAddExp << (uint8_t)eAnyTypeUInt32 << GetdwAddExp();
	by << (uint8_t) E_CARD_dwConsumeCoin << (uint8_t)eAnyTypeUInt32 << GetdwConsumeCoin();
	by << (uint8_t) E_CARD_dwGetCoin << (uint8_t)eAnyTypeUInt32 << GetdwGetCoin();
	by << (uint8_t) E_CARD_dwBreakExp << (uint8_t)eAnyTypeUInt32 << GetdwBreakExp();
	by << (uint8_t) E_CARD_byMultiPower << (uint8_t)eAnyTypeUInt8 << GetbyMultiPower();
	by << (uint8_t) E_CARD_dwMultiTime << (uint8_t)eAnyTypeUInt32 << GetdwMultiTime();
	by << (uint8_t) E_CARD_dwOneLevelExp << (uint8_t)eAnyTypeUInt32 << GetdwOneLevelExp();
	by << (uint8_t) E_CARD_dwTotalConsumeCoin << (uint8_t)eAnyTypeUInt32 << GetdwTotalConsumeCoin();
	by << (uint8_t) E_CARD_dwSkillPro << (uint8_t)eAnyTypeUInt32 << GetdwSkillPro();
	by << (uint8_t) E_CARD_dwSkillPower << (uint8_t)eAnyTypeUInt32 << GetdwSkillPower();
	by << (uint8_t) E_CARD_dwDurationExp << (uint8_t)eAnyTypeUInt32 << GetdwDurationExp();
	by << (uint8_t) E_CARD_dwDurationCoin << (uint8_t)eAnyTypeUInt32 << GetdwDurationCoin();
	by << (uint8_t) E_CARD_dwObjDrop << (uint8_t)eAnyTypeUInt32 << GetdwObjDrop();
	by << (uint8_t) E_CARD_dwStrengthRecover << (uint8_t)eAnyTypeUInt32 << GetdwStrengthRecover();
	by << (uint8_t) E_CARD_dwEnergyRecover << (uint8_t)eAnyTypeUInt32 << GetdwEnergyRecover();
	by << (uint8_t) E_CARD_dwAddHarm << (uint8_t)eAnyTypeUInt32 << GetdwAddHarm();
	by << (uint8_t) E_CARD_dwReduceHarm << (uint8_t)eAnyTypeUInt32 << GetdwReduceHarm();
	by << (uint8_t) E_CARD_dwAddCritHarm << (uint8_t)eAnyTypeUInt32 << GetdwAddCritHarm();
	by << (uint8_t) E_CARD_wLeaderLevel << (uint8_t)eAnyTypeUInt16 << GetwLeaderLevel();
	by << (uint8_t) E_CARD_dwLeaderIndex << (uint8_t)eAnyTypeUInt32 << GetdwLeaderIndex();
	by << (uint8_t) E_CARD_dwCardGroup << (uint8_t)eAnyTypeUInt32 << GetdwCardGroup();
	by << (uint8_t) E_CARD_dwMasterIndex << (uint8_t)eAnyTypeUInt32 << GetdwMasterIndex();
	by << (uint8_t) E_CARD_wAttackPlus << (uint8_t)eAnyTypeUInt16 << GetwAttackPlus();
	by << (uint8_t) E_CARD_wDefencePlus << (uint8_t)eAnyTypeUInt16 << GetwDefencePlus();
	by << (uint8_t) E_CARD_wHealthPlus << (uint8_t)eAnyTypeUInt16 << GetwHealthPlus();
//	IME_DEBUG("objid %u index %u exp %u explimit %u lastexplimit %u onelevelexp %u", GetdwObjId(), GetdwIndex(), GetdwExp(), GetdwExpLimit(), GetdwLastExpLimit(), GetdwOneLevelExp());  */
//	*/
}

void CCard::SerializeDB(ByteBuffer &by)
{
	int exp = 0;
	exp = (int)m_dwExp - m_dwLastExpLimit;
	exp = exp < 0 ? 0 : exp;
	SetdwOneLevelExp(exp);
/*  
    std::vector<uint8_t> vecEnum;
    vecEnum.push_back(E_CARD_dwObjId);
    vecEnum.push_back(E_CARD_dwIndex);
//    vecEnum.push_back(E_CARD_dwExp);
    vecEnum.push_back(E_CARD_wLevel);
	vecEnum.push_back(E_CARD_dwLimitBreak);
	vecEnum.push_back(E_CARD_byRare);
	vecEnum.push_back(E_CARD_dwOneLevelExp);
	vecEnum.push_back(E_CARD_dwTotalConsumeCoin);
	vecEnum.push_back(E_CARD_wLeaderLevel);
	vecEnum.push_back(E_CARD_wAttackPlus);
	vecEnum.push_back(E_CARD_wDefencePlus);
	vecEnum.push_back(E_CARD_wHealthPlus);
    saveDataNeed(by, vecEnum);
    clearDirtyFlag();
//	IME_DEBUG("ObjId %u index %u level %u OneExp %u", GetdwObjId(), GetdwIndex(), GetwLevel(), GetdwOneLevelExp());
*/
	by << (uint8_t) (11);

    by << (uint8_t) E_CARD_dwObjId << (uint8_t)eAnyTypeUInt32 << GetdwObjId();
    by << (uint8_t) E_CARD_dwIndex << (uint8_t)eAnyTypeUInt32 << GetdwIndex();
	by << (uint8_t) E_CARD_wLevel << (uint8_t)eAnyTypeUInt16 << GetwLevel();
	by << (uint8_t) E_CARD_dwLimitBreak << (uint8_t)eAnyTypeUInt32 << GetdwLimitBreak();
	by << (uint8_t) E_CARD_byRare << (uint8_t)eAnyTypeUInt32 << GetbyRare();
	by << (uint8_t) E_CARD_dwOneLevelExp << (uint8_t)eAnyTypeUInt32 << GetdwOneLevelExp();
	by << (uint8_t) E_CARD_dwTotalConsumeCoin << (uint8_t)eAnyTypeUInt32 << GetdwTotalConsumeCoin();
	by << (uint8_t) E_CARD_wLeaderLevel << (uint8_t)eAnyTypeUInt16 << GetwLeaderLevel();
	by << (uint8_t) E_CARD_wAttackPlus << (uint8_t)eAnyTypeUInt16 << GetwAttackPlus();
	by << (uint8_t) E_CARD_wDefencePlus << (uint8_t)eAnyTypeUInt16 << GetwDefencePlus();
	by << (uint8_t) E_CARD_wHealthPlus << (uint8_t)eAnyTypeUInt16 << GetwHealthPlus();
}

void CCard::DeserializeDB(ByteBuffer &by)
{
    loadDataLoose(by);
    clearDirtyFlag();
    SetdwObjId(getProperty(E_CARD_dwObjId).get<uint32>());
    SetdwIndex(getProperty(E_CARD_dwIndex).get<uint32>());
//    SetdwExp(getProperty(E_CARD_dwExp).get<uint32>());
	SetwLevel(getProperty(E_CARD_wLevel).get<uint16>());
	SetdwLimitBreak(getProperty(E_CARD_dwLimitBreak).get<uint32>());
	SetbyRare(getProperty(E_CARD_byRare).get<uint8>());
	SetdwOneLevelExp(getProperty(E_CARD_dwOneLevelExp).get<uint32>());
	SetdwTotalConsumeCoin(getProperty(E_CARD_dwTotalConsumeCoin).get<uint32>());
	SetwLeaderLevel(getProperty(E_CARD_wLeaderLevel).get<uint16>());
	SetwAttackPlus(getProperty(E_CARD_wAttackPlus).get<uint16>());
	SetwDefencePlus(getProperty(E_CARD_wDefencePlus).get<uint16>());
	SetwHealthPlus(getProperty(E_CARD_wHealthPlus).get<uint16>());
//	IME_DEBUG("ObjId %u index %u level %u OneExp %u", GetdwObjId(), GetdwIndex(), GetwLevel(), GetdwOneLevelExp());
}

int CCard::Upgrade(uint32_t index)
{
	SetdwIndex(index);
	SetwLevel(1);
	SetdwOneLevelExp(0);

	uint32_t exp = GetdwExp();

	if (LoadConf())
	{
		IME_ERROR("upgrade fail index %u", index);
		return -1;
	}
	SetdwExp(exp);
	if (Calculate())
	{
		IME_ERROR("cal fail index %u", index);
		return -1;
	}
	return 0;
}

int CCard::LoadConf()
{
    STC_CONF_CARD_BASE *ptemp = CConfCardBase::Find(m_dwIndex);
    if (ptemp == NULL)
    {
        IME_ERROR("get csv data from CardBase fail [%u]", m_dwIndex);
        return -1;
    }
    SetbyType(ptemp->byType);
    SetbyAttribute(ptemp->byAttribute);
	SetbyStarLevel(ptemp->byStarLevel);
	
	SetdwEffectIndex(ptemp->dwEffectIndex);
	SetdwAttackGrow(ptemp->dwAttackGrow);
	SetdwDefenceGrow(ptemp->dwDefenceGrow);
	SetdwHealthGrow(ptemp->dwHealthGrow);
	SetdwExpGrow(ptemp->dwExpGrow);
	SetdwCritRate(ptemp->dwCritRate);
	SetdwSuitIndex(ptemp->dwSuitIndex);	
	SetdwMasterSkillIndex(ptemp->dwMasterSkillIndex);
	SetdwSkillIndex(ptemp->dwSkillIndex);
	SetdwGrowLimit(ptemp->dwGrowLimit);
	SetdwLimitBreakNumber(ptemp->dwLimitBreakNumber);
	SetdwLimitBreakPro(ptemp->dwLimitBreakPro);
	SetdwAwakePro(ptemp->dwAwakePro);
	SetdwAddExp(ptemp->dwAddExp);
	SetdwConsumeCoin(ptemp->dwConsumeCoin);
	SetdwGetCoin(ptemp->dwGetCoin);
	SetdwBreakExp(ptemp->dwBreakExp);
	SetbyMultiPower(ptemp->byMultiPower);
	SetdwMultiTime(ptemp->dwMultiTime);
	SetdwLeaderIndex(ptemp->dwLeaderSkill);
	SetdwCardGroup(ptemp->dwCardGroup);
	SetdwMasterIndex(ptemp->dwMasterIndex);
	
	if (GetwLeaderLevel() == 0)
		SetwLeaderLevel(1);
	if (GetwLevel() == 0)
		SetwLevel(1);
	uint16_t level = GetwLevel();
	STC_CONF_CARD_UP *pConfCardUp;
	if (level == 0)
	{
		IME_ERROR("level is zero userid %u", m_pCRole->GetdwAccountId());
		return -1;
	}
	else if(level <= 1)
	{
		SetdwLastExpLimit(0);
		m_dwExp = m_dwOneLevelExp;
		SetdwExp(m_dwExp);
	}
	else
	{
		uint32_t lastexplimit = CConfCardUp::GetExp(GetbyStarLevel(), GetdwExpGrow(), level - 1);
		SetdwLastExpLimit(lastexplimit);
		m_dwExp = GetdwLastExpLimit() + m_dwOneLevelExp;
		SetdwExp(m_dwExp);
	}
	uint32_t lastexplimit = CConfCardUp::GetExp(GetbyStarLevel(), GetdwExpGrow(), level);
	SetdwExpLimit(lastexplimit);

//	if (m_pCRole != NULL && m_pCRole->GetCMission() != NULL)
//	{
//		if (GetbyType() == E_ITEM_PET_CARD || GetbyType() == E_ITEM_MASTER_CARD)
//			m_pCRole->GetCMission()->CardLevel(0, GetwLevel());
//	}

	return 0;
}

//int CCard::CalculateRelate(bool IsUpdate)
//{
//	int ret;
//    ret = Calculate(IsUpdate);
//	if (ret != 0)
//		return ret;
//    ret = m_pCRole->GetclsCardPacket()->Calculate(IsUpdate); 
//	return ret;
//}

bool CCard::IsFullLevel()
{
	return (GetbyFlag() & 0x1);
}

int CCard::Calculate(bool IsUpdate, bool IsLevelChange, bool IsCardLevel)
{
//	if (GetbyType() == E_ITEM_SKILL_CARD)
//	{
//
//	}
//	else
//	{

		uint32_t lastexplimit;
		uint32_t maxlevel;
		STC_CONF_CARD_BASE *ptemp = CConfCardBase::Find(m_dwIndex);
		if (ptemp == NULL)
		{
			IME_ERROR("get csv data from CardBase fail [%u]", m_dwIndex);
			return -1;
		}

		//	IME_DEBUG("level 1, attack min %u, max %u, defence min %u, max %u", GetdwAttackMin(), GetdwAttackMax(), GetdwDefenseMin(), GetdwDefenseMax());
		uint16_t level = m_wLevel;
		maxlevel = ptemp->dwGrowLimit + GetdwLimitBreak() * BREAK_LIMIT_PARA + (GetbyRare() == E_CR_AWAKE ? AWAKE_LEVEL_PARA : 0); 
		if (IsLevelChange)
		{
			level = CConfCardUp::FindLevel(GetbyStarLevel(), GetdwExp(), GetdwExpGrow());
			if (level == 0)
			{
				IME_ERROR("level is zero star %u, exp %u", GetbyStarLevel(), GetdwExp());
				return -1;
			}

			maxlevel = ptemp->dwGrowLimit + GetdwLimitBreak() * BREAK_LIMIT_PARA + (GetbyRare() == E_CR_AWAKE ? AWAKE_LEVEL_PARA : 0); 
			if (maxlevel == 0)
				maxlevel = 1;
//		IME_DEBUG("objid %u maxlevel %u", m_dwObjId, maxlevel);
			if (level > maxlevel)
				level = maxlevel;
		}

		if (level < GetwLevel())
		{
			IME_ERROR("level down it is error csv happen error up level %u now level %u", level, GetwLevel());
			level = GetwLevel();
		}

//		IME_DEBUG("level %u", level);
		STC_CONF_CARD_UP *pConfCardUp, *pConfCardUpNow;
		if (level <= 1)
		{
			lastexplimit = 0;
		}
		else
		{
//			IME_DEBUG("level %u", level);
//			STC_CONF_CARD_UP *pConfCardUpLast;
//			if ((pConfCardUpLast = CConfCardUp::Find(level - 1)) == NULL)
//			{
//				IME_ERROR("can not find record(star %u, level %u) in card up csv", GetbyStarLevel(), level - 1);
//				return -1;
//			}
			lastexplimit = CConfCardUp::GetExp(GetbyStarLevel(), GetdwExpGrow(), level - 1);
		}

		if ((pConfCardUpNow = CConfCardUp::Find(level)) == NULL)
		{
			IME_ERROR("can not find record(star %u, level %u) in card up csv", GetbyStarLevel(), level);
			return -1;
		}

		//	for (int i = 1; i < level; i++)
		//	{
		//		if ((pConfCardUp = CConfCardUp::Find(GetbyStarLevel(), i)) == NULL)
		//		{
		//			IME_ERROR("can not find record(star %u, level %u) in card up csv", GetbyStarLevel(), i);
		//			return -1;
		//		}
		//		SetdwAttackMin(GetdwAttackMin() * (100 + pConfCardUp->dwAttackMin) / 100.0);
		//		SetdwAttackMax(GetdwAttackMax() * (100 + pConfCardUp->dwAttackMax) / 100.0);
		//		SetdwDefenseMin(GetdwDefenseMin() * (100 + pConfCardUp->dwDefenseMin) / 100.0);
		//		SetdwDefenseMax(GetdwDefenseMax() * (100 + pConfCardUp->dwDefenseMax) / 100.0);
		//	}

		SetwLevel(level);
//		SetdwExpLimit(pConfCardUpNow->dwExp * GetdwExpGrow() / 100.0);
		SetdwExpLimit(CConfCardUp::GetExp(GetbyStarLevel(), GetdwExpGrow(), level));
		SetdwLastExpLimit(lastexplimit);
		
		uint16_t masterskilllv = level / 10 + 1 >= 9 ? 9 : level / 10 + 1;
		SetwMasterSkillLv(masterskilllv);

		uint32_t AwakeVal = 0;
		uint32_t AwakeValBase = 0;
		if (GetbyRare())
		{
			AwakeVal = AWAKE_PARA;
			AwakeValBase = AWAKE_BASE_PARA;
		}
		uint32_t BreakUp[] = BREAK_ATTRIBUTE_UP;
		uint32_t breakattrup = BreakUp[0];
		if (GetbyStarLevel() >= 1 && GetbyStarLevel() <= 7)
		{
			breakattrup = BreakUp[GetbyStarLevel()];
		}
		else
		{
			IME_ERROR("break attr up fail, card star level is not in 1-7 now %u", GetbyStarLevel());
		}

		uint32_t attack = ptemp->dwAttackBase * (100 + AwakeValBase) / 100.0 + (level - 1) * GetdwAttackGrow() * (100 + AwakeVal) / 100.0;
		attack *= (100 + breakattrup * GetdwLimitBreak()) / 100.0;
		SetdwAttack(attack);
		uint32_t defence = ptemp->dwDefenceBase * (100 + AwakeValBase) / 100.0 + (level - 1) * GetdwDefenceGrow() * (100 + AwakeVal) / 100.0;
		defence *= (100 + breakattrup * GetdwLimitBreak()) / 100.0;
//		IME_DEBUG("defencebase %u, level %u, GetdwDefenceGrow %u, AwakeVal %u", ptemp->dwDefenceBase, level, GetdwDefenceGrow(), AwakeVal);
		SetdwDefence(defence);
		uint32_t health = ptemp->dwHealthBase * (100 + AwakeValBase) / 100.0 + (level - 1) * GetdwHealthGrow() * (100 + AwakeVal) / 100.0;
		health *= (100 + breakattrup * GetdwLimitBreak()) / 100.0;
		SetdwHealth(health);

		if (GetwLevel() >= maxlevel)
		{
			SetbyFlag(GetbyFlag() | 0x1);
//			SetdwExpLimit(lastexplimit);
		}
		else
		{
			SetbyFlag(GetbyFlag() & 0xfe);
		}

//		IME_DEBUG("star %u lv %u index %u exp %u expgrow %u explimit %u, maxlevel %u, byFlag %d", GetbyStarLevel(), level, m_dwIndex, m_dwExp, GetdwExpGrow(), GetdwExpLimit(), maxlevel, m_byFlag);

		if (GetdwEffectIndex() != 0)
		{
//			STC_CONF_JEWEL_SKILL_STRENGTH *pJewel = CConfJewelSkillStrength::Find(GetdwIndex());
//			if (pJewel == NULL)
//			{
//				IME_ERROR("can not find this card csv %u, effect index %u", GetdwIndex(), GetdwEffectIndex());
//				return -1;
//			}
			STC_CONF_JEWEL_SKILL *pJewelInit = CConfJewelSkill::Find(GetdwEffectIndex());
			if (pJewelInit == NULL)
			{
				IME_ERROR("can not find this jewel card csv %u, effect index %u", GetdwIndex(), GetdwEffectIndex());
				return -1;
			}
			SetdwSkillPara((GetwLevel() - 1) * pJewelInit->dwGrow + pJewelInit->para1);
//			SetdwSkillPara((GetwLevel() - 1) * pJewel->dwSkillPara);
		}
		
		CCardPacket::itemequip_t temp;
		temp.Attack = m_dwAttack;
		temp.Defence = m_dwDefence;
		temp.Health = m_dwHealth;
		temp.Crit = m_dwCritRate;
		temp.Attribute = m_byAttribute;

		std::vector<int> vecTmp;
		STC_CONF_CARD_BASE *pConfCard = CConfCardBase::Find(GetdwIndex());
		if (pConfCard == NULL)
			return -1;
		vecTmp.push_back(pConfCard->dwBreakSkill1);
		vecTmp.push_back(pConfCard->dwBreakSkill2);
		vecTmp.push_back(pConfCard->dwBreakSkill3);
		vecTmp.push_back(pConfCard->dwBreakSkill4);
		vecTmp.push_back(pConfCard->dwBreakSkill5);

		CExSkill::Init();
		for (int ii = 0; ii < min((uint32_t)CARD_BREAK_AWARD_COUNT, GetdwLimitBreak() / 2); ii++)
		{
			if (vecTmp[ii] != 0)
			{
				if (CExSkill::ExecExSkillFunc(vecTmp[ii], &temp))
				{
					IME_ERROR("card break skill exec fail");
					return -1;
				}
			}
		}
		SetdwAttack(temp.Attack);
		SetdwDefence(temp.Defence);
		SetdwHealth(temp.Health);
		SetdwCritRate(temp.Crit);
		SetbyAttribute(temp.Attribute);
		SetdwSkillPro(temp.SkillPro);
		SetdwSkillPower(temp.SkillPower);
		SetdwDurationExp(temp.DurationExp);
		SetdwDurationCoin(temp.DurationCoin);
		SetdwObjDrop(temp.ObjDrop);
		SetdwStrengthRecover(temp.StrengthRecover);
		SetdwEnergyRecover(temp.EnergyRecover);
		SetdwAddHarm(temp.AddHarm);
		SetdwReduceHarm(temp.ReduceHarm);
		SetdwAddCritHarm(temp.AddCritHarm);

		uint16_t attackplus = GetwAttackPlus() > GlobalConfig::PlusMaxlimit ?  GlobalConfig::PlusMaxlimit : GetwAttackPlus();
		uint16_t defenceplus = GetwDefencePlus() > GlobalConfig::PlusMaxlimit ? GlobalConfig::PlusMaxlimit : GetwDefencePlus();
		uint16_t healthplus = GetwHealthPlus() > GlobalConfig::PlusMaxlimit ? GlobalConfig::PlusMaxlimit : GetwHealthPlus();
		SetdwAttack(GetdwAttack() + attackplus * GlobalConfig::PlusEveryVal);
		SetdwDefence(GetdwDefence() + defenceplus * GlobalConfig::PlusEveryVal);
		SetdwHealth(GetdwHealth() + healthplus * GlobalConfig::PlusEveryVal);

//	IME_DEBUG("level %u, attack min %u, max %u, defence min %u, max %u, exp %u, explimit %u", level, GetdwAttackMin(), GetdwAttackMax(), GetdwDefenseMin(), GetdwDefenseMax(), GetdwExp(), GetdwExpLimit());

//	}
	
		if (IsCardLevel)
		{
			if (m_pCRole != NULL && m_pCRole->GetCMission() != NULL)
			{
				if (GetbyType() == E_ITEM_PET_CARD || GetbyType() == E_ITEM_MASTER_CARD)
					m_pCRole->GetCMission()->CardLevel(0, GetwLevel());
			}
		}

    if (IsUpdate)
        SendProUpdate();
	return 0;
}

//void CCard::SendProUpdateRelate()
//{
//    SendProUpdate();
//    m_pCRole->SendProUpdate();
//}

void CCard::SendProUpdate()
{
	WorldPacket data(CMD_SC_CARD_PROPERTY);
	data << GetdwObjId();
//	saveDataDirty(data);
//	clearDirtyFlag();
	PacketData(data);
	if (data.size() > 1)
	{
		m_pCRole->SendPacket(&data);
	}
}

//void CCard::SetCard(uint32_t dwObjId,                           
//        uint8_t byType,                             
//        uint32_t dwIndex,                           
//        uint8_t byAttribute, //卡牌属性             
//        uint8_t byStarLevel, //卡牌星级             
//        uint16_t wLevel, //卡牌等级                 
//        uint32_t dwExp, //卡牌经验                  
//        uint32_t dwExpLimit, //下一级升级上限       
//        uint32_t dwAttackMin, //攻击力最小          
//        uint32_t dwAttackMax,                       
//        uint32_t dwDefenseMin,                      
//        uint32_t dwDefenseMax,                      
//        uint32_t dwEffectIndex,                     
//        uint32_t dwAttackSkillIndex,                
//        uint32_t dwDeffenseSkillIndex,              
//        uint32_t byRare,                            
//        uint32_t dwSuit1Index,                      
//        uint32_t dwSuit2Index,                      
//        uint32_t dwSuit3Index,                      
//        uint32_t dwSuit4Index,                       
//		uint32_t dwLastExplimit,
//		uint8_t byFlag
// )
//{
//    SetdwObjId(dwObjId);
//    SetdwIndex(dwIndex);
//    SetbyType(byType);
//    SetbyAttribute(byAttribute);
//    SetbyStarLevel(byStarLevel);
//    SetwLevel(wLevel);
//    SetdwExp(dwExp);
//    SetdwExpLimit(dwExpLimit);
//    SetdwAttackMin(dwAttackMin);
//    SetdwAttackMax(dwAttackMax);
//    SetdwDefenseMin(dwDefenseMin);
//    SetdwDefenseMax(dwDefenseMax);
//    SetdwEffectIndex(dwEffectIndex);
//    SetdwAttackSkillIndex(dwAttackSkillIndex);
//    SetdwDeffenseSkillIndex(dwDeffenseSkillIndex);
//    SetbyRare(byRare);
//    SetdwSuit1Index(dwSuit1Index);
//    SetdwSuit2Index(dwSuit2Index);
//    SetdwSuit3Index(dwSuit3Index);
//    SetdwSuit4Index(dwSuit4Index); 
//	SetdwLastExpLimit(dwLastExplimit);
//	SetbyFlag(byFlag);
//}

void CCard::CardEquip1(bool IsUpdate)
{
	SetbyFlag(GetbyFlag() | 0x02);
    if (IsUpdate)
        SendProUpdate();
}

void CCard::CardUnEquip1(bool IsUpdate)
{
	SetbyFlag(GetbyFlag() & 0xfd);

    if (IsUpdate)
        SendProUpdate();
}

//void CCard::CardEquip2(bool IsUpdate)
//{
//	SetbyFlag(GetbyFlag() | 0x04);
//    if (IsUpdate)
//        SendProUpdate();
//}
//
//void CCard::CardUnEquip2(bool IsUpdate)
//{
//	SetbyFlag(GetbyFlag() & 0xfb);
//
//    if (IsUpdate)
//        SendProUpdate();
//}

//int CCard::InitCardSkill()
//{
//	uint32_t SkillIndex;
//	if (GetbyType() == E_ITEM_PET_CARD)
//	{
//		if (GetbyStarLevel() > 1)
//		{
//			if (GetbyStarLevel() == 2)
//			{
//				SkillIndex = CConfSkillCreate::GetSkillIndex(GetbyStarLevel());	
//				if (SkillIndex == 0)
//					return -1;
//				SetdwAttackSkillIndex(SkillIndex);
//			}
//			else
//			{
//				SkillIndex = CConfSkillCreate::GetSkillIndex(GetbyStarLevel());	
//				if (SkillIndex == 0)
//					return -1;
//				SetdwAttackSkillIndex(SkillIndex);
//				SkillIndex = CConfSkillCreate::GetSkillIndex(GetbyStarLevel());	
//				if (SkillIndex == 0)
//					return -1;
//				SetdwDeffenseSkillIndex((SkillIndex));
//			}
//		}
//	}
//	return 0;
//}
//
int CCard::NpcCalculate()
{
	SetwMasterSkillLv(1);

	uint32_t lastexplimit;
	uint32_t maxlevel = 100;
	STC_CONF_CARD_BASE *ptemp = CConfCardBase::Find(m_dwIndex);
	if (ptemp == NULL)
	{
		IME_ERROR("get csv data from CardBase fail [%u]", m_dwIndex);
		return -1;
	}

	//	IME_DEBUG("level 1, attack min %u, max %u, defence min %u, max %u", GetdwAttackMin(), GetdwAttackMax(), GetdwDefenseMin(), GetdwDefenseMax());
	uint16_t level = CConfCardUp::FindLevel(GetbyStarLevel(), GetdwExp(), GetdwExpGrow());
	if (level == 0)
	{
		IME_ERROR("level is zero star %u, exp %u", GetbyStarLevel(), GetdwExp());
		return -1;
	}

	STC_CONF_CARD_UP *pConfCardUp, *pConfCardUpNow;
	if (level <= 1)
	{
		lastexplimit = 0;
	}
	else
	{
		lastexplimit = CConfCardUp::GetExp(GetbyStarLevel(), GetdwExpGrow(), level - 1);
	}

	if ((pConfCardUpNow = CConfCardUp::Find(level)) == NULL)
	{
		IME_ERROR("can not find record(star %u, level %u) in card up csv", GetbyStarLevel(), level);
		return -1;
	}

	SetwLevel(level);
	SetdwExpLimit(CConfCardUp::GetExp(GetbyStarLevel(), GetdwExpGrow(), level));
	SetdwLastExpLimit(lastexplimit);

//	uint32_t AwakeVal = 0;
//	if (GetbyRare())
//	{
//		AwakeVal *= AwakeVal * AWAKE_PARA;
//	}
//	uint32_t attack = ptemp->dwAttackBase + level * GetdwAttackGrow() * (100 + AwakeVal) / 100.0;
//	SetdwAttack(attack);
//	uint32_t defence = ptemp->dwDefenceBase + level * GetdwDefenceGrow() * (100 + AwakeVal) / 100.0;
//	SetdwDefence(defence);
//	uint32_t health = ptemp->dwHealthBase + level * GetdwHealthGrow() * (100 + AwakeVal) / 100.0;
//	SetdwHealth(health);

	uint32_t AwakeVal = 0;
	uint32_t AwakeValBase = 0;
	if (GetbyRare())
	{
		AwakeVal = AWAKE_PARA;
		AwakeValBase = AWAKE_BASE_PARA;
	}

	uint32_t BreakUp[] = BREAK_ATTRIBUTE_UP;
	uint32_t breakattrup = BreakUp[0];
	if (GetbyStarLevel() >= 1 && GetbyStarLevel() <= 7)
	{
		breakattrup = BreakUp[GetbyStarLevel()];
	}
	else
	{
		IME_ERROR("break attr up fail, card star level is not in 1-7 now %u", GetbyStarLevel());
	}

	uint32_t attack = ptemp->dwAttackBase * (100 + AwakeValBase) / 100.0 + (level - 1) * GetdwAttackGrow() * (100 + AwakeVal) / 100.0;
	attack *= (100 + breakattrup * GetdwLimitBreak()) / 100.0;
	SetdwAttack(attack);
	uint32_t defence = ptemp->dwDefenceBase * (100 + AwakeValBase) / 100.0 + (level - 1) * GetdwDefenceGrow() * (100 + AwakeVal) / 100.0;
	defence *= (100 + breakattrup * GetdwLimitBreak()) / 100.0;
	//		IME_DEBUG("defencebase %u, level %u, GetdwDefenceGrow %u, AwakeVal %u", ptemp->dwDefenceBase, level, GetdwDefenceGrow(), AwakeVal);
	SetdwDefence(defence);
	uint32_t health = ptemp->dwHealthBase * (100 + AwakeValBase) / 100.0 + (level - 1) * GetdwHealthGrow() * (100 + AwakeVal) / 100.0;
	health *= (100 + breakattrup * GetdwLimitBreak()) / 100.0;
	SetdwHealth(health);

	if (GetwLevel() >= maxlevel)
	{
		SetbyFlag(GetbyFlag() | 0x1);
		SetdwExpLimit(lastexplimit);
	}

	if (GetdwEffectIndex() != 0)
	{
//		STC_CONF_JEWEL_SKILL_STRENGTH *pJewel = CConfJewelSkillStrength::Find(GetdwIndex());
//		if (pJewel == NULL)
//		{
//			IME_ERROR("can not find this card csv %u, effect index %u", GetdwIndex(), GetdwEffectIndex());
//			return -1;
//		}
		STC_CONF_JEWEL_SKILL *pJewelInit = CConfJewelSkill::Find(GetdwEffectIndex());
		if (pJewelInit == NULL)
		{
			IME_ERROR("can not find this jewel card csv %u, effect index %u", GetdwIndex(), GetdwEffectIndex());
			return -1;
		}
		SetdwSkillPara((GetwLevel() - 1) * pJewelInit->dwGrow + pJewelInit->para1);
	}

	uint16_t attackplus = GetwAttackPlus() > GlobalConfig::PlusMaxlimit ?  GlobalConfig::PlusMaxlimit : GetwAttackPlus();
	uint16_t defenceplus = GetwDefencePlus() > GlobalConfig::PlusMaxlimit ? GlobalConfig::PlusMaxlimit : GetwDefencePlus();
	uint16_t healthplus = GetwHealthPlus() > GlobalConfig::PlusMaxlimit ? GlobalConfig::PlusMaxlimit : GetwHealthPlus();
	SetdwAttack(GetdwAttack() + attackplus * GlobalConfig::PlusEveryVal);
	SetdwDefence(GetdwDefence() + defenceplus * GlobalConfig::PlusEveryVal);
	SetdwHealth(GetdwHealth() + healthplus * GlobalConfig::PlusEveryVal);

	//	IME_DEBUG("level %u, attack min %u, max %u, defence min %u, max %u, exp %u, explimit %u", level, GetdwAttackMin(), GetdwAttackMax(), GetdwDefenseMin(), GetdwDefenseMax(), GetdwExp(), GetdwExpLimit());
	return 0;
}

int CCard::SetRealLv(uint32_t lv)
{
	if (lv <= 1)
		return 0;
	uint32_t maxlevel = GetdwGrowLimit() + GetdwLimitBreak() * BREAK_LIMIT_PARA + (GetbyRare() == E_CR_AWAKE ? AWAKE_LEVEL_PARA : 0); 
	if (maxlevel <= 1)
		return 0;
	if (lv > maxlevel)
		lv = maxlevel;
	uint32_t exp = CConfCardUp::GetExp(GetbyStarLevel(), GetdwExpGrow(), lv - 1);
	SetdwExp(exp);
	return Calculate();
}

int CCard::AddExp(uint32_t exp)
{
	int maxlevel = GetdwGrowLimit() + GetdwLimitBreak() * BREAK_LIMIT_PARA + (GetbyRare() == E_CR_AWAKE ? AWAKE_LEVEL_PARA : 0); 
	IME_DEBUG("now exp %u, maxlevel %u, maxexp %u", GetdwExp(), maxlevel, CConfCardUp::GetExp(GetbyStarLevel(), GetdwExpGrow(), maxlevel));
	uint32_t texp = CConfCardUp::GetExp(GetbyStarLevel(), GetdwExpGrow(), maxlevel - 1);
	if (GetdwExp() + exp > texp && texp > GetdwExp())
	{
		IME_LOG("exp overload maxexp");
		SetdwExp(texp);
	}
	else if (GetdwExp() + exp <= texp)
	{
		SetdwExp(GetdwExp() + exp);
	}
	IME_DEBUG("then exp %u", GetdwExp());
	return 0;
}

int CCard::CreateCard(uint32_t index, uint16_t lv, uint32_t objid)
{
	SetdwIndex(index);
	SetdwObjId(objid);
	if (LoadConf())
		return -1;
//	STC_CONF_CARD_UP *pConf = CConfCardUp::Find(GetbyStarLevel(), lv);
//	if (pConf == NULL)
//	{
//		IME_ERROR("star level %u, level %u", GetbyStarLevel(), lv);
//		return -1;
//	}
//	SetdwExp(pConf->dwExp * GetdwExpGrow() / 100.0);
	if (lv > 1)
	{
		SetdwExp(CConfCardUp::GetExp(GetbyStarLevel(), GetdwExpGrow(), lv - 1));
	}

//	IME_DEBUG("exp %u", CConfCardUp::GetExp(GetbyStarLevel(), GetdwExpGrow(), lv));
	if (NpcCalculate())
		return -1;
	return 0;
}

void CCard::ChangeAttackPlus(uint32_t attack)
{
	uint32_t attackplus = GetwAttackPlus() + attack;
	attackplus = attackplus > GlobalConfig::PlusMaxlimit ? GlobalConfig::PlusMaxlimit : attackplus;
	if (GetwAttackPlus() < attackplus)
		SetwAttackPlus(attackplus);
}

void CCard::ChangeDefencePlus(uint32_t defence)
{
	uint32_t defenceplus = GetwDefencePlus() + defence;
	defenceplus = defenceplus > GlobalConfig::PlusMaxlimit ? GlobalConfig::PlusMaxlimit : defenceplus;
	if (GetwDefencePlus() < defenceplus)
		SetwDefencePlus(defenceplus);
}

void CCard::ChangeHealthPlus(uint32_t health)
{
	uint32_t healthplus = GetwHealthPlus() + health;
	healthplus = healthplus > GlobalConfig::PlusMaxlimit ? GlobalConfig::PlusMaxlimit : healthplus;
	if (GetwHealthPlus() < healthplus)
		SetwHealthPlus(healthplus);
}
