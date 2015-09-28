#include "AnyType.h"

using std::string;
using std::vector;
using std::map;

typedef vector<AnyType> vectorA;
typedef map<unsigned char,AnyType> map1A;
typedef map<AnyType,AnyType> mapAA;

void* copyObject(eAnyType t,const void* p) {
	if(!p) return 0;
	switch(t) {
		case eAnyTypeString:	return new string(*(const string*)p);
		case eAnyTypeVector:	return new vectorA(*(const vectorA*)p);
		case eAnyTypeMap1A:		return new map1A(*(const map1A*)p);
		case eAnyTypeMapAA:		return new mapAA(*(const mapAA*)p);
		default:
			break;
	}
	return 0;
}

void deleteObject(eAnyType t,void* p) {
	if(!p) return;
	switch(t) {
		case eAnyTypeString:	delete (string*)p;	break;
		case eAnyTypeVector:	delete (vectorA*)p;	break;
		case eAnyTypeMap1A:		delete (map1A*)p;	break;
		case eAnyTypeMapAA:		delete (mapAA*)p;	break;
		default:
			break;
	}
}

bool isObjectEqual(eAnyType t,const void* p1,const void* p2) {
	if(p1==p2) return true;
	if((!p1)||(!p2)) return false;
	switch(t) {
		case eAnyTypeString:	return (*(string*)p1)==(*(string*)p2);
		case eAnyTypeVector:	return (*(vectorA*)p1)==(*(vectorA*)p2);
		case eAnyTypeMap1A:		return (*(map1A*)p1)==(*(map1A*)p2);
		case eAnyTypeMapAA:		return (*(mapAA*)p1)==(*(mapAA*)p2);
		default:
			break;
	}
	return false;
}
