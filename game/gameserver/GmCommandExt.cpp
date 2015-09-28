#include "GmCommandExt.h"
#include "DatabaseMysql.h"
#include "CenterDBCtrl.h"
#include "World.h"
#include "Role.h"
#include "Util.h"
#include "ShardingDB.h"
#include "QueryResult.h"
#include "Other.h"
#include "Mail.h"
#include "Role.h"
#include "Other.h"
#include "CsvReader.h"
#include "EnumDefines.h"

extern CShardingDB WorldDb;
extern std::string g_strConfigPath;
//extern ConfigManager 	configManager;

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

	if (strOpr == "active_friend_boss")
		ret = new ActiveFriendBoss();
	else if (strOpr == "send_mail")
		ret = new SendMailCommand();
	else if (strOpr == "cancel_gm_command")
		ret = new CancelCommand();
	else if (strOpr == "turntable")
		ret = new TurntableCommand();
	else if ( strOpr == "reload" )
		ret = new ReloadConfigCommand();
	else if ( strOpr == "notice" )
		ret = new AnnouncementCommand();
	else if ( strOpr == "roleauth" )
		ret = new SetRoleAuth();
	else if ( strOpr == "expadd" )
		ret = new ActiveExpAdd();
	else if ( strOpr == "coinadd" )
		ret = new ActiveCoinAdd();
	else if ( strOpr == "objdrop" )
		ret = new ActiveObjDrop();
	else if ( strOpr == "bossrank" )
		ret = new ActiveBossRank();
	else if ( strOpr == "setserver" )
		ret = new SetServer();
	else if ( strOpr == "charge" )
		ret = new ActiveCharge();
	else if ( strOpr == "extralogin" )
		ret = new ActiveExtraLogin();
	else if ( strOpr == "fund" )
		ret = new ActiveFund();
	else if ( strOpr == "task" )
		ret = new ActiveTask();
	else if ( strOpr == "store" )
		ret = new StoreLoad();
	else if ( strOpr == "forcecancel" )
		ret = new ForceCancel();
	else if ( strOpr == "whitelist" )
		ret = new WhiteList();
	else if ( strOpr == "noticectrl" )
		ret = new NoticeCtrl();
	else if ( strOpr == "limitrole")
		ret = new SetRoleAuth2();

