#pragma once
#include "token.h"
#include <string>
#include <vector>

class Statement
{
public:
	Statement() = default;
	virtual ~Statement() = default;
	virtual std::string print() = 0;
	virtual bool compareName(const std::string &theName) = 0;
};

class FuncStatement : public Statement
{
public:
	FuncStatement() = default;
	~FuncStatement() = default;
	std::string mName;
	Token* mType;
	std::vector<Statement*> mStatementArray;
	std::vector<Arguement*> mArgArray;
	bool compareName(const std::string &theName) override;
	std::string print() override;
};

class VarStatement : public Statement
{
public:
	VarStatement();
	~VarStatement() = default;
	std::string mName;
	Token* mValue, *mType;

	bool compareName(const std::string &theName) override;
	std::string print() override;
};