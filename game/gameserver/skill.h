/*
 * =====================================================================================
 *
 *       Filename:  skill.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2012年09月26日 16时57分00秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  imane (), imanecr@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef _SKILL_H_
#define _SKILL_H_

#include <stdint.h>
#include <map>
#include <vector>
#include "EnumDefines.h"
#include "Combat.h"
#include "CardPacket.h"
#include "ConfigData.h"

class CRole;
class CDuration;

/*
 * =====================================================================================
 *        Class:  CJewelSkill
 *  Description:  
 * =====================================================================================
 */
class CJewelSkill
{
	public:
		/* ====================  LIFECYCLE     ======================================= */

		/* ====================  ACCESSORS     ======================================= */
		typedef int (*JewelSkillFunc)(
				CCardPacket::itemequip_t *p,
				const int32_t &para1
				);

		typedef enum EJewelSkill {
			ATTACK = 1,
			DEFENCE,
			HEALTH,
			CRIT_RATE,
			DODGE_RATE,
			ADD_BUFFER,
			REDUCE_BUFFER,
			SKILL_PRO,
			SKILL_POWER
		} EJewelSkill;				

		/* ====================  OPERATORS     ======================================= */
		static void Init();

		static JewelSkillFunc Find(uint16_t fun)
		{
			return (m_mapFunc.find(fun) == m_mapFunc.end() ? NULL : m_mapFunc[fun]);
		}

		static int JewelSkillAttack(CCardPacket::itemequip_t *p, const int32_t &para1);
		static int JewelSkillDefence(CCardPacket::itemequip_t *p, const int32_t &para1);
		static int JewelSkillHealth(CCardPacket::itemequip_t *p, const int32_t &para1);
		static int JewelSkillCritRate(CCardPacket::itemequip_t *p, const int32_t &para1);
		static int JewelSkillDodgeRate(CCardPacket::itemequip_t *p, const int32_t &para1);
//		static int JewelSkillAddBuffer(CCardPacket::itemequip_t *p, const int32_t &para1);
//		static int JewelSkillReduceBuffer(CCardPacket::itemequip_t *p, const int32_t &para1);
		static int JewelSkillSkillPro(CCardPacket::itemequip_t *p, const int32_t &para1);
		static int JewelSkillSkillPower(CCardPacket::itemequip_t *p, const int32_t &para1);

	protected:
		/* ====================  METHODS       ======================================= */

		/* ====================  DATA MEMBERS  ======================================= */

	private:
		/* ====================  METHODS       ======================================= */
		static std::map<uint16_t, JewelSkillFunc> m_mapFunc;

		/* ====================  DATA MEMBERS  ======================================= */

}; /* -----  end of class CJewelSkill  ----- */



/*
 * =====================================================================================
 *        Class:  CExSkill
 *  Description:  
 * =====================================================================================
 */
class CExSkill
{
	public:
		/* ====================  LIFECYCLE     ======================================= */
		typedef int (*ExSkillFunc)(
				CCardPacket::itemequip_t *p,
				const int32_t &para1
				);

		typedef enum EJewelSkill {
			ATTACK = 1,
			DEFENCE,
			HEALTH,
			CRIT_RATE,
			ADD_HARM,
			SKILL_PRO,
			ADD_CRIT_HARM,
			REDUCE_HARM,
			DURATION_EXP,
			DURATION_COIN,
			OBJ_DROP,
			STRENGTH_RECOVER,
			ENERGY_RECOVER,
			SKILL_POWER
		} EJewelSkill;				

		static void Init();

		/* ====================  OPERATORS     ======================================= */
		static int ExecExSkillFunc(uint32_t index, CCardPacket::itemequip_t *p);
		static int ExecExSkillFuncByRelation(uint32_t cardindex, std::vector<int> vecRelatioship, CCardPacket::itemequip_t *p);

		static ExSkillFunc Find(uint16_t fun)
		{
			return (m_mapFunc.find(fun) == m_mapFunc.end() ? NULL : m_mapFunc[fun]);
		}

		static int ExSkillAttack(CCardPacket::itemequip_t *p, const int32_t &para1);
		static int ExSkillDefence(CCardPacket::itemequip_t *p, const int32_t &para1);
		static int ExSkillHealth(CCardPacket::itemequip_t *p, const int32_t &para1);
		static int ExSkillCritRate(CCardPacket::itemequip_t *p, const int32_t &para1);
		static int ExSkillAddHarm(CCardPacket::itemequip_t *p, const int32_t &para1);
		static int ExSkillSkillPro(CCardPacket::itemequip_t *p, const int32_t &para1);
		static int ExSkillAddCritHarm(CCardPacket::itemequip_t *p, const int32_t &para1);
		static int ExSkillReduceHarm(CCardPacket::itemequip_t *p, const int32_t &para1);
		static int ExSkillDurationExp(CCardPacket::itemequip_t *p, const int32_t &para1);
		static int ExSkillDurationCoin(CCardPacket::itemequip_t *p, const int32_t &para1);
		static int ExSkillObjDrop(CCardPacket::itemequip_t *p, const int32_t &para1);
		static int ExSkillStrengthRecover(CCardPacket::itemequip_t *p, const int32_t &para1);
		static int ExSkillEnergyRecover(CCardPacket::itemequip_t *p, const int32_t &para1);
		static int ExSkillSkillPower(CCardPacket::itemequip_t *p, const int32_t &para1);

