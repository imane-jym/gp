/*
 * =====================================================================================
 *
 *       Filename:  CommonType.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2013年05月27日 05时01分25秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  imane (), imanecr@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#ifndef _COMMONTYPE_H_
#define _COMMONTYPE_H_

#include <stdint.h>

struct STruntable 
{
	uint8_t type;
	int32_t para1;
	int32_t para2;
	uint32_t pro;
};

struct SObjEffect
{
	uint32_t number;
	uint32_t type;
	int32_t para1;
	int32_t para2;
};

struct SServerData
{
	uint32_t serverid;	
	uint8_t canlogin;
	uint8_t status;
	uint8_t isnew;
	std::string servername;
	std::string ip;
	uint32_t port;
	std::string domain;
	std::string resurl;
};

struct SActive
{
	uint8 byIsOpen;
};

struct SWorldChat
{
	uint8_t choose;
	uint32_t userid;
	std::string username;
	std::string content;
	uint16 viplv;
};

struct SGift
{
	uint32_t role;
	uint32_t day;
};

struct SObjType
{
	uint32_t type;
	uint32_t para1;
	uint32_t para2;
};

struct SMailObj
{
	uint32_t coin;
	uint32_t dollar;
	uint32_t energy;
	uint32_t strength;
	uint32_t worldhistory;
	SObjType obj;
};

struct SMissionObj
{
	uint32_t coin;
	uint32_t dollar;
	uint32_t energy;
	uint32_t strength;
	uint32_t worldhistory;
	uint32_t prestige;
	SObjType obj;
};

struct SPlayer
{
	uint32_t userid;
	uint16 level;
	std::string username;
};

struct SProduct
{
	uint16_t type;
	uint32_t para1;
	uint32_t para2;
};

struct SFBRankGift
{
	SProduct obj1;
	SProduct obj2;
	SProduct obj3;
	std::string tips;
};

struct SRank
{
	uint32_t start;
	uint32_t end;
};

struct SFBRankRole
{
	uint32_t userid;
	uint16_t level;
	std::string name;
	std::vector<uint32_t> cardindex;
	uint32_t masterindex;
	uint32_t devote;
};

struct SFBRank
{
	uint32_t devote;
	uint32_t userid;
};
//struct SActiveTime
//{
//	uint8_t byType;
//	uint32_t dwPara[MAX_ACTIVE_TIME_COUNT];
//	uint32_t dwStart;
//	uint32_t dwEnd;
//};

struct SChargePrize
{
	SProduct obj1;
	SProduct obj2;
	SProduct obj3;
	SProduct obj4;
	uint32_t prize;
	uint32_t number;
	std::string desc;
};

struct SCommonActivePara
{
	uint32_t para1;
	uint32_t para2;
};

struct SExtraLogin
{
	uint32_t index;
	SProduct obj1;
	SProduct obj2;
	SProduct obj3;
	uint32_t nextindex;
	std::string desc;
};

struct SFund
{
	SProduct obj1;
	SProduct obj2;
	SProduct obj3;
};

struct SFundArr
{
	uint32_t GetTime;
	uint32_t EndTime;
	SFund EveryPrize;
	uint32_t CostCoin;
	uint32_t CostDollar;
	std::string mailtitle;
	std::string mailcontent;
	uint32_t ReturnCoin;
	uint32_t ReturnDollar;
	std::string Desc;
};

struct SFundActive
{
	uint32_t index;
	SProduct obj1;
	SProduct obj2;
	SProduct obj3;
	uint32_t CostCoin;
	uint32_t CostDollar;
	uint32_t Day;
	std::string desc;
	uint32_t ReturnCoin;
	uint32_t ReturnDollar;
};

struct SJoin
{
	uint32_t userid;
	uint16 level;
	std::string username;
	uint32_t masterindex;
	uint32_t breakcount;
	uint32_t maxbreakcount;
	uint8_t awake;
	std::string sigature;
};

struct CDKEY_PRIZE
{
	uint32_t coin;
	uint32_t dollar;
	uint32_t worldhistory;
	SProduct obj[6];
	std::string title;
	std::string content;
};

//塔 排行榜
typedef struct _STowerRank{
	uint32 userId; //用户id
	std::string userName;
	uint32 layer; //闯关层数
	uint32 userLevel; //用户等级
	uint32 masterIndex; //上榜次数
} STowerRank;

#endif
