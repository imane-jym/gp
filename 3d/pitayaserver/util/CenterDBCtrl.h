#ifndef __CENTER_DB_CTRL__
#define __CENTER_DB_CTRL__

#include "DatabaseMysql.h"
#include "GmCommand.h"
#include <map>
#include "ByteBuffer.h"

#define GET_ARRAY_LENGTH(array) 	sizeof(array)/sizeof(array[0])

namespace CenterDB
{

enum ServerLoginEnable
{
	E_SERVER_LOGIN_ENABLE_FALSE	= 0,
	E_SERVER_LOGIN_ENABLE_TRUE	= 1,
};

enum ServerStatus
{
	E_SERVER_STATUS_NORMAL		= 0,
	E_SERVER_STATUS_NEW			= 1,
	E_SERVER_STATUS_HOT			= 2,
	E_SERVER_STATUS_MAINTAIN	= 3,
	E_SERVER_STATUS_INTERNAL 	= 4,

	E_SERVER_STATUS_COUNT
};

enum RoleStatus
{
	E_ROLE_STATUS_NORMAL		= 0,
	E_ROLE_STATUS_FORBID		= 1,
	E_ROLE_STATUS_BAN_SPEAK		= 2,
	E_ROLE_STATUS_GUILD			= 3,

	E_ROLE_STATUS_CNT			= 4,
};

enum LoginAuthType
{
	E_LOGIN_AUTH_TYPE_ACCOUNT		= 0,	// 注册帐号登录
	E_LOGIN_AUTH_TYPE_PLATFORM		= 1,	// 第三方平台登录
	E_LOGIN_AUTH_TYPE_FAST			= 2,	// 快速登录
};

enum LoginResultType
{
	E_LOGIN_RESULT_TYPE_OK			= 0,
	E_LOGIN_RESULT_TYPE_NOT_FOUND	= 1,
	E_LOGIN_RESULT_TYPE_WRONG_PWD	= 2,
	E_LOGIN_RESULT_ERROR			= 3,
};

enum ChargeState
{
	E_CHARGE_STATE_UNPAY			= 0,
	E_CHARGE_STATE_PAYED			= 1,
	E_CHARGE_STATE_DISTRIBUTED		= 2,
};

enum ChargeType
{
	E_CHARGE_TYPE_NORMAL			= 0,
	E_CHARGE_TYPE_OMIT				= 2,
};

enum GoodsState
{
	E_GOODS_STATE_OFF_SALE			= 0,
	E_GOODS_STATE_ON_SALE			= 1,
	E_GOODS_STATE_HOT				= 2,
};

enum LoginStrategyType
{
	E_LOGIN_STRATEGY_TYPE_PLATFORM	= 1,
	E_LOGIN_STRATEGY_TYPE_IP		= 2,
	E_LOGIN_STRATEGY_TYPE_AUTH		= 3,
	E_LOGIN_STRATEGY_TYPE_VERSION	= 4,
	E_LOGIN_STRATEGY_TYPE_DEVICE	= 5,
	E_LOGIN_STRATEGY_TYPE_REG_TIME	= 6,

	// 白名单
	E_LOGIN_STRATEGY_TYPE_SVN_VERSION = 7,

	// 黑名单
	E_LOGIN_STRATEGY_TYPE_SVN_VERSION_BLACK_LIST = 8,
	E_LOGIN_STRATEGY_TYPE_DEVICE_NUMBER = 10
};

enum LoginBindResultType
{
	E_LOGIN_BIND_RESULT_TYPE_SUCC			= 0,
	E_LOGIN_BIND_RESULT_TYPE_EMPTY_PASSPORT	= 1,
	E_LOGIN_BIND_RESULT_TYPE_INVALID_PWD_OR_MAIL = 2,
	E_LOGIN_BIND_RESULT_TYPE_PASSPORT_NOT_EXIST	 = 3,
	E_LOGIN_BIND_RESULT_TYPE_DUPLICATE_PASSPORT	 = 4,
	E_LOGIN_BIND_RESULT_TYPE_INVALID_AUTH_TYPE	 = 5,
};

enum NoticeUseType
{
	E_NOTICE_USE_TYPE_LOGIN				= 1,
	E_NOTICE_USE_TYPE_GAME				= 2,
	E_NOTICE_USE_TYPE_CUSTOM_SERVICE	= 3,
	E_NOTICE_USE_TYPE_WEIBO				= 4,
	E_NOTICE_USE_TYPE_UPDATE_ADDR		= 5,
	E_NOTICE_USE_TYPE_HIDE_HERO_TYPE	= 6,
	E_NOTICE_USE_TYPE_HIDE_PAY_TYPE		= 7,
	E_NOTICE_USE_TYPE_COMMENT_ADDRESS	= 8,
	E_NOTICE_USE_TYPE_SPRC_GIFT_CODE	= 9,

