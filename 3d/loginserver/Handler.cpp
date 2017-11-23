/*
 * =====================================================================================
 *
 *       Filename:  Handler.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2013年09月22日 16时53分45秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  imane (), imanecr@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include "Handler.h"
#include "SessCtrl.h"
#include "PlayerSession.h"
#include "RelayClientSession.h"
#include "miniini/miniini.h"

#include <string>
#include "Set.h"

#include "Log.h"
#include "Util.h"
#include <iostream>
#include <vector>
#include <string>
#include "DatabaseMysql.h"
#include "LoginGroup.h"
#include "GmCommandExt.h"
#include "CmdDefine.h"

extern INIFile g_serverIni;

uint8 	g_serverId;
DatabaseMysql loginDatabase;        ///< Accessor to the login database

bool StartDB()
{
	std::string dbstring;
	INISection * database = g_serverIni.GetSection("other");
	if (!database)
	{
		IME_ERROR("Miss section [other] in server.ini");
		return false;
	}

	if (!database->ReadString("logindatabase",dbstring))
	{
		IME_ERROR("Missing login database info");
		return false;
	}
    ///- Initialise the login database
	if( !loginDatabase.Initialize(dbstring.c_str()) )
	{
		IME_ERROR("Cannot connect to login database %s",dbstring.c_str());
		return false;
	}

	return true;
}
bool MainThreadInitServer()
{
	int server_id;
	INISection * server = g_serverIni.GetSection("other");
	if (!server)
	{
		IME_ERROR("Miss section [other] in server.ini");
		return false;
	}

	if (!server->ReadInt("server_id", server_id))
	{
		IME_ERROR("Missing server id info");
		return false;
	}

	g_serverId = server_id;

//	INISection* config = g_serverIni.GetSection( "config" );
//	if ( config )
//	{
//		config->ReadString( "update_addr", g_updateAddr );
//	}
//	else
//	{
//		g_updateAddr = "";
//	}

	return true;
}

bool HandlerInit(uint8_t thread)
{
	//to do implementation
//	IME_LOG("handler init %u", thread);
	if (thread == MAIN_THREAD)
	{
		if (!MainThreadInitServer())
		{
			IME_ERROR("InitServer Failed");
			return 0;
		}
		//do many init db here
		if (!StartDB())
		{
			IME_ERROR("StartDB Failed");
			return 0;
		}
	}
	else
	{
		GmCommandFactory::RegisterFunc(Create, CheckTargetType);
		UNUSED_RETURN(sLoginGroup->InitSettings());
	}
	return true;
}

void HandlerFinally(uint8_t thread)
{
	//to do implementation
//	IME_LOG("handler finally %u", thread);
	if (thread == MAIN_THREAD)
	{
	}
}

void HandlerOntimer(uint32_t interval)
{
	//to do implementation
//	IME_LOG("handler Ontimer %u", interval);
	if (interval == TIMER_1S)
	{
		sLoginGroup->OnTimer1s();
	}
	else if (interval == TIMER_5S)
	{
		sLoginGroup->OnTimer5s();
	}
	else if (interval == TIMER_15S)
	{
		sLoginGroup->OnTimer15s();
	}
	else if (interval == TIMER_5MIN)
	{
		loginDatabase.Ping();
    	sLoginGroup->OnCheckClearGmAuth();
	}
	
}

CHandlerPlayersession::CHandlerPlayersession()
{
	m_pPlayer = NULL;
	m_strIp = "";

	//to do implementation
	m_eUserStatus = E_STATUS_CONNECTED;
	m_bNoPacket = true,
 	m_dwLastGetServerListTime = 0;

	InitOpcodeHandler();
}

CHandlerPlayersession::~CHandlerPlayersession()
{

}

void CHandlerPlayersession::OnKick(uint32_t type)
{
}

void CHandlerPlayersession::Process(WorldPacket &packet)
{
	//to do implementation
//	IME_LOG("playersession get packet size %u", packet.size());
	
	m_bNoPacket = false;

	uint16 opcode = packet.GetOpcode();
	if ( GetUserStatus() != E_STATUS_AUTHED )
	{
		bool valid = true;
		//如果还没有验证通过 则只有少数几个协议处理
		if ( opcode == CMD_LOGIN_CS_GET_SERVER
				|| opcode == CMD_LOGIN_CS_CHOOSE_SERVER
				|| opcode == CMD_LOGIN_CS_MODIFY_PWD )
		{
			valid = false;
			return;
		}

		if ( !valid )
		{
			IME_SYSTEM_ERROR( "Login", "Invalid Operation Before Authed" );
			return;
		}
	}

	OpcodeHandlerFunc pHandlerFunc = FindOpcodeHandlerFunc(opcode);
	if (pHandlerFunc)
	{
		try
		{
			(this->*pHandlerFunc)(packet);
		}
		catch ( const std::exception& e )
		{
			IME_USER_ERROR( "HandleCommand", (uint32)GetAccountId(), "%s, opcode=%d, accountid=%lu", e.what(), opcode, GetAccountId() );
		}
	}
	else
	{
		IME_SYSTEM_ERROR( "Login", "Unknown Opcode, id=%d", opcode );
	}
}

void CHandlerPlayersession::UnInit()
{
	//to do implementation
//	IME_LOG("playersession uninit");
	Offline();
}

void CHandlerPlayersession::OnTimer(uint32_t interval)
{
	//to do implementation
//	IME_LOG("playersession ontimer %u", interval);
	if (interval == TIMER_5S)
	{
		OnTimer5s();
	}
	else if (interval == TIMER_1MIN)
	{
		OnTimer1min();
	}
	else if (interval == TIMER_3MIN)
	{
		OnTimer3min();
	}
}

void CHandlerPlayersession::SendPacketToRelay(WorldPacket *packet, int key)
{
	sSessCtrl->SendToRelay(*packet, key);
}

void CHandlerPlayersession::SendPacket(WorldPacket *packet)
{
	m_pPlayer->SendPacket(packet);
}

void CHandlerPlayersession::AddSessionToWorker()
{
//	sLoginGroup->AddSession(this);
}
void CHandlerPlayersession::Online()
{

}
void CHandlerPlayersession::Offline()
{
//	// 清除用户信息
//	sLoginGroup->ClearUserInfo(this);
}
void CHandlerPlayersession::OnTimer5s()
{

}

void CHandlerPlayersession::OnTimer1min()
{
//	if ( m_bNoPacket )
//	{
//		m_ptrNetClient->CloseSocketAsync(CNetClient::E_CLOSE_ASYNC_FLAG_DIRECT_CLOSE);
//		//m_ptrNetClient->CloseSocket();
//	}
//	m_bNoPacket = true;
}

void CHandlerPlayersession::OnTimer3min()
{

}

CHandlerPlayersession::OpcodeHandlerFunc CHandlerPlayersession::FindOpcodeHandlerFunc(uint16 opcode)
{
	OpcodeHandlerMap::iterator itr = m_mapOpcodeHandlerFuncs.find(opcode);
	if (itr != m_mapOpcodeHandlerFuncs.end())
	{
		return itr->second;
	}
	return NULL;
}

void CHandlerPlayersession::InitOpcodeHandler()
{
	m_mapOpcodeHandlerFuncs.clear();
	
	/////////////////////////////////////
	m_mapOpcodeHandlerFuncs[CMD_LOGIN_CS_ACCOUNT_LOGIN] 	= &CHandlerPlayersession::HandlerAccountLogin;
	m_mapOpcodeHandlerFuncs[CMD_LOGIN_CS_PLATFORM_LOGIN]	= &CHandlerPlayersession::HandlerPlatformLogin;
	m_mapOpcodeHandlerFuncs[CMD_LOGIN_CS_FAST_LOGIN]		= &CHandlerPlayersession::HandlerFastLogin;
	m_mapOpcodeHandlerFuncs[CMD_LOGIN_CS_GET_SERVER]		= &CHandlerPlayersession::HandlerGetServerList;
	m_mapOpcodeHandlerFuncs[CMD_LOGIN_CS_CHOOSE_SERVER]		= &CHandlerPlayersession::HandlerChooseServer;
	m_mapOpcodeHandlerFuncs[CMD_LOGIN_CS_REGISTER]			= &CHandlerPlayersession::HandlerRegister;
	m_mapOpcodeHandlerFuncs[CMD_LOGIN_CS_MODIFY_PWD]		= &CHandlerPlayersession::HandlerModifyPwd;
	m_mapOpcodeHandlerFuncs[CMD_LOGIN_CS_PASSPORT_EXIST]	= &CHandlerPlayersession::HandlerExistPassport;
}

void CHandlerPlayersession::SendLoginResult( uint8 byResult )
{
	IME_SYSTEM_LOG( "Login", "result=%d", (int)byResult );

	WorldPacket pck( CMD_LOGIN_SC_LOGIN_RESULT );
	pck << byResult;
	SendPacket( &pck );
}

void CHandlerPlayersession::SendRegisterResult( uint8 byResult )
{
	IME_SYSTEM_LOG( "Register", "result=%d", (int)byResult );
	WorldPacket pck( CMD_LOGIN_SC_REGISTER_RESULT );
	pck << byResult;
	SendPacket( &pck );
}

void CHandlerPlayersession::SendModifyPwdResult( uint8 byResult )
{
	IME_SYSTEM_LOG( "ModifyPwd", "result=%d", (int)byResult );
	WorldPacket pck( CMD_LOGIN_SC_MODIFY_PWD_RESULT );
	pck << byResult;
	SendPacket( &pck );
}

void CHandlerPlayersession::GetServerListAfterFilter( CLoginGroup::ServerStatusContainer& oServers )
{
	oServers.clear();
	const CLoginGroup::ServerStatusContainer& servers = sLoginGroup->GetAllServerStatus();

	uint16 	wPlatform;
	uint8 	byGmAuth;
	std::string strPlatformId;
	if ( !CenterDBCtrl::GetPassportInfo( m_dwAccountId, wPlatform, strPlatformId, byGmAuth ) )
	{
		IME_SYSTEM_ERROR( "LoginStrategy", "Error When Get Passport Info" );
		return;
	}
	IME_ERROR("m_strClientVersion:%s, svnVersion:%s, device:%s", m_strClientVersion.c_str(), m_strClientSvnVersion.c_str(), m_strDevice.c_str());

	for ( CLoginGroup::ServerStatusContainer::const_iterator it = servers.begin();
			it != servers.end(); ++it )
	{
		if ( m_strClientSvnVersion == "2.0.0.7423" && it->second.dwServerId == 252 )
		{
			oServers.clear();
			oServers.insert( std::make_pair( it->second.dwServerId, it->second ) );
			break;
		}

		if ( m_strClientSvnVersion == "1.7.1.8972" && it->second.dwServerId == 243 )
		{
			oServers.clear();
			oServers.insert( std::make_pair( it->second.dwServerId, it->second ) );
			break;
		}

		if ( it->second.dwLoginStrategy == 0 || ServerAccessable( it->second, wPlatform, byGmAuth ) )
		{
			
			oServers.insert( std::make_pair( it->second.dwServerId, it->second ) );
		}
	}
}

bool CHandlerPlayersession::ServerAccessable( const CenterDB::STC_SERVER_STATUS& server, uint16 wPlatform, uint8 byGmAuth )
{
	if ( server.dwLoginStrategy == 0 ) return true;

	std::vector<std::string> vecTime;
	struct tm stcTm;

	STC_LOGIN_STRATEGY strategy;
	if ( !CenterDBCtrl::GetLoginStrategy( server.dwLoginStrategy, strategy ) )
	{
		IME_SYSTEM_ERROR( "Login_Strategy", "Error When Get Strategy, id=%u", server.dwLoginStrategy );
		return false;
	}

	bool ret = true;
	for (unsigned int i = 0; i < strategy.vvConditions.size(); i++ )
	{
		std::vector<STC_LOGIN_STRATEGY_CONDITION>& group = strategy.vvConditions[i];

		bool flag = false;
		for ( std::vector<STC_LOGIN_STRATEGY_CONDITION>::iterator it = group.begin();
				it != group.end(); ++it )
		{
			switch ( it->byType )
			{
			case E_LOGIN_STRATEGY_TYPE_PLATFORM:
				flag = ( wPlatform == atoi( it->strValue.c_str() ) );
				break;

			case E_LOGIN_STRATEGY_TYPE_IP:
				flag = CheckIp( GetRemoteIP(), it->strValue );
				break;

			case E_LOGIN_STRATEGY_TYPE_AUTH:
				flag = ( byGmAuth >= atoi( it->strValue.c_str() ) );
				break;

			case E_LOGIN_STRATEGY_TYPE_VERSION:
			//	flag = ( m_strClientVersion == "" || m_strClientVersion == it->strValue );
				flag = ( m_strClientSvnVersion == "" || m_strClientSvnVersion == it->strValue );//svnVersion才是客户端的版本号 m_strClientVersion是客户端的服务器版本号
				break;

			case E_LOGIN_STRATEGY_TYPE_DEVICE:
				flag = ( m_strDevice.size() > 0 && it->strValue.size() > 0 && tolower(m_strDevice[0]) == tolower(it->strValue[0]) );
				break;

			case E_LOGIN_STRATEGY_TYPE_REG_TIME:

				CUtil::StrSplit( it->strValue, "-", vecTime );
				if ( vecTime.size() < 3 ) break;

				stcTm.tm_year 	= atoi( vecTime[0].c_str() ) - 1900;
				stcTm.tm_mon	= atoi( vecTime[1].c_str() ) - 1;
				stcTm.tm_mday	= atoi( vecTime[2].c_str() );
				stcTm.tm_hour 	= ( vecTime.size() >= 4 ? atoi( vecTime[3].c_str() ) : 0 );
				stcTm.tm_min 	= ( vecTime.size() >= 5 ? atoi( vecTime[4].c_str() ) : 0 );
				stcTm.tm_sec 	= ( vecTime.size() >= 6 ? atoi( vecTime[5].c_str() ) : 0 );

				flag = ( m_dwRegTime <= mktime( &stcTm ) );
				break;

			// svn 白名单
			case E_LOGIN_STRATEGY_TYPE_SVN_VERSION:
//				flag = ( m_strClientSvnVersion == "" || m_strClientSvnVersion == it->strValue );
				flag = ( m_strClientSvnVersion == it->strValue );
				break;

			// svn 黑名单
			case E_LOGIN_STRATEGY_TYPE_SVN_VERSION_BLACK_LIST:
				flag = ( m_strClientSvnVersion != it->strValue );
				break;

			case E_LOGIN_STRATEGY_TYPE_DEVICE_NUMBER:
				{
					std::vector<std::string> vecGroup;
					CUtil::StrSplit(it->strValue, std::string(","), vecGroup);
					std::vector<std::string>::iterator itGet;
					itGet = std::find(vecGroup.begin(), vecGroup.end(), m_strUid);
					flag = (itGet != vecGroup.end());
				}

				break;
			default:
				IME_SYSTEM_ERROR( "LoginStrategy", "Invalid Type, type=%u", it->byType );
				break;
			}
			if ( flag ) break;
		}

		if ( !flag )
		{
			ret = false;
			break;
		}
	}

	return ret;
}

bool CHandlerPlayersession::CheckIp( std::string strIp, std::string strPattern )
{
	std::vector<std::string> vTarget, vPattern;
	CUtil::StrSplit( strIp, ".", vTarget );
	if ( vTarget.size() != 4 )
	{
		IME_SYSTEM_ERROR( "LoginStrategy", "Bad Ip Format, ip=%s", strIp.c_str() );
		return false;
	}
	CUtil::StrSplit( strPattern, ".", vPattern );
	if ( vPattern.size() != 4 )
	{
		IME_SYSTEM_ERROR( "LoginStrategy", "Bad Ip Pattern, ip=%s", strPattern.c_str() );
		return false;
	}

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
			if ( vRange.size() != 2 )
			{
				IME_SYSTEM_ERROR( "LoginStrategy", "Bad Ip Pattern, ip=%s", strPattern.c_str() );
				return false;
			}
			int a = atoi( vTarget[i].c_str() ), low = atoi( vRange[0].c_str() ), high = atoi( vRange[1].c_str() );
			if ( low > high ) std::swap( low, high );
			if ( a < low || a > high )
			{
				return false;
			}
		}
		else
		{
			if ( atoi( vTarget[i].c_str() ) != atoi( vPattern[i].c_str() ) )
			{
				return false;
			}
		}
	}
	return true;
}

void CHandlerPlayersession::HandlerAccountLogin(WorldPacket& packet)
{
	std::string strAccount, strPwd;
	uint16 	wPlatform;
	packet >> strAccount >> strPwd >> wPlatform;

	IME_LOG( "HandlerAccountLogin, ip=%s", GetRemoteIP().c_str() );

	uint64 	dwAccountId = 0;
	CenterDB::LoginResultType result = CenterDBCtrl::ValidateAuthAccount( strAccount, strPwd, wPlatform, dwAccountId );

	switch ( result )
	{
	case CenterDB::E_LOGIN_RESULT_TYPE_OK:
		SetAccountId( dwAccountId );
		m_strDevice = CenterDBCtrl::GetDevice( dwAccountId );
		m_wPlatformId = wPlatform;
		m_dwRegTime = CenterDBCtrl::GetPassportRegTime( dwAccountId );
		SendLoginResult( result );
		break;

	case CenterDB::E_LOGIN_RESULT_TYPE_NOT_FOUND:
		SendLoginResult( result );
		break;

	case CenterDB::E_LOGIN_RESULT_TYPE_WRONG_PWD:
		SendLoginResult( result );
		break;

	case CenterDB::E_LOGIN_RESULT_ERROR:
		IME_SYSTEM_ERROR( "Login", "account=%s, pwd=%s", strAccount.c_str(), strPwd.c_str() );
		SendLoginResult( result );
		break;
	}
}

void CHandlerPlayersession::HandlerPlatformLogin(WorldPacket& packet)
{
	std::string strUid;
	std::string strToken;
	std::string strDeviceToken = "";
	uint16 wPlatform;
	std::string strDevice;
	std::string strDeviceType;

	packet >> strUid >> strToken >> wPlatform >> strDevice >> strDeviceType;

	std::string strOpenUdid, strAppleUid;

	if ( packet.rpos() < packet.size() )
	{
		packet >> strOpenUdid >> strAppleUid;
	}

	// For then same Facebook account, iOS version has suffix 'fb' while Android version doesn't.
	if ( wPlatform == 900 /* chukong */ && strDevice == "ios" )
	{
		size_t pos = strToken.rfind( "fb" );
		if ( pos != std::string::npos && pos == strToken.length() - 2 )
		{
			strToken = strToken.substr( 0, strToken.length() - 2 );
		}
	}

	IME_LOG( "HandlerPlatformLogin, platform=%u, device=%s, ip=%s", wPlatform, strDevice.c_str(), GetRemoteIP().c_str() );

	uint64 dwAccountId;
	CenterDB::LoginResultType result = CenterDBCtrl::ValidateAuthPlatform( strToken, strUid, strDeviceToken, wPlatform,
			GetRemoteIP(), strDevice, strDeviceType, strOpenUdid, strAppleUid, dwAccountId );

	switch ( result )
	{
	case CenterDB::E_LOGIN_RESULT_TYPE_OK:
		SetAccountId( dwAccountId );
		m_strDevice = strDevice;
		m_wPlatformId = wPlatform;
		m_dwRegTime = CenterDBCtrl::GetPassportRegTime( dwAccountId );
		m_strUid = strUid;
		SendLoginResult( result );
		break;
//	case CenterDB::E_LOGIN_RESULT_ERROR:
	default:
		IME_SYSTEM_ERROR( "Login", "token=%s, uid=%s", strToken.c_str(), strUid.c_str() );
		SendLoginResult( result );
		break;
	}
}

