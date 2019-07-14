#include "interpreter.h"
#include <typeinfo>
#include <sstream>
#include <iostream>
#include <cmath>

using namespace std;



StackFrame::StackFrame()
{

}

StackFrame::~StackFrame()
{

}


Interpreter::Interpreter(FuncStatement* v)
{

    currentAddress = 0;
	gFunc = v;

}

Interpreter::~Interpreter()
{

}

void
Interpreter::interpret()
{

    StackFrame *theFrame = new StackFrame;
    sfStack.push(theFrame);
    for(auto currentStatement = gFunc->mStatementArray.begin(); currentStatement != gFunc->mStatementArray.end(); ++currentStatement)
    {
        if((*currentStatement)->fetchId()== kState_VAR)
        {

        }
    }





	sfStack.pop();


}

