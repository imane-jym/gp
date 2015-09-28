#ifndef __GM_COMMAND_EXT_H__
#define __GM_COMMAND_EXT_H__

#include "GmCommand.h"
#include "ConfigData.h"
#include "CommonType.h"

class ActiveFriendBoss : public GmCommand
{
	virtual bool Validate();
	virtual bool Trigger();
	virtual bool Complete();
};

class SendMailCommand : public GmCommand
{
public:
	virtual bool Validate();
	virtual bool Trigger();
	virtual bool Complete();

	//virtual GmCommandHandleResult HandleServer();
	virtual bool HandleRole( void* pRole );

private:
	std::string title;
	std::string content;
	uint32 type;
	uint32 para1;
	uint32 para2;
	uint32 coin;
	uint32 dollar;
	uint32 energy;
	uint32 strength;
	uint32 worldhistory;
};

class CancelCommand : public GmCommand
{
public:
	virtual bool Validate();
	virtual bool Trigger();
	virtual bool Complete();

private:
	uint32	m_dwGmAutoId;
};

class TurntableCommand : public GmCommand
{
	virtual bool Validate();
	virtual bool Trigger();
	virtual bool Complete();
private:
	uint8 m_byChoose;
//	std::vector<STruntable> m_vecNormal;
//	std::vector<STruntable> m_vecSuper;
};

class ReloadConfigCommand : public GmCommand
{
public:
	virtual bool Validate();
	virtual bool Trigger();
	virtual bool Complete();
private:
	std::string m_strTable;
};

class AnnouncementCommand : public GmCommand
{
public:
	virtual bool Validate();
	virtual bool Trigger();
	virtual bool Complete();
private:
	uint32 m_Id;
	uint8 m_cmd;
	std::string m_para1;
	std::string m_strTitle;
	std::string m_strContent;
};

class SetRoleAuth: public GmCommand
{
public:
	virtual bool Validate();
	virtual bool Trigger();
	virtual bool Complete();
private:
	uint32 userid;
	uint8 gm;
};

class SetRoleAuth2: public GmCommand
{
public:
	virtual bool Validate();
	virtual bool Trigger();
	virtual bool Complete();
private:
	uint32 userid;
	uint32 limit;
};


class ActiveExpAdd: public GmCommand
{
	virtual bool Validate();
	virtual bool Trigger();
	virtual bool Complete();
};

class ActiveObjDrop: public GmCommand
{
	virtual bool Validate();
	virtual bool Trigger();
	virtual bool Complete();
};

class ActiveCoinAdd: public GmCommand
{
	virtual bool Validate();
	virtual bool Trigger();
	virtual bool Complete();
};

class ActiveBossRank: public GmCommand
{
	virtual bool Validate();
	virtual bool Trigger();
	virtual bool Complete();
};

class SetServer: public GmCommand
{
	virtual bool Validate();
	virtual bool Trigger();
	virtual bool Complete();
private:
	uint8_t m_areaid;
	uint8_t m_status;
	uint8_t m_new;
	uint8_t m_canlogin;
	std::string m_name;
	std::string m_domain;
	std::string m_resurl;
};

class ActiveCharge: public GmCommand
{
	virtual bool Validate();
	virtual bool Trigger();
	virtual bool Complete();
private:
	uint8_t m_activeIndex;
	std::string m_content;
};

class ActiveExtraLogin: public GmCommand
{
	virtual bool Validate();
	virtual bool Trigger();
	virtual bool Complete();
//private:
//	std::string m_content;
};

class ActiveFund: public GmCommand
{
	virtual bool Validate();
	virtual bool Trigger();
	virtual bool Complete();
//private:
//	std::string m_content;
};

class ActiveTask: public GmCommand
{
	virtual bool Validate();
	virtual bool Trigger();
	virtual bool Complete();
private:
	uint32_t m_activeIndex;
};

class StoreLoad: public GmCommand
{
	virtual bool Validate();
	virtual bool Trigger();
	virtual bool Complete();
private:
	std::vector<STC_CONF_STORE> m_vec;
};

class ForceCancel: public GmCommand
{
	virtual bool Validate();
	virtual bool Trigger();
	virtual bool Complete();
private:
	uint32_t m_activeIndex;

};

class WhiteList: public GmCommand
{
	virtual bool Validate();
	virtual bool Trigger();
	virtual bool Complete();
private:
	std::vector<uint32_t> m_vecUserid;

};

