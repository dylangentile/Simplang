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


void error(unsigned id = 666, std::string theMsg = "NULL", bool stop = false, Token* mToken = nullptr);
std::string errorOut();
void warning(unsigned id = 6, std::string theMsg = "NULL", Token* mToken = nullptr);
std::string warningOut();

bool isErrors();