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

    precedenceMap.insert(make_pair(kToken_MULTIPLY, 20));
    precedenceMap.insert(make_pair(kToken_DIVIDE, 20));
    precedenceMap.insert(make_pair(kToken_ADD, 10));
    precedenceMap.insert(make_pair(kToken_MULTIPLY, 10));




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

#include <iostream>

ExpressionStatement*
Parser::parseExpression(vector<Token*>::iterator start, vector<string> theNames, vector<Token*>::iterator *stop)
{
    //Stack<Token>
    ExpressionStatement* theStatement = new ExpressionStatement;


    Token *lookAhead = tokenLookahead(1);

    if (lookAhead->cat == kCat_OPERATOR)
    {
        bool *isEmpty = new bool;
        *isEmpty = true;
        parseBinExpression(theNames, isEmpty);


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


ExpressionStatement*
Parser::parseBinExpression(vector<string> theNames, bool *empty)
{


    //cout << "parseBinExpression" << endl;
    //while(true == false)
    {
        unsigned int opPrecendence = 0, opPrecendenceNext = 0;

        map<TokenID, unsigned int>::iterator opIterator;


        while(true)
        {

            Token* idToken1 = currentToken;
            Token* opToken = tokenLookahead(1);
            Token* idToken2 = tokenLookahead(2);
            Token *opTokenNext;


            opIterator = precedenceMap.find(opToken->type);
            if(opIterator != precedenceMap.end())
                opPrecendence = opIterator->second;
            else
            {
                //todo: error
            }



            opTokenNext = tokenLookahead(3);
            if(opTokenNext->type == kToken_SEMICOLON || opTokenNext->type == kToken_COMMA)
            {
                cout << " " << idToken2->cargo << " " << opToken->cargo;
                break;
            }


            if (opTokenNext->cat == kCat_OPERATOR)
            {
                opIterator = precedenceMap.find(opTokenNext->type);
                if(opIterator != precedenceMap.end())
                    opPrecendenceNext = opIterator->second;
                else
                {
                    //todo
                }

                if (opPrecendenceNext > opPrecendence)
                {
                    fetchToken(2);
                    parseBinExpression(theNames, empty);
                    cout << " " << idToken1->cargo << " " << opToken->cargo;
                    if(tokenLookahead(3)->type == kToken_SEMICOLON || tokenLookahead(3)->type == kToken_COMMA)
                    {
                        break;
                    }
                }
                else
                {
                    fetchToken(2);


                    if (*empty == false)
                    {

                        cout << " " << idToken2->cargo << " " << opToken->cargo;
                    }
                    else
                    {
                        *empty = false;
                        cout << idToken1->cargo << " " << idToken2->cargo << " " << opToken->cargo;
                    }
                }
            }


        }





        /*


        fetchToken(2);

        Token* nextOp = tokenLookahead(1);

        unsigned int op1P = opPrecedence->second;

        opPrecedence = precedenceMap.find(nextOp->type);

        if(opPrecedence == precedenceMap.end())
        {
            //todo: error
        }

        unsigned int op2P = opPrecedence->second;

        if(op1P < op2P)
        {
            parseBinExpression(theNames, empty);
            cout << " " << idToken1->cargo << " " << opToken->cargo;
        }
        else
        {
            if(*empty)
                cout << idToken1->cargo;
            *empty = false;
            cout << " " << idToken2->cargo << " " << opToken->cargo;
        }
        if(currentToken->type == kToken_SEMICOLON || currentToken->type == kToken_COMMA)
        {
            cout << " " << idToken2->cargo << " " << opToken->cargo;
            return nullptr;
        }

        if(tokenLookahead(1)->cat == kCat_OPERATOR)
        {
            auto pr = precedenceMap.find(tokenLookahead(1)->type);
            if(pr->second == op1P)
            {
                parseBinExpression(theNames, empty);
            }

        }*/

        cout << endl;
    }
    /*
    Token* idToken1 = currentToken;
    Token* opToken1 = tokenLookahead(1);
    Token* idToken2 = tokenLookahead(2);
    Token* opToken2 = tokenLookahead(3);

    auto sit = precedenceMap.find(opToken1->type);
    auto bit = precedenceMap.find(opToken2->type);
    unsigned int *p1 = new unsigned int, *p2 = new unsigned int;
    *p1 = sit->second;
    *p2 = bit->second;

    if(opToken2->type == kToken_SEMICOLON)
    {
        cout << idToken1->cargo << " " << opToken1->cargo << idToken2->cargo;
    }

    while(*p1 > *p2)
    {

    }
     */





    /*
    else if (opToken->type == kToken_MULTIPLY)
    {

  //      printf("%s %s *", idToken1->cargo.c_str(), idToken2->cargo.c_str());
        cout << idToken1->cargo << " " << idToken2->cargo << " * ";

    }
     */






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
                                cout << endl;
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