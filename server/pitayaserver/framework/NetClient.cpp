
#include "NetClient.h"
#include "SocketBuffer.h"
#include "WorldPacket.h"
#include "UserSession.h"
#include "Log.h"
#include <string.h>

extern CUserSession * GetSessionSpecial(ZThread::CountedPtr<CNetClient> ptrThis);

CNetClient::CNetClient(CServer *pServer, int nFd)
//	:evbase(_evbase)
:m_pSession(NULL),m_bIsClose(false),m_pServer(pServer),m_sockFd(nFd),m_eCloseAsyncFlag(E_CLOSE_ASYNC_FLAG_NULL)
{
	m_pBufferInput = new CSocketBuffer();
	m_pBufferOutput = new CSocketBuffer();
	m_pBufferInput->Init();
	m_pBufferOutput->Init();
}
CNetClient::~CNetClient()
{
	IME_LOG("CNetClient Over start");
	delete m_pBufferInput;
	delete m_pBufferOutput;
	WorldPacket *packet;

	///- Go through the to-be-sent queue and delete remaining packets
	while(!m_sendQueue.empty())
	{
        	packet = m_sendQueue.next();
	        delete packet;
	}
	if (!IsClose())
	{
		IME_ERROR("CNetClient Over Try close socket");
		CloseSocket();
	}
}

void hexdump(const char *p, int len)
{
    if (len > 1024)
        return;
    char hexdata[4096];
    int i = 0;
    for (; i < len; i++)
    {
        sprintf(&hexdata[i * 3], "%.2x ", (unsigned char)p[i]);
    }
    IME_DEBUG("%s",hexdata);
}

void CNetClient::OnRead()
{

	size_t buffSize = 0;

	int nParsed = 0;

	char* pData = (char*)m_pBufferInput->GetBuffer(buffSize);
	//少于一个完整包的最少字节
	ServerPktHeader hdr = {0};
	if(buffSize < sizeof(hdr))
		return ;

	while( 1 )
	{
		//TODO 可以修改成最小包
		if(buffSize < sizeof(hdr))//没有达到最小包
			break;

		memcpy(&hdr,pData,sizeof(hdr));
		uint32 wLen = hdr.size;

		if (wLen < sizeof(hdr))//todo
			break;
		if (wLen > buffSize)//没有完整包
			break;
		IME_DEBUG("cmdid %u", hdr.cmd);
        hexdump(pData, wLen);
		//有完整包
		//包长度 + 命令字 
//		uint16 wCmd = 0;
//		memcpy(&wCmd,pData + sizeof(uint16),sizeof(uint16));
		uint32 wRemaining = wLen - sizeof(hdr);

		///- Read the remaining of the packet
       	WorldPacket packet(hdr.cmd, wRemaining); 

		packet.resize(wRemaining);
		if (wRemaining)
		{
			//包长会为0
			memcpy((char *)(packet.contents()),pData + sizeof(hdr),wRemaining);
		}
		//从buff中读 返回读到的字节数 参数为user的缓冲区和size
		//m_pBufferInput->BufferRead((char*)packet.contents(), _remaining);
		

		//critical section
		{
			ZThread::Guard<ZThread::FastMutex> guard(m_mutexSession);
			if (m_pSession)
			{
				//if (packet->GetOpcode() == 1)
				//{
				//	//
				//	HandleAuthProcess(packet);
				//}
				//else
				{
					m_pSession->QueuePacket(packet);
				}
			
			}
			else
			{
				//todo
			}
		}
		pData += wLen;		//数据指针移动位置
		buffSize -= wLen;		//获得剩余长度
		nParsed += wLen;	//获得已经处理的长度
	}

	if( nParsed > 0 )
	{
		m_pBufferInput->Erase(nParsed);	
	}


}
void CNetClient::OnWrite()
{

//    const uint32 SEND_PACKETS_MAX = 100;
//	  const uint32 SEND_BUFFER_SIZE = 1024;

//    uint8 sendBuffer[SEND_BUFFER_SIZE];

    while (!m_sendQueue.empty())
    {
	
        bool haveBigPacket = true;
	/*
        uint32 bufferSize = 0;

        ///- While we have packets to send
        for (uint32 packetCount = 0; (packetCount < SEND_PACKETS_MAX) && !m_sendQueue.empty(); packetCount++)
        {
            ServerPktHeader *hdr = (ServerPktHeader*)&sendBuffer[bufferSize];

            // check merge possibility.
            WorldPacket *front = m_sendQueue.front();
            uint32 packetSize = front->size();

            if ((sizeof(*hdr) + packetSize) > SEND_BUFFER_SIZE)
            {
                haveBigPacket = true;
                break;
            }

            if ((bufferSize + sizeof(*hdr) + packetSize) > sizeof(sendBuffer))
                break;

            // can be merged
            WorldPacket *packet = m_sendQueue.next();

            //hdr->size = ntohs((uint16)packetSize + 2);
            hdr->cmd = packet->GetOpcode();
            
	    ///- Encrypt (if needed) the header
            //_crypt.EncryptSend((uint8*)hdr, sizeof(*hdr));
            bufferSize += sizeof(*hdr);

            if (packetSize)
            {
                memcpy(&sendBuffer[bufferSize], packet->contents(), packetSize);
                bufferSize += packetSize;
            }

            ///- Send the header and body to the client
            delete packet;
        }

        // send merged packets
        if (bufferSize) 
	{
		if (m_pBufferOutput->Append((char*)sendBuffer, bufferSize) != bufferSize)
		{
			//todo
		}
	}
	*/
        // send too big non-merged packet
        if (haveBigPacket) SendSinglePacket();
    }

}
void CNetClient::OnAccept(ZThread::CountedPtr<CNetClient> ptrThis, uint32 dwNetId)
{
	SetSession(GetSessionSpecial(ptrThis));
	//todo
	GetSession()->SetNetId(dwNetId);
	GetSession()->AddSessionToWorker();
	//sWorld->AddSession(GetSession());
}
void CNetClient::SendPacket(WorldPacket *packet)
{
    WorldPacket *pck = new WorldPacket(*packet);
    assert(pck);
    m_sendQueue.add(pck);
    //todo thread safe?
    event_del(&ev_write);
	if( 0 != event_add(&ev_write, NULL) )
	{
		CloseSocket();

		IME_ERROR("write event add failed");
		return ;
	}
	if (m_sendQueue.size() > E_MAX_PENDING_PACKETS)
	{
		IME_ERROR("Max Pending Packets!!!");
	}
}

