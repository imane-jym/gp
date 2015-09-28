/*
 * =====================================================================================
 *
 *       Filename:  Notice.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2013年01月24日 11时24分47秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  imane (), imanecr@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#ifndef _NOTICE_H_
#define _NOTICE_H_

#include <stdint.h>
#include "WorldPacket.h"
/*
 * =====================================================================================
 *        Class:  CNotice
 *  Description:  
 * =====================================================================================
 */
class CRole;
class CNotice
{
	public:
		/* ====================  LIFECYCLE     ======================================= */
		CNotice ();                             /* constructor */

		bool Init();
		void SaveData();
		/* ====================  ACCESSORS     ======================================= */

		/* ====================  OPERATORS     ======================================= */
		void AddUseEgg(std::string strname, uint32_t CardIndex);
		void StrengthCard(std::string strname, uint32_t CardIndex, uint32_t Lv);
	   	void OpenSpecialBox(std::string strname, uint32_t BoxIndex, uint32_t Index);	
		void MasterGetSkill(std::string strname, uint32_t BigSkillIndex);
		void SkillLvUp(std::string strname, uint32_t BigSkillIndex);
		void MasterGetMaster(std::string strname, uint32_t MasterIndex);
		void VipOnline(std::string strname, uint32_t userid, uint16_t vip);
		void VipHappenBoss(std::string strname, uint32_t userid, uint32_t vip, std::string bossname);
		void DiyNotice(std::string content);

		void AddNotice(const WorldPacket &packet);
		void HandlerBroadCast(CRole *pCRole);
		void HandlerSendSysBroadCast();

	protected:
		/* ====================  METHODS       ======================================= */

		/* ====================  DATA MEMBERS  ======================================= */

	private:
		/* ====================  METHODS       ======================================= */

		/* ====================  DATA MEMBERS  ======================================= */
		WorldPacket m_pkg;
		WorldPacket m_pkgVip;
		std::list<WorldPacket> m_lstPkg;
//		std::list<WorldPacket> m_lstPkgVip;
		uint32_t m_userindex;
//		uint32_t m_userindexvip;
		WorldPacket m_LastPkgVip;

}; /* -----  end of class CNotice  ----- */

#endif
