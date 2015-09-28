#ifndef _CMD_DEFINE_H
#define _CMD_DEFINE_H

//命令定义规范
//奇数为 Client 发送到 Server 
//偶数为 Server 发送到 Client
#define CMD_MAKE_CLIENT(v)			(((v)<<1)|0x0001)
#define	CMD_MAKE_SERVER(v)			((v)<<1)
#define CMD_IS_CLIENT(v)			(((v)&0x0001)==0x0001)
#define CMD_IS_SERVER(v)			(((v)&0x0001)==0x0000)

//Client 发送到 Server的命令
#define CMD_CS_TEST					CMD_MAKE_CLIENT(0x0001) //测试用 echo{string发送字符串}
#define CMD_CS_AUTH					CMD_MAKE_CLIENT(0x0002) //客户端请求认证{string account,string pwd,string version}
#define CMD_CS_GM_CMD			CMD_MAKE_CLIENT(0x0003) //GM命令{string}

#define CMD_CS_CARD_QUIPMENT_CHANGE				CMD_MAKE_CLIENT(0x0004) 
/*装备变更命令
{
      uint8 卡牌数目 攻击阵容
      [
          uint32卡牌objID
          ...
      ]
	  uint32 master objid
}
*/
#define CMD_CS_ITEM_QUIPMENT_CHANGE				CMD_MAKE_CLIENT(0x0005) //装备变更命令{uint8 team(0 pet, 1 master), uint8 卡牌位,uint32武器卡牌ID,uint32防具卡牌ID,uint32宝石卡牌ID,uint32 首饰卡牌ID, uint32 技能卡牌ID, uint32 师傅指点1, uint32 师傅指点2, uint32 师傅指点3}
#define CMD_CS_ACCEPT_SIMPLE_MISSION                           CMD_MAKE_CLIENT(0x0006) //领取任务{uint32 任务ID, uint8 1:attack 2: defence 不需要战斗的无视这个值}

#define CMD_CS_SEARCH_PLAYER                           CMD_MAKE_CLIENT(0x0007) //搜索玩家{string straccount} 
#define CMD_CS_AUTO_PLAYER                           CMD_MAKE_CLIENT(0x0008) //搜索玩家{unit8 自动匹配类型(暂时添0)} 
#define CMD_CS_DURATION_PREVIEW                          CMD_MAKE_CLIENT(0x0009) //战斗预览{unit32 玩家ID} 

#define CMD_CS_DURATION                          CMD_MAKE_CLIENT(0x000a) //决斗{unit32 玩家ID} 

//#define CMD_CS_CHANGE_TEAM                   CMD_MAKE_CLIENT(0x000b) //选择队伍{unit8 队伍编号目前就两个0,1} 
#define CMD_CS_SIMPLE_MISSION_INFO_REQUEST			CMD_MAKE_CLIENT(0x000c) // {uint32 任务ID}

#define CMD_CS_SEARCH_PLAYER_ITEM                       CMD_MAKE_CLIENT(0x000d) //搜索玩家{uint32材料index} 
#define CMD_CS_ROB                    CMD_MAKE_CLIENT(0x000e) //与玩家进行夺宝{unit32 玩家ID,uint32 材料index, uint8 flag 0:玩家 1:机器人} 

#define CMD_CS_ROB_PREVIEW                          CMD_MAKE_CLIENT(0x000f) //战斗预览{unit32 玩家ID} 

#define CMD_CS_CARD_COMBINE_REQUEST                         CMD_MAKE_CLIENT(0x0010) //合成请求{unit32 合成INDEX} 
#define CMD_CS_CARD_COMBINE_GET                         CMD_MAKE_CLIENT(0x0011) //合成装备获取{unit32 合成INDEX} 

//#define CMD_CS_CARD_EVOLUTION                        CMD_MAKE_CLIENT(0x0012) //合成请求{unit32 进化objid, uint32 被祭祀卡片objid} 
#define CMD_CS_SPECIAL_CARD_STRENGTH                       CMD_MAKE_CLIENT(0x0013) //合成请求{unit32 强化objid, uint16 number, [uint32 被祭祀卡片objid ... ]} 
#define CMD_CS_TECH_UPDATE                       CMD_MAKE_CLIENT(0x0014) //科技升级请求{uint32 升级科技ID} 
#define CMD_CS_FRIEND_AUTO_PLAYER_IN_FRIEND                           CMD_MAKE_CLIENT(0x0015) //查询玩家{unit8 自动匹配类型(暂时添0), uint16 off} 
#define CMD_CS_FRIEND_PLAYER_TEAM                           CMD_MAKE_CLIENT(0x0016) //查询玩家装备阵容{uint32 user_id} 
#define CMD_CS_FRIEND_AUTO_PLAYER_FOR_FRIEND                           CMD_MAKE_CLIENT(0x0017) //搜索玩家{unit8 自动匹配类型(暂时添0)} 
#define CMD_CS_FRIEND_ADD                          CMD_MAKE_CLIENT(0x0018) //申请加玩家为好友{uint32 user_id} 
#define CMD_CS_FRIEND_SEARCH_PLAYER                           CMD_MAKE_CLIENT(0x0019) //搜索玩家{std::string strname} 
#define CMD_CS_FRIEND_DEL                          CMD_MAKE_CLIENT(0x001a) //删除好友{uint32 user_id} 
#define CMD_CS_FRIEND_GET_NUMBER                         CMD_MAKE_CLIENT(0x001b) //请求好友数量{} 
#define CMD_CS_MAIL_DETAIL_INFO                         CMD_MAKE_CLIENT(0x001c) //请求邮件详情{uint32 userid, uint32 mail_id} 
#define CMD_CS_MAIL_HANDLE                         CMD_MAKE_CLIENT(0x001d) //请求邮件详情{uint32 userid, uint32 mail_id, uint8 handle(表明操作)} 
/*关于操作码
 * FRIEND_TEXT: 不关心 
 * DURATION_COIN: 不关心
 * ROB_GET: 不关心
 * ROB_LOSE: 不关心
 * DURATION_LOSE: 不关心
 * SYS_PAY: 0 同意接收
 * SYS_GIFT: 0 同意接收
 * SYS_SYS: 0 同意接收
 */
#define CMD_CS_MAIL_DEL                         CMD_MAKE_CLIENT(0x001e) //请求删除邮件{uint32 userid, uint32 mail_id} 
#define CMD_CS_FRIEND_MAIL                         CMD_MAKE_CLIENT(0x001f) // 给好友发送邮件{uint32 userid, string content} 

#define CMD_CS_ARENA_PLAYER_TEAM                           CMD_MAKE_CLIENT(0x0020) //查询玩家装备阵容{uint32 user_id} 
#define CMD_CS_ARENA_DURATION                         CMD_MAKE_CLIENT(0x0021) //决斗{unit32 玩家ID} 
#define CMD_CS_ARENA_OPEN                         CMD_MAKE_CLIENT(0x0022) //进入竞技场{} 
#define CMD_CS_ARENA_REQUEST_PRESTIGE_PRODUCT                           CMD_MAKE_CLIENT(0x0023) //{} 
//#define CMD_CS_ARENA_REQUEST_RANK_PRODUCT                           CMD_MAKE_CLIENT(0x0024) //{uint32 index} 
#define CMD_CS_MISSION_BRANCH_ACCEPT                         CMD_MAKE_CLIENT(0x0025) // {uint32 taskindex}

#define CMD_CS_GROUP_CREATE                          CMD_MAKE_CLIENT(0x0026) //{string 公会名字} 
#define CMD_CS_GROUP_AUTO_INFO                        CMD_MAKE_CLIENT(0x0027) //{uint32 off} 
#define CMD_CS_GROUP_SEARCH                       CMD_MAKE_CLIENT(0x0028) //{string 公会名字} 
#define CMD_CS_GROUP_LEVELUP                      CMD_MAKE_CLIENT(0x0029) //{} 
#define CMD_CS_GROUP_MAIL_INFO                        CMD_MAKE_CLIENT(0x002a) //{uint32 off} 
#define CMD_CS_GROUP_REQUSET_ENTER                        CMD_MAKE_CLIENT(0x002b) //{uint64 groupid, string content} 
#define CMD_CS_GROUP_MAIL_OPT                         CMD_MAKE_CLIENT(0x002c) //请求邮件详情{uint32 mail_id, uint8 handle(表明操作)} 
/*关于操作码
 * E_GT_MAIL_REQUEST_ENTER:1 同意， 2: 拒绝
 */
#define CMD_CS_GROUP_MISSION_ACCEPT                         CMD_MAKE_CLIENT(0x002d) //接收任务{uint32 taskindex} 
#define CMD_CS_GROUP_REQUSET_PRIZE                         CMD_MAKE_CLIENT(0x002e) //请求奖励{uint32 奖励Index} 
#define CMD_CS_GROUP_TECH_UPDATE                        CMD_MAKE_CLIENT(0x002f) //科技升级请求{uint32 升级科技ID} 
#define CMD_CS_GROUP_MEMBER_INFO                        CMD_MAKE_CLIENT(0x0030) //公会成员拉取{uint32 off} 
#define CMD_CS_GROUP_INFO                        CMD_MAKE_CLIENT(0x0031) //公会排行拉取{uint32 off} 
#define CMD_CS_GROUP_CHAT                        CMD_MAKE_CLIENT(0x0032) //公会聊天{string str, string name} 
#define CMD_CS_GROUP_APPOINT                        CMD_MAKE_CLIENT(0x0033) //公会任命人员{uint32 userid, string username, uint8 position} 
#define CMD_CS_GROUP_UNAPPOINT                        CMD_MAKE_CLIENT(0x0034) //公会解任人员{uint32 userid, string username, uint8 position} 
#define CMD_CS_GROUP_RANK_SEARCH                       CMD_MAKE_CLIENT(0x0035) //{string 公会名字} 
#define CMD_CS_GROUP_MEMBER_SEARCH_INFO                        CMD_MAKE_CLIENT(0x0036) //公会成员查询{string str} 
#define CMD_CS_GROUP_DETAIL_INFO                        CMD_MAKE_CLIENT(0x0037) //公会数据详情{uint64 groupid} 
#define CMD_CS_GROUP_MODIFY_INFO                        CMD_MAKE_CLIENT(0x0038) //修改公会公告{string notice} 
#define CMD_CS_GROUP_EXIT                        CMD_MAKE_CLIENT(0x0039) //退出公会{} 
#define CMD_CS_GROUP_DELETE_MEMBER                        CMD_MAKE_CLIENT(0x003a) //把别人踢出公会{uint32 userid} 
#define CMD_CS_GROUP_UNAPPOINT_MEMBER_INFO                        CMD_MAKE_CLIENT(0x003b) //拉取公会成员 
#define CMD_CS_GROUP_CANCEL                        CMD_MAKE_CLIENT(0x003c) //解散公会 {}
#define CMD_CS_SHOP_BUY                        CMD_MAKE_CLIENT(0x003d) //购买商城道具 {uint32 storeid, uint32 number}

#define CMD_CS_SEARCH_WIN		CMD_MAKE_CLIENT(0x003e) //{}

