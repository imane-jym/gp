/*
 * =====================================================================================
 *
 *       Filename:  Group.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2012年11月29日 14时16分56秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  imane (), imanecr@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdint.h>
#include <vector>
#include <algorithm>
#include "Group.h"
#include "EnumDefines.h"
#include "CmdDefine.h"
#include "Role.h"
#include "ConfigData.h"
#include "World.h"
#include "EnumDefines.h"
#include "CmdDefine.h"
#include "ErrorMsg.h"
#include "Task.h"
#include "RandomEvent.h"
#include "Tech.h"
#include "Role.h"

uint32_t GetNextTimeEveryDay(uint8_t hour, time_t timing)
{
	time_t timeset = timing;
	struct tm stcTm;
	localtime_r(&timeset, &stcTm);
	stcTm.tm_min = 0;
	stcTm.tm_sec = 0;
	stcTm.tm_hour = hour;
	time_t nexttime = mktime(&stcTm);
	if (nexttime <= timing)
		return nexttime + 24 * 3600;
	return nexttime;
}

CGroup::CGroup()
{
	SetdwId(0);
	SetstrName("");
	SetdwCreateId(0);
	SetstrCreateName("");
	SetdwCredit(0);
	SetwLevel(0);
	SetwPersionLimit(0);
	SetbyPosion1Limit(0);
	SetbyPosion2Limit(0);
	SetbyPosion3Limit(0);
	SetbyPosion4Limit(0);
	SetvecPosion1Persion(std::vector<AnyType>());
	SetvecPosion1PersionStr(std::vector<AnyType>());
	SetvecPosion2Persion(std::vector<AnyType>());
	SetvecPosion3Persion(std::vector<AnyType>());
	SetvecPosion4Persion(std::vector<AnyType>());
	SetvecPosion5Persion(std::vector<AnyType>());
	SetwPersionNumber(0);
	SetstrNotice("");
	SetdwNeedCredit(0);
	SetdwCreditLimit(0);
	SetdwMissionFlushTime(0);
	SetbyMissionFlushNumber(0);
	SetdwMissionFlushPoint(0);

	m_byFlag = 0;
}

bool
CGroup::Init (const CDBCtrl::SGroup &group)
{
	SetdwCreateId(group.dwUserId);	
	SetdwId(group.dwGroupId);
	m_byFlag = group.byFlag;
	SetstrCreateName(group.strCreateName);
	SetstrName(group.strGroupName);
	SetdwCredit(group.dwCredit);
	SetwLevel(group.wLevel);
	if (group.strMember == "") // for create new group
	{
		SGroupMember tmpMember;
		tmpMember.dwUserId = group.dwUserId;
		tmpMember.dwCredit = 0;
		tmpMember.byPosion = E_GA_PRESIDENT;
		tmpMember.dwCreditSupport = 0;
		m_mapUserGroup[group.dwUserId] = tmpMember;

		SetwPersionNumber(1);
		SetdwMissionFlushPoint(time(NULL));
		uint32_t remain = GetNextTimeEveryDay(GlobalConfig::FlashMission, m_dwMissionFlushPoint) - m_dwMissionFlushPoint; 
		SetdwMissionFlushTime(remain);
	}
	else
	{
		ByteBuffer tmp,tmpMember;
		tmp.append(group.strContent.data(), group.strContent.size());
		tmpMember.append(group.strMember.data(), group.strMember.size());
		if (DeserializeDB(tmp, tmpMember))
		{
			IME_ERROR("DeserializeDB fail userid %u, groupid %u", group.dwUserId, group.dwGroupId); 
			return false;
		}

		time_t now = time(NULL);
		if (now >= GetNextTimeEveryDay(GlobalConfig::FlashMission, m_dwMissionFlushPoint)) 
		{
			SetdwMissionFlushPoint(now);
			SetbyMissionFlushNumber(0);
			MissionFlush();
		}
		uint32_t remain = GetNextTimeEveryDay(GlobalConfig::FlashMission, m_dwMissionFlushPoint) - m_dwMissionFlushPoint;
		SetdwMissionFlushTime(remain);
	}
	if (LoadConf())
	{
		IME_ERROR("loadconf fail userid %u, groupid %u", group.dwUserId, group.dwGroupId); 
		return false;
	}
	if (Calculate())
	{
		IME_ERROR("calculate fail userid %u, groupid %u", group.dwUserId, group.dwGroupId);
		return false;
	}

	IME_LOG("group id %u, group name %s, create userid %u create name %s "
			"credit %u, level %u, persion limit %u,", 
			m_dwId, m_strName.c_str(), m_dwCreateId, m_strCreateName.c_str(), m_dwCredit, m_wLevel, m_wPersionLimit);
	for (int i = 0; i < m_vecPosion5Persion.size(); i++)
	{
		IME_LOG("%u userid %u ", i, m_vecPosion5Persion[i].get<uint32_t>());
	}
	IME_LOG("persion number %u, strNotice %s, need credit %u, credit limit %u ",
			m_wPersionNumber, m_strNotice.c_str(), m_dwNeedCredit, m_dwCreditLimit);
	std::map<uint32_t, SGroupMember>::iterator it;
	for (it = m_mapUserGroup.begin(); it != m_mapUserGroup.end(); it++)
	{
		IME_LOG("userid %u, credit %u, position %u", it->first, it->second.dwCredit, it->second.byPosion);
	}
	return true;
}		/* -----  end of method CGroup::Init  ----- */


//int
//CGroup::LoadData ()
//{
//	return ;
//}		/* -----  end of method CGroup::LoadData  ----- */


int
CGroup::SaveData ()
{
	std::string content;
	std::string member;
	ByteBuffer tmp,tmp2;
	if (SerializeDB(tmp, tmp2))
	{
		IME_ERROR("serializedb fail userid %u, groupid %u", GetdwCreateId(), GetdwId());
		return -1;
	}
	content.assign((const char *)tmp.contents(), tmp.size());
	member.assign((const char *)tmp2.contents(), tmp2.size());

	CDBCtrl::SaveGroup(GetdwCreateId(), GetdwId(),  content, m_byFlag, m_strCreateName, m_strName, m_dwCredit, m_wLevel, member);
	return 0;
}		/* -----  end of method CGroup::SaveData  ----- */

int
CGroup::SerializeDB (ByteBuffer &pkg, ByteBuffer &member)
{
	std::vector<uint8_t> vecEnums;
	vecEnums.push_back(E_GROUP_vecPosion1Persion);
	vecEnums.push_back(E_GROUP_vecPosion1PersionStr);
	vecEnums.push_back(E_GROUP_vecPosion2Persion);
	vecEnums.push_back(E_GROUP_vecPosion3Persion);
	vecEnums.push_back(E_GROUP_vecPosion4Persion);
	vecEnums.push_back(E_GROUP_vecPosion5Persion);
	vecEnums.push_back(E_GROUP_wPersionNumber);
	vecEnums.push_back(E_GROUP_strNotice);
	vecEnums.push_back(E_GROUP_dwMissionFlushTime);
	vecEnums.push_back(E_GROUP_byMissionFlushNumber);
	vecEnums.push_back(E_GROUP_dwMissionFlushPoint);
	saveDataNeed(pkg, vecEnums);
	clearDirtyFlag();

	member << (uint32_t) m_mapUserGroup.size();
	std::map<uint32_t, SGroupMember>::iterator it = m_mapUserGroup.begin();
	std::map<uint32_t, uint8_t>::iterator itmission;
	for ( ; it != m_mapUserGroup.end(); it++)
	{
		member << (it->second).dwUserId;
	   	member << (it->second).dwCredit;	
	   	member << (it->second).byPosion;	
		member << (uint32_t)(it->second).mapMission.size();
		for (itmission = (it->second).mapMission.begin(); itmission != (it->second).mapMission.end(); itmission++)
		{
			member << itmission->first;
			member << itmission->second;
		}
		member << (it->second).dwCreditSupport;
	}

	return 0;
}		/* -----  end of method CGroup::SerializeDB  ----- */

