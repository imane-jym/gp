/*
 * =====================================================================================
 *
 *       Filename:  Trial.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2013年11月12日 15时30分47秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  imane (), imanecr@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#ifndef _TRIAL_H_
#define _TRIAL_H_

#include <stdint.h>
#include <map>
#include <vector>
#include "WorldPacket.h"

class CRole;
/*
 * =====================================================================================
 *        Class:  CTrial
 *  Description:  
 * =====================================================================================
 */
class CTrial
{
	public:
		/* ====================  LIFECYCLE     ======================================= */
		CTrial ();                             /* constructor */
		int Init(CRole *pCRole);
		void OnTimer5s();
		void SaveData();

		struct STrialData
		{
			uint32_t ChallengeCnt;
			uint32_t FlushTime;
			uint32_t StartTime;
		};

		/* ====================  OPERATORS     ======================================= */
		void HandlerDataInfo();
		void HandlerTrial(WorldPacket &pkg);
		void HandlerEnterNext(WorldPacket &pkg);
		void ChangeTrial(uint32_t index);
		bool ValidChangeTrial(uint32_t index);

	private:
		/* ====================  METHODS       ======================================= */
		void SeriliazeDB(ByteBuffer &by);
		void DeSeriliazeDB(ByteBuffer &by);

		/* ====================  DATA MEMBERS  ======================================= */
		uint32_t m_dwCurrentdwIndex;
		std::map<uint32_t, STrialData> m_mapTrial;

		CRole *m_pCRole;
		uint32_t m_dwCurrentTrigger;
}; /* -----  end of class CTrial  ----- */

#endif
