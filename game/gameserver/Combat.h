/*
 * =====================================================================================
 *
 *       Filename:  Combat.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2012年09月14日 03时09分46秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  imane (), imanecr@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef _COMBAT_H_
#define _COMBAT_H_

#include "WorldPacket.h"
#include "ConfigData.h"
#include "EnumDefines.h"
#include "Util.h"
#include "CommonType.h"

class CRole;
class CCard;
//typedef struct sAttackTeam
//{
//} sAttackTeam;
//typedef struct sDefenceTeam
//{
//} sDefenceTeam;
typedef struct sCSEffect
{
	uint32_t ObjId;
	uint32_t SkillIndex;
	uint32_t SkillEffect;
	int32_t Para1;
	int32_t Para2;
	uint8_t Order;
	uint32_t attack;
	uint32_t defence;
	uint32_t attackattribute;
	uint32_t defenceattribute;
} sCSEffect;

/*
 * =====================================================================================
 *        Class:  CCombat
 *  Description:  
 * =====================================================================================
 */

class CCombat
{
	public:
		/* ====================  OPERATORS     ======================================= */
	static void HandlerSearchPlayer(CRole *pCRole, WorldPacket &pkg);
	static void HandlerAutoPlayer(CRole *pCRole, WorldPacket &pkg);
//	static void HandlerDuration(CRole *pCRole, WorldPacket &pkg);	
//	static void HandlerDurationPreview(CRole *pCRole, WorldPacket &pkg);
	static void HandlerSearchPlayerByItem(CRole *pCRole, WorldPacket &pkg);
	static void HandlerRob(CRole *pCRole, WorldPacket &pkg);
	static void HandlerRobPreview(CRole *pCRole, WorldPacket &pkg);
	static void HandlerSearchWin(CRole *pCRole, WorldPacket &pkg);
}; /* -----  end of class CCombat  ----- */

class CRobot;
class CRobotCtrl
{
	public:
		CRobotCtrl();
		int Init(CRole *pCRole);
		void SaveData();
//		void OnTimer5s();

		void PacketRandRobotTeamCardData(WorldPacket &pkg, uint8_t randnumber, uint32_t itemindex);
		bool CheckRobot(uint32_t robotid, uint32_t itemid);
		CRobot *GetRobot(uint32_t robotid);
		void SetUpadetime(uint32_t robotid);
	private:
		void SeriliazeDB(ByteBuffer &by);
		void DeSeriliazeDB(ByteBuffer &by);

		std::map<uint32_t, uint32_t> m_mapRob;	
		CRole *m_pCRole;
		bool m_IsInit;
};


