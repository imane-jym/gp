#ifndef WORLD_DB_H
#define WORLD_DB_H

/*
 *  WorldDb.h
 *
 *  Created on: 2011-12-15
 *      Author: hx
 */

#include <string>
#include <vector>
#include "Defines.h"


class DatabaseMysql;

class CWorldDb
{
public:
	CWorldDb();
	virtual ~CWorldDb();
	bool Init(int nShardingDBNum, const std::vector<std::string> & vecDBInfos);
	DatabaseMysql * GetDBConnection(uint32 dwShardingId);
	void Ping();

	uint32 GetDBIdx(uint32 dwAccountId);

	uint32 GetTable10Idx(uint32 dwAccountId);
	uint32 GetTable100Idx(uint32 dwAccountId);

private:
	int m_nShardingDBNum;
	int m_nDbNumPerMysql;
	std::vector<DatabaseMysql *> m_vecDbMysql;
};
#endif
