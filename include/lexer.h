#pragma once
#include "scanner.h"
#include "token.h"
#include <string>
#include <vector>
#include <unordered_map>


class Lexer{
public:
	Lexer();
	~Lexer();
	void init(std::string fileName);
	std::vector<Token*>* lex();
private:
	void getCharPackage(bool fetchWhitespace = false);
	Token* fetchTokenPtr();
	bool isWhitespaceChar(std::string i_char);
	bool isType(std::string i_char, TokenID &theId, int &mIt);
	bool isIdentifierChar(std::string i_char, bool begin = false);
	bool isKeyword(std::string i_cargo, TokenID &theId);
	bool isOperator(std::string i_char, TokenID &theId);
	bool isNumber(std::string i_char);
public:

private:
	Scanner* myScanner;
	std::string c1, c2;
	Character *c1Char;
	std::unordered_map<std::string, TokenID> typeMap, keywordMap, operatorMap;


};

std::string printTokenArray(std::vector<Token*>  *v);