void CHandlerPlayersession::HandlerFastLogin(WorldPacket& packet)
{
	std::string	strUid;
	std::string strDeviceToken = "";
	uint16		wPlatform;
	std::string strDevice;
	std::string strDeviceType;

	std::string strOpenUdid 	= "";
	std::string strAppleUdid	= "";
	uint8		byIosVersion	= E_IOS_VERSION_5;

	packet >> strUid >> wPlatform >> strDevice >> strDeviceType;

	if ( packet.rpos() < packet.size() )
	{
		packet >> strOpenUdid >> strAppleUdid >> byIosVersion;
	}

	IME_LOG( "HandlerFastLogin, platform=%u, device=%s, ip=%s", wPlatform, strDevice.c_str(), GetRemoteIP().c_str() );

	if ( wPlatform == 111 || wPlatform == 115 )
	{
		IME_ERROR( "HandlerFastLogin, Platform 111 and 115 can not fast login!, platform=%u", wPlatform );
		return;
	}

	uint64 dwAccountId = 0;
	CenterDB::LoginResultType result = CenterDBCtrl::ValidateAuthFast( strUid, strOpenUdid, strAppleUdid, byIosVersion,
			strDeviceToken, wPlatform, GetRemoteIP(), strDevice, strDeviceType, dwAccountId );

	switch ( result )
	{
	case CenterDB::E_LOGIN_RESULT_TYPE_OK:
		SetAccountId( dwAccountId );
		m_strDevice = strDevice;
		m_wPlatformId = wPlatform;
		m_dwRegTime = CenterDBCtrl::GetPassportRegTime( dwAccountId );
		SendLoginResult( result );
		break;
//	case CenterDB::E_LOGIN_RESULT_ERROR:
	default:
		IME_SYSTEM_ERROR( "Login", "uid=%s", strUid.c_str() );
		SendLoginResult( result );
		break;
	}
}