////attribute and vec must be set before duration 
//class CDuration
//{
//	public:
//
//		typedef class cardforduration
//		{
//			public:
//				cardforduration():CardObjId(0), CardIndex(0), TalentSkill(0), BigSkill(0), BigSkillLv(0), Attack(0), Defence(0), Health(0), Crit(0), Dodge(0), Action(0), Attribute(0), AddBuffer(0), ReduceBuffer(0), SkillPro(0), SkillPower(0), CardLv(0) 
//			{}
//				uint32_t CardObjId;
//				uint32_t CardIndex;
//				uint32_t TalentSkill;
//				uint32_t BigSkill;
//				uint32_t BigSkillLv;
//				uint32_t Attack;
//				uint32_t Defence;
//				uint32_t Health;
//				uint32_t Crit;
//				uint32_t Dodge;
//				uint32_t Action;
//				uint8_t Attribute;
//				uint32_t AddBuffer;
//				uint32_t ReduceBuffer;
//				uint32_t SkillPro;
//				uint32_t SkillPower;
//				uint16_t CardLv;
//		} cardforduration_t;
//		
//		struct DurationData;
//
//		typedef struct BufferData
//		{
//			uint32_t index;
//			uint32_t count;
//			DurationData *Card;
//			uint32_t position;
//			uint8_t bufferflag;
//		} BufferData_t;
//
//		typedef struct DurationData 
//		{
//				uint32_t CardObjId;
//				uint32_t CardIndex;
//				uint32_t TalentSkill;
//				uint32_t BigSkill;
//				uint32_t BigSkillLv;
//				uint32_t Attack;
//				uint32_t Defence;
//				uint32_t Health;
//				uint32_t HealthMax;
//				uint32_t Crit;
//				uint32_t Dodge;
//				uint32_t Action;
//				uint8_t Attribute;
//				uint32_t AddBuffer;
//				uint32_t ReduceBuffer;
//				uint32_t SkillPro;
//				uint32_t SkillPower;
//				BufferData_t* Buffer[MAX_STATUS];
//				BufferData_t* Debuffer[MAX_STATUS];
//				uint8_t Team;
//				uint32_t BigSkillNumber;
//				uint32_t TalentSkillNumber;
//				uint16_t CardLv;
//		} DurationData_t;
//
//
//		int PrepareData(uint8_t team)
//		{
//			SetTeam(team);
//			return PrepareDurationData(m_vecForDuration, m_vecLineUp);
//		}
//
//		virtual int PrepareDurationData(std::vector<cardforduration_t> &vec, std::vector<uint32_t> &vecline) = 0;
//		
//		/*
//		 *--------------------------------------------------------------------------------------
//		 *       Class:  CDuration
//		 *      Method:  get_m_launchobjid
//		 *--------------------------------------------------------------------------------------
//		 */
//		inline uint32_t
//			GetLaunchObjid (  ) const
//			{
//				return m_launchobjid;
//			}		/* -----  end of method CDuration::get_m_launchobjid  ----- */
//
//		/*
//		 *--------------------------------------------------------------------------------------
//		 *       Class:  CDuration
//		 *      Method:  set_m_launchobjid
//		 *--------------------------------------------------------------------------------------
//		 */
//		inline void
//			SetLaunchObjid ( uint32_t value )
//			{
//				m_launchobjid	= value;
//				return ;
//			}		/* -----  end of method CDuration::set_m_launchobjid  ----- */
//			
//		uint8_t GetTeam() const
//		{
//			return m_Team;
//		}			
//
//		void SetTeam(uint8_t team)
//		{
//			m_Team = team;
//			return;
//		}
//		
//		int CheckData();
//		cardforduration_t* GetCardBaseData(uint32_t objid);
//		DurationData_t* GetCardData(uint32_t objid);
//		// 0: 胜利 1: 失败 2: 平局 -1: 错误
//		friend int DurationJudgeWin(CDuration *pCRole, CDuration *pPeerCRole, ByteBuffer &by, uint8_t &evaluate);
//		int DurationDataCreate();
//		BufferData_t *GetBuffer(uint32_t objid, uint32_t index);
//		friend class CCardSkill;
//		void GetFreeBufferPosition(uint32_t objid, uint32_t index, uint8_t bufferflag, int32_t &position); 
//		void HexDump();
//		
//	private:
//		uint32_t m_launchobjid;
//		uint8_t m_Team;
//
////		uint32_t m_AttributeDuration;
//		std::vector<cardforduration_t> m_vecForDuration;
//		std::vector<uint32_t> m_vecLineUp;
//		std::vector<DurationData_t> m_vecDurationData;
////		std::vector<CDuration::skillforduration_t> m_vecSkillForDuration;
//	
//};
//
////int DurationJudgeWin(CDuration *pCRole, CDuration *pPeerCRole, ByteBuffer &by);

typedef struct DurationIn
{
	DurationIn():type(0),sword(100),hand(100),internalforce(100),dark(100),stick(100)
	{};
	uint8_t type; // DURATION_XXX
	uint32_t sword;
	uint32_t hand;
	uint32_t internalforce;
	uint32_t dark;
	uint32_t stick;
}SDurationIn;

//attribute and vec must be set before duration 
class CDuration
{
	public:
		virtual ~CDuration();

		enum 
		{
			FIRST_MYSELF,
			SECOND_MYSELF,
			MASTER_MYSELF,
			FIRST_PEER,
			SECOND_PEER,
			MASTER_PEER,
			MAX_ORDER
		};

