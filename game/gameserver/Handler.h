/*
 * =====================================================================================
 *
 *       Filename:  Handler.h
 *
 *    Description:  logical operations module. Some configs in Set.h. The client must keep 
 * alive or server will close client connection. You can change some behavior by CSet class
 *
 *        Version:  1.0
 *        Created:  2013年09月22日 15时05分15秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  imane (), imanecr@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#ifndef _HANDLER_H_
#define _HANDLER_H_

#include <stdint.h>
#include <string>
#include "WorldPacket.h"

class CRelayClientSession;
class CPlayerSession;
class CRole;

enum
{
	MAIN_THREAD,                                /* main */
	WORLD_THREAD                                /* logical */
};

enum
{
	TIMER_1S = 1,
	TIMER_3S = 3,
	TIMER_5S = 5,
	TIMER_30S = 30,
	TIMER_1MIN = 60,
	TIMER_3MIN = 180,
	TIMER_5MIN = 300,
	TIMER_10MIN = 600,
	TIMER_30MIN = 1800,
};

/* 
 * ===  FUNCTION  ======================================================================
 *  Name:  HandlerInit
 *  Description:  world thread(logical thread) or main thread first call this function
 *  thread: 0 main thread 1 world thread
 *  return: true is success, or is failure
 * =====================================================================================
 */
bool HandlerInit(uint8_t thread);


/* 
 * ===  FUNCTION  ======================================================================
 *  Name:  HandlerFinally
 *  Description:  world thread(logical thread) or main final call this function
 *  thread: 0 main thread 1 world thread
 * =====================================================================================
 */
void HandlerFinally(uint8_t thread);


/* 
 * ===  FUNCTION  ======================================================================
 *  Name:  HandlerOntimer
 *  Description:  world thread(logical thread) timer funciton. Notice every timer is 
 *  independent. For example now time is 0. When time arrives 3, this function will be 
 *  called twice(1s and 3s). But para is 1 or 3.
 *  interval: 1 1s timer, 3 3s timer, 5 5s timer, 60 1min timer, 600 10min, 1800 30min
 *  There are total 6 kinds of timers
 * =====================================================================================
 */
void HandlerOntimer(uint32_t interval);


/*
 * =====================================================================================
 *        Class:  CHandlerPlayersession
 *  Description:  When every connect arrive, this class will be construct
 * =====================================================================================
 */
class CHandlerPlayersession
{
	public:
		/* ====================  LIFECYCLE     ======================================= */
		CHandlerPlayersession ();                             /* constructor */
		~CHandlerPlayersession ();
		void SetstrIp(std::string ip) { m_strIp = ip;};
		void SetCPlayerSession(CPlayerSession *p) {m_pPlayer = p;};
		CPlayerSession* GetPlayersession() { return m_pPlayer; };

		/* ====================  OPERATIONS     ======================================= */

		/* 
		 * ===  FUNCTION  ======================================================================
		 *         Name:  Process
		 *  Description:  When world thread get the packet, this function will be called 
		 *  packet: data pkg
		 * =====================================================================================
		 */
		void Process(WorldPacket &packet);

		/* 
		 * ===  FUNCTION  ======================================================================
		 *         Name:  UnInit
		 *  Description:  call this function before deconstruct this class
		 * =====================================================================================
		 */
		void UnInit();

		/* 
		 * ===  FUNCTION  ======================================================================
		 *         Name:  OnTimer
		 *  Description:  timer function, there are total 6 kinds timers. every timer is independent
		 *  interval: TIMER_1S - TIMER_60MIN
		 * =====================================================================================
		 */
		void OnTimer(uint32_t interval);

		/* ====================  METHODS       ======================================= */
		/* 
		 * ===  FUNCTION  ======================================================================
		 *         Name:  SendPacket
		 *  Description:  send  packet to client that connect this server
		 * =====================================================================================
		 */
		void SendPacket(WorldPacket *packet);
		
		/* 
		 * ===  FUNCTION  ======================================================================
		 *         Name:  SendPacketRelay
		 *  Description:  send packet to server that clientsession connect to
		 *  		key:  every back-end server have itself id key
		 * =====================================================================================
		 */
		void SendPacketToRelay(WorldPacket *packet, int key);
		
