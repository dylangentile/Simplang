#include "error.h"
#include "parser.h"
#include <cstdio>




int main(int argc, const char *argv[])
{
	ErrorManager::create();
	
	Parser* myParser = new Parser(argv[1]);
	myParser->parse();

	//if(ErrorManager::haveErrors())
		printf("%s\n", ErrorManager::report().c_str());
	ErrorManager::destroy();
	//exit(0);
	return 0;
}