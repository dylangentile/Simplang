#pragma once
#include "token.h"
#include "statement.h"

typedef enum
{
	kE_Error,
	kE_Warning,
	kE_Fatal
}ErrorType;


class ErrorManager
{
private:
	ErrorManager();
	~ErrorManager();
	//static void generateWarning(const char* msg);
	//static void generateError(const char* mesg);
public:
	//static void logError(ErrorType type, const char* msg, ...);
	static ErrorManager* create();
	static void destroy();
	static void logDB(ErrorType type, DebugData* loc, const char* msg); 
	static void logError(ErrorType type, Token* location, const char* msg);
	static void logError(ErrorType type, Statement* state, const char* msg);
	//inline static void logError(ErrorType type, Token* token, const char* msg){ logError(type, token->mData, msg);}
	static bool haveErrors();
	static std::string report();


	static void resetCounter();
	static bool gotErrors();
	
private:
	static ErrorManager* gErrorManager;

	std::string warnings;
	std::string errors;

	uint32_t warningCount;
	uint32_t errorCount;

	uint32_t counter;
};

#define lerror(type, location, msg) ErrorManager::logError(type, location, msg);
#define lwarning(location, msg) ErrorManager::logError(kE_Warning, location, msg);
#define lerrorSTR(type, msg) ErrorManager::logDB(type, nullptr, msg);


