#include "error.h"
#include "parser.h"
#include <cstdio>

inline void printTabs(int x)
{
	for(int i = 0; i < x; i++) printf("\t");
}
void printScope(Scope* scope, int tabs = 0);


void printType(Type* t)
{
	if(t == nullptr)
	{
		printf("? ");
	}
	else if(t->mId == kType_Basic)
	{
		BasicType* x = dynamic_cast<BasicType*>(t);
		switch(x->basicId)
		{
			case kBaseT_VOID: 	printf("void   ");
			break;
			case kBaseT_STRING: printf("string ");
			break;
			case kBaseT_BYTE: 	printf("bytes  ");
			break;
			case kBaseT_BOOL: 	printf("bool   ");
			break;
			case kBaseT_INT8: 	printf("int8   ");
			break;
			case kBaseT_INT16: 	printf("int16  ");
			break;
			case kBaseT_INT32: 	printf("int32  ");
			break;
			case kBaseT_INT64: 	printf("int64  ");
			break;
			case kBaseT_UINT8: 	printf("uint8  ");
			break;
			case kBaseT_UINT16: printf("uint16 ");
			break;
			case kBaseT_UINT32: printf("uint32 ");
			break;
			case kBaseT_UINT64: printf("uint64 ");
			break;
			case kBaseT_SINGLE: printf("single ");
			break;
			case kBaseT_DOUBLE: printf("double ");
			break;
			case kBaseT_DYNAMIC:printf("dynamic ");
			break;
			case kBaseT_TYPENAME:printf("typename ");
			break;
		}
	}
	else if(t->mId == kType_Multiple)
	{
		MultipleType* mult = dynamic_cast<MultipleType*>(t);
		for(Type* x : mult->typeVec)
			printType(x);
	}
}



void printStatement(Statement* state, int tabs)
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
		printType(var->mType);
		printf("Variable %s", var->mName.c_str());

		if(var->mInitializer != nullptr)
		{
			printf(" = ");
			printStatement(var->mInitializer, tabs);
		}
	}
	else if(state->mId == kState_Return)
	{
		printf("return ");
	}
	else if(state->mId == kState_DeclEqual)
	{
		DeclEqual* dEq = dynamic_cast<DeclEqual*>(state);

		for(Variable* var : dEq->varVec)
		{
			printType(var->mType);
			printf("%s ", var->mName.c_str());
		}
		printf(":= ");
		//print expr
	}
	else if(state->mId == kState_BinOpExpr)
	{
		BinOp* theOp = dynamic_cast<BinOp*>(state);
		printStatement(theOp->operand1, 0);
		printStatement(theOp->operand2, 0);

		switch(theOp->mOp)
		{
			case kOp_ADD: printf("+ ");
			break;
			case kOp_SUB: printf("- ");
			break;
			case kOp_MUL: printf("* ");
			break;
			case kOp_DIV: printf("/ ");
			break;
			case kOp_MOD: printf("%% ");
			break;

			case kOp_L_AND: printf("&& ");
			break;
			case kOp_L_OR: printf("|| ");
			break;
			case kOp_L_EQUAL: printf("== ");
			break;
			case kOp_L_NOT_EQUAL: printf("!= ");
			break;

			case kOp_LESS: printf("< ");
			break;
			case kOp_GREATER: printf("> ");
			break;
			case kOp_LESS_EQUAL: printf("<= ");
			break;
			case kOp_GREATER_EQUAL: printf(">= ");
			break;

			case kOp_BIT_AND: printf("& ");
			break;
			case kOp_BIT_OR: printf("| ");
			break;

			case kOp_NULLCC: printf("?? ");
			break;
		}

	}
	else if(state->mId == kState_ImmediateExpr)
	{
		Immediate* val = dynamic_cast<Immediate*>(state);
		printf("%s ", val->ogString.c_str());
	}
	

}

void printScope(Scope* scope, int tabs)
{

	for(auto it = scope->structMap.map1.begin(); it != scope->structMap.map1.end(); it++)
	{
		printTabs(tabs);
		printf("struct ");
		printf("%s\n", it->first->definition->name.c_str());
		printTabs(tabs + 1);
		for(Variable* var : it->first->definition->members)
		{
			printType(var->mType);
			printf("%s, ", var->mName.c_str());
		}
		printf("\n");
	}

	for(auto it = scope->functionMap.map1.begin(); it != scope->functionMap.map1.end(); it++)
	{
		printTabs(tabs);
		printType(it->first->mType);
		printf("Function (");
		for(Variable* arg : it->first->args)
		{
			printType(arg->mType);
			printf("%s ", arg->mName.c_str());
		}
		printf(")\n");
		printScope(it->first->mBody, tabs + 1);
	}

	for(Statement* state : scope->statementVec)
	{
		printStatement(state, tabs);
		printf("\n");
	}



}


int main(int argc, const char *argv[])
{
	ErrorManager::create();
	
	if(argc != 2)
		lerrorSTR(kE_Fatal, "No source file supplied!");

	Parser* myParser = new Parser(argv[1]);
	Scope* tree = myParser->parse();



	//if(ErrorManager::haveErrors())
		printf("%s\n", ErrorManager::report().c_str());
	ErrorManager::destroy();

	printf("\n\n");
	printScope(tree);
	printf("\n\n");

	return 0;
}