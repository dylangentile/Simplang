#include "analyze.h"
#include "error.h"
#include <deque>


std::deque<Scope*> scopeStack;

void
validateStatement(Statement* state)
{
	switch(state->mId)
	{
		case kState_NULL: lerror(kE_Fatal, state, "compiler error: null statement!");
		break;
		case kState_Scope: validateSymbols(dynamic_cast<Scope*>(state));
		break;
		case kState_Variable:
		break;
		case kState_VariableAccess:
		{
			VariableAccess* varRef = dynamic_cast<VariableAccess*>(state);	
			for(auto it = scopeStack.rbegin(); it != scopeStack.rend(); it++)
			{
				auto finder = (*it)->variableDecl.find(varRef->mName);
				if(finder != (*it)->variableDecl.end())
				{
					varRef->refrencing = finder->second;
					break;
				}

			}
		}
		break;
		case kState_Structure: //we don't need to do anything
		break;
		case kState_Function: //don't need to do anything
		break;
		case kState_FunctionCall: 
		{
			//find the function it is calling
			FunctionCall* funcCall = dynamic_cast<FunctionCall*>(state);
			for(auto it = scopeStack.rbegin(); it != scopeStack.rend(); it++)
			{
				auto finder = (*it)->functionMap.find(funcCall->callName);
				if(!(*it)->functionMap.isEnd(finder))
				{
					funcCall->parentFunc = finder->second;
					break;
				}
			}

			if(funcCall->parentFunc == nullptr)
				lerror(kE_Error, state, "Unknown function!");

		
		}
		break;

		case kState_If:
		break;

		case kState_ImmediateExpr:
		break;
		case kState_BinOpExpr:
		break;

		case kState_DeclEqual:
		break;
		case kState_MultipleAssignment:
		break;
		case kState_Return:
		break;

		case kState_StatementList:
		break;
		case kState_Term:
		break;

	}
}


void
validateSymbols(Scope* scope)
{
	scopeStack.push_back(scope);
	for(Statement* state : scope->statementVec)
	{
		validateStatement(state);
	}
	scopeStack.pop_back();
}

void
typeAnalysis()
{
	
}
