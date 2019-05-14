#pragma once
#include "token.h"
#include <string>
#include <vector>

class Statement
{
public:
	Statement();
	virtual ~Statement();
	virtual std::string print() = 0;
	virtual bool compareName(std::string theName) = 0;
};

class FuncStatement : public Statement
{
public:
	FuncStatement();
	~FuncStatement();
	std::string mName;
	Token* mType;
	std::vector<Statement*> mStatementArray;
	std::vector<Arguement*> mArgArray;
	bool compareName(std::string theName);
	std::string print();
};

class VarStatement : public Statement
{
public:
	VarStatement();
	~VarStatement();
	std::string mName;
	Token* mValue, *mType;

	bool compareName(std::string theName);
	std::string print();
};