#ifndef _ENUM_DEFINES_H
#define _ENUM_DEFINES_H

//各种enum值的定义
#define INNER_VERSION 8
#define DEBUG_LOG_FILTER_CHOOSE 0xffffffff
//#define DEBUG_LOG_FILTER_CHOOSE 0x0

#define MAIL_LIMIT 200
#define GROUP_MAIL_LIMIT 100
#define ROLE_CARD_PACKET_LIMIT 200
#define EQUIP_CARD_PACKET_LIMIT 150

#define ENERGY_SHOPID 10
#define STRENGTH_SHOPID 11
#define HERO_REQUEST_SHOPID 26
#define TRUMPET_SHOPID 27
#define AVOID_DURATION_SHOPID 12
#define TURNTABLE_SHOPID 14
#define SUPER_TURNTABLE_SHOPID 66
#define NEWGIFT2_SHOPID 21
#define NEWGIFT4_SHOPID 22
#define NEWGIFT6_SHOPID 23
#define NEWGIFT8_SHOPID 38
#define STAMINA_SHOPID 93

#define CARD_SWITCH_EXP_NORMAL 24
#define CARD_SWITCH_EXP_HIGH 25
#define SHOP_BEGINNER_GIFT_ID 20
#define VIP_GIFT_ARR {28,29,30,31,32,33,34,35,36,37,45}
#define VIP_MAX 11

#define CARD_BREAK_AWARD_COUNT 5
#define CARD_RARE_GROW 10
#define MAX_STATUS 2

#define SKILL_CRIT_FACTOR 150
#define RECOGNIZE_BUFFER_FACTOR 3

#define DEFENCE_SKILL_FACTOR 100
#define CARD_NORAML_ATTACK_HIT 100

#define PACKET_SPLIT_NUMBER 4096
#define INIT_CHAPTER_DURATION_MISSION 3001
#define INIT_DURATION_MISSION 300101
#define INIT_HIGH_DURATION_MISSION 400106

#define TARGET_TROUGTH_POWER 50
#define MAX_SYS_BROAD_USER_ONE_TIMES 500

#define IPHONE      0x1000FFFF

//#define SERVER_VERSION      g_nVersion
//#define CONVERT_VERSION(v)  v & 0x0000FFFF
#define E_MAX_USERNAME_PWD_LEN          64       //用户名和密码的最大长度

#define SP_STAR_LEVEL 5 
#define SP_LEVEL 8

#define CLIENT_ACTIVE_TIME 600
#define ONE_TIME_NUMBER 3

#define SKILL_ATTR_FACTOR 110

#define BREAK_LIMIT_PARA 0
#define AWAKE_PARA 10 
#define AWAKE_BASE_PARA 10

#define MAX_SEARCH_ITEM_NUMBER 3 

#define INIT_THREE_CARD {201,202,203,204}
#define INIT_EQUIP_CARD {10001,20001,40001}
#define INIT_CARD_ONE_STAR 16
#define INIT_CARD_TWO_STAR 101
#define INIT_CARD_MAX 4
#define INIT_SHOP_ITEM 20
#define INIT_FRIEND_BOSS_PIECES_DROP 60004
#define INIT_FRIEND_BOSS_HAPPEN 999

#define AWAKE_LEVEL_PARA 30
#define STRENGTH_LEVEL_LIMIT_PARA 3

#define	CARD_SWITCH_EXP_NORMAL_EFFECT 50
#define	CARD_SWITCH_EXP_HIGH_EFFECT 70 
#define MAX_FRIEND_NUMBER 100

#define GACHA_ONE_TIMES 10

#define MAX_WORLD_BOSS_CAPACITY 3000
#define MAX_WORLD_BOSS_PRIZE_NUMBER 150
#define MAX_WORLD_BOSS_ATTACKED_NUMBER 30

#define DURATION_HARM_FIXED_RATE 0.25
#define DURATION_MASTER_HARM_PARA 1
#define DURATION_MASTER_HEALTH_PARA 1

#define FRIEND_BOSS_HEALTH_UP_PARA 3

#define MAX_BOSS_RECORD 100
#define MAX_FRIEND_BOSS_PRIZE_RECORD 400

#define MAX_SHOP_BUY_NUMBER 999
#define MAX_BROADCAST_LENGTH 100

#define INIT_TIMER_GIFT_INDEX 0
#define DURATION_MISSION_BOSS_RANDOM_PARA 20
#define BREAK_AUTO_STAR_LEVEL_LIMIT 2

// caution: active blob storage column total 65535bytes. every active total 1500, special 3000
#define LOGIN_SIGN_ACTIVE_ID 1
#define CARD_MULTI_BOSS_ACTIVE_ID 2
#define FRIEND_BOSS_ACTIVE_ID 3
#define TURN_NORMAL_TABLE_ACTIVE_ID 4
#define TURN_SUPER_TABLE_ACTIVE_ID 5
#define MASTER_ACTIVE_ID 6
#define RECOVER_ACTIVE_ID 7
#define EXP_ADD_ACTIVE_ID 8
#define COIN_ADD_ACTIVE_ID 9
#define OBJ_DROP_ACTIVE_ID 10
#define FRIEND_BOSS_RANK_ACTIVE_ID 11
#define CHARGE_SINGLE_ACTIVE_ID 12
#define CHARGE_ACCUMULATE_ACTIVE_ID 13
#define CHARGE_CYCLE_ACTIVE_ID 14
#define LOGIN_CYCLE_ACTIVE_ID 15
#define LOGIN_EXTRA_ACTIVE_ID 16
#define TASK_ACTIVE_MAX 7
#define FUND_ACTIVE_ID 17
#define TASK1_ACTIVE_ID 18
#define TASK2_ACTIVE_ID 19
#define TASK3_ACTIVE_ID 20
#define TASK4_ACTIVE_ID 21
#define TASK5_ACTIVE_ID 22
#define TASK6_ACTIVE_ID 23
#define TASK7_ACTIVE_ID 24
#define CDKEY_ACITVE_ID 25

