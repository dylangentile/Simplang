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
	void fetchChar(char* c1, char* c2);
	char c1, c2;

private:
	std::string mContent;
	std::string::iterator fileIt;


	std::vector<Token*> tokenVec;
	std::vector<DebugData*> debugDataVec;



	BiMap<TokenType, std::string, int, std::string> operatorMap,
													typeMap,
													keywordMap,
													langOperatorMap;






};