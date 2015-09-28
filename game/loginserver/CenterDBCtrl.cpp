#include "CenterDBCtrl.h"
#include "Log.h"
#include "QueryResult.h"
#include <vector>
#include <string>

DatabaseMysql* 	CenterDBCtrl::m_db = NULL;
uint8			CenterDBCtrl::m_byLoginServerId = 0;
uint32			CenterDBCtrl::m_maxPassportAutoInc = 0;
uint32			CenterDBCtrl::m_maxRoleAutoInc = 0;
bool			CenterDBCtrl::m_bReadOnly = false;
uint32			CenterDBCtrl::m_dwMaxHandledGmCommandId = 0;
uint32 			CenterDBCtrl::m_dwGmCommandUpdateTime = 0;

std::map<uint32, GmCommand*> CenterDBCtrl::m_mapGmCommandAll;

uint32 CenterDBCtrl::NextRoleId()
{
	++m_maxRoleAutoInc;
	return ( m_maxRoleAutoInc << 8 ) | ( (uint32)m_byLoginServerId << 1 ); // | 0  [lowest bit reserved for robot]
}

uint64 CenterDBCtrl::NextPassportId( uint16 wPlatformId )
{
	++m_maxPassportAutoInc;
	return ( ((uint64)m_maxPassportAutoInc) << 32 ) | ( ((uint64)m_byLoginServerId) << 24 ) | ( ((uint64)wPlatformId) << 8 );
}

bool CenterDBCtrl::InitCenterDB( DatabaseMysql* db )
{
	if ( !InitCenterDB( db, 0 ) )
	{
		return false;
	}
	m_bReadOnly = true;
	return true;
}

