#include <new>
#include "SessCtrl.h"
#include "UserSession.h"
#include "Log.h"
#include "RelayClientSession.h"
#include "CmdInnerDefine.h"
#include "google/malloc_extension.h"
#include "Handler.h"
#include "PlayerSession.h"
#include "Set.h"
#include "Util.h"
#include "ScriptGlue.h"

CSessCtrl * CSessCtrl::m_pInstance = NULL;

CSessCtrl::CSessCtrl()
{
//	m_pRelayClientSession = new CRelayClientSession;
}

CSessCtrl::~CSessCtrl()
{
	for (unsigned int i = 0; i < m_vecRelayClientSession.size(); i++)
	{
		if (m_vecRelayClientSession[i])
		{
			delete m_vecRelayClientSession[i];
		}
	}
	m_vecRelayClientSession.clear();

//	std::map<uint64_t, CGroup *>::iterator it = m_mapGroup.begin();
//
//	for (it = m_mapGroup.begin(); it != m_mapGroup.end(); it++)
//	{
//		delete it->second;
//	}
}

//void CSessCtrl::Init()
//{
//	// call after mysql init.
//
//}

CSessCtrl * CSessCtrl::GetInstance()
{
	if (!m_pInstance)
	{
		m_pInstance = new CSessCtrl();
	}
	return m_pInstance;
}
void CSessCtrl::AddSession(CUserSession *pSession)
{
	m_addSessQueue.add(pSession);
}
void CSessCtrl::_AddSession(CUserSession *pSession)
{
	assert(pSession);
//	printf("new session added~\n");	
	m_sessions[pSession->GetNetId()] = pSession;
}

bool CSessCtrl::InitSettings()
{
	m_timers[WUPDATE_1s].SetInterval(1 * 1000);	// 
	m_timers[WUPDATE_3s].SetInterval(3 * 1000);			// 3秒
	m_timers[WUPDATE_5s].SetInterval(5 * 1000);	// 
	m_timers[WUPDATE_15s].SetInterval(15 * 1000);	//  15秒
	m_timers[WUPDATE_30s].SetInterval(30 * 1000);	// 
	m_timers[WUPDATE_1min].SetInterval(1 * 60 * 1000);	// 1分钟
	m_timers[WUPDATE_3min].SetInterval(3 * 60 * 1000);	// 1分钟
	m_timers[WUPDATE_5min].SetInterval(5 * 60 * 1000);	// 1分钟
	m_timers[WUPDATE_10min].SetInterval(10 * 60 * 1000);	// 
	m_timers[WUPDATE_30min].SetInterval(30 * 60 * 1000);
	
	m_logtime = time(NULL);	
	if (!HandlerInit(WORLD_THREAD))
	{
		IME_ERROR("world thread handler init fail");
		return false;
	}
	return true;
}

void CSessCtrl::Update(time_t diff)
{
    ///- Update the different timers
    for (int i = 0; i < WUPDATE_COUNT; ++i)
    {
        if (m_timers[i].GetCurrent() >= 0)
            m_timers[i].Update(diff);
        else
            m_timers[i].SetCurrent(0);
    }

	INIT_PROCESS_TIMER;
    ///- Ping to keep MySQL connections alive
	if (m_timers[WUPDATE_1s].Passed())
	{
		m_timers[WUPDATE_1s].Reset();
		PROCESS_TIMER_START;
		Update1s();
		PROCESS_TIMER_END("world 1s");
	}
	if (m_timers[WUPDATE_3s].Passed())
	{
		m_timers[WUPDATE_3s].Reset();
		PROCESS_TIMER_START;
		Update3s();
		PROCESS_TIMER_END("world 3s");
	}
	if (m_timers[WUPDATE_5s].Passed())
	{
		m_timers[WUPDATE_5s].Reset();
		PROCESS_TIMER_START;
		Update5s();
		PROCESS_TIMER_END("world 5s");
	}
	if (m_timers[WUPDATE_15s].Passed())
	{
		m_timers[WUPDATE_15s].Reset();
		PROCESS_TIMER_START;
		Update15s();
		PROCESS_TIMER_END("world 15s");
	}
	if (m_timers[WUPDATE_30s].Passed())
	{
		m_timers[WUPDATE_30s].Reset();
		PROCESS_TIMER_START;
		Update30s();
		PROCESS_TIMER_END("world 30s");
	}
    if (m_timers[WUPDATE_1min].Passed())
    {
        m_timers[WUPDATE_1min].Reset();
		PROCESS_TIMER_START;
		Update1min();
		PROCESS_TIMER_END("world 1min");
    }
    if (m_timers[WUPDATE_3min].Passed())
    {
        m_timers[WUPDATE_3min].Reset();
		PROCESS_TIMER_START;
		Update3min();
		PROCESS_TIMER_END("world 3min");
    }
    if (m_timers[WUPDATE_5min].Passed())
    {
        m_timers[WUPDATE_5min].Reset();
		PROCESS_TIMER_START;
		Update5min();
		PROCESS_TIMER_END("world 5min");
    }
	if (m_timers[WUPDATE_10min].Passed())
	{
		m_timers[WUPDATE_10min].Reset();
		PROCESS_TIMER_START;
		Update10min();
		PROCESS_TIMER_END("world 10min");
	}
	if (m_timers[WUPDATE_30min].Passed())
	{
		m_timers[WUPDATE_30min].Reset();
		PROCESS_TIMER_START;
		Update30min();
		PROCESS_TIMER_END("world 30min");
	}

	UpdateSessions(diff);
	UpdateRelayClient(diff);
}
void CSessCtrl::UpdateRelayClient(time_t diff)
{
	for (size_t i = 0; i < m_vecRelayClientSession.size(); i++)
	{
//	assert(m_pRelayClientSession);
//	//todo
//	m_pRelayClientSession->Update(diff);
//		assert(m_vecRelayClientSession[i]);
		//todo
		if (m_vecRelayClientSession[i] == NULL)
		{
			IME_ERROR("relay session is NULL");
			return;
		}
		m_vecRelayClientSession[i]->Update(diff);
	}
}

