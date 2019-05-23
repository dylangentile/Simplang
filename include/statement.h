#pragma once
#include "token.h"
#include <string>
#include <vector>
#include <stack>


//template <typename T>
/*
class Term
{
public:
    Term();
    ~Term();
    std::string print();

};
*/






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


class FuncCallStatement : public Statement 
{
public:
	FuncCallStatement() = default;
	~FuncCallStatement() = default;
	bool compareName(const std::string& theName) { return false; }
	std::string print() override;
	std::vector<Statement*> mArgArray;
	FuncStatement* whoAmICalling;



};


class ValueStatement : public Statement
{
public:
	ValueStatement() = default;
	~ValueStatement() = default;
	Token* mValue;
	bool compareName(const std::string& theName) { return false; }
	std::string print() override;

};

class OperatorStatement : public Statement
{
public:
	OperatorStatement() = default;
	~OperatorStatement() = default;
	bool compareName(const std::string& theName) { return false; }
	std::string print() override;
	void insertOp(Token* x);
protected:
	TokenID mValue;
	Token* ogToken;
};



class ExpressionStatement : public Statement
{
public:
    ExpressionStatement() = default;
    virtual ~ExpressionStatement() = default;
    std::vector<Statement*> mTermVector;
    bool compareName(const std::string &theName) {return false;}
    std::string print();


};



class BinExpressionStatement : public ExpressionStatement
{
public:
    BinExpressionStatement() = default;
    ~BinExpressionStatement() = default;
	std::vector<Statement*> mTermVector;
	bool compareName(const std::string& theName) { return false; }
    std::string print();

    //  std::stack<Term*> mTermStack;
   // bool compareName(const std::string &theName) override;
    //std::string print() override;


};



class VarStatement : public Statement
{
public:
	VarStatement();
	~VarStatement() = default;
	std::string mName;
	Token* mType;
	ExpressionStatement* mValue;


	bool compareName(const std::string &theName) override;
	std::string print() override;
};