#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#ifdef HAVE_SYS_TIME_H
#include <sys/time.h>
#endif
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/resource.h>
#include <set>

#include "event.h"
#include "evutil.h"
#include "SocketBuffer.h"

#define MAX_EVENT 32768

typedef int evutil_socket_t;

// Default configuration
static const char * gHost = "127.0.0.1";
static int gPort = 6666;
static int gClients = 100;
static int gSendPkgTime = 250;
static int gBeatTime = 30;
static int gPkgLength = 200;
static time_t gStartTime;
static bool gDaemonFlag = false;
event_base * evbase;

typedef struct tagSP_TestClient {
	evutil_socket_t mFd;
	event mReadEvent;
	event mWriteEvent;
	event mTimeOutPkg;
	event mTimeOutBeat;
	int mSendMsgs;
	int mRecvMsgs;
	CSocketBuffer iBuf;
	CSocketBuffer oBuf;
} SP_TestClient;

set<SP_TestClient *> gSetClients;

void set_rlimit()
{
	rlimit rlim;
	rlim.rlim_cur = MAX_EVENT;
    rlim.rlim_max = MAX_EVENT;
    if (setrlimit( RLIMIT_NOFILE, &rlim ) != 0) {
    	sprintf(stderr, "setrlimit failed\n");
        exit(1);
    }
}

void show_usage(const char *program)
{
	printf("Stress Test Tools\n");
	printf("Usage: %s [-h <host>] [-p <port>] [-c <clients>] \n", program);
	printf("\t-h default is %s\n", gHost);
	printf("\t-p default is %d\n", gPort);
	printf("\t-c how many clients, default is %d\n", gClients);
	printf("\t-t the interval time between two pkgs, default is %d ms\n", gSendPkgTime);
	printf("\t-b the interval time between two beats, default is %d s\n", gBeatTime);
	printf("\t-l the length of one pkg, default is %d bytes\n", gPkgLength);
	printf("\t-d start as a daemon, default is false\n");
	printf("\n");
}

int is_try_again()
{
	return EINTR == errno || EAGAIN == errno;
}

void close_client(SP_TestClient * client)
{
	event_del(&client->mReadEvent);
	event_del(&client->mWriteEvent);
	event_del(&client->mTimeOutBeat);
	event_del(&client->mTimeOutPkg);
	set<SP_TestClient *>::iterator ii = gSetClients.find(client);
	if (ii != gSetClients.end())
	{
		gSetClients.erase(ii);
	}
	gClient--;
	delete client;
}

void unpack(SP_TestClient *client)
{
	u_short remaining = 0;

	while(1)
	{
		if (!remaining)
        {
            if (client->iBuf.GetSize() < 2)
                break;
            // obtain the length of package
			u_short hdr = *((u_short *)client->iBuf.GetBuffer());
			remaining = hdr;
        }
        if (client->iBuf.GetSize() < remaining)
            break;
        // erase this package from SocketBuffer
        client->iBuf.Erase(remaining);
        remaining = 0;
		client->mRecvMsgs++;
	}
}

void on_read(evutil_socket_t fd, short events, void *arg)
{
	SP_TestClient * client = (SP_TestClient *) arg;
	int i = 0;

	// read event
	if(EV_READ & events) {
		int len = client->iBuf.SockRead(client->mFd,-1);
		if(len < 0) {
			if(!is_try_again()) {
				fprintf(stderr, "#%d on_read error, errno %d\n", fd, EVUTIL_SOCKET_ERROR());
				close_client(client);
			}
		} else if (len == 0){
			fprintf(stderr, "#%d remote server close\n", fd);
			close_client(client);
		} else {
			unpack(client);
		}
	} else {
		fprintf(stderr, "#%d on_read timeout\n", fd);
		close_client(client);
	}
}

