#include "statement.h"
#include "typing.h"

Type::Type(TypeID id) : mId(id)
{

}

Type::~Type()
{
	
}

StructType::StructType(const std::string& name_) : Type(kType_Struct), name(name_), definition(nullptr)
{

}

StructType::~StructType()
{
	if(definition != nullptr)
		delete definition;
}


PointerType::PointerType(Type* pType, PointerID id_) : Type(kType_Ptr), ptrId(id_), pointsTo(pType)
{

}


PointerType::~PointerType()
{

}

ArrayType::ArrayType(Type* itemType, uint64_t size) : Type(kType_Array), tType(itemType), mSize(size)
{
	
}