	E_NOTICE_USE_TYPE_LOGIN_TIME_RANGE			= 10,
	E_NOTICE_USE_TYPE_GAME_TIME_RANGE			= 11,
	E_NOTICE_USE_TYPE_CUSTOM_SERVICE_TIME_RANGE	= 12,
	E_NOTICE_USE_TYPE_ONLINE_PARA				= 13,
	E_NOTICE_USE_TYPE_CHAT_ADDR					= 14,
	E_NOTICE_USE_TYPE_NOTICE_URL        = 15,
	E_NOTICE_USE_TYPE_CUSTOM_INFO       = 16
};

enum NoticeConditionType
{
	E_NOTICE_CONDITION_TYPE_DEFAULT		= 0,
	E_NOTICE_CONDITION_TYPE_PLATFORM	= 1,
	E_NOTICE_CONDITION_TYPE_SERVER_ID	= 2,
};

enum IOSVersion
{
	E_IOS_VERSION_5			= 5,
	E_IOS_VERSION_6			= 6,
	E_IOS_VERSION_7			= 7,
};

typedef struct STC_SERVER_STATUS
{
	uint32 			dwServerId;
	uint32			dwServerVersionCode;
	std::string		strServerName;
	std::string 	strIp;
	uint32 			dwPort;
	std::string		strVersion;
	std::string		strResVersionFull;
	std::string		strResVersionConfig;
	std::string		strResServerAddr;
	uint32 			dwOnlineNum;
	uint8 			byCanLogin;
	uint8 			byStatus;
	uint32			dwLoginStrategy;
	uint32			dwLastUpdateTime;
	bool 			bIsAlive;
	uint32			dwCanRegister;
	uint8 			dwMergeTimes;
	uint32 			dwLastMergeTime;
	uint32 			dwCenterPort;
} STC_SERVER_STATUS;

typedef struct STC_GOODS_INFO
{
	uint32			dwGoodsId;
	uint8			byShopType;
	uint32			dwBuyTypeId;
	uint32			dwBuyContentId;
	uint32			dwBuyCount;
	uint32			dwCostTypeId;
	uint32			dwCostContentId;
	uint32			dwCostCount;
	uint32			dwCostCountOld;
	GoodsState		byStatus;
	uint32			dwLimitDay;
	uint32			dwSortIdx;
	uint32			dwIconId;
	std::string		strName;
	std::string		strDescription;
	uint32			dwBuyLimitOnce;
	uint32			dwLimitTimeStart;
	uint32			dwLimitTimeEnd;
	uint8			byVipShow;
	uint8			byVipBuy;
	uint32			dwBuyCountTotal;
	uint32			dwBuyCountInc;
	std::string		strPlatformGoodsId;
	uint8			byPlatformType;
	uint32			dwWeight;

	uint32			dwDiamondPay;
	std::string		dwLimitNumber;
	uint32			dwLimitType;
	uint32			dwPrizeFloat;
	uint32			byUseRechargeRate;
	std::string		buyEx;
	std::string		thirdGoodId;
//	std::string		IOSGoodId;
//	std::string		AndroidGoodId;
	std::string		currency;
	uint32			goodGift;
	std::string		itemIcon;
	std::string		collectionIcon;

	uint32          dwPlayerLevel;
	uint32 			dwRechargeRate;
	std::string 	dwExtra;
	std::string 	dwOldPrice;
	STC_GOODS_INFO()
	{
		dwGoodsId = 0;
		byShopType = 0;
		dwBuyTypeId = 0;
		dwBuyContentId = 0;
		dwBuyCount = 0;
		dwCostTypeId = 0;
		dwCostContentId = 0;
		dwCostCount = 0;
		dwCostCountOld = 0;
		byStatus = (GoodsState)0;
		dwLimitDay = 0;
		dwSortIdx = 0;
		dwIconId = 0;
		strName = "";
		strDescription = "";
		dwBuyLimitOnce = 0;
		dwLimitTimeStart = 0;
		dwLimitTimeEnd = 0;
		byVipShow = 0;
		byVipBuy = 0;
		dwBuyCountTotal = 0;
		dwBuyCountInc = 0;
		strPlatformGoodsId = "";
		byPlatformType = 0;
		dwWeight = 0;

		dwDiamondPay = 0;
		dwLimitNumber = "";
		dwLimitType = 0;
		dwPrizeFloat = 0;
		byUseRechargeRate = 0;
		goodGift = 0;
		dwPlayerLevel = 0;
		dwRechargeRate = 0;
		dwExtra = "";
		dwOldPrice = "";
	}
} STC_GOODS_INFO;

typedef struct STC_ACTIVITY_INFO
{
	uint32		dwAutoId;
	uint32		dwServerId;
	uint8		byType;
	std::string	strParam;
	ByteBuffer	data;
	uint32		dwGmCommandId;
} STC_ACTIVITY_INFO;

typedef struct STC_LOGIN_STRATEGY_CONDITION
{
	uint8		byType;
	std::string strValue;

} STC_LOGIN_STRATEGY_CONDITION;

typedef struct STC_LOGIN_STRATEGY
{
	std::vector<
		std::vector< STC_LOGIN_STRATEGY_CONDITION >	// or
	> vvConditions;	// and
} STC_LOGIN_STRATEGY;

typedef struct CHARGE_INFO_STC
{
	uint32		dwAutoId;
	uint32		dwRoleId;
	uint32		dwGoodsId;
	uint32		dwGoodsQuantity;
	std::string	strAddition2;
	uint16		wPlatform;
	uint32		dwValue;

	std::string strAddition5;
	std::string	strAddition3;

	uint32		dwPlatformType;

} STC_CHARGE_INFO;

typedef struct STC_WORLD_BATTLE_SHARE
{
	uint32		dwRoleId;
	std::string	strName;
	uint32		dwServerId;
	std::string	strServerVer;
	std::string strServerName;
	uint32		dwLevel;
	uint32		dwCapHeroId;
	uint32		dwWinCnt;
	uint32		dwCombat;
	uint32		dwUpdateTime;
	uint32		dwRank;

} STC_WORLD_BATTLE_SHARE;

typedef struct STC_CDKEY
{
	std::string		strCDkey;
	uint32			dwChannel;
	uint32			dwStartTime;
	uint32			dwEndTime;
	uint32			dwLimitNumber;
	std::string		strPrize;
	uint8			byStatus;
	uint32			dwBatchId;
} STC_CDKEY;

typedef struct STC_ACTIVITY_CONF
{
	uint32 id;
	std::string type;
	std::string name;
	uint32 begintime;
	uint32 endtime;
	std::string detail;
}STC_ACTIVITY_CONF;

typedef struct STC_SHAREINFO_CONF
{
	uint32 id;
	uint64 role_id;
	uint64 inviter_id;
	uint32 type;
	bool   istest;
	uint32 inviter_time;
	uint32 gift_time;
}STC_SHAREINFO_CONF;

typedef struct STC_SERVER_MERGE
{
	uint32 originid;
	uint32 mergeid;
	uint32 mergetype;
}STC_SERVER_MERGE;

class CenterDBCtrl
{
public:

