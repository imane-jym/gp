/*
 * =====================================================================================
 *
 *       Filename:  mail.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2012年11月11日 03时21分58秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  imane (), imanecr@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include "Mail.h"
#include "Role.h"
#include "DBCtrl.h"
#include "CmdDefine.h"
#include "EnumDefines.h"
#include "WorldPacket.h"
#include "ErrorMsg.h"
#include "World.h"
#include "NameChecker.h"
#include "CardPacket.h"

void
CMail::HandlerEmailInfo (CRole *pCRole)
{
	std::vector<CDBCtrl::stuMail> vecMail;
	if (!CDBCtrl::GetMailInfo(pCRole->GetdwAccountId(), vecMail))
	{
//		IME_ERROR("get mail data fail user_id %u", pCRole->GetdwAccountId());
		return;
	}	

	WorldPacket info(CMD_SC_MAIL_INFO);
	info << (uint32)vecMail.size();
	for( int i = 0; i < vecMail.size(); i++)
	{
		info << vecMail[i].dwMailId;
		info << vecMail[i].dwSendId;
		if (vecMail[i].byType == MAIL_SYSTEM_TYPE)
		{
			info << GlobalConfig::SystemMailSendName;
		}
		else
		{
			info << vecMail[i].strName;
		}

//		IME_ERROR("str username %s", vecMail[i].strName.c_str());
		info << vecMail[i].byDoflag;
		info << vecMail[i].byType;
		info << vecMail[i].dwTime;	
		info << vecMail[i].byDetailType;
		info << vecMail[i].strTitle;	
		IME_DEBUG("mail id %u, userid %u, type %u, byDetailType %u", vecMail[i].dwMailId, pCRole->GetdwAccountId(), vecMail[i].byType, vecMail[i].byDetailType);
	}
	pCRole->SendPacket(&info);
	return ;
}		/* -----  end of method CMail::HandlerEmailInfo  ----- */


