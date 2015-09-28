/*
 * DBCtrl.h
 *
 *  Created on: 2011-6-21
 *      Author: yq
 */

#ifndef _DB_CTRL_H
#define _DB_CTRL_H
#include "Defines.h"
#include <string>
#include <map>
#include <vector>
#include <set>
#include "CommonType.h"

struct STC_CONF_STORE;
struct sCombRecord;

class CDBCtrl
{
public:

	typedef struct 
	{
		uint32 dwUserid;
		uint16 wLevel;
		std::string strName;
	} stuPlayer;
	typedef struct
	{
		uint32 dwUserid;
		uint16 wLevel;
		std::string strName;
		uint32 dwRank;
	} stuPlayerPlus;

	typedef struct
	{
		uint32 dwMailId;
		uint32 dwSendId;
		std::string strName;
		uint8 byDoflag;
		uint8 byType;
		uint32 dwTime;
		uint8 byDetailType;
		std::string strTitle;
	} stuMail;

	typedef struct
	{
		uint32 dwMailId;
		uint32 dwSendId;
		std::string strName;
		uint8 byDoflag;
		uint8 byType;
		uint32 dwTime;
		std::string content;
		std::string blob;
		uint8 byDetailType;
		std::string title;
	} stuMailDetail;

//	typedef struct
//	{
//		uint32 dwTaskId;
//		uint32 dwStep;
//		uint32 dwTime;
//	} STask;

	typedef struct
	{
		uint32 dwUserId;
		std::string strName;
		uint32 dwVal;
	} SRank;

	typedef struct
	{
		uint32 dwUserId;
		uint32 dwGroupId;
		std::string strContent;
		uint8 byFlag;
		std::string strCreateName;
		std::string strGroupName;
		uint32 dwCredit;
		uint16 wLevel;
		std::string strMember;
	} SGroup;

	typedef struct
	{
		uint64 lGroupId;
		uint32 dwMailId;
		uint8 byDoflag;
		uint8 byType;
		std::string content;
		std::string blob;
		uint32 dwTime;
		uint8 byDetailType;
	} SGroupMail;

	typedef struct
	{
		uint32 userid;
		uint32 rank;
		uint32 ranked;
		uint32 getprestige;
	} SArena;

	//sql 造表
	static bool CreateWorldDB();

	static bool IsNewRole(uint32_t AccountId);

	//创建玩家基本数据
	static uint32 NewUserData(uint32 accountId, std::string & strAccountRet);

//	static uint32 GetUserId(std::string& strAccountRet);

	static bool GetUserBaseData(
//			uint16 &wHeadPic,
			std::string & strUserName,
			uint16 &wLevel,
			uint32 &dwExp,
			uint16 &wVipLv,
			uint32 &dwVipExp,
			uint32 &dwDollar,
			uint32 &dwCoin,
			uint32 &dwEnergy,
			uint32 &dwEnergyMax,
			uint32 &dwEnergyRate,
			uint32 &dwStrength,
			uint32 &dwStrengthMax,
			uint32 &dwStrengthRate,
//			uint32 &dwCardAttributes,
//			uint32 &dwAttackMin,
//			uint32 &dwAttackMax,
//			uint32 &dwDefenceMin,
//			uint32 &dwDefenceMax,
//			uint32 &dwAttackWin,
//			uint32 &dwAttackTot,
//			uint32 &dwDefenceWin,
//			uint32 &dwDefenceTot,
			uint32 dwAccountId,
			uint32 &ObjInitialId,
			uint8 &bytask1,
			uint8 &bytask2,
			uint8 &bytask3,
			uint8 &bytask4,
			uint8 &byCardCap,
//			uint32 &dwCardAttributes2,
//			uint8 &byCurrentTeam,
			uint32 &dwExplimit,
			int32 &dwEnergyCnt,
			int32 &dwStrengthCnt,
			uint8 &byFlag,
			uint64 &lFindGroupId,
			uint32 &dwAvoidDurationTime,
			uint32 &dwWorldHistory,
			uint32 &dwWorldBroadTime,
			uint32 &dwGameFlow,
			uint32 &dwLuck,
			uint8 &byGm,
			uint32 &dwChargeLuck,
			uint32 &dwStamina,
			uint32 &dwStaminaTime,
			std::string &strSignature
				);

