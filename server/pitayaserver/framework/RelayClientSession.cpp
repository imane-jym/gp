/*
 * RelayClientSession.cpp
 *
 *  Created on: 2011-7-13
 *      Author: yq
 */

#include "WorldPacket.h"
#include "Client.h"
#include "Log.h"
#include "RelayClientSession.h"
#include "CmdInnerDefine.h"
#include <assert.h>

#include "SessCtrl.h"
#include "md5.h"
#include "Set.h"


CRelayClientSession::CRelayClientSession()
:m_pClient(NULL)
{
	m_timers[USER_UPDATE_TIMER_1s].SetInterval(1 * 1000);
	m_timers[USER_UPDATE_TIMER_3s].SetInterval(3 * 1000);
	m_timers[USER_UPDATE_TIMER_5s].SetInterval(5 * 1000);
	m_timers[USER_UPDATE_TIMER_1min].SetInterval(60 * 1000);
	m_timers[USER_UPDATE_TIMER_10min].SetInterval(600 * 1000);
	m_timers[USER_UPDATE_TIMER_30min].SetInterval(1800 * 1000);
	
	m_CHandler.SetRelay(this);
}
CRelayClientSession::~CRelayClientSession()
{

}
void CRelayClientSession::SendPacket(WorldPacket * packet)
{
	if (m_pClient)
	{
		m_pClient->SendPacket(packet);
	}
}
bool CRelayClientSession::Update(time_t diff)
{
	WorldPacket *packet;

	INIT_PROCESS_TIMER;
	char msg[1024];
	while(!m_recvQueue.empty())
	{
		packet = m_recvQueue.next();

		if (m_recvQueue.size() >= CSet::MaxPacketOverLoad)
		{
			//todo
			IME_ERROR("RelayClientSession packet overload uplimit close connect key %u", GetdwKey());
			Offline();
			return false;
		}

		PROCESS_TIMER_START;
		uint16 opcode = packet->GetOpcode();
		sprintf(msg, "relaysession cmd %u key %u", opcode, GetdwKey());
		m_CHandler.Process(*packet);
		PROCESS_TIMER_END(msg);
		delete packet;
	}

    ///- Update the different timers
    for (int i = 0; i < USER_UPDATE_COUNT; ++i)
    {
        if (m_timers[i].GetCurrent() >= 0)
            m_timers[i].Update(diff);
        else
            m_timers[i].SetCurrent(0);
    }

    ///-
	
	if (m_timers[USER_UPDATE_TIMER_1s].Passed())
	{
		m_timers[USER_UPDATE_TIMER_1s].Reset();
		sprintf(msg, "relaysession key %u relaysession 1s", GetdwKey());
		PROCESS_TIMER_START;
		m_CHandler.OnTimer(TIMER_1S);
		PROCESS_TIMER_END(msg);
	}
	if (m_timers[USER_UPDATE_TIMER_3s].Passed())
	{
		m_timers[USER_UPDATE_TIMER_3s].Reset();
		sprintf(msg, "relaysession key %u relaysession 3s", GetdwKey());
		PROCESS_TIMER_START
		m_CHandler.OnTimer(TIMER_3S);
		PROCESS_TIMER_END(msg);
	}
	if (m_timers[USER_UPDATE_TIMER_5s].Passed())
	{
		m_timers[USER_UPDATE_TIMER_5s].Reset();
		sprintf(msg, "relaysession key %u relaysession 5s", GetdwKey());
		PROCESS_TIMER_START
		m_CHandler.OnTimer(TIMER_5S);
		PROCESS_TIMER_END(msg);
	}
	if (m_timers[USER_UPDATE_TIMER_1min].Passed())
	{
		m_timers[USER_UPDATE_TIMER_1min].Reset();
		sprintf(msg, "relaysession key %u relaysession 1min", GetdwKey());
		PROCESS_TIMER_START;
		m_CHandler.OnTimer(TIMER_1MIN);
		PROCESS_TIMER_END(msg);
	}
	if (m_timers[USER_UPDATE_TIMER_10min].Passed())
	{
		m_timers[USER_UPDATE_TIMER_10min].Reset();
		sprintf(msg, "relaysession key %u relaysession 10min", GetdwKey());
		PROCESS_TIMER_START;
		m_CHandler.OnTimer(TIMER_10MIN);
		PROCESS_TIMER_END(msg);
	}
	if (m_timers[USER_UPDATE_TIMER_30min].Passed())
	{
		m_timers[USER_UPDATE_TIMER_30min].Reset();
		sprintf(msg, "relaysession key %u relaysession 30min", GetdwKey());
		PROCESS_TIMER_START;
		m_CHandler.OnTimer(TIMER_30MIN);
		PROCESS_TIMER_END(msg);
	}

	if (m_pClient && !m_pClient->IsConnected())
	{
		//todo
		Offline();
		return false;
	}

	return true;
}
void CRelayClientSession::AddSessionToWorker()
{
	IME_ERROR("This function is no need to run~");
	assert(false);
}
void CRelayClientSession::Offline()
{
	m_CHandler.UnInit();
}
