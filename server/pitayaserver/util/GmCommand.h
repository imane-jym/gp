#ifndef __GM_COMMAND_H__
#define __GM_COMMAND_H__

#include "Defines.h"
#include "DatabaseMysql.h"
#include "string.h"
#include "stdio.h"
#include "stdarg.h"

#define COMMAND_PROPERTY( T, varName, funName )						\
	protected: T varName;											\
	public: virtual T Get##funName() const {return varName;}		\
	public: virtual void Set##funName( T var ) {varName = var; m_bDirty = true; }

static char error_buf[1024];
static inline const char* format_error( const char* fmt, ... )
{
	va_list ap;
	va_start( ap, fmt );
	vsprintf( error_buf, fmt, ap );
	va_end( ap );
	return error_buf;
}

#define RETURN_FALSE_IF_TARGET_IS_PASSPORT 	\
	if ( m_eTargetType == E_GM_COMMAND_TARGET_TYPE_PASSPORT )	\
	{															\
		SetErrorMsg( format_error( "Target Type [Passport] Not Support" ) );	\
		return false;											\
	}

#define RETURN_FALSE_IF_TARGET_IS_ROLE		\
	if ( m_eTargetType == E_GM_COMMAND_TARGET_TYPE_ROLE )		\
	{															\
		SetErrorMsg( format_error( "Target Type [Role] Not Support") );	\
		return false;											\
	}

#define RETURE_FALSE_IF_NO_END_TIME			\
	if ( m_dwEndTime == 0 )					\
	{										\
		SetErrorMsg( format_error( "End Time [0] Not Support") );	\
		return false;						\
	}


#define RETURN_FALSE_IF_PARAMS_SIZE_INVALID( _COMP, _SIZE )	\
	std::vector<std::string> vParam;						\
	CUtil::StrSplit( m_strParams, "|", vParam );			\
	if ( !(vParam.size() _COMP _SIZE) )						\
	{														\
		SetErrorMsg( format_error("Param Count Error, count=%d, require "#_COMP" %d", (int)vParam.size(), _SIZE ) );	\
		return false;										\
	}

#define GET_CMD_PARAM_STRING( _IDX )		\
		vParam[_IDX]

#define GET_CMD_PARAM_INT( _IDX )			\
		atoi(vParam[_IDX].c_str())


namespace CenterDB
{

enum GmCommandTargetType
{
	E_GM_COMMAND_TARGET_TYPE_SERVER		= 1,
	E_GM_COMMAND_TARGET_TYPE_ROLE		= 2,
	E_GM_COMMAND_TARGET_TYPE_PASSPORT	= 3,
};

enum GmCommandStatus
{
	E_GM_COMMAND_STATUS_NEW			= 0,
	E_GM_COMMAND_STATUS_PENDING		= 1,
	E_GM_COMMAND_STATUS_RUNNING		= 2,
	E_GM_COMMAND_STATUS_COMPLETE	= 3,
	E_GM_COMMAND_STATUS_CANCEL		= 4,
	E_GM_COMMAND_STATUS_CANCELED	= 5,
	E_GM_COMMAND_STATUS_ERROR		= 6,
};

//enum GmCommandHandleResult
//{
//	E_GM_COMMAND_HANDLE_RESULT_OK	= 0,
//	E_GM_COMMAND_HANDLE_RESULT_COMP	= 1,
//	E_GM_COMMAND_HANDLE_RESULT_ERROR= 2,
//};

class GmCommand;

typedef GmCommand* (*pFuncCreate)(
			uint32				dwAutoId,
			std::string&		strOpr,
			GmCommandTargetType	eTargetType,
			uint64				dwTargetId,
			uint32				dwStartTime,
			uint32				dwEndTime,
			std::string&		strBackup,
			GmCommandStatus		eStatus,
			std::string&		strParams);
typedef bool (*pFuncCheck)(uint8 byTargetType, uint64 dwTargetId);

class GmCommandFactory
{
public:
	static GmCommand* Create(
			uint32				dwAutoId,
			std::string&		strOpr,
			GmCommandTargetType	eTargetType,
			uint64				dwTargetId,
			uint32				dwStartTime,
			uint32				dwEndTime,
			std::string&		strBackup,
			GmCommandStatus		eStatus,
			std::string&		strParams);

	static bool CheckTargetType( uint8 byTargetType, uint64 dwTargetId );
	static void RegisterFunc(pFuncCreate f1, pFuncCheck f2);
	static pFuncCreate m_f1;
	static pFuncCheck m_f2;
};


class GmCommand
{
	friend class GmCommandFactory;
public:
	GmCommand()
	:m_dwAutoId(0), m_strOpr(""), m_eTargetType(E_GM_COMMAND_TARGET_TYPE_SERVER),
	 m_dwTargetId(0), m_dwStartTime(0), m_dwEndTime(0), 
	 m_strBackup(""), m_eStatus(E_GM_COMMAND_STATUS_NEW), m_bDirty(false)
	{

	}

	virtual ~GmCommand()
	{
	}

	virtual void Tick()
	{
	}

	// return false if valid error
	virtual bool Validate() = 0;
	virtual bool Trigger() = 0;
	virtual bool Complete() = 0;
	virtual bool Cancel()
	{
		if ( m_eStatus == E_GM_COMMAND_STATUS_NEW || m_eStatus == E_GM_COMMAND_STATUS_PENDING )
		{
			SetStatus( E_GM_COMMAND_STATUS_CANCELED );
			return true;
		}
		if ( m_eStatus == E_GM_COMMAND_STATUS_RUNNING )
		{
			if ( this->Complete() )
			{
				SetStatus( E_GM_COMMAND_STATUS_CANCELED );
				return true;
			}
			return false;
		}
		return false;
	}

	//virtual GmCommandHandleResult HandleServer() = 0;
	virtual bool HandleRole( void* pRole ) { return true; }

	COMMAND_PROPERTY( uint32				, m_dwAutoId	, AutoId );
	COMMAND_PROPERTY( std::string			, m_strOpr		, Opr );
	COMMAND_PROPERTY( GmCommandTargetType	, m_eTargetType	, TargetType );
	COMMAND_PROPERTY( uint64				, m_dwTargetId	, TargetId );
	COMMAND_PROPERTY( uint32				, m_dwStartTime	, StartTime );
	COMMAND_PROPERTY( uint32				, m_dwEndTime	, EndTime );
	COMMAND_PROPERTY( std::string			, m_strBackup	, Backup );
	COMMAND_PROPERTY( GmCommandStatus		, m_eStatus		, Status );
	COMMAND_PROPERTY( std::string			, m_strParams	, Params );
	COMMAND_PROPERTY( std::string			, m_strErrorMsg	, ErrorMsg );

	bool Dirty() { return m_bDirty; }
	void Reset() { m_bDirty = false; }

protected:

	bool m_bDirty;


};

}

#endif
