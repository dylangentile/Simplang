#include "parser.h"
#include "error.h"

#include <stack>

int
getOperatorPrecedence(const BinaryOperationID& x)
{
	switch(x)
	{
		case kOp_ADD:
		case kOp_SUB: return 6;
		break;

		case kOp_MUL: 
		case kOp_DIV:
		case kOp_MOD: return 5;
		break;

		case kOp_L_AND: return 14;
		break;
		case kOp_L_OR: return 15; //this and this or this // read as (this and this) or this
		break;

		case kOp_L_EQUAL:
		case kOp_L_NOT_EQUAL: return 10;
		break;
		
		case kOp_LESS: 
		case kOp_GREATER:
		case kOp_LESS_EQUAL:
		case kOp_GREATER_EQUAL: return 9;
		break;
		
		case kOp_BIT_AND: return 11;
		break;
		case kOp_BIT_OR: return 12;
		break;
		
		case kOp_NULLCC: return 16;
		break;
		case kOp_NULL: lerrorSTR(kE_Fatal, "invalid call!");
	}

	lerrorSTR(kE_Fatal, "invalid call!");
	return 1000000; //to shut up clang

}



bool
isPrefixUnaryOperator(Token* tok) // ! - * ~ & @
{
	return 		tok->mType == kToken_NOT 		|| tok->mType == kToken_MINUS
			||	tok->mType == kToken_MULTIPLY	|| tok->mType == kToken_BIT_COMPL
			||	tok->mType == kToken_BIT_AND	|| tok->mType == kToken_ADDR;

}

bool
isPostfixUnaryOperator(Token* tok) // ++ -- [] . ?. 
{
	return 		tok->mType == kToken_INCREMENT 	|| tok->mType == kToken_DECREMENT
		  /*||	tok->mType == kToken_RBRACKET*/	|| tok->mType == kToken_DOT
			||	tok->mType == kToken_SFNAV;
}




Parser::Parser(const char* filename) : tokenVec(nullptr)
{
	mLexer = new Lexer(filename);
	globalScope = new Scope;

	typeMap.insert(kToken_VOID, new BasicType(kBaseT_VOID));
	typeMap.insert(kToken_STRING, new BasicType(kBaseT_STRING));
	typeMap.insert(kToken_BYTE, new BasicType(kBaseT_BYTE));
	typeMap.insert(kToken_BOOL, new BasicType(kBaseT_BOOL));

	typeMap.insert(kToken_INT8, new BasicType(kBaseT_INT8));
	typeMap.insert(kToken_INT16, new BasicType(kBaseT_INT16));
	typeMap.insert(kToken_INT32, new BasicType(kBaseT_INT32));
	typeMap.insert(kToken_INT64, new BasicType(kBaseT_INT64));

	typeMap.insert(kToken_UINT8, new BasicType(kBaseT_UINT8));
	typeMap.insert(kToken_UINT16, new BasicType(kBaseT_UINT16));
	typeMap.insert(kToken_UINT32, new BasicType(kBaseT_UINT32));
	typeMap.insert(kToken_UINT64, new BasicType(kBaseT_UINT64));

	typeMap.insert(kToken_SINGLE, new BasicType(kBaseT_SINGLE));
	typeMap.insert(kToken_DOUBLE, new BasicType(kBaseT_DOUBLE));

	typeMap.insert(kToken_DYNAMIC, new BasicType(kBaseT_DYNAMIC));
	typeMap.insert(kToken_TYPENAME, new BasicType(kBaseT_TYPENAME));


	preOpMap.insert(kToken_NOT, kPreOp_NOT);
	preOpMap.insert(kToken_MINUS, kPreOp_NEG);
	preOpMap.insert(kToken_MULTIPLY, kPreOp_DEREF);
	preOpMap.insert(kToken_BIT_COMPL, kPreOp_COMPL);
	preOpMap.insert(kToken_BIT_AND, kPreOp_ADDROF);
	preOpMap.insert(kToken_ADDR, kPreOp_ATADDR);



	postOpMap.insert(kToken_INCREMENT, kPostOp_INCR);
	postOpMap.insert(kToken_DECREMENT, kPostOp_DECR);
	postOpMap.insert(kToken_DOT, kPostOp_DOT);
	postOpMap.insert(kToken_SFNAV, kPostOp_SFNAV);



	binOpMap.insert(kToken_PLUS, kOp_ADD);
	binOpMap.insert(kToken_MINUS, kOp_SUB);
	binOpMap.insert(kToken_MULTIPLY, kOp_MUL);
	binOpMap.insert(kToken_DIVIDE, kOp_DIV);
	binOpMap.insert(kToken_MODULO, kOp_MOD);

	binOpMap.insert(kToken_LOGIC_AND, kOp_L_AND);
	binOpMap.insert(kToken_LOGIC_OR, kOp_L_OR);
	binOpMap.insert(kToken_LOGIC_EQUAL, kOp_L_EQUAL);
	binOpMap.insert(kToken_NOT_EQUAL, kOp_L_NOT_EQUAL);

	binOpMap.insert(kToken_LESS, kOp_LESS);
	binOpMap.insert(kToken_GREATER, kOp_GREATER);
	binOpMap.insert(kToken_LESS_EQUAL, kOp_LESS_EQUAL);
	binOpMap.insert(kToken_GREATER_EQUAL, kOp_GREATER_EQUAL);

	binOpMap.insert(kToken_BIT_AND, kOp_BIT_AND);
	binOpMap.insert(kToken_BIT_OR, kOp_BIT_OR);

	binOpMap.insert(kToken_NULLCC, kOp_NULLCC);




	nullToken = &theActualNullToken;

	nullToken->mCat = kCat_NULL;
	nullToken->mType = kToken_NULL;


}