	static bool ExistsIndex( const char* table, const char* index );
	static bool ExistsColumn( const char* table, const char* column );
	static bool ExistsTable( const char* table );
	static bool InitCenterDB( DatabaseMysql* db );
	static bool InitCenterDB( DatabaseMysql* db, uint8 byLoginServerId );

	static uint32 NextRoleId();
	static uint64 NextPassportId( uint16 wPlatformId );

	static uint32 GetDBTime();

	static bool ExistsPassport( uint64 dwPassport );
	static bool ExistsPassport( std::string strPassport, uint8 byAuthType, uint16 wPlatform );
	static bool ExistsRole( uint32 dwRoleId );

	static bool UpdateGameServerInfo(
			uint32 			dwServerId,
			std::string		strServerName,
			std::string		strIp,
			std::string		strLocalIp,
			uint32 			dwPort,
			std::string 	strResServerAddr,
			uint32 			dwOnlineNum,
			std::string 	strVersion,
			std::string		strResVersionFull,
			std::string		strResVersionConfig,
			uint8			byCanLogin,
			uint8 			byStatus,
			uint32			dwLoginStrategyId,
		    uint32			dwCanRegister,
			uint8 			dwMergeTimes,
			uint32 			dwLastMergeTime,
			uint32 			dwCenterPort	);
	static uint32 GetServerIdMerged( uint32 dwServerId );
	static void GetInfoMerged(uint32 dwServerId, uint32& dwMergedServerId, uint32& dwMergedType);
	static bool GetMergedListInfo(std::map<uint32, STC_SERVER_MERGE>& out);
	static uint32 GetServerIdOriginByRoleId(uint32 roleId);

