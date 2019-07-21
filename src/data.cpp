#include "data.h"
#include "error.h"
#include <iostream>
#include <cstdlib>
using namespace std;
#define DEBUG

#ifndef DEBUG
#ifndef PACKAGE_STRING
#error "Not built with autotools!"
#endif
#else
#define PACKAGE_STRING "simplang debug build"
#endif
Data::Data() : printValues(false), printWarnings(true), printTokens(false), hush(false)
{

}

Data::~Data(){

}

vector<string>
Data::parseArgs(int count, const char *args[])
{
	vector<string> rawArgVector;
	vector<string> fileVector;
	for(int i = 1; i < count; ++i)
	{
		rawArgVector.push_back(string(args[i]));
	}
	while(!rawArgVector.empty())
	{
		for(auto it = rawArgVector.begin(); it != rawArgVector.end(); ++it)
		{
			if(*it == "-o")
			{
				if(it + 1 == rawArgVector.end())
					error(7, "Invalid usage of -o!", true);
				
				outputBinName = *(it + 1);
				rawArgVector.erase(it + 1);
				
			}
			else if(*it == "--pf")
			{
				printValues = true;
			}
			else if(*it == "--hush-warnings")
			{
				printWarnings = false;
			}
			else if(*it == "--print-tokens")
			{
				printTokens = true;
			}
			else if(*it == "--hush-all")
			{
                hush = true;
			}
			else if(*it == "--version" || *it == "-v")
			{
				std::cout << PACKAGE_STRING << "\n";
				exit(0);
			}
			else
			{
				if((*it)[0] == '-')
				{
					error(7, "Invalid flag: " + *it, true);
				}

				fileVector.push_back(*it);
			}
			rawArgVector.erase(it);
			break;
		}
	}

	if(hush)
	{
		printValues = false;
		printWarnings = false;
		printTokens = false;
	}
	if(fileVector.empty())
	{
		error(7, "No input files!", true);
	}

	return fileVector;
}
