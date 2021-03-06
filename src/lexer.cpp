#include "lexer.h"
#include "error.h"
#include <iostream>
using namespace std;

Lexer::Lexer()
{
	myScanner = new Scanner;
	typeMap.insert(make_pair("num", kToken_NUMBER));
	typeMap.insert(make_pair("num8", kToken_NUMBER8));
	typeMap.insert(make_pair("num16", kToken_NUMBER16));
	typeMap.insert(make_pair("num32", kToken_NUMBER32));
	typeMap.insert(make_pair("num64", kToken_NUMBER64));
	typeMap.insert(make_pair("numfp", kToken_NUMBER_FP));
	typeMap.insert(make_pair("bool", kToken_BOOL));
	typeMap.insert(make_pair("string", kToken_STRING));
	typeMap.insert(make_pair("ptr", kToken_POINTER));
	typeMap.insert(make_pair("void", kToken_VOID));
	typeMap.insert(make_pair("typename", kToken_TYPENAME));
	typeMap.insert(make_pair("dynamic", kToken_DYNAMIC));



	keywordMap.insert(make_pair("print", kToken_PRINT));
	keywordMap.insert(make_pair("if", kToken_IF));
	keywordMap.insert(make_pair("else", kToken_ELSE));



	operatorMap.insert(make_pair("=", kToken_EQUALS));
	operatorMap.insert(make_pair("(", kToken_LPAREN));
	operatorMap.insert(make_pair(")", kToken_RPAREN));
	operatorMap.insert(make_pair("{", kToken_LCURLY));
	operatorMap.insert(make_pair("}", kToken_RCURLY));

	operatorMap.insert(make_pair("+", kToken_ADD));
	operatorMap.insert(make_pair("-", kToken_SUBTRACT));
	operatorMap.insert(make_pair("*", kToken_MULTIPLY));
	operatorMap.insert(make_pair("/", kToken_DIVIDE));
	operatorMap.insert(make_pair("%", kToken_MODULO));
	operatorMap.insert(make_pair("^", kToken_POWER));


	operatorMap.insert(make_pair("+=", kToken_PLUS_EQUALS));
	operatorMap.insert(make_pair("-=", kToken_MINUS_EQUALS));
	operatorMap.insert(make_pair("*=", kToken_MULTIPLY_EQUALS));
	operatorMap.insert(make_pair("/=", kToken_DIVIDE_EQUALS));
	operatorMap.insert(make_pair("^=", kToken_POWER_EQUALS));
	operatorMap.insert(make_pair("%=", kToken_MOD_EQUALS));

	operatorMap.insert(make_pair("==", kToken_EQUALITY));
	operatorMap.insert(make_pair("<", kToken_LESS));
	operatorMap.insert(make_pair(">", kToken_GREATER));
	operatorMap.insert(make_pair("!", kToken_NOT));
	operatorMap.insert(make_pair("<=", kToken_LESS_EQUAL));
	operatorMap.insert(make_pair(">=", kToken_GREATER_EQUAL));
	operatorMap.insert(make_pair("!=", kToken_NOT_EQUAL));
	operatorMap.insert(make_pair("&&", kToken_LOGIC_AND));
	operatorMap.insert(make_pair("||", kToken_LOGIC_OR));




}
Lexer::~Lexer()
{
	delete myScanner;
}

bool
Lexer::isKeyword(string i_cargo, TokenID &theId)
{
	unordered_map<string, TokenID>::iterator it = keywordMap.find(i_cargo);
	if(it == keywordMap.end())
		return false;
	theId = it->second;
	return true;

}

bool 
Lexer::isIdentifierChar(string i_char, bool begin)
{
	char theChar = i_char[0];
	if(begin)
	{
		return (theChar >= 'a' && theChar <= 'z') ||
			   (theChar >= 'A' && theChar <= 'Z');
	}
	return (theChar >= 'a' && theChar <= 'z') ||
		   (theChar >= 'A' && theChar <= 'Z') ||
		   (theChar >= '0' && theChar <= '9') ||
		   (theChar == '_');
}

bool
Lexer::isNumber(string i_char)
{
	char theChar = i_char[0];
	return (theChar >= '0' && theChar <= '9');
}

