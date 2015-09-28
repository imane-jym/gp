/**
 * =====================================================================================
 *       @file  CardPacket.h
 *      @brief  
 *
 *  Detailed description starts here.
 *
 *   @internal
 *     Created  2012年08月28日 13时46分16秒 
 *    Revision  1.0.0.0
 *    Compiler  gcc/g++
 *     Company  
 *   Copyright  Copyright (c) 2012, 
 *
 *     @author  imane (), imanecr@gmail.com
 * =====================================================================================
 */
#ifndef _CARDPACKET_H_
#define _CARDPACKET_H_

#define MAX_CARD_CAPACITY 6                     /* 弟子卡上阵的最大数量 */
#define POWER_CALCULATE(a,d,h) (a / 10 + d / 10 + h / 10)

#include <stdint.h>
#include <map>
#include <vector>
#include "World.h"
#include "Card.h"
#include "Combat.h"

class CRole;

//typedef struct sAttr
//{
//	uint8_t Attr;
//	uint8_t Number;
//} sAttr;

struct SSwitchCard
{
	uint32_t AddExp;
};

class CCardPacket
{
    public:
        enum
        {
            ADD,
            DEL
        };
		typedef class equip
		{
			public:
			equip():dwWeaponObjId(0),dwArmorObjId(0),dwJewelObjId(0),dwAccessoriesObjId(0),dwSkillObjId(0),dwMasterGuide1(0),dwMasterGuide2(0),dwMasterGuide3(0)
			{
			}
//			equip(uint32_t weapon, uint32_t armor, uint32_t jewel)
//			:dwWeaponObjId(weapon),dwArmorObjId(armor),dwJewelObjId(jewel)
//			{
//			}
			uint32_t dwWeaponObjId;
			uint32_t dwArmorObjId;
			uint32_t dwJewelObjId;
			uint32_t dwAccessoriesObjId;
			uint32_t dwSkillObjId;
			uint32_t dwMasterGuide1;
			uint32_t dwMasterGuide2;
			uint32_t dwMasterGuide3;
		} equip_t;

		typedef class itemequip
		{
			public:
//				itemequip():s_attack_min(0),s_attack_max(0),s_defence_min(0), s_defence_max(0),
//				si_attack_min(0), si_attack_max(0), si_defence_min(0), si_defence_max(0), attack_high_probability(0), defence_high_probability(0), skill_probability(0)
//				{}
//			uint32_t s_attack_min;
//			uint32_t s_attack_max;
//			uint32_t s_defence_min;
//			uint32_t s_defence_max;
//			uint32_t si_attack_min;
//			uint32_t si_attack_max;
//			uint32_t si_defence_min;
//			uint32_t si_defence_max;
//			uint32_t attack_high_probability;
//			uint32_t defence_high_probability;
//			uint32_t skill_probability;
			itemequip():Attack(0),Defence(0),Health(0),Crit(0),Dodge(0),Action(0),Attribute(0), SkillPro(0), SkillPower(0), DurationExp(0), DurationCoin(0), ObjDrop(0), StrengthRecover(0), EnergyRecover(0), AddHarm(0), ReduceHarm(0), AddCritHarm(0) {}
			uint32_t Attack;
			uint32_t Defence;
			uint32_t Health;
			uint32_t Crit;
			uint32_t Dodge;
			uint32_t Action;
			uint8_t Attribute;
			uint32_t SkillPro;
			uint32_t SkillPower;
			uint32_t DurationExp;
			uint32_t DurationCoin;
			uint32_t ObjDrop;
			uint32_t StrengthRecover;
			uint32_t EnergyRecover;
			uint32_t AddHarm;
			uint32_t ReduceHarm;
			uint32_t AddCritHarm;
		} itemequip_t;

		typedef struct AddCardStu
		{
//uint32_t cardindex, uint32_t &objid, uint32_t &opt, bool IsUpdate = true, int Awake = 0
			uint32_t cardindex;
			uint32_t Awake;	
			uint8_t IsAutoAwake;
			bool IsUpdate;
			uint32_t exp;
			uint32_t breaknumber;

			uint32_t attackplus;
			uint32_t defenceplus;
			uint32_t healthplus;
			uint32_t objid;
			uint32_t opt;

			uint32_t source;
		}SAddCardStu;

        CCardPacket();
        ~CCardPacket();
        CCardPacket(CRole *pCRole);
        void SetpCRole(CRole *pCRole) {m_pCRole = pCRole;};
        int Load(uint32_t userid);
		int LoadForOffLogin(uint32_t userid);
//        int HandlerCardAdd(uint32_t cardindex);
        int CardDel(uint32_t objid, int source);
        int SaveData();
		int EquipCardCalForDuration(itemequip_t &p, itemequip_t &q, int i, int flag);
        int Calculate(bool IsUpdate = false);
		bool IsFull(uint32_t number, uint32_t cardindex);
		bool IsFullSpecial(std::vector<SProduct> tmp);
    