void
CMail::HandlerGetEmailDetailInfo (CRole *pCRole, WorldPacket &pkg)
{
	IME_CHAR_LOG("action [userid %u get email detail]", pCRole->GetdwAccountId());
	uint32_t user_id;
	uint32_t mail_id;
	pkg >> user_id;
	pkg >> mail_id;
	CDBCtrl::stuMailDetail tempMailDetail;
	if (!CDBCtrl::GetMailDetailInfo(pCRole->GetdwAccountId(), mail_id, tempMailDetail))
	{
		IME_ERROR("get mail data fail useid %u, mail_id %u", pCRole->GetdwAccountId(), mail_id);
		pCRole->SendErrorMsg(ERRNO_MSG_DB_NO_DATA_OR_ERROR, CMD_SC_MAIL_DETAIL_INFO_RESULT);
		return;
	}
	ByteBuffer blob;
	uint8_t mailtype;
	uint32_t indexid;
	std::string name;
	uint32_t coin;
	uint32_t item_id;
	uint32_t type;
	uint32_t para1;
	uint32_t para2;
	std::string tip;
	uint32_t dollar;
	uint32_t energy;
	uint32_t strength;
	uint32_t worldhistory;

	blob.append(tempMailDetail.blob.data(), tempMailDetail.blob.size());
	WorldPacket info(CMD_SC_MAIL_DETAIL_INFO_RESULT);
	info << (uint16_t) 0;
	info << (uint32_t) tempMailDetail.dwMailId;
	mailtype = tempMailDetail.byDetailType;
	if (mailtype >= SYS_MAX)
	{
		IME_ERROR("can not verify this mail type");
		pCRole->SendErrorMsg(ERRNO_MSG_MAIL_NOT_VERIFY, CMD_SC_MAIL_DETAIL_INFO_RESULT);
		return;
	}
	info << mailtype;
	switch(mailtype)
	{
		case FRIEND_TEXT:
			info << tempMailDetail.content;
			break;
//		case FRIEND_REQUSET:
//			blob >> indexid;
//			blob >> name;
//			info << indexid;
//			info << name;
//			break;
//		case FRIEND_AGREE:
//			blob >> indexid;
//			blob >> name;
//			info << indexid;
//			info << name;
//			break;
//		case FRIEND_DEL:
//			blob >> indexid;
//			blob >> name;
//			info << indexid;
//			info << name;
//			break;
//		case DURATION_COIN:
//			blob >> indexid;
//			blob >> name;
//			blob >> coin;
//			info << indexid;
//			info << name;
//			info << coin;
//			break;
		case ROB_GET:
			blob >> indexid;
			blob >> name;
			blob >> item_id;
			info << indexid;
			info << name;
			info << item_id;
			break;
		case ROB_LOSE:
			blob >> indexid;
			blob >> name;
			blob >> item_id;
			info << indexid;
			info << name;
			info << item_id;
			break;
//		case DURATION_LOSE:
//			blob >> indexid;
//			blob >> name;
//			blob >> coin;
//			info << indexid;
//			info << name;
//			info << coin;
//			break;
		case SYS_PAY:
			blob >> indexid;
			blob >> type;
			blob >> para1;
			blob >> para2;
			blob >> coin;
			blob >> dollar;
			blob >> energy;
			blob >> strength;
			blob >> worldhistory;
			info << indexid;
			info << type;
			info << para1;
			info << para2;
			info << coin;
			info << dollar;
			info << energy;
			info << strength;
			info << worldhistory;
			break;
		case SYS_GIFT:
			blob >> indexid;
			blob >> type;
			blob >> para1;
			blob >> para2;
			blob >> tip;
			blob >> coin;
			blob >> dollar;
			blob >> energy;
			blob >> strength;
			blob >> worldhistory;
			info << indexid;
			info << type;
			info << para1;
			info << para2;
			info << tip;
			info << coin;
			info << dollar;
			info << energy;
			info << strength;
			info << worldhistory;
		case SYS_DIY:
			blob >> type;
			blob >> para1;
			blob >> para2;
//			blob >> tip;
			blob >> coin;
			blob >> dollar;
			blob >> energy;
			blob >> strength;
			blob >> worldhistory;
			info << type;
			info << para1;
			info << para2;
//			info << tip;
			info << coin;
			info << dollar;
			info << energy;
			info << strength;
			info << worldhistory;
			info << tempMailDetail.content;
//		case SYS_FRIEND_BOSS:
//			blob >> indexid;
//			blob >> para1;
//			blob >> para2;
//			blob >> tip;
//			info << indexid;
//			info << para1;
//			info << para2;
//			info << tip;
		defatult:
			break;
	}
	pCRole->SendPacket(&info);
	return ;
}		/* -----  end of method CMail::HandlerGetEmailDetailInfo  ----- */


