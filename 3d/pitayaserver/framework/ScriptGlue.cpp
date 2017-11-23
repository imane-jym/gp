/*
 * =====================================================================================
 *
 *       Filename:  ScriptGlue.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  11/27/2014 11:01:09 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  imane (), imanecr@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include "eluna/ELuna.h"
#include "ScriptGlue.h"
#include "Log.h"
#include "WorldPacket.h"
#include "Handler.h"

ELuna::Function_Vector ELuna::CPPGarbage::m_CPPFunctions;
ELuna::Method_Vector   ELuna::CPPGarbage::m_CPPMethods;

/*
 *--------------------------------------------------------------------------------------
 *       Class:  CScriptSupport
 *      Method:  CScriptSupport
 * Description:  constructor
 *--------------------------------------------------------------------------------------
 */
CScriptSupport* CScriptSupport::m_pInstance = NULL;
CScriptSupport::CScriptSupport ()
{
	m_pL = NULL;
	m_Type = (EScriptType)0;
}  /* -----  end of method CScriptSupport::CScriptSupport  (constructor)  ----- */

CScriptSupport* CScriptSupport::GetInstance()
{
	if (!m_pInstance)
	{
		m_pInstance = new (std::nothrow)CScriptSupport();
	}
	return m_pInstance;
}

/*
 *--------------------------------------------------------------------------------------
 *       Class:  CScriptSupport
 *      Method:  ~CScriptSupport
 * Description:  deconstructor
 *--------------------------------------------------------------------------------------
 */
CScriptSupport::~CScriptSupport ()
{
	UnInit();
}  /* -----  end of method CScriptSupport::CScriptSupport  (constructor)  ----- */


bool
CScriptSupport::Init (EScriptType e)
{
	if (e == LUA_SCRIPT)
	{
		m_pL = ELuna::openLua();
		m_Type = e;
	}
	else if (e == JAVASCRIPT_SCRIPT)
	{
		IME_ERROR("javascript can not support");
		return false;
	}
	else
	{
		IME_ERROR("script can not support");
		return false;
	}
	return true;
}		/* -----  end of method CScriptSupport::Init  ----- */


void
CScriptSupport::UnInit ()
{
	if (m_Type == LUA_SCRIPT)
	{
		ELuna::closeLua(m_pL);
		m_pL = NULL;
	}
	return ;
}		/* -----  end of method CScriptSupport::UnInit  ----- */


