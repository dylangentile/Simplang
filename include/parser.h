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
    ExpressionStatement* parseExpression(std::vector<Token*>::iterator start, std::vector<std::string> theNames, std::vector<Token*>::iterator *stop = nullptr);
    ExpressionStatement* parseBinExpression(std::vector<std::string> theNames, bool *empty, Token* prevOp);
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