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
	for(Statement* state : statementVec)
		delete state;
	
}

bool
Scope::usedSymbol(const std::string& name)
{
	return structMap[name] != nullptr || functionMap[name] != nullptr || enumMap[name] != nullptr;
}

Function* 			
Scope::insertFunction(Token* tok)
{
	if(tok->mCat != kCat_WildIdentifier)
		lerror(kE_Fatal, tok, "Invalid function identifier symbol!");

	if(usedSymbol(tok->mStr))
		lerror(kE_Fatal, tok, "Function redeclaration!");

	Function* func = new Function();
	func->mName = tok->mStr;
	functionMap.insert(func, tok->mStr);
	return func;

}


StructType* 
Scope::insertStruct(Token* tok)
{
	if(tok->mCat != kCat_WildIdentifier)
		lerror(kE_Fatal, tok, "Invalid struct identifier symbol!");
		
	if(usedSymbol(tok->mStr))
		lerror(kE_Fatal, tok, "Struct redeclaration!");


	StructType* theStruct = new StructType(tok->mStr);
	structMap.insert(theStruct, tok->mStr);
	return theStruct;


}

Variable*
Scope::insertVariable(Token* tok, Type* type)
{
	if(tok->mCat != kCat_WildIdentifier)
		lerror(kE_Fatal, tok, "symbol cannot be used for variable name!");

	Variable* var = new Variable(tok->mStr, type);
	statementVec.push_back(dynamic_cast<Statement*>(var));
	return var;
}


DeclEqual*
Scope::insertDeclEqual()
{
	DeclEqual* de = new DeclEqual();
	statementVec.push_back(dynamic_cast<Statement*>(de));
	return de;
}

MultipleAssignment*
Scope::insertMultipleAssignment()
{
	MultipleAssignment* ma = new MultipleAssignment();
	statementVec.push_back(dynamic_cast<Statement*>(ma));
	return ma;
}

Return*
Scope::insertReturn()
{
	Return* ret = new Return();
	statementVec.push_back(dynamic_cast<Return*>(ret));
	return ret;
}

Type* 
Scope::getType(const std::string& name)
{
	//the enumMap and structMap cannot contain duplicate symbols 
	//	since the insertStruct/Enum functions take care of that for us

	StructType** st = nullptr;
	UnknownType** ut = nullptr;
	/*Enum*/Type** et = nullptr;


	if((st = structMap[name]) != nullptr) 
		return dynamic_cast<Type*>(*st);
	if((ut = unknownTypeMap[name]) != nullptr) 
		return dynamic_cast<Type*>(*ut);
	if((et = enumMap[name]) != nullptr) 
		return dynamic_cast<Type*>(*et);
	
	return nullptr;

}

Variable::Variable(const std::string& name, Type* type) 
	: Statement(kState_Variable), mName(name), mType(type), mInitializer(nullptr)
{

}

Variable::~Variable()
{
	delete mInitializer;
}


DeclEqual::DeclEqual() : Statement(kState_DeclEqual), mInitializer(nullptr)
{

}

DeclEqual::~DeclEqual()
{
	for(Variable* var : varVec)
		delete var;
	delete mInitializer;
	
}

Variable*
DeclEqual::insert(const std::string& name)
{
	Variable* var = new Variable(name, nullptr);
	varVec.push_back(var);
	return var;
}


MultipleAssignment::MultipleAssignment() : Statement(kState_MultipleAssignment), expr(nullptr)
{

}

MultipleAssignment::~MultipleAssignment()
{
	//don't delete varible refs, they're deleted elsewhere (they're refrences)
	delete expr;
}



Structure::Structure() : Statement(kState_Structure)
{
	
}

Structure::~Structure()
{
	for(Variable* var: members)
		delete var;
}


Function::Function() : Statement(kState_Function)
{
	mBody = new Scope();
}

Function::~Function()
{
	
	for(Variable* var : args)
		delete var;

	delete mBody;
}


BinOp::BinOp() : Statement(kState_BinOpExpr)
{

}

BinOp::~BinOp()
{
	delete operand1;
	delete operand2;
}


FunctionCall::FunctionCall() : Statement(kState_FunctionCall)
{

}

FunctionCall::~FunctionCall()
{
	//don't delete parent func, deleted elsewhere
	for(Statement* arg : args)
		delete arg;
}


Return::Return() : Statement(kState_Return), expr(nullptr)
{

}

Return::~Return()
{
	delete expr;
}

Immediate::Immediate() : Statement(kState_ImmediateExpr), str(nullptr)
{

}

Immediate::~Immediate()
{
	//todo, delete the right thing
	if(mType->mId == kType_Basic)
	{
		BasicType* x = dynamic_cast<BasicType*>(mType);
		if(x->basicId == kBaseT_STRING)
		{
			if(str != nullptr)
				delete str;
		}
	}
}




















