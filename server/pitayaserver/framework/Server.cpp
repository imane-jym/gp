#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef WIN32
#include <windows.h>
#else
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <errno.h>
#endif

#include "Server.h"
#include "NetClient.h"
#include "SocketBuffer.h"
#include "Defines.h"
#include "Log.h"
#include "event2/thread.h"

extern volatile bool g_stopEvent;

CServer::CServer()
:evbase(NULL),m_dwNetIdTotal(0)
{
	m_clientMap.clear();
}

CServer::~CServer()
{
	for(ClientMap::iterator ii = m_clientMap.begin(); ii != m_clientMap.end(); ++ii)
	{	
		ii->second->CloseSocket();
	}
	m_clientMap.clear();
	event_del(&ev_accept);
	event_del(&ev_timer_stop_deref);
	if (evbase)
	{
		event_base_free(evbase);
	}
}

bool CServer::Init()
{
	/* Initialize libevent. */
	if (-1 == evthread_use_pthreads())
	{
		IME_ERROR("evthread_use_pthreads error");
		return false;
	}
	evbase = event_base_new();
	IME_LOG( "method : %s", event_base_get_method (evbase) );
	return true;
}
void CServer::EndServer()
{
	//todo
	event_base_loopbreak(evbase);
}
bool CServer::StartListen(const char * szAddr, int nPort)
{
	/* Create our listening socket. This is largely boiler plate
	 * code that I’ll abstract away in the future. */
	listen_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_fd < 0)
	{
		IME_LOG("create listen fd failed");
		return false;
	}
	
	int reuseaddr_on = 1;
	if (setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, (char*)&reuseaddr_on,
		sizeof(reuseaddr_on)) == -1)
	{	
		IME_LOG("setsockopt reuseaddr failed");
		return false;
	}
	memset(&listen_addr, 0, sizeof(listen_addr));
	listen_addr.sin_family = AF_INET;
	listen_addr.sin_addr.s_addr = INADDR_ANY;
	listen_addr.sin_port = htons(nPort);
	if (bind(listen_fd, (struct sockaddr *)&listen_addr,
		sizeof(listen_addr)) < 0)
	{
		IME_LOG("bind failed");
		return false;
	}
	if (listen(listen_fd, 1024) < 0)
	{
		IME_LOG("listen failed");
		return false;
	}
	IME_LOG( "listening on port[%d]", nPort);

	return true;
}

bool CServer::SetNonBlock()
{
	/* Set the socket to non-blocking, this is essential in event
	 * based programming with libevent. */
	if (evutil_make_socket_nonblocking(listen_fd) < 0)
	{
		IME_LOG( "failed to set server socket to non-blocking");
		return false;
	}
	return true;
}
bool CServer::StartServer(const char * szAddr, int nPort)
{
	if (!StartListen(szAddr,nPort))
	{
		return false;
	}
		
	/* We now have a listening socket, we create a read event to
	 * be notified when a client connects. */
	//important! for the thread safe
	event_set(&ev_accept, listen_fd, EV_READ | EV_PERSIST, OnAccept, this);
	//must do event_set first
	event_base_set(this->evbase,&ev_accept);
	evtimer_set(&ev_timer_stop_deref, OnTimerStopOrDeRef, this);
	event_base_set(this->evbase,&ev_timer_stop_deref);

	if (-1 == event_add(&ev_accept, NULL))
	{
		IME_LOG("event add error accept");
		return false;
	}
	timeval tv;
	evutil_timerclear(&tv);
	//??? 1s timer is enough?
	tv.tv_sec = 1;
	tv.tv_usec = 0;
	if (-1 == evtimer_add(&ev_timer_stop_deref,&tv))
	{
		IME_LOG("evtimer add error");
		return false;
	}

	if (!CreatePidFile())
	{
		return false;
	}

	/* Start the libevent event loop. */
	event_base_loop(this->evbase, 0);



	return true;
}
/**
 * This function will be called by libevent when there is a connection
 * ready to be accepted.
 */