bool CenterDBCtrl::InitCenterDB( DatabaseMysql* db, uint8 byLoginServerId )
{
	if ( db == NULL )
	{
		IME_SYSTEM_ERROR( "InitCenterDB", "Param Is Null");
		return false;
	}

	if ( m_db )
	{
		IME_SYSTEM_ERROR( "InitCenterDB", "Duplicated Init" );
		return false;
	}

	m_bReadOnly = false;
	m_db = db;
	m_byLoginServerId = byLoginServerId;

//	// gameserver_info
//	if (!ExistsTable("gameserver_info"))
//	{
//	m_db->PExecute(
//		"CREATE TABLE IF NOT EXISTS gameserver_info ("
//			"server_id		SMALLINT	UNSIGNED	NOT NULL,"
//			"server_name	VARCHAR(32)	CHARACTER SET utf8 NOT NULL,"
//			"ip				VARCHAR(64)	CHARACTER SET utf8 NOT NULL,"
//			"local_ip		VARCHAR(64)	CHARACTER SET utf8 NOT NULL,"
//			"port			SMALLINT	UNSIGNED	NOT NULL,"
//			"version		VARCHAR(20)	CHARACTER SET utf8 NOT NULL,"
//			"res_version	VARCHAR(20)	CHARACTER SET utf8 NOT NULL,"
//			"res_server_ip	VARCHAR(64) CHARACTER SET utf8 NOT NULL,"
//			"online_num		MEDIUMINT	UNSIGNED	NOT NULL,"
//			"can_login		TINYINT		UNSIGNED	NOT NULL,"
//			"status			TINYINT		UNSIGNED	NOT NULL,"
//			"update_time	INT			UNSIGNED	NOT NULL,"
//			"login_strategy_id	INT		UNSIGNED	NOT NULL,"
//			"PRIMARY KEY (server_id)"
//		") ENGINE=InnoDB DEFAULT CHARSET=utf8;"
//	);
//	}
//
//	if ( !ExistsColumn( "gameserver_info", "res_server_ip" ) )
//		m_db->PExecute( "ALTER TABLE gameserver_info ADD COLUMN res_server_ip VARCHAR(64)	CHARACTER SET utf8 NOT NULL" );
//	if ( !ExistsColumn( "gameserver_info", "res_version_config" ) )
//		m_db->PExecute( "ALTER TABLE gameserver_info ADD COLUMN res_version_config VARCHAR(20) 	CHARACTER SET utf8 NOT NULL after res_version" );
//
//	// player_info
//	if (!ExistsTable("player_info"))
//	{
//	m_db->PExecute(
//		"CREATE TABLE IF NOT EXISTS player_info ("
//			"role_id		INT			UNSIGNED	NOT NULL,"
//			"name			VARCHAR(32)	CHARACTER SET utf8 NOT NULL,"
//			"gm_auth		TINYINT		UNSIGNED	NOT NULL,"
//			"status			TINYINT		UNSIGNED	NOT NULL,"
//			"progress		INT			UNSIGNED	NOT NULL,"
//			"level			INT			UNSIGNED	NOT NULL,"
//			"gold			INT			UNSIGNED	NOT NULL,"
//			"diamond		INT			UNSIGNED	NOT NULL,"
//			"cur_stage		INT			UNSIGNED	NOT NULL,"
//			"cur_train		INT			UNSIGNED	NOT NULL,"
//			"vip_level		INT			UNSIGNED	NOT NULL,"
//			"vip_exp		INT			UNSIGNED	NOT NULL,"
//			"stamina		INT			UNSIGNED	NOT NULL,"
//			"energy			INT			UNSIGNED	NOT NULL,"
//			"quest			INT			UNSIGNED	NOT NULL,"
//			"last_login_time	INT		UNSIGNED	NOT NULL,"
//			"PRIMARY KEY (role_id)"
//		") ENGINE=InnoDB DEFAULT CHARSET=utf8;"
//	);
//	}
//
//	// passport_info
//	if (!ExistsTable("passport_info"))
//	{
//	m_db->PExecute(
//		"CREATE TABLE IF NOT EXISTS passport_info ("
//			"passport_id	BIGINT		UNSIGNED	NOT NULL,"
//			"passport		VARCHAR(64)	CHARACTER SET utf8 NOT NULL,"
//			"pwd			VARCHAR(64) CHARACTER SET utf8 NOT NULL,"
//			"mail			VARCHAR(64) CHARACTER SET utf8 NOT NULL,"
//			"uid			VARCHAR(128) CHARACTER SET utf8 NOT NULL,"
//			"token			VARCHAR(128) CHARACTER SET utf8 NOT NULL,"
//			"platform		MEDIUMINT	UNSIGNED	NOT NULL,"
//			"auth_type		TINYINT		UNSIGNED	NOT NULL,"
//			"create_time	INT			UNSIGNED	NOT NULL,"
//			"gm_auth		TINYINT		UNSIGNED	NOT NULL,"
//			"reg_ip			VARCHAR(64) CHARACTER SET utf8 NOT NULL,"
//			"reg_device		VARCHAR(32)	CHARACTER SET utf8 NOT NULL,"
//			"reg_device_type VARCHAR(64) CHARACTER SET utf8 NOT NULL,"
//			"last_login_time	INT		UNSIGNED	NOT NULL,"
//			"PRIMARY KEY (passport_id)"
//		") ENGINE=InnoDB DEFAULT CHARSET=utf8;"
//	);
//	}
//
//	if ( !ExistsIndex( "passport_info", "index_passport" ) )
//		m_db->PExecute( "CREATE INDEX index_passport ON passport_info(passport)" );
//
//	// login_strategy
//	if (!ExistsTable("login_strategy"))
//	{
//	m_db->PExecute(
//		"CREATE TABLE IF NOT EXISTS login_strategy ("
//			"auto_id		INT		UNSIGNED	NOT NULL	AUTO_INCREMENT,"
//			"strategy_id	INT		UNSIGNED	NOT NULL,"
//			"condition_id	TINYINT	UNSIGNED	NOT NULL,"
//			"type			TINYINT	UNSIGNED	NOT NULL,"
//			"value			VARCHAR(64) CHARACTER SET utf8 NOT NULL,"
//			"PRIMARY KEY(auto_id)"
//		") ENGINE=InnoDB DEFAULT CHARSET=utf8;"
//	);
//	}
//
//	// re_passport_player
//	if (!ExistsTable("re_passport_player"))
//	{
//	m_db->PExecute(
//		"CREATE TABLE IF NOT EXISTS re_passport_player ("
//			"role_id		INT			UNSIGNED	NOT NULL,"
//			"passport_id	BIGINT		UNSIGNED	NOT NULL,"
//			"server_id		SMALLINT	UNSIGNED	NOT NULL,"
//			"create_time	INT			UNSIGNED	NOT NULL,"
//			"PRIMARY KEY (role_id)"
//		") ENGINE=InnoDB DEFAULT CHARSET=utf8;"
//	);
//	}
//
//	if ( !ExistsColumn( "re_passport_player", "create_time" ) )
//	{
//		m_db->PExecute( "ALTER TABLE re_passport_player ADD COLUMN create_time INT UNSIGNED NOT NULL" );
//	}
//
//	// role_login_info
//	if (!ExistsTable("role_login_info"))
//	{
//	m_db->PExecute(
//		"CREATE TABLE IF NOT EXISTS role_login_info ("
//			"auto_id		INT			UNSIGNED	NOT NULL AUTO_INCREMENT,"
//			"role_id		INT			UNSIGNED	NOT NULL,"
//			"login_time		INT			UNSIGNED	NOT NULL,"
//			"logout_time	INT			UNSIGNED	NOT NULL,"
//			"login_ip				VARCHAR(64) CHARACTER SET utf8 NOT NULL,"
//			"login_device			VARCHAR(32)	CHARACTER SET utf8 NOT NULL,"
//			"login_device_type 		VARCHAR(64) CHARACTER SET utf8 NOT NULL,"
//			"PRIMARY KEY (auto_id)"
//		") ENGINE=InnoDB DEFAULT CHARSET=utf8;"
//	);
//	}
//
//	// goods_info
//	if (! ExistsTable("goods_info"))
//	{
//	m_db->PExecute(
//		"CREATE TABLE IF NOT EXISTS goods_info ("
//			"server_id		SMALLINT	UNSIGNED	NOT NULL,"
//			"goods_id			INT		UNSIGNED	NOT NULL,"
//			"shop_type			INT		UNSIGNED	NOT NULL,"
//			"buy_type_id		INT		UNSIGNED	NOT NULL,"
//			"buy_content_id		INT		UNSIGNED	NOT NULL,"
//			"buy_count			INT		UNSIGNED	NOT NULL,"
//			"cost_type_id		INT		UNSIGNED	NOT NULL,"
//			"cost_content_id	INT		UNSIGNED	NOT NULL,"
//			"cost_count			INT		UNSIGNED	NOT NULL,"
//			"cost_count_old		INT		UNSIGNED	NOT NULL,"
//			"status			TINYINT		UNSIGNED	NOT NULL,"
//			"limit_day			INT		UNSIGNED	NOT NULL,"
//			"sort_idx			INT		UNSIGNED	NOT NULL,"
//			"icon_id			INT		UNSIGNED	NOT NULL,"
//			"goods_name			VARCHAR(64)	CHARACTER SET utf8 NOT NULL,"
//			"description	VARCHAR(256) CHARACTER SET utf8 NOT NULL,"
//			"limit_once			INT		UNSIGNED	NOT NULL,"
//			"PRIMARY KEY (server_id, goods_id)"
//		") ENGINE=InnoDB DEFAULT CHARSET=utf8;"
//	);
//	}
//
//	if ( !ExistsColumn( "goods_info", "cost_count_old" ) )
//		m_db->PExecute( "ALTER TABLE goods_info ADD COLUMN cost_count_old INT UNSIGNED NOT NULL" );
//	if ( !ExistsColumn( "goods_info", "limit_once" ) )
//		m_db->PExecute( "ALTER TABLE goods_info ADD COLUMN limit_once INT UNSIGNED NOT NULL" );
//
//	// notice_info
//	if (!ExistsTable("notice_info_v2"))
//	{
//	m_db->PExecute(
//		"CREATE TABLE IF NOT EXISTS notice_info_v2 ("
//			"auto_id			INT		UNSIGNED	NOT NULL AUTO_INCREMENT,"
//			"use_type			TINYINT UNSIGNED	NOT NULL,"
//			"condition_type		TINYINT UNSIGNED	NOT NULL,"
//			"condition_value	INT UNSIGNED		NOT NULL,"
//			"content	VARCHAR(1024)	CHARACTER SET utf8 NOT NULL,"
//			"PRIMARY KEY (auto_id)"
//		") ENGINE=InnoDB DEFAULT CHARSET=utf8;"
//	);
//	}
//
//	// active_info
//	if (!ExistsTable("active_info"))
//	{
//	m_db->PExecute(
//		"CREATE TABLE IF NOT EXISTS active_info ("
//			"auto_id		INT		UNSIGNED	NOT NULL AUTO_INCREMENT,"
//			"server_id		INT		UNSIGNED	NOT NULL,"
//			"type_id	TINYINT		UNSIGNED	NOT NULL,"
//			"param		VARCHAR(256) CHARACTER SET utf8 NOT NULL,"
//			"act_data		blob,"
//			"gm_cmd_id		INT		UNSIGNED	NOT NULL,"
//			"PRIMARY KEY (auto_id)"
//		") ENGINE=InnoDB DEFAULT CHARSET=utf8;"
//	);
//	}
//
//	// charge_info
//	if (!ExistsTable("charge_info"))
//	{
//	m_db->PExecute(
//		"CREATE TABLE IF NOT EXISTS charge_info ("
//			"auto_id			INT		UNSIGNED	NOT NULL AUTO_INCREMENT,"
//			"role_id			INT		UNSIGNED	NOT NULL,"
//			"goods_id			INT		UNSIGNED	NOT NULL,"
//			"goods_quantity		INT		UNSIGNED	NOT NULL,"
//			"currency			VARCHAR(16) CHARACTER SET utf8 NOT NULL,"
//			"value				INT		UNSIGNED	NOT NULL,"
//			"virtual_value		INT		UNSIGNED	NOT NULL DEFAULT '0',"
//			"type				INT		UNSIGNED	NOT NULL,"
//			"inner_order_id			VARCHAR(128) CHARACTER SET utf8 NOT NULL,"
//			"platform			MEDIUMINT	UNSIGNED	NOT NULL,"
//			"platform_order_id		VARCHAR(128) CHARACTER SET utf8 NOT NULL,"
//			"platform_account_id	VARCHAR(128) CHARACTER SET utf8 NOT NULL,"
//			"platform_payment_type	TINYINT	UNSIGNED	NOT NULL,"
//			"state				TINYINT	UNSIGNED	NOT NULL,"
//			"payment_time		INT	UNSIGNED	NOT NULL DEFAULT '0',"
//			"distribute_time	INT UNSIGNED 	NOT NULL DEFAULT '0',"
//			"payment_ip			VARCHAR(64) CHARACTER SET utf8 NOT NULL DEFAULT '',"
//			"payment_device		VARCHAR(32) CHARACTER SET utf8 NOT NULL DEFAULT '',"
//			"payment_device_type VARCHAR(64) CHARACTER SET utf8 NOT NULL DEFAULT '',"
//			"payment_device_uid  VARCHAR(64) CHARACTER SET utf8 NOT NULL DEFAULT '',"
//			"client_order_id	VARCHAR(128) CHARACTER SET utf8 NOT NULL DEFAULT '',"
//			"addition1			VARCHAR(128) CHARACTER SET utf8 NOT NULL DEFAULT '',"
//			"addition2			VARCHAR(128) CHARACTER SET utf8 NOT NULL DEFAULT '',"
//			"addition3			VARCHAR(128) CHARACTER SET utf8 NOT NULL DEFAULT '',"
//			"addition4			VARCHAR(128) CHARACTER SET utf8 NOT NULL DEFAULT '',"
//			"PRIMARY KEY (auto_id)"
//		") ENGINE=InnoDB DEFAULT CHARSET=utf8;"
//	);
//	}
//
//	// gain_info
//	if (!ExistsTable("gain_info"))
//	{
//	m_db->PExecute(
//		"CREATE TABLE IF NOT EXISTS gain_info ("
//			"auto_id		INT 	UNSIGNED	NOT NULL AUTO_INCREMENT,"
//			"role_id		INT		UNSIGNED	NOT NULL,"
//			"source_type	INT		UNSIGNED	NOT NULL,"
//			"source_id		INT		UNSIGNED	NOT NULL,"
//			"value			INT		UNSIGNED	NOT NULL,"
//			"time			INT		UNSIGNED	NOT NULL,"
//			"PRIMARY KEY(auto_id)"
//		") ENGINE=InnoDB DEFAULT CHARSET=utf8;"
//	);
//	}
//
//	// purchase_info (diamond only)
//	if (!ExistsTable("purchase_info"))
//	{
//	m_db->PExecute(
//		"CREATE TABLE IF NOT EXISTS purchase_info ("
//			"auto_id		INT		UNSIGNED	NOT NULL AUTO_INCREMENT,"
//			"role_id		INT		UNSIGNED	NOT NULL,"
//			"goods_id		INT		UNSIGNED	NOT NULL,"
//			"goods_quantity	INT		UNSIGNED	NOT NULL,"
//			"value			INT		UNSIGNED	NOT NULL,"
//			"time			INT		UNSIGNED	NOT NULL,"
//			"PRIMARY KEY (auto_id)"
//		") ENGINE=InnoDB DEFAULT CHARSET=utf8;"
//	);
//	}
//
//	// gm_cmd
//	if (!ExistsTable("gm_cmd"))
//	{
//	m_db->PExecute(
//		"CREATE TABLE IF NOT EXISTS gm_cmd ("
//			"auto_id			INT		UNSIGNED	NOT NULL AUTO_INCREMENT,"
//			"opr			VARCHAR(32)	CHARACTER SET utf8 NOT NULL,"
//			"params			VARCHAR(8192) CHARACTER SET utf8 NOT NULL DEFAULT '',"
//			"target_type	TINYINT		UNSIGNED	NOT NULL,"
//			"target_id		BIGINT		UNSIGNED	NOT NULL,"
//			"start_time			INT		UNSIGNED	NOT NULL DEFAULT '0',"
//			"end_time			INT		UNSIGNED	NOT NULL DEFAULT '0',"
//			"backup_value	VARCHAR(128) 	CHARACTER SET utf8 NOT NULL DEFAULT '',"
//			"status			TINYINT		UNSIGNED	NOT NULL DEFAULT '0',"
//			"error_msg		VARCHAR(256)	CHARACTER SET utf8 NOT NULL DEFAULT '',"
//			"create_time		INT		UNSIGNED	NOT NULL DEFAULT '0',"
//			"author			VARCHAR(32)	NOT NULL DEFAULT '',"
//			"PRIMARY KEY (auto_id)"
//		") ENGINE=InnoDB DEFAULT CHARSET=utf8;"
//	);
//	}
//
//	if ( !ExistsIndex( "gm_cmd", "index_status" ) )
//		m_db->PExecute( "CREATE INDEX index_status ON gm_cmd(status)" );

	return true;
}

