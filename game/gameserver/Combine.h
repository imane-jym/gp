/*
 * =====================================================================================
 *
 *       Filename:  Combine.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2012年10月12日 14时03分46秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  imane (), imanecr@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef _COMBINE_H_
#define _COMBINE_H_

#include "WorldPacket.h"
#include <stdint.h>

class CRole;

typedef struct sCombRecord
{
	uint32_t ItemIndex;
	uint32_t Timestamp;
} sCombRecord;

/*
 * =====================================================================================
 *        Class:  CCombine
 *  Description:  
 * =====================================================================================
 */
class CCombine
{
	public:
		/* ====================  LIFECYCLE     ======================================= */

		/* ====================  OPERATORS     ======================================= */
		static void HandlerCardCombineRequest(CRole *pCRole, WorldPacket &pkg);
		static void HandlerCardCombineGet(CRole *pCRole, WorldPacket &pkg);
		static void HandlerCardCombineInfo(CRole *pCRole);
		static void TimerCheck(CRole *pCRole);
		static void HandlerCheckAndInfo(CRole *pCRole);

}; /* -----  end of class CCombine  ----- */

#endif
