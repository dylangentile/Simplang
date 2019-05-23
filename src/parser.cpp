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

	precedenceMap.insert(make_pair(kToken_POWER, 30));
    precedenceMap.insert(make_pair(kToken_MULTIPLY, 20));
    precedenceMap.insert(make_pair(kToken_DIVIDE, 20));
    precedenceMap.insert(make_pair(kToken_ADD, 10));
    precedenceMap.insert(make_pair(kToken_SUBTRACT, 10));
    precedenceMap.insert(make_pair(kToken_MODULO, 5)); //is this okay?





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
Parser::parseExpression(vector<string> theNames)
{
    //Stack<Token>
    ExpressionStatement* theStatement = new ExpressionStatement;


    Token *lookAhead = tokenLookahead(1);

    if (lookAhead->cat == kCat_OPERATOR || currentToken->type == kToken_LPAREN)
    {
        BinExpressionStatement* theTerms = nullptr;
        theTerms = parseBinExpression(theNames, nullptr);
        fetchToken(); //go past last value to non operator char.
        return dynamic_cast<ExpressionStatement*>(theTerms);

    }





}


unsigned int
Parser::getPrecedence(Token* x)
{
    auto it = precedenceMap.find(x->type);
    if(it == precedenceMap.end())
    {
        //todo error -- passed a tokenid without a precedence
    }
    return it->second;
}

