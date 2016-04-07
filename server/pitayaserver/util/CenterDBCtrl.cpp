#include "CenterDBCtrl.h"
#include "Log.h"
#include "QueryResult.h"
#include <vector>
#include <string>
#include "Util.h"

namespace CenterDB
{
DatabaseMysql* 	CenterDBCtrl::m_db = NULL;
uint8			CenterDBCtrl::m_byLoginServerId = 0;
uint32			CenterDBCtrl::m_maxPassportAutoInc = 0;
uint32			CenterDBCtrl::m_maxRoleAutoInc = 0;
bool			CenterDBCtrl::m_bReadOnly = false;
uint32			CenterDBCtrl::m_dwMaxHandledGmCommandId = 0;
uint32			CenterDBCtrl::m_dwMaxRunningGmCommandTime = 0;

std::map<uint32, GmCommand*> CenterDBCtrl::m_mapGmCommandAll;

bool CenterDBCtrl::ExistsIndex( const char* table, const char* index )
{
	QueryResult* result = m_db->PQuery(
		"select * from information_schema.statistics where table_name='%s' and index_name='%s'",
		table, index
	);

	if ( result )
	{
		delete result;
		return true;
	}
	return false;
}

bool CenterDBCtrl::ExistsColumn( const char* table, const char* column )
{
	QueryResult* result = m_db->PQuery(
		"select * from information_schema.columns where table_name='%s' and column_name='%s' and table_schema='%s'",
		table, column, m_db->GetDbName() );
	if ( result )
	{
		delete result;
		return true;
	}
	return false;
}

bool CenterDBCtrl::ExistsTable( const char* table )
{
	QueryResult* result = m_db->PQuery(
		"select * from information_schema.tables where table_schema='%s' and table_name='%s'",
		m_db->GetDbName(), table
	);
	if ( result )
	{
		delete result;
		return true;
	}
	return false;
}

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

	// gameserver_info
	m_db->PExecute(
		"CREATE TABLE IF NOT EXISTS gameserver_info ("
			"server_id		SMALLINT	UNSIGNED	NOT NULL,"
			"server_name	VARCHAR(32)	CHARACTER SET utf8 NOT NULL,"
			"ip				VARCHAR(64)	CHARACTER SET utf8 NOT NULL,"
			"local_ip		VARCHAR(64)	CHARACTER SET utf8 NOT NULL,"
			"port			SMALLINT	UNSIGNED	NOT NULL,"
			"version		VARCHAR(20)	CHARACTER SET utf8 NOT NULL,"
			"res_version	VARCHAR(20)	CHARACTER SET utf8 NOT NULL,"
			"res_version_config VARCHAR(20) CHARACTER SET utf8 NOT NULL,"
			"res_server_ip	VARCHAR(64) CHARACTER SET utf8 NOT NULL,"
			"online_num		MEDIUMINT	UNSIGNED	NOT NULL,"
			"can_login		TINYINT		UNSIGNED	NOT NULL,"
			"status			TINYINT		UNSIGNED	NOT NULL,"
			"update_time	INT			UNSIGNED	NOT NULL,"
			"login_strategy_id	INT		UNSIGNED	NOT NULL,"
			"is_test		TINYINT		UNSIGNED	NOT NULL DEFAULT '0',"
			"version_code	INT			UNSIGNED	NOT NULL DEFAULT '0',"
			"PRIMARY KEY (server_id)"
		") ENGINE=InnoDB DEFAULT CHARSET=utf8;"
	);

	if ( !ExistsColumn( "gameserver_info", "res_server_ip" ) )
		m_db->PExecute( "ALTER TABLE gameserver_info ADD COLUMN res_server_ip VARCHAR(64)	CHARACTER SET utf8 NOT NULL" );
	if ( !ExistsColumn( "gameserver_info", "res_version_config" ) )
		m_db->PExecute( "ALTER TABLE gameserver_info ADD COLUMN res_version_config VARCHAR(20)	CHARACTER SET utf8 NOT NULL" );
	if ( !ExistsColumn( "gameserver_info", "is_test" ) )
		m_db->PExecute( "ALTER TABLE gameserver_info ADD COLUMN is_test TINYINT		UNSIGNED	NOT NULL DEFAULT '0'" );
	if ( !ExistsColumn( "gameserver_info", "version_code" ) )
		m_db->PExecute( "ALTER TABLE gameserver_info ADD COLUMN version_code INT	UNSIGNED	NOT NULL DEFAULT '0'" );
	if ( !ExistsColumn( "gameserver_info", "can_register" ) )
		m_db->PExecute( "ALTER TABLE gameserver_info ADD COLUMN can_register INT	UNSIGNED	NOT NULL DEFAULT '1'" );

	// player_info
	m_db->PExecute(
		"CREATE TABLE IF NOT EXISTS player_info ("
			"role_id		INT			UNSIGNED	NOT NULL,"
			"name			VARCHAR(32)	CHARACTER SET utf8 NOT NULL,"
			"gm_auth		TINYINT		UNSIGNED	NOT NULL,"
			"status			TINYINT		UNSIGNED	NOT NULL,"
			"progress		INT			UNSIGNED	NOT NULL,"
			"level			INT			UNSIGNED	NOT NULL,"
			"gold			INT			UNSIGNED	NOT NULL,"
			"diamond		INT			UNSIGNED	NOT NULL,"
			"cur_stage		INT			UNSIGNED	NOT NULL,"
			"cur_train		INT			UNSIGNED	NOT NULL,"
			"vip_level		INT			UNSIGNED	NOT NULL,"
			"vip_exp		INT			UNSIGNED	NOT NULL,"
			"stamina		INT			UNSIGNED	NOT NULL,"
			"energy			INT			UNSIGNED	NOT NULL,"
			"quest			INT			UNSIGNED	NOT NULL,"
			"last_login_time	INT		UNSIGNED	NOT NULL,"
			"diamond_pay	INT			UNSIGNED	NOT NULL,"
			"PRIMARY KEY (role_id)"
		") ENGINE=InnoDB DEFAULT CHARSET=utf8;"
	);

	if ( !ExistsColumn( "player_info", "progress" ) )
		m_db->PExecute( "ALTER TABLE player_info ADD COLUMN progress INT UNSIGNED NOT NULL" );
	if ( !ExistsColumn( "player_info", "level" ) )
		m_db->PExecute( "ALTER TABLE player_info ADD COLUMN level INT UNSIGNED NOT NULL" );
	if ( !ExistsColumn( "player_info", "gold" ) )
		m_db->PExecute( "ALTER TABLE player_info ADD COLUMN gold INT UNSIGNED NOT NULL" );
	if ( !ExistsColumn( "player_info", "diamond" ) )
		m_db->PExecute( "ALTER TABLE player_info ADD COLUMN diamond INT UNSIGNED NOT NULL" );
	if ( !ExistsColumn( "player_info", "cur_stage" ) )
		m_db->PExecute( "ALTER TABLE player_info ADD COLUMN cur_stage INT UNSIGNED NOT NULL" );
	if ( !ExistsColumn( "player_info", "cur_train" ) )
		m_db->PExecute( "ALTER TABLE player_info ADD COLUMN cur_train INT UNSIGNED NOT NULL" );
	if ( !ExistsColumn( "player_info", "vip_level" ) )
		m_db->PExecute( "ALTER TABLE player_info ADD COLUMN vip_level INT UNSIGNED NOT NULL" );
	if ( !ExistsColumn( "player_info", "vip_exp" ) )
		m_db->PExecute( "ALTER TABLE player_info ADD COLUMN vip_exp INT UNSIGNED NOT NULL" );
	if ( !ExistsColumn( "player_info", "last_login_time" ) )
		m_db->PExecute( "ALTER TABLE player_info ADD COLUMN last_login_time INT UNSIGNED NOT NULL" );
	if ( !ExistsColumn( "player_info", "stamina" ) )
		m_db->PExecute( "ALTER TABLE player_info ADD COLUMN stamina INT UNSIGNED NOT NULL" );
	if ( !ExistsColumn( "player_info", "energy" ) )
		m_db->PExecute( "ALTER TABLE player_info ADD COLUMN energy INT UNSIGNED NOT NULL" );
	if ( !ExistsColumn( "player_info", "quest" ) )
		m_db->PExecute( "ALTER TABLE player_info ADD COLUMN quest INT UNSIGNED NOT NULL" );
	if ( !ExistsColumn( "player_info", "diamond_pay" ) )
		m_db->PExecute( "ALTER TABLE player_info ADD COLUMN diamond_pay INT UNSIGNED NOT NULL" );

	// passport_info
	m_db->PExecute(
		"CREATE TABLE IF NOT EXISTS passport_info ("
			"passport_id	BIGINT		UNSIGNED	NOT NULL,"
			"passport		VARCHAR(128)	CHARACTER SET utf8 NOT NULL,"
			"pwd			VARCHAR(64) CHARACTER SET utf8 NOT NULL,"
			"mail			VARCHAR(64) CHARACTER SET utf8 NOT NULL,"
			"uid			VARCHAR(128) CHARACTER SET utf8 NOT NULL,"
			"token			VARCHAR(128) CHARACTER SET utf8 NOT NULL,"
			"platform		MEDIUMINT	UNSIGNED	NOT NULL,"
			"auth_type		TINYINT		UNSIGNED	NOT NULL,"
			"create_time	INT			UNSIGNED	NOT NULL,"
			"gm_auth		TINYINT		UNSIGNED	NOT NULL,"
			"reg_ip			VARCHAR(64) CHARACTER SET utf8 NOT NULL,"
			"reg_device		VARCHAR(32)	CHARACTER SET utf8 NOT NULL,"
			"reg_device_type VARCHAR(64) CHARACTER SET utf8 NOT NULL,"
			"last_login_time	INT		UNSIGNED	NOT NULL,"
			"last_login_server	SMALLINT	UNSIGNED	NOT NULL DEFAULT '0',"
			"open_udid		VARCHAR(128)	CHARACTER SET utf8 NOT NULL DEFAULT '',"
			"apple_udid		VARCHAR(128)	CHARACTER SET utf8 NOT NULL DEFAULT '',"
			"token_ip		VARCHAR(32)	CHARACTER SET utf8 NOT NULL,"
			"token_time		INT	UNSIGNED	NOT NULL,"
			"PRIMARY KEY (passport_id)"
		") ENGINE=InnoDB DEFAULT CHARSET=utf8;"
	);

	if ( !ExistsIndex( "passport_info", "index_passport" ) )
		m_db->PExecute( "CREATE INDEX index_passport ON passport_info(passport)" );
	if ( !ExistsColumn( "passport_info", "last_login_time" ) )
		m_db->PExecute( "ALTER TABLE passport_info ADD COLUMN last_login_time INT UNSIGNED NOT NULL" );
	if ( !ExistsColumn( "passport_info", "last_login_server" ) )
		m_db->PExecute( "ALTER TABLE passport_info ADD COLUMN last_login_server SMALLINT	UNSIGNED	NOT NULL DEFAULT '0'" );
	m_db->PExecute( "ALTER TABLE passport_info CHANGE platform platform MEDIUMINT	UNSIGNED	NOT NULL" );

	if ( !ExistsColumn( "passport_info", "open_udid" ) )
		m_db->PExecute( "ALTER TABLE passport_info ADD COLUMN open_udid VARCHAR(128)	CHARACTER SET utf8 NOT NULL DEFAULT ''" );
	if ( !ExistsColumn( "passport_info", "apple_udid" ) )
		m_db->PExecute( "ALTER TABLE passport_info ADD COLUMN apple_udid VARCHAR(128)	CHARACTER SET utf8 NOT NULL DEFAULT ''" );
	if ( !ExistsIndex( "passport_info", "index_open_udid" ) )
		m_db->PExecute( "CREATE INDEX index_open_udid ON passport_info(open_udid)" );
	if ( !ExistsIndex( "passport_info", "index_apple_udid" ) )
		m_db->PExecute( "CREATE INDEX index_apple_udid ON passport_info(apple_udid)" );

	if ( !ExistsColumn( "passport_info", "token_ip" ) )
		m_db->PExecute( "ALTER TABLE passport_info ADD COLUMN token_ip VARCHAR(32) CHARACTER SET utf8 NOT NULL DEFAULT ''" );
	if ( !ExistsColumn( "passport_info", "token_time" ) )
		m_db->PExecute( "ALTER TABLE passport_info ADD COLUMN token_time INT UNSIGNED NOT NULL" );

	// login_strategy
	m_db->PExecute(
		"CREATE TABLE IF NOT EXISTS login_strategy ("
			"auto_id		INT		UNSIGNED	NOT NULL	AUTO_INCREMENT,"
			"strategy_id	INT		UNSIGNED	NOT NULL,"
			"condition_id	TINYINT	UNSIGNED	NOT NULL,"
			"type			TINYINT	UNSIGNED	NOT NULL,"
			"value			VARCHAR(64) CHARACTER SET utf8 NOT NULL,"
			"PRIMARY KEY(auto_id)"
		") ENGINE=InnoDB DEFAULT CHARSET=utf8;"
	);
	if ( !ExistsColumn( "login_strategy", "is_not" ) )
		m_db->PExecute( "ALTER TABLE login_strategy ADD COLUMN is_not TINYINT UNSIGNED	NOT NULL DEFAULT '0'" );

	// re_passport_player
	m_db->PExecute(
		"CREATE TABLE IF NOT EXISTS re_passport_player ("
			"role_id		INT			UNSIGNED	NOT NULL,"
			"passport_id	BIGINT		UNSIGNED	NOT NULL,"
			"server_id		SMALLINT	UNSIGNED	NOT NULL,"
			"server_id_origin SMALLINT	UNSIGNED	NOT NULL,"
			"create_time	INT			UNSIGNED	NOT NULL,"
			"PRIMARY KEY (role_id)"
		") ENGINE=InnoDB DEFAULT CHARSET=utf8;"
	);

	if ( !ExistsColumn( "re_passport_player", "create_time" ) )
		m_db->PExecute( "ALTER TABLE re_passport_player ADD COLUMN create_time INT UNSIGNED NOT NULL" );
//	if ( !ExistsIndex( "re_passport_player", "index_passport" ) )
//		m_db->PExecute( "CREATE INDEX index_passport ON re_passport_player(passport_id, server_id)" );

	// for merge
	if ( !ExistsColumn( "re_passport_player", "server_id_origin" ) )
	{
		m_db->PExecute( "ALTER TABLE re_passport_player ADD COLUMN server_id_origin SMALLINT UNSIGNED NOT NULL" );
		m_db->PExecute( "UPDATE re_passport_player set server_id_origin = server_id" );
	}
	if ( !ExistsIndex( "re_passport_player", "index_passport2" ) )
		m_db->PExecute( "CREATE INDEX index_passport2 ON re_passport_player(passport_id, server_id_origin)" );

