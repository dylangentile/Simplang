#include "parser.h"
#include "error.h"

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

bool
Parser::checkRedeclaration(const std::string& name, bool doError)
{
	auto finder = scopeStack.back()->usedNames.find(name);
	if(finder != scopeStack.back()->usedNames.end())
	{
		if(doError) lerror(kE_Error, currentToken, "This identifier is already being used!");
		return true;
	}

	return false;
}

StructType*
Parser::findStructType(const std::string& name)
{
	//go from back to front
	for(auto it = scopeStack.rbegin(); it != scopeStack.rend(); it++)
	{
		Scope* theScope = *it;
		StructType** finder = theScope->structMap[name];
		if(finder != nullptr)
			return *finder;
	}
	return nullptr;
}



Type* 
Parser::deriveTypeFromToken(Token* theToken)
{
	Type* theType = nullptr;
	if(theToken->mCat == kCat_BasicType)
	{

		Type** finder = typeMap[theToken->mType];
		if(finder == nullptr)
			lerror(kE_Fatal, theToken, "Token incorrectly categorized, or not included in typeMap! (Compiler Bug, not your fault)");
		theType = *finder;
	}
	else if(theToken->mCat == kCat_WildIdentifier)
	{
		StructType* finder = findStructType(theToken->mStr);
		if(finder == nullptr)
		{
			//finder = new StructType(theToken->mStr, false);
			//currentScope()->structMap.insert(finder, finder->name);
			theType = nullptr;
		}
		else
			theType = dynamic_cast<Type*>(finder);
	}

	if(theType == nullptr)
		lerror(kE_Error, theToken, "Expected Type!");

	return theType;
}

void 
Parser::getTypeList(std::vector<Type*>& typeVec)
{
	Type* theType = deriveTypeFromToken(currentToken);
	typeVec.push_back(theType);
	fetchToken();

	while(currentToken->mType == kToken_COMMA)
	{
		fetchToken();
		theType = deriveTypeFromToken(currentToken);
		typeVec.push_back(theType);
		fetchToken();
	}
}





FunctionCall*
Parser::parseFunctionCall()
{

	/*if(currentToken->mCat != kCat_WildIdentifier)
		lerror(kE_Error, currentToken, "Invalid token for function call!");

	Function* theFunc;
	Function** finder = currentScope()->functionDefMap[currentToken->mStr];
	if(finder == nullptr)
	{
		Function* theFunc = new Function();
		theFunc->mName = currentToken->mStr;
		currentScope()->functionDefMap.insert(theFunc, theFunc->mName);
	}
	else
		theFunc = *finder;


	FunctionCall* theCall = new FunctionCall;
	theCall->parentFunc = theFunc;

	fetchToken();

	if(currentToken->mType != kToken_LPAREN)
		lerror(kE_Fatal, currentToken, "Function call mis-id!");
	

	fetchToken();

	while(currentToken->mType != kToken_RPAREN)
	{
		if(currentToken == nullToken)
			lerror(kE_Fatal, nullptr, "reached EOF before end of function call!");
		fetchToken();
		theCall->args.push_back(parseExpr());
	}

	fetchToken();

	return theCall;*/
	return nullptr;






}



/*

Expressions are the construct of programming that get things done
I like to treat expressions like in math, i.e x = 5 is an equation,
5 is an expression.

An expression contains identifiers, function calls, immediates, and operations.
identifiers, function calls, and immediates are all acted upon by the operations,
therefore I will call them operands


*/


Statement*
Parser::parseExpr(/*Type* typeHint*/)
{
	if(currentToken->mType == kToken_ASSIGN_EQUAL)
		fetchToken();

	Statement* operand1 = nullptr;
	if(currentToken->mCat == kCat_WildIdentifier)
	{
		if(lookAhead(1)->mType == kToken_LPAREN)
		{
			FunctionCall* parseFunctionCall();
		}
	}	
}








Variable*
Parser::fetchNextVariable(Type* theType)
{
	if(currentToken->mCat != kCat_WildIdentifier)
	{
		lerror(kE_Error, currentToken, "Cannot define variable with non-identifier");
		fetchToken();
		return nullptr;
	}

	//the outside world will "catch" the error
	// in this case either parseStruct or parseVarDefs
	checkRedeclaration(currentToken->mStr);

	Variable* theVar = new Variable(currentToken->mStr, theType);

	fetchToken();
	if(currentToken->mType == kToken_ASSIGN_EQUAL)
		theVar->mInitializer = parseExpr();

	return theVar;

}




void
Parser::parseVarDefs(std::vector<Type*>* typeArray)
{
	ErrorManager::resetCounter();

	std::vector<Type*> rtypeArray;
	if(typeArray == nullptr)
	{
		getTypeList(rtypeArray);
		typeArray = &rtypeArray;
	}

	

	


	if(currentToken->mType == kToken_COLON)
	{
		fetchToken();
		for(auto it = typeArray->begin(); it != typeArray->end(); it++)
		{
			Variable* theVar = fetchNextVariable(*it);
			if(theVar == nullptr)
				break;

			currentScope()->statementVec.push_back(dynamic_cast<Statement*>(theVar));
			currentScope()->usedNames.insert(theVar->mName);

			if(currentToken->mType == kToken_COMMA)
			{
				fetchToken();
				if(it + 1 == typeArray->end())
					lerror(kE_Fatal, currentToken, "more variables than types!");
				continue;
			}

			if(currentToken->mType == kToken_SEMICOLON)
			{
				if(it + 1 != typeArray->end())
				{
					lwarning(currentToken, "declared more types than symbols!");
				}
				break;
			}

			lerror(kE_Error, currentToken, "Unknown token in variable declaration(s)!");
			break;

		}
	}
	else
	{

		Type* theType = (*typeArray)[0];
		while(true)
		{
			Variable* theVar = fetchNextVariable(theType);
			if(theVar == nullptr)
				break;

			scopeStack.back()->statementVec.push_back(dynamic_cast<Statement*>(theVar));
			scopeStack.back()->usedNames.insert(theVar->mName);

			if(currentToken->mType == kToken_COMMA)
			{
				fetchToken();
				continue;
			}

			if(currentToken->mType == kToken_SEMICOLON)
				break;

			lerror(kE_Error, currentToken, "Unknown token in variable declaration(s)!");
			break;
		}

		if(typeArray->size() > 1)
			lerror(kE_Error, currentToken, "Probably missing a semicolon in your declaration!");
	}



	if(ErrorManager::gotErrors())
		lerror(kE_Fatal, nullptr, "failed variable declaration parsing!");



}


void
Parser::parseStruct()
{

}

void
Parser::parseFunction()
{

}

void
Parser::parseIntoScope()
{
	Token* prevToken = currentToken;
	while(currentToken != nullptr && currentToken != nullToken)
	{
		if(currentToken->mCat == kCat_Keyword)
		{
			if(currentToken->mType == kToken_STRUCT)
				parseStruct();
			
		}
		else if(currentToken->mCat == kCat_BasicType)
		{
			std::vector<Type*> typeVec;
			getTypeList(typeVec);
			
			if(currentToken->mCat == kCat_WildIdentifier)
				parseFunction();
			else if(currentToken->mType == kToken_COLON)
				parseVarDefs(&typeVec);				
		}


		

		if(prevToken == currentToken)
		{
			//lerror(kE_Error, prevToken, "Unhandled token!");
			fetchToken();
		}
		prevToken = currentToken;
	}

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