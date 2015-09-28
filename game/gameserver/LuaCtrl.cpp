/*
 * LuaCtrl.cpp
 *
 *  Created on: 2011-7-19
 *      Author: yq
 */

#include "LuaCtrl.h"
#include "LuaState.h"
#include "LuaScript.h"
#include "Log.h"

extern CLuaState g_luaState;
extern std::string g_strConfigPath;

CLuaCtrl::MAP_LUA_SCRIPTS CLuaCtrl::m_mapScripts;

#define LOAD_LUA_SCRIPT(name) do { \
	CLuaScript * pLuaScript = new CLuaScript; \
	if (!pLuaScript->Init(g_luaState.GetState(),g_strConfigPath + name)) \
	{ \
		delete pLuaScript; \
		IME_ERROR("error init lua scirpt %s",name); \
		return false; \
	} \
	m_mapScripts[name] = pLuaScript; \
	}while(0)

bool CLuaCtrl::Init()
{

	LOAD_LUA_SCRIPT("Role.lua");

	return true;
}

void CLuaCtrl::UnInit()
{
	MAP_LUA_SCRIPTS::iterator ii = m_mapScripts.begin();
	for(; ii != m_mapScripts.end(); ++ii)
	{
		delete ii->second;
	}
}

CLuaScript * CLuaCtrl::FindScript(const std::string & strName)
{
	MAP_LUA_SCRIPTS::iterator ii = m_mapScripts.begin();
	if (ii != m_mapScripts.end())
		return ii->second;
	return NULL;
}
bool CLuaCtrl::ReloadByName(const std::string & strName)
{
	CLuaScript * pLuaScript = FindScript(strName);
	if (!pLuaScript)
		return false;
	delete pLuaScript;
	pLuaScript = new CLuaScript;
	if (!pLuaScript->Init(g_luaState.GetState(),strName))
	{
		delete pLuaScript;
		IME_ERROR("error reload lua scirpt %s",strName.c_str());
		return false;
	}
	m_mapScripts[strName] = pLuaScript;
	return true;
}
