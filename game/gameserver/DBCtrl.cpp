/*
 * DBCtrl.cpp
 *
 *  Created on: 2011-6-21
 *      Author: yq
 */
#include <string>
#include <assert.h>
#include <string.h>
#include "World.h"
#include "DBCtrl.h"
#include "Defines.h"
#include "DatabaseMysql.h"
#include "QueryResult.h"
#include "Field.h"
#include "ShardingDB.h"
#include "WorldDb.h"
#include "Log.h"
#include "Combine.h"
#include "EnumDefines.h"
#include "Util.h"
#include "CenterDBCtrl.h"
#include "ConfigData.h"

extern DatabaseMysql LoginDatabase;
extern CShardingDB WorldDb;
extern std::string g_strWorldDatabaseName;
//extern DatabaseMysql CenterDatabase;

extern int g_nVersion;

bool CDBCtrl::ExistsIndex(const char* table, const char* index)
{
	DatabaseMysql * mysql = WorldDb.GetDBConnection(0);
	QueryResult* result = mysql->PQuery(
			"select * from information_schema.statistics where table_name='%s' and index_name='%s' and table_schema = '%s'",
			table, index, g_strWorldDatabaseName.c_str()
			);

	if ( result )
	{
		delete result;
		return true;
	}
	return false;
}

bool CDBCtrl::ExistsColumn(const char* table, const char* column)
{
	DatabaseMysql * mysql = WorldDb.GetDBConnection(0);
	QueryResult* result = mysql->PQuery(
			"select * from information_schema.columns where table_name='%s' and column_name='%s' and table_schema='%s'",
			table, column, g_strWorldDatabaseName.c_str()
			);
	if ( result )
	{
		delete result;
		return true;
	}
	return false;
}

bool CDBCtrl::ExistsTable(const char* table)
{
	DatabaseMysql * mysql = WorldDb.GetDBConnection(0);
	QueryResult* result = mysql->PQuery(
			"select * from information_schema.tables where table_schema='%s' and table_name='%s'",
			g_strWorldDatabaseName.c_str(), table
			);
	if ( result )
	{
		delete result;
		return true;
	}
	return false;
}

bool CDBCtrl::ExistsRecord(const char* table, const char *condition)
{
	DatabaseMysql * mysql = WorldDb.GetDBConnection(0);
	QueryResult* result = mysql->PQuery(
			"select * from %s where %s",
			table, condition
			);
	if ( result )
	{
		delete result;
		return true;
	}
	return false;
}


//sql 造表
bool CDBCtrl::CreateWorldDB()
{
	std::vector<DatabaseMysql *> vecDBs;
	WorldDb.GetDBConnections(vecDBs);

	for(int i = 0; i < vecDBs.size(); ++i)
	{
		//暂时只1个库
		//create tables.....
		DatabaseMysql * mysql = vecDBs[i];

//		mysql->PExecute("use card_world;");

		if (!ExistsTable("user_login"))
		{
		////角色行为数据表
		mysql->PExecute(
				"create table user_login( "
				"user_id bigint unsigned primary key,"			//user_id
				"account varchar(64) not null default '',"						//帐号
				"last_login_time int unsigned not null default 0,"				//上次登录时间
				"last_logout_time int unsigned not null default 0,"				//上次下线时间 //挂了就错了～～
				"cur_login_time int unsigned not null default 0,"				//上次登录时间
				"login_day smallint not null default 0,"						//已连续登录几天
				"last_login_ip varchar(32) not null default '',"				//上次登录ip
				"key(account) "

				")engine=innodb DEFAULT charset=utf8;");
		}
		//这里做alter！！！！！！！

		if (!ExistsTable("user_game"))
		{
		////角色游戏数据表
		mysql->PExecute(
				"create table user_game( "
				"user_id bigint unsigned primary key,"		//user_id
				"name varchar(32) not null default 'unknow',"				//昵称
				"level smallint not null default 0,"						//等级
				"exp int unsigned not null default 0,"								//经验
				"vip_level int not null default 0,"							//vip等级
				"vip_exp int unsigned not null default 0,"					//vip经验
				"dollar int unsigned not null default 0,"					//元宝
				"coin int unsigned not null default 0,"						//金币
				"energy int unsigned not null default 0,"					//精力
				"energy_max int unsigned not null default 0,"				//精力max
				"energy_rate int unsigned not null default 0,"				//精力回复速度
				"strength int unsigned not null default 0,"					//体力
				"strength_max int unsigned not null default 0,"				//体力max
				"strength_rate int unsigned not null default 0,"			//体力回复速度
				"obj_initial_id int unsigned not null default 0,"				//物品初始ID
				"task_location1 tinyint unsigned not null default 0,"				//章节
				"task_location2 tinyint unsigned not null default 0,"				//任务
				"task_location3 tinyint unsigned not null default 0,"				//关卡
				"task_location4 tinyint unsigned not null default 0,"				//进度
				"card_capacity tinyint unsigned not null default 0,"				//卡牌凹槽容量
				"exp_limit int unsigned not null default 0, "						//经验
				"energy_count int not null default 0, "					//精力记数
				"strength_count int not null default 0, "					//体力记数
				"flag tinyint unsigned not null default 0, "				//标记开通竞技场 0:未开通 1:开通
				"group_id bigint unsigned not null default 0,"		//user_id
				"avoid_time int unsigned not null default 0, "		//避免战斗的时间
				"world_history int unsigned not null default 0, "		//江湖阅历值
				"world_broad_time int unsigned not null default 0, "		//世界广播的时间
				"game_flow int unsigned not null default 0, "			//玩家游戏流程
				"luck int unsigned not null default 0, "			//玩家幸运值
				"gm int unsigned not null default 0 "				//gm权限
				")engine=innodb DEFAULT charset=utf8;");
		}
		//这里做alter！！！！！！！
		if (!ExistsColumn("user_game", "gm"))
		{
			mysql->PExecute(
					"alter table user_game add (gm int unsigned not null default 0) "
					);
		}
		if (!ExistsColumn("user_game", "status"))
		{
			mysql->PExecute(
					"alter table user_game add (status int unsigned not null default 0) "
					);
		}
		if (!ExistsColumn("user_game", "charge_luck"))
		{
			mysql->PExecute(
					"alter table user_game add (charge_luck int unsigned not null default 0) "		//玩家充值幸运值
					);
		}
		if (!ExistsColumn("user_game", "stamina"))
		{
			mysql->PExecute(
					"alter table user_game add (stamina int unsigned not null default 0) "
					);
		}
		if (!ExistsColumn("user_game", "stamina_time"))
		{
			mysql->PExecute(
					"alter table user_game add (stamina_time int unsigned not null default 0) "
					);
		}
		if (!ExistsColumn("user_game", "sigature"))
		{
			mysql->PExecute(
					"alter table user_game add (sigature varchar(255) not null) "	//个性签名
					);
		}

		if (!ExistsColumn("user_game", "limit_role"))
		{
			mysql->PExecute("alter table user_game add (limit_role tinyint not null default 0) ");//限制玩家登录，0不限制，1限制
		}

		////角色游戏数据表blob
		if (!ExistsTable("user_game_blob"))
		{
		mysql->PExecute(
				"create table user_game_blob( "
                                "user_id bigint unsigned primary key,"						//user_id
                                "first_team varchar(511) not null default '',"					//第一阵容
                                "lineup varchar(511) not null default '',"				//套装属性
                                "equip_first_team varchar(511) not null default '',"				//第一卡牌阵容装备
                                "master_card varchar(128) not null default '',"				//师傅卡牌
                                "equip_master varchar(511) not null default '',"				//套装属性
                                "pet_card blob default '',"							//卡牌包
                                "item blob default '',"								//道具包
                                "task blob default '',"								//任务数据
                                "tujian blob default '',"							//图鉴数据
                                "tech blob default '',"							//图鉴数据
                                "pet_equip_card blob default '', "							//卡牌装备数据
                                "shop blob default '', "							//商店数据
                                "master blob default '', "							//师傅系统数据
                                "mission blob default '' "							//任务系统数据

				")engine=innodb DEFAULT charset=utf8;");
		}
		//这里做alter！！！！！！！

		if (!ExistsColumn("user_game_blob", "base_data"))
		{
			mysql->PExecute(
					"alter table user_game_blob add (base_data blob default '') "	//个性签名
					);
		}

		////道具反向索引表
		if (!ExistsTable("item_user"))
		{
		mysql->PExecute(
				"create table item_user( "
							   "item_id int unsigned not null, "
                               "user_id bigint unsigned not null, "						//user_id
                               "index (item_id) "						//user_id

				")engine=innodb DEFAULT charset=utf8;");
		}
		//这里做alter！！！！！！！
		//
		if (!ExistsTable("user_combine"))
		{
		mysql->PExecute(
				"create table user_combine( "
                               "user_id bigint unsigned not null, "						//user_id
							   "combine_index int unsigned not null, "
							   "start_time int unsigned not null, "
                               "primary key (user_id, combine_index) "						//user_id

				")engine=innodb DEFAULT charset=utf8;");
		}
		//这里做alter！！！！！！！

		if (!ExistsTable("user_email"))
		{
		mysql->PExecute(
				"create table user_email( "
                               "user_id bigint unsigned not null, "						//user_id
							   "email_id bigint unsigned not null default 1, "			//user_id
							   "sendid_user_id bigint unsigned not null, "			//receive user_id
							   "sendid_user_name varchar(32) not null default '', "			//receive user_name
							   "IsDo tinyint unsigned not null default 0 comment '0:表示未读或未执行 1:表示已读或已执行', "				//该邮件的处理
							   "type tinyint unsigned not null default 0 comment '0:表示战斗相关邮件 1:好友相关邮件 2:系统文本邮件', "				//邮件类别
							   "content varchar(512) not null default '' comment '邮件内容', "			//邮件内容
							   "attachment blob not null default '' comment '关于发送的礼物相关数据', "			//附件内容
							   "time_stamp int unsigned not null comment '邮件发送时间', "
							   "flag tinyint unsigned not null default 0 comment '0:表示未删除 1:表示已删除', "				//该邮件的处理
							   "detail_type tinyint unsigned not null default 0 comment '具体邮件类型', "				//该邮件的处理
                               "primary key (email_id,user_id), index(time_stamp) "

				")engine=innodb DEFAULT charset=utf8;");
		}
		if (!ExistsColumn("user_email", "title"))
		{
			mysql->PExecute(
					"alter table user_email add title varchar(512) not null default '' comment '邮件标题' after type"
					);
		}
		//这里做alter！！！！！！！
		if (!ExistsTable("user_friend"))
		{
		mysql->PExecute(
				"create table user_friend( "
                               "user_id bigint unsigned not null, "						//user_id
							   "friend_id bigint unsigned not null, "			//friend_user_id
                               "primary key (user_id, friend_id) "

				")engine=innodb DEFAULT charset=utf8;");
		}
		//这里做alter！！！！！！！
		//
		if (!ExistsTable("user_friend_request"))
		{
		mysql->PExecute(
				"create table user_friend_request( "
                               "user_id bigint unsigned not null, "						//user_id
							   "request_id bigint unsigned not null, "			//friend_user_id
                               "primary key (user_id, request_id) "

				")engine=innodb DEFAULT charset=utf8;");
		}
		//这里做alter！！！！！！！

		if (!ExistsTable("user_arena"))
		{
		mysql->PExecute(
				"create table user_arena( "
                               "user_id bigint unsigned not null primary key, "						//user_id
							   "prestige bigint unsigned not null default 0, "			//声望
							   "rank bigint unsigned not null default 0, "			//排名
							   "lasttime int unsigned not null default 0, "			//上一次的时间
							   "challenge_number int unsigned not null default 0 , "			//挑战数量

							   "ranked int unsigned not null default 0 , "			//已排过的名次
							   "number int unsigned not null default 0 , "			//要领取的声望
							   "index(rank) "			//上一次的时间

				")engine=innodb DEFAULT charset=utf8;");
				for (int i = 0; i <  100; i++)
				{
					mysql->PExecute(
							"insert into user_arena(user_id, rank) values "
							"(%u, %u), "
							"(%u, %u), "
							"(%u, %u), "
							"(%u, %u), "
							"(%u, %u), "
							"(%u, %u), "
							"(%u, %u), "
							"(%u, %u), "
							"(%u, %u), "
							"(%u, %u) ",
							(2000 - i * 10) << 8 | (2000 - i * 10) % 256, i * 10 + 1,
							(1999 - i * 10) << 8 | (1999 - i * 10) % 256, i * 10 + 2,
							(1998 - i * 10) << 8 | (1998 - i * 10) % 256, i * 10 + 3,
							(1997 - i * 10) << 8 | (1997 - i * 10) % 256, i * 10 + 4,
							(1996 - i * 10) << 8 | (1996 - i * 10) % 256, i * 10 + 5,
							(1995 - i * 10) << 8 | (1995 - i * 10) % 256, i * 10 + 6,
							(1994 - i * 10) << 8 | (1994 - i * 10) % 256, i * 10 + 7,
							(1993 - i * 10) << 8 | (1993 - i * 10) % 256, i * 10 + 8,
							(1992 - i * 10) << 8 | (1992 - i * 10) % 256, i * 10 + 9,
							(1991 - i * 10) << 8 | (1991 - i * 10) % 256, i * 10 + 10);
				}
		}
		//这里做alter！！！！！！！
		if (!ExistsTable("user_lose"))
		{
		mysql->PExecute(
				"create table user_lose("
                               "user_id bigint unsigned not null, "						//user_id
                               "win_user_id bigint unsigned not null, "					//win_user_id
                               "primary key (user_id, win_user_id) "

				")engine=innodb DEFAULT charset=utf8;");
		}
		//这里做alter！！！！！！！
//		mysql->PExecute(
//				"create table user_mission("
//                               "user_id bigint unsigned not null, "						//user_id
//                               "task_id int unsigned not null, "						//task_id
//                               "time int unsigned not null default 0, "					//时间
//                               "step int unsigned not null default 0, "					//步骤
//                               "primary key (user_id, task_id) "
//
//				")engine=innodb DEFAULT charset=utf8;");
		//这里做alter！！！！！！！
//		mysql->PExecute(
//				"create table user_rank("
//                               "user_id bigint unsigned not null primary key, "						//user_id
//							   "name varchar(32) not null default 'unknow', "				//昵称
//							   "level smallint not null default 0, "						//等级
//                               "attack int unsigned not null default 0, "					//攻击力最大
//                               "defence int unsigned not null default 0, "					//防御力最大
//                               "index(level), index(attack), index(defence) "					//防御力最大
//
//				")engine=innodb DEFAULT charset=utf8;");
		//这里做alter！！！！！！！
//		mysql->PExecute(
//				"create table user_timer("
//                               "user_id bigint unsigned not null, "						//user_id
//                               "timer_type bigint unsigned not null, "					//timer_type 0:副本类型
//                               "timer_id bigint unsigned not null, "					//timer_id
//                               "time int unsigned not null default 0, "					//time
//                               "primary key (user_id, timer_type, timer_id) "
//
//				")engine=innodb DEFAULT charset=utf8;");
		//这里做alter！！！！！！！

//		mysql->PExecute(
//				"create table group_info("
//                               "user_id bigint unsigned not null, "						//user_id
//                               "group_id int unsigned not null, "						//group_id
//                               "content blob default '',"							//道具包
//							   "flag tinyint unsigned not null default 0,"				//0 开启, 1 删除
//							   "create_name varchar(32) not null default '',"				//会长名称
//							   "group_name varchar(32) not null default '',"				//公会名称
//                               "credit int unsigned not null default 0, "						//group_id
//                               "level int unsigned not null default 1, "						//group_id
//                               "member blob default '',"							//道具包
//                               "primary key(user_id, group_id) "					//防御力最大
//
//				")engine=innodb DEFAULT charset=utf8;");
		//这里做alter！！！！！！！

//		mysql->PExecute(
//				"create table group_email( "
//                               "group_id bigint unsigned not null, "						//user_id
//							   "email_id bigint unsigned not null default 1, "			//user_id
//							   "IsDo tinyint unsigned not null default 0 comment '0:表示未执行 不同邮件表示不同效果:1:表示已执行', "				//该邮件的处理
//							   "type tinyint unsigned not null default 0 comment '0:系统文本邮件', "				//邮件类别
//							   "content varchar(512) not null default '' comment '邮件内容', "			//邮件内容
//							   "attachment blob not null default '' comment '关于发送的礼物相关数据', "			//附件内容
//							   "time_stamp int unsigned not null comment '邮件发送时间', "
//							   "detail_type tinyint unsigned not null default 0 comment '具体邮件类型', "				//该邮件的处理
//                               "primary key(group_id, email_id), index(time_stamp) "
//
//				")engine=innodb DEFAULT charset=utf8;");
		//这里做alter！！！！！！！
		if (!ExistsTable("user_dollar_record"))
		{
		mysql->PExecute(
				"create table user_dollar_record("
                               "user_id bigint unsigned not null, "						//user_id
                               "order_id int unsigned not null, "						//消费或充值订单号
                               "dollar int not null default 0, "					//消费或充值的钱数量
                               "time int unsigned not null default 0, "				//时间
                               "shop_id int not null default 0, "					//商品ID
                               "number int unsigned not null default 0, "				//购买的数量
                               "type int unsigned not null default 0, "				//类型 0:shop 1:开通工会 2:减CD 3:充值 4:GM 5:各种活动和奖励金钱
                               "step int unsigned not null default 0, "				//消费 总共两步 0:表示扣过钱 1:表示给过物品  充值 就一步 0:表示充过钱了
							   "primary key(user_id, order_id) "

				")engine=innodb DEFAULT charset=utf8;");
		}
		//这里做alter！！！！！！！
		if (!ExistsTable("user_lose_duration"))
		{
		mysql->PExecute(
				"create table user_lose_duration("
                               "user_id bigint unsigned not null, "						//user_id
                               "win_user_id bigint unsigned not null, "					//win_user_id
                               "win_number int unsigned not null default 1, "						//group_id
                               "primary key (user_id, win_user_id) "

				")engine=innodb DEFAULT charset=utf8;");
		}
		//这里做alter！！！！！！！
		if (!ExistsTable("announcement"))
		{
		mysql->PExecute(
				"create table announcement("
                               "id bigint unsigned not null, "						//id
                               "content blob default '' not null,"							//公告
                               "primary key (id) "

				")engine=innodb DEFAULT charset=utf8;");
		//这里做alter！！！！！！！
		mysql->PExecute(
				"insert into announcement(id) values(1);"
				);
		mysql->PExecute(
				"insert into announcement(id) values(2);"
				);
		mysql->PExecute(
				"insert into announcement(id) values(3);"
				);
		mysql->PExecute(
				"insert into announcement(id) values(4);"
				);
		mysql->PExecute(
				"insert into announcement(id) values(5);"
				);
		mysql->PExecute(
				"insert into announcement(id) values(6);"
				);
		mysql->PExecute(
				"insert into announcement(id) values(7);"
				);
		mysql->PExecute(
				"insert into announcement(id) values(8);"
				);
		mysql->PExecute(
				"insert into announcement(id) values(9);"
				);
		mysql->PExecute(
				"insert into announcement(id) values(10);"
				);
		}
		//这里做alter！！！！！！！
		if (!ExistsTable("user_game_blob2"))
		{
		mysql->PExecute(
				"create table user_game_blob2( "
                                "user_id bigint unsigned primary key,"						//user_id
                                "active blob default '', "							//礼包数据
                                "rob_robort blob default '', "							//礼包数据
                                "world_boss blob default '', "							//世界BOSS数据
                                "role_other blob default '', "							//角色的一些杂项
                                "timer_gift blob default '' "							//记时礼包数据

				")engine=innodb DEFAULT charset=utf8;");
		}
		if (!ExistsColumn("user_game_blob2", "achieve"))
		{
			mysql->PExecute(
					"alter table user_game_blob2 add (achieve blob default '') "
					);
		}
		if (!ExistsColumn("user_game_blob2", "trial"))
		{
			mysql->PExecute(
					"alter table user_game_blob2 add (trial blob default '') "
					);
		}
		if (!ExistsColumn("user_game_blob2", "multiple_card_exp")) {
			mysql->PExecute(
					"alter table user_game_blob2 add (multiple_card_exp blob default '') ");
		}
		//这里做alter！！！！！！！
		if (!ExistsTable("user_game_active_blob"))
		{
		mysql->PExecute(
				"create table user_game_active_blob( "
                                "user_id bigint unsigned not null,"						//user_id
                                "active_id bigint unsigned not null, "							//活动ID
                                "active blob default '', "							//礼包数据
                                "primary key (user_id, active_id) "

				")engine=innodb DEFAULT charset=utf8;");
		}

		if (!ExistsTable("active_global"))
		{
		mysql->PExecute(
				"create table active_global( "
                                "active_id bigint unsigned primary key,"						//user_id
                                "content BLOB default '' "							//礼包数据

				")engine=innodb DEFAULT charset=utf8;");
		}

		if (!ExistsTable("store"))
		{
		mysql->PExecute(
				"create table store( "
                                "server_id int unsigned not null,"						//server_id
                                "store_id int unsigned not null, "						//商品ID
                                "store_type int unsigned not null, "						//商城类别
                                "goods_id int unsigned not null, "						//物品ID
                                "view_type int unsigned not null, "						//显示类别
                                "consume_type int unsigned not null, "						//消耗货币类别
                                "consume_number int unsigned not null, "						//消耗货币数量
                                "is_buy int unsigned not null, "						//是否上架
                                "view_order int unsigned not null, "						//显示顺序
                                "content varchar(512) not null, "						//描述
                                "limit_number int unsigned not null, "						//当天购买限制
                                "role_limit int unsigned not null, "						//终身购买限制
                                "store_number int unsigned not null, "						//售卖数量(充值有效)
                                "gift_number int unsigned not null, "						//赠送数量(充值有效)
                                "view_number int unsigned not null, "						//显示数量(充值有效)
                                "max_number int unsigned not null, "						//最大购买数量
                                "channel int unsigned not null, "						//商店渠道
                               	"primary key(server_id, store_id), index(server_id) "
				")engine=innodb DEFAULT charset=utf8;");
		}
		if (!ExistsColumn("store", "goods_type"))
		{
			mysql->PExecute(
					"alter table store add (goods_type int unsigned not null) "
					);
		}
		if (!ExistsTable("global_data"))
		{
			mysql->PExecute(
					"create table global_data( "
					"id bigint unsigned primary key,"						//user_id
					"content BLOB default '' "							//礼包数据

					")engine=innodb DEFAULT charset=utf8;");
			mysql->PExecute(
					"insert into global_data(id,content) values(1, '');"
					);
			mysql->PExecute(
					"insert into global_data(id,content) values(2, '');"
					);
			mysql->PExecute(
					"insert into global_data(id,content) values(3, '');"
					);
			mysql->PExecute(
					"insert into global_data(id,content) values(4, '');"
					);
		}
		if (!ExistsRecord("global_data", "id = 99"))
		{
			mysql->PExecute(
					"insert into global_data(id,content) values(99, '');"
					);
		}

		if(!ExistsTable("rank")){
			mysql->PExecute("CREATE TABLE rank ("
					"user_id int unsigned NOT NULL,"
					"user_name VARCHAR(32) DEFAULT '',"
					"layer int unsigned NOT NULL DEFAULT 0,"
					"break_blockade_times int unsigned NULL DEFAULT 0,"//闯关次数
					"user_level int unsigned NULL DEFAULT 0,"//用户等级
					"ability int unsigned NULL DEFAULT 0," //战力
					"master_index int unsigned NULL DEFAULT 0,"//上榜次数
					"PRIMARY KEY (user_id))engine=innodb DEFAULT charset=utf8;");
//					//初始化20个机器人
//			std::map< uint32, uint32> *layerMap = CConfRobRobot::GetRobotLayerMap();
//			for(std::map< uint32, uint32>::iterator it = layerMap->begin(); it != layerMap->end(); it ++){
//				uint32 userId = it->first;
////				uint32 layer = it->second;
//				STC_CONF_ROB_ROBOT *robot =  CConfRobRobot::Find(userId);
//				std::string name = robot->strName;
//				mysql->escape_string(name);
//				mysql->PExecute("insert into rank(user_id,user_name,layer,break_blockade_times,user_level,ability,master_index)values(%u,'%s',0,0,%u,0,%)",
//						userId,name.c_str(),robot->wLevel,robot->dwMasterCardIndex);
//			}
////
//			mysql->PExecute("insert into rank(user_id,user_name,layer,break_blockade_times,user_level,ability,master_index)values(256489,'%保荣%',0,0,5,1234,60001)");
//			mysql->PExecute("insert into rank(user_id,user_name,layer,break_blockade_times,user_level,ability,master_index)values(264457,'︶贼ヽ婆◆',0,0,6,1500,60005)");
//			mysql->PExecute("insert into rank(user_id,user_name,layer,break_blockade_times,user_level,ability,master_index)values(337446,'北京大贝勒',0,04,7,1860,60104)");
//			mysql->PExecute("insert into rank(user_id,user_name,layer,break_blockade_times,user_level,ability,master_index)values(337960,'北里五号',0,0,9,2296,60106)");
//			mysql->PExecute("insert into rank(user_id,user_name,layer,break_blockade_times,user_level,ability,master_index)values(338217,'北脸',0,0,14,3003,60107)");
//			mysql->PExecute("insert into rank(user_id,user_name,layer,break_blockade_times,user_level,ability,master_index)values(342586,'贝利亚',0,0,12,2705,60002)");
//			mysql->PExecute("insert into rank(user_id,user_name,layer,break_blockade_times,user_level,ability,master_index)values(342843,'贝莉卡',0,0,11,2450,60003)");
//			mysql->PExecute("insert into rank(user_id,user_name,layer,break_blockade_times,user_level,ability,master_index)values(346955,'奔驰',0,0,13,2597,60004)");
//			mysql->PExecute("insert into rank(user_id,user_name,layer,break_blockade_times,user_level,ability,master_index)values(351581,'笨笨熊',0,0,15,1805,60006)");
//			mysql->PExecute("insert into rank(user_id,user_name,layer,break_blockade_times,user_level,ability,master_index)values(373940,'彬琳天下',0,0,14,1507,60005)");
//			mysql->PExecute("insert into rank(user_id,user_name,layer,break_blockade_times,user_level,ability,master_index)values(379851,'冰岛爆米花',0,0,16,2580,60101)");
//			mysql->PExecute("insert into rank(user_id,user_name,layer,break_blockade_times,user_level,ability,master_index)values(398870,'菠萝菠萝蜜',0,0,14,1409,60104)");
//			mysql->PExecute("insert into rank(user_id,user_name,layer,break_blockade_times,user_level,ability,master_index)values(447700,'苍穹剑影',0,0,26,1508,60102)");
//			mysql->PExecute("insert into rank(user_id,user_name,layer,break_blockade_times,user_level,ability,master_index)values(461065,'曾几何时',0,0,18,3007,60106)");
//			mysql->PExecute("insert into rank(user_id,user_name,layer,break_blockade_times,user_level,ability,master_index)values(489849,'沉睡的狮子',0,0,19,3270,60201)");
//			mysql->PExecute("insert into rank(user_id,user_name,layer,break_blockade_times,user_level,ability,master_index)values(493190,'陈方怡',0,0,19,3090,60001)");
//			mysql->PExecute("insert into rank(user_id,user_name,layer,break_blockade_times,user_level,ability,master_index)values(497599,'陈龙',0,0,22,3570,60002)");
//			mysql->PExecute("insert into rank(user_id,user_name,layer,break_blockade_times,user_level,ability,master_index)values(506554,'宸熙',0,0,21,1234,60003)");
//			mysql->PExecute("insert into rank(user_id,user_name,layer,break_blockade_times,user_level,ability,master_index)values(510409,'晨光曦微',0,0,20,3470,60004)");
//			mysql->PExecute("insert into rank(user_id,user_name,layer,break_blockade_times,user_level,ability,master_index)values(512208,'称霸旮旯',0,0,23,3370,60006)");

					//初始化所有已存在的用户
			//mysql->PExecute("insert into rank (user_id,user_name,user_level) select user_id,name,level from user_game");


		}
		if(!ExistsTable("tower")){
			mysql->PExecute("CREATE TABLE tower ("
					"user_id int unsigned NOT NULL,"
					"dw_index int unsigned NOT NULL DEFAULT 0,"
					"three_attack int unsigned NULL DEFAULT 0, "//COMMENT '1：购买3倍攻击\n0：未购买',
					"two_attack int unsigned NULL DEFAULT 0,"//COMMENT '1：购买2倍攻击\n0：未购买',
					"refresh_attribute int unsigned NULL DEFAULT 0," //COMMENT '0：没有\n1：刷新属性',
					"attribute varchar(32) NULL DEFAULT '0,0,0,0,0', "//COMMENT '剑系特效、掌系特效、内系特效、暗系特效、棍系特效',
					"break_blockade_times int unsigned NULL DEFAULT 0,"
					"buy_times int unsigned NULL DEFAULT 0,"
					"health int unsigned NULL DEFAULT 0,"
					"is_max_layer int unsigned NULL DEFAULT 0,"
					"PRIMARY KEY (user_id))engine=innodb DEFAULT charset=utf8;");
			mysql->PExecute("insert into tower(user_id) (select user_id from user_game)");
		}
		if (!ExistsColumn("tower", "timestamp")) {
			mysql->PExecute("alter table tower add (timestamp int unsigned not null DEFAULT 0) ");
		}

	}

	return true;
}

