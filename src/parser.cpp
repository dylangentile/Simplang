#include "parser.h"
using namespace std;

Parser::Parser()
{
    //whereAmI = 0;
	mVerbose = false;
	itAtEnd = false;
	myLexer = new Lexer;
	currentTokenIterator = tokenArray->begin();
	currentToken = *currentTokenIterator;
}

void
Parser::fetchToken(unsigned t)
{
    if(!itAtEnd)
    {
        currentTokenIterator += t;
        if(currentTokenIterator == tokenArray->end())
         {
            itAtEnd = true;
        }
        currentToken = *currentTokenIterator;
    }
    else
    {
        //todo: redo this error
        error(555, "The parser kept going past end of token array.", currentToken);
    }

}


Token*
Parser::tokenLookahead(unsigned x)
{
    if(currentTokenIterator + x == tokenArray->end())
    {
        //TODO: error
    }
    return *(currentTokenIterator + x);
}

void
Parser::init(const string &fileName, bool fVerbose)
{
	mVerbose = fVerbose;
	myLexer->init(fileName);
}

bool
Parser::doesNameNotExist(vector<string> &nameArray, const string &what)
{
    auto finder = find(nameArray.begin(), nameArray.end(), what);
    return finder == nameArray.end();
}


void
Parser::doParseOnFunc(FuncStatement *theFunc, vector<string> theNames)
{
    if(currentToken->cat == kCat_TYPE && tokenLookahead(1)->cat == kCat_IDENTIFIER)
    {
        //are we initializing a variable
        if(tokenLookahead(2)->type == kToken_EQUALS && tokenLookahead(3)->cat == kCat_VALUE)
        {
            if(tokenLookahead(4)->type == kToken_SEMICOLON)
            {
                if(currentToken->type == tokenLookahead(3)->type)
                {
                    if(doesNameNotExist(theNames, tokenLookahead(1)->cargo))
                    {
                        //todo: generalize this into a function... perhaps?
                        theNames.push_back(tokenLookahead(1)->cargo);
                        VarStatement *theVar = new VarStatement;
                        theVar->mType = currentToken;
                        fetchToken();
                        theVar->mName = currentToken->cargo;
                        fetchToken(2);
                        theVar->mValue = currentToken;
                        fetchToken(2);
                        theFunc->mStatementArray.push_back(theVar);
                    }
                    else
                    {
                        //TODO: this error
                    }
                }
                else
                {
                    //TODO: this error
                }
            }
            else if(tokenLookahead(4)->type == kToken_COMMA)
            {

            }
            else
            {
                //TODO: ELSE CASE
            }
        }
        else if(tokenLookahead(2)->type == kToken_SEMICOLON)
        {

        }
        else if(tokenLookahead(2)->type == kToken_COMMA)
        {

        }
        else if(tokenLookahead(2)->type == kToken_LPAREN)
        {
            //TODO: FUNCTION PARSER
        }
        else
        {
            //TODO: WEIRD ELSE CASE
        }
    }
}


void
Parser::parse()
{
    tokenArray = myLexer->lex();
    FuncStatement* gFunc = new FuncStatement;
    vector<string> names;
    names.push_back("reservedKeyword");
    while(!itAtEnd)
    {
        doParseOnFunc(gFunc, names);
    }


    
    
    
}