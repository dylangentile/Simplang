#pragma once
#include <vector>
#include "scanner.h"
typedef enum{
	kCat_OPERATOR,
	kCat_KEYWORD,
	kCat_VALUE,
	kCat_IDENTIFIER,
	kCat_EOS, //End Of Statement
	kCat_UNKNOWN,
	kCat_TYPE
}TokenCAT;

typedef enum 
{
	kToken_EOF,
	kToken_UNKNOWN

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
} Token;





