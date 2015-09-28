/*
 * =====================================================================================
 *
 *       Filename:  Task.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2012年09月06日 10时23分55秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  imane (), imanecr@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef _TASK_H_
#define _TASK_H_
#include <stdint.h>
#include "Combat.h"
#include "ConfigData.h"

/*
 * =====================================================================================
 *        Class:  CTask
 *  Description:  
 * =====================================================================================
 */
class CRole;
class WorldPacket;

//class CTask
//{
//	public:
//		static void HandlerMissionInfo(CRole *pCRole, WorldPacket &pkg);
//		static void HandlerMissionAccept(CRole *pCRole, WorldPacket &pkg);
////		static void HandlerMissionBranchInfo(CRole *pCRole);
////		static void HandlerMissionBranchAccept(CRole *pCRole, WorldPacket &pkg);
//
//}; /* -----  end of class CTask  ----- */


class CBoss:public CDuration
{
	public:
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
			uint8_t byBreakCount;
		} SBossCard;
		int Init(uint32_t index);
		void AddTujian(CRole *pCRole);
		int PrepareDurationData(std::vector<cardforduration_t> &vec, std::vector<uint32_t> &vecline);
	private:
		std::vector<SBossCard> m_vec;	
		std::vector<uint32_t> m_vecline;
		std::string m_strUserName;
		uint32_t m_masterid;
};

//int ProcessBoss(CRole *pCRole, CBoss *pCBoss, uint32_t dwBossId, uint16_t cmd, int DurationCmd);


/*
 * =====================================================================================
 *        Class:  CDurationMission
 *  Description:  
 * =====================================================================================
 */
class CDurationMission
{
	public:
		/* ====================  LIFECYCLE     ======================================= */
		CDurationMission ();                             /* constructor */
		typedef struct DMission
		{
			uint32_t Index;
			uint16_t Number;
			uint32_t FlushTime;
			uint8_t MaxEva;
		} DMission_t;

		typedef struct BranchMission
		{
			uint32_t dwTaskId;
			uint32_t dwTime;
		} BMission_t;	

		int Init(CRole *pCRole);
		void SaveData();
		/* ====================  OPERATORS     ======================================= */
		void FlushMissionChallenge(uint32_t index, uint8_t type);
		void OnTimer5s();
		void HandlerDurationMissionInfo(uint32_t index = 0);
		void HandlerDurationAccept(WorldPacket &pkg);

		void HandlerHighDurationMissionInfo(uint32_t index = 0);
		void HandlerActiveDurationMissionInfo(uint32_t index = 0);
//		void HandlerHighDurationMissionAccept(WorldPacket &pkg);

		void HandlerMissionBranchInfo(uint32_t index = 0);
		void HandlerMissionBranchAccept(WorldPacket &pkg);
		
		void DelBranchMissionCD(uint32_t index);
		uint32_t GetMissionPosition();

		void SetMissionPosition(uint32_t pos)
		{
			STC_CONF_DURATION_MISSION *p = CConfDurationMission::Find(pos);
			if (p == NULL)
			{
				IME_ERROR("load mission conf fail taskindex %u", pos);
				return;
			}
			if (p->byType != E_MISSION_MAIN)
			{
				IME_ERROR("it is not main mission %u",  pos);
				return;
			}
			m_MissionPosition = pos;
			m_HighMissionPositionLimit = std::max(m_HighMissionPositionLimit, p->dwHighMission);
		}

		void SetHighMissionPosition(uint32_t pos)
		{
			STC_CONF_DURATION_MISSION *p = CConfDurationMission::Find(pos);
			if (p == NULL)
			{
				IME_ERROR("load mission conf fail taskindex %u", pos);
				return;
			}
			if (p->byType != E_MISSION_HIGH)
			{
				IME_ERROR("it is not main mission %u",  pos);
				return;
			}
			m_HighMissionPosition = pos;
		}

		void HandlerMissionDurationAllStarGet(WorldPacket &pkg);
		void HandlerMissionDurationAllStarInfo(uint32_t index);

	protected:
		/* ====================  METHODS       ======================================= */

		/* ====================  DATA MEMBERS  ======================================= */

	private:
		/* ====================  METHODS       ======================================= */
		void SeriliazeDB(ByteBuffer &by);
		void DeSeriliazeDB(ByteBuffer &by);

		/* ====================  DATA MEMBERS  ======================================= */
		std::map<uint32_t, DMission_t> m_mapMission; 
		CRole *m_pCRole;
		bool m_IsInit;
		uint32_t m_MissionPosition;

		uint32_t m_HighMissionPosition;
		uint32_t m_HighMissionPositionLimit;
		std::map<uint32_t, DMission_t> m_mapHighMission;

		std::map<uint32_t, BMission_t> m_mapBranchMission; 
		
		std::map<uint32_t, DMission_t> m_mapActiveMission;

		std::map<uint32_t, uint8_t> m_mapAllStar;
}; /* -----  end of class CDurationMission  ----- */

#endif
