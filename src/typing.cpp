#include "statement.h"
#include "typing.h"


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