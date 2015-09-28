#ifndef __GM_COMMAND_EXT_H__
#define __GM_COMMAND_EXT_H__

#include "GmCommand.h"
#include "ConfigReader.h"

//class ActiveFriendBoss : public GmCommand
//{
//	virtual bool Validate();
//	virtual bool Trigger();
//	virtual bool Complete();
//};
//
//class SendMailCommand : public GmCommand
//{
//public:
//	virtual bool Validate();
//	virtual bool Trigger();
//	virtual bool Complete();
//
//	//virtual GmCommandHandleResult HandleServer();
//	virtual bool HandleRole( void* pRole );
//
//private:
//	std::string title;
//	std::string content;
//	uint32 type;
//	uint32 para1;
//	uint32 para2;
//	uint32 coin;
//	uint32 dollar;
//	uint32 energy;
//	uint32 strength;
//	uint32 worldhistory;
//};

class CancelCommand : public GmCommand
{
public:
	virtual bool Validate();
	virtual bool Trigger();
	virtual bool Complete();

private:
	uint32	m_dwGmAutoId;
};

//class TurntableCommand : public GmCommand
//{
//	virtual bool Validate();
//	virtual bool Trigger();
//	virtual bool Complete();
//private:
//	uint8 m_byChoose;
//};
//
//class ReloadConfigCommand : public GmCommand
//{
//public:
//	virtual bool Validate();
//	virtual bool Trigger();
//	virtual bool Complete();
//};
//
//class AnnouncementCommand : public GmCommand
//{
//public:
//	virtual bool Validate();
//	virtual bool Trigger();
//	virtual bool Complete();
//private:
//	uint32 m_Id;
//	uint8 m_cmd;
//	std::string m_para1;
//	std::string m_strTitle;
//	std::string m_strContent;
//};
//
//class SetRoleAuth: public GmCommand
//{
//public:
//	virtual bool Validate();
//	virtual bool Trigger();
//	virtual bool Complete();
//private:
//	uint32 userid;
//	uint8 gm;
//};
//
//class ActiveExpAdd: public GmCommand
//{
//	virtual bool Validate();
//	virtual bool Trigger();
//	virtual bool Complete();
//};
//
//class ActiveObjDrop: public GmCommand
//{
//	virtual bool Validate();
//	virtual bool Trigger();
//	virtual bool Complete();
//};
//
//class ActiveCoinAdd: public GmCommand
//{
//	virtual bool Validate();
//	virtual bool Trigger();
//	virtual bool Complete();
//};
//
//class ActiveBossRank: public GmCommand
//{
//	virtual bool Validate();
//	virtual bool Trigger();
//	virtual bool Complete();
//};
//
//class SetServer: public GmCommand
//{
//	virtual bool Validate();
//	virtual bool Trigger();
//	virtual bool Complete();
//private:
//	uint8_t m_areaid;
//	uint8_t m_status;
//	uint8_t m_new;
//	uint8_t m_canlogin;
//	std::string m_name;
//};
//
//class ActiveCharge: public GmCommand
//{
//	virtual bool Validate();
//	virtual bool Trigger();
//	virtual bool Complete();
//private:
//	uint8_t m_activeIndex;
//	std::string m_content;
//};
//
//class ActiveExtraLogin: public GmCommand
//{
//	virtual bool Validate();
//	virtual bool Trigger();
//	virtual bool Complete();
////private:
////	std::string m_content;
//};
//
//class ActiveFund: public GmCommand
//{
//	virtual bool Validate();
//	virtual bool Trigger();
//	virtual bool Complete();
////private:
////	std::string m_content;
//};
//
//class ActiveTask: public GmCommand
//{
//	virtual bool Validate();
//	virtual bool Trigger();
//	virtual bool Complete();
//private:
//	uint32_t m_activeIndex;
//};
//

#endif