		//==========================================
		typedef enum _E_USER_STATUS
		{
			E_STATUS_CONNECTED,			//连接到服务器
			E_STATUS_WAITING_AUTH,		//等待认证
			E_STATUS_AUTHED,			//认证通过

		}E_USER_STATUS;

		typedef void (CHandlerPlayersession::*OpcodeHandlerFunc)(WorldPacket &packet);
		typedef std::map<uint16, OpcodeHandlerFunc> OpcodeHandlerMap;

		typedef void (CHandlerPlayersession::*OpRelayPacketFun)(WorldPacket& pkg);
		typedef std::map<uint16, OpRelayPacketFun> MapOpRelayPacket;

		uint32 GetAccountId() { return m_dwAccountId;}
		CRole * GetRole() { return m_pRole;}
		void SetRole(CRole* pRole);
		bool  RoleChgClientAndCloseSocket(CHandlerPlayersession* pSess);

		E_USER_STATUS GetUserStatus() { return m_eUserStatus;}

		std::string & GetUid() { return m_strUid;};
		void Online();
		void Offline();
		void OnTimer1s();
		void OnTimer5s();
		void OnTimer3min();

		void OpRelayPacket(uint16 wCmd, WorldPacket& pkg);

		//验证通过
		void UserAuthed(uint32 accountId, const std::string & strAccount);
		void CharacterCreate(uint64 account_id, uint32 &character_id, uint16 serverid);

		/////////////////////////////
		//
		void HandlerEcho(WorldPacket & packet);
		//CMD_CS_AUTH 客户端请求认证
		void HandlerClientAuth(WorldPacket & packet);
		void HandlerGM(WorldPacket &packet);
		//	void HandlerCardQuipmentChange(WorldPacket &packet);
		//	void HandlerChangeTeam(WorldPacket &packet);
		void HandlerItemQuipmentChange(WorldPacket &packet);
		void HandlerMissionInfo(WorldPacket &packet);
		void HandlerMissionAccept(WorldPacket &packet);
		void HandlerSearchPlayer(WorldPacket &pkg);
		void HandlerAutoPlayer(WorldPacket &pkg);
		void HandlerDuration(WorldPacket &pkg);
		void HandlerDurationPreview(WorldPacket &pkg);	
		void HandlerSearchPlayerByItem(WorldPacket &pkg);
		void HandlerRobPreview(WorldPacket &pkg);
		void HandlerRob(WorldPacket &pkg);
		void HandlerCardCombineRequest(WorldPacket &pkg);
		void HandlerCardCombineGet(WorldPacket &pkg);
		void HandlerCardStrength(WorldPacket &pkg);
		//	void HandlerCardEvolution(WorldPacket &pkg);
		void HandlerTechLevelUp(WorldPacket &pkg);
		void HandlerFriendAutoPlayerInFriend(WorldPacket &packet);
		void HandlerFriendPlayerTeam(WorldPacket &packet);
		void HandlerFriendAutoPlayerForFriend(WorldPacket &packet);
		void HandlerFriendAdd(WorldPacket &packet);
		void HandlerFriendSearchPlayer(WorldPacket &packet);
		void HandlerFriendDel(WorldPacket &packet);
		void HandlerFriendGetNumber(WorldPacket &packet);
		void HandlerMailDetailInfo(WorldPacket &packet);
		void HandlerMailHandle(WorldPacket &packet);
		void HandlerMailDel(WorldPacket &packet);
		void HandlerFriendMail(WorldPacket &packet);
		void HandlerArenaPlayerTeam(WorldPacket &packet);
		void HandlerArenaDuration(WorldPacket &packet);
		void HandlerArenaOpen(WorldPacket &packet);
		void HandlerArenaRequestPrestigeProduct(WorldPacket &packet);
		//	void HandlerArenaRequestRankProduct(WorldPacket &packet);
		void HandlerMissionBranchAccept(WorldPacket &packet);
		void HandlerGroupCreate(WorldPacket &packet);
		void HandlerGroupAutoInfo(WorldPacket &packet);
		void HandlerGroupSearch(WorldPacket &packet);
		void HandlerGroupLevelUp(WorldPacket &packet);
		void HandlerGroupMailInfo(WorldPacket &packet);
		void HandlerGroupRequestEnter(WorldPacket &packet);
		void HandlerGroupMailOpt(WorldPacket &packet);
		void HandlerGroupAcceptMission(WorldPacket &packet);
		void HandlerGroupRequestPrize(WorldPacket &packet);
		void HandlerGroupTechUp(WorldPacket &packet);
		void HandlerGroupMemberInfo(WorldPacket &packet);
		void HandlerGroupInfo(WorldPacket &packet);
		void HandlerGroupChat(WorldPacket &packet);
		void HandlerGroupAppoint(WorldPacket &packet);
		void HandlerGroupUnAppoint(WorldPacket &packet);
		void HandlerGroupRankSearch(WorldPacket &packet);
		void HandlerGroupMemberSearch(WorldPacket &packet);
		void HandlerGetGroupDetail(WorldPacket &packet);
		void HandlerGroupModifyInfo(WorldPacket &packet);
		void HandlerGroupEXit(WorldPacket &packet);
		void HandlerGroupDeleteMember(WorldPacket &packet);
		void HandlerGroupUnAppointMember(WorldPacket &packet);
		void HandlerGroupCancel(WorldPacket &packet);
		void HandlerShopBuy(WorldPacket &packet);
		void HandlerPlayerLose(WorldPacket &packet);
		void HandlerShopUse(WorldPacket &packet);
		//	void HandlerSkillFlush(WorldPacket &packet);
		void HandlerBuyDelCD(WorldPacket &packet);
		void HandlerCardLineUpChange(WorldPacket &packet);
		void HandlerMasterRequest(WorldPacket &packet);
		void HandlerMasterYesOrNo(WorldPacket &packet);
		void HandlerMasterSendRed(WorldPacket &packet);
		void HandlerMasterBless(WorldPacket &packet);
		void HandlerDurationMissionAccept(WorldPacket &packet);
		void HandlerNormalCardStrength(WorldPacket &packet);
		void HandlerSellCard(WorldPacket &packet);
		void HandlerRequestGiftPacket(WorldPacket &packet);

