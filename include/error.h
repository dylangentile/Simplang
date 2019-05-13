#pragma once
#include "token.h"
#include <string>

class ErrorData{
public:
	ErrorData();
	~ErrorData();
	std::string msg;
	unsigned errorCount;
};


void error(unsigned id = 666, std::string theMsg = "NULL", Token *mToken = nullptr);
std::string errorOut();