/*
 * =====================================================================================
 *
 *       Filename:  Friend.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2012年11月09日 03时34分50秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  imane (), imanecr@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include <map>
#include <stdint.h> 
#include "Role.h"
#include "Friend.h"
#include "ErrorMsg.h"
#include "CmdDefine.h"
#include "DBCtrl.h"
#include "WorldPacket.h"
#include "CardPacket.h"
#include "Mail.h"
#include "World.h"
#include "Mission.h"
#include "ConfigData.h"

void
CFriend::HandlerAutoPlayerInFriend (CRole *pCRole, WorldPacket &pkg)
{
	uint8_t type = 0;	
	uint16_t off = 0;
	pkg >> type;
	pkg >> off;
	
	IME_CHAR_LOG("action [userid %u get friend list off %u]",pCRole->GetdwAccountId(), off);
	std::vector <CDBCtrl::stuPlayerPlus> vecPlayer;
	CDBCtrl::GetUserBaseDataAuto10InFriend(pCRole->GetdwAccountId(), off, vecPlayer);

	WorldPacket info(CMD_SC_FRIEND_AUTO_PLAYER_IN_FRIEND_RESULT);
	info << (uint16_t)0;
	info << (uint8_t) vecPlayer.size();
	for (int i = 0; i < vecPlayer.size(); i++)
	{
		info << vecPlayer[i].dwUserid;
		info << vecPlayer[i].wLevel;
		info << vecPlayer[i].strName;
		uint32_t userid = vecPlayer[i].dwUserid;
//		CRole *pCRole = sWorld->FindUserRole(userid);
//		CRole temp(NULL);
//		if (pCRole == NULL)
//		{
//			pCRole = &temp;
//			assert(pCRole != NULL);
//			if (!pCRole->LoadDataForDuration(userid))
//			{
//				IME_ERROR("load peer role from DB fail userid %u", userid);
//				pCRole->SendErrorMsg(ERRNO_MSG_DB_NO_DATA_OR_ERROR, CMD_SC_FRIEND_AUTO_PLAYER_IN_FRIEND_RESULT);
//				return;
//			}
//		}
//		if (pCRole->GetclsCardPacket()->PreDataTeamCardDataForFriend(info))
//		{
//			pCRole->SendErrorMsg(ERRNO_MSG_DB_NO_DATA_OR_ERROR, CMD_SC_FRIEND_AUTO_PLAYER_IN_FRIEND_RESULT);
//			return;
//		}
		CBaseRoleData *p = NULL;
		CBaseRoleData temp;
		if (p == NULL)
		{
			p = &temp;
			if (!p->Init(userid))
			{
				IME_ERROR("load peer role from DB fail userid %u", userid);
				pCRole->SendErrorMsg(ERRNO_MSG_DB_NO_DATA_OR_ERROR, CMD_SC_FRIEND_AUTO_PLAYER_IN_FRIEND_RESULT);
				return;
			}
		}
		p->PreDataTeamCardDataForFriend(info);
		IME_DEBUG("userid %u, wLevel %u", vecPlayer[i].dwUserid, vecPlayer[i].wLevel);
	}
	pCRole->SendPacket(&info);
}		/* -----  end of method CFriend::HandlerAutoPlayerFriend  ----- */

void
CFriend::HandlerFriendTeamRequest (CRole *pCRole, WorldPacket &pkg)
{
//	uint32_t userid;
//	pkg >> userid;
//	if (pCRole->GetdwAccountId() == userid)
//	{
//		IME_ERROR("can not request friend team data with myself");
//		pCRole->SendErrorMsg(ERRNO_MSG_DATA_ILLEGAL, CMD_SC_FRIEND_PLAYER_TEAM_RESULT);
//		return;
//	}
//
//
//	bool IsOnline = true;
//	CRole *pPeerCRole = sWorld->FindUserRole(userid);
//	if (pPeerCRole == NULL)
//	{
//		pPeerCRole = new(std::nothrow) CRole(NULL);
//		assert(pPeerCRole != NULL);
//		if (!pPeerCRole->LoadDataForDuration(userid))
//		{
//			IME_ERROR("load peer role from DB fail userid %u", userid);
//			pCRole->SendErrorMsg(ERRNO_MSG_DB_NO_DATA_OR_ERROR , CMD_SC_FRIEND_PLAYER_TEAM_RESULT);
//			return;
//		}
//		IsOnline = false;
//	}
//	
//	WorldPacket info(CMD_SC_FRIEND_PLAYER_TEAM_RESULT);
//	info << (uint16_t) 0;
//	pPeerCRole->GetclsCardPacket()->PacketForFriendProtocol(info);
//	pCRole->SendPacket(&info);
//
//	if (IsOnline)
//	{
//	}
//	else
//	{
//		delete pPeerCRole;
//	}
	IME_ERROR("unvalid protocol");
	return ;
}		/* -----  end of method CFriend::HandlerFriendTeamRequest  ----- */