void CSessCtrl::AddRelayClientSession(CRelayClientSession *pRelay) 
{ 
	if (pRelay->GetdwKey() >= m_vecRelayClientSession.size()) 
	{
		m_vecRelayClientSession.resize((pRelay->GetdwKey() + 1), NULL);
	}
	m_vecRelayClientSession[pRelay->GetdwKey()] = pRelay;
}

void CSessCtrl::UpdateSessions(time_t diff)
{
	//add new sessions
	CUserSession *sess;
	while(!m_addSessQueue.empty())
	{
		sess = m_addSessQueue.next();
		_AddSession(sess);
	}
	SessionMap::iterator itr = m_sessions.begin();
	SessionMap::iterator next;
	for(; itr != m_sessions.end(); itr = next)
	{
		next = itr;
		++next;
		CUserSession *pSession = itr->second;
		if (!pSession->Update(diff))
		{
			//todo
			m_sessions.erase(itr);
			delete pSession;
		}
	}
}

void CSessCtrl::AllOffline()
{
	IME_ERROR("OFFLINE SESSION NUM[Num=%u]", (uint32)m_sessions.size());
	IME_ERROR("-------------------------------------waiting for alloffline------------------------------");

	HandlerFinally(WORLD_THREAD);

	SessionMap::iterator it = m_sessions.begin();
	for ( ; it !=m_sessions.end(); ++it)
	{
		CUserSession* pSess = it->second;
		pSess->Offline();
		delete pSess;
	}
	for (size_t i = 0; i < m_vecRelayClientSession.size(); i++)
	{
		if (m_vecRelayClientSession[i] != NULL)
			m_vecRelayClientSession[i]->Offline();
	}
	IME_ERROR("-------------------------------------alloffline over------------------------------");
}

bool CSessCtrl::SendToRelay( WorldPacket& pkg , unsigned int key)
{
	if (m_vecRelayClientSession.size() <= key)
	{
		IME_ERROR("can not find this key associate clientsession key %u", key);
		return false;
	}
	if (!m_vecRelayClientSession[key])
	{
		return false;
	}
	m_vecRelayClientSession[key]->SendPacket(&pkg);
	return true;
}

void CSessCtrl::SendPkgToAll( WorldPacket& pkg )
{
	SessionMap::iterator it = m_sessions.begin();
	for ( ; it != m_sessions.end(); ++it)
	{
		CUserSession* pSess = it->second;
		pSess->SendPacket(&pkg);
	}
}

void CSessCtrl::SendPkgToPart( WorldPacket& pkg, uint32_t off, uint32_t number)
{
	uint32_t i = 0;
	SessionMap::iterator it = m_sessions.begin();
	for ( ; it != m_sessions.end(); ++it)
	{
		if (i >= off && i < off + number)
		{
			CUserSession* pSess = it->second;
			pSess->SendPacket(&pkg);
		}
		i++;
	}
}

void CSessCtrl::Update1min()
{
	HandlerOntimer(TIMER_1MIN);
}

void CSessCtrl::Update3min()
{
	HandlerOntimer(TIMER_3MIN);
}

void CSessCtrl::Update5min()
{
	HandlerOntimer(TIMER_5MIN);
}

void CSessCtrl::Update30min()
{
	MallocExtension::instance()->ReleaseFreeMemory();
	HandlerOntimer(TIMER_30MIN);
}

void CSessCtrl::Update10min()
{
	if (!CUtil::IsToday(m_logtime))
	{
		m_logtime = time(NULL);
		sLog->LogFileChange();
	}
	HandlerOntimer(TIMER_10MIN);
}

void CSessCtrl::Update3s()
{
	HandlerOntimer(TIMER_3S);
	if (CSet::ClientKeepAlive)
	{
		SessionMap::iterator itr = m_sessions.begin();
		SessionMap::iterator next;
		for(; itr != m_sessions.end(); itr = next)
		{    
			next = itr;
			++next;
			CPlayerSession *pSession = (CPlayerSession *)(itr->second);
			if (pSession->GetdwActiveTime() + CSet::ClientActiveTime <= time(NULL))
			{    
				IME_ERROR("Session ip %s not alive close it alivetime %u, now %u", pSession->GetRemoteIP().c_str(), pSession->GetdwActiveTime(), (uint32_t)time(NULL));
//				m_sessions.erase(itr);
//				pSession->Offline();
//				delete pSession;
				pSession->HandlerKick(CPlayerSession::KICK_NOT_ALIVE);
			}        
		}
	}
}

void CSessCtrl::Update1s()
{
	HandlerOntimer(TIMER_1S);
}

void CSessCtrl::Update5s()
{
	HandlerOntimer(TIMER_5S);
}

void CSessCtrl::Update15s()
{
	HandlerOntimer(TIMER_15S);
}

void CSessCtrl::Update30s()
{
	HandlerOntimer(TIMER_30S);
}

uint32 CSessCtrl::GetOnlineNum()
{
	return m_sessions.size();
}
