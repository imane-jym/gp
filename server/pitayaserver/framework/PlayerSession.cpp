#include "PlayerSession.h"
#include "NetClient.h"
#include "Log.h"
#include "SessCtrl.h"
#include <string>
#include "CmdInnerDefine.h"
#include "WorldPacket.h"
#include "NameChecker.h"
#include "Set.h"

CPlayerSession::CPlayerSession(ZThread::CountedPtr<CNetClient> ptrNetClient)
:CUserSession(ptrNetClient)
{
	m_timers[USER_UPDATE_TIMER_1s].SetInterval(1 * 1000);
	m_timers[USER_UPDATE_TIMER_3s].SetInterval(3 * 1000);
	m_timers[USER_UPDATE_TIMER_5s].SetInterval(5 * 1000);
	m_timers[USER_UPDATE_TIMER_30s].SetInterval(30 * 1000);
	m_timers[USER_UPDATE_TIMER_1min].SetInterval(1 * 60 * 1000);
	m_timers[USER_UPDATE_TIMER_3min].SetInterval(3 * 60 * 1000);
	m_timers[USER_UPDATE_TIMER_5min].SetInterval(5 * 60 * 1000);
	m_timers[USER_UPDATE_TIMER_10min].SetInterval(10 * 60 * 1000);
	m_timers[USER_UPDATE_TIMER_30min].SetInterval(30 * 60 * 1000);

	m_dwActiveTime = time(NULL);
	m_CHandler.SetCPlayerSession(this);
	m_CHandler.SetstrIp(GetRemoteIP());
	IME_LOG("NEW PLAYERSESSION IP %s", GetRemoteIP().c_str());
}

CPlayerSession::~CPlayerSession()
{
	//todo
	IME_LOG("DEL PLAYERSESSION");
}

bool CPlayerSession::Update(time_t diff)
{
	WorldPacket *packet;
//	printf("UserSession update~\n");
	INIT_PROCESS_TIMER;
	char msg[1024];
	while(!m_ptrNetClient->IsClose() && !m_recvQueue.empty()) 
	{
		packet = m_recvQueue.next();
		
		m_dwActiveTime = time(NULL);
		if (m_recvQueue.size() >= CSet::MaxPacketOverLoad)
		{
			//todo
			IME_ERROR("PlayerSession packet overload uplimit close connect ip %s", GetRemoteIP().c_str());
			Offline();
			return false;
		}
		
		PROCESS_TIMER_START;
		uint16 opcode = packet->GetOpcode();
		sprintf(msg, "playersession cmd %u", opcode);
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

	if (m_timers[USER_UPDATE_TIMER_1s].Passed())
	{
		m_timers[USER_UPDATE_TIMER_1s].Reset();
		PROCESS_TIMER_START;
		OnTimer1s();
		PROCESS_TIMER_END("playersession 1s");
	}
	if (m_timers[USER_UPDATE_TIMER_3s].Passed())
	{
		m_timers[USER_UPDATE_TIMER_3s].Reset();
		PROCESS_TIMER_START;
		OnTimer3s();
		PROCESS_TIMER_END("playersession 3s");
	}
    ///-
	if (m_timers[USER_UPDATE_TIMER_5s].Passed())
	{
		m_timers[USER_UPDATE_TIMER_5s].Reset();
		PROCESS_TIMER_START;
		OnTimer5s();
		PROCESS_TIMER_END("playersession 5s");
	}
    ///-
	if (m_timers[USER_UPDATE_TIMER_30s].Passed())
	{
		m_timers[USER_UPDATE_TIMER_30s].Reset();
		PROCESS_TIMER_START;
		OnTimer30s();
		PROCESS_TIMER_END("playersession 30s");
	}
    ///-
    if (m_timers[USER_UPDATE_TIMER_1min].Passed())
    {
        m_timers[USER_UPDATE_TIMER_1min].Reset();
		PROCESS_TIMER_START;
        OnTimer1min();
		PROCESS_TIMER_END("playersession 1min");
    }
    if (m_timers[USER_UPDATE_TIMER_3min].Passed())
    {
        m_timers[USER_UPDATE_TIMER_3min].Reset();
		PROCESS_TIMER_START;
        OnTimer3min();
		PROCESS_TIMER_END("playersession 1min");
    }
    if (m_timers[USER_UPDATE_TIMER_5min].Passed())
    {
        m_timers[USER_UPDATE_TIMER_5min].Reset();
		PROCESS_TIMER_START;
        OnTimer5min();
		PROCESS_TIMER_END("playersession 1min");
    }
    if (m_timers[USER_UPDATE_TIMER_10min].Passed())
    {
        m_timers[USER_UPDATE_TIMER_10min].Reset();
		PROCESS_TIMER_START;
        OnTimer10min();
		PROCESS_TIMER_END("playersession 10min");
    }
    if (m_timers[USER_UPDATE_TIMER_30min].Passed())
    {
        m_timers[USER_UPDATE_TIMER_30min].Reset();
		PROCESS_TIMER_START;
        OnTimer30min();
		PROCESS_TIMER_END("playersession 30min");
    }

	if (m_ptrNetClient->IsClose())
	{
		//todo
		IME_ERROR("PlayerSession Offline");
		Offline();
		return false;
	}
	return true;
}

void CPlayerSession::AddSessionToWorker()
{
	sSessCtrl->AddSession(this);
}

void CPlayerSession::Offline()
{
	m_CHandler.UnInit();
}

void CPlayerSession::OnTimer1s()
{
	m_CHandler.OnTimer(TIMER_1S);
}
void CPlayerSession::OnTimer3s()
{
	m_CHandler.OnTimer(TIMER_3S);
}
void CPlayerSession::OnTimer5s()
{
	m_CHandler.OnTimer(TIMER_5S);
}
void CPlayerSession::OnTimer30s()
{
	m_CHandler.OnTimer(TIMER_30S);
}
void CPlayerSession::OnTimer1min()
{
	m_CHandler.OnTimer(TIMER_1MIN);
}
void CPlayerSession::OnTimer3min()
{
	m_CHandler.OnTimer(TIMER_3MIN);
}
void CPlayerSession::OnTimer5min()
{
	m_CHandler.OnTimer(TIMER_5MIN);
}
void CPlayerSession::OnTimer10min()
{
	m_CHandler.OnTimer(TIMER_10MIN);
}
void CPlayerSession::OnTimer30min()
{
	m_CHandler.OnTimer(TIMER_30MIN);
}
void CPlayerSession::KickSession(CNetClient::CloseAsyncFlagType e)
{
	m_ptrNetClient->CloseSocketAsync(e);
}
void CPlayerSession::HandlerKick(uint32_t type)
{
	m_CHandler.OnKick(type);
	KickSession(CNetClient::E_CLOSE_ASYNC_FLAG_FLUSH_CLOSE);
}
