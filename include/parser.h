#pragma once
#include "token.h"
#include "lexer.h"
#include "statement.h"
#include "bimap.h"

#include <deque>

inline bool isBinaryOperator(TokenType);

class Parser
{
public:
	Parser(const char* filename);
	~Parser();

	//it is up to the person outside the parser to delete the scope
	Scope* parse();

	std::string unitName;
private:

	void getDefs();

	
	Token* fetchToken();
	Token* lookAhead(uint32_t offset);

	Type* findType(Token* theToken);
	void getTypeList(std::vector<Type*>& typeVec);


	//parseExpr helpers
	FunctionCall* parseFunctionCall();
	Statement* parseExpr(bool haltComma = false);
	Statement* parseBinaryExpr(Statement* term1);
	


	Variable* fetchNextVariable(Type* theType);
	void parseVarDefs(std::vector<Type*>* typeArray = nullptr);


	void parseStruct();
	void parseFunction(const std::vector<Type*>& typeVec);
	void handleType(std::vector<Variable*>* varVec = nullptr);

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
	std::unordered_map<Type*, Type*> ptrTypeMap;
};