BinExpressionStatement*
Parser::parseBinExpression(vector<string> theNames, Token* prevOp, BinExpressionStatement *theTerms)
{
    
    
    if(theTerms == nullptr)
    {
        theTerms = new BinExpressionStatement;
    }

    while(true)
    {
        bool idP = true;
        Token* idToken = currentToken;
        if(idToken->type == kToken_LPAREN)
        {
            fetchToken();
            continue;
        }

        if(idToken->type == kToken_RPAREN)
        {
            idP = false;
        }

        Token* opToken = tokenLookahead(1);
        if(opToken->cat != kCat_OPERATOR)
            break;
        else if(opToken->type == kToken_RPAREN)
        {
            fetchToken();
            break;
        }
        Token* nextIdToken = tokenLookahead(2);
        Token* nextOpToken = tokenLookahead(3);

        if(nextIdToken->type == kToken_LPAREN)
        {
            OperatorStatement *theOp = new OperatorStatement;
            theOp->insertOp(opToken);
            ValueStatement *theId = new ValueStatement;
            theId->mValue = idToken;
            if(idP)
                theTerms->mTermVector.push_back(theId);
            fetchToken(2);
            parseBinExpression(theNames, nullptr, theTerms);
            theTerms->mTermVector.push_back(theOp);
            prevOp = opToken;
            continue;
        }

        if(nextOpToken->cat != kCat_OPERATOR || nextOpToken->type == kToken_RPAREN)
        {
            if(prevOp == nullptr || getPrecedence(prevOp) < getPrecedence(opToken))
            {
                ValueStatement *nextId = new ValueStatement,*theId = new ValueStatement;
                theId->mValue = idToken;
                nextId->mValue = nextIdToken;
                OperatorStatement *theOp = new OperatorStatement;
                theOp->insertOp(opToken);
                if(idP)
                    theTerms->mTermVector.push_back(theId);
                theTerms->mTermVector.push_back(nextId);
                theTerms->mTermVector.push_back(theOp);
                
                //cout << " " << idToken->cargo << " " << nextIdToken->cargo << " " << opToken->cargo;
            }
            else
            {
                ValueStatement *nextId = new ValueStatement;
                nextId->mValue = nextIdToken;
                OperatorStatement *theOp = new OperatorStatement;
                theOp->insertOp(opToken);
                
                theTerms->mTermVector.push_back(nextId);
                theTerms->mTermVector.push_back(theOp);
                //cout << " " << nextIdToken->cargo << " " << opToken->cargo;
            }
            if(nextOpToken->type == kToken_RPAREN)
            {
                fetchToken(3);
                continue;
            }
            else
                fetchToken(2);

            break;
        }

        if(prevOp == nullptr)
        {

            if(getPrecedence(nextOpToken) > getPrecedence(opToken))
            {
                ValueStatement *theId = new ValueStatement;
                theId->mValue = idToken;
                if(idP)
                    theTerms->mTermVector.push_back(theId);
                //cout << " " << idToken->cargo;
                fetchToken(2);
                parseBinExpression(theNames, opToken, theTerms);
                OperatorStatement* theOp = new OperatorStatement;
                theOp->insertOp(opToken);
                //cout << " " << opToken->cargo;
            }
            else
            {
                ValueStatement *nextId = new ValueStatement,*theId = new ValueStatement;
                theId->mValue = idToken;
                nextId->mValue = nextIdToken;
                OperatorStatement *theOp = new OperatorStatement;
                theOp->insertOp(opToken);
                if(idP)
                    theTerms->mTermVector.push_back(theId);
                theTerms->mTermVector.push_back(nextId);
                theTerms->mTermVector.push_back(theOp);
                
                //cout << " " << idToken->cargo << " " << nextIdToken->cargo << " " << opToken->cargo;
                fetchToken(2);
            }

        }
        else
        {
            if(getPrecedence(nextOpToken) > getPrecedence(opToken))
            {

                if(getPrecedence(prevOp) < getPrecedence(opToken))
                {
                    ValueStatement *theId = new ValueStatement;
                    theId->mValue = idToken;
                    if(idP)
                        theTerms->mTermVector.push_back(theId);
                    //cout << " " << idToken->cargo;
                }
               fetchToken(2);
               
               parseBinExpression(theNames, opToken, theTerms);
               OperatorStatement *theOp = new OperatorStatement;
               theOp->insertOp(opToken);
               theTerms->mTermVector.push_back(theOp);


            }
            else
            {
                if(getPrecedence(prevOp) == getPrecedence(opToken))
                {
                    ValueStatement *nextId = new ValueStatement;
                    nextId->mValue = nextIdToken;
                    OperatorStatement *theOp = new OperatorStatement;
                    theOp->insertOp(opToken);
                    
                    theTerms->mTermVector.push_back(nextId);
                    theTerms->mTermVector.push_back(theOp);
                    //cout << " " << nextIdToken->cargo << " " << opToken->cargo;
                }
                else if(getPrecedence(prevOp) < getPrecedence(opToken))
                {
                    ValueStatement *nextId = new ValueStatement,*theId = new ValueStatement;
                    theId->mValue = idToken;
                    nextId->mValue = nextIdToken;
                    OperatorStatement *theOp = new OperatorStatement;
                    theOp->insertOp(opToken);
                    if(idP)
                        theTerms->mTermVector.push_back(theId);
                    theTerms->mTermVector.push_back(nextId);
                    theTerms->mTermVector.push_back(theOp);
                    //cout << " " << idToken->cargo << " " << nextIdToken->cargo << " " << opToken->cargo;
                }
                else
                {
                    ValueStatement *nextId = new ValueStatement;
                    nextId->mValue = nextIdToken;
                    OperatorStatement* theOp = new OperatorStatement;
                    theOp->insertOp(opToken);
                    theTerms->mTermVector.push_back(nextId);
                    theTerms->mTermVector.push_back(theOp);
                    //cout << " " << nextIdToken->cargo << " " << opToken->cargo;
                }
                fetchToken(2);
            }

        }


        prevOp = opToken;



    }


    return theTerms;

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
                        if(tokenLookahead(2)->cat == kCat_VALUE || tokenLookahead(2)->cat == kCat_IDENTIFIER || tokenLookahead(2)->type == kToken_LPAREN)
                        {
                            if(tokenLookahead(2)->type == kToken_LPAREN || tokenLookahead(2)->type == typePtr->type)
                            {
                                Token *theID = currentToken;
                                ExpressionStatement* theValue;
                                fetchToken(2);
                                theValue = parseExpression(theNames);
                                theNames.push_back(theID->cargo);
                                VarStatement* theVar = new VarStatement;
                                theVar->mType = typePtr;
                                theVar->mName = theID->cargo;
                                theVar->mValue = theValue;
                                theFunc->mStatementArray.push_back(theVar);
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
                        theVar->mValue = nullptr;
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
                theVar->mValue = nullptr;
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