void
CFriend::HandlerAutoPlayerForFriend (CRole *pCRole, WorldPacket &pkg)
{
	uint8_t type = 0;	
	pkg >> type;
	
	IME_CHAR_LOG("action [userid %u find friends]", pCRole->GetdwAccountId());
	std::vector <CDBCtrl::stuPlayer> vecPlayer, vecDetail;
	CDBCtrl::GetUserBaseDataAuto10ForFriend(pCRole->GetdwAccountId(), pCRole->GetwLevel(), vecDetail);

	std::vector<int> vecVal;

	if (vecDetail.size() <= MAX_EVERY_FRIEND_NUMBER)
	{
		vecPlayer = vecDetail;
	}
	else
	{
		vecVal = CUtil::RandByRangeMulti(0, vecDetail.size(), MAX_EVERY_FRIEND_NUMBER);
		for (int i = 0; i < vecVal.size(); i++)
		{
			vecPlayer.push_back(vecDetail[vecVal[i]]);
		}
	}

	WorldPacket info(CMD_SC_FRIEND_AUTO_PLAYER_FOR_FRIEND_RESULT);
	info << (uint16_t) 0;
	info << (uint8_t) vecPlayer.size();
	for (int i = 0; i < vecPlayer.size(); i++)
	{
		info << vecPlayer[i].dwUserid;
		info << vecPlayer[i].wLevel;
		info << vecPlayer[i].strName;
		uint32_t userid = vecPlayer[i].dwUserid;
//		CRole *pPeerCRole = sWorld->FindUserRole(userid);
//		CRole temp(NULL);
//		if (pPeerCRole == NULL)
//		{
//			pPeerCRole = &temp;
//			assert(pPeerCRole != NULL);
//			if (!pPeerCRole->LoadDataForDuration(userid))
//			{
//				IME_ERROR("load peer role from DB fail userid %u", userid);
//				pCRole->SendErrorMsg(ERRNO_MSG_DB_NO_DATA_OR_ERROR, CMD_SC_FRIEND_AUTO_PLAYER_FOR_FRIEND_RESULT);
//				return;
//			}
//		}
//		if (pPeerCRole->GetclsCardPacket()->PreDataTeamCardDataForFriend(info))
//		{
//			pCRole->SendErrorMsg(ERRNO_MSG_DB_NO_DATA_OR_ERROR, CMD_SC_FRIEND_AUTO_PLAYER_FOR_FRIEND_RESULT);
//			return;
//		}
		CBaseRoleData *p = NULL;
		CBaseRoleData temp;
		if (p == NULL)
		{
			p = &temp;
			if (!p->Init(userid))
			{
				IME_ERROR("load peer role from DB fail userid %u", userid);
				pCRole->SendErrorMsg(ERRNO_MSG_DB_NO_DATA_OR_ERROR, CMD_SC_FRIEND_AUTO_PLAYER_FOR_FRIEND_RESULT);
				return;
			}
			p->PreDataTeamCardDataForFriend(info);
		}
		IME_DEBUG("userid %u, wLevel %u", vecPlayer[i].dwUserid, vecPlayer[i].wLevel);
	}
	pCRole->SendPacket(&info);
	return ;
}		/* -----  end of method CFriend::HandlerAutoPlayerForFriend  ----- */