	static bool UpdateServerVersionCode( uint32 dwGameServerId, uint32 dwServerVersionCode );
	static bool UpdateServerMergeTimes( uint32 dwGameServerId, uint8 dwMergeTimes);
	static bool UpdateServerLastMergeTime( uint32 dwGameServerId, uint32 mergetime);
	static uint8 GetServerMergeTimes( uint32 dwGameServerId);
	static uint32 GetServerLastMergeTime( uint32 dwGameServerId);
	static uint32 GetServerVersionCode( uint32 dwGameServerId );
	static int GetLoginStrategyId( uint32 dwGameServerId );
	static bool UpdateCenterPort( uint32 dwCenterPort, uint32 dwGameServerId);
	static uint32 GetCenterPort( uint32 dwGameServerId);

	static LoginResultType ValidateAuthAccount(
			std::string 	strPassport,
			std::string 	strPwd,
			uint16			wPlatform,
			uint64&			odwPassportId );

	static LoginResultType ValidateAuthPlatform(
			std::string		strPlatformToken,
			std::string		strUid,
			std::string 	strDeviceToken,
			uint16			wPlatform,
			std::string		strRegIp,
			std::string		strRegDevice,
			std::string		strRegDeviceType,
			std::string		strOpenUdid,
			std::string		strAppleUdid,
			uint64&			odwPassportId );

	static LoginResultType ValidateAuthFast(
			std::string		strUid,
			std::string		strOpenUdid,
			std::string		strAppleUdid,
			uint8			byIOSVersion,
			std::string		strDeviceToken,
			uint16			wPlatform,
			std::string		strRegIp,
			std::string		strRegDevice,
			std::string		strRegDeviceType,
			uint64&			odwPassportId );

	static bool ModifyPassword( uint64 dwPassportId, std::string strNewPwd );
	static bool ModifyPassword( uint32 dwRoleId, std::string strOldPwd, std::string strNewPwd );
	static bool ModifyPlatform( uint64 passportId, uint16 platform );

	static bool InsertPassportInfo(
			std::string		strPassport,
			std::string		strPwd,
			std::string		strMail,
			std::string		strUid,
			std::string		strToken,
			uint16			wPlatform,
			uint8			byAuthType,
			uint32			dwCreateTime,
			uint8			byGmAuth,
			std::string		strCreateIp,
			std::string		strCreateDevice,
			std::string		strCreateDeviceType,
			std::string		strOpenUdid,
			std::string		strAppleUdid,
			uint64&			odwPassportId );

	static bool GetPassportInfo(
			uint64			dwPassportId,
			uint16&			owPlatform,
			std::string&	ostrPlatformId,
			uint8&			obyGmAuth );
	static bool GetCountryNameByPassportId(uint64_t dwPassportId, std::string &country_name);
	static uint32 GetPassportRegTime( uint64 dwPassportId );

	static std::string GetDevice( uint64 dwPassportId );

	static uint64 GetPassportId( uint32 dwRoleId );

	static bool GetLoginStrategy(
			uint32				dwStrategyId,
			STC_LOGIN_STRATEGY& oStrategy );

	static bool BackupPassportOfRole( DatabaseMysql* dstDB, uint32 dwRoleId );

