#include "parser.h"
using namespace std;

Parser::Parser(const string &fileName, bool fVerbose)
{
    //todo: refactor this not to be in the constructor
    myLexer = new Lexer;
    mVerbose = fVerbose;
    myLexer->init(fileName);
    tokenArray = myLexer->lex();
	itAtEnd = false;
	currentTokenIterator = tokenArray->begin();
	currentToken = *currentTokenIterator;
	mLog = "";
}

void
Parser::fetchToken(unsigned t)
{
    if(!itAtEnd)
    {
        currentTokenIterator += t;
        if(currentTokenIterator >= tokenArray->end())
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

bool
Parser::doesNameNotExist(vector<string> &nameArray, const string &what)
{
    auto finder = find(nameArray.begin(), nameArray.end(), what);
    return finder == nameArray.end();
}


ExpressionStatement*
Parser::parseExpression(vector<Token*>::iterator start, vector<string> theNames, vector<Token*>::iterator *stop)
{
    ExpressionStatement* theStatement = new ExpressionStatement;
    theStatement->mTokenArray.push_back(*start);
    return theStatement;
    //todo: implement this...
    /*
    if(stop != nullptr)
    {
        vector<Token*>::iterator theEnd = *stop;

    }
    else
    {

    }
     */
}


void
Parser::multipleVarInitializations(FuncStatement *theFunc, vector<string> theNames)
{
    //todo: are these ifs arbitrary?
    Token* typePtr = currentToken;
    if(currentToken->cat == kCat_TYPE && tokenLookahead(1)->cat == kCat_IDENTIFIER)
    {
        fetchToken();
        while(true)
        {
            if(currentToken->cat == kCat_IDENTIFIER)
            {
                if(doesNameNotExist(theNames, currentToken->cargo))
                {

                    if(tokenLookahead(1)->type == kToken_EQUALS)
                    {
                        if(tokenLookahead(2)->cat == kCat_VALUE)
                        {
                            if(tokenLookahead(2)->type == typePtr->type)
                            {


                                //todo: this could definitely be turned into a function...
                                unsigned i = 1;
                                while(tokenLookahead(i)->type!=kToken_SEMICOLON ||
                                        tokenLookahead(i)->type!=kToken_COMMA)
                                {
                                    if(tokenLookahead(i)->type==kToken_EOF)
                                    {
                                        //todo: error
                                    }
                                    ++i;
                                }
                                vector<Token*>::iterator* eEnd = new vector<Token*>::iterator;
                                *eEnd = currentTokenIterator
                                        +(i-1); //i-1 removes the unecessary semicolon/comma at the end.
                                ExpressionStatement* theValue;
                                theValue = parseExpression(currentTokenIterator+2, theNames, eEnd);
                                theNames.push_back(currentToken->cargo);
                                VarStatement* theVar = new VarStatement;
                                theVar->mType = typePtr;
                                theVar->mName = currentToken->cargo;
                                theVar->mValue = theValue;
                                theFunc->mStatementArray.push_back(theVar);
                                fetchToken(i);
                            }
                            else
                            {
                                //todo: this...
                            }

                        }
                        else
                        {

                        }
                        //todo: if(equation)
                    }
                    else if(tokenLookahead(1)->type == kToken_COMMA || tokenLookahead(1)->type == kToken_SEMICOLON)
                    {
                        VarStatement* theVar = new VarStatement;
                        theVar->mName = currentToken->cargo;
                        theVar->mType = typePtr;
                        theFunc->mStatementArray.push_back(theVar);
                        fetchToken();
                    }
                    else
                    {
                        //todo: this error
                    }
                }
                else
                {
                    //todo: var already exists error
                }
            }
            else if(currentToken->type == kToken_COMMA)
            {
                fetchToken();
            }
            else if(currentToken->type == kToken_SEMICOLON)
            {
                fetchToken();
                break;
            }
            else
            {
                //todo: weird error
            }
        }

    }
    else
    {
        //todo: implement me...
    }



}


void  //NOTE: theNames can't be a refrence because the deeper we go in scoping we don't want to carry junk back out with us.
Parser::doParseOnFunc(FuncStatement *theFunc, vector<string> theNames)
{
    while(!itAtEnd)
    {
        if(currentTokenIterator == tokenArray->end())
        {
            break;
        }
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
                            VarStatement* theVar = new VarStatement;
                            theVar->mType = currentToken;
                            fetchToken();
                            theVar->mName = currentToken->cargo;
                            fetchToken(2);
                            ExpressionStatement* theStatement = new ExpressionStatement;
                            theStatement->mTokenArray.push_back(
                                    currentToken); //todo: update, but this requires fully implemented expression obj
                            theVar->mValue = theStatement;
                            theFunc->mStatementArray.push_back(theVar);
                            fetchToken(3);
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
                    multipleVarInitializations(theFunc, theNames);
                }
                else
                {
                    //TODO: ELSE CASE
                }
            }
            else if(tokenLookahead(2)->type == kToken_SEMICOLON)
            {
                VarStatement* theVar = new VarStatement;
                theVar->mType = currentToken;
                fetchToken();
                theVar->mName = currentToken->cargo;
                theFunc->mStatementArray.push_back(theVar);
            }
            else if(tokenLookahead(2)->type == kToken_COMMA)
            {
                multipleVarInitializations(theFunc, theNames);
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
}


void
Parser::parse()
{
    FuncStatement* gFunc = new FuncStatement;
    gFunc->mName = "globalScope";
    Token* gType = new Token;
    gType->type = kToken_NUMBER;
    gFunc->mType = gType;
    vector<string> names;
    names.push_back("");

    doParseOnFunc(gFunc, names);

    if(mVerbose)
        mLog = gFunc->print();
    
    
}