class NoticeCtrl: public GmCommand
{
	virtual bool Validate();
	virtual bool Trigger();
	virtual bool Complete();
private:
	std::string m_strContent;
	uint8_t m_byChatType;
	uint8_t m_byChoose;
};
//class SetServerStateCommand : public GmCommand
//{
//public:
//	virtual bool Validate();
//	virtual bool Trigger();
//	virtual bool Complete();
//private:
//	uint8	m_byState;
//};
//
//class SetLoginStrategyCommand : public GmCommand
//{
//public:
//	virtual bool Validate();
//	virtual bool Trigger();
//	virtual bool Complete();
//private:
//	uint32	m_dwStrategyId;
//};
//
//class SetServerNameCommand : public GmCommand
//{
//public:
//	virtual bool Validate();
//	virtual bool Trigger();
//	virtual bool Complete();
//private:
//	std::string m_strServerName;
//};
//
//class SetResServerAddrCommand : public GmCommand
//{
//public:
//	virtual bool Validate();
//	virtual bool Trigger();
//	virtual bool Complete();
//private:
//	std::string m_resServerAddr;
//};
//
//class SetResVersionCommand : public GmCommand
//{
//public:
//	virtual bool Validate();
//	virtual bool Trigger();
//	virtual bool Complete();
//private:
//	std::string m_strResVersion;
//};
//
//
//class BroadcastCommand : public GmCommand
//{
//public:
//	virtual bool Validate();
//	virtual bool Trigger();
//	virtual bool Complete();
//	virtual void Tick();
//private:
//	std::string m_strContent;
//	uint32		m_dwInterval;
//	uint32		m_dwLastUpdateTime;
//};
//
//class UpdateShopCommand : public GmCommand
//{
//public:
//	virtual bool Validate();
//	virtual bool Trigger();
//	virtual bool Complete();
//private:
//	ShopConfig 	m_shopConfig;
//};
//
//class ActivityExpBonusCommand : public GmCommand
//{
//public:
//	virtual bool Validate();
//	virtual bool Trigger();
//	virtual bool Complete();
//private:
//	std::string		m_strSmallPicName;
//	std::string		m_strBigPicName;
//	uint32			m_dwMinutesAhead;
//	uint16			m_wBonusRate;
//};
//
//class ActivityDiamondCommand : public GmCommand
//{
//public:
//	virtual bool Validate();
//	virtual bool Trigger();
//	virtual bool Complete();
//private:
//	std::string		m_strSmallPicName;
//	std::string		m_strBigPicName;
//	uint32			m_dwMinutesAhead;
//	bool			m_bIsCharge;
//	ThresholdAndReward m_mapRequestAndReward;
//};
//
//class ActivityExchangeCommand : public GmCommand
//{
//public:
//	virtual bool Validate();
//	virtual bool Trigger();
//	virtual bool Complete();
//private:
//	std::string		m_strSmallPicName;
//	std::string		m_strBigPicName;
//	uint32			m_dwMinutesAhead;
//	uint32			m_dwUseItemId;
//	uint32			m_dwUseItemCnt;
//	uint32			m_dwGetItemId;
//	uint32			m_dwGetItemCnt;
//};
//
//class ActivitySpecialDropCommand : public GmCommand
//{
//public:
//	virtual bool Validate();
//	virtual bool Trigger();
//	virtual bool Complete();
//private:
//	std::string		m_strSmallPicName;
//	std::string		m_strBigPicName;
//	uint32			m_dwMinutesAhead;
//	uint32			m_dwDropItemId;
//	uint16			m_wDropProp;
//	uint8			m_byMustElite;
//};
//
//class ActivityLotteryCommand : public GmCommand
//{
//public:
//	virtual bool Validate();
//	virtual bool Trigger();
//	virtual bool Complete();
//private:
//	uint32	m_dwDelayCloseDay;
//};
//
//class ActivityAccLoginCommand : public GmCommand
//{
//public:
//	virtual bool Validate();
//	virtual bool Trigger();
//	virtual bool Complete();
//private:
//	std::string		m_strSmallPicName;
//	std::string		m_strBigPicName;
//	uint32			m_dwMinutesAhead;
//	ThresholdAndReward m_mapReqAndReward;
//};
//
//class ActivityStageCommand : public GmCommand
//{
//public:
//	virtual bool Validate();
//	virtual bool Trigger();
//	virtual bool Complete();
//private:
//	std::string		m_strSmallPicName;
//	std::string		m_strBigPicName;
//	uint32			m_dwMinutesAhead;
//	ThresholdAndReward m_mapStageAndReward;
//};
//
//class ActivityArenaRankCommand : public GmCommand
//{
//public:
//	virtual bool Validate();
//	virtual bool Trigger();
//	virtual bool Complete();
//private:
//	std::string		m_strSmallPicName;
//	std::string		m_strBigPicName;
//	uint32			m_dwMinutesAhead;
//	ThresholdAndReward	m_mapRankAndReward;
//};
//
//class ActivityRoleLevelCommand : public GmCommand
//{
//public:
//	virtual bool Validate();
//	virtual bool Trigger();
//	virtual bool Complete();
//private:
//	std::string		m_strSmallPicName;
//	std::string		m_strBigPicName;
//	uint32			m_dwMinutesAhead;
//	ThresholdAndReward	m_mapLevelAndReward;
//};
//
//class ActivityCommon : public GmCommand
//{
//public:
//	virtual bool Validate();
//	virtual bool Trigger();
//	virtual bool Complete();
//private:
//	std::string		m_strSmallPicName;
//	std::string		m_strBigPicName;
//	uint32			m_dwMinutesAhead;
//};
//
//class ActivityExtendRewardCommand : public GmCommand
//{
//public:
//	virtual bool Validate();
//	virtual bool Trigger();
//	virtual bool Complete();
//
//private:
//	std::string		m_strSmallPicName;
//	std::string		m_strBigPicName;
//	uint32			m_dwMinutesAhead;
//	uint8			m_byIdType;
//	std::string		m_strTableName;
//};
//
//class SetGameServerNoticeCommand : public GmCommand
//{
//public:
//	virtual bool Validate();
//	virtual bool Trigger();
//	virtual bool Complete();
//private:
//	std::string		m_strNotice;
//};
//
//class SetLoginServerNoticeCommand : public GmCommand
//{
//public:
//	virtual bool Validate();
//	virtual bool Trigger();
//	virtual bool Complete();
//private:
//	std::string		m_strNotice;
//};
//
//class SetGmAuthCommand : public GmCommand
//{
//public:
//	virtual bool Validate();
//	virtual bool Trigger();
//	virtual bool Complete();
//
//private:
//	uint8	m_byGmAuth;
//};
//
//class ReCreateRobotCommand : public GmCommand
//{
//public:
//	virtual bool Validate();
//	virtual bool Trigger();
//	virtual bool Complete();
//};


#endif