void
CMail::HandlerEmailOpt (CRole *pCRole, WorldPacket &pkg)
{
	uint32_t user_id;
	uint32_t mail_id;
	uint8_t flag;
	pkg >> user_id;
	pkg >> mail_id;
	pkg >> flag;
	IME_DEBUG("user_id %u, mail_id %u", user_id, mail_id);
	IME_CHAR_LOG("action [userid %u operate mail %u send userid %u flag %u]", pCRole->GetdwAccountId(), mail_id, user_id, flag);
	
	CShopEffect::SGiveProduct tmp = {0};
	SMailObj ObjType = { 0 };	
	CDBCtrl::stuMailDetail tempMailDetail;
	if (!CDBCtrl::GetMailDetailInfo(pCRole->GetdwAccountId(), mail_id, tempMailDetail))
	{
		IME_ERROR("get mail data fail");
		pCRole->SendErrorMsg(ERRNO_MSG_DB_NO_DATA_OR_ERROR, CMD_SC_MAIL_HANDLE_RESULT);
		return;
	}
	if (tempMailDetail.byDoflag == 1)
	{
		IME_ERROR("the mail have been handled userid %u mailid %u", pCRole->GetdwAccountId(), mail_id);
		pCRole->SendErrorMsg(ERRNO_MSG_MAIL_ALREADY_HANDLE, CMD_SC_MAIL_HANDLE_RESULT);
		return;
	}

	if (pCRole->GetdwAccountId() == tempMailDetail.dwSendId)
	{
		IME_ERROR("cannot add friend to myself userid %u,fiendid %u", pCRole->GetdwAccountId(), tempMailDetail.dwSendId);
		pCRole->SendErrorMsg(ERRNO_MSG_DATA_ILLEGAL, CMD_SC_MAIL_HANDLE_RESULT);
		return;
	}
	uint16_t opt = MAIL_READ;
	
//	if (tempMailDetail.byDetailType == FRIEND_REQUSET)
//	{	
//		if (flag == 0)
//		{
//			uint32_t friend_number, peerfriend_number;	
//			friend_number = CDBCtrl::GetUserFriendNumber(pCRole->GetdwAccountId());
//			peerfriend_number = CDBCtrl::GetUserFriendNumber(tempMailDetail.dwSendId);
//			if (friend_number >= GlobalConfig::FriendLimit || peerfriend_number >= GlobalConfig::FriendLimit)
//			{
//				IME_ERROR("userid friend's number big than FriendLimit");
//				pCRole->SendErrorMsg(ERRNO_MSG_FRIEND_NUMBER_LIMIT, CMD_SC_MAIL_HANDLE_RESULT);
//				return;
//			}
//
//			if (!CDBCtrl::InsertFriend(pCRole->GetdwAccountId(), tempMailDetail.dwSendId))
//			{
//				IME_ERROR("add friend fail userid %u, friendid %u", pCRole->GetdwAccountId(), tempMailDetail.dwSendId);
//				pCRole->SendErrorMsg(ERRNO_MSG_DB , CMD_SC_MAIL_HANDLE_RESULT);
//				return;
//			}
//			if (!CDBCtrl::InsertFriend(tempMailDetail.dwSendId, pCRole->GetdwAccountId()))
//			{
//				IME_ERROR("add friend  fail friendid %u, userid %u", tempMailDetail.dwSendId, pCRole->GetdwAccountId());
//				pCRole->SendErrorMsg(ERRNO_MSG_DB , CMD_SC_MAIL_HANDLE_RESULT);
//				return;
//			}
//			uint32_t newmailid;
//			CDBCtrl::stuMailDetail mail;
//			if (AddEmailFriendAgree(pCRole, tempMailDetail.dwSendId, newmailid, mail))
//			{
//				IME_ERROR("send email fail friendid %u, userid %u", tempMailDetail.dwSendId, pCRole->GetdwAccountId());
//				pCRole->SendErrorMsg(ERRNO_MSG_DB , CMD_SC_MAIL_HANDLE_RESULT);
//				return;
//
//			}
//			CRole *pPeerCRole = sWorld->FindUserRole(tempMailDetail.dwSendId);
//			if (pPeerCRole != NULL)
//			{
//				EmailUpdate(pPeerCRole, ADD, &mail, 0); 
//			}
//			opt = MAIL_FRIEND_AGREE;
//		}	
//		else
//		{
//			opt = MAIL_FRIEND_REFUSE;
//		}
//	}
//	else if (tempMailDetail.byDetailType == SYS_PAY || tempMailDetail.byDetailType == SYS_GIFT) 
	
	if (tempMailDetail.byDetailType == SYS_PAY || tempMailDetail.byDetailType == SYS_GIFT || tempMailDetail.byDetailType == SYS_DIY) 
	{
		uint32_t order_id;
		if (!CDBCtrl::GetShopOrderId(pCRole->GetdwAccountId(), order_id))
		{
			IME_ERROR("user %u give dollar get order fail", pCRole->GetdwAccountId());
			pCRole->SendErrorMsg(ERRNO_MSG_DB, CMD_SC_MAIL_HANDLE_RESULT);
			return;
		}
		

		ByteBuffer by;
		by.append((const char *)tempMailDetail.blob.data(), tempMailDetail.blob.size());
		uint32_t indexid;
		uint32_t type;
		uint32_t para1;
		uint32_t para2;
		std::string tip;
		uint32_t coin;
		uint32_t dollar;
		uint32_t energy;
		uint32_t strength;
		uint32_t worldhistory;
		if (tempMailDetail.byDetailType != SYS_DIY)
		{
			by >> indexid;
		}
		by >> type;
		by >> para1;
		by >> para2;
		if (tempMailDetail.byDetailType == SYS_GIFT)
		{
			by >> tip;
		}
		by >> coin;
		by >> dollar;
		by >> energy;
		by >> strength;
		by >> worldhistory;
		
		int32_t getdollar = dollar;
		ObjType.coin = coin;
		ObjType.dollar = getdollar;
		ObjType.energy = energy;
		ObjType.strength = strength;
		ObjType.worldhistory = worldhistory;	
		ObjType.obj.type = type;
		ObjType.obj.para1 = para1;
		ObjType.obj.para2 = para2;

		uint32_t number = 1;
		if (type == E_OBJ_CARD_MORE)
		{
			number = para2;
		}

		if (pCRole->GetclsCardPacket()->IsFull(number, 0))
		{
			IME_ERROR("card packet is full userid %u", pCRole->GetdwAccountId());
			pCRole->SendErrorMsg(ERRNO_MSG_CARD_PACKET_NOT_ENOUGH, CMD_SC_MAIL_HANDLE_RESULT);
			return;
		}

		// to do data update to client about email
		if (!CDBCtrl::UpdateMail(pCRole->GetdwAccountId(), mail_id))
		{
			IME_ERROR("update mail fail from not do to do userid %u,mail %u", pCRole->GetdwAccountId(), mail_id);
			pCRole->SendErrorMsg(ERRNO_MSG_DB_NO_DATA_OR_ERROR, CMD_SC_MAIL_HANDLE_RESULT);
			return;
		}
		EmailUpdate(pCRole, UPDATE, NULL, mail_id);
		
		pCRole->ChangeCoin(coin, SOURCE_MAIL);
		pCRole->ChangeDollarWithExtra(getdollar, SOURCE_MAIL);
		pCRole->ChangeEnergyNoLimit(energy);
		pCRole->ChangeStrengthNoLimit(strength);
		pCRole->ChangeWorldHistory(worldhistory, SOURCE_MAIL);
		pCRole->SendProUpdate();
		pCRole->HandlerInfoOpt();

		if (dollar > 0)
		{
			CDBCtrl::LogShopRecord(pCRole->GetdwAccountId(), order_id, dollar, 0, 0, E_DC_GIVE);	
		}
		
		if (type < E_OBJ_MAX || type == E_OBJ_CARD_MORE)
		{
			CShopEffect::GiveGameProductSpecial(pCRole, type, para1, para2, tmp, SOURCE_MAIL);
		}
	}
	else
	{
		// to do data update to client about email
		if (!CDBCtrl::UpdateMail(pCRole->GetdwAccountId(), mail_id))
		{
			IME_ERROR("update mail fail from not do to do userid %u,mail %u", pCRole->GetdwAccountId(), mail_id);
			pCRole->SendErrorMsg(ERRNO_MSG_DB_NO_DATA_OR_ERROR, CMD_SC_MAIL_HANDLE_RESULT);
			return;
		}
		EmailUpdate(pCRole, UPDATE, NULL, mail_id);
	}
//	else if (tempMailDetail.byDetailType == SYS_FRIEND_BOSS)
//	{
//		//todo friend gacha 
//		if (pCRole->GetclsCardPacket()->IsFull())
//		{
//			IME_ERROR("card packet is full userid %u", pCRole->GetdwAccountId());
//			pCRole->SendErrorMsg(ERRNO_MSG_CARD_PACKET_NOT_ENOUGH, CMD_SC_MAIL_HANDLE_RESULT);
//			return;
//		}
//
//		ByteBuffer by;
//		by.append((const char *)tempMailDetail.blob.data(), tempMailDetail.blob.size());
//		uint32_t indexid;
//		uint32_t masterindex;
//		uint32_t lv;
//		by >> indexid;
//		by >> masterindex;
//		by >> lv;
//		
//		STC_CONF_FRIEND_BOSS *pConf = CConfFriendBoss::Find(masterindex);
//		if (pConf == NULL)
//		{
//			IME_ERROR("can not find master index %u in friend boss csv", masterindex);
//			pCRole->SendErrorMsg(ERRNO_MSG_CSV_LOAD, CMD_SC_MAIL_HANDLE_RESULT);
//			return;
//		}
//	}

	WorldPacket info(CMD_SC_MAIL_HANDLE_RESULT);
	info << (uint16_t) 0;
	info << mail_id;
	info << opt;
	info << ObjType.coin;
	info << ObjType.dollar;
	info << ObjType.energy;
	info << ObjType.strength;
	info << ObjType.worldhistory;
	info << tmp.effect;
	info << tmp.para1;
	info << tmp.para2;
	pCRole->SendPacket(&info);

	return ;
}		/* -----  end of method CMail::HandlerEmailOpt  ----- */