uint32 CenterDBCtrl::GetDBTime()
{
	QueryResult* result = m_db->PQuery( "select unix_timestamp()" );
	if ( result )
	{
		uint32 ret = result->Fetch()[0].GetUInt32();

		delete result;
		return ret;
	}
	else
	{
		IME_SQLERROR( "Error Get DataBase Time" );
	}

	return 0;
}

bool CenterDBCtrl::ExistsPassport( uint64 dwPassport )
{
	QueryResult* result = m_db->PQuery(
		"select passport_id from passport_info where passport_id=%llu", dwPassport
	);
	if ( result )
	{
		delete result;
		return true;
	}
	return false;
}

bool CenterDBCtrl::ExistsRole( uint32 dwRoleId )
{
	QueryResult* result = m_db->PQuery(
		"select role_id from player_info where role_id=%u", dwRoleId );
	if ( result )
	{
		delete result;
		return true;
	}
	return false;
}

bool CenterDBCtrl::UpdateGameServerInfo(
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
			uint32			dwLoginStrategyId )
{
	QueryResult* result = m_db->PQuery(
		"select update_time from gameserver_info where server_id=%u",
		dwServerId
	);

	m_db->escape_string( strIp );
	m_db->escape_string( strServerName );
	m_db->escape_string( strVersion );
	m_db->escape_string( strResVersion );
	m_db->escape_string( strLocalIp );
	m_db->escape_string( strResServerAddr );

	if ( result )
	{
		delete result;
		m_db->PExecute(
			"update gameserver_info set server_name='%s',ip='%s',local_ip='%s', port=%u, version='%s', res_version='%s', "
			"online_num=%u, can_login=%u, status=%u, login_strategy_id=%u, res_server_ip='%s', update_time=%u where server_id=%u", strServerName.c_str(),
			strIp.c_str(), strLocalIp.c_str(), dwPort, strVersion.c_str(), strResVersion.c_str(), dwOnlineNum,
			byCanLogin, byStatus, dwLoginStrategyId, strResServerAddr.c_str(), GetDBTime(), dwServerId
		);
	}
	else
	{
		m_db->PExecute(
			"insert into gameserver_info "
			"(server_id, server_name, ip, local_ip, port, version, res_version, online_num, can_login, status, login_strategy_id, res_server_ip, update_time) "
			"values(%u, '%s', '%s', '%s', %u, '%s', '%s', %u, %u, %u, %u, '%s', %u)",
			dwServerId, strServerName.c_str(), strIp.c_str(), strLocalIp.c_str(), dwPort, strVersion.c_str(), strResVersion.c_str(),
			dwOnlineNum, byCanLogin, byStatus, dwLoginStrategyId, strResServerAddr.c_str(), GetDBTime()
		);
	}
	return true;
}

LoginResultType CenterDBCtrl::ValidateAuthAccount(
			std::string 	strPassport,
			std::string 	strPwd,
			uint16			wPlatform,
			uint64&			odwPassportId )
{
	m_db->escape_string( strPassport );

	QueryResult* result = m_db->PQuery(
		"select passport_id, pwd from passport_info where passport='%s' and platform=%u and auth_type=%u",
		strPassport.c_str(), wPlatform, E_LOGIN_AUTH_TYPE_ACCOUNT
	);

	if ( result )
	{
		Field* field = result->Fetch();
		uint64 dwPassportId 	= field[0].GetUInt64();
		std::string strTruePwd	= field[1].GetString();
		delete result;

		if ( strPwd == strTruePwd )
		{
			odwPassportId = dwPassportId;
			return E_LOGIN_RESULT_TYPE_OK;
		}
		else
		{
			return E_LOGIN_RESULT_TYPE_WRONG_PWD;
		}
	}
	else
	{
		return E_LOGIN_RESULT_TYPE_NOT_FOUND;
	}
}

LoginResultType CenterDBCtrl::ValidateAuthPlatform(
			std::string		strPlatformToken,
			std::string		strUid,
			std::string 	strDeviceToken,
			uint16			wPlatform,
			std::string		strRegIp,
			std::string		strRegDevice,
			std::string		strRegDeviceType,
			uint64&			odwPassportId )
{
	m_db->escape_string( strPlatformToken );
	m_db->escape_string( strUid );
	m_db->escape_string( strDeviceToken );

	QueryResult* result = m_db->PQuery(
		"select passport_id from passport_info where passport='%s' and platform=%u and auth_type=%u",
		strPlatformToken.c_str(), wPlatform, E_LOGIN_AUTH_TYPE_PLATFORM
	);

	if ( result )
	{
		Field* field = result->Fetch();
		uint64 dwPassportId = field[0].GetUInt64();
		delete result;

		odwPassportId = dwPassportId;
		return E_LOGIN_RESULT_TYPE_OK;
	}
	else
	{
		m_db->escape_string( strRegIp );
		m_db->escape_string( strRegDevice );
		m_db->escape_string( strRegDeviceType );

		if ( InsertPassportInfo( strPlatformToken, "", "", strUid, strDeviceToken, wPlatform, E_LOGIN_AUTH_TYPE_PLATFORM,
				GetDBTime(), 0, strRegIp.c_str(), strRegDevice.c_str(), strRegDeviceType.c_str(), odwPassportId ) )
		{
			return E_LOGIN_RESULT_TYPE_OK;
		}
		else
		{
			return E_LOGIN_RESULT_ERROR;
		}
	}
}

bool CenterDBCtrl::InsertOrUpdatePassportInfo(
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
		uint8 			byAuthType,
		uint32			dwLastLoginTime
		)
{
	m_db->escape_string( strPassport );

	QueryResult* result = m_db->PQuery("select passport_id from passport_info where passport='%s' and platform=%u and auth_type=%u",
			strPassport.c_str(), wPlatform, byAuthType);

	if ( result )
	{
		delete result;
		m_db->escape_string( strDeviceToken );
		return  m_db->PExecute("update passport_info set token = '%s',last_login_time = %u where passport_id = '%lld'", strDeviceToken.c_str(), dwLastLoginTime, odwPassportId);
	}
	else
	{
		m_db->escape_string( strPwd );
		m_db->escape_string( strMail );
		m_db->escape_string( strUid );
		m_db->escape_string( strDeviceToken );
		m_db->escape_string( strRegIp );
		m_db->escape_string( strRegDevice );
		m_db->escape_string( strRegDeviceType );

		return InsertPassportInfo( strPassport, strPwd, strMail, strUid, strDeviceToken, wPlatform, byAuthType, GetDBTime(), 0, strRegIp, strRegDevice, strRegDeviceType, odwPassportId);
	}
}

bool CenterDBCtrl::InsertRePassport(
		uint64			odwPassportId,	
		uint32			dwCharacterId,
		uint32			dwServerId
		)
{
	QueryResult* result = m_db->PQuery(
		"select passport_id from re_passport_player where role_id=%u", dwCharacterId
	);
	
	if ( result )
	{
		delete result;
		return true;
	}
	else
	{
		return m_db->PExecute("insert into re_passport_player(role_id, passport_id, server_id, create_time) values(%u, %llu, %u, %u)", dwCharacterId, odwPassportId, dwServerId, time(NULL));
	}
}


LoginResultType CenterDBCtrl::ValidateAuthFast(
		std::string		strUid,
		std::string		strDeviceToken,
		uint16			wPlatform,
		std::string		strRegIp,
		std::string		strRegDevice,
		std::string		strRegDeviceType,
		uint64&			odwPassportId )
{
	m_db->escape_string( strUid );
	m_db->escape_string( strDeviceToken );

	QueryResult* result = m_db->PQuery(
		"select passport_id from passport_info where passport='%s' and platform=%u and auth_type=%u",
		strUid.c_str(), wPlatform, E_LOGIN_AUTH_TYPE_FAST
	);

	if ( result )
	{
		Field* field = result->Fetch();
		uint64 dwPassport = field[0].GetUInt64();
		delete result;

		odwPassportId = dwPassport;
		return E_LOGIN_RESULT_TYPE_OK;
	}
	else
	{
		if ( InsertPassportInfo( strUid, "", "", strUid, strDeviceToken, wPlatform, E_LOGIN_AUTH_TYPE_FAST,
				GetDBTime(), 0, strRegIp, strRegDevice, strRegDeviceType, odwPassportId ) )
		{
			return E_LOGIN_RESULT_TYPE_OK;
		}
		else
		{
			return E_LOGIN_RESULT_ERROR;
		}
	}
}

