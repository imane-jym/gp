/*
 * =====================================================================================
 *
 *       Filename:  Other.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2013年02月04日 13时21分05秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  imane (), imanecr@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#ifndef _OTHER_H_
#define _OTHER_H_

#include <vector>
#include <set>
#include <stdint.h>
#include "WorldPacket.h"
#include "CommonType.h"
#include "GmCommand.h"
#include "EnumDefines.h"

#define READER_CSV_INT(value) \
		do{\
		value = atol(csv.GetCell(i, j++).c_str());\
		}while(0)

#define READER_CSV_FLOAT(value) \
	do{\
	value = atof(csv.GetCell(i, j++).c_str());\
	}while(0)

#define READER_CSV_STR(value) \
	do{\
		value = csv.GetCell(i, j++);\
	}while(0)

#define IS_READER_CSV_ERROR \
        do{\
            if (csv.GetErrorStr() != "") \
            {\
				IME_ERROR("csv reader fail %s", csv.GetErrorStr().c_str());\
				return false;\
            }\
        }while(0)

class CRole;
/*
 * =====================================================================================
 *        Class:  CAnnouncement
 *  Description:  
 * =====================================================================================
 */
class CAnnouncement
{
	public:
		/* ====================  LIFECYCLE     ======================================= */
		CAnnouncement ();                             /* constructor */
		bool Init();

		/* ====================  OPERATORS     ======================================= */
		void HandlerUpdateContent(const std::string &str, uint32_t dwId);
		void HandlerInfo(CRole *pCRole);

	protected:
		/* ====================  METHODS       ======================================= */

		/* ====================  DATA MEMBERS  ======================================= */

	private:
		/* ====================  METHODS       ======================================= */

		/* ====================  DATA MEMBERS  ======================================= */
		std::vector<std::string> m_vecContent;
}; /* -----  end of class CAnnouncement  ----- */


/*
 * =====================================================================================
 *        Class:  CActive
 *  Description:  
 * =====================================================================================
 */
class CActive
{
	public:
		/* ====================  LIFECYCLE     ======================================= */
		CActive ();                             /* constructor */
		int Init(CRole *pCRole);
		void SaveAllData();
		int LoadAllConf();
		/* ====================  OPERATORS     ======================================= */

		void OnTimer5s();
		void HandlerInfo();

		// gift for cd timer unused
//		void HandlerInfoGift();
//		void HandlerRequestGiftPacket(WorldPacket &packet);
//		int LoadConfGift();
//		void SaveDataGift();

		// sign
		void HandlerInfoSign();
		uint8_t IsSigned();
		void SaveDataSign();
		void HandlerLoginSignActive(WorldPacket &packet);
		void HandlerSignClear();
		void SetExtraSign(uint32_t number)
		{
			m_dwUsedExtra = number;
		}

		//turn table
		
		//energy and strength recover
		void HandlerInfoRecover();
		void HandlerRecover(WorldPacket &packet);
		void SaveDataRecover();
		void RecoverOntimer5s();
		uint32 GetRecoverCD(uint32 &startime, uint32 &endtime);

		//friend boss rank
		void ChangeDevote(int32_t devote);
		void HandlerFriendBossRank();
		void SaveDataFriendbossRank();
		void FriendBossRankOntimer5s();
		void HandlerDevoteInfo();

		//common active opt
		void HandlerCommonActiveOpt(WorldPacket &packet);
		void SaveCommonActiveData(uint32_t activeindex);
		void HandlerCommonActiveInfo(uint32_t activeindex);
		void CommonActiveOntimer5s();
		void ChangeCommonActive(uint32_t activeindex, SCommonActivePara para, uint8_t IsUpdate);

		//cdkey
		void HandlerCDKeyGet(WorldPacket &packet);

//		//charge single
//		void HandlerChargeSingleGet();
//		void HandlerChargeSingleInfo();
//		void SaveDataChargeSingle();
//		void ChangeChargeSingle(uint32_t rmb);
//
//		//charge accumulate
//		void HandlerChargeAccumulateGet();
//		void HandlerChargeAccumulateInfo();
//		void SaveDataChargeAccumulate();
//		void ChangeChargeAccumulate(uint32_t rmb);
//
//		//charge cycle
//		void HandlerChargeCycleGet();
//		void HandlerChargeCycleInfo();
//		void SaveDataChargeCycleInfo();
//		void ChangeChargeCycle(uint32_t rmb);

	protected:
		/* ====================  METHODS       ======================================= */

		/* ====================  DATA MEMBERS  ======================================= */

	private:
		/* ====================  METHODS       ======================================= */
		void SeriliazeDBGift(ByteBuffer &by);
		void DeSeriliazeDBGift(ByteBuffer &by);