void
CFriend::HandlerAddFriend (CRole *pCRole, WorldPacket &pkg, uint8_t IsInner)
{
	uint32_t userid;
	pkg >> userid;

	IME_CHAR_LOG("action [userid %u add friend %u]", pCRole->GetdwAccountId(), userid);
	if (pCRole->GetdwAccountId() == userid)
	{
		IME_ERROR("can not add friend with myself");
		if (!IsInner)
		{
			pCRole->SendErrorMsg(ERRNO_MSG_DATA_ILLEGAL, CMD_SC_FRIEND_ADD_RESULT);
		}
		return;
	}
	if (IS_ROBORT(userid))
	{
		if (!IsInner)
		{
			WorldPacket info(CMD_SC_FRIEND_ADD_RESULT);
			info << (uint16_t)0;
			info << (uint32_t)userid;
			pCRole->SendPacket(&info);
		}
		return;
	}

	if (CDBCtrl::IsFriend(pCRole->GetdwAccountId(), userid))
	{
		IME_ERROR("you have been his friend");
		if (!IsInner)
		{
			pCRole->SendErrorMsg(ERRNO_MSG_ALREADY_FRIEND, CMD_SC_FRIEND_ADD_RESULT);
		}
		return;
	}
	if (CDBCtrl::GetFriendRequest(userid, pCRole->GetdwAccountId()))
	{
		IME_ERROR("user have sended friend request requestid %u userid %u", pCRole->GetdwAccountId(), userid);
		if (!IsInner)
		{
			pCRole->SendErrorMsg(ERRNO_MSG_ALREADY_REQUEST_FRIEND, CMD_SC_FRIEND_ADD_RESULT);
		}
		return;
	}

	CRole *pPeerCRole = sWorld->FindUserRole(userid);
	CRole tmp(NULL);
	if (pPeerCRole == NULL)
	{
		pPeerCRole = &tmp;
		if (!pPeerCRole->LoadDataForDuration(userid))
		{
			IME_ERROR("load peer role from DB fail userid %u", userid);
			if (!IsInner)
			{
				pCRole->SendErrorMsg(ERRNO_MSG_DB_NO_DATA_OR_ERROR, CMD_SC_AGREE_FRIEND_REQUEST_RESULT);
			}
			return;
		}
	}
	uint32_t friend_number;	
	friend_number = CDBCtrl::GetUserFriendNumber(userid);

	uint32_t vipextrafriendlimit = 0;
	STC_CONF_VIP *pConfVip = NULL;
	pConfVip = CConfVip::Find(pPeerCRole->GetwVipLv());
	if (pConfVip == NULL)
	{
		IME_ERROR("vip lv %u can not find in vip csv", pPeerCRole->GetwVipLv());
	}
	else
	{
		vipextrafriendlimit = pConfVip->dwVipFriendLimit;
	}

	if (friend_number >= pPeerCRole->GetdwFriendLimit() + vipextrafriendlimit)
	{
		if (!IsInner)
		{
			pCRole->SendErrorMsg(ERRNO_MSG_FRIEND_LIMIT_PEER, CMD_SC_FRIEND_ADD_RESULT);
		}
		return;
	}

	if (!CDBCtrl::InsertFriendRequest(userid, pCRole->GetdwAccountId()))
	{
		IME_ERROR("send friend request fail requestid %u userid %u", pCRole->GetdwAccountId(), userid);
		if (!IsInner)
		{
			pCRole->SendErrorMsg(ERRNO_MSG_DB, CMD_SC_FRIEND_ADD_RESULT);
		}
		return;
	}

//	uint32_t newmailid;
//	CDBCtrl::stuMailDetail mail;
//	if (CMail::AddEmailFriendRequest(pCRole, userid, newmailid, mail))
//	{
//		IME_ERROR("send email fail friendid %u, userid %u", userid, pCRole->GetdwAccountId());
//		pCRole->SendErrorMsg(ERRNO_MSG_DB , CMD_SC_FRIEND_ADD_RESULT);
//		return;
//
//	}
//	CRole *pPeerCRole = sWorld->FindUserRole(userid);
//	if (pPeerCRole != NULL)
//	{
//		CMail::EmailUpdate(pPeerCRole, CMail::ADD, &mail, 0); 
//	}

	if (!IsInner)
	{
		WorldPacket info(CMD_SC_FRIEND_ADD_RESULT);
		info << (uint16_t)0;
		info << (uint32_t)userid;
		pCRole->SendPacket(&info);
	}
	return ;
}		/* -----  end of method CFriend::HandlerAddFriend  ----- */

