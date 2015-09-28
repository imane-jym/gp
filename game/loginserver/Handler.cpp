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

#include "DatabaseMysql.h"
#include "NameChecker.h"
#include "LoginGroup.h"
#include "QueryResult.h"
#include "EnumDefines.h"
#include "CenterDBCtrl.h"
#include "Util.h"
#include "LoginGroup.h"
#include "EnumDefines.h"
#include "CmdDefine.h"
#include "ConfigReader.h"

extern INIFile g_serverIni;

DatabaseMysql LoginDatabase;        ///< Accessor to the login database
//DatabaseMysql CharacterDatabase;	///< Accessor to the character database
DatabaseMysql OperateDatabase;       ///< Accessor to the Operation database
std::string g_strConfigPath;
std::string g_strLoginNotice;
std::string g_strPatchUrl;
std::string g_strLoginDatabaseName;

uint32 g_InitCharacterid = 0;
int g_serverId;
std::map<uint32_t, std::string> g_mapRes;
std::vector<std::string> g_vecAd;

int g_nVersion = 0;

bool ExistsIndex( const char* table, const char* index )
{
	QueryResult* result = LoginDatabase.PQuery(
			"select * from information_schema.statistics where table_name='%s' and index_name='%s' and table_schema = '%s'",
			table, index, g_strLoginDatabaseName.c_str() 
			);

	if ( result )
	{
		delete result;
		return true;
	}
	return false;
}

bool ExistsColumn( const char* table, const char* column )
{
	QueryResult* result = LoginDatabase.PQuery(
			"select * from information_schema.columns where table_name='%s' and column_name='%s' and table_schema='%s'",
			table, column, g_strLoginDatabaseName.c_str() 
			);
	if ( result )
	{
		delete result;
		return true;
	}
	return false;
}

bool ExistsTable( const char* table )
{
	QueryResult* result = LoginDatabase.PQuery(
			"select * from information_schema.tables where table_schema='%s' and table_name='%s'",
			g_strLoginDatabaseName.c_str(), table
			);
	if ( result )
	{
		delete result;
		return true;
	}
	return false;
}

bool StartDB()
{
	std::string dbstring;
	INISection * database = g_serverIni.GetSection("other");
	if (!database)
	{
		IME_ERROR("Miss section [database] in loginserver.ini");
		return false;
	}

	if (!database->ReadString("logindatabase",dbstring))
	{
		IME_ERROR("Missing login database info");
		return false;
	}
    ///- Initialise the login database
	if(!LoginDatabase.Initialize(dbstring.c_str()))
	{
		IME_ERROR("Cannot connect to login database %s",dbstring.c_str());
		return false;
	}

//	if (!database->ReadString("characterdatabase",dbstring))
//	{
//		IME_ERROR("Missing character database info");
//		return false;
//	}
//    ///- Initialise the character database
//	if(!CharacterDatabase.Initialize(dbstring.c_str()))
//	{
//		IME_ERROR("Cannot connect to character database %s",dbstring.c_str());
//		return false;
//	}

	std::vector<std::string> vecData;
	CUtil::StrSplit(dbstring, "'", vecData);
	g_strLoginDatabaseName = vecData[4];

	if (!ExistsTable("gameserver_info"))
	{
	LoginDatabase.PExecute(                                                
			"CREATE TABLE `gameserver_info` ("
			"`area_id` int(11) primary key NOT NULL,"
			"`ip` varchar(64) NOT NULL,"
			"`port` int(11) NOT NULL,"
			"`online_num` int(11) NOT NULL,"
			"`version` int(11) NOT NULL,"
			"`update_time` bigint(20) NOT NULL,"
			"`can_login` smallint(6) NOT NULL DEFAULT '0'," /* 0:不可登录 1: 可以登录(测试登录,这个字段不起效果) */
			"`status` smallint(6) NOT NULL DEFAULT '0',"
			"`is_new` smallint(6) NOT NULL DEFAULT '0',"
			"`server_name` varchar(64) NOT NULL "
			") ENGINE=InnoDB DEFAULT CHARSET=utf8 ");
	}

	if (!ExistsColumn("gameserver_info", "domain"))
	{
		LoginDatabase.PExecute(
				"alter table gameserver_info add (`domain` varchar(255) NOT NULL)"
				);
	}
	if (!ExistsColumn("gameserver_info", "res_url"))
	{
		LoginDatabase.PExecute("alter table gameserver_info add (`res_url` varchar(255) NOT NULL)"
				);
	}

	//这里做alter！！！！！！！                                     
	if (!ExistsTable("user_center"))
	{
	LoginDatabase.PExecute(                                                
			"CREATE TABLE `user_center` ("
				"`user_id` bigint(20) NOT NULL primary key AUTO_INCREMENT,"
				"`account` varchar(64) NOT NULL,"
				"`channel` int unsigned NOT NULL DEFAULT '0',"
				"`pwd` varchar(64) NOT NULL,"
				"`mail` varchar(64) NOT NULL,"
				"`uid` varchar(64) NOT NULL,"
				"`isauto` tinyint(1) NOT NULL DEFAULT '0',"
				"`access` smallint(6) NOT NULL DEFAULT '0'," /* 0:正常 1:不许登录 */
				"`time` datetime NOT NULL,"
				"`token` varchar(128) NOT NULL,"
				"`status` tinyint NOT NULL DEFAULT '0',"
				"`reg_ip` varchar(128) NOT NULL,"
				"`reg_device` varchar(128) NOT NULL,"
				"`reg_device_type` varchar(128) NOT NULL,"
				"`last_login_time` int unsigned NOT NULL,"
				"`second_channel` varchar(128) NOT NULL,"
				"UNIQUE KEY (`account`, `channel`), "
				"KEY (`uid`, `channel`, `isauto`) "
				") ENGINE=InnoDB DEFAULT CHARSET=utf8 AUTO_INCREMENT=10000");
	}
	//这里做alter！！！！！！！                                     
	if (!ExistsTable("character_info"))
	{
	LoginDatabase.PExecute(                                                
			"CREATE TABLE `character_info` ("
				"`character_id` bigint(20) NOT NULL primary key AUTO_INCREMENT,"
				"`area_id` int unsigned NOT NULL, "
				"`user_id` bigint(20) NOT NULL, "
				"unique index (`area_id`, `user_id`) "
				") ENGINE=InnoDB DEFAULT CHARSET=utf8 AUTO_INCREMENT=10000");
	}
	//这里做alter！！！！！！！                                     
	QueryResult *result = LoginDatabase.PQuery("select max(character_id) from character_info;");
	if (result == NULL)
	{
		IME_ERROR("get character max id fail");
		return false;
	}
	uint32 maxautoid = result->Fetch()[0].GetUInt32(); 
	g_InitCharacterid = maxautoid >> 8;
	g_InitCharacterid = g_InitCharacterid < PLAYER_INIT_ID ? PLAYER_INIT_ID - 1 : g_InitCharacterid; 
	IME_LOG("init autoid %u", g_InitCharacterid);	

	dbstring = "";
	if (!database->ReadString("operatedatabase",dbstring))
	{
		IME_ERROR("Missing operate database info");
		return false;
	}
    ///- Initialise the operate database
	if(!OperateDatabase.Initialize(dbstring.c_str()))
	{
		IME_ERROR("Cannot connect to login database %s",dbstring.c_str());
		return false;
	}
	return true;
}


