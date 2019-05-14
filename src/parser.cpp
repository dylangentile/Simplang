#include "parser.h"

using namespace std;

Parser::Parser()
{
	mVerbose = false;
	itAtEnd = false;
	myLexer = new Lexer;
	currentToken = tokenArray->begin();
}

Parser::~Parser()
{
    whereAmI = 0;
}
void
Parser::fetchToken()
{
    if(!itAtEnd)
    {
        ++currentTokenIterator;
        if(currentTokenIterator == tokenArray->end())
         {
            itAtEnd = true;
        }
        currentToken = *currentTokenIterator;
    }
    else
    {
    error(555, "The parser kept going past end of token array.", currenToken);
    }

}

void
Parser::init(string fileName, bool fVerbose)
{
	mVerbose = fVerbose;
	myLexer->init(fileName);
}

void
Parser::ParseOnFunc(FuncStatement *theFunc)
{
    
}


void
Parser::parse()
{
    tokenArray = myLexer->lex();
    FuncStatement* gFunc = new FuncStatement;
    while(!itAtEnd)
    {
        ParseOnFunc(gFunc);
    }

    
    
    
}