bool CDBCtrl::IsNewRole(uint32_t accountId)
{
	DatabaseMysql * mysql = WorldDb.GetDBConnection(0);
//	std::string strAccount = strAccountRet;
//	mysql->escape_string(strAccount);
	QueryResult *result = mysql->PQuery(
			"select account "
			"from user_login where user_id = '%u'",
			accountId
			);
	if (result)
	{
		delete result;
		return false;
	}

	return true;
}

//创建玩家基本数据
uint32 CDBCtrl::NewUserData(uint32 accountId, std::string & strAccountRet)
{
	uint32 dwAccountId = 0;
	DatabaseMysql * mysql = WorldDb.GetDBConnection(dwAccountId);//我们只用一个库
	assert(mysql);

	std::string strAccount = strAccountRet;
	mysql->escape_string(strAccount);

	mysql->PExecute(
			"insert into user_login "
			"(user_id, account) "
			"values (%u, '%s')",
			accountId,
			strAccount.c_str()
			//todo
	);

	dwAccountId = accountId;

	mysql->PExecute(
		"insert into user_game (user_id, name) values(%u, '%u')",
		dwAccountId, dwAccountId
		);
	mysql->PExecute(
		"insert into user_game_blob (user_id) values(%u)",
		dwAccountId
		);
	mysql->PExecute(
		"insert into user_game_blob2 (user_id) values(%u)",
		dwAccountId
		);
	mysql->PExecute("insert into tower(user_id) values(%u)",dwAccountId);
	mysql->PExecute("insert into rank (user_id,user_name,user_level) select user_id,name,level from user_game where user_id = %u",dwAccountId);
	//todo need log?

	return dwAccountId;
}

bool CDBCtrl::GetUserLoginData(
		uint32 accountId,
		uint32 &last_login_time,
		uint32 &last_logout_time,
		uint32 &cur_login_time,
		uint32 &login_day,
		std::string &last_login_ip
)
{
	DatabaseMysql * mysql = WorldDb.GetDBConnection(accountId);
	QueryResult *result = mysql->PQuery(
			"select last_login_time,last_logout_time,cur_login_time,login_day,last_login_ip "
			"from user_login where user_id = %u",
			accountId
			);
	if (result)
	{
		int i = 0;
		Field *fields = result->Fetch();

		last_login_time = fields[i++].GetUInt32();
		last_logout_time = fields[i++].GetUInt32();
		cur_login_time = fields[i++].GetUInt32();
		login_day = fields[i++].GetUInt32();
		last_login_ip = fields[i++].GetCppString();

		delete result;
		return true;
	}
	return false;
}

void CDBCtrl::SaveUserLoginData(
		uint32 accountId,
		uint32 last_login_time,
		uint32 last_logout_time,
		uint32 cur_login_time,
		uint32 login_day,
		std::string last_login_ip
		)
{
	DatabaseMysql * mysql = WorldDb.GetDBConnection(accountId);
	std::string str = last_login_ip;
	mysql->escape_string(str);
	mysql->PExecute(
			"update user_login set "
			"last_login_time = %u,last_logout_time = %u,cur_login_time = %u,login_day = %u,last_login_ip = '%s' "
			"where user_id=%u",
			last_login_time,last_logout_time,cur_login_time,login_day,str.c_str(),
			accountId
			);

}
//uint32 CDBCtrl::GetUserId(std::string& strAccountRet)
//{
//	DatabaseMysql * mysql = WorldDb.GetDBConnection(0);//我们只用一个库
//	assert(mysql);
//
//	std::string strAccount = strAccountRet;
//	mysql->escape_string(strAccount);
//
//	QueryResult* retChar = mysql->PQuery(
//		"select user_id from user_login where account = '%s'",
//		strAccount.c_str()
//		);
//	if (!retChar)
//	{
//		return 0;
//	}
//	Field *fieldsChar = retChar->Fetch();
//	uint32 dwUserId = fieldsChar[0].GetUInt32();
//	delete retChar;
//
//	return dwUserId;
//}


