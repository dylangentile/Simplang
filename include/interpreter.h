#pragma once
#include "statement.h"
#include "error.h"
#include "object.h"
#include <iostream>
#include <map>
#include <cstdint>
/*
template <typename T>
class Variant
{
public:
	Variant(T in) { value = in; }
	~Variant() = default;
	T value;

};
*/



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
	Interpreter(FuncStatement* v);
	~Interpreter();

	void interpret();

private:
	FuncStatement *gFunc;
	std::map<size_t, StatementID> typeHashMatch;
    //std::map<std::string, uint8_t> nameMap;
	std::stack<StackFrame*> sfStack;
	uint8_t currentAddress;


	Variant* findVariant(std::string name, StackFrame* theFrame);

	void doPrint(std::vector<Statement*> args, StackFrame* theFrame);


};