bool
CScriptSupport::LoadScript ( const char *script )
{
	if (m_Type == LUA_SCRIPT)
	{
#ifdef LUA_USE_VERSION
		if (m_pL != NULL)
		{
			ELuna::closeLua(m_pL);
			m_pL = NULL;
			m_pL = ELuna::openLua();
		}

		ELuna::registerClass<WorldPacket>(m_pL, "WorldPacket", ELuna::constructor<WorldPacket, uint16_t>);

		typedef void (WorldPacket::*Func)();
		ELuna::registerMethod<WorldPacket>(m_pL, "Clear", (Func)&WorldPacket::clear);
		typedef void (WorldPacket::*FuncSet8)(int8_t);
		ELuna::registerMethod<WorldPacket>(m_pL, "Set8", (FuncSet8)&WorldPacket::Set8);
		typedef void (WorldPacket::*FuncSet16)(int16_t);
		ELuna::registerMethod<WorldPacket>(m_pL, "Set16", (FuncSet16)&WorldPacket::Set16);
		typedef void (WorldPacket::*FuncSet32)(int32_t);
		ELuna::registerMethod<WorldPacket>(m_pL, "Set32", (FuncSet32)&WorldPacket::Set32);
		typedef void (WorldPacket::*FuncSet64)(int64_t);
		ELuna::registerMethod<WorldPacket>(m_pL, "Set64", (FuncSet64)&WorldPacket::Set64);
		typedef int8_t (WorldPacket::*FuncGet8)();
		ELuna::registerMethod<WorldPacket>(m_pL, "Get8", (FuncSet8)&WorldPacket::Get8);
		typedef int16_t (WorldPacket::*FuncGet16)();
		ELuna::registerMethod<WorldPacket>(m_pL, "Get16", (FuncGet16)&WorldPacket::Get16);
		typedef int32_t (WorldPacket::*FuncGet32)();
		ELuna::registerMethod<WorldPacket>(m_pL, "Get32", (FuncGet32)&WorldPacket::Get32);
		typedef int64_t (WorldPacket::*FuncGet64)();
		ELuna::registerMethod<WorldPacket>(m_pL, "Get64", (FuncGet64)&WorldPacket::Get64);
		typedef size_t (WorldPacket::*FuncGetrpos)();
		ELuna::registerMethod<WorldPacket>(m_pL, "Getrpos", (FuncGetrpos)&WorldPacket::Getrpos);
		typedef size_t (WorldPacket::*FuncSetrpos)(size_t);
		ELuna::registerMethod<WorldPacket>(m_pL, "Setrpos", (FuncSetrpos)&WorldPacket::Setrpos);
		typedef size_t (WorldPacket::*FuncGetwpos)();
		ELuna::registerMethod<WorldPacket>(m_pL, "Getwpos", (FuncGetwpos)&WorldPacket::Getwpos);
		typedef size_t (WorldPacket::*FuncSetwpos)(size_t);
		ELuna::registerMethod<WorldPacket>(m_pL, "Setwpos", (FuncSetwpos)&WorldPacket::Setwpos);
		typedef const char* (WorldPacket::*FuncGetStr)(void);
		ELuna::registerMethod<WorldPacket>(m_pL, "GetStr", (FuncGetStr)&WorldPacket::GetStr);
		typedef void (WorldPacket::*FuncSetStr)(const char *str);
		ELuna::registerMethod<WorldPacket>(m_pL, "SetStr", (FuncSetStr)&WorldPacket::SetStr);
		typedef void (WorldPacket::*FuncLuaAppend)(ELuna::LuaString str);
		ELuna::registerMethod<WorldPacket>(m_pL, "Append", (FuncLuaAppend)&WorldPacket::luaappend);
		typedef ELuna::LuaString (WorldPacket::*FuncGetContents)();
		ELuna::registerMethod<WorldPacket>(m_pL, "GetContents", (FuncGetContents)&WorldPacket::luacontents);
		typedef void (WorldPacket::*FuncHexlike)();
		ELuna::registerMethod<WorldPacket>(m_pL, "HexLike", (FuncHexlike)&WorldPacket::hexlike);
		ELuna::registerClass<CHandlerPlayersession>(m_pL, "CHandlerPlayersession", ELuna::constructor<CHandlerPlayersession>);
		ELuna::registerMethod<CHandlerPlayersession>(m_pL, "SendPacket", &CHandlerPlayersession::SendPacket);
		ELuna::registerMethod<CHandlerPlayersession>(m_pL, "SendPacketToRelay", &CHandlerPlayersession::SendPacketToRelay);
		ELuna::registerClass<CHandlerClientSession>(m_pL, "CHandlerClientSession", ELuna::constructor<CHandlerClientSession>);
		ELuna::registerMethod<CHandlerClientSession>(m_pL, "SendPacket", &CHandlerClientSession::SendPacket);

		ELuna::registerFunction<void>(m_pL, "log", &ime_log_script);
		ELuna::registerFunction<void>(m_pL, "detail_log", &ime_detail_script);
		ELuna::registerFunction<void>(m_pL, "debug_log", &ime_debug_script);
		ELuna::registerFunction<void>(m_pL, "error_log", &ime_error_script);
		ELuna::doFile(m_pL, script);
#endif
	}
	else if (m_Type == JAVASCRIPT_SCRIPT)
	{
		IME_ERROR("javascript can not support");
		return false;
	}
	else
	{
		IME_ERROR("other script can not support");
		return false;
	}
	return true;
}		/* -----  end of method CScriptSupport::LoadScript  ----- */