bool
Lexer::isType(string i_char, TokenID &theId, int &mIt)
{
	
	mIt = 2;
	string la = myScanner->lookAhead(mIt);
	mIt++;
	while(isIdentifierChar(la))
	{
		i_char += la;
		la = myScanner->lookAhead(mIt);
		mIt++;
	}

	unordered_map<string, TokenID>::iterator it = typeMap.find(i_char);
	if(it == typeMap.end())
		return false;
	//todo: rewrite this for better
	theId = it->second;

	/*if(theId == kToken_NUMBER)
		theId = kToken_NUMBER32; //todo: debate whether or not this is good??*/ //also token.h already takes car of this...
	return true;



}

bool
Lexer::isOperator(string i_char, TokenID &theId)
{
	unordered_map<string, TokenID>::iterator it = operatorMap.find(i_char);
	if(it == operatorMap.end())
		return false;
	theId = it->second;
	return true;
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
}

void
Lexer::getCharPackage(bool fetchWhitespace)
{
	c1Char = myScanner->getChar();
	c1 = c1Char->cargo;
	if(!fetchWhitespace)
	{
		while(isWhitespaceChar(c1))
		{
			c1Char = myScanner->getChar();
			c1 = c1Char->cargo;
		}
	}
	c2 = c1 + myScanner->lookAhead();
}


Token*
Lexer::fetchTokenPtr()
{
	getCharPackage();

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
			while(c1 != "\n" && c1 != "\0")
			{
				getCharPackage(true);
				commentBegin->addChar(c1Char);
			}
		}
		commentBegin->type = c1 == "\0" ? kToken_EOF : kToken_COMMENT;
		return commentBegin;
	}

	Token *theToken = new Token;
	theToken->charPass(c1Char);

	if(c1 == "\0")
	{
		theToken->type = kToken_EOF;
		return theToken;
	}

	if(c1 == ";")
	{
		theToken->type = kToken_SEMICOLON;
		theToken->cat = kCat_EOS;
		return theToken;

	}

	if(c1 == ",")
	{
		theToken->type = kToken_COMMA;
		theToken->cat = kCat_NULL;
		return theToken;
	}
	{
		int laAmount = 0;
		if(isType(c2, theToken->type, laAmount))
		{
			for(int i = 0; i < laAmount - 2; i++)
			{
				getCharPackage();
				theToken->addChar(c1Char);
			}
			theToken->cat = kCat_TYPE;
			theToken->fixTypes();
			return theToken;
		}
	}


	if(isIdentifierChar(c1, true))
	{
		//getCharPackage();
		if(isIdentifierChar(myScanner->lookAhead(1)))
		{
			getCharPackage();
			while(isIdentifierChar(myScanner->lookAhead(1)))
			{
				theToken->addChar(c1Char);
				getCharPackage();
			}
			theToken->addChar(c1Char);
		}
		if (theToken->cargo == "true" || theToken->cargo == "false")
		{
			theToken->cat = kCat_VALUE;
			theToken->type = kToken_BOOL;
			return theToken;
		}

		if(isKeyword(theToken->cargo, theToken->type))
		{
			theToken->cat = kCat_KEYWORD;
			return theToken;
		}
		theToken->cat = kCat_IDENTIFIER;
		theToken->type= kToken_IDENTIFIER;
		return theToken;
	}

	if(isNumber(c1))
	{
		bool fp = false;
		
		while(isNumber(myScanner->lookAhead(1)) || myScanner->lookAhead(1) == ".")
		{
			getCharPackage();
			theToken->addChar(c1Char);
			if(c1 == ".")
			{
				if(fp)
					error(6, "number value: '^0^' at ^1^:^2^ has extra periods...", true, theToken);
				fp = true;
				
			}
		}

		theToken->type = fp ? kToken_NUMBER_FP : kToken_NUMBER;
		theToken->cat = kCat_VALUE;
		theToken->fixTypes();
		return theToken;
	}

	if(c1 == "\"")
	{
		//todo: account for all escape codes...
		getCharPackage(true);
		if(c1 != "\"")
		{
			if(c1 == "\\")
			{
				getCharPackage(true);
				if(c1 == "n")
				{
					c1Char->cargo = "\n";
				}
			}
			theToken->charPass(c1Char);
		}
        else
        {
            error(6, "empty string declared at ^1^:^2^", true, theToken);
        }
		getCharPackage(true);
		while(c1 != "\"")
		{
			if(c1 == "\\")
			{
				getCharPackage(true);
				if(c1 == "n")
				{
					c1Char->cargo = "\n";
				}
				theToken->addChar(c1Char);
				getCharPackage(true);
			}
			else
			{
				theToken->addChar(c1Char);
				getCharPackage(true);
			}
			
		}
		theToken->cat = kCat_VALUE;
		theToken->type = kToken_STRING;
		return theToken;

	}

	if(isOperator(c2, theToken->type) && c2.size() == 2)
	{
		getCharPackage();
		theToken->addChar(c1Char);
		theToken->cat = kCat_OPERATOR;
		return theToken;
	}
	if(isOperator(c1, theToken->type))
	{
		theToken->cat = kCat_OPERATOR;
		return theToken;
	}

	theToken->type = kToken_UNKNOWN;
	theToken->cat = kCat_UNKNOWN;
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
			error(1, "Unknown Character at ^1^:^2^: ^0^", false,  theToken);
		}
		if(theToken->type != kToken_COMMENT)
		{
			theV->push_back(theToken);
		}
	}
}


