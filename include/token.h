#pragma once
#include <vector>
#include "scanner.h"
typedef enum{
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
	kToken_NUMBER128,
	kToken_POINTER,
	kToken_BOOL,
	kToken_PRINT

} TokenID;

typedef struct{
	std::string cargo, *sourceName;
	std::string *sourceText;
	int *lineIndex, *colIndex, *strIndex; 
	std::vector<Character*> cObjArray;
	TokenID type;
	TokenCAT cat;
	void charPass(Character *x)
	{
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
} Token;





