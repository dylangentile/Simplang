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

//	kBaseT_VOIDPTR
}BasicTypeID;

typedef enum 
{
	kType_Struct,
	kType_Ptr,
	kType_Basic,
	kType_Array,
	kType_Unknown
}TypeID;


class Type
{
public:
	Type(TypeID id);
	virtual ~Type() = 0;

	const TypeID mId;
};


class StructType : public Type
{
public:
	StructType(const std::string& name_);
	~StructType();
	std::string name;
	Structure* definition;
};

class BasicType : public Type
{
public:
	BasicType(BasicTypeID id) : Type(kType_Basic), basicId(id) {}
	~BasicType(){}
	BasicTypeID basicId;
};



class PointerType : public Type
{
public:
	PointerType(Type* pType, PointerID id_ = kPtr_Raw);
	~PointerType();
	PointerID ptrId;
	Type* pointsTo;

};


class ArrayType : public Type
{
public:
	ArrayType(Type* itemType, uint64_t size = 0);
	~ArrayType();

	Type* tType;
	uint64_t mSize;
}

class UnknownType : public Type
{
public:
	UnknownType() : Type(kType_Unknown) {}
	~UnknownType(){}

	Type* actualType = nullptr;


};



