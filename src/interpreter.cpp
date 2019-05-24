#include "interpreter.h"
#include <typeinfo>
#include <sstream>
#include <iostream>
#include <cmath>

using namespace std;



Variant::Variant(uint8_t addr,TokenID x, TokenID y)
{
    mType = x;
    subType = y;
    mValue = nullptr;
    address = addr;
}

StackFrame::StackFrame()
{

}

StackFrame::~StackFrame()
{

}


Interpreter::Interpreter(FuncStatement* v)
{
	VarStatement a;
	FuncCallStatement b;
	ValueStatement c;
	OperatorStatement d;
	ExpressionStatement e;
	BinExpressionStatement f;
	FuncStatement g;
	typeHashMatch.insert(make_pair(typeid(a).hash_code(), kState_VAR));
	typeHashMatch.insert(make_pair(typeid(b).hash_code(), kState_FUNCCALL));
	typeHashMatch.insert(make_pair(typeid(c).hash_code(), kState_VALUE));
	typeHashMatch.insert(make_pair(typeid(d).hash_code(), kState_OP));
	typeHashMatch.insert(make_pair(typeid(e).hash_code(), kState_EXPR));
	typeHashMatch.insert(make_pair(typeid(f).hash_code(), kState_BINEXPR));
	typeHashMatch.insert(make_pair(typeid(g).hash_code(), kState_FUNC));
    currentAddress = 0;
	gFunc = v;
}

Interpreter::~Interpreter()
{

}


StatementID
Interpreter::MatchType(Statement* x)
{
    std::map<size_t, StatementID>::iterator id = typeHashMatch.find(typeid(*x).hash_code());
    if(id == typeHashMatch.end())
    {

        //todo: runtime error
    }

    return id->second;

}