		void SeriliazeDBSign(ByteBuffer &by);
		void DeSeriliazeDBSign(ByteBuffer &by);

		void SeriliazeDBRecover(ByteBuffer &by);
		void DeSeriliazeDBRecover(ByteBuffer &by);

		void SeriliazeDBFBRank(ByteBuffer &by);
		void DeSeriliazeDBFBRank(ByteBuffer &by);
		/* ====================  DATA MEMBERS  ======================================= */
		CRole *m_pCRole;
		bool m_IsInit;
		
		/* ==================gift==================== */
		uint32_t m_dwGiftTime;
		uint32_t m_dwGetNumber;
		uint32_t m_dwMaxNumber;
		uint32_t m_dwCD;
		uint32_t m_dwAcceptTime;
		uint32_t m_dwRandomIndex;
		/* ==================Sign==================== */
//		uint8_t m_bySign;
		uint32_t m_dwSign;
		uint32_t m_dwSignTime;
		uint32_t m_dwSignFlushTime;
		uint32_t m_dwUsedExtra;
		/* =================Recover=================== */
		uint16_t m_wEnergyUsed;
		uint16_t m_wStrengthUsed;
		uint8_t m_byRecover;
		uint32_t m_dwRecoverEndTime;
		/* =================boss rank=================== */
		uint32_t m_dwFBRankFlushTime;
		uint32_t m_dwDevote;
		int m_dwRank;
		/* =================single charge=================== */
		uint32_t m_dwSingleChargeTime;
		std::map<uint32_t, uint32_t> m_mapSingleChargeVal; /* already get */
		std::map<uint32_t, uint32_t> m_mapSingleChargeAgree; /* allow get */
		/* =================accumulate charge=================== */
		uint32_t m_dwAccumulateChargeTime;
		uint32_t m_dwAccumulateVal;
		std::set<uint32_t> m_setAccumulateCharge; /* already get */
		/* =================cycle charge=================== */
		uint32_t m_dwCycleChargeTime;
		std::map<uint32_t, uint32_t> m_mapCycleChargeVal;
		uint32_t m_dwCyclePoint;
		/* =================cycle login=================== */
		uint32_t m_dwCycleLoginTime;
		uint32_t m_dwLoginCount;
		uint8_t m_byOk;
		std::vector<uint32_t> m_vecCycleLogin;
		uint32_t m_dwCycleLoginTime2;
		/* =================accumulate login=================== */
		uint32_t m_dwAccumulateFlushTime;
		uint32_t m_dwAccumulateLoginTime;
		uint32_t m_dwAccumulateLoginCount;
		uint8_t m_byAccumulateOk;
		/* =================fund active=================== */
		std::map<uint32_t, SFundArr> m_mapFund;
}; /* -----  end of class CActive  ----- */

/*
 * =====================================================================================
 *        Class:  CActiveCtrl
 *  Description:  
 * =====================================================================================
 */
#define MAX_ACTIVE_TIME_COUNT 20

struct SActiveData
{
	uint8_t byType;
	std::vector<uint32_t> vecPara;
	uint32_t dwStart_t;
	uint32_t dwEnd_t;
	GmCommand *pGm;
	std::string strTitle;
	std::string strContent;
};

#define TIME_DELAY_ALL 1
#define TIME_EVERYDAY 2
#define TIME_CD 3

class CActiveCtrl
{
	public:
		/* ====================  LIFECYCLE     ======================================= */
		CActiveCtrl ();                             /* constructor */

		static bool GetData();
		static void SaveData();
		static void HandlerActiveInfo(CRole *pCRole, uint8_t ignore);
		static void HandlerActiveBase(CRole *pCRole, uint8_t ignore);
		static void OnTimer1s();
		/* ====================  OPERATORS     ======================================= */
		static bool IsActiveStart(uint32_t activeid);
		static uint8_t IsUpdate;

//		static bool IsCardMultiBossActive(uint32_t cardindex);
//		static bool IsFriendBossActive(uint32_t friendbossindex);

		// every day sign active
//		static uint8_t LoginSign; 

		// multi power card active		
//		static uint8_t CardMultiBoss;

		// friend boss active
		static uint8_t FriendBoss;
		static uint32_t FriendBossRealStartTime;
//		static uint32_t FriendBossStartTime;
//		static uint32_t FriendBossEndTime;
		static void SetFriendBoss(uint8_t opt)
		{
			FriendBoss = opt;
		}
		static void SetFriendBossRealStartTime(uint32_t val)
		{
			FriendBossRealStartTime = val;
		}
		static uint32_t FriendBossIndex;
		static uint32_t FriendBossPro;

		// turntable active 
		static uint8_t NormalTurntable;
		static uint8_t SuperTurntable;
		static std::vector<STruntable> vecNormalTurnTable;
		static std::vector<STruntable> vecSuperTurnTable;
		static void GetTurnResult(CRole *pCRole, uint8_t choose, uint32_t number, std::vector<int> &vectmp);

