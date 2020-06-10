#include "lexer.h"
#include "error.h"
#include <fstream>


Lexer::Lexer(const char* filename)
{
	std::ifstream ifs(filename);
	if(!ifs)
		lerrorSTR(kE_Fatal, "Invalid file path!");
	mContent = std::string((std::istreambuf_iterator<char>(ifs) ),
						   (std::istreambuf_iterator<char>()   ));

	fileIt = mContent.begin();


	


	typeMap.insert(kToken_VOID, "void");
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
	keywordMap.insert(kToken_FANCY,  "fancy");

	keywordMap.insert(kToken_IF, "if");
	keywordMap.insert(kToken_ELSE, "else");
	keywordMap.insert(kToken_FOR, "for");
	keywordMap.insert(kToken_WHILE, "while");
	keywordMap.insert(kToken_MATCH, "match");
	keywordMap.insert(kToken_IS, "is");


	keywordMap.insert(kToken_BREAK, "break");
	keywordMap.insert(kToken_CONTINUE, "continue");
	keywordMap.insert(kToken_RETURN, "return");


	keywordMap.insert(kToken_ENUM, "enum");
	keywordMap.insert(kToken_STRUCT, "struct");

	keywordMap.insert(kToken_USE, "use");
	keywordMap.insert(kToken_IMPORT, "import");
	keywordMap.insert(kToken_AS, "as");
	keywordMap.insert(kToken_UNIT, "unit");
	keywordMap.insert(kToken_CINCLUDE, "cinclude");
	
	keywordMap.insert(kToken_PUBLIC, "public");
	keywordMap.insert(kToken_PRIVATE, "private");

	keywordMap.insert(kToken_ISNULL, "isnull");
	keywordMap.insert(kToken_SIZEOF, "sizeof");
	
	langOperatorMap.insert(kToken_SEMICOLON, ";");
	langOperatorMap.insert(kToken_COLON, ":");
	langOperatorMap.insert(kToken_COMMA, ",");
	langOperatorMap.insert(kToken_LPAREN, "(");
	langOperatorMap.insert(kToken_RPAREN, ")");
	langOperatorMap.insert(kToken_LCURLY, "{");
	langOperatorMap.insert(kToken_RCURLY, "}");
	langOperatorMap.insert(kToken_LBRACKET, "[");
	langOperatorMap.insert(kToken_RBRACKET, "]");

	operatorMap.insert(kToken_DECLARE_EQUAL, ":=");

	operatorMap.insert(kToken_ASSIGN_EQUAL, "=");
	operatorMap.insert(kToken_PLUS, "+");
	operatorMap.insert(kToken_MINUS, "-");
	operatorMap.insert(kToken_MULTIPLY, "*");
	operatorMap.insert(kToken_DIVIDE, "/");
	operatorMap.insert(kToken_MODULO, "%");

	operatorMap.insert(kToken_PLUS_EQUAL, "+=");
	operatorMap.insert(kToken_MINUS_EQUAL, "-=");
	operatorMap.insert(kToken_MULTIPLY_EQUAL, "*=");
	operatorMap.insert(kToken_DIVIDE_EQUAL, "/=");
	operatorMap.insert(kToken_MODULO_EQUAL, "%=");

	operatorMap.insert(kToken_INCREMENT, "++");
	operatorMap.insert(kToken_DECREMENT, "--");

	operatorMap.insert(kToken_LOGIC_EQUAL, "==");
	operatorMap.insert(kToken_LESS, "<");
	operatorMap.insert(kToken_GREATER, ">");
	operatorMap.insert(kToken_NOT, "!");

	operatorMap.insert(kToken_LESS_EQUAL, "<=");
	operatorMap.insert(kToken_GREATER_EQUAL, ">=");
	operatorMap.insert(kToken_NOT_EQUAL, "!=");

	operatorMap.insert(kToken_LOGIC_AND, "&&");
	operatorMap.insert(kToken_LOGIC_OR, "||");

	operatorMap.insert(kToken_ADDR, "@");
	operatorMap.insert(kToken_DOT, ".");

	operatorMap.insert(kToken_SFNAV, "?.");
	operatorMap.insert(kToken_NULLCC,"??");


	operatorMap.insert(kToken_BIT_AND, "&");
	operatorMap.insert(kToken_BIT_OR, "|");
	operatorMap.insert(kToken_BIT_COMPL, "~");


	linePointer = mContent.c_str();
}

Lexer::~Lexer()
{

	for(auto it = tokenVec.begin(); it != tokenVec.end(); it++)
	{
		delete *it;
	}
}


inline bool
isWhitespace(const char& c)
{
	return  c == ' ' ||
			c == '\n'||
			c == '\t'||
			c == '\r';
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
	return isAlphaNumeric(c) || c == '_'; 
}


void 
Lexer::fetchChar()
{
	if(fileIt == mContent.end())
	{
		c1 = 0; c2 = 0;
		return;
	}

	if(c1 == '\n')
	{
		currentLine++;
		currentCol = 0;
		linePointer = mContent.c_str() + std::distance(mContent.begin(), fileIt);
	}

	if(c1 == '\t')
	{
		currentCol += 7;
		(*counter) += 7;
	}

	c1 = *fileIt;
	fileIt++;
	if(fileIt == mContent.end())
		c2 = 0;
	else
		c2 = *fileIt;

	(*counter)++;
	currentCol++;
}