void
CMail::HandlerEmailDel (CRole *pCRole, WorldPacket &pkg)
{
	uint32_t user_id;
	uint32_t mail_id;
	pkg >> user_id;
	pkg >> mail_id;
	IME_CHAR_LOG("action [userid %u del mail %u send userid %u]", pCRole->GetdwAccountId(), mail_id, user_id);

	if (!CDBCtrl::DelMail(pCRole->GetdwAccountId(), mail_id))
	{
		IME_ERROR("delete mail fail");
		pCRole->SendErrorMsg(ERRNO_MSG_DB_NO_DATA_OR_ERROR, CMD_SC_MAIL_DEL_RESULT);
		return;
	}
	
	EmailUpdate(pCRole, DEL, NULL, mail_id);	

	WorldPacket info(CMD_SC_MAIL_DEL_RESULT);
	info << (uint16_t) 0;
	info << (uint32_t) mail_id;
	pCRole->SendPacket(&info);

	return ;
}		/* -----  end of method CMail::HandlerEmailDel  ----- */


void
CMail::EmailUpdate (CRole *pCRole, uint8_t cmd, CDBCtrl::stuMailDetail *pmail, uint32_t mailid)
{
	if (cmd == ADD)
	{
		WorldPacket info(CMD_SC_MAIL_INFO_CHANGE);
		info << (uint8_t) ADD;
		info << pmail->dwMailId;
		info << pmail->dwSendId;
		if (pmail->byType == MAIL_SYSTEM_TYPE)
		{
			info << GlobalConfig::SystemMailSendName;
		}
		else
		{
			info << pmail->strName;
		}
		info << pmail->byDoflag;
		info << pmail->byType;
		info << pmail->dwTime;
		info << pmail->byDetailType;
		info << pmail->title;
		pCRole->SendPacket(&info);
	}
	else if (cmd == DEL)
	{
		WorldPacket info(CMD_SC_MAIL_INFO_CHANGE);
		info << (uint8_t) DEL;
		info << mailid;	
		pCRole->SendPacket(&info);
	}
	else if (cmd == UPDATE)
	{
		WorldPacket info(CMD_SC_MAIL_INFO_CHANGE);
		info << (uint8_t) UPDATE;
		info << mailid;	
		pCRole->SendPacket(&info);
	}
	else
	{
		IME_ERROR("unvalid cmd %u about mail update type", cmd);
	}
	return ;
}		/* -----  end of method CMail::HandlerUpdate  ----- */

