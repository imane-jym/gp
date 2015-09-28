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
	
	std::string str;
	packet >> str;
	IME_LOG("get data string %s", str.c_str()); 

	WorldPacket info;
	info << "back send front data hello front server";
	SendPacket(&info);
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
