
#ifndef _WORLD_RUNNABLE_H
#define _WORLD_RUNNABLE_H
#include "zthread/ZThread.h"

class CWorldRunnable: public ZThread::Runnable
{
public:
	void run();

};
#endif