	// role_login_info
	m_db->PExecute(
		"CREATE TABLE IF NOT EXISTS role_login_info ("
			"auto_id		INT			UNSIGNED	NOT NULL AUTO_INCREMENT,"
			"role_id		INT			UNSIGNED	NOT NULL,"
			"login_time		INT			UNSIGNED	NOT NULL,"
			"logout_time	INT			UNSIGNED	NOT NULL,"
			"login_ip				VARCHAR(64) CHARACTER SET utf8 NOT NULL,"
			"login_device			VARCHAR(32)	CHARACTER SET utf8 NOT NULL,"
			"login_device_type 		VARCHAR(64) CHARACTER SET utf8 NOT NULL,"
			"PRIMARY KEY (auto_id)"
		") ENGINE=InnoDB DEFAULT CHARSET=utf8;"
	);

	// goods_info
	m_db->PExecute(
		"CREATE TABLE IF NOT EXISTS goods_info ("
			"server_id		SMALLINT	UNSIGNED	NOT NULL,"
			"goods_id			INT		UNSIGNED	NOT NULL,"
			"shop_type			INT		UNSIGNED	NOT NULL,"
			"buy_type_id		INT		UNSIGNED	NOT NULL,"
			"buy_content_id		INT		UNSIGNED	NOT NULL,"
			"buy_count			INT		UNSIGNED	NOT NULL,"
			"cost_type_id		INT		UNSIGNED	NOT NULL,"
			"cost_content_id	INT		UNSIGNED	NOT NULL,"
			"cost_count			INT		UNSIGNED	NOT NULL,"
			"cost_count_old		INT		UNSIGNED	NOT NULL,"
			"status			TINYINT		UNSIGNED	NOT NULL,"
			"limit_day			INT		UNSIGNED	NOT NULL,"
			"sort_idx			INT		UNSIGNED	NOT NULL,"
			"icon_id			INT		UNSIGNED	NOT NULL,"
			"goods_name			VARCHAR(64)	CHARACTER SET utf8 NOT NULL,"
			"description	VARCHAR(256) CHARACTER SET utf8 NOT NULL,"
			"limit_once			INT		UNSIGNED	NOT NULL,"
			"limit_time_start	INT		UNSIGNED	NOT NULL,"
			"limit_time_end		INT		UNSIGNED	NOT NULL,"
			"vip_show		SMALLINT	UNSIGNED	NOT NULL,"
			"vip_buy		SMALLINT	UNSIGNED	NOT NULL,"
			"buy_count_total	INT		UNSIGNED	NOT NULL,"
			"buy_count_inc		INT		UNSIGNED	NOT NULL,"
			"platform_goods_id	VARCHAR(64)	CHARACTER SET utf8 NOT NULL,"
			"platform_type	TINYINT		UNSIGNED	NOT NULL,"
			"weight				INT		UNSIGNED	NOT NULL,"
			"diamond_pay		INT		UNSIGNED	NOT NULL,"
			"limit_number	VARCHAR(64)	CHARACTER SET utf8 NOT NULL,"
			"limit_type		INT		UNSIGNED	NOT NULL,"
			"prize_float		INT		UNSIGNED	NOT NULL,"
			"is_double		INT		UNSIGNED	NOT NULL,"
			"buy_ex VARCHAR(2560)	CHARACTER SET utf8 NOT NULL,"
			"ios_goods_id	VARCHAR(64)	CHARACTER SET utf8 NOT NULL,"
			"android_goods_id	VARCHAR(64)	CHARACTER SET utf8 NOT NULL,"
			"currency		VARCHAR(64)	CHARACTER SET utf8 NOT NULL,"
			"good_gift		INT		UNSIGNED	NOT NULL,"
			"item_icon		VARCHAR(64)	CHARACTER SET utf8 NOT NULL,"
			"collect_icon		VARCHAR(64)	CHARACTER SET utf8 NOT NULL,"
			"PRIMARY KEY (server_id, goods_id)"
		") ENGINE=InnoDB DEFAULT CHARSET=utf8;"
	);
	if ( !ExistsColumn( "goods_info", "cost_count_old" ) )
		m_db->PExecute( "ALTER TABLE goods_info ADD COLUMN cost_count_old INT UNSIGNED NOT NULL" );
	if ( !ExistsColumn( "goods_info", "limit_once" ) )
		m_db->PExecute( "ALTER TABLE goods_info ADD COLUMN limit_once INT UNSIGNED NOT NULL" );
	if ( !ExistsColumn( "goods_info", "limit_time_start" ) )
		m_db->PExecute( "ALTER TABLE goods_info ADD COLUMN limit_time_start INT UNSIGNED NOT NULL" );
	if ( !ExistsColumn( "goods_info", "limit_time_end" ) )
		m_db->PExecute( "ALTER TABLE goods_info ADD COLUMN limit_time_end INT UNSIGNED NOT NULL" );

	if ( !ExistsColumn( "goods_info", "vip_show" ) )
		m_db->PExecute( "ALTER TABLE goods_info ADD COLUMN vip_show		SMALLINT	UNSIGNED	NOT NULL" );
	if ( !ExistsColumn( "goods_info", "vip_buy" ) )
		m_db->PExecute( "ALTER TABLE goods_info ADD COLUMN vip_buy		SMALLINT	UNSIGNED	NOT NULL" );
	if ( !ExistsColumn( "goods_info", "buy_count_total" ) )
		m_db->PExecute( "ALTER TABLE goods_info ADD COLUMN buy_count_total INT UNSIGNED NOT NULL" );
	if ( !ExistsColumn( "goods_info", "buy_count_inc" ) )
		m_db->PExecute( "ALTER TABLE goods_info ADD COLUMN buy_count_inc INT UNSIGNED NOT NULL" );
	if ( !ExistsColumn( "goods_info", "platform_goods_id" ) )
		m_db->PExecute( "ALTER TABLE goods_info ADD COLUMN platform_goods_id VARCHAR(64)	CHARACTER SET utf8 NOT NULL" );
	if ( !ExistsColumn( "goods_info", "platform_type" ) )
		m_db->PExecute( "ALTER TABLE goods_info ADD COLUMN platform_type TINYINT UNSIGNED	NOT NULL" );
	if ( !ExistsColumn( "goods_info", "weight" ) )
		m_db->PExecute( "ALTER TABLE goods_info ADD COLUMN weight INT UNSIGNED	NOT NULL" );
	if ( !ExistsColumn( "goods_info", "diamond_pay" ) )
		m_db->PExecute( "ALTER TABLE goods_info ADD COLUMN diamond_pay INT		UNSIGNED	NOT NULL" );
	if ( !ExistsColumn( "goods_info", "limit_number" ) )
		m_db->PExecute( "ALTER TABLE goods_info ADD COLUMN limit_number varchar(4096) CHARACTER SET utf8 NOT NULL" );
	if ( !ExistsColumn( "goods_info", "limit_type" ) )
		m_db->PExecute( "ALTER TABLE goods_info ADD COLUMN limit_type  INT		UNSIGNED	NOT NULL" );
	if ( !ExistsColumn( "goods_info", "prize_float" ) )
		m_db->PExecute( "ALTER TABLE goods_info ADD COLUMN prize_float VARCHAR(256) CHARACTER SET utf8 NOT NULL" );
	if ( !ExistsColumn( "goods_info", "is_double" ) )
		m_db->PExecute( "ALTER TABLE goods_info ADD COLUMN is_double TINYINT UNSIGNED	NOT NULL" );
	if ( !ExistsColumn( "goods_info", "buy_ex" ) )
		m_db->PExecute( "ALTER TABLE goods_info ADD COLUMN buy_ex VARCHAR(2560)	CHARACTER SET utf8 NOT NULL" );

	if ( !ExistsColumn( "goods_info", "ios_goods_id" ) )
		m_db->PExecute( "ALTER TABLE goods_info ADD COLUMN ios_goods_id	VARCHAR(64)	CHARACTER SET utf8 NOT NULL" );
	if ( !ExistsColumn( "goods_info", "android_goods_id" ) )
		m_db->PExecute( "ALTER TABLE goods_info ADD COLUMN android_goods_id VARCHAR(64)	CHARACTER SET utf8 NOT NULL" );
	if ( !ExistsColumn( "goods_info", "currency" ) )
		m_db->PExecute( "ALTER TABLE goods_info ADD COLUMN currency		VARCHAR(64)	CHARACTER SET utf8 NOT NULL" );
	if ( !ExistsColumn( "goods_info", "good_gift" ) )
		m_db->PExecute( "ALTER TABLE goods_info ADD COLUMN good_gift		INT		UNSIGNED	NOT NULL" );
	if ( !ExistsColumn( "goods_info", "item_icon" ) )
		m_db->PExecute( "ALTER TABLE goods_info ADD COLUMN item_icon		VARCHAR(64)	CHARACTER SET utf8 NOT NULL" );
	if ( !ExistsColumn( "goods_info", "collect_icon" ) )
		m_db->PExecute( "ALTER TABLE goods_info ADD COLUMN collect_icon		VARCHAR(64)	CHARACTER SET utf8 NOT NULL" );

	// notice_info
	// Depreciated
	m_db->PExecute(
		"CREATE TABLE IF NOT EXISTS notice_info ("
			"auto_id			INT		UNSIGNED	NOT NULL AUTO_INCREMENT,"
			"server_id			INT		UNSIGNED	NOT NULL,"
			"content	VARCHAR(512)	CHARACTER SET utf8 NOT NULL,"
			"PRIMARY KEY (auto_id)"
		") ENGINE=InnoDB DEFAULT CHARSET=utf8;"
	);

	// notice_info_v2
	m_db->PExecute(
		"CREATE TABLE IF NOT EXISTS notice_info_v2 ("
			"auto_id			INT		UNSIGNED	NOT NULL AUTO_INCREMENT,"
			"use_type			TINYINT		UNSIGNED	NOT NULL,"
			"condition_type		TINYINT		UNSIGNED	NOT NULL,"
			"condition_value	MEDIUMINT	UNSIGNED	NOT NULL,"
			"start_time			INT		UNSIGNED	NOT NULL,"
			"end_time			INT		UNSIGNED	NOT NULL,"
			"content			VARCHAR(1024) CHARACTER SET utf8 NOT NULL,"
			"PRIMARY KEY (auto_id)"
		") ENGINE=InnoDB DEFAULT CHARSET=utf8;"
	);

	if ( !ExistsColumn( "notice_info_v2", "start_time" ) )
		m_db->PExecute( "ALTER TABLE notice_info_v2 ADD COLUMN start_time INT UNSIGNED NOT NULL" );

	if ( !ExistsColumn( "notice_info_v2", "end_time" ) )
		m_db->PExecute( "ALTER TABLE notice_info_v2 ADD COLUMN end_time	INT	UNSIGNED NOT NULL" );

	// active_info
	m_db->PExecute(
		"CREATE TABLE IF NOT EXISTS active_info ("
			"auto_id		INT		UNSIGNED	NOT NULL AUTO_INCREMENT,"
			"server_id		INT		UNSIGNED	NOT NULL,"
			"type_id	TINYINT		UNSIGNED	NOT NULL,"
			"param		VARCHAR(256) CHARACTER SET utf8 NOT NULL,"
			"act_data		longblob,"
			"gm_cmd_id		INT		UNSIGNED	NOT NULL,"
			"PRIMARY KEY (auto_id)"
		") ENGINE=InnoDB DEFAULT CHARSET=utf8;"
	);

	// charge_info
	m_db->PExecute(
		"CREATE TABLE IF NOT EXISTS charge_info ("
			"auto_id			INT		UNSIGNED	NOT NULL AUTO_INCREMENT,"
			"role_id			INT		UNSIGNED	NOT NULL,"
			"goods_id			INT		UNSIGNED	NOT NULL,"
			"goods_quantity		INT		UNSIGNED	NOT NULL,"
			"currency			VARCHAR(16) CHARACTER SET utf8 NOT NULL,"
			"value				INT		UNSIGNED	NOT NULL,"
			"virtual_value		INT		UNSIGNED	NOT NULL DEFAULT '0',"
			"type				INT		UNSIGNED	NOT NULL,"
			"inner_order_id			VARCHAR(128) CHARACTER SET utf8 NOT NULL,"
			"platform			MEDIUMINT	UNSIGNED	NOT NULL,"
			"platform_order_id		VARCHAR(128) CHARACTER SET utf8 NOT NULL,"
			"platform_account_id	VARCHAR(128) CHARACTER SET utf8 NOT NULL,"
			"platform_payment_type	TINYINT	UNSIGNED	NOT NULL,"
			"state				TINYINT	UNSIGNED	NOT NULL,"
			"payment_time		INT	UNSIGNED	NOT NULL DEFAULT '0',"
			"distribute_time	INT UNSIGNED 	NOT NULL DEFAULT '0',"
			"payment_ip			VARCHAR(64) CHARACTER SET utf8 NOT NULL DEFAULT '',"
			"payment_device		VARCHAR(32) CHARACTER SET utf8 NOT NULL DEFAULT '',"
			"payment_device_type VARCHAR(64) CHARACTER SET utf8 NOT NULL DEFAULT '',"
			"payment_device_uid	VARCHAR(128) CHARACTER SET utf8 NOT NULL DEFAULT '',"
			"client_order_id	VARCHAR(128) CHARACTER SET utf8 NOT NULL DEFAULT '',"
			"addition1			VARCHAR(128) CHARACTER SET utf8 NOT NULL DEFAULT '',"
			"addition2			VARCHAR(128) CHARACTER SET utf8 NOT NULL DEFAULT '',"
			"addition3			VARCHAR(128) CHARACTER SET utf8 NOT NULL DEFAULT '',"
			"addition4			VARCHAR(128) CHARACTER SET utf8 NOT NULL DEFAULT '',"
			"addition5			VARCHAR(128) CHARACTER SET utf8 NOT NULL DEFAULT '',"
			"PRIMARY KEY (auto_id)"
		") ENGINE=InnoDB DEFAULT CHARSET=utf8;"
	);

	if ( !ExistsColumn( "charge_info", "payment_device_uid" ) )
		m_db->PExecute( "ALTER TABLE charge_info ADD COLUMN payment_device_uid VARCHAR(128) CHARACTER SET utf8 NOT NULL DEFAULT ''" );
	if ( !ExistsColumn( "charge_info", "addition5" ) )
		m_db->PExecute( "ALTER TABLE charge_info ADD COLUMN addition5 VARCHAR(128) CHARACTER SET utf8 NOT NULL DEFAULT ''" );
	if ( !ExistsColumn( "charge_info", "diamond_pay" ) )
		m_db->PExecute( "ALTER TABLE `charge_info` ADD COLUMN `diamond_pay` int(10) unsigned NOT NULL DEFAULT 0 COMMENT '充值付费虚拟货币';");

