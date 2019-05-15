#include "statement.h"

using namespace std;

VarStatement::VarStatement()
{
    mValue = nullptr;
}


string
VarStatement::print()
{
	return mType->cargo + ": " + mName + "=" + mValue->cargo + "\n";
}

bool
VarStatement::compareName(const string &theName)
{
	if (mName == theName)
		return true;
	return false;
}




string
FuncStatement::print()
{
	string msg = "\nFunction: " + mType->cargo + " " + mName + "(";
	for (auto it = mArgArray.begin(); it != mArgArray.end(); it++)
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
	
	for(auto it = mStatementArray.begin(); it != mStatementArray.end(); ++it)
	{
	    Statement *temp = *it;
	    msg += temp->print();
	}
	return msg;

}

bool
FuncStatement::compareName(const string &theName)
{
    bool ret = false;
    for(auto it = mStatementArray.begin(); it != mStatementArray.end(); ++it)
	{
	    Statement *temp = *it;
	    bool x = temp->compareName(theName);
	    if(x)
	        ret = true;
	    break;
	}
    return ret;
    
}