/*
 * =====================================================================================
 *
 *       Filename:  Tech.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2012年11月03日 02时30分36秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  imane (), imanecr@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#ifndef _TECH_H_
#define _TECH_H_

#include <map>
#include <stdint.h>
#include "WorldPacket.h"
#include "Group.h"

class CRole;

/*
 * =====================================================================================
 *        Class:  CTech
 *  Description:  
 * =====================================================================================
 */
class CTech
{
	public:
		/* ====================  LIFECYCLE     ======================================= */
		CTech ();                             /* constructor */
		enum 
		{
			ATTACK = 1,
			DEFENCE,
			HEALTH,
			COIN,
			EXP
		};

		
		int Init(CRole *pCROle);
		/* ====================  OPERATORS     ======================================= */

		int LoadData();
		int SaveData();
		int DirtySaveData();

		void TechAttack(uint32_t &val);
		void TechDefence(uint32_t &val);
		void TechHealth(uint32_t &val);
		void TechCoin(uint32_t &val);
		void TechExp(uint32_t &val);

		void HandlerTechUp(WorldPacket &pkg);
		void HandlerTechInfo();
		void HandlerGroupTechUp(WorldPacket &pkg, CGroup *pCGroup);

	private:
		/* ====================  METHODS       ======================================= */

		/* ====================  DATA MEMBERS  ======================================= */
		bool IsInit;
		bool IsUpdate;
		CRole *m_pCRole;
		std::map<uint32_t, uint16_t> m_mapTech; 
		
		std::map<uint32_t, int32_t> m_mapEffectVal;

}; /* -----  end of class CTech  ----- */


#endif
