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
#include "ScriptGlue.h"

extern INIFile g_serverIni;

bool HandlerInit(uint8_t thread)
{
	//to do implementation
//	IME_LOG("handler init %u", thread);
	ELuna::LuaFunction<bool> f(sSCript->GetLuaState(), "HandlerInit");
	return f(thread);
}

void HandlerFinally(uint8_t thread)
{
	//to do implementation
//	IME_LOG("handler finally %u", thread);
	ELuna::LuaFunction<void> f(sSCript->GetLuaState(), "HandlerFinally");
	f(thread);
}

void HandlerOntimer(uint32_t interval)
{
	//to do implementation
//	IME_LOG("handler Ontimer %u", interval);
	ELuna::LuaFunction<void> f(sSCript->GetLuaState(), "HandlerOntimer");
	f(interval);
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
	
	ELuna::LuaFunction<void> f(sSCript->GetLuaState(), "HandlerPlayersessionProcess");
	f(this, &packet);
}

void CHandlerPlayersession::UnInit()
{
	//to do implementation
//	IME_LOG("playersession uninit");
	ELuna::LuaFunction<void> f(sSCript->GetLuaState(), "HandlerPlayersessionUnInit");
	f(this);
}

void CHandlerPlayersession::OnTimer(uint32_t interval)
{
	//to do implementation
//	IME_LOG("playersession ontimer %u", interval);
	ELuna::LuaFunction<void> f(sSCript->GetLuaState(), "HandlerPlayersessionOnTimer");
	f(this, interval);
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
	ELuna::LuaFunction<void> f(sSCript->GetLuaState(), "HandlerClientsessionProcess");
	f(this, &packet);
}

void CHandlerClientSession::UnInit()
{
	//to do implementation
//	IME_LOG("clientsession uninit");
	ELuna::LuaFunction<void> f(sSCript->GetLuaState(), "HandlerClientsessionUnInit");
	f(this);
}

void CHandlerClientSession::OnTimer(uint32_t interval)
{
	//to do implementation
//	IME_LOG("clientsession ontimer interval %u", interval);
	ELuna::LuaFunction<void> f(sSCript->GetLuaState(), "HandlerClientsessionOnTimer");
	f(this);
}

void CHandlerClientSession::SendPacket(WorldPacket *packet)
{
	m_pRelay->SendPacket(packet);
}
