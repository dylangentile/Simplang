#pragma once
#include "tokenenums.h"


#include <cstddef>
#include <string>

typedef struct DebugData
{
	const char* linePtr = nullptr;
	uint32_t offset = 0;
	uint32_t lineNum = 0;
	uint32_t length = 0;

}DebugData;



typedef struct Token
{
	TokenType mType = kToken_NULL;
	TokenCategory mCat = kCat_NULL;

	DebugData mData;
	std::string mStr;
}Token;