#define CMD_CS_SHOP_USE                        CMD_MAKE_CLIENT(0x003f) //使用商城道具 {uint32 shopid}
//#define CMD_CS_SKILL_FLUSH                     CMD_MAKE_CLIENT(0x0040) //合成技能 {uint32 card objid, uint8 position卡牌技能位, uint32 属性石, uint32 阵容之石, uint32 概率之石,  uint32 技能书}
#define CMD_CS_SHOP_BUY_DEL_CD                     CMD_MAKE_CLIENT(0x0041) //消CD {uint32 CD商品类别, int32 para1, int32 para2(E_DCD_DURATION_MISSION: para1 任务ID, para2 没有用, E_DCD_BRANCH_MISSION: para1 任务ID, para2 没有用, E_DCD_HIGH_DURATION_MISSION: para1 任务ID, para2 没有用, E_RECOVER_ENERGY: para1 没有用, para2 没有用, E_RECOVER_STRENGTH: para1 没有用, para2 没有用, E_INCREASE_ROLE_PACKET: para1 没有用, para2 没有用, E_INCREASE_EQUIP_PACKET: para1 没有用, para2 没有用, E_DCD_ACTIVE_DURATION_MISSION: para1 任务ID, para2 没有用, E_RECOVER_STAMINA: para1 没有用, para2 没有用,E_TOWER_THREE_ATTACK:para1 无，para2 无; E_TOWER_TWO_ATTACK:para1 无，para2 无, E_TOWER_PURCHASE_BREAK_TIMES:para1 num，para2 无; E_TOWER_REFRESH_ATTRIBUTE:para1 index，para2 无; E_DCD_ARENA_CHALLENGES:para1 无, para2 无)}
#define CMD_CS_CARD_LINEUP_CHANGE					CMD_MAKE_CLIENT(0x0042) // 改变阵型 {uint32 objid, uint32 objid, uint32 objid ... 目前总共六个, 从一号位依次到六号位}
#define CMD_CS_MASTER_REQUEST					CMD_MAKE_CLIENT(0x0043) // 拜师请求 {uint32 拜师选择(1:百里 2:千里 3 万里)}
#define CMD_CS_MASTER_YES_OR_NO					CMD_MAKE_CLIENT(0x0044) // 同意或拒绝这个师傅 {uint32 (0:同意 1:拒绝), uint32 master}
#define CMD_CS_MASTER_SEND_RED					CMD_MAKE_CLIENT(0x0045) // 塞红包 {uint32 拜师index}
#define CMD_CS_MASTER_BLESS					CMD_MAKE_CLIENT(0x0046) // 上香 {uint32 拜师index}
#define CMD_CS_DURATION_MISSION_ACCEPT					CMD_MAKE_CLIENT(0x0047) // 接收任务 {uint32 任务index, uint32 userid(0:单挑, 其他 userid 表示联合进攻)}
#define CMD_CS_NORMAL_CARD_STRENGTH                       CMD_MAKE_CLIENT(0x0048) //合成请求{unit32 强化objid, uint32 number合成花费钱币(0:直接升一级, 其他:花费金钱数)} 
#define CMD_CS_SELL_CARD                       CMD_MAKE_CLIENT(0x0049) //卖卡请求{uint16 number [unit32 强化objid...], uint16 number [{uint32 itemid, uint32 number} ... ] } 
#define CMD_CS_REQUEST_GIFT_PACKET                       CMD_MAKE_CLIENT(0x004a) //{} 

#define CMD_CS_LOGIN          CMD_MAKE_CLIENT(0x004b) //客户端登录 {std::string uid(不关心), uint32版本号 string name  string pwd uint16 channel, uint16 serverid}
#define CMD_CS_FASTLOGIN      CMD_MAKE_CLIENT(0x004c) //客户端登快速登录 {std::string uid, uint32版本号 string name(不关心)  string pwd uint16 channel, uint16 serverid}
#define CMD_CS_OTHERLOGIN        CMD_MAKE_CLIENT(0x004d) //第三方客户端登录 {std::string uid(不关心), uint32版本号 string name  string pwd uint16 channel, uint16 serverid, uint8 flag}
#define CMD_CS_CHANGE_PWD        CMD_MAKE_CLIENT(0x004e) //修改密码 {std::string oldpwd, std::string newpwd}
#define CMD_CS_INIT_ROLE        CMD_MAKE_CLIENT(0x004f) //初始化角色信息 {std::string username, uint32 masterindex}

#define CMD_CS_CARD_QUIPMENT_CHANGE_SINGLE				CMD_MAKE_CLIENT(0x0050) 
/*装备变更命令
{
	uint8 cmd 
	0: add by line
	{
		uint32 position 0~5 阵容位
		uint32 cardobjid	
	}
	1: switch
	{
		uint32 old cardobjid
		uint32 new cardobjid
	}
	2: disboard
	{
		uint32 cardobjid
	}
	3:add by attackteam
	{
		uint32 cardobjid
	}
}
*/
#define CMD_CS_SWITCH_CARD_EXP                       CMD_MAKE_CLIENT(0x0051) //传功请求{unit32 强化objid, uint32 number [uint32 消耗objid... ], uint8 flag 0:normal传功 1:强化传功} 
#define CMD_CS_GACHA                       CMD_MAKE_CLIENT(0x0053) //GACHA请求{uint8 0:英雄帖 1:江湖阅历} 
#define CMD_CS_TEN_GACHA                       CMD_MAKE_CLIENT(0x0054) //GACHA十连抽请求{} 
#define CMD_CS_FRIEND_BOSS_LIST_REQUEST                       CMD_MAKE_CLIENT(0x0055) //好友BOSS数据请求{} 
#define CMD_CS_FRIEND_BOSS_HISTORY_REQUEST                       CMD_MAKE_CLIENT(0x0056) //好友BOSS历史请求{uint32 userid}
#define CMD_CS_FRIEND_BOSS_DURATION_REQUEST                       CMD_MAKE_CLIENT(0x0057) //好友BOSS战斗请求{uint32 userid}
#define CMD_CS_FRIEND_BOSS_GACHA_REQUEST                       CMD_MAKE_CLIENT(0x0058) //好友BOSS Gacha请求{uint8 all (1:全领 后面的字段没有意义 0:正常领取 后面的字段有意义), uint32 prize_id}
#define CMD_CS_STR_DIRTY_CHECK_REQUEST                       CMD_MAKE_CLIENT(0x0059) //用户名字检测请求, 包含脏词和重名{strig str}
#define CMD_CS_AGREE_FRIEND_REQUEST                       CMD_MAKE_CLIENT(0x005a) //好友同意请求{uint32 userid, uint8 agree 0:同意 1:拒绝}
#define CMD_CS_FRIEND_APPLY_REQUEST                       CMD_MAKE_CLIENT(0x005b) //好友申请列表请求{uint16 off}
#define CMD_CS_WORLD_BROADCAST_REQUEST                       CMD_MAKE_CLIENT(0x005c) //好友聊天请求{uint8 choose(0:普通聊天, 1:使用小喇叭聊天), string content}
#define CMD_CS_SHOP_USE_CANCEL                        CMD_MAKE_CLIENT(0x005d) //商城道具使用效果撤销(目前只有免战牌有用) {uint32 shopid}
#define CMD_CS_REQUSET_TIMER_GIFT                        CMD_MAKE_CLIENT(0x005e) //请求计时礼包 {uint32 giftindex}
#define CMD_CS_LOGIN_SIGN_ACTIVE                        CMD_MAKE_CLIENT(0x005f) //签到活动次数 {uint8 choose (0:normal sign 1:extra sign)}
#define CMD_CS_STORE_REQUEST                        CMD_MAKE_CLIENT(0x0060) //商场数据请求 {uint8 storechannel (STORE_DOLLAR:dollar shop  STORE_PRESTIGE:prestige shop), uint32 kind(商场类别)}
#define CMD_CS_GAME_FLOW_REQUEST                        CMD_MAKE_CLIENT(0x0061) //新手流程请求 {uint32 key, int32 val, uint32 currentkey}
#define CMD_CS_HEARTBEAT                        CMD_MAKE_CLIENT(0x0062) //服务心跳包 {uint32 timestamp}
#define CMD_CS_TURNTABLE_TURN                        CMD_MAKE_CLIENT(0x0063) //转盘转动请求 {uint8 choose, uint32 number}
#define CMD_CS_STATICS_DATA                        CMD_MAKE_CLIENT(0x0064) //数据统计请求 {为了统计 std::string reg_device(首次登陆设备), std::string reg_device_type(首次登陆设备型号), std::string second_channel, std::string uid}
#define CMD_CS_RECOVER_REQUEST                      CMD_MAKE_CLIENT(0x0065) //请求吃肉和酒 {uint8 (0:酒 1:肉)}
#define CMD_CS_ACHIEVE_REQUEST                      CMD_MAKE_CLIENT(0x0066) //请求奖励 {uint32 index}
#define CMD_CS_FIRST_CHARGE_REQUEST                      CMD_MAKE_CLIENT(0x0067) //请求首充大礼包 {}
#define CMD_CS_UPGRADE_REQUEST                      CMD_MAKE_CLIENT(0x0068) //进化申请 {uint32 upgrade index, uint32 card objid}
#define CMD_CS_UPGRADE_REVIEW_REQUEST                      CMD_MAKE_CLIENT(0x0069) //进化预览请求 {uint32 upgrade index, uint32 card objid}
#define CMD_CS_SKILL_STRENGTH_REVIEW                      CMD_MAKE_CLIENT(0x006a) //技能升级预览请求 {uint32 objid, uint32 number [uint32 card objid1...]}
#define CMD_CS_COMMON_ACTIVE_GET_REQUEST                      CMD_MAKE_CLIENT(0x006b) // 常规活动领取请求{uint32 activeindex, uint32 targetid}
#define CMD_CS_JOIN_PLAYER_REQUEST					CMD_MAKE_CLIENT(0x006c) // {}
#define CMD_CS_NOTICE_SHARE_REQUEST					CMD_MAKE_CLIENT(0x006d) // {}
#define CMD_CS_LEADER_SKILL_PREVIEW_REQUEST					CMD_MAKE_CLIENT(0x006e) // {uint32 objid, uint32 number [ uint32 card ObjId ... ] }
#define CMD_CS_TRIAL_REQUEST					CMD_MAKE_CLIENT(0x006f) // {}
#define CMD_CS_TRIAL_NEXT_REQUEST					CMD_MAKE_CLIENT(0x0070) // {uint8 isenter}
#define CMD_CS_ROLE_INFO_UPDATE_REQUEST					CMD_MAKE_CLIENT(0x0071) // {string username("": 表示不修改), string content("":表示不修改)}
#define CMD_CS_CDKEY_REQUEST					CMD_MAKE_CLIENT(0x0072) // {string cdkey}

//client tower begin
#define CMD_CS_TOWER_COMBAT   CMD_MAKE_CLIENT(0x0073)//战斗{uint32 index}
//client tower end
#define CMD_CS_DURATION_MISSION_ALL_STAR   CMD_MAKE_CLIENT(0x0074)//全三星{uint32 index(任务index)}
#define CMD_CS_ACTIVE_PRIZE_GET   CMD_MAKE_CLIENT(0x0075)//活跃奖励获取{uint32 index(奖励index)}

