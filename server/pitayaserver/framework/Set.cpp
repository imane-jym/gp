/*
 * =====================================================================================
 *
 *       Filename:  Set.cpp
 *
 *    Description:  Used to set some parameter
 *
 *        Version:  1.0
 *        Created:  2013年09月22日 21时57分17秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  imane (), imanecr@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include "Set.h"

uint32_t CSet::ClientActiveTime = 300;                   
uint8_t CSet::ClientKeepAlive = 1;
uint8_t CSet::ClientSessionRun = 0;
uint32_t CSet::ExecutionTimeInterval = 20;
uint32_t CSet::MaxPacketOverLoad = 50;
#ifdef LUA_USE_VERSION
const char *CSet::ScriptMainFile = "main.lua";
#elif defined JAVASCRIPT_USE_VERSION
const char *CSet::ScriptMainFile = "main.js";
#else
const char *CSet::ScriptMainFile = "";
#endif

