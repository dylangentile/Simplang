#pragma once
#include "statement.h"
#include "lexer.h"
#include "error.h"

class Parser {
public:
	Parser();
	~Parser();
	void init(std::string fileName, bool fVerbose = true);
	void parse();

public:

private:
    void fetchToken();
    void lookahead(unsigned x);
    void doParseOnFunc(FuncStatement *theFunc);
private:
    std::vector<Token*>::iterator currentTokenIterator;
    Token *currentToken;
	bool mVerbose;
	bool itAtEnd;
	Lexer* myLexer;
	std::vector<Token*>* tokenArray;


};