#ifndef _SHARDING_DB_H
#define _SHARDING_DB_H

#include <string>
#include <vector>
#include "Defines.h"


class DatabaseMysql;

class CShardingDB
{
public:
	CShardingDB();
	virtual ~CShardingDB();
	bool Init(int nShardingDBNum, const std::vector<std::string> & vecDBInfos);
	DatabaseMysql * GetDBConnection(uint32 uShardingId);
	void Ping();

	void GetDBConnections(std::vector<DatabaseMysql *> &vecDBs);

	inline uint32 GetDBIdx(uint32 dwAccountId)
	{
		return 0;
		//return dwAccountId % 100;
	}
	inline uint32 GetTable10Idx(uint32 dwAccountId)
	{
		return (dwAccountId / 100) % 10;
	}
	inline uint32 GetTable100Idx(uint32 dwAccountId)
	{
		return (dwAccountId / 100) % 100;
	}
private:
	int m_nShardingDBNum;
	std::vector<DatabaseMysql *> m_vecDBs;
};
#endif