	// gain_info
	m_db->PExecute(
		"CREATE TABLE IF NOT EXISTS gain_info ("
			"auto_id		INT 	UNSIGNED	NOT NULL AUTO_INCREMENT,"
			"role_id		INT		UNSIGNED	NOT NULL,"
			"source_type	INT		UNSIGNED	NOT NULL,"
			"source_id		INT		UNSIGNED	NOT NULL,"
			"value			INT		UNSIGNED	NOT NULL,"
			"time			INT		UNSIGNED	NOT NULL,"
			"PRIMARY KEY(auto_id)"
		") ENGINE=InnoDB DEFAULT CHARSET=utf8;"
	);

	// purchase_info (diamond only)
	m_db->PExecute(
		"CREATE TABLE IF NOT EXISTS purchase_info ("
			"auto_id		INT		UNSIGNED	NOT NULL AUTO_INCREMENT,"
			"role_id		INT		UNSIGNED	NOT NULL,"
			"goods_id		INT		UNSIGNED	NOT NULL,"
			"goods_quantity	INT		UNSIGNED	NOT NULL,"
			"value			INT		UNSIGNED	NOT NULL,"
			"diamond_paid_use	INT		UNSIGNED	NOT NULL,"
			"time			INT		UNSIGNED	NOT NULL,"
			"PRIMARY KEY (auto_id)"
		") ENGINE=InnoDB DEFAULT CHARSET=utf8;"
	);

	if ( !ExistsColumn( "purchase_info", "diamond_paid_use" ) )
		m_db->PExecute( "ALTER TABLE purchase_info ADD COLUMN diamond_paid_use INT UNSIGNED NOT NULL" );

	// gm_cmd
	m_db->PExecute(
		"CREATE TABLE IF NOT EXISTS gm_cmd ("
			"auto_id			INT		UNSIGNED	NOT NULL AUTO_INCREMENT,"
			"opr			VARCHAR(32)	CHARACTER SET utf8 NOT NULL,"
			"params			VARCHAR(21000) CHARACTER SET utf8 NOT NULL DEFAULT '',"
			"target_type	TINYINT		UNSIGNED	NOT NULL,"
			"target_id		BIGINT		UNSIGNED	NOT NULL,"
			"start_time			INT		UNSIGNED	NOT NULL DEFAULT '0',"
			"end_time			INT		UNSIGNED	NOT NULL DEFAULT '0',"
			"backup_value	VARCHAR(128) 	CHARACTER SET utf8 NOT NULL DEFAULT '',"
			"status			TINYINT		UNSIGNED	NOT NULL DEFAULT '0',"
			"error_msg		VARCHAR(256)	CHARACTER SET utf8 NOT NULL DEFAULT '',"
			"create_time		INT		UNSIGNED	NOT NULL DEFAULT '0',"
			"author			VARCHAR(32) 	CHARACTER SET utf8 NOT NULL DEFAULT '',"
			"PRIMARY KEY (auto_id)"
		") ENGINE=InnoDB DEFAULT CHARSET=utf8;"
	);

	if ( !ExistsColumn( "gm_cmd", "author" ) )
		m_db->PExecute( "ALTER TABLE gm_cmd ADD COLUMN author VARCHAR(32) 	CHARACTER SET utf8 NOT NULL DEFAULT ''" );
	if ( !ExistsColumn( "gm_cmd", "create_time" ) )
		m_db->PExecute( "ALTER TABLE gm_cmd ADD COLUMN create_time INT UNSIGNED NOT NULL" );
	if ( !ExistsColumn( "gm_cmd", "params" ) )
		m_db->PExecute( "ALTER TABLE gm_cmd ADD COLUMN params VARCHAR(21000)	CHARACTER SET utf8 NOT NULL" );
	if ( !ExistsIndex( "gm_cmd", "index_status" ) )
		m_db->PExecute( "CREATE INDEX index_status ON gm_cmd(status)" );

	// gift_box
	m_db->PExecute(
		"CREATE TABLE IF NOT EXISTS gift_box("
			"id			INT		UNSIGNED	NOT NULL,"
			"idx		INT		UNSIGNED	NOT NULL,"
			"role_id	INT		UNSIGNED	NOT NULL,"
			"PRIMARY KEY(id, idx)"
		") ENGINE=InnoDB DEFAULT CHARSET=utf8;"
	);

	// invite_info
	m_db->PExecute(
		"CREATE TABLE IF NOT EXISTS invite_info("
			"role_id	INT		UNSIGNED	NOT NULL,"
			"level		INT		UNSIGNED	NOT NULL,"
			"inviter_id	INT		UNSIGNED	NOT NULL,"
			"PRIMARY KEY(role_id)"
		") ENGINE=InnoDB DEFAULT CHARSET=utf8;"
	);

	// gift_box_config
	m_db->PExecute(
		"CREATE TABLE IF NOT EXISTS gift_box_config("
			"id			INT		UNSIGNED	NOT NULL,"
			"param1		INT		UNSIGNED	NOT NULL,"
			"param2		INT		UNSIGNED	NOT NULL,"
			"param3		INT		UNSIGNED	NOT NULL,"
			"reward		VARCHAR(512) 	CHARACTER SET utf8 NOT NULL DEFAULT '',"
			"server		VARCHAR(512) 	CHARACTER SET utf8 NOT NULL DEFAULT '',"
			"dead_time	INT		UNSIGNED	NOT NULL,"
			"use_max	INT		UNSIGNED	NOT NULL,"
			"platform	VARCHAR(512) 	CHARACTER SET utf8 NOT NULL DEFAULT '0',"
			"PRIMARY KEY(id)"
		") ENGINE=InnoDB DEFAULT CHARSET=utf8;"
	);

	if ( !ExistsColumn( "gift_box_config", "platform" ) )
		m_db->PExecute( "ALTER TABLE gift_box_config ADD COLUMN platform VARCHAR(512)	CHARACTER SET utf8 NOT NULL DEFAULT '0'" );

	// active_stat
	m_db->PExecute(
		"CREATE TABLE IF NOT EXISTS active_stat("
			"auto_id		INT		UNSIGNED	NOT NULL AUTO_INCREMENT,"
			"active_id		INT 	UNSIGNED	NOT NULL,"
			"active_type	INT		UNSIGNED	NOT NULL,"
			"role_id		INT		UNSIGNED	NOT NULL,"
			"reach_idx		INT		UNSIGNED	NOT NULL,"
			"reach_time		INT		UNSIGNED	NOT NULL,"
			"PRIMARY KEY(auto_id)"
		") ENGINE=InnoDB DEFAULT CHARSET=utf8;"
	);

	// ad_active_info
	m_db->PExecute(
		"CREATE TABLE IF NOT EXISTS ad_active_info("
			"auto_id		INT		UNSIGNED	NOT NULL AUTO_INCREMENT,"
			"device_mac_md5	VARCHAR(128) CHARACTER SET utf8 NOT NULL DEFAULT '',"
			"status			TINYINT	UNSIGNED	NOT NULL,"
			"cp 			VARCHAR(64) CHARACTER SET utf8 NOT NULL DEFAULT '',"
			"app_id			VARCHAR(64) CHARACTER SET utf8 NOT NULL DEFAULT '',"
			"addition1		VARCHAR(256) CHARACTER SET utf8 NOT NULL DEFAULT '',"
			"addition2		VARCHAR(256) CHARACTER SET utf8 NOT NULL DEFAULT '',"
			"addition3		VARCHAR(256) CHARACTER SET utf8 NOT NULL DEFAULT '',"
			"PRIMARY KEY(auto_id)"
		") ENGINE=InnoDB DEFAULT CHARSET=utf8;"
	);

	if ( !ExistsColumn( "ad_active_info", "cp" ) )
		m_db->PExecute( "ALTER TABLE ad_active_info ADD COLUMN cp VARCHAR(64) CHARACTER SET utf8 NOT NULL DEFAULT ''" );
	if ( !ExistsColumn( "ad_active_info", "app_id" ) )
		m_db->PExecute( "ALTER TABLE ad_active_info ADD COLUMN app_id VARCHAR(64) CHARACTER SET utf8 NOT NULL DEFAULT ''" );
	if ( !ExistsIndex( "ad_active_info", "index_md5" ) )
		m_db->PExecute( "CREATE UNIQUE INDEX index_md5 ON ad_active_info(device_mac_md5)" );
	if ( !ExistsIndex( "ad_active_info", "index_status" ) )
		m_db->PExecute( "CREATE INDEX index_status ON ad_active_info(status)" );

	// guild_reg
	m_db->PExecute(
		"CREATE TABLE IF NOT EXISTS guild_reg("
			"guild_id		INT		UNSIGNED	NOT NULL AUTO_INCREMENT,"
			"role_id		INT		UNSIGNED	NOT NULL,"
			"create_time	INT		UNSIGNED	NOT NULL,"
			"PRIMARY KEY(guild_id)"
		") ENGINE=InnoDB DEFAULT CHARSET=utf8 AUTO_INCREMENT=10000;"
	);

	// player_blob_share
	m_db->PExecute(
		"CREATE TABLE IF NOT EXISTS player_blob_share ("
			"role_id		BIGINT(20) UNSIGNED NOT NULL,"
			"server_id		INT	UNSIGNED NOT NULL DEFAULT 0,"
			"name			VARCHAR(32)	CHARACTER SET utf8 NOT NULL,"
			"level			INT UNSIGNED NOT NULL DEFAULT 0,"
			"cap_hero_id	INT UNSIGNED NOT NULL DEFAULT 0,"
			"achieve		blob		NULL,"
			"battle_hero	blob		NULL,"
			"photo			blob		NULL,"
			"tech			blob		NULL,"
			"quest			blob		NULL,"
			"buff			blob		NULL,"
			"explore		blob		NULL,"
			"totem			blob		NULL,"
			"win_cnt		INT	UNSIGNED NOT NULL DEFAULT 0,"
			"combat			INT UNSIGNED NOT NULL DEFAULT 0,"
			"update_time	INT UNSIGNED NOT NULL DEFAULT 0,"
			"PRIMARY KEY(role_id)"
		") ENGINE=InnoDB DEFAULT CHARSET=utf8;"
	);

	if ( !ExistsColumn( "player_blob_share", "totem" ) )
		m_db->PExecute( "ALTER TABLE player_blob_share ADD COLUMN totem blob	NULL" );

	// player_rank
	m_db->PExecute(
		"CREATE TABLE IF NOT EXISTS player_rank ("
			"server_id		INT			UNSIGNED NOT NULL DEFAULT 0,"
			"rank_type		TINYINT		UNSIGNED NOT NULL DEFAULT 0,"
			"rank			MEDIUMINT	UNSIGNED NOT NULL DEFAULT 0,"
			"role_name		VARCHAR(32)	CHARACTER SET utf8 NOT NULL,"
			"rank_value		INT			UNSIGNED NOT NULL DEFAULT 0,"
			"PRIMARY KEY( server_id, rank_type, rank )"
		")ENGINE=InnoDB DEFAULT CHARSET=utf8;"
	);

	// gameserver_merge_log
	m_db->PExecute(
		"CREATE TABLE IF NOT EXISTS gameserver_merge_log ("
			"auto_id			INT		UNSIGNED 	NOT NULL AUTO_INCREMENT,"
			"server_id_origin	INT		UNSIGNED	NOT NULL DEFAULT 0,"
			"server_id_merged	INT		UNSIGNED	NOT NULL DEFAULT 0,"
			"merge_time			INT		UNSIGNED	NOT NULL DEFAULT 0,"
			"PRIMARY KEY( auto_id )"
		")ENGINE=InnoDB DEFAULT CHARSET=utf8;"
	);
	if ( !ExistsColumn( "gameserver_merge_log", "merge_type" ) )
		m_db->PExecute( "ALTER TABLE gameserver_merge_log ADD COLUMN merge_type INT UNSIGNED DEFAULT 0" );

	// player_detail_info
	m_db->PExecute(
		"CREATE TABLE IF NOT EXISTS player_detail("
			"role_id		INT			UNSIGNED	NOT NULL,"
			"diamond_pay	INT			UNSIGNED	NOT NULL DEFAULT '0',"
			"last_op		INT 		UNSIGNED 	DEFAULT 0,"
			"totem	 		varchar(1024) CHARACTER SET utf8 NULL,"
			"jewel			varchar(1024) CHARACTER SET utf8 NULL,"
			"godskill		varchar(2048) CHARACTER SET utf8 NULL,"
			"herobag		varchar(2048) CHARACTER SET utf8 NULL,"
			"equipbag		varchar(4096) CHARACTER SET utf8 NULL,"
			"gembag			varchar(4096) CHARACTER SET utf8 NULL,"
			"oper_statistics	varchar(65535) CHARACTER SET utf8 NULL,"
			"timecard_state		varchar(126) CHARACTER SET utf8 NOT NULL DEFAULT '',"
			"PRIMARY KEY(role_id)"
		") ENGINE=InnoDB DEFAULT CHARSET=utf8;"
	);

	if ( !ExistsColumn( "player_detail", "last_op" ) )
		m_db->PExecute( "ALTER TABLE player_detail ADD COLUMN last_op INT UNSIGNED DEFAULT 0" );

	if ( !ExistsColumn( "player_detail", "totem" ) )
		m_db->PExecute( "ALTER TABLE player_detail ADD COLUMN totem varchar(1024) CHARACTER SET utf8 NULL" );

	if ( !ExistsColumn( "player_detail", "godskill" ) )
		m_db->PExecute( "ALTER TABLE player_detail ADD COLUMN godskill varchar(2048) CHARACTER SET utf8 NULL" );

	if ( !ExistsColumn( "player_detail", "jewel" ) )
		m_db->PExecute( "ALTER TABLE player_detail ADD COLUMN jewel varchar(1024) CHARACTER SET utf8 NULL" );

	if ( !ExistsColumn( "player_detail", "herobag" ) )
		m_db->PExecute( "ALTER TABLE player_detail ADD COLUMN herobag varchar(2048) CHARACTER SET utf8 NULL" );

	if ( !ExistsColumn( "player_detail", "equipbag" ) )
		m_db->PExecute( "ALTER TABLE player_detail ADD COLUMN equipbag varchar(4096) CHARACTER SET utf8 NULL" );