#define SWITCH_OBJ1_ACTIVE_ID 21
#define SWITCH_OBJ2_ACTIVE_ID 22
#define SWITCH_OBJ3_ACTIVE_ID 23
#define SWITCH_OBJ4_ACTIVE_ID 24
#define SWITCH_OBJ5_ACTIVE_ID 25
#define SWITCH_OBJ6_ACTIVE_ID 26
#define SWITCH_OBJ7_ACTIVE_ID 27
#define SWITCH_OBJ8_ACTIVE_ID 28
#define SWITCH_OBJ9_ACTIVE_ID 29
#define SWITCH_OBJ10_ACTIVE_ID 30

#define COMMON_ACTIVE_ID 99999

#define LOGIN_SIGN_ACTIVE_EXTRA_COUNT 0

#define ARENA_USER_GET_INTERVAL {{1,20}, {21,100}, {101,200}, {201, 500}, {501,1000}, {1001,5000},{5001,0}}
#define ARENA_USER_GET_INTERVAL_VAL {1,5,10,15,20,50,100}
#define ARENA_USER_GET_INTERVAL_MAX 7
#define ARENA_USER_PRIZE_INTERVAL {{1,1}, {2,2}, {3,3}, {4,6}, {7,10}, {11,20}, {21,50}, {51,100}, {101,500}, {501,1000}, {1001,0}}
#define ARENA_USER_PRIZE_INTERVAL_MAX 11
#define ARENA_USER_PRIZE_INTERVAL_VAL {350, 300, 250, 225, 200, 175, 150, 125, 100, 75, 50}

#define MAX_MASTER_SKILL_LV 9
#define MAX_FRIEND_BOSS_LV 1000
#define MAX_TOP_RANK_FLUSH 10

#define MAX_MASTER_LOW_S 0
#define MAX_MASTER_LOW_E 5
#define MAX_MASTER_MIDDLE_S 4 
#define MAX_MASTER_MIDDLE_E 6
#define MAX_MASTER_HIGH_S 5
#define MAX_MASTER_HIGH_E 7

#define RESOURCE_DOWNLOAD_NAME "res"
#define AD_DOWNLOAD_NAME "ad"
#define EXTRA_RESOURCE_DOWNLOAD_NAME "resextra"

#define SELL_CARD_COIN 60
#define HERO_REQUEST_SHOPID_COUNT_EVERY 5

#define TURNTABLE_TURN_COUNT_1 1
#define TURNTABLE_TURN_COUNT_10 10
#define TURNTABLE_TURN_COUNT_100 100

#define TURNTABLE_MAX_CARD_GET 10
#define TURNTABLE_CONTENT_COUNT 12

#define DURATION_MISSION_FULL_STAR 3
#define EVERY_MONTH_SIGN_DAY 30

#define CONSUME_MAX_LEVEL_CONDITION 20
#define MOST_MAX_HARM 9999999

#define DURATION_ROUND_CONDITION 2
#define DURATION_HEALTH_CONDITION 70

#define SKILL_CARD_LEVELUP_PRO_MAX 80
#define SKILL_CARD_LEVELUP_FAIL_PRO_MAX 5
#define SKILL_CARD_LEVELUP_STAR_UP 0.5
#define SKILL_CARD_LEVELUP_LEVEL_UP 0.5
#define SKILL_CARD_LEVELUP_RATE_PARA 10
#define LOW_MASTER_EVERVY_LIMIT 10

#define INIT_WORLDBOSS_TASK 300201
#define DOLLAR_LUCK_PARA 1

#define MAX_SWITCH_CARD_NUMBER 10
#define EVERY_USE_ENERGY 1
#define EVERY_USE_STRENGTH 1

#define MAX_EVERY_FRIEND_NUMBER 10
#define MAX_WORLD_CHAT_NUMBER 10
#define MAX_NOTICE_NUMBER 6

#define FIRST_CHARGE_MULTI_PARA GlobalConfig::FirstChargeMulti
#define BREAK_ATTRIBUTE_UP {10, 10, 10, 10, 10, 10, 10, 10, 10, 10}

#define CHARGE_DOUBLE_LIMIT GlobalConfig::ChargeSegmentLine
#define PLAYER_INIT_ID 2001
#define ROBOT_INIT_ID 1000

#define IS_ROBORT(userid) ((userid >> 8) < PLAYER_INIT_ID && (userid >> 8) > 1000)
#define MAX_FRIEND_BOSS_RANK_NUMBER 2000

#define BOSS_DEVOTE_PARA 10
#define BOSS_DEVOTE_AWAKE_PARA 10

#define MASTER_FORCE_BASE 500
#define MASTER_FORCE_GROW 2000
#define MASTER_EXP_MAX 1000.0

#define JOIN_EVERY_COUNT 1
#define JOIN_LIMIT_MAX_COUNT 100

#define ACTIVE_INIT_SPACE 300

#define CHANNEL_MIN 750
#define CHANNEL_MAX 800

#define CHANNEL_MIN2 10001
#define CHANNEL_MAX2 20000

#define CHANNEL_MIN3 40001
#define CHANNEL_MAX3 50000

#define MAX_JOIN_NUMBER 5
#define NETWORK_CHECK_SEC 20
#define MAX_PACKET_OVERLOAD 50
#define MAX_ACTIVE_PRIZE_NUMBER 50

#define TWELVE_CLOCK_SECOND 43200


