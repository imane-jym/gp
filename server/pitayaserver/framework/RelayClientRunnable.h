/*
 * RelayClientRunnable.h
 *
 *  Created on: 2011-7-13
 *      Author: yq
 */

#ifndef RELAYCLIENTRUNNABLE_H_
#define RELAYCLIENTRUNNABLE_H_

#include "zthread/ZThread.h"


class CRelayClientRunnable: public ZThread::Runnable
{
public:
	void run();
private:

};

#endif /* RELAYCLIENTRUNNABLE_H_ */
