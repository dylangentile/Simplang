#pragma once
#include "statement.h"
#include <map>

template <typename T>
class Variant
{
public:
	Variant(T in) { value = in; }
	~Variant() = default;
	T value;

};


class StackFrame
{
public:
	StackFrame();
	~StackFrame();
	std::map<std::string, Variant*> VariantMaps;
};


typedef enum
{
	kState_VAR,
	kState_FUNCCALL,
	kState_FUNC,
	kState_BINEXPR,
	kState_EXPR,
	kState_OP,
	kState_VALUE
}StatementID;


class Interpreter
{
public:
	Interpreter(FuncStatement* v);
	~Interpreter();

	void interpret();

private:
	FuncStatement *gFunc;
	std::map<size_t, StatementID> typeHashMatch;

};