//////////////////////////////////////////////////////////////////////////
//Server 发送到 Client的命令
#define CMD_SC_TEST					CMD_MAKE_SERVER(0x0001) //测试用 echo 服务器返回{string返回字符串;}
#define CMD_SC_AUTH_RESULT			CMD_MAKE_SERVER(0x0002) //服务器认证返回信息{uint16返回结果;}0认证通过，其他失败。应该包括多种失败信息，详细见EnumDefine
#define CMD_SC_ERROR_MSG			CMD_MAKE_SERVER(0x0003) //uint16 ErrorMsgNum
#define CMD_SC_ROLE_BASE_INFO		CMD_MAKE_SERVER(0x0004) //角色基本信息{***参看CRole::SendPlayerBaseInfo***}
#define CMD_SC_ROLE_PROPERTY		CMD_MAKE_SERVER(0x0005) //角色属性更新{CDynamicStruct.loose更新信息(saveDataDirty AnyType);}
#define CMD_SC_ERROR_STR			CMD_MAKE_SERVER(0x0006)	//错误字符串信息 {string strMsg}
#define CMD_SC_MSG_STR			CMD_MAKE_SERVER(0x0007)	//字符串信息GM test命令等 {string strMsg}

#define CMD_SC_ROLE_CARD_PACKET_INFO		CMD_MAKE_SERVER(0x0008) 
/*角色卡牌背包数据拉取
{
    uint32 number 总卡牌数量
    [
        {
        参见卡牌属性更新 loose更新信息
        }
        ...
    ]
}
*/
#define CMD_SC_ROLE_CARD_PACKET_CHANGE		CMD_MAKE_SERVER(0x0009) 
/*
角色卡牌背包更新
{
    uint32 number //操作数量
    [
        {
            uint8 cmd   //操作命令， 0：添加 1：删除
            0 添加
            {
                参加卡牌属性更新信息 loose更新信息
            }
            1 删除
            uint32 card_objid
        }
        ....
    ]
    
}
*/
#define CMD_SC_ROLE_CARD_PROPERTY		CMD_MAKE_SERVER(0x000a) //角色卡牌信息属性更新{uint32卡牌ID，CDynamicStruct. 参见_E_PROPERTY_ROLE_CARD loose}

#define CMD_SC_SIMPLE_MISSION_RESULT    CMD_MAKE_SERVER(0x000b) //领取任务，返回值{uint16返回结果,uint32效果事件index, int32(para1), int32(para2), int32(para3)}  任务成功， 其他失败 ERROR_MSG_NOT_ENOUGH_ENERGY {uint16错误码，uint32下一点回复时间，uint32 全部恢复时间, ERRNO_MSG_MISSION_DURATION_LOSE 决斗 负{}} 

#define CMD_SC_CARD_QUIPMENT_CHANGE_RESULT				CMD_MAKE_SERVER(0x000c)
/*
装备变更命令
{
    uint16 返回结果
}
*/
#define CMD_SC_ITEM_QUIPMENT_CHANGE_RESULT				CMD_MAKE_SERVER(0x000d) 
/*装备变更命令{unit16返回结果} 0 装备更换成功， 其他失败
 * {
 * 		uint16 返回结果
 * 		0 成功时候
 * 		{
 * 			uint8 team // 0 弟子卡牌列, 1:师傅卡牌列
 * 			{
 * 				uint8 位置
 * 				uint32 武器卡牌objid
 * 				uint32 防具卡牌objid
 * 				uint32 宝石卡牌objid 
 * 				uint32 首饰卡牌objid
 * 				uint32 技能卡牌objid
 * 				uint32 师傅指点1objid
 * 				uint32 师傅指点2objid
 * 				uint32 师傅指点3objid
 * 			}
 *
 * 		}
 * 		其他失败
 * 		{}
 * }
*/
#define CMD_SC_SEARCH_PLAYER_RESULT                           CMD_MAKE_SERVER(0x000e) //搜索玩家结果{unit16返回结果,uint8玩家数量,[{uint32玩家ID,uint16等级,string名字,uint32 number [uint32 index, ...], uint32 masterindex} ...]} 
#define CMD_SC_AUTO_PLAYER_RESULT                           CMD_MAKE_SERVER(0x000f) //AI匹配玩家结果{unit16返回结果,uint8玩家数量,[{uint32玩家ID,uint16等级,string名字,uint32 number, [uint32 index, ...], uint32 masterindex} ...]} 

#define CMD_SC_DURATION_RESULT                          CMD_MAKE_SERVER(0x0010) 
/*
决斗结果
{
uint16 status 返回结果 0:战斗成功， 其他失败
0: 战斗成功
uint8 0 胜利， 1 失败
uint32 exp
int32 coin
uint32效果事件index
int32(para1)
int32(para2)
int32(para3)
} 

ERRNO_MSG_NOT_ENOUGH_STRENGTH:
uint32 next_point_time
uint32 all_time
*/

#define CMD_SC_SEARCH_PLAYER_ITEM_RESULT                       CMD_MAKE_SERVER(0x0011) //根据道具搜索玩家结果{unit16返回结果,uint8玩家数量,[{uint32玩家ID,uint16等级,string名字,uint32 number [uint32 index, ...], unit32 masterindex, uint32 武力值} ...], uint8 机器人数量, [同上]} 
#define CMD_SC_ROB_RESULT                    CMD_MAKE_SERVER(0x0012) 
/*
夺宝结果
{
uint16 status 返回结果 0:战斗成功， 其他失败
0: 成功
uint8 0 胜利， 1 失败
uint32 exp
uint32效果事件index
int32(para1)
int32(para2)
int32(para3)
} 

ERRNO_MSG_NOT_ENOUGH_STRENGTH:
uint32 next_point_time
uint32 all_time
*/

#define CMD_SC_ROLE_CARD_EQUIP_INFO			CMD_MAKE_SERVER(0x0013)
/* 卡牌装备信息
 *
 * [
 * 		uint8 数目 第一阵容
 * 		[
 * 		{
 * 			uint8 位置
 * 			uint32 武器卡牌objid
 * 			uint32 防具卡牌objid
 * 			uint32 宝石卡牌objid 
 * 			uint32 首饰卡牌objid
 * 			uint32 技能卡牌objid
 * 			uint32 师傅指点1objid
 * 			uint32 师傅指点2objid
 * 			uint32 师傅指点3objid
 * 		}
 * 		...
 * 		]
 * 		师傅卡牌 装备
 * 		{
 * 			uint8 位置
 *			uint32 武器卡牌objid
 *			uint32 防具卡牌objid
 *			uint32 宝石卡牌objid
 *			uint32 首饰卡牌objid
 *			uint32 技能卡牌objid
 *			uint32 师傅指点1objid
 *			uint32 师傅指点2objid
 *			uint32 师傅指点3objid
 * 		}
 * ]
 * */

#define CMD_SC_SIMPLE_MISSION_INFO        CMD_MAKE_SERVER(0x0014) // {uint8步骤上限,uint8类型,uint16消耗精力,uint16获得经验,uint16获得金钱,uint32BOSS 0:无BOSS 其他:BOSS index}
#define CMD_SC_DURATION_PREVIEW_RESULT                          CMD_MAKE_SERVER(0x0015) //战斗预览{uint16 status, (胜利的情况)uint32 经验, int32 金钱, (失败的情况) uint32经验, int32 金钱, string 对方的名字}  

#define CMD_SC_ROLE_ITEM_PACKET_INFO		CMD_MAKE_SERVER(0x0016) 
/*角色道具背包数据拉取
{
    uint32 number 总道具数量
    [
        {
			uint32 objid,
			uint32 number
        }
        ...
    ]
}
*/
#define CMD_SC_ROLE_ITEM_PACKET_CHANGE		CMD_MAKE_SERVER(0x0017) 
/*
角色道具背包更新
{
    uint32 number //操作数量
    [
        {
            uint8 cmd   //操作命令， 0：添加 1：删除
            0 添加
            {
				uint32 objid,
				uint32 number
            }
            1 删除
            uint32 objid
        }
        ....
    ]
    
}
*/
#define CMD_SC_ROB_PREVIEW_RESULT                          CMD_MAKE_SERVER(0x0018) //战斗预览{uint16 status, (胜利的情况)uint32 经验, (失败的情况) uint32经验, string 对方的名字}  

#define CMD_SC_CARD_COMBINE_REQUEST_RESULT                         CMD_MAKE_SERVER(0x0019) //合成请求{uint16 返回结果，uint32 combindex, unit32 还需要多长时间(单位秒)} 
#define CMD_SC_CARD_COMBINE_GET_RESULT                         CMD_MAKE_SERVER(0x001a) //合成装备获取{uint16 返回结果, unit32 合成物品ID, uint32 combindex} 
#define CMD_SC_CARD_COMBINE_INFO                        CMD_MAKE_SERVER(0x001b) //合成装备获取{uint16 number [uint32 合成Index, unit32 时间戳单位(秒)] ...} 

//#define CMD_SC_CARD_EVOLUTION_RESULT                       CMD_MAKE_SERVER(0x001c) //合成装备获取{uint16 返回结果, uint32 消耗金钱, uint32 进化产生的card objid} 
#define CMD_SC_SPECIAL_CARD_STRENGTH_RESULT                       CMD_MAKE_SERVER(0x001d) //合成装备获取{uint16 返回结果, uint32 消耗金钱, uint32 强化的card objid, uint8 IsSuccess 0:成功 1:不成功}
#define CMD_SC_CARD_PROPERTY                       CMD_MAKE_SERVER(0x001e) //card 属性通知{uint32 ID, CDynamicStruct.loose}
#define CMD_SC_CARD_TUJIAN_INFO						CMD_MAKE_SERVER(0x001f) //
/* 
 * uint32 number
 * [
 * {
 *		uint32 index, 
 *		uint8 rare,
 *		uint32 BreakCount NORMAL
 *		uint32 BreakCount AWAKE
 *		uint8 Status
 * }
 * ...
 * ]
 * */
#define CMD_SC_CARD_TUJIAN_CHANGE						CMD_MAKE_SERVER(0x0020) //
/* 
 *		uint32 index, 
 *		uint8 rare,
 *		uint32 BreakCount NORMAL
 *		uint32 BreakCount AWAKE
 *		uint8 Status
 * */
#define CMD_SC_TECH_UPDATE_RESULT                       CMD_MAKE_SERVER(0x0021) //科技升级请求{uint16 返回结果, 0:{ uint32 科技ID int16 lv(-1:表示满级), uint16 role lv, uint32 数值, uint32 coin } } ERRNO_MSG_TECH_LEVEL_NOT_ARRIVE: {uint32 科技ID} 其他错误{}  
#define CMD_SC_TECH_INFO                      CMD_MAKE_SERVER(0x0022) //科技升级请求{uint32 number,[tech id (1-22){uint32 tech id, int16 level(-1:表示已满级), uint16 role lv, uint32 para1, uint32 coin, uint32 credit} ...] } 