typedef enum _E_ITEM_TYPE
{
	E_ITEM_PET_CARD,
	E_ITEM_WEAPON_CARD,
	E_ITEM_ARMOR_CARD,
	E_ITEM_JEWEL_CARD,
	E_ITEM_ACCESSORIES_CARD,
	E_ITEM_SKILL_CARD,
	E_ITEM_MASTER_CARD,
	E_ITEM_STUFF_CARD,
}E_ITEM_TYPE;

// 物品发放格式 type para1 para2
typedef enum _E_OBJ_TYPE
{
	E_OBJ_CARD = 1,                             	/* para1 index */
	E_OBJ_ITEM = 2,                                 /* para1 index para2 number */
	E_OBJ_SHOP = 3,                                 /* para1 index para2 number */
	E_OBJ_DOLLAR = 4,                               /* para2 number */
	E_OBJ_COIN = 5,                                 /* para2 number */
	E_OBJ_CARD_EXP = 6,                             /* para2 number */
	E_OBJ_SKILL = 7,                                /* para1 cardindex para2 lv */
	E_OBJ_AWAKE_CARD = 8,                       	/* para1 cardindex para2 awake */
	E_OBJ_WORLD_EXP = 9,                        	/* para2 number */
	E_OBJ_ROLE_EXP = 10,                        	/* para2 number */
	E_OBJ_ENERGY = 11,                          	/* para2 number */
	E_OBJ_STRENGTH = 12,                        	/* para2 number */
	E_OBJ_STAMINA = 13,                         	/* para2 number */
	E_OBJ_MAX = 14,

	E_OBJ_VIP_EXP = 99,                         	/* para2 number */
	// -----------------	
	E_OBJ_CARD_MORE = 100,                       	/* para1 index para2 number */

	// --------------- collection -------------------
	E_OBJ_COL_CARD_EXP = 21,                    	 /* para1 minexp para2 maxexp*/
	E_OBJ_COL_COIN = 22,                             /* para1 mincoin para2 maxcoin */
	E_OBJ_COL_CARD = 23,                             /* para1 cardcollection index */
	E_OBJ_COL_ITEM = 24,                             /* para1 itemcollection index */
	E_OBJ_COL_SHOP = 25,                             /* para1 shopcollection index */
	E_OBJ_COL_DOLLAR = 26,                           /* para1 mindollar para2 maxdollar */
	E_OBJ_COL_WORLD_EXP = 27,                        /* para1 min para2 max */
	E_OBJ_COL_CARD_PLUS = 28,                        /* para1 cardcollection index para2 pluspro*/
}E_OBJ_TYPE;

// 发放物品的结果 type para1 para2
typedef enum _E_OBJ_EFFECT
{
	E_EFFECT_CARD = 1,                          /* para1 objid*/
	E_EFFECT_ITEM,                              /* para1 index para2 number*/
	E_EFFECT_SHOP,                              /* para1 index para2 number*/
	E_EFFECT_DOLLAR,                            /* para1 number */
	E_EFFECT_COIN,                              /* para1 coin */
	E_EFFECT_CARD_EXP,                          /* para1 exp */
	E_EFFECT_SKILL,                             /* para1 objid para2 lv*/
	E_EFFECT_AWAKE_CARD,                        /* para1 objid */
	E_EFFECT_WORLD_EXP,                         /* para1 number */
	E_EFFECT_ROLE_EXP,                          /* para1 exp */
	E_EFFECT_ENERGY,                          	/* para1 number */
	E_EFFECT_STRENGTH,                        	/* para1 number */
	E_EFFECT_STAMINA,                         	/* para1 number */
	
	E_EFFECT_VIP_EXP = 99,                      /* para1 vipexp */
	// --------------
	E_EFFECT_CARD_MORE = 100,                   /* para1 index para2 number */

	// --------------- collection ---------------------
	E_EFFECT_COL_CARD_EXP = 21,                 /* para1 exp */
	E_EFFECT_COL_COIN,                          /* para1 coin */
	E_EFFECT_COL_CARD,                          /* para1 cardobjid */
	E_EFFECT_COL_ITEM,                          /* para1 itemid, para2 number*/
	E_EFFECT_COL_SHOP,                          /* para1 shopid, para2 number*/
	E_EFFECT_COL_DOLLAR,                        /* para1 dollar */
	E_EFFECT_COL_WORLD_EXP,                     /* para1 worldexp */
	E_EFFECT_COL_CARD_PLUS,                     /* para1 cardobjid*/
}E_OBJ_EFFECT;

typedef enum _E_MISSION_TYPE
{
	E_MISSION_MAIN = 0, 
	E_MISSION_BRANCH,
	E_MISSION_HIGH,                              /* 精英关卡 */
	E_MISSION_ACTIVE
}E_MISSION_TYPE;

typedef enum _E_ATTRIBUTE {
	NONE = 0,
	SWORD = 1,
	HAND,
	INTERNAL_FORCE,
	DARK,
	STICK,
    MASTER,
	MAX_ATTRIBUTE
}E_ATTRIBUTE;

// 随机事件 effect para1 para2 para3
typedef enum _E_EFFECT {
	EXP_UP = 1,                                 /* 经验上涨 para1(人物经验值) para2(card 经验上涨) */
	COIN_UP,                                    /* 获得额外金钱 para1(值) */
	CARD_GET,                                   /* 掉落卡牌 para1(星级) para2(卡牌objid) */
	ITEM_GET,                                   /* 掉落道具 para1(index)*/
	SHOP_GET,                                   /* 掉落商城道具 para1(index), para2(number)*/
	WORLD_EXP_UP,                               /* 江湖阅历 para1(value) */
}E_EFFECT;

// 做决斗的效果事件
typedef enum _E_EFFECT_DURATION {
	WEAPON_GET = 1                                 /* 武器防具获得 para1(objid) */
}E_EFFECT_DURATION;