Parser::~Parser()
{
	delete mLexer;


	//for(auto it = typeMap.begin(); it != typeMap.end(); it++)
	//	delete it->second;
	

	//don't delete globalScope, handled on outside
}

Scope*
Parser::currentScope()
{
	return scopeStack.back();
}

Token* 
Parser::fetchToken()
{
	if(tokenIt == tokenVec->end())
		return (currentToken = nullToken);

	currentToken = *tokenIt;
	tokenIt++;
	return currentToken;
}

Token*
Parser::lookAhead(uint32_t offset)
{
	if(offset < 1)
		return nullToken;
	if((tokenIt + (offset - 1)) >= tokenVec->end())
		return nullToken;

	return *(tokenIt + (offset - 1));
}



MultipleType*
Parser::getTypeList()
{

	MultipleType* theTypeList = new MultipleType();
	do
	{
		Type* cType = nullptr;
		PointerID thePtrID = kPtr_Raw;

		if(currentToken->mType == kToken_SHARED)
		{
			thePtrID = kPtr_Shared;
			fetchToken();
		}
		else if(currentToken->mType == kToken_UNIQUE)
		{
			thePtrID = kPtr_Unique;
			fetchToken();
		}
		

		if(currentToken->mCat == kCat_BasicType)
		{
			auto fType = typeMap.find(currentToken->mType);
			if(typeMap.isEnd(fType))
				lerror(kE_Fatal, currentToken, "Compiler error code: 0x001 -- Please Report");
			cType = fType->second;
			
		}
		else if(currentToken->mCat == kCat_WildIdentifier)
		{
			for(auto scopeIt = scopeStack.rbegin(); scopeIt != scopeStack.rend(); scopeIt++)
			{
				if((cType = (*scopeIt)->getType(currentToken->mStr)) != nullptr)
					break;
			}

			if(cType == nullptr)
			{
				UnknownType* uType = new UnknownType();
				currentScope()->unknownTypeMap.insert(uType, currentToken->mStr);
				cType = dynamic_cast<Type*>(uType);
			}
		}
		else
		{
			lerror(kE_Error, currentToken, "invalid token in type sequence");
		}

		if(lookAhead(1)->mType == kToken_MULTIPLY)
		{
			auto finder = ptrTypeMap.find(cType);
			
			if(finder == ptrTypeMap.end())
			{
				Type* pType = new PointerType(cType, thePtrID);
				ptrTypeMap.insert(std::make_pair(cType, pType));
				cType = pType;
			}
			else
			{
				cType = finder->second;
			}

			fetchToken();

		}
		else if(thePtrID != kPtr_Raw)
		{
			lerror(kE_Error, currentToken, "Declared unique/shared attribute of non-pointer type!");
		}


		if(lookAhead(1)->mType == kToken_LBRACKET)
		{
			fetchToken();
			fetchToken();

			ArrayType* arr = new ArrayType(cType);
			cType = dynamic_cast<Type*>(arr);


			if(currentToken->mType != kToken_RBRACKET && currentToken->mCat == kCat_Immediate)
			{
				if(currentToken->mType != kToken_UINT64)
					lerror(kE_Fatal, currentToken, "expected integer!");

				arr->mSize = std::stoull(currentToken->mStr);
				fetchToken();
			}
		}

		if(cType == nullptr)
				lerror(kE_Error, currentToken, "invalid type!");

		theTypeList->insert(cType);

		if(fetchToken()->mType != kToken_COMMA)
			break;
		fetchToken();

	} while(true);

	return theTypeList;
}