#define CMD_SC_FRIEND_AUTO_PLAYER_IN_FRIEND_RESULT                           CMD_MAKE_SERVER(0x0023) //AI匹配玩家结果{unit16返回结果,uint8玩家数量,[{uint32玩家ID,uint16等级,string名字,uint32 武力, uint32 masterindex, uint32 lastlogintime} ...]} 
#define CMD_SC_FRIEND_PLAYER_TEAM_RESULT                           CMD_MAKE_SERVER(0x0024)  
/*  获得玩家装备阵容
 	uint16 status
    uint8 number 装备卡牌数量 //第一卡牌阵容
    [
        uint32 card_objid
        ...
    ]
	uint8 number 阵容 
	[
		uint32 card_objid
		...
	]
	uint8 数目 第一阵容
	[
	{
		uint8 位置
		uint32 武器卡牌objid
		uint32 防具卡牌objid
		uint32 宝石卡牌objid 
		uint32 首饰卡牌objid
		uint32 技能卡牌objid
	}
	...
	]
    uint32 number 总装备卡牌数量
    [
        {
        参见卡牌属性更新 loose更新信息
        }
        ...
    ]
	uint8 卡牌凹槽容量
*/
#define CMD_SC_FRIEND_AUTO_PLAYER_FOR_FRIEND_RESULT                           CMD_MAKE_SERVER(0x0025) //AI匹配玩家结果{unit16返回结果,uint8玩家数量,[{uint32玩家ID,uint16等级,string名字,uint32 武力值, uint32 masterindex, uint32 lastlogintime} ...]} 
#define CMD_SC_FRIEND_ADD_RESULT                          CMD_MAKE_SERVER(0x0026) //申请加玩家为好友{uint16 返回结果,0: uint32 user_id 其他错误: {} } 
#define CMD_SC_FRIEND_SEARCH_PLAYER_RESULT                           CMD_MAKE_SERVER(0x0027) //搜索玩家结果{unit16返回结果,uint8玩家数量,[{uint32玩家ID,uint16等级,string名字,uint32 武力值, uint32 masterindex, uint32 lastlogintime} ...]} 
#define CMD_SC_FRIEND_DEL_RESULT                           CMD_MAKE_SERVER(0x0028) //删除好友{uint16 返回结果,0: uint32 user_id 其他错误: {} } 
#define CMD_SC_FRIEND_NUMBER_RESULT                           CMD_MAKE_SERVER(0x0029) //好友数量{uint16 返回结果,0: uint16当前好友数,uint16 好友上限数,uint16 申请好友数, number 其他错误: {} } 

#define CMD_SC_MAIL_INFO                         CMD_MAKE_SERVER(0x002a) //邮件信息拉取
/* 
 * uint32 number
 * [
 * 	{
 * 		uint32 mail_id
 *		uint32 send_user_id,
 *		string send_user_name,
 *		uint8 0:未读或未处理 1:处理或已读
 *		uint8 1:好友相关, 2:系统相关, 3:战斗相关 
 *		uint32 timestamp uinx 时间戳
 *		uint8 detailtype
 *		string title
 * }
 * ...
 * ]
 * */
#define CMD_SC_MAIL_INFO_CHANGE                         CMD_MAKE_SERVER(0x002b) //邮件更新协议 
/*
 * uint8 cmd 0:add 1: del 2: update 表面该邮件已读或已处理
 *	0:
 *	{
 * 		uint32 mail_id
 *		uint32 send_user_id,
 *		string send_user_name,
 *		uint8 0:未读或未处理 1:处理或已读
 *		uint8 1:好友相关, 2:系统相关,3:战斗相关, 
 *		uint32 timestamp uinx 时间戳
 *		uint8 detailtype
 *	} 
 *	1:
 *	{
 *		uint32 mail_id
 *	}
 *	2:
 *	{
 *		uint32 mail_id
 *	}
 */
#define CMD_SC_MAIL_DETAIL_INFO_RESULT                         CMD_MAKE_SERVER(0x002c) //请求邮件详情信息{uint16 status, uint32 mail_id, uint8 type } 
/*
 * FRIEND_TEXT:
 * 	{
 * 		string  content
 * 	}
 * 	DURATION_COIN
 * 	{
 * 		uint32 INDEX_ID
 * 		string username
 * 		uint32 coin
 * 	}
 * 	ROB_GET
 * 	{
 * 		uint32 INDEX_ID
 * 		string username
 * 		uint32 item_id
 * 	}
 * 	ROB_LOSE
 * 	{
 * 		uint32 INDEX_ID
 * 		string username
 * 		uint32 item_id
 * 	}
 *	DURATION_LOSE
 *	{
 *		uint32 INDEX_ID
 *		string username
 *		uint32 coin
 *	}
 *	SYS_PAY
 *	{
 *		uint32 INDEX_ID
 *		uint32 type  // type para1 para2 参考 E_OBJ_TYPE 发送物品格式
 *		uint32 para1
 *		uint32 para2
 *		uint32 coin
 *		uint32 dollar
 *		uint32 Energy
 *		uint32 Strength
 *		uint32 worldhistory
 *	} 
 *	SYS_GIFT
 *	{
 *		uint32 INDEX_ID
 *		uint32 type // 同SYS_PAY
 *		uint32 para1
 *		uint32 para2
 *		string tip
 *		uint32 coin
 *		uint32 dollar
 *		uint32 Energy
 *		uint32 Strength
 *		uint32 worldhistory
 *	}
 *	SYS_DIY
 *	{
 *		uint32 type // 同SYS_PAY
 *		uint32 para1
 *		uint32 para2
 *		string tip
 *		uint32 coin
 *		uint32 dollar
 *		uint32 Energy
 *		uint32 Strength
 *		uint32 worldhistory
 *	}
 */
#define CMD_SC_MAIL_HANDLE_RESULT                         CMD_MAKE_SERVER(0x002d) //邮件操作{uint16 返回结果,0: 成功 {uint32 mail_id, uint16 opt (参见E_OPT_MAIL), uint32 coin, uint32 dollar, uint32 energy, uint32 strength, uint32 worldhistory, {uint32 type uint32 para1, uint32 para2(E_OBJ_EFFECT)}, 其他:错误 {} } 
#define CMD_SC_MAIL_DEL_RESULT                         CMD_MAKE_SERVER(0x002e) //邮件操作{uint16 返回结果,0: 成功 {uint32 mail_id}, 其他:错误 {} } 
#define CMD_SC_FRIEND_MAIL_RESULT                         CMD_MAKE_CLIENT(0x002f) // 给好友发送邮件{uint16 status, 0: 成功  其他: 错误 } 
#define CMD_SC_ARENA_PLAYER_TEAM_RESULT                           CMD_MAKE_SERVER(0x0030)  
/*  获得玩家装备阵容
 	uint16 status
    uint8 number 卡牌数量
    [
        uint32 card_objid
        ...
    ]
	uint8 number 位置索引
	[
		uint32 card_objid
		...
	]
	uint8 数目 
	[
	{
		uint8 位置
		uint32 武器卡牌objid
		uint32 防具卡牌objid
		uint32 宝石卡牌objid 
		uint32 首饰卡牌objid
		uint32 技能卡牌objid
		uint32 师傅卡1objid
		uint32 师傅卡2objid
		uint32 师傅卡3objid
	}
	...
	]
    uint32 number 总装备卡牌数量
    [
        {
        参见卡牌属性更新 loose更新信息
        }
        ...
    ]
	uint8 卡牌凹槽容量
	uint16 卡牌属性数量
	[
	{
		uint32 attack
		uint32 defence
		uint32 health
		uint32 crit
		uint8 attribute
	}
	]
	师傅属性
	{
		uint32 attack
		uint32 defence
		uint32 health
		uint32 crit
		uint8 attribute
	}
	uint32 武力
*/
#define CMD_SC_ARENA_TOP_PLAYER                           CMD_MAKE_SERVER(0x0031) //AI匹配玩家结果{uint8玩家数量,[{uint32玩家ID,uint16等级,string名字,uint32 rank, uint32 战力, unit32 masterindex} ...]} 
//#define CMD_SC_ARENA_GET_ENEMY                           CMD_MAKE_SERVER(0x0032) //AI匹配玩家结果{uint8玩家数量,[{uint32玩家ID,uint16等级,string名字,uint32 rank, uint32 number [uint32 index, ...], uint32 masterindex} ...]} 
#define CMD_SC_ARENA_HIGH_RANK_PLAYER                           CMD_MAKE_SERVER(0x0033) //AI匹配玩家结果{uint8玩家数量,[{uint32玩家ID,uint16等级,string名字,uint32 rank, uint32 战力, uint32 masterindex} ...]} 
#define CMD_SC_ARENA_DURATION_RESULT                          CMD_MAKE_SERVER(0x0034) 
/*
决斗结果
{
uint16 返回结果 0:战斗成功， 其他失败
uint8 0 胜利, 1 失败
uint32 获得声望
} 
*/
#define CMD_SC_ARENA_INFO                           CMD_MAKE_SERVER(0x0035) //{uint32 userid, uint32 prestige, uint32 rank, uint32 CD, uint32 challengenumber(剩余的挑战次数), uint32 ranked(领取奖励已排的名次), uint32 领取的声望, uint32 lastprestige, uint32 nowprestie} 
#define CMD_SC_ARENA_OPEN_RESULT                           CMD_MAKE_SERVER(0x0036) //{uint16 status, ERRNO_MSG_ARENA_LEVEL_LIMIT: uint16 level} 
//#define CMD_SC_ARENA_PRESTIGE_PRODUCT_LIST                           CMD_MAKE_SERVER(0x0037) //{uint32 number [uint32 index ...], uint32 timestamp下次刷新时间} 
//#define CMD_SC_ARENA_RANK_PRODUCT_LIST                           CMD_MAKE_SERVER(0x0038) //{uint32 index, uint8 number, uint32 timestamp 下次刷新时间} 
#define CMD_SC_ARENA_REQUEST_PRESTIGE_PRODUCT_RESULT                           CMD_MAKE_SERVER(0x0039) //{uint16 status, uint32 getprestige} 
//#define CMD_SC_ARENA_REQUEST_RANK_PRODUCT_RESULT                           CMD_MAKE_SERVER(0x003a) //{uint16 status} 

//#define CMD_SC_MISSION_DURATION_RESULT                          CMD_MAKE_SERVER(0x003b) 
#define CMD_SC_MISSION_BRANCH_INFO                          CMD_MAKE_SERVER(0x003c) // { uint16 number [{uint32 taskindex, uint32 timestamp} ... ]}
#define CMD_SC_MISSION_BRANCH_ACCEPT_RESULT                         CMD_MAKE_SERVER(0x003d) // {格式和CMD_SC_DURATION_MISSION_ACCEPT_RESULT一样}
//#define CMD_SC_MISSION_BRANCH_DURATION_RESULT                         CMD_MAKE_SERVER(0x003e) //同 CMD_SC_MISSION_DURATION_RESULT 
//#define CMD_SC_RANK_RESULT              CMD_MAKE_SERVER(0x003f) //{等级榜 uint8玩家数量,[{uint32玩家ID,string名字,uint32等级} ...] 攻击力榜 uint8 玩家数量 [{uint32玩家ID,string名字,uint32 攻击力最大} ...] 防御力榜 uint8 玩家数量 [{uint32玩家ID,string名字,uint32 防御力最大} ...]}
#define CMD_SC_GROUP_INFO                          CMD_MAKE_SERVER(0x0040) //{公会属性 数据拉取参见 E_PROPERTY_GROUP} 
#define CMD_SC_GROUP_PROPERTY                          CMD_MAKE_SERVER(0x0041) //{公会属性更新{CDynamicStruct.loose更新信息(saveDataDirty AnyType);}} 
#define CMD_SC_GROUP_MEMBER_INFO                          CMD_MAKE_SERVER(0x0042) //{公会成员数据拉取uint32 userid, unit32 荣誉, uint8 职位, uint32 任务数据数量 [{uint32 taskindex, uint8 step} ...] uint32 荣誉} 
#define CMD_SC_GROUP_CREATE_RESULT                         CMD_MAKE_SERVER(0x0043) //{创建工会 uint16 status, uint32 groupid}} 
#define CMD_SC_GROUP_AUTO_RESULT                         CMD_MAKE_SERVER(0x0044) //{uint16 status, uint32 number[{ uint64 groupid, uint16 level, string 工会名字, string 会长名字, uint16 persion number, uint16 persion limit}... ]} 
#define CMD_SC_GROUP_SEARCH_RESULT                         CMD_MAKE_SERVER(0x0045) //{uint16 status, uint32 number[{ uint64 groupid, uint16 level, string 工会名字, string 会长名字, uint16 persion number, uint16 persion limit}... ]} 
#define CMD_SC_GROUP_LEVELUP_RESULT                      CMD_MAKE_SERVER(0x0046) //{uint16 status} 
#define CMD_SC_GROUP_MAIL_INFO_RESULT                      CMD_MAKE_SERVER(0x0047) //{uint16 status, uint32 number 
/*
 * [
 * {
 * uint32 time
 * uint32 mailid
 * uint8 Do 操作结果
 * uint8 type
 * E_GT_MAIL_REQUEST_ENTER:
 * string content;
 *
 * uint32 userid;
 * uint32 index;
 * string strusername
 *
 * E_GT_MAIL_AGREE_ENTER:
 *
 * uint32 index;
 * uint8 position;
 * string agreeusername;
 * string strusername;
 *
 * E_GT_MAIL_CANCEL:
 *
 * uint32 index;
 * string strusername
 *
 * E_GT_MAIL_DELETE:
 * uint32 index;
 * uint8 position;
 * string agreeusername;
 * string strusername
 * }
 * ... 
 * ]} 
 */