	if ( !ExistsColumn( "player_detail", "gembag" ) )
		m_db->PExecute( "ALTER TABLE player_detail ADD COLUMN gembag varchar(4096) CHARACTER SET utf8 NULL" );

	if ( !ExistsColumn( "player_detail", "oper_statistics" ) )
	{
		m_db->PExecute( "ALTER TABLE player_detail ADD COLUMN oper_statistics varchar(65535) CHARACTER SET utf8 NULL" );
		m_db->PExecute( "ALTER TABLE player_detail MODIFY COLUMN oper_statistics varchar(65535) CHARACTER SET utf8 NULL" );
	}

	if ( !ExistsColumn( "player_detail", "timecard_state" ) )
		m_db->PExecute( "ALTER TABLE player_detail ADD COLUMN timecard_state varchar(126) CHARACTER SET utf8 NOT NULL DEFAULT '' " );


	m_db->PExecute(
		"CREATE TABLE IF NOT EXISTS exchange_record("
			"auto_id		BIGINT		UNSIGNED	NOT NULL AUTO_INCREMENT,"
			"role_id		INT			UNSIGNED	DEFAULT 0,"
			"goods_id	 	INT 		UNSIGNED 	DEFAULT 0,"
			"goods_quantity	INT 		UNSIGNED 	DEFAULT 0,"
			"value			INT 		UNSIGNED 	DEFAULT 0,"
			"time			INT 		UNSIGNED 	DEFAULT 0,"
			"PRIMARY KEY(auto_id)"
		") ENGINE=InnoDB DEFAULT CHARSET=utf8;"
	);

	m_db->PExecute(
			"CREATE TABLE IF NOT EXISTS `login_strategy_backup` ("
			"`idx` int(10) unsigned NOT NULL AUTO_INCREMENT,"
			"`auto_id` int(10) unsigned NOT NULL DEFAULT '0',"
			"`strategy_id` int(10) unsigned NOT NULL,"
			"`condition_id` tinyint(3) unsigned NOT NULL,"
			"`type` tinyint(3) unsigned NOT NULL,"
			"`value` varchar(64) NOT NULL,"
			"`creator` varchar(32) DEFAULT NULL COMMENT '记录创建人',"
			"`remark` varchar(100) DEFAULT NULL COMMENT '备注',"
			"`backup_time` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '备份时间',"
			"PRIMARY KEY (`idx`)"
			") ENGINE=InnoDB AUTO_INCREMENT=30 DEFAULT CHARSET=utf8;"
	);
	if ( !ExistsColumn( "login_strategy_backup", "is_not" ) )
		m_db->PExecute( "ALTER TABLE login_strategy_backup ADD COLUMN is_not TINYINT UNSIGNED	NOT NULL DEFAULT '0'" );

	// cdkey_generate_batch
	if (!ExistsTable("cdkey_generate_batch"))
	{
	m_db->PExecute(
		"CREATE TABLE IF NOT EXISTS cdkey_generate_batch ("
			"auto_id			INT		UNSIGNED	NOT NULL AUTO_INCREMENT,"
			"channel			INT 	UNSIGNED 	NOT NULL ,"
			"start_time			INT		UNSIGNED	NOT NULL DEFAULT '0',"
			"end_time			INT		UNSIGNED	NOT NULL DEFAULT '0',"
			"limit_number				INT		UNSIGNED	NOT NULL ,"
			"batch_content			VARCHAR(256)	CHARACTER SET utf8 NOT NULL DEFAULT '',"
			"create_time		INT		UNSIGNED	NOT NULL DEFAULT '0',"
			"author				VARCHAR(32)	NOT NULL DEFAULT '',"
			"prize				VARCHAR(8192) default '',"
			"PRIMARY KEY (auto_id), INDEX(channel)"
		") ENGINE=InnoDB DEFAULT CHARSET=utf8;"
	);
	}

	// cdkey_list
	if (!ExistsTable("cdkey_list"))
	{
	m_db->PExecute(
		"CREATE TABLE IF NOT EXISTS cdkey_list ("
			"cdkey				VARCHAR(128)		NOT NULL ,"
			"batch_id			INT 	UNSIGNED 	NOT NULL ,"
			"create_time		INT		UNSIGNED	NOT NULL DEFAULT '0',"
			"create_author				VARCHAR(32)	NOT NULL DEFAULT '',"
			"get_characterid			VARCHAR(32)	NOT NULL DEFAULT '',"
			"status			TINYINT		UNSIGNED	NOT NULL DEFAULT '0',"
			"PRIMARY KEY (cdkey), INDEX(get_characterid)"
		") ENGINE=InnoDB DEFAULT CHARSET=utf8;"
	);
	}

	QueryResult* result = m_db->PQuery(
		"select MAX(passport_id) from passport_info"
	);

	if ( !result )
	{
		IME_SQLERROR( "Error Get Max 'passport_id' from passport_info" );
		return false;
	}

	uint64 maxPassportId = result->Fetch()[0].GetUInt64();
	m_maxPassportAutoInc = (uint32)( maxPassportId >> 32 );

	IME_SYSTEM_LOG( "CenterDBCtrl Init", "maxPassportAutoInc=%u", m_maxPassportAutoInc );
	delete result;
	result = NULL;

	result = m_db->PQuery(
		"select MAX(role_id) from re_passport_player where mod(role_id >> 1, 128)=%u", m_byLoginServerId
	);

	if ( !result )
	{
		IME_SQLERROR( "Error Get Max 'role_id' from re_passport_player" );
		return false;
	}
	uint32 maxRoleId = result->Fetch()[0].GetUInt32();
	m_maxRoleAutoInc = (uint32)( maxRoleId >> 8 );

	IME_SYSTEM_LOG( "CenterDBCtrl Init", "maxRoleAutoInc=%u", m_maxRoleAutoInc );

	delete result;

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

bool CenterDBCtrl::ExistsPassport( std::string strPassport, uint8 byAuthType, uint16 wPlatform )
{
	m_db->escape_string( strPassport );

	QueryResult* result = m_db->PQuery(
		"select passport_id from passport_info where passport='%s' and platform=%u and auth_type=%u",
		strPassport.c_str(), wPlatform, byAuthType );

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
			std::string		strResVersionFull,
			std::string		strResVersionConfig,
			uint8			byCanLogin,
			uint8 			byStatus,
			uint32			dwLoginStrategyId,
	   		uint32			dwCanRegister	)
{
	QueryResult* result = m_db->PQuery(
		"select update_time from gameserver_info where server_id=%u",
		dwServerId
	);

	m_db->escape_string( strIp );
	m_db->escape_string( strServerName );
	m_db->escape_string( strVersion );
	m_db->escape_string( strResVersionFull );
	m_db->escape_string( strResVersionConfig );
	m_db->escape_string( strLocalIp );
	m_db->escape_string( strResServerAddr );

	if ( result )
	{
		delete result;
		m_db->PExecute(
			"update gameserver_info set server_name='%s',ip='%s',local_ip='%s', port=%u, version='%s', res_version='%s', res_version_config='%s', "
			"online_num=%u, can_login=%u, status=%u, login_strategy_id=%u, res_server_ip='%s', update_time=%u, can_register=%u where server_id=%u", strServerName.c_str(),
			strIp.c_str(), strLocalIp.c_str(), dwPort, strVersion.c_str(), strResVersionFull.c_str(), strResVersionConfig.c_str(), dwOnlineNum,
			byCanLogin, byStatus, dwLoginStrategyId, strResServerAddr.c_str(), GetDBTime(), dwCanRegister, dwServerId
		);
	}
	else
	{
		m_db->PExecute(
			"insert into gameserver_info "
			"(server_id, server_name, ip, local_ip, port, version, res_version, res_version_config, online_num, can_login, status, login_strategy_id, res_server_ip, update_time) "
			"values(%u, '%s', '%s', '%s', %u, '%s', '%s', '%s', %u, %u, %u, %u, '%s', %u)",
			dwServerId, strServerName.c_str(), strIp.c_str(), strLocalIp.c_str(), dwPort, strVersion.c_str(), strResVersionFull.c_str(), strResVersionConfig.c_str(),
			dwOnlineNum, byCanLogin, byStatus, dwLoginStrategyId, strResServerAddr.c_str(), GetDBTime()
		);
	}
	return true;
}

bool CenterDBCtrl::UpdateServerVersionCode( uint32 dwGameServerId, uint32 dwServerVersionCode )
{
	return m_db->PExecute("update gameserver_info set version_code=%u where server_id=%u", dwServerVersionCode, dwGameServerId );
}

uint32 CenterDBCtrl::GetServerVersionCode( uint32 dwGameServerId )
{
	QueryResult* result = m_db->PQuery(
		"select version_code from gameserver_info where server_id=%u", dwGameServerId
		);

	if ( result )
	{
		Field* field = result->Fetch();

		uint32 dwServerVersionCode = field[0].GetUInt32();

		delete result;
		return dwServerVersionCode;
	}

	IME_SYSTEM_ERROR( "GetServerVersionCode", "ServerVersionCode Not Found, serverId=%u", dwGameServerId );
	return 0;
}

uint32 CenterDBCtrl::GetServerIdMerged( uint32 dwServerIdOrigin )
{
	uint32 dwId = dwServerIdOrigin;

	while ( true )
	{
		QueryResult* result = m_db->PQuery( "select server_id_merged from gameserver_merge_log where server_id_origin = %u", dwId );

		if ( result )
		{
			dwId = result->Fetch()[0].GetUInt32();
			delete result;
		}
		else
		{
			break;
		}
	}

	return dwId;
}

void CenterDBCtrl::GetInfoMerged(uint32 dwServerIdOrigin, uint32& dwMergedServerId, uint32& dwMergedType)
{
	uint32 dwId = dwServerIdOrigin;
	uint32 dwType = 0;

	while ( true )
	{
		QueryResult* result = m_db->PQuery( "select server_id_merged,merge_type from gameserver_merge_log where server_id_origin = %u", dwId );

		if ( result )
		{
			Field* field = result->Fetch();
			dwId = field[0].GetUInt32();
			dwType = field[1].GetUInt32();
			delete result;
		}
		else
		{
			break;
		}
	}
	dwMergedServerId = dwId;
	dwMergedType = dwType;
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

//	// WinPhone 中文版923, 至尊版10103,帐号互通
//	if ( result == NULL && ( 923 == wPlatform ||  10103 == wPlatform) )
//	{
//		result = m_db->PQuery(
//				"select passport_id, pwd from passport_info where passport='%s' and (platform = 923 or platform = 10103) and auth_type=%u",
//				strPassport.c_str(), E_LOGIN_AUTH_TYPE_ACCOUNT
//		);
//	}

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
			std::string		strOpenUdid,
			std::string		strAppleUdid,
			uint64&			odwPassportId )
{
	m_db->escape_string( strPlatformToken );
	m_db->escape_string( strUid );
	m_db->escape_string( strDeviceToken );

	QueryResult* result = m_db->PQuery(
		"select passport_id from passport_info where passport='%s' and platform=%u and auth_type=%u",
		strPlatformToken.c_str(), wPlatform, E_LOGIN_AUTH_TYPE_PLATFORM
	);

	if ( result == NULL &&
			( (wPlatform >= 750 && wPlatform <= 800) || (wPlatform >= 10001 && wPlatform <= 20000) || (wPlatform >= 40001 && wPlatform <= 50000) ) )  /* ADVIEW platform group */
	{
		result = m_db->PQuery(
			"select passport_id from passport_info where passport='%s' and ((platform >= 750 and platform <= 800) or (platform >= 10001 and platform <=20000) or (platform >= 40001 and platform <= 50000)) and auth_type=%u",
			strPlatformToken.c_str(), E_LOGIN_AUTH_TYPE_PLATFORM
		);
	}

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
				GetDBTime(), 0, strRegIp.c_str(), strRegDevice.c_str(), strRegDeviceType.c_str(), strOpenUdid, strAppleUdid, odwPassportId ) )
		{
			return E_LOGIN_RESULT_TYPE_OK;
		}
		else
		{
			return E_LOGIN_RESULT_ERROR;
		}
	}
}

LoginResultType CenterDBCtrl::ValidateAuthFast(
		std::string		strUid,
		std::string		strOpenUdid,
		std::string		strAppleUdid,
		uint8			byIOSVersion,
		std::string		strDeviceToken,
		uint16			wPlatform,
		std::string		strRegIp,
		std::string		strRegDevice,
		std::string		strRegDeviceType,
		uint64&			odwPassportId )
{
	m_db->escape_string( strUid );
	m_db->escape_string( strDeviceToken );

	QueryResult* result = NULL;

	if ( byIOSVersion < E_IOS_VERSION_7 )
	{
		result = m_db->PQuery(
			"select passport_id from passport_info where passport='%s' and platform=%u and auth_type=%u",
			strUid.c_str(), wPlatform, E_LOGIN_AUTH_TYPE_FAST
		);
	}
	else
	{
		do
		{
			if ( strOpenUdid.length() )
			{
				result =  m_db->PQuery(
					"select passport_id from passport_info where open_udid='%s' and platform=%u and auth_type=%u",
					strOpenUdid.c_str(), wPlatform, E_LOGIN_AUTH_TYPE_FAST
				);

				if ( result ) break;
			}

			if ( strAppleUdid.length() )
			{

				result = m_db->PQuery(
					"select passport_id from passport_info where apple_udid='%s' and platform=%u and auth_type=%u",
					strAppleUdid.c_str(), wPlatform, E_LOGIN_AUTH_TYPE_FAST
				);

				if ( result ) break;
			}

		} while (0);
	}

	if ( result )
	{
		Field* field = result->Fetch();
		uint64 dwPassport = field[0].GetUInt64();
		delete result;

		if ( strOpenUdid.length() )
		{
			m_db->PExecute("update passport_info set open_udid='%s' where passport_id=%llu",
					strOpenUdid.c_str(), dwPassport );
		}

		if ( strAppleUdid.length() )
		{
			m_db->PExecute("update passport_info set apple_udid='%s' where passport_id=%llu",
					strAppleUdid.c_str(), dwPassport );
		}
		odwPassportId = dwPassport;
		return E_LOGIN_RESULT_TYPE_OK;
	}
	else
	{
		if ( InsertPassportInfo( strUid, "", "", strUid, strDeviceToken, wPlatform, E_LOGIN_AUTH_TYPE_FAST,
				GetDBTime(), 0, strRegIp, strRegDevice, strRegDeviceType, strOpenUdid, strAppleUdid, odwPassportId ) )
		{
			return E_LOGIN_RESULT_TYPE_OK;
		}
		else
		{
			return E_LOGIN_RESULT_ERROR;
		}
	}
}

