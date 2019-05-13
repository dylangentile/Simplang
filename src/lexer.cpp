#include "lexer.h"
#include "error.h"
using namespace std;

Lexer::Lexer()
{
	myScanner = new Scanner;
}

Lexer::~Lexer()
{
	delete myScanner;
}


bool
Lexer::isWhitespaceChar(string i_char)
{
	int len = i_char.length(); 

	return (len == 1 && 
			(i_char.compare(" ")  == 0 || 
			i_char.compare("\n")  == 0 || 
			i_char.compare("\t")  == 0 || 
			i_char.compare("\r")  == 0 )
		);
}

void 
Lexer::init(string fileName)
{
	myScanner->init(fileName);
	getCharPackage();
}

void
Lexer::getCharPackage()
{
	c1Char = myScanner->getChar();
	c1 = c1Char->cargo;
	while(isWhitespaceChar(c1))
	{
		c1Char = myScanner->getChar();
		c1 = c1Char->cargo;
	}
	c2 = c1 + myScanner->lookAhead();
}


Token*
Lexer::fetchTokenPtr()
{

	if(c2 == "/*" || c2 == "//")
	{
		Token *commentBegin = new Token;
		commentBegin->charPass(c1Char);
		while(c2 == "/*")
		{
			while(c2 != "*/")
			{
				if(c1== "\0")
				{
					error(7, "Comment(^0^) opened at ^1^:^2^ doesn't have a closing tag!", commentBegin);
				}
				getCharPackage();
			}
			getCharPackage(); //iterate past the / in */
		}
		while(c2 == "//")
		{
			while(c1 != "\n")
			{
				getCharPackage();
			}
		}
	}

	Token *theToken = new Token;
	theToken->charPass(c1Char);

	if(c1 == "\0")
	{
		theToken->type = kToken_EOF;
		return theToken;
	}


	theToken->type = kToken_UNKNOWN;
	getCharPackage();
	return theToken;



}


vector<Token*>*
Lexer::lex()
{
	vector<Token*> *theV = new vector<Token*>;
	while(true)
	{
		Token *theToken;
		theToken = fetchTokenPtr();
		if(theToken->type == kToken_EOF)
		{
			return theV;
		}
		if(theToken->type == kToken_UNKNOWN)
		{
			error(1, "Unknown Character at ^1^:^2^ == ^0^", theToken);
		}
		theV->push_back(theToken);
	}
}



