/*
 * =====================================================================================
 *
 *       Filename:  tujian.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2012年11月01日 23时36分13秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  imane (), imanecr@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#ifndef _TUJIAN_H_
#define _TUJIAN_H_

#include <set>

class CRole;

/*
 * =====================================================================================
 *        Class:  CTujian
 *  Description:  
 * =====================================================================================
 */
class CTujian
{
	public:
		/* ====================  LIFECYCLE     ======================================= */
		typedef struct
		{
			uint8_t Rare;
			uint32_t BreakCount[E_CR_MAX];
			uint8_t Status;
		} SDataTujian;

		CTujian ();                             /* constructor */
		int Init(CRole *pCRole);
		bool SaveData();
		bool LoadData();
		int DirtySaveData();
		/* ====================  ACCESSORS     ======================================= */

		/* ====================  OPERATORS     ======================================= */
		bool IsTujianGet(uint32_t index);
		bool AddTujian(uint32_t index, uint8_t rare, uint32_t breaknumber, uint8_t status);
		void HandlerCardTujianInfo();
		void CardTujianUpdate(uint32_t index, uint8_t type);

	private:
		/* ====================  METHODS       ======================================= */

		/* ====================  DATA MEMBERS  ======================================= */
		bool IsInit;
		bool IsUpdate;
		CRole *m_pCRole;
//		std::set<uint32_t> m_setIndexid;
		std::map<uint32_t, SDataTujian> m_mapTujian; 
}; /* -----  end of class Tujian  ----- */


#endif
