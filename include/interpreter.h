#pragma once
#include "statement.h"
#include "error.h"
#include "object.h"
#include <iostream>
#include <map>
#include <cstdint>



class StackFrame
{
public:
	StackFrame();
	~StackFrame();
	std::map<std::string, Variant*> VariantMap;
};

class Interpreter
{
public:
	Interpreter(FuncStatement* theGlobal/*, std::vector<FuncStatement*>* allFuncs*/);
	~Interpreter();

	

	void interpret(std::vector<Statement*> statementVector, bool needNewStack = true);
	void interpret();
private:
	std::vector<FuncStatement*> allFunctions;
	FuncStatement* gFunc;
	std::stack<StackFrame*> sfStack;

private:
	void doPrint(std::vector<Statement*> args, StackFrame* theFrame);
	Variant* findVariant(std::string name, StackFrame* theFrame = nullptr);
	void printVariant(Variant* printedOne);
	Variant ValState_to_Variant(ValueStatement valState);
	void EvaluateBinExpr(Variant* dest, BinExpressionStatement* theExpr);
	Variant* StatementToVariant(Statement* theStatement);


};