void
CFriend::HandlerDelFriend (CRole *pCRole, WorldPacket &pkg)
{
	uint32_t userid;
	pkg >> userid;
	IME_CHAR_LOG("action [userid %u del friend %u]", pCRole->GetdwAccountId(), userid);

	CRole *pPeerCRole = sWorld->FindUserRole(userid);
	CRole tmp(NULL);
	if (pPeerCRole == NULL)
	{
		pPeerCRole = &tmp;
		if (!pPeerCRole->LoadDataForDuration(userid))
		{
			IME_ERROR("load peer role from DB fail userid %u", userid);
			pCRole->SendErrorMsg(ERRNO_MSG_DB_NO_DATA_OR_ERROR, CMD_SC_FRIEND_DEL_RESULT);
			return;
		}
	}

//	if (pCRole->GetdwDelFriendTime() >= GlobalConfig::DelFriendLimit)
//	{
//		pCRole->SendErrorMsg(ERRNO_MSG_MYSELF_DEL_FRIEND_LIMIT, CMD_SC_FRIEND_DEL_RESULT);
//		return;
//	}
//	if (pPeerCRole->GetdwDelFriendTime() >= GlobalConfig::DelFriendLimit)
//	{
//		pCRole->SendErrorMsg(ERRNO_MSG_PEER_DEL_FRIEND_LIMIT, CMD_SC_FRIEND_DEL_RESULT);
//		return;
//	}

	if (!CDBCtrl::DelFriend(pCRole->GetdwAccountId(), userid))
	{
		IME_ERROR("delete friend fail userid %u, friendid %u", pCRole->GetdwAccountId(), userid);
		pCRole->SendErrorMsg(ERRNO_MSG_DB , CMD_SC_FRIEND_DEL_RESULT);
		return;
	}
	if (!CDBCtrl::DelFriend(userid, pCRole->GetdwAccountId()))
	{
		IME_ERROR("delete friend  fail friendid %u, userid %u", userid, pCRole->GetdwAccountId());
		pCRole->SendErrorMsg(ERRNO_MSG_DB , CMD_SC_FRIEND_DEL_RESULT);
		return;
	}

//	uint32_t newmailid;
//	CDBCtrl::stuMailDetail mail;
//	if (CMail::AddEmailFriendDel(pCRole, userid, newmailid, mail))
//	{
//		IME_ERROR("send email fail friendid %u, userid %u", userid, pCRole->GetdwAccountId());
//		pCRole->SendErrorMsg(ERRNO_MSG_DB , CMD_SC_FRIEND_DEL_RESULT);
//		return;
//
//	}
//	CRole *pPeerCRole = sWorld->FindUserRole(userid);
//	if (pPeerCRole != NULL)
//	{
//		CMail::EmailUpdate(pPeerCRole, CMail::ADD, &mail, 0); 
//	}

	WorldPacket info(CMD_SC_FRIEND_DEL_RESULT);
	info << (uint16_t)0;
	info << (uint32_t)userid;
	pCRole->SendPacket(&info);
	return ;
}		/* -----  end of method CFriend::HandlerAddFriend  ----- */


void
CFriend::HandlerSearchPlayer (CRole *pCRole, WorldPacket &pkg)
{
	std::string user;
	uint32_t dwPlayer;
	pkg >> user;
	IME_CHAR_LOG("action [userid %u search player %s]", pCRole->GetdwAccountId(), user.c_str());
	dwPlayer = atol(user.c_str());
//	if (pCRole->GetdwAccountId() == dwPlayer)
//	{
//		pCRole->SendErrorMsg(ERRNO_MSG_ADD_FRIEND_MYSELF, CMD_SC_FRIEND_SEARCH_PLAYER_RESULT);
//		return;
//	}

	std::vector<CDBCtrl::stuPlayer> vecPlayer;
	CDBCtrl::GetUserBaseDataByNameId(dwPlayer, user, vecPlayer, pCRole->GetdwAccountId());

	WorldPacket info(CMD_SC_FRIEND_SEARCH_PLAYER_RESULT);
	info << (uint16_t)0;
	info << (uint8_t) vecPlayer.size();
	for (int i = 0; i < vecPlayer.size(); i++)
	{
		info << vecPlayer[i].dwUserid;
		info << vecPlayer[i].wLevel;
		info << vecPlayer[i].strName;
		uint32_t userid = vecPlayer[i].dwUserid;
		CRole *pCRole = sWorld->FindUserRole(userid);
		CRole temp(NULL);
		if (pCRole == NULL)
		{
			pCRole = &temp;
			assert(pCRole != NULL);
			if (!pCRole->LoadDataForDuration(userid))
			{
				IME_ERROR("load peer role from DB fail userid %u", userid);
				pCRole->SendErrorMsg(ERRNO_MSG_DB_NO_DATA_OR_ERROR, CMD_SC_FRIEND_SEARCH_PLAYER_RESULT);
				return;
			}
		}
		if (pCRole->GetclsCardPacket()->PreDataTeamCardDataForFriend(info))
		{
			pCRole->SendErrorMsg(ERRNO_MSG_DB_NO_DATA_OR_ERROR, CMD_SC_FRIEND_SEARCH_PLAYER_RESULT);
			return;
		}
		IME_DEBUG("userid %u, wLevel %u", vecPlayer[i].dwUserid, vecPlayer[i].wLevel);
	}
	pCRole->SendPacket(&info);
}		/* -----  end of method CFriend::HandlerSearchPlayer  ----- */


