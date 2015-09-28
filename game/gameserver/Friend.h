/*
 * =====================================================================================
 *
 *       Filename:  Friend.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2012年11月09日 03时35分06秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  imane (), imanecr@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef _FRIEND_H_
#define _FRIEND_H_

class CRole;

/*
 * =====================================================================================
 *        Class:  CFriend
 *  Description:  
 * =====================================================================================
 */
class CFriend
{
	public:
		/* ====================  LIFECYCLE     ======================================= */

		/* ====================  OPERATORS     ======================================= */
		static void HandlerAutoPlayerInFriend(CRole *pCRole, WorldPacket &pkg);

		static void HandlerFriendTeamRequest(CRole *pCRole, WorldPacket &pkg);

		static void HandlerAutoPlayerForFriend(CRole *pCRole, WorldPacket &pkg);

		static void HandlerAddFriend(CRole *pCRole, WorldPacket &pkg, uint8_t IsInner = 0);

		static void HandlerDelFriend(CRole *pCRole, WorldPacket &pkg);

		static void HandlerSearchPlayer(CRole *pCRole, WorldPacket &pkg);

		static void HandlerGetFriendNumber(CRole *pCRole, WorldPacket &pkg);

		static void HandlerSendFriendEmail(CRole *pCRole, WorldPacket &pkg);

		static void HandlerAgreeFriend(CRole *pCRole, WorldPacket &pkg);

		static void HandlerFriendApplyRequest(CRole *pCRole, WorldPacket &pkg);
		/* ====================  DATA MEMBERS  ======================================= */

}; /* -----  end of class CFriend  ----- */


#endif
