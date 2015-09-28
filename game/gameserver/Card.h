/**
 * =====================================================================================
 *       @file  Card.h
 *      @brief  
 *
 *  Detailed description starts here.
 *
 *   @internal
 *     Created  2012年08月28日 08时41分20秒 
 *    Revision  1.0.0.0
 *    Compiler  gcc/g++
 *     Company  
 *   Copyright  Copyright (c) 2012, 
 *
 *     @author  imane, imanecr@gmail.com
 * =====================================================================================
 */

#ifndef _CARD_H_
#define _CARD_H_

#include <stdint.h>
#include "DynamicStruct.h"
#include "World.h"

#define PROPERTY_DEF_CARD(T,name) \
        T Get##name() { return m_##name;}; \
        void Set##name(T name, bool bSend = false) \
        { m_##name = name; \
/*        setProperty(E_CARD_##name, name);  */ \
        if (bSend) SendProUpdate(); \
        ;}; \

class CRole;

class CCard: public CDynamicStruct
{
    public:
        typedef enum _E_PROPERTY_ROLE_CARD
        {
            E_CARD_dwObjId,      //物品ID  32
            E_CARD_dwIndex,       //卡牌索引 32
            E_CARD_byType,        //卡牌类型 8 
            E_CARD_byAttribute,           //属性 8
            E_CARD_byStarLevel,        //卡牌星级 8
            E_CARD_wLevel,        //卡牌等级 16
            E_CARD_dwExp,        //卡牌经验 32
            E_CARD_dwExpLimit,        //卡牌经验 32
            E_CARD_dwEffectIndex,       //效果索引 宝石卡牌使用
            E_CARD_byRare,         //稀有度 8
            E_CARD_dwLastExpLimit,         //上一级经验值 32
			E_CARD_byFlag, 				// 0 无意义 0x1 第一位表示满级
			E_CARD_dwSkillPara,           //技能效果
			E_CARD_dwAttack,				//攻击
			E_CARD_dwDefence,				//防御
			E_CARD_dwHealth,				//血
            E_CARD_dwAttackGrow,         //攻击成长 32
            E_CARD_dwDefenceGrow,         //防御成长 32
            E_CARD_dwHealthGrow,         //生命成长 32
            E_CARD_dwExpGrow,         //经验成长 32
            E_CARD_dwCritRate,         //暴击率 32
            E_CARD_dwSuitIndex,         //套装索引 32
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
			E_CARD_wHealthPlus
        }E_PROPERTY_ROLE_CARD;

        CCard();
        ~CCard();
        
        void SetCRole(CRole *pCRole){m_pCRole = pCRole;};
		CRole *GetCRole() {return m_pCRole;};

		bool IsFullLevel();
        
        void SerializeDB(ByteBuffer &by);
        void DeserializeDB(ByteBuffer &by);
        void SerializeClient(ByteBuffer &by);
//        int CalculateRelate(bool IsUpdate = false);
        int Calculate(bool IsUpdate = false, bool IsLevelChange = true, bool IsCardLevel = true);
//        void SendProUpdateRelate(); 
        void SendProUpdate();
        int LoadConf();
		int AddExp(uint32_t exp);

		int InitCardSkill();

		void CardEquip1(bool IsUpdate = false);
		void CardUnEquip1(bool IsUpdate = false);
        
