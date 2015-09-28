/*
 * =====================================================================================
 *
 *       Filename:  Master.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2013年01月04日 10时07分32秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  imane (), imanecr@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef _MASTER_H_
#define _MASTER_H_

#include <stdint.h>
#include "WorldPacket.h"

class CRole;
/*
 * =====================================================================================
 *        Class:  CMaster
 *  Description:  
 * =====================================================================================
 */
class CMaster
{
	public:
		/* ====================  LIFECYCLE     ======================================= */
		typedef struct SMaster
		{
			uint32_t Master;
			uint32_t StartTime;
			uint32_t BlessTime;
			uint32_t BlessNumber;
			uint32_t NumberMax;
			uint32_t TimeCD;
			uint8_t IsAppr;
			uint32_t Exp;
			uint16_t Level;
		} SMaster;

		CMaster ();                             /* constructor */
		int Init(CRole *pCRole);
		void SaveData();
		int LoadConf(uint32_t index = 0);

		/* ====================  OPERATORS     ======================================= */
		void HandlerMasterInfo(uint32_t index = 0);
		void HandlerMasterRequest(WorldPacket &pkg);
		void HandlerMasterYesOrNo(WorldPacket &pkg);
		void HandlerMasterSendRed(WorldPacket &pkg);
		void HandlerMasterBless(WorldPacket &pkg);

		void OnTimer5s();
		
		int Calculate(uint32_t index = 0);
		int AddMasterExp(uint32_t masterindex, uint32_t exp);

		void RequestByShop(uint32_t choose, uint32_t &master, uint32_t &flag);

		uint32_t GetMasterExp(uint32_t masterindex);
	protected:
		/* ====================  METHODS       ======================================= */

		/* ====================  DATA MEMBERS  ======================================= */

	private:
		/* ====================  METHODS       ======================================= */
		void SeriliazeDB(ByteBuffer &by);
		void DeSeriliazeDB(ByteBuffer &by);

		/* ====================  DATA MEMBERS  ======================================= */
		CRole *m_pCRole;
//		uint32_t m_dwMaster;

//		uint32_t m_dwLowStartTime;
//		uint32_t m_dwLowBlessTime;
//		uint32_t m_dwLowBlessNumber;
//		uint32_t m_dwLowNumberMax;
//		uint32_t m_dwLowTimeCD;
		uint32_t m_dwLowFirstTime;

//		uint32_t m_dwMiddleStartTime;
//		uint32_t m_dwMiddleBlessTime;
//		uint32_t m_dwMiddleBlessNumber;
//		uint32_t m_dwMiddleNumberMax;
//		uint32_t m_dwMiddleTimeCD;
		uint32_t m_dwMiddleFirstTime;

//		uint32_t m_dwHighStartTime;
//		uint32_t m_dwHighBlessTime;
//		uint32_t m_dwHighBlessNumber;
//		uint32_t m_dwHighNumberMax;
//		uint32_t m_dwHighTimeCD;
		uint32_t m_dwHighFirstTime;

		bool m_IsInit;

		std::map<uint32_t, SMaster> m_mapMaster;

}; /* -----  end of class CMaster  ----- */


#endif