int
CMail::AddEmailFriendText (CRole *pCRole, uint32_t receiverid, const std::string &content, uint32_t &mail_id, CDBCtrl::stuMailDetail &temp)
{
	mail_id = CDBCtrl::GetMailId(receiverid);
	temp.dwMailId = mail_id;
	temp.dwSendId = pCRole->GetdwAccountId();
	temp.strName = pCRole->GetstrUserName();
	temp.byDoflag = 0;
	temp.byType = MAIL_FRIEND_TYPE;
	temp.dwTime = time(NULL);
	temp.content = content;
	CNameChecker::ReplaceString(temp.content);
	temp.blob = "";
	temp.byDetailType = FRIEND_TEXT; 
	if (!CDBCtrl::InsertMail(receiverid, temp))
		return -1;
	return 0;
}		/* -----  end of method CMail::AddEmailFriendText  ----- */


//int
//CMail::AddEmailFriendRequest (CRole *pCRole, uint32_t receiverid, uint32_t &mail_id, CDBCtrl::stuMailDetail &temp)
//{
//	mail_id = CDBCtrl::GetMailId(receiverid);
//	temp.dwMailId = mail_id;
//	temp.dwSendId = pCRole->GetdwAccountId();
//	temp.strName = pCRole->GetstrUserName();
//	temp.byDoflag = 0;
//	temp.byType = MAIL_FRIEND_TYPE;
//	temp.dwTime = time(NULL);
//	temp.content = "";
//	ByteBuffer blob;
//	blob << (uint32_t)FRIEND_REQUSET_INDEX; 
//	blob << pCRole->GetstrUserName();
//	temp.blob.assign((const char *)blob.contents(), blob.size());
//	temp.byDetailType = FRIEND_REQUSET; 
//	if (!CDBCtrl::InsertMail(receiverid, temp))
//		return -1;
//	return 0;
//}		/* -----  end of method CMail::AddEmailFriendRequest  ----- */
//
//
//int
//CMail::AddEmailFriendAgree (CRole *pCRole, uint32_t receiverid, uint32_t &mail_id, CDBCtrl::stuMailDetail &temp)
//{
//	mail_id = CDBCtrl::GetMailId(receiverid);
//	temp.dwMailId = mail_id;
//	temp.dwSendId = pCRole->GetdwAccountId();
//	temp.strName = pCRole->GetstrUserName();
//	temp.byDoflag = 0;
//	temp.byType = MAIL_FRIEND_TYPE;
//	temp.dwTime = time(NULL);
//	temp.content = "";
//	ByteBuffer blob;
//	blob << (uint32_t)FRIEND_AGREE_INDEX; 
//	blob << pCRole->GetstrUserName();
//	temp.blob.assign((const char *)blob.contents(), blob.size());
//	temp.byDetailType = FRIEND_AGREE; 
//	if (!CDBCtrl::InsertMail(receiverid, temp))
//		return -1;
//	return 0;
//}		/* -----  end of method CMail::AddEmailFriendAgree  ----- */
//
//int 
//CMail::AddEmailFriendDel (CRole *pCRole, uint32_t receiverid, uint32_t &mail_id, CDBCtrl::stuMailDetail &temp)
//{
//	mail_id = CDBCtrl::GetMailId(receiverid);
//	temp.dwMailId = mail_id;
//	temp.dwSendId = pCRole->GetdwAccountId();
//	temp.strName = pCRole->GetstrUserName();
//	temp.byDoflag = 0;
//	temp.byType = MAIL_FRIEND_TYPE;
//	temp.dwTime = time(NULL);
//	temp.content = "";
//	ByteBuffer blob;
//	blob << (uint32_t)FRIEND_DEL_INDEX; 
//	blob << pCRole->GetstrUserName();
//	temp.blob.assign((const char *)blob.contents(), blob.size());
//	temp.byDetailType = FRIEND_DEL; 
//	if (!CDBCtrl::InsertMail(receiverid, temp))
//		return -1;
//	return 0;
//}		/* -----  end of method CMail::AddEmailFriendDel  ----- */