        PROPERTY_DEF_CARD(uint32_t, dwObjId); 
        PROPERTY_DEF_CARD(uint32_t, dwIndex);
        PROPERTY_DEF_CARD(uint8_t, byType);
        PROPERTY_DEF_CARD(uint8_t, byAttribute);
        PROPERTY_DEF_CARD(uint8_t, byStarLevel);
        PROPERTY_DEF_CARD(uint16_t, wLevel);
        PROPERTY_DEF_CARD(uint32_t, dwExp);
        PROPERTY_DEF_CARD(uint32_t, dwExpLimit);
        PROPERTY_DEF_CARD(uint32_t, dwEffectIndex);
        PROPERTY_DEF_CARD(uint32_t, byRare);
        PROPERTY_DEF_CARD(uint32_t, dwLastExpLimit);
        PROPERTY_DEF_CARD(uint8_t, byFlag);
		PROPERTY_DEF_CARD(uint32_t, dwSkillPara);
        PROPERTY_DEF_CARD(uint32_t, dwAttack);
        PROPERTY_DEF_CARD(uint32_t, dwDefence);
        PROPERTY_DEF_CARD(uint32_t, dwHealth);
        PROPERTY_DEF_CARD(uint32_t, dwAttackGrow);
        PROPERTY_DEF_CARD(uint32_t, dwDefenceGrow);
        PROPERTY_DEF_CARD(uint32_t, dwHealthGrow);
        PROPERTY_DEF_CARD(uint32_t, dwExpGrow);
        PROPERTY_DEF_CARD(uint32_t, dwCritRate);
//        PROPERTY_DEF_CARD(uint32_t, dwDodgeRate);
//        PROPERTY_DEF_CARD(uint32_t, dwAction);
        PROPERTY_DEF_CARD(uint32_t, dwSuitIndex);
//        PROPERTY_DEF_CARD(uint32_t, dwInitSkillIndex);
        PROPERTY_DEF_CARD(uint32_t, dwMasterSkillIndex);
		PROPERTY_DEF_CARD(uint16_t, wMasterSkillLv);  
		PROPERTY_DEF_CARD(uint32_t, dwSkillIndex);		
		PROPERTY_DEF_CARD(uint32_t, dwGrowLimit);
		PROPERTY_DEF_CARD(uint32_t, dwLimitBreakNumber);
		PROPERTY_DEF_CARD(uint32_t, dwLimitBreak);
		PROPERTY_DEF_CARD(uint32_t, dwLimitBreakPro);
		PROPERTY_DEF_CARD(uint32_t, dwAwakePro);
		PROPERTY_DEF_CARD(uint32_t, dwAddExp);
		PROPERTY_DEF_CARD(uint32_t, dwConsumeCoin);
		PROPERTY_DEF_CARD(uint32_t, dwGetCoin);  
		PROPERTY_DEF_CARD(uint32_t, dwBreakExp);  
		PROPERTY_DEF_CARD(uint8_t, byMultiPower);  
		PROPERTY_DEF_CARD(uint32_t, dwMultiTime);  
		PROPERTY_DEF_CARD(uint32_t, dwOneLevelExp);  
		PROPERTY_DEF_CARD(uint32_t, dwTotalConsumeCoin);  

		PROPERTY_DEF_CARD(uint32_t, dwSkillPro);
		PROPERTY_DEF_CARD(uint32_t, dwSkillPower);
		PROPERTY_DEF_CARD(uint32_t, dwDurationExp);
		PROPERTY_DEF_CARD(uint32_t, dwDurationCoin);
		PROPERTY_DEF_CARD(uint32_t, dwObjDrop);
		PROPERTY_DEF_CARD(uint32_t, dwStrengthRecover);
		PROPERTY_DEF_CARD(uint32_t, dwEnergyRecover);
		PROPERTY_DEF_CARD(uint32_t, dwAddHarm);
		PROPERTY_DEF_CARD(uint32_t, dwReduceHarm);
		PROPERTY_DEF_CARD(uint32_t, dwAddCritHarm);
		PROPERTY_DEF_CARD(uint16_t, wLeaderLevel);
		PROPERTY_DEF_CARD(uint32_t, dwLeaderIndex);
		PROPERTY_DEF_CARD(uint32_t, dwCardGroup);
		PROPERTY_DEF_CARD(uint32_t, dwMasterIndex);
		PROPERTY_DEF_CARD(uint16_t, wAttackPlus);
		PROPERTY_DEF_CARD(uint16_t, wDefencePlus);
		PROPERTY_DEF_CARD(uint16_t, wHealthPlus);

//        void SetCard(uint32_t dwObjId,                          
//                uint8_t byType,                            
//                uint32_t dwIndex,                          
//                uint8_t byAttribute, //卡牌属性            
//                uint8_t byStarLevel, //卡牌星级            
//                uint16_t wLevel, //卡牌等级                
//                uint32_t dwExp, //卡牌经验                 
//                uint32_t dwExpLimit, //下一级升级上限      
//                uint32_t dwAttackMin, //攻击力最小         
//                uint32_t dwAttackMax,                      
//                uint32_t dwDefenseMin,                     
//                uint32_t dwDefenseMax,                     
//                uint32_t dwEffectIndex,                    
//                uint32_t dwAttackSkillIndex,               
//                uint32_t dwDeffenseSkillIndex,             
//                uint32_t byRare,                           
//                uint32_t dwSuit1Index,                     
//                uint32_t dwSuit2Index,                     
//                uint32_t dwSuit3Index,                     
//                uint32_t dwSuit4Index,
//				uint32_t dwLastExplimit,
//				uint8_t byFlag
//                );
                	
