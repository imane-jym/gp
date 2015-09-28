#include "object.h"

int TypeInfoOrder = 0;

TypeInfo TObject::TYPEINFO_MEMBER(TObject)={(char *)"TObject",0,NULL};
InitTypeInfo TObject::initClassInfo(&(TObject::TYPEINFO_MEMBER(TObject)));

bool TObject::IsKindOf(TypeInfo& cls)
{
	TypeInfo* p=&(this->GetTypeInfo());
	while(p!=NULL)
	{
		if(p->type_id==cls.type_id)
		{
			return true;
		}
		p=p->pBaseClass;
	}
	return false;
}

bool TObject::IsInstanceOf( TypeInfo& cls )
{
	TypeInfo* p=&(this->GetTypeInfo());
	
	if(p->type_id==cls.type_id)
	{
		return true;
	}
	
	return false;
}
