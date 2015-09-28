/*
 * =====================================================================================
 *
 *       Filename:  Item.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2012年09月22日 06时24分35秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  imane (), imanecr@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#ifndef _ITEM_H_
#define _ITEM_H_
#include <map>
#include <stdint.h>
#include "ByteBuffer.h"
#include "Log.h"
#include "stdint.h"

class CRole;
/*
 * =====================================================================================
 *        Class:  CItem
 *  Description:  
 * =====================================================================================
 */
class CItem
{
	public:
		/* ====================  LIFECYCLE     ======================================= */
		CItem ();                             /* constructor */
		~CItem ();
		int Init(CRole *pCRole);

		/* ====================  ACCESSORS     ======================================= */
		typedef struct 
		{	
			uint32_t dwObjId;
			uint32_t dwNum;
//			uint32_t dwEffect;
		} Item_t;

		typedef enum 
		{
			ADD,
			DEL
		} action_t;

		void SetCRole(CRole *pCRole) { m_pCRole = pCRole;};

		/* ====================  OPERATORS     ======================================= */
		bool LoadData();
		int SaveData();
		int SerilizeDB(ByteBuffer &byte);
		int DeSerilizeDB(ByteBuffer &byte);

		void HandlerItemInfo();
		int AddItem(uint32_t objid, int source, uint32_t number = 1);
		int DelItem(uint32_t objid, int source, uint32_t number = 1);

		void ItemUpdate(uint32_t objid, uint8_t action, uint32_t number = 1);
		int JudgeItemNumber(uint32_t objid, uint32_t number = 1);

	protected:
		/* ====================  METHODS       ======================================= */

		/* ====================  DATA MEMBERS  ======================================= */

	private:
		/* ====================  METHODS       ======================================= */

		/* ====================  DATA MEMBERS  ======================================= */
		std::map<uint32_t, Item_t> m_mapItem;
		CRole *m_pCRole;
		bool m_IsInit;
		uint32_t m_dwTotal;
		uint32_t m_dwUsed;

}; /* -----  end of class CItem  ----- */


#endif