	   	int CreateCard(uint32_t index, uint16_t level, uint32_t objid);	
		int NpcCalculate();
		int SetRealLv(uint32_t lv);
		int Upgrade(uint32_t index);
		void ChangeAttackPlus(uint32_t attack);
		void ChangeDefencePlus(uint32_t defence);
		void ChangeHealthPlus(uint32_t health);
		void PacketData(ByteBuffer &by);
        
    private:
        CRole *m_pCRole;

        uint32_t m_dwObjId;
        uint8_t m_byType;
        uint32_t m_dwIndex;
        uint8_t m_byAttribute; //卡牌属性
        uint8_t m_byStarLevel; //卡牌星级
        uint16_t m_wLevel; //卡牌等级
        uint32_t m_dwExp; //卡牌经验
        uint32_t m_dwExpLimit; //下一级升级上限
        uint32_t m_dwEffectIndex;
        uint32_t m_dwAttackSkillIndex;
        uint32_t m_dwDeffenseSkillIndex;
        uint32_t m_byRare;
		uint32_t m_dwLastExpLimit;
		uint32_t m_byFlag;
		uint32_t m_dwAttack;
		uint32_t m_dwDefence;
		uint32_t m_dwHealth;
		uint32_t m_dwAttackGrow;
		uint32_t m_dwDefenceGrow;
		uint32_t m_dwHealthGrow;
		uint32_t m_dwExpGrow;
		uint32_t m_dwCritRate;
//		uint32_t m_dwDodgeRate;
//		uint32_t m_dwAction;
		uint32_t m_dwSuitIndex;
		uint32_t m_dwSkillPara;
//		uint32_t m_dwInitSkillIndex;
//		uint32_t m_dwStudySkillIndex;
		uint32_t m_dwMasterSkillIndex;
		uint16_t m_wMasterSkillLv;
		uint32_t m_dwSkillIndex;			
		uint32_t m_dwGrowLimit;
		uint32_t m_dwLimitBreakNumber;
		uint32_t m_dwLimitBreak;
		uint32_t m_dwLimitBreakPro;
		uint32_t m_dwAwakePro;
		uint32_t m_dwAddExp;
		uint32_t m_dwConsumeCoin;
		uint32_t m_dwGetCoin;
		uint32_t m_dwBreakExp;
		uint8_t m_byMultiPower;
		uint32_t m_dwMultiTime;
		uint32_t m_dwTotalConsumeCoin;
		uint32_t m_dwSkillPro;
		uint32_t m_dwSkillPower;
		uint32_t m_dwDurationExp;
		uint32_t m_dwDurationCoin;
		uint32_t m_dwObjDrop;
		uint32_t m_dwStrengthRecover;
		uint32_t m_dwEnergyRecover;
		uint32_t m_dwAddHarm;
		uint32_t m_dwReduceHarm;
		uint32_t m_dwAddCritHarm;
		uint16_t m_wLeaderLevel;
		uint32_t m_dwLeaderIndex;
		uint32_t m_dwCardGroup;
		uint32_t m_dwMasterIndex;
		uint16_t m_wAttackPlus;
		uint16_t m_wDefencePlus;
		uint16_t m_wHealthPlus;

		uint32_t m_dwOneLevelExp;
};

#endif