bool CenterDBCtrl::ModifyPassword( uint64 dwPassportId, std::string strNewPwd )
{
	m_db->escape_string( strNewPwd );

	return m_db->PExecute(
		"update passport_info set pwd='%s' where passport_id=%llu and auth_type=%u",
		strNewPwd.c_str(), dwPassportId, E_LOGIN_AUTH_TYPE_ACCOUNT
	);
}

bool CenterDBCtrl::ModifyPassword( uint32 dwRoleId, std::string strOldPwd, std::string strNewPwd )
{
	m_db->escape_string( strOldPwd );
	m_db->escape_string( strNewPwd );

	uint64 dwPassportId = GetPassportId( dwRoleId );
	if ( dwPassportId == 0 ) return false;

	QueryResult* result = m_db->PQuery(
		"select pwd, auth_type from passport_info where passport_id=%llu", dwPassportId
	);

	std::string strPwd;
	uint8		byAuthType;

	if ( result )
	{
		Field* field = result->Fetch();

		strPwd		= field[0].GetString();
		byAuthType	= field[1].GetUInt8();

		delete result;

		if ( byAuthType != E_LOGIN_AUTH_TYPE_ACCOUNT ) return false;
		if ( strPwd != strOldPwd ) return false;
	}
	else
	{
		return false;
	}

	return m_db->PExecute(
		"update passport_info set pwd='%s' where passport_id=%llu",
		strNewPwd.c_str(), dwPassportId
	);
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
			std::string		strOpenUdid,
			std::string		strAppleUdid,
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

	odwPassportId = NextPassportId( wPlatform );

	bool ret = m_db->PExecute(
		"insert into passport_info(passport_id,passport,pwd,mail,uid,token,platform,auth_type,create_time,gm_auth,reg_ip,reg_device,reg_device_type,open_udid,apple_udid) "
		"values(%llu,'%s','%s','%s','%s','%s',%u,%u,%u,%u,'%s','%s','%s','%s','%s')",
		odwPassportId, strPassport.c_str(), strPwd.c_str(), strMail.c_str(), strUid.c_str(), strToken.c_str(),
		wPlatform, byAuthType, dwCreateTime, byGmAuth, strCreateIp.c_str(),
		strCreateDevice.c_str(), strCreateDeviceType.c_str(), strOpenUdid.c_str(), strAppleUdid.c_str()
	);

	if ( ret )
	{
		// activate advertise
		m_db->PExecute( "update ad_active_info set status=1 where device_mac_md5='%s' and status=0", strUid.c_str() );
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
		std::string&	ostrPlatformId,
		uint8&			obyGmAuth )
{
	QueryResult* result = m_db->PQuery(
		"select platform, passport, gm_auth from passport_info where passport_id=%llu", dwPassportId
	);

	if ( result )
	{
		Field* field = result->Fetch();

		owPlatform  	= field[0].GetUInt16();
		ostrPlatformId 	= field[1].GetString();
		obyGmAuth		= field[2].GetUInt8();

		delete result;
		return true;
	}
	else
	{
		return false;
	}
}

uint32 CenterDBCtrl::GetPassportRegTime( uint64 dwPassportId )
{
	QueryResult* result = m_db->PQuery(
		"select create_time from passport_info where passport_id=%llu", dwPassportId
	);

	uint32 ret = 0;

	if ( result )
	{
		ret = result->Fetch()[0].GetUInt32();
		delete result;
	}
	return ret;
}

std::string CenterDBCtrl::GetDevice( uint64 dwPassportId )
{
	QueryResult* result = m_db->PQuery(
		"select reg_device from passport_info where passport_id=%llu", dwPassportId
	);

	if ( result )
	{
		std::string ret = result->Fetch()[0].GetString();
		delete result;

		return ret;
	}
	return "";
}

uint64 CenterDBCtrl::GetPassportId( uint32 dwRoleId )
{
	QueryResult* result = m_db->PQuery(
		"select passport_id from re_passport_player where role_id=%u", dwRoleId
	);

	if ( result )
	{
		uint64 ret = result->Fetch()[0].GetUInt64();
		delete result;

		return ret;
	}
	return 0;
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
		for (unsigned int i = 0; i < cnt; i++ )
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
		IME_SQLERROR( "Error Backup Passport To GameServer, passportId=%llu", (unsigned long long int)dwPassportId );
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
		"select server_id, server_name, ip,port,version,res_version,res_version_config,online_num,can_login,status,login_strategy_id, res_server_ip, update_time, version_code, can_register from gameserver_info" );

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
			stcServer.strResVersionFull	= fields[5].GetString();
			stcServer.strResVersionConfig = fields[6].GetString();
			stcServer.dwOnlineNum 	= fields[7].GetUInt32();
			stcServer.byCanLogin	= fields[8].GetUInt8();
			stcServer.byStatus		= fields[9].GetUInt8();
			stcServer.dwLoginStrategy = fields[10].GetUInt32();
			stcServer.strResServerAddr = fields[11].GetString();
			stcServer.dwLastUpdateTime = fields[12].GetUInt32();
			stcServer.dwServerVersionCode = fields[13].GetUInt32();
			stcServer.dwCanRegister = fields[14].GetUInt32();

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

		for ( std::map<uint32, STC_SERVER_STATUS>::iterator it = mapServer.begin();
				it != mapServer.end(); ++it )
		{
			if ( !it->second.bIsAlive )
			{
				// check whether this server has been merged
//				uint32 dwServerIdMerged = GetServerIdMerged( it->second.dwServerId );
				uint32 dwServerIdMerged,dwMergedType;
				GetInfoMerged(it->second.dwServerId, dwServerIdMerged, dwMergedType);
				if ( dwServerIdMerged != it->second.dwServerId )
				{
					std::map<uint32, STC_SERVER_STATUS>::iterator itMaster = mapServer.find( dwServerIdMerged );
					if ( itMaster != mapServer.end() )
					{
						IME_SYSTEM_LOG( "GetOrUpdateGameServerStatus", "Replace With MasterInfo, slaveId=%u, masterId=%u,"
								"slaveStra=%u, masterStra=%u", it->second.dwServerId, itMaster->second.dwServerId,
								it->second.dwLoginStrategy, itMaster->second.dwLoginStrategy );

						// same entrance condition as its master server
						it->second.strIp		= itMaster->second.strIp;
						it->second.dwPort		= itMaster->second.dwPort;
						it->second.strVersion	= itMaster->second.strVersion;
						it->second.strResVersionFull	= itMaster->second.strResVersionFull;
						it->second.strResVersionConfig 	= itMaster->second.strResVersionConfig;

						it->second.dwOnlineNum 	= 0;
						it->second.byCanLogin	= itMaster->second.byCanLogin;
						it->second.byStatus		= itMaster->second.byStatus;
						if (dwMergedType == 0)
							it->second.dwLoginStrategy 	= itMaster->second.dwLoginStrategy;
						it->second.strResServerAddr = itMaster->second.strResServerAddr;
						it->second.bIsAlive 	= itMaster->second.bIsAlive;
						it->second.dwCanRegister = itMaster->second.dwCanRegister;
					}
				}
			}
		}
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

//			uint32 dwTime = GetDBTime();
//			// slow
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

int CenterDBCtrl::GetOrInsertRoleId( uint64 dwPassportId, uint32 dwServerIdOrigin, uint32 &roleId, uint32 canRegister)
{
	QueryResult* result = m_db->PQuery(
		"select role_id from re_passport_player where passport_id=%llu and server_id_origin=%u",
		dwPassportId, dwServerIdOrigin
	);

	if ( result )
	{
		Field* field = result->Fetch();
		uint32 dwRoleId = field[0].GetUInt32();
		delete result;
		roleId = dwRoleId;
		return 0;
	}
	else
	{
		if (!canRegister)
		{
			return 1;
		}
		uint32 dwRoleId = NextRoleId();

		if (m_db->PExecute( "insert into re_passport_player(role_id, passport_id, server_id, server_id_origin, create_time) values(%u, %llu, %u, %u, %u)",
				dwRoleId, dwPassportId, GetServerIdMerged(dwServerIdOrigin), dwServerIdOrigin, GetDBTime() ) )
		{
			roleId = dwRoleId;
			return 0;
		}
		else
		{
			IME_ERROR( "Error When Insert Into re_passport_player" );
			return -1;
		}
	}
}

bool CenterDBCtrl::IsRoleForbid( uint32 dwRole )
{
	return GetRoleStatus( dwRole ) == E_ROLE_STATUS_FORBID;
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
			std::string		strRegDeviceType,
			std::string		strOpenUdid,
			std::string		strAppleUdid )
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
				GetDBTime(), 0, strRegIp, strRegDevice, strRegDeviceType, strOpenUdid, strAppleUdid, dwPassportId );
	}
}


bool CenterDBCtrl::InsertOrUpdateRoleInfo(
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
			uint32			dwDiamondPay
	)
{
	if ( dwRoleId & 1 ) return true;

	QueryResult* result = m_db->PQuery( "select role_id from player_info where role_id=%u", dwRoleId );
	if ( result )
	{
		delete result;

		m_db->escape_string( strRoleName );

		UpdateInviteLevel( dwRoleId, dwLevel );

		if ( dwMainQuestId )
		{
			return m_db->PExecute( "update player_info set name='%s', progress=%u, level=%u, "
					"gold=%u, diamond=%u, cur_stage=%u, cur_train=%u, vip_level=%u, vip_exp=%u, stamina=%u, energy=%u,"
					"quest=%u, diamond_pay=%u  where role_id=%u",
					strRoleName.c_str(), dwProgress, dwLevel, dwGold, dwDiamond, dwCurStage, dwCurTrain, dwVipLevel, dwVipExp,
					dwStamina, dwEnergy, dwMainQuestId, dwDiamondPay, dwRoleId
			);
		}
		else
		{
			return m_db->PExecute( "update player_info set name='%s', progress=%u, level=%u, "
					"gold=%u, diamond=%u, cur_stage=%u, cur_train=%u, vip_level=%u, vip_exp=%u, stamina=%u, energy=%u, diamond_pay=%u where role_id=%u",
					strRoleName.c_str(), dwProgress, dwLevel, dwGold, dwDiamond, dwCurStage, dwCurTrain, dwVipLevel, dwVipExp,
					dwStamina, dwEnergy, dwDiamondPay, dwRoleId
			);
		}
	}
	else
	{
		m_db->escape_string( strRoleName );

		return m_db->PExecute( "insert into player_info( role_id, name, gm_auth, status, progress, level, "
				"gold, diamond, cur_stage, cur_train, vip_level, vip_exp, stamina, energy, quest, diamond_pay ) "
				"values( %u, '%s', %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u )",
				dwRoleId, strRoleName.c_str(), 0, 0, dwProgress, dwLevel, dwGold, dwDiamond, dwCurStage, dwCurTrain, dwVipLevel, dwVipExp,
				dwStamina, dwEnergy, dwMainQuestId, dwDiamondPay
		);
	}
}

bool CenterDBCtrl::InsertOrUpdateRoleDetailInfo(
			uint32			dwRoleId,
			uint32			dwDiamondPay
	)
{
	if ( dwRoleId & 1 ) return true;

	QueryResult* result = m_db->PQuery( "select role_id from player_detail where role_id=%u", dwRoleId );
	if ( result )
	{
		delete result;
		return m_db->PExecute( "update player_detail set diamond_pay='%u' where role_id=%u", dwDiamondPay, dwRoleId );
	}
	else
	{
		return m_db->PExecute( "insert into player_detail( role_id, diamond_pay ) values( %u, %u )", dwRoleId, dwDiamondPay );
	}
}

