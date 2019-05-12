#pragma once
#include <string>

class ErrorData{
public:
	ErrorData();
	~ErrorData();
	std::string msg;
	unsigned errorCount;
};


void error(unsigned id, std::string theMsg);
std::string errorOut();