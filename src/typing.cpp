#include "statement.h"
#include "typing.h"


StructType::StructType(const std::string& name_, bool defined) : Type(kType_Struct), name(name_)
{
	if(defined)
		definition = new Structure;
	else
		definition = nullptr;
}

StructType::~StructType()
{
	delete definition;
}