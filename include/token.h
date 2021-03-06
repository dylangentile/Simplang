#pragma once
#include <vector>
#include "scanner.h"

typedef enum
{
	kCat_NULL,
	kCat_OPERATOR,
	kCat_KEYWORD,
	kCat_VALUE,
	kCat_IDENTIFIER,
	kCat_EOS, //End Of Statement
	kCAT_OPENSCOPE,
	kCAT_CLOSESCOPE,
	kCat_UNKNOWN,
	kCat_TYPE
}TokenCAT;

typedef enum 
{
    kToken_COMMENT,
    kToken_IDENTIFIER,
	kToken_EOF,
	kToken_UNKNOWN,
	kToken_SEMICOLON,
	kToken_COMMA,
	kToken_STRING,
	kToken_NUMBER,
	kToken_NUMBER_FP, //floating point
	kToken_NUMBER8,
	kToken_NUMBER16,
	kToken_NUMBER32,
	kToken_NUMBER64,
	kToken_POINTER,
	kToken_BOOL,
	kToken_TYPENAME,
	kToken_DYNAMIC,
	kToken_PRINT,
	kToken_IF,
	kToken_ELSE,
	kToken_LCURLY,
	kToken_RCURLY,
	kToken_EQUALS,
	kToken_PLUS_EQUALS,
	kToken_MINUS_EQUALS,
	kToken_MULTIPLY_EQUALS,
	kToken_DIVIDE_EQUALS,
	kToken_POWER_EQUALS,
	kToken_MOD_EQUALS,
	kToken_LPAREN,
	kToken_VOID,
	kToken_RPAREN,
	kToken_ADD,
	kToken_SUBTRACT,
	kToken_MULTIPLY,
	kToken_DIVIDE,
	kToken_MODULO,
	kToken_POWER,
	kToken_EQUALITY,
	kToken_LESS,
	kToken_GREATER,
	kToken_NOT,
	kToken_LESS_EQUAL,
	kToken_GREATER_EQUAL,
	kToken_NOT_EQUAL,
	kToken_LOGIC_AND,
	kToken_LOGIC_OR,
	kToken_OBJECT,
	kToken_NULL

} TokenID;

typedef struct{
	std::string cargo, *sourceName;
	std::string *sourceText;
	int *lineIndex, *colIndex, *strIndex; 
	std::vector<Character*> cObjArray;
	TokenID type, subType;
	TokenCAT cat;
	//bool funcId = false;
	void charPass(Character *x)
	{
		subType = kToken_NULL;
		lineIndex = &(x->lineIndex);
		colIndex = &(x->columnIndex);
		strIndex = &(x->sourceIndex);
		sourceText = x->sourceText; //already a pointer
		sourceName = &(x->memberOfFile);	
		cObjArray.clear();
		cObjArray.push_back(x);
		cargo = x->cargo;
	}
	void addChar(Character *x)
	{
		cargo += x->cargo;
		cObjArray.push_back(x);
	}

	void fixTypes()
	{
		if (type == kToken_NUMBER && subType == kToken_NULL)
		{
			subType = kToken_NUMBER64;//default number prescision
		}
		else if (type == kToken_NUMBER8 ||
			type == kToken_NUMBER16 ||
			type == kToken_NUMBER32 ||
			type == kToken_NUMBER64 ||
			type == kToken_NUMBER_FP)
		{
			subType = type;
			type = kToken_NUMBER;
		}
	}

} Token;


typedef struct
{
	Token* mType, *mIdentifier, *overload = nullptr;
}Arguement;






