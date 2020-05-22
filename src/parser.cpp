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
Parser::parseIntoScope()
{
	Token* prevToken = currentToken;
	while(currentToken != nullptr && currentToken != nullToken)
	{
		if(currentToken->mCat == kCat_Keyword)
		{
			if(currentToken->mType == kToken_UNIQUE || currentToken->mType == kToken_SHARED)
			{
				goto handleType;
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
			handleType:
			std::vector<Type*> typeVec;
			getTypeList(typeVec);

		}
		else if(currentToken->mCat == kCat_WildIdentifier)
		{
			
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