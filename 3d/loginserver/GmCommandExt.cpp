#include "GmCommandExt.h"
#include "DatabaseMysql.h"
#include "CenterDBCtrl.h"
#include "Util.h"
#include "stdlib.h"

using namespace CenterDB;

GmCommand* Create(
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
	if ( strOpr == "set_gm_auth" )
		ret = new SetGmAuthCommand();
	else if (strOpr == "cancel_gm_command")
		ret = new CancelCommand();

	if ( ret )
	{
		ret->SetAutoId( dwAutoId );
		ret->SetOpr( strOpr );
		ret->SetTargetType( eTargetType );
		ret->SetTargetId( dwTargetId );
		ret->SetStartTime( dwStartTime );
		ret->SetEndTime( dwEndTime );
		ret->SetBackup( strBackup );
		ret->SetStatus( eStatus );
		ret->SetParams( strParams );
	}

	return ret;
}

bool CheckTargetType( uint8 byTargetType, uint64 dwTargetId )
{
	if ( byTargetType == E_GM_COMMAND_TARGET_TYPE_PASSPORT )
	{
		return true;
	}
	return false;
}


bool SetGmAuthCommand::Validate()
{
	RETURN_FALSE_IF_PARAMS_SIZE_INVALID( ==, 1 );

	m_byGmAuth		= atoi( vParam[0].c_str() );

	if ( !CenterDBCtrl::ExistsPassport( m_dwTargetId ) )
	{
		SetErrorMsg( format_error( "Passport Not Exists, passportId=%llu", m_dwTargetId ) );
		return false;
	}
	return true;
}

bool SetGmAuthCommand::Trigger()
{
	if ( !CenterDBCtrl::UpdatePassportGmAuth( m_dwTargetId, m_byGmAuth ) )
	{
		SetErrorMsg( format_error( "Update Passport Fail, passportId=%llu", m_dwTargetId ) );
		return false;
	}
	return true;
}

bool SetGmAuthCommand::Complete()
{
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