	static bool GetUserBaseDataAuto10(const uint32 &dwAccountId, const uint16 &wLevel, std::vector<stuPlayer> &vecPlayer);

	static bool GetUserBaseDataByName(const uint32 &dwAccountId, const std::string & strUserName, std::vector<stuPlayer> &vecPlayer);
	static bool GetUserBaseDataById(const uint32 &dwAccountId, std::vector<stuPlayer> &vecPlayer);
	static bool GetUserBaseDataByNameId(const uint32 &dwAccountId, const std::string &strUserName, std::vector<stuPlayer> &vecPlayer, const uint32 &userid);

	static void SaveUserBaseData(
//			uint16 wHeadPic,
			const std::string &strUserName,
			uint16 wLevel,
			uint32 dwExp,
			uint16 wVipLv,
			uint32 dwVipExp,
			uint32 dwDollar,
			uint32 dwCoin,
			uint32 dwEnergy,
			uint32 dwEnergyMax,
			uint32 dwEnergyRate,
			uint32 dwStrength,
			uint32 dwStrengthMax,
			uint32 dwStrengthRate,
//			uint32 dwCardAttributes,
//			uint32 dwAttackMin,
//			uint32 dwAttackMax,
//			uint32 dwDefenceMin,
//			uint32 dwDefenceMax,
//			uint32 dwAttackWin,
//			uint32 dwAttackTot,
//			uint32 dwDefenceWin,
//			uint32 dwDefenceTot,
			uint32 dwAccountId,
			uint32 ObjInitialId, 
			uint8 bytask1,
			uint8 bytask2,
			uint8 bytask3,
			uint8 bytask4,
			uint8 byCardCap,
//			uint32 dwCardAttributes2,
//			uint8 byCurrentTeam,
			uint32 dwExplimit,
			int32 dwEnergyCnt,
			int32 dwStrengthCnt,
			uint8 byFlag,
			uint64 lFindGroupId,
			uint32 dwAvoidDurationTime,
			uint32 dwWorldHistory,
			uint32 dwWorldBroadTime,
			uint32 dwGameFlow,
			uint32 dwLuck,
			uint8 byGm,
			uint32 dwChargeLuck,
			uint32 dwStamina,
			uint32 dwStaminaTime,
			std::string strSignature
			);

	static bool GetUserBaseDataForDurationPreview(  
			std::string & strUserName,                
			uint32 &dwCoin,                           
			uint16 &wLevel,
			uint32 &dwAccountId                       
			);

        static bool GetUserCardPacketData(std::string &str1team, std::string &strsuit, std::string &cards, const uint32 &dwAccountId, std::string &equip_first, std::string &master, std::string &equip_master);
        static void SaveUserCardPacketData(const std::string &str1team, const std::string &strsuit, const std::string &cards, const uint32 &dwAccountId, const std::string &equip_first, const std::string &equipcards, const std::string &master, const std::string &equip_master, std::string base);
		static bool GetUserItemPacketData(std::string &item, const uint32 &dwAccountId);
		static void SaveUserItemPacketData(const std::string &item, const uint32 &dwAccountId);
		
		static bool InsertItemUser(uint32 item, uint32 userid);
		static bool DeleteItemUser(uint32 item, uint32 userid);
		static bool GetUserbyItem(const uint32 &dwAccountId, const uint32 &dwItemId, const uint16 &wLevel, std::vector<stuPlayer> &vecPlayer);

		static bool GetItemCombine(uint32 userid, uint32 comindex, uint32 &timestamp);
		static bool InsertItemCombine(uint32 userid, uint32 comindex, uint32 timestamp);
		static bool DelItemCombine(uint32 userid, uint32 comindex);
		static bool GetMultiItemCombine(uint32 userid, std::vector<sCombRecord> &comrecord);

		static bool GetUserCardTujianData(std::string &strtujian, uint32 dwAccountId);
		static void SaveUserCardTujianData(const std::string &strtujian, uint32 dwAccountId);

		static bool GetUserTechData(std::string &strtech, uint32 dwAccountId);
		static void SaveUserTechData(const std::string &strtech, uint32 dwAccountId);