bool HandlerInit(uint8_t thread)
{
	if (thread == MAIN_THREAD)
	{
	//to do implementation
//	IME_LOG("handler init %u", thread);
	INISection * configPath = g_serverIni.GetSection("other");
	if (!configPath)
	{
		IME_ERROR("Miss section [other] in loginserver.ini");
		return false;
	}
	
	int nVersion = 0;
	if (!configPath->ReadInt("minimum_version", nVersion))
	{
		IME_ERROR("Missing versoin info");
		return false;
	}
	g_nVersion = nVersion;
	std::string strConfigPath;
	if (!configPath->ReadString("config_path",strConfigPath))
	{
		IME_ERROR("Missing config_path info");
		return false;
	}
	g_strConfigPath = strConfigPath;

	std::string strKeyWordFile = g_strConfigPath + "keyword.txt";
	if (!CNameChecker::Init(strKeyWordFile.c_str()))
	{
		IME_ERROR("Load keyword.txt error");
		return false;
	}
	std::string strResVersion = g_strConfigPath + "ServerRes.csv";
	if (!InitResVersion(g_mapRes, strResVersion))
	{
		IME_ERROR("Load ServerRes.csv error");
		return false;
	}

//	if (!CAreaConf::Init(configPath))
//	{
//		IME_ERROR("Load AreaConf error");
//		return false;
//	}
	if (!configPath->ReadString("login_notice",g_strLoginNotice))
	{
		IME_ERROR("Missing login_notice info");
		return false;
	}
	if (!configPath->ReadString("patch_url",g_strPatchUrl))
	{
		IME_ERROR("Missing patch_url info");
		return false;
	}
	if (!configPath->ReadInt("area_id",g_serverId))
	{
		IME_ERROR("Missing area_id info");
		return false;
	}
	
//	sLoginGroup->LoadTestAccount(configPath);
	//do many init db here              
	if (!StartDB())                     
	{
		IME_ERROR("StartDB failed");    
		return 0;
	}

	}
	else if (thread == WORLD_THREAD)
	{
		sLoginGroup->InitSettings();
	}

	return true;
}

void HandlerFinally(uint8_t thread)
{
	//to do implementation
//	IME_LOG("handler finally %u", thread);
}

void HandlerOntimer(uint32_t interval)
{
	//to do implementation
//	IME_LOG("handler Ontimer %u", interval);
	if (interval == TIMER_1MIN)
	{
		sLoginGroup->OnTimer1min();
	}
}

CHandlerPlayersession::CHandlerPlayersession()
{
	m_pPlayer = NULL;
	m_strIp = "";

	//to do implementation
	InitOpcodeHandler();
	m_byFlag = 0;
	m_eUserStatus = E_STATUS_AUTHED;
}