int
CGroup::DeserializeDB (ByteBuffer &pkg, ByteBuffer &member)
{
	loadDataLoose(pkg);
	clearDirtyFlag();
	SetvecPosion1Persion(getProperty(E_GROUP_vecPosion1Persion).get<std::vector<AnyType> >());
	SetvecPosion1PersionStr(getProperty(E_GROUP_vecPosion1PersionStr).get<std::vector<AnyType> >());
	SetvecPosion2Persion(getProperty(E_GROUP_vecPosion2Persion).get<std::vector<AnyType> >());
	SetvecPosion3Persion(getProperty(E_GROUP_vecPosion3Persion).get<std::vector<AnyType> >());
	SetvecPosion4Persion(getProperty(E_GROUP_vecPosion4Persion).get<std::vector<AnyType> >());
	SetvecPosion5Persion(getProperty(E_GROUP_vecPosion5Persion).get<std::vector<AnyType> >());
	SetwPersionNumber(getProperty(E_GROUP_wPersionNumber).get<uint16_t>());
	SetstrNotice(getProperty(E_GROUP_strNotice).get<std::string>());
	SetdwMissionFlushTime(getProperty(E_GROUP_dwMissionFlushTime).get<uint32_t>());
	SetbyMissionFlushNumber(getProperty(E_GROUP_byMissionFlushNumber).get<uint8_t>());
	SetdwMissionFlushPoint(getProperty(E_GROUP_dwMissionFlushPoint).get<uint32_t>());

	uint32_t number;
	uint32_t numbermission;
	uint32_t missionkey;
	uint8_t missionstep;
	member >> number;
	SGroupMember SGMtmp;
	for(int i = 0; i < number; i++)
	{
		member >> SGMtmp.dwUserId;
		member >> SGMtmp.dwCredit;	
		member >> SGMtmp.byPosion;	
		member >> numbermission;
		for (int j = 0; j < numbermission; j++)
		{
			member >> missionkey;
			member >> missionstep;

			SGMtmp.mapMission[missionkey] = missionstep;
		}
		member >> SGMtmp.dwCreditSupport;
		m_mapUserGroup[SGMtmp.dwUserId] = SGMtmp;
	}
	return 0;
}		/* -----  end of method CGroup::DeserializeDB  ----- */

void CGroup::PackProUpdate(WorldPacket &pkg)
{
	pkg.SetOpcode(CMD_SC_GROUP_PROPERTY);
	saveDataDirty(pkg);
	clearDirtyFlag();
}

void CGroup::PackGroupInfo(WorldPacket &pkg)
{
	std::vector<unsigned char> vecEnums;
	vecEnums.push_back(E_GROUP_dwId);
	vecEnums.push_back(E_GROUP_strName);
	vecEnums.push_back(E_GROUP_dwCreateId);
	vecEnums.push_back(E_GROUP_strCreateName);
	vecEnums.push_back(E_GROUP_dwCredit);
	vecEnums.push_back(E_GROUP_wLevel);
	vecEnums.push_back(E_GROUP_wPersionLimit);
	vecEnums.push_back(E_GROUP_byPosion1Limit);
	vecEnums.push_back(E_GROUP_byPosion2Limit);
	vecEnums.push_back(E_GROUP_byPosion3Limit);
	vecEnums.push_back(E_GROUP_byPosion4Limit);
	vecEnums.push_back(E_GROUP_vecPosion1Persion);
	vecEnums.push_back(E_GROUP_vecPosion1PersionStr);
	vecEnums.push_back(E_GROUP_vecPosion2Persion);
	vecEnums.push_back(E_GROUP_vecPosion3Persion);
	vecEnums.push_back(E_GROUP_vecPosion4Persion);
	vecEnums.push_back(E_GROUP_vecPosion5Persion);
	vecEnums.push_back(E_GROUP_wPersionNumber);
	vecEnums.push_back(E_GROUP_strNotice);
	vecEnums.push_back(E_GROUP_dwNeedCredit);
	vecEnums.push_back(E_GROUP_dwCreditLimit);
	vecEnums.push_back(E_GROUP_dwMissionFlushTime);
	vecEnums.push_back(E_GROUP_byMissionFlushNumber);
	vecEnums.push_back(E_GROUP_dwMissionFlushPoint);

	pkg.SetOpcode(CMD_SC_GROUP_INFO);
	saveDataNeed(pkg, vecEnums);
	clearDirtyFlag();
}

int CGroup::Calculate()
{
	STC_CONF_GROUP_INFO *pConf = CConfGroupInfo::Find(GetwLevel());
	if (pConf == NULL)
	{
		IME_ERROR("can not find data in group info csv index %u", GetwLevel());
		return -1;
	}
	SetdwNeedCredit(pConf->dwNeedCredit);
	SetdwCreditLimit(pConf->dwCreditLimit);
	SetwPersionLimit(pConf->wPersionLimit);
	SetbyPosion1Limit(pConf->position1Persion);
	SetbyPosion2Limit(pConf->position2Persion);
	SetbyPosion3Limit(pConf->position3Persion);
	SetbyPosion4Limit(pConf->position4Persion);
	return 0;
}

int CGroup::LoadConf()
{
	return 0;
}

void CGroup::PackGroupMemberInfo(uint32_t userid, WorldPacket &info)
{
	info.SetOpcode(CMD_SC_GROUP_MEMBER_INFO);
	std::map<uint32_t, SGroupMember>::iterator it = m_mapUserGroup.find(userid);
	std::map<uint32_t, uint8_t>::iterator itmission;
	if (it != m_mapUserGroup.end())
	{
		info << (it->second).dwUserId;
		info << (it->second).dwCredit;
		info << (it->second).byPosion;	

		info << (uint32_t)(it->second).mapMission.size();
		for (itmission = (it->second).mapMission.begin(); itmission != (it->second).mapMission.end(); itmission++)
		{
			info << itmission->first;
			info << itmission->second;
		}

		info << (it->second).dwCreditSupport;
	}
	else
	{
		info << (uint32_t) 0;
		info << (uint32_t) 0;
		info << (uint8_t) 0;

		info << (uint32_t) 0;

		info << (uint32_t) 0;
	}
}	

void CGroup::SendAllGroupData(CRole *pCRole)
{
	WorldPacket info;
	PackGroupInfo(info);	
	pCRole->SendPacket(&info);
	info.clear();

	PackGroupMemberInfo(pCRole->GetdwAccountId(), info);
	pCRole->SendPacket(&info);
}

void CGroup::SendDataToAllMember(WorldPacket *pkg)
{
	std::map<uint32_t, SGroupMember>::iterator it;
	CRole *pCRole;
	for (it = m_mapUserGroup.begin(); it != m_mapUserGroup.end(); it++)
	{
		pCRole = sWorld->FindUserRole(it->first);
		if(pCRole != NULL)
		{
			pCRole->SendPacket(pkg);
		}
	}	
}

int CGroup::ChangeCredit(int32 credit)
{
	if (credit < 0)
	{
		if (GetdwCredit() < -credit)
		{
			SetdwCredit(m_dwCredit + credit);
			return 0;
		}
		else
		{
			return -1;
		}

	}
	else
	{
			if (m_dwCredit + credit > m_dwCreditLimit)
			{
				SetdwCredit(m_dwCreditLimit);
			}
			else
			{
				SetdwCredit(m_dwCredit + credit);
			}
	}
	return 0;
}

void CGroup::HandlerLevelUp(CRole *pCRole, WorldPacket &pkg)
{
	STC_CONF_GROUP_INFO *pUp = CConfGroupInfo::Find(GetwLevel() + 1);
	if (pUp == NULL)
	{
		pCRole->SendErrorMsg(ERRNO_MSG_ALREADY_FULL_LEVEL, CMD_SC_GROUP_LEVELUP_RESULT);
		return;
	}
	if (GetdwCredit() < GetdwNeedCredit())
	{
		pCRole->SendErrorMsg(ERRNO_MSG_CREDIT_NOT_ENOUGH, CMD_SC_GROUP_LEVELUP_RESULT);
		return;
	}
	SGroupMember *pSGroupMember = GetGroupMember(pCRole->GetdwAccountId());
	// admin verify
	if (!CConfGroupAdmin::GetVal(pSGroupMember->byPosion, E_GA_LEVELUP))
	{
		pCRole->SendErrorMsg(ERRNO_MSG_GROUP_NOT_AUTH, CMD_SC_GROUP_LEVELUP_RESULT);
		return;
	}
	
//	SetdwCredit(m_dwCredit - m_dwNeedCredit);	
	
	ChangeCredit(-m_dwNeedCredit);
	SetwLevel(m_wLevel + 1);

	if (Calculate())
	{
		pCRole->SendErrorMsg(ERRNO_MSG_CALCULATE, CMD_SC_GROUP_LEVELUP_RESULT);
		return;

	}
	// notify all group member
	WorldPacket info;
	PackProUpdate(info);
	SendDataToAllMember(&info);
	info.clear();

	info.SetOpcode(CMD_SC_GROUP_LEVELUP_RESULT);
	info << (uint16_t) 0;
	pCRole->SendPacket(&info);
	return;
}