void CServer::OnAccept(evutil_socket_t fd, short ev, void *arg)
{
	CServer * pThis = (CServer *)arg;
	int client_fd;
	sockaddr_in client_addr;
	socklen_t client_len = sizeof(client_addr);
	CNetClient *client;
	
	client_fd = accept(fd, (sockaddr *)&client_addr, &client_len);
	if (client_fd == -1) {
		//todo error
		IME_LOG("accept failed");
		return;
	}
	
	//setsockopt( client_fd, SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT, (char *)&fd, sizeof(fd) );
	
	/* Set the client socket to non-blocking mode. */
	if (evutil_make_socket_nonblocking(client_fd) < 0)
	{
		IME_LOG("failed to set client socket non-blocking");
		return;
	}
	/* We’ve accepted a new client, allocate a client object to
	 * maintain the state of this client. */
	client = new CNetClient(pThis,client_fd); 
	if (client == NULL)
	{
		IME_LOG("new failed for client struct");
		return;
	}
	//for thread safe
	event_set(&client->ev_read, client_fd, EV_READ | EV_PERSIST, OnRead, client );
	event_set(&client->ev_write, client_fd, EV_WRITE, OnWrite, client );
	event_base_set(pThis->evbase,&client->ev_read);
	event_base_set(pThis->evbase,&client->ev_write);
	/* Setting up the event does not activate, add the event so it
	 * becomes active. */
	if( 0 != event_add(&client->ev_read, NULL) ) 
	{
		client->CloseSocket();
		return ;
	}
	
//	if( 0 != event_add(&client->ev_write, NULL) )
//	{
//		client->CloseSocket();
//		return ;
//	}
	ZThread::CountedPtr<CNetClient> ptrNetClient(client);
	//important!
	pThis->RefClient(client,ptrNetClient);	

	IME_LOG("accept %d",client_fd);

	memset(&client_addr, 0, client_len);
	getpeername(client_fd, (struct sockaddr *)&client_addr, &client_len);
	IME_LOG("%s\n", inet_ntoa(client_addr.sin_addr));
	client->SetRemoteIP(inet_ntoa(client_addr.sin_addr));

	client->OnAccept(ptrNetClient,pThis->m_dwNetIdTotal++);
}


/**
 * This function will be called by libevent when the client socket is
 * ready for reading.
 */
void CServer::OnRead(evutil_socket_t fd, short ev, void *arg)
{
	CNetClient *client = (CNetClient *)arg;
	int len;
	
	len = client->GetBufferInput()->SockRead(fd);	

	if (len == 0) {
		IME_LOG("Client(%d) disconnected when read",fd);
		client->CloseSocket();
		client->GetServer()->DeRefClient(client);
		return;
	}
	else if (len < 0 && ( ! IsTryAgain() ) ) {
		IME_LOG("recv(%d) fail, len %d, errno %d", fd, len, EVUTIL_SOCKET_ERROR() );
		client->CloseSocket();
		client->GetServer()->DeRefClient(client);
		return;
	}
	client->OnRead();
	//IME_LOG("client %d recv",fd);
}

/**
 * This function will be called by libevent when the client socket is
 * ready for writing.
 */
void CServer::OnWrite(evutil_socket_t fd, short ev, void *arg)
{
	CNetClient *client = (CNetClient *)arg;
	client->OnWrite();
	int len;
	if (client->GetBufferOutput()->GetSize() <= 0)
		return ;
	unsigned int dwTotalLen = 0;
	if (client->GetBufferOutput()->GetSize() > E_MAX_BUFFER_LEN)
	{
		IME_LOG("Client(%d) max buffer len when write, size=%lu",fd, client->GetBufferOutput()->GetSize());
		client->CloseSocket();
		client->GetServer()->DeRefClient(client);
		return;
	}
	while(client->GetBufferOutput()->GetSize() > 0)
	{
		len = client->GetBufferOutput()->Sends(fd);

		if (len == 0) {
			IME_LOG("Client(%d) disconnected when write",fd);
			client->CloseSocket();
			client->GetServer()->DeRefClient(client);
			return;
		}
		else if (len < 0 && ( !IsTryAgain() ) ) {
			IME_LOG("send(%d) fail, len %d, errno %d", fd, len, EVUTIL_SOCKET_ERROR() );
			client->CloseSocket();
			client->GetServer()->DeRefClient(client);
			return;
		}
		dwTotalLen += len;
		
		if (len < 0 && IsTryAgain())
		{
			client->TryAddWriteEvent();
			break;
		}
	}
//	if (client->GetBufferOutput()->GetSize() > 0)
//	{
//		event_del(&client->ev_write);
//		if( 0 != event_add(&client->ev_write, NULL) )
//		{
//			client->CloseSocket();
//			client->GetServer()->DeRefClient(client);
//			IME_LOG("write event add failed");
//			return ;
//		}
//	}
	//IME_LOG("client %d write len : %u ",fd,dwTotalLen);
}

