

#ifndef _NET_CLIENT_H
#define _NET_CLIENT_H

#include "event.h"
#include "zthread/ZThread.h"
#include "zthread/FastMutex.h"
#include "zthread/LockedQueue.h"
#include "zthread/Runnable.h"
#include "zthread/Thread.h"
#include "Defines.h"
#include "Handler.h"

class CSocketBuffer;
class CUserSession;
class WorldPacket;
class CServer;
/// Client Packet Header
//struct ClientPktHeader
//{
//    uint16 size;
//    uint16 cmd;
//} __attribute__ ((packed));

/// Server Packet Header
struct ServerPktHeader
{
#ifdef BIG_PACKET_HEADER
    uint32 size;
    uint32 cmd;
#else
    uint16 size;
    uint16 cmd;
#endif
} __attribute__ ((packed));


class CNetClient
{
public:

	enum
	{
		E_MAX_PENDING_PACKETS		= 1000,		//最大pending的包的数目
	};

	enum CloseAsyncFlagType                    
	{                                          
		E_CLOSE_ASYNC_FLAG_NULL         = 0,   
		E_CLOSE_ASYNC_FLAG_DIRECT_CLOSE = 1,   
		E_CLOSE_ASYNC_FLAG_FLUSH_CLOSE  = 2,   
	};                                         

	CNetClient(CServer *pServer, int nFd);
	virtual	~CNetClient();
	
	CSocketBuffer * GetBufferInput() {return m_pBufferInput;}
	CSocketBuffer * GetBufferOutput() {return m_pBufferOutput;}

	virtual void OnAccept(ZThread::CountedPtr<CNetClient> ptrThis, uint32 dwNetId);
	virtual void OnRead();
	virtual void OnWrite();
	
	void SetSession(CUserSession * pSession) { m_pSession = pSession;}
	CUserSession * GetSession() { return m_pSession;}
	
	void SendPacket(WorldPacket* packet);
	void CloseSocket();
	void CloseSocketAsync(CloseAsyncFlagType eFlag); 
	CloseAsyncFlagType GetCloseAsyncFlag();
	bool IsClose();
	CServer * GetServer();
	int GetSocketFd();

	void SetRemoteIP(char * szIP);
	std::string GetRemoteIP() {return m_strRemoteIP;};
	bool TryAddWriteEvent();

public:
	event ev_read;
	event ev_write;
	
	//event_base * evbase;
private:
	void SendSinglePacket();
private:
	CSocketBuffer * m_pBufferInput;
	CSocketBuffer * m_pBufferOutput;

	ZThread::FastMutex m_mutexSession;
	CUserSession * m_pSession;
	
	ZThread::FastMutex m_mutexClose;
	bool m_bIsClose;
	
	CServer * m_pServer;
	int m_sockFd;
	ZThread::LockedQueue<WorldPacket*,ZThread::FastMutex> m_sendQueue;

	std::string m_strRemoteIP;

	ZThread::FastMutex m_mutexCloseAsync; 
	CloseAsyncFlagType m_eCloseAsyncFlag;
};
#endif

