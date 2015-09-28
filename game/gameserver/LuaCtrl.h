/*
 * LuaCtrl.h
 *
 *  Created on: 2011-7-19
 *      Author: yq
 */

#ifndef LUACTRL_H_
#define LUACTRL_H_

#include <map>
#include <string>

class CLuaScript;

class CLuaCtrl
{
public:
	CLuaCtrl() {};
	~CLuaCtrl() {};

	typedef std::map<std::string, CLuaScript *> MAP_LUA_SCRIPTS;

	static bool Init();

	static void UnInit();

	static CLuaScript * FindScript(const std::string & strName);

	static bool ReloadByName(const std::string & strName);

private:
	static MAP_LUA_SCRIPTS m_mapScripts;
};

#endif /* LUACTRL_H_ */
