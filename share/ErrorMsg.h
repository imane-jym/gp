#ifndef _ERROR_MSG_H
#define _ERROR_MSG_H

//错误码信息
enum E_ERROR_MSG
{
	ERROR_MSG_SUCC						    = 0,	// 操作成功
	ERROR_MSG_NOT_ENOUGH_ENERGY	   		 	= 1,	// 没有足够多的活力值
	ERROR_MSG_NOT_ENOUGH_COIN			 	= 2,	// 没有足够多的金币

	ERRNO_MSG_EQUIP_NOT_IN_PACKET           = 3,  	/* 该装备不在背包里 */
	ERRNO_MSG_EQUIP_DUPLICATE_OBJID         = 4, 	/* 装备重复的装备了 */
	ERRNO_MSG_EQUIP_OVERLOAD                = 5,   /* 装备超过卡牌槽上限 */

	ERRNO_MSG_DATA_ILLEGAL          		= 6,     /* 发送的数据不合法 */
	ERRNO_MSG_EQUIP_DATA                	= 7,     /* 发送装备相关的数据包数据不合法 */
	ERRNO_MSG_EQUIP_FIRST_CARD              = 8,     /* 先要装备卡牌，然后才能武器，防具等 */

	ERRNO_MSG_CSV_LOAD   					= 9,     /* CSV 文件读取失败 */
	ERRNO_MSG_CSV_DATA       				= 10,    /* CSV 数据配置有问题 */

	ERRNO_MSG_MISSION_OVER_CURRENT       	= 11,     /* 请求的任务 超前了 */
	ERRNO_MSG_CALCULATE                     = 12,     /* 数据计算发生错误 */

	ERRNO_MSG_DB_NO_DATA_OR_ERROR       	= 13,   /* 数据库没有该数据或者发生错误 */
	ERRNO_MSG_NOT_ENOUGH_STRENGTH           = 14,   /* 体力不足 */
	ERRNO_MSG_PEER_NOT_ENOUGH_ITEM          = 15,   /* 对方没有足够的道具夺取 */

	ERRNO_MSG_NOT_ENOUGH_ITEM_COMBINE       = 16, 		/* 该用户没有足够的材料 */
	ERRNO_MSG_DB                         	= 17,       /* 数据库错误 */
	ERRNO_MSG_COMBINE_TIME_NOT             	= 18,    /* 合成物品时间还没有到 */
	ERRNO_MSG_ALREADY_ITEM_COMBINE          = 19,   /* 该物品正在合成 */

	ERRNO_MSG_CARD_NOT_IN_PACKET            = 20,   /* 该卡牌不在背包里 */
	ERRNO_MSG_NOT_ENOUGH_COIN               = 21,  /* 该用户没有足够的金钱 */
	ERRNO_MSG_NOT_ENOUGH_LEVEL          	= 22,    /* 该卡牌没有满级 */
	ERRNO_MSG_NOT_ALLOW_SAME_CARD          	= 23,    /* 不允许卡牌进化或者强化 祭祀用自己 */
	ERRNO_MSG_CONSUME_CARD_IN_EQUIP         = 24,  /* 该卡牌正在装备栏 */

	ERRNO_MSG_TECH_LEVEL_NOT_ARRIVE        	= 25,  	/* 该用户等级不满足科技升级的级别要求 */
	ERRNO_MSG_MAIL_NOT_VERIFY         		= 26,	/* 该邮件不可识别 */
	ERRNO_MSG_MAIL_ALREADY_HANDLE         	= 27,	/* 该邮件已被处理 */

	ERRNO_MSG_ARENA_NOT_ENOUGH_NUMBER      	= 28,   /* 该用户今天的挑战次数已经用完了 */
	ERRNO_MSG_ARENA_IN_CD     				= 29,	/* 该次挑战CD还在冷却中 */
	ERRNO_MSG_ARENA_NOT_OPEN          	    = 30,	/* 该用户没有参加竞技场 */
	ERRNO_MSG_ARENA_LEVEL_LIMIT            	= 31,	/* 该用户等级不满足开通竞技场的级别要求 */
	ERRNO_MSG_ARENA_INIT          		    = 32,	/* 初始化竞技场对象失败 */
	ERRNO_MSG_ARENA_NOT_ENOUGH_PRESTIGE    	= 33,   /* 没有足够的声望 */
	ERRNO_MSG_ARENA_ITEM_DOWN        	    = 34,	/* 该商品已下架 */
	ERRNO_MSG_ARENA_OVERLOAD_NUMBER      	= 35,  	/* 领取该商品超过最大次数 */