string
printTokenArray(vector<Token*> *v)
{
	string msg = "";

	for (vector<Token*>::iterator it = v->begin(); it != v->end(); it++)
	{

		Token* theToken = *it;

		if (theToken->cat == kCat_TYPE)
		{
			if (theToken->type == kToken_NUMBER)
			{
				if (theToken->subType == kToken_NUMBER8)
					msg += "\n  NUMBER8(TYPE): " + theToken->cargo;
				else if (theToken->subType == kToken_NUMBER16)
					msg += "\n NUMBER16(TYPE): " + theToken->cargo;
				else if (theToken->subType == kToken_NUMBER32)
					msg += "\n NUMBER32(TYPE): " + theToken->cargo;
				else if (theToken->subType == kToken_NUMBER64)
					msg += "\n NUMBER64(TYPE): " + theToken->cargo;
				else if(theToken->subType == kToken_NUMBER_FP)
					msg += "\n NUMBERFP(TYPE): " + theToken->cargo;
				else
					msg += "\n   NUMBER(TYPE): " + theToken->cargo;
			}
			else if (theToken->type == kToken_BOOL)
			{
				msg += "\n     BOOL(TYPE): " + theToken->cargo;
			}
			else if (theToken->type == kToken_STRING)
			{
				msg += "\n   STRING(TYPE): " + theToken->cargo;
			}
			else if(theToken->type == kToken_DYNAMIC)
			{
				msg += "\n  DYNAMIC(TYPE): " + theToken->cargo;
			}
			else if (theToken->type == kToken_UNKNOWN)
			{
				msg += "\n  UNKNOWN(TYPE): " + theToken->cargo;
			}
			else
			{
				msg += "\n  WHAT IS THIS?: " + theToken->cargo;
			}
		}
		else if (theToken->cat == kCat_VALUE)
		{
			if (theToken->type == kToken_NUMBER)
			{
				if (theToken->subType == kToken_NUMBER8)
					msg += "\n NUMBER8(VALUE): " + theToken->cargo;
				else if (theToken->subType == kToken_NUMBER16)
					msg += "\nNUMBER16(VALUE): " + theToken->cargo;
				else if (theToken->subType == kToken_NUMBER32)
					msg += "\nNUMBER32(VALUE): " + theToken->cargo;
				else if (theToken->subType == kToken_NUMBER64)
					msg += "\nNUMBER64(VALUE): " + theToken->cargo;
				else if(theToken->subType == kToken_NUMBER_FP)
					msg += "\nNUMBERFP(VALUE): " + theToken->cargo;
				else
					msg += "\n  NUMBER(VALUE): " + theToken->cargo;

			}
			else if (theToken->type == kToken_BOOL)
			{
				msg += "\n    BOOL(VALUE): " + theToken->cargo;
			}
			else if (theToken->type == kToken_STRING)
			{
				msg += "\n  STRING(VALUE): " + theToken->cargo;
			}
			else
			{
				msg += "\n UNKNOWN(VALUE): " + theToken->cargo;
			}
			
		}
		else if(theToken->cat == kCat_IDENTIFIER)
		{
				msg += "\n     IDENTIFIER: " + theToken->cargo;
		}
		else if (theToken->cat == kCat_EOS)
		{
			if (theToken->type == kToken_SEMICOLON)
			{
				msg += "\n      SEMICOLON: " + theToken->cargo;
			}
			else
			{
				msg += "\n    UNKNOWN_EOS: " + theToken->cargo;
			}
		}
		else if(theToken->cat == kCat_OPERATOR)
		{
				msg += "\n       OPERATOR: " + theToken->cargo;
		}
		else
		{
				msg += "\n    NO CATEGORY: " + theToken->cargo;
		}
	}
	return msg;
}