void CHandlerPlayersession::Process(WorldPacket &packet)
{
	//to do implementation
//	IME_LOG("playersession get packet size %u", packet.size());
	
	uint16 opcode = packet.GetOpcode();
	if (GetUserStatus() != E_STATUS_AUTHED)
	{
		//如果还没有验证通过 则只有少数几个协议处理
		switch(opcode)
		{
			//todo
			default:
				IME_ERROR("client send packet before auth.opcode %d",opcode);

		};
		return;
	}

	OpcodeHandlerFunc pHandlerFunc = FindOpcodeHandlerFunc(opcode);
	if (pHandlerFunc)
	{
		(this->*pHandlerFunc)(packet);
	}
	else
	{
		IME_ERROR("unknow opcode %d",opcode);
	}
}

void CHandlerPlayersession::UnInit()
{
	//to do implementation
//	IME_LOG("playersession uninit");
}

void CHandlerPlayersession::OnTimer(uint32_t interval)
{
	//to do implementation
//	IME_LOG("playersession ontimer %u", interval);
	if (interval == TIMER_5S)
	{
		std::string strResVersion = g_strConfigPath + "ServerRes.csv";
		InitResVersion(g_mapRes, strResVersion);
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
	m_mapOpcodeHandlerFuncs[CMD_LOGIN_CS_TEST] = &CHandlerPlayersession::HandlerEcho;
	m_mapOpcodeHandlerFuncs[CMD_LOGIN_CS_SERVER_LIST] = &CHandlerPlayersession::HandlerReqServerList;
	m_mapOpcodeHandlerFuncs[CMD_LOGIN_CS_PATCH_URL] = &CHandlerPlayersession::HandlerPatchUrl;
	m_mapOpcodeHandlerFuncs[CMD_LOGIN_CS_REG] = &CHandlerPlayersession::HandlerClientReg;
	m_mapOpcodeHandlerFuncs[CMD_LOGIN_CS_LOGIN] = &CHandlerPlayersession::HandlerClientLogin;
	m_mapOpcodeHandlerFuncs[CMD_LOGIN_CS_FASTLOGIN] = &CHandlerPlayersession::HandlerClientFastLogin;
	m_mapOpcodeHandlerFuncs[CMD_LOGIN_CS_OTHERLOGIN] = &CHandlerPlayersession::HandlerClientOtherLogin;
	m_mapOpcodeHandlerFuncs[CMD_LOGIN_CS_VERSION_NEED] = &CHandlerPlayersession::HandlerClientVersionNeed;
	m_mapOpcodeHandlerFuncs[CMD_LOGIN_CS_AD] = &CHandlerPlayersession::HandlerAd;
	m_mapOpcodeHandlerFuncs[CMD_LOGIN_CS_SERVER_LIST2] = &CHandlerPlayersession::HandlerReqServerListV2;
}

///////////////////////////////////////////////////////////////
void CHandlerPlayersession::HandlerEcho(WorldPacket & packet)
{
	std::string strContent;
	packet >> strContent;
	IME_LOG("*********Recive Echo Content: %s", strContent.c_str());

	WorldPacket data(CMD_LOGIN_SC_TEST, packet.size());
	data << "Hello from relay server~";
	SendPacket(&data);
}
void CHandlerPlayersession::HandlerClientReg(WorldPacket & packet)
{
	std::string strAccount;
	std::string strPwd;
	std::string strMail;
	uint16 wChannel;
	packet >> strAccount;
	packet >> wChannel;
	packet >> strPwd;
	packet >> strMail;

	//验证帐号密码 gameserver也验证
	if (strAccount.size() == 0 || strAccount.size() > E_MAX_USERNAME_PWD_LEN)
	{
		SendRegResult(E_ERRNO_AUTH_NAME_LEN,strAccount,strPwd);
		return ;
	}
	if (strPwd.size() == 0 || strPwd.size() > E_MAX_USERNAME_PWD_LEN)
	{
		SendRegResult(E_ERRNO_AUTH_PWD_LEN,strAccount,strPwd);
		return ;
	}
	if (strAccount.size() > 2 && strAccount.at(0) == 'u' && CUtil::IsDigit(strAccount.at(1)))
	{
		SendRegResult(E_ERRNO_AUTH_ACCOUNT_HAVE,strAccount,strPwd);
		return ;
	}
	if (!(wChannel == E_CHANNEL_IOS || wChannel == E_CHANNEL_ANDROID || wChannel == E_CHANNEL_WIN))
	{
		SendRegResult(E_ERRNO_AUTH_CHANNEL, strAccount, strPwd);
		return;
	}
	LoginDatabase.escape_string(strAccount);
	LoginDatabase.StartTransaction();
	QueryResult* result = LoginDatabase.PQuery(
		"select user_id,pwd from user_center where account = '%s' and channel = '%d' for update",
		strAccount.c_str(), wChannel);

	uint32 accountId = 0;
	if (result)
	{
		delete result;
		SendRegResult(E_ERRNO_AUTH_ACCOUNT_HAVE,strAccount,strPwd);
		LoginDatabase.EndCommit();
		return ;
	}
	LoginDatabase.escape_string(strPwd);
	LoginDatabase.escape_string(strMail);
//	LoginDatabase.escape_string(strUid);
	LoginDatabase.PExecute(
			"insert into pet_center (account, channel, pwd, mail, uid, isauto, time) values ('%s', '%d', '%s','%s','%s',%d, now())",
			strAccount.c_str(),wChannel,strPwd.c_str(),strMail.c_str(),"", 0
			);
	LoginDatabase.EndCommit();
	SendRegResult(E_ERRNO_AUTH_REG_SUCCESS,strAccount,strPwd);
}

void CHandlerPlayersession::HandlerClientLogin(WorldPacket & packet)
{
	
	uint32 dwVersion;
	std::string strAccount;
	std::string strPwd;
//	std::string strIp;
//	int nPort;
	std::string strUid;
	uint16 wChannel;
	packet >> strUid;
	packet >> dwVersion;
	packet >> strAccount;
	packet >> strPwd;
	packet >> wChannel;

	//IME_LOG("client %u",uint32(CONVERT_VERSION(dwVersion)));
	//IME_LOG("server %u",uint32(SERVER_VERSION));

//	if (dwVersion != INNER_VERSION)
//	{
//		SendLoginResult(E_ERRNO_AUTH_WRONG_VERSION);
//		return ;
//	}


	//验证帐号密码 gameserver也验证
	if (strAccount.size() == 0 || strAccount.size() > E_MAX_USERNAME_PWD_LEN)
	{
		SendLoginResult(E_ERRNO_AUTH_NAME_LEN);
		return ;
	}
	if (strPwd.size() == 0 || strPwd.size() > E_MAX_USERNAME_PWD_LEN)
	{
		SendLoginResult(E_ERRNO_AUTH_PWD_LEN);
		return ;
	}
//	if (!(wChannel == E_CHANNEL_IOS || wChannel == E_CHANNEL_ANDROID || wChannel == E_CHANNEL_WIN))
//	{
//		SendLoginResult(E_ERRNO_AUTH_CHANNEL);
//		return ;
//	}

	LoginDatabase.escape_string(strAccount);
//	LoginDatabase.escape_string(strPwd);
//	LoginDatabase.escape_string(strUid);
	QueryResult *result = LoginDatabase.PQuery(
			"select user_id,pwd,uid,isauto from user_center where account = '%s' and channel = %d",
			strAccount.c_str(), wChannel
			);
	uint32 accountId = 0;
	if (result)
	{
		//todo
		Field *fields = result->Fetch();
		accountId = fields[0].GetUInt32();
		std::string strPwdReal = fields[1].GetString();
		std::string strUidReal = fields[2].GetString();
		int nIsAuto = fields[3].GetInt32();
		delete result;

		if (nIsAuto && strPwdReal.length() == 0)
		{
			//自动生成的帐号 并且没有修改过密码
			{
				SendLoginResult(E_ERRNO_AUTH_NO_PWD);
				return ;
			}
		}
		else
		{
			//
			if (strPwdReal == strPwd)
			{
				//验证成功 版本比对 选择服务器
//				if (ChooseServer(dwVersion,strIp,nPort,accountId))
//				{
					SendLoginResult(E_ERRNO_AUTH_SUCCESS);
//				}
//				else
//				{
//					SendLoginResult(E_ERRNO_AUTH_NO_SERVER);
//				}
			}
			else
			{
				//密码不正确
				SendLoginResult(E_ERRNO_AUTH_WRONG_PWD);
			}
		}
	}
	else
	{
		//没有该用户
		SendLoginResult(E_ERRNO_AUTH_NO_ACCOUNT);
	}
}

void CHandlerPlayersession::HandlerClientFastLogin(WorldPacket &packet)
{
	std::string strUid;
	uint32 dwVersion;
	std::string strName;
	std::string strPwd;
//	std::string strIp;
//	int nPort;
	uint16 wChannel;
	packet >> strUid;
	packet >> dwVersion;
	packet >> strName;
	packet >> strPwd;
	packet >> wChannel;
	//IME_LOG("FAST LOGIN 1[Uid=%s len=%u ]", strUid.c_str(), strUid.length());
//	if (dwVersion != INNER_VERSION)
//	{
//		SendFastLoginResult(E_ERRNO_AUTH_WRONG_VERSION);
//		return ;
//	}
//	if (!(wChannel == E_CHANNEL_IOS || wChannel == E_CHANNEL_ANDROID || wChannel == E_CHANNEL_WIN))
//	{
//		SendFastLoginResult(E_ERRNO_AUTH_CHANNEL);
//		return ;
//	}
	LoginDatabase.escape_string(strUid);
//	LoginDatabase.escape_string(strName);
//	LoginDatabase.escape_string(strPwd);
	//IME_LOG("FAST LOGIN 2[Uid=%s len=%u ]", strUid.c_str(), strUid.length());
	QueryResult *result = LoginDatabase.PQuery(
			"select user_id,account,pwd,uid,isauto from pet_center where uid = '%s' and channel = %d and isauto = 1",
			strUid.c_str(), wChannel
			);


	if (result)
	{
		//有
//		Field *fields = result->Fetch();
//		uint32 dwAccountId = fields[0].GetUInt32();
//		if (ChooseServer(dwVersion,strIp,nPort,dwAccountId))
//		{
//			SendFastLoginResult(E_ERRNO_AUTH_SUCCESS,strIp,nPort);
//		}
//		else
//		{
//			SendFastLoginResult(E_ERRNO_AUTH_NO_SERVER,strIp,nPort);
//		}
		SendFastLoginResult(E_ERRNO_AUTH_SUCCESS);
		delete result;
		return ;
	}
	else
	{
		//IME_LOG("FAST LOGIN 3[Uid=%s len=%u ]", strUid.c_str(), strUid.length());
		//没有需要生成
		LoginDatabase.StartTransaction();
		int ret = LoginDatabase.PExecute(
				"insert into user_center (account,channel,pwd,mail,uid,isauto,time) values ('%s',%d, '%s','%s','%s',%d,now())",
				"tmp",wChannel,"","",strUid.c_str(),1
				);
		if (ret)
		{

			QueryResult *resultNext = LoginDatabase.PQuery(
					"select user_id from user_center where uid = '%s' and channel = %d and isauto = 1 for update",
					strUid.c_str(), wChannel
					);
			if (resultNext)
			{
				Field *fields = resultNext->Fetch();
				uint32 dwAccountId = fields[0].GetUInt32();
				IME_LOG("FASTH LOGIN USER ID[UserId=%u]", dwAccountId);
				LoginDatabase.PExecute(
						"update pet_center set account = '%s' where user_id = %u",
						GetAutoAccount(dwAccountId).c_str(),dwAccountId
						);
				delete resultNext;

				//			if (ChooseServer(dwVersion,strIp,nPort,dwAccountId))
				//			{
				SendFastLoginResult(E_ERRNO_AUTH_SUCCESS);
				//			}
				//			else
				//			{
				//				SendFastLoginResult(E_ERRNO_AUTH_NO_SERVER,strIp,nPort);
				//			}
				LoginDatabase.EndCommit();
				return ;
			}
			else
			{
				IME_ERROR("sql error find result next");
			}
		}
		SendFastLoginResult(E_ERRNO_AUTH_SYS);
		LoginDatabase.EndRollback();
		return;
	}
}

void CHandlerPlayersession::CharacterCreate(uint64 account_id, uint32 &character_id, uint16 serverid)
{
	QueryResult *result = LoginDatabase.PQuery(
			"select character_id from character_info where user_id = '%lld' and area_id = %d",
			account_id, serverid
			);
	if (result)
	{
		//todo
		Field *fields = result->Fetch();
		character_id = fields[0].GetUInt32();
		delete result;
		return;
	}
	else
	{
		
		character_id = g_InitCharacterid;
		character_id++;
		character_id = character_id << 8 | character_id % 256;

		bool ret = LoginDatabase.PExecute(
				"insert into character_info(character_id, user_id, area_id) values(%u, %lld, %d)",
				character_id, account_id, serverid
				);
		if (!ret)
		{
			character_id = 0;
			return;
		}
		
		g_InitCharacterid++;	
//		CenterDBCtrl::InsertRePassport(account_id, character_id, serverid);
		return;
	}
}

void CHandlerPlayersession::HandlerClientOtherLogin(WorldPacket &packet)
{
	std::string strUid;
	uint32 dwVersion;
	std::string strName;
	std::string strPwd;
//	std::string strIp;
//	int nPort;
	std::string strmail;
	std::string strtoken;
	std::string strregdevice;
	std::string strregdevicetype;
	uint16 wChannel;
	std::string strsecondchannel;
	uint16 AreaId;
	uint8 gm;
	packet >> strUid;
	packet >> dwVersion;
	packet >> strName;
	packet >> strPwd;
	packet >> wChannel;
	packet >> strmail;
	packet >> strtoken;
	packet >> strregdevice;
	packet >> strregdevicetype;
	packet >> strsecondchannel;
	packet >> AreaId;
	packet >> gm; 
//	if (dwVersion != INNER_VERSION)
//	{
//		SendOtherLoginResult(E_ERRNO_AUTH_WRONG_VERSION);
//		return ;
//	}
//	if (!(wChannel == E_CHANNEL_IOS || wChannel == E_CHANNEL_ANDROID || wChannel == E_CHANNEL_WIN))
//	{
//		SendOtherLoginResult(E_ERRNO_AUTH_CHANNEL);
//		return ;
//	}

	IME_LOG("struid %s version %u name %s passwd %s channel %u areaid %u gm %u", strUid.c_str(), dwVersion, strName.c_str(), strPwd.c_str(), wChannel, AreaId, gm);
	std::map<uint16, CLoginGroup::STC_SERVER_STATUS> mapTemp = sLoginGroup->GetServerList();
	std::map<uint16, CLoginGroup::STC_SERVER_STATUS>::iterator it = mapTemp.find(AreaId);
	if (it == mapTemp.end())
	{
		IME_ERROR("unvalid areaid %u", AreaId);
		SendOtherLoginResult(E_ERRNO_AUTH_NO_SERVER);
		return;
	}

	LoginDatabase.escape_string(strUid);
	LoginDatabase.escape_string(strName);
	QueryResult *result = NULL;
	if (wChannel >= CHANNEL_MIN && wChannel <= CHANNEL_MAX)
	{
		result = LoginDatabase.PQuery(
				"select user_id,account,pwd,uid,isauto from user_center where account = '%s' and channel >= %u and channel <= %u",
				strName.c_str(), CHANNEL_MIN, CHANNEL_MAX
				);
	}
	else if (wChannel >= CHANNEL_MIN2 && wChannel <= CHANNEL_MAX2)
	{
		result = LoginDatabase.PQuery(
				"select user_id,account,pwd,uid,isauto from user_center where account = '%s' and channel >= %u and channel <= %u",
				strName.c_str(), CHANNEL_MIN2, CHANNEL_MAX2
				);
	}
	else if (wChannel >= CHANNEL_MIN3 && wChannel <= CHANNEL_MAX3)
	{
		result = LoginDatabase.PQuery(
				"select user_id,account,pwd,uid,isauto from user_center where account = '%s' and channel >= %u and channel <= %u",
				strName.c_str(), CHANNEL_MIN3, CHANNEL_MAX3
				);
	}
	else
	{
		result = LoginDatabase.PQuery(
				"select user_id,account,pwd,uid,isauto from user_center where account = '%s' and channel = %d",
				strName.c_str(), wChannel
				);
	}


	if (result)
	{
		LoginDatabase.escape_string(strtoken);
		if (wChannel >= CHANNEL_MIN && wChannel <= CHANNEL_MAX)
		{
			LoginDatabase.PExecute(
					"update user_center set token = '%s',last_login_time = %u where account = '%s' and channel >= %d and channel <= %d",
					strtoken.c_str(), time(NULL), strName.c_str(), CHANNEL_MIN, CHANNEL_MAX
					);
		}
		else if (wChannel >= CHANNEL_MIN2 && wChannel <= CHANNEL_MAX2)
		{
			LoginDatabase.PExecute(
					"update user_center set token = '%s',last_login_time = %u where account = '%s' and channel >= %d and channel <= %d",
					strtoken.c_str(), time(NULL), strName.c_str(), CHANNEL_MIN2, CHANNEL_MAX2
					);
		}
		else if (wChannel >= CHANNEL_MIN3 && wChannel <= CHANNEL_MAX3)
		{
			LoginDatabase.PExecute(
					"update user_center set token = '%s',last_login_time = %u where account = '%s' and channel >= %d and channel <= %d",
					strtoken.c_str(), time(NULL), strName.c_str(), CHANNEL_MIN3, CHANNEL_MAX3
					);
		}
		else
		{
			LoginDatabase.PExecute(
					"update user_center set token = '%s',last_login_time = %u where account = '%s' and channel = %d",
					strtoken.c_str(), time(NULL), strName.c_str(), wChannel
					);
		}
		//有
		//验证成功 版本比对 选择服务器
		Field *fields = result->Fetch();
//		uint32 dwAccountId = fields[0].GetUInt32();
		uint16 IsAuto = fields[4].GetUInt8();
//		if (ChooseServer(dwVersion,strIp,nPort,dwAccountId))
//		{
//			Send91LoginResult(E_ERRNO_AUTH_SUCCESS,strIp,nPort);
//		}
//		else
//		{
//			Send91LoginResult(E_ERRNO_AUTH_NO_SERVER,strIp,nPort);
//		}

				uint64 dwAccountId = fields[0].GetUInt64();
				uint32 characterid = 0;
					
				CharacterCreate(dwAccountId, characterid, AreaId);
				if (characterid == 0)
				{
					IME_ERROR("create character fail account %s, channel %d", strName.c_str(), wChannel);
					SendOtherLoginResult(E_ERRNO_AUTH_SYS);
					delete result;
					return;
				}

		if (IsAuto)
		{
			WorldPacket pkg,pkg2;
			pkg << gm;
			pkg2.append(pkg.contents(), pkg.size());
//			IME_ERROR("flag %u", m_byFlag);
			HandlerReqServerList(pkg);
			HandlerReqServerListV2(pkg2);
			SendOtherLoginResult(E_ERRNO_AUTH_SUCCESS);
		}
		else
		{
			SendOtherLoginResult(E_ERRNO_AUTH_UNVALID);
		}
		delete result;
		return ;
	}
	else
	{
		//没有需要生成
		LoginDatabase.escape_string(strtoken);
		LoginDatabase.escape_string(strregdevice);
		LoginDatabase.escape_string(strregdevicetype);
		std::string strip = m_pPlayer->GetRemoteIP();
		LoginDatabase.escape_string(strip);
		int ret = LoginDatabase.PExecute(
				"insert into user_center (account,channel,pwd,mail,uid,isauto,time, token, reg_ip, reg_device, reg_device_type, last_login_time, second_channel) values ('%s',%d,'%s','%s','%s',%d, now(), '%s', '%s', '%s', '%s', '%u', '%s')",
				strName.c_str(),wChannel,"","",strUid.c_str(),1, strtoken.c_str(), strip.c_str(), strregdevice.c_str(), strregdevicetype.c_str(), time(NULL), strsecondchannel.c_str()
				);
		if (ret)
		{
//			QueryResult *resultNext = LoginDatabase->PQuery(
//					"select user_id from pet_users where account = '%s' and channel = %d for update",
//					strName.c_str()
//					);
//			if (resultNext)
//			{
//				Field *fields = resultNext->Fetch();
//				uint32 dwAccountId = fields[0].GetUInt32();
				//			CharacterDatabase.PExecute(
				//					"update pet_users set account = '%s' where user_id = %u",
				//					GetAutoAccount(dwAccountId).c_str(),dwAccountId
				//					);
//				delete resultNext;
//
//				if (ChooseServer(dwVersion,strIp,nPort,dwAccountId))
//				{

			QueryResult *resultNext = LoginDatabase.PQuery(
					"select user_id,account,pwd,uid,isauto from user_center where account = '%s' and channel = %d",
					strName.c_str(), wChannel
					);
			if (!resultNext)
			{
				IME_ERROR("get accountid fail account %s, channel %d", strName.c_str(), wChannel);
				SendOtherLoginResult(E_ERRNO_AUTH_SYS);
				return;
			}
				Field *fields = resultNext->Fetch();
				uint64 dwAccountId = fields[0].GetUInt64();
				uint32 characterid = 0;
				delete resultNext;
					
					CharacterCreate(dwAccountId, characterid, AreaId);
					if (characterid == 0)
					{
						IME_ERROR("create character fail account %s, channel %d", strName.c_str(), wChannel);
						SendOtherLoginResult(E_ERRNO_AUTH_SYS);
						return;
					}

					WorldPacket pkg,pkg2;
					pkg << gm;
					pkg2.append(pkg.contents(), pkg.size());
//					IME_ERROR("flag %u", m_byFlag);
					HandlerReqServerList(pkg);
					HandlerReqServerListV2(pkg2);
					SendOtherLoginResult(E_ERRNO_AUTH_SUCCESS);
//				}
//				else
//				{
//					Send91LoginResult(E_ERRNO_AUTH_NO_SERVER,strIp,nPort);
//				}
				return ;
//			}
//			else
//			{
//				IME_ERROR("sql error find result next");
//			}
		}
		IME_ERROR("insert account sys error");
		SendOtherLoginResult(E_ERRNO_AUTH_SYS);
		return;
	}
}

std::string CHandlerPlayersession::GetAutoAccount(uint32 dwAccountId)
{
	std::string strPre = "u";
	std::string strLast = CUtil::Uint32ToString(dwAccountId);
	for(int i = int(strLast.size()); i < 6; ++i)
	{
		strPre += "0";
	}

	return strPre + strLast;
}

/////////////////////////////////////////////////////
void CHandlerPlayersession::SendLoginResult(uint16 byErrno)
{
	WorldPacket packet(CMD_LOGIN_SC_LOGINRESULT);
	packet << byErrno;
	SendPacket(&packet);
}
void CHandlerPlayersession::SendRegResult(uint16 byErrno,std::string &strAccount, std::string &strPwd)
{
	WorldPacket packet(CMD_LOGIN_SC_REGRESULT);
	packet << byErrno;
	packet << strAccount;
	packet << strPwd;
	SendPacket(&packet);
}
void CHandlerPlayersession::SendFastLoginResult(uint16 byErrno)
{
	WorldPacket packet(CMD_LOGIN_SC_FASTRESULT);
	packet << byErrno;
	SendPacket(&packet);
}
void CHandlerPlayersession::SendOtherLoginResult(uint16 byErrno)
{
	WorldPacket packet(CMD_LOGIN_SC_OTHERRESULT);
	packet << byErrno;
	SendPacket(&packet);
}


void CHandlerPlayersession::HandlerReqServerList(WorldPacket &packet)
{
	uint8 flag;
	packet >> flag;
//	IME_ERROR("byflag %u, request flag %u", m_byFlag, flag);
	m_byFlag = flag;
//	IME_ERROR("after byflag %u, request flag %u", m_byFlag, flag);
	
	std::map<uint16, CLoginGroup::STC_SERVER_STATUS> mapTemp = sLoginGroup->GetServerList();
	std::map<uint16, CLoginGroup::STC_SERVER_STATUS>::iterator it;
	uint16 number = 0;
	WorldPacket data(CMD_LOGIN_SC_SERVER_LIST);
	ByteBuffer tmp;
	
	for (it = mapTemp.begin(); it != mapTemp.end(); it++)
	{
		IME_ERROR("areaid %u canlogin %u flag %u", it->second.wAreaId, it->second.byCanLogin, flag);
		if (flag) // test reqlist
		{
			tmp << it->second.bIsAlive;
			tmp << it->second.wAreaId;
			tmp << it->second.strIp;
			tmp << it->second.dwPort;
			tmp << it->second.dwVersion;
			tmp << it->second.byStatus;
			tmp << it->second.byIsNew;
			tmp << it->second.strServerName;
			tmp << it->second.strDomain;
			number++;
		}	
		else
		{
			if (it->second.byCanLogin)
			{
				tmp << it->second.bIsAlive;
				tmp << it->second.wAreaId;
				tmp << it->second.strIp;
				tmp << it->second.dwPort;
				tmp << it->second.dwVersion;
				tmp << it->second.byStatus;
				tmp << it->second.byIsNew;
				tmp << it->second.strServerName;
				tmp << it->second.strDomain;
				number++;
			}
		}
	}
	data << number;
	data.append((const char *)tmp.contents(), tmp.size());
	data << g_strLoginNotice;
	SendPacket(&data);
}

void CHandlerPlayersession::HandlerReqServerListV2(WorldPacket &packet)
{
	uint8 flag;
	packet >> flag;
//	IME_ERROR("byflag %u, request flag %u", m_byFlag, flag);
	m_byFlag = flag;
//	IME_ERROR("after byflag %u, request flag %u", m_byFlag, flag);
	
	std::map<uint16, CLoginGroup::STC_SERVER_STATUS> mapTemp = sLoginGroup->GetServerList();
	std::map<uint16, CLoginGroup::STC_SERVER_STATUS>::iterator it;
	uint16 number = 0;
	WorldPacket data(CMD_LOGIN_SC_SERVER_LISTV2);
	ByteBuffer tmp;
	
	std::string url;

	for (it = mapTemp.begin(); it != mapTemp.end(); it++)
	{
		IME_ERROR("areaid %u canlogin %u flag %u", it->second.wAreaId, it->second.byCanLogin, flag);
		if (flag) // test reqlist
		{
			tmp << it->second.bIsAlive;
			tmp << it->second.wAreaId;
			tmp << it->second.strIp;
			tmp << it->second.dwPort;
			tmp << it->second.dwVersion;
			tmp << it->second.byStatus;
			tmp << it->second.byIsNew;
			tmp << it->second.strServerName;
			tmp << it->second.strDomain;
//			url = it->second.strResUrl + "/" + RESOURCE_DOWNLOAD_NAME;
//			tmp << url;
//			url = it->second.strResUrl + "/" + AD_DOWNLOAD_NAME; 
//			tmp << url;
			url = it->second.strResUrl;
			tmp << url;
			tmp << url;
			number++;
		}	
		else
		{
			if (it->second.byCanLogin)
			{
				tmp << it->second.bIsAlive;
				tmp << it->second.wAreaId;
				tmp << it->second.strIp;
				tmp << it->second.dwPort;
				tmp << it->second.dwVersion;
				tmp << it->second.byStatus;
				tmp << it->second.byIsNew;
				tmp << it->second.strServerName;
				tmp << it->second.strDomain;
//				url = it->second.strResUrl + "/" + RESOURCE_DOWNLOAD_NAME;
//				tmp << url;
//				url = it->second.strResUrl + "/" + AD_DOWNLOAD_NAME; 
//				tmp << url;
				url = it->second.strResUrl;
				tmp << url;
				tmp << url;
				number++;
			}
		}
	}
	data << number;
	data.append((const char *)tmp.contents(), tmp.size());
	data << g_strLoginNotice;
	SendPacket(&data);
}

void CHandlerPlayersession::HandlerPatchUrl(WorldPacket &pkg)
{
	uint32 ver;
	pkg >> ver;
	
	std::string verstr = "";
	if (g_mapRes.find(ver) != g_mapRes.end())
	{
		verstr = "/" + g_mapRes[ver];
	}

	WorldPacket data(CMD_LOGIN_SC_PATCH_URL);
	std::string url = g_strPatchUrl;
	url += "/" + CUtil::Uint32ToString(ver) + verstr + "/" + RESOURCE_DOWNLOAD_NAME;
	data << uint32(INNER_VERSION);
	data << url;
	IME_LOG("first down url %s", url.c_str());
	url.clear();
	url += g_strPatchUrl + "/" + CUtil::Uint32ToString(ver) + verstr + "/" + AD_DOWNLOAD_NAME;
	data << url;
	IME_LOG("second down url %s", url.c_str());
	url.clear();
	url += g_strPatchUrl + "/" + CUtil::Uint32ToString(ver) + verstr + "/" + EXTRA_RESOURCE_DOWNLOAD_NAME;
	data << url;
	IME_LOG("third down url %s", url.c_str());
	SendPacket(&data);
}

void CHandlerPlayersession::HandlerClientVersionNeed(WorldPacket &pkg)
{
	WorldPacket data(CMD_LOGIN_SC_VERSION_NEED_RESULT);
	data << g_nVersion;
	SendPacket(&data);
}

void CHandlerPlayersession::HandlerAd(WorldPacket &pkg)
{
	uint32_t ver;
	pkg >> ver;
	
	WorldPacket info(CMD_LOGIN_SC_AD_RESULT);
	info << (uint32_t)g_vecAd.size();
	for (int i = 0; i < g_vecAd.size(); i++)
	{
		info << g_vecAd[i];
	}
	SendPacket(&info);
}

bool InitResVersion(std::map<uint32_t, std::string> &map, std::string filepath)
{
	CConfigReader reader;
	if (!reader.init(filepath))
	{
		IME_ERROR("init %s for res version fail", filepath.c_str());
		return false;
	}	
	
	uint8_t key;
	std::string val;
	std::map<uint32_t, std::string> temp;
	int nRow = reader.rowCount();
	for (int i = 1; i < nRow; i++)
	{
		int j = 0;
		READER_INT(key);
		READER_STR(val);
		IS_READER_ERROR;

		if (temp.insert(std::make_pair(key, val)).second == false)
		{
			IME_ERROR("ERROR CONF DUPLICATE [CHESTID = %u]", key);
			return false;
		}
	}

	g_mapRes = temp;
	return true;
}

CHandlerClientSession::CHandlerClientSession()
{
	m_pRelay = NULL;

	//to do implementation
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
