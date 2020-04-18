#pragma once
#include "typeenums.h"


#include <cstddef>
#include <string>

class DebugData
{
public:
	DebugData(const char* lptr, uint32_t lnum) : linePtr(lptr), lineNum(lnum) {}
	~DebugData() = default;
	const char* linePtr;
	uint32_t lineNum;
};



typedef struct
{
	TokenType mType = kToken_NULL;
	TokenCategory mCat = kCat_NULL;

	DebugData* mData = nullptr;
	std::string mStr;
	uint32_t colNum = 0;
}Token;


