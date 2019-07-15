#include "interpreter.h"
#include <typeinfo>
#include <sstream>
#include <iostream>
//#include <cmath>

using namespace std;

int64_t dPower(int64_t x, int64_t y)
{
    int64_t result = x;
    if(y == 0)
    {
        return 1;
    }
    for(size_t i = 0; i < y - 1; ++i)
    {
        result *= x;
    }
    return result;
}



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


Variant
ValState_to_Variant(ValueStatement valState)
{
    Variant myVariant;
    
    if(valState.mValue->type == kToken_NUMBER)
    {
        stringstream ss;
        ss << valState.mValue->cargo;
        if(valState.mValue->subType == kToken_NUMBER64)
        {
            ss >> myVariant.num64;
            myVariant.mPrec = p64;
            myVariant.mType = vType_NUM;
        }
        else if (valState.mValue->subType == kToken_NUMBER32)
        {
            ss >> myVariant.num32;
            myVariant.mPrec = p32;
            myVariant.mType = vType_NUM;
        }
        else if (valState.mValue->subType == kToken_NUMBER16)
        {
            ss >> myVariant.num16;
            myVariant.mPrec = p16;
            myVariant.mType = vType_NUM;
        }
        else if (valState.mValue->subType == kToken_NUMBER8)
        {
            ss >> myVariant.num8;
            myVariant.mPrec = p8;
            myVariant.mType = vType_NUM;
        }
        else if(valState.mValue->subType == kToken_NUMBER_FP)
        {
            ss >> myVariant.numfp;
            myVariant.mPrec = pFp;
            myVariant.mType = vType_NUM;
        }
    }
    else if(valState.mValue->type == kToken_BOOL)
    {
       
        myVariant.mbool = valState.mValue->cargo == "true";
        myVariant.mType = vType_BOOL;
    }
    else if(valState.mValue->type == kToken_STRING)
    {
        myVariant.mstring = valState.mValue->cargo;
        myVariant.mType = vType_STR;
    }
    return myVariant;
}


void
EvaluateBinExpr(Variant* dest, BinExpressionStatement* theExpr)
{
    vector<Statement*> exprVector = theExpr->mTermVector;
    while(exprVector.size() != 1)
    {
        for(auto it = exprVector.begin(); it != exprVector.end(); ++it)
        {
            if((*it)->fetchId() == kState_OP)
            {
                OperatorStatement* myOp = dynamic_cast<OperatorStatement*>(*it);
                Variant val1, val2, *result = new Variant;
                it--;
                if((*it)->fetchId() == kState_FUNCCALL)
                {
                    //evaluate function statement;
                    //replace that statement with a value statement;
                    //etc
                }
                

                if((*it)->fetchId() == kState_VALUE)
                {
                    val2 = ValState_to_Variant(*dynamic_cast<ValueStatement*>(*it));
                    it--;
                    if((*it)->fetchId() == kState_FUNCCALL)
                    {
                        //evaluate function statement;
                        //replace that statement with a value statement;
                        //etc
                    }
                    
                    if((*it)->fetchId() == kState_VALUE)
                    {
                        val1 = ValState_to_Variant(*dynamic_cast<ValueStatement*>(*it));
                    }
                    else if((*it)->fetchId() == kState_VAR_PH)
                    {
                        VariantPlaceHolder* poop = dynamic_cast<VariantPlaceHolder*>(*it);
                        val1 = *(poop->mVar);
                        delete poop->mVar;
                    }
                }
                else if((*it)->fetchId() == kState_VAR_PH)
                {
                    VariantPlaceHolder* poop2 = dynamic_cast<VariantPlaceHolder*>(*it);
                    val2 = *(poop2->mVar);
                    delete poop2->mVar;
                    it--;
                    if((*it)->fetchId() == kState_FUNCCALL)
                    {
                        //evaluate function statement;
                        //replace that statement with a value statement;
                        //etc
                    }
                    
                    if((*it)->fetchId() == kState_VALUE)
                    {
                        val1 = ValState_to_Variant(*dynamic_cast<ValueStatement*>(*it));
                    }
                    else if((*it)->fetchId() == kState_VAR_PH)
                    {
                        VariantPlaceHolder* poop = dynamic_cast<VariantPlaceHolder*>(*it);
                        val1 = *(poop->mVar);
                        delete poop->mVar;

                    }

                }

                if(val2.mType == val1.mType && val2.mType == vType_NUM)
                {
                    result->mType = vType_NUM;
                    result->mPrec = p64;
                    switch(myOp->mValue)
                    {

                        case kToken_ADD: result->num64 = val1.num64 + val2.num64;
                        break;
                        case kToken_SUBTRACT: result->num64 = val2.num64 - val1.num64;
                        break;
                        case kToken_MULTIPLY: result->num64 = val1.num64 * val2.num64;
                        break;
                        case kToken_DIVIDE: result->num64 = val2.num64 / val1.num64;
                        break;
                        case kToken_MODULO: result->num64 = val2.num64 % val1.num64;
                        break;
                        case kToken_POWER: result->num64 = dPower(val2.num64, val1.num64);
                        break;
                        case kToken_EQUALITY: result->mbool = val2.num64 == val1.num64; result->num64 = (int64_t)result->mbool;
                        break;
                        case kToken_LESS: result->mbool = val2.num64 < val1.num64; result->num64 = (int64_t)result->mbool;
                        break;
                        case kToken_GREATER: result->mbool = val2.num64 > val1.num64; result->num64 = (int64_t)result->mbool;
                        break;
                        case kToken_LESS_EQUAL: result->mbool = val2.num64 <= val1.num64; result->num64 = (int64_t)result->mbool;
                        break;
                        case kToken_GREATER_EQUAL: result->mbool = val2.num64 >= val1.num64; result->num64 = (int64_t)result->mbool;
                        break;
                        case kToken_NOT_EQUAL: result->mbool = val2.num64 != val1.num64; result->num64 = (int64_t)result->mbool;
                        break;
                        default: error(6, "Spooky Scary Skeletons!", true); break;
                    }


                }
                else if(val2.mType == vType_BOOL)
                {

                }
                else if(val2.mType == vType_STR)
                {

                }
              /*  else if(val2.mType == kToken_OBJECT)
                {
                    //do objects memeber operation function corresponding to myOp;
                }*/


                exprVector.erase(it + 2);
                exprVector.erase(it + 1);
                VariantPlaceHolder* myVPH = new VariantPlaceHolder;
                myVPH->mVar = result;
                *it = dynamic_cast<Statement*>(myVPH);
                break;




            }
        }

    }
    dest = (dynamic_cast<VariantPlaceHolder*>(exprVector[0]))->mVar;
}

void
Interpreter::interpret()
{

    StackFrame *theFrame = new StackFrame;
    sfStack.push(theFrame);
    for(auto currentStatement = gFunc->mStatementArray.begin(); currentStatement != gFunc->mStatementArray.end(); ++currentStatement)
    {
        if((*currentStatement)->fetchId() == kState_VAR)
        {
            VarStatement* theVar = dynamic_cast<VarStatement*>(*currentStatement);
            Variant* myVariant = new Variant;
            if(theVar->mValue->fetchId() == kState_BINEXPR)
            {
                BinExpressionStatement* theExpr = dynamic_cast<BinExpressionStatement*>(theVar->mValue);
                EvaluateBinExpr(myVariant, theExpr);
            }


        }
    }





	sfStack.pop();


}

