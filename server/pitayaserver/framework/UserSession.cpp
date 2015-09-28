
#include "UserSession.h"
#include "NetClient.h"
#include "Log.h"

#include <string>


CUserSession::CUserSession()
{

}
CUserSession::CUserSession(ZThread::CountedPtr<CNetClient> ptrNetClient)
:m_ptrNetClient(ptrNetClient)
{

}
CUserSession::~CUserSession()
{
	IME_LOG("UserSession Over");
	if (m_ptrNetClient && !m_ptrNetClient->IsClose())
	{
		//todo

		m_ptrNetClient->CloseSocket();
	}
    ///- empty incoming packet queue 
	while(!m_recvQueue.empty()) 
	{  
		WorldPacket *packet = m_recvQueue.next();   
		delete packet;  
	}
}

bool CUserSession::Update(time_t diff)
{

	if (m_ptrNetClient && m_ptrNetClient->IsClose())
	{
		//todo
		Offline();
		return false;
	}
	return true;
}
void CUserSession::Offline()
{

}

void CUserSession::SendPacket(WorldPacket * packet)
{
	if (m_ptrNetClient && m_ptrNetClient->IsClose())
		return ;
	if (m_ptrNetClient)
	{
		m_ptrNetClient->SendPacket(packet);
	}
}

void CUserSession::QueuePacket(WorldPacket& packet)
{
    WorldPacket *pck = new WorldPacket(packet);
    m_recvQueue.add(pck);
}

std::string CUserSession::GetRemoteIP()
{
	if (m_strIP == "")
	{
		if (m_ptrNetClient && m_ptrNetClient->IsClose())
			return "";
		if (m_ptrNetClient)
		{
			m_strIP = m_ptrNetClient->GetRemoteIP(); 
			return m_strIP;
		}
		return "";
	}
	else
	{
		return m_strIP;
	}
}

void CUserSession::CloseSocket()
{
	if (m_ptrNetClient && !m_ptrNetClient->IsClose())
	{
		//todo

		m_ptrNetClient->CloseSocket();
	}
}
