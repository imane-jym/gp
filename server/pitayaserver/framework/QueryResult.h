#ifndef _QUERY_RESULT_H
#define _QUERY_RESULT_H


#include "Defines.h"
#include "Field.h"

class QueryResult
{
    public:
        QueryResult(uint64 rowCount, uint32 fieldCount)
            : mFieldCount(fieldCount), mRowCount(rowCount) {}

        virtual ~QueryResult() {}

        virtual bool NextRow() = 0;

        Field *Fetch() const { return mCurrentRow; }

        const Field & operator [] (int index) const { return mCurrentRow[index]; }

        uint32 GetFieldCount() const { return mFieldCount; }

        uint64 GetRowCount() const { return mRowCount; }

    protected:
        Field *mCurrentRow;
        uint32 mFieldCount;
        uint64 mRowCount;
};
#endif
