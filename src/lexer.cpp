#include "lexer.h"
#include "error.h"
using namespace std;

Lexer::Lexer()
{
	myScanner = new Scanner;
	typeMap.insert(make_pair("num", kToken_NUMBER));
	typeMap.insert(make_pair("num8", kToken_NUMBER8));
	typeMap.insert(make_pair("num16", kToken_NUMBER16));
	typeMap.insert(make_pair("num32", kToken_NUMBER32));
	typeMap.insert(make_pair("num64", kToken_NUMBER64));
	typeMap.insert(make_pair("num128", kToken_NUMBER128));
	typeMap.insert(make_pair("bool", kToken_BOOL));
	typeMap.insert(make_pair("string", kToken_STRING));
	typeMap.insert(make_pair("ptr", kToken_POINTER));
	
	keywordMap.insert(make_pair("print", kToken_PRINT));

	operatorMap.insert(make_pair("=", kToken_EQUALS));

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
	 	   (theChar >= '0' && theChar <= '9');
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
	theId = it->second;
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
	}
	{
		int laAmount = 0;
		if(isType(c2, theToken->type, laAmount))
		{
			for(int i = 0; i < laAmount - 1; i++)
			{
				getCharPackage();
				theToken->addChar(c1Char);
			}
			theToken->cat = kCat_TYPE;
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

		if(isKeyword(theToken->cargo, theToken->type))
		{
			theToken->cat = kCat_KEYWORD;
			return theToken;
		}
		theToken->cat = kCat_IDENTIFIER;
		return theToken;
	}

	if(isNumber(c1))
	{
		bool fp = false;
		if (isNumber(myScanner->lookAhead(1)) || (myScanner->lookAhead(1) == "." && isNumber(myScanner->lookAhead(2))))
		{
			if (c1 == ".")
			{
				fp = true;
			}
			getCharPackage();
			while (isNumber(myScanner->lookAhead(1)) || (!fp && (c1 == "." && isNumber(myScanner->lookAhead(1)))))
			{
				if (c1 == ".")
				{
					fp = true;
				}
				theToken->addChar(c1Char);
				getCharPackage();
			}
		}
		theToken->type = fp ? kToken_NUMBER_FP : kToken_NUMBER;
		theToken->cat = kCat_VALUE;
		return theToken;
	}

	if(c1 == "\"")
	{
		theToken->addChar(c1Char);
		getCharPackage();
		while(c1 != "\"")
		{
			if(c1 == "\0")
				error(5, "String ^0^ opened at ^1^:^2^ doesn't close before end of file");
			theToken->addChar(c1Char);
			getCharPackage();
		}
		theToken->type = kToken_STRING;
		theToken->cat = kCat_VALUE;

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
			error(1, "Unknown Character at ^1^:^2^ == ^0^", theToken);
		}
		theV->push_back(theToken);
	}
}

string
printTokenArray(vector<Token*>* v)
{
	string msg = "";

	for (vector<Token*>::iterator it = v->begin(); it != v->end(); it++)
	{
		Token* theToken = *it;

		if (theToken->cat == kCat_TYPE)
		{
			if (theToken->type == kToken_NUMBER)
			{
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
			else if (theToken->type == kToken_NUMBER8)
			{
				msg += "\n  NUMBER8(TYPE): " + theToken->cargo;
			}
			else if (theToken->type == kToken_NUMBER16)
			{
				msg += "\n NUMBER16(TYPE): " + theToken->cargo;
			}
			else if (theToken->type == kToken_NUMBER32)
			{
				msg += "\n NUMBER32(TYPE): " + theToken->cargo;
			}
			else if (theToken->type == kToken_NUMBER64)
			{
				msg += "\n NUMBER64(TYPE): " + theToken->cargo;
			}
			else if (theToken->type == kToken_NUMBER128)
			{
				msg += "\nNUMBER128(TYPE): " + theToken->cargo;
			}
			else if (theToken->type == kToken_UNKNOWN)
			{
				msg += "\n  UNKNOWN(TYPE): " + theToken->cargo;
			}
		}
	}
	return msg;
}