Token*
Lexer::fetchToken()
{

	//fetchChar();

	charCleanup:

	if(c1 == 0)
		return nullptr;

	while(isWhitespace(c1))
		fetchChar();

	if(c1 == '/' && c2 == '/')
	{
		while(c1 != '\n' && c1 != '\0')
			fetchChar();
		goto charCleanup;
	}

	

	Token* theToken = new Token;
	theToken->mData.linePtr = linePointer;
	theToken->mData.offset = currentCol;
	theToken->mData.lineNum = currentLine;
	counter = &theToken->mData.length;

	if(isAlpha(c1) || c1 == '_')
	{
		while(isValidIdentifierChar(c1))
		{
			theToken->mStr.push_back(c1);
			fetchChar();
		}
		TokenType* type = nullptr;
		if(theToken->mStr == "int")
		{
			theToken->mCat = kCat_BasicType;
			theToken->mType = kToken_INT32;
		}
		else if((type = typeMap[theToken->mStr]) != nullptr)
		{
			theToken->mCat = kCat_BasicType;
			theToken->mType = *type;
		}
		else if((type = keywordMap[theToken->mStr]) != nullptr)
		{
			theToken->mCat = kCat_Keyword;
			theToken->mType = *type;
		}
		else
		{
			theToken->mCat = kCat_WildIdentifier;
			theToken->mType = kToken_UNKNOWN;
		}

		/*if(theToken->mType == kToken_PTR)
			lerror(kE_Error, theToken, "ptr is a reserved keyword!");*/

		return theToken;
	}

	theToken->mStr = "";
	theToken->mStr.push_back(c1);
	theToken->mStr.push_back(c2);
	TokenType* type = nullptr;
	
	if((type = operatorMap[theToken->mStr]) != nullptr)
	{
		theToken->mCat = kCat_Operator;
		theToken->mType = *type;
		fetchChar();
		fetchChar();
		return theToken; 
	}

	theToken->mStr.erase(theToken->mStr.end() - 1);

	if((type = langOperatorMap[theToken->mStr]) != nullptr)
	{
		theToken->mCat = kCat_LanguageOperator;
		theToken->mType = *type;
		fetchChar();

		return theToken;
	}


	if((type = operatorMap[theToken->mStr]) != nullptr)
	{
		theToken->mCat = kCat_Operator;
		theToken->mType = *type;
		fetchChar();

		return theToken; 
	}

	





	if(c1 == '\"')
	{
		fetchChar();
		theToken->mStr.clear();
		while(c1 != '\"')
		{
			if(c1 == 0 || c1 == '\n')
			{
				lerror(kE_Error, theToken, "String not terminated before EOF or newline!");
				theToken->mCat = kCat_UNKNOWN;
				return theToken;
			}
			theToken->mStr.push_back(c1);
			fetchChar();
		}

		fetchChar();
		theToken->mCat = kCat_Immediate;
		theToken->mType = kToken_STRING;
		return theToken;
	}

	if(c1 == '0' && c2 == 'x')
	{
		fetchChar();
		theToken->mStr.push_back(c1);
		fetchChar();
		while(isNumeric(c1) || ('A' <= c1 && c1 <= 'F') || ('a' <= c1 && c1 <= 'f'))
		{
			theToken->mStr.push_back(c1);
			fetchChar();
		}

		theToken->mCat = kCat_Immediate;
		theToken->mType = kToken_UINT64;
		return theToken;
	}

	if(isNumeric(c1))
	{
		fetchChar();
		while(isNumeric(c1))
		{
			theToken->mStr.push_back(c1);
			fetchChar();
		}

		theToken->mCat = kCat_Immediate;
		theToken->mType = kToken_UINT64;
		return theToken;
	}


	if(c1 == 0)
		return nullptr;
	fetchChar();
	return theToken;
}



std::vector<Token*>*
Lexer::lex()
{
	uint32_t temp = 0;
	counter = &temp;
	Token* theToken = nullptr;
	fetchChar();
	while(true)
	{
		if((theToken = fetchToken()) == nullptr)
			break;
			
		if(theToken->mCat == kCat_NULL)
			lerror(kE_Error, theToken, "unknown token!");

		tokenVec.push_back(theToken);
		counter = &temp;
		temp = 0;
	}


	for(auto it = tokenVec.begin(); it != tokenVec.end(); it++)
	{
		theToken = *it;
		switch(theToken->mCat)
		{
			case kCat_NULL : printf("NULL Token: ");
			break;
			case kCat_UNKNOWN : printf("UNKNOWN Token: ");
			break;
			case kCat_Keyword : printf("Keyword: ");
			break;
			case kCat_WildIdentifier : printf("Identifier: ");
			break;
			case kCat_Immediate : printf("Immediate: ");
			break;
			case kCat_Operator : printf("Operator: ");
			break;
			case kCat_LanguageOperator : printf("LangOp: ");
			break;
			case kCat_BasicType : printf("BasicType: ");
			break;
			default: printf("????: ");
		}

		printf("%s\n", theToken->mStr.c_str());
	}
	printf("\n\n");
	return &tokenVec;
}