// 做夺宝的效果事件
typedef enum _E_EFFECT_ROB {
	STUFF_GET = 1                                 /* 道具获得 para1(objid) */
}E_EFFECT_ROB;

// 邮件详细类别
typedef enum _E_DETAIL_TYPE_EMAIL {
	FRIEND_TEXT,                                /* 好友私信 */
//	FRIEND_REQUSET,                             /* 申请好友 */
//	FRIEND_AGREE,                               /* 同意加好友 */
//	FRIEND_DEL,                                 /* 删除加好友 */
//	DURATION_COIN,                        		/* 战斗抢走金钱 */
	ROB_GET,                        			/* 夺宝抢走碎片 */
	ROB_LOSE,                                   /* 夺宝战斗成功获取碎片失败 */
//	DURATION_LOSE,                              /* 战斗失败丢失金钱 */
	SYS_PAY,                                    /* 系统补偿 */
	SYS_GIFT,                                   /* 系统礼包 */
//	SYS_FRIEND_BOSS,                      		/* 好友BOSS邮件 */
	SYS_DIY,                                    /* 自定义邮件 */
	SYS_MAX
}E_DETAIL_TYPE_EMAIL;

//邮件类别
typedef enum _E_TYPE_EMAIL {       
	MAIL_ALL_TYPE,
	MAIL_FRIEND_TYPE,
	MAIL_SYSTEM_TYPE,
	MAIL_DURATION_TYPE
}E_TYPE_EMAIL;

//邮件动作类别
typedef enum _E_OPT_EMAIL {       
	MAIL_READ = 1,                              /* 邮件已读 */
}E_OPT_EMAIL;

// 邮件模版映射
typedef enum _E_TYPE_EMAIL_INDEX {
	FRIEND_TEXT_INDEX = 3,                                /* 好友私信 */
//	DURATION_COIN_INDEX = 1,                              /* 战斗获取金钱 */
	ROB_GET_INDEX = 1,                                 	  /* 夺宝获取碎片 */
	ROB_LOSE_INDEX = 2,                                   /* 夺宝战斗成功获取碎片失败 */
//	DURATION_LOSE_INDEX = 4,                              /* 战斗失败丢失金钱 */
	SYS_PAY_INDEX = 4,                               	  /* 系统补偿 */
	SYS_GIFT_INDEX = 5,                               	  /* 系统礼包 */
}E_TYPE_EMAIL_INDEX;

// 做决斗的技能效果
typedef enum _E_EFFECT_DURATION_SKILL {
	ATTACK_BUFF,                                /* 攻击BUFF para1(属性) para2(系数)*/
	DEFENCE_BUFF,                               /* 防御BUFF para1(属性) para2(系数)*/
	DEFENCE_DEBUFF,                             /* 防御DEBUFF para1(属性) para2(系数)*/
	IGNORE_DEFENCE,                             /* 无视防御技能 para1(属性) */
	CHANGE_PEER_ATTRIBUTE,                      /* 改变对方卡组属性 para1(属性) */
	ATTACK_CRIT,                                /* 攻击暴击  para1(属性) para2(系数)*/
	ATTACK_DEBUFF,                              /* 攻击DEBUFF para1(属性) para2(系数) */
	IGNORE_ATTACK,                              /* 无视攻击技能 para1(属性) */
	CHANGE_ATTRIBUTE,                           /* 改变本方属性 para1(属性) */
	DEFENCE_CRIT,                               /* 防御暴击技能 para1(属性) para2(系数) */
	ALL_ATTACK_BUFF,                            /* 全员攻击BUFF para1(属性) para2(系数) */
	ALL_DEFENCE_DEBUFF,                         /* 全员防御DEBUFF para1(属性) para2(系数) */
	ALL_DEFENCE_BUFF,                           /* 全员防御BUFF para1(属性) para2(系数) */
	ALL_ATTACK_DEBUFF                           /* 全员防御DEBUFF para1(属性) para2(系数) */
	
}E_EFFECT_DURATION_SKILL;

//登陆反馈信息
typedef enum _E_ERRNO_AUTH
{
	E_ERRNO_AUTH_SUCCESS					= 0,	//验证通过
	E_ERRNO_AUTH_NAME_LEN					= 401,	//验证用户名长度不正确
	E_ERRNO_AUTH_PWD_LEN					= 402,	//验证密码长度不正确
	E_ERRNO_AUTH_WRONG_PWD					= 403,	//密码错误
	E_ERRNO_AUTH_NO_ACCOUNT					= 404,	//帐号不存在
	E_ERRNO_AUTH_NO_SERVER					= 405,	//服务器维护中
	E_ERRNO_AUTH_ACCOUNT_HAVE				= 406,	//帐号已经存在
	E_ERRNO_AUTH_REG_SUCCESS				= 407,	//注册成功
	E_ERRNO_AUTH_WRONG_VERSION				= 408,	//版本不正确
	E_ERRNO_AUTH_NO_PWD						= 409,	//密码没有修改
	E_ERRNO_AUTH_UNVALID					= 410,	// 账号不合法
	E_ERRNO_AUTH_SYS						= 411,	// 系统错误 
	E_ERRNO_AUTH_CHANNEL					= 412,	// 登录渠道错误 
	E_E_ERRNO_AUTH_LIMIT                    = 413,  //登录角色限制
}E_ERRNO_AUTH;


