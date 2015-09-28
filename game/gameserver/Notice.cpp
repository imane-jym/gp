/*
 * =====================================================================================
 *
 *       Filename:  Notice.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2013年01月24日 11时24分41秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  imane (), imanecr@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include "Notice.h"
#include "EnumDefines.h"
#include "CmdDefine.h"
#include "World.h"
#include "Role.h"

void hexdump(const char *p, int len);

CNotice::CNotice()
{
	m_userindex = 0;
//	m_userindexvip = 0;
	m_pkg.SetOpcode(CMD_SC_BROAD_CAST);
}

void
CNotice::AddUseEgg (std::string strname, uint32_t CardIndex)
{
	m_pkg.clear();	
	m_pkg << (uint16_t)SYS_BROAD_EGG;
	m_pkg << strname;
	m_pkg << CardIndex;
	AddNotice(m_pkg);
	return ;
}		/* -----  end of method CNotice::AddUseEgg  ----- */

void CNotice::StrengthCard(std::string strname, uint32_t CardIndex, uint32_t Lv)
{
	m_pkg.clear();	
	m_pkg << (uint16_t)SYS_BROAD_STRENGTH_CARD;
	m_pkg << strname;
	m_pkg << CardIndex;
	m_pkg << Lv;
	AddNotice(m_pkg);
	return;
}

void CNotice::OpenSpecialBox(std::string strname, uint32_t BoxIndex, uint32_t Index)
{
	m_pkg.clear();	
	m_pkg << (uint16_t)SYS_BROAD_OPEN_SPECIAL_BOX;
	m_pkg << strname;
	m_pkg << BoxIndex;
	m_pkg << Index;
	AddNotice(m_pkg);
	return;
}

void CNotice::MasterGetSkill(std::string strname, uint32_t BigSkillIndex)
{
	m_pkg.clear();	
	m_pkg << (uint16_t)SYS_BROAD_MASTER_GET_SKILL;
	m_pkg << strname;
	m_pkg << BigSkillIndex;
	AddNotice(m_pkg);
	return;
}

void CNotice::SkillLvUp(std::string strname, uint32_t BigSkillIndex)
{
	m_pkg.clear();	
	m_pkg << (uint16_t)SYS_BROAD_SKILL_LV_UP;
	m_pkg << strname;
	m_pkg << BigSkillIndex;
	AddNotice(m_pkg);
	return;
}

void CNotice::VipOnline(std::string strname, uint32_t userid, uint16_t vip)
{
	m_pkgVip.clear();
	m_pkgVip << (uint16_t)SYS_BROAD_VIP_ONLINE;
	m_pkgVip << strname;
	m_pkgVip << userid;
	m_pkgVip << vip;
//	AddNotice(m_pkgVip);
	m_LastPkgVip = m_pkgVip;
	return;
}

void CNotice::VipHappenBoss(std::string strname, uint32_t userid, uint32_t vip, std::string bossname)
{
	m_pkgVip.clear();
	m_pkgVip << (uint16_t)SYS_BROAD_BOSS_HAPPEN;
	m_pkgVip << strname;
	m_pkgVip << userid;
	m_pkgVip << vip;
	m_pkgVip << bossname;
//	AddNotice(m_pkgVip);
	m_LastPkgVip = m_pkgVip;
	return;
}

void CNotice::HandlerSendSysBroadCast()
{
	if (m_pkg.size() != 0)
	{
		m_pkg.SetOpcode(CMD_SC_BROAD_CAST);
		sSessCtrl->SendPkgToPart(m_pkg, m_userindex, MAX_SYS_BROAD_USER_ONE_TIMES);	
		if (m_pkgVip.size() != 0)
		{
			m_pkgVip.SetOpcode(CMD_SC_BROAD_CAST);
			sSessCtrl->SendPkgToPart(m_pkgVip, m_userindex, MAX_SYS_BROAD_USER_ONE_TIMES);
		}
//		IME_LOG("*********** send sys broad ************");
		m_userindex += MAX_SYS_BROAD_USER_ONE_TIMES;
		if (m_userindex >= sWorld->GetOnlineNum())
		{
			m_pkg.clear();
			m_userindex = 0;
		}
	}
	else
	{
		if (m_pkgVip.size() != 0)
		{
			m_pkgVip.SetOpcode(CMD_SC_BROAD_CAST);
			sSessCtrl->SendPkgToPart(m_pkgVip, m_userindex, MAX_SYS_BROAD_USER_ONE_TIMES);
			m_userindex += MAX_SYS_BROAD_USER_ONE_TIMES;
			if (m_userindex >= sWorld->GetOnlineNum())
			{
				m_pkgVip.clear();
				m_userindex = 0;
			}
		}
	}
//	if (m_pkgVip.size() != 0)
//	{
//		m_pkgVip.SetOpcode(CMD_SC_BROAD_CAST);
//		sWorld->SendPkgToPart(m_pkgVip, m_userindexvip, MAX_SYS_BROAD_USER_ONE_TIMES);	
//		IME_LOG("send sys broad ************");
//		m_userindexvip += MAX_SYS_BROAD_USER_ONE_TIMES;
//		if (m_userindexvip >= sWorld->GetOnlineNum())
//		{
//			m_pkgVip.clear();
//			m_userindexvip = 0;
//		}
//	}
}

