#pragma once
#include "statement.h"
#include "lexer.h"
#include "error.h"
#include <map>

class Parser {
public:
	Parser(const std::string &fileName, bool fVerbose = true);
	~Parser() = default;
	//void init(const std::string &fileName, bool fVerbose = true);
	void parse();
    //std::string print();


 public:
    std::string mLog;




private:
    void fetchToken(unsigned t = 1);
    Token* tokenLookahead(unsigned x);
    void doParseOnFunc(FuncStatement *theFunc, std::vector<std::string> theNames);
    void multipleVarInitializations(FuncStatement *theFunc, std::vector<std::string> theNames);
    ExpressionStatement* parseExpression(std::vector<std::string> theNames);
    BinExpressionStatement* parseBinExpression(std::vector<std::string> theNames, Token* prevOp, BinExpressionStatement *theTerms = nullptr);
    unsigned int getPrecedence(Token* x);
    bool doesNameNotExist(std::vector<std::string> &nameArray, const std::string &what);



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