typedef enum _E_PROPERTY_ROLE
{
	E_ROLE_dwAccountId,
	E_ROLE_strAccount,
	E_ROLE_strUserName,
	E_ROLE_wLevel,
	E_ROLE_dwExp,
	E_ROLE_wVipLv,
	E_ROLE_dwVipExp,
	E_ROLE_dwDollar,
	E_ROLE_dwCoin,
	E_ROLE_dwEnergy,
	E_ROLE_dwEnergyMax,
	E_ROLE_dwEnergyRate,
	E_ROLE_dwStrength,
	E_ROLE_dwStrengthMax,
	E_ROLE_dwStrengthRate,
	E_ROLE_byTaskLocation1, 
	E_ROLE_byTaskLocation2,
	E_ROLE_byTaskLocation3,
	E_ROLE_byTaskLocation4,
	E_ROLE_byCardCapacity,
	E_ROLE_dwExplimit,
	E_ROLE_byCurrentTeam,
	E_ROLE_dwLastExplimit,
	E_ROLE_byFlag,  // 第一位表示已开通竞技场 第二位表示已加入公会
	E_ROLE_lFindGroupId,
	E_ROLE_dwAvoidDurationTime,
	E_ROLE_dwWorldHistory,                      // 江湖阅历
	E_ROLE_dwFriendLimit,                       // 好友上限 
	E_ROLE_dwGameFlow,                          // 引导进度
	E_ROLE_dwLuck,
	E_ROLE_byGm,
	E_ROLE_wNextVipLv,
	E_ROLE_dwNextVipExp,
	E_ROLE_dwChargeLuck,
	E_ROLE_dwStamina,
	E_ROLE_dwStaminaMax,
	E_ROLE_dwStaminaRate,
	E_ROLE_strSignature
}E_PROPERTY_ROLE;

typedef enum _E_PROPERTY_ROLE_CARD
{
    E_CARD_dwObjId,      //物品ID  32
    E_CARD_dwIndex,       //卡牌索引 32
    E_CARD_byType,        //卡牌类型 8 
    E_CARD_byAttribute,           //属性 8
    E_CARD_byStarLevel,        //卡牌星级 8
    E_CARD_wLevel,        //卡牌等级 16
    E_CARD_dwExp,        //卡牌经验 32
    E_CARD_dwExpLimit,        //卡牌经验 32
    E_CARD_dwEffectIndex,       //效果索引 武器，防具，宝石卡牌使用
    E_CARD_byRare,         //稀有度 8
    E_CARD_dwLastExpLimit,       //上一级经验值 32
	E_CARD_byFlag, 				// 0 无意义 0x1 第一位表示满级
	E_CARD_dwSkillPara,           //技能效果
	E_CARD_dwAttack,				//攻击
	E_CARD_dwDefence,				//防御
	E_CARD_dwHealth,				//血
	E_CARD_dwAttackGrow,         //攻击成长 32
	E_CARD_dwDefenceGrow,         //防御成长 32
	E_CARD_dwHealthGrow,         //生命成长 32
	E_CARD_dwExpGrow,         //经验成长 32
	E_CARD_dwCritRate,         //暴击率 32
	E_CARD_dwSuitIndex,         //套装索引 32
	E_CARD_dwMasterSkillIndex,
	E_CARD_wMasterSkillLv,
	E_CARD_dwSkillIndex,			
	E_CARD_dwGrowLimit,
	E_CARD_dwLimitBreakNumber,
	E_CARD_dwLimitBreak,
	E_CARD_dwLimitBreakPro,
	E_CARD_dwAwakePro,
	E_CARD_dwAddExp,
	E_CARD_dwConsumeCoin,
	E_CARD_dwGetCoin,
	E_CARD_dwBreakExp,
	E_CARD_byMultiPower,
	E_CARD_dwMultiTime,
	E_CARD_dwOneLevelExp,
	E_CARD_dwTotalConsumeCoin,
	E_CARD_dwSkillPro,
	E_CARD_dwSkillPower,
	E_CARD_dwDurationExp,
	E_CARD_dwDurationCoin,
	E_CARD_dwObjDrop,
	E_CARD_dwStrengthRecover,
	E_CARD_dwEnergyRecover,
	E_CARD_dwAddHarm,
	E_CARD_dwReduceHarm,
	E_CARD_dwAddCritHarm,
	E_CARD_wLeaderLevel,
	E_CARD_dwLeaderIndex,
	E_CARD_dwCardGroup,
	E_CARD_dwMasterIndex,
	E_CARD_wAttackPlus,
	E_CARD_wDefencePlus,
	E_CARD_wHealthPlus
}E_PROPERTY_ROLE_CARD;

typedef enum _E_DOLLAR_TYPE
{
	E_CHARGE,
	E_CONSUME_GROUP,

}E_DOLLAR_TYPE;

typedef enum _E_GLOBAL_CONFIG
{
	E_GLOBAL_PRESTIGE = 1,
	E_GLOBAL_RANK_GET_NUMBER,
	E_GLOBAL_JOIN_GROUP_LIMIT,
	E_GLOBAL_CREATE_GROUP_LIMIT,
}E_GLOBAL_CONFIG;

typedef enum _E_GROUP_ADMIN_POSITION
{
	E_GA_PRESIDENT = 1,
	E_GA_VICE_PRESIDENT,
	E_GA_INNER_MINISTER,
	E_GA_OUT_MINISTER,
	E_GA_ELITE,
	E_GA_NORMAL
}E_GLOBAL_ADMIN_POSITION;

typedef enum _E_GROUP_ADMIN_AUTH
{
	E_GA_APPOINT,
	E_GA_LEVELUP,
	E_GA_MEMBER_IO,
	E_GA_UPDATE_NOTICE,
	E_GA_FLUSH_TASK,
	E_GA_MAKE_ALLIANCE,
	E_GA_FIGHT,
	E_GA_CREDIT
}E_GLOBAL_ADMIN_AUTH;

// 邮件类别
typedef enum _E_GROUP_DETAIL_TYPE_EMAIL {
	E_GDT_MAIL_REQUEST_ENTER,
	E_GDT_MAIL_AGREE_ENTER,
	E_GDT_MAIL_CANCEL,
	E_GDT_MAIL_DELETE,
}E_GROUP_DETAIL_TYPE_EMAIL;

