#include "statement.h"

using namespace std;

VarStatement::VarStatement()
{
    mValue = nullptr;
    mId = kState_VAR;
}




string
VarStatement::print()
{
    string msg = mType->cargo + ": " + mName + "= ";

    if(mValue != nullptr)
    {
        msg += mValue->print();
    }
    else
        msg += "uninitialized";

    return msg + "\n";
}

bool
VarStatement::compareName(const string &theName)
{
    return mName == theName;
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

/*bool
ExpressionStatement::compareName(const std::string &theName)
{
    return false;
}*/

string
ExpressionStatement::print()
{
    string msg = "";
    //todo: format
    for(auto it = mTermVector.begin(); it != mTermVector.end(); ++it)
    {
        msg += (*it)->print() + " ";
    }
    return msg;
}


string
BinExpressionStatement::print()
{
    string msg = "";
    //todo: format
    for(auto it = mTermVector.begin(); it != mTermVector.end(); ++it)
    {
        msg += (*it)->print() + " ";
    }
    return msg;
}

string
FuncCallStatement::print()
{
	string msg = "";
	msg += whoAmICalling->mName + "(";
	for (auto it = mArgArray.begin(); it != mArgArray.end(); ++it)
	{
		msg += (*it)->print() + ", ";
	}
	msg += ")";
	return msg;
}


string
ValueStatement::print()
{
	return mValue->cargo;
}

string
OperatorStatement::print()
{
	return ogToken->cargo;
}

void
OperatorStatement::insertOp(Token* x)
{
	if (x->cat != kCat_OPERATOR)
	{
		//todo: error
	}

	mValue = x->type;
	ogToken = x;
}

string
VariantPlaceHolder::print()
{

	return std::string("VariantPlaceHolder");
}