void CGroup::HandlerMailInfo(CRole *pCRole, WorldPacket &pkg)
{
	uint32_t off;
	pkg >> off;

	std::vector<CDBCtrl::SGroupMail> vec;
	uint64 tmpid;
	tmpid = MAKE_FIND_GROUPID(GetdwCreateId(), GetdwId());
	CDBCtrl::GetGroupMailInfo(tmpid, vec, off);

	ByteBuffer blob;
	uint8_t mailtype;
	uint32_t indexid;
	uint32_t userid;
	uint8_t position;

	std::string str;
	std::string str_tmp;

	WorldPacket info(CMD_SC_GROUP_MAIL_INFO_RESULT);
	info << (uint16_t) 0;
	info << (uint32_t) vec.size();
	for (int i = 0; i < vec.size(); i++)
	{
		info << (uint32_t) vec[i].dwTime;
		blob.clear();
		blob.append(vec[i].blob.data(), vec[i].blob.size());
		info << (uint32_t) vec[i].dwMailId;
		info << (uint8_t) vec[i].byDoflag;
		mailtype = vec[i].byDetailType;
		if (mailtype > E_GDT_MAIL_DELETE)
		{
			IME_ERROR("can not verify this mail type");
			pCRole->SendErrorMsg(ERRNO_MSG_MAIL_NOT_VERIFY, CMD_SC_GROUP_MAIL_INFO_RESULT);
			return;
		}
		info << mailtype;
		switch(mailtype)
		{
			case E_GDT_MAIL_REQUEST_ENTER:
				info << vec[i].content;
				blob >> userid;
				blob >> indexid;
				blob >> str;
				info << userid;
				info << indexid;
				info << str;
				break;
			case E_GDT_MAIL_AGREE_ENTER:
				blob >> indexid;
				blob >> position; 
				blob >> str;
				blob >> str_tmp;
				info << indexid;
				info << position;
				info << str;
				info << str_tmp;
				break;
			case E_GDT_MAIL_CANCEL:
				blob >> indexid;
				blob >> str;
				info << indexid;
				info << str;
				break;
			case E_GDT_MAIL_DELETE:
				blob >> indexid;
				blob >> position; 
				blob >> str;
				blob >> str_tmp;
				info << indexid;
				info << position;
				info << str;
				info << str_tmp;
				break;
defatult:
				break;
		}
	}
	pCRole->SendPacket(&info);
	return ;
}

void CGroup::CreateSGroupMemberNormal(SGroupMember &SGMtmp, uint32_t userid)
{
	SGMtmp.byPosion = E_GA_NORMAL;
	SGMtmp.dwCredit = 0;
	SGMtmp.dwUserId = userid;
	SGMtmp.mapMission.clear();
}

void CGroup::HandlerMailOpt(CRole *pCRole, WorldPacket &pkg)
{
	uint64 group_id;
	uint32_t mail_id;
	uint8_t flag;
	group_id = MAKE_FIND_GROUPID(GetdwCreateId(), GetdwId()); 
	pkg >> mail_id;
	pkg >> flag;
	IME_LOG("group_id %llu, mail_id %u", group_id, mail_id);
	
	CDBCtrl::SGroupMail tempMailDetail;
	if (!CDBCtrl::GetGroupMailInfoSingle(group_id, mail_id, tempMailDetail))
	{
		IME_ERROR("get mail data fail");
		pCRole->SendErrorMsg(ERRNO_MSG_DB_NO_DATA_OR_ERROR, CMD_SC_GROUP_MAIL_OPT_RESULT);
		return;
	}
	if (flag == 0)
	{
		IME_ERROR("nothing to do email groupid %llu, mail_id %u, opt %u", group_id, mail_id, flag);
		pCRole->SendErrorMsg(ERRNO_MSG_DATA_ILLEGAL, CMD_SC_GROUP_MAIL_OPT_RESULT);
		return;
	}

	ByteBuffer blob;
	blob.append(tempMailDetail.blob.data(), tempMailDetail.blob.size());
	uint32_t userid;
	uint32_t index;
	std::string username;
	if (tempMailDetail.byDetailType == E_GDT_MAIL_REQUEST_ENTER)
	{	
		if (tempMailDetail.byDoflag != 0)
		{
			IME_ERROR("the mail have been handled groupid %u mailid %u", group_id, mail_id);
			pCRole->SendErrorMsg(ERRNO_MSG_MAIL_ALREADY_HANDLE, CMD_SC_GROUP_MAIL_OPT_RESULT);
			return;
		}

		SGroupMember *pSGroupMember = GetGroupMember(pCRole->GetdwAccountId());
		if (!CConfGroupAdmin::GetVal(pSGroupMember->byPosion, E_GA_MEMBER_IO))
		{
			pCRole->SendErrorMsg(ERRNO_MSG_GROUP_NOT_AUTH, CMD_SC_GROUP_MAIL_OPT_RESULT);
			return;
		}

		if (GetwPersionNumber() >= GetwPersionLimit())
		{
			IME_ERROR("overload persion limit now %u limit %u", GetwPersionNumber(), GetwPersionLimit());
			pCRole->SendErrorMsg(ERRNO_MSG_GROUP_PERSION_OVERLOAD, CMD_SC_GROUP_MAIL_OPT_RESULT);
			return;
		}
		blob >> userid >> index >> username;
		if (sWorld->GetCGroupByUserId(userid) != NULL)
		{
			IME_ERROR("this user %u have joined group", userid);
			pCRole->SendErrorMsg(ERRNO_MSG_ALREADY_IN_GROUP, CMD_SC_GROUP_MAIL_OPT_RESULT);
			return;
		}
		

		if (!CDBCtrl::UpdateGroupMail(group_id, mail_id, flag))
		{
			IME_ERROR("update mail fail from not do to do groupid %u,mail %u", group_id, mail_id);
			pCRole->SendErrorMsg(ERRNO_MSG_DB_NO_DATA_OR_ERROR, CMD_SC_GROUP_MAIL_OPT_RESULT);
			return;
		}

		if (flag == 1)
		{
			IME_LOG("userid %u want to join group %llu", userid, group_id);
			SetwPersionNumber(m_wPersionNumber + 1);
			m_vecPosion5Persion.push_back(AnyType(userid));
			SetvecPosion5Persion(m_vecPosion5Persion);
			SGroupMember SGMtmp;
			CreateSGroupMemberNormal(SGMtmp, userid);
			m_mapUserGroup[userid] = SGMtmp;	

			SendMailAgreeEnter(pCRole, username);

			WorldPacket updatepkg;
			PackProUpdate(updatepkg);
			SendDataToAllMember(&updatepkg);
		}	
		else
		{
		}
	}
	else
	{
			IME_ERROR("mail cannot be verified groupid %u,mail %u, detailtype", group_id, mail_id, tempMailDetail.byDetailType);
			pCRole->SendErrorMsg(ERRNO_MSG_MAIL_NOT_VERIFY, CMD_SC_GROUP_MAIL_OPT_RESULT);
			return;

	}

	WorldPacket info(CMD_SC_GROUP_MAIL_OPT_RESULT);
	info << (uint16_t) 0;
	info << group_id;
	info << mail_id;
	info << flag;
	pCRole->SendPacket(&info);

	return ;
}

void CGroup::MissionFlush()
{
	std::map<uint32_t, SGroupMember>::iterator it;
	for (it = m_mapUserGroup.begin(); it != m_mapUserGroup.end(); it++)
	{
		(it->second).mapMission.clear();
	}
}