	static bool GetOrUpdateGameServerStatus(
			std::map<uint32, STC_SERVER_STATUS>& mapServer );

	static bool UpdateClosedGameServer( uint32 dwServerId );

	static int GetOrInsertRoleId( uint64 dwPassportId, uint32 dwServerId, uint32 &roleId, uint32 canRegister);
	static bool IsRoleForbid( uint32 dwRole );

	static bool RegisterPassport(
			std::string		strPassport,
			std::string		strPwd,
			std::string		strMail,
			std::string		strUid,
			std::string		strToken,
			uint16			wPlatform,
			std::string		strRegIp,
			std::string		strRegDevice,
			std::string		strRegDeviceType,
			std::string		strOpenUdid,
			std::string		strAppleUdid );

	static bool InsertOrUpdateRoleInfo(
			uint32			dwRoleId,
			std::string		strRoleName,
			uint32			dwProgress,
			uint32			dwLevel,
			uint32			dwGold,
			uint32			dwDiamond,
			uint32			dwCurStage,
			uint32			dwCurTrain,
			uint32			dwVipLevel,
			uint32			dwVipExp,
			uint32			dwStamina,
			uint32			dwEnergy,
			uint32			dwMainQuestId,
			uint32			dwDiamondPay );

	static bool InsertOrUpdateRoleDetailInfo(
				uint32			dwRoleId,
				uint32			dwDiamondPay );

	static bool InsertOrUpdateRoleLastOp(
				uint32			dwRoleId,
				uint32			dwDiamondPay );

	static uint32 InsertActivity(
			uint32			dwServerId,
			uint8			byTypeId,
			std::string		strParam,
			ByteBuffer&		data,
			uint32			dwGmCommandId );

	static bool UpdateActivity(
			uint32			dwAutoId,
			ByteBuffer&		data );

	static bool UpdateTimeCardRecord( uint32 dwRoleId, std::string szCardInfo );

	static bool GetAllActivity(
			std::vector<STC_ACTIVITY_INFO>& vActivity, uint32 dwGameServerId );
	static bool DeleteAllActivity( uint32 dwGameServerId );

	static bool DeleteActivity( uint32 dwAutoId );

	static bool UpdateRoleLastLoginTime(
			uint32			dwRoleId
	);

	static bool UpdatePassportGmAuth(
			uint64			dwPassportId,
			uint8			byGmAuth
	);

	static bool GetHeroBegsInfo( uint32 dwRoleId, std::string &strBegsInfo );

	static bool InsertOrUpdateOperStatisticsInfo( uint32 dwRoleId, ByteBuffer &data );

	static bool UpdateRoleGmAuth(uint32	dwRoleId, uint8	byGmAuth, bool toSamePassportRoles=false);

	static bool UpdateRoleGmAuthByPassport(uint64 dwPassportId, uint8 byGmAuth);

	static bool ClearPassportGmAuth();

	static bool ClearRoleGmAuth();

	static uint32 InsertLoginInfo(
			uint32 			dwRoleId,
			std::string		strRegIp,
			std::string		strRegDevice,
			std::string		strRegDeviceType );
	static bool InsertLogoutInfo( uint32 dwAutoId );

	static bool GetExtendReward( const char* strTableName, const char* strId, uint32& dwItemId, uint32& dwItemCnt, uint8& byState );
	static bool SetExtendRewardGot( const char* strTableName, const char* strId, uint8 byState );

	static uint32 GethandledTotalCharge( uint32 account_id );
	static bool GethandledTotalChargeAll( std::map<uint32, uint32> &mapCharge );

	static bool GetUnhandledCharge(
			std::list<STC_CHARGE_INFO>& vCharges );

	static bool ChargeHandled(
			uint32		dwAutoId,
			uint32		dwDiamondValue,
			std::string strIp,
			std::string strDevice,
			std::string strDeviceType,
			std::string strDeviceUid,
		    uint32		diamondPay	);

	static uint32 CreateCharge(
			uint32		dwRoleId,
			uint32		dwGoodsId,
			uint32		dwGoodsQty,
			std::string	strCurrency,
			uint32		dwValue,
			std::string	strInnerOrderId,
			std::string strPlatformOrderId,
			std::string strPlatformAccount,
			uint16		wPlatform,
			uint16		wPaymentType,
			uint32		dwPaymentTime,
			std::string	strClientOrderId,
		    std::string	addition2	);

