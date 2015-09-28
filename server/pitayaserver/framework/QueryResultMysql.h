#ifndef _QUERYRESULT_MYSQL_H
#define _QUERYRESULT_MYSQL_H

#ifdef WIN32
#define FD_SETSIZE 1024
#include <winsock2.h>
#include <mysql/mysql.h>
#else
#include <mysql/mysql.h>
#endif
#include "Defines.h"
#include "Field.h"
#include "QueryResult.h"

class QueryResultMysql : public QueryResult
{
    public:
        QueryResultMysql(MYSQL_RES *result, uint64 rowCount, uint32 fieldCount);

        ~QueryResultMysql();

        bool NextRow();

    private:
        enum Field::DataTypes ConvertNativeType(enum_field_types mysqlType) const;
        void EndQuery();

        MYSQL_RES *mResult;
};

#endif
