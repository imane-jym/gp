/*
 * IDCtrl.cpp
 *
 *  Created on: 2011-7-4
 *      Author: yq
 */

#include "IDCtrl.h"
#include "Log.h"
#include <stdio.h>

CLinkedList::CLinkedList()
{
	pHead = pTail = pCurPosition = NULL;
}

CLinkedList::~CLinkedList()
{
	pHead = pTail = pCurPosition = NULL;
}

IDNode * CLinkedList::GetNext()
{
	if (!pCurPosition)
	{
		return NULL;
	}
	IDNode *pCur = pCurPosition;
	pCurPosition = pCurPosition->pNext;
	return pCur;
}

void CLinkedList::Append(IDNode * pNode)
{
	pNode->pPrev = pTail;
	pNode->pNext = NULL;
	if (pTail != NULL)
		pTail->pNext = pNode;
	pTail = pNode;
	if (pHead == NULL)
		pHead = pNode;
	if (pCurPosition == NULL)
		pCurPosition = pNode;
}

void CLinkedList::Remove(IDNode * pNode)
{
	IDNode * pNext;
	IDNode * pPrev;
	pPrev = pNode->pPrev;
	pNext = pNode->pNext;
	if (pPrev != NULL)
		pPrev->pNext = pNext;
	else
		pHead = pNext;
	if (pNext != NULL)
		pNext->pPrev = pPrev;
	else
		pTail = pPrev;

	Append(pNode);
}

////////////////////////////////////////
//------------------------------CObjID-------------------------------------------

CObjID::CObjID(uint32 dwIDStart, uint32 dwIDEnd, uint32 dwStep, uint32 dwIDInitNum, uint32 dwIncrease)
:m_dwIDStart(dwIDStart), m_dwIDNumMax(dwIDEnd - dwIDStart), m_dwStep(dwStep), m_dwIncrease(dwIncrease)
{
	pLinkList = new CLinkedList();
	Resize(dwIDInitNum, dwStep);
	m_dwCount = 0;
}

CObjID::~CObjID()
{
	delete pLinkList;
}

IDNode* CObjID::GetNext()
{
	IDNode* pNode = pLinkList->GetNext();
	if (!pNode)
	{
		if (m_dwCount + m_dwIncrease * m_dwStep >= m_dwIDNumMax)
		{
			IME_LOG("CObjID::GetNext count=%u, maxnum=%u", m_dwCount, m_dwIDNumMax);
			return NULL;
		}
		Resize(m_dwIncrease, m_dwStep);
		pNode = pLinkList->GetNext();
		if (!pNode)
		{
			IME_LOG("CObjID::GetNext null");
		}
	}
	if (pNode)
		m_dwCount++;
	return pNode;
}

void CObjID::Remove(IDNode* pNode)
{
	pLinkList->Remove(pNode);
	m_dwCount--;
}

void CObjID::Resize(uint32 dwNum, uint32 dwStep)
{
	IDNode* pNode;
	uint32 dwI, dwEnd = dwNum * dwStep + m_dwIDStart;
	for (dwI = m_dwIDStart; dwI < dwEnd;)
	{
		pNode = new IDNode;
		if (!pNode)
		{
			IME_LOG("CObjID::Resize null");
			m_dwIDStart = dwI - dwStep;
			return;
		}
		pNode->dwID = dwI;
		pLinkList->Append(pNode);
		dwI += dwStep;
	}
	m_dwIDStart = dwI;
}

//------------------------------CObjIDCtrl-------------------------------------------
CObjID* CObjIDCtrl::m_pPetID;
CObjID* CObjIDCtrl::m_pItemID;

CObjIDCtrl::CObjIDCtrl()
{
}

CObjIDCtrl::~CObjIDCtrl()
{
}

void CObjIDCtrl::Init()
{
	m_pPetID = new CObjID(ID_PET_BEGIN, ID_PET_END, STEP_TWO, 25000, 1000);
	m_pItemID = new CObjID(ID_ITEM_BEGIN, ID_ITEM_END, STEP_THREE, 20000, 1000);
}

void CObjIDCtrl::UnInit()
{
	delete m_pPetID;
	delete m_pItemID;
}

//得到数据
void CObjIDCtrl::GetCount(uint32 & dwCountA, uint32 & dwCountB)
{
	dwCountA = m_pPetID->GetCount();
	dwCountB = m_pItemID->GetCount();
}

//得到生物非人物ID
IDNode * CObjIDCtrl::GetPetID()
{
	return m_pPetID->GetNext();
}

//释放生物非人物ID
void CObjIDCtrl::DelPetID(IDNode* pNode)
{
	m_pPetID->Remove(pNode);
}

//得到物品ID
IDNode* CObjIDCtrl::GetItemID()
{
	return m_pItemID->GetNext();
}

//释放物品ID
void CObjIDCtrl::DelItemID(IDNode* pNode)
{
	m_pItemID->Remove(pNode);
}




