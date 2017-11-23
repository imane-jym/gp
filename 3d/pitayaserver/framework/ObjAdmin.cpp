/*
 * =====================================================================================
 *
 *       Filename:  ObjAdmin.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2013年03月21日 05时00分53秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  imane (), imanecr@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include "ObjAdmin.h"
/*
 *--------------------------------------------------------------------------------------
 *       Class:  CObjCtrl
 *      Method:  CObjCtrl
 * Description:  constructor
 *--------------------------------------------------------------------------------------
 */
CObjCtrl::CObjCtrl ()
{
}  /* -----  end of method CObjCtrl::CObjCtrl  (constructor)  ----- */

CObjCtrl::~CObjCtrl()
{
	std::list<CStu *>::iterator it;
	for (it = m_listIndex.begin(); it != m_listIndex.end(); it++)
	{
		delete *it;
	}
}

int
CObjCtrl::Init (uint32_t max_capacity)
{
	if (max_capacity == 0)
		return -1;
	m_MaxCapacity = max_capacity;
	return 0;
}		/* -----  end of method CObjCtrl::Init  ----- */

CStu *CObjCtrl::GetData(uint32_t key)
{
	CStu *p;
	if (m_mapData.find(key) == m_mapData.end())
	{
//		IME_LOG("online %u, MaxCapacity %u", m_mapData.size(), m_MaxCapacity);
		if (m_mapData.size() >= m_MaxCapacity)
		{
			// delete oldest node
			std::list<CStu *>::reverse_iterator ritdel;
			CStu *ptemp;
			for (ritdel = m_listIndex.rbegin(); ritdel != m_listIndex.rend(); ritdel++)
			{
				if (m_ResidentData.find((*ritdel)->key) == m_ResidentData.end())
				{
					ptemp = *ritdel;	
					if (!SaveInnerData(ptemp))
					{
						IME_ERROR("delete oldest element save data fail key %u", ptemp->key);
						return NULL;
					}
					m_mapData.erase(ptemp->key);
					m_listIndex.erase((++ritdel).base());
					delete ptemp;
					break;
				}
			}
			if (ritdel == m_listIndex.rend())
			{
				IME_ERROR("have not enough memory");
				return NULL;
			}
		}

		p = GetInnerData(key);
		if (p == NULL)
		{
			return NULL;
		}
		p->key = key;
		p->timestamp = time(NULL);
		m_listIndex.insert(m_listIndex.begin(), p);
		m_mapData[key] = p;
//		p->it = m_listIndex.begin();
	}
	else
	{
		p = m_mapData[key];
		std::list<CStu *>::iterator it;
		for (it = m_listIndex.begin(); it != m_listIndex.end(); it++)
		{
			if ((*it)->key == key)
			{
				m_listIndex.erase(it);
				break;
			}
		}
		m_listIndex.insert(m_listIndex.begin(), p);
	}
	return p;
}

void CObjCtrl::OnTimer()
{
	std::list<CStu *>::iterator it,itdel;
	for (it = m_listIndex.begin(); it != m_listIndex.end();)
	{
		uint32_t t = time(NULL);
		if ((*it)->timestamp + MAX_ALIVE_TIME < t && m_ResidentData.find((*it)->key) == m_ResidentData.end() )
		{
			if (!SaveInnerData(*it))
			{
				IME_ERROR("save data fail key %u", (*it)->key);
				return;
			}
			else
			{
				itdel = it;	
				it++;
				CStu *ptemp = *itdel;
				m_mapData.erase((*itdel)->key);
				m_listIndex.erase(itdel);
				delete ptemp;
			}
		}
		else
		{
			it++;
		}
	}
	return;
}

void CObjCtrl::DataDump()
{
	IME_LOG("============obj data dump=============");
	IME_LOG("============max capacity %u============", m_MaxCapacity);
	IME_LOG("============list size %u============", (uint32_t)m_listIndex.size());
	std::list<CStu *>::iterator it;
	for (it = m_listIndex.begin(); it != m_listIndex.end(); it++)
	{
		IME_LOG("obj admin key %u, timestamp %u", (*it)->key, (*it)->timestamp);
	}
	IME_LOG("============map size %u============", (uint32_t)m_mapData.size());
	std::map<uint32_t, CStu*>::iterator mit;
	for (mit = m_mapData.begin(); mit != m_mapData.end(); mit++)
	{
		IME_LOG("obj admin map key %u, val %u", mit->first, mit->second->timestamp);
	}
	return;
}

void CObjCtrl::SaveAllData()
{
	std::list<CStu *>::iterator it,itdel;
	for (it = m_listIndex.begin(); it != m_listIndex.end(); it++)
	{
		SaveInnerData(*it);
	}
	return;
}	

void CObjCtrl::AddResidentKey(uint32_t key)
{
	m_ResidentData.insert(key);
}

void CObjCtrl::ClearResidentKey()
{
	m_ResidentData.clear();
}

CStu::CStu()
{

}

CStu::~CStu()
{

}
