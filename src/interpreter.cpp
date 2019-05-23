#include "interpreter.h"
#include <typeinfo>
#include <iostream>

using namespace std;

Interpreter::Interpreter(FuncStatement* v)
{
	VarStatement a;
	FuncCallStatement b;
	ValueStatement c;
	OperatorStatement d;
	ExpressionStatement e;
	BinExpressionStatement f;
	FuncStatement g;
	typeHashMatch.insert(make_pair(typeid(a).hash_code(), kState_VAR));
	typeHashMatch.insert(make_pair(typeid(b).hash_code(), kState_FUNCCALL));
	typeHashMatch.insert(make_pair(typeid(c).hash_code(), kState_VALUE));
	typeHashMatch.insert(make_pair(typeid(d).hash_code(), kState_OP));
	typeHashMatch.insert(make_pair(typeid(e).hash_code(), kState_EXPR));
	typeHashMatch.insert(make_pair(typeid(f).hash_code(), kState_BINEXPR));
	typeHashMatch.insert(make_pair(typeid(g).hash_code(), kState_FUNC));

	gFunc = v;
}

Interpreter::~Interpreter()
{

}

void
Interpreter::interpret()
{
	cout << "\n";
	for (auto it = gFunc->mStatementArray.begin(); it != gFunc->mStatementArray.end(); ++it)
	{
		std::map<size_t, StatementID>::iterator id = typeHashMatch.find(typeid(*(*it)).hash_code());
		
		if (id == typeHashMatch.end())
		{
			//runtime error;
		}
		StatementID theId = id->second;
		if (theId == kState_VAR)
		{
			
		}
	}
}