bool CenterDBCtrl::InsertPassportInfo(
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
			uint64&			odwPassportId )
{
	if ( m_bReadOnly )
	{
		IME_SQLERROR( "ReadOnly Is On" );
		return false;
	}

	m_db->escape_string( strPassport );
	m_db->escape_string( strPwd );
	m_db->escape_string( strMail );
	m_db->escape_string( strUid );
	m_db->escape_string( strToken );
	m_db->escape_string( strCreateIp );
	m_db->escape_string( strCreateDevice );
	m_db->escape_string( strCreateDeviceType );

//	odwPassportId = NextPassportId( wPlatform );

	bool ret = m_db->PExecute(
		"insert into passport_info(passport_id,passport,pwd,mail,uid,token,platform,auth_type,create_time,gm_auth,reg_ip,reg_device,reg_device_type) "
		"values(%llu,'%s','%s','%s','%s','%s',%u,%u,%u,%u,'%s','%s','%s')",
		odwPassportId, strPassport.c_str(), strPwd.c_str(), strMail.c_str(), strUid.c_str(), strToken.c_str(),
		wPlatform, byAuthType, dwCreateTime, byGmAuth, strCreateIp.c_str(),
		strCreateDevice.c_str(), strCreateDeviceType.c_str()
	);

	if ( ret )
	{
		return true;
	}
	else
	{
		IME_SQLERROR("Error Insert Passport");
	}
	return ret;
}

bool CenterDBCtrl::GetPassportInfo(
		uint64			dwPassportId,
		uint16&			owPlatform,
		uint8&			obyGmAuth )
{
	QueryResult* result = m_db->PQuery(
		"select platform, gm_auth from passport_info where passport_id=%llu", dwPassportId
	);

	if ( result )
	{
		Field* field = result->Fetch();

		owPlatform  = field[0].GetUInt16();
		obyGmAuth	= field[1].GetUInt8();

		delete result;
		return true;
	}
	else
	{
		return false;
	}
}

bool CenterDBCtrl::GetLoginStrategy(
		uint32				dwStrategyId,
		STC_LOGIN_STRATEGY& oStrategy )
{
	QueryResult* result = m_db->PQuery(
		"select condition_id, type, value from login_strategy where strategy_id=%u order by condition_id", dwStrategyId );
	if ( result )
	{
		oStrategy.vvConditions.clear();

		int cond = -1;
		uint64 cnt = result->GetRowCount();
		for ( int i = 0; i < cnt; i++ )
		{
			Field* field = result->Fetch();

			if ( (int)field[0].GetUInt8() != cond )
			{
				std::vector<STC_LOGIN_STRATEGY_CONDITION> vCond;
				oStrategy.vvConditions.push_back( vCond );

				cond = (int)field[0].GetUInt8();
			}

			STC_LOGIN_STRATEGY_CONDITION t;
			t.byType	= field[1].GetUInt8();
			t.strValue	= field[2].GetString();

			oStrategy.vvConditions.back().push_back( t );

			result->NextRow();
		}

		delete result;
		return true;
	}
	else
	{
		return false;
	}
}

bool CenterDBCtrl::BackupPassportOfRole( DatabaseMysql* dstDB, uint32 dwRoleId )
{
	if ( dwRoleId & 1 ) return true;

	QueryResult* result = m_db->PQuery(
		"select passport_id from re_passport_player where role_id=%u", dwRoleId
	);

	if ( !result )
	{
		IME_SQLERROR( "Error Backup Passport To GameServer, roleId=%u", dwRoleId );
		return false;
	}
	Field* field = result->Fetch();
	uint64 dwPassportId = field[0].GetUInt64();
	delete result;

	result = m_db->PQuery(
		"select passport,pwd,mail,uid,token,platform,auth_type,create_time,gm_auth,reg_ip,reg_device,reg_device_type from passport_info "
		"where passport_id=%llu", dwPassportId
	);

	if ( !result )
	{
		IME_SQLERROR( "Error Backup Passport To GameServer, passportId=%llu", (long long unsigned int)dwPassportId );
		return false;
	}
	field = result->Fetch();

	bool ret = dstDB->PExecute(
			"insert into passport_info(passport_id,passport,pwd,mail,uid,token,platform,auth_type,create_time,gm_auth,reg_ip,reg_device,reg_device_type) "
			"values(%llu,'%s','%s','%s','%s','%s',%u,%u,%u,%u,'%s','%s','%s')",
			dwPassportId,
			field[0].GetString(),
			field[1].GetString(),
			field[2].GetString(),
			field[3].GetString(),
			field[4].GetString(),
			field[5].GetUInt16(),
			field[6].GetUInt8(),
			field[7].GetUInt32(),
			field[8].GetUInt8(),
			field[9].GetString(),
			field[10].GetString(),
			field[11].GetString()
		);

	delete result;

	if ( !ret )
	{
		IME_SQLERROR( "Insert Error When Backup Passport To GameServer" );
		return false;
	}
	return true;
}

bool CenterDBCtrl::GetOrUpdateGameServerStatus(
		std::map<uint32, STC_SERVER_STATUS>& mapServer )
{
	for ( std::map<uint32, STC_SERVER_STATUS>::iterator it = mapServer.begin();
			it != mapServer.end(); ++it )
	{
		it->second.bIsAlive = false;
	}

	QueryResult *result = m_db->Query(
		"select server_id, server_name, ip,port,version,res_version,online_num,can_login,status,login_strategy_id, res_server_ip, update_time from gameserver_info" );

	if ( result )
	{
		uint64 u64Cnt = result->GetRowCount();
		for(uint64 i = 0; i < u64Cnt; ++i)
		{
			Field *fields = result->Fetch();

			STC_SERVER_STATUS stcServer;

			stcServer.dwServerId 	= fields[0].GetUInt16();
			stcServer.strServerName	= fields[1].GetString();
			stcServer.strIp			= fields[2].GetString();
			stcServer.dwPort		= fields[3].GetUInt32();
			stcServer.strVersion	= fields[4].GetString();
			stcServer.strResVersion	= fields[5].GetString();
			stcServer.dwOnlineNum 	= fields[6].GetUInt32();
			stcServer.byCanLogin	= fields[7].GetUInt8();
			stcServer.byStatus		= fields[8].GetUInt8();
			stcServer.dwLoginStrategy = fields[9].GetUInt32();
			stcServer.strResServerAddr = fields[10].GetString();
			stcServer.dwLastUpdateTime = fields[11].GetUInt32();

			std::map<uint32, STC_SERVER_STATUS>::iterator it = mapServer.find( stcServer.dwServerId );

			if ( it != mapServer.end() )
			{
				uint32 dwPreUpdateTime = it->second.dwLastUpdateTime;
				it->second = stcServer;
				it->second.bIsAlive = ( dwPreUpdateTime != it->second.dwLastUpdateTime );

				if ( !it->second.bIsAlive )
				{
					UpdateClosedGameServer( stcServer.dwServerId );
				}
			}
			else
			{
				stcServer.bIsAlive = false;
				mapServer.insert( std::make_pair( stcServer.dwServerId, stcServer ) );
			}
			result->NextRow();
		}

		delete result;
	}
	return true;
}

bool CenterDBCtrl::UpdateClosedGameServer( uint32 dwServerId )
{
	QueryResult* result = m_db->PQuery(
		"select online_num from gameserver_info where server_id=%u", dwServerId );

	if ( result )
	{
		uint16 wOnlineNum = result->Fetch()[0].GetUInt16();
		delete result;

		if ( wOnlineNum )
		{
			m_db->PExecute("update gameserver_info set online_num=%u where server_id=%u", 0, dwServerId );

			uint32 dwTime = GetDBTime();
			// slow
//			m_db->PExecute("update role_login_info set logout_time=%u where logout_time=0 and role_id in"
//					" ( select role_id from re_passport_player where server_id=%u )", dwTime, dwServerId );
		}

		return true;
	}
	else
	{
		return false;
	}
}

uint32 CenterDBCtrl::GetOrInsertRoleId( uint64 dwPassportId, uint8 byServerId )
{
	QueryResult* result = m_db->PQuery(
		"select role_id from re_passport_player where passport_id=%llu and server_id=%u",
		dwPassportId, byServerId
	);

	if ( result )
	{
		Field* field = result->Fetch();
		uint32 dwRoleId = field[0].GetUInt32();
		delete result;
		return dwRoleId;
	}
	else
	{
		uint32 dwRoleId = NextRoleId();

		if (m_db->PExecute( "insert into re_passport_player(role_id, passport_id, server_id, create_time) values(%u, %llu, %u, %u)",
				dwRoleId, dwPassportId, byServerId, GetDBTime() ) )
		{
			return dwRoleId;
		}
		else
		{
			IME_ERROR( "Error When Insert Into re_passport_player" );
			return 0;
		}
	}
}