void
Parser::handleType(std::vector<Variable*>* varVec)
{
	
	MultipleType* types = getTypeList();

	if(types->typeVec.size() < 1)
	{
		lerror(kE_Fatal, currentToken, "cannot form variable from invalid type list!");
	}

	if(currentToken->mCat == kCat_WildIdentifier)
	{
		if(types->typeVec.size() > 1 || lookAhead(1)->mType == kToken_LPAREN)
		{
			parseFunction(types);
		}
		else
		{
			Variable* var = nullptr;
			if(varVec == nullptr)
			 	var = currentScope()->insertVariable(currentToken, types->typeVec[0]);
			else
			{
				var = new Variable(currentToken->mStr, types->typeVec[0]);
				varVec->push_back(var);
			}
			

			fetchToken();

			if(currentToken->mType == kToken_ASSIGN_EQUAL)
				var->mInitializer = parseExpr();
		}


	}
	else if(currentToken->mType == kToken_COLON)
	{
		fetchToken();

		

		auto it = types->typeVec.begin();

		do
		{
			if(currentToken->mCat != kCat_WildIdentifier)
				lerror(kE_Fatal, currentToken, "invalid variable token!");
			
			Variable* var = nullptr;
			if(varVec == nullptr)
				currentScope()->insertVariable(currentToken, *it);
			else
			{
				var = new Variable(currentToken->mStr, *it);
				varVec->push_back(var);
			}
			
			fetchToken();

			if(currentToken->mType == kToken_ASSIGN_EQUAL)
				var->mInitializer = parseExpr(true);


			if(currentToken->mType == kToken_SEMICOLON)
				break;

			if(currentToken->mType != kToken_COMMA)
				lerror(kE_Fatal, currentToken, "invalid token in variable sequence");

			fetchToken();

			if(it + 1 != types->typeVec.end())
				it++;
		} while(true);

		if(it + 1 != types->typeVec.end())
			lerror(kE_Warning, currentToken, "More types than variables!");
	}
}

/*
Possible cases

let + = all binary operators

op:	case:
+ 	(
+ 	var
+ 	var++
+ 	func()
+	(unaryPrefix)case
+ 	case(unaryPostfix)


//binop case higher-precedence
+ case *
//binop case lower-precedence
+ case &&
+ case )
+ case ,
+ case ;


steps:
1. parse case
2. read next operator
3. recurse or return


*/


