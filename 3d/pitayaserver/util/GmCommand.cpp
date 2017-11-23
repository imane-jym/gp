/*
 * =====================================================================================
 *
 *       Filename:  GmCommand.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  03/19/2015 04:45:56 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  imane (), imanecr@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include "GmCommand.h"
#include "DatabaseMysql.h"
#include "CenterDBCtrl.h"
#include "Util.h"
#include "stdlib.h"

using namespace CenterDB;

pFuncCreate GmCommandFactory::m_f1 = NULL;
pFuncCheck GmCommandFactory::m_f2 = NULL;

void GmCommandFactory::RegisterFunc(pFuncCreate f1, pFuncCheck f2)
{
	m_f1 = f1;
	m_f2 = f2;
}

GmCommand* GmCommandFactory::Create(
			uint32				dwAutoId,
			std::string&		strOpr,
			GmCommandTargetType	eTargetType,
			uint64				dwTargetId,
			uint32				dwStartTime,
			uint32				dwEndTime,
			std::string&		strBackup,
			GmCommandStatus		eStatus,
			std::string&		strParams )
{
	GmCommand* ret = 0;
	if (m_f1)
	{
		ret = m_f1(dwAutoId, strOpr, eTargetType, dwTargetId, dwStartTime, dwEndTime, strBackup, eStatus, strParams);
	}

	return ret;
}

bool GmCommandFactory::CheckTargetType( uint8 byTargetType, uint64 dwTargetId )
{
	if (m_f2)
	{
		return m_f2(byTargetType, dwTargetId);	
	}
	return false;
}

