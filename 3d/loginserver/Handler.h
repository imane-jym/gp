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
#include "LoginGroup.h"

class CRelayClientSession;
class CPlayerSession;

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
		void OnKick(uint32_t type);

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

		/* =========================== */
	public:
		/// Timers for different object refresh rates
		enum E_USER_STATUS
		{
			E_STATUS_CONNECTED,			//连接到服务器
			E_STATUS_AUTHED,			//认证通过
		};

		enum E_LOGIN_RESULT
		{
			E_LOGIN_RESULT_OK	= 0,
			E_LOGIN_NO_PLAYER	= 1,
			E_LOGIN_PWD_ERROR	= 2
		};

		enum E_REGISTER_RESULT
		{
			E_REGISTER_RESULT_OK		= 0,
			E_REGISTER_RESULT_DEPLICATE	= 1,
			E_REGISTER_RESULT_INVALID	= 2
		};

		enum E_CHOOSE_SERVER_RESULT
		{
			E_CHOOSE_SERVER_RESULT_OK		 = 0,
			E_CHOOSE_SERVER_RESULT_NOT_FOUND = 1,
			E_CHOOSE_SERVER_RESULT_VERSION	 = 2,
			E_CHOOSE_SERVER_RESULT_FORBID	 = 3,
			E_CHOOSE_SERVER_RESULT_ERROR	 = 4,
			E_CHOOSE_SERVER_RESULT_NEW_ACCOUNT_FORBID	 = 5,
		};

		typedef void (CHandlerPlayersession::*OpcodeHandlerFunc)(WorldPacket &packet);
		typedef std::map<uint16, OpcodeHandlerFunc> OpcodeHandlerMap;

		E_USER_STATUS GetUserStatus() { return m_eUserStatus; }
		uint64 GetAccountId() { return m_dwAccountId; }
		void SetAccountId( uint64 dwAccountId ) { m_eUserStatus = E_STATUS_AUTHED; m_dwAccountId = dwAccountId; }

		void AddSessionToWorker();
		void Online();
		void Offline();
		void OnTimer5s();
		void OnTimer1min();
		void OnTimer3min();

		/////////////////////////////////////////
		void HandlerAccountLogin(WorldPacket& packet);
		void HandlerPlatformLogin(WorldPacket& packet);
		void HandlerFastLogin(WorldPacket& packet);

		void HandlerGetServerList(WorldPacket& packet);
		void HandlerChooseServer(WorldPacket& packet);

		void HandlerRegister(WorldPacket& packet);
		void HandlerModifyPwd(WorldPacket& packet);
		void HandlerExistPassport(WorldPacket& packet);
		std::string GetRemoteIP() { return m_strIp; }

	private:
		void InitOpcodeHandler();
		OpcodeHandlerFunc FindOpcodeHandlerFunc(uint16 opcode);

		void SendLoginResult( uint8 byResult );
		void SendRegisterResult( uint8 byResult );
		void SendModifyPwdResult( uint8 byResult );

		void GetServerListAfterFilter( CLoginGroup::ServerStatusContainer& oServers );
		bool ServerAccessable( const CenterDB::STC_SERVER_STATUS& server, uint16 wPlatform, uint8 byGmAuth  );
		bool CheckIp( std::string strIp, std::string strPattern );

	private:
		/* ====================  DATA MEMBERS  ======================================= */
		CPlayerSession *m_pPlayer;
		std::string m_strIp;
		
		/* ======================== */
		OpcodeHandlerMap m_mapOpcodeHandlerFuncs;
		E_USER_STATUS 	m_eUserStatus;
		uint64			m_dwAccountId;
		std::string		m_strClientVersion;
		std::string		m_strClientSvnVersion;

		std::string		m_strDevice;
		uint16			m_wPlatformId;
		uint32			m_dwRegTime;
		std::string		m_strUid;

		bool			m_bNoPacket;
		uint32			m_dwLastGetServerListTime;
}; /* -----  end of class CHandlerPlayersession  ----- */

void handlerOnConnect(bool isReconnect, int key = 0);

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