//This function makes me want to tear my hair out... (but it works[I think...])
Statement*
Parser::parseTerm()
{

	Statement* theTerm = nullptr;

	//variable,  

	if(isPrefixUnaryOperator(currentToken))
	{
		Term* term = new Term();
		term->preOp = *preOpMap[currentToken->mType];
		theTerm = dynamic_cast<Statement*>(term);
		fetchToken();
	} 

	if(currentToken->mCat == kCat_WildIdentifier)
	{
		Statement* result = nullptr;
		

		if(lookAhead(1)->mType == kToken_LPAREN)
		{
			result = dynamic_cast<Statement*>(parseFunctionCall());
		}
		else
		{
			result = dynamic_cast<Statement*> (new VariableAccess(currentToken->mStr));	
			fetchToken();
		}

		if(theTerm == nullptr)
		{
			theTerm = result;
		}
		else
		{
			Term* term = dynamic_cast<Term*>(theTerm);
			term->operand = result;

		}

	}
	else if(currentToken->mCat == kCat_Immediate)
	{
		if(theTerm != nullptr)
			lerror(kE_Fatal, currentToken, "cannot use prefix operator on immediate values!");

		auto tfinder = typeMap.find(currentToken->mType);
		if(typeMap.isEnd(tfinder))
			lerror(kE_Fatal, currentToken, "compiler error, miscategorization or un-implemented type!");

		if(tfinder->second->mId != kType_Basic)
			lerror(kE_Fatal, currentToken, "compiler error! somehow this got categorized as a non-basic type immediate!");

		Immediate* val = new Immediate(dynamic_cast<BasicType*>(tfinder->second));
		if(!val->parseValue(currentToken->mStr))
			lerror(kE_Fatal, currentToken, "invalid immediate value!");

		theTerm = dynamic_cast<Statement*>(val);
	}
	else
	{
		lerror(kE_Fatal, currentToken, "token isn't valid term in expresion sequence!");
	}



	if(isPostfixUnaryOperator(currentToken))
	{
		if(theTerm->mId == kState_ImmediateExpr)
			lerror(kE_Fatal, currentToken, "cannot use postfix operator on immdiate values!");
		Term* term = nullptr;
		if(theTerm->mId != kState_Term)
		{
			term = new Term();
			term->operand = theTerm;
		}
		else
		{
			term = dynamic_cast<Term*>(theTerm);
		}

		term->postOp = *postOpMap[currentToken->mType];

		fetchToken();

	}

	return theTerm;

}

//https://en.wikipedia.org/wiki/Shunting-yard_algorithm
//should enter with first term
Statement*
Parser::parseBinExpr(bool haltRPAREN)
{
	std::deque<Statement*> outputDeque;
	std::stack<TokenType> operatorStack;

	Token* errorToken = currentToken;

	bool parenEnd = false;

	while(	currentToken->mType != kToken_SEMICOLON && currentToken->mType != kToken_COMMA
		&&	currentToken->mType != kToken_LCURLY) //if statement
	{
		BinaryOperationID* id = binOpMap[currentToken->mType];
		if(id != nullptr)
		{
			while(	   !operatorStack.empty()
					&& binOpMap.contains(operatorStack.top())
					&& (getOperatorPrecedence(*id) > getOperatorPrecedence(*binOpMap[operatorStack.top()]))
				 )
			{
				BinaryOperationID* operatorEnum = binOpMap[operatorStack.top()];
				operatorStack.pop();
				
				if(operatorEnum == nullptr)
					lerror(kE_Fatal, errorToken, "parseBinExpr failed!");

				BinOp* theOp = new BinOp(*operatorEnum);

				if(outputDeque.size() < 2)
					lerror(kE_Fatal, errorToken, "parseBinExpr failed! WTF");

				theOp->operand2 = outputDeque.back(); outputDeque.pop_back();
				theOp->operand1 = outputDeque.back(); outputDeque.pop_back();

				outputDeque.push_back(dynamic_cast<Statement*>(theOp));
			}

			operatorStack.push(currentToken->mType);
		}
		else if(currentToken->mType == kToken_LPAREN)
		{
			operatorStack.push(currentToken->mType);
		}
		else if(currentToken->mType == kToken_RPAREN)
		{
			while(operatorStack.top() != kToken_LPAREN)
			{
				
				BinaryOperationID* operatorEnum = binOpMap[operatorStack.top()];
				operatorStack.pop();
				
				if(operatorEnum == nullptr)
					lerror(kE_Fatal, errorToken, "parseBinExpr failed!");

				BinOp* theOp = new BinOp(*operatorEnum);

				if(outputDeque.size() < 2)
					lerror(kE_Fatal, errorToken, "parseBinExpr failed! WTF");

				theOp->operand2 = outputDeque.back(); outputDeque.pop_back();
				theOp->operand1 = outputDeque.back(); outputDeque.pop_back();

				if(operatorStack.empty() && !haltRPAREN)
				{
					lerror(kE_Fatal, errorToken, "mismatched parentheses!");
				}
				else
				{
					parenEnd = true;
					break;
				}


				outputDeque.push_back(dynamic_cast<Statement*>(theOp));


			}

			if(parenEnd)
			{
				fetchToken();
				break;
			}

			operatorStack.pop(); //pop the lParen
		}
		else //must be a term or throw error
		{
			outputDeque.push_back(parseTerm());
		}

		fetchToken();

	}

	while(!operatorStack.empty())
	{
		BinaryOperationID* operatorEnum = binOpMap[operatorStack.top()];
		operatorStack.pop();
		
		if(operatorEnum == nullptr)
			lerror(kE_Fatal, errorToken, "parseBinExpr failed!");

		BinOp* theOp = new BinOp(*operatorEnum);

		if(outputDeque.size() < 2)
			lerror(kE_Fatal, errorToken, "parseBinExpr failed! WTF");

		theOp->operand2 = outputDeque.back(); outputDeque.pop_back();
		theOp->operand1 = outputDeque.back(); outputDeque.pop_back();

		outputDeque.push_back(dynamic_cast<Statement*>(theOp));
	}

	if(outputDeque.size() != 1)
		lerror(kE_Fatal, errorToken, "bad expression!");

	return outputDeque[0];

}


