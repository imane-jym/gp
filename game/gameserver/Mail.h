/*
 * =====================================================================================
 *
 *       Filename:  mail.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2012年11月11日 03时23分57秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  imane (), imanecr@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef _MAIL_H_
#define _MAIL_H_

#include <stdint.h>
#include "WorldPacket.h"
#include "DBCtrl.h"

class CRole;
class CMail
{
	public:
		enum 
		{
			ADD,
			DEL,
			UPDATE
		};

		typedef struct BaseAttr
		{
			uint32_t coin;
			uint32_t dollar;
			uint32_t energy;
			uint32_t strength;
			uint32_t worldhistory;
		}SBaseAttr;

		static void HandlerEmailInfo(CRole *pCRole);
		static void EmailUpdate(CRole *pCRole, uint8_t cmd, CDBCtrl::stuMailDetail *pmail, uint32_t mailid);
		static void HandlerGetEmailDetailInfo(CRole *pCRole, WorldPacket &pkg);
		static void HandlerEmailOpt(CRole *pCRole, WorldPacket &pkg);
		static void HandlerEmailDel(CRole *pCRole, WorldPacket &pkg);
		
		static int AddEmailFriendText(CRole *pCRole, uint32_t receiverid, const std::string &content, uint32_t &mail_id, CDBCtrl::stuMailDetail &mail);
//		static int AddEmailFriendRequest(CRole *pCRole, uint32_t receiverid, uint32_t &mail_id, CDBCtrl::stuMailDetail &mail);
//		static int AddEmailFriendAgree(CRole *pCRole, uint32_t receiverid, uint32_t &mail_id, CDBCtrl::stuMailDetail &mail);
//		static int AddEmailFriendDel(CRole *pCRole, uint32_t receiverid, uint32_t &mail_id, CDBCtrl::stuMailDetail &mail);

//		static int AddEmailDurationWin (CRole *pCRole, uint32_t receiverid, uint32_t &mail_id, CDBCtrl::stuMailDetail &temp, uint32_t coin);
		static int AddEmailRobGet (CRole *pCRole, uint32_t receiverid, uint32_t &mail_id, CDBCtrl::stuMailDetail &temp, uint32_t piecesindex);
		static int AddEmailRobLose (CRole *pCRole, uint32_t receiverid, uint32_t &mail_id, CDBCtrl::stuMailDetail &temp, uint32_t piecesindex);
//		static int AddEmailDurationLose (CRole *pCRole, uint32_t receiverid, uint32_t &mail_id, CDBCtrl::stuMailDetail &temp, uint32_t coin);
		static int AddEmailSysPay (uint32_t receiverid, uint32_t &mail_id, CDBCtrl::stuMailDetail &temp, uint32_t type, uint32_t para1, uint32_t para2, const SBaseAttr &attr);
		static int AddEmailSysGift (uint32_t receiverid, uint32_t &mail_id, CDBCtrl::stuMailDetail &temp, uint32_t type, uint32_t para1, uint32_t para2, std::string &straction, const SBaseAttr &attr);
		static int AddEmailSysDiy (uint32_t receiverid, uint32_t &mail_id, CDBCtrl::stuMailDetail &temp, uint32_t type, uint32_t para1, uint32_t para2, std::string &straction, const SBaseAttr &attr, std::string &title);
};

#endif