#define CMD_SC_GROUP_REQUSET_ENTER_RESULT                        CMD_MAKE_SERVER(0x0048) //{uint16 status} 
#define CMD_SC_GROUP_MAIL_OPT_RESULT                         CMD_MAKE_SERVER(0x0049) //请求邮件详情{uint16 status, uint64 groupid, uint32 mail_id, uint8 handle} 
#define CMD_SC_GROUP_MISSION_ACCEPT_RESULT                         CMD_MAKE_SERVER(0x004a) // {uint16 status(0 成功, 其他失败), uint32 taskindex, uint32 step, uint32效果事件index, int32(para1), int32(para2), int32(para3)  其他失败 E_ERRNO_MISSON_NOT_ENOUGH_ENERGY {uint16错误码，uint32下一点回复时间，uint32 全部恢复时间}
#define CMD_SC_GROUP_REQUSET_PRIZE_RESULT                         CMD_MAKE_SERVER(0x004b) //请求荣誉奖励{uint32 status} 
#define CMD_SC_GROUP_TECH_UPDATE_RESULT                       CMD_MAKE_SERVER(0x004c) //科技升级请求{uint16 返回结果, 0:{ uint32 科技ID {1-19 21-22: int16 lv(-1:表示满级), uint16 role lv, uint32 数值, uint32 credit 20: int16 lv(-1:表示满级), uint16 role lv, uint32 精力提升上限, uint32 credit}} } ERRNO_MSG_TECH_LEVEL_NOT_ARRIVE: {uint32 科技ID} 其他错误{}  
#define CMD_SC_GROUP_MEMBER_INFO_RESULT                        CMD_MAKE_SERVER(0x004d) //公会成员拉取{uint16 status, uint32 number [{uint32 userid, string username, uint16 level, uint32 荣誉值} ... ]} 
#define CMD_SC_GROUP_INFO_RESULT                        CMD_MAKE_SERVER(0x004e) //公会拉取{uint16 status, uint32 number [{uint64 groupid, string groupname, uint16 grouplevel, string createname, uint16 persion, uint16 persionlimit} ... ]} 
#define CMD_SC_GROUP_CHAT_RESULT                        CMD_MAKE_SERVER(0x004f) //{uint16 status} 
#define CMD_SC_GROUP_CHAT_MSG_UDPATE                        CMD_MAKE_SERVER(0x0050) //公会聊天信息通知{string str, string name} 
#define CMD_SC_GROUP_APPOINT_RESULT                        CMD_MAKE_SERVER(0x0051) //公会任命{uint16 status, uint32 userid, string name, uint8 position} 
#define CMD_SC_GROUP_UNAPPOINT_RESULT                        CMD_MAKE_SERVER(0x0052) //公会拉取{uint16 status, uint32 userid, string name, uint8 position} 
#define CMD_SC_GROUP_RANK_SEARCH_RESULT                         CMD_MAKE_SERVER(0x0053) //{uint16 status, uint32 number [{uint32 rank, uint64 groupid, string groupname, uint16 grouplevel, string createname, uint16 persion, uint16 persionlimit} ... ]} 
#define CMD_SC_GROUP_MEMBER_SEARCH_INFO_RESULT                        CMD_MAKE_SERVER(0x0054) //公会成员拉取{uint16 status, uint32 number [{uint32 userid, string username, uint16 level, uint32 荣誉值}]} 
#define CMD_SC_GROUP_DETAIL_INFO_RESULT                        CMD_MAKE_SERVER(0x0055) //公会数据详情{uint16 status, uint64 groupid, string groupname, uint16 grouplevel, string createname, uint16 persion, uint16 persionlimit, string notice} 
#define CMD_SC_GROUP_MODIFY_INFO_RESULT                        CMD_MAKE_SERVER(0x0056) //修改公会公告{uint16 status} 
#define CMD_SC_GROUP_EXIT_INFO_RESULT                        CMD_MAKE_SERVER(0x0057) //退出公会{uint16 status} 
#define CMD_SC_GROUP_DELETE_MEMBER_RESULT                        CMD_MAKE_SERVER(0x0058) //把别人踢出公会{uint16 status, uint32 userid} 
#define CMD_SC_GLOBAL_INFO                       CMD_MAKE_SERVER(0x0059) //全局信息{uint32 加入公会等级限制, uint32 创建公会等级限制, uint32 创建公会消耗钻石, uint32 推图CD消耗, uint32 精英CD消耗, uint32 gacha消耗阅历, uint32 gacha消耗元宝, uint32 强化宝石系数, uint32 夺宝体力消耗, uint32好友BOSS消耗体力, uint32 传功每张卡消耗金钱, uint32 侠客卡牌背包扩大消费, uint32 侠客卡牌扩大条目, uint32 装备卡牌背包扩大消费, uint32 装备卡牌背包扩大条目, uint32 number [{uint32 type, uint32 para1, uint32 para2} ... ], uint32 活动关卡消耗元宝字段, uint32 改名字消耗元宝数, uint32 plusval,uint32 购买闯关次数需要的元宝，uint32 购买三倍攻击元宝，uint32 购买2倍攻击元宝，uint32 刷新属性元宝, uint32 挑战次数回复消耗元宝}
#define CMD_SC_GROUP_UNAPPOINT_MEMBER_INFO_RESULT                        CMD_MAKE_SERVER(0x005a) //拉取公会成员{uint16 status, uint32 number [{uint32 userid, string username} ... ]} 
#define CMD_SC_GROUP_CANCEL_RESULT                        CMD_MAKE_SERVER(0x005b) //解散公会 {uint16 status}
#define CMD_SC_SHOP_PACKET_INFO		CMD_MAKE_SERVER(0x005c) 
/*角色商城道具数据拉取{uint32 number 总道具数量[{uint32 index, uint32 number} ... ] }
*/
#define CMD_SC_SHOP_BUY_RESULT                        CMD_MAKE_SERVER(0x005d) //购买商城道具结果 {uint16 status, uint32 storeid, uint32 number}
#define CMD_SC_ROLE_CARD_TEAM_EQUIP_INFO		CMD_MAKE_SERVER(0x005e) 
/* 卡牌战斗位 装备获取
 {
    uint8 number 装备卡牌数量 //第一卡牌阵容
    [
        uint32 card_objid
        ...
    ]
	uint32 master objid 师傅卡牌ObjId
 }
*/
//#define CMD_SC_TEAM_CHANGE_RESULT		CMD_MAKE_SERVER(0x005f) //{uint16 status}
#define CMD_SC_EQUIP_CARD_CAL_INFO		CMD_MAKE_SERVER(0x0060) //{uint16 number [{uint8 location, uint32 attack, uint32 defence, unit32 health, uint32 crit, uint32 dodge, uint32 action, uint8 attribute} ...] master卡牌信息 {uint32 attack, uint32 defence, uint32 health, uint32 crit, uint32 dodge, uint32 action, uint8 attribute}, uint32 武力值}
#define CMD_SC_SEARCH_WIN_RESULT		CMD_MAKE_SERVER(0x0061) //{uint16 status, uint8 number [{uint32 userid, uint16 wlevel, string strname, uint32 number, [unit32 index, ...], uint32 masterindex} ...]}
#define CMD_SC_BROAD_CAST		CMD_MAKE_SERVER(0x0062) //{
// uint16 type, 
// SYS_BROAD_EGG: string str, uint32 card index
// SYS_BROAD_STRENGTH_CARD: string str, uint32 card index, uint32 lv
// SYS_BROAD_OPEN_SPECIAL_BOX: string str, uint32 BoxIndex, uint32 index
// SYS_BROAD_MASTER_GET_SKILL: string str, uint32 BigSkillIndex
// SYS_BROAD_SKILL_LV_UP: string str, uint32 BigSkillIndex}
// SYS_BROAD_VIP_ONLINE: string str, uint32 userid, uint16 vip
// SYS_BROAD_BOSS_HAPPEN: string name, uint32 userid, uint16 vip, string bossname
// SYS_BROAD_DIY: string content
#define CMD_SC_SHOP_USE_RESULT                        CMD_MAKE_SERVER(0x0063) //使用商城道具 {uint16 status, uint32 shopid, uint32 functype(参见 EShopUseEffect), uint32 number, [{int32 para1, int32 para2, int32 para3} ...]}
//#define CMD_SC_SKILL_FLUSH_RESULT                     CMD_MAKE_SERVER(0x0064) //合成技能 {uint16 status, uint32 effect, int32 para1, int32 para2, int32 para3, int32 para4}
#define CMD_SC_SHOP_BUY_DEL_CD_RESULT                     CMD_MAKE_SERVER(0x0065) //消CD {uint16 status, uint32 CDid Cd商品ID, int32 para1, int32 para2}
#define CMD_SC_CARD_LINEUP_INFO                     CMD_MAKE_SERVER(0x0066) //卡牌阵容信息 {uint32 objid ...  总共有六个,从1号位依次到6号位}
#define CMD_SC_CARD_LINEUP_CHANGE_RESULT					CMD_MAKE_SERVER(0x0067) // 改变阵型 {uint16 status}
#define CMD_SC_MASTER_INFO					CMD_MAKE_SERVER(0x0068) // 拜师系统数据拉取 {uint32 百里免费领取剩余时间, uint32 千里寻师免费领取剩余时间, uint32 万里寻师免费领取时间, uint32 师傅数量 [{uint32 index 师傅Index, uint32 剩余时间, uint32 上香剩余CD, uint32 上香次数, uint8 是否处于拜师状态 1:处于, uint32 exp, uint16 level} ... ]}
#define CMD_SC_MASTER_REQUEST_RESULT					CMD_MAKE_SERVER(0x0069) // 拜师请求 {uint16 status, uint32 choose 选择的选项, uint32 masterindex, uint8 flag 1:增长经验, 0:无, uint32 type, uint32 para1, uint32 para2(参加E_OBJ_EFFECT)}
#define CMD_SC_MASTER_YES_OR_NO_RESULT					CMD_MAKE_SERVER(0x006a) // 同意或拒绝这个师父 {uint16 status, uint32 同意或拒绝, uint32 master}
#define CMD_SC_MASTER_SEND_RED_RESULT					CMD_MAKE_SERVER(0x006b) // 塞红包 {uint16 status, uint32 拜师index, uint32 type, uint32 para1, uint32 para2 (type 为E_OBJ_EFFECT 说明)}
#define CMD_SC_MASTER_BLESS_RESULT					CMD_MAKE_SERVER(0x006c) // 上香 {uint16 status, uint32 拜师index, uint32 type, uint32 para1, uint32 para2}

