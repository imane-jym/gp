/*
 * =====================================================================================
 *
 *       Filename:  Handler.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2013年09月22日 16时53分45秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  imane (), imanecr@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include "Handler.h"
#include "SessCtrl.h"
#include "PlayerSession.h"
#include "RelayClientSession.h"
#include "miniini/miniini.h"

#include <string>
#include "Set.h"

extern INIFile g_serverIni;

bool HandlerInit(uint8_t thread)
{
	CSet::ClientSessionRun = 1;
	//to do implementation
//	IME_LOG("handler init %u", thread);
	return true;
}

void HandlerFinally(uint8_t thread)
{
	//to do implementation
//	IME_LOG("handler finally %u", thread);
}

void HandlerOntimer(uint32_t interval)
{
	//to do implementation
//	IME_LOG("handler Ontimer %u", interval);
}

CHandlerPlayersession::CHandlerPlayersession()
{
	m_pPlayer = NULL;
	m_strIp = "";

	//to do implementation
}

CHandlerPlayersession::~CHandlerPlayersession()
{

}

void CHandlerPlayersession::Process(WorldPacket &packet)
{
	//to do implementation
//	IME_LOG("playersession get packet size %u", packet.size());
	
	char test[200];
	std::string str;
	packet.read((uint8_t *)test, 200);
	IME_LOG("get data string %s", test); 

	WorldPacket info;
	info << "I get the data packet";
	SendPacket(&info);

	WorldPacket pkg;
	pkg << std::string("front send data to back hello backserver");
	SendPacketToRelay(&pkg, 0);
	SendPacketToRelay(&pkg, 1);
	IME_DEBUG("send pkg to every backends");
}

void CHandlerPlayersession::UnInit()
{
	//to do implementation
//	IME_LOG("playersession uninit");
}

void CHandlerPlayersession::OnTimer(uint32_t interval)
{
	//to do implementation
//	IME_LOG("playersession ontimer %u", interval);
}

void CHandlerPlayersession::SendPacketToRelay(WorldPacket *packet, int key)
{
	sSessCtrl->SendToRelay(*packet, key);
}

void CHandlerPlayersession::SendPacket(WorldPacket *packet)
{
	m_pPlayer->SendPacket(packet);
}

CHandlerClientSession::CHandlerClientSession()
{
	m_pRelay = NULL;

	//to do implementation
}

CHandlerClientSession::~CHandlerClientSession()
{

}

void CHandlerClientSession::Process(WorldPacket &packet)
{
	//to do implementation
//	IME_LOG("clientsession get packet size %u", packet.size());
	IME_LOG("clientsession key %u get packet size %u", (uint32_t)m_pRelay->GetdwKey(), (uint32_t)packet.size());
	std::string str;
	packet >> str;
	IME_LOG("get data str %s", str.c_str());

//	WorldPacket info;
//	info << cmd;
//	info << std::string("backend recv and send back");
//	SendPacket(&info);
}

void CHandlerClientSession::UnInit()
{
	//to do implementation
//	IME_LOG("clientsession uninit");
}

void CHandlerClientSession::OnTimer(uint32_t interval)
{
	//to do implementation
//	IME_LOG("clientsession ontimer interval %u", interval);
}

void CHandlerClientSession::SendPacket(WorldPacket *packet)
{
	m_pRelay->SendPacket(packet);
}
