#include "parser.h"

using namespace std;

Parser::Parser()
{
	mVerbose = false;
	myLexer = new Lexer;
}

Parser::~Parser()
{

}


void
Parser::init(string fileName, bool fVerbose)
{
	mVerbose = fVerbose;
	myLexer->init(fileName);

}


void
Parser::parse()
{

}