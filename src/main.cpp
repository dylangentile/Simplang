#include "error.h"
#include "parser.h"
#include <cstdio>

inline void printTabs(int x)
{
	for(int i = 0; i < x; i++) printf("\t");
}

void printScope(Scope* scope, int tabs = 0)
{
	for(auto it = scope->functionMap.map1.begin(); it != scope->functionMap.map1.end(); it++)
	{
		printTabs(tabs);
		printf("Function\n");
		printScope(it->first->mBody, tabs + 1);
	}

	for(Statement* state : scope->statementVec)
	{
		printTabs(tabs);

		if(state->mId == kState_NULL)
		{
			printf("NULL");
		}
		else if(state->mId == kState_Scope)
		{
			printScope(dynamic_cast<Scope*>(state), tabs + 1);
		}
		else if(state->mId == kState_Variable)
		{
			Variable* var = dynamic_cast<Variable*>(state);

			if(var->mType != nullptr && var->mType->mId == kType_Basic)
			{
				//Type* 
			}
		}
	}

}


int main(int argc, const char *argv[])
{
	ErrorManager::create();
	
	if(argc != 2)
		lerror(kE_Fatal, nullptr, "No source file supplied!");

	Parser* myParser = new Parser(argv[1]);
	Scope* tree = myParser->parse();



	//if(ErrorManager::haveErrors())
		printf("%s\n", ErrorManager::report().c_str());
	ErrorManager::destroy();

	printf("\n\n");
	//printScope(tree);
	printf("\n\n");

	return 0;
}