bool CenterDBCtrl::IsRoleForbid( uint32 dwRole )
{
	QueryResult* result = m_db->PQuery(
		"select status from player_info where role_id=%u", dwRole
	);
	if ( result )
	{
		uint8 byStatus = result->Fetch()->GetUInt8();
		delete result;
		return byStatus != 0;
	}
	else
	{
		return false;
	}
}

bool CenterDBCtrl::RegisterPassport(
			std::string		strPassport,
			std::string		strPwd,
			std::string		strMail,
			std::string		strUid,
			std::string		strToken,
			uint16			wPlatform,
			std::string		strRegIp,
			std::string		strRegDevice,
			std::string		strRegDeviceType )
{
	m_db->escape_string( strPassport );

	QueryResult* result = m_db->PQuery("select passport_id from passport_info where passport='%s' and platform=%u and auth_type=%u",
			strPassport.c_str(), wPlatform, E_LOGIN_AUTH_TYPE_ACCOUNT );

	if ( result )
	{
		delete result;
		return false;
	}
	else
	{
		m_db->escape_string( strPwd );
		m_db->escape_string( strMail );
		m_db->escape_string( strUid );
		m_db->escape_string( strToken );
		m_db->escape_string( strRegIp );
		m_db->escape_string( strRegDevice );
		m_db->escape_string( strRegDeviceType );

		uint64 dwPassportId;

		return InsertPassportInfo( strPassport, strPwd, strMail, strUid, strToken, wPlatform, E_LOGIN_AUTH_TYPE_ACCOUNT,
				GetDBTime(), 0, strRegIp, strRegDevice, strRegDeviceType, dwPassportId );
	}
}


bool CenterDBCtrl::InsertOrUpdateRoleInfo(
			uint32			dwRoleId,
			std::string		strName,
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
	)
{
//	if ( dwRoleId & 1 ) return true;

	QueryResult* result = m_db->PQuery( "select role_id from player_info where role_id=%u", dwRoleId );
	if ( result )
	{
		delete result;
		std::string strRoleName = strName;
		m_db->escape_string( strRoleName );

//		if ( dwMainQuestId )
//		{
			return m_db->PExecute( "update player_info set name='%s', status = %u,  gm_auth = %u ,progress=%u, level=%u, "
					"gold=%u, diamond=%u, cur_stage=%u, cur_train=%u, vip_level=%u, vip_exp=%u, stamina=%u, energy=%u,"
					"quest=%u where role_id=%u",
					strRoleName.c_str(), byStatus, byGm, dwProgress, dwLevel, dwGold, dwDiamond, dwCurStage, dwCurTrain, dwVipLevel, dwVipExp,
					dwStamina, dwEnergy, dwMainQuestId, dwRoleId
			);
//		}
//		else
//		{
//			return m_db->PExecute( "update player_info set name='%s', progress=%u, level=%u, "
//					"gold=%u, diamond=%u, cur_stage=%u, cur_train=%u, vip_level=%u, vip_exp=%u, stamina=%u, energy=%u where role_id=%u",
//					strRoleName.c_str(), dwProgress, dwLevel, dwGold, dwDiamond, dwCurStage, dwCurTrain, dwVipLevel, dwVipExp,
//					dwStamina, dwEnergy, dwRoleId
//			);
//		}
	}
	else
	{
		std::string strRoleName = strName;
		m_db->escape_string( strRoleName );

		return m_db->PExecute( "insert into player_info( role_id, name, gm_auth, status, progress, level, "
				"gold, diamond, cur_stage, cur_train, vip_level, vip_exp, stamina, energy, quest ) values( %u, '%s', %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u )",
				dwRoleId, strRoleName.c_str(), byGm, byStatus, dwProgress, dwLevel, dwGold, dwDiamond, dwCurStage, dwCurTrain, dwVipLevel, dwVipExp,
				dwStamina, dwEnergy, dwMainQuestId
		);
	}
}

uint32 CenterDBCtrl::InsertActivity(
		uint32			dwServerId,
		uint8			byTypeId,
		std::string		strParam,
		ByteBuffer&		data,
		uint32			dwGmCommandId )
{
	std::string str;
	str.append( (const char*)data.contents(), data.size() );
	m_db->escape_string( str );
	m_db->escape_string( strParam );

	if ( !m_db->PExecute(
		"insert into active_info(server_id, type_id, param, act_data, gm_cmd_id) values(%u, %u, '%s', '%s', %u)",
		dwServerId, byTypeId, strParam.c_str(), str.c_str(), dwGmCommandId
	) )
	{
		IME_SQLERROR("Error When Insert Activity Info");
		return 0;
	}

	QueryResult* result = m_db->PQuery( "select LAST_INSERT_ID()" );

	if ( result )
	{
		uint32 ret = result->Fetch()[0].GetUInt32();
		delete result;

		return ret;
	}
	else
	{
		IME_SQLERROR("Error When Insert Activity Info");
		return 0;
	}
}

bool CenterDBCtrl::UpdateActivity(
		uint32			dwAutoId,
		ByteBuffer&		data )
{
	std::string str;
	str.append( (const char*)data.contents(), data.size() );
	m_db->escape_string( str );

	if ( !m_db->PExecute(
		"update active_info set act_data='%s' where auto_id=%u", str.c_str(), dwAutoId ) )
	{
		IME_SQLERROR( "Error When Update Activity Info, autoId=%u", dwAutoId );
		return false;
	}
	return true;
}

bool CenterDBCtrl::GetAllActivity(
		std::vector<STC_ACTIVITY_INFO>& vActivity, uint32 dwGameServerId )
{
	QueryResult* result = m_db->PQuery( "select auto_id, type_id, param, act_data, gm_cmd_id from active_info where server_id=%u", dwGameServerId );
	vActivity.clear();

	if ( result )
	{
		uint64 cnt = result->GetRowCount();
		for ( int i = 0; i < cnt; i++ )
		{
			Field* field = result->Fetch();
			STC_ACTIVITY_INFO t;

			t.dwAutoId	= field[0].GetUInt32();
			t.dwServerId= dwGameServerId;
			t.byType	= field[1].GetUInt8();
			t.strParam	= field[2].GetString();

			std::string str;
			str.append( field[3].GetString(), field[3].GetLength() );
			t.data.clear();
			t.data.append( str.data(), str.size() );

			t.dwGmCommandId	= field[4].GetUInt32();

			vActivity.push_back( t );
			result->NextRow();
		}

		delete result;
	}
	return true;
}

bool CenterDBCtrl::DeleteActivity( uint32 dwAutoId )
{
	if ( !m_db->PExecute( "delete from active_info where auto_id=%u", dwAutoId ) )
	{
		IME_SQLERROR("Error When Delete Activity Info");
		return false;
	}
	return true;
}

bool CenterDBCtrl::UpdateRoleLastLoginTime(
			uint32			dwRoleId )
{
	uint32 dwTime = GetDBTime();

	QueryResult* result = m_db->PQuery( "select role_id from player_info where role_id=%u", dwRoleId );
	if ( !result )
	{
		m_db->PExecute( "insert into player_info(role_id, last_login_time) values(%u, %u)", dwRoleId, dwTime );
	}
	else
	{
		m_db->PExecute( "update player_info set last_login_time=%u where role_id=%u", dwTime, dwRoleId );
		delete result;
	}

	result = m_db->PQuery( "select passport_id from re_passport_player where role_id=%u", dwRoleId );
	if ( result )
	{
		uint64 pid = result->Fetch()[0].GetUInt64();
		m_db->PExecute( "update passport_info set last_login_time=%u where passport_id=%llu", dwTime, pid );

		delete result;
		return true;
	}
	return false;
}

bool CenterDBCtrl::UpdatePassportGmAuth(
		uint64			dwPassportId,
		uint8			byGmAuth )
{
	return m_db->PExecute("update passport_info set gm_auth=%u where passport_id=%llu", byGmAuth, dwPassportId );
}

bool CenterDBCtrl::UpdateRoleGmAuth(
		uint32			dwRoleId,
		uint8			byGmAuth )
{
	return m_db->PExecute("update player_info set gm_auth=%u where role_id=%u", byGmAuth, dwRoleId );
}