		static bool GetUserBaseDataAuto10InFriend(uint32 dwAccountId, uint16 off, std::vector<stuPlayerPlus> &vecPlayer);
		static bool GetUserBaseDataAuto10ForFriend(uint32 dwAccountId, const uint16 &wLevel, std::vector<stuPlayer> &vecPlayer);
		static uint32 GetUserFriendNumber(uint32 dwAccountId);
		static uint32 GetUserRequestFriendNumber(uint32 dwAccountId);

        static bool GetUserCardPacketDataOffOnline(std::string &str1team, std::string &strsuit, std::string &cards, const uint32 &dwAccountId, std::string &equip_first, std::string &master, std::string &equip_master);
		static bool DelFriend(uint32 userid, uint32 friendid);
		static bool InsertFriend(uint32 userid, uint32 friendid);
		static bool IsHaveRole(uint32 userid);
		static bool GetMailInfo(uint32 userid, std::vector<stuMail> &vecMail);
		static bool GetMailDetailInfo(uint32 userid, uint32 mailid, stuMailDetail &Mail);
		static bool DelMail(uint32 userid, uint32 mailid);
		static bool UpdateMail(uint32 userid, uint32 mailid);
		
		static uint32 GetMailId(uint32 userid);
		static bool InsertMail(uint32 userid, const stuMailDetail &Mail); 
		static bool IsFriend(uint32 userid, uint32 friendid);

		static bool GetUserArena(uint32 &userid, uint32 &prestige, uint32 &rank, uint32 &lasttime, uint32 &challenge_number, uint32 &ranked, uint32 &number);
		static bool GetUserArenaAll(std::vector<SArena> &data);
		static bool UpdateUserArenaForRankPrestige(uint32 rank, uint32 getprestige, uint32 userid);

		static void SaveUserArena(uint32_t userid, uint32_t prestige, uint32_t rank, uint32_t lasttime, uint32_t challenge_number, uint32_t ranked, uint32_t number);
		static uint32 GetArenaRank(uint32 userid);
		static bool InsertArena(uint32 userid, uint32 prestige, uint32 rank, uint32 lasttime, uint32 challenge_number);
		static bool GetUserBaseDataArenaTop(uint32 dwAccountId, std::vector<stuPlayerPlus> &vecPlayer);
		static bool GetUserBaseDataArenaLose(uint32 dwAccountId, uint32 rank, std::vector<stuPlayerPlus> &vecPlayer);
		static bool GetUserBaseDataArenaChallenge(uint32 dwAccountId, uint32 rank, std::vector<stuPlayerPlus> &vecPlayer, std::vector<uint32_t> interval);
		static bool InsertArenaLose(uint32 userid, uint32 friendid);
		static bool UpdateUserRank(uint32 rank1, uint32 rank2, uint32 userid1, uint32 userid2);
		static uint32 GetUserTimer(uint32 dwAccountId, uint16 type, uint32 timerid);
		static bool InsertUserTimer(uint32 userid, uint16 type, uint32 timerid, uint32 time);
		static void SaveUserTimer(uint32 userid, uint16 type, uint32 timerid, uint32 time);

//		static bool GetUserMission(uint32 dwAccountId, uint32 task_id, uint32 &time, uint32 &step);
//		static bool GetUserMissionVec(uint32 dwAccountId, std::vector<STask> &vec);
//		static bool InsertUserMission(uint32 userid, uint32 task_id);
//		static void SaveUserMission(uint32 dwAccountId, uint32 task_id, uint32 time, uint32 step);

//		static bool InsertUserRank(uint32 userid, std::string name, uint16 level, uint32 attack, uint32 defence);
//		static void SaveUserRank(uint32 dwAccountId, uint16 level, uint32 attack, uint32 defence);
//		static bool GetUserRank(uint32 dwAccountId, uint16 &level, uint32 &attack, uint32 &defence);
//		static bool GetUserRankLevel(uint32 dwAccountId, std::vector<SRank> &vec);
//		static bool GetUserRankAttack(uint32 dwAccountId, std::vector<SRank> &vec);	
//		static bool GetUserRankDefence(uint32 dwAccountId, std::vector<SRank> &vec);	
		static bool GetUserArenaTop500(std::map<uint32, uint32> &map);	
		
