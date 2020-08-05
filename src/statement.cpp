#include "statement.h"
#include "error.h"



Stage1Program::Stage1Program()
{

}

Stage1Program::~Stage1Program()
{

}
















Statement::Statement(StatementID id) : mId(id)
{

}

Statement::~Statement()
{

}

void 
Statement::setDebug(const DebugData& thedata)
{
	debug = thedata;
}

Type*
Statement::getTypeInfo()
{
	return nullptr;
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
	return structMap.contains(name) || functionMap.contains(name) || enumMap.contains(name);
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
	func->setDebug(tok->mData);

	functionMap.insert(func, tok->mStr);
	return func;

}


StructType* 
Scope::insertStruct(Token* tok, bool isPublic)
{
	if(tok->mCat != kCat_WildIdentifier)
		lerror(kE_Fatal, tok, "Invalid struct identifier symbol!");
		
	if(usedSymbol(tok->mStr))
		lerror(kE_Fatal, tok, "Struct redeclaration!");


	StructType* theStruct = new StructType(tok->mStr);
	theStruct->definition = new Structure(tok->mStr, isPublic);
	theStruct->definition->setDebug(tok->mData);

	structMap.insert(theStruct, tok->mStr);
	return theStruct;


}

Variable*
Scope::insertVariable(Token* tok, Type* type)
{
	if(tok->mCat != kCat_WildIdentifier)
		lerror(kE_Fatal, tok, "symbol cannot be used for variable name!");

	Variable* var = new Variable(tok->mStr, type);
	var->setDebug(tok->mData);

	statementVec.push_back(dynamic_cast<Statement*>(var));
	return var;
}


DeclEqual*
Scope::insertDeclEqual(Token* tok)
{
	DeclEqual* de = new DeclEqual();
	de->setDebug(tok->mData);

	statementVec.push_back(dynamic_cast<Statement*>(de));
	return de;
}

MultipleAssignment*
Scope::insertMultipleAssignment(Token* tok)
{
	MultipleAssignment* ma = new MultipleAssignment();
	ma->setDebug(tok->mData);

	statementVec.push_back(dynamic_cast<Statement*>(ma));
	return ma;
}

Return*
Scope::insertReturn(Token* tok)
{
	Return* ret = new Return();
	ret->setDebug(tok->mData);

	statementVec.push_back(dynamic_cast<Return*>(ret));
	return ret;
}

void 
Scope::pushVariableStage2(Variable* x)
{
	auto finder = variableDecl.find(x->mName);
	if(finder != variableDecl.end())
		lerror(kE_Fatal, dynamic_cast<Statement*>(x), "duplicate symbol!");

	variableDecl.insert(std::make_pair(x->mName, x));
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

void
Scope::pushStatement(Statement* x)
{
	statementVec.push_back(x);
}

Variable::Variable(const std::string& name, Type* type) 
	: Statement(kState_Variable), mName(name), mType(type), mInitializer(nullptr)
{

}

Variable::~Variable()
{
	delete mInitializer;
}

Type*
Variable::getTypeInfo()
{
	return mType;
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
	for(VariableAccess* it : variableRefs)
		delete it;

	delete expr;
}

void 
MultipleAssignment::insert(const std::string& name)
{
	variableRefs.push_back(new VariableAccess(name));
}


Structure::Structure(const std::string& name_, bool isPublic_) 
					: Statement(kState_Structure), name(name_), isPublic(isPublic_)
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


IfStatement::IfStatement() : Statement(kState_If), condition(nullptr), elseEval(nullptr)
{
	mBody = new Scope();
}

IfStatement::~IfStatement()
{

}






BinOp::BinOp(const BinaryOperationID& op) : Statement(kState_BinOpExpr), mOp(op),  operand1(nullptr), operand2(nullptr)
{

}

BinOp::~BinOp()
{
	delete operand1;
	delete operand2;
}


Term::Term() : Statement(kState_Term), preOp(kPreOp_NULL), operand(nullptr), postOp(kPostOp_NULL)
{

}

Term::~Term()
{
	delete operand;
}

VariableAccess::VariableAccess(const std::string& name) : Statement(kState_VariableAccess), mName(name), refrencing(nullptr)
{

}

VariableAccess::~VariableAccess()
{
	//don't delete Variable* VariableAccess::refrencing, it's a refrence!
}


FunctionCall::FunctionCall() : Statement(kState_FunctionCall), parentFunc(nullptr), args(nullptr), argTypes(nullptr)
{

}

FunctionCall::~FunctionCall()
{
	//don't delete parent func, deleted elsewhere
	delete args;
}


Return::Return() : Statement(kState_Return), expr(nullptr)
{

}

Return::~Return()
{
	delete expr;
}

Immediate::Immediate(BasicType* type) : Statement(kState_ImmediateExpr), mType(type), u64(0)
{

}

Immediate::~Immediate()
{
	
	if(mType->basicId == kBaseT_STRING)
	{
		if(str != nullptr)
			delete str;
	}
	
}

//todo: proper error handling from here
bool 
Immediate::parseValue(const std::string& valStr)
{
	try
	{
		if(mType->basicId == kBaseT_STRING)
		{
			//todo: parse \n \\ \t etc
			memcpy(str, valStr.c_str(), valStr.size());
		}
		else if(mType->basicId == kBaseT_BOOL)
		{
			if(valStr == "true")
				boolVal = true;
			else if(valStr == "false")
				boolVal = false;
			else
				return false; //not a boolean value!
		}
		else if(mType->basicId == kBaseT_SINGLE)
		{
			fVal = std::stof(valStr);
		}
		else if(mType->basicId == kBaseT_DOUBLE)
		{
			dVal = std::stod(valStr);
		}
		else //must be a number
		{
			u64 = std::stoull(valStr);
		}

		ogString = valStr;
	}
	catch (const std::invalid_argument& e)
	{
		return false;
	}
	catch(const std::out_of_range& e)
	{
		return false;
	}
	return true;
	
}

StatementList::StatementList() : Statement(kState_StatementList)
{

}

StatementList::~StatementList()
{
	for(Statement* state : mVec)
		delete state;
}
void 
StatementList::insert(Statement* x)
{
	mVec.push_back(x);
}


















