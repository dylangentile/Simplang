#include "error.h"
#include "lexer.h"
#include <cstdio>




int main(int argc, const char *argv[])
{
	ErrorManager::create();
	
	Lexer* myLexer = new Lexer(argv[1]);


	if(ErrorManager::haveErrors())
		printf("%s\n", ErrorManager::report().c_str());
	ErrorManager::destroy();
	return 0;
}