void CGroup::OnTimer1s()
{
	time_t now = time(NULL);
	if (now >= GetNextTimeEveryDay(GlobalConfig::FlashMission, m_dwMissionFlushPoint)) 
	{
		SetdwMissionFlushPoint(now);
		SetbyMissionFlushNumber(0);
		MissionFlush();
		uint32_t remain = GetNextTimeEveryDay(GlobalConfig::FlashMission, m_dwMissionFlushPoint) - m_dwMissionFlushPoint;
		SetdwMissionFlushTime(remain); 

		WorldPacket pkg;
		PackProUpdate(pkg);
		SendDataToAllMember(&pkg);
	}
}

       
void CGroup::HandlerAcceptMission(CRole *pCRole, WorldPacket &pkg)
{
	/* 
	uint32_t taskindex;
	pkg >> taskindex;

	if (taskindex == 0)
	{
		IME_ERROR("taskindex is unvalid 0");
		pCRole->SendErrorMsg(ERRNO_MSG_DATA_ILLEGAL, CMD_SC_GROUP_MISSION_ACCEPT_RESULT);
		return;
	}

	STC_CONF_GROUP_MISSION *p = CConfGroupMission::Find(taskindex);
	if (p == NULL)
	{
		IME_ERROR("load group mission conf fail taskindex %u", taskindex);
		pCRole->SendErrorMsg(ERRNO_MSG_CSV_LOAD, CMD_SC_GROUP_MISSION_ACCEPT_RESULT);
		return;
	}

	if (GetwLevel() < p->wLevelLimit)
	{
		IME_ERROR("this mission taskindex %u then levellimit %u, now role level", taskindex, GetwLevel(), pCRole->GetwLevel());
		pCRole->SendErrorMsg(ERRNO_MSG_MISSION_LEVEL_NOT_ARRIVE, CMD_SC_GROUP_MISSION_ACCEPT_RESULT);
		return;
	}


	STC_CONF_RANDOM_EVENT *q = NULL;
	if (p->dwRandomEventIndex != 0)
	{
		q = CConfRandomEvent::Find(p->dwRandomEventIndex);
		if (q == NULL)
		{
			IME_ERROR("load random conf fail index %u", p->dwRandomEventIndex);
			pCRole->SendErrorMsg(ERRNO_MSG_CSV_LOAD, CMD_SC_GROUP_MISSION_ACCEPT_RESULT);
			return;

		}	
	}

	
	uint32_t tempenergy = pCRole->GetdwEnergy();
	if (pCRole->GetdwEnergy() < p->wConsumeEnergy)
	{
		//todo something about resume
//		IME_ERROR("energy not enough taskindex %u", taskindex);
		//todo format wrong
		WorldPacket pkg(CMD_SC_GROUP_MISSION_ACCEPT_RESULT);
		pkg << (uint16_t) ERROR_MSG_NOT_ENOUGH_ENERGY;
		pkg << (uint32_t) 0;
		pkg << (uint32_t) 0;
		pCRole->SendPacket(&pkg);
		return;
	}

	
	uint32_t step;
	SGroupMember *pSGM = GetGroupMember(pCRole->GetdwAccountId());
	if (pSGM->mapMission.find(taskindex) != pSGM->mapMission.end())
	{
		step = pSGM->mapMission[taskindex]; 
	}
	else
	{
		step = pSGM->mapMission[taskindex] = 0;
	}

	if (step >= p->byStepLimit)
	{
		IME_ERROR("userid %u have done group mission", pCRole->GetdwAccountId());
		pCRole->SendErrorMsg(ERRNO_MSG_GROUP_MISSION_OVERLOAD, CMD_SC_GROUP_MISSION_ACCEPT_RESULT);
		return;
	}

	STC_CONF_RANDOM_EVENT retemp;
	if (step + 1 >= p->byStepLimit)
	{
		if (p->dwBoss != 0)
		{
			CBoss *pCBoss = new(std::nothrow) CBoss();
			assert(pCBoss != NULL);
			int ret = ProcessBoss(pCRole, pCBoss, p->dwIndex, CMD_SC_MISSION_BRANCH_DURATION_RESULT, p->byDurationCmd);
			int ret = 0;
			delete pCBoss;
			if (ret == -1)
			{
				pCRole->SendErrorMsg(ERRNO_MSG_CSV_LOAD, CMD_SC_GROUP_MISSION_ACCEPT_RESULT);
				return;
			}
			else if (ret == 1)
			{
				pCRole->ChangeEnergy(-(p->wConsumeEnergy));
				pCRole->SendProUpdate();
				pCRole->SendErrorMsg(ERRNO_MSG_MISSION_DURATION_LOSE, CMD_SC_GROUP_MISSION_ACCEPT_RESULT);
				return;
			}
		}
		step += 1;
		
		pCRole->ChangeEnergy(-(p->wConsumeEnergy));
		pSGM->dwCredit += CConfGroupAdmin::GetVal(pSGM->byPosion, E_GA_CREDIT);

		pSGM->dwCreditSupport += GlobalConfig::GroupHonourPlus;
		ChangeCredit(GlobalConfig::GroupHonourPlus);

		WorldPacket info;
		PackProUpdate(info);
		SendDataToAllMember(&info);
		
	}
	else
	{
		pCRole->ChangeEnergy(-(p->wConsumeEnergy));
		step += 1;
	}
	
	pSGM->mapMission[taskindex] = step;

//	{
//		IME_ERROR("save data in db fail userid %u, taskindex %u, time %u, step %u",pCRole->GetdwAccountId(), taskindex, mytime, step);
//		pCRole->SendErrorMsg(ERRNO_MSG_DB, CMD_SC_GROUP_MISSION_ACCEPT_RESULT);
//		return;
//	}

	retemp.para1 = 0;
	retemp.para2 = 0;
	retemp.para3 = 0;
	retemp.para4 = 0;
	if (q != NULL)
	{
		IME_LOG("mission event %u, random type %u", p->dwRandomEventIndex, q->wType);
		if (CUtil::RandEither(p->byRate, 100)) // random event happen
		{
			memcpy(&retemp, q, sizeof(STC_CONF_RANDOM_EVENT));
			CRandomEvent::initialization();
			CRandomEvent::REventFunc HandlerRandomEventFunc;
			HandlerRandomEventFunc = CRandomEvent::Find(q->wType);
			if (HandlerRandomEventFunc == NULL)
			{
				IME_ERROR("no this random type %u", q->wType);
				pCRole->SendErrorMsg(ERRNO_MSG_CSV_DATA, CMD_SC_GROUP_MISSION_ACCEPT_RESULT);
				return;
			}
			else
			{
				if (HandlerRandomEventFunc(pCRole, retemp.para1, retemp.para2, retemp.para3, retemp.para4))
				{
					IME_ERROR("random handler fail");
					pCRole->SendErrorMsg(ERRNO_MSG_CSV_DATA, CMD_SC_GROUP_MISSION_ACCEPT_RESULT);
					return;
				}
			}
		}	
	}

	uint32_t tcoin = p->wGetCoin;
	IME_LOG("add coin %u", tcoin);
	pCRole->GetclsTech()->TechCoin(tcoin);
	pCRole->ChangeCoin(tcoin);
	pCRole->SendProUpdate();
	IME_LOG("energy %u energylimit %u", pCRole->GetdwEnergy(), pCRole->GetdwEnergyMax());

	pSGM->dwCredit += p->wGetCredit;
	WorldPacket info;
	PackGroupMemberInfo(pCRole->GetdwAccountId(), info);
	pCRole->SendPacket(&info);
	info.clear();

	info.SetOpcode(CMD_SC_GROUP_MISSION_ACCEPT_RESULT);
	info << (uint16_t) 0;
	info << (uint32_t) taskindex;
	info << (uint32_t) step;
	info << retemp.para1;
	info << retemp.para2;
	info << retemp.para3;
	info << retemp.para4;

	IME_LOG("ret %u %u %u %u", retemp.para1, retemp.para2, retemp.para3, retemp.para4);
	pCRole->SendPacket(&info);
	return;
	*/
}

void CGroup::HandlerRequestPrize(CRole *pCRole, WorldPacket &pkg)
{
	uint32_t itemindex;
	pkg >> itemindex;
	STC_CONF_GROUP_PRIZE *p = CConfGroupPrize::Find(itemindex);
	if (p == NULL)
	{
		IME_ERROR("can not find this item [%u] in group prize csv", itemindex);
		pCRole->SendErrorMsg(ERRNO_MSG_CSV_LOAD ,CMD_SC_GROUP_REQUSET_PRIZE_RESULT);
		return;
	}

	SGroupMember *pSGM = GetGroupMember(pCRole->GetdwAccountId());

	if (pSGM->dwCredit < p->dwConsumeCredit)
	{
		pCRole->SendErrorMsg(ERRNO_MSG_GROUP_CREDIT_NOT_ENGOUH, CMD_SC_GROUP_REQUSET_PRIZE_RESULT);
		return;
	}


	if (p->byType == E_OBJ_CARD)
	{
		if (CConfCardCollection::Find(p->dwCombineId)->size() == 0)
		{
			IME_ERROR("can not find this combineid [%u] in cardcollection csv", p->dwCombineId);
			pCRole->SendErrorMsg(ERRNO_MSG_CSV_LOAD ,CMD_SC_GROUP_REQUSET_PRIZE_RESULT);
			return;
		}

		int32_t para1 = p->dwCombineId;
		int32_t para2 = 0;
		int32_t para3 = 0;
		int32_t para4 = 0;
		CRandomEvent::REventCardDrop(pCRole, para1, para2, para3, para4); 
	}
	else if (p->byType == E_OBJ_ITEM)
	{

		if (CConfItemCollection::Find(p->dwCombineId)->size() == 0)
		{
			IME_ERROR("can not find this combineid [%u] in itemcollection csv", p->dwCombineId);
			pCRole->SendErrorMsg(ERRNO_MSG_CSV_LOAD ,CMD_SC_GROUP_REQUSET_PRIZE_RESULT);
			return;
		}

		int32_t para1 = p->dwCombineId;
		int32_t para2 = 0;
		int32_t para3 = 0;
		int32_t para4 = 0;
		CRandomEvent::REventItemDrop(pCRole, para1, para2, para3, para4); 
	}
	else if (p->byType == E_OBJ_SHOP)
	{

	}
	else
	{
		pCRole->SendErrorMsg(ERRNO_MSG_DATA_ILLEGAL, CMD_SC_GROUP_REQUSET_PRIZE_RESULT);
	}

	pSGM->dwCredit -= p->dwConsumeCredit;
	WorldPacket groupmemberpkg;
	PackGroupMemberInfo(pCRole->GetdwAccountId(), groupmemberpkg);
	pCRole->SendPacket(&groupmemberpkg);

	WorldPacket info(CMD_SC_GROUP_REQUSET_PRIZE_RESULT);
	info << (uint16_t) 0;
	pCRole->SendPacket(&info);
	return;
}

