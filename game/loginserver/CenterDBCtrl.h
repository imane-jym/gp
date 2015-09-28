#ifndef __CENTER_DB_CTRL__
#define __CENTER_DB_CTRL__

#include "DatabaseMysql.h"
#include "GmCommand.h"
#include <map>
#include "ByteBuffer.h"

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

	E_SERVER_STATUS_COUNT
};

enum RoleStatus
{
	E_ROLE_STATUS_NORMAL		= 0,
	E_ROLE_STATUS_FORBID		= 1,
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
};

enum NoticeType
{
	E_NOTICE_USE_TYPE_LOGIN 			= 1,
	E_NOTICE_USE_TYPE_GAME 				= 2,
	E_NOTICE_USE_TYPE_CUSTOMER_SERVICE 	= 3
};

typedef struct STC_SERVER_STATUS
{
	uint32 			dwServerId;
	std::string		strServerName;
	std::string 	strIp;
	uint32 			dwPort;
	std::string		strVersion;
	std::string		strResVersion;
	std::string		strResServerAddr;
	uint32 			dwOnlineNum;
	uint8 			byCanLogin;
	uint8 			byStatus;
	uint32			dwLoginStrategy;
	uint32			dwLastUpdateTime;
	bool 			bIsAlive;
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

typedef struct STC_CHARGE_INFO
{
	uint32		dwAutoId;
	uint32		dwRoleId;
	uint32		dwGoodsId;
	uint32		dwGoodsQuantity;
} STC_CHARGE_INFO;

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
			std::string		strResVersion,
			uint8			byCanLogin,
			uint8 			byStatus,
			uint32			dwLoginStrategyId );
	
	static bool InsertRePassport(
			uint64			odwPassportId,	
			uint32			dwCharacterId,
			uint32			dwServerId
			);

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
			uint64&			odwPassportId );

	static bool InsertOrUpdatePassportInfo(
			uint64			odwPassportId,	
			std::string		strPassport,
			std::string 	strPwd,
			std::string		strMail,
			std::string		strUid,
			std::string 	strDeviceToken,
			uint16			wPlatform,
			std::string		strRegIp,
			std::string		strRegDevice,
			std::string		strRegDeviceType,
			uint8			byAuthType,
			uint32			dwLastLoginTime
			);

	static bool InsertOrUpdateRoleInfo(
			uint32			dwRoleId,
			std::string		strRoleName,
			uint8			byGm,
			uint8			byStatus,
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
			uint32			dwMainQuestId
	);

	static bool UpdateRoleLastLoginTime(
			uint32			dwRoleId
	);


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
			uint64&			odwPassportId );

	static LoginResultType ValidateAuthFast(
			std::string		strUid,
			std::string		strDeviceToken,
			uint16			wPlatform,
			std::string		strRegIp,
			std::string		strRegDevice,
			std::string		strRegDeviceType,
			uint64&			odwPassportId );

	static bool GetPassportInfo(
			uint64			dwPassportId,
			uint16&			owPlatform,
			uint8&			obyGmAuth );

	static bool GetLoginStrategy(
			uint32				dwStrategyId,
			STC_LOGIN_STRATEGY& oStrategy );

	static bool BackupPassportOfRole( DatabaseMysql* dstDB, uint32 dwRoleId );

	static bool GetOrUpdateGameServerStatus(
			std::map<uint32, STC_SERVER_STATUS>& mapServer );

	static bool UpdateClosedGameServer( uint32 dwServerId );

	static uint32 GetOrInsertRoleId( uint64 dwPassportId, uint8 byServerId );
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
			std::string		strRegDeviceType );

	static uint32 InsertActivity(
			uint32			dwServerId,
			uint8			byTypeId,
			std::string		strParam,
			ByteBuffer&		data,
			uint32			dwGmCommandId );

	static bool UpdateActivity(
			uint32			dwAutoId,
			ByteBuffer&		data );

	static bool GetAllActivity(
			std::vector<STC_ACTIVITY_INFO>& vActivity, uint32 dwGameServerId );

	static bool DeleteActivity( uint32 dwAutoId );

	static bool UpdatePassportGmAuth(
			uint64			dwPassportId,
			uint8			byGmAuth
	);

	static bool UpdateRoleGmAuth(
			uint32			dwRoleId,
			uint8			byGmAuth );

	static uint32 InsertLoginInfo(
			uint32 			dwRoleId,
			std::string		strRegIp,
			std::string		strRegDevice,
			std::string		strRegDeviceType );
	static bool InsertLogoutInfo( uint32 dwAutoId );

	static bool GetUnhandledCharge(
			std::list<STC_CHARGE_INFO>& vCharges );

	static bool ChargeHandled(
			uint32		dwAutoId,
			uint32		dwDiamondValue,
			std::string strIp,
			std::string strDevice,
			std::string strDeviceType,
		    std::string strDeviceUid	
			);

	static uint32 InsertPurchaseInfo(
			uint32			dwRoleId,
			uint32			dwGoodsId,
			uint32			dwGoodsQuantity,
			uint32			dwValue,
			uint32			dwTime );

	static bool GetNoticeOfServer( uint32 dwServerId, std::string& strNotice );
	static bool GetNoticeByType( uint32 dwTypeId, std::vector<std::string> &vecstrNotice);
	static bool UpdateOrInsertNotice( uint32 dwServerId, std::string strNotice );

	// return passport_id
	static uint64 UpdateRoleToken( uint32 dwRoleId, std::string strToken );

	static void GetGoodsInfoOfGameServer( std::map<uint32, STC_GOODS_INFO>& vGoods, uint32 dwGameServerId );
	static void UpdateGoodsInfoOfGameServer( std::map<uint32, STC_GOODS_INFO>& vGoods, uint32 dwGameServerId );

	static uint8 GetGmAuth( uint32 dwRoleId );

	static uint32 GetGmCommandUpdateTime()
	{
		return m_dwGmCommandUpdateTime;
	}

	//////////////////////////////////////
	/////////// GM Command ///////////////
	static void UpdateErrorstr(GmCommand *cmd);

	static bool InitGmCommand();

	// poll in game server
	static bool ReadNewGmCommand();
	static bool UpdateGmCommand();
	static bool HasGmCommand( uint32 dwGmCommandId );
	static bool CancelGmCommand( uint32 dwGmCommandId );
	//static bool HandlerGmCommandServer( DatabaseMysql* dbGameServer );
	static uint32 HandlerGmCommandRole( void* pRole, uint32 dwLastCmdTime );

private:

	static void ReadCommands( QueryResult* result );
	static bool AppendGmCommand( GmCommand* pCommand );
	static bool RemoveGmCommand( GmCommand* pCommand );

	static DatabaseMysql* 	m_db;
	static uint8			m_byLoginServerId;
	static uint32			m_maxPassportAutoInc;
	static uint32			m_maxRoleAutoInc;
	static bool 			m_bReadOnly;

	static uint32			m_dwMaxHandledGmCommandId;

	typedef std::map<uint32, GmCommand*> GmCommandMapType;
	static GmCommandMapType m_mapGmCommandAll;
	static uint32 			m_dwGmCommandUpdateTime;
};

#endif