Statement* 
Parser::parseExpr(bool haltComma, bool haltRPAREN)
{

	if(currentToken->mType == kToken_ASSIGN_EQUAL || currentToken->mType == kToken_DECLARE_EQUAL)
		fetchToken();

	Statement* x = parseBinExpr(haltRPAREN);

	if(currentToken->mType == kToken_COMMA && !haltComma)
	{
		StatementList* myList = new StatementList();
		myList->insert(x);

		while((currentToken->mType != kToken_SEMICOLON && currentToken->mType != kToken_LCURLY ) || (haltRPAREN && currentToken->mType != kToken_RPAREN))
		{

			if(currentToken->mType != kToken_COMMA)
				lerror(kE_Fatal, currentToken, "expected comma or semicolon!");
			fetchToken();

			myList->insert(parseBinExpr(haltRPAREN));
			

		}

		x = dynamic_cast<Statement*>(myList);
	}

	fetchToken(); //must be a ender

	return x;
}


void
Parser::parseStruct()
{
	if(currentToken->mType != kToken_STRUCT)
		lerror(kE_Fatal, currentToken, "expected 'struct' token!");


	Token* name = fetchToken();

	if(currentToken->mCat != kCat_WildIdentifier)
		lerror(kE_Fatal, currentToken, "invalid struct name!");

	fetchToken();

	bool publicStruct = false;

	if(currentToken->mType == kToken_COLON)
	{
		fetchToken();
		if(currentToken->mType == kToken_PUBLIC)
		{
			publicStruct = true;
		}
		else if(currentToken->mType != kToken_PRIVATE)
		{
			lerror(kE_Fatal, currentToken, "invalid token! public or private expected");
		}
		fetchToken();
	}


	if(currentToken->mType != kToken_LCURLY)
		lerror(kE_Fatal, currentToken, "expected l-curly bracket!");

	fetchToken();

	StructType* sType = currentScope()->insertStruct(name, publicStruct);
	Structure* theStruct = sType->definition;

	while(currentToken->mType != kToken_RCURLY)
	{
		handleType(&theStruct->members);
		if(currentToken->mType == kToken_SEMICOLON)
			fetchToken();
		
	}
	fetchToken();

	if(currentToken->mType != kToken_SEMICOLON)
		lerror(kE_Fatal, currentToken, "missing semicolon!");

	fetchToken();




}

FunctionCall*
Parser::parseFunctionCall()
{
	FunctionCall* theCall = new FunctionCall();
	theCall->setDebug(currentToken->mData);
	if(currentToken->mCat != kCat_WildIdentifier)
		lerror(kE_Fatal, currentToken, "expected function call identifier!");

	fetchToken();

	if(currentToken->mType != kToken_LPAREN)
		lerror(kE_Fatal, currentToken, "expected left parenthesis!");

	fetchToken();

	Statement* theArgs = parseExpr(false, true);
	if(theArgs->mId != kState_StatementList)
	{
		theCall->args = new StatementList;
		theCall->args->insert(theArgs);
	}
	else
	{
		theCall->args = dynamic_cast<StatementList*>(theArgs);
	}

	return theCall;

}

