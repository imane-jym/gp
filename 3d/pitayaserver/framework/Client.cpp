/*
 * Client.cpp
 *
 *  Created on: 2011-7-12
 *      Author: yq
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef WIN32
#include <windows.h>
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <errno.h>
#endif

#include "event2/thread.h"
#include "UserSession.h"
#include "Client.h"
#include "NetClient.h"
#include "SocketBuffer.h"
#include "Defines.h"
#include "Log.h"
#include "WorldPacket.h"
#include "RelayClientSession.h"
#include "Handler.h"

extern volatile bool g_stopEvent;

CClient::CClient(CUserSession *pSession)
:evbase(NULL),m_bIsConnected(false),m_nPort(0),m_nReConnectTime(1),m_pSession(pSession)
{
	m_pBufferInput = new CSocketBuffer();
	m_pBufferOutput = new CSocketBuffer();
	m_pBufferInput->Init();
	m_pBufferOutput->Init();
}

CClient::~CClient()
{
	delete m_pBufferInput;
	delete m_pBufferOutput;
	WorldPacket *packet;

	///- Go through the to-be-sent queue and delete remaining packets
	while(!m_sendQueue.empty())
	{
        	packet = m_sendQueue.next();
	        delete packet;
	}
	if (IsConnected())
	{
		CloseClient();
	}

	event_del(&ev_timer);
//	if (evbase)
//	{
//		event_base_free(evbase);
//	}
}

bool CClient::Init(int nReConnectTime, event_base *evba)
{
//	if (-1 == evthread_use_pthreads())
//	{
//		IME_ERROR("evthread_use_pthreads error");
//		return false;
//	}
	/* Initialize libevent. */
//	evbase = event_base_new();
//	IME_LOG( "clinet method : %s", event_base_get_method (evbase) );
	
	evbase = evba;
	// set send package event
	timeval tv;
	evtimer_set(&ev_timer, OnTimer, this);
	event_base_set(evbase,&ev_timer);

	evutil_timerclear(&tv);
	tv.tv_sec = nReConnectTime;
	tv.tv_usec = 0;
	evtimer_add(&ev_timer, &tv);
	m_nReConnectTime = nReConnectTime;

	return true;
}
void CClient::StartClient()
{
	/* Start the libevent event loop. */
	event_base_loop(this->evbase, 0);
}
void CClient::EndClient()
{
	//todo
	event_base_loopbreak(evbase);
}

bool CClient::SetNonBlock()
{
	/* Set the socket to non-blocking, this is essential in event
	 * based programming with libevent. */
	if (evutil_make_socket_nonblocking(client_fd) < 0)
	{
		IME_LOG( "failed to set client socket to non-blocking");
		return false;
	}
	return true;
}

bool CClient::ReConnect()
{
	return Connect(m_strHost.c_str(),m_nPort);
}