void CNotice::MasterGetMaster(std::string strname, uint32_t MasterIndex)
{
	m_pkg.clear();	
	m_pkg << (uint16_t)SYS_BROAD_GET_MASTER_CARD;
	m_pkg << strname;
	m_pkg << MasterIndex;
	AddNotice(m_pkg);
	return;
}

void CNotice::DiyNotice(std::string str)
{
	m_pkg.clear();	
	m_pkg << (uint16_t)SYS_BROAD_DIY;
	m_pkg << str;
	AddNotice(m_pkg);
	return;
}

void CNotice::AddNotice(const WorldPacket &packet)
{
//	packet.SetOpcode(CMD_SC_BROAD_CAST);
	m_lstPkg.push_back(packet);
	if (m_lstPkg.size() > MAX_NOTICE_NUMBER)
	{
		m_lstPkg.pop_front();
	}
}

void CNotice::HandlerBroadCast(CRole *pCRole)
{
//	IME_LOG("broadcast data push!!!");
	std::list<WorldPacket>::iterator it;
	for (it = m_lstPkg.begin(); it != m_lstPkg.end(); it++)
	{
		it->SetOpcode(CMD_SC_BROAD_CAST);
		pCRole->SendPacket(&(*it));
//		IME_LOG("broad cast !!!!");
	}

	if (m_LastPkgVip.size() != 0)
	{
		m_LastPkgVip.SetOpcode(CMD_SC_BROAD_CAST);
		pCRole->SendPacket(&m_LastPkgVip);
//		IME_LOG("vip broad cast !!!!");
	}
}

bool CNotice::Init()
{
	std::string str;
	if (!CDBCtrl::GetGlobalData(DB_GLOBAL_NOTICE, str))
	{
		IME_ERROR("get notice data fail");
		return false;
	}
	ByteBuffer by;
	by.append(str.data(), str.size());
	uint32_t number;
	uint32_t len;
	by >> number;
	IME_DEBUG("number %u", number);
	WorldPacket pkg(CMD_SC_BROAD_CAST);
	for (int i = 0; i < number; i++)
	{
		by >> len;
		pkg.clear();
		std::string temp;
		by.read(temp, len);
		pkg.append(temp.data(), temp.size());
		m_lstPkg.push_back(pkg);
//		hexdump(pkg.contents(), pkg.size());
//		IME_ERROR("len %u", len);
	}
	by >> len;
	pkg.clear();
	std::string temp;
	by.read(temp, len);
	pkg.append(temp.data(), temp.size());
	m_LastPkgVip = pkg;
//	hexdump(pkg.contents(), pkg.size());

	return true;
}

void CNotice::SaveData()
{
	std::string str;
	ByteBuffer by;
	by << (uint32_t)m_lstPkg.size();
	std::list<WorldPacket>::iterator it;
	
//	IME_ERROR("pkg number %u", m_lstPkg.size());

	for (it = m_lstPkg.begin(); it != m_lstPkg.end(); it++)
	{
		by << (uint32_t)it->size();
		by.append(it->contents(), it->size());
//		IME_ERROR("pkg len %u", it->size());
//		hexdump(it->contents(), it->size());
	}
	by << (uint32_t)m_LastPkgVip.size();
//	IME_ERROR("last vip pkg size %u", m_LastPkgVip.size());
	by.append(m_LastPkgVip.contents(), m_LastPkgVip.size());
//	hexdump(m_LastPkgVip.contents(), m_LastPkgVip.size());
	str.assign(by.contents(), by.size());

	CDBCtrl::SaveGlobalData(str, DB_GLOBAL_NOTICE);
//	IME_ERROR("total data");
//	hexdump(str.c_str(), str.length());
}