        void HandlerBaseInfo();
        void HandlerCardQuipmentChange(WorldPacket &info);
//      void HandlerChangeTeam(WorldPacket &packet);
		void HandlerItemQuipmentChange(WorldPacket &info);
		void HandlerItemQuipmentInfo();
		void HandlerEquipCalInfo(uint8_t team);
		void HandlerLineUpInfo();
		void HandlerLineUpChange(WorldPacket &info);

		int EquipCardInfo();

        int InitCards(uint32_t masterindex);
		
		CCard * Find(uint32_t val) 
		{ 
			std::map<uint32_t, CCard *>::iterator it;
			return (it = m_mapCards.find(val)) == m_mapCards.end() ? NULL : it->second; 
		}

        int CardAddByPara(uint32_t cardindex, uint32_t &objid, uint32_t &opt, int source, bool IsUpdate = true, int Awake = 0);
		int CardAddByParaNoAutoAwake(uint32_t cardindex, uint32_t &objid, uint32_t &opt, int source, bool IsUpdate = true, int Awake = 0);
		int CardAddByParaWithExp(uint32_t cardindex, uint32_t &objid, uint32_t exp, uint32_t breaknumber, uint32_t awake, int source, bool IsUpdate = true);
        int CardAddByParaWithPlus(uint32_t cardindex, uint32_t &objid, int &attack, int &defence, int &health, int source, bool IsUpdate = true);
		int CardAddByParaWithRandPlus(uint32_t cardindex, uint32_t &objid, int pluspro, int source, bool IsUpdate = true);
		int CardAdd(SAddCardStu &stu);

		int CardBreak(CCard *ped, std::vector<CCard *> p, const SSwitchCard &swt);
//		int CardAddByParaWithInfo(SAddCardStu &stu, bool IsUpdate = true);

//		int GetEquipCards(std::vector<uint32_t> &vecEquip, std::vector<uint32_t> &vecIndex);
//		int LoadOnlyEquip(uint32_t userid, uint8_t currentteam, std::vector<uint32_t> &vecEquip, std::vector<uint32_t> &vecIndex); 
		int GetEquippCards(std::vector<CCard *> &vecEquip);

		itemequip_t *GetItemEquip(uint16_t location)
		{
			if (m_vecAttackTeam.size() <= location)
			{
				return NULL;
			}
			return &m_vecItemEquip1[location];
		}
		itemequip_t *GetMasterItemEquip()
		{
			return &m_MasterItemEquip;
		}

//		void PacketForFriendProtocol(WorldPacket &pkg);
		//************************* attribute cal **************************************
		int SingleCardAddUpdate(const uint32_t &objid);
		int SingleCardDelUpdate(uint32_t objid);

//		int CalEquip1(uint32_t location, bool IsUpdate = false);
//		int CalEquip2(uint32_t location, bool IsUpdate = false);
//		int Cal1stTeam(bool IsUpdate = false);
//		int Cal2ndTeam(bool IsUpdate = false);
//		int CalChangeTeam(bool IsUpdate = false);

		// team 0:master 1:follower
		bool IsInEquip(CCard *p, uint32_t &location, uint8_t &team); 
//		int ChangeEquipPet(CCard *pDown, CCard *pUp, uint32_t location, uint8_t team);

		void UpdateEquipCards();

//		void Dumpdata();
		uint32_t GetMaxObjId()
		{
			std::map<uint32_t, CCard *>::iterator it = m_mapCards.end();
			if (m_mapCards.size() == 0)
			{
				return 0;
			}
			else
			{
				it--;
				return it->first;
			}
		}
//		std::vector<itemequip_t>* GetVecItemEquip1() {return &m_vecItemEquip1;};
//		std::vector<uint32_t>* GetVecLineUp() {return &m_vecLineUp;};
		int PrepareDurationData(std::vector<CDuration::cardforduration_t> &vec, std::vector<uint32_t> &vecline, std::vector<int> &vecRelationshipEffect);
		int PreDataTeamCardData(WorldPacket &by, int number);
		int PreDataTeamCardDataForFriend(WorldPacket &by);
		int PreDataTeamCardDataForRob(WorldPacket &by, int number);

		void PreDataForUpgrade(std::map<uint32, std::vector<CConfUpgrade::SCard> > &map, uint32_t objid);

		void HandlerCardQuipmentInfo();
		void HandlerViewInfo();
		void HandlerCardChangeSingle(WorldPacket &info);

		uint32_t GetViewObjId(uint32_t pos)
		{
			if (pos < m_vecView.size())
			{
				return m_vecView[pos];
			}
			else
			{
				return 0;
			}
		}

