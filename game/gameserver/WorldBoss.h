/*
 * =====================================================================================
 *
 *       Filename:  WorldBoss.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2013年03月21日 05时00分59秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  imane (), imanecr@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#ifndef _WORLDBOSS_H_
#define _WORLDBOSS_H_

#include <stdint.h>
#include "WorldPacket.h"
#include "ObjAdmin.h"
#include "Combat.h"
#include "Notice.h"

/*
 * =====================================================================================
 *        Class:  CObjCtrl
 *  Description:  
 * =====================================================================================
 */
class CWorldBoss: public CObjCtrl
{
	public:
		/* ====================  LIFECYCLE     ======================================= */
		CWorldBoss ();                             /* constructor */
		virtual ~CWorldBoss ();                    

		/* ====================  ACCESSORS     ======================================= */

		/* ====================  OPERATORS     ======================================= */

		CStu* GetInnerData(uint32_t key);
		bool SaveInnerData(CStu *p);

	protected:
		/* ====================  METHODS       ======================================= */

		/* ====================  DATA MEMBERS  ======================================= */

	private:
		/* ====================  METHODS       ======================================= */

		/* ====================  DATA MEMBERS  ======================================= */

}; /* -----  end of class CObjCtrl  ----- */


/*
 * =====================================================================================
 *        Class:  CStu
 *  Description:  
 * =====================================================================================
 */

class CFBoss;
class CFriendBoss: public CStu
{
	public:
		/* ====================  LIFECYCLE     ======================================= */
		typedef struct Record
		{
			uint32_t userid;
			std::string username;
			uint32_t time;
			uint32_t skillindex;
			uint16_t skilllevel;
			uint32_t totalharm;
		}SRecord;

		typedef struct Prize
		{
			uint32_t key;
			uint32_t userid;
			std::string username;
			uint32_t masterindex;
			uint16_t viewlv;
			uint16_t lv;
			uint32_t t;
			uint8_t number;
			uint32_t time;
		}SPrize;

		typedef struct Attacked
		{
			uint32_t key;
			uint32_t userid;
			std::string username;
			uint32_t masterindex;
			uint16_t lv;
			uint32_t time;
		}SAttacked;

		CFriendBoss ();                             /* constructor */
		virtual ~CFriendBoss ();
		int Init(uint32_t userid);
		int SaveData();
		void OnTimer5s();
		/* ====================  ACCESSORS     ======================================= */
		uint32_t GetUserId() {return m_UserId;}
		uint32_t GetBossId() {return m_BossIndex;}
		uint16_t GetBossLv() {return m_Level;}

		/* ====================  OPERATORS     ======================================= */
		void HandlerBossInfo(CRole *pCRole);
		void HandlerBossHarmHistoryInfo(CRole *pCRole);
		void HandlerBossHarmAdd(CRole *pCRole, SRecord &re);
		void HandlerHappenBoss(CRole *pCRole, uint32_t BossIndex);
		void HandlerDurationBoss(CRole *pCRole);
		void HandlerBossPrizeInfo(CRole *pCRole);
		void HandlerBossAttackedInfo(CRole *pCRole);
		void HandlerBossGacha(CRole *pCRole, WorldPacket &pkg);

		void PkgBossInfo(WorldPacket &pkg);
		void PkgBossHarmHistoryInfo(WorldPacket &pkg);
		bool IsAlreadyPrize(uint32_t prize_id);
		void AddPrize(Prize &p);
		void AddAttacked(Attacked &p);
		uint16_t GetBossPrizeLv(uint32_t BossIndex, CRole *pCRole);
		void ChangeBossLv(uint32_t bossindex);

	private:
		/* ====================  METHODS       ======================================= */
		int SeriliazeDB(ByteBuffer &by);
		int DeSeriliazeDB(ByteBuffer &by);

		/* ====================  DATA MEMBERS  ======================================= */
		uint32_t m_BossIndex;
		uint32_t m_StartTime;
		uint32_t m_BossHealth;                  /* harm health */
		uint16_t m_Level;

		std::map<uint32_t, uint16_t> m_mapActiveBoss;	
		std::list<SRecord> m_listRecord;
		std::map<uint32_t, Prize> m_mapPrize;
		std::map<uint32_t, Attacked> m_mapAttacked; 
		std::vector<uint32_t> m_vecUserId;
		
		std::list<Prize *> m_listPrize;
		std::list<Attacked *> m_listAttacked;

		uint32_t m_ActiveFlushTime;
		uint8_t m_Type;
		bool IsInit;
		uint32_t m_UserId;
		std::string m_username;
		uint8_t m_FirstPrize;

		uint32_t m_BossAliveTime;
		uint32_t m_BossLvUpPara;
}; /* -----  end of class CStu  ----- */

class CFBoss:public CDuration
{
	public:
		typedef struct BossCard
		{
			BossCard():dwObjId(0), dwIndex(0), byAttribute(0), dwAttack(0), dwDefence(0), dwHealth(0), dwHealthMax(0), dwCritRate(0), dwBigSkill(0), dwBigSkillLv(0), dwCardLv(0), byCardType(0), byRare(0), byBreakCount(0)
			{
			}
			uint32_t dwObjId;	
			uint32_t dwIndex;
			uint8_t byAttribute;
			uint32_t dwAttack;
			uint32_t dwDefence;
			uint32_t dwHealth;
			uint32_t dwHealthMax;
			uint32_t dwCritRate;
			uint32_t dwBigSkill;
			uint32_t dwBigSkillLv;
			uint32_t dwCardLv;
			uint8_t byCardType;
			uint8_t byRare;
			uint8_t byBreakCount;
		} SBossCard;
		int Init(uint32_t index, uint16_t level);
		void OptAllHarmHealth(uint32_t health);
		uint32_t GetAllHealthMax();
		uint32_t GetAllHealth();
		int PrepareDurationData(std::vector<cardforduration_t> &vec, std::vector<uint32_t> &vecline);

	private:
		std::vector<SBossCard> m_vec;	
		std::vector<uint32_t> m_vecline;
		std::string m_strUserName;
		uint32_t m_masterid;
};

#endif
