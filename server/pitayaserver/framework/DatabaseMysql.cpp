#include "DatabaseMysql.h"
#include "QueryResultMysql.h"
#include "Util.h"
#include "Log.h"
#include <stdio.h>
#include <vector>
#include <stdlib.h>
#include <string>
#include <stdarg.h>

void DatabaseMysql::ThreadStart()
{
    //mysql_thread_init();
}

void DatabaseMysql::ThreadEnd()
{
    //mysql_thread_end();
}

size_t DatabaseMysql::db_count = 0;

DatabaseMysql::DatabaseMysql() : mMysql(NULL)
{
    // before first connection
    if( db_count++ == 0 )
    {
        // Mysql Library Init
        mysql_library_init(-1, NULL, NULL);

		/*todo thread safe MySql library need compile manual!!! We should use database only in World thread~ By YinQing
        if (!mysql_thread_safe())
        {
            printf("FATAL ERROR: Used MySQL library isn't thread-safe.");
            //todo it's not a good idea do this in constructor. By YinQing
			//exit(1);
        }
		*/
    }
}

DatabaseMysql::~DatabaseMysql()
{
    if (mMysql)
        mysql_close(mMysql);

    //Free Mysql library pointers for last ~DB
    if(--db_count == 0)
        mysql_library_end();
}
void DatabaseMysql::ReConnect()
{
    if (mMysql)
        mysql_close(mMysql);
    Initialize(m_strInfoString.c_str());
}
bool DatabaseMysql::Initialize(const char *infoString)
{
	m_strInfoString = infoString;
    MYSQL *mysqlInit;
    mysqlInit = mysql_init(NULL);
    if (!mysqlInit)
    {
        IME_SQLERROR("Could not initialize Mysql connection");
        return false;
    }

	std::vector<std::string> vecData;
	CUtil::StrSplit(infoString, "'", vecData);

    std::string host, port_or_socket, user, password, database;
    int port;
    char const* unix_socket;

	if (vecData.size() != 5 && vecData.size() != 4)
	{
		IME_SQLERROR("VEC DATA SIZE ERROR[%d]", (int)vecData.size());
		return false;
	}
		
    host = vecData[0];
    port_or_socket = vecData[1];
    user = vecData[2];
    password = vecData[3];
	if (vecData.size() == 5)
	{
		database = vecData[4];
	}
	else
	{
		database = "";
	}

	m_strDbName = database;

	port = atoi(port_or_socket.c_str());
    mysql_options(mysqlInit,MYSQL_SET_CHARSET_NAME,"utf8");
    char value = 1;
    mysql_options(mysqlInit, MYSQL_OPT_RECONNECT, (char *)&value);

#ifdef WIN32
    if(host==".")                                           // named pipe use option (Windows)
    {
        unsigned int opt = MYSQL_PROTOCOL_PIPE;
        mysql_options(mysqlInit,MYSQL_OPT_PROTOCOL,(char const*)&opt);
        port = 0;
        unix_socket = 0;
    }
    else                                                    // generic case
    {
        port = atoi(port_or_socket.c_str());
        unix_socket = 0;
    }
#else
    if(host==".")                                           // socket use option (Unix/Linux)
    {
        unsigned int opt = MYSQL_PROTOCOL_SOCKET;
        mysql_options(mysqlInit,MYSQL_OPT_PROTOCOL,(char const*)&opt);
        host = "localhost";
        port = 0;
        unix_socket = port_or_socket.c_str();
    }
    else                                                    // generic case
    {
        port = atoi(port_or_socket.c_str());
        unix_socket = 0;
    }
#endif

    mMysql = mysql_real_connect(mysqlInit, host.c_str(), user.c_str(),
        password.c_str(), database.c_str(), port, unix_socket, CLIENT_FOUND_ROWS);

    if (mMysql)
    {
        IME_SQLDRIVER("Connected to MySQL database at %s. Database : %s",
            host.c_str(),database.c_str());
        IME_SQLDRIVER("MySQL client library: %s", mysql_get_client_info());
        IME_SQLDRIVER("MySQL server ver: %s", mysql_get_server_info( mMysql));
		//todo ???
        /*----------SET AUTOCOMMIT ON---------*/
        // It seems mysql 5.0.x have enabled this feature
        // by default. In crash case you can lose data!!!
        // So better to turn this off
        // ---
        // This is wrong since mangos use transactions,
        // autocommit is turned of during it.
        // Setting it to on makes atomic updates work
        if (!mysql_autocommit(mMysql, 1))
        	IME_SQLDRIVER("AUTOCOMMIT SUCCESSFULLY SET TO 1");
        else
        	IME_SQLDRIVER("AUTOCOMMIT NOT SET TO 1");
        /*-------------------------------------*/
        return true;
    }
    else
    {
    	IME_SQLERROR( "Could not connect to MySQL database at %s: %s\n",
            host.c_str(),mysql_error(mysqlInit));
        mysql_close(mysqlInit);
        return false;
    }
}