void CGroup::HandlerTechUpdate(CRole *pCRole, WorldPacket &pkg)
{
	pCRole->GetclsTech()->HandlerGroupTechUp(pkg, this);	
}

typedef struct
{
	uint32_t userid;
	std::string name;
	uint16_t level;
	uint32_t credit;
	uint32_t attack_max;
	uint32_t attack_min;
	uint32_t defence_max;
	uint32_t defence_min;
} SMember;


void CGroup::HandlerMemberInfo(CRole *pCRole, WorldPacket &pkg)
{
	uint32_t off;
	pkg >> off;

	std::map<uint32_t, SGroupMember>::iterator it;
	int i = 0;
	int j = 10;
	std::vector<SMember> vec;
	SMember temp;

	for(it = m_mapUserGroup.begin(); it != m_mapUserGroup.end(); it++)
	{

		if (i >= off)
		{
			uint8_t IsOnline = true;	
			CRole *pRole = sWorld->FindUserRole(it->first);
			if (pRole == NULL)
			{
				pRole = new(std::nothrow) CRole(NULL);
				assert(pRole != NULL);
				if (!pRole->LoadDataForDuration(it->first))
				{
					IME_ERROR("CRole init fail %u", it->first);
					pCRole->SendErrorMsg(ERRNO_MSG_DB_NO_DATA_OR_ERROR, CMD_SC_GROUP_MEMBER_INFO_RESULT);
					delete pRole;
					return;
				}
				IsOnline = false;
			}

			temp.userid = pRole->GetdwAccountId();
			temp.name = pRole->GetstrUserName();
			temp.level = pRole->GetwLevel();
			temp.credit = (it->second).dwCredit;
			vec.push_back(temp);

			if(!IsOnline)
			{
				delete pRole;
			}


			j--;
			if (j <= 0)
				break;
		}
		i++;
	}

	WorldPacket info(CMD_SC_GROUP_MEMBER_INFO_RESULT);
	info << (uint16_t) 0;
	info << (uint32_t) vec.size();
   	for(int i = 0; i < vec.size(); i++)
	{
		info << vec[i].userid;
		info << vec[i].name;
		info << vec[i].level;
		info << vec[i].credit;
	}	
	pCRole->SendPacket(&info);
}

void CGroup::HandlerChat(CRole *pCRole, WorldPacket &pkg)
{
	std::string str;
	std::string name;
	pkg >> str;
	pkg >> name;
	
	WorldPacket info(CMD_SC_GROUP_CHAT_MSG_UDPATE);
	info << str;
	info << name;
	SendDataToAllMember(&info);	

	info.clear();
	info.SetOpcode(CMD_SC_GROUP_CHAT_RESULT);
	info << (uint16_t) 0;
	pCRole->SendPacket(&info);
}

void CGroup::HandlerAppoint(CRole *pCRole, WorldPacket &pkg)
{
	uint32_t userid;
	std::string username;
	uint8_t position;
	pkg >> userid >> username >> position;
	
	SGroupMember *pSGMPeer = GetGroupMember(userid);	
	if (pSGMPeer == NULL)
	{
		IME_ERROR("userid %u is not in this group", userid);
		pCRole->SendErrorMsg(ERRNO_MSG_ALREADY_NOT_IN_GROUP, CMD_SC_GROUP_APPOINT_RESULT);
		return;
	}
	
	if (std::find(m_vecPosion1Persion.begin(), m_vecPosion1Persion.end(), AnyType(userid)) != m_vecPosion1Persion.end() || 
		std::find(m_vecPosion2Persion.begin(), m_vecPosion2Persion.end(), AnyType(userid)) != m_vecPosion2Persion.end() ||
		std::find(m_vecPosion3Persion.begin(), m_vecPosion3Persion.end(), AnyType(userid)) != m_vecPosion3Persion.end() ||
		std::find(m_vecPosion4Persion.begin(), m_vecPosion4Persion.end(), AnyType(userid)) != m_vecPosion4Persion.end() || userid == GetdwCreateId())
	{
		IME_ERROR("userid %u is already position", userid);
		pCRole->SendErrorMsg(ERRNO_MSG_GROUP_ALREADY_POSITION, CMD_SC_GROUP_APPOINT_RESULT);
		return;
	}
	SGroupMember *pSGM = GetGroupMember(pCRole->GetdwAccountId());	
	uint32_t admin = CConfGroupAdmin::GetVal(pSGM->byPosion, E_GA_APPOINT);
	if (admin > position || admin == 0)
	{
		IME_ERROR("userid %u have this positon %u admin %u", pCRole->GetdwAccountId(), position, admin); 
		pCRole->SendErrorMsg(ERRNO_MSG_GROUP_NOT_AUTH, CMD_SC_GROUP_APPOINT_RESULT);
		return;
	}

	switch (position)
	{
		case E_GA_VICE_PRESIDENT:
			if (m_vecPosion1Persion.size() >= GetbyPosion1Limit())
			{
				IME_ERROR("overload up limit now %u, limit %u", m_vecPosion1Persion.size(), GetbyPosion1Limit());
				pCRole->SendErrorMsg(ERRNO_MSG_GROUP_POSITION_NUMBER_LIMIT, CMD_SC_GROUP_APPOINT_RESULT);
				return;
			}
			m_vecPosion1Persion.push_back(userid);
			m_vecPosion1PersionStr.push_back(AnyType(username, true));
			SetvecPosion1Persion(m_vecPosion1Persion);
			SetvecPosion1PersionStr(m_vecPosion1PersionStr);
			pSGMPeer->byPosion = E_GA_VICE_PRESIDENT;
			break;
		case E_GA_INNER_MINISTER:
			if (m_vecPosion2Persion.size() >= GetbyPosion2Limit())
			{
				IME_ERROR("overload up limit now %u, limit %u", m_vecPosion2Persion.size(), GetbyPosion2Limit());
				pCRole->SendErrorMsg(ERRNO_MSG_GROUP_POSITION_NUMBER_LIMIT, CMD_SC_GROUP_APPOINT_RESULT);
				return;
			}
			m_vecPosion2Persion.push_back(userid);
			SetvecPosion2Persion(m_vecPosion2Persion);
			pSGMPeer->byPosion = E_GA_INNER_MINISTER;
			break;
		case E_GA_OUT_MINISTER:
			if (m_vecPosion3Persion.size() >= GetbyPosion3Limit())
			{
				IME_ERROR("overload up limit now %u, limit %u", m_vecPosion3Persion.size(), GetbyPosion3Limit());
				pCRole->SendErrorMsg(ERRNO_MSG_GROUP_POSITION_NUMBER_LIMIT, CMD_SC_GROUP_APPOINT_RESULT);
				return;
			}
			m_vecPosion3Persion.push_back(userid);
			SetvecPosion3Persion(m_vecPosion3Persion);
			pSGMPeer->byPosion = E_GA_OUT_MINISTER;
			break;
		case E_GA_ELITE:
			if (m_vecPosion4Persion.size() >= GetbyPosion4Limit())
			{
				IME_ERROR("overload up limit now %u, limit %u", m_vecPosion4Persion.size(), GetbyPosion4Limit());
				pCRole->SendErrorMsg(ERRNO_MSG_GROUP_POSITION_NUMBER_LIMIT, CMD_SC_GROUP_APPOINT_RESULT);
				return;
			}
			m_vecPosion4Persion.push_back(userid);
			SetvecPosion4Persion(m_vecPosion4Persion);
			pSGMPeer->byPosion = E_GA_ELITE;
			break;
		default:
			IME_ERROR("this position %u can not appoint", position);
			pCRole->SendErrorMsg(ERRNO_MSG_GROUP_UNVALID_POSITION, CMD_SC_GROUP_APPOINT_RESULT);
			return;
	}

	std::vector<AnyType>::iterator it = std::find(m_vecPosion5Persion.begin(), m_vecPosion5Persion.end(), AnyType(userid));
	if (it != m_vecPosion5Persion.end())
	{
		m_vecPosion5Persion.erase(it);
	}
	SetvecPosion5Persion(m_vecPosion5Persion);

	CRole *pPeerCRole = sWorld->FindUserRole(userid); 
	if (pPeerCRole != NULL)
	{
		WorldPacket memberpkg;
		PackGroupMemberInfo(userid, memberpkg);
		pPeerCRole->SendPacket(&memberpkg);
	}

	WorldPacket grouppkg;
	PackProUpdate(grouppkg);
	SendDataToAllMember(&grouppkg);
	
	WorldPacket info(CMD_SC_GROUP_APPOINT_RESULT);
	info << (uint16_t) 0;
	info << userid;
	info << username;
	info << position;
	pCRole->SendPacket(&info);
}