//	if ( strOpr == "send_mail" )
//		ret = new SendMailCommand();
//	else if ( strOpr == "update_shop" )
//		ret = new UpdateShopCommand();
//	else if ( strOpr == "broadcast" )
//		ret = new BroadcastCommand();
//	else if ( strOpr == "set_server_state" )
//		ret = new SetServerStateCommand();
//	else if ( strOpr == "set_login_strategy" )
//		ret = new SetLoginStrategyCommand();
//	else if ( strOpr == "set_server_name" )
//		ret = new SetServerNameCommand();
//	else if ( strOpr == "set_res_server_addr" )
//		ret = new SetResServerAddrCommand();
//	else if ( strOpr == "set_res_version" )
//		ret = new SetResVersionCommand();
//	else if ( strOpr == "reload" )
//		ret = new ReloadConfigCommand();
//	else if ( strOpr == "activity_exp" )
//		ret = new ActivityExpBonusCommand();
//	else if ( strOpr == "activity_diamond" )
//		ret = new ActivityDiamondCommand();
//	else if ( strOpr == "activity_exchange" )
//		ret = new ActivityExchangeCommand();
//	else if ( strOpr == "activity_drop" )
//		ret = new ActivitySpecialDropCommand();
//	else if ( strOpr == "activity_lottery" )
//		ret = new ActivityLotteryCommand();
//	else if ( strOpr == "activity_acc_login" )
//		ret = new ActivityAccLoginCommand();
//	else if ( strOpr == "activity_stage" )
//		ret = new ActivityStageCommand();
//	else if ( strOpr == "activity_arena_rank" )
//		ret = new ActivityArenaRankCommand();
//	else if ( strOpr == "activity_role_level" )
//		ret = new ActivityRoleLevelCommand();
//	else if ( strOpr == "activity_common" )
//		ret = new ActivityCommon();
//	else if ( strOpr == "activity_extend_reward" )
//		ret = new ActivityExtendRewardCommand();
//	else if ( strOpr == "set_game_server_notice" )
//		ret = new SetGameServerNoticeCommand();
//	else if ( strOpr == "set_login_server_notice" )
//		ret = new SetLoginServerNoticeCommand();
//	else if ( strOpr == "set_gm_auth" )
//		ret = new SetGmAuthCommand();
//	else if ( strOpr == "re_create_robot" )
//		ret = new ReCreateRobotCommand();

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
		return sWorld->IsTarget((uint32)dwTargetId);
	}
	else if ( byTargetType == E_GM_COMMAND_TARGET_TYPE_ROLE )
	{
		DatabaseMysql * mysql = WorldDb.GetDBConnection(0);
		QueryResult* result = mysql->PQuery(
			"select user_id from user_login where user_id=%u", (uint32)dwTargetId
		);
		if ( result )
		{
			delete result;
			return true;
		}
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

///////// send_mail impl //////////////
// send_mail title content type para1 para2 coin dollar energy strength worldhistory

bool SendMailCommand::Validate()
{
	RETURN_FALSE_IF_TARGET_IS_PASSPORT;
	RETURN_FALSE_IF_PARAMS_SIZE_INVALID( >=, 10 );

	title = vParam[0];
	content = vParam[1];
	type = atol(vParam[2].c_str());
	para1 = atol(vParam[3].c_str());
	para2 = atol(vParam[4].c_str());
	coin = atol(vParam[5].c_str());
	dollar = atol(vParam[6].c_str());
	energy = atol(vParam[7].c_str());
	strength = atol(vParam[8].c_str());
	worldhistory = atol(vParam[9].c_str());
//	uint8 	byType 	= atoi(vParam[2].c_str());
//	uint32	dwId	= atoi(vParam[3].c_str());
//	uint32	dwQuantity = vParam.size() == 5 ? atoi( vParam[4].c_str() ) : 1;

//	if ( dwQuantity == 0 )
//	{
//		SetErrorMsg( "Quantity Can Not Be 0" );
//		return false;
//	}
//
//	void* pConfig;
//
//	switch ( byType )
//	{
//	case 1:
//		m_reward.SetGold( dwQuantity );
//		break;
//	case 2:
//		m_reward.SetDiamond( dwQuantity );
//		break;
//	case 3:
//		pConfig = GET_CONFIG_STC( ItemConfig, dwId );
//		if ( pConfig == NULL )
//		{
//			SetErrorMsg( format_error( "Item Not Found, id=%d", dwId ) );
//			return false;
//		}
//		m_reward.GetItems().insert( std::make_pair( dwId, dwQuantity ) );
//		break;
//	case 4:
//		pConfig = GET_CONFIG_STC( HeroConfig, dwId );
//		if ( pConfig == NULL )
//		{
//			SetErrorMsg( format_error( "Hero Not Found, id=%d", dwId) );
//			return false;
//		}
//		if ( dwQuantity != 1 )
//		{
//			SetErrorMsg( format_error( "Quantity Can Not Larger Than 1 When Add Hero" ) );
//			return false;
//		}
//		m_reward.GetHeros().push_back( dwId );
//		break;
//	case 5:
//		pConfig = GET_CONFIG_STC( EquipConfig, dwId );
//		if ( pConfig == NULL )
//		{
//			SetErrorMsg( format_error( "Equip Not Found, id=%d", dwId) );
//			return false;
//		}
//		if ( dwQuantity != 1 )
//		{
//			SetErrorMsg( format_error( "Quantity Can Not Larger Than 1 When Add Equipment" ) );
//			return false;
//		}
//		m_reward.GetEquips().push_back( dwId );
//		break;
//	case 6:
//		// empty
//		break;
//	default:
//		SetErrorMsg( format_error( "Unknown Reward Type, type=%d", byType ) );
//		return false;
//	}

	return true;
}

bool SendMailCommand::Trigger()
{
	if ( m_eTargetType == E_GM_COMMAND_TARGET_TYPE_ROLE )
	{
		CMail::SBaseAttr tmpAttr = {0};
		tmpAttr.coin = coin;
		tmpAttr.dollar = dollar;
		tmpAttr.energy = energy;
		tmpAttr.strength = strength;
		tmpAttr.worldhistory = worldhistory;

		uint32_t mail_id = 0;
		CDBCtrl::stuMailDetail tmp = {0};
//		if (mailtype == "pay")
//		{
//			CMail::AddEmailSysPay(m_dwTargetId, mail_id, tmp, type, para1, para2, tmpAttr);   
//		}
//		else
//		{
			CMail::AddEmailSysDiy(m_dwTargetId, mail_id, tmp, type, para1, para2, content, tmpAttr, title);   
//		}
		
		CRole *pPeerCRole = sWorld->FindUserRole(m_dwTargetId);
		if (pPeerCRole != NULL)
		{
			CMail::EmailUpdate(pPeerCRole, CMail::ADD, &tmp, 0);
		}
	}
	else
	{
//		SetEndTime( CenterDBCtrl::GetDBTime() + MailSystem::P_MAIL_UNREAD_MAIL_SAVE_TIME );
	}
	return true;
}
bool SendMailCommand::Complete()
{
	// do nothing
	return true;
}

bool SendMailCommand::HandleRole( void* pRole )
{
	if ( m_eTargetType == E_GM_COMMAND_TARGET_TYPE_SERVER )
	{
		CMail::SBaseAttr tmpAttr = {0};
		tmpAttr.coin = coin;
		tmpAttr.dollar = dollar;
		tmpAttr.energy = energy;
		tmpAttr.strength = strength;
		tmpAttr.worldhistory = worldhistory;

		uint32_t mail_id = 0;
		CDBCtrl::stuMailDetail tmp = {0};
//		if (mailtype == "pay")
//		{
//			CMail::AddEmailSysPay(((CRole *)pRole)->GetdwAccountId(), mail_id, tmp, type, para1, para2, tmpAttr);   
//		}
//		else
//		{
//			CMail::AddEmailSysGift(((CRole *)pRole)->GetdwAccountId(), mail_id, tmp, type, para1, para2, mailtype, tmpAttr);   
//		}

		CMail::AddEmailSysDiy(((CRole *)pRole)->GetdwAccountId(), mail_id, tmp, type, para1, para2, content, tmpAttr, title);   
		CMail::EmailUpdate((CRole *)pRole, CMail::ADD, &tmp, 0);
	}
	return true;
}

////////////////////////////
//// ActiveFriendBoss [state]
bool ActiveFriendBoss::Validate()
{
	RETURN_FALSE_IF_TARGET_IS_PASSPORT;
	RETURN_FALSE_IF_TARGET_IS_ROLE;

	return true;
}

bool ActiveFriendBoss::Trigger()
{
//	if (CActiveCtrl::FriendBoss)
//	{
//		SetErrorMsg("friend boss active already start");
//		return false;
//	}
//	CActiveCtrl::FriendBoss = 1;	
//	CActiveCtrl::FriendBossRealStartTime = time(NULL);
//	CActiveCtrl::IsUpdate = 1;
	std::string error = "";
	CActiveCtrl::AddActive(FRIEND_BOSS_ACTIVE_ID, m_strParams, error, m_dwStartTime, m_dwEndTime);
	if (error != "")
	{
		SetErrorMsg(error);
		return false;
	}
	return true;
}

bool ActiveFriendBoss::Complete()
{
	std::string error;
	CActiveCtrl::DelActive(FRIEND_BOSS_ACTIVE_ID, error);
	if (error != "")
	{
		SetErrorMsg(error);
		return false;
	}
	return true;
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

///////////////////////////////////
// turntable_command [gm_auto_id]

bool TurntableCommand::Validate()
{
	RETURN_FALSE_IF_TARGET_IS_PASSPORT;
	RETURN_FALSE_IF_TARGET_IS_ROLE;
	RETURN_FALSE_IF_PARAMS_SIZE_INVALID( ==, TURNTABLE_CONTENT_COUNT * 4 + 4);

	m_byChoose = GET_CMD_PARAM_INT(3);

	if (m_byChoose == 0)
	{
	}
	else if (m_byChoose == 1)
	{
	}
	else
	{
		SetErrorMsg( "Turntable select fail");
		return false;
	}

	return true;
}

bool TurntableCommand::Trigger()
{
	if (m_byChoose == 0)
	{
		std::string error = "";
		CActiveCtrl::AddActive(TURN_NORMAL_TABLE_ACTIVE_ID, m_strParams, error, m_dwStartTime, m_dwEndTime);
		if (error != "")
		{
			SetErrorMsg(error);
			return false;
		}
	}
	else
	{
		std::string error = "";
		CActiveCtrl::AddActive(TURN_SUPER_TABLE_ACTIVE_ID, m_strParams, error, m_dwStartTime, m_dwEndTime);
		if (error != "")
		{
			SetErrorMsg(error);
			return false;
		}
	}
	return true;
}

bool TurntableCommand::Complete()
{
	if (m_byChoose == 0)
	{
		std::string error;
		CActiveCtrl::DelActive(TURN_NORMAL_TABLE_ACTIVE_ID, error);
		if (error != "")
		{
			SetErrorMsg(error);
			return false;
		}
	}
	else
	{
		std::string error;
		CActiveCtrl::DelActive(TURN_SUPER_TABLE_ACTIVE_ID, error);
		if (error != "")
		{
			SetErrorMsg(error);
			return false;
		}
	}
	return true;
}

// reload config impl //////////////
// reload

bool ReloadConfigCommand::Validate()
{
	RETURN_FALSE_IF_TARGET_IS_PASSPORT;
	RETURN_FALSE_IF_TARGET_IS_ROLE;
	RETURN_FALSE_IF_PARAMS_SIZE_INVALID( ==, 1 );

	m_strTable = GET_CMD_PARAM_STRING(0);
	if (!ConfValid(g_strConfigPath, m_strTable))
	{
		SetErrorMsg("csv config valid fail");
		return false;
	}
	return true;
}

bool ReloadConfigCommand::Trigger()
{
	if ( !ConfReload(g_strConfigPath, m_strTable) )
	{
		SetErrorMsg( "Reload Config fail" );
		return false;
	}
	return true;
}

bool ReloadConfigCommand::Complete()
{
	return true;
}

// Announcement impl //////////////

bool AnnouncementCommand::Validate()
{
	RETURN_FALSE_IF_TARGET_IS_PASSPORT;
	RETURN_FALSE_IF_TARGET_IS_ROLE;
	RETURN_FALSE_IF_PARAMS_SIZE_INVALID( ==, 5 );
	
	m_Id = GET_CMD_PARAM_INT(0);	
	m_cmd = GET_CMD_PARAM_INT(1);
	m_para1 = GET_CMD_PARAM_STRING(2);
	m_strTitle = GET_CMD_PARAM_STRING(3);
	m_strContent = GET_CMD_PARAM_STRING(4);
	if (m_Id <= 0 || m_Id > 10)
	{
		SetErrorMsg(format_error("Announcement id is unvalid %u", m_Id));
		return false;
	}
	return true;
}

bool AnnouncementCommand::Trigger()
{
	std::string desc = "";
	if (m_strContent != "0")
	{
		desc = CUtil::Uint32ToString(m_cmd);
		desc += "|";
		desc += m_para1;
		desc += "\n";
		desc += m_strTitle;
		desc += "\n";
		desc += m_strContent;
	}
	sWorld->GetCAnnouncement()->HandlerUpdateContent(desc.c_str(), m_Id);
	return true;
}

bool AnnouncementCommand::Complete()
{
	return true;
}

///////////////////////////////////////
// set_role_auth [auth]
bool SetRoleAuth::Validate()
{
	RETURN_FALSE_IF_TARGET_IS_PASSPORT;
	RETURN_FALSE_IF_TARGET_IS_SERVER;
	RETURN_FALSE_IF_PARAMS_SIZE_INVALID( ==, 1 );

	gm = GET_CMD_PARAM_INT(0);

	return true;
}

bool SetRoleAuth::Trigger()
{
	CRole *pCRole;
	CRole temp(NULL);
	uint8 isOnline = 1;
	pCRole = sWorld->FindUserRole(m_dwTargetId);
	if (pCRole == NULL)
	{
		isOnline = 0;
		pCRole = &temp;
		if (!pCRole->LoadDataForDuration(m_dwTargetId))
		{
			SetErrorMsg( format_error("role init fail roleid %u", m_dwTargetId) );
			return false;
		}
	}
	pCRole->SetbyGm(gm);
	if (isOnline)
	{
		pCRole->SendProUpdate();
	}
	else
	{
		pCRole->SaveDataForDuration();
		pCRole->statisticsRole();
	}
	return true;
}

bool SetRoleAuth::Complete()
{
	return true;
}
/////////////////////////

bool SetRoleAuth2::Validate()
{
	RETURN_FALSE_IF_TARGET_IS_PASSPORT;
	RETURN_FALSE_IF_TARGET_IS_SERVER;
	RETURN_FALSE_IF_PARAMS_SIZE_INVALID( ==, 1 );

	limit = GET_CMD_PARAM_INT(0);

	return true;
}

bool SetRoleAuth2::Trigger()
{
	CDBCtrl::setLimitRolelogin(m_dwTargetId,limit);
	return true;
}

bool SetRoleAuth2::Complete()
{
	return true;
}

///////////////////////////////////
// active_exp_add [time format] [title] [content] [add multi] [type] [mission]

bool ActiveExpAdd::Validate()
{
	RETURN_FALSE_IF_TARGET_IS_PASSPORT;
	RETURN_FALSE_IF_TARGET_IS_ROLE;
	RETURN_FALSE_IF_PARAMS_SIZE_INVALID( ==, 3 + 3);

	uint32 Choose = GET_CMD_PARAM_INT(4);

	if (Choose == 0)
	{
	}
	else if (Choose == 1)
	{
	}
	else
	{
		SetErrorMsg( "expadd type para unvalid");
		return false;
	}

	return true;
}

bool ActiveExpAdd::Trigger()
{
	std::string error = "";
	CActiveCtrl::AddActive(EXP_ADD_ACTIVE_ID, m_strParams, error, m_dwStartTime, m_dwEndTime);
	if (error != "")
	{
		SetErrorMsg(error);
		return false;
	}
	return true;
}

bool ActiveExpAdd::Complete()
{
	std::string error;
	CActiveCtrl::DelActive(EXP_ADD_ACTIVE_ID, error);
	if (error != "")
	{
		SetErrorMsg(error);
		return false;
	}
	return true;
}

///////////////////////////////////
// active_coin_add [time format] [title] [content] [add multi] [type] [mission]

bool ActiveCoinAdd::Validate()
{
	RETURN_FALSE_IF_TARGET_IS_PASSPORT;
	RETURN_FALSE_IF_TARGET_IS_ROLE;
	RETURN_FALSE_IF_PARAMS_SIZE_INVALID( ==, 3 + 3);

	uint32 Choose = GET_CMD_PARAM_INT(4);

	if (Choose == 0)
	{
	}
	else if (Choose == 1)
	{
	}
	else
	{
		SetErrorMsg( "expadd type para unvalid");
		return false;
	}

	return true;
}

bool ActiveCoinAdd::Trigger()
{
	std::string error = "";
	CActiveCtrl::AddActive(COIN_ADD_ACTIVE_ID, m_strParams, error, m_dwStartTime, m_dwEndTime);
	if (error != "")
	{
		SetErrorMsg(error);
		return false;
	}
	return true;
}

bool ActiveCoinAdd::Complete()
{
	std::string error;
	CActiveCtrl::DelActive(COIN_ADD_ACTIVE_ID, error);
	if (error != "")
	{
		SetErrorMsg(error);
		return false;
	}
	return true;
}

///////////////////////////////////
// active_obj_drop_add [time format] [title] [content] [add multi] [type] [mission]

bool ActiveObjDrop::Validate()
{
	RETURN_FALSE_IF_TARGET_IS_PASSPORT;
	RETURN_FALSE_IF_TARGET_IS_ROLE;
	RETURN_FALSE_IF_PARAMS_SIZE_INVALID( ==, 3 + 3);

	uint32 Choose = GET_CMD_PARAM_INT(4);

	if (Choose == 0)
	{
	}
	else if (Choose == 1)
	{
	}
	else
	{
		SetErrorMsg( "expadd type para unvalid");
		return false;
	}

	return true;
}

bool ActiveObjDrop::Trigger()
{
	std::string error = "";
	CActiveCtrl::AddActive(OBJ_DROP_ACTIVE_ID, m_strParams, error, m_dwStartTime, m_dwEndTime);
	if (error != "")
	{
		SetErrorMsg(error);
		return false;
	}
	return true;
}

bool ActiveObjDrop::Complete()
{
	std::string error;
	CActiveCtrl::DelActive(OBJ_DROP_ACTIVE_ID, error);
	if (error != "")
	{
		SetErrorMsg(error);
		return false;
	}
	return true;
}

///////////////////////////////////
// active_boss_rank [time format] [title] [content] [content] [prize] [title] [content]

bool ActiveBossRank::Validate()
{
	RETURN_FALSE_IF_TARGET_IS_PASSPORT;
	RETURN_FALSE_IF_TARGET_IS_ROLE;
	RETURN_FALSE_IF_PARAMS_SIZE_INVALID( ==, 3 + 4);

	return true;
}

bool ActiveBossRank::Trigger()
{
	std::string error = "";
	CActiveCtrl::AddActive(FRIEND_BOSS_RANK_ACTIVE_ID, m_strParams, error, m_dwStartTime, m_dwEndTime);
	if (error != "")
	{
		SetErrorMsg(error);
		return false;
	}
	return true;
}

bool ActiveBossRank::Complete()
{
	std::string error;
	CActiveCtrl::DelActive(FRIEND_BOSS_RANK_ACTIVE_ID, error);
	if (error != "")
	{
		SetErrorMsg(error);
		return false;
	}
	return true;
}

///////////////////////////////////
// setserver [status] [is_new] [can_login] [name] [domain] [res_url]

bool SetServer::Validate()
{
	RETURN_FALSE_IF_TARGET_IS_PASSPORT;
	RETURN_FALSE_IF_TARGET_IS_ROLE;
	RETURN_FALSE_IF_PARAMS_SIZE_INVALID( ==, 6);
	
	m_areaid = GetTargetId();
	m_status = GET_CMD_PARAM_INT(0); 
	m_new = GET_CMD_PARAM_INT(1); 
	m_canlogin = GET_CMD_PARAM_INT(2); 
	m_name = GET_CMD_PARAM_STRING(3); 
	m_domain = GET_CMD_PARAM_STRING(4); 
	m_resurl = GET_CMD_PARAM_STRING(5); 
	return true;
}

bool SetServer::Trigger()
{
	std::map<uint32, SServerData> &Server = sWorld->GetServerList();
	if (Server.find(m_areaid) != Server.end())
	{
		if (m_canlogin != 255)
			Server[m_areaid].canlogin = m_canlogin;
		if (m_status != 255)
			Server[m_areaid].status = m_status;
		if (m_new != 255)
			Server[m_areaid].isnew = m_new;
		if (m_name != "" && m_name != "0")
			Server[m_areaid].servername = m_name;
		if (m_domain != "" && m_domain != "0")
			Server[m_areaid].domain = m_domain;
		if (m_resurl != "" && m_resurl != "0")
			Server[m_areaid].resurl = m_resurl;
//		IME_ERROR("m_name %s", m_name.c_str());
		return true;
	}
	else
	{
		SetErrorMsg(format_error("area_id is not unvalid %u", m_areaid));
		return false;
	}
	return true;
}

bool SetServer::Complete()
{
	return true;
}

///////////////////////////////////
// active_charge [time format] [title] [content] [activeindex] [content]

bool ActiveCharge::Validate()
{
	RETURN_FALSE_IF_TARGET_IS_PASSPORT;
	RETURN_FALSE_IF_TARGET_IS_ROLE;
	RETURN_FALSE_IF_PARAMS_SIZE_INVALID( ==, 5);
	
	m_activeIndex = GET_CMD_PARAM_INT(3);
	m_content = GET_CMD_PARAM_STRING(4); 
	if (m_activeIndex != CHARGE_SINGLE_ACTIVE_ID && m_activeIndex != CHARGE_ACCUMULATE_ACTIVE_ID && m_activeIndex != CHARGE_CYCLE_ACTIVE_ID)
	{
		SetErrorMsg(format_error("activeindex is unvalid %u", m_activeIndex));
		return false;
	}
	return true;
}

bool ActiveCharge::Trigger()
{
	std::string error = "";
	if (m_activeIndex == CHARGE_SINGLE_ACTIVE_ID)
	{
		CActiveCtrl::AddActive(CHARGE_SINGLE_ACTIVE_ID, m_strParams, error, m_dwStartTime, m_dwEndTime);
	}
	else if (m_activeIndex == CHARGE_ACCUMULATE_ACTIVE_ID)
	{
		CActiveCtrl::AddActive(CHARGE_ACCUMULATE_ACTIVE_ID, m_strParams, error, m_dwStartTime, m_dwEndTime);
	}
	else
	{
		CActiveCtrl::AddActive(CHARGE_CYCLE_ACTIVE_ID, m_strParams, error, m_dwStartTime, m_dwEndTime);
	}
	if (error != "")
	{
		SetErrorMsg(error);
		return false;
	}
	return true;
}

bool ActiveCharge::Complete()
{
	std::string error;
	if (m_activeIndex == CHARGE_SINGLE_ACTIVE_ID)
	{
		CActiveCtrl::DelActive(CHARGE_SINGLE_ACTIVE_ID, error);
	}
	else if (m_activeIndex == CHARGE_ACCUMULATE_ACTIVE_ID)
	{
		CActiveCtrl::DelActive(CHARGE_ACCUMULATE_ACTIVE_ID, error);
	}
	else
	{
		CActiveCtrl::DelActive(CHARGE_CYCLE_ACTIVE_ID, error);
	}

	if (error != "")
	{
		SetErrorMsg(error);
		return false;
	}
	return true;
}

///////////////////////////////////
// extralogin [time format] [title] [content] [content]

bool ActiveExtraLogin::Validate()
{
	RETURN_FALSE_IF_TARGET_IS_PASSPORT;
	RETURN_FALSE_IF_TARGET_IS_ROLE;
	RETURN_FALSE_IF_PARAMS_SIZE_INVALID( ==, 4);
	
//	m_content = GET_CMD_PARAM_STRING(3); 
	return true;
}

bool ActiveExtraLogin::Trigger()
{
	std::string error = "";
	CActiveCtrl::AddActive(LOGIN_EXTRA_ACTIVE_ID, m_strParams, error, m_dwStartTime, m_dwEndTime);
	if (error != "")
	{
		SetErrorMsg(error);
		return false;
	}
	return true;
}

bool ActiveExtraLogin::Complete()
{
	std::string error;
	CActiveCtrl::DelActive(LOGIN_EXTRA_ACTIVE_ID, error);

	if (error != "")
	{
		SetErrorMsg(error);
		return false;
	}
	return true;
}

///////////////////////////////////
// fund [time format] [title] [content] [content] [mailtitle] [mailcontent]

bool ActiveFund::Validate()
{
	RETURN_FALSE_IF_TARGET_IS_PASSPORT;
	RETURN_FALSE_IF_TARGET_IS_ROLE;
	RETURN_FALSE_IF_PARAMS_SIZE_INVALID( ==, 6);
	
//	m_content = GET_CMD_PARAM_STRING(3); 
	return true;
}

bool ActiveFund::Trigger()
{
	std::string error = "";
	CActiveCtrl::AddActive(FUND_ACTIVE_ID, m_strParams, error, m_dwStartTime, m_dwEndTime);
	if (error != "")
	{
		SetErrorMsg(error);
		return false;
	}
	return true;
}

bool ActiveFund::Complete()
{
	std::string error;
	CActiveCtrl::DelActive(FUND_ACTIVE_ID, error);

	if (error != "")
	{
		SetErrorMsg(error);
		return false;
	}
	return true;
}

///////////////////////////////////
// task [time format] [title] [content] [index] [content]

bool ActiveTask::Validate()
{
	RETURN_FALSE_IF_TARGET_IS_PASSPORT;
	RETURN_FALSE_IF_TARGET_IS_ROLE;
	RETURN_FALSE_IF_PARAMS_SIZE_INVALID( ==, 5);
	
	m_activeIndex = GET_CMD_PARAM_INT(3); 
	if (!(m_activeIndex >= TASK1_ACTIVE_ID && m_activeIndex <= TASK7_ACTIVE_ID))
	{
		SetErrorMsg(format_error("active index para is unvalid %u", m_activeIndex));
		return false;
	}
	return true;
}

bool ActiveTask::Trigger()
{
	std::string error = "";
	CActiveCtrl::AddActive(m_activeIndex, m_strParams, error, m_dwStartTime, m_dwEndTime);
	if (error != "")
	{
		SetErrorMsg(error);
		return false;
	}
	return true;
}

bool ActiveTask::Complete()
{
	std::string error;
	CActiveCtrl::DelActive(m_activeIndex, error);

	if (error != "")
	{
		SetErrorMsg(error);
		return false;
	}
	return true;
}

///////////////////////////////////
// store [content]

bool StoreLoad::Validate()
{
	RETURN_FALSE_IF_TARGET_IS_PASSPORT;
	RETURN_FALSE_IF_TARGET_IS_ROLE;
	RETURN_FALSE_IF_PARAMS_SIZE_INVALID( ==, 1);
	
	std::string content = GET_CMD_PARAM_STRING(0);
	CCsvReader csv;
	if (!csv.Init(content))
	{
		SetErrorMsg("store csv init fail");
		return false;
	}
	for (int i = 1; i < csv.GetLineCount(); i++)
	{
		int j = 0;
		STC_CONF_STORE info;
		READER_CSV_INT(info.dwStoreIndex);
		READER_CSV_INT(info.byStoreChannel);
		READER_CSV_INT(info.dwIndex);
		READER_CSV_INT(info.byType);
		READER_CSV_INT(info.byObjType);
		READER_CSV_INT(info.dwObjNumber);
		READER_CSV_INT(info.byIsBuy);
		READER_CSV_INT(info.dwViewOrder);
		READER_CSV_STR(info.strContent);
		READER_CSV_INT(info.dwLimitNumber);
		READER_CSV_INT(info.dwRoleLimitNumber);
		READER_CSV_INT(info.dwIndexNumber);
		READER_CSV_INT(info.dwSendPrize);
		READER_CSV_INT(info.dwViewPrize);
		READER_CSV_INT(info.dwMaxBuy);
		READER_CSV_INT(info.dwKind);
		READER_CSV_INT(info.byGoodsType);
		IS_READER_CSV_ERROR;
		m_vec.push_back(info);

		if (info.byGoodsType == E_OBJ_SHOP)
		{
			if (CConfGoods::Find(info.dwIndex) == NULL)
			{
				SetErrorMsg(format_error("store index %u can not find item index %u", info.dwStoreIndex, info.dwIndex));
				return false;
			}
		}
		else if (info.byGoodsType == E_OBJ_CARD)
		{
			if (CConfCardBase::Find(info.dwIndex) == NULL)
			{
				SetErrorMsg(format_error("store index %u can not find card index %u", info.dwStoreIndex, info.dwIndex));
				return false;
			}
		}
		else if (info.byGoodsType == 0)
		{
		}
		else
		{
			SetErrorMsg(format_error("store index %u can not find type %u", info.dwStoreIndex, info.dwIndex));
			return false;
		}
	}
	return true;
}

bool StoreLoad::Trigger()
{
	std::vector<uint32_t> vectmp = sWorld->GetServerId();
//	CConfStore::MapData *p = CConfStore::GetVal();
//	CConfStore::MapData::iterator it;
	for (int j = 0; j < vectmp.size(); j++)
	{
		std::map<uint32, STC_GOODS_INFO> mapGoods;
//		for (it = p->begin(); it != p->end(); it++)
//		{
		if (!CDBCtrl::ReloadStoreData(vectmp[j], m_vec))
		{
			SetErrorMsg("reload store data into db fail");
			return false;
		}	

		for (int i = 0; i < m_vec.size(); i++)
		{
			std::string strTitle = m_vec[i].strContent;
			std::string strContent = m_vec[i].strContent;
			if (m_vec[i].byObjType == STORE_OBJ_COIN || m_vec[i].byObjType == STORE_OBJ_DOLLAR || m_vec[i].byObjType == STORE_OBJ_PRESTIGE)
			{
				STC_CONF_GOODS *pItemConf = CConfGoods::Find(m_vec[i].dwIndex);
				if (pItemConf != NULL)
				{
					strTitle = pItemConf->strTitle;
					strContent = pItemConf->strContent;
				}
			}
			STC_GOODS_INFO goods;
			goods.dwGoodsId = m_vec[i].dwStoreIndex;
			goods.byShopType = m_vec[i].byStoreChannel;
			goods.dwBuyTypeId = 2;
			goods.dwBuyContentId = m_vec[i].dwIndex;
			goods.dwBuyCount = m_vec[i].dwIndexNumber;
			goods.dwCostTypeId = m_vec[i].byObjType;
			goods.dwCostContentId = CURRENCY_CNY;
			goods.dwCostCount = m_vec[i].dwObjNumber;
			goods.dwCostCountOld = m_vec[i].dwViewPrize;
			goods.byStatus = m_vec[i].byIsBuy;
			goods.dwLimitDay = m_vec[i].dwLimitNumber;
			goods.dwSortIdx = m_vec[i].dwViewOrder;
			goods.dwIconId = 0;
			goods.strName = strTitle;
			goods.strDescription = strContent;
			goods.dwBuyLimitOnce = m_vec[i].dwMaxBuy;
			goods.dwKind = m_vec[i].dwKind;
			mapGoods[m_vec[i].dwStoreIndex] = goods;
		}

		CenterDBCtrl::UpdateGoodsInfoOfGameServer(mapGoods, vectmp[j]);
	}

	if (!CConfStore::Init())
	{
		SetErrorMsg("CConfStore init fail");
		return false;
	}

	return true;
}

bool StoreLoad::Complete()
{
	return true;
}

///////////////////////////////////
// ForceCancel [index]

bool ForceCancel::Validate()
{
	RETURN_FALSE_IF_TARGET_IS_PASSPORT;
	RETURN_FALSE_IF_TARGET_IS_ROLE;
	RETURN_FALSE_IF_PARAMS_SIZE_INVALID( ==, 1);
	
	m_activeIndex = GET_CMD_PARAM_INT(0); 
	return true;
}

bool ForceCancel::Trigger()
{
	std::string error = "";
	CActiveCtrl::DelActive(m_activeIndex, error);
	if (error != "")
	{
		SetErrorMsg(error);
		return false;
	}
	return true;
}

bool ForceCancel::Complete()
{
	return true;
}

///////////////////////////////////
// WhiteList [useridlist]

bool WhiteList::Validate()
{
	RETURN_FALSE_IF_TARGET_IS_PASSPORT;
	RETURN_FALSE_IF_TARGET_IS_ROLE;
	RETURN_FALSE_IF_PARAMS_SIZE_INVALID( ==, 1);
	
	std::string tmp = GET_CMD_PARAM_STRING(0); 
	CUtil::StrSplit(tmp, ",", m_vecUserid);
	return true;
}

bool WhiteList::Trigger()
{
	sWorld->SetWhiteList(m_vecUserid);
	return true;
}

bool WhiteList::Complete()
{
	return true;
}

///////////////////////////////////
// noticectrl [choose] [notice_type] [content]

bool NoticeCtrl::Validate()
{
	RETURN_FALSE_IF_TARGET_IS_PASSPORT;
	RETURN_FALSE_IF_TARGET_IS_ROLE;
	RETURN_FALSE_IF_PARAMS_SIZE_INVALID( ==, 3);
	
	m_byChoose = GET_CMD_PARAM_INT(0);
	m_byChatType = GET_CMD_PARAM_INT(1);
	m_strContent = GET_CMD_PARAM_STRING(2);
	return true;
}

bool NoticeCtrl::Trigger()
{
	if (m_byChoose == 0)
	{
		SWorldChat tmp;
		tmp.choose = m_byChatType;
		tmp.userid = 0;
		tmp.username = GlobalConfig::NoticeTypeName;
		tmp.content = m_strContent;
		tmp.viplv = 0;
		WorldPacket info;
		sWorld->PkgWorldChat(tmp, info);
//		sWorld->SendPkgToAll(info);	
		sSessCtrl->SendPkgToAll(info);	
	}
	else
	{
		sWorld->GetCNotice()->DiyNotice(m_strContent);
	}
	return true;
}

bool NoticeCtrl::Complete()
{
	return true;
}

/////////////////// Util Function //////////////////
/* 
static bool ReadThresholdAndReward( ThresholdAndReward& tr, std::string& str, std::string& err )
{
	std::vector<std::string> vLevel;
	CUtil::StrSplit( str, ",", vLevel );
	if ( vLevel.size() == 0 )
	{
		err = "Format Error( vLevel.size() == 0 )";
		return false;
	}
	for ( std::vector<std::string>::iterator it = vLevel.begin();
			it != vLevel.end(); ++it )
	{
		std::vector<std::string> vReqAndRwd;
		CUtil::StrSplit( *it, "-", vReqAndRwd );

		uint32 dwReq, dwItemId, dwQuantity;

		if ( vReqAndRwd.size() == 2 )
		{
			dwReq 		= atoi( vReqAndRwd[0].c_str() );
			dwItemId	= atoi( vReqAndRwd[1].c_str() );
			dwQuantity	= 1;
		}
		else if ( vReqAndRwd.size() == 3 )
		{
			dwReq 		= atoi( vReqAndRwd[0].c_str() );
			dwItemId	= atoi( vReqAndRwd[1].c_str() );
			dwQuantity	= atoi( vReqAndRwd[2].c_str() );
		}
		else
		{
			err = "Format Error( Not ?-?-? or ?-? )";
			return false;
		}

		if ( GET_CONFIG_STC( ItemConfig, dwItemId ) == NULL )
		{
			char buf[32];
			sprintf( buf, "Item Not Found, id=%d", dwItemId );
			err = buf;
			return false;
		}

		STC_ACTIVITY_REWARD rwd;
		rwd.dwItemId	= dwItemId;
		rwd.dwQuantity	= dwQuantity;

		if ( tr.insert( std::make_pair( dwReq, rwd ) ).second == false )
		{
			err = "Require Value Duplicated";
			return false;
		}
	}

	return true;
}

////////////////////////////
//// set_server_state [state]
bool SetServerStateCommand::Validate()
{
	RETURN_FALSE_IF_TARGET_IS_PASSPORT;
	RETURN_FALSE_IF_TARGET_IS_ROLE;
	RETURN_FALSE_IF_PARAMS_SIZE_INVALID( ==, 1 );

	m_byState = GET_CMD_PARAM_INT( 0 );

	if ( m_byState >= E_SERVER_STATUS_COUNT + 1 ) // +shutdown
	{
		SetErrorMsg( "Invalid State" );
		return false;
	}

	return true;
}

bool SetServerStateCommand::Trigger()
{
	if ( m_byState == 0 )
	{
		sWorld->SetCanLogin( false );
	}
	else if ( m_byState == 4 )
	{
		sWorld->SetCanLogin( false );
		sWorld->SetStatus( m_byState - 1 );
	}
	else
	{
		sWorld->SetCanLogin( true );
		sWorld->SetStatus( m_byState - 1 );
	}

	return true;
}

bool SetServerStateCommand::Complete()
{
	return true;
}

/////////////////////////////
///// set_login_strategy [strategy_id]
bool SetLoginStrategyCommand::Validate()
{
	RETURN_FALSE_IF_TARGET_IS_PASSPORT;
	RETURN_FALSE_IF_TARGET_IS_ROLE;
	RETURN_FALSE_IF_PARAMS_SIZE_INVALID( ==, 1 );
	m_dwStrategyId = GET_CMD_PARAM_INT( 0 );

	STC_LOGIN_STRATEGY strategy;

	if ( !CenterDBCtrl::GetLoginStrategy( m_dwStrategyId, strategy ) )
	{
		SetErrorMsg( "Strategy Not Exist" );
		return false;
	}

	return true;
}

bool SetLoginStrategyCommand::Trigger()
{
	sWorld->SetLoginStrategy( m_dwStrategyId );
	return true;
}

bool SetLoginStrategyCommand::Complete()
{
	return true;
}

/////// broadcast impl //////////
// broadcast content
bool BroadcastCommand::Validate()
{
	RETURN_FALSE_IF_TARGET_IS_PASSPORT;
	RETURN_FALSE_IF_TARGET_IS_ROLE;
	RETURN_FALSE_IF_PARAMS_SIZE_INVALID( ==, 2 );

	m_strContent 	= GET_CMD_PARAM_STRING( 0 );
	m_dwInterval	= GET_CMD_PARAM_INT( 1 );
	return true;
}

bool BroadcastCommand::Trigger()
{
	m_dwLastUpdateTime = 0;
	return true;
}

bool BroadcastCommand::Complete()
{
	return true;
}

void BroadcastCommand::Tick()
{
	uint32 t = time(NULL);
	if ( t - m_dwLastUpdateTime > m_dwInterval * 60 )
	{
		ChatSystem::GetSystem().SystemBroadcast( m_strContent );

		m_dwLastUpdateTime = t;
	}
}

////// set server name impl ///////
// set_server_name [name]
bool SetServerNameCommand::Validate()
{
	RETURN_FALSE_IF_TARGET_IS_PASSPORT;
	RETURN_FALSE_IF_TARGET_IS_ROLE;
	RETURN_FALSE_IF_PARAMS_SIZE_INVALID( ==, 1 );

	m_strServerName = vParam[0];
	return true;
}

bool SetServerNameCommand::Trigger()
{
	sWorld->SetServerName( m_strServerName );
	return true;
}

bool SetServerNameCommand::Complete()
{
	return true;
}

////// set res server addr impl /////
// set_res_server_addr [addr]
bool SetResServerAddrCommand::Validate()
{
	RETURN_FALSE_IF_TARGET_IS_PASSPORT;
	RETURN_FALSE_IF_TARGET_IS_ROLE;
	RETURN_FALSE_IF_PARAMS_SIZE_INVALID( ==, 1 );

	m_resServerAddr = GET_CMD_PARAM_STRING(0);
	return true;
}

bool SetResServerAddrCommand::Trigger()
{
	sWorld->SetResServerIp( m_resServerAddr );
	return true;
}

bool SetResServerAddrCommand::Complete()
{
	return true;
}

////// set res version impl ///////
// set_res_version [res_version]

bool SetResVersionCommand::Validate()
{
	RETURN_FALSE_IF_TARGET_IS_PASSPORT;
	RETURN_FALSE_IF_TARGET_IS_ROLE;
	RETURN_FALSE_IF_PARAMS_SIZE_INVALID( ==, 1 );

	m_strResVersion = GET_CMD_PARAM_STRING(0);
	return true;
}

bool SetResVersionCommand::Trigger()
{
	if ( !sWorld->DownloadRes( m_strResVersion.c_str() ) )
	{
		SetErrorMsg( sWorld->GetResDownloadErrorMsg() );
		return false;
	}
	return true;
}

bool SetResVersionCommand::Complete()
{
	return true;
}

////// update shop impl ///////
// update_shop [csv_data]
bool UpdateShopCommand::Validate()
{
	RETURN_FALSE_IF_TARGET_IS_PASSPORT;
	RETURN_FALSE_IF_TARGET_IS_ROLE;

	if ( m_strParams.size() == 0 || m_strParams[0] != '1' )
	{
		SetErrorMsg( format_error( "Invalid CSV" ) );
		return false;
	}

	if ( !m_shopConfig.LoadRawData( m_strParams.c_str() ) )
	{
		SetErrorMsg( format_error( "Config Parse Error" ) );
		return false;
	}

	return true;
}

bool UpdateShopCommand::Trigger()
{
	if ( !StoreSystem::GetSystem().UpdateGoodsInfo( m_shopConfig ) )
	{
		SetErrorMsg( "Update Error" );
		return false;
	}
	return true;
}

bool UpdateShopCommand::Complete()
{
	return true;
}

///////////////////////////
// activity_exp_bonus [small_pic_name] [big_pic_name] [rate]
bool ActivityExpBonusCommand::Validate()
{
	RETURN_FALSE_IF_TARGET_IS_ROLE;
	RETURN_FALSE_IF_TARGET_IS_PASSPORT;
	RETURE_FALSE_IF_NO_END_TIME;
	RETURN_FALSE_IF_PARAMS_SIZE_INVALID( ==, 4 );

	m_strSmallPicName	= GET_CMD_PARAM_STRING( 0 );
	m_strBigPicName		= GET_CMD_PARAM_STRING( 1 );
	m_dwMinutesAhead	= GET_CMD_PARAM_INT( 2 );
	m_wBonusRate 		= GET_CMD_PARAM_INT( 3 );

	ActivitySystem::GetSystem().CreateActivityPending(
			m_dwAutoId,
			m_strSmallPicName.c_str(),
			m_strBigPicName.c_str(),
			m_dwStartTime - m_dwMinutesAhead * 60,
			m_dwStartTime );

	return true;
}

bool ActivityExpBonusCommand::Trigger()
{
	if ( !ActivitySystem::GetSystem().CreateExpBonusActivity(
			m_dwAutoId,
			m_strSmallPicName.c_str(),
			m_strBigPicName.c_str(),
			m_wBonusRate ) )
	{
		SetErrorMsg( "Create Activity Error" );
		return false;
	}
	return true;
}

bool ActivityExpBonusCommand::Complete()
{
	ActivitySystem::GetSystem().DeleteActivityPending( m_dwAutoId );
	if ( !ActivitySystem::GetSystem().DeleteActivity( m_dwAutoId ) )
	{
		SetErrorMsg( "Delete Activity Error" );
		return false;
	}
	return true;
}

//////////////////////////////////////
// activity_diamond [small_pic_name] [big_pic_name] [is_charge] [request_str]

bool ActivityDiamondCommand::Validate()
{
	RETURN_FALSE_IF_TARGET_IS_ROLE;
	RETURN_FALSE_IF_TARGET_IS_PASSPORT;
	RETURE_FALSE_IF_NO_END_TIME;
	RETURN_FALSE_IF_PARAMS_SIZE_INVALID( ==, 5 );

	m_strSmallPicName	= GET_CMD_PARAM_STRING( 0 );
	m_strBigPicName		= GET_CMD_PARAM_STRING( 1 );
	m_dwMinutesAhead	= GET_CMD_PARAM_INT( 2 );
	m_bIsCharge			= GET_CMD_PARAM_INT( 3 );
	std::string str		= GET_CMD_PARAM_STRING( 4 );

	if ( !ReadThresholdAndReward( m_mapRequestAndReward, str, m_strErrorMsg ) )
	{
		return false;
	}

	ActivitySystem::GetSystem().CreateActivityPending(
				m_dwAutoId,
				m_strSmallPicName.c_str(),
				m_strBigPicName.c_str(),
				m_dwStartTime - m_dwMinutesAhead * 60,
				m_dwStartTime );

	return true;
}

bool ActivityDiamondCommand::Trigger()
{
	if ( !ActivitySystem::GetSystem().CreateChargeOrSpendActivity(
			m_dwAutoId,
			m_strSmallPicName.c_str(),
			m_strBigPicName.c_str(),
			m_bIsCharge,
			m_mapRequestAndReward ) )
	{
		SetErrorMsg( "Create Activity Error" );
		return false;
	}
	return true;
}

bool ActivityDiamondCommand::Complete()
{
	ActivitySystem::GetSystem().DeleteActivityPending( m_dwAutoId );

	if ( !ActivitySystem::GetSystem().DeleteActivity( m_dwAutoId ) )
	{
		SetErrorMsg( "Delete Activity Error" );
		return false;
	}
	return true;
}

/////////////////////////////////////
// activity_exchange [small_pic_name] [big_pic_name] [use_item_id] [use_item_cnt] [get_item_id] [get_item_cnt]

bool ActivityExchangeCommand::Validate()
{
	RETURN_FALSE_IF_TARGET_IS_ROLE;
	RETURN_FALSE_IF_TARGET_IS_PASSPORT;
	RETURE_FALSE_IF_NO_END_TIME;
	RETURN_FALSE_IF_PARAMS_SIZE_INVALID( ==, 7 );

	m_strSmallPicName	= GET_CMD_PARAM_STRING( 0 );
	m_strBigPicName		= GET_CMD_PARAM_STRING( 1 );
	m_dwMinutesAhead	= GET_CMD_PARAM_INT( 2 );
	m_dwUseItemId		= GET_CMD_PARAM_INT( 3 );
	m_dwUseItemCnt		= GET_CMD_PARAM_INT( 4 );
	m_dwGetItemId		= GET_CMD_PARAM_INT( 5 );
	m_dwGetItemCnt		= GET_CMD_PARAM_INT( 6 );

	ItemConfig::STC_ITEM_CONFIG* pConfig = GET_CONFIG_STC( ItemConfig, m_dwUseItemId );
	if ( pConfig == NULL )
	{
		SetErrorMsg( format_error( "UseItemId Not Found,id=%d", m_dwUseItemId ) );
		return false;
	}
	pConfig = GET_CONFIG_STC( ItemConfig, m_dwGetItemId );
	if ( pConfig == NULL )
	{
		SetErrorMsg( format_error( "GetItemId Not Found, id=%d", m_dwGetItemId ) );
		return false;
	}

	ActivitySystem::GetSystem().CreateActivityPending(
				m_dwAutoId,
				m_strSmallPicName.c_str(),
				m_strBigPicName.c_str(),
				m_dwStartTime - m_dwMinutesAhead * 60,
				m_dwStartTime );

	return true;
}

bool ActivityExchangeCommand::Trigger()
{
	if ( !ActivitySystem::GetSystem().CreateExchangeActivity(
			m_dwAutoId,
			m_strSmallPicName.c_str(),
			m_strBigPicName.c_str(),
			m_dwUseItemId,
			m_dwUseItemCnt,
			m_dwGetItemId,
			m_dwGetItemCnt ) )
	{
		SetErrorMsg( "Create Activity Error" );
		return false;
	}
	return true;
}

bool ActivityExchangeCommand::Complete()
{
	ActivitySystem::GetSystem().DeleteActivityPending( m_dwAutoId );

	if ( !ActivitySystem::GetSystem().DeleteActivity( m_dwAutoId ) )
	{
		SetErrorMsg( "Delete Activity Error" );
		return false;
	}
	return true;
}

///////////////////////////////////////////
// activity_drop [small_pic_name] [big_pic_name] [drop_item_id] [drop_prop]
bool ActivitySpecialDropCommand::Validate()
{
	RETURN_FALSE_IF_TARGET_IS_ROLE;
	RETURN_FALSE_IF_TARGET_IS_PASSPORT;
	RETURE_FALSE_IF_NO_END_TIME;
	RETURN_FALSE_IF_PARAMS_SIZE_INVALID( ==, 6 );

	m_strSmallPicName	= GET_CMD_PARAM_STRING( 0 );
	m_strBigPicName		= GET_CMD_PARAM_STRING( 1 );
	m_dwMinutesAhead	= GET_CMD_PARAM_INT( 2 );
	m_dwDropItemId		= GET_CMD_PARAM_INT( 3 );
	m_wDropProp			= GET_CMD_PARAM_INT( 4 );
	m_byMustElite		= GET_CMD_PARAM_INT( 5 );

	ItemConfig::STC_ITEM_CONFIG* pConfig = GET_CONFIG_STC( ItemConfig, m_dwDropItemId );
	if ( pConfig == NULL )
	{
		SetErrorMsg( format_error("DropItemId Not Found, id=%d", m_dwDropItemId ) );
		return false;
	}

	ActivitySystem::GetSystem().CreateActivityPending(
				m_dwAutoId,
				m_strSmallPicName.c_str(),
				m_strBigPicName.c_str(),
				m_dwStartTime - m_dwMinutesAhead * 60,
				m_dwStartTime );

	return true;
}

bool ActivitySpecialDropCommand::Trigger()
{
	if ( !ActivitySystem::GetSystem().CreateSpecialDropActivity(
			m_dwAutoId,
			m_strSmallPicName.c_str(),
			m_strBigPicName.c_str(),
			m_dwDropItemId,
			m_wDropProp,
			m_byMustElite ) )
	{
		SetErrorMsg( "Create Activity Error" );
		return false;
	}
	return true;
}

bool ActivitySpecialDropCommand::Complete()
{
	ActivitySystem::GetSystem().DeleteActivityPending( m_dwAutoId );

	if ( !ActivitySystem::GetSystem().DeleteActivity( m_dwAutoId ) )
	{
		SetErrorMsg( "Delete Activity Error" );
		return false;
	}
	return true;
}

/////////////////////////////////////////////
// activity_lottery [delay_close_day]

bool ActivityLotteryCommand::Validate()
{
	RETURN_FALSE_IF_TARGET_IS_ROLE;
	RETURN_FALSE_IF_TARGET_IS_PASSPORT;
	RETURE_FALSE_IF_NO_END_TIME;
	RETURN_FALSE_IF_PARAMS_SIZE_INVALID( ==, 1 );

	m_dwDelayCloseDay = GET_CMD_PARAM_INT( 0 );
	if ( m_dwDelayCloseDay == 0 )
	{
		SetErrorMsg( "Invalid Param" );
		return false;
	}
	return true;
}

bool ActivityLotteryCommand::Trigger()
{
	if ( !LotterySystem::GetSystem().StartLottery( m_dwStartTime, m_dwEndTime ) )
	{
		SetErrorMsg( "Error When Start Lottery" );
		return false;
	}
	return true;
}

bool ActivityLotteryCommand::Complete()
{
	if ( !LotterySystem::GetSystem().StopLottery( m_dwDelayCloseDay * 24 * 60 * 60 ) )
	{
		SetErrorMsg( "Error When Stop Lottery" );
		return false;
	}
	return true;
}

////////////////////////////////////
// activity_acc_login [format_string]

bool ActivityAccLoginCommand::Validate()
{
	RETURN_FALSE_IF_TARGET_IS_ROLE;
	RETURN_FALSE_IF_TARGET_IS_PASSPORT;
	RETURE_FALSE_IF_NO_END_TIME;
	RETURN_FALSE_IF_PARAMS_SIZE_INVALID( ==, 4 );

	m_strSmallPicName	= GET_CMD_PARAM_STRING( 0 );
	m_strBigPicName		= GET_CMD_PARAM_STRING( 1 );
	m_dwMinutesAhead	= GET_CMD_PARAM_INT( 2 );
	std::string str		= GET_CMD_PARAM_STRING( 3 );

	if ( !ReadThresholdAndReward( m_mapReqAndReward, str, m_strErrorMsg ) )
	{
		return false;
	}

	ActivitySystem::GetSystem().CreateActivityPending(
				m_dwAutoId,
				m_strSmallPicName.c_str(),
				m_strBigPicName.c_str(),
				m_dwStartTime - m_dwMinutesAhead * 60,
				m_dwStartTime );
	return true;
}

bool ActivityAccLoginCommand::Trigger()
{
	if ( !ActivitySystem::GetSystem().CreateAccLoginRewardActivity(
			m_dwAutoId,
			m_strSmallPicName.c_str(),
			m_strBigPicName.c_str(),
			m_mapReqAndReward ) )
	{
		SetErrorMsg( "Create Activity Error" );
		return false;
	}
	return true;
}

bool ActivityAccLoginCommand::Complete()
{
	ActivitySystem::GetSystem().DeleteActivityPending( m_dwAutoId );

	if ( !ActivitySystem::GetSystem().DeleteActivity( m_dwAutoId ) )
	{
		SetErrorMsg( "Delete Activity Error" );
		return false;
	}
	return true;
}

///////////////////////////////////
// activity_stage [format_string]

bool ActivityStageCommand::Validate()
{
	RETURN_FALSE_IF_TARGET_IS_ROLE;
	RETURN_FALSE_IF_TARGET_IS_PASSPORT;
	RETURE_FALSE_IF_NO_END_TIME;
	RETURN_FALSE_IF_PARAMS_SIZE_INVALID( ==, 4 );

	m_strSmallPicName	= GET_CMD_PARAM_STRING( 0 );
	m_strBigPicName		= GET_CMD_PARAM_STRING( 1 );
	m_dwMinutesAhead	= GET_CMD_PARAM_INT( 2 );
	std::string str		= GET_CMD_PARAM_STRING( 3 );

	if ( !ReadThresholdAndReward( m_mapStageAndReward, str, m_strErrorMsg ) )
	{
		return false;
	}

	for ( ThresholdAndReward::iterator it = m_mapStageAndReward.begin();
			it != m_mapStageAndReward.end(); ++it )
	{
		if ( GET_CONFIG_STC( StageConfig, it->first ) == NULL )
		{
			SetErrorMsg( format_error( "Stage Not Found, id=%d", it->first ) );
			return false;
		}
	}

	ActivitySystem::GetSystem().CreateActivityPending(
				m_dwAutoId,
				m_strSmallPicName.c_str(),
				m_strBigPicName.c_str(),
				m_dwStartTime - m_dwMinutesAhead * 60,
				m_dwStartTime );

	return true;
}

bool ActivityStageCommand::Trigger()
{
	if ( !ActivitySystem::GetSystem().CreateStageActivity(
			m_dwAutoId,
			m_strSmallPicName.c_str(),
			m_strBigPicName.c_str(),
			m_mapStageAndReward ) )
	{
		SetErrorMsg( "Create Activity Error" );
		return false;
	}
	return true;
}

bool ActivityStageCommand::Complete()
{
	ActivitySystem::GetSystem().DeleteActivityPending( m_dwAutoId );

	if ( !ActivitySystem::GetSystem().DeleteActivity( m_dwAutoId ) )
	{
		SetErrorMsg( "Delete Activity Error" );
		return false;
	}
	return true;
}

/////////////////////////////////
// activity_arena_rank [format_string]
bool ActivityArenaRankCommand::Validate()
{
	RETURN_FALSE_IF_TARGET_IS_ROLE;
	RETURN_FALSE_IF_TARGET_IS_PASSPORT;
	RETURE_FALSE_IF_NO_END_TIME;
	RETURN_FALSE_IF_PARAMS_SIZE_INVALID( ==, 4 );

	m_strSmallPicName	= GET_CMD_PARAM_STRING( 0 );
	m_strBigPicName		= GET_CMD_PARAM_STRING( 1 );
	m_dwMinutesAhead	= GET_CMD_PARAM_INT( 2 );
	std::string str		= GET_CMD_PARAM_STRING( 3 );

	if ( !ReadThresholdAndReward( m_mapRankAndReward, str, m_strErrorMsg ) )
	{
		return false;
	}

	return true;
}

bool ActivityArenaRankCommand::Trigger()
{
	if ( !ActivitySystem::GetSystem().CreateArenaRankActivity(
			m_dwAutoId,
			m_strSmallPicName.c_str(),
			m_strBigPicName.c_str(),
			m_mapRankAndReward ) )
	{
		SetErrorMsg( "Create Activity Error" );
		return false;
	}
	return true;
}

bool ActivityArenaRankCommand::Complete()
{
	ActivitySystem::GetSystem().DeleteActivityPending( m_dwAutoId );

	if ( !ActivitySystem::GetSystem().DeleteActivity( m_dwAutoId ) )
	{
		SetErrorMsg( "Delete Activity Error" );
		return false;
	}
	return true;
}

/////////////////////////////////////
// activity_role_level [level] [item_id]

bool ActivityRoleLevelCommand::Validate()
{
	RETURN_FALSE_IF_TARGET_IS_ROLE;
	RETURN_FALSE_IF_TARGET_IS_PASSPORT;
	RETURE_FALSE_IF_NO_END_TIME;
	RETURN_FALSE_IF_PARAMS_SIZE_INVALID( ==, 4 );

	m_strSmallPicName	= GET_CMD_PARAM_STRING( 0 );
	m_strBigPicName		= GET_CMD_PARAM_STRING( 1 );
	m_dwMinutesAhead	= GET_CMD_PARAM_INT( 2 );
	std::string str 	= GET_CMD_PARAM_STRING( 3 );

	if ( !ReadThresholdAndReward( m_mapLevelAndReward, str, m_strErrorMsg ) )
	{
		return false;
	}

	return true;
}

bool ActivityRoleLevelCommand::Trigger()
{
	if ( !ActivitySystem::GetSystem().CreateRoleLevelActivity(
			m_dwAutoId,
			m_strSmallPicName.c_str(),
			m_strBigPicName.c_str(),
			m_mapLevelAndReward ) )
	{
		SetErrorMsg( "Create Activity Error" );
		return false;
	}
	return true;
}

bool ActivityRoleLevelCommand::Complete()
{
	ActivitySystem::GetSystem().DeleteActivityPending( m_dwAutoId );

	if ( !ActivitySystem::GetSystem().DeleteActivity( m_dwAutoId ) )
	{
		SetErrorMsg( "Delete Activity Error" );
		return false;
	}
	return true;
}

//////////////////////////////////
// activity_common

bool ActivityCommon::Validate()
{
	RETURN_FALSE_IF_TARGET_IS_ROLE;
	RETURN_FALSE_IF_TARGET_IS_PASSPORT;
	RETURE_FALSE_IF_NO_END_TIME;
	RETURN_FALSE_IF_PARAMS_SIZE_INVALID( ==, 3 );

	m_strSmallPicName	= GET_CMD_PARAM_STRING( 0 );
	m_strBigPicName		= GET_CMD_PARAM_STRING( 1 );
	m_dwMinutesAhead	= GET_CMD_PARAM_INT( 2 );

	return true;
}

bool ActivityCommon::Trigger()
{
	if ( !ActivitySystem::GetSystem().CreateCommonActivity(
			m_dwAutoId,
			m_strSmallPicName.c_str(),
			m_strBigPicName.c_str() ) )
	{
		SetErrorMsg( "Create Activity Error" );
		return false;
	}
	return true;
}

bool ActivityCommon::Complete()
{
	ActivitySystem::GetSystem().DeleteActivityPending( m_dwAutoId );

	if ( !ActivitySystem::GetSystem().DeleteActivity( m_dwAutoId ) )
	{
		SetErrorMsg( "Delete Activity Error" );
		return false;
	}
	return true;
}

//////////////////////////////////
// ActivityExtendRewardCommand

bool ActivityExtendRewardCommand::Validate()
{
	RETURN_FALSE_IF_TARGET_IS_ROLE;
	RETURN_FALSE_IF_TARGET_IS_PASSPORT;
	RETURE_FALSE_IF_NO_END_TIME;
	RETURN_FALSE_IF_PARAMS_SIZE_INVALID( ==, 5 );

	m_strSmallPicName	= GET_CMD_PARAM_STRING( 0 );
	m_strBigPicName		= GET_CMD_PARAM_STRING( 1 );
	m_dwMinutesAhead	= GET_CMD_PARAM_INT( 2 );
	m_byIdType			= GET_CMD_PARAM_INT( 3 );
	m_strTableName		= GET_CMD_PARAM_STRING( 4 );

	if ( !CenterDBCtrl::ExistsTable( m_strTableName.c_str() ) )
	{
		SetErrorMsg( "Table Not Exist In Database" );
		return false;
	}

	return true;
}

bool ActivityExtendRewardCommand::Trigger()
{
	if ( !ActivitySystem::GetSystem().CreateExtendRewardActivity(
			m_dwAutoId,
			m_strSmallPicName.c_str(),
			m_strBigPicName.c_str(),
			m_byIdType,
			m_strTableName ) )
	{
		SetErrorMsg( "Create Activity Error" );
		return false;
	}
	return true;
}

bool ActivityExtendRewardCommand::Complete()
{
	ActivitySystem::GetSystem().DeleteActivityPending( m_dwAutoId );

	if ( !ActivitySystem::GetSystem().DeleteActivity( m_dwAutoId ) )
	{
		SetErrorMsg( "Delete Activity Error" );
		return false;
	}
	return true;
}

//////////////////////////////////
// set_game_server_notice [content]

bool SetGameServerNoticeCommand::Validate()
{
	RETURN_FALSE_IF_TARGET_IS_ROLE;
	RETURN_FALSE_IF_TARGET_IS_PASSPORT;
	RETURN_FALSE_IF_PARAMS_SIZE_INVALID( ==, 1 );

	m_strNotice = GET_CMD_PARAM_STRING( 0 );

	return true;
}

bool SetGameServerNoticeCommand::Trigger()
{
	ChatSystem::GetSystem().SetNoticeContent( m_strNotice );
	return true;
}

bool SetGameServerNoticeCommand::Complete()
{
	return true;
}

//////////////////////////////////////
// set_login_server_notice [content]

bool SetLoginServerNoticeCommand::Validate()
{
	RETURN_FALSE_IF_TARGET_IS_ROLE;
	RETURN_FALSE_IF_TARGET_IS_PASSPORT;
	RETURN_FALSE_IF_PARAMS_SIZE_INVALID( ==, 1 );

	m_strNotice = GET_CMD_PARAM_STRING( 0 );

	return true;
}

bool SetLoginServerNoticeCommand::Trigger()
{
	CenterDBCtrl::UpdateOrInsertNotice( 0, m_strNotice );
	return true;
}

bool SetLoginServerNoticeCommand::Complete()
{
	return true;
}

//////////////////////////////////
// re_create_robot
bool ReCreateRobotCommand::Validate()
{
	RETURN_FALSE_IF_TARGET_IS_PASSPORT;
	RETURN_FALSE_IF_TARGET_IS_ROLE;
	RETURN_FALSE_IF_PARAMS_SIZE_INVALID( ==, 0 );

	return true;
}

bool ReCreateRobotCommand::Trigger()
{
	return RobotSystem::GetSystem().ReCreateAllRobot();
}

bool ReCreateRobotCommand::Complete()
{
	return true;
}
*/
