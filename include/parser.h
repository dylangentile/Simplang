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

	//it is up to the person outside the parser to delete the scope
	Scope* parse();

	std::string unitName;
private:

	void getDefs();

	
	Token* fetchToken();
	Token* lookAhead(uint32_t offset);

	Type* findType(Token* theToken);
	MultipleType* getTypeList();


	//parseExpr helpers
	FunctionCall* parseFunctionCall();
	Statement* parseExpr(bool haltComma = false, bool haltRPAREN = false);
	Statement* parseBinExpr(bool haltRPAREN = false);
	Statement* parseTerm();

	Statement* parseIfStatement();


	Variable* fetchNextVariable(Type* theType);
	void parseVarDefs(std::vector<Type*>* typeArray = nullptr);


	void parseStruct();
	void parseFunction(Type*);
	void handleType(std::vector<Variable*>* varVec = nullptr);

	void initScopeParse(Scope*);
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

	BiMap<TokenType, PrefixOpID, int, int> preOpMap;
	BiMap<TokenType, PostfixOpID, int, int> postOpMap;

	BiMap<TokenType, BinaryOperationID, int, int> binOpMap;


};