//bool CenterDBCtrl::InsertPlayerInfo(
//			uint32			dwRoleId,
//			std::string		strRoleName,
//			uint8			byGmAuth,
//			uint8			byStatus
//	)
//{
//	m_db->escape_string( strRoleName );
//
//	return m_db->PExecute(
//		"insert into player_info( role_id, name, gm_auth, status ) values(%u, '%s', %u, %u)",
//		dwRoleId, strRoleName.c_str(), byGmAuth, byStatus
//	);
//}
//
//bool CenterDBCtrl::UpdatePlayerInfo(
//			uint32			dwRoleId,
//			std::string		strRoleName,
//			uint8			byGmAuth,
//			uint8			byStatus
//	)
//{
//	m_db->escape_string( strRoleName );
//
//	return m_db->PExecute(
//		"update player_info set name='%s', gm_auth=%u, status=%u where role_id=%u",
//		strRoleName.c_str(), byGmAuth, byStatus, dwRoleId
//	);
//}

uint32 CenterDBCtrl::InsertLoginInfo(
			uint32 			dwRoleId,
			std::string		strRegIp,
			std::string		strRegDevice,
			std::string		strRegDeviceType )
{
//	if ( dwRoleId & 1 ) return 0;

	m_db->escape_string( strRegIp );
	m_db->escape_string( strRegDevice );
	m_db->escape_string( strRegDeviceType );

	// NOTE: 'InsertLogoutInfo' will not be invoke as expected if game server crash
	m_db->PExecute(
		"insert into role_login_info(role_id, login_time, logout_time, login_ip, login_device, login_device_type) "
		"values(%u, %u, %u, '%s', '%s', '%s')",
		dwRoleId, GetDBTime(), 0, strRegIp.c_str(), strRegDevice.c_str(), strRegDeviceType.c_str()
	);

	QueryResult* result = m_db->PQuery( "select LAST_INSERT_ID()" );

	if ( result )
	{
		uint32 ret = result->Fetch()[0].GetUInt32();
		delete result;

		return ret;
	}
	else
	{
		IME_SQLERROR("Error When Insert Login Info");
		return 0;
	}
}

bool CenterDBCtrl::InsertLogoutInfo( uint32 dwAutoId )
{
	if ( dwAutoId == 0 ) return false;

	return m_db->PExecute(
		"update role_login_info set logout_time=%u where auto_id=%u",
		GetDBTime(), dwAutoId
	);
}

//uint32 CenterDBCtrl::InsertChargeInfo(
//			uint32			dwRoleId,
//			uint32			dwGoodsId,
//			uint32			dwGoodsQuantity,
//			std::string		strCurrency,
//			uint32			dwValue,
//			uint32			dwVirtualValue,
//			uint8			byType,
//			std::string		strInnerOrderId,
//			std::string		strPlatformOrderId,
//			std::string		strPlatformAccountId,
//			uint16			wPlatform,
//			uint8			byPlatformPaymentType,
//			ChargeState		byState,
//			std::string		strPaymentIp,
//			std::string		strPaymentDevice,
//			std::string		strPaymentDeviceType )
//{
//	uint32	dwTime = GetDBTime();
//	uint32	dwPaymentTime = 0, dwDistributeTime = 0;
//	if ( byState == E_CHARGE_STATE_PAYED )
//	{
//		dwPaymentTime = dwTime;
//	}
//	else if ( byState == E_CHARGE_STATE_DISTRIBUTED )
//	{
//		dwPaymentTime = dwDistributeTime = dwTime;
//	}
//
//	m_db->escape_string( strCurrency );
//	m_db->escape_string( strInnerOrderId );
//	m_db->escape_string( strPlatformOrderId );
//	m_db->escape_string( strPlatformAccountId );
//	m_db->escape_string( strPaymentIp );
//	m_db->escape_string( strPaymentDevice );
//	m_db->escape_string( strPaymentDeviceType );
//
//	bool succ = m_db->PExecute(
//		"insert into charge_info(role_id, goods_id, goods_quantity, currency, value, virtual_value, type, inner_order_id, "
//		"platform_order_id, platform_account_id, platform, platform_payment_type, state, payment_time, distribute_time, "
//		"payment_ip, payment_device, payment_device_type) values(%u, %u, %u, '%s', %u, %u, %u, '%s', '%s', '%s', %u, "
//		"%u, %u, %u, %u, '%s', '%s', '%s')", dwRoleId, dwGoodsId, dwGoodsQuantity, strCurrency.c_str(), dwValue, dwVirtualValue,
//		byType, strInnerOrderId.c_str(), strPlatformOrderId.c_str(), strPlatformAccountId.c_str(), wPlatform, byPlatformPaymentType,
//		byState, dwPaymentTime, dwDistributeTime, strPaymentIp.c_str(), strPaymentDevice.c_str(), strPaymentDeviceType.c_str()
//	);
//	if ( succ )
//	{
//		QueryResult* result = m_db->PQuery( "select LAST_INSERT_ID()" );
//
//		if ( result )
//		{
//			uint32 ret = result->Fetch()[0].GetUInt32();
//			delete result;
//
//			return ret;
//		}
//		else
//		{
//			IME_SQLERROR("Error When Get Auto Id Of Charge Info");
//			return 0;
//		}
//	}
//	else
//	{
//		IME_SQLERROR( "Error When Insert Charge Info" );
//		return 0;
//	}
//}

//bool CenterDBCtrl::UpdateChargeState( uint32 dwAutoId, ChargeState byState )
//{
//	QueryResult* result = m_db->PQuery(
//		"select state from charge_info where auto_id=%u", dwAutoId
//	);
//
//	if ( !result ) return false;
//
//	uint8 byOldState = result->Fetch()[0].GetUInt8();
//	delete result;
//
//	if ( byState <= byOldState ) return true;
//	uint32 dwTime = GetDBTime();
//
//	if ( byState == E_CHARGE_STATE_PAYED )
//	{
//		return m_db->PExecute(
//			"update charge_info set payment_time=%u, state=%u where auto_id=%u",
//			dwTime, byState, dwAutoId
//		);
//	}
//	else if ( byState == E_CHARGE_STATE_DISTRIBUTED )
//	{
//		if ( byOldState == E_CHARGE_STATE_UNPAY )
//		{
//			return m_db->PExecute(
//				"update charge_info set payment_time=%u, distribute_time=%u, state=%u where auto_id=%u",
//				dwTime, dwTime, byState, dwAutoId
//			);
//		}
//		else
//		{
//			return m_db->PExecute(
//				"update charge_info set distribute_time=%u, state=%u where auto_id=%u",
//				dwTime, byState, dwAutoId
//			);
//		}
//	}
//	return false;
//}

bool CenterDBCtrl::GetUnhandledCharge(
		std::list<STC_CHARGE_INFO>& vCharges )
{
	QueryResult* result = m_db->PQuery(
		"select auto_id, role_id, goods_id, goods_quantity from charge_info where state=%u",
		E_CHARGE_STATE_PAYED
	);

	vCharges.clear();

	if ( result )
	{
		uint64 cnt = result->GetRowCount();
		for ( int i = 0; i < cnt; i++ )
		{
			Field* field = result->Fetch();

			STC_CHARGE_INFO ci;
			ci.dwAutoId		= field[0].GetUInt32();
			ci.dwRoleId		= field[1].GetUInt32();
			ci.dwGoodsId	= field[2].GetUInt32();
			ci.dwGoodsQuantity = field[3].GetUInt32();

			vCharges.push_back( ci );

			result->NextRow();
		}

		delete result;
	}

	return true;
}

bool CenterDBCtrl::ChargeHandled(
			uint32		dwAutoId,
			uint32		dwDiamondValue,
			std::string strIp,
			std::string strDevice,
			std::string strDeviceType,
	   		std::string strDeviceUid
		)
{
	m_db->escape_string( strIp );
	m_db->escape_string( strDevice );
	m_db->escape_string( strDeviceType );
	m_db->escape_string( strDeviceUid );

	return m_db->PExecute(
		"update charge_info set virtual_value=%u, state=%u, distribute_time=%u, payment_ip='%s',"
		"payment_device='%s', payment_device_type='%s', payment_device_uid='%s' where auto_id=%u", dwDiamondValue, E_CHARGE_STATE_DISTRIBUTED,
		GetDBTime(), strIp.c_str(), strDevice.c_str(), strDeviceType.c_str(), strDeviceUid.c_str(), dwAutoId
	);

	return false;
}

uint32 CenterDBCtrl::InsertPurchaseInfo(
			uint32			dwRoleId,
			uint32			dwGoodsId,
			uint32			dwGoodsQuantity,
			uint32			dwValue,
			uint32			dwTime )
{
	bool succ = m_db->PExecute(
		"insert into purchase_info(role_id, goods_id, goods_quantity, value, time) "
		"values(%u, %u, %u, %u, %u)", dwRoleId, dwGoodsId, dwGoodsQuantity, dwValue, dwTime
	);

	if ( succ )
	{
		QueryResult* result = m_db->PQuery( "select LAST_INSERT_ID()" );
		if ( result )
		{
			uint32 ret = result->Fetch()[0].GetUInt32();
			delete result;

			return ret;
		}
		else
		{
			IME_SQLERROR("Error When Get Auto Id Of Purchase Info");
			return 0;
		}
	}
	else
	{
		IME_SQLERROR( "Error When Insert Purchase Info" );
		return 0;
	}
}