	static bool HasCharge( uint16 wPlatform, std::string strPlatformOrderId );

	static uint32 InsertPurchaseInfo(
			uint32			dwRoleId,
			uint32			dwGoodsId,
			uint32			dwGoodsQuantity,
			uint32			dwValue,
			uint32			dwDiamondPaidUse,
			uint32			dwTime );

//	static bool GetNoticeOfServer( uint32 dwServerId, std::string& strNotice );
//	static bool UpdateOrInsertNotice( uint32 dwServerId, std::string strNotice );

	static std::string GetNotice( NoticeUseType eUseType, NoticeConditionType eCondType, uint32 dwCondValue );
	static bool GetNoticeTimeRange( NoticeUseType eUseType, NoticeConditionType eCondType, uint32 dwCondValue, uint32 &dwStartTime, uint32 &dwEndTime );

	// return passport_id
	static uint64 UpdateRoleToken( uint32 dwRoleId, std::string strToken );

	static void GetGoodsInfoOfGameServer( std::map<uint32, STC_GOODS_INFO>& vGoods, uint32 dwGameServerId );
	static void UpdateGoodsInfoOfGameServerOnly( std::map<uint32, STC_GOODS_INFO>& vGoods, uint32 dwGameServerId, uint8 byTypeOnly );
	static void UpdateGoodsInfoOfGameServerExcept( std::map<uint32, STC_GOODS_INFO>& vGoods, uint32 dwGameServerId, uint8 byTypeEx );
	static void UpdateGoodsInfoOfGameServerAll( std::map<uint32, STC_GOODS_INFO>& vGoods, uint32 dwGameServerId );

	static uint8 GetGmAuth( uint32 dwRoleId );
	static uint16 GetPlatformId( uint32 dwRoleId );

	static uint32 GetRoleCreateTime( uint32 dwRoleId );
	static std::string GetRoleName( uint32 dwRoleId );
	static uint16 GetRoleServerId( uint32 dwRoleId );
	static int GetRolePlatform( uint32 dwRoleId ); // return -1 if error

	static bool SetRoleStatus( uint32 dwRoleId, uint8 byStatus );
	static uint8 GetRoleStatus( uint32 dwRoleId );

	static uint16 GetLastLoginServer( uint64 dwPassportId );
	static bool SetLastLoginServer( uint64 dwPassportId, uint16 wServerId );

	static bool IsTestServer( uint32 dwServerId );

	//////////////////////////////////////
	/////////// Gift Box /////////////////
	static bool GetGiftCodeInfo( uint32 dwId, uint32& odwParam1, uint32& odwParam2, uint32& odwParam3,
			std::string& ostrReward, std::string& ostrServers, std::string& ostrPlatform, uint32& odwDeadTime, uint32& odwMaxUse );
	static bool CheckGiftCodeUsed( uint32 dwId, uint32 dwIdx );
	static uint32 GetGiftCodeSameTypeUsed( uint32 dwRoleId, uint32 dwId );
	static bool InsertGiftCodeUse( uint32 dwRoleId, uint32 dwId, uint32 dwIdx );

	//////////////////////////////////////
	/////////// Invite Info //////////////
	static bool InsertInviteInfo( uint32 dwRoleId, uint32 dwLevel, uint32 dwInviterId );
	static bool UpdateInviteLevel( uint32 dwRoleId, uint32 dwLevel );
	static bool GetInviteLevelList( uint32 dwRoleId, std::vector<uint32>& vLevel, std::vector<uint32>& vId );
	static uint32 GetInviterId( uint32 dwRoleId );


	////////////////////////////////////
	//////////// Bind //////////////////
	static uint8 BindPassport( uint32 dwRoleId, LoginAuthType eAuthType, std::string strPassport,
			std::string strPassword = "", std::string strMail = "" );

	///////////////////////////////////
	/////////// Activity Stat /////////
	static void AddActivityStat( uint32 dwActivityId, uint32 dwType, uint32 dwRoleId, uint32 dwReachIdx, uint32 dwReachTime );

	///////////////////////////////////
	//////////// Guild Reg ////////////
	static uint32 RegisterGuild( uint32 dwRoleId );