//int 
//CMail::AddEmailDurationWin (CRole *pCRole, uint32_t receiverid, uint32_t &mail_id, CDBCtrl::stuMailDetail &temp, uint32_t coin)
//{
//	mail_id = CDBCtrl::GetMailId(receiverid);
//	temp.dwMailId = mail_id;
//	temp.dwSendId = pCRole->GetdwAccountId();
//	temp.strName = pCRole->GetstrUserName();
//	temp.byDoflag = 0;
//	temp.byType = MAIL_DURATION_TYPE;
//	temp.dwTime = time(NULL);
//	temp.content = "";
//	ByteBuffer blob;
//	blob << (uint32_t)DURATION_COIN_INDEX; 
//	blob << pCRole->GetstrUserName();
//	blob << coin;
//	temp.blob.assign((const char *)blob.contents(), blob.size());
//	temp.byDetailType = DURATION_COIN; 
//	if (!CDBCtrl::InsertMail(receiverid, temp))
//		return -1;
//	return 0;
//}

int 
CMail::AddEmailRobGet (CRole *pCRole, uint32_t receiverid, uint32_t &mail_id, CDBCtrl::stuMailDetail &temp, uint32_t piecesindex)
{
	mail_id = CDBCtrl::GetMailId(receiverid);
	temp.dwMailId = mail_id;
	temp.dwSendId = pCRole->GetdwAccountId();
	temp.strName = pCRole->GetstrUserName();
	temp.byDoflag = 0;
	temp.byType = MAIL_DURATION_TYPE;
	temp.dwTime = time(NULL);
	temp.content = "";
	ByteBuffer blob;
	blob << (uint32_t)ROB_GET_INDEX; 
	blob << pCRole->GetstrUserName();
	blob << piecesindex;
	temp.blob.assign((const char *)blob.contents(), blob.size());
	temp.byDetailType = ROB_GET; 
	if (!CDBCtrl::InsertMail(receiverid, temp))
		return -1;
	return 0;
}

