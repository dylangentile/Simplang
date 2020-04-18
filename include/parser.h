#pragma once
#include "token.h"
#include "lexer.h"
#include "statement.h"
#include "bimap.h"

#include <stack>

class Parser
{
public:
	Parser(const char* filename);
	~Parser();

	//it is up to the person outside the api to delete the scope
	Scope* parse();


private:
	
	Token* fetchToken();
	Token* lookAhead(uint32_t offset);

	void parseStruct();
	void parseIntoScope(Scope* theScope);

private:
	Lexer* mLexer;
	std::vector<Token*>* tokenVec;
	std::vector<Token*>::iterator tokenIt;
	Token* currentToken;

	Scope* globalScope;
	std::stack<Scope*> scopeStack; 


	BiMap<TokenType, Type*, int, Type*> typeMap;
};