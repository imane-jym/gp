#ifndef _OBJECT_H_
#define _OBJECT_H_
#include <iostream>

// 单继承，所有对象的基类

class TObject;

struct TypeInfo
{
	char* className;
	int type_id;
	TypeInfo* pBaseClass;
	bool operator== (const TypeInfo& info){
		return this==&info;
	}
	bool operator!= (const TypeInfo& info){
		return this!=&info;
	}
};

inline std::ostream& operator<< (std::ostream& os,TypeInfo& info)
{
	return (os<< "[" << &info << "]" << "\t"
		<< info.type_id << ":"
		<< info.className << ":"
		<< info.pBaseClass << std::endl);
}

extern int TypeInfoOrder;

struct InitTypeInfo
{
	InitTypeInfo(/*TypeInfo* base,*/TypeInfo* info)
	{
		info->type_id=TypeInfoOrder++;
	}
};

#define TYPEINFO_OF_CLASS(class_name) (class_name::GetTypeInfoClass())
#define TYPEINFO_OF_OBJ(obj_name) (obj_name.GetTypeInfo())
#define TYPEINFO_OF_PTR(ptr_name) (ptr_name->GetTypeInfo())

#define DECLARE_TYPEINFO(class_name) \
public: \
	virtual int GetTypeID(){return TYPEINFO_MEMBER(class_name).type_id;} \
	virtual char* GetTypeName(){return TYPEINFO_MEMBER(class_name).className;} \
	virtual TypeInfo& GetTypeInfo(){return TYPEINFO_MEMBER(class_name);} \
	static TypeInfo& GetTypeInfoClass(){return TYPEINFO_MEMBER(class_name);} \
private: \
	static TypeInfo TYPEINFO_MEMBER(class_name); \
	static InitTypeInfo initClassInfo; 

#define IMPLEMENT_TYPEINFO(class_name,base_name) \
	TypeInfo class_name::TYPEINFO_MEMBER(class_name)= \
{(char *)#class_name,0,&(base_name::GetTypeInfoClass())}; \
	InitTypeInfo class_name::initClassInfo(&(class_name::TYPEINFO_MEMBER(class_name)));

#define DYNAMIC_CAST(object_ptr,class_name) \
	safe_cast<class_name>(object_ptr,TYPEINFO_OF_CLASS(class_name))

#define TYPEINFO_MEMBER(class_name) rttiTypeInfo

class TObject
{
public:
	bool IsKindOf(TypeInfo& cls);
	bool IsInstanceOf(TypeInfo& cls);
public:
	virtual int GetTypeID(){return TYPEINFO_MEMBER(TObject).type_id;}
	virtual char* GetTypeName(){return TYPEINFO_MEMBER(TObject).className;}
	virtual TypeInfo& GetTypeInfo(){return TYPEINFO_MEMBER(TObject);}
	static TypeInfo& GetTypeInfoClass(){return TYPEINFO_MEMBER(TObject);}
private:
	static TypeInfo TYPEINFO_MEMBER(TObject);
	static InitTypeInfo initClassInfo;
};

template <typename T>
	inline T* safe_cast(TObject* ptr,TypeInfo& cls)
{
	if (ptr == 0)
	{
		return 0;
	}

	return (ptr->IsKindOf(cls)?(T*)ptr:NULL);
}
#endif
