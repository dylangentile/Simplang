#include "statement.h"
#include "error.h"

Statement::Statement(StatementID id) : mId(id)
{

}

Statement::~Statement()
{

}


Scope::Scope() : Statement(kState_Scope)
{

}

Scope::~Scope()
{
	for(auto it = statementVec.begin(); it != statementVec.end(); it++)
		delete *it;
}

bool
Scope::usedSymbol(const std::string& name)
{
	return structMap[name] != nullptr || functionMap[name] != nullptr || enumMap[name] != nullptr;
}

StructType* 
Scope::insertStruct(Token* tok)
{
	if(tok->mCat != kCat_WildIdentifier)
	{
		lerror(kE_Error, tok, "Invalid struct identifier symbol!");
		return nullptr;
	}

	if(usedSymbol(tok->mStr))
	{
		lerror(kE_Error, tok, "Symbol is already used in scope!");
		return nullptr;
	}
	StructType* theStruct = new StructType(tok->mStr);
	structMap.insert(theStruct, tok->mStr);
	return theStruct;


}

Variable::Variable(const std::string& name, Type* type) 
	: Statement(kState_Variable), mName(name), mType(type), mInitializer(nullptr)
{

}

Variable::~Variable()
{

}

VariableAssignment::VariableAssignment() : Statement(kState_VariableAssignment)
{

}

VariableAssignment::~VariableAssignment()
{

}

Structure::Structure() : Statement(kState_Structure)
{
	for(auto it = members.begin(); it != members.end(); it++)
		delete *it;
}

Structure::~Structure()
{

}


Function::Function() : Statement(kState_Function), mBody(nullptr)
{

}

Function::~Function()
{

}

/*
Expression::Expression(StatementID id) : Statement(id)
{

}

Expression::~Expression()
{

}*/

BinOp::BinOp() : Statement(kState_BinOpExpr)
{

}

BinOp::~BinOp()
{

}


FunctionCall::FunctionCall() : Statement(kState_FunctionCall)
{

}

FunctionCall::~FunctionCall()
{

}


Immediate::Immediate() : Statement(kState_ImmediateExpr), str(nullptr)
{

}

Immediate::~Immediate()
{

}




