		typedef class cardforduration
		{
			public:
				cardforduration():CardObjId(0), CardIndex(0), BigSkill(0), BigSkillLv(0), AttackM(0), AttackE(0), DefenceM(0), DefenceE(0), Health(0), HealthMaxM(0), HealthMaxE(0), Crit(0), Attribute(0), SkillPro(0), SkillPower(0), CardLv(0), CardType(0), Rare(0), AddHarm(0), ReduceHarm(0), AddCritHarm(0), DurationExp(0), DurationCoin(0), ObjDrop(0), StrengthRecover(0), EnergyRecover(0), BreakCount(0)
			{}
				uint32_t CardObjId;
				uint32_t CardIndex;
				uint32_t BigSkill;
				uint32_t BigSkillLv;
				uint32_t AttackM;
				uint32_t AttackE;
				uint32_t DefenceM;
				uint32_t DefenceE;
				uint32_t Health;
				uint32_t HealthMaxM;
				uint32_t HealthMaxE;
				uint32_t Crit;
				uint8_t Attribute;
				uint32_t SkillPro;
				uint32_t SkillPower;
				uint16_t CardLv;
				uint8_t CardType;
				uint8_t Rare;

				uint32_t AddHarm;
				uint32_t ReduceHarm;
				uint32_t AddCritHarm;

				uint32_t DurationExp;
				uint32_t DurationCoin;
				uint32_t ObjDrop;
				uint32_t StrengthRecover;
				uint32_t EnergyRecover;
				uint32_t BreakCount;
		} cardforduration_t;
		
//		struct DurationData;
//
//		typedef struct BufferData
//		{
//			uint32_t index;
//			uint32_t count;
//			DurationData *Card;
//			uint32_t position;
//			uint8_t bufferflag;
//		} BufferData_t;
		
		typedef struct TotalData
		{
			uint32_t Defence;
			uint32_t Health;
			uint32_t HealthMax;
			uint32_t ReduceHarm;
		} TotalData_t;
		typedef struct DurationData 
		{
				uint32_t CardObjId;
				uint32_t CardIndex;
				uint32_t BigSkill;
				uint32_t BigSkillLv;
				uint32_t AttackM;
				uint32_t AttackE;
				uint32_t Crit;
				uint8_t Attribute;
				uint32_t SkillPro;
				uint32_t SkillPower;
		} DurationData_t;


		int PrepareData(uint8_t team)
		{
			SetTeam(team);
			m_harm = 0;
			m_UserName = "test";
			m_VipExtra = 0;
			return PrepareDurationData(m_vecForDuration, m_vecLineUp);
		}

		// vecline 第一个是队长, 其他是队员
		virtual int PrepareDurationData(std::vector<cardforduration_t> &vec, std::vector<uint32_t> &vecline) = 0;

		uint32_t GetExtraExp(uint32_t objid)
		{
			cardforduration_t *p = GetCardBaseData(objid);	
			if (p == NULL)
				return 0;
			return p->DurationExp;
		}

		uint32_t GetExtrObjDrop()
		{
			uint32_t pro = 0;
			for (int i = 0; i < m_vecForDuration.size(); i++)
			{
				pro += m_vecForDuration[i].ObjDrop;
			}
			return pro;
		}

		uint32_t GetExtrCoin()
		{
			uint32_t tol = 0;
			for (int i = 0; i < m_vecForDuration.size(); i++)
			{
				tol += m_vecForDuration[i].DurationCoin;
			}
			return tol;
		}

		uint32_t GetExtraEnergy()
		{
			uint32_t pro = 0;
			for (int i = 0; i < m_vecForDuration.size(); i++)
			{
				if (pro < m_vecForDuration[i].EnergyRecover)
				{
					pro = m_vecForDuration[i].EnergyRecover;
				}
			}
			IME_DEBUG("extra energy %u", pro);
			if (CUtil::RandEither(pro, 100))
			{
				return 1;
			}
			return 0;
		}

		uint32_t GetExtraStrength()
		{
			uint32_t pro = 0;
			for (int i = 0; i < m_vecForDuration.size(); i++)
			{
				if (pro < m_vecForDuration[i].StrengthRecover)
				{
					pro = m_vecForDuration[i].StrengthRecover;
				}
			}
			IME_DEBUG("extra strength %u", pro);
			if (CUtil::RandEither(pro, 100))
			{
				return 1;
			}
			return 0;
		}

		uint32_t GetTotalReduceHarm()
		{
			return m_TotalData.ReduceHarm;
		}
			
		uint8_t GetTeam() const
		{
			return m_Team;
		}			

		void SetTeam(uint8_t team)
		{
			m_Team = team;
			return;
		}
		
		uint32_t GetCardNumber()
		{
			int number = 0;
			for(int i = 0; i < m_vecLineUp.size(); i++)
			{
				if (m_vecLineUp[i] != 0)
					number++;
			}
			number += 1;
			return number;
		}