	ERRNO_MSG_MISSION_BOSS_DURATION     	= 36,  	/* 作BOSS任务战斗失败 */
	ERRNO_MSG_MISSION_IN_CD     			= 37,  	/* 任务在CD冷却中 */
	ERRNO_MSG_MISSION_DURATION_LOSE    		= 38,  	/* 任务战斗结果为 败 */
	ERRNO_MSG_MISSION_TYPE_UNVALID   		= 39,  	/* 请求的任务类型不合法 */
	ERRNO_MSG_MISSION_LEVEL_NOT_ARRIVE   	= 40,  	/* 该用户的等级没有达到 */

	ERRNO_MSG_ARENA_LEVEL_SMALL   			= 41,   /* 该用户在竞技场挑战级别小的 */
	ERRNO_MSG_ARENA_RANK_PRIZE 				= 42,   /* 该用户不满足在竞技场领取的商品的排名 */
	ERRNO_MSG_GROUP_LEVEL_LIMIT				= 43,   /* 该用户不满足创建工会的等级需求 */
	ERRNO_MSG_DOLLAR_NOT_ENGOUH				= 44,   /* 该用户元宝余额不足 */
	ERRNO_MSG_GROUP_INIT					= 45,   /* 工会对象初始化失败 */
	ERRNO_MSG_DOLLAR_CONSUME_FAIL			= 46,   /* 扣费失败 */
	ERRNO_MSG_ALREADY_IN_GROUP				= 47,   /* 已加入工会 */
	ERRNO_MSG_ALREADY_NOT_IN_GROUP			= 48,   /* 未加入工会 */
	ERRNO_MSG_ALREADY_FULL_LEVEL			= 49,   /* 该工会已满级 */
	ERRNO_MSG_CREDIT_NOT_ENOUGH				= 50,   /* 该工会荣誉值不足 */
	ERRNO_MSG_GROUP_NOT_AUTH				= 51,   /* 该用户没有该权限 */
	ERRNO_MSG_GROUP_NOT_LIVE				= 52,   /* 该公会不存在 */
	ERRNO_MSG_GROUP_PERSION_OVERLOAD		= 53,   /* 超过该工会人数限制 */
	ERRNO_MSG_GROUP_MISSION_OVERLOAD		= 54,   /* 公会任务完成超过最大次数 */
	ERRNO_MSG_GROUP_CREDIT_NOT_ENGOUH		= 55,   /* 公会成员个人荣誉不足 */
	ERRNO_MSG_GROUP_LEVEL_NOT_ARRIVAL		= 56,   /* 公会等级不满足升级科技要求 */
	ERRNO_MSG_GROUP_ALREADY_POSITION		= 57,   /* 公会成员已有职位 */
	ERRNO_MSG_GROUP_UNVALID_POSITION		= 58,   /* 公会 无效的任命职位 */
	ERRNO_MSG_GROUP_NOT_IN_POSITION			= 59,   /* 公会 成员不是这个职位 */
	ERRNO_MSG_GROUP_POSITION_NUMBER_LIMIT	= 60,   /* 公会 该职位 人数上限 */
	ERRNO_MSG_GROUP_CREATE_NO_CANCEL		= 61,   /* 公会 创始人不能退出公会 */
	ERRNO_MSG_SHOP_GET_ORDER_ID				= 62,   /* 商城获取订单号失败 */
	ERRNO_MSG_SHOP_MAX_NUMBER				= 63,   /* 商城购买物品最大次数 */
	ERRNO_MSG_SHOP_DEL_DOLLAR				= 64,   /* 商城扣费失败 */
	ERRNO_MSG_FRIEND_NUMBER_LIMIT			= 65,   /* 对方或者自己好友数量限制 */

	ERRNO_MSG_SHOP_ITEM_UNALLOW_USE			= 66,   /* 该物品不可使用 */
	ERRNO_MSG_SHOP_ITEM_USE_FAIL			= 67,   /* 该物品使用失败 */
	ERRNO_MSG_SKILL_COMBINE_FAIL			= 68,   /* 技能合成失败 */

	ERRNO_MSG_AVOID_DURATION				= 69,   /* 对方免战状态 */
	ERRNO_MSG_CARD_PACKET_NOT_ENOUGH		= 70,   /* 卡牌背包已满了 */

	ERRNO_MSG_SHOP_ITEM_HAVE_NOT			= 71,   /* 该用户没有该商场物品 */
	ERRNO_MSG_CARD_DUPLICATE_LINEUP			= 72,   /* 该用户重复上阵了 */
	ERRNO_MSG_CARD_LINEUP_NOT_OPEN			= 73,   /* 卡牌阵容位还没有开启 */
	ERRNO_MSG_CARD_LINEUP_NOT_IN_EQUIPMENT	= 74,   /* 要上阵容的卡牌不在卡牌位上 */