//邮件类别
typedef enum _E_GROUP_TYPE_EMAIL {       
	E_GT_MAIL_SYSTEM,                                /* 系统邮件 */
}E_GROUP_TYPE_EMAIL;

//公会邮件模版
typedef enum _E_GROUP_TYPE_EMAIL_INDEX {       
	E_GT_MAIL_REQUEST_ENTER_INDEX = 1,
	E_GT_MAIL_AGREE_ENTER_INDEX = 2,
	E_GT_MAIL_CANCEL_INDEX = 4,
	E_GT_MAIL_DELETE_INDEX = 3,
}E_GROUP_TYPE_EMAIL_INDEX;

 
typedef enum _E_PROPERTY_GROUP
{
	E_GROUP_dwId,
	E_GROUP_strName,
	E_GROUP_dwCreateId,
	E_GROUP_strCreateName,
	E_GROUP_dwCredit,
	E_GROUP_wLevel,
	E_GROUP_wPersionLimit,
	E_GROUP_byPosion1Limit,
	E_GROUP_byPosion2Limit,
	E_GROUP_byPosion3Limit,
	E_GROUP_byPosion4Limit,
	E_GROUP_vecPosion1Persion,
	E_GROUP_vecPosion1PersionStr,
	E_GROUP_vecPosion2Persion,
	E_GROUP_vecPosion3Persion,
	E_GROUP_vecPosion4Persion,
	E_GROUP_vecPosion5Persion,
	E_GROUP_wPersionNumber,
	E_GROUP_strNotice,
	E_GROUP_dwNeedCredit,
	E_GROUP_dwCreditLimit,
	E_GROUP_dwMissionFlushTime,
	E_GROUP_byMissionFlushNumber,
	E_GROUP_dwMissionFlushPoint,
}E_PROPERTY_GROUP;

//钻石消费类别
typedef enum _E_DOLLAR_CHANGE {       
	E_DC_SHOP,                                  /* 2步 */
	E_DC_CREATE_GROUP,                          /* 2步 */
	E_DC_DEL_CD,                                /* 2步 */
	E_DC_CHARGE,                                /* 1步 */
	E_DC_GM,                                    /* 1步 */
	E_DC_GIVE,                                  /* 1步 */
	E_DC_MASTER,                                /* 2步 */
	E_DC_MASTER_RED,                            /* 2步 */
	E_DC_GACHA,                            		/* 1步 */
	E_DC_ACTIVE,                                /* 1步 */
	E_DC_CHANGE_ROLE,                                /* 1步 */
}E_DOLLAR_CHANGE;

//消CD商品类别
typedef enum _E_DELETE_CD {       
	E_DCD_DURATION_MISSION = 1,
	E_DCD_BRANCH_MISSION,
	E_DCD_HIGH_DURATION_MISSION,
	E_RECOVER_ENERGY,
	E_RECOVER_STRENGTH,
	E_INCREASE_ROLE_PACKET,
	E_INCREASE_EQUIP_PACKET,
	E_DCD_ACTIVE_DURATION_MISSION,
	E_RECOVER_STAMINA,
	E_TOWER_THREE_ATTACK, //通天塔中的购买三倍攻击
	E_TOWER_TWO_ATTACK,//通天塔中的购买两倍攻击
	E_TOWER_PURCHASE_BREAK_TIMES,//购买闯关次数
	E_TOWER_REFRESH_ATTRIBUTE,    //刷新属性
	E_DCD_ARENA_CHALLENGES
}E_DELETE_CD;

typedef enum _E_SKILL_TEAM 
{
	E_ST_ATTACK = 1,
	E_ST_DEFENCE = 2,
	E_ST_ASSIST = 3
}E_SKILL_TEAM;

typedef enum _E_CARD_RARE_TYPE
{
	E_CR_NORMAL = 0,
	E_CR_AWAKE = 1,
	E_CR_MAX = 2
}E_CARD_RARE_TYPE;

typedef enum _E_CARD_LINEUP_TYPE
{
	E_CL_MAX = 6,
}E_CARD_LINEUP_TYPE;

typedef enum _E_CARD_LINEUP_MASTER_TYPE
{
	E_CLM_MAX = 1,
}E_CARD_LINEUP_MASTER_TYPE;

//enum
//{
//	ACTION_1 = 2,	
//	ACTION_2 = 0,	
//	ACTION_3 = 4,	
//	ACTION_4 = 3,	
//	ACTION_5 = 1,	
//	ACTION_6 = 5
//};

//enum
//{
//	S_POISON = 1,
//	S_HALT = 2,
//	S_RECOGNIZE = 3,
//	S_ADD_HEALTH = 4,	
//	S_ADD_ATTACK = 5,
//	S_ADD_DEFENCE = 6,
//	S_REDUCE_ATTACK = 7,
//	S_REDUCE_DEFENCE = 8,
//	S_DEL_BUFFER = 9,
//	S_UNVALID_BUFFER = 10,
//	S_TARGET = 11
//};

enum
{
	ACTION_ATTACK = 1,
	ACTION_DEFENCE = 2,
};

//enum
//{
//	ACTION_BUFFER_ADD = 1,
//	ACTION_BUFFER_EFFECT = 2,
//	ACTION_BUFFER_DEL = 3,
//	ACTION_BUFFER_UPDATE = 4
//};
//
//enum
//{
//	TARGET_ATTACK_ONE = 1,
//	TARGET_ATTACK_COLUMN = 2,
//	TARGET_ATTACK_ONE_TROUGTH = 3,
//	TARGET_ATTACK_FRONT_LINE = 4,
//	TARGET_ATTACK_BEHIND_LINE = 5,
//	TARGET_ATTACK_ALL = 6,
//	TARGET_DEFENCE_ONE = 7,
//	TARGET_DEFENCE_ALL = 8,
//	TARGET_DEFENCE_MYSELF = 9
//};