		// exp Add
		static uint8_t ExpAdd;
		static uint8_t ExpMulti;
		static std::vector<uint32_t> vecExpMission;
		static uint32_t GetMultiExp(uint32_t taskindex, uint32_t val);

		// coin add
		static uint8_t CoinAdd;
		static uint8_t CoinMulti;
		static std::vector<uint32_t> vecCoinMission;
		static uint32_t GetMultiCoin(uint32_t taskindex, uint32_t val);

		// obj drop
		static uint8_t ObjDrop;
		static uint8_t ObjDropMulti;
		static std::vector<uint32_t> vecObjDropMission;
		static uint32_t GetMultiObjDrop(uint32_t taskindex, uint32_t val);

		// friend boss rank
		static uint8_t FriendBossRank;
		static std::vector<SFBRankGift> vecFBRankPrize;
		static std::vector<SRank> vecFBRankSec;
		static std::map<uint32_t, SFBRankRole> mapPlayerRank; /* userid -> player info */
		static std::vector<SFBRank> vecFBRank; 
		static std::map<uint32_t, SFBRankRole> mapPlayerLastRank; /* userid -> player info */
		static std::vector<SFBRank> vecFBLastRank;
		static std::string FBContent;
		static uint32_t FBRankFlushTime;
		static uint32_t FBRankGrowTime;
		static void UpdateRank(SFBRankRole tmp, int &rank);
		static void RankData(int rank, CRole *pCRole);
		static int PreDataForRank(uint32_t userid, uint8_t flag, WorldPacket &pkg); /* flag 0 now, 1 last */
		
		//charge single
		static uint8_t SingleCharge;
		static std::map<uint32_t, SChargePrize> mapSingleChargePrize;
		static uint32_t SingleChargeFlushTime;

		//charge accumulate
		static uint8_t AccumulateCharge;
		static std::map<uint32_t, SChargePrize> mapAccumulateChargePrize;
		static uint32_t AccumulateChargeFlushTime;

		//charge cycle
		static uint8_t CycleCharge;
		static std::map<uint32_t, SChargePrize> mapCycleChargePrize;
		static uint32_t CycleChargeFlushTime;
		
		//extra login
		static uint8_t ExtraLogin;
		static std::map<uint32_t, SExtraLogin> mapExtraLogin;
		static uint32_t ExtraLoginFlushTime;
		static uint32_t GetFirstRecord();

		//fund
		static uint8_t Fund;
		static std::map<uint32_t, SFundActive> mapFund;
		static std::string FundMailTitle;
		static std::string FundMailContent;
		
		//active task
		static uint8_t Task[TASK_ACTIVE_MAX];
		static std::vector<std::vector<uint32_t> > vecTask;
		static uint32_t TaskFlushTime[TASK_ACTIVE_MAX];
		static bool TaskCheck(uint32_t index);

		/* ===============time type data ============ */
		static bool ActivePreHandler(uint32_t index);
		static bool ActiveAfrHandler(uint32_t index);

		static bool IsPrepare(uint32_t activeindex);
		static bool CalculateTime(GmCommand *cmd, SActiveData &act, uint32_t t);
		static std::map<uint32_t, SActiveData> m_mapT; 
		static void UpdateActive();
		static void AddActive(uint32_t activeindex, std::string strParams, std::string &errormsg, uint32_t start_t, uint32_t end_t);
		static void DelActive(uint32 activeindex, std::string &errormsg);
		static GmCommand *CreateActive(uint32_t activeindex, std::string strParams, uint8_t status, uint32_t start, uint32_t end, std::string msg);
		static bool InitActive();
		static void SaveActive();
}; /* -----  end of class CActiveCtrl  ----- */

class FriendBossActive : public GmCommand
{
	virtual bool Validate();
	virtual bool Trigger();
	virtual bool Complete();
private:
	uint32_t m_BossIndex;
	uint32_t m_BossPro;
//	uint32_t m_ItemId;
//	uint32_t m_ItemProBase;
//	uint32_t m_CardId;
//	uint32_t m_CardProBase;
//	uint32_t m_ProIncrease;
};

class TurntableNormalActive: public GmCommand
{
	virtual bool Validate();
	virtual bool Trigger();
	virtual bool Complete();
private:
//	uint8 m_byChoose;
	std::vector<STruntable> m_vecNormal;
//	std::vector<STruntable> m_vecSuper;
};

class TurntableSuperActive: public GmCommand
{
	virtual bool Validate();
	virtual bool Trigger();
	virtual bool Complete();
private:
//	uint8 m_byChoose;
//	std::vector<STruntable> m_vecNormal;
	std::vector<STruntable> m_vecSuper;
};