/// Handle the update order for the socket
void CNetClient::SendSinglePacket()
{
    WorldPacket *packet;
    ServerPktHeader hdr;

    ///- If we have packet to send
    //if (!m_sendQueue.empty())
    {
        packet = m_sendQueue.next();

        hdr.size = packet->size() + sizeof(hdr);//ntohs((uint16)packet->size() + 2);
        hdr.cmd = packet->GetOpcode();

        ///- Encrypt (if needed) the header
        //_crypt.EncryptSend((uint8*)&hdr, 4);

        char buffer[1024];
        int len = 0;
        memcpy(buffer, (char *)&hdr, sizeof(hdr));
        memcpy(buffer + sizeof(hdr), (char *)packet->contents(), packet->size() > 800 ? len = 800 : len = packet->size());
        len += sizeof(hdr);
		IME_DEBUG("cmdid %u", hdr.cmd);
        hexdump(buffer, len);

        ///- Send the header and body to the client
        m_pBufferOutput->Append((char*)&hdr, sizeof(hdr));
        if(!packet->empty()) 
        {
        	m_pBufferOutput->Append((char*)packet->contents(), packet->size());
        }
        delete packet;
    }
}

void CNetClient::CloseSocket()
{
	{
		ZThread::Guard<ZThread::FastMutex> guard(m_mutexClose);
		if (IsClose())
			return ;
		m_bIsClose = true;
		event_del(&ev_read);
		event_del(&ev_write);
		EVUTIL_CLOSESOCKET(m_sockFd);
	}
	{
		ZThread::Guard<ZThread::FastMutex> guard(m_mutexSession);
		m_pSession = NULL;
	}
}

bool CNetClient::IsClose()
{
	//todo need mutex???
	return m_bIsClose;
}

void CNetClient::CloseSocketAsync(CloseAsyncFlagType eFlag)
{   
	{
		ZThread::Guard<ZThread::FastMutex> guard(m_mutexCloseAsync);
		m_eCloseAsyncFlag = eFlag;
	}
}

CNetClient::CloseAsyncFlagType CNetClient::GetCloseAsyncFlag()
{
	{
		ZThread::Guard<ZThread::FastMutex> guard(m_mutexCloseAsync);
		return m_eCloseAsyncFlag;
	}
}   

CServer * CNetClient::GetServer()
{
	return m_pServer;
}

int CNetClient::GetSocketFd()
{
	return m_sockFd;
}

void CNetClient::SetRemoteIP(char * szIP)
{
	m_strRemoteIP = szIP;
}

bool CNetClient::TryAddWriteEvent()
{
	if( 0 != event_add(&ev_write, NULL) )
	{
		CloseSocket();
		IME_ERROR("write event add failed");
		return false;
	}
	return true;
}
