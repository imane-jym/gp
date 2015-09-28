#include "QueryResultMysql.h"
#include <assert.h>

QueryResultMysql::QueryResultMysql(MYSQL_RES *result, uint64 rowCount, uint32 fieldCount) :
QueryResult(rowCount, fieldCount), mResult(result)
{

    mCurrentRow = new Field[mFieldCount];
    assert(mCurrentRow);

//	  mysql-5.0.22 version mysql_fetch_fields get data error, maybe mysql static lib have changed
//    MYSQL_FIELD *fields = mysql_fetch_fields(mResult);
//
//    for (uint32 i = 0; i < mFieldCount; i++)
//    {
//        mCurrentRow[i].SetName(fields[i].name);
//        mCurrentRow[i].SetType(ConvertNativeType(fields[i].type));
//
//    }

    MYSQL_FIELD *field = NULL;

    for (uint32 i = 0; i < mFieldCount; i++)
    {
    	field = mysql_fetch_field(mResult);
        mCurrentRow[i].SetName(field->name);
        mCurrentRow[i].SetType(ConvertNativeType(field->type));
    }
}

QueryResultMysql::~QueryResultMysql()
{
    EndQuery();
}

bool QueryResultMysql::NextRow()
{
    MYSQL_ROW row;

    if (!mResult)
        return false;

    row = mysql_fetch_row(mResult);
    if (!row)
    {
        EndQuery();
        return false;
    }
    long unsigned int * fieldLength = mysql_fetch_lengths(mResult);
    for (uint32 i = 0; i < mFieldCount; i++)
    {
        mCurrentRow[i].SetValue(row[i],fieldLength[i]);
        mCurrentRow[i].SetLength(fieldLength[i]);
    }

    return true;
}

void QueryResultMysql::EndQuery()
{
    if (mCurrentRow)
    {
        delete [] mCurrentRow;
        mCurrentRow = 0;
    }

    if (mResult)
    {
        mysql_free_result(mResult);
        mResult = 0;
    }
}

enum Field::DataTypes QueryResultMysql::ConvertNativeType(enum_field_types mysqlType) const
{
    switch (mysqlType)
    {
    	case FIELD_TYPE_BLOB:
    		return Field::DB_TYPE_BLOB;
        case FIELD_TYPE_TIMESTAMP:
        case FIELD_TYPE_DATE:
        case FIELD_TYPE_TIME:
        case FIELD_TYPE_DATETIME:
        case FIELD_TYPE_YEAR:
        case FIELD_TYPE_STRING:
        case FIELD_TYPE_VAR_STRING:
        case FIELD_TYPE_SET:
        case FIELD_TYPE_NULL:
            return Field::DB_TYPE_STRING;
        case FIELD_TYPE_TINY:

        case FIELD_TYPE_SHORT:
        case FIELD_TYPE_LONG:
        case FIELD_TYPE_INT24:
        case FIELD_TYPE_LONGLONG:
        case FIELD_TYPE_ENUM:
            return Field::DB_TYPE_INTEGER;
        case FIELD_TYPE_DECIMAL:
        case FIELD_TYPE_FLOAT:
        case FIELD_TYPE_DOUBLE:
            return Field::DB_TYPE_FLOAT;
        default:
            return Field::DB_TYPE_UNKNOWN;
    }
}
