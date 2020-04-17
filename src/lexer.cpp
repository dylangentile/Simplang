#include "lexer.h"
#include "error.h"
#include <fstream>


Lexer::Lexer(const char* filename)
{
	std::ifstream ifs(filename);
	if(!ifs)
		lerror(kE_Fatal, nullptr, "Invalid file path!");
	mContent = std::string((std::istreambuf_iterator<char>(ifs) ),
						   (std::istreambuf_iterator<char>()   ));

	fileIt = mContent.begin();


	



	typeMap.insert(kToken_STRING, "string");
	typeMap.insert(kToken_BYTE, "byte");
	typeMap.insert(kToken_BOOL, "bool");
	
	typeMap.insert(kToken_INT8, "int8");
	typeMap.insert(kToken_INT16, "int16");
	typeMap.insert(kToken_INT32, "int32");
	typeMap.insert(kToken_INT64, "int64");
	
	typeMap.insert(kToken_UINT8, "uint8");
	typeMap.insert(kToken_UINT16, "uint16");
	typeMap.insert(kToken_UINT32, "uint32");
	typeMap.insert(kToken_UINT64, "uint64");
	
	typeMap.insert(kToken_SINGLE, "float");
	typeMap.insert(kToken_DOUBLE, "double");
	
	typeMap.insert(kToken_DYNAMIC, "dynamic");
	typeMap.insert(kToken_TYPENAME, "typename");
	
	typeMap.insert(kToken_PTR, "ptr");
	keywordMap.insert(kToken_UNIQUE, "unique");
	keywordMap.insert(kToken_SHARED, "shared");

	keywordMap.insert(kToken_IF, "if");
	keywordMap.insert(kToken_ELSE, "else");
	keywordMap.insert(kToken_FOR, "for");
	keywordMap.insert(kToken_WHILE, "while");
	keywordMap.insert(kToken_MATCH, "match");

	keywordMap.insert(kToken_ENUM, "enum");
	keywordMap.insert(kToken_STRUCT, "struct");
	
	
	langOperatorMap.insert(kToken_SEMICOLON, ";");
	langOperatorMap.insert(kToken_COLON, ":");
	langOperatorMap.insert(kToken_COMMA, ",");
	langOperatorMap.insert(kToken_LPAREN, "(");
	langOperatorMap.insert(kToken_RPAREN, ")");
	langOperatorMap.insert(kToken_LCURLY, "{");
	langOperatorMap.insert(kToken_RCURLY, "}");
	langOperatorMap.insert(kToken_LBRACKET, "[");
	langOperatorMap.insert(kToken_RBRACKET, "]");


	operatorMap.insert(kToken_ASSIGN_EQUAL, "=");
	operatorMap.insert(kToken_PLUS, "+");
	operatorMap.insert(kToken_MINUS, "-");
	operatorMap.insert(kToken_MULTIPLY, "*");
	operatorMap.insert(kToken_DIVIDE, "/");
	operatorMap.insert(kToken_MODULO, "%");

	operatorMap.insert(kToken_LOGIC_EQUAL, "==");
	operatorMap.insert(kToken_LESS, "<");
	operatorMap.insert(kToken_GREATER, ">");
	operatorMap.insert(kToken_NOT, "!");

	operatorMap.insert(kToken_LESS_EQUAL, "<=");
	operatorMap.insert(kToken_GREATER_EQUAL, ">=");
	operatorMap.insert(kToken_NOT_EQUAL, "!=");

	operatorMap.insert(kToken_LOGIC_AND, "&&");
	operatorMap.insert(kToken_LOGIC_OR, "||");

	operatorMap.insert(kToken_DEREF, "@");

	operatorMap.insert(kToken_BIT_AND, "&");
	operatorMap.insert(kToken_BIT_OR, "|");



}

Lexer::~Lexer()
{
	for(auto it = debugDataVec.begin(); it != debugDataVec.end(); it++)
	{
		delete *it;
	}

	for(auto it = tokenVec.begin(); it != tokenVec.end(); it++)
	{
		delete *it;
	}
}




inline bool
isAlpha(const char& c)
{
	return  ('a' <= c && c <= 'z') || 
			('A' <= c && c <= 'Z');
}

inline bool
isNumeric(const char& c)
{
	return '0' <= c && c <= '9';
}


inline bool 
isAlphaNumeric(const char& c)
{
	return isNumeric(c) || isAlpha(c);
}

inline bool 
isValidIdentifierChar(const char& c)
{
	return isAlphaNumeric || c == '_'; 
}


void 
Lexer::fetchChar()
{
	if(fileIt == mContent.end())
	{
		c1 = 0; c2 = 0;
		return;
	}

	c1 = *fileIt;
	fileIt++;
	if(fileIt == mContent.end())
		c2 = 0;
	else
		c2 = *fileIt;
}

Token*
Lexer::fetchToken()
{

	fetchChar();

	if(c1 == '/' && c2 == '/')
	{
		fetchChar();
	}


	return nullptr;
}



std::vector<Token*>*
Lexer::lex()
{
	Token* theToken = nullptr;
	while(true)
	{
		if((theToken = fetchToken()) == nullptr)
			break;
			
		if(theToken->mCat == kCat_NULL || theToken->mCat == kCat_UNKNOWN)
			lerror(kE_Error, theToken->mData, "unknown token!");

		tokenVec.push_back(theToken);
	}
	return &tokenVec;
}






