void
CFriend::HandlerGetFriendNumber (CRole *pCRole, WorldPacket &pkg)
{
	IME_CHAR_LOG("action [userid %u get friend number]", pCRole->GetdwAccountId());
	uint32_t number;
	uint32_t request_number;
	number = CDBCtrl::GetUserFriendNumber(pCRole->GetdwAccountId());
	request_number = CDBCtrl::GetUserRequestFriendNumber(pCRole->GetdwAccountId());
	WorldPacket info(CMD_SC_FRIEND_NUMBER_RESULT);
	info << (uint16_t) 0;
	info << (uint16_t) number;
	uint16_t friendLimit = 0;
	STC_CONF_VIP *pVipConf = CConfVip::Find(pCRole->GetwVipLv());
	if (pVipConf == NULL)
	{
		IME_ERROR("can not find vip lv %u in vip csv", pCRole->GetwVipLv());
	}
	else
	{
		friendLimit = pVipConf->dwVipFriendLimit;
	}
	info << (uint16_t) pCRole->GetdwFriendLimit() + friendLimit;
	info << (uint16_t) request_number;
	IME_DEBUG("total %u friend %u", number, pCRole->GetdwFriendLimit());
	pCRole->SendPacket(&info);
	pCRole->GetCMission()->FriendNumber(0, number);
	return ;
}		/* -----  end of method CFriend::HandlerGetFriendNumber  ----- */

void
CFriend::HandlerSendFriendEmail (CRole *pCRole, WorldPacket &pkg)
{
	uint32_t userid;
	std::string content;
	pkg >> userid;
	pkg >> content;
	IME_CHAR_LOG("action [userid %u send mail to friend userid %u content %s]", pCRole->GetdwAccountId(), userid, content.c_str());
	if (pCRole->GetdwAccountId() == userid)
	{
		IME_ERROR("can not add friend with myself");
		pCRole->SendErrorMsg(ERRNO_MSG_DATA_ILLEGAL, CMD_SC_FRIEND_MAIL_RESULT);
		return;
	}
	if (!CDBCtrl::IsFriend(pCRole->GetdwAccountId(), userid))
	{
		IME_ERROR("you have been not his friend");
		pCRole->SendErrorMsg(ERRNO_MSG_ALREADY_NOT_FRIEND, CMD_SC_FRIEND_MAIL_RESULT);
		return;
	}

	uint32_t newmailid;
	CDBCtrl::stuMailDetail mail;
	if (CMail::AddEmailFriendText(pCRole, userid, content, newmailid, mail))
	{
		IME_ERROR("send email fail friendid %u, userid %u", userid, pCRole->GetdwAccountId());
		pCRole->SendErrorMsg(ERRNO_MSG_DB , CMD_SC_FRIEND_MAIL_RESULT);
		return;

	}
	CRole *pPeerCRole = sWorld->FindUserRole(userid);
	if (pPeerCRole != NULL)
	{
		CMail::EmailUpdate(pPeerCRole, CMail::ADD, &mail, 0); 
	}


	WorldPacket info(CMD_SC_FRIEND_MAIL_RESULT);
	info << (uint16_t) 0;
	pCRole->SendPacket(&info);
	return ;
}		/* -----  end of method CFriend::HandlerSendFriendEmail  ----- */

