#pragma once
#include "token.h"
#include "bimap.h"
#include <vector>

class Lexer
{
public:
	Lexer(const char* filename);
	~Lexer();



	
	std::vector<Token*>* lex();

private:
	Token* fetchToken();
	void fetchChar();
	char c1, c2;

private:
	std::string mContent;
	std::string::iterator fileIt;
	uint32_t currentLine = 1, currentCol = 0;
	uint32_t* counter;
	const char* linePointer;

	std::vector<Token*> tokenVec;



	BiMap<TokenType, std::string, int, std::string> operatorMap,
													typeMap,
													keywordMap,
													langOperatorMap;






};