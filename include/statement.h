#pragma once
#include "typing.h"
#include "bimap.h"

#include <cstdlib>
#include <vector>
#include <string>
#include <unordered_set>

typedef enum 
{
	kState_NULL = 0,
	kState_Scope,
	kState_Variable,
	kState_VariableAssignment,
	kState_Structure,
	kState_Function,
	kState_FunctionCall,
	
	//kState_BaseExpr,
	kState_ImmediateExpr,
	kState_BinOpExpr

}StatementID;

typedef enum
{
	kOp_ADD,
	kOp_SUB,
	kOp_MUL,
	kOp_DIV,
	kOp_MOD
}OperationID;




class Statement
{
public:
	Statement(StatementID id_);
	~Statement();

	StatementID mId;

};

//class Expression;

class Scope : public Statement
{
public:
	Scope();
	~Scope();

	BiMap<StructType*, std::string, StructType*, std::string> structDefinitions;
	std::unordered_set<std::string> usedNames;

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


class VariableAssignment : public Statement
{
public:
	VariableAssignment();
	~VariableAssignment();

	Variable* assignTo;
	Statement* value; 
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

	Type* type;

	std::string mName;
	std::vector<Variable*> args;
	Scope* mBody;
};


/*class Expression : public Statement
{
public:
	Expression(StatementID id = kState_BaseExpr);
	~Expression();


};*/


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














