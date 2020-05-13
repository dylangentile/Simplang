#pragma once
#include "token.h"
#include "lexer.h"
#include "statement.h"
#include "bimap.h"

#include <deque>

class Parser
{
public:
	Parser(const char* filename);
	~Parser();

	//it is up to the person outside the api to delete the scope
	Scope* parse();


private:

	void getDefs();

	
	Token* fetchToken();
	Token* lookAhead(uint32_t offset);

	Type* deriveTypeFromToken(Token* theToken);
	void getTypeList(std::vector<Type*>& typeVec);


	//parseExpr helpers
	FunctionCall* parseFunctionCall();
	Statement* parseExpr();
	


	Variable* fetchNextVariable(Type* theType);
	void parseVarDefs(std::vector<Type*>* typeArray = nullptr);


	void parseStruct();
	void parseFunction();


	void parseIntoScope();

	bool checkRedeclaration(const std::string& name, bool doError = true);
	StructType* findStructType(const std::string& name);


	Scope* currentScope();
private:
	Lexer* mLexer;

	std::vector<Token*>* tokenVec;
	std::vector<Token*>::iterator tokenIt;
	Token* currentToken;

	Token* nullToken; // = &theActualNullToken
	Token theActualNullToken;


	Scope* globalScope;
	std::deque<Scope*> scopeStack; 


	BiMap<TokenType, Type*, int, Type*> typeMap;
};