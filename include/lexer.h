#pragma once
#include "scanner.h"
#include "token.h"
#include <string>
#include <vector>


class Lexer{
public:
	Lexer();
	~Lexer();
	void init(std::string fileName);
	std::vector<Token*>* lex();
private:
	void getCharPackage();
	Token* fetchTokenPtr();
	bool isWhitespaceChar(std::string i_char);

public:

private:
	Scanner* myScanner;
	std::string c1, c2;
	Character *c1Char;

};