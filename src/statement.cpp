#include "statement.h"


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


Function::Function() : Statement(kState_Function)
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




















