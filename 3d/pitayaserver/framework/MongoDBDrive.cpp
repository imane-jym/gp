/*
 * =====================================================================================
 *
 *       Filename:  MongoDBDrive.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  04/13/2016 02:50:21 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  imane (), imanecr@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include "MongoDBDrive.h"
#include "Log.h"

static void log_handler (mongoc_log_level_t  log_level,const char* log_domain, const char* message, void* user_data)
{
	if (log_level >= MONGOC_LOG_LEVEL_CRITICAL)
	{
		IME_ERROR("mongodb %s\n", message);
	}
	else if (log_level >= MONGOC_LOG_LEVEL_WARNING)
	{
		IME_WARN("mongodb %s\n", message);
	}
	else
	{
		IME_LOG("mongodb %s\n", message);
	}
}

CMongo::CMongo ()
{
	m_client = NULL;
	m_collection = NULL;
}		/* -----  end of method CMongo::CMongo  ----- */


CMongo::~CMongo ()
{
	mongoc_collection_destroy(m_collection);
	mongoc_client_destroy(m_client);
	mongoc_cleanup();
	return ;
}		/* -----  end of method CMongo::~CMongo  ----- */


int
CMongo::Init (std::string connectStr, std::string colName)
{
	std::string dbName;
	bson_t *command, reply;
	bson_error_t error;
	bool retval;
	if (m_client != NULL)
	{
		IME_ERROR("must not allow init twice");
		return -1;
	}
	mongoc_init ();
	m_client = mongoc_client_new (connectStr.c_str());
	if (!m_client)
	{
		IME_ERROR("failed to parse url %s", connectStr.c_str());
		return -1;
	}
	// get default name, it must not set dataname in connectStr
	mongoc_database_t *db = mongoc_client_get_default_database (m_client);
	if (db == NULL)
	{
		IME_ERROR("mongo can not set databasename");
		return -1;
	}
	dbName = mongoc_database_get_name(db);

//	m_database = mongoc_client_get_database (client, dbName);
	m_collection = mongoc_client_get_collection (m_client, dbName.c_str(), colName.c_str());

	command = BCON_NEW("ping", BCON_INT32(1));
	retval = mongoc_client_command_simple(m_client, dbName.c_str(), command, NULL, &reply, &error);
	if (!retval)
	{	
		IME_ERROR("mongo error msg %s", error.message);
		return -1;
	}
	char *str = bson_as_json(&reply, NULL);
	IME_LOG("mongo str %s", str);
	
	mongoc_log_set_handler (log_handler, NULL);

	bson_destroy(&reply);
	bson_destroy(command);
	bson_free(str);
	return 0;
}		/* -----  end of method CMongo::Init  ----- */


int
CMongo::Upsert (bson_t *query, bson_t *update)
{
	bson_error_t error;
	if (!mongoc_collection_update (m_collection, MONGOC_UPDATE_UPSERT, query, update, NULL, &error)) {
		IME_ERROR ("mongodb upsert fail msg %s\n", error.message);
		return -1;
	}
	return 0;
}		/* -----  end of method CMongo::Update  ----- */

int
CMongo::Find (bson_t *query, std::vector<bson_t *> &vecResult)
{
	mongoc_cursor_t *cursor;
	bson_error_t error;
	cursor = mongoc_collection_find (m_collection, MONGOC_QUERY_NONE, 0, 0, 0, query, NULL, NULL);

	const bson_t *doc;
	while (mongoc_cursor_next (cursor, &doc)) {
//		str = bson_as_json (doc, NULL);
//		printf ("%s\n", str);
//		bson_free (str);
		bson_t *ret = bson_copy(doc);
		vecResult.push_back(ret);
	}
	if (mongoc_cursor_error (cursor, &error)) {
		IME_ERROR("mongodb An error occurred: %s\n", error.message);
		return -1;
	}
	mongoc_cursor_destroy (cursor);
	return 0;
}		/* -----  end of method CMongo::Find  ----- */

void
CMongo::ReleaseRes (std::vector<bson_t *> &vecResult)
{
	for (unsigned int i = 0; i < vecResult.size(); i++)
	{
		bson_destroy(vecResult[i]);
	}
	return ;
}		/* -----  end of method CMongo::ReleaseRes  ----- */

