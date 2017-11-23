

#ifndef _DATABASEMYSQL_H
#define _DATABASEMYSQL_H


//#include "zthread/FastMutex.h"

#ifdef WIN32
#define FD_SETSIZE 1024
#include <winsock2.h>
#include <mysql/mysql.h>
#else
#include <mysql/mysql.h>
#endif

#include <string>
#include "Defines.h"

//todo not enough!?
// not enough when set/get long blob
// default stack limit = 8M
//#define MAX_QUERY_LEN 1024 * 64
#define MAX_QUERY_LEN 2 * 1024 * 1024

class QueryResult;

class DatabaseMysql
{
    
    public:
        DatabaseMysql();
        ~DatabaseMysql();

        //! Initializes Mysql and connects to a server.
        /*! infoString should be formated like hostname;username;password;database. */
        bool Initialize(const char *infoString);

		QueryResult* PQuery(const char * format,...);
        QueryResult* Query(const char *sql);
		
		//jym add it used to distinguish none data and error
		QueryResult* PQueryStrict(const char * format,...);
        QueryResult* QueryStrict(const char *sql);
		//jym add end
		
		bool PExecute(const char * format,...);
        bool Execute(const char *sql);
        bool DirectExecute(const char* sql);

        operator bool () const { return mMysql != NULL; }

        unsigned long escape_string(char *to, const char *from, unsigned long length);
        void escape_string(std::string& str);

        // must be call before first query in thread
        void ThreadStart();
        // must be call before finish thread run
        void ThreadEnd();
		
		void Ping() { mysql_ping(mMysql); }

		void ReConnect();

		// autocommit is false;
		void StartTransaction();
		void EndCommit();
		void EndRollback();

		const char* GetDbName() { return m_strDbName.c_str(); }

    private:
        //ZThread::FastMutex mMutex;

        MYSQL *mMysql;
        std::string m_strInfoString;
        std::string m_strDbName;
        static size_t db_count;

};
#endif

