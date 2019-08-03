#pragma once
#include "statement.h"
#include "lexer.h"
#include "error.h"
#include <map>

typedef struct
{
  Token *mName;
  Statement *mValue = nullptr;
  FuncStatement *mFunc = nullptr;
  TokenID mType;
} Identifier;


class Parser {
public:
	Parser(const std::string &fileName, bool fVerbose = true);
	~Parser();
	//void init(const std::string &fileName, bool fVerbose = true);
	FuncStatement* parse();
    //std::string print();


 public:
    std::string mLog;




private:
    void fetchToken(unsigned t = 1);
    Token* tokenLookahead(unsigned x);
    void doParseOnFunc(FuncStatement *theFunc, std::vector<Identifier *> theNames);
    void multipleVarInitializations(FuncStatement *theFunc, std::vector<Identifier*> &theNames);
    ExpressionStatement* parseExpression(std::vector<Identifier *> theNames, TokenID type);
    BinExpressionStatement* parseBinExpression(std::vector<Identifier *> theNames, TokenID type, Token* prevOp, BinExpressionStatement *theTerms = nullptr);
    unsigned int getPrecedence(Token* x);
    Identifier* doesNameNotExist(std::vector<Identifier *> &nameArray, Token *what);
    Statement* makeBinExprTerm(Token* outOfMe);
    std::vector<Statement*> parseFuncArgs(std::vector<Identifier*> theNames);



private:
  //unsigned whereAmI;
  std::vector<Token*>::iterator currentTokenIterator;
  Token *currentToken;
	bool mVerbose;
	bool itAtEnd;
	Lexer* myLexer;
	std::vector<Token*>* tokenArray;
	std::map<TokenID, unsigned int> precedenceMap;


};
