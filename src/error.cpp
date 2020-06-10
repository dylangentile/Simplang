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
ErrorManager::logDB(ErrorType type, DebugData* loc, const char* msg)
{
	assert(gErrorManager != nullptr && "Need to create the error manager!");

	std::string err;
	if(loc != nullptr)
	{

		const char* ptr = loc->linePtr;
		if(ptr == nullptr)
		{
			goto ZAWARUDO;
		}


		err = "Line #" + std::to_string(loc->lineNum) + ": " + std::string(msg) + "\n\t";
		
		while(*ptr != '\0' && *ptr != '\n')
		{
			err.push_back(*ptr);
			ptr++;
		}

		err += "\n\t";
		for(uint32_t i = 1; i < loc->offset; i++)
			err.push_back(' ');
		err.push_back('^');

		for(uint32_t i = 1; i < loc->length; i++)
			err.push_back('~');
	}
	else
	{
		ZAWARUDO:
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

void
ErrorManager::logError(ErrorType type, Token* location, const char* msg)
{
	assert(gErrorManager != nullptr && "Need to create the error manager!");

	logDB(type, &location->mData, msg);
	


}

void 
ErrorManager::logError(ErrorType type, Statement* state, const char* msg)
{
	assert(gErrorManager != nullptr && "Need to create the error manager!");

	logDB(type, &state->debug, msg);

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















