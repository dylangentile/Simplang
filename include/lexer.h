#pragma once
#include "scanner.h"
#include "token.h"
#include <string>


class Lexer{
public:
	Lexer();
	~Lexer();
	void init(std::string fileName);
	Token* fetchTokenPtr();
private:
	void getCharPackage();
public:

private:
	Scanner* myScanner;
	std::string c1, c2;
	Character *c1Char;
	Token* currentToken;

};