enum
{
	DURATION_MISSION_NPC = 0,
	DURATION_MISSION_BOSS = 1
};

enum
{
	MASTER_LOW = 1,                             /* 百里寻师 */
	MASTER_MIDDLE = 2,                          /* 千里寻师 */
	MASTER_HIGH = 3                             /* 万里寻师 */
};

enum
{
	SYS_BROAD_EGG = 1,
	SYS_BROAD_STRENGTH_CARD = 2,
	SYS_BROAD_OPEN_SPECIAL_BOX = 3,
	SYS_BROAD_MASTER_GET_SKILL = 4,
	SYS_BROAD_SKILL_LV_UP = 5,
	SYS_BROAD_GET_MASTER_CARD = 8,
	SYS_BROAD_VIP_ONLINE = 9,
	SYS_BROAD_BOSS_HAPPEN = 10,
	SYS_BROAD_DIY = 99
};

enum
{
	TUJIAN_CARD = 1,
	TUJIAN_MASTER = 2
};

enum
{
	E_AREA_STATUS_STOP		,//维护
	E_AREA_STATUS_GOOD = 1	,//良好
	E_AREA_STATUS_BUSY		,//拥挤
	E_AREA_STATUS_FULL		,//火爆
};

enum
{
	E_CHANNEL_IOS,
	E_CHANNEL_ANDROID,
	E_CHANNEL_WIN
};

// functype para1 para2 para3
typedef enum EShopUseEffect 
{
	EGG_EFFECT = 1,                             /* 非商城使用 */
	GIFT_EFFECT,                                /* number 物品数量 para1-para3 E_OBJ_EFFECT一样*/
	ENERGY_ITEM_EFFECT,                         /* number 1 E_OBJ_EFFECT */
	STRENGTH_ITEM_EFFECT,                       /* number 1 E_OBJ_EFFECT */
	MISS_DURATION_EFFECT,                       /* none */
	COIN_PACKET_EFFECT,                         /* number 1 para1 获得的金钱 */
	COIN_BOX_EFFECT,                            /* none */
	BOX_EFFECT,                                 /* number 1 E_OBJ_EFFECT一样 */
	KEY_EFFECT,                                 /* number 1 E_OBJ_EFFECT一样 */
	MASTER_EFFECT,                              /* number 1 para1 选项, para2 masterindex, para3 flag 1:增长经验 0:无 */
	SWITCH_EFFECT,                             	/* 非商城使用 */
	UNIQUE_GIFT_EFFECT,							/* number 1 E_OBJ_EFFECT一样 */
	HERO_EFFECT,                                /* 非商城使用 */
	TRUMPET_EFFECT,                             /* 非商城使用 */
	CHIP_EFFECT,                                /* number 1 E_OBJ_EFFECT一样 */
	TURNTABLE_EFFECT,                           /* 非商城使用 */
	VIP_EFFECT,                           		/* number 1 E_OBJ_EFFECT一样 */
	ETC_EFFECT,                               	/* 非商城使用 */
	TURNTABLE2_EFFECT,                          /* 非商城使用 */
	ENERGY_ITEM_COUNT_EFFECT,                   /* number 1 E_OBJ_EFFECT一样 */
	STRENGTH_ITEM_COUNT_EFFECT,                 /* number 1 E_OBJ_EFFECT一样 */
	STAMINA_ITEM_EFFECT,                        /* number 1 E_OBJ_EFFECT一样 */
	STAMINA_ITEM_COUNT_EFFECT,                  /* number 1 E_OBJ_EFFECT一样 */
	TRIAL_ENTER_EFFECT,                         /* number 1 para1 index */
	ROLE_EXP_EFFECT,                            /* number 1 E_OBJ_EFFECT一样 */
	CARD_EXP_EFFECT,                            /* number 1 para1-para3 无意义*/
}EItemUse;

enum 
{
	FISRT_LINE,
	SECOND_LINE,
	MASTER_LINE
};

enum
{
	FOLLOWER_UPLINE,
	MASTER_UPLINE
};

enum
{
	CARD_CHANGE_ADD_LINE,
	CARD_CHANGE_SWITCH,
	CARD_CHANGE_DISBOARD,
	CARD_CHANGE_ADD_ATTACKTEAM
};

enum
{
	FRIEND_BOSS_STORY = 0,
	FRIEND_BOSS_ACTIVE = 1
};

enum
{
	FRIEND_BOSS_NORMAL = 0,
	FRIEND_BOSS_AWAKE = 1
};

enum 
{
	STORE_OBJ_COIN = 0,
	STORE_OBJ_DOLLAR = 1,
	STORE_OBJ_PRESTIGE = 2,
	STORE_OBJ_RMB = 3,
};

enum 
{
	STORE_DOLLAR = 0,
	STORE_PRESTIGE = 1, 
	STORE_CHARGE = 2,
	STORE_MAX,
};

enum
{
	DURATION_NORMAL = 0,
	DURATION_FRIEND_BOSS = 1
};

enum
{
	TURNTABLE_NORMAL_CHOOSE = 0,
	TURNTABLE_SUPER_CHOOSE = 1
};

enum
{
	TUJIAN_NONE = 0,
	TUJIAN_HAPPEN = 1,
	TUJIAN_GET = 2,
	TUJIAN_MAX
};

enum
{
	EVERYDAY_MISSION = 1,
	PROCESS_MISSION = 2,
	ACHIEVE = 3,
    SHARE = 4,
	ACTIVE_MISSION = 5
};

enum
{
	DIRECT_JUDGE = 0,
	ACCEPT_MISSION = 1
};

enum
{
	MISSION_NONE = 0,
	MISSION_PRIZE = 1,
	MISSION_COMPLETE = 2
};

