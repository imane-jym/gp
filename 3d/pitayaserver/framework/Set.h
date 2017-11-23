/*
 * =====================================================================================
 *
 *       Filename:  Set.h
 *
 *    Description:  Used to set some parameter
 *
 *        Version:  1.0
 *        Created:  2013年09月22日 21时58分54秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  imane (), imanecr@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef _SETSYSYEM_H_
#define _SETSYSYEM_H_
#include <stdint.h>

class CSet 
{
public:
	static uint32_t ClientActiveTime;           /* client keep alive time default 300 */
	static uint8_t ClientKeepAlive;             /* client keep alive yes(1) or no(0) default yes*/
	static uint8_t ClientSessionRun;            /* client session run yes(1) or no(0) default no*/
	static uint32_t ExecutionTimeInterval;      /* program exec time interval default 20ms*/
	static uint32_t MaxPacketOverLoad;          /* recv queue allow max packet number default 50*/
	static const char *ScriptMainFile;          /* script init file */
};
#endif