void CHandlerPlayersession::HandlerGetServerList(WorldPacket& packet)
{
	if ( time(NULL) < m_dwLastGetServerListTime + 3 )
	{
		IME_ERROR( "HandlerGetServerList, deny, ip=%s", GetRemoteIP().c_str() );
		return;
	}

	if ( packet.rpos() < packet.size() )
	{
		packet >> m_strClientVersion;
	}
	else
	{
		m_strClientVersion = "";
	}

	if ( packet.rpos() < packet.size() )
	{
		packet >> m_strClientSvnVersion;
	}
	else
	{
		m_strClientSvnVersion = "";
	}
	uint8 isBigServerSize = false;
	if (packet.rpos() < packet.size())
		packet >> isBigServerSize;

	WorldPacket pck( CMD_LOGIN_SC_SERVER_LIST );
	WorldPacket pckNew( CMD_LOGIN_SC_SERVER_LIST_NEW );

	CLoginGroup::ServerStatusContainer servers;

	GetServerListAfterFilter( servers );
	uint32 lastServerId = 0;

	if (!isBigServerSize)
	{
		pck << (uint8)servers.size();
		pckNew << (uint8)servers.size();
	}
	else
	{
		pck << (uint16)servers.size();
		pckNew << (uint16)servers.size();
	}
	uint8 server_status = (uint8)E_SERVER_STATUS_MAINTAIN;
	for ( CLoginGroup::ServerStatusContainer::reverse_iterator it = servers.rbegin();
			it != servers.rend(); ++it )
	{
		server_status = it->second.byStatus;
		// 处理内部可见的状态
		if (server_status == (uint8)E_SERVER_STATUS_INTERNAL)
			server_status = (m_strIp == "180.168.54.62" ? (uint8)E_SERVER_STATUS_NORMAL : (uint8)E_SERVER_STATUS_MAINTAIN);

		pck << (uint16)it->second.dwServerId;
		pck << it->second.strServerName;
		//pck << (bool)( it->second.bIsAlive );//&& it->second.byCanLogin == 1 );
		pck << true;
		pck << (it->second.bIsAlive && it->second.byCanLogin ? server_status : (uint8)E_SERVER_STATUS_MAINTAIN);

		pckNew << (uint16)it->second.dwServerId;
		pckNew << it->second.dwServerVersionCode;
		pckNew << it->second.strServerName;
		pckNew << true;
		pckNew << (it->second.bIsAlive && it->second.byCanLogin ? server_status : (uint8)E_SERVER_STATUS_MAINTAIN);
		if (lastServerId == 0)
			lastServerId = it->second.dwServerId;
		
	}
	uint32 dwStartTime = 0;
	uint32 dwEndTime = 0;
	uint32 dwNow = time(NULL);
	std::string strLoginNotice;

	CenterDBCtrl::GetNoticeTimeRange( E_NOTICE_USE_TYPE_LOGIN_TIME_RANGE, E_NOTICE_CONDITION_TYPE_PLATFORM, m_wPlatformId, dwStartTime, dwEndTime );

	// 在定时公告时间范围内，显示定时公告
	if(dwNow >= dwStartTime && dwNow <= dwEndTime)
	{
		strLoginNotice = CenterDBCtrl::GetNotice( E_NOTICE_USE_TYPE_LOGIN_TIME_RANGE, E_NOTICE_CONDITION_TYPE_PLATFORM, m_wPlatformId );
	}
	// 否则显示默认公告
	else
	{
		strLoginNotice = CenterDBCtrl::GetNotice( E_NOTICE_USE_TYPE_LOGIN, E_NOTICE_CONDITION_TYPE_PLATFORM, m_wPlatformId );
	}

	pck << strLoginNotice;
	uint16 serverid = CenterDBCtrl::GetLastLoginServer( m_dwAccountId );
	if (serverid == 0)
		serverid = lastServerId;
	pck << serverid;

	pckNew << strLoginNotice;
	pckNew << CenterDBCtrl::GetLastLoginServer( m_dwAccountId );

	SendPacket( &pck );
//	SendPacket( &pckNew );

	m_dwLastGetServerListTime = time(NULL);
}

