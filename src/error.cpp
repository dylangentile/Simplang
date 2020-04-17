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
ErrorManager::logError(ErrorType type, DebugData* location, const char* msg)
{
	assert(gErrorManager != nullptr && "Need to create the error manager!");

	std::string err;

	if(location != nullptr)
	{
		err = "Line #" + std::to_string(location->lineNum) + ": " + std::string(msg) + "\n\t";
		for(const char* ptr = location->linePtr; *ptr != '\n' && *ptr != '\0'; ptr++)
		{
			err.push_back(*ptr);
		}
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
		abort();
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