	//////////////////////////////////
	//// World Battle Cache //////////
	static bool GetPlayerBlobShare( ByteBuffer& buf, uint32 accountId, const char* field );
	static bool SetPlayerBlobShare( ByteBuffer& buf, uint32 accountId, const char* field );
	static bool GetPlayerInfoShareAll( std::vector<STC_WORLD_BATTLE_SHARE>& vRoles, uint32 dwServerId );
	static bool SetPlayerInfoShare( uint32 dwRoleId, uint32 dwServerId, const char* strName, uint32 dwLevel, uint32 dwCapHeroId,
			uint32 dwWinCnt, uint32 dwCombat );

	//////////////////////////////////
	//// Player Rank /////////////////
	static bool InsertOrUpdatePlayerRank( uint32 dwServerId, uint8 byRankType, uint32 dwRank, std::string strName, uint32 dwValue );

	////////////////////////////////////
	/////  获取单个server的信息 //////////
	static bool GetServerName( uint32 dwServerId, std::string &strServerName );
	static bool GetServerIdAndName(std::map<uint32, std::string>& out);

	static bool GetCDKEYAndVerify(STC_CDKEY &t);
	static int GetCount(uint32 batch_id, uint32 characterid, uint32 channel);
	static bool UpdateCDKEY(std::string cdkey, uint32 characterid);
	//////////////////////////////////////
	/////////// GM Command ///////////////

	static bool InitGmCommand();

	// poll in game server
	static bool ReadNewGmCommand();
	static bool UpdateGmCommand();
	static bool HasGmCommand( uint32 dwGmCommandId );
	static bool CancelGmCommand( uint32 dwGmCommandId );
	static const GmCommand* GetGmCommand( uint32 dwGmCommandId );
	static uint32 HandlerGmCommandRole( void* pRole, uint32 dwLastCmdTime );
	static uint32 CreateSysGmCommand( std::string strOpr, std::string strParams, uint8 byTargetType,
			uint32 dwTargetId, uint32 dwStartTime, uint32 dwEndTime );

	////////// login token checking /////////////
	static void CreateLoginToken( uint64 ddwPassportId, std::string strIp );
	static void ClearLoginToken( uint32 dwRoleId );
	static bool CheckWhiteIP(std::string strIp);
	static bool CheckLoginToken( uint32 dwRoleId, std::string strIp, bool &isWhite );
	static void CreateLoginTokenByRoleId( uint64 roleId, uint32 time);

	// 玩家信息解析相关
	static bool SetPlayerDetailInfo(uint32 dwRoleId, std::string colum, std::string &buf );
	static bool GetUserBetweenLoginTime(uint32 dwServerId, uint32 dwBeginTime, uint32 dwEndTime, std::vector<uint32> &vecAccount  );

	static bool SetExchangeRecordInfo(uint32 dwRoleId, uint32 dwGoodId, uint32 dwGoodNum, uint32 dwTime, uint32 dwValue );

	static void UpdateActivityConf( std::map<uint32, STC_ACTIVITY_CONF>& vActs, uint32 dwGameServerId );
	static void DeleteActivityConf(std::string ids_str, uint32 dwGameServerId);
	static void ClearActivityConf(uint32 dwGameServerId);

	static void GetShareInfo(std::vector<STC_SHAREINFO_CONF>& vShareInfo);
	static void UpdateShareInfo(uint32 id, uint32 type = 0);
private:

	static void UpdateGoodsInfoOfGameServer( std::map<uint32, STC_GOODS_INFO>& vGoods, uint32 dwGameServerId );

	static void ReadCommands( QueryResult* result );
	static bool AppendGmCommand( GmCommand* pCommand );
	static bool RemoveGmCommand( GmCommand* pCommand );

	static DatabaseMysql* 	m_db;
	static uint8			m_byLoginServerId;
	static uint32			m_maxPassportAutoInc;
	static uint32			m_maxRoleAutoInc;
	static bool 			m_bReadOnly;

	static uint32			m_dwMaxHandledGmCommandId;
	static uint32			m_dwMaxRunningGmCommandTime;

	typedef std::map<uint32, GmCommand*> GmCommandMapType;
	static GmCommandMapType m_mapGmCommandAll;

};

}




#endif
