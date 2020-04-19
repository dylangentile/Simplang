#include "error.h"
#include <cassert>
#include <cstdlib>
#include <cstdio>
ErrorManager* ErrorManager::gErrorManager = nullptr;


ErrorManager::ErrorManager() : warningCount(0), errorCount(0)
{

}


ErrorManager::~ErrorManager()
{

}

ErrorManager*
ErrorManager::create()
{
	if(gErrorManager == nullptr)
		gErrorManager = new ErrorManager;
	return gErrorManager;
}

void
ErrorManager::destroy()
{
	if(gErrorManager != nullptr)
		delete gErrorManager;
}

void
ErrorManager::logError(ErrorType type, Token* location, const char* msg)
{
	assert(gErrorManager != nullptr && "Need to create the error manager!");

	std::string err;

	if(location != nullptr)
	{
		err = "Line #" + std::to_string(location->mData->lineNum) + ": " + std::string(msg) + "\n\t";// + " '" + location->mStr + "'\n\t";
		const char* ptr = location->mData->linePtr;
		while(*ptr != 0 && *ptr != '\0' && *ptr != '\n')
		{
			err.push_back(*ptr);
			ptr++;
		}
		err += "\n\t";
		for(int i = 1; i < location->colNum; i++)
			err.push_back(' ');
		err.push_back('^');

		int offset = location->mCat == kCat_Immediate && location->mType == kToken_STRING ? 2 : 0;

		for(int i = 1; i < location->mStr.size() + offset; i++)
			err.push_back('~');

	}
	else
	{
		err = std::string(msg);
	}
	err += "\n\n";
	

	if(type == kE_Error || type == kE_Fatal)
	{
		gErrorManager->errorCount++;
		gErrorManager->errors += err;
	}
	else
	{
		gErrorManager->warningCount++;
		gErrorManager->warnings += err;
	}

	if(type == kE_Fatal)
	{
		printf("%s\n", gErrorManager->report().c_str());
		exit(0);
	}


}

bool
ErrorManager::haveErrors()
{
	assert(gErrorManager != nullptr && "Need to create the error manager!");
	return gErrorManager->errorCount > 0;
}



std::string
ErrorManager::report()
{
	assert(gErrorManager != nullptr && "Need to create the error manager!");
	std::string retmsg = gErrorManager->warnings + gErrorManager->errors;
	retmsg += "slc exited with: " + std::to_string(gErrorManager->warningCount) + " warnings and " + std::to_string(gErrorManager->errorCount) + " errors.";
	return retmsg;
}





void
ErrorManager::resetCounter()
{
	assert(gErrorManager != nullptr && "Need to create the error manager!");
	gErrorManager->counter = 0;
}

bool
ErrorManager::gotErrors()
{
	assert(gErrorManager != nullptr && "Need to create the error manager!");
	return gErrorManager->counter > 0;
}