int 
CMail::AddEmailRobLose (CRole *pCRole, uint32_t receiverid, uint32_t &mail_id, CDBCtrl::stuMailDetail &temp, uint32_t piecesindex)
{
	mail_id = CDBCtrl::GetMailId(receiverid);
	temp.dwMailId = mail_id;
	temp.dwSendId = pCRole->GetdwAccountId();
	temp.strName = pCRole->GetstrUserName();
	temp.byDoflag = 0;
	temp.byType = MAIL_DURATION_TYPE;
	temp.dwTime = time(NULL);
	temp.content = "";
	ByteBuffer blob;
	blob << (uint32_t)ROB_LOSE_INDEX; 
	blob << pCRole->GetstrUserName();
	blob << piecesindex;
	temp.blob.assign((const char *)blob.contents(), blob.size());
	temp.byDetailType = ROB_LOSE; 
	if (!CDBCtrl::InsertMail(receiverid, temp))
		return -1;
	return 0;
}

//int 
//CMail::AddEmailDurationLose (CRole *pCRole, uint32_t receiverid, uint32_t &mail_id, CDBCtrl::stuMailDetail &temp, uint32_t coin)
//{
//	mail_id = CDBCtrl::GetMailId(receiverid);
//	temp.dwMailId = mail_id;
//	temp.dwSendId = pCRole->GetdwAccountId();
//	temp.strName = pCRole->GetstrUserName();
//	temp.byDoflag = 0;
//	temp.byType = MAIL_DURATION_TYPE;
//	temp.dwTime = time(NULL);
//	temp.content = "";
//	ByteBuffer blob;
//	blob << (uint32_t)DURATION_LOSE_INDEX; 
//	blob << pCRole->GetstrUserName();
//	blob << coin;
//	temp.blob.assign((const char *)blob.contents(), blob.size());
//	temp.byDetailType = DURATION_LOSE; 
//	if (!CDBCtrl::InsertMail(receiverid, temp))
//		return -1;
//	return 0;
//}

int 
CMail::AddEmailSysPay (uint32_t receiverid, uint32_t &mail_id, CDBCtrl::stuMailDetail &temp, uint32_t type, uint32_t para1, uint32_t para2, const SBaseAttr &attr)
{
	mail_id = CDBCtrl::GetMailId(receiverid);
	temp.dwMailId = mail_id;
	temp.dwSendId = 0;
	temp.strName = "";
	temp.byDoflag = 0;
	temp.byType = MAIL_SYSTEM_TYPE;
	temp.dwTime = time(NULL);
	temp.content = "";
	if (type >= E_OBJ_MAX)
		type = 0;
	ByteBuffer blob;
	blob << (uint32_t)SYS_PAY_INDEX; 
	blob << type;
	blob << para1;
	blob << para2;
	blob << attr.coin;
	blob << attr.dollar;
	blob << attr.energy;
	blob << attr.strength;
	blob << attr.worldhistory;
	temp.blob.assign((const char *)blob.contents(), blob.size());
	temp.byDetailType = SYS_PAY; 
	if (!CDBCtrl::InsertMail(receiverid, temp))
		return -1;
	return 0;
}