bool CDBCtrl::GetUserBaseData(
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
		uint32 dwAccountId,
		uint32 &ObjInitialId,
		uint8 &bytask1,
		uint8 &bytask2,
		uint8 &bytask3,
		uint8 &bytask4,
		uint8 &byCardCap,
		uint32 &dwExplimit,
		int32 &dwEnergyCnt,
		int32 &dwStrengthCnt,
		uint8 &byFlag,
		uint64 &lFindGroupId,
		uint32 &dwAvoidTime,
		uint32 &dwWorldHistory,
		uint32 &dwWorldBroadTime,
		uint32 &dwGameFlow,
		uint32 &dwLuck,
		uint8 &byGm,
		uint32 &dwChargeLuck,
		uint32 &dwStamina,
		uint32 &dwStaminaTime,
		std::string &strSignature
		)
{
	DatabaseMysql * mysql = WorldDb.GetDBConnection(dwAccountId);
	QueryResult *result = mysql->PQuery(
			"select name,level,exp,vip_level,vip_exp,dollar,"
			"coin,energy,energy_max,energy_rate,strength,strength_max,strength_rate,"
			"obj_initial_id,task_location1,task_location2,task_location3,task_location4,card_capacity, "
            "exp_limit,energy_count,strength_count,flag,group_id,avoid_time,world_history,world_broad_time,game_flow,luck,gm,charge_luck,stamina,stamina_time,sigature "
			"from user_game where user_id = %u",
			dwAccountId
			);
	if (result)
	{
		int i = 0;
		Field *fields = result->Fetch();

		strUserName = fields[i++].GetString();
		wLevel = fields[i++].GetUInt16();
		dwExp = fields[i++].GetUInt32();
		wVipLv = fields[i++].GetUInt16();
		dwVipExp = fields[i++].GetUInt32();
		dwDollar = fields[i++].GetUInt32();
		dwCoin = fields[i++].GetUInt32();
		dwEnergy = fields[i++].GetUInt32();
		dwEnergyMax = fields[i++].GetUInt32();
		dwEnergyRate = fields[i++].GetUInt32();
		dwStrength = fields[i++].GetUInt32();
		dwStrengthMax = fields[i++].GetUInt32();
		dwStrengthRate = fields[i++].GetUInt32();
		ObjInitialId = fields[i++].GetUInt32();
		bytask1 = fields[i++].GetUInt8();
		bytask2 = fields[i++].GetUInt8();
		bytask3 = fields[i++].GetUInt8();
		bytask4 = fields[i++].GetUInt8();
		byCardCap = fields[i++].GetUInt8();
		dwExplimit = fields[i++].GetUInt32();
		dwEnergyCnt = fields[i++].GetInt32();
		dwStrengthCnt = fields[i++].GetInt32();
		byFlag = fields[i++].GetUInt8();
		lFindGroupId = fields[i++].GetUInt64();
		dwAvoidTime = fields[i++].GetUInt32();
		dwWorldHistory = fields[i++].GetUInt32();
		dwWorldBroadTime = fields[i++].GetUInt32();
		dwGameFlow = fields[i++].GetUInt32();
		dwLuck = fields[i++].GetUInt32();
		byGm = fields[i++].GetUInt8();
		dwChargeLuck = fields[i++].GetUInt32();
		dwStamina = fields[i++].GetUInt32();
		dwStaminaTime = fields[i++].GetUInt32();
		strSignature = fields[i++].GetCppString();

		delete result;
		return true;
	}
	return false;
}

bool CDBCtrl::GetUserBaseDataForDurationPreview(
		std::string & strUserName,
		uint32 &dwCoin,
		uint16 &wLevel,
		uint32 &dwAccountId
		)
{
	DatabaseMysql * mysql = WorldDb.GetDBConnection(0);
	QueryResult *result = mysql->PQuery(
			"select name,coin,level "
			"from user_game where user_id = %u",
			dwAccountId
			);
	if (result)
	{
		int i = 0;
		Field *fields = result->Fetch();

		strUserName = fields[i++].GetString();
		dwCoin = fields[i++].GetUInt32();
		wLevel = fields[i++].GetUInt16();

		delete result;
		return true;
	}
	return false;
}

bool CDBCtrl::GetUserBaseDataAuto10(const uint32 &dwAccountId, const uint16 &wLevel, std::vector<stuPlayer> &vecPlayer)
{
	int maxlevel = wLevel + 10;
	int minlevel = wLevel - 10;
	DatabaseMysql * mysql = WorldDb.GetDBConnection(0);
	QueryResult *result = mysql->PQuery(
			"select user_id,name,level "
			"from user_game where level >= %d and level <= %d and user_id != %u and level != 0 limit 5",
			minlevel, maxlevel, dwAccountId
			);
	IME_DEBUG("minlevel %d maxlevel %d user_id %d", minlevel, maxlevel, dwAccountId);

	if (result)
	{
		int i = 0;
		int j = 0;
		int number = result->GetRowCount();
		Field *fields = result->Fetch();
		vecPlayer.clear();
		stuPlayer tmpPlayer;
		for (; j < number; j++)
		{
			i = 0;
			tmpPlayer.dwUserid = fields[i++].GetUInt32();
			tmpPlayer.strName = fields[i++].GetString();
			tmpPlayer.wLevel = fields[i++].GetUInt16();
			vecPlayer.push_back(tmpPlayer);
			result->NextRow();
			fields = result->Fetch();
		}

		delete result;
		return true;
	}
	return false;
}

bool CDBCtrl::GetUserBaseDataByName(const uint32 &dwAccountId, const std::string &strUserName, std::vector<stuPlayer> &vecPlayer)
{

	DatabaseMysql * mysql = WorldDb.GetDBConnection(0);
	std::string name = strUserName;
	mysql->escape_string(name);
	QueryResult *result = mysql->PQuery(
			"select user_id,name,level "
			"from user_game where name = '%s' and user_id != %u and level != 0 limit 10",
			name.c_str(), dwAccountId
			);
	if (result)
	{
		int i = 0;
		int j = 0;
		int number = result->GetRowCount();
		Field *fields = result->Fetch();
		vecPlayer.clear();
		stuPlayer tmpPlayer;
		for (; j < number; j++)
		{
			i = 0;
			tmpPlayer.dwUserid = fields[i++].GetUInt32();
			tmpPlayer.strName = fields[i++].GetString();
			tmpPlayer.wLevel = fields[i++].GetUInt16();
			vecPlayer.push_back(tmpPlayer);
			result->NextRow();
			fields = result->Fetch();
		}

		delete result;
		return true;
	}
	return false;
}

bool CDBCtrl::GetUserBaseDataByNameId(const uint32 &dwAccountId, const std::string &strUserName, std::vector<stuPlayer> &vecPlayer, const uint32 &userid)
{

	DatabaseMysql * mysql = WorldDb.GetDBConnection(0);
	std::string name = strUserName;
	mysql->escape_string(name);
	QueryResult *result = mysql->PQuery(
			"select user_id,name,level "
			"from user_game where (name = '%s' or user_id = %u) and user_id != %u and level != 0 limit 10",
			name.c_str(), dwAccountId, userid
			);
	if (result)
	{
		int i = 0;
		int j = 0;
		int number = result->GetRowCount();
		Field *fields = result->Fetch();
		vecPlayer.clear();
		stuPlayer tmpPlayer;
		for (; j < number; j++)
		{
			i = 0;
			tmpPlayer.dwUserid = fields[i++].GetUInt32();
			tmpPlayer.strName = fields[i++].GetString();
			tmpPlayer.wLevel = fields[i++].GetUInt16();
			vecPlayer.push_back(tmpPlayer);
			result->NextRow();
			fields = result->Fetch();
		}

		delete result;
		return true;
	}
	return false;
}

bool CDBCtrl::GetUserBaseDataById(const uint32 &dwAccountId, std::vector<stuPlayer> &vecPlayer)
{

	DatabaseMysql * mysql = WorldDb.GetDBConnection(0);
	QueryResult *result = mysql->PQuery(
			"select user_id,name,level "
			"from user_game where user_id = %u and level != 0",
			dwAccountId
			);
	if (result)
	{
		int i = 0;
		int j = 0;
		int number = result->GetRowCount();
		Field *fields = result->Fetch();
		vecPlayer.clear();
		stuPlayer tmpPlayer;
		for (; j < number; j++)
		{
			i = 0;
			tmpPlayer.dwUserid = fields[i++].GetUInt32();
			tmpPlayer.strName = fields[i++].GetString();
			tmpPlayer.wLevel = fields[i++].GetUInt16();
			vecPlayer.push_back(tmpPlayer);
			result->NextRow();
			fields = result->Fetch();
		}

		delete result;
		return true;
	}
	return false;
}

void CDBCtrl::SaveUserBaseData(
//		uint16 wHeadPic,
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
//		uint32 dwCardAttributes,
//		uint32 dwAttackMin,
//		uint32 dwAttackMax,
//		uint32 dwDefenceMin,
//		uint32 dwDefenceMax,
//		uint32 dwAttackWin,
//		uint32 dwAttackTot,
//		uint32 dwDefenceWin,
//		uint32 dwDefenceTot,
		uint32 dwAccountId,
		uint32 ObjInitialId,
		uint8 bytask1,
		uint8 bytask2,
		uint8 bytask3,
		uint8 bytask4,
		uint8 byCardCap,
//		uint32 dwCardAttributes2,
//		uint8 byCurrentTeam,
		uint32 dwExplimit,
		int32 dwEnergyCnt,
		int32 dwStrengthCnt,
		uint8 byFlag,
		uint64 lFindGroupId,
		uint32 dwAvoidTime,
		uint32 dwWorldHistory,
		uint32 dwWorldBroadTime,
		uint32 dwGameFlow,
		uint32 dwLuck,
		uint8 byGm,
		uint32 dwChargeLuck,
		uint32 dwStamina,
		uint32 dwStaminaTime,
		std::string strSignature
		)
{
	DatabaseMysql * mysql = WorldDb.GetDBConnection(dwAccountId);
	std::string strNameRet= strUserName;
	mysql->escape_string(strNameRet);
	mysql->PExecute(
			"update user_game set "
			"name = '%s',level = %u,exp = %u,vip_level = %u,vip_exp = %u,dollar = %u,"
			"coin = %u,energy = %u,energy_max = %u,energy_rate = %u,strength = %u,strength_max = %u,strength_rate = %u,"
			"obj_initial_id = %u, task_location1 = %u, task_location2 = %u, task_location3 = %u, task_location4 = %u, card_capacity = %u,"
			"exp_limit = %u, energy_count = %u, strength_count = %u, flag = %u, group_id = %llu, avoid_time = %u, world_history = %u, world_broad_time = %u, game_flow = %u, luck = %u, gm = %u, charge_luck = %u, stamina = %u, stamina_time = %u, sigature = '%s'	"
			"where user_id=%u",
			strNameRet.c_str(),wLevel,dwExp,wVipLv,dwVipExp,dwDollar,dwCoin,dwEnergy,
			dwEnergyMax,dwEnergyRate,dwStrength,dwStrengthMax,dwStrengthRate,
			ObjInitialId, bytask1, bytask2, bytask3, bytask4, byCardCap,
			dwExplimit, dwEnergyCnt, dwStrengthCnt, byFlag, lFindGroupId, dwAvoidTime, dwWorldHistory, dwWorldBroadTime, dwGameFlow, dwLuck, byGm, dwChargeLuck, dwStamina, dwStaminaTime, strSignature.c_str(),
			dwAccountId
			);
}

bool CDBCtrl::GetUserCardPacketData(std::string &str1team, std::string &strsuit, std::string &cards, const uint32 &dwAccountId, std::string &equip_first, std::string &master, std::string &equip_master)
{
	DatabaseMysql * mysql = WorldDb.GetDBConnection(dwAccountId);
	QueryResult *result = mysql->PQuery(
			"select first_team,lineup,pet_card,equip_first_team,master_card,equip_master "
			"from user_game_blob where user_id = %u",
			dwAccountId
			);
	if (result)
	{
		int i = 0;
		Field *fields = result->Fetch();

                str1team = fields[i++].GetCppString();
                strsuit = fields[i++].GetCppString();
                cards.assign(fields[i].GetString(), fields[i].GetLength());
				i++;
                equip_first.assign(fields[i].GetString(), fields[i].GetLength());
				i++;
                master.assign(fields[i].GetString(), fields[i].GetLength());
				i++;
				equip_master.assign(fields[i].GetString(), fields[i].GetLength());

		delete result;
		return true;
	}
	return false;
}

void CDBCtrl::SaveUserCardPacketData(const std::string &cstr1team, const std::string &cstrsuit, const std::string &ccards, const uint32 &dwAccountId, const std::string &equip_first, const std::string &equipcards, const std::string &master, const std::string &equip_master, std::string base)
{
	DatabaseMysql * mysql = WorldDb.GetDBConnection(dwAccountId);

        std::string str1team = cstr1team;
        std::string strsuit = cstrsuit;
        std::string cards = ccards;
		std::string equip1 = equip_first;
		std::string equipcard = equipcards;
		std::string masterstr = master;
		std::string masterequip = equip_master;
//		IME_LOG("str %s, str %s", equip_first.c_str(), equip1.c_str());
        mysql->escape_string(str1team);
        mysql->escape_string(strsuit);
        mysql->escape_string(cards);
        mysql->escape_string(equip1);
        mysql->escape_string(equipcard);
		mysql->escape_string(masterstr);
		mysql->escape_string(masterequip);
		mysql->escape_string(base);
	mysql->PExecute(
		"update user_game_blob set "
		"first_team = '%s', lineup = '%s', pet_card = '%s', "
		"equip_first_team = '%s', pet_equip_card = '%s', master_card = '%s', "
		"equip_master = '%s', base_data = '%s' "
		"where user_id=%u",
                str1team.c_str(), strsuit.c_str(), cards.c_str(),
				equip1.c_str(), equipcard.c_str(), masterstr.c_str(),
				masterequip.c_str(), base.c_str(),
		dwAccountId
		);
}

bool CDBCtrl::GetUserItemPacketData(std::string &item, const uint32 &dwAccountId)
{
	DatabaseMysql * mysql = WorldDb.GetDBConnection(dwAccountId);
	QueryResult *result = mysql->PQuery(
			"select item "
			"from user_game_blob where user_id = %u",
			dwAccountId
			);
	if (result)
	{
		int i = 0;
		Field *fields = result->Fetch();
		item.assign(fields[0].GetString(), fields[0].GetLength());
		delete result;
		return true;
	}
	return false;
}

void CDBCtrl::SaveUserItemPacketData(const std::string &item, const uint32 &dwAccountId)
{
	DatabaseMysql * mysql = WorldDb.GetDBConnection(dwAccountId);

	std::string stritem = item;
	mysql->escape_string(stritem);
	mysql->PExecute(
			"update user_game_blob set "
			"item = '%s' "
			"where user_id=%u",
			stritem.c_str(),
			dwAccountId
			);
}

