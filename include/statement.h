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





class Statement
{
public:
	Statement() = default;
	virtual ~Statement() = default;
	virtual std::string print() = 0;
	virtual bool compareName(const std::string &theName) = 0;
	virtual StatementID fetchId() = 0;
	//StatementID mId;
};

class FuncStatement : public Statement
{
public:
	FuncStatement() {mId = kState_FUNC;}
	~FuncStatement() = default;
	std::string mName;
	Token* mType;
	std::vector<Statement*> mStatementArray;
	std::vector<Arguement*> mArgArray;
	bool compareName(const std::string &theName);
	std::string print();
	StatementID mId;
    StatementID fetchId() {return mId;}

};


class FuncCallStatement : public Statement 
{
public:
	FuncCallStatement() { mId = kState_FUNCCALL; }
	~FuncCallStatement() = default;
	bool compareName(const std::string& theName) { return false; }
	std::string print();
	std::vector<Statement*> mArgArray;
	FuncStatement* whoAmICalling;
    StatementID mId;
    StatementID fetchId(){return mId;}






};


class ValueStatement : public Statement
{
public:
	ValueStatement() { mId = kState_VALUE; }
	~ValueStatement() = default;
	Token* mValue;
	bool compareName(const std::string& theName) { return false; }
	std::string print();
    StatementID mId;
    StatementID fetchId(){return mId;}



};

class OperatorStatement : public Statement
{
public:
	OperatorStatement() {mId = kState_OP;}
	~OperatorStatement() = default;
	bool compareName(const std::string& theName) { return false; }
	std::string print();
	void insertOp(Token* x);
    TokenID mValue;
    Token* ogToken;
    StatementID mId;
    StatementID fetchId(){return mId;}


};



class ExpressionStatement : public Statement
{
public:
    ExpressionStatement(){mId = kState_EXPR;}
    virtual ~ExpressionStatement() = default;
    std::vector<Statement*> mTermVector;
    bool compareName(const std::string &theName) {return false;}
    std::string print();
    StatementID mId;
    StatementID fetchId(){return mId;}



};



class BinExpressionStatement : public ExpressionStatement
{
public:
    BinExpressionStatement() {mId = kState_BINEXPR;}
    ~BinExpressionStatement() = default;
	std::vector<Statement*> mTermVector;
	bool compareName(const std::string& theName) { return false; }
    std::string print();
	StatementID mId;
    StatementID fetchId(){return mId;}


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
    StatementID mId;
    StatementID fetchId(){return mId;}


    bool compareName(const std::string &theName);
	std::string print();
};