int 
CMail::AddEmailSysGift (uint32_t receiverid, uint32_t &mail_id, CDBCtrl::stuMailDetail &temp, uint32_t type, uint32_t para1, uint32_t para2, std::string &straction, const SBaseAttr &attr)
{
	mail_id = CDBCtrl::GetMailId(receiverid);
	temp.dwMailId = mail_id;
	temp.dwSendId = 0;
	temp.strName = "";
	temp.byDoflag = 0;
	temp.byType = MAIL_SYSTEM_TYPE;
	temp.dwTime = time(NULL);
	temp.content = "";
	if (type >= E_OBJ_MAX)
		type = 0;
	ByteBuffer blob;
	blob << (uint32_t)SYS_GIFT_INDEX; 
	blob << type;
	blob << para1;
	blob << para2;
	blob << straction;
	blob << attr.coin;
	blob << attr.dollar;
	blob << attr.energy;
	blob << attr.strength;
	blob << attr.worldhistory;
	temp.blob.assign((const char *)blob.contents(), blob.size());
	temp.byDetailType = SYS_GIFT; 
	if (!CDBCtrl::InsertMail(receiverid, temp))
		return -1;
	return 0;
}

//int 
//CMail::AddEmailSysGift (uint32_t receiverid, uint32_t &mail_id, CDBCtrl::stuMailDetail &temp, uint32_t master, uint32_t lv, std::string &straction)
//{
//	mail_id = CDBCtrl::GetMailId(receiverid);
//	temp.dwMailId = mail_id;
//	temp.dwSendId = 0;
//	temp.strName = "";
//	temp.byDoflag = 0;
//	temp.byType = MAIL_SYSTEM_TYPE;
//	temp.dwTime = time(NULL);
//	temp.content = "";
//	ByteBuffer blob;
//	blob << (uint32_t)SYS_FRIEND_BOSS_INDEX; 
//	blob << master;
//	blob << lv;
////	blob << straction;
//	temp.blob.assign((const char *)blob.contents(), blob.size());
//	temp.byDetailType = SYS_GIFT; 
//	if (!CDBCtrl::InsertMail(receiverid, temp))
//		return -1;
//	return 0;
//}

int 
CMail::AddEmailSysDiy (uint32_t receiverid, uint32_t &mail_id, CDBCtrl::stuMailDetail &temp, uint32_t type, uint32_t para1, uint32_t para2, std::string &straction, const SBaseAttr &attr, std::string &title)
{
	mail_id = CDBCtrl::GetMailId(receiverid);
	temp.dwMailId = mail_id;
	temp.dwSendId = 0;
	temp.strName = "";
	temp.byDoflag = 0;
	temp.byType = MAIL_SYSTEM_TYPE;
	temp.dwTime = time(NULL);
	temp.content = straction;
	temp.title = title;
	if (type >= E_OBJ_MAX && type != E_OBJ_CARD_MORE)
	{
		IME_ERROR("type is unvalid %u", type);
		type = 0;
	}
	ByteBuffer blob;
	blob << type;
	blob << para1;
	blob << para2;
//	blob << straction;
	blob << attr.coin;
	blob << attr.dollar;
	blob << attr.energy;
	blob << attr.strength;
	blob << attr.worldhistory;
	temp.blob.assign((const char *)blob.contents(), blob.size());
	temp.byDetailType = SYS_DIY; 
	if (!CDBCtrl::InsertMail(receiverid, temp))
		return -1;
	return 0;
}

