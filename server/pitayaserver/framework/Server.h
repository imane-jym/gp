
#ifndef _SERVER_H
#define _SERVER_H


#include "event.h"
#include "Defines.h"
#include "zthread/CountedPtr.h"
#include "NetClient.h"
#include <map>



class CServer
{
public:
	enum
	{
		E_MAX_BUFFER_LEN		= 1024 * 1024 * 6,	//6M
	};
	CServer();
	~CServer();
	bool Init();
	bool StartListen(const char * szAddr, int nPort);
	bool StartServer(const char * szAddr, int nPort);
	void EndServer();
	static void OnAccept(evutil_socket_t fd, short ev, void *arg);
	static void OnRead(evutil_socket_t fd, short ev, void *arg);
	static void OnWrite(evutil_socket_t fd, short ev, void *arg);
	static bool IsTryAgain();

	static void OnTimerStopOrDeRef(evutil_socket_t fd, short ev, void *arg);
	
	void RefClient(CNetClient * pNetClient, ZThread::CountedPtr<CNetClient> ptrNetClient);
	void DeRefClient(CNetClient * pNetClient);
	
	void CheckCloseSocket();
	event * GetEvTimer();
	event_base * GetEvbase() { return evbase;};

	bool CreatePidFile();
private:
	bool SetNonBlock();
private:
	event_base * evbase;
	evutil_socket_t listen_fd;
	sockaddr_in listen_addr;
	/* The socket accept event. */
	event ev_accept;
	event ev_timer_stop_deref;
	typedef std::map< CNetClient *, ZThread::CountedPtr<CNetClient> > ClientMap;
	ClientMap m_clientMap;
	
	uint32 m_dwNetIdTotal;

};
#endif 