enum
{
	EVERY_WORLD = 1,
	EVERY_MATCH,
	EVERY_MASTER,
	EVERY_ROB,
	EVERY_STRENGTH,
	EVERY_WORLDBOSS,
	GET_FOUR_CARD,
	GET_FIVE_CARD,
	NORMAL_TASK,
	HIGH_TASK,
	ROLE_LEVEL,
	CARD_LEVEL,
	CARD_BREAK_NUMBER,
	CONTINUE_LOGIN,
	ROB_SUCCESS = 15,
	LEARN_SKILL,
	CARD_AWAKE_NUMBER,
	BOSS_ATTACK,
	MATCH_RANK,
	FRIEND_NUMBER,
	AVOID_TIME,
	MEAT_COUNT,
	WINE_COUNT,
	HAVE_CARD,
	HAVE_SWORD_CARD,
	HAVE_HAND_CARD,
	HAVE_INTERNAL_FORCE_CARD,
	HAVE_DARK_CARD,
	HAVE_STICK_CARD,
	HAVE_FULL_BREAK_CARD,
	SIGN,
	FORCE_VALUE,
	WORLD_THREE_STAR,
	GET_CARD,
	SNS_SHARE,
	EVERY_SHOP,
	EVERY_GACHA,
	EVERY_CARD_UP,
	BREAK_TOWER
};

enum
{
	PUSH_DATA_MONEY = 1,
	PUSH_DATA_VIP = 2,
	PUSH_DATA_LVUP = 3,
	PUSH_DATA_BOSS = 4,
	PUSH_DATA_ACHIEVE = 5
};

enum
{
	STORE_KIND_IOS = 1,
	STORE_KIND_ANDROID = 2
};

enum
{
	NEXT_ENTER = 0,
	BOSS_HAPPEN = 1,
	COIN_GET = 2,
	ROLE_EXP = 3,
	CARD_EXP = 4,
	WORLD_EXP = 5,
	ITEM_DROP = 6,
	PIECES_DROP = 7,
	CARD_DROP = 8,
	ENERGY_RECOVER = 9,
	LOT_ENERGY_RECOVER = 10,
	STRENGTH_RECOVER = 11,
	LOT_STRENGTH_RECOVER = 12,
	STAMINA_RECOVER = 13,
	LOT_STAMINA_RECOVER = 14,
	HAPPEN_ROLE = 15,
};

enum
{
	PRODUCT_GET = 1,
	HAPPEN_BOSS = 2,
};

enum                                            /* resource source */
{
	SOURCE_OTHER				= 0,
	SOURCE_GROUP_CREATE_CONSUME = 1,
	SOURCE_SELL_CARD			= 2,
	SOURCE_INIT_ROLE			= 3,
	SOURCE_ITEM_USE				= 4,
	SOURCE_WORLDBOSS_DURATION	= 5,
	SOURCE_CHARGE				= 6,
	SOURCE_STORE_BUY			= 7,
	SOURCE_COIN_PACKET,
	SOURCE_BOX_PACKET,
	SOURCE_DCD_NORMAL_TASK,
	SOURCE_DCD_HIGH_TASK,
	SOURCE_DCD_BRANCH_TASK,
	SOURCE_ENERGY_RECOVER,
	SOURCE_STRENGTH_RECOVER,
	SOURCE_ROLE_PACKET,
	SOURCE_EQUIP_PACKET,
	SOURCE_DCD_ACTIVE_TASK,
	SOURCE_FIRST_PRIZE,
	SOURCE_WORLDBOSS_PRIZE,
	SOURCE_UNIQUE_GIFT,
	SOURCE_GIFT,
	SOURCE_CHIP,
	SOURCE_HERO_GACHA,
	SOURCE_DOLLAR_GACHA,
	SOURCE_WORLD_GACHA,
	SOURCE_UPGRADE,
	SOURCE_MASTER_REQUEST,
	SOURCE_SEND_RED,
	SOURCE_MASTER_BLESS,
	SOURCE_GM,
	SOURCE_TRIAL,
	SOURCE_TASK,
	SOURCE_COMBINE,
	SOURCE_ACHIEVE,
	SOURCE_ACTIVE,
	SOURCE_MAIL,
	SOURCE_CARDUP,
	SOURCE_ROB,
	SOURCE_STAMINA_RECOVER,
	SOURCE_INIT,
	SOURCE_SNS,
	SOURCE_CHANGE_ROLE_INFO,
	SOURCE_TOWER,
//	SOURCE_BREAK_BLOCKADE_TIMES_RECOVER,
//	SOURCE_REFRESH_ATTRIBUTE,
//	SOURCE_TWO_ATTACK,
//	SOURCE_THREE_ATTACK
	SOURCE_ALL_STAR,
	SOURCE_ACTIVE_MISSION,
	SOURCE_ARENA_CHALLENGES
};

enum
{
	CURRENCY_CNY = 99001,
	CURRENCY_USD = 99002,
	CURRENCY_TWD = 99003,
	CURRENCY_HKD = 99004
};

enum
{
	DB_GLOBAL_NOTICE = 1,
	DB_GLOBAL_CHAT = 2,
	DB_GLOBAL_WHITE_LIST = 3,
	DB_GLOBAL_DROP = 4,
	DB_GLOBAL_SERVER_OPERATIONS = 99,
};

enum
{
	ALL_ROLE_CARDPACKET = 1
};

enum
{
	ALL_STAR_NO = 0,
	ALL_STAR_RECV = 1,
	ALL_STAR_ALREADY_GET = 2
};

enum
{
	ACTIVE_PRIZE_NO = 0,	
	ACTIVE_PRIZE_RECV = 1,
	ACTIVE_PRIZE_ALREADY_GET = 2
};

#endif