void CHandlerPlayersession::HandlerChooseServer(WorldPacket& packet)
{
	uint16 	wServerId;
	std::string	strClientVersion;
	packet >> wServerId >> strClientVersion;

	uint16	wServerIdMerged = CenterDBCtrl::GetServerIdMerged( wServerId );

	WorldPacket pck( CMD_LOGIN_SC_CHOOSE_RESULT );

	const CLoginGroup::ServerStatusContainer& servers = sLoginGroup->GetAllServerStatus();
	CLoginGroup::ServerStatusContainer::const_iterator it = servers.find( wServerIdMerged );

	do
	{
		if ( it == servers.end() || !it->second.byCanLogin || !it->second.bIsAlive )
		{
			pck << (uint8)E_CHOOSE_SERVER_RESULT_NOT_FOUND;
			break;
		}
		IME_LOG("clientVersion:%s, serverVersion:%s", m_strClientSvnVersion.c_str(), it->second.strVersion.c_str());

		std::vector<uint32> clientVersionVec;
		std::vector<uint32> serverVersionVec;
		CUtil::StrSplit(m_strClientSvnVersion, ".", clientVersionVec);
		CUtil::StrSplit(it->second.strVersion, ".", serverVersionVec);
		if (clientVersionVec.size() > 0)
		{
			if (clientVersionVec.size() < 3 || serverVersionVec.size() < 3)
			{
				pck << (uint8)E_CHOOSE_SERVER_RESULT_VERSION << it->second.strVersion
				<< CenterDBCtrl::GetNotice( E_NOTICE_USE_TYPE_UPDATE_ADDR, E_NOTICE_CONDITION_TYPE_PLATFORM, m_wPlatformId );
				break;
			}
			if (clientVersionVec[0] != serverVersionVec[0] || 
					clientVersionVec[1] != serverVersionVec[1] || 
					serverVersionVec[2] > clientVersionVec[2])
			{
				pck << (uint8)E_CHOOSE_SERVER_RESULT_VERSION << it->second.strVersion
					<< CenterDBCtrl::GetNotice( E_NOTICE_USE_TYPE_UPDATE_ADDR, E_NOTICE_CONDITION_TYPE_PLATFORM, m_wPlatformId );
				break;
			}
		}
		uint32 dwRoleId = 0;
		int ret  = CenterDBCtrl::GetOrInsertRoleId( GetAccountId(), wServerId, dwRoleId, it->second.dwCanRegister);
		if ( ret == 0 )
		{
			if ( CenterDBCtrl::IsRoleForbid( dwRoleId) )
			{
				pck << (uint8)E_CHOOSE_SERVER_RESULT_FORBID;
			}
			else
			{
				pck << (uint8)E_CHOOSE_SERVER_RESULT_OK << it->second.strIp << it->second.dwPort << dwRoleId;
				pck << it->second.strResServerAddr << it->second.strResVersionFull;

				CenterDBCtrl::SetLastLoginServer( m_dwAccountId, wServerId );
				CenterDBCtrl::CreateLoginToken( GetAccountId(), GetRemoteIP() );
			}
			break;
		}
		else if ( ret == 1)
		{
			pck << (uint8)E_CHOOSE_SERVER_RESULT_NEW_ACCOUNT_FORBID;
		}
		else
		{
			pck << (uint8)E_CHOOSE_SERVER_RESULT_ERROR;
		}

	} while (0);
	SendPacket( &pck );
}

