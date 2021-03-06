#include "parser.h"
//#include "data.h"
#include <iostream>
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
	precedenceMap.insert(make_pair(kToken_MODULO, 7)); //is this okay?
	precedenceMap.insert(make_pair(kToken_EQUALITY, 5));
	precedenceMap.insert(make_pair(kToken_LESS, 5));
	precedenceMap.insert(make_pair(kToken_GREATER, 5));
	precedenceMap.insert(make_pair(kToken_NOT, 6));
	precedenceMap.insert(make_pair(kToken_LESS_EQUAL, 5));
	precedenceMap.insert(make_pair(kToken_GREATER_EQUAL, 5));
	precedenceMap.insert(make_pair(kToken_NOT_EQUAL, 5));
	precedenceMap.insert(make_pair(kToken_LOGIC_AND, 4));
	precedenceMap.insert(make_pair(kToken_LOGIC_OR, 3));






}

Parser::~Parser()
{

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

Identifier*
Parser::doesNameNotExist(vector<Identifier*> &nameArray, Token *what)
{
	vector<Identifier*> v = nameArray;
	for(vector<Identifier*>::iterator it = v.begin(); it != v.end(); ++it)
	{
		if((*it)->mName->cargo == what->cargo)
		{
			Identifier *n = *it;
			return n;
		}
	}
	return nullptr;
}

//#include <iostream>

vector<Statement*>
Parser::parseFuncArgs(vector<Identifier*> theNames)
{
	vector<Statement*> retVec;
	for(;currentToken->type != kToken_RPAREN; fetchToken())
	{
		if(currentToken->cat == kCat_IDENTIFIER || currentToken->cat == kCat_VALUE)
		{
			ExpressionStatement* myVal;
			myVal = parseExpression(theNames, kToken_NULL);
			retVec.push_back(dynamic_cast<Statement*>(myVal));
            if(currentToken->type == kToken_RPAREN)
                break;
		}
		else if(currentToken->cat == kCat_VALUE)
		{
			
		}
		else
		{
			if(currentToken->type != kToken_COMMA)
				error(6, "passing function unknown token '^0^' at ^1^:^2^!", false, currentToken);
		}
	}


	return retVec;
}


Token*
Parser::checkValueRefs(BinExpressionStatement* myExpr, vector<Identifier*>& theNames)
{
	//if(theExpr->fetchId() == kState_BINEXPR)
	{
		//BinExpressionStatement* myExpr = dynamic_cast<BinExpressionStatement*>(theExpr);
		for(vector<Statement*>::iterator it = myExpr->mTermVector.begin(); it != myExpr->mTermVector.end(); ++it)
		{
			Statement* currentStatement = *it;

			if(currentStatement->fetchId() == kState_VALUE_REFRENCE)
			{
				ValueRefrenceStatement* valRef = dynamic_cast<ValueRefrenceStatement*>(currentStatement);
				if(doesNameNotExist(theNames, valRef->refName) == nullptr)
					return valRef->refName;
			}
		}
	}
	//else
	{
		//return nullptr;
		//todo: blah blah
	}

	return nullptr;
}

ExpressionStatement*
Parser::parseExpression(vector<Identifier*> theNames, TokenID type)
{
	//Stack<Token>
	//ExpressionStatement* theStatement = new ExpressionStatement;


	Token *lookAhead = tokenLookahead(1);

	if ((lookAhead->cat == kCat_OPERATOR || currentToken->type == kToken_LPAREN) && lookAhead->type != kToken_RPAREN)
	{
		BinExpressionStatement* theTerms = nullptr;
		theTerms = parseBinExpression(theNames, type, nullptr);
		fetchToken(); //go past last value to non operator char.
		Token* test = checkValueRefs(theTerms, theNames);
		if(test != nullptr)
		{
			error(6, "Undeclared variable '^0^' being using in binary expression.", true, test);
		}
		return dynamic_cast<ExpressionStatement*>(theTerms);

	}
	else if(lookAhead->cat == kCat_EOS || lookAhead->type == kToken_COMMA || lookAhead->type == kToken_RPAREN)
	{
		BinExpressionStatement* theTerms = new BinExpressionStatement;
        Statement* myVal = makeBinExprTerm(currentToken);
		theTerms->mTermVector.push_back(myVal);
		fetchToken();
		Token* test = checkValueRefs(theTerms, theNames);
		if(test != nullptr)
		{
			error(6, "Undeclared variable '^0^' being using in binary expression.", true, test);
		}
		return dynamic_cast<ExpressionStatement*>(theTerms);
		
		
	}

	return nullptr;





}


unsigned int
Parser::getPrecedence(Token* x)
{
	auto it = precedenceMap.find(x->type);
	if(it == precedenceMap.end())
	{
		error(6, "Passed non-operator character '^0^' into expression statement", true, x);
	}
	return it->second;
}


Statement* 
Parser::makeBinExprTerm(Token* outOfMe)
{
	if(outOfMe->cat == kCat_IDENTIFIER)
	{
		ValueRefrenceStatement* me = new ValueRefrenceStatement;
		me->refName = outOfMe;
		return dynamic_cast<Statement*>(me);
	}
	else
	{
		ValueStatement* me = new ValueStatement;
		me->mValue = outOfMe;
		return dynamic_cast<Statement*>(me);
	}
	return nullptr;
}

BinExpressionStatement*
Parser::parseBinExpression(vector<Identifier*> theNames, TokenID type, Token* prevOp, BinExpressionStatement *theTerms)
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
		if(idToken->cat == kCat_IDENTIFIER)
		{
			Identifier *memOf = nullptr;
			memOf = doesNameNotExist(theNames, idToken);
			if(memOf == nullptr)
				error(6, "Non existent identifier: '^0^'", true, idToken);
			else
			{

				if(memOf->mFunc != nullptr)
				{
					//if void function... check type
					//todo: parse arguements
				}
				else if(memOf->mValue == nullptr)
				{
					//todo fix this!
					warning(6, "Uninitialized identifier: '^0^' being passed into expression!", idToken);
				}

				if(memOf->mType != type && type != kToken_NULL)
				{
					//todo: should i be this harsh? or should I let it keep going???? Maybe just have some default castings in the interpreter/compiler w/ a warning instead?
					error(6, "Unmatching types between identifier '^0^' and the type of the expression's assignment!",true, idToken);
				}
			}
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
		if(nextIdToken->cat == kCat_IDENTIFIER)
		{
			Identifier *memOf = nullptr;
			memOf = doesNameNotExist(theNames, nextIdToken);
			if(memOf == nullptr)
				error(6, "Non existent identifier: '^0^'", true, nextIdToken);
			else
			{

				if(memOf->mFunc != nullptr)
				{
					//if void function... check type
					//todo: parse arguements
				}
				else if(memOf->mValue == nullptr)
				{
					warning(6, "Uninitialized identifier: '^0^' being passed into expression!", nextIdToken);
				}

				if(memOf->mType != type && type != kToken_NULL)
				{
					//todo: should i be this harsh? or should I let it keep going???? Maybe just have some default castings in the interpreter/compiler w/ a warning instead?
					error(6, "Unmatching types between identifier '^0^' and the type of the expression's assignment!",
							true, nextIdToken);
				}
			}




		}
		Token* nextOpToken = tokenLookahead(3);

		if(nextIdToken->type == kToken_LPAREN)
		{
			OperatorStatement *theOp = new OperatorStatement;
			theOp->insertOp(opToken);
			if(idP)
				theTerms->mTermVector.push_back(makeBinExprTerm(idToken));
			fetchToken(2);
			parseBinExpression(theNames, type, nullptr, theTerms);
			theTerms->mTermVector.push_back(theOp);
			prevOp = opToken;
			continue;
		}

		if(nextOpToken->cat != kCat_OPERATOR || nextOpToken->type == kToken_RPAREN)
		{
			if(prevOp == nullptr || getPrecedence(prevOp) < getPrecedence(opToken))
			{
				OperatorStatement *theOp = new OperatorStatement;												
				theOp->insertOp(opToken);
				
				if(idP)
					theTerms->mTermVector.push_back(makeBinExprTerm(idToken));

				theTerms->mTermVector.push_back(makeBinExprTerm(nextIdToken));
				theTerms->mTermVector.push_back(theOp);
				
				//cout << " " << idToken->cargo << " " << nextIdToken->cargo << " " << opToken->cargo;
			}
			else
			{
				OperatorStatement *theOp = new OperatorStatement;
				theOp->insertOp(opToken);
				theTerms->mTermVector.push_back(makeBinExprTerm(nextIdToken));
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
            if(idToken->type == kToken_RPAREN)
            {
                break;
            }
			if(getPrecedence(nextOpToken) > getPrecedence(opToken))
			{
				if(idP)
					theTerms->mTermVector.push_back(makeBinExprTerm(idToken));
				
				//cout << " " << idToken->cargo;
				fetchToken(2);
				parseBinExpression(theNames, type, opToken, theTerms);
				OperatorStatement* theOp = new OperatorStatement;
				theOp->insertOp(opToken);
				theTerms->mTermVector.push_back(theOp);
				//cout << " " << opToken->cargo;
			}
			else
			{
				OperatorStatement *theOp = new OperatorStatement;
				theOp->insertOp(opToken);
				
				if(idP)
					theTerms->mTermVector.push_back(makeBinExprTerm(idToken));
				theTerms->mTermVector.push_back(makeBinExprTerm(nextIdToken));
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
					if(idP)
						theTerms->mTermVector.push_back(makeBinExprTerm(idToken));
					//cout << " " << idToken->cargo;
				}
			   fetchToken(2);
			   
			   parseBinExpression(theNames, type, opToken, theTerms);
			   OperatorStatement *theOp = new OperatorStatement;
			   theOp->insertOp(opToken);
			   theTerms->mTermVector.push_back(theOp);
			}
			else
			{
				if(getPrecedence(prevOp) == getPrecedence(opToken))
				{
					OperatorStatement *theOp = new OperatorStatement;
					theOp->insertOp(opToken);
					
					theTerms->mTermVector.push_back(makeBinExprTerm(nextIdToken));
					theTerms->mTermVector.push_back(theOp);
					//cout << " " << nextIdToken->cargo << " " << opToken->cargo;
				}
				else if(getPrecedence(prevOp) < getPrecedence(opToken))
				{
					
					OperatorStatement *theOp = new OperatorStatement;
					theOp->insertOp(opToken);

					if(idP)
						theTerms->mTermVector.push_back(makeBinExprTerm(idToken));
					theTerms->mTermVector.push_back(makeBinExprTerm(nextIdToken));
					theTerms->mTermVector.push_back(theOp);
					//cout << " " << idToken->cargo << " " << nextIdToken->cargo << " " << opToken->cargo;
				}
				else
				{
					OperatorStatement* theOp = new OperatorStatement;
					theOp->insertOp(opToken);

					theTerms->mTermVector.push_back(makeBinExprTerm(nextIdToken));
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
Parser::multipleVarInitializations(FuncStatement *theFunc, vector<Identifier*> &theNames)
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
				if(doesNameNotExist(theNames, currentToken) == nullptr)
				{

					if(tokenLookahead(1)->type == kToken_EQUALS)
					{
						if(tokenLookahead(2)->cat == kCat_VALUE || tokenLookahead(2)->cat == kCat_IDENTIFIER || tokenLookahead(2)->type == kToken_LPAREN)
						{
							//if(tokenLookahead(2)->type == kToken_LPAREN || tokenLookahead(2)->type == typePtr->type)
								//if(tokenLookahead(3)->type == kToken_SEMICOLON)
								{
									Token *theID = currentToken;
									ExpressionStatement* theValue;
									fetchToken(2);
									theValue = parseExpression(theNames, typePtr->type);
									VarStatement* theVar = new VarStatement;
									theVar->mType = typePtr;
									theVar->mName = theID->cargo;
									theVar->mValue = theValue;
									theFunc->mStatementArray.push_back(theVar);
									Identifier *myId = new Identifier;
									myId->mValue = theValue;
									myId->mName = theID;
									myId->mType = typePtr->type;
									theNames.push_back(myId);
								}
							
							/*else
							{
								error(5, "Type at: " + to_string(*typePtr->lineIndex + 1) + ":" + to_string(*typePtr->colIndex + 1) + ", doesnt match type of value of '^0^' at ^1^:^2^", true, tokenLookahead(1));
							} */

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
						Identifier *myId = new Identifier;
						myId->mName = currentToken;
						myId->mType = typePtr->type;
						theNames.push_back(myId);
						fetchToken();
					}
					else
					{
						error(6, "Identifier initialization is followed by invalid token ^0^ at ^1^:^2^", true, tokenLookahead(1));
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



vector<Statement*> 
Parser::createScopedStatementVector(vector<Identifier*> theNames)
{

	
	if(currentToken->type == kToken_LCURLY)
	{
		fetchToken();
		
		//todo: you can probably make a function out of this
		vector<Token*> newTokenArray, *oldTokenArray;
		
		int curlys = 1;
		while(curlys != 0)
		{
			if(currentToken->type == kToken_LCURLY)
			{
				curlys++;
			}
			else if(currentToken->type == kToken_RCURLY)
			{
				curlys--;
			}

			newTokenArray.push_back(currentToken);
			fetchToken();

		}
		
		auto toBeErased = newTokenArray.end() - 1;
		newTokenArray.erase(toBeErased);
		
		vector<Token*>::iterator oldCurrentIterator = currentTokenIterator;
		oldTokenArray = tokenArray;
		tokenArray = &newTokenArray; //this should be safe...
		currentTokenIterator = tokenArray->begin();
		currentToken = *currentTokenIterator;

		FuncStatement* innerIf = new FuncStatement;
		innerIf->mName = "innerIf"; //todo: make thisbetter
		Token* fType = new Token;
		fType->type = kToken_VOID;
		innerIf->mType = fType;
		doParseOnFunc(innerIf, theNames);

		//IfStatement* myIf = new IfStatement();
		

		tokenArray = oldTokenArray;
		currentTokenIterator = oldCurrentIterator;
		currentToken = *currentTokenIterator;

		vector<Statement*> theRet = innerIf->mStatementArray;
		itAtEnd = false;
		delete innerIf;
		return theRet;

	}
	error(7, "sicko mobamba", true);
	
	
}

void  //NOTE: theNames can't be a refrence because the deeper we go in scoping we don't want to carry junk back out with us.
Parser::doParseOnFunc(FuncStatement *theFunc, vector<Identifier*> theNames)
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
			if(tokenLookahead(2)->type == kToken_EQUALS && (tokenLookahead(3)->cat == kCat_VALUE || tokenLookahead(3)->cat == kCat_IDENTIFIER))
			{
				
				multipleVarInitializations(theFunc, theNames);
				
			}
			else if(tokenLookahead(2)->type == kToken_SEMICOLON)
			{
				if(doesNameNotExist(theNames, tokenLookahead(1)) == nullptr)
				{
					VarStatement* theVar = new VarStatement;
					theVar->mType = currentToken;
					fetchToken();
					theVar->mName = tokenLookahead(1)->cargo;
					theVar->mValue = nullptr;

					theFunc->mStatementArray.push_back(theVar);
					Identifier* myId = new Identifier;
					myId->mName = tokenLookahead(1);
					myId->mType = currentToken->type;
					theNames.push_back(myId);
				}
				else
				{
					error(6, "Identifier '^0^' at ^1^:^2^ has already been used!", false, tokenLookahead(1));
				}
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
		else if(currentToken->cat == kCat_IDENTIFIER)
		{
			Identifier* refrencing = doesNameNotExist(theNames, currentToken);
			if(refrencing != nullptr)
			{
				//todo check for function
				ValueRefrenceStatement* theref = new ValueRefrenceStatement;
				theref->refName = currentToken;
				ExpressionStatement* theValue;
				Token* theOperator = tokenLookahead(1);
				fetchToken(2);
				theValue = parseExpression(theNames, refrencing->mType);
				
				


				if(	theOperator->type == kToken_PLUS_EQUALS 	|| 
					theOperator->type == kToken_MINUS_EQUALS 	|| 
					theOperator->type == kToken_MULTIPLY_EQUALS	|| 
					theOperator->type == kToken_DIVIDE_EQUALS 	|| 
					theOperator->type == kToken_POWER_EQUALS 	|| 
					theOperator->type == kToken_MOD_EQUALS
				)
				{
					BinExpressionStatement* theBinExpr = dynamic_cast<BinExpressionStatement*>(theValue);
					ValueRefrenceStatement* anotherRef = new ValueRefrenceStatement;
					anotherRef->refName = theref->refName;
					
					OperatorStatement* myOp = new OperatorStatement;
					switch(theOperator->type)
					{
						case kToken_PLUS_EQUALS: myOp->mValue = kToken_ADD;
						break;
						case kToken_MINUS_EQUALS: myOp->mValue = kToken_SUBTRACT;
						break;
						case kToken_MULTIPLY_EQUALS: myOp->mValue = kToken_MULTIPLY;
						break;
						case kToken_DIVIDE_EQUALS: myOp->mValue = kToken_DIVIDE;
						break;
						case kToken_POWER_EQUALS: myOp->mValue = kToken_POWER;
						break;
						case kToken_MOD_EQUALS: myOp->mValue = kToken_MODULO;
						break;
						
					}
					
					myOp->ogToken = theOperator;					
					theBinExpr->mTermVector.insert(theBinExpr->mTermVector.begin(), anotherRef);
					theBinExpr->mTermVector.push_back(myOp);

					theValue = dynamic_cast<ExpressionStatement*>(theBinExpr);
				}

				theref->mExpr = theValue;
				theFunc->mStatementArray.push_back(theref);
				refrencing->mValue = theValue;



			}
			else
			{
				error(6, "Identifier '^0^' at ^1^:^2^ has not been declared!", true, currentToken);
			}
		}
		else if(currentToken->cat == kCat_KEYWORD)
		{
			if(currentToken->type == kToken_PRINT)
			{
				if(tokenLookahead(1)->type == kToken_LPAREN)
				{
					fetchToken(2);
					SpecializedFunctionCall* myPrinter = new SpecializedFunctionCall(kFunc_PRINT);
					myPrinter->mArgArray = parseFuncArgs(theNames);
					theFunc->mStatementArray.push_back(myPrinter);
					fetchToken();
					if(currentToken->type != kToken_SEMICOLON)
					{
						error(6, "Fuction Call at ^1^:^2^ is missing a ending semicolon!", true, currentToken);
					}
				}
				else
				{
					error(6, "Keyword 'print' followed by unknown token '^0^' at ^1^:^2^. Was expecting '('!", true, tokenLookahead(1));
				}
			}
			else if(currentToken->type == kToken_IF)
			{
				fetchToken();
				if(currentToken->type == kToken_LPAREN)
				{
					fetchToken();
					ExpressionStatement* comparsion = parseExpression(theNames, kToken_NULL);
					if(currentToken->type != kToken_RPAREN)
					{
						//todo: throw error!
					}

					//fetchToken();
					if(currentToken->type == kToken_LCURLY)
					{
						
						

						IfStatement* myIf = new IfStatement;

						myIf->ifBody = createScopedStatementVector(theNames);
						myIf->condition = comparsion;
						theFunc->mStatementArray.push_back(myIf);
						continue;
						//the createScopedStatementVector places currentToken on the token after the closing right curly

					}
					else
					{
						//todo: do only one line.
					}



				}
				else
				{
					//todo: this crap
					//if(currentToken->type == ELSE )
				}
			}
			else if(currentToken->type == kToken_ELSE)
			{
				if(theFunc->mStatementArray.back()->fetchId() == kState_IF || theFunc->mStatementArray.back()->fetchId() == kState_ELIF)
				{
					fetchToken();
					if(currentToken->type == kToken_IF)
					{
						fetchToken();
						if(currentToken->type != kToken_LPAREN)
						{
							//todo: error
						}
						fetchToken();
						ExpressionStatement* comparsion = parseExpression(theNames, kToken_NULL);
						
						if(currentToken->type != kToken_RPAREN)
						{
							//todo: throw error
						}
						//fetchToken();
						if(currentToken->type == kToken_LCURLY)
						{
							IfStatement* myIf = new IfStatement(kState_ELIF);
							myIf->condition = comparsion;
							myIf->ifBody = createScopedStatementVector(theNames);
							theFunc->mStatementArray.push_back(myIf);
							continue;
						}
						else
						{
							//todo; do this weird logic
						}
						



					}
					else
					{		
						IfStatement* myIf = new IfStatement(kState_ELSE);

						myIf->ifBody = createScopedStatementVector(theNames);
						theFunc->mStatementArray.push_back(myIf);
						continue;
					}
				}
			}

		}
		fetchToken();
	}
}


FuncStatement*
Parser::parse()
{
	FuncStatement* gFunc = new FuncStatement;
	gFunc->mName = "globalScope";
	Token* gType = new Token;
	gType->type = kToken_NUMBER;
	gFunc->mType = gType;
	vector<Identifier*> names;
	if(mVerbose)
		cout << printTokenArray(tokenArray);
	doParseOnFunc(gFunc, names);

	if(mVerbose)
		mLog = gFunc->print();
	
	return gFunc;
}
