#include "parser.h"
#include <stack>
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
        if(currentTokenIterator == tokenArray->end())
			itAtEnd = true;
		else
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
		error(5, "Reached EOF before end of statement", true, currentToken);
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
    //Stack<Token>
    ExpressionStatement* theStatement = new ExpressionStatement;


    Token *lookAhead = tokenLookahead(1);

    if (lookAhead->cat == kCat_OPERATOR)
    {

        parseBinExpression(start, theNames, stop);


    }





    if(stop != nullptr)
    {
        vector<Token*>::iterator theEnd = *stop;

    }
    else
    {

    }



    return nullptr;

}

#include <iostream>

ExpressionStatement*
Parser::parseBinExpression(vector<Token*>::iterator start, vector<string> theNames, vector<Token*>::iterator *stop)
{


    cout << "parseBinExpression" << endl;

    Token *idToken1 = currentToken;
    Token *opToken = tokenLookahead(1);
    Token *idToken2 = tokenLookahead(2);
    if (opToken->type == kToken_ADD)
    {

        fetchToken();
        //Token * opToken =  currentToken;

        //verify that this is a valid token for the add operator
        Token *nextIDToken = tokenLookahead(1);


        Token *nextToken = tokenLookahead(2);
        //todo: is this case necessary?
        if (nextToken->cat == kCat_OPERATOR && nextToken->type == kToken_MULTIPLY)
        {
            fetchToken();
            parseBinExpression(start, theNames, stop);


            cout << idToken1->cargo << " + " ;
//            printf("%s %s *", idToken1->cargo.c_str(), idToken2->cargo.c_str());


        }
        else
        {
            cout << idToken1->cargo << " " << idToken2->cargo << " + ";
//            printf("%s %s +\n", idToken1->cargo.c_str(), idToken2->cargo.c_str());

        }



    }
    else if (opToken->type == kToken_MULTIPLY)
    {

  //      printf("%s %s *", idToken1->cargo.c_str(), idToken2->cargo.c_str());
        cout << idToken1->cargo << " " << idToken2->cargo << " * ";

    }



    cout << endl;


    return nullptr;

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
                        if(tokenLookahead(2)->cat == kCat_VALUE /*|| tokenLookahead(2)->cat == kCat_IDENTIFIER*/)
                        {
                            if(tokenLookahead(2)->type == typePtr->type)
                            {


                                //todo: this could definitely be turned into a function...
                                unsigned i = 1;
                                while(tokenLookahead(i)->type!=kToken_SEMICOLON &&
                                        tokenLookahead(i)->type!=kToken_COMMA)
                                {
                                    if(tokenLookahead(i)->type==kToken_EOF)
                                    {
										error(5, "Reached EOF before the end of commaed initializations.", true, tokenLookahead(i));
                                    }
                                    ++i;
                                }
                                vector<Token*>::iterator* eEnd = new vector<Token*>::iterator;
                                *eEnd = currentTokenIterator
                                        +(i-1); //i-1 removes the unecessary semicolon/comma at the end.
                                ExpressionStatement* theValue;
                                fetchToken(2); //todo:  remove me
                                theValue = parseExpression(currentTokenIterator, theNames, eEnd);
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
								error(5, "Type at: " + to_string(*typePtr->lineIndex + 1) + ":" + to_string(*typePtr->colIndex + 1) + ", doesnt match type of value of '^0^' at ^1^:^2^", false, tokenLookahead(1));
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
					error(6, "Identifier '^0^' at ^1^:^2^ has already been used!", false, currentToken);
                }
            }
            else if(currentToken->type == kToken_COMMA)
            {
                fetchToken();
            }
            else if(currentToken->type == kToken_SEMICOLON)
            {
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
                            //theStatement->mTokenArray.push_back(
                              //      currentToken); //todo: update, but this requires fully implemented expression obj
                            theVar->mValue = theStatement;
                            theFunc->mStatementArray.push_back(theVar);
                            fetchToken();
                        }
                        else
                        {
							error(6, "Identifier '^0^' at ^1^:^2^ has already been used!", false, tokenLookahead(1));
                        }
                    }
                    else
                    {
						error(6, "Type at: " + to_string(*currentToken->lineIndex + 1) + ":" + to_string(*currentToken->colIndex + 1) + ", doesnt match type of value of '^0^' at ^1^:^2^", false, tokenLookahead(3));

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
		fetchToken();
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