QueryResult* DatabaseMysql::Query(const char *sql)
{
    if (!mMysql)
        return 0;

    MYSQL_RES *result = 0;
    uint64 rowCount = 0;
    uint32 fieldCount = 0;

    {
        // guarded block for thread-safe mySQL request
        //ZThread::Guard<ZThread::FastMutex> query_connection_guard(mMutex);
		//I dont want do make this thread safe yet~I mean no need now. By YinQing
#ifdef MANGOS_DEBUG
        uint32 _s = getMSTime();
#endif
        if(mysql_query(mMysql, sql))
        {
        	IME_SQLERROR("SQL: %s", sql);
        	IME_SQLERROR("query ERROR: %s", mysql_error(mMysql));
            return NULL;
        }
        else
        {
#ifdef MANGOS_DEBUG
        	IME_SQLERROR("[%u ms] SQL: %s", getMSTime() - _s, sql );
#endif
        }

        result = mysql_store_result(mMysql);

        rowCount = mysql_affected_rows(mMysql);
        fieldCount = mysql_field_count(mMysql);
        // end guarded block
    }

    if (!result )
        return NULL;

    if (!rowCount)
    {
        mysql_free_result(result);
        return NULL;
    }

    QueryResultMysql *queryResult = new QueryResultMysql(result, rowCount, fieldCount);

    queryResult->NextRow();

    return queryResult;
}

QueryResult* DatabaseMysql::QueryStrict(const char *sql)
{
    if (!mMysql)
        return 0;

    MYSQL_RES *result = 0;
    uint64 rowCount = 0;
    uint32 fieldCount = 0;

    {
        // guarded block for thread-safe mySQL request
        //ZThread::Guard<ZThread::FastMutex> query_connection_guard(mMutex);
		//I dont want do make this thread safe yet~I mean no need now. By YinQing
#ifdef MANGOS_DEBUG
        uint32 _s = getMSTime();
#endif
        if(mysql_query(mMysql, sql))
        {
        	IME_SQLERROR("SQL: %s", sql);
        	IME_SQLERROR("query ERROR: %s", mysql_error(mMysql));
            return NULL;
        }
        else
        {
#ifdef MANGOS_DEBUG
        	IME_SQLERROR("[%u ms] SQL: %s", getMSTime() - _s, sql );
#endif
        }

        result = mysql_store_result(mMysql);

        rowCount = mysql_affected_rows(mMysql);
        fieldCount = mysql_field_count(mMysql);
        // end guarded block
    }

    if (!result )
        return NULL;

    QueryResultMysql *queryResult = new QueryResultMysql(result, rowCount, fieldCount);

    queryResult->NextRow();

    return queryResult;
}

bool DatabaseMysql::DirectExecute(const char* sql)
{
    if (!mMysql)
        return false;

    {
        // guarded block for thread-safe mySQL request
        //ZThread::Guard<ZThread::FastMutex> query_connection_guard(mMutex);
#ifdef MANGOS_DEBUG
        uint32 _s = getMSTime();
#endif
        if(mysql_query(mMysql, sql))
        {
        	IME_SQLERROR("SQL: %s", sql);
        	IME_SQLERROR("SQL ERROR: %s", mysql_error(mMysql));
            return false;
        }
        else
        {
#ifdef MANGOS_DEBUG
        	IME_SQLDRIVER("[%u ms] SQL: %s", getMSTime() - _s, sql );
#endif
        }
		// ----- change by huangxi
		if (mysql_affected_rows(mMysql) == 0)
		{
			return false;
		}
		
        // end guarded block
    }

    return true;
}

unsigned long DatabaseMysql::escape_string(char *to, const char *from, unsigned long length)
{
    if (!mMysql || !to || !from || !length)
        return 0;

    return(mysql_real_escape_string(mMysql, to, from, length));
}
void DatabaseMysql::escape_string(std::string& str)
{
    if(str.empty())
        return;
	//todo here is not fast! try to avoid use new! By YinQing
    char* buf = new char[str.size()*2+1];
	//char szBuf[str.size() * 2 + 1];
    escape_string(buf,str.c_str(),str.size());
    str = buf;
    delete[] buf;
}
QueryResult * DatabaseMysql::PQuery(const char * format,...)
{

    if(!format) return NULL; 
	va_list ap; 
	char szQuery [MAX_QUERY_LEN];
	va_start(ap, format);  
	int res = vsnprintf( szQuery, MAX_QUERY_LEN, format, ap );  
	va_end(ap);  
	if(res == -1) 
	{  
		IME_SQLERROR("SQL Query truncated (and not execute) for format: %s",format);
		return false; 
	}  
	return Query(szQuery);
}

QueryResult * DatabaseMysql::PQueryStrict(const char * format,...)
{

    if(!format) return NULL; 
	va_list ap; 
	char szQuery [MAX_QUERY_LEN];
	va_start(ap, format);  
	int res = vsnprintf( szQuery, MAX_QUERY_LEN, format, ap );  
	va_end(ap);  
	if(res == -1) 
	{  
		IME_SQLERROR("SQL Query truncated (and not execute) for format: %s",format);
		return false; 
	}  
	return QueryStrict(szQuery);
}
bool DatabaseMysql::PExecute(const char * format,...)
{
    if (!format)
		return false; 
	va_list ap;
	char szQuery [MAX_QUERY_LEN]; 
	va_start(ap, format); 
	int res = vsnprintf( szQuery, MAX_QUERY_LEN, format, ap );  
	va_end(ap);  
	if(res == -1)
	{  
		IME_SQLERROR("SQL Query truncated (and not execute) for format: %s",format);
		return false;  
	}  
	
	return DirectExecute(szQuery);
}

void DatabaseMysql::StartTransaction()
{
	DirectExecute("START TRANSACTION");
}

void DatabaseMysql::EndCommit()
{
	DirectExecute("COMMIT");
}

void DatabaseMysql::EndRollback()
{
	DirectExecute("ROLLBACK");
}
