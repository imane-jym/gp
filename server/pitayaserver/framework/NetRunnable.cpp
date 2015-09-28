#include "NetRunnable.h"
#include "Server.h"
#include "Log.h"
#include "miniini/miniini.h"

#include <stdio.h>

extern INIFile g_serverIni;

extern volatile bool g_stopEvent;

void CNetRunnable::run()
{
	INISection * server = g_serverIni.GetSection("server");
	if (!server)
	{
		IME_ERROR("Miss section [server] in ***server.ini");
		return ;
	}
	int nPort = 0;
	if (!server->ReadInt("listen_port",nPort))
	{
		IME_ERROR("Miss listen_port");
		return ;
	}
	IME_LOG("NetRunnable thread start!");
	m_server.Init();
	if (!m_server.StartServer("",nPort))
	{
		g_stopEvent = true;
	}
	IME_LOG("NetRunnable thread exit");
}
