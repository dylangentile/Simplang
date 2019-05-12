#pragma once
#include <vector>
#include "scanner.h"
typedef enum{
	kCat_OPERATOR,
	kCat_KEYWORD,
	kCat_VALUE,
	kCat_IDENTIFIER,
	kCat_EOS, //End Of Statement
	kCat_COMMA,
	kCat_UNKNOWN,
	kCat_TYPE
}TokenCAT;

typedef enum 
{
	
} TokenID;

typedef struct{
	std::string cargo, sourceText, sourceName;
	int lineIndex, colIndex; 
	std::vector<Character*> cObjArray;
	TokenID type;
	TokenCAT cat;
} Token;


