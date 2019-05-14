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
VarStatement::compare(string theName)
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
	string msg = "Function: " + mType->cargo + " " + mName + "(";
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
	msg += ")";

}