void
Interpreter::interpret()
{
	cout << "\n";
	StackFrame *theFrame = new StackFrame;
	sfStack.push(theFrame);
	for (auto it = gFunc->mStatementArray.begin(); it != gFunc->mStatementArray.end(); ++it)
	{
		std::map<size_t, StatementID>::iterator id = typeHashMatch.find(typeid((*(*it))).hash_code());
		
		if (id == typeHashMatch.end())
		{
			//runtime error;
		}
		StatementID theId = id->second;
		if (theId == kState_VAR)
		{
		    VarStatement* theVar = dynamic_cast<VarStatement*>(*it);
			cout << "kstate_var";

			Variant *theVariant = new Variant(currentAddress, theVar->mType->type, theVar->mType->subType);
			addressMap.insert(make_pair(currentAddress, theVariant));
			currentAddress++;
			if(theVar->mType->type == kToken_NUMBER && theVar->mType->subType != kToken_NUMBER_FP)
            {
			    int64_t val = 0;
                vector<Statement*> termVector = theVar->mValue->mTermVector;
                vector<GenericTerm*> trueTermVector;
                for(auto tVit = termVector.begin(); tVit != termVector.end(); tVit++)
                {
                    if(MatchType(*tVit) == kState_OP)
                    {
                        OperatorStatement *theOp = dynamic_cast<OperatorStatement*>(*tVit);
                        GenericTermOp *tOp = new GenericTermOp;
                        tOp->mId = theOp->mValue;
                        trueTermVector.push_back(tOp);
                    }
                    else if(MatchType(*tVit) == kState_VALUE)
                    {
                        ValueStatement *tVal = dynamic_cast<ValueStatement*>(*tVit);
                        if(tVal->mValue->type == kToken_IDENTIFIER)
                        {
                            //todo: better search algo
                            auto findMeIt = theFrame->VariantMap.find(tVal->mValue->cargo);
                            Variant *me = findMeIt->second;
                            if(me->mType == kToken_NUMBER)
                            {
                                if(true) //force everything to 64 bit for now.
                                {
                                    GenericTermVal<int64_t> *theGTerm = new GenericTermVal<int64_t>;
                                    theGTerm->mValue = *static_cast<int64_t*>(me->mValue);
                                    trueTermVector.push_back(theGTerm);
                                }
                                else if(me->subType == kToken_NUMBER8)
                                {
                                    GenericTermVal<int8_t> *theGTerm = new GenericTermVal<int8_t>;
                                    theGTerm->mValue = *static_cast<int8_t*>(me->mValue);
                                    trueTermVector.push_back(theGTerm);
                                }
                                else if(me->subType == kToken_NUMBER16)
                                {
                                    GenericTermVal<int16_t> *theGTerm = new GenericTermVal<int16_t>;
                                    theGTerm->mValue = *static_cast<int16_t*>(me->mValue);
                                    trueTermVector.push_back(theGTerm);
                                }
                                else if(me->subType == kToken_NUMBER32)
                                {
                                    GenericTermVal<int32_t> *theGTerm = new GenericTermVal<int32_t>;
                                    theGTerm->mValue = *static_cast<int32_t*>(me->mValue);
                                    trueTermVector.push_back(theGTerm);
                                }
                                else if(me->subType == kToken_NUMBER64)
                                {
                                    GenericTermVal<int64_t> *theGTerm = new GenericTermVal<int64_t>;
                                    theGTerm->mValue = *static_cast<int64_t*>(me->mValue);
                                    trueTermVector.push_back(theGTerm);
                                }
                                else if(me->subType == kToken_NUMBER128)
                                {
                                    //todo: resolve support
                                }
                                else if(me->subType == kToken_NUMBER_FP)
                                {
                                    GenericTermVal<double> *theGTerm = new GenericTermVal<double>;
                                    theGTerm->mValue = *static_cast<double*>(me->mValue);
                                    trueTermVector.push_back(theGTerm);
                                }
                            }

                        }
                        else
                        {
                            if(tVal->mValue->type == kToken_NUMBER)
                            {
                                stringstream ss;
                                ss << tVal->mValue->cargo;
                                if(true) //force everything to 64 bit for now
                                {
                                    GenericTermVal<int64_t> *theGTerm = new GenericTermVal<int64_t>;
                                    ss >> theGTerm->mValue;
                                    trueTermVector.push_back(theGTerm);
                                }
                                else if(tVal->mValue->subType == kToken_NUMBER8)
                                {
                                    GenericTermVal<int8_t> *theGTerm = new GenericTermVal<int8_t>;
                                    ss >> theGTerm->mValue;
                                    trueTermVector.push_back(theGTerm);
                                }
                                else if(tVal->mValue->subType == kToken_NUMBER16)
                                {
                                    GenericTermVal<int16_t> *theGTerm = new GenericTermVal<int16_t>;
                                    ss >> theGTerm->mValue;
                                    trueTermVector.push_back(theGTerm);
                                }
                                else if(tVal->mValue->subType == kToken_NUMBER32)
                                {
                                    GenericTermVal<int32_t> *theGTerm = new GenericTermVal<int32_t>;
                                    ss >> theGTerm->mValue;
                                    trueTermVector.push_back(theGTerm);
                                }
                                else if(tVal->mValue->subType == kToken_NUMBER64)
                                {
                                    GenericTermVal<int64_t> *theGTerm = new GenericTermVal<int64_t>;
                                    ss >> theGTerm->mValue;
                                    trueTermVector.push_back(theGTerm);
                                }
                                else if(tVal->mValue->subType == kToken_NUMBER128)
                                {
                                    //todo: resolve support
                                }
                                else if(tVal->mValue->subType == kToken_NUMBER_FP)
                                {
                                    GenericTermVal<double> *theGTerm = new GenericTermVal<double>;
                                    ss >> theGTerm->mValue;
                                    trueTermVector.push_back(theGTerm);
                                }
                            }
                            else if(tVal->mValue->type == kToken_BOOL)
                            {
                                GenericTermVal<int8_t> *theGTerm = new GenericTermVal<int8_t>;
                                theGTerm->mValue = tVal->mValue->cargo == "true";
                                trueTermVector.push_back(theGTerm);
                            }

                        }
                    }
                    else if(MatchType(*tVit) == kState_FUNCCALL)
                    {
                        //todo: do this...
                    }
                }
                size_t opName;
                {
                    GenericTermOp rrrr;
                    opName = typeid(rrrr).hash_code();

                }
			    //here:
			    for(auto loop = trueTermVector.begin(); loop != trueTermVector.end(); ++loop)
                {
                    if(typeid(*loop).hash_code() == opName)
                    {
                        GenericTermOp *theOp = static_cast<GenericTermOp*>(*loop);
                        GenericTermVal<int64_t> *Sval1 = static_cast<GenericTermVal<int64_t> *>(*loop);
                        GenericTermVal<int64_t> *Sval2= static_cast<GenericTermVal<int64_t> *>(*loop);
                        int64_t val1, val2, result = 0;
                        val1 = Sval1->mValue;
                        val2 = Sval2->mValue;

                        switch(theOp->mId)
                        {
                        case kToken_ADD: result = val1 + val2;
                            break;
                        case kToken_SUBTRACT: result = val2 - val1; // 2 1 - means 'subtract 2 from 1' todo: might need to flip?????
                            break;
                        case kToken_MULTIPLY: result = val1 * val2;
                            break;
                        case kToken_DIVIDE: result = val2 / val1; // 2 1 / means 'divide 1 by 2' todo: might need to flip?????
                            break;
                        case kToken_POWER: result = pow(val1, val2); // 2 1 ^ means '1 to the power of 2' todo: might need to flip?????
                            break;
                        case kToken_MODULO: result = val2 % val1; // 2 1 % means '1 % 2' todo: might need to flip?????
                            break;
                        }



                        loop -= 2;

                        Sval2->mValue = result;
                        trueTermVector.erase(loop + 1);
                        trueTermVector.erase(loop + 2);

                       // goto here;
                    }
                }
            }

		}
	}


	sfStack.pop();

}
