#pragma once
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

private:
	bool mVerbose;
	Lexer* myLexer;
	std::vector<Token*>* tokenArray;


};