		uint32_t GetMasterEquipObjId(uint32_t type)
		{
			std::vector<uint32_t> vectmp;
			vectmp.push_back(m_MasterEquip.dwWeaponObjId);
			vectmp.push_back(m_MasterEquip.dwArmorObjId);
			vectmp.push_back(m_MasterEquip.dwAccessoriesObjId);
			vectmp.push_back(m_MasterEquip.dwJewelObjId);
			vectmp.push_back(m_MasterEquip.dwSkillObjId);
			vectmp.push_back(m_MasterEquip.dwMasterGuide1);
			vectmp.push_back(m_MasterEquip.dwMasterGuide2);
			vectmp.push_back(m_MasterEquip.dwMasterGuide3);
			if (type > 8)
			{
				return 0;
			}
			return vectmp[type];
		}

		uint32_t GetEquipObjId(uint32_t pos, uint32_t type)
		{
			uint32_t obj = m_vecView[pos];
			uint32_t realpos = find(m_vecAttackTeam.begin(), m_vecAttackTeam.end(), obj) - m_vecAttackTeam.begin();
			if (realpos >= m_vecAttackTeam.size())
			{
				return 0;
			}
			std::vector<uint32_t> vectmp;
			vectmp.push_back(m_vecEquip1[realpos].dwWeaponObjId);
			vectmp.push_back(m_vecEquip1[realpos].dwArmorObjId);
			vectmp.push_back(m_vecEquip1[realpos].dwAccessoriesObjId);
			vectmp.push_back(m_vecEquip1[realpos].dwJewelObjId);
			vectmp.push_back(m_vecEquip1[realpos].dwSkillObjId);
			vectmp.push_back(m_vecEquip1[realpos].dwMasterGuide1);
			vectmp.push_back(m_vecEquip1[realpos].dwMasterGuide2);
			vectmp.push_back(m_vecEquip1[realpos].dwMasterGuide3);
			if (type > 8)
			{
				return 0;
			}
			return vectmp[type];
		}

		uint32_t GetMasterObjId()
		{
			return m_MasterCard;
		}

		void PacketForShowProtocol(WorldPacket &info);
		int PreDataTeamCardDataForArena(WorldPacket &by);
		void PreDataForRank(SFBRankRole &tmp, uint32_t devote);
		int GetCardIndex(uint32_t &masterindex, std::vector<uint32_t> &vecCardIndex);

		CDuration::cardforduration_t CreateMasterEquipForJoin(uint32_t objid, uint32_t rate);
		void SetMasterEquipForJoin(CDuration::cardforduration_t equip)
		{
			m_masterForJoin = equip;
		}
		
		void PreDataRole(SJoin &tmp);
		int LoadForBaseData(uint32_t userid);

		int FixData(uint32_t userid);
		
		int GetPower();
		void setAttack(uint32 times);

    private:
//		int CalResult(bool IsUpdate = false);
//        int InnerCardAdd(uint32_t cardindex, uint32_t &objid);
//        int InnerCardDel(uint32_t objid);
        void StrMerge(std::string &src, const std::string &sep, const std::vector<uint32_t> &vecRes);
        void StrSplit(const std::string &src, const std::string &sep, std::vector<uint32_t> &vecRes);
//		bool IsSuit(std::vector<uint32_t> *pSuitCollection, uint8_t team, uint8_t location);
        void deserialize(std::string &str);
        void serialize(std::string &str);
        void serializeEquipCard(std::string &str);
        CRole *m_pCRole;
        uint32_t m_dwTotalSpace;
		uint32_t m_dwCardNumber;
        uint32_t m_dwCardNumberRole;
		uint32_t m_dwCardNumberEquip;
        
		uint32_t m_dwTotalForce;
        std::map<uint32_t, CCard *> m_mapCards;
        std::vector<uint32_t> m_vecAttackTeam;
		std::vector<equip_t> m_vecEquip1;
		
		std::set<uint32_t> m_setForEquip1;      /*only for test data legal for equip1 contain master*/
		std::set<uint32_t> m_setobjid; /*only for test data legal for attackteam contain master*/
        bool m_bIsInit;

		std::vector<itemequip_t> m_vecItemEquip1;
		itemequip_t m_MasterItemEquip;
		equip_t m_MasterEquip;

		std::map<uint32_t, CCard *> m_mapEquipCards;

//		std::vector<uint32_t> m_vecLineUpIndex;
		std::vector<uint32_t> m_vecLineUp;
		std::vector<uint32_t> m_vecView;
		uint32_t m_MasterCard;
		CDuration::cardforduration_t m_masterForJoin;

		uint32 addAttackTimes;//增加卡牌攻击力（不含武器等其他攻击力），1：表示不增加，2：表示2倍，3：表示3倍
		uint16_t m_wMaxCardLevel;
};

#endif
