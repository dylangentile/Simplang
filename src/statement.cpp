#include "statement.h"

using namespace std;


Statement::Statement()
{

}

Statement::~Statement()
{

}


string
VarStatement::print()
{
	return mType->cargo + ": " + mName + "=" + mValue->cargo + "\n";
}

bool
VarStatement::compareName(string theName)
{
	if (mName == theName)
		return true;
	return false;
}

FuncStatement::FuncStatement()
{

}

FuncStatement::~FuncStatement()
{

}

string
FuncStatement::print()
{
	string msg = "\nFunction: " + mType->cargo + " " + mName + "(";
	for (vector<Arguement*>::iterator it = mArgArray.begin(); it != mArgArray.end(); it++)
	{
		Arguement* theArg = *it;
		msg += theArg->mType->cargo + " " + theArg->mIdentifier->cargo;
		if (theArg->overload != nullptr)
		{
			msg += " = " + theArg->overload->cargo;
		}
		msg += ", ";
	}
	msg += "):\n";
	
	for(vector<Statement*>::iterator it = mStatementArray.begin(); it != mStatementArray.end(); ++it)
	{
	    Statement *temp = *it;
	    msg += temp->print();
	}
	return msg;

}

bool
FuncStatement::compareName(string theName)
{
    bool ret = false;
    for(vector<Statement*>::iterator it = mStatementArray.begin(); it != mStatementArray.end(); ++it)
	{
	    Statement *temp = *it;
	    bool x = temp->compareName(theName);
	    if(x)
	        ret = true;
	    break;
	}
    return ret;
    
}







