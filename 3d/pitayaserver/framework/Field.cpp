#include "Field.h"
#include <string.h>

Field::Field() :
mValue(NULL), mName(NULL), mType(DB_TYPE_UNKNOWN), m_dwLength(0)
{
}

Field::Field(Field &f)
{
    const char *value, *name;

    value = f.GetString();
    name = f.GetName();

    if (value && (mValue = new char[strlen(value) + 1]))
        strcpy(mValue, value);
    else
        mValue = NULL;

    if (name && (mName = new char[strlen(name) + 1]))
        strcpy(mName, name);
    else
        mName = NULL;

    mType = f.GetType();
    m_dwLength = f.GetLength();
}

Field::Field(const char *value, const char *name, enum Field::DataTypes type) :
mType(type)
{
    if (value && (mValue = new char[strlen(value) + 1]))
        strcpy(mValue, value);
    else
        mValue = NULL;

    if (name && (mName = new char[strlen(name) + 1]))
        strcpy(mName, name);
    else
        mName = NULL;
}

Field::~Field()
{
    delete [] mValue;
    delete [] mName;
}

void Field::SetValue(const char *value, uint32 dwLength)
{
    delete [] mValue;

    if (value)
    {
    	if (GetType() == DB_TYPE_BLOB)
    	{
    		mValue = new char[dwLength];
    		memcpy(mValue,value,dwLength);
    	}
    	else
    	{
			mValue = new char[strlen(value) + 1];
			strcpy(mValue, value);
    	}
    }
    else
        mValue = NULL;
}

void Field::SetName(const char *name)
{
    delete [] mName;

    if (name)
    {
        mName = new char[strlen(name) + 1];
        strcpy(mName, name);
    }
    else
        mName = NULL;
}