void
Parser::parseFunction(Type* funcType)
{
	Function* func = currentScope()->insertFunction(currentToken);
	func->mType = funcType;
	fetchToken();
	if(currentToken->mType != kToken_LPAREN)
		lerror(kE_Fatal, currentToken, "expected lparen");

	fetchToken();

	
	MultipleType* argTypes = getTypeList();

	if(currentToken->mType != kToken_COLON)
		lerror(kE_Fatal, currentToken, "expected colon!");

	fetchToken();

		

	auto it = argTypes->typeVec.begin();

	do
	{
		if(currentToken->mCat != kCat_WildIdentifier)
			lerror(kE_Fatal, currentToken, "invalid arguement variable token!");
		Variable* var = new Variable(currentToken->mStr, *it);
		func->args.push_back(var);
		
		fetchToken();

		if(currentToken->mType == kToken_ASSIGN_EQUAL)
			lerror(kE_Fatal, currentToken, "Function arg defaults are not a feature of Simplang!");


		if(currentToken->mType == kToken_RPAREN)
			break;

		if(currentToken->mType != kToken_COMMA)
			lerror(kE_Fatal, currentToken, "invalid token in arg sequence");

		fetchToken();

		if(it + 1 != argTypes->typeVec.end())
			it++;
	} while(true);

	if(it + 1 != argTypes->typeVec.end())
		lerror(kE_Fatal, currentToken, "More types than args!");

	fetchToken();

	initScopeParse(func->mBody);

	//done!
}

Statement*
Parser::parseIfStatement()
{
	if(currentToken->mType != kToken_IF)
		lerror(kE_Fatal, currentToken, "expected if statement!");

	fetchToken();
	if(currentToken->mType != kToken_LPAREN)
		lerror(kE_Fatal, currentToken, "expected left parenthesis!")

	IfStatement* theIf = new IfStatement();
	theIf->condition = parseBinExpr();
	initScopeParse(theIf->mBody);

	if(currentToken->mType == kToken_ELSE)
	{
		fetchToken();
		if(currentToken->mType == kToken_IF)
		{
			theIf->elseEval = parseIfStatement();
		}
		else if(currentToken->mType == kToken_LCURLY)
		{
			Scope* elseScope = new Scope();
			initScopeParse(elseScope);
			theIf->elseEval = dynamic_cast<Statement*>(elseScope);
		}
		else
		{
			lerror(kE_Fatal, currentToken, "expected 'if' or '{' !");
		}
	}

	return dynamic_cast<Statement*>(theIf);
}


void
Parser::initScopeParse(Scope* scope)
{
	if(currentToken->mType != kToken_LCURLY)
					lerror(kE_Fatal, currentToken, "expected left curly!");

	fetchToken();

	scopeStack.push_back(scope);
	parseIntoScope();
	scopeStack.pop_back();

	fetchToken(); //fetch past rcurly
}


