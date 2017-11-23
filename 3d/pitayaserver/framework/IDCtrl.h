/*
 * IDCtrl.h
 *
 *  Created on: 2011-7-4
 *      Author: yq
 */

#ifndef IDCTRL_H_
#define IDCTRL_H_
#include "Defines.h"

struct IDNode
{
	IDNode():dwID(0) {}
	IDNode * pPrev;
	IDNode * pNext;
	uint32 dwID;

};

class CLinkedList
{
public:
	CLinkedList();
	~CLinkedList();
	void Append(IDNode * pNode);
	IDNode * GetNext();
	void Remove(IDNode * pNode);
private:
	IDNode * pHead;
	IDNode * pTail;
	IDNode * pCurPosition;
};

////////////////////////////////////////////////
class CObjID
{
public:
	CObjID(uint32 dwIDStart, uint32 dwIDEnd, uint32 dwStep, uint32 dwIDInitNum, uint32 dwIncrease);
	~CObjID();
	IDNode * GetNext();
	void Remove(IDNode * pNode);
	uint32 GetCount() { return m_dwCount;}
private:
	void Resize(uint32 dwNum, uint32 dwStep);
	CLinkedList * pLinkList;
	uint32 m_dwIDStart;
	uint32 m_dwIDNumMax;
	uint32 m_dwStep;
	uint32 m_dwIncrease;
	uint32 m_dwCount;
};

////////////////////////////////////////////////
class CObjIDCtrl
{
public:
	CObjIDCtrl();
	~CObjIDCtrl();
	enum
	{
		ID_PET_BEGIN		= 0x00000000,
		ID_PET_END			= 0x50000000,

		ID_ITEM_BEGIN		= 0x50000001,
		ID_ITEM_END			= 0x90000000,

		STEP_NORMAL			= 1,
		STEP_TWO			= 2,
		STEP_THREE			= 3,
	};
	static void Init();
	static void UnInit();

	static void GetCount(uint32 & dwCountA, uint32 & dwCountB);

	static IDNode * GetPetID();

	static void DelPetID(IDNode * pNode);

	static IDNode * GetItemID();

	static void DelItemID(IDNode * pNode);

private:
	static CObjID * m_pPetID;
	static CObjID * m_pItemID;

};

#endif /* IDCTRL_H_ */