		int CheckData();
		cardforduration_t* GetCardBaseData(uint32_t objid);
		DurationData_t* GetCardData(uint32_t objid);
		// 0: 胜利 1: 失败 2: 平局 -1: 错误
		friend int DurationJudgeWin(CDuration *pCRole, CDuration *pPeerCRole, ByteBuffer &by, uint8_t &evaluate, SDurationIn in);
		int DurationDataCreate();
//		BufferData_t *GetBuffer(uint32_t objid, uint32_t index);
		friend class CCardSkill;
//		void GetFreeBufferPosition(uint32_t objid, uint32_t index, uint8_t bufferflag, int32_t &position); 
		void HexDump();
		bool IsEmpty(uint32_t line)
		{
			if (line == MASTER_LINE)
			{
				if (m_DurationMaster.CardObjId == 0)
				{
					return true;
				}
				else
				{
					return false;
				}
			}
			else
			{
				bool flag = true;
				uint32_t linebase = line * ONE_TIME_NUMBER;
				for(int i = 0; i < ONE_TIME_NUMBER; i++)
				{
					if (m_vecLineUp[linebase + i] != 0)
					{
						flag = false;
						break;
					}
				}

				return flag;
			}
			return true;
		}

		void GetMasterSkill(uint32_t &skill, uint16_t &lv)
		{
			skill = m_DurationMaster.BigSkill;
			lv = m_DurationMaster.BigSkillLv;
		}
		uint32_t GetTotalHarm()
		{
			return m_harm;
		}

		std::string GetstrUserName()
		{
			return m_UserName;
		}

		void SetStrUserName(std::string str)
		{
//			IME_ERROR("duration !!!!!");
			m_UserName = str;
		}

		void SetVipExtraHarm(uint32_t harm)
		{
			m_VipExtra = harm;
		}

		std::vector<int> &GetRelationIndex()
		{
			return m_vecRelationIndex;
		}

		void SetRelationIndex(const std::vector<int> &vec)
		{
			m_vecRelationIndex = vec;
		}
		
	private:
//		uint32_t m_launchobjid;
		uint8_t m_Team;
		std::string m_UserName;
		uint32_t m_VipExtra;
		std::vector<int> m_vecRelationIndex;

		// prepare data
		std::vector<cardforduration_t> m_vecForDuration;
		std::vector<uint32_t> m_vecLineUp;
		
		// create data
		std::vector<DurationData_t> m_vecDurationData;
		TotalData_t m_TotalData;
		DurationData_t m_DurationMaster;

		// for statistic
		uint32_t m_harm;
};

class CRobot:public CDuration
{
	public:
		~CRobot();
		typedef struct
		{
			uint32_t dwObjId;	
			uint32_t dwIndex;
			uint8_t byAttribute;
			uint32_t dwAttack;
			uint32_t dwDefence;
			uint32_t dwHealth;
			uint32_t dwCritRate;
			uint32_t dwBigSkill;
			uint32_t dwBigSkillLv;
			uint32_t dwCardLv;
			uint8_t byCardType;
		} SBossCard;
		int Init(uint32_t index);
		int PrepareDurationData(std::vector<cardforduration_t> &vec, std::vector<uint32_t> &vecline);
		friend class CRobotCtrl; 
		SBossCard* GetMaster();

		int PreDataTeamCardDataForArena(WorldPacket &by);
		int PreDataForRank(SFBRankRole &tmp);

		uint32_t GetdwRank()
		{
			return m_dwRank;
		}
		void SetdwRank(uint32_t rank)
		{
			m_dwRank = rank;
		}
		int InitDataForArena();

		uint32_t GetdwRobotId()
		{
			return m_RobotId;
		}

		std::string GetstrUserName()
		{
			return m_strUserName;
		}

		uint16_t GetwMasterLv()
		{
			return m_MasterLv;
		}

		uint16_t GetRoleLv()
		{
			return m_RoleLv;
		}

		uint32_t GetTotalForce();

	private:
		std::vector<SBossCard> m_vec;	
		std::vector<uint32_t> m_vecline;
		uint32_t m_RobotId;
		std::string m_strUserName;

		uint32_t m_dwRank;
		uint32_t m_MasterId;
		uint16_t m_MasterLv;
		uint16_t m_RoleLv;

		uint32_t m_dwDevote;
		CCard *m_pCard;
};

#endif
