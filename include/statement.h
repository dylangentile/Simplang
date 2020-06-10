#pragma once
#include "typing.h"
#include "bimap.h"
#include "token.h"

#include <cstdlib>
#include <vector>
#include <string>
#include <unordered_set>

typedef enum 
{
	kState_NULL = 0,
	kState_Scope,
	kState_Variable,
	kState_VariableAccess,
	kState_Structure,
	kState_Function,
	kState_FunctionCall,

	kState_If,
	
	kState_ImmediateExpr,
	kState_BinOpExpr,
	
	kState_DeclEqual,
	kState_MultipleAssignment,
	kState_Return,

	kState_StatementList,
	kState_Term

}StatementID;

typedef enum
{
	kOp_NULL,

	kOp_ADD,
	kOp_SUB,
	kOp_MUL,
	kOp_DIV,
	kOp_MOD,
	
	kOp_L_AND,
	kOp_L_OR,
	kOp_L_EQUAL,
	kOp_L_NOT_EQUAL,

	kOp_LESS,
	kOp_GREATER,
	kOp_LESS_EQUAL,
	kOp_GREATER_EQUAL,

	kOp_BIT_AND,
	kOp_BIT_OR,

	kOp_NULLCC //null collasesce
}BinaryOperationID;


typedef enum 
{
	kPreOp_NULL,
	kPreOp_NOT, 	//!
	kPreOp_NEG, 	//-
	kPreOp_DEREF,	//*
	kPreOp_COMPL,	//~
	kPreOp_ADDROF,	//&
	kPreOp_ATADDR	//@
}PrefixOpID;

typedef enum 
{
	kPostOp_NULL,
	kPostOp_INCR, //++
	kPostOp_DECR, //--
	//kPostOp_ARRAY,
	kPostOp_DOT,  //.
	kPostOp_SFNAV //?.
}PostfixOpID;

class Scope;
class Function;
class Variable;
class FunctionCall;
class DeclEqual;
class MultipleAssignment;
class Return;
class VariableAccess;
class StatementList;



class Stage1Program
{
public:
	Stage1Program();
	~Stage1Program();
};




class Statement
{
public:
	Statement(StatementID id_);
	virtual ~Statement();

	virtual void setDebug(const DebugData&);
	virtual Type* getTypeInfo();
	const StatementID mId;
	DebugData debug;

};


class Scope : public Statement
{
public:
	Scope();
	~Scope();

	bool usedSymbol(const std::string& name);
	
	Function* 			insertFunction(Token* tok);
	StructType* 		insertStruct(Token* tok, bool isPublic);
	Variable* 			insertVariable(Token* tok, Type* type);
	DeclEqual* 			insertDeclEqual(Token*);
	MultipleAssignment*	insertMultipleAssignment(Token*);
	Return*				insertReturn(Token*);
	void				pushStatement(Statement*);


	void pushVariableStage2(Variable*);

	Type* getType(const std::string& name);

public:
	BiMap<StructType*, std::string, StructType*, std::string> structMap;
	BiMap</*Enum*/Type*, std::string, /*Enum*/Type*, std::string> enumMap;
	BiMap<Function*, std::string, Function*, std::string> functionMap;
	BiMap<UnknownType*, std::string, UnknownType*, std::string> unknownTypeMap; 


	std::unordered_map<std::string, Variable*> variableDecl;
	
	std::vector<Statement*> statementVec;

};

class Variable : public Statement
{
public:
	Variable(const std::string& name, Type* type);
	~Variable();
	virtual Type* getTypeInfo() override;


	std::string mName;
	Type* mType;
	Statement* mInitializer;

};


class DeclEqual : public Statement
{
public:
	DeclEqual();
	~DeclEqual();

	Variable* insert(const std::string& name);
	
	std::vector<Variable*> varVec;
	Statement* mInitializer;



	
};

class MultipleAssignment : public Statement
{
public:
	MultipleAssignment();
	~MultipleAssignment();

	void insert(const std::string&);

	std::vector<VariableAccess*> variableRefs; 
	Statement* expr;
};



class Structure : public Statement
{
public:
	Structure(const std::string& name_, bool isPublic_ = false);
	~Structure();
	std::vector<Variable*> members;
	std::string name;
	bool isPublic;
	//TODO verify names on second pass
};


class Function : public Statement
{
public:
	Function();
	~Function();

	MultipleType* typeVec;

	std::string mName;
	std::vector<Variable*> args;
	
	MultipleType* argTypes;

	Scope* mBody;
};


class IfStatement : public Statement
{
public:
	IfStatement();
	~IfStatement();

	Statement* condition;
	Scope* mBody;
	Statement* elseEval;

};




class BinOp : public Statement
{
public:
	BinOp(const BinaryOperationID& op);
	~BinOp();

	BinaryOperationID mOp;
	Statement* operand1;
	Statement* operand2;
};

class Term : public Statement
{
public:
	Term();
	~Term();

	PrefixOpID 		preOp;
	Statement* 		operand;
	PostfixOpID 	postOp;
};

class VariableAccess : public Statement
{
public:
	VariableAccess(const std::string& name);
	~VariableAccess();

	std::string mName;
	Variable* refrencing;
};


class FunctionCall : public Statement
{
public:
	FunctionCall();
	~FunctionCall();

	Function* parentFunc;// 2nd stage
	std::string callName;
	
	StatementList* args;
	MultipleType* argTypes;
};


class Return : public Statement
{
public:
	Return();
	~Return();
	Statement* expr;
};

class Immediate : public Statement
{
public:
	Immediate(BasicType* type);
	~Immediate();
	BasicType* mType;

	union
	{
		uint64_t u64;
		uint32_t u32;
		uint16_t u16;
		uint8_t u8;
		uint8_t byte;
		

		int64_t i64;
		int32_t i32;
		int16_t i16;
		int8_t i8;

		char* str;

		Type* typenameVal;

		float fVal;
		double dVal;

		bool boolVal;

	};

	std::string ogString;

	bool parseValue(const std::string& valStr);
};



class StatementList : public Statement
{
public:
	StatementList();
	~StatementList();

	void insert(Statement* x);
	std::vector<Statement*> mVec;

};