void
Parser::parseIntoScope()
{
	Token* prevToken = currentToken;
	while(currentToken != nullptr && currentToken != nullToken)
	{
		if(currentToken->mType == kToken_RCURLY)
			break;
		
		if(currentToken->mCat == kCat_Keyword)
		{
			switch(currentToken->mType)
			{
				case kToken_UNIQUE:
				case kToken_SHARED: handleType();
				break;
				case kToken_IF: currentScope()->pushStatement(parseIfStatement());
				break;
				case kToken_ELSE: lerror(kE_Fatal, currentToken, "floating else! No if preceding it!");
				break;
				case kToken_FOR:
				break;
				case kToken_WHILE:
				break;
				case kToken_MATCH:
				break;
				case kToken_BREAK:
				break;
				case kToken_CONTINUE:
				break;
				case kToken_RETURN:
				{
					Return* tRotK = currentScope()->insertReturn(currentToken);
					fetchToken();
					tRotK->expr = parseExpr();
				}
				break;
				case kToken_ENUM:
				break;
				case kToken_STRUCT: parseStruct();
				break;
				case kToken_USE:
				break;
				case kToken_IMPORT:
				break;
				case kToken_UNIT:
				break;
				case kToken_CINCLUDE:
				break;
				case kToken_ISNULL:
				break;
				case kToken_SIZEOF:
				break;

			}
		}
		else if(currentToken->mCat == kCat_BasicType)
		{
			handleType();
		}
		else if(currentToken->mCat == kCat_WildIdentifier)
		{
			Token* future = lookAhead(1);

			if(future->mCat == kCat_WildIdentifier)
			{
				handleType();
			}
			else if(future->mType == kToken_DECLARE_EQUAL)
			{
				Variable* var = currentScope()->insertVariable(currentToken, nullptr);
				
				fetchToken();

				var->mInitializer = parseExpr();
			}
			else if(future->mType == kToken_ASSIGN_EQUAL)
			{
				MultipleAssignment* multAss = currentScope()->insertMultipleAssignment(currentToken);
				multAss->insert(currentToken->mStr);
				multAss->expr = parseExpr();

			}
			else if(future->mType == kToken_LPAREN)
			{
				currentScope()->pushStatement(parseFunctionCall());
			}
			else if(future->mType == kToken_INCREMENT || future->mType == kToken_DECREMENT)
			{
				if(lookAhead(2)->mType != kToken_SEMICOLON)
					lerror(kE_Fatal, lookAhead(2), "expected semicolon!");

				lerror(kE_Fatal, future, "++/-- not yet supported :(");


			}
			else if
				(	future->mType == kToken_PLUS_EQUAL 		|| future->mType == kToken_MINUS_EQUAL
				||	future->mType == kToken_MULTIPLY_EQUAL	|| future->mType == kToken_DIVIDE_EQUAL
				||	future->mType == kToken_MODULO_EQUAL
				)
			{
				lerror(kE_Fatal, future, "+-*/%= not yet supported :(");
			}
			else
			{
				int i = 0;
				while(true)
				{

					if(lookAhead(i+1)->mType == kToken_ASSIGN_EQUAL)
					{
						MultipleAssignment* multAss = currentScope()->insertMultipleAssignment(currentToken);
						multAss->insert(currentToken->mStr);
						fetchToken();
						while(currentToken->mType != kToken_ASSIGN_EQUAL)
						{
							fetchToken();
							multAss->insert(currentToken->mStr);
							fetchToken();
						}
						//parse on the = sign
						multAss->expr = parseExpr();
						break;
					}
					else if(lookAhead(i+1)->mType == kToken_DECLARE_EQUAL)
					{
						DeclEqual* dEq = currentScope()->insertDeclEqual(currentToken);
						dEq->insert(currentToken->mStr);
						fetchToken();
						while(currentToken->mType != kToken_DECLARE_EQUAL)
						{
							fetchToken();
							dEq->insert(currentToken->mStr);
							fetchToken();
						}
						//parse on the := sign
						dEq->mInitializer = parseExpr();
						break;
					}
					else if(lookAhead(i+1)->mType != kToken_COMMA)
					{
						handleType();
						break;
					}
					
					i += 2;
					
					if(lookAhead(i)->mCat != kCat_WildIdentifier)
					{
						handleType();
						break;
					}

				}
				
			}


		}

		

		if(prevToken == currentToken)
		{
			lerror(kE_Error, prevToken, "Unhandled token!");
			fetchToken();
		}
		prevToken = currentToken;
	}

	if(scopeStack.size() == 1 && currentToken != nullptr && currentToken != nullToken)
		lerrorSTR(kE_Warning, "Extra rcurly at end of file!");

}




Scope*
Parser::parse()
{
	tokenVec = mLexer->lex();
	if(ErrorManager::haveErrors())
	{
		//printf("%s\n", ErrorManager::report().c_str());
		return nullptr;
	}
	tokenIt = tokenVec->begin();
	fetchToken();

	scopeStack.push_back(globalScope);
	parseIntoScope();



	return globalScope;
}