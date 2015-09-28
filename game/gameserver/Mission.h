/*
 * =====================================================================================
 *
 *       Filename:  Mission.h
 *
 *    Description:  something about Misson
 *
 *        Version:  1.0
 *        Created:  2012年09月03日 16时00分43秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  imane (), imanecr@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef _MISSON_H_
#define _MISSON_H_

#include <stdint.h>
#include "DynamicStruct.h"
#include "World.h"

#define PROPERTY_DEF_MISSION(T,name) \
        T Get##name() { return m_##name;}; \
        void Set##name(T name, bool bSend = false) \
        { m_##name = name; \
        setProperty(E_MISSION_##name, name); \
        if (bSend) SendProUpdate(); \
        ;}; \

struct SMission
{
	uint8_t status;
	uint32_t step;
};

/*
 * =====================================================================================
 *        Class:  CMission
 *  Description:  
 * =====================================================================================
 */
class CMission
{
	public:
		/* ====================  LIFECYCLE     ======================================= */
		CMission();                             /* constructor */
		~CMission();	
		int Init(CRole *pCRole);
		void SaveData();
		int LoadData();
		void OnTimer5s();
		
		int SerializeDB(ByteBuffer &pkg);
		int DeserializeDB(ByteBuffer &pkg);
		/* ====================  OPERATORS     ======================================= */
//		int SerializeClientAll(ByteBuffer &pkg);
//		int SerializeClientDirty(ByteBuffer &pkg);
//		void SendProUpdate();
		
		void EveryWorld(uint32_t objid, uint32_t val);
		void EveryMatch(uint32_t objid, uint32_t val);
		void EveryMaster(uint32_t objid, uint32_t val);
		void EveryRob(uint32_t objid, uint32_t val);
		void EveryStrength(uint32_t objid, uint32_t val);
		void EveryWorldBoss(uint32_t objid, uint32_t val);
		void GetFourCard(uint32_t objid, uint32_t val);
		void GetFiveCard(uint32_t objid, uint32_t val);
		void NormalTask(uint32_t objid, uint32_t val);
		void HighTask(uint32_t objid, uint32_t val);
		void RoleLevel(uint32_t objid, uint32_t val);
		void CardLevel(uint32_t objid, uint32_t val);
		void CardBreakNumber(uint32_t objid, uint32_t val);
		void ContinueLogin(uint32_t objid, uint32_t val);
		void RobSuccess(uint32_t objid, uint32_t val);
		void LearnSkill(uint32_t objid, uint32_t val);
		void CardAwakeNumber(uint32_t objid, uint32_t val);
		void BossAttack(uint32_t objid, uint32_t val);
		void MatchRank(uint32_t objid, uint32_t val);
		void FriendNumber(uint32_t objid, uint32_t val);
		void AvoidTime(uint32_t objid, uint32_t val);
		void MeatCount(uint32_t objid, uint32_t val);
		void WineCount(uint32_t objid, uint32_t val);
		void HaveCard(uint32_t objid, uint32_t val);
		void HaveSwordCard(uint32_t objid, uint32_t val);
		void HaveHandCard(uint32_t objid, uint32_t val);
		void HaveInternalForceCard(uint32_t objid, uint32_t val);
		void HaveDarkCard(uint32_t objid, uint32_t val);
		void HaveStickCard(uint32_t objid, uint32_t val);
		void HaveFullBreakCard(uint32_t objid, uint32_t val);
		void Sign(uint32_t objid, uint32_t val);
		void ForceValue(uint32_t objid, uint32_t val);
		void WorldThreeStar(uint32_t objid, uint32_t val);
		void GetCard(uint32_t objid, uint32_t val);
		void SnsShare(uint32_t objid, uint32_t val);
		void BreakTower(uint32_t objid, uint32_t val);//每日闯塔
		void EveryShop(uint32_t objid, uint32_t val);//每日购买
		void EveryGacha(uint32_t objid, uint32_t val);//每日招募
		void EveryCardUpgrade(uint32_t objid, uint32_t val);//每日转生

		void HandlerMissionInfo(uint32_t index);
		void HandlerMissionBaseInfo();
		void HandlerGetPrize(WorldPacket &packet);
		void HandlerGetActivePrize(WorldPacket &packet);
		void HandlerMissionActiveInfo();
		void GMAddActive(uint32);

	protected:
		/* ====================  METHODS       ======================================= */

		/* ====================  DATA MEMBERS  ======================================= */

	private:
		/* ====================  METHODS       ======================================= */

		/* ====================  DATA MEMBERS  ======================================= */
		bool m_IsInit;
//		bool m_IsUpdate;
		std::map<uint32_t, SMission> m_mapAchieve;
		CRole *m_pCRole;
		uint32_t m_dwAchieve;
		uint32_t m_dwFlushTime;

		uint32_t m_dwActive;
		std::set<uint32_t> m_setActive;
}; /* -----  end of class CMission  ----- */

#endif