#define CMD_SC_DURATION_PKG						CMD_MAKE_SERVER(0x006d)
/* 战斗报文
 * string myself
 * string peer
 * 自己
 * 总共发满, 当前6个 顺序决定战斗位置
 * [
 * {
 * 		uint32 card objid
 * 		uint32 health
 * 		uint32 health max
 * 		uint16 card lv
 * 		uint32 attack
 * 		uint8 rare
 * 		uint16 BreakCount 突破次数
 * 		uint16 fullbreak 1:满破 0:没有
 * }
 * ...
 * ]
 * 师傅
 * {
 * 		uint32 card objid
 * 		uint32 health
 * 		uint32 health max
 * 		uint32 index
 * 		uint16 card lv
 * 		uint32 attack
 * 		uint8 rare
 * 		uint16 BreakCount 突破次数
 * 		uint16 fullbreak 1:满破 0:没有
 * }
 * 对方 
 * 总共发满, 当前6个 顺序决定战斗位置
 * [
 * {
 * 		uint32 card objid
 * 		uint32 health
 * 		uint32 health max
 * 		uint32 index
 * 		uint16 card lv
 * 		uint32 attack
 * 		uint8 rare
 * 		uint16 BreakCount 突破次数
 * 		uint16 fullbreak 1:满破 0:没有
 * }
 * ...
 * ]
 * 师傅
 * {
 * 		uint32 card objid
 * 		uint32 health
 * 		uint32 health max
 * 		uint32 index
 * 		uint16 card lv
 * 		uint32 attack
 * 		uint8 rare
 * 		uint16 BreakCount 突破次数
 * 		uint16 fullbreak 1:满破 0:没有
 * }
 * 我方
 * uint32 total health
 * uint32 total maxhealth
 * 对方
 * uint32 total health
 * uint32 total maxhealth
 * uint8 evaluation 1 2 3 总共三档 3 为最高档
 * {}// 空 预留回合前动作,目前只有加血
 * uint32 roundnumber
 * [
 * {
 * 		uint32 action number
 * 		[
 * 		{
 * 			uint8 type(总共有四种action)
 * 			ACTION_ATTACK:
 * 				uint8 攻击阵容 FIRST_LINE SECOND_LINE MASTER_LINE
 *				uint8 team 阵营 0:自己 1:对方
 *				uint32 skill index 0:普攻 非0: 技能攻击
 *				uint16 lv
 *				uint32 发技能卡的序号 
 *				uint8 IsAttr 技能是否有属性加成
 *				uint32 AfterAttack 放过技能后的attack
 *
 *				[ // 只对弟子卡有意义
 *					目前3个
 *					{
 *					uint32 harm  // 伤害为0 也说明这个位置卡牌没有, 只要有不可能伤害为0
 *					uint8 IsCrit // 是否暴击
 *					}
 *					...
 *				]
 *				uint32 totalharm
 *				uint8 IsCrit // 师傅是否造成暴击, 对于弟子攻击无意义
 *
 * 			ACTION_DEFENCE:
 * 				uint8 攻击阵容 FIRST_LINE SECOND_LINE MASTER_LINE
 *				uint8 team 阵营 0:自己 1:对方
 *				uint32 skill index 0:普攻 非0: 技能攻击
 *				uint16 lv
 *				uint32 发技能卡的序号 
 *				uint32 health 技能加血量
 *
 *				[ // 只对弟子卡有意义
 *					目前3个
 *					{
 *					uint32 harm  // 伤害为0 也说明这个位置卡牌没有, 只要有不可能伤害为0
 *					uint8 IsCrit // 是否暴击
 *					}
 *					...
 *				]
 *				uint32 totalharm
 *				uint8 IsCrit // 师傅是否造成暴击, 对于弟子攻击无意义
 * 		}
 * 		...
 *		]
 * }
 * ...
 * ]
 * myself
 * uint32 relation number
 * [
 * uint32 缘技能index ...
 * ]
 * peer
 * uint32 relation number
 * [
 * uint32 缘技能index ...
 * ]
 */

#define CMD_SC_DURATION_MISSION_INFO					CMD_MAKE_SERVER(0x006e) // {uint32 taskindex, uint32 number [{uint32 index 任务ID, uint16 number 完成次数, uint32 刷新时间, uint8 最大星级} ... ]}
#define CMD_SC_DURATION_MISSION_ACCEPT_RESULT					CMD_MAKE_SERVER(0x006f) // 接收任务 {uint16 status(0:成功, 其他:错误), uint8 win or lose 0:win 1:lose uint32 taskindex,uint32 exp, uint32 lastexplimit, uint32 explimit, uint16 lv,  uint32 exp, uint32 lastexplimit, uint32 explimit, uint16 lv(变化后的), uint32 addexp, uint32 coin, uint32 number [{uint32 objid, uint16 lv, uint32 exp, uint32 lastexplimit, uint32 explimit, uint16 lv, 加过后的经验uint32 exp, uint32 lastexplimit, uint32 explimit}...], uint32效果事件index, int32(para1), int32(para2), int32(para3), BOSS奖励uint32效果事件index, int32(para1), int32(para2), int32(para3), uint8 任务分类(主线,精英) uint32 type(满星奖励, 参见E_OBJ_EFFECT), int32 para1, int32 para2, uint8 IsSkip(1:可以跳过, 0:不可跳过) 其他失败 ERROR_MSG_NOT_ENOUGH_ENERGY uint16错误码，uint32下一点回复时间，uint32 全部恢复时间 ERRNO_MSG_DURATION_MISSION_OVER uint16 错误码, uint32 taskindex, uint8 type}(前发送掉卡特效协议,然后在发送BOSS随机事件)
#define CMD_SC_NORMAL_CARD_STRENGTH_RESULT                       CMD_MAKE_SERVER(0x0070) //合成装备获取{uint16 返回结果, uint32 消耗金钱, uint32 强化的card objid, uint32 Crit 强化暴击数量}
#define CMD_SC_SELL_CARD_RESULT                       CMD_MAKE_SERVER(0x0071) //卖卡请求{unit16 status, uint8 type(第一个卡牌的类型)} 
#define CMD_SC_ANNOUNCEMENT_INFO                       CMD_MAKE_SERVER(0x0072) //公告信息拉取{uint16 number [string str1,...]} 
#define CMD_SC_GIFT_PACKET_INFO                       CMD_MAKE_SERVER(0x0073) //礼包信息拉取{uint32 number, uint32 time} 
#define CMD_SC_REQUEST_GIFT_PACKET_RESULT                       CMD_MAKE_SERVER(0x0074) //{uint16 status} 
#define CMD_SC_CHANGE_PWD_RESULT        CMD_MAKE_SERVER(0x0075) //修改密码 {uint16 status}
#define CMD_SC_ROLE_INFO_OPT        CMD_MAKE_SERVER(0x0076) //角色杂类信息通知 {uint32 CDtime, uint8 是否今天首次登陆 0:不是 1:是, uint32 服务器当前时间, uint8 是否今天签到过 0:没有 1:有, uint32 nextenergy, uint32 allenergy, uint32 nextstrength, uint32 allstrength, uint32 extra pro, uint32 mastercount, uint8 IsMax(1以达最大百里拜师次数, 0 没有), uint32 number [{uint32 key, uint32 val}], uint32 currentkey, uint16 精力使用次数, uint16 体力使用次数, uint32 精力使用消耗金钱, uint32 体力使用消耗金钱, uint32 侠客背包上限, uint32 装备背包上限, uint32 侠客背包消耗金钱, uint32 装备背包消耗金钱, uint32 聊天CD, uint8 吃鸡(0 可 1 不可), uint8 喝酒 (0 可 1 不可), uint32 extraDollar(乘以100后的数据), uint32 currentdollarrate, uint32 nextdollarrate, uint32 stamina, uint32 stamin_next_point, uint32 stamina_next_all, uint16 活力使用次数, uint32 恢复活力消耗金钱, uint8 firstchangename(0:第一次改名字 1:不是第一次 ), uint8 firstSendRed(0:第一次塞红包}
#define CMD_SC_INIT_ROLE_RESULT        CMD_MAKE_SERVER(0x0077) //初始化角色信息回包 {uint16 status}
#define CMD_SC_INIT_ROLE_INFO        CMD_MAKE_SERVER(0x0078) //初始化角色提供信息 {uint32 1星弟子卡牌index, uint32 2星弟子卡牌objid, uint32 3星弟子卡牌objid}
#define CMD_SC_CARD_BREAK_AWAKE        CMD_MAKE_SERVER(0x0079) //卡牌突破觉醒 {uint8 cmd(1:none 0:突破), 
 // 0: 变化前的卡牌属性更新loose更新信息, uint32 number, [材料卡牌属性loose更新 ...], 变化后参见卡牌属性loose更新 
 // 1: 获取到的新卡属性 loose}
#define CMD_SC_VIEW_CARD_TEAM_INFO			CMD_MAKE_SERVER(0x007a) // 视图数据更新 {uint16 number [uint32 objid, ...]}
#define CMD_SC_CARD_QUIPMENT_CHANGE_SINGLE_RESULT				CMD_MAKE_SERVER(0x007b) // {uint16 status, uint8 cmd, uint32 objid} 
#define CMD_SC_SWITCH_CARD_EXP_RESULT                       CMD_MAKE_SERVER(0x007c) //传功结果{uint16 status, uint8 flag 0:normal传功 1:强化传功, uint32 objid} 
#define CMD_SC_FRIEND_BOSS_INFO                       CMD_MAKE_SERVER(0x007d) //BOSS信息{uint32 userid, uint32 BossIndex, uint32 cd时间, uint32 boss health, uint32 boss healthmax, uint16 level, string strname} 
#define CMD_SC_FRIEND_BOSS_HARM_HISTORY_INFO                       CMD_MAKE_SERVER(0x007e) //自己BOSS历史战绩信息{uint32 number [{uint32 userid, string name, uint32 time, uint32 skillindex, uint16 skilllevel, uint32 totalharm} ... ]} 
#define CMD_SC_FRIEND_BOSS_HARM_ADD                       CMD_MAKE_SERVER(0x007f) //自己BOSS历史战绩信息更新{uint32 userid, string name, uint32 time, uint32 skillindex, uint16 skilllevel, uint32 totalharm} 
#define CMD_SC_FRIEND_BOSS_HAPPEN                       CMD_MAKE_SERVER(0x0080) //BOSS 出现{uint32 bossindex} 
#define CMD_SC_GACHA_RESULT                       CMD_MAKE_SERVER(0x0082) //GACHA请求{uint16 status, uint8 flag, uint8 number [{uint8 opt 1:新 0:已有, uint32 objid} ... ] } 
#define CMD_SC_TEN_GACHA_RESULT                       CMD_MAKE_SERVER(0x0083) //GACHA十连抽请求{uint16 status, uint8 number [{uint8 opt 1:新 0:已有, uint32 objid} ... ]} 
#define CMD_SC_FRIEND_BOSS_LIST_REQUEST_RESULT                       CMD_MAKE_SERVER(0x0084) //好友BOSS请求{uint16 status, uint16 number [{uint32 userid, uint32 bossindex, uint32 cd, uint32 health, uint32 healthmax, uint16 level, string strname} ... ]} 
#define CMD_SC_FRIEND_BOSS_HISTORY_REQUEST_RESULT                       CMD_MAKE_SERVER(0x0085) //好友BOSS历史战绩信息请求{uint32 number [{uint32 userid, string name, uint32 time, uint32 skillindex, uint16 skilllevel, uint32 totalharm} ... ]}
#define CMD_SC_FRIEND_BOSS_DURATION_REQUEST_RESULT                       CMD_MAKE_SERVER(0x0086) //好友BOSS战斗请求{uint16 status, uint32 userid, uint8 win or lose 0:成功 1:失败 uint32 coin 
 // uint32 number [{uint32 objid, (变化前的)uint16 lv, uint32 exp, uint32 last explimit, uint32 explimit, u(变化后的)int16 lv, uint32 exp, uint32 last explimit, uint32 explimit} ... ]}