void CFriend::HandlerAgreeFriend(CRole *pCRole, WorldPacket &pkg)
{
	uint32_t userid;
	uint8_t flag;
	pkg >> userid;
	pkg >> flag;
	IME_CHAR_LOG("action [userid %u agree friend %u]", userid, flag);

	if (pCRole->GetdwAccountId() == userid)
	{
		IME_ERROR("cannot add friend to myself userid %u,request %u", pCRole->GetdwAccountId(), userid);
		pCRole->SendErrorMsg(ERRNO_MSG_ADD_FRIEND_MYSELF, CMD_SC_AGREE_FRIEND_REQUEST_RESULT);
		return;
	}
	if (!CDBCtrl::GetFriendRequest(pCRole->GetdwAccountId(), userid))
	{
		IME_ERROR("get friend request data fail userid %u, request %u", pCRole->GetdwAccountId(), userid);
		pCRole->SendErrorMsg(ERRNO_MSG_NO_REQUEST_FRIEND, CMD_SC_AGREE_FRIEND_REQUEST_RESULT);
		return;
	}
	if (CDBCtrl::IsFriend(pCRole->GetdwAccountId(), userid))
	{
		IME_ERROR("you have been his friend");
		CDBCtrl::DelFriendRequest(pCRole->GetdwAccountId(), userid);
		pCRole->SendErrorMsg(ERRNO_MSG_ALREADY_FRIEND, CMD_SC_AGREE_FRIEND_REQUEST_RESULT);
		return;
	}
	
	if (flag == 0)
	{
		bool IsOnline = true;
		CRole *pPeerCRole = sWorld->FindUserRole(userid);
		CRole tmp(NULL);
		if (pPeerCRole == NULL)
		{
			pPeerCRole = &tmp;
			if (!pPeerCRole->LoadDataForDuration(userid))
			{
				IME_ERROR("load peer role from DB fail userid %u", userid);
				pCRole->SendErrorMsg(ERRNO_MSG_DB_NO_DATA_OR_ERROR, CMD_SC_AGREE_FRIEND_REQUEST_RESULT);
				return;
			}
			IsOnline = false;
		}

//		IME_DEBUG("role %u, peer %u, FriendLimitLevel %u", pCRole->GetwLevel(), pPeerCRole->GetwLevel(), GlobalConfig::FriendLimitLevel);
		if (pCRole->GetwLevel() >= GlobalConfig::FriendLimitLevel)
		{
			if (pCRole->GetdwAddFriend() >= GlobalConfig::AddFriendLimit)
			{
				pCRole->SendErrorMsg(ERRNO_MSG_MYSELF_ADD_FRIEND_LIMIT, CMD_SC_AGREE_FRIEND_REQUEST_RESULT);
				return;
			}
		}

		if (pPeerCRole->GetwLevel() >= GlobalConfig::FriendLimitLevel)
		{
			if (pPeerCRole->GetdwAddFriend() >= GlobalConfig::AddFriendLimit)
			{
				pCRole->SendErrorMsg(ERRNO_MSG_PEER_ADD_FRIEND_LIMIT, CMD_SC_AGREE_FRIEND_REQUEST_RESULT);
				return;
			}
		}

		uint32_t friend_number, peerfriend_number;	
		friend_number = CDBCtrl::GetUserFriendNumber(pCRole->GetdwAccountId());
		peerfriend_number = CDBCtrl::GetUserFriendNumber(userid);

		uint32_t vipextrafriendlimit = 0;
		STC_CONF_VIP *pConfVip = NULL;
		pConfVip = CConfVip::Find(pCRole->GetwVipLv());
		if (pConfVip == NULL)
		{
			IME_ERROR("vip lv %u can not find in vip csv", pCRole->GetwVipLv());
		}
		else
		{
			vipextrafriendlimit = pConfVip->dwVipFriendLimit;
		}

		if (friend_number >= pCRole->GetdwFriendLimit() + vipextrafriendlimit)
		{
			pCRole->SendErrorMsg(ERRNO_MSG_FRIEND_LIMIT_MYSELF, CMD_SC_AGREE_FRIEND_REQUEST_RESULT);
			return;
		}

		vipextrafriendlimit = 0;
		pConfVip = CConfVip::Find(pPeerCRole->GetwVipLv());
		if (pConfVip == NULL)
		{
			IME_ERROR("vip lv %u can not find in vip csv", pPeerCRole->GetwVipLv());
		}
		else
		{
			vipextrafriendlimit = pConfVip->dwVipFriendLimit;
		}

		if (peerfriend_number >= pPeerCRole->GetdwFriendLimit() + vipextrafriendlimit)
		{
			pCRole->SendErrorMsg(ERRNO_MSG_FRIEND_LIMIT_PEER, CMD_SC_AGREE_FRIEND_REQUEST_RESULT);
			return;
		}

		if (!CDBCtrl::InsertFriend(pCRole->GetdwAccountId(), userid))
		{
			IME_ERROR("add friend fail userid %u, friendid %u", pCRole->GetdwAccountId(), userid);
			pCRole->SendErrorMsg(ERRNO_MSG_DB , CMD_SC_AGREE_FRIEND_REQUEST_RESULT);
			return;
		}
		if (!CDBCtrl::InsertFriend(userid, pCRole->GetdwAccountId()))
		{
			IME_ERROR("add friend  fail friendid %u, userid %u", userid, pCRole->GetdwAccountId());
			pCRole->SendErrorMsg(ERRNO_MSG_DB, CMD_SC_AGREE_FRIEND_REQUEST_RESULT);
			return;
		}

		pCRole->ChangedwAddFriend(1);	
		pPeerCRole->ChangedwAddFriend(1);	
		pPeerCRole->SaveDataForDuration();

		pCRole->GetCMission()->FriendNumber(0, friend_number);
	}
	else //refuse
	{

	}	

	CDBCtrl::DelFriendRequest(pCRole->GetdwAccountId(), userid);

	WorldPacket info(CMD_SC_AGREE_FRIEND_REQUEST_RESULT);
	info << (uint16_t) 0;
	info << flag;
	info << userid;
	pCRole->SendPacket(&info);
}