	protected:
		/* ====================  METHODS       ======================================= */

		/* ====================  DATA MEMBERS  ======================================= */

	private:
		/* ====================  METHODS       ======================================= */

		/* ====================  DATA MEMBERS  ======================================= */
		static std::map<uint16_t, ExSkillFunc> m_mapFunc;

}; /* -----  end of class CExSkill  ----- */

/*
 * =====================================================================================
 *        Class:  CSuitSkill
 *  Description:  
 * =====================================================================================
 */
//class CSuitSkill
//{
//	public:
//		/* ====================  LIFECYCLE     ======================================= */
//
//		/* ====================  ACCESSORS     ======================================= */
//		typedef int (*SuitSkillFunc)(
//				CRole *pCRole,
//				uint16_t location,
//				uint8_t team,
//				const int32_t &para1,
//				const int32_t &para2
//				);
//
//		typedef enum ESuitSkill {
//			ATTACK,
//			DEFENCE
//		} ESuitSkill;				
//
//		/* ====================  OPERATORS     ======================================= */
//		static void Init();
//
//		static SuitSkillFunc Find(uint16_t fun)
//		{
//			return (m_mapFunc.find(fun) == m_mapFunc.end() ? NULL : m_mapFunc[fun]);
//		}
//
//		static int SuitSkillAttack(CRole *pCRole, uint16_t location, uint8_t team, const int32_t &para1,const int32_t &para2);
//		static int SuitSkillDefence(CRole *pCRole, uint16_t location, uint8_t team, const int32_t &para1,const int32_t &para2);
//
//	protected:
//		/* ====================  METHODS       ======================================= */
//
//		/* ====================  DATA MEMBERS  ======================================= */
//
//	private:
//		/* ====================  METHODS       ======================================= */
//		static std::map<uint16_t, SuitSkillFunc> m_mapFunc;
//
//		/* ====================  DATA MEMBERS  ======================================= */
//
//}; /* -----  end of class CSuitSkill  ----- */

/*
 * =====================================================================================
 *        Class:  CCardSkill
 *  Description:  
 * =====================================================================================
 */
class CCardSkill
{
	public:
		/* ====================  LIFECYCLE     ======================================= */

		/* ====================  ACCESSORS     ======================================= */
		typedef struct SSkillStu
		{
//			std::list<CDuration::BufferData_t>* pBufferlist;
			STC_CONF_CARD_SKILL *pConf;
			uint8_t IsLaunchSkill;
			uint8_t line;
			int SkillSeq;
//			uint8_t type;
			uint8_t ismyself;
		} SSkillStu;

		typedef int (*CardSkillFunc)(
				CDuration *pCRole,
				CDuration *pPeerCRole,
				SSkillStu *pConf,
				ByteBuffer &by,
				SDurationIn in
				);


		typedef enum ECardSkill {
            ATTACK = 1,                        
            DEFENCE,                       
		} ECardSkill;				

//		typedef struct SCondition
//		{
//			int round;
//		} SCondition;
//
//		typedef int (*SkillConditionFunc)(
//				CDuration *pCRole,
//				SCondition *pStu
//				);

//		typedef enum ESkillCondition {
//			HEALTH_30 = 1,
//			HEALTH_40,
//			HEALTH_50,
//			HEALTH_70,
//			HEALTH_100,
//			ROUND_1,
//			ROUND_2,
//			ROUND_2_DOWN,
//			ROUND_3_DOWN,	
//		}ESkillCondition;

//		typedef int (*BufferEffectFunc)(
//				CDuration *pCRole,
//				STC_CONF_CARD_SKILL *pConf,
//				ByteBuffer &by
//				);

		/* ====================  OPERATORS     ======================================= */
		static void Init();

		static CardSkillFunc FindSkill(uint16_t fun)
		{
			return (m_mapFuncSkill.find(fun) == m_mapFuncSkill.end() ? NULL : m_mapFuncSkill[fun]);
		}

//		static SkillConditionFunc FindCondition(uint16_t fun)
//		{
//			return (m_mapFuncCondition.find(fun) == m_mapFuncCondition.end() ? NULL : m_mapFuncCondition[fun]);
//		}
//
//		static BufferEffectFunc FindBuffer(uint16_t fun)
//		{
//			return (m_mapFuncBuffer.find(fun) == m_mapFuncBuffer.end() ? NULL : m_mapFuncBuffer[fun]);
//		}

//		static int CardAttack(CDuration *pCRole, CDuration *pPeerCRole, ByteBuffer &by);

