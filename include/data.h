#pragma once
#include <vector>
#include <string>
class Data
{
public:
	Data();
	~Data();
	std::vector<std::string> parseArgs(int count, const char *args[]);
	std::string outputBinName;
	bool printValues;
	bool printWarnings;
	bool printTokens;
	bool printTree;
	bool hush;
};

static Data theData;