void CFriend::HandlerFriendApplyRequest(CRole *pCRole, WorldPacket &pkg)
{
	uint16_t off = 0;
	pkg >> off;
	IME_CHAR_LOG("action [userid %u get request friend list]", pCRole->GetdwAccountId());
	
	std::vector <CDBCtrl::stuPlayerPlus> vecPlayer;
	CDBCtrl::GetFriendRequestList(pCRole->GetdwAccountId(), off, vecPlayer);

	WorldPacket info(CMD_SC_FRIEND_APPLY_REQUEST_RESULT);
	info << (uint16_t)0;
	info << (uint8_t) vecPlayer.size();
	for (int i = 0; i < vecPlayer.size(); i++)
	{
		info << vecPlayer[i].dwUserid;
		info << vecPlayer[i].wLevel;
		info << vecPlayer[i].strName;
		uint32_t userid = vecPlayer[i].dwUserid;
//		CRole *pCRole = sWorld->FindUserRole(userid);
//		CRole temp(NULL);
//		if (pCRole == NULL)
//		{
//			pCRole = &temp;
//			assert(pCRole != NULL);
//			if (!pCRole->LoadDataForDuration(userid))
//			{
//				IME_ERROR("load peer role from DB fail userid %u", userid);
//				pCRole->SendErrorMsg(ERRNO_MSG_DB_NO_DATA_OR_ERROR, CMD_SC_FRIEND_APPLY_REQUEST_RESULT);
//				return;
//			}
//		}
//		if (pCRole->GetclsCardPacket()->PreDataTeamCardDataForFriend(info))
//		{
//			pCRole->SendErrorMsg(ERRNO_MSG_DB_NO_DATA_OR_ERROR, CMD_SC_FRIEND_APPLY_REQUEST_RESULT);
//			return;
//		}
		CBaseRoleData *p = NULL;
		CBaseRoleData temp;
		if (p == NULL)
		{
			p = &temp;
			if (!p->Init(userid))
			{
				IME_ERROR("load peer role from DB fail userid %u", userid);
				pCRole->SendErrorMsg(ERRNO_MSG_DB_NO_DATA_OR_ERROR, CMD_SC_FRIEND_APPLY_REQUEST_RESULT);
				return;
			}
		}
		p->PreDataTeamCardDataForFriend(info);
		IME_DEBUG("userid %u, wLevel %u", vecPlayer[i].dwUserid, vecPlayer[i].wLevel);
	}
	pCRole->SendPacket(&info);
}
