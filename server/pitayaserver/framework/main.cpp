#include "Server.h"
#include "WorldRunnable.h"
#include "NetRunnable.h"
#include "RelayClientRunnable.h"
#include "SessCtrl.h"
#include "DatabaseMysql.h"
#include "miniini/miniini.h"
#include "IDCtrl.h"
#include "Log.h"
#include "PlayerSession.h"
#include "Util.h"
#include <signal.h>
#include <iostream>
#include <vector>
#include <string>
#include "NameChecker.h"
#include "Set.h"
#include "ScriptGlue.h"

//CShardingDB WorldDb;				///< Accessor to the sharding world database
//DatabaseMysql LoginDatabase;        ///< Accessor to the login database
//DatabaseMysql OperateDatabase;       ///< Accessor to the Operation database

//CLuaState g_luaState;
INIFile g_serverIni;
//std::string g_strConfigPath;
//int g_nVersion;
//std::string g_strWorldDatabaseName;
//std::string g_strOperateDatabaseName;

///////////
bool gDaemonFlag = false;

volatile bool g_stopEvent = false;

//extern int tolua_LuaExport_open(lua_State * pState);

CUserSession * GetSessionSpecial(ZThread::CountedPtr<CNetClient> ptrThis)
{
	return new CPlayerSession(ptrThis);
}

bool InitServer()
{
	srand(time(NULL));

	//日志等级
	INISection * configPath = g_serverIni.GetSection("config");
	if (!configPath)
	{
		IME_ERROR("Miss section [config] in config ini");
		return false;
	}
	std::string strLogLevel;
	if (!configPath->ReadString("loglevel",strLogLevel))
	{
		IME_ERROR("Missing loglevel info");
		return false;
	}
	sLog->SetLogLevel((char *)strLogLevel.c_str());

	int debugmask = 0;
	if (!configPath->ReadInt("logdebug", debugmask))
	{
		IME_ERROR("Missing logdebug");
		return false;
	}
	IME_LOG("log debug %x", (unsigned int)debugmask);
	sLog->SetDebugLogMask((DebugLogFilters)debugmask);

	//////////////////////////////////////////////////////////////////////////
#ifdef LUA_USE_VERSION
	sSCript->Init(CScriptSupport::LUA_SCRIPT);
	sSCript->LoadScript(CSet::ScriptMainFile);
#elif defined JAVASCRIPT_uSE_VERSION

#endif
	if (!HandlerInit(MAIN_THREAD))
	{
		IME_ERROR("man thread handler init fail");
		return false;
	}

	////////
//	if (!g_luaState.Init())
//	{
//		IME_ERROR("luaState Init failed.");
//		return false;
//	}
//	//tolua_LuaExport_open(g_luaState.GetState());
//
//	if (!CLuaCtrl::Init())
//	{
//		IME_ERROR("CLuaCtrl init error");
//		return false;
//	}

	return true;
}

void EndServer()
{
	HandlerFinally(MAIN_THREAD);
//	CLuaCtrl::UnInit();
//	g_luaState.Destroy();
}

// Handle termination signals
// Put the World::m_stopEvent to 'true' if a termination signal is caught 
void OnSignal(int s)
{
	switch (s)  
	{ 
		case SIGINT: 
		case SIGQUIT: 
		case SIGTERM: 
//		case SIGABRT:
		case SIGUSR1:
			//sWorld->AllOffline();
			g_stopEvent = true;
			break; 
	} 
	signal(s, OnSignal);
}
// Define hook '_OnSignal' for all termination signalsi
void HookSignals()
{    
	signal(SIGINT, OnSignal);
	signal(SIGQUIT, OnSignal);
	signal(SIGTERM, OnSignal);
	signal(SIGABRT, OnSignal);
	signal(SIGPIPE, SIG_IGN);
	signal(SIGUSR1, OnSignal);
}
/// Unhook the signals before leaving
void UnHookSignals()
{    
	signal(SIGINT, 0);
	signal(SIGQUIT, 0); 
	signal(SIGTERM, 0);
	signal(SIGABRT, 0);
	signal(SIGUSR1, 0);
}
void ShowUsage(const char * name)
{
	IME_ERROR("pitayaserver");
	IME_ERROR("Usage: %s [-d] ",name);
	IME_ERROR("-d start as a daemon, default is false");
}
void ParseArg(int argc, char * argv[])
{
	for(int i = 1; i < argc; i += 2)
	{
		if(0 == strcmp(argv[i], "-d"))
		{
			gDaemonFlag = true;
		}
		else
		{
			ShowUsage(argv[0]);
			exit(0);
		}
	}

}
int main(int argc, char * argv[])
{
	ParseArg(argc,argv);

	if (gDaemonFlag)
	{
		CUtil::DaemonInit();
	}
	
	sSessCtrl;
	sLog->Initialize();

	//if (!CUtil::SetRLimit())
	//	return 0;

	if (!g_serverIni.OpenFile("server.ini"))
	{
		IME_ERROR("Cannot open server.ini");
		return 0;
	}

	if (!InitServer())
	{
		IME_ERROR("InitServer failed");
		return 0;
	}

//	//do many init db here
//	if (!StartDB())
//	{
//		IME_ERROR("StartDB failed");
//		return 0;
//	}

	IME_LOG("GameServer Start!");
	
	//todo 
	//hooksignal
	HookSignals();

	///- Launch CNetRunnable thread
	CNetRunnable * pNetRunnable = new CNetRunnable;
	ZThread::Thread netThread(pNetRunnable);
	netThread.setPriority((ZThread::Priority)1);

	if (CSet::ClientSessionRun)
	{
		///- Launch CRelayClientRunnable thread
		CRelayClientRunnable * pRelayClientRunnable = new CRelayClientRunnable;
		ZThread::Thread relayClientThread(pRelayClientRunnable);
		relayClientThread.setPriority((ZThread::Priority)1);
	}

	///- Launch CWorldRunnable thread
	ZThread::Thread worldThread(new CWorldRunnable);
	worldThread.setPriority((ZThread::Priority)1);

	worldThread.wait();
	netThread.wait();
	//relayClientThread.wait();

	//unhooksignal	
	UnHookSignals();
	EndServer();

	return 0;
}