void on_write(evutil_socket_t fd, short events, void *arg)
{
	SP_TestClient * client = (SP_TestClient *) arg;
	int len = 0;

	if(EV_WRITE & events) {
		if (client->oBuf.GetSize() <= 0)
			return;
		len = client->oBuf.Sends(fd);
		if(len <= 0 && !is_try_again()) {
			fprintf(stderr, "#%d on_write error, errno %d\n", fd, EVUTIL_SOCKET_ERROR());
			close_client(client);
		}
	} else {
		fprintf(stderr, "#%d on_write timeout\n", fd);
		close_client(client);
	}
}

void timeout_cb_pkg(int fd, short events, void *arg)
{
	SP_TestClient * client = (SP_TestClient *) arg;
	timeval tv;
	static char buf[512];

	int pkglen = (rand() % gPkgLength + 1) % 512;

	u_short head = pkglen + 2 * sizeof(u_short);
	u_short cmd  = rand() % 10000 + 1;

	// package length (2 bytes)
	// cmd code (2 bytes)
	// package
	client->oBuf.WriteToBuffer((char *)&head,sizeof(u_short));
	client->oBuf.WriteToBuffer((char *)&cmd,sizeof(u_short));
	client->oBuf.WriteToBuffer(buf,pkglen);
	client->mSendMsgs++;

	evutil_timerclear(&tv);
	tv.tv_sec = 0;
	tv.tv_usec = gSendPkgTime * 1000;
	evtimer_add(&client->mTimeOutPkg, &tv);
}

void timeout_cb_beat(int fd, short events, void *arg)
{
	return;
}

void set_timer(SP_TestClient * client)
{
	// set send package event
	timeval tv;
	evtimer_set(&client->mTimeOutPkg, timeout_cb_pkg, client);
	evutil_timerclear(&tv);
	tv.tv_sec = 0;
	tv.tv_usec = gSendPkgTime * 1000;
	evtimer_add(&client->mTimeOutPkg, &tv);
}

void print_status(int fd, short events, void *arg)
{
	printf("status:----------------------\n");

	int nTotalSend = 0;
	int nTotalRecv = 0;
	time_t tNow;
	time(&tNow);
	set<SP_TestClient *>::iterator ii;
	for(ii = gSetClients.begin(); ii != gSetClients.end(); ++ii)
	{
		nTotalSend += (*ii)->mSendMsgs;
		nTotalRecv += (*ii)->mRecvMsgs;
	}
	printf("Total pkgs send: %d \n",nTotalSend);
	printf("Average pkgs send: %d /s\n", (int)(nTotalSend * 1.0 / (tNow - gStartTime)));
	printf("\n");
	printf("Total pkgs recv: %d \n",nTotalRecv);
	printf("Average pkgs recv: %d /s\n", (int)(nTotalRecv * 1.0 / (tNow - gStartTime)));
	printf("\n");
	printf("Current connections: %d\n",gSetClients.size());
	printf("end:-------------------------\n\n");

	// iterate adding print event
	timeval tv;
	event * ev = (event *) arg;

	evtimer_set(ev,print_status, ev);
	evutil_timerclear(&tv);
	tv.tv_sec = 3;
	evtimer_add(ev, &tv);
}

void set_status_timer()
{
	// set print event: print status per 3 second
	timeval tv;
	event * ev = (event *) calloc(1,sizeof(event));
	evtimer_set(ev, print_status, ev);
	evutil_timerclear(&tv);
	tv.tv_sec = 3;
	evtimer_add(ev, &tv);

	// TODO: add beat event
}