#define CMD_SC_FRIEND_BOSS_PRIZE_INFO                       CMD_MAKE_SERVER(0x0087) //好友BOSS战斗奖励推送{uint32 number[ {uint32 prize_id, uint32 userid, string username, uint32 bossindex, uint16 lv, uint32 cd time, uint8 总共可以抽卡次数}...]}
#define CMD_SC_FRIEND_BOSS_ATTACKED_INFO                       CMD_MAKE_SERVER(0x0088) //好友BOSS历史BOSS 记录{uint32 number [{uint32 attacked_id, uint32 userid, string strname, uint32 bossindex, uint16 lv} ... ]}
#define CMD_SC_FRIEND_BOSS_GACHA_REQUEST_RESULT                       CMD_MAKE_SERVER(0x0089) //好友BOSS Gacha请求{uint16 status, uint8 number [{uint32 type, uint32 para1, uint32 para2} ... ]}
#define CMD_SC_STR_DIRTY_CHECK_REQUEST_RESULT                       CMD_MAKE_SERVER(0x008a) //脏词检测请求{uint16 status}
#define CMD_SC_AGREE_FRIEND_REQUEST_RESULT                       CMD_MAKE_SERVER(0x008b) //好友同意请求回复{uint16 status, uint8 agree, uint32 userid}
#define CMD_SC_FRIEND_APPLY_REQUEST_RESULT                      CMD_MAKE_SERVER(0x008c) //好友申请列表回复{uint16 status, uint8 number [{uint32 userid, uint16 lv, string username, uint32 武力值, uint32 masterindex, uint32 lastlogintime} ... ]}
#define CMD_SC_WORLD_BROADCAST_INFO                      CMD_MAKE_SERVER(0x008d) //聊天相关信息推送{uint8 choose, uint32 userid(0:系统), string username, string content, uint16 viplv}
#define CMD_SC_WORLD_BROADCAST_REQUEST_RESULT                       CMD_MAKE_SERVER(0x008e) //好友聊天请求{uint16 status}
#define CMD_SC_SHOP_USE_CANCEL_RESULT                        CMD_MAKE_SERVER(0x008f) //商城道具使用效果撤销(目前只有免战牌有用) {uint16 status}
#define CMD_SC_TIMER_GIFT_INFO                        CMD_MAKE_SERVER(0x0090) //计时礼包数据更新 记时礼包index 是连续的 {uint32 CD, uint32 giftindex(要领的gift:注意这里当领完所有礼包的时候,会发送0, 配表里会找不到)}
#define CMD_SC_REQUSET_TIMER_GIFT_RESULT                        CMD_MAKE_SERVER(0x0091) //请求计时礼包回包 {uint16 status, uint32 giftindex, uint32 type (参加E_OBJ_EFFECT), uint32 para1, uint32 para2}
#define CMD_SC_ACTIVE_OPEN_INFO                        CMD_MAKE_SERVER(0x0092) 
 //活动数据 目前只有四个活动{uint16 number [{uint32 active_id, 
 //	uint8 IsOpen 1:开启 0:未开启
 // FRIEND_BOSS_ACTIVE_ID
 // {}
 //	TURN_NORMAL_TABLE_ACTIVE_ID:
 //{
 //	uint32 number 转盘内容 目前一定为16
 //	[
 //		uint8 type
 //		uint32 para1
 //		uint32 para2
 //	]
 //	uint32 consume
 //}
 //TURN_SUPER_TABLE_ACTIVE_ID:
 //{
 //	uint32 number 转盘内容 目前一定为16
 //	[
 //		uint8 type
 //		uint32 para1
 //		uint32 para2
 //	]
 //	uint32 consume
 //}
 //FRIEND_BOSS_RANK_ACTIVE_ID:
 //{
 //	string content;
 //	uint16 number 排名奖励数目
 //	[
 //		{uint32 startrank, uint32 endrank, uint16 number [{uint16 type, uint32 para1, uint32 para2}... ] string tips}...
 //	]
 //}
 //
 //CHARGE_SINGLE_ACTIVE_ID:
 //CHARGE_ACCUMULATE_ACTIVE_ID:
 //CHARGE_CYCLE_ACTIVE_ID:
 //{
 //	uint16 number 单笔充值奖励数目
 //	[
 //		{uint32 index, [{uint16 type, uint32 para1, uint32 para2} ...(4次 写死的) ],uint32 prize, uint32 number(0:无限制, 其他:最大购买数据) string tips}...
 //	]
 //}
 //
 //LOGIN_EXTRA_ACTIVE_ID:
 //{
 //	uint16 number 累计登录奖励
 //	[
 //		{uint32 index, [{uint16 type, uint32 para1, uint32 para2} ...(3次 写死的) ],uint32 nextindex, string tips}...
 //	]
 //}
 //
 //FUND_ACTIVE_ID:
 //{
 //	uint16 number 基金奖励
 //	[
 //		{uint32 index, [{uint16 type, uint32 para1, uint32 para2} ...(3次 写死的) ], uint32 CostCoin, uint32 costdollar, uint32 day, string tips, uint32 returncoin, uint32 returndollar}...
 //	]
 //}
 //TASK1_ACTIVE_ID:
 //TASK2_ACTIVE_ID:
 //...
 //TASK7_ACTIVE_ID:
 //{
 //		uint16 number [{uint32 index, uint32 cd}...]
 //}
 //} ... ]}
#define CMD_SC_SIGN_ACTIVE_INFO                        CMD_MAKE_SERVER(0x0093) //签到数据请求{uint32 签到次数, uint32 剩余额外补签次数}
#define CMD_SC_LOGIN_SIGN_ACTIVE_RESULT                        CMD_MAKE_SERVER(0x0094) //签到活动回复 {uint16 status, uint8 choose, uint32 type, uint32 para1, uint32 para2}
#define CMD_SC_STORE_REQUEST_RESULT                        CMD_MAKE_SERVER(0x0095) //商场数据回复 {uint8 storechannel (STORE_DOLLAR:dollar shop  STORE_PRESTIGE:prestige shop) uint32 number [{uint32 storeid, uint8 storechannel, uint32 shopid, uint16 商场分类, uint8 货币类型(STORE_OBJ_XXX), uint32 货币数量, uint8 IsBuy, uint32 viewOrder, string content, uint32 商场物品数量, uint32 赠送数量, int32 剩余role限制(-1:不限, 其他:剩余次数), int32 剩余day限制(-1:不限, 其他:剩余限制), uint32 折扣显示价格, uint32 最大购买数量, uint8 goodstype(1:card, 3:道具)} ...]}
#define CMD_SC_GAME_FLOW_REQUEST_RESULT                        CMD_MAKE_SERVER(0x0096) //新手流程回复 {uint16 status, uint32 key, uint32 val, uint32 currentkey}
#define CMD_SC_CLOSE_CONNECT                        CMD_MAKE_SERVER(0x0097) //服务要断开连接 {}
#define CMD_SC_HEARTBEAT                        CMD_MAKE_SERVER(0x0098) //服务心跳包 {uint32 timestamp}
#define CMD_SC_HIGH_DURATION_MISSION_INFO					CMD_MAKE_SERVER(0x0099) // {uint32 taskindex, uint32 taskindexlimit uint32 number [{uint32 index 任务ID, uint16 number 完成次数, uint32 刷新时间, uint8 最大星级} ... ]}
#define CMD_SC_TURNTABLE_TURN_RESULT                       CMD_MAKE_SERVER(0x009a) //转盘转动回复 {uint16 status, uint8 choose, uint32 number(请求转动次数), uint32 number(获得的物品数量) [{uint32 count(后面的这一类型获取次数), int index} ... ], uint32 random 最后的选择} ERRNO_MSG_GOODS_NOT_ENOUGH {uint16 status, uint32 consume, uint8 choose}
#define CMD_SC_ACTIVE_BASE_INFO                       CMD_MAKE_SERVER(0x009b) //活动基本数据拉取 {uint16 number [{uint32 index, uint8 type, string name, string content, uint16 opr, uint32 para, uint32 CD(0:开启), uint32 starttime, uint32 endtime} ... ]}
#define CMD_SC_DATA_NOTICE_PUSH                      CMD_MAKE_SERVER(0x009c) //数据通知推送 {uint8 charge 
 //(1:充值到账), uint32 到账的元宝
 //(2:vip 升级), uint32 viplv
 //(3:lv 升级), uint32 prelv, uint32 newlv
 //(4:好友讨伐BOSS失败), string username, uint32 userid, string bossname 
 //(5:任务完成), uint32 mission index
 //}
