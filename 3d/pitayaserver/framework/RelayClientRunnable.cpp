/*
 * RelayClientRunnable.cpp
 *
 *  Created on: 2011-7-13
 *      Author: yq
 */
#include "SessCtrl.h"
#include "Log.h"
#include "RelayClientSession.h"
#include "RelayClientRunnable.h"
#include "miniini/miniini.h"
#include "Client.h"
#include "event2/thread.h"
extern INIFile g_serverIni;

void CRelayClientRunnable::run()
{
	INISection * server = g_serverIni.GetSection("relayserver");
	if (!server)
	{
		IME_ERROR("Miss section [relayserver] in ***server.ini");
		return ;
	}

	std::vector<std::string> vecHost;
	if (!server->ReadStrings("relayserver_host", vecHost))
	{
		IME_ERROR("Miss relayserver_string");
		return ;
	}

	std::vector<int> vecPort;
	if (!server->ReadInts("relayserver_port", vecPort))
	{
		IME_ERROR("Miss relayserver_port");
		return ;
	}


	IME_LOG("RelayClientRunnable thread start!");

	if (-1 == evthread_use_pthreads())
	{
		IME_ERROR("evthread_use_pthreads error");
		return;
	}
	event_base *evbase = event_base_new();
	std::vector<CClient *> vecClient;

	for (size_t i = 0; i < vecHost.size(); i++)
	{
		CRelayClientSession *pRelay = new CRelayClientSession();
		CClient * pClient = new CClient((CUserSession *)(pRelay));
		pRelay->SetClient(pClient);
		pRelay->SetdwKey(i);

		//todo
		pClient->Init(3, evbase);
		pClient->Connect(vecHost[i].c_str(),vecPort[i]);
		vecClient.push_back(pClient);
		//todo multi thread lock
		sSessCtrl->AddRelayClientSession(pRelay);

		handlerOnConnect(false, i);
	}

//	pClient->StartClient();
	event_base_loop(evbase, 0);
	event_base_free(evbase);

	for (size_t i = 0; i < vecClient.size(); i++)
	{
		delete vecClient[i];
	}
//	delete pClient;
	IME_LOG("RelayClientRunnable thread exit");
}