bool CClient::Connect(const char * szHost, int nPort)
{
	// set socket
	m_strHost = szHost;
	m_nPort = nPort;

	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr(szHost);
	server_addr.sin_port = htons(nPort);

	client_fd = socket(AF_INET, SOCK_STREAM, 0);

	// if some socket failed
	if (client_fd < 0)
	{
		IME_ERROR("create client fd failed");
		return false;
	}

	struct timeval timeo = {3, 0};
	socklen_t len = sizeof(timeo);
	setsockopt(client_fd, SOL_SOCKET, SO_SNDTIMEO, &timeo, len);

	// add on_read/on_write event
	event_set(&mReadEvent, client_fd, EV_READ | EV_PERSIST, OnRead, this);
	event_set(&mWriteEvent, client_fd, EV_WRITE, OnWrite, this);
	// for thread_safe
	event_base_set(evbase, &mReadEvent);
	event_base_set(evbase, &mWriteEvent);

	if (connect(client_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
	{
		CloseClient();
        if (errno == EINPROGRESS)
        {
        	IME_ERROR("connect timeout");
            return false;
        }
		IME_ERROR(" connect failed, errno %d errmsg %s", EVUTIL_SOCKET_ERROR(), evutil_socket_error_to_string(EVUTIL_SOCKET_ERROR()));
		return false;
	}

	/* Set the socket to non-blocking, this is essential in event
	 * based programming with libevent. */
	if (evutil_make_socket_nonblocking(client_fd) < 0) {
		CloseClient();
		IME_ERROR(" failed to set client socket to non-blocking, errno %d", EVUTIL_SOCKET_ERROR());
		return false;
	}


	if (0 != event_add(&mReadEvent, NULL)) {
		CloseClient();
		return false;
	}
	IME_LOG("Connect success~");
	m_bIsConnected = true;
	return true;
}

void CClient::CloseClient()
{
	m_bIsConnected = false;
	event_del(&mReadEvent);
	event_del(&mWriteEvent);
	EVUTIL_CLOSESOCKET(client_fd);
}

/**
 * This function will be called by libevent when the client socket is
 * ready for reading.
 */
void CClient::OnRead(evutil_socket_t fd, short ev, void *arg)
{
	CClient *pClient = (CClient *)arg;
	int len;

	len = pClient->GetBufferInput()->SockRead(fd);

	if (len == 0) {
		IME_LOG("Server(%d) disconnected when read",fd);
		pClient->CloseClient();
		return;
	}
	else if (len < 0 && ( ! IsTryAgain() ) ) {
		IME_LOG("recv(%d) fail, len %d, errno %d", fd, len, EVUTIL_SOCKET_ERROR() );
		pClient->CloseClient();
		return;
	}
	pClient->OnRead();
//	IME_DEBUG("client %d recv, len %d", fd, len);
}

/**
 * This function will be called by libevent when the client socket is
 * ready for writing.
 */
void CClient::OnWrite(evutil_socket_t fd, short ev, void *arg)
{
	CClient *pClient = (CClient *)arg;
	pClient->OnWrite();
	int len;
	if (pClient->GetBufferOutput()->GetSize() <= 0)
		return ;
	len = pClient->GetBufferOutput()->Sends(fd);

	if (len == 0) {
		IME_LOG("Server(%d) disconnected when write",fd);
		pClient->CloseClient();

		return;
	}
	else if (len < 0 && ( !IsTryAgain() ) ) {
		IME_LOG("send(%d) fail, len %d, errno %d", fd, len, EVUTIL_SOCKET_ERROR() );
		pClient->CloseClient();

		return;
	}
	if (pClient->GetBufferOutput()->GetSize() > 0)
	{
		if( 0 != event_add(&pClient->mWriteEvent, NULL) )
		{
			pClient->CloseClient();

			IME_LOG("write event add failed");
			return ;
		}
	}
//	IME_DEBUG("client %d write len : %d ",fd,len);
}

bool CClient::IsTryAgain()
{
	// thread safe???

#ifdef WIN32
	return WSAEWOULDBLOCK == WSAGetLastError();
#else
	return EAGAIN == errno || EWOULDBLOCK == errno;
#endif

}

void CClient::OnTimer(evutil_socket_t fd, short ev, void *arg)
{
	CClient *pClient = (CClient *)arg;

	if (g_stopEvent)
	{
		pClient->EndClient();
		return ;
	}

	if (!pClient->IsConnected())
	{
		pClient->ReConnect();
		handlerOnConnect(true, pClient->GetdwKey());
	}

	timeval tv;
	evutil_timerclear(&tv);
	tv.tv_sec = pClient->GetReConnectTime();
	tv.tv_usec = 0;
	evtimer_add(pClient->GetEvTimer(),&tv);
}

event * CClient::GetEvTimer()
{
	return &ev_timer;
}
void CClient::OnRead()
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

		if(wLen > buffSize)//没有完整包
			break;
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
void CClient::OnWrite()
{
//	const uint32 SEND_PACKETS_MAX = 100;
//	const uint32 SEND_BUFFER_SIZE = 1024;

//	uint8 sendBuffer[SEND_BUFFER_SIZE];

	while (!m_sendQueue.empty())
	{

		bool haveBigPacket = true;

		if (haveBigPacket) SendSinglePacket();
	}

}
/// Handle the update order for the socket
void CClient::SendSinglePacket()
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

        ///- Send the header and body to the client
        m_pBufferOutput->Append((char*)&hdr, sizeof(hdr));
        if(!packet->empty())
        {
        	m_pBufferOutput->Append((char*)packet->contents(), packet->size());
        }
        delete packet;
    }
}

void CClient::SendPacket(WorldPacket *packet)
{
	if (!IsConnected())
		return ;
    WorldPacket *pck = new WorldPacket(*packet);
    assert(pck);
    m_sendQueue.add(pck);
    //todo thread safe?
    event_del(&mWriteEvent);
	if( 0 != event_add(&mWriteEvent, NULL) )
	{
		CloseClient();

		IME_ERROR("write event add failed");
		return ;
	}
}

int CClient::GetdwKey()
{
	return ((CRelayClientSession *)m_pSession)->GetdwKey();
}