		// return 0: success -1: fail
		static int CardSkillAttack(CDuration *pCRole, CDuration *pPeerCRole, SSkillStu *pConf, ByteBuffer &by, SDurationIn in);
		static int CardSkillDefence(CDuration *pCRole, CDuration *pPeerCRole, SSkillStu *pConf, ByteBuffer &by, SDurationIn in);

//		// return 0: success 1: fail
//		static int SkillConditionHealth(CDuration *pCRole, SCondition *pStu, int rate);
//		static int SkillConditionRound(CDuration *pCRole, SCondition *pStu, int round);
//		static int SkillConditionRoundDown(CDuration *pCRole, SCondition *pStu, int rounddown);
//
//		static int SkillConditionHealth30(CDuration *pCRole, SCondition *pStu);
//		static int SkillConditionHealth40(CDuration *pCRole, SCondition *pStu);
//		static int SkillConditionHealth50(CDuration *pCRole, SCondition *pStu);
//		static int SkillConditionHealth70(CDuration *pCRole, SCondition *pStu);
//		static int SkillConditionHealth100(CDuration *pCRole, SCondition *pStu);
//		static int SkillConditionRound1(CDuration *pCRole, SCondition *pStu);
//		static int SkillConditionRound2(CDuration *pCRole, SCondition *pStu);
//		static int SkillConditionRound2Down(CDuration *pCRole, SCondition *pStu);
//		static int SkillConditionRound3Down(CDuration *pCRole, SCondition *pStu);
//
//		// return 0: success -1: fail
//		static int BufferEffectPosition(CDuration *pCRole, STC_CONF_CARD_SKILL *pConf, ByteBuffer &by);
////		static int BufferEffectHalt(CDuration *pCRole, STC_CONF_CARD_SKILL *pConf, ByteBuffer &by);
////		static int BufferEffectRecognize(CDuration *pCRole, STC_CONF_CARD_SKILL *pConf, ByteBuffer &by);
//		static int BufferEffectAddHealth(CDuration *pCRole, STC_CONF_CARD_SKILL *pConf, ByteBuffer &by);
//		static int BufferEffectAddAttack(CDuration *pCRole, STC_CONF_CARD_SKILL *pConf, ByteBuffer &by);
//		static int BufferEffectAddDefence(CDuration *pCRole, STC_CONF_CARD_SKILL *pConf, ByteBuffer &by);
//		static int BufferEffectReduceAttack(CDuration *pCRole, STC_CONF_CARD_SKILL *pConf, ByteBuffer &by);
//		static int BufferEffectReduceDefence(CDuration *pCRole, STC_CONF_CARD_SKILL *pConf, ByteBuffer &by);
////		static int BufferEffectDelBuffer(CDuration *pCRole, STC_CONF_CARD_SKILL *pConf, ByteBuffer &by);
////		static int BufferEffectUnvalidBuffer(CDuration *pCRole, STC_CONF_CARD_SKILL *pConf, ByteBuffer &by);
////		static int BufferEffectTarget(CDuration *pCRole, STC_CONF_CARD_SKILL *pConf, ByteBuffer &by);
//
//		static void GetTargetAttackSpecialOne(CDuration *pCRole, CDuration *pPeerCRole, std::vector<uint32_t> &vecTargetObjId);
//		static void GetTargetAttackOne(CDuration *pCRole, CDuration *pPeerCRole, std::vector<uint32_t> &vecTargetObjId);
//		static void GetTargetAttackColumn(CDuration *pCRole, CDuration *pPeerCRole, std::vector<uint32_t> &vecTargetObjId);
//		static void GetTargetAttackOneTrough(CDuration *pCRole, CDuration *pPeerCRole, std::vector<uint32_t> &vecTargetObjId);
//		static void GetTargetAttackFrontLine(CDuration *pCRole, CDuration *pPeerCRole, std::vector<uint32_t> &vecTargetObjId);
//		static void GetTargetAttackBehindLine(CDuration *pCRole, CDuration *pPeerCRole, std::vector<uint32_t> &vecTargetObjId);
//		static void GetTargetAttackAll(CDuration *pCRole, CDuration *pPeerCRole, std::vector<uint32_t> &vecTargetObjId);
//		static void GetTargetDefenceOne(CDuration *pCRole, CDuration *pPeerCRole, std::vector<uint32_t> &vecTargetObjId);
//		static void GetTargetDefenceAll(CDuration *pCRole, CDuration *pPeerCRole, std::vector<uint32_t> &vecTargetObjId);
//		static void GetTargetDefenceMyself(CDuration *pCRole, CDuration *pPeerCRole, std::vector<uint32_t> &vecTargetObjId);

	protected:
		/* ====================  METHODS       ======================================= */

		/* ====================  DATA MEMBERS  ======================================= */

	private:
		/* ====================  METHODS       ======================================= */

		/* ====================  DATA MEMBERS  ======================================= */
		static std::map<uint16_t, CardSkillFunc> m_mapFuncSkill;
//		static std::map<uint16_t, SkillConditionFunc> m_mapFuncCondition;
//		static std::map<uint16_t, BufferEffectFunc> m_mapFuncBuffer;

}; /* -----  end of class CCardSkill  ----- */

#endif
