
#include "WorldRunnable.h"
#include "SessCtrl.h"
#include "Timer.h"
#include "Defines.h"
#include "Log.h"
#include <stdio.h>

#ifdef WIN32
#define WORLD_SLEEP_CONST 50
#else
#define WORLD_SLEEP_CONST 100         //Is this still needed?? [On linux some time ago not working 50ms]
#endif

extern volatile bool g_stopEvent;

void CWorldRunnable::run()
{

	IME_LOG("WorldRunnable thread start!");
	//todo
	//do some init things here
	time_t realCurrTime = 0;
	time_t realPrevTime = getMSTime();
	time_t prevSleepTime = 0;                               // used for balanced full tick time length near WORLD_SLEEP_CONST
	
	if (!sSessCtrl->InitSettings())
	{
		IME_ERROR("World InitSettings failed");
		return ;
	}
	///- While we have not g_stopEvent, update the world
	while (!g_stopEvent)
	{
		realCurrTime = getMSTime();
		time_t diff;
		if (realPrevTime > realCurrTime)//getMSTime() have limited data range and this is case when it overflow in this tick            
			diff = 0xFFFFFFFFFFFFFFFF - (realPrevTime - realCurrTime);
		else
			diff = realCurrTime - realPrevTime;
		sSessCtrl->Update( diff );
		realPrevTime = realCurrTime;

		// diff (D0) include time of previous sleep (d0) + tick time (t0)
		// we want that next d1 + t1 == WORLD_SLEEP_CONST
		// we can't know next t1 and then can use (t0 + d1) == WORLD_SLEEP_CONST requirement
		// d1 = WORLD_SLEEP_CONST - t0 = WORLD_SLEEP_CONST - (D0 - d0) = WORLD_SLEEP_CONST + d0 - D0
		if (diff <= WORLD_SLEEP_CONST+prevSleepTime)
		{
			prevSleepTime = WORLD_SLEEP_CONST+prevSleepTime-diff;
			ZThread::Thread::sleep(prevSleepTime);
		}
		else
			prevSleepTime = 0;

		//printf("world loop~\n");

	}
	sSessCtrl->AllOffline();
	//todo
	//do some uninit things here
	IME_LOG("WorldRunnable thread exit");
}