bool CenterDBCtrl::GetNoticeOfServer( uint32 dwServerId, std::string& strNotice )
{
//	m_db->PExecute(
//			"CREATE TABLE IF NOT EXISTS notice_info ("
//				"auto_id			INT		UNSIGNED	NOT NULL AUTO_INCREMENT,"
//				"server_id			INT		UNSIGNED	NOT NULL,"
//				"content	VARCHAR(512)	CHARACTER SET utf8 NOT NULL,"
//				"PRIMARY KEY (auto_id)"
//			") ENGINE=InnoDB DEFAULT CHARSET=utf8;"
//		);

	QueryResult* result = m_db->PQuery(
		"select content from notice_info where server_id=%u", dwServerId
	);

	if ( result )
	{
		strNotice = result->Fetch()[0].GetString();

		delete result;
		return true;
	}
	return false;
}

bool CenterDBCtrl::GetNoticeByType( uint32 dwTypeId, std::vector<std::string> &vecstrNotice )
{
//	m_db->PExecute(
//			"CREATE TABLE IF NOT EXISTS notice_info ("
//				"auto_id			INT		UNSIGNED	NOT NULL AUTO_INCREMENT,"
//				"server_id			INT		UNSIGNED	NOT NULL,"
//				"content	VARCHAR(512)	CHARACTER SET utf8 NOT NULL,"
//				"PRIMARY KEY (auto_id)"
//			") ENGINE=InnoDB DEFAULT CHARSET=utf8;"
//		);

	QueryResult* result = m_db->PQuery(
		"select content from notice_info_v2 where use_type=%u", dwTypeId
	);

	if ( result )
	{
//		strNotice = result->Fetch()[0].GetString();
		int j = 0;
		int number = result->GetRowCount();
		Field *fields = result->Fetch();
		vecstrNotice.clear();
		std::string str;
		for (; j < number; j++)
		{
			str = fields[0].GetCppString();
			vecstrNotice.push_back(str);
			result->NextRow();
			fields = result->Fetch();	
		}

		delete result;
		return true;
	}
	return false;
}


bool CenterDBCtrl::UpdateOrInsertNotice( uint32 dwServerId, std::string strNotice )
{
	m_db->escape_string( strNotice );
	QueryResult* result = m_db->PQuery(
		"select auto_id from notice_info where server_id=%u", dwServerId
	);
	if ( result )
	{
		delete result;
		return m_db->PExecute("update notice_info set content='%s' where server_id=%u", strNotice.c_str(), dwServerId );
	}
	else
	{
		return m_db->PExecute("insert into notice_info( server_id, content ) values( %u, '%s' )", dwServerId, strNotice.c_str() );
	}
}

uint64 CenterDBCtrl::UpdateRoleToken( uint32 dwRoleId, std::string strToken )
{
	m_db->escape_string( strToken );

	QueryResult* result = m_db->PQuery(
		"select passport_id from re_passport_player where role_id=%u", dwRoleId
	);

	if ( result )
	{
		uint64 ret = result->Fetch()[0].GetUInt64();
		delete result;

		m_db->PExecute(
			"update passport_info set token='%s' where passport_id=%llu", strToken.c_str(), ret
		);

		return ret;
	}
	else
	{
		return 0;
	}
}

void CenterDBCtrl::GetGoodsInfoOfGameServer( std::map< uint32, STC_GOODS_INFO>& vGoods, uint32 dwGameServerId )
{
	QueryResult* result = m_db->PQuery(
		"select goods_id, shop_type, buy_type_id, buy_content_id, buy_count, cost_type_id, cost_content_id, cost_count, cost_count_old,"
		"status, limit_day, sort_idx, icon_id, goods_name, description, limit_once from goods_info where server_id=%u", dwGameServerId
	);

	vGoods.clear();
	if ( result )
	{
		uint64 cnt = result->GetRowCount();
		for ( int i = 0; i < cnt; i++ )
		{
			Field* field = result->Fetch();
			STC_GOODS_INFO t;

			t.dwGoodsId			= field[0].GetUInt32();
			t.byShopType		= field[1].GetUInt8();
			t.dwBuyTypeId		= field[2].GetUInt32();
			t.dwBuyContentId	= field[3].GetUInt32();
			t.dwBuyCount		= field[4].GetUInt32();
			t.dwCostTypeId		= field[5].GetUInt32();
			t.dwCostContentId	= field[6].GetUInt32();
			t.dwCostCount		= field[7].GetUInt32();
			t.dwCostCountOld	= field[8].GetUInt32();
			t.byStatus			= (GoodsState)field[9].GetUInt8();
			t.dwLimitDay		= field[10].GetUInt32();
			t.dwSortIdx			= field[11].GetUInt32();
			t.dwIconId			= field[12].GetUInt32();
			t.strName			= field[13].GetString();
			t.strDescription	= field[14].GetString();
			t.dwBuyLimitOnce	= field[15].GetUInt32();

			vGoods.insert( std::make_pair( t.dwGoodsId, t ) );
			result->NextRow();
		}
		delete result;
	}
}

void CenterDBCtrl::UpdateGoodsInfoOfGameServer( std::map<uint32, STC_GOODS_INFO>& vGoods, uint32 dwGameServerId )
{
	m_db->PExecute( "delete from goods_info where server_id=%u", dwGameServerId );
	for ( std::map<uint32, STC_GOODS_INFO>::iterator it = vGoods.begin();
			it != vGoods.end(); ++it )
	{
		m_db->PExecute( "insert into goods_info(server_id, goods_id, shop_type, buy_type_id, "
				"buy_content_id, buy_count, cost_type_id, cost_content_id, cost_count, cost_count_old, "
				"status, limit_day, sort_idx, icon_id, goods_name, description, limit_once) values(%u, %u, "
				"%u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, '%s', '%s', %d)",
				dwGameServerId,
				it->second.dwGoodsId,
				it->second.byShopType,
				it->second.dwBuyTypeId,
				it->second.dwBuyContentId,
				it->second.dwBuyCount,
				it->second.dwCostTypeId,
				it->second.dwCostContentId,
				it->second.dwCostCount,
				it->second.dwCostCountOld,
				it->second.byStatus,
				it->second.dwLimitDay,
				it->second.dwSortIdx,
				it->second.dwIconId,
				it->second.strName.c_str(),
				it->second.strDescription.c_str(),
				it->second.dwBuyLimitOnce );
	}
}

uint8 CenterDBCtrl::GetGmAuth( uint32 dwRoleId )
{
	uint8 byAuthRole = 0, byAuthPassport = 0;
	QueryResult* result = m_db->PQuery(
		"select gm_auth from player_info where role_id=%u", dwRoleId
	);
	if ( result )
	{
		byAuthRole = result->Fetch()[0].GetUInt8();
		delete result;
	}
	else
	{
		IME_SYSTEM_ERROR( "GetGmAuth", "RoleAuth Not Found, roleId=%u", dwRoleId );
	}
	result = m_db->PQuery( "select gm_auth from passport_info where passport_id="
			"(select passport_id from re_passport_player where role_id=%u)", dwRoleId );
	if ( result )
	{
		byAuthPassport = result->Fetch()[0].GetUInt8();
		delete result;
	}
	else
	{
		IME_SYSTEM_ERROR( "GetGmAuth", "PassportAuth Not Found, roleId=%u", dwRoleId );
	}

	return std::max( byAuthRole, byAuthPassport );
}

bool CenterDBCtrl::InitGmCommand()
{
	QueryResult* result = m_db->PQuery(
		"select auto_id, opr, target_type, target_id, start_time, end_time, backup_value, status, params from gm_cmd"
		" where status = %u or status = %u or status = %u",
		E_GM_COMMAND_STATUS_NEW, E_GM_COMMAND_STATUS_PENDING, E_GM_COMMAND_STATUS_RUNNING
	);

	if ( result )
	{
		ReadCommands( result );
		delete result;
	}
	else
	{
		m_dwMaxHandledGmCommandId = 0;
	}
	m_dwGmCommandUpdateTime = GetDBTime();
	return true;
}

bool CenterDBCtrl::ReadNewGmCommand()
{
	QueryResult* result = m_db->PQuery(
		"select auto_id, opr, target_type, target_id, start_time, end_time, backup_value, status, params from gm_cmd"
		" where auto_id > %u and status = %u",
		m_dwMaxHandledGmCommandId, E_GM_COMMAND_STATUS_NEW
	);

	if ( result )
	{
		ReadCommands( result );
		delete result;
	}
	return true;
}

void CenterDBCtrl::UpdateErrorstr(GmCommand *cmd)
{
	IME_SYSTEM_LOG( "GmCommand", "GM Command Error, id=%d, opr=%s, msg=%s",
			cmd->GetAutoId(), cmd->GetOpr().c_str(), cmd->GetErrorMsg().c_str() );
	if ( !m_db->PExecute("update gm_cmd set error_msg='%s' where auto_id=%u",
				cmd->GetErrorMsg().c_str(), cmd->GetAutoId() ) )
	{
		IME_SQLERROR( "Error Update ErrorMsg in 'gm_cmd', id=%u", cmd->GetAutoId() );
	}
}