#define CMD_SC_RECOVER_INFO                      CMD_MAKE_SERVER(0x009d) //每日吃鸡和酒活动 {uint32 CD, uint8 (0:不可恢复精力 1:可以恢复精力), uint8 (0:不可恢复体力 1:可以恢复体力) }
#define CMD_SC_RECOVER_RESULT                      CMD_MAKE_SERVER(0x009e) //请求吃鸡和酒回复 {uint16 status, uint8 choose(0:酒 1:肉)}
#define CMD_SC_ACHIEVE_INFO                      CMD_MAKE_SERVER(0x009f) //任务成就数据 {uint32 number [{uint32 index, uint8 status(0:未完成 1:可以领取 2:完成), uint32 step}...]}
#define CMD_SC_ACHIEVE_RESULT                      CMD_MAKE_SERVER(0x00a0) //任务成就请求奖励回复 {uint16 status, uint32 index, uint32 coin, uint32 dollar, uint32 energy, uint32 strength, uint32 worldhistory, uint32 prestige, {uint32 type uint32 para1, uint32 para2(E_OBJ_TYPE)}
#define CMD_SC_ACHIEVE_BASE_INFO                      CMD_MAKE_SERVER(0x00a1) //任务成就基础数据 {uint32 成就值, uint32 活跃值}
#define CMD_SC_FIRST_CHARGE_RESULT                      CMD_MAKE_SERVER(0x00a2) //首从奖励回复 {uint16 status}
#define CMD_SC_UPGRADE_RESULT                      CMD_MAKE_SERVER(0x00a3) //进化申请回复 {uint16 status, uint32 upgradeindex, uint32 objid}
#define CMD_SC_UPGRADE_REVIEW_RESULT                      CMD_MAKE_SERVER(0x00a4) //进化预览申请回复 {uint16 status, 新卡属性 loose}
#define CMD_SC_SKILL_STRENGTH_REVIEW_RESULT                      CMD_MAKE_SERVER(0x00a5) //技能升级预览请求回复 {uint16 status, uint16 pro}
#define CMD_SC_FRIEND_BOSS_RANK_INFO                      CMD_MAKE_SERVER(0x00a6) // 好友BOSS 排行{uint16 number本次top排名数量 [{uint32 userid, uint16 level, string name, uint16 number [uint32 cardindex ...], uint32 masterindex, uint32 devote, uint32 rank(-1:还未参数排名, 0:MAX_FRIEND_BOSS_RANK_NUMBER最大排名之外} ...], uint32 rank, uint16 number(前次排名数量) [{同上} ...], uint32 rank(-1:还未参与, 0:MAX_FRIEND_BOSS_RANK_NUMBER)}
#define CMD_SC_FRIEND_BOSS_RANK_DEVOTE_INFO                      CMD_MAKE_SERVER(0x00a7) // 好友BOSS 贡献度{uint32 devote}
#define CMD_SC_SINGLE_CHARGE_INFO                      CMD_MAKE_SERVER(0x00a8) // 单笔充值活动数据{uint32 number [{uint32 index, uint32 have, uint8 IsOk} ... ]}
#define CMD_SC_ACCUMULATE_CHARGE_INFO                      CMD_MAKE_SERVER(0x00a9) // 累积充值活动数据{uint32 point(累积充值值), uint32 number [{uint32 index, uint32 have, uint8 IsOk} ... ]}
#define CMD_SC_CYCLE_CHARGE_INFO                      CMD_MAKE_SERVER(0x00aa) // 循环充值活动数据{uint32 point(累积充值值), uint32 number [{uint32 index, uint32 have, uint8 IsOk} ... ]}
#define CMD_SC_COMMON_ACTIVE_GET_RESULT                      CMD_MAKE_SERVER(0x00ab) // 常规活动领取回复{uint16 status, uint32 activeindex, uint32 targetid}
#define CMD_SC_CYCLE_LOGIN_ACTIVE_INFO                      CMD_MAKE_SERVER(0x00ac) // 循环登录活动领取推送{uint32 number [{uint32 index, [{uint16 type, uint32 para1, uint32 para2} ... (3个写死的)], uint32 nextindex, string strdesc, uint8 status(1:可以领取 0：不可领取 2:已领取} ... ]}
#define CMD_SC_EXTRA_LOGIN_ACTIVE_INFO                      CMD_MAKE_SERVER(0x00ad) // 额外登录活动领取数据推送{uint32 choose, uint8 IsOk}
#define CMD_SC_FUND_ACTIVE_INFO                      CMD_MAKE_SERVER(0x00ae) // 基金活动领取请求{uint32 number [{uint32 targetid, uint32 endtime, std::string desc} ... ]}
#define CMD_SC_ACTIVE_DURATION_MISSION_ACTIVE_INFO					CMD_MAKE_SERVER(0x00af) // {uint32 number [{uint32 index 任务ID, uint16 number 完成次数, uint32 刷新时间, uint8 最大星级} ... ]}
#define CMD_SC_JOIN_PLAYER_RESULT					CMD_MAKE_SERVER(0x00b0) // {uint16 status, uint32 number, [{uint32 userid, uint16 level, std::string name, uint32 masterindex, uint8 IsFriend} ... ], uint16 number, uint16 maxnumber}
#define CMD_SC_PRODUCT_GET_INFO					CMD_MAKE_SERVER(0x00b1) 
// {type uint16, 
//PRODUCT_GET: 
//uint32 number, [{uint32 type, uint32 para1, uint32 para2} ... ]}
//HAPPEN_BOSS: {}
//HAPPEN_ROLE: {uint32 userid, uint16 level, string username, uint32_t masterindex, uint32 break count, uint32 break count max, uint8 awake, string 个性签名}
//
#define CMD_SC_TRIAL_INFO					CMD_MAKE_SERVER(0x00b2) // {uint32 index, uint32 remain_chanllenge (-1最大次数), uint16 consumestamina, uint32 stamina, uint32 roleexp, uint32 coin, uint32 cardexp, uint32 CD(-1: 无上限, 其他:时间(单位秒)), uint32 IsNextIndex(0:没有开启, 其他:开启关卡ID)}
#define CMD_SC_TRIAL_RESULT					CMD_MAKE_SERVER(0x00b3) // {uint16 status, uint32_t roleexp, uint32_t coin, uint32_t cardexp}
#define CMD_SC_TRIAL_NEXT_RESULT					CMD_MAKE_SERVER(0x00b4) // {uint16 status} status等于ERRNO_MSG_TRIAL_LEVEL {uint16 status, uint16 needlevel}
#define CMD_SC_ROLE_INFO_UPDATE_RESULT					CMD_MAKE_SERVER(0x00b5) // {uint16 status}
#define CMD_SC_CDKEY_RESULT					CMD_MAKE_SERVER(0x00b6) // {uint16 status}

//server tower begin
#define CMD_SC_TOWER_RANK   CMD_MAKE_SERVER(0x00b7)//排行榜{uint32 number, [(uint32 user_id, string name, uint32 level, uint32 layer, uint32 masterindex) …]}
#define CMD_SC_TOWER_COMBAT_RESULT  CMD_MAKE_SERVER(0x00b8)//战斗返回{uint16 status(0:正常，其他：错误码）,uint8 win (0:win,1:lose)}
#define CMD_SC_TOWER_INFO    CMD_MAKE_SERVER(0x00b9)//服务端推送tower关卡{uint32 index,uint32 health,uint32 healthMax,uint32 闯关次数,uint32 sword,uint32 hand,uint32 inner, uint32 dark, uint32 stick,uint32 threeAttack,uint32 twoAttack,uint32 isMaxLayer,uint32 remain}
//server tower end
#define CMD_SC_DURATION_MISSION_ALL_STAR_RESULT   CMD_MAKE_SERVER(0x00ba)//全三星奖励{uint16 status, uint32 index}
#define CMD_SC_DURATION_MISSION_ALL_STAR_INFO   CMD_MAKE_SERVER(0x00bb)//全三星奖励{uint32 number, [{uint32 index, uint8(ALL_STAR_*)} ...]}
#define CMD_SC_MISSION_ACTIVE_PRIZE_INFO   CMD_MAKE_SERVER(0x00bc)//活跃奖励{uint32 number, [{uint32 index, uint32 activeval, uint8(0:不可领取, 1:可以领取, 2:已领取(EnumDefine ACTIVE_PRIZE_*, uint16 type, uint32 para1, uint32 para2, uint16 type, uint32 para1,uint32 para2} ...]}
#define CMD_SC_ACTIVE_PRIZE_GET_RESULT   CMD_MAKE_SERVER(0x00bd)//活跃奖励获取{uint16 status, uint32 index(奖励index)}


//////////////////////////////////////////////////////////////////////////
//命令定义规范
//奇数为 Client 发送到 Server 
//偶数为 Server 发送到 Client
//Client 发送到 LoginServer
#define CMD_LOGIN_CS_TEST           CMD_MAKE_CLIENT(0x0001) //测试用 echo{string发送字符串}
#define CMD_LOGIN_CS_SERVER_LIST         CMD_MAKE_CLIENT(0x0002) //客户端91获取服务器列表{uint8 flag, 1:GM版拉取会获取，禁止登录的服务器, 0:正常拉取}
#define CMD_LOGIN_CS_PATCH_URL         CMD_MAKE_CLIENT(0x0003) //客户端获取下载地址{uint32 version}
#define CMD_LOGIN_CS_REG            CMD_MAKE_CLIENT(0x0004) //客户端注册 {std::string account, uint16 channel,string pwd, string mail}       
#define CMD_LOGIN_CS_LOGIN          CMD_MAKE_CLIENT(0x0005) //客户端登录 {std::string uid(不关心), uint32版本号 string name  string pwd uint16 channel}
#define CMD_LOGIN_CS_FASTLOGIN      CMD_MAKE_CLIENT(0x0006) //客户端登快速登录 {std::string uid, uint32版本号 string name(不关心)  string pwd uint16 channel}
#define CMD_LOGIN_CS_OTHERLOGIN        CMD_MAKE_CLIENT(0x0007) //第三方客户端登录 {std::string uid(统计用), uint32版本号 string name  string pwd uint16 channel, (为了统计string mail, string token, std::string reg_device(首次登陆设备), std::string reg_device_type(首次登陆设备型号), std::string second_channel, uint16 area_id, uint8 flag}
#define CMD_LOGIN_CS_VERSION_NEED        CMD_MAKE_CLIENT(0x0008) //要求最低版本 {}
#define CMD_LOGIN_CS_AD        CMD_MAKE_CLIENT(0x0009) //要求最低版本 {uint32 version}
#define CMD_LOGIN_CS_SERVER_LIST2         CMD_MAKE_CLIENT(0x0010) //客户端91获取服务器列表{uint8 flag, 1:GM版拉取会获取，禁止登录的服务器, 0:正常拉取}

//////////////////////////////////////////////////////
//LoginServer 发送到 Client的命令
#define CMD_LOGIN_SC_TEST                   CMD_MAKE_SERVER(0x1001) //测试用 echo 服务器返回{string返回字符串;}                    
#define CMD_LOGIN_SC_SERVER_LIST        CMD_MAKE_SERVER(0x1002)     //服务器列表{uint16 number [{uint8 IsAlive, uint16 服务器ID， string ip, uint32 port, uint32 version, uint8 status(E_AREA_STATUS_STOP ...), uint8 是否是新服, string servername, string domain}...], string 登录通知} 
#define CMD_LOGIN_SC_PATCH_URL                  CMD_MAKE_SERVER(0x1003) //服务器返回{uint32 version, string返回patch url字符串, string 广告一类的url字符串, string 增量url字符串}
#define CMD_LOGIN_SC_REGRESULT              CMD_MAKE_SERVER(0x1004) //注册返回 uint16 结果 string name string pwd                   
#define CMD_LOGIN_SC_LOGINRESULT            CMD_MAKE_SERVER(0x1005) //登录返回 uint16 结果                       
#define CMD_LOGIN_SC_FASTRESULT            CMD_MAKE_SERVER(0x1006) //登录返回 uint16 结果                       
#define CMD_LOGIN_SC_OTHERRESULT            CMD_MAKE_SERVER(0x1007) //登录返回 uint16 结果                       
#define CMD_LOGIN_SC_VERSION_NEED_RESULT            CMD_MAKE_SERVER(0x1008) // 版本检测结果{uint32 version (要求最低版本)}                      
#define CMD_LOGIN_SC_AD_RESULT            CMD_MAKE_SERVER(0x1009) // 公告结果{uint32 number [string desc ... ]}                      
#define CMD_LOGIN_SC_SERVER_LISTV2        CMD_MAKE_SERVER(0x100a)     //服务器列表{uint16 number [{uint8 IsAlive, uint16 服务器ID， string ip, uint32 port, uint32 version, uint8 status(E_AREA_STATUS_STOP ...), uint8 是否是新服, string servername, string domain, string patch url, string 广告url}...], string 登录通知} 
#endif