		void HandlerClientLogin(WorldPacket & packet);
		void HandlerClientFastLogin(WorldPacket & packet);
		void HandlerClientOtherLogin(WorldPacket & packet);
		void HandlerChangePwd(WorldPacket &packet);
		void HandlerRoleInitInfo(WorldPacket &packet);
		void HandlerCardChangeSingle(WorldPacket &packet);
		void HandlerSwitchCardExp(WorldPacket &packet);
		void HandlerGacha(WorldPacket &packet);
		void HandlerTenGacha(WorldPacket &packet);
		void HandlerFriendBossListRequset(WorldPacket &packet);
		void HandlerFriendBossHistoryRequest(WorldPacket &packet);
		void HandlerFriendBossDurationRequest(WorldPacket &packet);
		void HandlerFriendBossGacha(WorldPacket &packet);
		void HandlerDirtyWordCheck(WorldPacket &packet);
		void HandlerAgreeFriend(WorldPacket &packet);
		void HandlerFriendApply(WorldPacket &packet);
		void HandlerWorldBroadCast(WorldPacket &packet);
		void HandlerShopUseCancel(WorldPacket &packet);
		void HandlerRequestTimerGift(WorldPacket &packet);
		void HandlerLoginSignActive(WorldPacket &packet);
		void HandlerStoreRequest(WorldPacket &packet);
		void HandlerGameFlow(WorldPacket &packet);
		void HandlerHeartBeat(WorldPacket &packet);
		void HandlerTurntable(WorldPacket &packet);
		void HandlerStaticsData(WorldPacket &packet);
		void HandlerRecoverRequest(WorldPacket &packet);
		void HandlerAchieveRequest(WorldPacket &packet);
		void HandlerFirstPrize(WorldPacket &packet);
		void HandlerUpgrade(WorldPacket &packet);
		void HandlerUpgradeReview(WorldPacket &packet);
		void HandlerSkillStrengthReview(WorldPacket &packet);
		void HandlerCommonActiveOpt(WorldPacket &packet);
		void HandlerJoinPlayerRequest(WorldPacket &packet);
		void HandlerNoticeShareRequest(WorldPacket &packet);
		void HandlerTrial(WorldPacket &packet);
		void HandlerTrialNext(WorldPacket &packet);
		void HandlerRoleInfoUpdate(WorldPacket &packet);
		void HandlerCDKeyGet(WorldPacket &packet);
		void HandlerDurationMissionAllStar(WorldPacket &packet);
		void HandlerActivePrizeGet(WorldPacket &packet);

