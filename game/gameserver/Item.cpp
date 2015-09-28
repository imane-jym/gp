/*
 * =====================================================================================
 *
 *       Filename:  Item.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2012年09月22日 06时20分08秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  imane (), imanecr@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include "Item.h"
#include "DBCtrl.h"
#include "Role.h"
#include "CmdDefine.h"

CItem::CItem():m_IsInit(false),m_dwTotal(3000),m_dwUsed(0)
{
}

CItem::~CItem()
{
}

int CItem::Init(CRole *pCRole)
{
	SetCRole(pCRole);
	if (!LoadData())
	{
		m_IsInit = false;
		IME_ERROR("load data fail");
		return -1;
	}
	m_IsInit = true;
	IME_DEBUG("used item %u", m_dwUsed);
	return 0;
}		/* -----  end of method CItem::Init  ----- */


bool CItem::LoadData ( )
{
	std::string temp;
	if (!CDBCtrl::GetUserItemPacketData(temp, m_pCRole->GetdwAccountId()))
	{
		IME_ERROR("get item data fail, user_id %u", m_pCRole->GetdwAccountId());
		return false;
	}
	ByteBuffer content;
	content.append(temp.data(), temp.size());
	if (DeSerilizeDB(content))
	{
		IME_ERROR("serilizeDB fail");
		return false;
	}
	return true;
}		/* -----  end of method CItem::LoadData  ----- */


int CItem::SaveData ()
{
	if (!m_IsInit)
		return -1;
	ByteBuffer byte;
	if (SerilizeDB(byte))
	{
		IME_ERROR("SerilizeDB fail");
		return -1;
	}
	std::string temp;
	temp.assign((const char *)byte.contents(), byte.size());
	CDBCtrl::SaveUserItemPacketData(temp, m_pCRole->GetdwAccountId());
	return 0;
}		/* -----  end of method CItem::SaveData  ----- */


int CItem::SerilizeDB ( ByteBuffer &byte)
{
	if (m_mapItem.size() == 0)
		return 0;
	byte << (uint32_t)m_mapItem.size();
	std::map<uint32_t, Item_t>::iterator it;
	for (it = m_mapItem.begin(); it != m_mapItem.end(); it++)
	{
		byte << (it->second).dwObjId;
		byte << (it->second).dwNum;
	}

	return 0;
}		/* -----  end of method CItem::SerilizeDB  ----- */

int CItem::DeSerilizeDB ( ByteBuffer &byte)
{
	if (byte.size() == 0)
		return 0;
	uint32_t item_number;
	uint32_t objid;
	uint32_t number;
	byte >> item_number;
	m_dwUsed = item_number;
	for (int i = 0; i < item_number; i++)
	{
		byte >> objid;
		byte >> number;
		Item_t tempItem = {objid, number};
		m_mapItem[objid] = tempItem;
		IME_DEBUG("item objid %u, number %u", objid, number);
	}

	return 0;
}		/* -----  end of method CItem::DeSerilizeDB  ----- */


void
CItem::HandlerItemInfo ()
{
	WorldPacket info(CMD_SC_ROLE_ITEM_PACKET_INFO);
	uint32_t number = m_mapItem.size();
	info << number;
	std::map<uint32_t, Item_t>::iterator it;
	for(it = m_mapItem.begin(); it != m_mapItem.end(); it++)
	{
		info << (it->second).dwObjId;
		info << (it->second).dwNum;
		IME_DEBUG("pieces index %u number %u", (it->second).dwObjId, (it->second).dwNum);
	}
	m_pCRole->SendPacket(&info);	
	return ;
}		/* -----  end of method CItem::HandlerItemInfo  ----- */

int CItem::AddItem(uint32_t objid, int source, uint32_t number)
{
	Item_t CellItem = {objid, number};	
	if (m_mapItem.find(objid) != m_mapItem.end())
	{
		m_mapItem[objid].dwNum += number;
		IME_CHAR_LOG("resource [status=success,res=pieces,opt=add,character=%u,index=%u,number=%u,reason=ok,source=%d]", m_pCRole->GetdwAccountId(), objid, number, source);
		return 0;
	}
	else
	{
		if (m_dwUsed >= m_dwTotal)
		{
			IME_ERROR("total space is enough %u limit %u", m_dwUsed, m_dwTotal);
			return -1;
		}

		if (!CDBCtrl::InsertItemUser(objid, m_pCRole->GetdwAccountId()))
		{
			IME_ERROR("insert itemuser table fail objid %u, userid %u", objid, m_pCRole->GetdwAccountId());
			return -1;
		}

		m_mapItem[objid] = CellItem;

		m_dwUsed++;	
		IME_CHAR_LOG("resource [status=success,res=pieces,opt=add,character=%u,index=%u,number=%u,reason=ok,source=%d]", m_pCRole->GetdwAccountId(), objid, number, source);
		return 0;
	}
}

int CItem::DelItem(uint32_t objid, int source, uint32_t number)
{
	std::map<uint32_t, Item_t>::iterator it;
	it = m_mapItem.find(objid);
	if (it != m_mapItem.end())
	{
		if (m_mapItem[objid].dwNum < number)
		{
			IME_ERROR("its number [%u] small than request [%u]", m_mapItem[objid].dwNum, number);
			return -1;
		}
		else if (m_mapItem[objid].dwNum == number)
		{
			if (!CDBCtrl::DeleteItemUser(objid, m_pCRole->GetdwAccountId()))
			{
				IME_ERROR("delete itemuser table fail objid %u, userid %u", objid, m_pCRole->GetdwAccountId());
				IME_CHAR_LOG("resource [status=success,res=pieces,opt=del,character=%u,index=%u,number=%u,reason=delete item user data fail,source=%d]", m_pCRole->GetdwAccountId(), objid, number, source);
				return -1;
			}
			m_mapItem.erase(it);
			m_dwUsed--;
			IME_CHAR_LOG("resource [status=success,res=pieces,opt=del,character=%u,index=%u,number=%u,reason=ok,source=%d]", m_pCRole->GetdwAccountId(), objid, number, source);
			return 0;
		}
		else
		{
			m_mapItem[objid].dwNum -= number;
			IME_CHAR_LOG("resource [status=success,res=pieces,opt=del,character=%u,index=%u,number=%u,reason=ok,source=%d]", m_pCRole->GetdwAccountId(), objid, number, source);
			return 0;
		}
	}
	else
	{
		IME_ERROR("it is not in packet %u", objid);
		return -1;
	}
}

void CItem::ItemUpdate(uint32_t objid, uint8_t action, uint32_t number)
{
	WorldPacket info(CMD_SC_ROLE_ITEM_PACKET_CHANGE);
	if (action == ADD)
	{
		info << (uint32_t)1;
		info << (uint8_t)ADD;
		info << objid;
		info << number;
	}
	else if (action == DEL)
	{
		info << (uint32_t)1;
		info << (uint8_t)DEL;
		info << objid;
		info << number;
	}
	else
	{
		IME_ERROR("unknow action %u", action);
		return;
	}
	m_pCRole->SendPacket(&info);
}

int CItem::JudgeItemNumber(uint32_t objid, uint32_t number)
{
	std::map<uint32_t, Item_t>::iterator it;
	it = m_mapItem.find(objid);
	if (it != m_mapItem.end())
	{
		if (m_mapItem[objid].dwNum < number)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	else
	{
		IME_ERROR("it is not in packet %u", objid);
		return -1;
	}
}
