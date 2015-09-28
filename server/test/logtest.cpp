/*
 * =====================================================================================
 *
 *       Filename:  logtest.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2014年03月04日 16时58分08秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  imane (), imanecr@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include <stdio.h>
#include "Log.h"

int main(int argc, char *argv[])
{
	sLog->Initialize();
	sLog->outChat("Let us have a meeting (total %u people)", 3);
//	IME_CHAR_LOG("char %u have a product to sell", 336467);
	IME_LOG("Normal log start");
	for (int i = 0; i < 10000; i++)
	{
		IME_LOG("no is %u then we will change file name", i);
		sLog->LogFileChange();
		sleep(1);
	}
	return 0;
}