void CGroup::HandlerUnAppoint(CRole *pCRole, WorldPacket &pkg)
{
	uint32_t userid;
	std::string username;
	uint8_t position;
	pkg >> userid >> username >> position;

	SGroupMember *pSGMPeer = GetGroupMember(userid);	
	if (pSGMPeer == NULL)
	{
		IME_ERROR("userid %u is not in this group", userid);
		pCRole->SendErrorMsg(ERRNO_MSG_ALREADY_NOT_IN_GROUP, CMD_SC_GROUP_UNAPPOINT_RESULT);
		return;
	}
	
	SGroupMember *pSGM = GetGroupMember(pCRole->GetdwAccountId());	
	uint32_t admin = CConfGroupAdmin::GetVal(pSGM->byPosion, E_GA_APPOINT);
	if (admin > position || admin == 0 || userid == GetdwCreateId())
	{
		IME_ERROR("userid %u have this positon %u admin %u or unappoint create id", pCRole->GetdwAccountId(), position, admin); 
		pCRole->SendErrorMsg(ERRNO_MSG_GROUP_NOT_AUTH, CMD_SC_GROUP_UNAPPOINT_RESULT);
		return;
	}
	
	std::vector<AnyType>::iterator it;
	switch (position)
	{
		case E_GA_VICE_PRESIDENT:
			it = std::find(m_vecPosion1Persion.begin(), m_vecPosion1Persion.end(), AnyType(userid));		
			if (it == m_vecPosion1Persion.end())
			{
				pCRole->SendErrorMsg(ERRNO_MSG_GROUP_NOT_IN_POSITION, CMD_SC_GROUP_UNAPPOINT_RESULT);
				return;
			}
			m_vecPosion1Persion.erase(it);
			m_vecPosion1PersionStr.erase(m_vecPosion1PersionStr.begin() + int(it-m_vecPosion1Persion.begin()));
			SetvecPosion1Persion(m_vecPosion1Persion);
			SetvecPosion1PersionStr(m_vecPosion1PersionStr);
			break;
		case E_GA_INNER_MINISTER:
			it = std::find(m_vecPosion2Persion.begin(), m_vecPosion2Persion.end(), AnyType(userid));		
			if (it == m_vecPosion2Persion.end())
			{
				pCRole->SendErrorMsg(ERRNO_MSG_GROUP_NOT_IN_POSITION, CMD_SC_GROUP_UNAPPOINT_RESULT);
				return;
			}
			m_vecPosion2Persion.erase(it);
			SetvecPosion2Persion(m_vecPosion2Persion);
			break;
		case E_GA_OUT_MINISTER:
			it = std::find(m_vecPosion3Persion.begin(), m_vecPosion3Persion.end(), AnyType(userid));		
			if (it == m_vecPosion3Persion.end())
			{
				pCRole->SendErrorMsg(ERRNO_MSG_GROUP_NOT_IN_POSITION, CMD_SC_GROUP_UNAPPOINT_RESULT);
				return;
			}
			m_vecPosion3Persion.erase(it);
			SetvecPosion3Persion(m_vecPosion3Persion);
			break;
		case E_GA_ELITE:
			it = std::find(m_vecPosion4Persion.begin(), m_vecPosion4Persion.end(), AnyType(userid));		
			if (it == m_vecPosion4Persion.end())
			{
				pCRole->SendErrorMsg(ERRNO_MSG_GROUP_NOT_IN_POSITION, CMD_SC_GROUP_UNAPPOINT_RESULT);
				return;
			}
			m_vecPosion4Persion.erase(it);
			SetvecPosion4Persion(m_vecPosion4Persion);
			break;
		default:
			IME_ERROR("this position %u can not appoint", position);
			pCRole->SendErrorMsg(ERRNO_MSG_GROUP_UNVALID_POSITION, CMD_SC_GROUP_UNAPPOINT_RESULT);
			return;
	}

	m_vecPosion5Persion.push_back(userid);
	SetvecPosion5Persion(m_vecPosion5Persion);

	pSGMPeer->byPosion = E_GA_NORMAL;
	CRole *pPeerCRole = sWorld->FindUserRole(userid); 
	if (pPeerCRole != NULL)
	{
		WorldPacket memberpkg;
		PackGroupMemberInfo(userid, memberpkg);
		pPeerCRole->SendPacket(&memberpkg);
	}

	WorldPacket grouppkg;
	PackProUpdate(grouppkg);
	SendDataToAllMember(&grouppkg);
	
	WorldPacket info(CMD_SC_GROUP_UNAPPOINT_RESULT);
	info << (uint16_t) 0;
	info << userid;
	info << username;
	info << position;
	pCRole->SendPacket(&info);
}

void CGroup::HandlerMemberSearchInfo(CRole *pCRole, WorldPacket &pkg)
{
	std::string str;
	pkg >> str;

	std::map<uint32_t, SGroupMember>::iterator it;
	int j = 10;
	std::vector<SMember> vec;
	SMember temp;

	for(it = m_mapUserGroup.begin(); it != m_mapUserGroup.end(); it++)
	{

			uint8_t IsOnline = true;	
			CRole *pRole = sWorld->FindUserRole(it->first);
			if (pRole == NULL)
			{
				pRole = new(std::nothrow) CRole(NULL);
				assert(pRole != NULL);
				if (!pRole->LoadDataForDuration(it->first))
				{
					IME_ERROR("CRole init fail %u", it->first);
					pCRole->SendErrorMsg(ERRNO_MSG_DB_NO_DATA_OR_ERROR, CMD_SC_GROUP_MEMBER_SEARCH_INFO_RESULT);
					delete pRole;
					return;
				}
				IsOnline = false;
			}
			
			if (pRole->GetstrUserName() == str)
			{
				temp.userid = pRole->GetdwAccountId();
				temp.name = pRole->GetstrUserName();
				temp.level = pRole->GetwLevel();
				temp.credit = (it->second).dwCredit;

				if(!IsOnline)
				{
					delete pRole;
				}

				vec.push_back(temp);

				j--;
				if (j <= 0)
					break;
			}
			else
			{
				if(!IsOnline)
				{
					delete pRole;
				}

			}
	}

	WorldPacket info(CMD_SC_GROUP_MEMBER_SEARCH_INFO_RESULT);
	info << (uint16_t) 0;
	info << (uint32_t) vec.size();
   	for(int i = 0; i < vec.size(); i++)
	{
		info << vec[i].userid;
		info << vec[i].name;
		info << vec[i].level;
		info << vec[i].credit;
	}	
	pCRole->SendPacket(&info);
}

void CGroup::HandlerModifyInfo(CRole *pCRole, WorldPacket &pkg)
{
	std::string str;
	pkg >> str;
	
		
	SGroupMember *pSGM = GetGroupMember(pCRole->GetdwAccountId());
	if (!CConfGroupAdmin::GetVal(pSGM->byPosion, E_GA_UPDATE_NOTICE))
	{
		pCRole->SendErrorMsg(ERRNO_MSG_GROUP_NOT_AUTH, CMD_SC_GROUP_MODIFY_INFO_RESULT);
		return;
	}

	SetstrNotice(str);
	WorldPacket info;
	PackProUpdate(info);
	SendDataToAllMember(&info);

	info.clear();
	info.SetOpcode(CMD_SC_GROUP_MODIFY_INFO_RESULT);
	info << (uint16_t) 0;
	pCRole->SendPacket(&info);
}

int CGroup::SendMailRequestEnter(CRole *pCRole, std::string str)
{
	uint64 groupid = MAKE_FIND_GROUPID(pCRole->GetdwAccountId(), GetdwId());
	uint32_t mailid = CDBCtrl::GetGroupMailId(groupid);
	CDBCtrl::SGroupMail SGMtmp;
	SGMtmp.lGroupId = groupid;
	SGMtmp.dwTime = time(NULL);
	SGMtmp.dwMailId = mailid;
	SGMtmp.content = str;
	SGMtmp.byType = E_GT_MAIL_SYSTEM;
	SGMtmp.byDoflag = 0;
	SGMtmp.byDetailType = E_GDT_MAIL_REQUEST_ENTER;

	ByteBuffer blob;
	blob << pCRole->GetdwAccountId();
	blob << E_GT_MAIL_REQUEST_ENTER_INDEX;
	blob << pCRole->GetstrUserName();
	SGMtmp.blob.assign((const char *)blob.contents(), blob.size());

	if (!CDBCtrl::InsertGroupMail(SGMtmp))
	{
		IME_ERROR("insert data fail groupid %llu, %u", groupid, pCRole->GetdwAccountId());
		return -1;
	}
	return 0;
}

