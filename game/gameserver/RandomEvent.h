/*
 * =====================================================================================
 *
 *       Filename:  RandomEvent.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2012年09月06日 07时56分59秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  imane (), imanecr@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#ifndef _RANDOMEVENT_H_
#define _RANDOMEVENT_H_

#include <stdint.h>
#include <map>
#include <vector>
#include "ConfigData.h"

/*
 * =====================================================================================
 *        Class:  CRandomEvent
 *  Description:  
 * =====================================================================================
 */
class CRole;

class CRandomEvent
{
	public:
		/* ====================  OPERATORS     ======================================= */
		typedef int (*REventFunc)(
				CRole *pCRole,
				int32_t &para1,
				int32_t &para2,
				int32_t &para3,
				int32_t &para4
				);

		enum REvent {
			EXP_RATE_UP = 1,
			COIN_RATE_UP,
			CARD_DROP,
			ITEM_DROP,
			SHOP_DROP,
			WORLD_EXP_UP
		};				/* ----------  end of enum REvent  ---------- */

		typedef enum REvent REvent;

		static void initialization();

		static REventFunc Find(uint32_t fun)
		{
			return (m_mapFunc.find(fun) == m_mapFunc.end() ? NULL : m_mapFunc[fun]);
		}

		static int REventExpRateUp(
				CRole *pCRole,
				int32_t &para1,
				int32_t &para2,
				int32_t &para3,
				int32_t &para4		
				);

		static int REventCoinRateUp(
				CRole *pCRole,
				int32_t &para1,
				int32_t &para2,
				int32_t &para3,
				int32_t &para4		
				);

		static int REventCardDrop(
				CRole *pCRole,
				int32_t &para1,
				int32_t &para2,
				int32_t &para3,
				int32_t &para4
				);

		static int REventItemDrop(
			CRole *pCRole,
			int32_t &para1,
			int32_t &para2,
			int32_t &para3,		
			int32_t &para4	
			);

		static int REventShopDrop(
			CRole *pCRole,
			int32_t &para1,
			int32_t &para2,
			int32_t &para3,		
			int32_t &para4	
			);

		static int REventWorldExpUp(
			CRole *pCRole,
			int32_t &para1,
			int32_t &para2,
			int32_t &para3,		
			int32_t &para4	
			);
		
//		static int ExecRandomEvent(CRole *pCRole, uint8_t rate, STC_CONF_RANDOM_EVENT &retemp);

		static std::map<uint16_t, REventFunc> m_mapFunc;

}; /* -----  end of class CRandomEvent  ----- */
#endif