		static bool GetGroupInfoAll(std::vector<SGroup> &vec);
		static bool GetGroupInfo(uint32 userid, uint32 groupid, std::string &content, uint8 &flag);	
		static bool InsertGroup(uint32 userid, uint32 groupid, const std::string &createname, const std::string &groupname);
		static void SaveGroup(uint32 userid, uint32 groupid, const std::string &content, uint8 flag, const std::string &createname, const std::string &groupname, uint16 credit, uint16 level, const std::string &member);
		static uint32 GetGroupId(uint32 userid);
		static bool GetGroupMailInfo(uint64 dwGroupId, std::vector<SGroupMail> &vecMail, uint32 off);
		static bool UpdateGroupMail(uint64 groupid, uint32 mailid, uint8 Do);
		static uint32 GetGroupMailId(uint64 groupid);
		static bool InsertGroupMail(const SGroupMail &Mail);
		static bool GetGroupMailInfoSingle(uint64 dwGroupId, uint32 mailid, SGroupMail &tmpMail);	
		static bool GetUserShopPacketData(std::string &shop, const uint32 &dwAccountId);
		static void SaveUserShopPacketData(const std::string &item, const uint32 &dwAccountId);
		static bool GetShopOrderId(uint32 userid, uint32 &order_id);
		static void LogShopRecord(const uint32 &dwAccountId, uint32 order, uint32 dollar, uint32 shopid, uint32 number, uint32 type);
		static void LogShopProductGive(const uint32 &dwAccountId, uint32 order);
		static bool InsertDurationLose(uint32 userid, uint32 winid);
		static bool GetUserBaseDataAuto10ForLose(uint32 dwAccountId, std::vector<uint32_t> &vecPlayer);	
		static int GetStatusArena(uint32_t userid);
		static bool GetUserCardMasterData(std::string &strMaster, uint32 dwAccountId);
		static void SaveUserCardMasterData(const std::string &strMaster, uint32 dwAccountId);
		static bool GetUserDurationMission(std::string &strMission, uint32 dwAccountId);
		static void SaveUserDurationMission(const std::string &strMission, uint32 dwAccountId);

		static bool GetAnnouncementData(std::vector<std::string> &vecAd);
		static void SaveAnnouncementData(const std::string &strAd, uint32 dwId);

		static bool GetUserDataActive(std::string &str, uint32 AccountId);
		static void SaveUserDataActive(const std::string &str, uint32 AccountId);
		static bool GetLoginInfo(uint32 dwServerid, const std::string &m_strIp, int m_nPort, uint32 onlinenum, uint8_t &can_login, uint8_t &status, uint8_t &is_new, std::string &server_name, std::string &domain, std::string &resurl);
		static void UpdateLoginInfo(uint32 serverid, std::string strIp, int nPort, uint32 dwOnlineNum, uint8_t can_login, uint8_t status, uint8_t is_new, std::string servername, std::string domain, std::string resurl);
		static bool IsCanLogin(uint32 serverid);