int CGroup::SendMailAgreeEnter(CRole *pCRole, std::string str)
{
	uint64 groupid = MAKE_FIND_GROUPID(pCRole->GetdwAccountId(), GetdwId());
	uint32_t mailid = CDBCtrl::GetGroupMailId(groupid);
	CDBCtrl::SGroupMail SGMtmp;
	SGMtmp.lGroupId = groupid;
	SGMtmp.dwTime = time(NULL);
	SGMtmp.dwMailId = mailid;
	SGMtmp.content = "";
	SGMtmp.byType = E_GT_MAIL_SYSTEM;
	SGMtmp.byDoflag = 1;
	SGMtmp.byDetailType = E_GDT_MAIL_AGREE_ENTER;

	SGroupMember *pSGM = GetGroupMember(pCRole->GetdwAccountId());
	if (pSGM == NULL)
		return -1;
	ByteBuffer blob;
	blob << E_GT_MAIL_AGREE_ENTER_INDEX;
	blob << pSGM->byPosion;
	blob << pCRole->GetstrUserName();
	blob << str;
	SGMtmp.blob.assign((const char *)blob.contents(), blob.size());

	if (!CDBCtrl::InsertGroupMail(SGMtmp))
	{
		IME_ERROR("insert data fail groupid %llu, %u", groupid, pCRole->GetdwAccountId());
		return -1;
	}
	return 0;
}

int CGroup::SendMailCancel(CRole *pCRole)
{
	uint64 groupid = MAKE_FIND_GROUPID(pCRole->GetdwAccountId(), GetdwId());
	uint32_t mailid = CDBCtrl::GetGroupMailId(groupid);
	CDBCtrl::SGroupMail SGMtmp;
	SGMtmp.lGroupId = groupid;
	SGMtmp.dwTime = time(NULL);
	SGMtmp.dwMailId = mailid;
	SGMtmp.content = "";
	SGMtmp.byType = E_GT_MAIL_SYSTEM;
	SGMtmp.byDoflag = 1;
	SGMtmp.byDetailType = E_GDT_MAIL_CANCEL;

	ByteBuffer blob;
	blob << E_GT_MAIL_CANCEL_INDEX;
	blob << pCRole->GetstrUserName();
	SGMtmp.blob.assign((const char *)blob.contents(), blob.size());

	if (!CDBCtrl::InsertGroupMail(SGMtmp))
	{
		IME_ERROR("insert data fail groupid %llu, %u", groupid, pCRole->GetdwAccountId());
		return -1;
	}
	return 0;
}

int CGroup::SendMailDelete(CRole *pCRole, std::string PeerUserName)
{
	uint64 groupid = MAKE_FIND_GROUPID(pCRole->GetdwAccountId(), GetdwId());
	uint32_t mailid = CDBCtrl::GetGroupMailId(groupid);
	CDBCtrl::SGroupMail SGMtmp;
	SGMtmp.lGroupId = groupid;
	SGMtmp.dwTime = time(NULL);
	SGMtmp.dwMailId = mailid;
	SGMtmp.content = "";
	SGMtmp.byType = E_GT_MAIL_SYSTEM;
	SGMtmp.byDoflag = 1;
	SGMtmp.byDetailType = E_GDT_MAIL_DELETE;

	SGroupMember *pSGM = GetGroupMember(pCRole->GetdwAccountId());
	if (pSGM == NULL)
		return -1;
	ByteBuffer blob;
	blob << E_GT_MAIL_DELETE_INDEX;
	blob << pSGM->byPosion;
	blob << pCRole->GetstrUserName();
	blob << PeerUserName; 
	SGMtmp.blob.assign((const char *)blob.contents(), blob.size());

	if (!CDBCtrl::InsertGroupMail(SGMtmp))
	{
		IME_ERROR("insert data fail groupid %llu, %u", groupid, pCRole->GetdwAccountId());
		return -1;
	}
	return 0;
}

void CGroup::HandlerExit(CRole *pCRole, WorldPacket &pkg)
{
	if (pCRole->GetdwAccountId() == GetdwCreateId())
	{
		IME_ERROR("create can not cancel group userid %u", GetdwCreateId());
		pCRole->SendErrorMsg(ERRNO_MSG_GROUP_CREATE_NO_CANCEL, CMD_SC_GROUP_EXIT_INFO_RESULT);
		return;
	}

	SGroupMember *p = GetGroupMember(pCRole->GetdwAccountId());
	uint8_t position = p->byPosion;
	uint32_t userid = pCRole->GetdwAccountId();

	std::vector<AnyType>::iterator it;
	switch (position)
	{
		case E_GA_VICE_PRESIDENT:
			it = std::find(m_vecPosion1Persion.begin(), m_vecPosion1Persion.end(), AnyType(userid));		
			if (it == m_vecPosion1Persion.end())
			{
				pCRole->SendErrorMsg(ERRNO_MSG_GROUP_NOT_IN_POSITION, CMD_SC_GROUP_EXIT_INFO_RESULT);
				break;
			}
			m_vecPosion1Persion.erase(it);
			SetvecPosion1Persion(m_vecPosion1Persion);
			break;
		case E_GA_INNER_MINISTER:
			it = std::find(m_vecPosion2Persion.begin(), m_vecPosion2Persion.end(), AnyType(userid));		
			if (it == m_vecPosion2Persion.end())
			{
				pCRole->SendErrorMsg(ERRNO_MSG_GROUP_NOT_IN_POSITION, CMD_SC_GROUP_EXIT_INFO_RESULT);
				break;
			}
			m_vecPosion2Persion.erase(it);
			SetvecPosion2Persion(m_vecPosion2Persion);
			break;
		case E_GA_OUT_MINISTER:
			it = std::find(m_vecPosion3Persion.begin(), m_vecPosion3Persion.end(), AnyType(userid));		
			if (it == m_vecPosion3Persion.end())
			{
				pCRole->SendErrorMsg(ERRNO_MSG_GROUP_NOT_IN_POSITION, CMD_SC_GROUP_EXIT_INFO_RESULT);
				break;
			}
			m_vecPosion3Persion.erase(it);
			SetvecPosion3Persion(m_vecPosion3Persion);
			break;
		case E_GA_ELITE:
			it = std::find(m_vecPosion4Persion.begin(), m_vecPosion4Persion.end(), AnyType(userid));		
			if (it == m_vecPosion4Persion.end())
			{
				pCRole->SendErrorMsg(ERRNO_MSG_GROUP_NOT_IN_POSITION, CMD_SC_GROUP_EXIT_INFO_RESULT);
				break;
			}
			m_vecPosion4Persion.erase(it);
			SetvecPosion4Persion(m_vecPosion4Persion);
			break;
		default:
			it = std::find(m_vecPosion5Persion.begin(), m_vecPosion5Persion.end(), AnyType(userid));		
			if (it == m_vecPosion5Persion.end())
			{
				pCRole->SendErrorMsg(ERRNO_MSG_GROUP_NOT_IN_POSITION, CMD_SC_GROUP_EXIT_INFO_RESULT);
				break;
			}
			m_vecPosion5Persion.erase(it);
			SetvecPosion5Persion(m_vecPosion4Persion);
			return;
	}
	
	m_mapUserGroup.erase(userid);
	SendMailCancel(pCRole);

	pCRole->SetbyFlag(CLOSE_GROUP(pCRole->GetbyFlag()));
	pCRole->SetlFindGroupId(0);
	pCRole->SendProUpdate();

	WorldPacket info;
	PackProUpdate(info);
	SendDataToAllMember(&info);

	info.clear();
	info.SetOpcode(CMD_SC_GROUP_EXIT_INFO_RESULT);
	info << (uint16_t) 0;
	pCRole->SendPacket(&info);
}