	ERRNO_MSG_MASTER_BLESS_IN_CD			= 75,   /* 上香在CD中 */
	ERRNO_MSG_DURATION_MISSION_OVER			= 76,   /* 战斗任务挑战次数已满 */
	ERRNO_MSG_DURATION_MISSION_OVER_CURRENT	= 77,   /* 请求的任务超前了 */
	ERRNO_MSG_CARD_FULL_LEVEL				= 78,   /* 该卡牌满级了 */

	ERRNO_MSG_GIFT_PACKET_IN_CD				= 79,   /* 获取礼包在CD中 */
	ERRNO_MSG_GIFT_PACKET_MAX_NUMBER		= 80,   /* 获取礼包已经搭到最大值了 */
	ERRNO_MSG_PREVIOUS_MASTER_NONE			= 81,   /* 之前的拜师信息为无 */
	ERRNO_MSG_MASTER_NOT_FOUND				= 82,   /* 无法发现这个拜师信息 */
	ERRNO_MSG_CHANGE_PWD_FAILED				= 83,   /* 改变密码失败 */
	ERRNO_MSG_WRONG_PWD						= 84,   /* 密码输入失败 */
	ERRNO_MSG_NO_BOX_KEY					= 85,   /* 没有相应匹配的箱子或者钥匙 */
	ERRNO_MSG_SHOP_NOT_SELL					= 86,   /* 该商品不可出售 */
	ERRNO_MSG_NOT_ALL_EMPTY					= 87,   /* 至少要上阵一个弟子和师傅 */
	ERRNO_MSG_MYSELF_AVOID_DURATION			= 88,   /* 自己处于免战状态 */
	ERRNO_MSG_LINEUP_ALREADY_CARD			= 89,   /* 该位置已存在卡牌 */
	ERRNO_MSG_CARD_LEVEL_OVERLOAD_ROLE		= 90,   /* 该卡牌等级已达到当前角色所允许的上限 */
	ERRNO_MSG_CARD_NOT_IN_ATTACKTEAM		= 91,   /* 该卡牌不在上阵阵容里 */
	ERRNO_MSG_CARD_SWITCH_LEVEL_ONE			= 92,   /* 传功消耗卡牌等级为1级 */
	ERRNO_MSG_CARD_LINEUP_SAME				= 93,   /* 不能上阵同样的卡牌 */
	ERRNO_MSG_NOT_ENOUGH_WORLD_EXP			= 94,   /* 没有足够的江湖阅历 */
	ERRNO_MSG_FRIEND_BOSS_NOT_FIND			= 95,   /* 没有找到相应的好友BOSS数据 */
	ERRNO_MSG_FRIEND_BOSS_INIT_FAIL			= 96,   /* 好友BOSS 初始化非法 */
	ERRNO_MSG_FRIEND_BOSS_GACHA_NO_PRIZE    = 97,   /* 好友BOSS 的该奖励 已领取或者消失了 */
	ERRNO_MSG_STR_HAVE_DIRTY    			= 98,   /* 该文本不合法 含有脏词或者敏感词 */
	ERRNO_MSG_ALREADY_REQUEST_FRIEND        = 99,   /* 已经提交过好友请求 */
	ERRNO_MSG_NO_REQUEST_FRIEND        		= 100,  /* 没有该好友请求 */
	ERRNO_MSG_CHAT_IN_CD        			= 101,  /* 聊天属于CD中 */
	ERRNO_MSG_SYS							= 102, 	/* 系统错误 */
	ERRNO_MSG_MYSELF_ADD_FRIEND_LIMIT		= 103, 	/* 自己超过加好友超过上限 */
	ERRNO_MSG_PEER_ADD_FRIEND_LIMIT			= 104, 	/* 对方超过加好友超过上限 */
	ERRNO_MSG_MYSELF_DEL_FRIEND_LIMIT		= 105, 	/* 自己删除好友超过上限 */
	ERRNO_MSG_PEER_DEL_FRIEND_LIMIT			= 106, 	/* 对方删除好友超过上限 */
	ERRNO_MSG_BROADCAST_OVER_MAX_LENGTH		= 107, 	/* 广播字长超过最大上限 */
	ERRNO_MSG_ALREADY_FRIEND				= 108, 	/* 已经是好友了 */
	ERRNO_MSG_ALREADY_NOT_FRIEND			= 109,	/* 已经不是好友了 */
	ERRNO_MSG_TIMER_GIFT_OVERLOAD			= 110,	/* 记时礼包超前了 */
	ERRNO_MSG_TIMER_GIFT_NOT_START			= 111,	/* 记时礼包还没有开始 */
	ERRNO_MSG_TIMER_GIFT_IN_CD				= 112,	/* 记时礼包在CD中 */
	ERRNO_MSG_SHOP_LEVEL_LIMIT				= 113,	/* 该商品使用等级限制 */
	ERRNO_MSG_FRIEND_BOSS_LEAVE				= 114,	/* 该好友BOSS已经离开或者已经被击杀 */
	ERRNO_MSG_LOGIN_SIGN_ACTIVE_MAX			= 115,	/* 登录签到补签超过最大次数 */
	ERRNO_MSG_LOGIN_ALREADY_SIGN			= 116,	/* 今天已经签过了 */
	ERRNO_MSG_ADD_FRIEND_MYSELF				= 117,	/* 不能加自己为好友 */
	ERRNO_MSG_FRIEND_LIMIT_MYSELF			= 118,	/* 自己好友数量达到上限*/
	ERRNO_MSG_FRIEND_LIMIT_PEER				= 119,	/* 对方好友数量达到上限*/
	ERRNO_MSG_INIT_ROLE						= 120,	/* 配置信息不合法 角色初始化失败*/
	ERRNO_MSG_GOODS_NOT_ENOUGH				= 121,	/* 物品道具不充足*/
	ERRNO_MSG_NO_PRESIGE					= 122,	/* 没有声望可以领取*/
	ERRNO_MSG_ENERGY_FULL					= 123,	/* 精力已满*/
	ERRNO_MSG_STRENGTH_FULL					= 124,	/* 体力已满*/
	ERRNO_MSG_SWITCH_CARD_FULL				= 125,	/* 被传功卡牌 满级 满破 已觉醒*/
	ERRNO_MSG_SWITCH_SAME_CARD				= 126,	/* 被传功卡牌 满级,只剩余破和觉醒*/
	ERRNO_MSG_DEL_DURATION_CHALLENGE_MAX	= 127,	/* 已达今天消除的最大次数 */
	ERRNO_MSG_ENERGY_USE_MAX				= 128,	/* 今日精力药使用已达上限 */
	ERRNO_MSG_STRENGTH_USE_MAX				= 129,	/* 今日体力药使用已达上限 */
	ERRNO_MSG_VIP_GIFT_NOT_ALLOW			= 130,	/* 你的VIP等级不足够买此礼包 */
	ERRNO_MSG_VIP_GIFT_ALREADY				= 131,	/* 你已经买过此VIP礼包了 */
	ERRNO_MSG_VIP_GIFT_OVERLOAD_MAX_COUNT	= 132,	/* VIP礼包不可买两次 */
	ERRNO_MSG_ACTIVE_NOT_OPEN				= 133,	/* 活动还没有开启 */
	ERRNO_MSG_CARD_ROLE_LEVEL_OVERLOAD_ROLE	= 134,  /* 传功时, 角色以达最大角色等级*/
	ERRNO_MSG_CARD_ROLE_PACKET_NOT_ENOUGH	= 135,  /* 侠客卡牌背包剩余空间不足了 */
	ERRNO_MSG_CARD_EQUIP_PACKET_NOT_ENOUGH	= 136,  /* 装备卡牌背包剩余空间不足了 */
	ERRNO_MSG_MASTER_ALREADY_BLESS			= 137,  /* 拜师奖励已领取 */
	ERRNO_MSG_ALREADY_USE_ENERY				= 138,  /* 本次活动的肉已吃过 */
	ERRNO_MSG_ALREADY_USE_STRENGTH			= 139,  /* 本次活动的酒已吃过 */
	ERRNO_MSG_ROLE_MAX_PACKET_LIMIT			= 140,  /* 已达最大侠客背包限制 */
	ERRNO_MSG_EQUIP_MAX_PACKET_LIMIT		= 141,  /* 已达最大装备背包限制 */
	ERRNO_MSG_ACHIEVE_NOT_COMPLETE			= 142,  /* 该成就未完成 */
	ERRNO_MSG_FIRST_PRIZE					= 143,  /* 未完成首冲 */
	ERRNO_MSG_ALREADY_GOODS_ROLE			= 144,  /* 超过角色购买数量上限 */
	ERRNO_MSG_ALREADY_GOODS_DAY				= 145,  /* 超过今日购买数量上限 */
	ERRNO_MSG_UPGRADE_NOT_ENOUGH			= 146,  /* 升阶材料不足 */
	ERRNO_MSG_UPGRADE_LEVEL_LIMIT			= 147,  /* 进阶等级限制 */
	ERRNO_MSG_UPGRADE_CARD_IN_EQUIP			= 148,  /* 卡牌已装备上 */
	ERRNO_MSG_CARD_GROUP_IN_TEAM			= 149,  /* 卡牌上阵同组了 */
	ERRNO_MSG_NOT_ARRIVE_CONDITION			= 150,  /* 未达到领取条件 */
	ERRNO_MSG_NOT_FIND_TARGET				= 151,  /* 未找到领取目标 */
	ERRNO_MSG_ALREADY_GET_TARGET			= 152, 	/* 你已经领取了这个活动奖励 */
	ERRNO_MSG_ALREADY_OVERLOAD_MAX			= 153, 	/* 你使用这个好友联手已达上限 */
	ERRNO_MSG_JOIN_UNALLOW					= 154, 	/* 本关卡不允许联手战 */
	ERRNO_MSG_SELL_UNALLOW					= 155, 	/* 本物品不允许贩卖 */
	ERRNO_MSG_NOT_ENOUGH_STAMINA	   		= 156,	/* 没有足够多的活力 */
	ERRNO_MSG_TRIAL_LEVEL	   		 		= 157,	/* 用户等级没有达到该层试炼要求 */
	ERRNO_MSG_TRIAL_CHALLENGE				= 158, 	/* 已达该层试炼的最大次数 */
	ERRNO_MSG_TRIAL_NEXT					= 159, 	/* 不满足进入下一层试炼的机会 */
	ERRNO_MSG_STAMINA_FULL					= 160,	/* 活力已满*/
	ERRNO_MSG_STAMINA_USE_MAX				= 161,	/* 今日活力药使用已达上限 */
	ERRNO_MSG_CDKEY_NONE					= 162, 	/* 没有该兑换码 */
	ERRNO_MSG_CDKEY_INVALID					= 163, 	/* 该兑换码已经过期了 */
	ERRNO_MSG_CDKEY_CHANNEL					= 164, 	/* 该兑换码不是你的渠道兑换码 */
	ERRNO_MSG_CDKEY_LIMIT_NUMBER			= 165, 	/* 本批次你已经领取超过最大次数 */
	ERRNO_MSG_CDKEY_ALREADY_USE				= 166, 	/* 该兑换码已经被使用过 */
	ERRNO_MSG_USER_NAME_ALREADY_USE			= 167, 	/* 该用户名已经被使用 */
	ERRNO_MSG_SAME_ACCOUNT_LOGIN			= 168, 	/* 相同的账号在其他地方登陆 */
	ERRNO_MSG_ALL_STAR_PRIZE_NOT_CONFORM	= 169, 	/* 本章节有些关卡没有打满三星 */
	ERRNO_MSG_HAVE_ALREADY_GET_PRIZE		= 170, 	/* 已经获得该奖励 */
	ERRNO_MSG_ACTIVE_NOT_ENOUGH				= 171, 	/* 活跃度不足以领取奖励 */
	ERRNO_MSG_NO_BREAK_TIMES                = 172,  /*塔的闯关次数已用完*/
	ERRNO_MSG_BUY_BREAK_BLOCKADE_TIMES_LIMIT          = 173,  /*购买闯关次数限制*/

//	E_ERRNO_AUTH_NAME_LEN					= 401,	//验证用户名长度不正确
//	E_ERRNO_AUTH_PWD_LEN,					= 402,	//验证密码长度不正确
//	E_ERRNO_AUTH_WRONG_PWD,					= 403,	//密码错误
//	E_ERRNO_AUTH_NO_ACCOUNT,				= 404,	//帐号不存在
//	E_ERRNO_AUTH_NO_SERVER,					= 405,	//服务器维护中
//	E_ERRNO_AUTH_ACCOUNT_HAVE,				= 406,	//帐号已经存在
//	E_ERRNO_AUTH_REG_SUCCESS,				= 407,	//注册成功
//	E_ERRNO_AUTH_WRONG_VERSION,				= 408,	//版本不正确
//	E_ERRNO_AUTH_NO_PWD,					= 409,	//密码没有修改
//	E_ERRNO_AUTH_UNVALID,					= 410,	// 账号不合法
//	E_ERRNO_AUTH_SYS,						= 411,	// 系统错误 
//	E_ERRNO_AUTH_CHANNEL					= 412,	// 登录渠道错误 
//  E_E_ERRNO_AUTH_LIMIT                    = 413,  //登录角色限制
};

#endif