bool CServer::IsTryAgain()
{
	// thread safe???

#ifdef WIN32
	return WSAEWOULDBLOCK == WSAGetLastError();
#else
	return EAGAIN == errno || EWOULDBLOCK == errno;
#endif

}
void CServer::RefClient(CNetClient * pNetClient, ZThread::CountedPtr<CNetClient> ptrNetClient)
{
	m_clientMap[pNetClient] = ptrNetClient;
	IME_LOG("RefClient %s",pNetClient->GetRemoteIP().c_str());
}
void CServer::DeRefClient(CNetClient * pNetClient)
{
	ClientMap::iterator ii = m_clientMap.find(pNetClient);
	if (ii != m_clientMap.end())
	{
		IME_ERROR("DefClient %s",pNetClient->GetRemoteIP().c_str());
		m_clientMap.erase(ii);
	}
	else
	{
		IME_LOG("nonref client deref!");
		assert(0);
	}

}

void CServer::OnTimerStopOrDeRef(evutil_socket_t fd, short ev, void *arg)
{
	CServer *pServer = (CServer *)arg;
	pServer->CheckCloseSocket();
	if (g_stopEvent)
	{
		pServer->EndServer();
		return ;
	}
	timeval tv;
	evutil_timerclear(&tv);
	tv.tv_sec = 1;
	tv.tv_usec = 0;
	evtimer_add(pServer->GetEvTimer(),&tv);
}

event * CServer::GetEvTimer()
{
	return & ev_timer_stop_deref;
}

void CServer::CheckCloseSocket()
{
	/////////////////////
	// Handle AsyncClose
	/////////////////////
	ClientMap::iterator it = m_clientMap.begin(), nxt;
	while ( it != m_clientMap.end() )
	{   
		nxt = it; ++nxt;

		CNetClient* pClient = &(*it->second);
		CNetClient::CloseAsyncFlagType eFlag = pClient->GetCloseAsyncFlag();

		if ( eFlag == CNetClient::E_CLOSE_ASYNC_FLAG_DIRECT_CLOSE )
		{   
			IME_SYSTEM_LOG( "AsyncCloseSocket", "Direct Close" );
			pClient->CloseSocket();
		}   
		else if ( eFlag == CNetClient::E_CLOSE_ASYNC_FLAG_FLUSH_CLOSE )
		{   
			IME_SYSTEM_LOG( "AsyncCloseSocket", "Flushing" );
			OnWrite( pClient->GetSocketFd(), EV_WRITE, pClient );

			if ( !pClient->IsClose() && pClient->GetBufferOutput()->GetSize() == 0 ) 
			{   
				IME_SYSTEM_LOG( "AsyncCloseSocket", "Close After Flush" );
				pClient->CloseSocket();
			}   
		}   
		it = nxt;
	}

	ClientMap::iterator ii = m_clientMap.begin();
	for(; ii != m_clientMap.end();)
	{
		if (ii->second->IsClose())
		{
			IME_ERROR("CheckCloseSocket");
			//IME_ERROR();
			std::string strTmp = (ii->first)->GetRemoteIP();
			IME_ERROR("remote ip %s",strTmp.c_str());
			m_clientMap.erase(ii++);
			continue;
		}
		else
		{
			++ii;
		}
	}
}

bool CServer::CreatePidFile()
{
	int fd = open("./.pid", O_TRUNC | O_RDWR);
	if (fd == -1)
	{
		fd = open("./.pid", O_CREAT | O_RDWR, 0644);
		if (fd == -1)
		{
			IME_ERROR("CREATE PID FILE 'pid' FAIL");
			return false;
		}
		IME_LOG("CREATE PID FILE SUCC");
	}
	pid_t pid = getpid();
	char sz_pid[64];
	memset(sz_pid, 0, sizeof(sz_pid));
	sprintf(sz_pid, "%d", pid);

	IME_LOG("WRITE PID FILE [pid=%d]", pid);
	if (write(fd, sz_pid, strlen(sz_pid)) == -1)
	{
		IME_ERROR("WRITE PID FILE FAIL");
		return false;
	}
	close(fd);
	return true;
}
