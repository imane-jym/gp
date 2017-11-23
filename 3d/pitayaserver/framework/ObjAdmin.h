/*
 * =====================================================================================
 *
 *       Filename:  ObjAdmin.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2013年03月21日 05时00分59秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  imane (), imanecr@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include <stdint.h>
#include <set>
#include "WorldPacket.h"


/*
 * =====================================================================================
 *        Class:  CObjCtrl
 *  Description:  
 * =====================================================================================
 */
#define MAX_ALIVE_TIME 21600
class CStu;
class CObjCtrl
{
	public:
		/* ====================  LIFECYCLE     ======================================= */
		CObjCtrl ();                             /* constructor */
		virtual ~CObjCtrl ();                    

//		typedef class Node
//		{
//			uint32_t key;
//			uint32_t timestamp;
//			std::list<Node_t *>::iterator it;
//			CStu *p;
//		}Node_t;

		/* ====================  ACCESSORS     ======================================= */

		/* ====================  OPERATORS     ======================================= */
		int Init(uint32_t max_capacity);
		// notice: current CStu pointer data may be free after next get data method
		CStu* GetData(uint32_t key);
		void OnTimer();
		void SaveAllData();
		void AddResidentKey(uint32_t key);
		void ClearResidentKey();
		void DataDump();

		virtual CStu* GetInnerData(uint32_t key) = 0;
		virtual bool SaveInnerData(CStu *p) = 0;

	protected:
		/* ====================  METHODS       ======================================= */

		/* ====================  DATA MEMBERS  ======================================= */

	private:
		/* ====================  METHODS       ======================================= */

		/* ====================  DATA MEMBERS  ======================================= */
		uint32_t m_MaxCapacity;
		std::map<uint32_t, CStu*> m_mapData;
		std::list<CStu *> m_listIndex;
		std::set<uint32_t> m_ResidentData;
}; /* -----  end of class CObjCtrl  ----- */


/*
 * =====================================================================================
 *        Class:  CStu
 *  Description:  
 * =====================================================================================
 */
class CStu
{
	public:
		/* ====================  LIFECYCLE     ======================================= */
		CStu ();                             /* constructor */
		virtual ~CStu ();
		/* ====================  ACCESSORS     ======================================= */

		friend class CObjCtrl;
		/* ====================  OPERATORS     ======================================= */

	private:
		/* ====================  METHODS       ======================================= */

		/* ====================  DATA MEMBERS  ======================================= */
		uint32_t key;
		uint32_t timestamp;
//		std::list<CStu *>::iterator it;

}; /* -----  end of class CStu  ----- */