void CGroup::HandlerDeleteMember(CRole *pCRole, WorldPacket &pkg)
{
	uint32_t userid;
	pkg >> userid;
	
	if (userid == GetdwCreateId())
	{
		IME_ERROR("create can not cancel group userid %u", GetdwCreateId());
		pCRole->SendErrorMsg(ERRNO_MSG_GROUP_CREATE_NO_CANCEL, CMD_SC_GROUP_DELETE_MEMBER_RESULT);
		return;
	}

	SGroupMember *pSGM = GetGroupMember(pCRole->GetdwAccountId());
	uint8_t position = pSGM->byPosion;
	if (!CConfGroupAdmin::GetVal(position, E_GA_MEMBER_IO))
	{
		pCRole->SendErrorMsg(ERRNO_MSG_GROUP_NOT_AUTH, CMD_SC_GROUP_DELETE_MEMBER_RESULT);
		return;
	}

	SGroupMember *pPeerSGM = GetGroupMember(userid);
	if (pPeerSGM == NULL)
	{
		IME_ERROR("this user %u is not in group", userid);
		pCRole->SendErrorMsg(ERRNO_MSG_ALREADY_NOT_IN_GROUP, CMD_SC_GROUP_DELETE_MEMBER_RESULT);
		return;
	}
	position = pPeerSGM->byPosion;

	uint8_t IsOnline = true;	
	CRole *pRole = sWorld->FindUserRole(userid);
	if (pRole == NULL)
	{
		pRole = new(std::nothrow) CRole(NULL);
		assert(pRole != NULL);
		if (!pRole->LoadDataForDuration(userid))
		{
			IME_ERROR("CRole init fail %u", userid);
			pCRole->SendErrorMsg(ERRNO_MSG_DB_NO_DATA_OR_ERROR, CMD_SC_GROUP_DELETE_MEMBER_RESULT);
			delete pRole;
			return;
		}
		IsOnline = false;
	}

	std::vector<AnyType>::iterator it;
	switch (position)
	{
		case E_GA_VICE_PRESIDENT:
			it = std::find(m_vecPosion1Persion.begin(), m_vecPosion1Persion.end(), AnyType(userid));		
			if (it == m_vecPosion1Persion.end())
			{
				pCRole->SendErrorMsg(ERRNO_MSG_GROUP_NOT_IN_POSITION, CMD_SC_GROUP_DELETE_MEMBER_RESULT);
				break;
			}
			m_vecPosion1Persion.erase(it);
			SetvecPosion1Persion(m_vecPosion1Persion);
			break;
		case E_GA_INNER_MINISTER:
			it = std::find(m_vecPosion2Persion.begin(), m_vecPosion2Persion.end(), AnyType(userid));		
			if (it == m_vecPosion2Persion.end())
			{
				pCRole->SendErrorMsg(ERRNO_MSG_GROUP_NOT_IN_POSITION, CMD_SC_GROUP_DELETE_MEMBER_RESULT);
				break;
			}
			m_vecPosion2Persion.erase(it);
			SetvecPosion2Persion(m_vecPosion2Persion);
			break;
		case E_GA_OUT_MINISTER:
			it = std::find(m_vecPosion3Persion.begin(), m_vecPosion3Persion.end(), AnyType(userid));		
			if (it == m_vecPosion3Persion.end())
			{
				pCRole->SendErrorMsg(ERRNO_MSG_GROUP_NOT_IN_POSITION, CMD_SC_GROUP_DELETE_MEMBER_RESULT);
				break;
			}
			m_vecPosion3Persion.erase(it);
			SetvecPosion3Persion(m_vecPosion3Persion);
			break;
		case E_GA_ELITE:
			it = std::find(m_vecPosion4Persion.begin(), m_vecPosion4Persion.end(), AnyType(userid));		
			if (it == m_vecPosion4Persion.end())
			{
				pCRole->SendErrorMsg(ERRNO_MSG_GROUP_NOT_IN_POSITION, CMD_SC_GROUP_DELETE_MEMBER_RESULT);
				break;
			}
			m_vecPosion4Persion.erase(it);
			SetvecPosion4Persion(m_vecPosion4Persion);
			break;
		default:
			it = std::find(m_vecPosion5Persion.begin(), m_vecPosion5Persion.end(), AnyType(userid));		
			if (it == m_vecPosion5Persion.end())
			{
				pCRole->SendErrorMsg(ERRNO_MSG_GROUP_NOT_IN_POSITION, CMD_SC_GROUP_DELETE_MEMBER_RESULT);
				break;
			}
			m_vecPosion5Persion.erase(it);
			SetvecPosion5Persion(m_vecPosion4Persion);
			break;
	}
	
	m_mapUserGroup.erase(userid);


	SendMailDelete(pCRole, pRole->GetstrUserName());

	pRole->SetbyFlag(CLOSE_GROUP(pCRole->GetbyFlag()));
	pRole->SetlFindGroupId(0);
	if(!IsOnline)
	{
		pRole->SaveDataForDuration();
		delete pRole;
	}
	else
	{
		pCRole->SendProUpdate();
	}

	WorldPacket info;
	PackProUpdate(info);
	SendDataToAllMember(&info);

	info.clear();
	info.SetOpcode(CMD_SC_GROUP_DELETE_MEMBER_RESULT);
	info << (uint16_t) 0;
	info << userid;
	pCRole->SendPacket(&info);
}

void CGroup::HandlerUnAppointMemberInfo(CRole *pCRole, WorldPacket &pkg)
{
	std::map<uint32_t, SGroupMember>::iterator it;
	std::vector<SMember> vec;
	SMember temp;

	for(it = m_mapUserGroup.begin(); it != m_mapUserGroup.end(); it++)
	{
		uint8_t IsOnline = true;	
		CRole *pRole = sWorld->FindUserRole(it->first);
		if (pRole == NULL)
		{
			pRole = new(std::nothrow) CRole(NULL);
			assert(pRole != NULL);
			if (!pRole->LoadDataForDuration(it->first))
			{
				IME_ERROR("CRole init fail %u", it->first);
				pCRole->SendErrorMsg(ERRNO_MSG_DB_NO_DATA_OR_ERROR, CMD_SC_GROUP_UNAPPOINT_MEMBER_INFO_RESULT);
				delete pRole;
				return;
			}
			IsOnline = false;
		}

		temp.userid = pRole->GetdwAccountId();
		temp.name = pRole->GetstrUserName();
		temp.level = pRole->GetwLevel();
		temp.credit = (it->second).dwCredit;
		vec.push_back(temp);

		if(!IsOnline)
		{
			delete pRole;
		}
	}

	WorldPacket info(CMD_SC_GROUP_UNAPPOINT_MEMBER_INFO_RESULT);
	info << (uint16_t) 0;
	info << (uint32_t) vec.size();
   	for(int i = 0; i < vec.size(); i++)
	{
		info << vec[i].userid;
		info << vec[i].name;
	}	
	pCRole->SendPacket(&info);
}

int CGroup::GroupCancel(CRole *pCRole, WorldPacket &pkg)
{
	if (pCRole->GetdwAccountId() != GetdwCreateId())
	{
		pCRole->SendErrorMsg(ERRNO_MSG_GROUP_NOT_AUTH, CMD_SC_GROUP_CANCEL_RESULT);
		return -1;
	}

	std::map<uint32_t, SGroupMember>::iterator it;

	for(it = m_mapUserGroup.begin(); it != m_mapUserGroup.end(); it++)
	{
		uint8_t IsOnline = true;	
		CRole *pRole = sWorld->FindUserRole(it->first);
		if (pRole == NULL)
		{
			pRole = new(std::nothrow) CRole(NULL);
			assert(pRole != NULL);
			if (!pRole->LoadDataForDuration(it->first))
			{
				IME_ERROR("CRole init fail %u", it->first);
				pCRole->SendErrorMsg(ERRNO_MSG_DB_NO_DATA_OR_ERROR, CMD_SC_GROUP_CANCEL_RESULT);
				delete pRole;
				return -1;
			}
			IsOnline = false;
		}

		pRole->SetbyFlag(CLOSE_GROUP(pCRole->GetbyFlag()));
		pRole->SetlFindGroupId(0);

		if(!IsOnline)
		{
			pRole->SaveDataForDuration();
			delete pRole;
		}
		else
		{
			pRole->SendProUpdate();
		}
	}
	
	SGroupMember *pSGM = GetGroupMember(pCRole->GetdwAccountId());
	SGroupMember tmp = *pSGM;
	m_mapUserGroup.clear();
	m_mapUserGroup[pCRole->GetdwAccountId()] = tmp;	
	SetvecPosion1Persion(std::vector<AnyType>());
	SetvecPosion1PersionStr(std::vector<AnyType>());
	SetvecPosion2Persion(std::vector<AnyType>());
	SetvecPosion3Persion(std::vector<AnyType>());
	SetvecPosion4Persion(std::vector<AnyType>());
	SetvecPosion5Persion(std::vector<AnyType>());
	SetwPersionNumber(1);

	m_byFlag = 1;
	
	WorldPacket info(CMD_SC_GROUP_CANCEL_RESULT);
	info << (uint16_t) 0;
	pCRole->SendPacket(&info);
	return 0;
}
