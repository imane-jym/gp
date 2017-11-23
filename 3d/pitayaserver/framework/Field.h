#ifndef _FIELD_H
#define _FIELD_H

#include "Defines.h"
#include <string>
#include <stdio.h>
#include <stdlib.h>

class Field
{
    public:

        enum DataTypes
        {
            DB_TYPE_UNKNOWN = 0x00,
            DB_TYPE_STRING  = 0x01,
            DB_TYPE_INTEGER = 0x02,
            DB_TYPE_FLOAT   = 0x03,
            DB_TYPE_BLOB	= 0x04,
        };

        Field();

        ~Field();

        const char *GetName() const { return mName; }
        enum DataTypes GetType() const { return mType; }
        uint32 GetLength() const { return m_dwLength; };
        const char *GetString() const { return mValue; }
        std::string GetCppString() const
        {
            return mValue ? mValue : "";                    // std::string s = 0 have undefine result in C++
        }
		std::string GetTextString() const          
		{                                          
			std::string text;                      
			text.append( mValue, m_dwLength );     

			return text;                           
		}                                          
        float GetFloat() const { return mValue ? static_cast<float>(atof(mValue)) : 0; }
        bool GetBool() const { return mValue ? atoi(mValue) > 0 : false; }
        int32 GetInt32() const { return mValue ? static_cast<int32>(atol(mValue)) : 0; }
        uint8 GetUInt8() const { return mValue ? static_cast<uint8>(atol(mValue)) : 0; }
        uint16 GetUInt16() const { return mValue ? static_cast<uint16>(atol(mValue)) : 0; }
        int16 GetInt16() const { return mValue ? static_cast<int16>(atol(mValue)) : 0; }
        uint32 GetUInt32() const { return mValue ? static_cast<uint32>(atol(mValue)) : 0; }
        uint64 GetUInt64() const
        {
            if(mValue)
            {
                uint64 value;
                //sscanf(mValue,I64FMTD,&value);
				//todo
				sscanf(mValue,"%llu",(unsigned long long *)&value);
                return value;
            }
            else
                return 0;
        }

        void SetName(const char *name);
        void SetType(enum DataTypes type) { mType = type; }

        void SetValue(const char *value, uint32 dwLength);
        void SetLength(uint32 dwLength) {m_dwLength = dwLength;};

    private:
        char *mValue;
        char *mName;
        enum DataTypes mType;
        uint32 m_dwLength;
        ////下面两个函数暂时不给使用
        Field(Field &f);
        Field(const char *value, const char *name, enum DataTypes type);

};
#endif
