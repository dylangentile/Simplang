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
	kState_Structure,
	kState_Function,
	kState_FunctionCall,
	
	kState_ImmediateExpr,
	kState_BinOpExpr,
	
	kState_DeclEqual,
	kState_MultipleAssignment,
	kState_Return

}StatementID;

typedef enum
{
	kOp_ADD,
	kOp_SUB,
	kOp_MUL,
	kOp_DIV,
	kOp_MOD
}OperationID;

class Function;
class Variable;
class FunctionCall;
class DeclEqual;
class MultipleAssignment;
class Return;


class Statement
{
public:
	Statement(StatementID id_);
	virtual ~Statement();

	const StatementID mId;

};


class Scope : public Statement
{
public:
	Scope();
	~Scope();

	bool usedSymbol(const std::string& name);
	
	Function* 			insertFunction(Token* tok);
	StructType* 		insertStruct(Token* tok);
	Variable* 			insertVariable(Token* tok, Type* type);
	DeclEqual* 			insertDeclEqual();
	MultipleAssignment*	insertMultipleAssignment();
	Return*				insertReturn();

	Type* getType(const std::string& name);

public:
	BiMap<StructType*, std::string, StructType*, std::string> structMap;
	BiMap</*Enum*/Type*, std::string, /*Enum*/Type*, std::string> enumMap;
	BiMap<Function*, std::string, Function*, std::string> functionMap;
	BiMap<UnknownType*, std::string, UnknownType*, std::string> unknownTypeMap; 

	
	std::vector<Statement*> statementVec;

};

class Variable : public Statement
{
public:
	Variable(const std::string& name, Type* type);
	~Variable();

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

	std::vector<std::string> names; //1st stage
	std::vector<Variable*> variableRefs; //2nd stage
	Statement* expr;
};



class Structure : public Statement
{
public:
	Structure();
	~Structure();
	std::vector<Variable*> members;
	//TODO verify names on second pass
};


class Function : public Statement
{
public:
	Function();
	~Function();

	std::vector<Type*> typeVec;

	std::string mName;
	std::vector<Variable*> args;
	Scope* mBody;
};




class BinOp : public Statement
{
public:
	BinOp();
	~BinOp();

	OperationID mOp;
	Statement* operand1;
	Statement* operand2;
};


class FunctionCall : public Statement
{
public:
	FunctionCall();
	~FunctionCall();

	Function* parentFunc;
	std::vector<Statement*> args;
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
	Immediate();
	~Immediate();
	Type* mType;

	union
	{
		uint64_t u64;
		uint32_t u32;
		uint16_t u16;
		union
		{
			uint8_t u8;
			uint8_t byte;
			uint8_t boolean;
		};

		int64_t i64;
		int32_t i32;
		int16_t i16;
		int8_t i8;

		char* str;

		Type* typenameVal;

		float fVal;
		double dVal;

	};
};














