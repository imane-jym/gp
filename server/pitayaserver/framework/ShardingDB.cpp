#include "ShardingDB.h"
#include "DatabaseMysql.h"
#include <vector>
#include <string>
#include <assert.h>


CShardingDB::CShardingDB()
:m_nShardingDBNum(0)
{
	m_vecDBs.clear();
}
CShardingDB::~CShardingDB()
{
	for(size_t i = 0; i < m_vecDBs.size(); ++i)
	{
		delete m_vecDBs[i];
	}
}

bool CShardingDB::Init(int nShardingDBNum, const std::vector<std::string> & vecDBInfos)
{
	assert(nShardingDBNum > 0 && nShardingDBNum < 100);
	m_nShardingDBNum = nShardingDBNum;
	if (vecDBInfos.size() != (size_t)m_nShardingDBNum)
		return false;
	for(int i = 0; i < m_nShardingDBNum; ++i)
	{
		DatabaseMysql * db = new DatabaseMysql();
		assert(db);
		if (!db->Initialize(vecDBInfos[i].c_str()))
		{
			delete db;
			return false;
		}
		m_vecDBs.push_back(db);
	}
	return true;
}

DatabaseMysql * CShardingDB::GetDBConnection(uint32 uShardingId)
{
	assert(m_nShardingDBNum > 0);
	int nDBLink = uShardingId % m_nShardingDBNum;
	//some bugs here~ for the size of vecDBs is not enough~ By YinQing
	
	return m_vecDBs[nDBLink];
}
void CShardingDB::Ping()
{
	for(size_t i = 0; i < m_vecDBs.size(); ++i)
	{
		m_vecDBs[i]->Ping();
	}
}
void CShardingDB::GetDBConnections(std::vector<DatabaseMysql *> &vecDBs)
{
	vecDBs.clear();
	for(size_t i = 0; i < m_vecDBs.size(); ++i)
	{
		vecDBs.push_back(m_vecDBs[i]);
	}
}