bool CDBCtrl::InsertItemUser(uint32 item, uint32 userid)
{
	uint32 dwAccountId = 0;
	DatabaseMysql * mysql = WorldDb.GetDBConnection(dwAccountId);//我们只用一个库
	assert(mysql);

	if (mysql->PExecute(
			"insert into item_user "
			"(item_id, user_id) "
			"values (%u, %u)",
			item, userid
			//todo
	))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool CDBCtrl::DeleteItemUser(uint32 item, uint32 userid)
{
	uint32 dwAccountId = 0;
	DatabaseMysql * mysql = WorldDb.GetDBConnection(dwAccountId);//我们只用一个库
	assert(mysql);

	if (mysql->PExecute(
			"delete from  item_user "
			"where item_id = %u and user_id = %u",
			item, userid
			//todo
	))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool CDBCtrl::GetUserbyItem(const uint32 &dwAccountId, const uint32 &dwItemId, const uint16 &wLevel, std::vector<stuPlayer> &vecPlayer)
{
	int maxlevel = wLevel + 10;
	int minlevel = wLevel - 10;
	DatabaseMysql * mysql = WorldDb.GetDBConnection(0);
	QueryResult *result = mysql->PQuery(
			"select user_id,name,level "
			"from user_game where level >= %d and level <= %d and user_id != %u and user_id in "
			"(select user_id from item_user where item_id = %u) limit 30",
			minlevel, maxlevel, dwAccountId, dwItemId
			);
	IME_DEBUG("minlevel %d maxlevel %d user_id %d, item_id %u", minlevel, maxlevel, dwAccountId, dwItemId);

	if (result)
	{
		int i = 0;
		int j = 0;
		int number = result->GetRowCount();
		Field *fields = result->Fetch();
		vecPlayer.clear();
		stuPlayer tmpPlayer;
		for (; j < number; j++)
		{
			i = 0;
			tmpPlayer.dwUserid = fields[i++].GetUInt32();
			tmpPlayer.strName = fields[i++].GetString();
			tmpPlayer.wLevel = fields[i++].GetUInt16();
			vecPlayer.push_back(tmpPlayer);
			result->NextRow();
			fields = result->Fetch();
		}

		delete result;
		return true;
	}
	return false;
}

bool CDBCtrl::GetItemCombine(uint32 userid, uint32 comindex, uint32 &timestamp)
{
	DatabaseMysql * mysql = WorldDb.GetDBConnection(0);
	QueryResult *result = mysql->PQuery(
			"select start_time  "
			"from user_combine where user_id = %u and combine_index = %u",
			userid, comindex
			);
	if (result)
	{
		int i = 0;
		Field *fields = result->Fetch();

		timestamp = fields[i++].GetUInt32();

		delete result;
		return true;
	}
	return false;
}

bool CDBCtrl::InsertItemCombine(uint32 userid, uint32 comindex, uint32 timestamp)
{
	uint32 dwAccountId = 0;
	DatabaseMysql * mysql = WorldDb.GetDBConnection(dwAccountId);//我们只用一个库
	assert(mysql);

	if (mysql->PExecute(
			"insert into user_combine "
			"(user_id, combine_index, start_time) "
			"values (%u, %u, %u) ",
			userid, comindex, timestamp
			//todo
	))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool CDBCtrl::GetMultiItemCombine(uint32 userid, std::vector<sCombRecord> &comrecord)
{
	DatabaseMysql * mysql = WorldDb.GetDBConnection(0);
	QueryResult *result = mysql->PQuery(
			"select combine_index, start_time  "
			"from user_combine where user_id = %u",
			userid
			);

	if (result)
	{
		int i = 0;
		int j = 0;
		int number = result->GetRowCount();
		Field *fields = result->Fetch();
		comrecord.clear();
		sCombRecord tmp;
		for (; j < number; j++)
		{
			i = 0;
			tmp.ItemIndex = fields[i++].GetUInt32();
			tmp.Timestamp = fields[i++].GetUInt32();
			comrecord.push_back(tmp);
			result->NextRow();
			fields = result->Fetch();
		}

		delete result;
		return true;
	}
	return false;
}

bool CDBCtrl::DelItemCombine(uint32 userid, uint32 comindex)
{
	uint32 dwAccountId = 0;
	DatabaseMysql * mysql = WorldDb.GetDBConnection(dwAccountId);//我们只用一个库
	assert(mysql);

	if (mysql->PExecute(
			"delete from user_combine "
			"where combine_index = %u and user_id = %u",
			comindex, userid
			//todo
	))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool CDBCtrl::GetUserCardTujianData(std::string &strtujian, uint32 dwAccountId)
{
	DatabaseMysql * mysql = WorldDb.GetDBConnection(dwAccountId);
	QueryResult *result = mysql->PQuery(
			"select tujian "
			"from user_game_blob where user_id = %u",
			dwAccountId
			);
	if (result)
	{
		int i = 0;
		Field *fields = result->Fetch();

		strtujian.assign(fields[0].GetString(), fields[0].GetLength());

		delete result;
		return true;
	}
	return false;
}

void CDBCtrl::SaveUserCardTujianData(const std::string &strtujian, uint32 dwAccountId)
{
	DatabaseMysql * mysql = WorldDb.GetDBConnection(dwAccountId);

        std::string str = strtujian;
        mysql->escape_string(str);
	mysql->PExecute(
		"update user_game_blob set "
		"tujian = '%s' "
		"where user_id=%u",
		str.c_str(),
		dwAccountId
		);
}

bool CDBCtrl::GetUserTechData(std::string &strtech, uint32 dwAccountId)
{
	DatabaseMysql * mysql = WorldDb.GetDBConnection(dwAccountId);
	QueryResult *result = mysql->PQuery(
			"select tech "
			"from user_game_blob where user_id = %u",
			dwAccountId
			);
	if (result)
	{
		int i = 0;
		Field *fields = result->Fetch();
		strtech.assign(fields[0].GetString(), fields[0].GetLength());
		delete result;
		return true;
	}
	return false;
}

void CDBCtrl::SaveUserTechData(const std::string &strtech, uint32 dwAccountId)
{
	DatabaseMysql * mysql = WorldDb.GetDBConnection(dwAccountId);

	std::string str = strtech;
	mysql->escape_string(str);
	mysql->PExecute(
			"update user_game_blob set "
			"tech = '%s' "
			"where user_id=%u",
			str.c_str(),
			dwAccountId
			);
}

bool CDBCtrl::GetUserBaseDataAuto10InFriend(uint32 dwAccountId, uint16_t off, std::vector<stuPlayerPlus> &vecPlayer)
{
	DatabaseMysql * mysql = WorldDb.GetDBConnection(0);
	QueryResult *result = mysql->PQuery(
			"select user_id,name,level "
			"from user_game where user_id in (select friend_id from user_friend where user_id = %u) and level != 0 "
		    "order by level desc limit %u,15",
			dwAccountId, off
			);
//	IME_LOG("user_id %d off %u", dwAccountId, off);

	if (result)
	{
		int i = 0;
		int j = 0;
		int number = result->GetRowCount();
		Field *fields = result->Fetch();
		vecPlayer.clear();
		stuPlayerPlus tmpPlayer;
		for (; j < number; j++)
		{
			i = 0;
			tmpPlayer.dwUserid = fields[i++].GetUInt32();
			tmpPlayer.strName = fields[i++].GetString();
			tmpPlayer.wLevel = fields[i++].GetUInt16();
			vecPlayer.push_back(tmpPlayer);
			result->NextRow();
			fields = result->Fetch();
		}

		delete result;
		return true;
	}
	return false;
}

bool CDBCtrl::GetUserBaseDataInFriend(uint32 dwAccountId, std::vector<stuPlayerPlus> &vecPlayer)
{
	DatabaseMysql * mysql = WorldDb.GetDBConnection(0);
	QueryResult *result = mysql->PQuery(
			"select user_id,name,level "
			"from user_game where user_id in (select friend_id from user_friend where user_id = %u) and level != 0 "
		    "order by level desc limit %u",
			dwAccountId, MAX_FRIEND_NUMBER
			);
//	IME_LOG("user_id %d", dwAccountId);

	if (result)
	{
		int i = 0;
		int j = 0;
		int number = result->GetRowCount();
		Field *fields = result->Fetch();
		vecPlayer.clear();
		stuPlayerPlus tmpPlayer;
		for (; j < number; j++)
		{
			i = 0;
			tmpPlayer.dwUserid = fields[i++].GetUInt32();
			tmpPlayer.strName = fields[i++].GetString();
			tmpPlayer.wLevel = fields[i++].GetUInt16();
			vecPlayer.push_back(tmpPlayer);
			result->NextRow();
			fields = result->Fetch();
		}

		delete result;
		return true;
	}
	return false;
}

bool CDBCtrl::GetUserBaseDataAuto10ForFriend(uint32 dwAccountId, const uint16 &wLevel, std::vector<stuPlayer> &vecPlayer)
{
	int maxlevel = wLevel + 5;
	int minlevel = wLevel - 5;
	DatabaseMysql * mysql = WorldDb.GetDBConnection(0);
	QueryResult *result = mysql->PQuery(
			"select user_id,name,level "
			"from user_game where user_id not in (select friend_id from user_friend where user_id = %u) and user_id != %u and level != 0 and level >= %d and level <= %d limit 500",
			dwAccountId, dwAccountId, minlevel, maxlevel
			);
//	IME_LOG("minlevel %d maxlevel %d user_id %d", minlevel, maxlevel, dwAccountId);

	if (result)
	{
		int i = 0;
		int j = 0;
		int number = result->GetRowCount();
		Field *fields = result->Fetch();
		vecPlayer.clear();
		stuPlayer tmpPlayer;
		for (; j < number; j++)
		{
			i = 0;
			tmpPlayer.dwUserid = fields[i++].GetUInt32();
			tmpPlayer.strName = fields[i++].GetString();
			tmpPlayer.wLevel = fields[i++].GetUInt16();
			vecPlayer.push_back(tmpPlayer);
			result->NextRow();
			fields = result->Fetch();
		}

		delete result;
		return true;
	}
	return false;
}

uint32_t CDBCtrl::GetUserFriendNumber(uint32 dwAccountId)
{
	DatabaseMysql * mysql = WorldDb.GetDBConnection(0);//我们只用一个库

	QueryResult* result = mysql->PQuery(
		"select count(*) from user_friend where user_id = '%u'",
		dwAccountId
		);
	if (!result)
	{
		return 0;
	}
	Field *fieldsChar = result->Fetch();
	uint32_t number = fieldsChar[0].GetUInt32();
	delete result;

	return number;
}

uint32_t CDBCtrl::GetUserRequestFriendNumber(uint32 dwAccountId)
{
	DatabaseMysql * mysql = WorldDb.GetDBConnection(0);//我们只用一个库

	QueryResult* result = mysql->PQuery(
		"select count(*) from user_friend_request where user_id = %u",
		dwAccountId
		);
	if (!result)
	{
		return 0;
	}
	Field *fieldsChar = result->Fetch();
	uint32_t number = fieldsChar[0].GetUInt32();
	delete result;

	return number;
}


bool CDBCtrl::GetUserCardPacketDataOffOnline(std::string &str1team, std::string &strsuit, std::string &cards, const uint32 &dwAccountId, std::string &equip_first, std::string &master, std::string &equip_master)
{
	DatabaseMysql * mysql = WorldDb.GetDBConnection(dwAccountId);
	QueryResult *result = mysql->PQuery(
			"select first_team,lineup,pet_equip_card,equip_first_team,master_card,equip_master "
			"from user_game_blob where user_id = %u",
			dwAccountId
			);
	if (result)
	{
		int i = 0;
		Field *fields = result->Fetch();

                str1team = fields[i++].GetCppString();
                strsuit = fields[i++].GetCppString();
                cards.assign(fields[i].GetString(), fields[i].GetLength());
				i++;
                equip_first.assign(fields[i].GetString(), fields[i].GetLength());
				i++;
				master.assign(fields[i].GetString(), fields[i].GetLength());
				i++;
				equip_master.assign(fields[i].GetString(), fields[i].GetLength());

		delete result;
		return true;
	}
	return false;
}

bool CDBCtrl::GetUserCardPacketBaseData(std::string &str1team, std::string &strsuit, const uint32 &dwAccountId, std::string &equip_first, std::string &master, std::string &equip_master, std::string &base)
{
	DatabaseMysql * mysql = WorldDb.GetDBConnection(dwAccountId);
	QueryResult *result = mysql->PQuery(
			"select first_team,lineup,equip_first_team,master_card,equip_master,base_data "
			"from user_game_blob where user_id = %u",
			dwAccountId
			);
	if (result)
	{
		int i = 0;
		Field *fields = result->Fetch();

                str1team = fields[i++].GetCppString();
                strsuit = fields[i++].GetCppString();
                equip_first.assign(fields[i].GetString(), fields[i].GetLength());
				i++;
				master.assign(fields[i].GetString(), fields[i].GetLength());
				i++;
				equip_master.assign(fields[i].GetString(), fields[i].GetLength());
				i++;
				base.assign(fields[i].GetString(), fields[i].GetLength());

		delete result;
		return true;
	}
	return false;
}

bool CDBCtrl::DelFriend(uint32 userid, uint32 friendid)
{
	uint32 dwAccountId = 0;
	DatabaseMysql * mysql = WorldDb.GetDBConnection(dwAccountId);//我们只用一个库
	assert(mysql);

	if (mysql->PExecute(
			"delete from user_friend "
			"where user_id = %u and friend_id = %u",
			userid, friendid
			//todo
	))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool CDBCtrl::InsertFriend(uint32 userid, uint32 friendid)
{
	uint32 dwAccountId = 0;
	DatabaseMysql * mysql = WorldDb.GetDBConnection(dwAccountId);//我们只用一个库
	assert(mysql);

	if (mysql->PExecute(
			"insert into user_friend(user_id, friend_id) values(%u, %u) ",
			userid, friendid
			//todo
	))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool CDBCtrl::IsHaveRole(uint32 userid)
{
	DatabaseMysql * mysql = WorldDb.GetDBConnection(0);
	QueryResult *result = mysql->PQuery(
			"select user_id "
			"from user_login where user_id = %u",
			userid
			);
	if (result)
	{
		delete result;
		return false;
	}

	return true;
}

bool CDBCtrl::GetMailInfo(uint32 dwAccountId, std::vector<stuMail> &vecMail)
{
	DatabaseMysql * mysql = WorldDb.GetDBConnection(0);
	QueryResult *result = mysql->PQuery(
			"select email_id, sendid_user_id, sendid_user_name, IsDo, type, time_stamp, detail_type, title "
			"from user_email where user_id = %u and flag = 0 order by time_stamp desc limit %u",
			dwAccountId, MAIL_LIMIT
			);

	if (result)
	{
		int i = 0;
		int j = 0;
		int number = result->GetRowCount();
		Field *fields = result->Fetch();
		vecMail.clear();
		stuMail tmpMail;
		for (; j < number; j++)
		{
			i = 0;
			tmpMail.dwMailId = fields[i++].GetUInt32();
			tmpMail.dwSendId = fields[i++].GetUInt32();
			tmpMail.strName = fields[i++].GetString();
			tmpMail.byDoflag = fields[i++].GetUInt8();
			tmpMail.byType = fields[i++].GetUInt8();
			tmpMail.dwTime = fields[i++].GetUInt32();
            tmpMail.byDetailType = fields[i++].GetUInt8();
			tmpMail.strTitle = fields[i++].GetCppString();
			vecMail.push_back(tmpMail);
			result->NextRow();
			fields = result->Fetch();
		}

		delete result;
		return true;
	}
	return false;
}

bool CDBCtrl::GetMailDetailInfo(uint32 dwAccountId, uint32 mailid, stuMailDetail &Mail)
{
	DatabaseMysql * mysql = WorldDb.GetDBConnection(0);
	QueryResult *result = mysql->PQuery(
			"select email_id, sendid_user_id, sendid_user_name, IsDo, type, time_stamp, content, attachment, detail_type, title "
			"from user_email where user_id = %u and flag = 0 and email_id = %u order by time_stamp desc limit 200",
			dwAccountId, mailid
			);

	if (result)
	{
		int i = 0;
		int number = result->GetRowCount();
		Field *fields = result->Fetch();

			Mail.dwMailId = fields[i++].GetUInt32();
			Mail.dwSendId = fields[i++].GetUInt32();
			Mail.strName = fields[i++].GetString();
			Mail.byDoflag = fields[i++].GetUInt8();
			Mail.byType = fields[i++].GetUInt8();
			Mail.dwTime = fields[i++].GetUInt32();
            Mail.content = fields[i++].GetString();
			int ii = i;
            Mail.blob.assign(fields[ii].GetString(), fields[ii].GetLength());
			i++;
            Mail.byDetailType = fields[i++].GetUInt8();
			Mail.title = fields[i++].GetCppString();

		delete result;
		return true;
	}
	return false;
}

bool CDBCtrl::DelMail(uint32 userid, uint32 mailid)
{
	uint32 dwAccountId = 0;
	DatabaseMysql * mysql = WorldDb.GetDBConnection(dwAccountId);//我们只用一个库
	assert(mysql);

	if (mysql->PExecute(
			"update user_email set flag = 1 where user_id = %u and email_id = %u ",
			userid, mailid
			//todo
	))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool CDBCtrl::UpdateMail(uint32 userid, uint32 mailid)
{
	uint32 dwAccountId = 0;
	DatabaseMysql * mysql = WorldDb.GetDBConnection(dwAccountId);//我们只用一个库
	assert(mysql);

	if (mysql->PExecute(
			"update user_email set IsDo = 1 where user_id = %u and email_id = %u ",
			userid, mailid
			//todo
	))
	{
		return true;
	}
	else
	{
		return false;
	}
}

uint32 CDBCtrl::GetMailId(uint32 userid)
{
	DatabaseMysql * mysql = WorldDb.GetDBConnection(0);//我们只用一个库

	QueryResult* retChar = mysql->PQuery(
		"select max(email_id) from user_email where user_id = '%u'",
		userid
		);
	if (!retChar)
	{
		return 1;
	}
	Field *fieldsChar = retChar->Fetch();
	uint32 dwUserId = fieldsChar[0].GetUInt32();
	delete retChar;

	return dwUserId + 1;
}

bool CDBCtrl::InsertMail(uint32 userid, const stuMailDetail &Mail)
{
	uint32 dwAccountId = 0;
	DatabaseMysql * mysql = WorldDb.GetDBConnection(dwAccountId);//我们只用一个库
	assert(mysql);

	std::string strName = Mail.strName;
	mysql->escape_string(strName);
	std::string strContent = Mail.content;
	mysql->escape_string(strContent);
	std::string strBlob = Mail.blob;
	IME_DEBUG("insert mail %s",strBlob.c_str());
	mysql->escape_string(strBlob);
	std::string strTitle = Mail.title;
	mysql->escape_string(strTitle);

	if (mysql->PExecute(
			"insert into user_email(user_id, email_id, sendid_user_id, sendid_user_name, IsDo, type, content, attachment, time_stamp, detail_type, title) values(%u, %u, %u, '%s',%u, %u, '%s', '%s', %u, %u, '%s') ",
			userid, Mail.dwMailId, Mail.dwSendId, strName.c_str(), Mail.byDoflag, Mail.byType, strContent.c_str(), strBlob.c_str(), Mail.dwTime, Mail.byDetailType, strTitle.c_str()
	))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool CDBCtrl::IsFriend(uint32 userid, uint32 friendid)
{
	DatabaseMysql * mysql = WorldDb.GetDBConnection(0);
	QueryResult *result = mysql->PQuery(
			"select user_id "
			"from user_friend where user_id = '%u' and friend_id = '%u' ",
			userid, friendid
			);
	if (result)
	{
		delete result;
		return true;
	}

	return false;
}

bool CDBCtrl::GetUserArena(uint32_t &userid, uint32_t &prestige, uint32_t &rank, uint32_t &lasttime, uint32_t &challenge_number, uint32_t &ranked, uint32_t &number)
{
	DatabaseMysql * mysql = WorldDb.GetDBConnection(userid);
	QueryResult *result = mysql->PQuery(
			"select user_id, prestige, rank, lasttime, challenge_number, ranked, number "
			"from user_arena where user_id = %u",
			userid
			);
	if (result)
	{
		int i = 0;
		Field *fields = result->Fetch();

		userid = fields[i++].GetUInt32();
		prestige = fields[i++].GetUInt32();
		rank = fields[i++].GetUInt32();
		lasttime = fields[i++].GetUInt32();
		challenge_number = fields[i++].GetUInt32();
		ranked = fields[i++].GetUInt32();
		number = fields[i++].GetUInt32();

		delete result;
		return true;
	}
	return false;
}

bool CDBCtrl::GetUserArenaAll(std::vector<SArena> &data)
{
	DatabaseMysql * mysql = WorldDb.GetDBConnection(0);
	QueryResult *result = mysql->PQuery(
			"select user_id, rank, ranked, number "
			"from user_arena"
			);
	if (result)
	{
		int i = 0;
		int j = 0;
		int number = result->GetRowCount();
		Field *fields = result->Fetch();
		data.clear();
		SArena tmpPlayer;
		for (; j < number; j++)
		{
			i = 0;
			tmpPlayer.userid = fields[i++].GetUInt32();
			tmpPlayer.rank = fields[i++].GetUInt32();
			tmpPlayer.ranked = fields[i++].GetUInt32();
			tmpPlayer.getprestige = fields[i++].GetUInt32();
			data.push_back(tmpPlayer);
			result->NextRow();
			fields = result->Fetch();
		}

		delete result;
		return true;
	}
	return false;
}

bool CDBCtrl::UpdateUserArenaForRankPrestige(uint32 rank, uint32 getprestige, uint32 userid)
{
	DatabaseMysql * mysql = WorldDb.GetDBConnection(0);
	int ret = 0;
	return mysql->PExecute(
				"update user_arena set "
				"ranked = %u,number = %u "
				"where user_id = %u",
				rank, getprestige,
				userid
				);
}

void CDBCtrl::SaveUserArena(uint32_t userid, uint32_t prestige, uint32_t rank, uint32_t lasttime, uint32_t challenge_number, uint32_t ranked, uint32_t number)
{
	DatabaseMysql * mysql = WorldDb.GetDBConnection(userid);

	mysql->PExecute(
		"update user_arena set "
		"prestige = '%u', rank = '%u', lasttime = '%u', challenge_number = '%u', ranked = %u, number = %u "
		"where user_id=%u",
		prestige, rank, lasttime, challenge_number, ranked, number,
		userid
		);
}

bool CDBCtrl::UpdateUserRank(uint32 rank1, uint32 rank2, uint32 userid1, uint32 userid2)
{
	DatabaseMysql * mysql = WorldDb.GetDBConnection(0);
	int ret = 0;
	mysql->StartTransaction();
	if (!mysql->PExecute(
				"update user_arena set "
				"rank = %u "
				"where user_id = %u",
				rank1,
				userid2
				))
	{
		mysql->EndRollback();
		return false;
	}
	if (!mysql->PExecute(
				"update user_arena set "
				"rank = %u "
				"where user_id = %u",
				rank2,
				userid1
				))
	{
		mysql->EndRollback();
		return false;
	}
	mysql->EndCommit();
	return true;

}

uint32 CDBCtrl::GetArenaRank(uint32 userid)
{
	DatabaseMysql * mysql = WorldDb.GetDBConnection(0);//我们只用一个库

	QueryResult* retChar = mysql->PQuery(
		"select max(rank) from user_arena"
		);
	if (!retChar)
	{
		return 0;
	}
	Field *fieldsChar = retChar->Fetch();
	uint32 rank = fieldsChar[0].GetUInt32();
	delete retChar;

	return rank + 1;
}

bool CDBCtrl::InsertArena(uint32 userid, uint32 prestige, uint32 rank, uint32 lasttime, uint32 challenge_number)
{
	uint32 dwAccountId = 0;
	DatabaseMysql * mysql = WorldDb.GetDBConnection(dwAccountId);//我们只用一个库

	if (mysql->PExecute(
			"insert into user_arena(user_id, prestige, rank, lasttime, challenge_number) values(%u, %u, %u, %u, %u) ",
			userid, prestige, rank, lasttime, challenge_number
	))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool CDBCtrl::GetUserBaseDataArenaTop(uint32 dwAccountId, std::vector<stuPlayerPlus> &vecPlayer)
{
	DatabaseMysql * mysql = WorldDb.GetDBConnection(0);
	QueryResult *result = mysql->PQuery(
			"select user_arena.user_id,name,level,"
			"rank "
			"from user_arena left join user_game on user_game.user_id = user_arena.user_id "
		    "order by rank limit 10"
			);

	if (result)
	{
		int i = 0;
		int j = 0;
		int number = result->GetRowCount();
		Field *fields = result->Fetch();
		vecPlayer.clear();
		stuPlayerPlus tmpPlayer;
		for (; j < number; j++)
		{
			i = 0;
			tmpPlayer.dwUserid = fields[i++].GetUInt32();
			tmpPlayer.strName = fields[i++].GetCppString();
			tmpPlayer.wLevel = fields[i++].GetUInt16();
			tmpPlayer.dwRank = fields[i++].GetUInt32();
			vecPlayer.push_back(tmpPlayer);
			result->NextRow();
			fields = result->Fetch();
		}

		delete result;
		return true;
	}
	return false;
}

bool CDBCtrl::GetUserBaseDataArenaLose(uint32 dwAccountId, uint32 rank, std::vector<stuPlayerPlus> &vecPlayer)
{
	DatabaseMysql * mysql = WorldDb.GetDBConnection(0);
	QueryResult *result = mysql->PQuery(
			"select user_game.user_id,name,level,"
			"rank "
			"from user_game, user_arena where user_game.user_id = user_arena.user_id and rank < %u and user_game.user_id in (select win_user_id from user_lose where user_id = %u) and user_game.user_id != %u "
		    "order by rank limit 5",
			rank, dwAccountId, dwAccountId
			);
	IME_DEBUG("user_id %d rank %u", dwAccountId, rank);

	if (result)
	{
		int i = 0;
		int j = 0;
		int number = result->GetRowCount();
		Field *fields = result->Fetch();
		vecPlayer.clear();
		stuPlayerPlus tmpPlayer;
		for (; j < number; j++)
		{
			i = 0;
			tmpPlayer.dwUserid = fields[i++].GetUInt32();
			tmpPlayer.strName = fields[i++].GetString();
			tmpPlayer.wLevel = fields[i++].GetUInt16();
			tmpPlayer.dwRank = fields[i++].GetUInt32();
			vecPlayer.push_back(tmpPlayer);
			result->NextRow();
			fields = result->Fetch();
		}

		delete result;
		return true;
	}
	return false;
}

bool CDBCtrl::GetUserBaseDataArenaChallenge(uint32 dwAccountId, uint32 rank, std::vector<stuPlayerPlus> &vecPlayer, std::vector<uint32_t> vecRank)
{
	DatabaseMysql * mysql = WorldDb.GetDBConnection(0);
	std::string str;
	CUtil::StrMerge(str, ",", vecRank);

	QueryResult *result = mysql->PQuery(
			"select user_arena.user_id,name,level,"
			"rank "
			"from user_arena left join user_game on user_game.user_id = user_arena.user_id where rank in (%s) "
		    "order by rank",
			str.c_str()
			);
	IME_DEBUG("user_id %d rank %u", dwAccountId, rank);

	if (result)
	{
		int i = 0;
		int j = 0;
		int number = result->GetRowCount();
		Field *fields = result->Fetch();
		vecPlayer.clear();
		stuPlayerPlus tmpPlayer;
		for (; j < number; j++)
		{
			i = 0;
			tmpPlayer.dwUserid = fields[i++].GetUInt32();
			tmpPlayer.strName = fields[i++].GetCppString();
			tmpPlayer.wLevel = fields[i++].GetUInt16();
			tmpPlayer.dwRank = fields[i++].GetUInt32();
			vecPlayer.push_back(tmpPlayer);
			result->NextRow();
			fields = result->Fetch();
		}

		delete result;
		return true;
	}
	return false;
}

bool CDBCtrl::InsertArenaLose(uint32 userid, uint32 friendid)
{
	uint32 dwAccountId = 0;
	DatabaseMysql * mysql = WorldDb.GetDBConnection(dwAccountId);//我们只用一个库

	return mysql->PExecute(
			"insert into user_lose(user_id, win_user_id) values(%u, %u) ",
			userid, friendid
	);
}

uint32 CDBCtrl::GetUserTimer(uint32 dwAccountId, uint16 type, uint32 timerid)
{
	DatabaseMysql * mysql = WorldDb.GetDBConnection(0);
	QueryResult *result = mysql->PQuery(
			"select time,"
			"from user_timer "
		    "where user_id = %u and timer_type = %u and timer_id = %u",
			dwAccountId, type, timerid
			);

	if (result)
	{
		int i = 0;
		int number = result->GetRowCount();
		Field *fields = result->Fetch();
		uint32 temp = fields[i++].GetUInt32();

		delete result;
		return temp;
	}
	return 0;
}

bool CDBCtrl::InsertUserTimer(uint32 userid, uint16 type, uint32 timerid, uint32 time)
{
	uint32 dwAccountId = 0;
	DatabaseMysql * mysql = WorldDb.GetDBConnection(dwAccountId);//我们只用一个库

	return mysql->PExecute(
			"insert into user_timer(user_id, timer_type, timer_id, time) values(%u, %u, %u, %u) ",
			userid, type, timerid, time
	);
}

void CDBCtrl::SaveUserTimer(uint32 userid, uint16 type, uint32 timerid, uint32 time)
{
	DatabaseMysql * mysql = WorldDb.GetDBConnection(userid);

	mysql->PExecute(
			"update user_timer set "
			"time = '%u' "
			"where user_id= %u and timer_type = %u and timer_id = %u",
			time,
			userid, type, timerid
			);
}


//bool CDBCtrl::GetUserMission(uint32 dwAccountId, uint32 task_id, uint32 &time, uint32 &step)
//{
//	DatabaseMysql * mysql = WorldDb.GetDBConnection(0);
//	QueryResult *result = mysql->PQuery(
//			"select time,step "
//			"from user_mission "
//		    "where user_id = %u and task_id = %u",
//			dwAccountId, task_id
//			);
//
//	if (result)
//	{
//		int i = 0;
//		int number = result->GetRowCount();
//		Field *fields = result->Fetch();
//		time = fields[i++].GetUInt32();
//		step = fields[i++].GetUInt32();
//
//		delete result;
//		return true;
//	}
//	return false;
//}
//
//
//bool CDBCtrl::InsertUserMission(uint32 userid, uint32 task_id)
//{
//	uint32 dwAccountId = 0;
//	DatabaseMysql * mysql = WorldDb.GetDBConnection(dwAccountId);//我们只用一个库
//
//	return mysql->PExecute(
//			"insert ignore into user_mission(user_id, task_id) values(%u, %u) ",
//			userid, task_id
//	);
//}
//
//void CDBCtrl::SaveUserMission(uint32 dwAccountId, uint32 task_id, uint32 time, uint32 step)
//{
//	DatabaseMysql * mysql = WorldDb.GetDBConnection(0);
//
//	mysql->PExecute(
//			"update user_mission set "
//			"time = '%u',step = %u "
//			"where user_id= %u and task_id = %u",
//			time, step,
//			dwAccountId, task_id
//			);
//}
//
//bool CDBCtrl::GetUserMissionVec(uint32 dwAccountId, std::vector<STask> &vec)
//{
//	DatabaseMysql * mysql = WorldDb.GetDBConnection(0);
//	QueryResult *result = mysql->PQuery(
//			"select task_id,time,step "
//			"from user_mission "
//		    "where user_id = %u",
//			dwAccountId
//			);
//
//	if (result)
//	{
//		int i = 0;
//		int j = 0;
//		int number = result->GetRowCount();
//		Field *fields = result->Fetch();
//		vec.clear();
//		STask tmp;
//		for (; j < number; j++)
//		{
//			i = 0;
//			tmp.dwTaskId = fields[i++].GetUInt32();
//			tmp.dwTime = fields[i++].GetUInt32();
//			tmp.dwStep = fields[i++].GetUInt32();
//			vec.push_back(tmp);
//			result->NextRow();
//			fields = result->Fetch();
//		}
//
//		delete result;
//		return true;
//	}
//	return false;
//}

//bool CDBCtrl::InsertUserRank(uint32 userid, std::string name, uint16 level, uint32 attack, uint32 defence)
//{
//	uint32 dwAccountId = 0;
//	DatabaseMysql * mysql = WorldDb.GetDBConnection(dwAccountId);//我们只用一个库
//
//	std::string strName = name;
//
//	mysql->escape_string(strName);
//	return mysql->PExecute(
//			"insert into user_rank(user_id, name, level, attack, defence) values(%u, '%s', %u, %u, %u) ",
//			userid, strName.c_str(), level, attack, defence
//	);
//}
//
//void CDBCtrl::SaveUserRank(uint32 dwAccountId, uint16 level, uint32 attack, uint32 defence)
//{
//	DatabaseMysql * mysql = WorldDb.GetDBConnection(0);
//
//	mysql->PExecute(
//			"update user_rank set "
//			"level = %u,attack = %u,defence = %u "
//			"where user_id= %u",
//			level, attack, defence,
//			dwAccountId
//			);
//}
//
//bool CDBCtrl::GetUserRank(uint32 dwAccountId, uint16 &level, uint32 &attack, uint32 &defence)
//{
//	DatabaseMysql * mysql = WorldDb.GetDBConnection(0);
//	QueryResult *result = mysql->PQuery(
//			"select level,attack,defence "
//			"from user_rank "
//		    "where user_id = %u",
//			dwAccountId
//			);
//
//	if (result)
//	{
//		int i = 0;
//		int number = result->GetRowCount();
//		Field *fields = result->Fetch();
//		level = fields[i++].GetUInt32();
//		attack = fields[i++].GetUInt32();
//		defence = fields[i++].GetUInt32();
//
//		delete result;
//		return true;
//	}
//	return false;
//}
//
//bool CDBCtrl::GetUserRankLevel(uint32 dwAccountId, std::vector<SRank> &vec)
//{
//	DatabaseMysql * mysql = WorldDb.GetDBConnection(0);
//	QueryResult *result = mysql->PQuery(
//			"select user_id, name, level "
//			"from user_rank "
//		    "order by level desc, user_id asc limit 20",
//			dwAccountId
//			);
//
//	if (result)
//	{
//		int i = 0;
//		int j = 0;
//		int number = result->GetRowCount();
//		Field *fields = result->Fetch();
//		vec.clear();
//		SRank tmp;
//		for (; j < number; j++)
//		{
//			i = 0;
//			tmp.dwUserId = fields[i++].GetUInt32();
//           	tmp.strName.assign(fields[1].GetString(), fields[1].GetLength());
//			i++;
//			tmp.dwVal = fields[i++].GetUInt32();
//			vec.push_back(tmp);
//			result->NextRow();
//			fields = result->Fetch();
//		}
//
//		delete result;
//		return true;
//	}
//	return false;
//}
//
//bool CDBCtrl::GetUserRankAttack(uint32 dwAccountId, std::vector<SRank> &vec)
//{
//	DatabaseMysql * mysql = WorldDb.GetDBConnection(0);
//	QueryResult *result = mysql->PQuery(
//			"select user_id, name, attack "
//			"from user_rank "
//		    "order by attack desc, user_id asc limit 20",
//			dwAccountId
//			);
//
//	if (result)
//	{
//		int i = 0;
//		int j = 0;
//		int number = result->GetRowCount();
//		Field *fields = result->Fetch();
//		vec.clear();
//		SRank tmp;
//		for (; j < number; j++)
//		{
//			i = 0;
//			tmp.dwUserId = fields[i++].GetUInt32();
//           	tmp.strName.assign(fields[1].GetString(), fields[1].GetLength());
//			i++;
//			tmp.dwVal = fields[i++].GetUInt32();
//			vec.push_back(tmp);
//			result->NextRow();
//			fields = result->Fetch();
//		}
//
//		delete result;
//		return true;
//	}
//	return false;
//}
//
//bool CDBCtrl::GetUserRankDefence(uint32 dwAccountId, std::vector<SRank> &vec)
//{
//	DatabaseMysql * mysql = WorldDb.GetDBConnection(0);
//	QueryResult *result = mysql->PQuery(
//			"select user_id, name, defence "
//			"from user_rank "
//		    "order by defence desc, user_id asc limit 20",
//			dwAccountId
//			);
//
//	if (result)
//	{
//		int i = 0;
//		int j = 0;
//		int number = result->GetRowCount();
//		Field *fields = result->Fetch();
//		vec.clear();
//		SRank tmp;
//		for (; j < number; j++)
//		{
//			i = 0;
//			tmp.dwUserId = fields[i++].GetUInt32();
//           	tmp.strName.assign(fields[1].GetString(), fields[1].GetLength());
//			i++;
//			tmp.dwVal = fields[i++].GetUInt32();
//			vec.push_back(tmp);
//			result->NextRow();
//			fields = result->Fetch();
//		}
//
//		delete result;
//		return true;
//	}
//	return false;
//}

bool CDBCtrl::GetUserArenaTop500(std::map<uint32, uint32> &map)
{
	DatabaseMysql * mysql = WorldDb.GetDBConnection(0);
	QueryResult *result = mysql->PQuery(
			"select user_id,rank "
			"from user_arena "
		    "order by rank limit 500"
			);
	uint32 userid;
	uint32 rank;
	map.clear();

	if (result)
	{
		int i = 0;
		int j = 0;
		int number = result->GetRowCount();
		Field *fields = result->Fetch();
		for (; j < number; j++)
		{
			i = 0;
			userid = fields[i++].GetUInt32();
			rank = fields[i++].GetUInt32();
			map[userid] = rank;
			result->NextRow();
			fields = result->Fetch();
		}

		delete result;
		return true;
	}
	return false;
}


bool CDBCtrl::GetGroupInfoAll(std::vector<SGroup> &vec)
{
	DatabaseMysql * mysql = WorldDb.GetDBConnection(0);
	QueryResult *result = mysql->PQuery(
			"select user_id, group_id, content, flag, create_name, group_name, credit, level, member "
			"from group_info where flag = 0"
			);

	if (result)
	{
		int i = 0;
		int j = 0;
		int number = result->GetRowCount();
		Field *fields = result->Fetch();
		vec.clear();
		SGroup tmp;
		for (; j < number; j++)
		{
			i = 0;
			tmp.dwUserId = fields[i++].GetUInt32();
			tmp.dwGroupId = fields[i++].GetUInt32();
           	tmp.strContent.assign(fields[2].GetString(), fields[2].GetLength());
			i++;
			tmp.byFlag = fields[i++].GetUInt8();
			tmp.strCreateName.assign(fields[i].GetString(), fields[i].GetLength());
			i++;
			tmp.strGroupName.assign(fields[i].GetString(), fields[i].GetLength());
			i++;
			tmp.dwCredit = fields[i++].GetUInt32();
			tmp.wLevel = fields[i++].GetUInt16();
			tmp.strMember.assign(fields[i].GetString(), fields[i].GetLength());
			i++;
			vec.push_back(tmp);
			result->NextRow();
			fields = result->Fetch();
		}

		delete result;
		return true;
	}
	return false;
}

bool CDBCtrl::GetGroupInfo(uint32 userid, uint32 groupid, std::string &content, uint8 &flag)
{
	DatabaseMysql * mysql = WorldDb.GetDBConnection(0);
	QueryResult *result = mysql->PQuery(
			"select content, flag "
			"from group_info "
			"where user_id= %u and group_id = %u",
			userid, groupid
			);

	if (result)
	{
		int i = 0;
		int number = result->GetRowCount();
		Field *fields = result->Fetch();

           	content.assign(fields[0].GetString(), fields[0].GetLength());
			i++;
			flag = fields[i++].GetUInt8();

		delete result;
		return true;
	}
	return false;
}


bool CDBCtrl::InsertGroup(uint32 userid, uint32 groupid, const std::string &createname, const std::string &groupname)
{
	uint32 dwAccountId = 0;
	DatabaseMysql * mysql = WorldDb.GetDBConnection(dwAccountId);//我们只用一个库

	std::string strUserName = createname;
	std::string strGroupName = groupname;
	mysql->escape_string(strUserName);
	mysql->escape_string(strGroupName);

	return mysql->PExecute(
			"insert into group_info(user_id, group_id, create_name, group_name) values(%u, %u, '%s', '%s') ",
			userid, groupid, strUserName.c_str(), strGroupName.c_str()
	);
}

void CDBCtrl::SaveGroup(uint32 userid, uint32 groupid, const std::string &content, uint8 flag, const std::string &createname, const std::string &groupname, uint16 credit, uint16 level, const std::string &member)
{
	DatabaseMysql * mysql = WorldDb.GetDBConnection(0);

	std::string str = content;
	std::string strCreateName = createname;
	std::string strGroupName = groupname;
	std::string strMember = member;
	mysql->escape_string(str);
	mysql->escape_string(strCreateName);
	mysql->escape_string(strGroupName);
	mysql->escape_string(strMember);

	mysql->PExecute(
			"update group_info set "
			"content = '%s',flag = %u,create_name = '%s',group_name = '%s', credit = %u, level = %u, member = '%s'  "
			"where user_id= %u and group_id = %u",
			str.c_str(), flag, strCreateName.c_str(), strGroupName.c_str(), credit, level, strMember.c_str(),
			userid, groupid
			);
}

uint32 CDBCtrl::GetGroupId(uint32 userid)
{
	DatabaseMysql * mysql = WorldDb.GetDBConnection(0);//我们只用一个库

	QueryResult* retChar = mysql->PQuery(
		"select max(group_id) from group_info where user_id = %u",
		userid
		);
	if (!retChar)
	{
		return 1;
	}
	Field *fieldsChar = retChar->Fetch();
	uint32 val = fieldsChar[0].GetUInt32();
	delete retChar;

	return val + 1;
}

bool CDBCtrl::GetGroupMailInfo(uint64 dwGroupId, std::vector<SGroupMail> &vecMail, uint32 off)
{
	if (off > GROUP_MAIL_LIMIT - 10)
		off = GROUP_MAIL_LIMIT - 10;
	DatabaseMysql * mysql = WorldDb.GetDBConnection(0);
	QueryResult *result = mysql->PQuery(
			"select email_id, IsDo, type, content, attachment, time_stamp, detail_type "
			"from group_email where group_id = %llu order by time_stamp desc limit %u, 10",
			dwGroupId, off
			);

	if (result)
	{
		int i = 0;
		int j = 0;
		int number = result->GetRowCount();
		Field *fields = result->Fetch();
		vecMail.clear();
		SGroupMail tmpMail;
		for (; j < number; j++)
		{
			i = 0;
			tmpMail.dwMailId = fields[i++].GetUInt32();
			tmpMail.byDoflag = fields[i++].GetUInt8();
			tmpMail.byType = fields[i++].GetUInt8();
			tmpMail.content.assign(fields[i].GetString(), fields[i].GetLength());
			i++;
			tmpMail.blob.assign(fields[i].GetString(), fields[i].GetLength());
			i++;
			tmpMail.dwTime = fields[i++].GetUInt32();
            tmpMail.byDetailType = fields[i++].GetUInt8();
			vecMail.push_back(tmpMail);
			result->NextRow();
			fields = result->Fetch();
		}

		delete result;
		return true;
	}
	return false;
}

bool CDBCtrl::UpdateGroupMail(uint64 groupid, uint32 mailid, uint8 Do)
{
	uint32 dwAccountId = 0;
	DatabaseMysql * mysql = WorldDb.GetDBConnection(dwAccountId);//我们只用一个库
	assert(mysql);

	if (mysql->PExecute(
			"update group_email set IsDo = %u where group_id = %llu and email_id = %u ",
			Do, groupid, mailid
			//todo
	))
	{
		return true;
	}
	else
	{
		return false;
	}
}

uint32 CDBCtrl::GetGroupMailId(uint64 groupid)
{
	DatabaseMysql * mysql = WorldDb.GetDBConnection(0);//我们只用一个库

	QueryResult* retChar = mysql->PQuery(
		"select max(email_id) from group_email where group_id = '%llu'",
		groupid
		);
	if (!retChar)
	{
		return 1;
	}
	Field *fieldsChar = retChar->Fetch();
	uint32 dwUserId = fieldsChar[0].GetUInt32();
	delete retChar;

	return dwUserId + 1;
}

bool CDBCtrl::InsertGroupMail(const SGroupMail &Mail)
{
	uint32 dwAccountId = 0;
	DatabaseMysql * mysql = WorldDb.GetDBConnection(dwAccountId);//我们只用一个库
	assert(mysql);

	std::string strContent = Mail.content;
	mysql->escape_string(strContent);
	std::string strBlob = Mail.blob;
	mysql->escape_string(strBlob);

	if (mysql->PExecute(
			"insert into group_email(group_id, email_id, IsDo, type, content, attachment, time_stamp, detail_type) values(%llu, %u, %u , %u, '%s', '%s', %u, %u) ",
		Mail.lGroupId, Mail.dwMailId, Mail.byDoflag, Mail.byType, strContent.c_str(), strBlob.c_str(), Mail.dwTime, Mail.byDetailType
	))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool CDBCtrl::GetGroupMailInfoSingle(uint64 dwGroupId, uint32 mail, SGroupMail &tmpMail)
{
	DatabaseMysql * mysql = WorldDb.GetDBConnection(0);
	QueryResult *result = mysql->PQuery(
			"select email_id, IsDo, type, content, attachment, time_stamp, detail_type "
			"from group_email where group_id = %llu and email_id = %u",
			dwGroupId, mail
			);

	if (result)
	{
		int i = 0;
		int number = result->GetRowCount();
		Field *fields = result->Fetch();

		tmpMail.dwMailId = fields[i++].GetUInt32();
		tmpMail.byDoflag = fields[i++].GetUInt8();
		tmpMail.byType = fields[i++].GetUInt8();
		tmpMail.content.assign(fields[i].GetString(), fields[i].GetLength());
		i++;
		tmpMail.blob.assign(fields[i].GetString(), fields[i].GetLength());
		i++;
		tmpMail.dwTime = fields[i++].GetUInt32();
		tmpMail.byDetailType = fields[i++].GetUInt8();

		delete result;
		return true;
	}
	return false;
}

bool CDBCtrl::GetUserShopPacketData(std::string &shop, const uint32 &dwAccountId)
{
	DatabaseMysql * mysql = WorldDb.GetDBConnection(dwAccountId);
	QueryResult *result = mysql->PQuery(
			"select shop "
			"from user_game_blob where user_id = %u",
			dwAccountId
			);
	if (result)
	{
		int i = 0;
		Field *fields = result->Fetch();
		shop.assign(fields[0].GetString(), fields[0].GetLength());
		delete result;
		return true;
	}
	return false;
}

void CDBCtrl::SaveUserShopPacketData(const std::string &shop, const uint32 &dwAccountId)
{
	DatabaseMysql * mysql = WorldDb.GetDBConnection(dwAccountId);

	std::string stritem = shop;
	mysql->escape_string(stritem);
	mysql->PExecute(
			"update user_game_blob set "
			"shop = '%s' "
			"where user_id=%u",
			stritem.c_str(),
			dwAccountId
			);
}

bool CDBCtrl::GetShopOrderId(uint32 userid, uint32 &order_id)
{
	DatabaseMysql * mysql = WorldDb.GetDBConnection(0);//我们只用一个库

	QueryResult* retChar = mysql->PQuery(
		"select max(order_id) from user_dollar_record where user_id = '%u'",
		userid
		);
	if (!retChar)
	{
		return false;
	}
	Field *fieldsChar = retChar->Fetch();
	uint32 dwUserId = fieldsChar[0].GetUInt32();
	delete retChar;

	order_id = dwUserId + 1;
	return true;
}

void CDBCtrl::LogShopRecord(const uint32 &dwAccountId, uint32 order, uint32 dollar, uint32 shopid, uint32 number, uint32 type)
{
	DatabaseMysql * mysql = WorldDb.GetDBConnection(dwAccountId);
	if (mysql->PExecute(
			"insert into user_dollar_record "
			"(user_id, order_id, dollar, time, shop_id, number, type, step) "
			"values (%u, %u, %u, %u, %u, %u, %u, 0) ",
			dwAccountId, order, dollar, time(NULL), shopid, number, type
			//todo
	))
	{
		if (type != E_DC_CHARGE && type != E_DC_GM && type != E_DC_GIVE)
		{
			if (CenterDBCtrl::InsertPurchaseInfo(dwAccountId, type * 1000000 + shopid, number, dollar, (uint32_t)time(NULL)))
			{
				IME_ERROR("insert purchase_info fail [user_id=%u,order_id=%u,dollar=%u,time=%u,shopid=%u,number=%u,type=%u]", dwAccountId, order, dollar, time(NULL), shopid, number, type);
			}
		}
		return;
	}
	else
	{
		IME_LOG("[user_id=%u,order_id=%u,dollar=%u,time=%u,shopid=%u,number=%u,type=%u] Description: insert shop record fail, first step del dollar or charge dollar", dwAccountId, order, dollar, time(NULL), shopid, number, type);
	}
}

void CDBCtrl::LogShopProductGive(const uint32 &dwAccountId, uint32 order)
{
	DatabaseMysql * mysql = WorldDb.GetDBConnection(dwAccountId);

	if (mysql->PExecute(
			"update user_dollar_record set "
			"step = 1 "
			"where user_id= %u and order_id = %u",
			dwAccountId, order
			))
	{
		return;
	}
	else
	{
		IME_LOG("[user_id=%u,order_id=%u] Description: update shop record fail, second step add product", dwAccountId, order);
	}
}

bool CDBCtrl::InsertDurationLose(uint32 userid, uint32 winid)
{
	uint32 dwAccountId = 0;
	DatabaseMysql * mysql = WorldDb.GetDBConnection(dwAccountId);//我们只用一个库

	return mysql->PExecute(
			"insert into user_lose_duration(user_id, win_user_id) values(%u, %u) ON DUPLICATE KEY UPDATE win_number = win_number + 1",
			userid, winid
	);
}

bool CDBCtrl::GetUserBaseDataAuto10ForLose(uint32 dwAccountId, std::vector<uint32_t> &vecPlayer)
{
	DatabaseMysql * mysql = WorldDb.GetDBConnection(0);
	QueryResult *result = mysql->PQuery(
			"select win_user_id from user_lose_duration where user_id = %u order by win_number desc limit 5",
			dwAccountId
			);

	if (result)
	{
		int i = 0;
		int j = 0;
		int number = result->GetRowCount();
		Field *fields = result->Fetch();
		vecPlayer.clear();
		stuPlayer tmpPlayer;
		for (; j < number; j++)
		{
			i = 0;
			uint32_t dwUserid = fields[i++].GetUInt32();
			vecPlayer.push_back(dwUserid);
			result->NextRow();
			fields = result->Fetch();
		}

		delete result;
		return true;
	}
	return false;
}

int CDBCtrl::GetStatusArena(uint32_t userid)
{
	DatabaseMysql * mysql = WorldDb.GetDBConnection(userid);
	QueryResult *result = mysql->PQueryStrict(
			"select user_id "
			"from user_arena where user_id = %u",
			userid
			);
	if (result)
	{
		int i = 0;
		Field *fields = result->Fetch();

		if (fields == NULL)
		{
			delete result;
			return 0;
		}
		else
		{
			delete result;
			return 1;
		}
	}
	return -1;
}

bool CDBCtrl::GetUserCardMasterData(std::string &strMaster, uint32 dwAccountId)
{
	DatabaseMysql * mysql = WorldDb.GetDBConnection(dwAccountId);
	QueryResult *result = mysql->PQuery(
			"select master "
			"from user_game_blob where user_id = %u",
			dwAccountId
			);
	if (result)
	{
		int i = 0;
		Field *fields = result->Fetch();

		strMaster.assign(fields[0].GetString(), fields[0].GetLength());

		delete result;
		return true;
	}
	return false;
}

void CDBCtrl::SaveUserCardMasterData(const std::string &strMaster, uint32 dwAccountId)
{
	DatabaseMysql * mysql = WorldDb.GetDBConnection(dwAccountId);

        std::string str = strMaster;
        mysql->escape_string(str);
	mysql->PExecute(
		"update user_game_blob set "
		"master = '%s' "
		"where user_id=%u",
		str.c_str(),
		dwAccountId
		);
}

bool CDBCtrl::GetUserDurationMission(std::string &strMission, uint32 dwAccountId)
{
	DatabaseMysql * mysql = WorldDb.GetDBConnection(dwAccountId);
	QueryResult *result = mysql->PQuery(
			"select mission "
			"from user_game_blob where user_id = %u",
			dwAccountId
			);
	if (result)
	{
		int i = 0;
		Field *fields = result->Fetch();

		strMission.assign(fields[0].GetString(), fields[0].GetLength());

		delete result;
		return true;
	}
	return false;
}

void CDBCtrl::SaveUserDurationMission(const std::string &strMission, uint32 dwAccountId)
{
	DatabaseMysql * mysql = WorldDb.GetDBConnection(dwAccountId);

        std::string str = strMission;
        mysql->escape_string(str);
	mysql->PExecute(
		"update user_game_blob set "
		"mission = '%s' "
		"where user_id=%u",
		str.c_str(),
		dwAccountId
		);
}

bool CDBCtrl::GetAnnouncementData(std::vector<std::string> &strAd)
{
	DatabaseMysql * mysql = WorldDb.GetDBConnection(0);
	QueryResult *result = mysql->PQuery(
			"select content "
			"from announcement limit 10"
			);
	if (result)
	{
		int i = 0;
		int j = 0;
		int number = result->GetRowCount();
		Field *fields = result->Fetch();
		std::string str;
		for (; j < number; j++)
		{
			i = 0;
			str.assign(fields[0].GetString(), fields[0].GetLength());
			strAd.push_back(str);
			result->NextRow();
			fields = result->Fetch();
		}

		delete result;
		return true;
	}

	return false;
}

void CDBCtrl::SaveAnnouncementData(const std::string &strAd, uint32 dwId)
{
	DatabaseMysql * mysql = WorldDb.GetDBConnection(0);

        std::string str = strAd;
        mysql->escape_string(str);
	mysql->PExecute(
		"update announcement set "
		"content = '%s' "
		"where id=%u",
		str.c_str(),
		dwId
		);
}

bool CDBCtrl::GetUserDataActive(std::string &str, uint32 dwAccountId)
{
	DatabaseMysql * mysql = WorldDb.GetDBConnection(0);
	QueryResult *result = mysql->PQuery(
			"select active "
			"from user_game_blob2 where user_id = %u", dwAccountId
			);
	if (result)
	{
		int i = 0;
		Field *fields = result->Fetch();
		str.assign(fields[0].GetString(), fields[0].GetLength());

		delete result;
		return true;
	}

	return false;
}

void CDBCtrl::SaveUserDataActive(const std::string &strAd, uint32 dwId)
{
	DatabaseMysql * mysql = WorldDb.GetDBConnection(0);

        std::string str = strAd;
        mysql->escape_string(str);
	mysql->PExecute(
		"update user_game_blob2 set "
		"active = '%s' "
		"where user_id = %u",
		str.c_str(),
		dwId
		);
}

bool CDBCtrl::GetLoginInfo(uint32 dwServerid, const std::string &strIp, int nPort, uint32 dwOnlineNum, uint8_t &can_login, uint8_t &status, uint8_t &is_new, std::string &server_name, std::string &domain, std::string &resurl)
{
	bool ret;
	QueryResult *result = LoginDatabase.PQuery(
			"select can_login, status, is_new, server_name, domain, res_url from gameserver_info where area_id=%d",
			dwServerid
			);
	std::string Ip = strIp;
	LoginDatabase.escape_string(Ip);
	std::string Res = resurl;
	LoginDatabase.escape_string(Res);
	if (result)
	{
		Field *fields = result->Fetch();
		can_login = fields[0].GetUInt8();
		status = fields[1].GetUInt8();
		is_new = fields[2].GetUInt8();
		server_name = fields[3].GetCppString();
		domain = fields[4].GetCppString();
		resurl = fields[5].GetCppString();
		return true;
	}
	else
	{
		ret = LoginDatabase.PExecute(
				"insert into gameserver_info "
				"(area_id, ip, port, online_num, version, update_time, res_url) "
				"values (%d,'%s',%d,%u,%u,%u, '%s')",
				dwServerid,
				strIp.c_str(),nPort,dwOnlineNum,uint32(INNER_VERSION),time(NULL), Res.c_str()
				);
		return ret;
	}
	return false;
}

void CDBCtrl::UpdateLoginInfo(uint32_t serverid, std::string strIp, int nPort, uint32 dwOnlineNum, uint8_t can_login, uint8_t status, uint8_t is_new, std::string servername, std::string domain, std::string resurl)
{
//	QueryResult *result = LoginDatabase.PQuery(
//			"select can_login, status, update_time from gameserver_info where area_id=%d",
//			sWorld->GetServerId()
//			);
	LoginDatabase.escape_string(strIp);
	LoginDatabase.escape_string(servername);
	LoginDatabase.escape_string(domain);
//	if (result)
//	{
//		delete result;
	LoginDatabase.PExecute(
				"update gameserver_info set ip='%s', port = %d, online_num=%u, update_time=%u, version=%u, can_login = %d, status = %d, is_new = %d, server_name = '%s', domain = '%s', res_url = '%s' where area_id=%d",
				strIp.c_str(), nPort, dwOnlineNum, time(NULL), uint32(INNER_VERSION), can_login, status, is_new, servername.c_str(), domain.c_str(), resurl.c_str(), serverid
				);
//	}
//	else
//	{
//		LoginDatabase.PExecute(
//				"insert into gameserver_info "
//				"(area_id, ip, port, online_num, version, update_time) "
//				"values (%d,'%s',%d,%u,%u,%u)",
//				sWorld->GetServerId(),
//				strIp.c_str(),nPort,dwOnlineNum,uint32(INNER_VERSION),time(NULL)
//				);
//		//IME_ERROR("version %u",uint32(SERVER_VERSION));
//	}
}

bool CDBCtrl::IsCanLogin(uint32 serverid)
{
	QueryResult *result = LoginDatabase.PQuery(
			"select can_login,status from gameserver_info where area_id=%d",
			serverid
			);
	if (result)
	{
		int i = 0;
		Field *fields = result->Fetch();
		bool ret = fields[0].GetBool();
		uint32 status = fields[1].GetUInt32();

		delete result;
		return (ret && status != E_AREA_STATUS_STOP);
	}
	return false;
}

bool CDBCtrl::GetUserRob(std::string &str, uint32 dwAccountId)
{
	DatabaseMysql * mysql = WorldDb.GetDBConnection(0);
	QueryResult *result = mysql->PQuery(
			"select rob_robort "
			"from user_game_blob2 where user_id = %u", dwAccountId
			);
	if (result)
	{
		int i = 0;
		Field *fields = result->Fetch();
		str.assign(fields[0].GetString(), fields[0].GetLength());

		delete result;
		return true;
	}

	return false;
}

void CDBCtrl::SaveUserRob(const std::string &strAd, uint32 dwId)
{
	DatabaseMysql * mysql = WorldDb.GetDBConnection(0);

        std::string str = strAd;
        mysql->escape_string(str);
	mysql->PExecute(
		"update user_game_blob2 set "
		"rob_robort = '%s' "
		"where user_id = %u",
		str.c_str(),
		dwId
		);
}

bool CDBCtrl::GetUserFriendBoss(std::string &str, uint32 dwAccountId)
{
	DatabaseMysql * mysql = WorldDb.GetDBConnection(0);
	QueryResult *result = mysql->PQuery(
			"select world_boss "
			"from user_game_blob2 where user_id = %u", dwAccountId
			);
	if (result)
	{
		int i = 0;
		Field *fields = result->Fetch();
		str.assign(fields[0].GetString(), fields[0].GetLength());

		delete result;
		return true;
	}

	return false;
}

void CDBCtrl::SaveUserFriendBoss(const std::string &strAd, uint32 dwId)
{
	DatabaseMysql * mysql = WorldDb.GetDBConnection(0);

        std::string str = strAd;
        mysql->escape_string(str);
	mysql->PExecute(
		"update user_game_blob2 set "
		"world_boss = '%s' "
		"where user_id = %u",
		str.c_str(),
		dwId
		);
}

bool CDBCtrl::GetUserFriend(uint32 dwAccountId, std::vector<uint32_t> &vecFriend)
{
	DatabaseMysql * mysql = WorldDb.GetDBConnection(0);
	QueryResult *result = mysql->PQuery(
			"select friend_id from user_friend where user_id = %u "
		    "order by level desc limit %u",
			dwAccountId, MAX_FRIEND_NUMBER
			);

	if (result)
	{
		int i = 0;
		int j = 0;
		int number = result->GetRowCount();
		uint32_t friendid;
		Field *fields = result->Fetch();
		for (; j < number; j++)
		{
			i = 0;
			friendid = fields[i++].GetUInt32();
			vecFriend.push_back(friendid);

			result->NextRow();
			fields = result->Fetch();
		}

		delete result;
		return true;
	}
	return false;
}

bool CDBCtrl::GetFriendRequest(uint32 friendid, uint32 requestid)
{
	DatabaseMysql * mysql = WorldDb.GetDBConnection(0);
	QueryResult *result = mysql->PQuery(
			"select user_id "
			"from user_friend_request where user_id = '%u' and request_id = '%u'",
			friendid, requestid
			);
	if (result)
	{
		delete result;
		return true;
	}
	return false;
}

bool CDBCtrl::InsertFriendRequest(uint32 friendid, uint32 requestid)
{
	uint32 dwAccountId = 0;
	DatabaseMysql * mysql = WorldDb.GetDBConnection(dwAccountId);//我们只用一个库
	assert(mysql);

	return mysql->PExecute(
			"insert into user_friend_request "
			"(user_id, request_id) "
			"values (%u, %u)",
			friendid, requestid
			//todo
	);
}

bool CDBCtrl::DelFriendRequest(uint32 friendid, uint32 requestid)
{
	uint32 dwAccountId = 0;
	DatabaseMysql * mysql = WorldDb.GetDBConnection(dwAccountId);//我们只用一个库
	assert(mysql);

	return mysql->PExecute(
			"delete from user_friend_request where user_id = %u and request_id = %u ",
			friendid, requestid
	);
}

bool CDBCtrl::GetFriendRequestList(uint32 friendid, uint16 off, std::vector<stuPlayerPlus> &vecPlayer)
{
	DatabaseMysql * mysql = WorldDb.GetDBConnection(0);
	QueryResult *result = mysql->PQuery(
			"select user_id,name,level "
			"from user_game where user_id in (select request_id from user_friend_request where user_id = %u) and level != 0 "
		    "order by level desc limit %u,3",
			friendid, off
			);
//	IME_LOG("user_id %d off %u", friendid, off);

	if (result)
	{
		int i = 0;
		int j = 0;
		int number = result->GetRowCount();
		Field *fields = result->Fetch();
		vecPlayer.clear();
		stuPlayerPlus tmpPlayer;
		for (; j < number; j++)
		{
			i = 0;
			tmpPlayer.dwUserid = fields[i++].GetUInt32();
			tmpPlayer.strName = fields[i++].GetString();
			tmpPlayer.wLevel = fields[i++].GetUInt16();
			vecPlayer.push_back(tmpPlayer);
			result->NextRow();
			fields = result->Fetch();
		}

		delete result;
		return true;
	}
	return false;
}

bool CDBCtrl::GetUserDataOther(std::string &str, uint32 dwAccountId)
{
	DatabaseMysql * mysql = WorldDb.GetDBConnection(0);
	QueryResult *result = mysql->PQuery(
			"select role_other "
			"from user_game_blob2 where user_id = %u", dwAccountId
			);
	if (result)
	{
		int i = 0;
		Field *fields = result->Fetch();
		str.assign(fields[0].GetString(), fields[0].GetLength());

		delete result;
		return true;
	}

	return false;
}

void CDBCtrl::SaveUserDataOther(const std::string &strOther, uint32 dwId)
{
	DatabaseMysql * mysql = WorldDb.GetDBConnection(0);

        std::string str = strOther;
        mysql->escape_string(str);
	mysql->PExecute(
		"update user_game_blob2 set "
		"role_other = '%s' "
		"where user_id = %u",
		str.c_str(),
		dwId
		);
}

bool CDBCtrl::GetUserDataTimerGift(std::string &str, uint32 dwAccountId)
{
	DatabaseMysql * mysql = WorldDb.GetDBConnection(0);
	QueryResult *result = mysql->PQuery(
			"select timer_gift "
			"from user_game_blob2 where user_id = %u", dwAccountId
			);
	if (result)
	{
		int i = 0;
		Field *fields = result->Fetch();
		str.assign(fields[0].GetString(), fields[0].GetLength());

		delete result;
		return true;
	}

	return false;
}

void CDBCtrl::SaveUserDataTimerGift(const std::string &strOther, uint32 dwId)
{
	DatabaseMysql * mysql = WorldDb.GetDBConnection(0);

        std::string str = strOther;
        mysql->escape_string(str);
	mysql->PExecute(
		"update user_game_blob2 set "
		"timer_gift = '%s' "
		"where user_id = %u",
		str.c_str(),
		dwId
		);
}

bool CDBCtrl::InsertUserOneAciveData(uint32 userid, uint32 activeid, std::string str)
{
	uint32 dwAccountId = 0;
	DatabaseMysql * mysql = WorldDb.GetDBConnection(dwAccountId);//我们只用一个库
	assert(mysql);
	std::string con = str;
	mysql->escape_string(con);

	return mysql->PExecute(
			"insert into user_game_active_blob "
			"(user_id, active_id, active) "
			"values (%u, %u, '%s')",
			userid, activeid, str.c_str()
	);
}

bool CDBCtrl::GetUserOneActiveData(uint32 userid, uint32 activeid, std::string &str)
{
	DatabaseMysql * mysql = WorldDb.GetDBConnection(0);
	QueryResult *result = mysql->PQuery(
			"select active "
			"from user_game_active_blob where user_id = %u and active_id = %u",userid, activeid
			);
	if (result)
	{
		int i = 0;
		Field *fields = result->Fetch();
		str.assign(fields[0].GetString(), fields[0].GetLength());

		delete result;
		return true;
	}

	return false;
}

void CDBCtrl::SaveUserOneActiveData(const std::string &strActive, uint32 userid, uint32 activeid)
{
	DatabaseMysql * mysql = WorldDb.GetDBConnection(0);

        std::string str = strActive;
        mysql->escape_string(str);
	mysql->PExecute(
		"update user_game_active_blob set "
		"active = '%s' "
		"where user_id = %u and active_id = %u",
		str.c_str(),
		userid, activeid
		);
}

bool CDBCtrl::GetGlobalActiveData(uint32 activeid, std::string &str)
{
	DatabaseMysql * mysql = WorldDb.GetDBConnection(0);
	QueryResult *result = mysql->PQuery(
			"select content "
			"from active_global where active_id = %u", activeid
			);
	if (result)
	{
		int i = 0;
		Field *fields = result->Fetch();
		str.assign(fields[0].GetString(), fields[0].GetLength());

		delete result;
		return true;
	}

	return false;
}

bool CDBCtrl::InsertGlobalActiveData(uint32 activeid, std::string &str)
{
	uint32 dwAccountId = 0;
	DatabaseMysql * mysql = WorldDb.GetDBConnection(dwAccountId);//我们只用一个库
	std::string con = str;
	mysql->escape_string(con);

	return mysql->PExecute(
			"insert into active_global "
			"(active_id, content) "
			"values (%u, '%s')",
			activeid, con.c_str()
	);
}

void CDBCtrl::SaveGlobalActiveData(const std::string &strActive, uint32 activeid)
{
	DatabaseMysql * mysql = WorldDb.GetDBConnection(0);

        std::string str = strActive;
        mysql->escape_string(str);
	mysql->PExecute(
		"update active_global set "
		"content = '%s' "
		"where active_id = %u",
		str.c_str(),
		activeid
		);
}

bool CDBCtrl::GetUserDataAchieve(std::string &str, uint32 dwAccountId)
{
	DatabaseMysql * mysql = WorldDb.GetDBConnection(0);
	QueryResult *result = mysql->PQuery(
			"select achieve "
			"from user_game_blob2 where user_id = %u", dwAccountId
			);
	if (result)
	{
		int i = 0;
		Field *fields = result->Fetch();
		str.assign(fields[0].GetString(), fields[0].GetLength());

		delete result;
		return true;
	}

	return false;
}

void CDBCtrl::SaveUserDataAchieve(const std::string &strOther, uint32 dwId)
{
	DatabaseMysql * mysql = WorldDb.GetDBConnection(0);

        std::string str = strOther;
        mysql->escape_string(str);
	mysql->PExecute(
		"update user_game_blob2 set "
		"achieve = '%s' "
		"where user_id = %u",
		str.c_str(),
		dwId
		);
}

bool CDBCtrl::GetUserTrialData(std::string &str, uint32 dwAccountId)
{
	DatabaseMysql * mysql = WorldDb.GetDBConnection(0);
	QueryResult *result = mysql->PQuery(
			"select trial "
			"from user_game_blob2 where user_id = %u", dwAccountId
			);
	if (result)
	{
		int i = 0;
		Field *fields = result->Fetch();
		str.assign(fields[0].GetString(), fields[0].GetLength());

		delete result;
		return true;
	}

	return false;
}
void CDBCtrl::SaveUserTrialData(std::string str, uint32 dwId)
{
	DatabaseMysql * mysql = WorldDb.GetDBConnection(0);

    mysql->escape_string(str);
	mysql->PExecute(
		"update user_game_blob2 set "
		"trial = '%s' "
		"where user_id = %u",
		str.c_str(),
		dwId
		);
}

bool CDBCtrl::GetStoreData(uint32 ServerId, std::vector<STC_CONF_STORE> &vec)
{
	DatabaseMysql * mysql = WorldDb.GetDBConnection(0);
	QueryResult *result = mysql->PQuery(
			"select store_id, store_type, "
			"goods_id, view_type, consume_type, consume_number, is_buy, view_order, content, limit_number, role_limit, store_number, gift_number, view_number, max_number, channel, goods_type "
			"from store where server_id = %u",
			ServerId
			);

	if (result)
	{
		int i = 0;
		int j = 0;
		int number = result->GetRowCount();
		Field *fields = result->Fetch();
		vec.clear();
		STC_CONF_STORE tmp;
		for (; j < number; j++)
		{
			i = 0;
			tmp.dwStoreIndex = fields[i++].GetUInt32();
			tmp.byStoreChannel = fields[i++].GetUInt8();
			tmp.dwIndex = fields[i++].GetUInt32();
			tmp.byType = fields[i++].GetUInt16();
			tmp.byObjType = fields[i++].GetUInt8();
			tmp.dwObjNumber = fields[i++].GetUInt32();
			tmp.byIsBuy = fields[i++].GetUInt8();
			tmp.dwViewOrder = fields[i++].GetUInt32();
			tmp.strContent = fields[i++].GetCppString();
			tmp.dwLimitNumber = fields[i++].GetUInt32();
			tmp.dwRoleLimitNumber = fields[i++].GetUInt32();
			tmp.dwIndexNumber = fields[i++].GetUInt32();
			tmp.dwSendPrize = fields[i++].GetUInt32();
			tmp.dwViewPrize = fields[i++].GetUInt32();
			tmp.dwMaxBuy = fields[i++].GetUInt32();
			tmp.dwKind = fields[i++].GetUInt32();
			tmp.byGoodsType = fields[i++].GetUInt8();
			vec.push_back(tmp);
			result->NextRow();
			fields = result->Fetch();
		}

		delete result;
		return true;
	}
	return false;
}

bool CDBCtrl::ReloadStoreData(uint32 ServerId, std::vector<STC_CONF_STORE> vec)
{
	DatabaseMysql *mysql = WorldDb.GetDBConnection(0);
	mysql->PExecute(
		"delete from store "
		"where server_id = %u",
		ServerId
		);

	for (int i = 0; i < vec.size(); i++)
	{
		mysql->escape_string(vec[i].strContent);
		if (!mysql->PExecute(
					"insert into store(server_id,store_id,store_type,goods_id,view_type,"
					"consume_type,consume_number,is_buy,view_order,content,"
					"limit_number,role_limit,store_number,gift_number,"
					"view_number,max_number,channel,goods_type) "
				   	"values(%u,%u,%u,%u,%u,"
					"%u,%u,%u,%u,'%s',"
					"%u,%u,%u,%u,"
					"%u,%u,%u,%u)",
					ServerId, vec[i].dwStoreIndex, vec[i].byStoreChannel, vec[i].dwIndex, vec[i].byType,
					vec[i].byObjType, vec[i].dwObjNumber, vec[i].byIsBuy, vec[i].dwViewOrder, vec[i].strContent.c_str(),
					vec[i].dwLimitNumber, vec[i].dwRoleLimitNumber, vec[i].dwIndexNumber, vec[i].dwSendPrize,
					vec[i].dwViewPrize, vec[i].dwMaxBuy, vec[i].dwKind, vec[i].byGoodsType
					))
		{
			IME_ERROR("insert into data fail serverid %u", ServerId);
			return false;
		}
	}
	return true;
}

void CDBCtrl::SaveGlobalData(const std::string &strtemp, uint32 id)
{
	DatabaseMysql * mysql = WorldDb.GetDBConnection(0);

        std::string str = strtemp;
        mysql->escape_string(str);
		mysql->PExecute(
				"update global_data set "
				"content = '%s' "
				"where id = %u",
				str.c_str(),
				id
				);
}

bool CDBCtrl::GetGlobalData(uint32 id, std::string &str)
{
	DatabaseMysql * mysql = WorldDb.GetDBConnection(0);
	QueryResult *result = mysql->PQuery(
			"select content "
			"from global_data where id = %u", id
			);
	if (result)
	{
		int i = 0;
		Field *fields = result->Fetch();
		str.assign(fields[0].GetString(), fields[0].GetLength());

		delete result;
		return true;
	}
	return false;
}

bool CDBCtrl::GetUserName(std::set<std::string> &setUserName)
{

	DatabaseMysql * mysql = WorldDb.GetDBConnection(0);
	QueryResult *result = mysql->PQuery(
			"select name "
			"from user_game where level != 0"
			);
	if (result)
	{
		int i = 0;
		int j = 0;
		int number = result->GetRowCount();
		Field *fields = result->Fetch();
		setUserName.clear();
		for (; j < number; j++)
		{
			i = 0;
			std::string tmp = fields[i++].GetCppString();
			setUserName.insert(tmp);
			result->NextRow();
			fields = result->Fetch();
		}

		delete result;
		return true;
	}
	return false;
}

bool CDBCtrl::GetAllUserId(std::vector<uint32_t> &vecuserid)
{

	DatabaseMysql * mysql = WorldDb.GetDBConnection(0);
	QueryResult *result = mysql->PQueryStrict(
			"select user_id "
			"from user_game"
			);
	if (result)
	{
		int i = 0;
		int j = 0;
		int number = result->GetRowCount();
		Field *fields = result->Fetch();
		vecuserid.clear();
		for (; j < number; j++)
		{
			i = 0;
			uint32_t tmp = fields[i++].GetUInt32();
			vecuserid.push_back(tmp);
			result->NextRow();
			fields = result->Fetch();
		}

		delete result;
		return true;
	}
	return false;
}

void CDBCtrl::SaveUserCardPacketDataOnlyForBase(const uint32 &dwAccountId, std::string base)
{
	DatabaseMysql * mysql = WorldDb.GetDBConnection(dwAccountId);

		mysql->escape_string(base);
	mysql->PExecute(
		"update user_game_blob set "
		"base_data = '%s' "
		"where user_id=%u",
		base.c_str(),
		dwAccountId
		);
}

//todo
bool CDBCtrl::GetRank(std::vector<STowerRank> &rank){
	DatabaseMysql * mysql = WorldDb.GetDBConnection(0);
	QueryResult *result = mysql->PQuery("select a.user_id,a.user_name,a.layer,a.user_level,a.master_index from rank a order by a.layer desc,a.user_level desc,a.break_blockade_times asc,a.ability desc limit 20;");
	if (result) {
		int count = result->GetRowCount();
		Field* fields = result->Fetch();
		for(int i = 0; i < count;i++,result->NextRow(),fields = result->Fetch()){
			//读取排名数据
			STowerRank userRank;
			int j = 0;
			userRank.userId = fields[j++].GetInt32();
			userRank.userName = fields[j++].GetCppString();
			userRank.layer = fields[j++].GetInt32();
			userRank.userLevel = fields[j++].GetInt32();
			userRank.masterIndex = fields[j++].GetInt32();
			//rank中数据的顺序即为排名顺序
			rank.push_back(userRank);
		}
		delete result;
		return true;
	}
	return false;
}

void CDBCtrl::SaveRank(uint32 userId,std::string userName,uint32 layer,uint32 times,uint32 userLevel,uint32 ability,uint32 masterIndex){
	DatabaseMysql * mysql = WorldDb.GetDBConnection(0);
	mysql->PExecute("update rank a set a.layer=%u,a.break_blockade_times=%u,a.user_level=%u,a.user_name='%s',a.ability=%u,a.master_index=%u where a.user_id = %u",layer,times,userLevel,userName.c_str(),ability,masterIndex,userId);
}
//void CDBCtrl::SaveRank(uint32 userId,std::string userName,uint32 layer,uint32 times,uint32 userLevel,uint32 ability){
//	DatabaseMysql * mysql = WorldDb.GetDBConnection(0);
//	mysql->PExecute("update rank a set a.layer=%u,a.break_blockade_times=%u,a.user_level=%u,a.user_name='%s',a.ability=%u where a.user_id = %u",layer,times,userLevel,userName.c_str(),ability,userId);
//}
bool CDBCtrl::GetUserTowerData(
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
			uint32 &isMaxLayer,uint32 &timestamp){
	DatabaseMysql * mysql = WorldDb.GetDBConnection(0);
	QueryResult *result = mysql->PQuery("select user_id,dw_index,two_attack,three_attack,refresh_attribute,attribute,break_blockade_times,buy_times,health,is_max_layer,timestamp from tower where user_id=%u", userId);
	if (result) {
		int count = result->GetRowCount();
		Field* fields = result->Fetch();
		for (int i = 0; i < count;
				i++, result->NextRow(), fields = result->Fetch()) {
			//读取排名数据
			int j = 0;
			j++; //不需要读取userId
			index = fields[j++].GetInt32();
			twoAttack = fields[j++].GetInt32();
			threeAttack = fields[j++].GetInt32();
			refreshAttribute = fields[j++].GetInt32();
			std::string attr = fields[j++].GetCppString();
			//处理attribute数据
			int temp = 0;
			int index = attr.find_first_of(',', temp);
			sword = atoi(attr.substr(temp, index).c_str());

			temp = index + 1;
			index = attr.find_first_of(',', temp);
			hand = atoi(attr.substr(temp, index).c_str());

			temp = index + 1;
			index = attr.find_first_of(',', temp);
			innerForce = atoi(attr.substr(temp, index).c_str());

			temp = index + 1;
			index = attr.find_first_of(',', temp);
			dark = atoi(attr.substr(temp, index).c_str());

			temp = index + 1;
			index = attr.find_first_of(',', temp);
			stick = atoi(attr.substr(temp, index).c_str());

			breakBlockadeTimes = fields[j++].GetInt32();
			buyTimes = fields[j++].GetInt32();
			health = fields[j++].GetInt32();
			isMaxLayer = fields[j++].GetInt32();
			timestamp = fields[j++].GetInt32();
		}
		delete result;
		return true;
	}
	return false;
}
bool CDBCtrl::SaveUserTowerData(
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
			uint32 isMaxLayer,uint32 timestamp){
	DatabaseMysql * mysql = WorldDb.GetDBConnection(0);
	//
	std::string attr = "";
	char temp[10];
	sprintf(temp,"%d",sword);
	attr += temp;
	sprintf(temp,"%d",hand);
	attr.append(",");
	attr.append(temp);
	sprintf(temp,"%d",innerForce);
	attr.append(",");
	attr.append(temp);
	sprintf(temp,"%d",dark);
	attr.append(",");
	attr.append(temp);
	sprintf(temp,"%d",stick);
	attr.append(",");
	attr.append(temp);

	//处理字符
	mysql->escape_string(attr);
	mysql->PExecute("update tower t set t.dw_index = %u,t.two_attack=%u,t.three_attack=%u,t.refresh_attribute=%u,t.attribute='%s',t.break_blockade_times=%u,t.buy_times=%u, t.health=%u,t.is_max_layer=%u,timestamp=%u where t.user_id = %u",
			index,twoAttack,threeAttack,refreshAttribute,attr.c_str(),breakBlockadeTimes,buyTimes,health,isMaxLayer,timestamp,userId);
	return true;
}

//重置tower的数据
void CDBCtrl::ResetUserTowerData(uint32 user_id,uint32 timestamp){
	DatabaseMysql * mysql = WorldDb.GetDBConnection(0);
	mysql->PExecute("update tower set dw_index = 0,break_blockade_times = 0, three_attack=0,two_attack=0,refresh_attribute=0,attribute='0,0,0,0,0',buy_times=0,health=0,is_max_layer=0,timestamp=%u where user_id = %u",timestamp,user_id);
}

//删除非机器人的排名，闯关的时候在写入排名表
void CDBCtrl::ClearRankData(){
	DatabaseMysql * mysql = WorldDb.GetDBConnection(0);
//	mysql->PExecute("delete from rank where user_id > %u or user_id < %u ", (uint32)(PLAYER_INIT_ID << 8),(uint32)(ROBOT_INIT_ID << 8));
	mysql->PExecute("delete from rank");
}

void CDBCtrl::InsertUserRank(uint32 userId,std::string userName,uint32 layer,uint32 times,uint32 userLevel,uint32 ability,uint32 masterIndex){

	DatabaseMysql * mysql = WorldDb.GetDBConnection(0);
	mysql->escape_string(userName);
	mysql->PExecute("insert into rank (user_id,layer,break_blockade_times,user_level,user_name,ability,master_index)values(%u,%u,%u,%u,'%s',%u,%u)",userId,layer,times,userLevel,userName.c_str(),ability,masterIndex);
}

bool CDBCtrl::GetRankByUserId(uint32 userId){
	DatabaseMysql * mysql = WorldDb.GetDBConnection(0);
	QueryResult *result = mysql->PQuery("select user_id from rank where user_id = %u", userId);
	if (result) {
		return true;
	}
	return false;
}

bool CDBCtrl::GetUserMultipleCardExp(uint32 userId,std::string &str){
	DatabaseMysql * mysql = WorldDb.GetDBConnection(0);
	QueryResult *result = mysql->PQuery("select multiple_card_exp from user_game_blob2 where user_id = %u",userId);
	if(result){
		Field *fields = result->Fetch();
		str.assign(fields[0].GetString(), fields[0].GetLength());
		delete result;
		return true;
	}
	return false;
}

void CDBCtrl::SaveUserMultipleCardExp(uint32 userId,std::string str){
	DatabaseMysql * mysql = WorldDb.GetDBConnection(0);
	mysql->escape_string(str);
	mysql->PExecute("update user_game_blob2 set multiple_card_exp = '%s' where user_id = %u",str.c_str(), userId);
}

bool  CDBCtrl::isLimitlogin(uint32 accountId){
	DatabaseMysql * mysql = WorldDb.GetDBConnection(0);
	QueryResult *result = mysql->PQuery("select limit_role from user_game where user_id = %u",accountId);
	if(result){
		Field *fields = result->Fetch();
		int i = 0;
		uint32 limit = fields[i++].GetUInt32();
		if(limit == 0){
			return false;
		}else{
			return true;
		}
	}
	return false;
}

void  CDBCtrl::setLimitRolelogin(uint32 accountId,uint32 limit){
	DatabaseMysql * mysql = WorldDb.GetDBConnection(0);
	mysql->PExecute("update user_game set limit_role = %u where user_id = %u",limit,accountId);
}