		//Tower combat
		void HandlerTowerCombat(WorldPacket &packet);

		///////////////HandlerRelay///////////////////////////////////////////////


		//////////////////////////////////////////////////////////////////////////
		void SendAuthInfo(uint16 errno);
		void SendChangePwdResult(uint16 wErrno);

//		uint32 GetdwActiveTime(){return m_dwActiveTime;}

		std::string GetRegIP() { return m_strregip;}
		std::string GetRegDevice() { return m_strregdevice;}
		std::string GetRegDeviceType() { return m_strregdevicetype;}
		std::string GetRegDeviceUid() { return m_strregdeviceuid;}

		std::string GetLoginDevice() { return m_strlogindevice;}
		std::string GetLoginDeviceType() { return m_strlogindevicetype;}
		std::string GetLoginSecondChannel() { return m_strloginsecondchannel;}
		std::string GetLoginDeviceUid() { return m_strlogindeviceuid;}

		uint16 GetwChannel() { return m_wChannel;}

		void InitOpcodeHandler();
		OpcodeHandlerFunc FindOpcodeHandlerFunc(uint16 opcode);

		void InitOpRelayPacket();

		std::string GetRemoteIP();
		//==========================================
		
	private:
		/* ====================  DATA MEMBERS  ======================================= */
		CPlayerSession *m_pPlayer;
		std::string m_strIp;

		//==========================================
		OpcodeHandlerMap m_mapOpcodeHandlerFuncs;
		MapOpRelayPacket m_mapOpRelayPacketFun;
		E_USER_STATUS m_eUserStatus;
		uint32 m_dwAccountId;
		std::string m_strAccount;
		//玩家指针
		CRole * m_pRole;
		std::string m_strUid;
//		IntervalTimer m_timers[USER_UPDATE_COUNT];

//		bool m_bNeedOffline;
//		uint32 m_dwActiveTime;
		uint32 m_dwBeatTime;

		std::string m_strregip;
		std::string m_strregdevice;
		std::string m_strregdevicetype;
		std::string m_strregdeviceuid;

		std::string m_strlogindevice;
		std::string m_strlogindevicetype;
		std::string m_strloginsecondchannel;
		std::string m_strlogindeviceuid;

		uint16 m_wChannel;
		//==========================================
		
}; /* -----  end of class CHandlerPlayersession  ----- */


/*
 * =====================================================================================
 *        Class:  CHandlerClientSession
 *  Description:  Construct this class to connect other server and to communicate
 * =====================================================================================
 */
class CHandlerClientSession
{
	public:
		/* ====================  LIFECYCLE     ======================================= */
		CHandlerClientSession ();                             /* constructor */
		~CHandlerClientSession ();
		void SetRelay(CRelayClientSession *p) { m_pRelay = p; };

		/* ====================  OPERATIONS    ======================================= */
				
		/* 
		 * ===  FUNCTION  ======================================================================
		 *         Name:  Process
		 *  Description:  When world thread get the packet, this function will be called 
		 *  packet: data pkg
		 * =====================================================================================
		 */
		void Process(WorldPacket &packet);

		/* 
		 * ===  FUNCTION  ======================================================================
		 *         Name:  UnInit
		 *  Description:  call this function before deconstruct this class
		 * =====================================================================================
		 */
		void UnInit();
		
		/* 
		 * ===  FUNCTION  ======================================================================
		 *         Name:  OnTimer
		 *  Description:  timer function, there are total 6 kinds timers. every timer is independent
		 *  interval: TIMER_1S - TIMER_60MIN
		 * =====================================================================================
		 */
		void OnTimer(uint32_t interval);
		
		/* 
		 * ===  FUNCTION  ======================================================================
		 *         Name:  SendPacket
		 *  Description:  send packet to server that this clientsession connect to
		 * =====================================================================================
		 */
		void SendPacket(WorldPacket *packet);

	protected:
		/* ====================  METHODS       ======================================= */

		/* ====================  DATA MEMBERS  ======================================= */

	private:
		/* ====================  METHODS       ======================================= */

		/* ====================  DATA MEMBERS  ======================================= */
		CRelayClientSession *m_pRelay;

}; /* -----  end of class CHandlerClientSession  ----- */

#endif