		static bool GetUserRob(std::string &strrob, uint32 userid);
		static void SaveUserRob(const std::string &strrob, uint32 userid);
		static bool GetUserFriendBoss(std::string &str, uint32 dwAccountId);
		static void SaveUserFriendBoss(const std::string &strAd, uint32 dwId);
		static bool GetUserFriend(uint32 dwAccountId, std::vector<uint32_t> &vecFriend);
		static bool GetUserBaseDataInFriend(uint32 dwAccountId, std::vector<stuPlayerPlus> &vecPlayer);	
		static bool GetFriendRequest(uint32 friendid, uint32 requestid);
		static bool InsertFriendRequest(uint32 friendid, uint32 requestid);
		static bool DelFriendRequest(uint32 friendid, uint32 requestid);
		static bool GetFriendRequestList(uint32 friendid, uint16 off, std::vector<stuPlayerPlus> &vecPlayer);
		static bool GetUserDataOther(std::string &str, uint32 dwAccountId);
		static void SaveUserDataOther(const std::string &strOther, uint32 dwId);
		static bool GetUserDataTimerGift(std::string &str, uint32 dwAccountId);
		static void SaveUserDataTimerGift(const std::string &strOther, uint32 dwId);
		static bool InsertUserOneAciveData(uint32 userid, uint32 activeid, std::string str);
		static bool GetUserOneActiveData(uint32 userid, uint32 activeid, std::string &str);
		static void SaveUserOneActiveData(const std::string &strAcive, uint32 userid, uint32 activeid);
		static bool GetUserLoginData(
		uint32 accountId,
		uint32 &last_login_time,
		uint32 &last_logout_time,
		uint32 &cur_login_time,
		uint32 &login_day,
		std::string &last_login_ip
		);
		static void SaveUserLoginData(
		uint32 accountId,
		uint32 last_login_time,
		uint32 last_logout_time,
		uint32 cur_login_time,
		uint32 login_day,
		std::string last_login_ip
		);
		static bool GetGlobalActiveData(uint32 activeid, std::string &str);
		static void SaveGlobalActiveData(const std::string &strActive, uint32 activeid);
		static bool InsertGlobalActiveData(uint32 activeid, std::string &str);
		static bool GetUserDataAchieve(std::string &str, uint32 dwAccountId);
		static void SaveUserDataAchieve(const std::string &str, uint32 dwId);
		static bool GetUserTrialData(std::string &str, uint32 dwId);
		static void SaveUserTrialData(std::string str, uint32 dwId);
		static bool GetStoreData(uint32 ServerId, std::vector<STC_CONF_STORE> &vec);
		static bool ReloadStoreData(uint32 ServerId, std::vector<STC_CONF_STORE> vec);
		static void SaveGlobalData(const std::string &strtemp, uint32 id);
		static bool GetGlobalData(uint32 id, std::string &str);
		static bool GetUserName(std::set<std::string> &setUserName);
		static bool GetUserCardPacketBaseData(std::string &str1team, std::string &strsuit, const uint32 &dwAccountId, std::string &equip_first, std::string &master, std::string &equip_master, std::string &base);
		static bool GetAllUserId(std::vector<uint32_t> &vecuserid);
		static void SaveUserCardPacketDataOnlyForBase(const uint32 &dwAccountId, std::string base);
		//tower
		static bool GetRank(std::vector<STowerRank> &rank);
		static void SaveRank(uint32 userId,std::string userName,uint32,uint32,uint32,uint32,uint32 masterIndex);
		//static void SaveRank(uint32 userId,std::string userName,uint32,uint32,uint32,uint32);
		static bool GetUserTowerData(
				uint32 userId,
				uint32 &index,
				uint32  &twoAttack,
				uint32 &threeAttack,
				uint32 &refreshAttribute,
				uint32 &sword,
				uint32 &hand,
				uint32 &innerForce,
				uint32 &dark,
				uint32 &stick,
				uint32 &breakBlockadeTimes,
				uint32 &buyTimes,
				uint32 &health,
				uint32 &isMaxLayer,uint32 &timestamp);
		static bool SaveUserTowerData(
				uint32 userId,
				uint32 index,
				uint32 twoAttack,
				uint32 threeAttack,
				uint32 refreshAttribute,
				uint32 sword,
				uint32 hand,
				uint32 innerForce,
				uint32 dark,
				uint32 stick,
				uint32 breakBlockadeTimes,
				uint32 buyTimes,
				uint32 health,
				uint32 isMaxLayer,uint32 timestamp);
		static void ResetUserTowerData(uint32 user_id,uint32 timestamp);
		static void ClearRankData();
		static void InsertUserRank(uint32 userId,std::string userName,uint32 layer,uint32 times,uint32 userLevel,uint32 ability,uint32 masterIndex);
		static bool GetRankByUserId(uint32 userId);
		static bool GetUserMultipleCardExp(uint32 userId,std::string &str);
		static void SaveUserMultipleCardExp(uint32 userId,std::string str);
		static bool isLimitlogin(uint32 accountId);
		static void setLimitRolelogin(uint32 accountId,uint32 limit);


private:
		static bool ExistsIndex( const char* table, const char* index );
		static bool ExistsColumn( const char* table, const char* column );
		static bool ExistsTable( const char* table );
		static bool ExistsRecord(const char* table, const char *condition);

	CDBCtrl();
	~CDBCtrl();
};
#endif