bool CenterDBCtrl::InsertOrUpdateRoleLastOp(
			uint32			dwRoleId,
			uint32			dwLastOp
	)
{
	if ( dwRoleId & 1 ) return true;

	QueryResult* result = m_db->PQuery( "select role_id from player_detail where role_id=%u", dwRoleId );
	if ( result )
	{
		delete result;
		return m_db->PExecute( "update player_detail set last_op='%u' where role_id=%u", dwLastOp , dwRoleId );
	}
	else
	{
		return m_db->PExecute( "insert into player_detail( role_id, last_op ) values( %u, %u )", dwRoleId, dwLastOp );
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

bool CenterDBCtrl::UpdateTimeCardRecord( uint32 dwRoleId, std::string szCardInfo )
{
   m_db->PExecute("update player_detail set timecard_state = '%s' where role_id=%llu ", szCardInfo.c_str(), dwRoleId );
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
		for (unsigned int i = 0; i < cnt; i++ )
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

bool CenterDBCtrl::DeleteAllActivity( uint32 dwGameServerId )
{
	return m_db->PExecute("delete from active_info where server_id=%u", dwGameServerId );
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

bool CenterDBCtrl::ClearPassportGmAuth()
{
	return m_db->PExecute("update passport_info set gm_auth = 0 where gm_auth > 0");
}

bool CenterDBCtrl::ClearRoleGmAuth()
{
	return m_db->PExecute("update player_info set gm_auth = 0 where gm_auth > 0");
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

bool CenterDBCtrl::GetExtendReward( const char* strTableName, const char* strId, uint32& dwItemId, uint32& dwItemCnt, uint8& byState )
{
	if ( !ExistsTable( strTableName ) )
	{
		IME_SQLERROR( "Table Not Found When GetExtendReward, name=%s", strTableName );
		return false;
	}

	QueryResult* result = m_db->PQuery(
		"select item_id, item_cnt, state from %s where id='%s'",
		strTableName, strId );

	if ( result )
	{
		Field* field = result->Fetch();

		dwItemId	= field[0].GetUInt32();
		dwItemCnt	= field[1].GetUInt32();
		byState		= field[2].GetUInt8();

		delete result;
		return true;
	}
	else
	{
		return false;
	}
}

bool CenterDBCtrl::InsertOrUpdateOperStatisticsInfo(uint32	dwRoleId,ByteBuffer &data)
{
	std::string str = "";
	str.append( (const char*)data.contents(), data.size() );

	QueryResult* result = m_db->PQuery( "select oper_statistics from player_detail where role_id=%u", dwRoleId );
	if ( result )
	{
		delete result;
		return m_db->PExecute( "update player_detail set oper_statistics='%s' where role_id=%u", str.c_str(), dwRoleId );
	}
	else
	{
		return false;
	}
}


bool CenterDBCtrl::SetExtendRewardGot( const char* strTableName, const char* strId, uint8 byState )
{
	if ( !ExistsTable( strTableName ) )
	{
		IME_SQLERROR( "Table Not Found When GetExtendReward, name=%s", strTableName );
		return false;
	}

	return m_db->PExecute(
		"update %s set state=%u where id='%s'", strTableName, byState, strId
	);
}

bool CenterDBCtrl::GetUnhandledCharge(
		std::list<STC_CHARGE_INFO>& vCharges )
{
	QueryResult* result = m_db->PQuery(
		"select auto_id, role_id, goods_id, goods_quantity, addition2, platform, addition5 from charge_info where state=%u",
		E_CHARGE_STATE_PAYED
	);

	vCharges.clear();

	if ( result )
	{
		uint64 cnt = result->GetRowCount();
		for (unsigned int i = 0; i < cnt; i++ )
		{
			Field* field = result->Fetch();

			STC_CHARGE_INFO ci;
			ci.dwAutoId		= field[0].GetUInt32();
			ci.dwRoleId		= field[1].GetUInt32();
			ci.dwGoodsId	= field[2].GetUInt32();
			ci.dwGoodsQuantity = field[3].GetUInt32();
			ci.strAddition2	= field[4].GetString();
			ci.wPlatform	= field[5].GetUInt16();
			ci.strAddition5 = field[6].GetString();

			vCharges.push_back( ci );

			result->NextRow();
		}

		delete result;
	}

	return true;
}

uint32 CenterDBCtrl::GethandledTotalCharge(
		uint32 accountid )
{
	QueryResult* result = m_db->PQuery(
		"select sum(virtual_value) from charge_info where state=%u and role_id =%u",
		E_CHARGE_STATE_DISTRIBUTED, accountid
	);


	if ( result )
	{
		Field* field = result->Fetch();
		uint32 total = field[0].GetUInt32();
		delete result;
		return total;
	}

	return 0;
}

bool CenterDBCtrl::GethandledTotalChargeAll(
		std::map<uint32, uint32> &mapCharge )
{
	QueryResult* result = m_db->PQuery(
		"select role_id, virtual_value from charge_info where state=%u",
		E_CHARGE_STATE_DISTRIBUTED
	);

	mapCharge.clear();
	if ( result )
	{
		uint64 cnt = result->GetRowCount();
		for (unsigned int i = 0; i < cnt; i++ )
		{
			Field* field = result->Fetch();

			uint32 role_id		 = field[0].GetUInt32();
			uint32 virtual_value = field[1].GetUInt32();
			
			if (mapCharge.find(role_id) == mapCharge.end())
			{
				mapCharge[role_id] = virtual_value;
			}
			else
			{
				mapCharge[role_id] += virtual_value;
			}

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
			std::string strDeviceUid,
	   		uint32		diamondPay	)
{
	m_db->escape_string( strIp );
	m_db->escape_string( strDevice );
	m_db->escape_string( strDeviceType );
	m_db->escape_string( strDeviceUid );

	return m_db->PExecute(
		"update charge_info set virtual_value=%u, state=%u, distribute_time=%u, payment_ip='%s',"
		"payment_device='%s', payment_device_type='%s', payment_device_uid='%s', diamond_pay = '%u' where auto_id=%u", dwDiamondValue, E_CHARGE_STATE_DISTRIBUTED,
		GetDBTime(), strIp.c_str(), strDevice.c_str(), strDeviceType.c_str(), strDeviceUid.c_str(), diamondPay, dwAutoId
	);

	return false;
}

uint32 CenterDBCtrl::CreateCharge(
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
	    std::string	addition2	)
{
	m_db->escape_string( strCurrency );
	m_db->escape_string( strInnerOrderId );
	m_db->escape_string( strPlatformOrderId );
	m_db->escape_string( strPlatformAccount );
	m_db->escape_string( strClientOrderId );

	bool succ = m_db->PExecute(
		"insert into charge_info(role_id, goods_id, goods_quantity, currency, value, type, inner_order_id,"
		"platform_order_id, platform_account_id, platform, platform_payment_type, payment_time, client_order_id, addition2, state) "
		"values(%u, %u, %u, '%s', %u, 0, '%s', '%s', '%s', %u, %u, %u, '%s', '%s', 1)", dwRoleId, dwGoodsId, dwGoodsQty,
		strCurrency.c_str(), dwValue, strInnerOrderId.c_str(), strPlatformOrderId.c_str(), strPlatformAccount.c_str(),
		wPlatform, wPaymentType, dwPaymentTime, strClientOrderId.c_str(), addition2.c_str() );

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
		IME_SQLERROR("Error When Insert Info charge_info");
		return 0;
	}
}

bool CenterDBCtrl::HasCharge( uint16 wPlatform, std::string strPlatformOrderId )
{
	m_db->escape_string( strPlatformOrderId );

	QueryResult* result = m_db->PQuery(
		"select auto_id from charge_info where platform=%u and platform_order_id='%s'",
		wPlatform, strPlatformOrderId.c_str()
	);

	if ( result )
	{
		delete result;
		return true;
	}
	return false;
}

uint32 CenterDBCtrl::InsertPurchaseInfo(
			uint32			dwRoleId,
			uint32			dwGoodsId,
			uint32			dwGoodsQuantity,
			uint32			dwValue,
			uint32			dwDiamondPaidUse,
			uint32			dwTime )
{
	bool succ = m_db->PExecute(
		"insert into purchase_info(role_id, goods_id, goods_quantity, value, diamond_paid_use, time) "
		"values(%u, %u, %u, %u, %u, %u)", dwRoleId, dwGoodsId, dwGoodsQuantity, dwValue, dwDiamondPaidUse, dwTime
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

//bool CenterDBCtrl::GetNoticeOfServer( uint32 dwServerId, std::string& strNotice )
//{
//	QueryResult* result = m_db->PQuery(
//		"select content from notice_info where server_id=%u", dwServerId
//	);
//
//	if ( result )
//	{
//		strNotice = result->Fetch()[0].GetString();
//
//		delete result;
//		return true;
//	}
//	return false;
//}
//
//bool CenterDBCtrl::UpdateOrInsertNotice( uint32 dwServerId, std::string strNotice )
//{
//	m_db->escape_string( strNotice );
//	QueryResult* result = m_db->PQuery(
//		"select auto_id from notice_info where server_id=%u", dwServerId
//	);
//	if ( result )
//	{
//		delete result;
//		return m_db->PExecute("update notice_info set content='%s' where server_id=%u", strNotice.c_str(), dwServerId );
//	}
//	else
//	{
//		return m_db->PExecute("insert into notice_info( server_id, content ) values( %u, '%s' )", dwServerId, strNotice.c_str() );
//	}
//}

std::string CenterDBCtrl::GetNotice( NoticeUseType eUseType, NoticeConditionType eCondType, uint32 dwCondValue )
{
	QueryResult* result = m_db->PQuery(
		"select content from notice_info_v2 where use_type=%u and condition_type=%u and condition_value=%u",
		(uint8)eUseType, (uint8)eCondType, dwCondValue
	);

	std::string ret = "";

	if ( result )
	{
		uint64 cnt = result->GetRowCount();
		for (unsigned int i = 0; i < cnt; i++ )
		{
			ret.append( result->Fetch()[0].GetString() );

			result->NextRow();
		}
		delete result;
	}
	return ret;
}

bool CenterDBCtrl::GetNoticeTimeRange( NoticeUseType eUseType, NoticeConditionType eCondType, uint32 dwCondValue, uint32 &dwStartTime, uint32 &dwEndTime )
{
	QueryResult* result = m_db->PQuery(
		"select start_time, end_time from notice_info_v2 where use_type=%u and condition_type=%u and condition_value=%u",
		(uint8)eUseType, (uint8)eCondType, dwCondValue
	);

	if ( result )
	{
		dwStartTime = result->Fetch()[0].GetUInt32();
		dwEndTime = result->Fetch()[1].GetUInt32();

		delete result;
		return true;
	}

	return false;
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
		"status, limit_day, sort_idx, icon_id, goods_name, description, limit_once, limit_time_start, limit_time_end,"
		"vip_show, vip_buy, buy_count_total, buy_count_inc, platform_goods_id, platform_type, weight, diamond_pay, limit_number, limit_type, prize_float, is_double, buy_ex, ios_goods_id, android_goods_id, currency, good_gift, item_icon, collect_icon from goods_info where server_id=%u", dwGameServerId
	);

	vGoods.clear();
	if ( result )
	{
		uint64 cnt = result->GetRowCount();
		for (unsigned int i = 0; i < cnt; i++ )
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
			t.dwLimitTimeStart	= field[16].GetUInt32();
			t.dwLimitTimeEnd	= field[17].GetUInt32();
			t.byVipShow			= field[18].GetUInt8();
			t.byVipBuy			= field[19].GetUInt8();
			t.dwBuyCountTotal	= field[20].GetUInt32();
			t.dwBuyCountInc		= field[21].GetUInt32();
			t.strPlatformGoodsId= field[22].GetString();
			t.byPlatformType	= field[23].GetUInt8();
			t.dwWeight			= field[24].GetUInt32();
			t.dwDiamondPay		= field[25].GetUInt32();
			t.dwLimitNumber		= field[26].GetCppString();
			t.dwLimitType		= field[27].GetUInt32();
			t.dwPrizeFloat		= field[28].GetUInt32();
			t.byIsDouble		= field[29].GetUInt32();
			t.buyEx				= field[30].GetCppString();

//			t.IOSGoodId			= field[31].GetCppString();
//			t.AndroidGoodId		= field[32].GetCppString();
			t.thirdGoodId		= field[31].GetCppString();
			t.currency			= field[33].GetCppString();
			t.goodGift			= field[34].GetUInt32();
			t.itemIcon			= field[35].GetCppString();
			t.collectionIcon	= field[36].GetCppString();
			vGoods.insert( std::make_pair( t.dwGoodsId, t ) );
			result->NextRow();
		}
		delete result;
	}
}

void CenterDBCtrl::UpdateGoodsInfoOfGameServerOnly( std::map<uint32, STC_GOODS_INFO>& vGoods, uint32 dwGameServerId, uint8 byTypeOnly )
{
	m_db->PExecute( "delete from goods_info where server_id=%u and shop_type = %u", dwGameServerId, byTypeOnly );
	UpdateGoodsInfoOfGameServer( vGoods, dwGameServerId );
}

void CenterDBCtrl::UpdateGoodsInfoOfGameServerExcept( std::map<uint32, STC_GOODS_INFO>& vGoods, uint32 dwGameServerId, uint8 byTypeEx )
{
	m_db->PExecute( "delete from goods_info where server_id=%u and shop_type <> %u", dwGameServerId, byTypeEx );
	UpdateGoodsInfoOfGameServer( vGoods, dwGameServerId );
}

void CenterDBCtrl::UpdateGoodsInfoOfGameServerAll( std::map<uint32, STC_GOODS_INFO>& vGoods, uint32 dwGameServerId )
{
	m_db->PExecute( "delete from goods_info where server_id=%u", dwGameServerId );
	UpdateGoodsInfoOfGameServer( vGoods, dwGameServerId );
}

void CenterDBCtrl::UpdateGoodsInfoOfGameServer( std::map<uint32, STC_GOODS_INFO>& vGoods, uint32 dwGameServerId )
{
	for ( std::map<uint32, STC_GOODS_INFO>::iterator it = vGoods.begin();
			it != vGoods.end(); ++it )
	{
		m_db->PExecute( "insert ignore into goods_info(server_id, goods_id, shop_type, buy_type_id, "
				"buy_content_id, buy_count, cost_type_id, cost_content_id, cost_count, cost_count_old, "
				"status, limit_day, sort_idx, icon_id, goods_name, description, limit_once, limit_time_start, limit_time_end,"
				"vip_show, vip_buy, buy_count_total, buy_count_inc, platform_goods_id, platform_type, weight, diamond_pay, limit_number, limit_type, prize_float, is_double, ios_goods_id, android_goods_id, currency, good_gift, item_icon, collect_icon, buy_ex) values(%u, %u, "
				"%u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, '%s', '%s', %d, %u, %u, %u, %u, %u, %u, '%s', %u, %u, %u, '%s', %u, %u, %u, '%s', '%s', '%s', %u, '%s', '%s', '%s')",
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
				it->second.dwBuyLimitOnce,
				it->second.dwLimitTimeStart,
				it->second.dwLimitTimeEnd,
				it->second.byVipShow,
				it->second.byVipBuy,
				it->second.dwBuyCountTotal,
				it->second.dwBuyCountInc,
				it->second.strPlatformGoodsId.c_str(),
				it->second.byPlatformType,
				it->second.dwWeight,
				it->second.dwDiamondPay,
			    it->second.dwLimitNumber.c_str(),
				it->second.dwLimitType,
				it->second.dwPrizeFloat, 
				it->second.byIsDouble,
				it->second.thirdGoodId.c_str(),
				it->second.thirdGoodId.c_str(),
				it->second.currency.c_str(),
				it->second.goodGift,
				it->second.itemIcon.c_str(),
				it->second.collectionIcon.c_str(),
				it->second.buyEx.c_str()
				);
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

uint16 CenterDBCtrl::GetPlatformId( uint32 dwRoleId )
{
	QueryResult* result = m_db->PQuery( "select platform from passport_info where passport_id="
			"(select passport_id from re_passport_player where role_id=%u)", dwRoleId );

	if ( result )
	{
		uint16 ret = result->Fetch()[0].GetUInt16();
		delete result;

		return ret;
	}
	else
	{
		return 0;
	}
}

uint32 CenterDBCtrl::GetRoleCreateTime( uint32 dwRoleId )
{
	uint32 ret = 0;
	QueryResult* result = m_db->PQuery(
		"select create_time from re_passport_player where role_id=%u", dwRoleId
	);
	if ( result )
	{
		ret = result->Fetch()[0].GetUInt32();
		delete result;
	}

	return ret;
}

std::string CenterDBCtrl::GetRoleName( uint32 dwRoleId )
{
	QueryResult* result = m_db->PQuery(
		"select name from player_info where role_id=%u", dwRoleId );

	if ( result )
	{
		std::string ret = result->Fetch()[0].GetString();
		delete result;

		return ret;
	}
	return "";
}

uint16 CenterDBCtrl::GetRoleServerId( uint32 dwRoleId )
{
	// get server_id after merged
	QueryResult* result = m_db->PQuery(
		"select server_id from re_passport_player where role_id=%u", dwRoleId
	);
	if ( result )
	{
		uint16 ret = result->Fetch()[0].GetUInt16();
		delete result;
		return ret;
	}
	return 0;
}

int CenterDBCtrl::GetRolePlatform( uint32 dwRoleId )
{
	QueryResult* result = m_db->PQuery(
		"select b.platform from re_passport_player a inner join passport_info b on (a.passport_id = b.passport_id) where role_id=%u",
		dwRoleId
	);

	if ( result )
	{
		uint16 ret = result->Fetch()[0].GetUInt16();
		delete result;
		return ret;
	}
	return -1;
}



bool CenterDBCtrl::SetRoleStatus( uint32 dwRoleId, uint8 byStatus )
{
	return m_db->PExecute(
		"update player_info set status=%u where role_id=%u", byStatus, dwRoleId
	);
}

uint8 CenterDBCtrl::GetRoleStatus( uint32 dwRoleId )
{
	QueryResult* result = m_db->PQuery(
		"select status from player_info where role_id=%u", dwRoleId
	);

	if ( result )
	{
		uint8 ret = result->Fetch()[0].GetUInt8();
		delete result;
		return ret;
	}
	return 0;
}

uint16 CenterDBCtrl::GetLastLoginServer( uint64 dwPassportId )
{
	QueryResult* result = m_db->PQuery(
		"select last_login_server from passport_info where passport_id=%llu", dwPassportId
	);

	if ( result )
	{
		uint16 ret = result->Fetch()[0].GetUInt16();
		delete result;
		return ret;
	}
	return 0;
}

bool CenterDBCtrl::SetLastLoginServer( uint64 dwPassportId, uint16 wServerId )
{
	return m_db->PExecute( "update passport_info set last_login_server=%u where passport_id=%llu",
			wServerId, dwPassportId );
}

bool CenterDBCtrl::IsTestServer( uint32 dwServerId )
{
	QueryResult* result = m_db->PQuery(
		"select is_test from gameserver_info where server_id=%u", dwServerId );

	if ( result )
	{
		uint8 byIsTest = result->Fetch()[0].GetUInt8();

		delete result;
		return byIsTest;
	}
	return false;
}

bool CenterDBCtrl::GetGiftCodeInfo( uint32 dwId, uint32& odwParam1, uint32& odwParam2, uint32& odwParam3,
		std::string& ostrReward, std::string& ostrServers, std::string& ostrPlatform, uint32& odwDeadTime, uint32& odwMaxUse )
{
	QueryResult* result = m_db->PQuery(
		"select param1, param2, param3, reward, server, platform, dead_time, use_max from gift_box_config where id=%u", dwId
	);

	if ( result )
	{
		Field* field = result->Fetch();

		odwParam1 	= field[0].GetUInt32();
		odwParam2 	= field[1].GetUInt32();
		odwParam3 	= field[2].GetUInt32();
		ostrReward	= field[3].GetString();
		ostrServers	= field[4].GetString();
		ostrPlatform= field[5].GetString();
		odwDeadTime	= field[6].GetUInt32();
		odwMaxUse	= field[7].GetUInt32();

		delete result;
		return true;
	}
	else
	{
		return false;
	}
}

bool CenterDBCtrl::CheckGiftCodeUsed( uint32 dwId, uint32 dwIdx )
{
	QueryResult* result = m_db->PQuery(
		"select * from gift_box where id=%u and idx=%u", dwId, dwIdx );

	if ( result )
	{
		delete result;
		return true;
	}
	return false;
}

uint32 CenterDBCtrl::GetGiftCodeSameTypeUsed( uint32 dwRoleId, uint32 dwId )
{
	QueryResult* result = m_db->PQuery(
		"select * from gift_box where id=%u and role_id=%u", dwId, dwRoleId );
	if ( result )
	{
		uint32 ret = result->GetRowCount();
		delete result;
		return ret;
	}
	return 0;
}

bool CenterDBCtrl::InsertGiftCodeUse( uint32 dwRoleId, uint32 dwId, uint32 dwIdx )
{
	return m_db->PExecute( "insert into gift_box( id, idx, role_id ) values( %u, %u, %u )",
			dwId, dwIdx, dwRoleId );
}

bool CenterDBCtrl::InsertInviteInfo( uint32 dwRoleId, uint32 dwLevel, uint32 dwInviterId )
{
	return m_db->PExecute( "insert into invite_info( role_id, level, inviter_id ) values( %u, %u, %u )",
			dwRoleId, dwLevel, dwInviterId );
}

bool CenterDBCtrl::UpdateInviteLevel( uint32 dwRoleId, uint32 dwLevel )
{
	return m_db->PExecute( "update invite_info set level=%u where role_id=%u", dwLevel, dwRoleId );
}

bool CenterDBCtrl::GetInviteLevelList( uint32 dwRoleId, std::vector<uint32>& vLevel, std::vector<uint32>& vId )
{
	vLevel.clear();
	QueryResult* result = m_db->PQuery("select role_id, level from invite_info where inviter_id=%u", dwRoleId );

	if ( result )
	{
		uint64 cnt = result->GetRowCount();
		for (unsigned int i = 0; i < cnt; i++ )
		{
			Field* field = result->Fetch();

			uint32 dwId		= field[0].GetUInt32();
			uint32 dwLevel 	= field[1].GetUInt32();

			vId.push_back( dwId );
			vLevel.push_back( dwLevel );

			result->NextRow();
		}

		delete result;
	}

	return true;
}

uint32 CenterDBCtrl::GetInviterId( uint32 dwRoleId )
{
	QueryResult* result = m_db->PQuery("select inviter_id from invite_info where role_id=%u", dwRoleId );

	if ( result )
	{
		uint32 ret = result->Fetch()[0].GetUInt32();
		delete result;
		return ret;
	}
	return 0;
}

uint8 CenterDBCtrl::BindPassport( uint32 dwRoleId, LoginAuthType eAuthType, std::string strPassport,
			std::string strPassword, std::string strMail )
{
	if ( strPassport.empty() )
		return E_LOGIN_BIND_RESULT_TYPE_EMPTY_PASSPORT;

	if ( eAuthType == E_LOGIN_AUTH_TYPE_ACCOUNT && ( strPassword.size() < 6 || strMail.empty() ) )
		return E_LOGIN_BIND_RESULT_TYPE_INVALID_PWD_OR_MAIL;

	uint64 dwPassportId = GetPassportId( dwRoleId );
	if ( dwPassportId == 0 )
		return E_LOGIN_BIND_RESULT_TYPE_PASSPORT_NOT_EXIST;

	uint16 wPlatformId;
	std::string strOldPlatformId;
	uint8 byGmAuth;

	GetPassportInfo( dwPassportId, wPlatformId, strOldPlatformId, byGmAuth );

	m_db->escape_string( strPassport );
	m_db->escape_string( strMail );

	QueryResult* result = m_db->PQuery( "select passport_id from passport_info where auth_type=%u and platform=%u and passport='%s'",
			(uint8)eAuthType, wPlatformId, strPassport.c_str() );

	if ( result )
	{
		if ( result->Fetch()[0].GetUInt64() != dwPassportId )
		{
			delete result;
			return E_LOGIN_BIND_RESULT_TYPE_DUPLICATE_PASSPORT;
		}
		else
		{
			delete result;
			return E_LOGIN_BIND_RESULT_TYPE_SUCC;
		}
	}

	if ( eAuthType == E_LOGIN_AUTH_TYPE_ACCOUNT )
	{
		m_db->PExecute( "update passport_info set passport='%s', pwd='%s', mail='%s', auth_type=%u where passport_id=%llu",
				strPassport.c_str(), strPassword.c_str(), strMail.c_str(), eAuthType, dwPassportId );
		return E_LOGIN_BIND_RESULT_TYPE_SUCC;
	}
	else if ( eAuthType == E_LOGIN_AUTH_TYPE_PLATFORM )
	{
		m_db->PExecute( "update passport_info set passport='%s', auth_type=%u where passport_id=%llu",
				strPassport.c_str(), eAuthType, dwPassportId );
		return E_LOGIN_BIND_RESULT_TYPE_SUCC;
	}
	else
	{
		return E_LOGIN_BIND_RESULT_TYPE_INVALID_AUTH_TYPE;
	}
}

void CenterDBCtrl::AddActivityStat( uint32 dwActivityId, uint32 dwType, uint32 dwRoleId, uint32 dwReachIdx, uint32 dwReachTime )
{
	m_db->PExecute(
		"insert into active_stat( active_id, active_type, role_id, reach_idx, reach_time ) values( %u, %u, %u, %u, %u )",
		dwActivityId, dwType, dwRoleId, dwReachIdx, dwReachTime
	);
}

uint32 CenterDBCtrl::RegisterGuild( uint32 dwRoleId )
{
	if ( !m_db->PExecute(
		"insert into guild_reg( role_id, create_time ) values( %u, %u )",
		dwRoleId, GetDBTime() ) )
	{
		IME_SQLERROR("Error When Insert Guild Reg");
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
		IME_SQLERROR("Error When Insert Guild Reg");
		return 0;
	}
}

bool CenterDBCtrl::GetPlayerBlobShare( ByteBuffer& buf, uint32 accountId, const char* field )
{
	QueryResult* result = m_db->PQuery(
		"select %s from player_blob_share where role_id=%u",
		field, accountId
	);

	if ( result )
	{
		Field* field = result->Fetch();

		buf.clear();
		buf.append( field[0].GetString(), field[0].GetLength() );

		delete result;

		return true;
	}
	else
	{
		IME_SQLERROR( "GetPlayerBlobShareError, id=%u, field=%s", accountId, field );
	}
	return false;
}

bool CenterDBCtrl::SetPlayerBlobShare( ByteBuffer& buf, uint32 accountId, const char* field )
{
	std::string str;
	str.append( (const char*)buf.contents(), buf.size() );
	m_db->escape_string( str );

	QueryResult* result = m_db->PQuery("select role_id from player_blob_share where role_id=%u", accountId );
	if ( result )
	{
		delete result;
	}
	else
	{
		m_db->PExecute( "insert into player_blob_share (role_id) values(%u)", accountId );
	}

	bool ret = m_db->PExecute(
		"update player_blob_share set %s='%s' where role_id=%u",
		field, str.c_str(), accountId
	);

	if ( !ret )
	{
		IME_SQLERROR( "SetPlayerBlobShareError, id=%u, field=%s", accountId, field );
		return false;
	}
	return true;
}

//worldDatabase.PExecute(
//		"CREATE TABLE IF NOT EXISTS player_blob_share ("
//			"role_id		BIGINT(20) UNSIGNED NOT NULL,"
//			"server_id		INT	UNSIGNED NOT NULL DEFAULT 0,"
//			"name			VARCHAR(32)	CHARACTER SET utf8 NOT NULL,"
//			"level			INT UNSIGNED NOT NULL DEFAULT 0,"
//			"cap_hero_id	INT UNSIGNED NOT NULL DEFAULT 0,"
//			"achieve		blob		NULL,"
//			"battle_hero	blob		NULL,"
//			"photo			blob		NULL,"
//			"tech			blob		NULL,"
//			"quest			blob		NULL,"
//			"buff			blob		NULL,"
//			"explore		blob		NULL,"
//			"win_cnt		INT	UNSIGNED NOT NULL DEFAULT 0,"
//			"combat			INT UNSIGNED NOT NULL DEFAULT 0,"
//			"PRIMARY KEY(account_id)"
//		") ENGINE=InnoDB DEFAULT CHARSET=utf8;"
//	);

bool CenterDBCtrl::GetPlayerInfoShareAll( std::vector<STC_WORLD_BATTLE_SHARE>& vRoles, uint32 dwServerId )
{
	QueryResult* result = NULL;

	uint32 dwTime = GetDBTime();
	if ( dwTime == 0 ) return false;

	// fetch only active player ( update in a week )
	dwTime -= 60 * 60 * 24 * 7;

	if ( IsTestServer( dwServerId ) )
	{
		result = m_db->PQuery( "select role_id, b.server_id, b.version, b.server_name, name, level, cap_hero_id, win_cnt, combat, a.update_time from "
			"player_blob_share a inner join gameserver_info b on a.server_id = b.server_id where a.update_time > %u", dwTime );
	}
	else
	{
		result = m_db->PQuery( "select role_id, b.server_id, b.version, b.server_name, name, level, cap_hero_id, win_cnt, combat, a.update_time from "
			"player_blob_share a inner join gameserver_info b on a.server_id = b.server_id where b.is_test=0 and a.update_time > %u", dwTime );
	}

	vRoles.clear();

	if ( result )
	{
		uint64 cnt = result->GetRowCount();
		for (unsigned int i = 0; i < cnt; i++ )
		{
			Field* field = result->Fetch();

			STC_WORLD_BATTLE_SHARE t;
			t.dwRoleId		= field[0].GetUInt32();
			t.dwServerId	= field[1].GetUInt32();
			t.strServerVer	= field[2].GetString();
			t.strServerName	= field[3].GetString();
			t.strName		= field[4].GetString();
			t.dwLevel		= field[5].GetUInt32();
			t.dwCapHeroId	= field[6].GetUInt32();
			t.dwWinCnt		= field[7].GetUInt32();
			t.dwCombat		= field[8].GetUInt32();
			t.dwUpdateTime	= field[9].GetUInt32();

			vRoles.push_back( t );
			result->NextRow();
		}

		delete result;
	}

	return true;
}

bool CenterDBCtrl::SetPlayerInfoShare( uint32 dwRoleId, uint32 dwServerId, const char* strName, uint32 dwLevel, uint32 dwCapHeroId,
		uint32 dwWinCnt, uint32 dwCombat )
{
	QueryResult* result = m_db->PQuery("select role_id from player_blob_share where role_id=%u", dwRoleId );
	if ( result )
	{
		delete result;
	}
	else
	{
		m_db->PExecute( "insert into player_blob_share (role_id) values(%u)", dwRoleId );
	}

	std::string strNameEs = strName;
	m_db->escape_string( strNameEs );

	return m_db->PExecute("update player_blob_share set server_id=%u, name='%s', level=%u, cap_hero_id=%u,"
			"win_cnt=%u, combat=%u, update_time=%u where role_id=%u", dwServerId, strNameEs.c_str(), dwLevel,
			dwCapHeroId, dwWinCnt, dwCombat, GetDBTime(), dwRoleId );
}

//m_db->PExecute(
//		"CREATE TABLE IF NOT EXISTS player_rank ("
//			"server_id		INT			UNSIGNED NOT NULL DEFAULT 0,"
//			"rank_type		TINYINT		UNSIGNED NOT NULL DEFAULT 0,"
//			"rank			MEDIUMINT	UNSIGNED NOT NULL DEFAULT 0,"
//			"role_name		VARCHAR(32)	CHARACTER SET utf8 NOT NULL,"
//			"rank_value		INT			UNSIGNED NOT NULL DEFAULT 0,"
//			"PRIMARY KEY( server_id, rank_type, rank )"
//		")ENGINE=InnoDB DEFAULT CHARSET=utf8;"
//	);

bool CenterDBCtrl::InsertOrUpdatePlayerRank( uint32 dwServerId, uint8 byRankType, uint32 dwRank, std::string strName, uint32 dwValue )
{
	m_db->escape_string( strName );

	QueryResult* result = m_db->PQuery("select * from player_rank where server_id=%u and rank_type=%u and rank=%u",
			dwServerId, byRankType, dwRank );

	if ( result )
	{
		delete result;
		return m_db->PExecute( "update player_rank set role_name='%s', rank_value=%u where server_id=%u and rank_type=%u and rank=%u",
				strName.c_str(), dwValue, dwServerId, byRankType, dwRank );
	}
	else
	{
		return m_db->PExecute( "insert into player_rank( server_id, rank_type, rank, role_name, rank_value ) values( %u, %u, %u, '%s', %u )",
				dwServerId, byRankType, dwRank, strName.c_str(), dwValue );
	}
}

bool CenterDBCtrl::GetCDKEYAndVerify(STC_CDKEY &t)
{
	std::string cdkey = t.strCDkey;
	m_db->escape_string(cdkey);
	QueryResult* result = m_db->PQuery(
		"select cdkey, channel, start_time, end_time, limit_number, prize, status, batch_id "
		" from cdkey_generate_batch,cdkey_list where cdkey_list.batch_id=cdkey_generate_batch.auto_id and cdkey = '%s'", cdkey.c_str()
	);

	if ( result )
	{
		Field* field = result->Fetch();

		t.strCDkey			= field[0].GetCppString();
		t.dwChannel			= field[1].GetUInt32();
		t.dwStartTime		= field[2].GetUInt32();
		t.dwEndTime			= field[3].GetUInt32();
		t.dwLimitNumber		= field[4].GetUInt32();
		t.strPrize			= field[5].GetCppString();
		t.byStatus			= field[6].GetUInt8();
		t.dwBatchId			= field[7].GetUInt32();
		return true;
	}
	delete result;
	return false;
}

int CenterDBCtrl::GetCount(uint32 batch_id, uint32 characterid, uint32 channel)
{
//	m_db->escape_string(cdkey);
	QueryResult* result = m_db->PQuery(
		"select count(*) "
		" from cdkey_list where batch_id=%u and get_characterid = %u", batch_id, characterid
	);

	if ( result )
	{
		Field* field = result->Fetch();

		int number = field[0].GetInt32();
		return number;
	}
	delete result;
	return -1;
}

bool CenterDBCtrl::UpdateCDKEY(std::string cdkey, uint32 characterid)
{
	m_db->escape_string(cdkey);
	return m_db->PExecute(
		"update cdkey_list "
		"set get_characterid = %u, status = 1 where cdkey = '%s'", characterid, cdkey.c_str()
	);
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

bool CenterDBCtrl::UpdateGmCommand()
{
	uint32 dwTime = GetDBTime();

	if ( dwTime <= m_dwMaxRunningGmCommandTime )
	{
		IME_SYSTEM_ERROR( "UpdateGmCommand", "Invalid Update, max=%u, cur=%u", m_dwMaxRunningGmCommandTime, dwTime );
		return false;
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

	// loop again
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

				std::string strErrorMsg = cmd->GetErrorMsg();
				m_db->escape_string( strErrorMsg );

				if ( !m_db->PExecute("update gm_cmd set error_msg='%s' where auto_id=%u",
						strErrorMsg.c_str(), cmd->GetAutoId() ) )
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

	for ( std::vector<GmCommand*>::iterator it = vRemoveCmd.begin();
			it != vRemoveCmd.end(); ++it )
	{
		RemoveGmCommand( *it );
	}
	m_dwMaxRunningGmCommandTime = dwTime;
	return true;
}

bool CenterDBCtrl::HasGmCommand( uint32 dwGmCommandId )
{
	return m_mapGmCommandAll.find( dwGmCommandId ) != m_mapGmCommandAll.end();
}

bool CenterDBCtrl::CancelGmCommand( uint32 dwGmCommandId )
{
	GmCommandMapType::iterator it = m_mapGmCommandAll.find( dwGmCommandId );
	if ( it != m_mapGmCommandAll.end() )
	{
		return it->second->Cancel();
	}
	return false;
}

const GmCommand* CenterDBCtrl::GetGmCommand( uint32 dwGmCommandId )
{
	GmCommandMapType::iterator it = m_mapGmCommandAll.find( dwGmCommandId );
	if ( it != m_mapGmCommandAll.end() )
	{
		return it->second;
	}
	return NULL;
}

uint32 CenterDBCtrl::HandlerGmCommandRole( void* pRole, uint32 dwLastCmdTime )
{
	if ( m_dwMaxRunningGmCommandTime <= dwLastCmdTime ) return dwLastCmdTime;

	for ( GmCommandMapType::iterator it = m_mapGmCommandAll.begin();
			it != m_mapGmCommandAll.end(); ++it )
	{
		if ( it->second->GetStatus() == E_GM_COMMAND_STATUS_RUNNING && dwLastCmdTime < it->second->GetStartTime() )
		{
			it->second->HandleRole( pRole );
		}
	}
	return m_dwMaxRunningGmCommandTime;
}

uint32 CenterDBCtrl::CreateSysGmCommand( std::string strOpr, std::string strParams, uint8 byTargetType,
			uint32 dwTargetId, uint32 dwStartTime, uint32 dwEndTime )
{
	m_db->escape_string( strOpr );
	m_db->escape_string( strParams );

	bool res = m_db->PExecute( "insert into gm_cmd (opr, params, target_type, target_id, start_time, end_time, backup_value,"
			"status, error_msg, create_time, author) values( '%s', '%s', %u, %u, %u, %u, '%s', %u, '%s', %u, '%s' )",
			strOpr.c_str(), strParams.c_str(), byTargetType, dwTargetId, dwStartTime, dwEndTime, "", 0, "", GetDBTime(), "sys" );

	if ( res )
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
			return 0;
		}
	}
	else
	{
		return 0;
	}
}

void CenterDBCtrl::ReadCommands( QueryResult* result )
{
	uint64 cnt = result->GetRowCount();
	for (unsigned int i = 0; i < cnt; i++ )
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
		std::string strParams	= field[8].GetTextString();

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

void CenterDBCtrl::CreateLoginToken( uint64 ddwPassportId, std::string strIp )
{
	m_db->PExecute( "update passport_info set token_ip='%s', token_time=%u where passport_id=%llu",
			strIp.c_str(), GetDBTime(), ddwPassportId );
}

void CenterDBCtrl::CreateLoginTokenByRoleId( uint64 roleId, uint32 time)
{
	m_db->PExecute( "update passport_info set token_time=%u where passport_id=(select passport_id from re_passport_player where role_id = %llu)",
			GetDBTime() + time, roleId );
}

void CenterDBCtrl::ClearLoginToken( uint32 dwRoleId )
{
	m_db->PExecute( "update passport_info set token_ip='', token_time=0 where passport_id=%llu", GetPassportId( dwRoleId ) );
}

static bool CheckIp( std::string strIp, std::string strPattern )
{
	std::vector<std::string> vTarget, vPattern;
	CUtil::StrSplit( strIp, ".", vTarget );
	if ( vTarget.size() != 4 ) return false;
	CUtil::StrSplit( strPattern, ".", vPattern );
	if ( vPattern.size() != 4 ) return false;

	for ( int i = 0; i < 4; i++ )
	{
		if ( vPattern[i] == "*" )
		{
			continue;
		}
		else if ( vPattern[i].find('-') != std::string::npos )
		{
			std::vector<std::string> vRange;
			CUtil::StrSplit( vPattern[i], "-", vRange );
			if ( vRange.size() != 2 ) return false;
			int a = atoi( vTarget[i].c_str() ), low = atoi( vRange[0].c_str() ), high = atoi( vRange[1].c_str() );
			if ( low > high ) std::swap( low, high );
			if ( a < low || a > high ) return false;
		}
		else
		{
			if ( atoi( vTarget[i].c_str() ) != atoi( vPattern[i].c_str() ) ) return false;
		}
	}
	return true;
}
bool CenterDBCtrl::CheckWhiteIP(std::string strIp)
{
	std::string strError = "Timeout";
	QueryResult* result = m_db->PQuery( "select value from login_strategy where strategy_id=255 and type=%u",
			E_LOGIN_STRATEGY_TYPE_IP );
	if ( result == NULL )
	{
		IME_ERROR( "CheckLoginToken ip %s %s", strIp.c_str(), strError.c_str() );
		return false;
	}

	int cnt = result->GetRowCount();
	bool bIpInWhite = false;

	for ( int i = 0; i < cnt; i++ )
	{
		if ( CheckIp( strIp, result->Fetch()[0].GetString() ) )
		{
			bIpInWhite = true;
			break;
		}
		result->NextRow();
	}

	delete result;

	if ( bIpInWhite )
	{
		IME_LOG( "CheckLoginToken Success, IP In White List" );
		return true;
	}
	return false;
}

bool CenterDBCtrl::CheckLoginToken( uint32 dwRoleId, std::string strIp, bool &isWhite )
{
	uint64 ddwPassportId = GetPassportId( dwRoleId );
	if ( ddwPassportId == 0 )
	{
		IME_USER_ERROR( "CheckLoginToken", dwRoleId, "Passport Not Found" );
		return false;
	}

	QueryResult* result = m_db->PQuery( "select token_ip, token_time from passport_info where passport_id=%llu",
			ddwPassportId );

	if ( result )
	{
		Field* field = result->Fetch();

		std::string strLoginIp 	= field[0].GetString();
		uint32		dwLoginTime	= field[1].GetUInt32();

		delete result;

		if ( GetDBTime() < dwLoginTime + 180 )
		{
			isWhite = false;
			IME_USER_LOG( "CheckLoginToken", dwRoleId, "Success" );
			return true;
		}
		else
		{
			std::string strError = "Timeout";

			result = m_db->PQuery( "select value from login_strategy where strategy_id=255 and type=%u",
					E_LOGIN_STRATEGY_TYPE_IP );
			if ( result == NULL )
			{
				IME_USER_ERROR( "CheckLoginToken", dwRoleId, "%s", strError.c_str() );
				return false;
			}

			int cnt = result->GetRowCount();
			bool bIpInWhite = false;

			for ( int i = 0; i < cnt; i++ )
			{
				if ( CheckIp( strIp, result->Fetch()[0].GetString() ) )
				{
					bIpInWhite = true;
					break;
				}
				result->NextRow();
			}

			delete result;

			if ( bIpInWhite )
			{
				isWhite = true;
				IME_USER_LOG( "CheckLoginToken", dwRoleId, "Success, IP In White List" );
				return true;
			}

			IME_USER_ERROR( "CheckLoginToken", dwRoleId, "%s", strError.c_str() );
			return false;
		}
	}
	else
	{
		IME_USER_ERROR( "CheckLoginToken", dwRoleId, "Token Not Found" );
		return false;
	}
}

bool CenterDBCtrl::GetServerName( uint32 dwServerId, std::string &strServerName)
{
	QueryResult* res = m_db->PQuery( "select server_name from gameserver_info where server_id=%u", dwServerId);

	if ( res )
	{
		strServerName = res->Fetch()[0].GetString();
		delete res;

		return true;
	}
	else
	{
		return false;
	}
}



bool CenterDBCtrl::GetUserBetweenLoginTime(uint32 dwServerId, uint32 dwBeginTime, uint32 dwEndTime, std::vector<uint32> &vecAccount)
{
	if(dwBeginTime > dwEndTime)
	{
		return false;
	}

	QueryResult* result = m_db->PQuery(
		"select a.role_id from player_info a inner join re_passport_player b on(a.role_id = b.role_id ) where a.last_login_time >= %u and a.last_login_time <= %u and b.server_id=%u",
		dwBeginTime, dwEndTime, dwServerId
	);

	vecAccount.clear();

	if ( result )
	{
		uint64 cnt = result->GetRowCount();
		for (unsigned int i = 0; i < cnt; i++ )
		{
			uint32 dwAccountdId;
			Field* field = result->Fetch();

			dwAccountdId = field[0].GetUInt32();
			vecAccount.push_back( dwAccountdId );

			result->NextRow();
		}

		delete result;
	}

	return true;
}

bool CenterDBCtrl::GetHeroBegsInfo(uint32 dwRoleId, std::string &strBegsInfo)
{
	strBegsInfo = "";
	QueryResult* result = m_db->PQuery("select herobag from player_detail where role_id=%u", dwRoleId);
	if (result) {
		uint64 u64Cnt = result->GetRowCount();
		if (1 != u64Cnt) {

			IME_SQLERROR( "error select herobag from player_detail by roleId=%u", dwRoleId );
			return false;
		} else {

			Field *fields = NULL;
			fields = result->Fetch();
			if (NULL != fields) {
				if (NULL != fields[0].GetString()) {

					strBegsInfo.append(fields[0].GetString());
				}
			}
			return true;
		}
	}
	return false;
}

bool CenterDBCtrl::SetPlayerDetailInfo(uint32 dwRoleId, std::string colum, std::string &buf)
{
	QueryResult* result = m_db->PQuery( "select role_id from player_detail where role_id=%u", dwRoleId );

	m_db->escape_string(buf);

	if ( result )
	{
		delete result;

		return m_db->PExecute( "update player_detail set %s='%s' where role_id=%u", colum.c_str(), buf.c_str(), dwRoleId );
	}
	else
	{
		return m_db->PExecute( "insert into player_detail(role_id, %s) values(%u, '%s')", colum.c_str(), dwRoleId, buf.c_str() );
	}

	return true;
}

bool CenterDBCtrl::SetExchangeRecordInfo(uint32 dwRoleId, uint32 dwGoodId, uint32 dwGoodNum, uint32 dwTime, uint32 dwValue)
{
	QueryResult* result = m_db->PQuery(
			"select goods_quantity from exchange_record where role_id=%u and goods_id=%u and time=%u ", dwRoleId, dwGoodId, dwTime
		);

	// 今天的存过
	if ( result )
	{
		uint64 cnt = result->GetRowCount();
		if(cnt > 1)
		{
			return false;
		}

		Field* field = result->Fetch();
		uint32 dwNum = field[0].GetUInt32();

		delete result;

		return m_db->PExecute( "update exchange_record set goods_quantity=%u where role_id=%u and goods_id=%u and time=%u",
				dwNum + dwGoodNum, dwRoleId, dwGoodId, dwTime);
	}
	// 还没有
	else
	{
		return m_db->PExecute( "insert into exchange_record(role_id, goods_id, goods_quantity, value, time) values(%u, %u, %u, %u, %u)",
				dwRoleId, dwGoodId, dwGoodNum, dwValue, dwTime);
	}
}

}
