/*
 * =====================================================================================
 *
 *       Filename:  CardUp.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2012年10月16日 11时02分06秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  imane (), imanecr@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef _CARDUP_H_
#define _CARDUP_H_

#include <stdint.h> 
#include "WorldPacket.h"

class CRole;
/*
 * =====================================================================================
 *        Class:  CCardUp
 *  Description:  
 * =====================================================================================
 */
class CCardUp
{
	public:
		/* ====================  LIFECYCLE     ======================================= */
		CCardUp ();                             /* constructor */

		/* ====================  OPERATORS     ======================================= */
//		static void HandlerCardEvolution(CRole *pCRole, WorldPacket &pkg);
		static void HandlerSpecialCardStrength(CRole *pCRole, WorldPacket &pkg);
//		static void HandlerCardSkillFlush(CRole *pCRole, WorldPacket &pkg);
		static void HandlerNormalCardStrength(CRole *pCRole, WorldPacket &pkg);
		static void HandlerSwitchCardExp(CRole *pCRole, WorldPacket &pkg);
		static void HandlerSkillStrengthReview(CRole *pCRole, WorldPacket &pkg);
}; /* -----  end of class CCardUp  ----- */

#endif