bool CenterDBCtrl::UpdateGmCommand()
{
	uint32 dwTime = GetDBTime();
	if (dwTime <= m_dwGmCommandUpdateTime)
	{
		return true;
	}

	std::vector<GmCommand*> vRemoveCmd;

	for ( GmCommandMapType::iterator it = m_mapGmCommandAll.begin();
			it != m_mapGmCommandAll.end(); it++ )
	{
		GmCommand* cmd = it->second;

		do
		{
			if ( cmd->GetStatus() == E_GM_COMMAND_STATUS_NEW )
			{
				IME_SYSTEM_LOG( "GmCommand", "Valid GM Command, id=%d, opr=%s", cmd->GetAutoId(), cmd->GetOpr().c_str() );
				if ( !cmd->Validate() )
				{
					cmd->SetStatus( E_GM_COMMAND_STATUS_ERROR );
					break;
				}
				IME_SYSTEM_LOG( "GmCommand", "GM Command Pending, id=%d, opr=%s", cmd->GetAutoId(), cmd->GetOpr().c_str() );
				cmd->SetStatus( E_GM_COMMAND_STATUS_PENDING );
			}

			if ( cmd->GetStatus() == E_GM_COMMAND_STATUS_PENDING )
			{
				if ( cmd->GetStartTime() == 0 || cmd->GetStartTime() < dwTime )
				{
					IME_SYSTEM_LOG( "GmCommand", "Trigger GM Command, id=%d, opr=%s", cmd->GetAutoId(), cmd->GetOpr().c_str() );
					cmd->SetStartTime( dwTime ); //
					if ( !cmd->Trigger() )
					{
						cmd->SetStatus( E_GM_COMMAND_STATUS_ERROR );
						break;
					}
					IME_SYSTEM_LOG( "GmCommand", "GM Command Running, id=%d, opr=%s", cmd->GetAutoId(), cmd->GetOpr().c_str() );
					cmd->SetStatus( E_GM_COMMAND_STATUS_RUNNING );
				}
			}

			if ( cmd->GetStatus() == E_GM_COMMAND_STATUS_RUNNING )
			{
				cmd->Tick();
				if ( cmd->GetEndTime() == 0 || cmd->GetEndTime() < dwTime )
				{
					IME_SYSTEM_LOG( "GmCommand", "Finish GM Command, id=%d, opr=%s", cmd->GetAutoId(), cmd->GetOpr().c_str() );
					if ( !cmd->Complete() )
					{
						cmd->SetStatus( E_GM_COMMAND_STATUS_ERROR );
						break;
					}
					cmd->SetStatus( E_GM_COMMAND_STATUS_COMPLETE );
					IME_SYSTEM_LOG( "GmCommand", "GM Command Complete, id=%d, opr=%s", cmd->GetAutoId(), cmd->GetOpr().c_str() );
				}
			}

		} while(0);
	}

	for ( GmCommandMapType::iterator it = m_mapGmCommandAll.begin();
			it != m_mapGmCommandAll.end(); it++ )
	{
		GmCommand* cmd = it->second;

		if ( cmd->Dirty() )
		{
			if ( !m_db->PExecute(
				"update gm_cmd set opr='%s', target_type=%u, target_id=%u, start_time=%u, end_time=%u, "
				"backup_value='%s', status=%u where auto_id=%u",
				cmd->GetOpr().c_str(), cmd->GetTargetType(), cmd->GetTargetId(), cmd->GetStartTime(), cmd->GetEndTime(),
				cmd->GetBackup().c_str(),  cmd->GetStatus(), cmd->GetAutoId()
			) )
			{
				IME_SQLERROR( "Error Update GmCommand in 'gm_cmd', id=%u", cmd->GetAutoId() );
			}

			if ( cmd->GetStatus() == E_GM_COMMAND_STATUS_ERROR )
			{
				IME_SYSTEM_LOG( "GmCommand", "GM Command Error, id=%d, opr=%s, msg=%s",
						cmd->GetAutoId(), cmd->GetOpr().c_str(), cmd->GetErrorMsg().c_str() );
				if ( !m_db->PExecute("update gm_cmd set error_msg='%s' where auto_id=%u",
						cmd->GetErrorMsg().c_str(), cmd->GetAutoId() ) )
				{
					IME_SQLERROR( "Error Update ErrorMsg in 'gm_cmd', id=%u", cmd->GetAutoId() );
				}
			}
		}

		cmd->Reset();

		if ( cmd->GetStatus() == E_GM_COMMAND_STATUS_COMPLETE || cmd->GetStatus() == E_GM_COMMAND_STATUS_ERROR
				|| cmd->GetStatus() == E_GM_COMMAND_STATUS_CANCELED )
		{
			vRemoveCmd.push_back( cmd );
		}
	}
	m_dwGmCommandUpdateTime = dwTime;

	for ( std::vector<GmCommand*>::iterator it = vRemoveCmd.begin();
			it != vRemoveCmd.end(); ++it )
	{
		RemoveGmCommand( *it );
	}
	return true;
}

bool CenterDBCtrl::HasGmCommand( uint32 dwGmCommandId )
{
	for ( GmCommandMapType::iterator it = m_mapGmCommandAll.begin();
				it != m_mapGmCommandAll.end(); it++ )
	{
		GmCommand* cmd = it->second;
		if ( cmd->GetAutoId() == dwGmCommandId ) return true;
	}
	return false;
}

bool CenterDBCtrl::CancelGmCommand( uint32 dwGmCommandId )
{
	for ( GmCommandMapType::iterator it = m_mapGmCommandAll.begin();
				it != m_mapGmCommandAll.end(); it++ )
	{
		GmCommand* cmd = it->second;
		if ( cmd->GetAutoId() == dwGmCommandId )
		{
			return cmd->Cancel();
		}
	}
	return false;
}

uint32 CenterDBCtrl::HandlerGmCommandRole( void* pRole, uint32 dwLastCmdTime )
{
	for ( GmCommandMapType::iterator it = m_mapGmCommandAll.begin();
			it != m_mapGmCommandAll.end(); ++it )
	{
		if ( it->second->GetStatus() == E_GM_COMMAND_STATUS_RUNNING && dwLastCmdTime < it->second->GetStartTime() )
		{
			it->second->HandleRole( pRole );
		}
	}
	return CenterDBCtrl::GetGmCommandUpdateTime();
}

void CenterDBCtrl::ReadCommands( QueryResult* result )
{
	uint64 cnt = result->GetRowCount();
	for ( int i = 0; i < cnt; i++ )
	{
		Field* field = result->Fetch();

		uint32 		dwAutoId	= field[0].GetUInt32();
		std::string strOpr	 	= field[1].GetString();
		uint8		byTargetType= field[2].GetUInt8();
		uint64		dwTargetId	= field[3].GetUInt64();
		uint32		dwStartTime = field[4].GetUInt32();
		uint32		dwEndTime	= field[5].GetUInt32();
		std::string strBackup	= field[6].GetString();
		uint8		byStatus	= field[7].GetUInt8();
		std::string strParams	= field[8].GetString();

		m_dwMaxHandledGmCommandId = std::max( m_dwMaxHandledGmCommandId, dwAutoId );

		// check if this game server should handle this command
		if ( GmCommandFactory::CheckTargetType( byTargetType, dwTargetId ) )
		{
			GmCommand* cmd = GmCommandFactory::Create( dwAutoId, strOpr, (GmCommandTargetType)byTargetType,
					dwTargetId, dwStartTime, dwEndTime, strBackup, (GmCommandStatus)byStatus, strParams );

			if ( cmd == NULL )
			{
				IME_SYSTEM_ERROR( "GmCommand", "Operation Not Support, opr=%s", strOpr.c_str() );

				m_db->PExecute( "update gm_cmd set status=%u, error_msg='%s' where auto_id=%u",
						E_GM_COMMAND_STATUS_ERROR, "Operation Not Support", dwAutoId );
			}
			else
			{
				AppendGmCommand( cmd );
			}
		}

		result->NextRow();
	}
}

bool CenterDBCtrl::AppendGmCommand( GmCommand* pCommand )
{
	IME_SYSTEM_LOG( "GmCommand", "Append GM Command, id=%d, opr=%s", pCommand->GetAutoId(), pCommand->GetOpr().c_str() );
	m_mapGmCommandAll.insert( std::make_pair( pCommand->GetAutoId(), pCommand ) );
	return true;
}

bool CenterDBCtrl::RemoveGmCommand( GmCommand* pCommand )
{
	IME_SYSTEM_LOG( "GmCommand", "Remove GM Command, id=%d, opr=%s", pCommand->GetAutoId(), pCommand->GetOpr().c_str() );
	m_mapGmCommandAll.erase( pCommand->GetAutoId() );
	delete pCommand;
	return true;
}