void parse_arg(int argc, char * argv[])
{
	// parse arg
	// -h host
	// -p port
	// -c # of clients
	// -t time interval of two package
	// -b beat interval of two package
	// -l package length
	// -d daemon flag
	int i = 0;
	for ( i = 1; i < argc; i+=2 ) {
		if (0 == strcmp(argv[i], "-h")) {
			gHost = argv[i+1];
		} else if (0 == strcmp(argv[i], "-p")) {
			gPort = atoi( argv[i+1] );
		} else if (0 == strcmp(argv[i], "-c")) {
			gClients = atoi(argv[i+1]);
		} else if (0 == strcmp(argv[i], "-t")) {
			gSendPkgTime = atoi(argv[i+1]);
		} else if (0 == strcmp(argv[i], "-b")) {
			gBeatTime = atoi(argv[i+1]);
		} else if (0 == strcmp(argv[i], "-l")) {
			gPkgLength = atoi(argv[i+1]);
		} else if (0 == strcmp(argv[i], "-d")) {
			gDaemonFlag = true;
		} else {
			show_usage(argv[0]);
			exit(0);
		}
	}
}

int daemon_init()
{
	int	i;
	pid_t pid;
	if ((pid = fork()) < 0)
		return -1;
	else if(pid)
		_exit(0);/* parent terminates */

	/* child 1 continues... */
	if (setsid() < 0)/* become session leader */
		return -1;

	signal(SIGHUP, SIG_IGN);
	if ((pid = fork()) < 0)
		return -1;
	else if (pid)
		_exit(0);/* child 1 terminates */

	/* close off file descriptors */
	for (i = 0 ; i < 64 ; i ++)
		close(i);

	/* redirect stdin, stdout, and stderr to /dev/null */
	open("/dev/null", O_RDONLY);
	open("/dev/null", O_RDWR);
	open("/dev/null", O_RDWR);

	return 0;/* success */
}

int main(int argc, char * argv[])
{
	// deal with main() arguments
	parse_arg(argc, argv);

	// set daemon
	if (gDaemonFlag && (daemon_init() == -1)) {
		fprintf(stderr, "daemon start failed\n");
		exit(1);
	}

	signal(SIGPIPE, SIG_IGN);
	set_rlimit();
	gSetClients.clear();
	time(&gStartTime);
	evbase = event_init();

	// set print status event
	set_status_timer();

	// set socket
	sockaddr_in sin;
	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = inet_addr(gHost);
	sin.sin_port = htons(gPort);

	printf( "Try to create %d connections to server, it will take some minutes to complete.\n", gClients );
	for (int i = 0; i < gClients; i++) {
		SP_TestClient * client = new SP_TestClient;
		client->mRecvMsgs = client->mSendMsgs = 0;
		client->iBuf.Init();
		client->oBuf.Init();
		client->mFd = socket(AF_INET, SOCK_STREAM, 0);

		// if some socket failed
		if (client->mFd < 0) {
			fprintf(stderr, "#%d, socket failed, errno %d\n", i, EVUTIL_SOCKET_ERROR());
			delete client;
			continue;
		}

		if (connect(client->mFd, (struct sockaddr *)&sin, sizeof(sin)) != 0) {
			fprintf(stderr, "#%d, connect failed, errno %d\n", i, EVUTIL_SOCKET_ERROR());
			delete client;
			continue;
		}

	    /* Set the socket to non-blocking, this is essential in event
		 * based programming with libevent. */
		if (evutil_make_socket_nonblocking(client->mFd) < 0) {
			fprintf(stderr, "#%d, failed to set client socket to non-blocking, errno %d\n", i, EVUTIL_SOCKET_ERROR() );
			delete client;
			continue;
		}

		gSetClients.insert(client);
		set_timer(client);

		// add on_read/on_write event
		event_set(&client->mReadEvent, client->mFd, EV_READ | EV_PERSIST, on_read, client);
		event_set(&client->mWriteEvent, client->mFd, EV_WRITE | EV_PERSIST, on_write, client);

		// for thread_safe
		event_base_set(evbase, &client->mReadEvent);
		event_base_set(evbase, &client->mWriteEvent);

		if (0 != event_add(&client->mReadEvent, NULL)) {
			close_client(client);
			continue;
		}
		if (0 != event_add(&client->mWriteEvent, NULL)) {
			close_client(client);
			continue;
		}
	}
	printf("start dispatch\n");

	// start the libevent event loop.
	event_dispatch();
	printf("end dispatch\n");
	return 0;
}