void CHandlerPlayersession::HandlerRegister(WorldPacket& packet)
{
	std::string strUid;
	std::string	strAccount, strPwd, strMail;
	std::string strToken = "";
	uint16		wPlarform;
	std::string strDevice;
	std::string strDeviceType;
	packet >> strUid >> strAccount >> strPwd >> strMail;
	packet >> wPlarform >> strDevice >> strDeviceType;

	std::string strOpenUdid, strAppleUdid;

	if ( packet.rpos() < packet.size() )
	{
		packet >> strOpenUdid >> strAppleUdid;
	}

	if ( strAccount.size() == 0 || strPwd.size() == 0 || strMail.size() == 0 )
	{
		SendRegisterResult( E_REGISTER_RESULT_INVALID );
		return;
	}

	if ( CenterDBCtrl::RegisterPassport( strAccount, strPwd, strMail, strUid, strToken,
			wPlarform, GetRemoteIP(), strDevice, strDeviceType, strOpenUdid, strAppleUdid ) )
	{
		SendRegisterResult( E_REGISTER_RESULT_OK );
	}
	else
	{
		SendRegisterResult( E_REGISTER_RESULT_DEPLICATE );
	}
}

void CHandlerPlayersession::HandlerModifyPwd(WorldPacket& packet)
{
	std::string strNewPwd;
	packet >> strNewPwd;

	if ( GetUserStatus() != E_STATUS_AUTHED )
	{
		SendModifyPwdResult( 1 );
		return;
	}
	if ( CenterDBCtrl::ModifyPassword( GetAccountId(), strNewPwd ) )
	{
		SendModifyPwdResult( 0 );
	}
	else
	{
		SendModifyPwdResult( 2 );
	}
}

void CHandlerPlayersession::HandlerExistPassport(WorldPacket& packet)
{
	std::string strPassport;
	uint16 wPlatform;
	uint8 byAuthType;
	packet >> strPassport >> byAuthType >> wPlatform;

	WorldPacket pck( CMD_LOGIN_SC_PASSPORT_EXIST );
	pck << (uint8)CenterDBCtrl::ExistsPassport( strPassport, byAuthType, wPlatform );
	SendPacket( &pck );
}

void handlerOnConnect(bool isReconnect, int key)
{
}

CHandlerClientSession::CHandlerClientSession()
{
	m_pRelay = NULL;

	//to do implementation
}
CHandlerClientSession::~CHandlerClientSession()
{

}

void CHandlerClientSession::Process(WorldPacket &packet)
{
	//to do implementation
//	IME_LOG("clientsession get packet size %u", packet.size());
}

void CHandlerClientSession::UnInit()
{
	//to do implementation
//	IME_LOG("clientsession uninit");
}

void CHandlerClientSession::OnTimer(uint32_t interval)
{
	//to do implementation
//	IME_LOG("clientsession ontimer interval %u", interval);
}

void CHandlerClientSession::SendPacket(WorldPacket *packet)
{
	m_pRelay->SendPacket(packet);
}
