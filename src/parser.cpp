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



void
Parser::getTypeList(std::vector<Type*>& typeVec)
{
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
			Type** fType = typeMap[currentToken->mType];
			if(fType == nullptr)
				lerror(kE_Fatal, currentToken, "Compiler error code: 0x001 -- Please Report");
			cType = *fType;
			
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


		if(cType == nullptr)
				lerror(kE_Error, currentToken, "invalid type!");

		typeVec.push_back(cType);

		if(fetchToken()->mType != kToken_COMMA)
			break;
		fetchToken();

	} while(true);
}


void
Parser::handleType()
{
	std::vector<Type*> typeVec;
	//todo:
	//std::vector<Immediate*>  immediateVec;
	getTypeList(typeVec/*,immediateVec*/ );

	if(typeVec.size() < 1)
	{
		lerror(kE_Fatal, currentToken, "cannot form variable from invalid type list!");
	}

	if(currentToken->mCat == kCat_WildIdentifier)
	{
		if(typeVec.size() > 1 || lookAhead(1)->mType == kToken_LPAREN)
		{
			parseFunction(typeVec);
		}
		else
		{
			Variable* var = currentScope()->insertVariable(currentToken, typeVec[0]);

			fetchToken();

			if(currentToken->mType == kToken_ASSIGN_EQUAL)
				var->mInitializer = parseExpr();

			if(currentToken->mType != kToken_SEMICOLON)
				lerror(kE_Fatal, currentToken, "missing semicolon!");
		}


	}

	if(currentToken->mType == kToken_COLON)
	{
		fetchToken();

		

		auto it = typeVec.begin();

		do
		{
			if(currentToken->mCat != kCat_WildIdentifier)
				lerror(kE_Fatal, currentToken, "invalid variable token!");
			Variable* var = currentScope()->insertVariable(currentToken, *it);
			
			fetchToken();

			if(currentToken->mType == kToken_ASSIGN_EQUAL)
				var->mInitializer = parseExpr();


			if(currentToken->mType == kToken_SEMICOLON)
				break;

			if(currentToken->mType != kToken_COMMA)
				lerror(kE_Fatal, currentToken, "invalid token in variable sequence");

			fetchToken();

			if(it + 1 != typeVec.end())
				it++;
		} while(true);

		if(it + 1 != typeVec.end())
			lerror(kE_Warning, currentToken, "More types than variables!");
	}
}

Statement* 
Parser::parseExpr()
{
	while(currentToken->mType != kToken_SEMICOLON)
		fetchToken();

	return nullptr;
}



void
Parser::parseFunction(const std::vector<Type*>& typeVec)
{
	Function* func = currentScope()->insertFunction(currentToken);
	func->typeVec = typeVec;
	fetchToken();
	if(currentToken->mType != kToken_LPAREN)
		lerror(kE_Fatal, currentToken, "expected lparen");

	fetchToken();

	std::vector<Type*> argTypeVec;
	getTypeList(argTypeVec);

	if(currentToken->mType != kToken_COLON)
		lerror(kE_Fatal, currentToken, "expected colon!");

	fetchToken();

		

	auto it = typeVec.begin();

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

		if(it + 1 != typeVec.end())
			it++;
	} while(true);

	if(it + 1 != typeVec.end())
		lerror(kE_Fatal, currentToken, "More types than args!");

	fetchToken();

	if(currentToken->mType != kToken_LCURLY)
		lerror(kE_Fatal, currentToken, "expected curly bracket!");

	fetchToken();

	//Function() allocates mBody for us. (it also deletes it)
	scopeStack.push_back(func->mBody);
	parseIntoScope();
	scopeStack.pop_back();

	//done!
}

void
Parser::parseIntoScope()
{
	Token* prevToken = currentToken;
	while(currentToken != nullptr && currentToken != nullToken)
	{
		if(currentToken->mCat == kCat_Keyword)
		{
			if(currentToken->mType == kToken_UNIQUE || currentToken->mType == kToken_SHARED)
			{
				handleType();
			}
			else if(currentToken->mType == kToken_IF)
			{

			}
			else if(currentToken->mType == kToken_ELSE)
			{

			}
			else if(currentToken->mType == kToken_FOR)
			{

			}
			else if(currentToken->mType == kToken_WHILE)
			{

			}
			else if(currentToken->mType == kToken_MATCH)
			{

			}
			else if(currentToken->mType == kToken_BREAK)
			{

			}
			else if(currentToken->mType == kToken_CONTINUE)
			{

			}
			else if(currentToken->mType == kToken_RETURN)
			{
				Return* tRotK = currentScope()->insertReturn();
				fetchToken();
				tRotK->expr = parseExpr();
			}
			else if(currentToken->mType == kToken_ENUM)
			{

			}
			else if(currentToken->mType == kToken_STRUCT)
			{

			}
			else if(currentToken->mType == kToken_USE)
			{

			}
			else if(currentToken->mType == kToken_IMPORT)
			{

			}
			else if(currentToken->mType == kToken_UNIT) 
			{

			}
			else if(currentToken->mType == kToken_CINCLUDE)
			{
				lerror(kE_Error, currentToken, "Reserved Keyword!");
			}
			else if(currentToken->mType == kToken_ISNULL)
			{

			}
			else if(currentToken->mType == kToken_SIZEOF)
			{

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
				MultipleAssignment* multAss = currentScope()->insertMultipleAssignment();
				multAss->names.push_back(currentToken->mStr);
				multAss->expr = parseExpr();
			}
			else
			{
				int i = 0;
				while(true)
				{

					if(lookAhead(i+1)->mType == kToken_ASSIGN_EQUAL)
					{
						MultipleAssignment* multAss = currentScope()->insertMultipleAssignment();
						multAss->names.push_back(currentToken->mStr);
						fetchToken();
						while(currentToken->mType != kToken_ASSIGN_EQUAL)
						{
							fetchToken();
							multAss->names.push_back(currentToken->mStr);
							fetchToken();
						}
						//parse on the = sign
						multAss->expr = parseExpr();
						break;
					}
					else if(lookAhead(i+1)->mType == kToken_DECLARE_EQUAL)
					{
						DeclEqual* dEq = currentScope()->insertDeclEqual();
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
			if(currentToken->mType != kToken_SEMICOLON)
				lerror(kE_Fatal, currentToken, "Expected Semicolon!");
		}

		

		if(prevToken == currentToken)
		{
			lerror(kE_Error, prevToken, "Unhandled token!");
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