class ExpAddActive: public GmCommand
{
	virtual bool Validate();
	virtual bool Trigger();
	virtual bool Complete();
private:
	uint8_t m_byMulti;
	std::vector<uint32_t> m_vecMission;
};

class CoinAddActive: public GmCommand
{
	virtual bool Validate();
	virtual bool Trigger();
	virtual bool Complete();
private:
	uint8_t m_byMulti;
	std::vector<uint32_t> m_vecMission;
};

class ObjDropActive: public GmCommand
{
	virtual bool Validate();
	virtual bool Trigger();
	virtual bool Complete();
private:
	uint8_t m_byMulti;
	std::vector<uint32_t> m_vecMission;
};

class BossRankActive: public GmCommand
{
	virtual bool Validate();
	virtual bool Trigger();
	virtual bool Complete();
	virtual bool PreHandler();
	virtual bool AfrHandler();
private:
	std::string content;
	std::string prize;
	std::string mailtitle;
	std::string mailcontent;
};

//class SingleChargeActive: public GmCommand
//{
//	virtual bool Validate();
//	virtual bool Trigger();
//	virtual bool Complete();
//	virtual bool PreHandler();
//	virtual bool AfrHandler();
//private:
//	std::map<SChargePrize> map;
//};
//
//class AccumulateChargeActive: public GmCommand
//{
//	virtual bool Validate();
//	virtual bool Trigger();
//	virtual bool Complete();
//	virtual bool PreHandler();
//	virtual bool AfrHandler();
//private:
//	std::map<SChargePrize> map;
//};
//
//class CycleChargeActive: public GmCommand
//{
//	virtual bool Validate();
//	virtual bool Trigger();
//	virtual bool Complete();
//	virtual bool PreHandler();
//	virtual bool AfrHandler();
//private:
//	std::map<SChargePrize> map;
//};

class CommonChargeActive: public GmCommand
{
	virtual bool Validate();
	virtual bool Trigger();
	virtual bool Complete();
private:
	uint32_t activeindex;
	std::map<uint32_t, SChargePrize> map;
};

class ExtraLoginActive: public GmCommand
{
	virtual bool Validate();
	virtual bool Trigger();
	virtual bool Complete();
private:
	std::map<uint32_t, SExtraLogin> map;
};

class FundActive: public GmCommand
{
	virtual bool Validate();
	virtual bool Trigger();
	virtual bool Complete();
private:
	std::map<uint32_t, SFundActive> map;
	std::string mailtitle;
	std::string mailcontent;
};

class TaskActive: public GmCommand
{
	virtual bool Validate();
	virtual bool Trigger();
	virtual bool Complete();
private:
	std::vector<uint32_t> vec;
	uint32_t m_activeindex;
};

/*
 * =====================================================================================
 *        Class:  CTimerGift
 *  Description:  
 * =====================================================================================
 */
class CTimerGift
{
	public:
		/* ====================  LIFECYCLE     ======================================= */
		CTimerGift ();                             /* constructor */

		int Init(CRole *pCRole);
		void SaveData();
		void OnTimer5s();

		/* ====================  OPERATORS     ======================================= */
		void HandlerInfo();
		void HandlerRequestGiftPacket(WorldPacket &packet);
		void StartTimer();

	protected:
		/* ====================  METHODS       ======================================= */

		/* ====================  DATA MEMBERS  ======================================= */

	private:
		/* ====================  METHODS       ======================================= */
		void SeriliazeDB(ByteBuffer &by);
		void DeSeriliazeDB(ByteBuffer &by);

		/* ====================  DATA MEMBERS  ======================================= */
		CRole *m_pCRole;
		bool m_IsInit;
//		bool m_IsStart;

		uint32_t m_dwGiftTime;
		int32_t m_dwGiftIndex;

}; /* -----  end of class CTimerGift  ----- */


/*
 * =====================================================================================
 *        Class:  CDataPush
 *  Description:  
 * =====================================================================================
 */
class CDataPush
{
	public:
		/* ====================  LIFECYCLE     ======================================= */
		CDataPush ();                             /* constructor */

		/* ====================  OPERATORS     ======================================= */
		void ClearData();
		void HandlerDataPush(CRole *pCRole);	
		uint8_t m_byCharge;
		uint32_t m_dwChargeNumber;

		void HandlerDataPushVip(CRole *pCRole, uint32_t viplv);
		void HandlerDataPushLevelUp(CRole *pCRole, uint32_t prelv, uint32_t newlv);
		void HandlerDataPushBossAttack(CRole *pCRole, std::string username, uint32_t userid, std::string bossname);
		void HandlerDataPushMissionComplete(CRole *pCRole, uint32_t index);

}; /* -----  end of class CDataPush  ----- */

#endif
