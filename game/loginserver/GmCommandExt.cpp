#include "GmCommandExt.h"
#include "DatabaseMysql.h"
#include "CenterDBCtrl.h"
#include "ShardingDB.h"
#include "QueryResult.h"
#include "Util.h"

extern CShardingDB WorldDb;
extern std::string g_strConfigPath;
//extern ConfigManager 	configManager;
extern int g_serverId;

////////// factory impl ///////////////

GmCommand* GmCommandFactory::Create(
		uint32				dwAutoId,
		std::string&		strOpr,
		GmCommandTargetType	eTargetType,
		uint64				dwTargetId,
		uint32				dwStartTime,
		uint32				dwEndTime,
		std::string&		strBackup,
		GmCommandStatus		eStatus,
		std::string&		strParams )
{
	GmCommand* ret = 0;

//	if (strOpr == "active_friend_boss")
//		ret = new ActiveFriendBoss();
//	else if (strOpr == "send_mail")
//		ret = new SendMailCommand();
//	else if (strOpr == "cancel_gm_command")
//		ret = new CancelCommand();
//	else if (strOpr == "turntable")
//		ret = new TurntableCommand();
//	else if ( strOpr == "reload" )
//		ret = new ReloadConfigCommand();
//	else if ( strOpr == "notice" )
//		ret = new AnnouncementCommand();
//	else if ( strOpr == "roleauth" )
//		ret = new SetRoleAuth();
//	else if ( strOpr == "expadd" )
//		ret = new ActiveExpAdd();
//	else if ( strOpr == "coinadd" )
//		ret = new ActiveCoinAdd();
//	else if ( strOpr == "objdrop" )
//		ret = new ActiveObjDrop();
//	else if ( strOpr == "bossrank" )
//		ret = new ActiveBossRank();
//	else if ( strOpr == "setserver" )
//		ret = new SetServer();
//	else if ( strOpr == "charge" )
//		ret = new ActiveCharge();
//	else if ( strOpr == "extralogin" )
//		ret = new ActiveExtraLogin();
//	else if ( strOpr == "fund" )
//		ret = new ActiveFund();
//	else if ( strOpr == "task" )
//		ret = new ActiveTask();

	if ( ret )
	{
		ret->m_dwAutoId 	= dwAutoId;
		ret->m_strOpr		= strOpr;
		ret->m_eTargetType	= eTargetType;
		ret->m_dwTargetId	= dwTargetId;
		ret->m_dwStartTime	= dwStartTime;
		ret->m_dwEndTime	= dwEndTime;
		ret->m_strBackup	= strBackup;
		ret->m_eStatus		= eStatus;
		ret->m_strParams	= strParams;

		// fix
		if ( eStatus == E_GM_COMMAND_STATUS_PENDING || eStatus == E_GM_COMMAND_STATUS_RUNNING )
		{
			// when user update params, error will happen
			if ( !(ret->Validate()) ) // read param again
			{
				CenterDBCtrl::UpdateErrorstr(ret);
			}
		}
	}

	return ret;
}

bool GmCommandFactory::CheckTargetType( uint8 byTargetType, uint64 dwTargetId )
{
	if ( byTargetType == E_GM_COMMAND_TARGET_TYPE_SERVER )
	{
//		return (uint16)dwTargetId == sWorld->GetServerId();
		return dwTargetId == g_serverId;
	}
	else if ( byTargetType == E_GM_COMMAND_TARGET_TYPE_ROLE )
	{
//		DatabaseMysql * mysql = WorldDb.GetDBConnection(0);
//		QueryResult* result = mysql->PQuery(
//			"select user_id from user_login where user_id=%u", (uint32)dwTargetId
//		);
//		if ( result )
//		{
//			delete result;
//			return true;
//		}
		return false;
	}
	else if ( byTargetType == E_GM_COMMAND_TARGET_TYPE_PASSPORT )
	{
		return false;
	}
	else
	{
		return false;
	}
}

///////////////////////////////////
// cancel_gm_command [gm_auto_id]

bool CancelCommand::Validate()
{
	RETURN_FALSE_IF_TARGET_IS_PASSPORT;
	RETURN_FALSE_IF_TARGET_IS_ROLE;
	RETURN_FALSE_IF_PARAMS_SIZE_INVALID( ==, 1 );

	m_dwGmAutoId = GET_CMD_PARAM_INT( 0 );
	if ( m_dwGmAutoId == GetAutoId() || !CenterDBCtrl::HasGmCommand( m_dwGmAutoId ) )
	{
		SetErrorMsg( "Command Not Found" );
		return false;
	}
	return true;
}

bool CancelCommand::Trigger()
{
	if ( !CenterDBCtrl::CancelGmCommand( m_dwGmAutoId ) )
	{
//		SetErrorMsg( "Error When Cancel" );
		return false;
	}
	return true;
}

bool CancelCommand::Complete()
{
	return true;
}
