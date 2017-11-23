/*
 * =====================================================================================
 *
 *       Filename:  MongoDBDrive.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  04/13/2016 02:49:49 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  imane (), imanecr@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#ifndef _MONGODBDRIVER_H_
#define _MONGODBDRIVER_H_
#include <bson.h>
#include <bcon.h>
#include <mongoc.h>
#include <vector>
#include <string>

/*
 * =====================================================================================
 *        Class:  CMongo
 *  Description:  used to handle mongodb operation
 * =====================================================================================
 */
class CMongo
{
	public:
		/* ====================  LIFECYCLE     ======================================= */
		CMongo ();                             /* constructor */
		~CMongo ();

		/* ====================  ACCESSORS     ======================================= */

		/* ====================  MUTATORS      ======================================= */

		/* ====================  OPERATORS     ======================================= */
		int Init (std::string connectStr, std::string colName);
		int Upsert(bson_t *query, bson_t *update);
		int Find(bson_t *query, std::vector<bson_t *> &vecResult);
		void ReleaseRes(std::vector<bson_t *> &vecResult);
	protected:
		/* ====================  METHODS       ======================================= */

		/* ====================  DATA MEMBERS  ======================================= */

	private:
		/* ====================  METHODS       ======================================= */

		/* ====================  DATA MEMBERS  ======================================= */
		mongoc_client_t	*m_client;
		mongoc_collection_t *m_collection;

}; /* -----  end of class CMongo  ----- */
#endif
