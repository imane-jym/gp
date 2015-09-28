/*
 *  WorldDb.h
 *
 *  Created on: 2011-12-15
 *      Author: hx
 */

#include "WorldDb.h"
#include "DatabaseMysql.h"
#include "Log.h"

#include <assert.h>


CWorldDb::CWorldDb()
:m_nShardingDBNum(0)
{
	m_vecDbMysql.clear();
}
CWorldDb::~CWorldDb()
{
	for(unsigned int i = 0; i < m_vecDbMysql.size(); ++i)
	{
		delete m_vecDbMysql[i];
	}
}

bool CWorldDb::Init(int nShardingDBNum, const std::vector<std::string> & vecDBInfos)
{
	assert(nShardingDBNum > 0 && nShardingDBNum <= 100 && (100 % nShardingDBNum) == 0 );
	IME_LOG("WORLD DB NUM[num=%u]", nShardingDBNum);
	m_nShardingDBNum = nShardingDBNum;
	if (vecDBInfos.size() != (size_t)m_nShardingDBNum)
	{
		IME_ERROR("ERROR INT CONFIG[%u %u]", (uint32_t)vecDBInfos.size(), m_nShardingDBNum);
		return false;
	}
	m_nDbNumPerMysql = 100 / nShardingDBNum;
	if (m_nDbNumPerMysql ==0)
	{
		IME_ERROR("ERROR INT DB NUM[SharndingNum=%u]", nShardingDBNum);
		return false;
	}
	for(int i = 0; i < m_nShardingDBNum; ++i)
	{
		DatabaseMysql * db = new DatabaseMysql();
		assert(db);
		if (!db->Initialize(vecDBInfos[i].c_str()))
		{
			delete db;
			return false;
		}
		m_vecDbMysql.push_back(db);
	}
	IME_LOG("WORLD DB INIT NUM[%u]", (uint32_t)m_vecDbMysql.size());
	return true;
}

DatabaseMysql * CWorldDb::GetDBConnection(uint32 uShardingId)
{
	assert(m_nShardingDBNum > 0);

	int nDBLink = (uShardingId % 100) / m_nDbNumPerMysql;

	assert((size_t)nDBLink < m_vecDbMysql.size());
	return m_vecDbMysql[nDBLink];
}
void CWorldDb::Ping()
{
	for(size_t i = 0; i < m_vecDbMysql.size(); ++i)
	{
		m_vecDbMysql[i]->Ping();
	}
}

uint32 CWorldDb::GetDBIdx(uint32 dwAccountId)
{
	//return (dwAccountId % 100);
	return dwAccountId % 10;
};
uint32 CWorldDb::GetTable10Idx(uint32 dwAccountId)
{
	return (dwAccountId / 100) % 10;
}
uint32 CWorldDb::GetTable100Idx(uint32 dwAccountId)
{
	//return (dwAccountId / 100) % 100;
	return (dwAccountId / 100) % 10;
}
