#pragma once
#include "statement.h"
#include "lexer.h"
#include "error.h"

class Parser {
public:
	Parser();
	~Parser() = default;
	void init(const std::string &fileName, bool fVerbose = true);
	void parse();

public:

private:
    void fetchToken(unsigned t = 1);
    Token* tokenLookahead(unsigned x);
    void doParseOnFunc(FuncStatement *theFunc, std::vector<std::string> theNames);

    bool doesNameNotExist(std::vector<std::string> &nameArray, const std::string &what);
private:
    //unsigned whereAmI;
    std::vector<Token*>::iterator currentTokenIterator;
    Token *currentToken;
	bool mVerbose;
	bool itAtEnd;
	Lexer* myLexer;
	std::vector<Token*>* tokenArray;


};