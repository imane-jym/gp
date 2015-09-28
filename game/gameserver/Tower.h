/*
 * Tower.h
 *
 *  Created on: Jan 8, 2014
 *      Author: long
 */

#ifndef TOWER_H_
#define TOWER_H_

#include <stdint.h>
#include <map>
#include "WorldPacket.h"
#include "Combat.h"
const uint32 one = 1;//角色基础攻击力
const uint32 two = 2;//角色2倍基础攻击力
const uint32 three = 3;//角色3倍基础攻击力

class CRole;


class CTower {
public:
	CTower();
	/**
	 * 定时刷新
	 */
	void onTime5s();

	void saveData();

	int init(CRole *pCRole);


	void handlerTowerThreeAttack();
	void handlerTowerTwoAttack();
	void handlerTowerRefreshAttribute();
	void handlerTowerBreakBlockadeTimes(uint32 times);
	/**
	 * load Tower information
	 */
	void handlerTowerInfo();
	/**
	 * deal with combat
	 */
	void handlerTowerCombat(WorldPacket &wpk);
	/**
	 * 查询排行榜
	 */
	void handlerTowerqueryRank();

	typedef struct _STowerData{
			uint32 index;
			uint32  twoAttack;
			uint32 threeAttack;
			uint32 refreshAttribute;
			uint32 sword;
			uint32 hand;
			uint32 internalForce;
			uint32 dark;
			uint32 stick;
			uint32 breakBlockadeTimes;//闯关次数
			uint32 buyTimes;//闯关购买次数
			uint32 health;//当前血量
			uint32 isMaxLayer;
			uint32 timestamp;
	}STowerData;

	uint32 getThreeAttack();
	uint32 getTwoAttack();
	uint32 getBuytimes();

//	void setThreeAttack(uint32 attack);
//	void setTwoAttack(uint32 attack);
//	void setRefreshAttribute(uint32 refresh);
//	void setBreakBlockadeTimes(uint32 time);

	virtual ~CTower();





private:

	//塔信息
	//std::map<uint32,STowerData> m_mapTower;

	STowerData towerData;

	//std::vector<STowerRank> rank;

//	uint32 buyTowerTimes;

	CRole *m_pCRole;

//	uint32 health;
	uint32 healthMax;

	void randomBoss(uint32 nextLayer,STowerData *towerData,uint32 type);

	bool judgeFirstTowerDaily(uint32 userId);

	time_t timestamp;

	class TowerData:public CDuration{
	public :
		typedef struct BossCard{
			BossCard() :dwObjId(0), dwIndex(0), byAttribute(0), dwAttack(0), dwDefence(0), dwHealth(0), dwHealthMax(0), dwCritRate(0), dwBigSkill(0), dwBigSkillLv(0), dwCardLv(0), byCardType(0), byRare(0), byBreakCount(0) {}
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
		int init(STC_CONF_TOWER* configData,uint32 health );
		int PrepareDurationData(std::vector<cardforduration_t> &vec, std::vector<uint32_t> &vecline);
	private:
		std::vector<SBossCard> vec;
		std::string bossName;
		uint32_t masterid;
	};

//	void handlerTowerGlobalConfig();

};





#endif /* TOWER_H_ */
