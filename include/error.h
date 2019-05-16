#pragma once
#include "token.h"
#include <string>

class ErrorData{
public:
	ErrorData(std::string file, bool doColors = true);
	~ErrorData();
	std::string msg;
	std::string srcFile;
	unsigned errorCount;
	bool color;

};


void error(unsigned id = 666, std::string theMsg = "NULL", Token *mToken = nullptr, bool stop = false);
std::string errorOut();