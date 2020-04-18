#include "parser.h"
#include "error.h"

Parser::Parser(const char* filename) : tokenVec(nullptr)
{
	mLexer = new Lexer(filename);
	globalScope = new Scope;


}

Parser::~Parser()
{
	delete mLexer;
	//don't delete globalScope, handled on outside
}

Token* 
Parser::fetchToken()
{
	if(tokenIt == tokenVec->end())
		return (currentToken = nullptr);

	currentToken = *tokenIt;
	tokenIt++;
	return currentToken;
}

Token*
Parser::lookAhead(uint32_t offset)
{
	if(offset < 1)
		return nullptr;
	if((tokenIt + (offset - 1)) >= tokenVec->end())
		return nullptr;

	return *(tokenIt + (offset - 1));
}

void
Parser::parseStruct()
{
	fetchToken();

	if(currentToken->mCat != kCat_WildIdentifier)
		lerror(kE_Error, currentToken, "Struct declaration must be followed by valid identifier!");
	
	if(scopeStack.top()->structDefinitions[currentToken->mStr] != nullptr)
		lerror(kE_Fatal, currentToken, "Struct re-declaration!");
	
	/*Structure* structDef = new Structure;	
	StructType* structTyping = new StructType(structDef);
	
	structTyping->name = currentToken->mStr;
*/
	fetchToken();
	if(currentToken->mType != kToken_LCURLY)
		lerror(kE_Error, currentToken, "invalid token following struct declaration");

	fetchToken();
	while(currentToken->mType != kToken_RCURLY)
	{

		fetchToken();
	}

	

}


void
Parser::parseIntoScope(Scope* theScope)
{
	Token* prevToken = currentToken;
	while(currentToken != nullptr)
	{
		if(currentToken->mCat == kCat_Keyword)
		{
			if(currentToken->mType == kToken_STRUCT)
			{
				parseStruct();
			}
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

	scopeStack.push(globalScope);
	parseIntoScope(globalScope);


	return globalScope;
}