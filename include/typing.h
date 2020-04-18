#pragma once
#include <string>

class Structure;

typedef enum
{
	kPtr_Shared,
	kPtr_Unique,
	kPtr_Raw
}PointerID;

typedef enum
{
	kBaseT_VOID,
	kBaseT_STRING,
	kBaseT_BYTE,
	kBaseT_BOOL,

	kBaseT_INT8,
	kBaseT_INT16,
	kBaseT_INT32,
	kBaseT_INT64,

	kBaseT_UINT8,
	kBaseT_UINT16,
	kBaseT_UINT32,
	kBaseT_UINT64,

	kBaseT_SINGLE,
	kBaseT_DOUBLE,

	kBaseT_DYNAMIC,
	kBaseT_TYPENAME
}BasicTypeID;

typedef enum 
{
	kType_Struct,
	kType_Ptr,
	kType_Basic
}TypeID;


class Type
{
public:
	Type(TypeID id) : mId(id) {}
	~Type(){}

	const TypeID mId;
};


class StructType : public Type
{
public:
	StructType(Structure* def) : 
		Type(kType_Struct), definition(def) {}
	~StructType(){}
	std::string name;
	Structure* definition;
};

class BasicType : public Type
{
public:
	BasicType() : Type(kType_Basic) {}
	~BasicType(){}
	BasicTypeID basicId;
};



class PointerType : public Type
{
public:
	PointerType() : Type(kType_Ptr) {}
	~PointerType(){}
	PointerID ptrId;
	Type* pointsTo;

};