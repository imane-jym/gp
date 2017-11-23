/*
 * Client.h
 *
 *  Created on: 2011-7-12
 *      Author: yq
 */

#ifndef CLIENT_H_
#define CLIENT_H_

#include "Defines.h"
#include "Log.h"
#include "event.h"
#include "zthread/CountedPtr.h"
#include "zthread/FastMutex.h"
#include "zthread/LockedQueue.h"
#include "zthread/Runnable.h"
#include "zthread/Thread.h"

class CSocketBuffer;
class CUserSession;
class WorldPacket;

class CClient
{
public:
	CClient(CUserSession *pSession);
	~CClient();

	bool Init(int nReConnectTime, event_base *evbase);
	bool Connect(const char * szHost, int nPort);
	bool ReConnect();
	bool IsConnected() {return m_bIsConnected;};
	void CloseClient();
	void StartClient();
	void EndClient();

	static void OnRead(evutil_socket_t fd, short ev, void *arg);
	static void OnWrite(evutil_socket_t fd, short ev, void *arg);
	static bool IsTryAgain();

	static void OnTimer(evutil_socket_t fd, short ev, void *arg);


	event * GetEvTimer();
	event_base * GetEvbase() { return evbase;};

	CSocketBuffer * GetBufferInput() {return m_pBufferInput;}
	CSocketBuffer * GetBufferOutput() {return m_pBufferOutput;}
	int GetReConnectTime() {return m_nReConnectTime;};

	void OnRead();
	void OnWrite();

	void SendPacket(WorldPacket *packet);

	int GetdwKey();

private:
	void SendSinglePacket();
	bool SetNonBlock();
public:
	event_base * evbase;
	evutil_socket_t client_fd;
	sockaddr_in server_addr;
	/* The socket accept event. */
	event ev_timer;
	event mReadEvent;
	event mWriteEvent;
private:
	volatile bool m_bIsConnected;

	std::string m_strHost;
	int m_nPort;
	int m_nReConnectTime;

	CSocketBuffer * m_pBufferInput;
	CSocketBuffer * m_pBufferOutput;

	ZThread::LockedQueue<WorldPacket*,ZThread::FastMutex> m_sendQueue;

	ZThread::FastMutex m_mutexSession;
	CUserSession * m_pSession;
};

#endif /* CLIENT_H_ */
