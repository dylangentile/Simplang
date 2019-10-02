#include "interpreter.h"
#include <cmath>
#include <sstream>
using namespace std;

StackFrame::StackFrame()
{

}

StackFrame::~StackFrame()
{

}


Interpreter::Interpreter(FuncStatement* theGlobal/*, vector<FuncStatement*>* allFuncs*/)
{
	gFunc = theGlobal;
	//allFunctions = *allFuncs;
//will do something with args
}

Interpreter::~Interpreter()
{
	delete gFunc;
	for(auto it = allFunctions.begin(); it != allFunctions.end(); ++it)
	{
		delete *it;
	}

	while(!sfStack.empty())
	{
		delete sfStack.top();
		sfStack.pop();
	}


}

/******************************************************************************




Printing Utilities




******************************************************************************/

void 
Interpreter::printVariant(Variant* printedOne)
{
	if(printedOne == nullptr)
	{
		error(7, "Runtime error! Printing non-existent variable/value!\n P.S.(The parser failed to find this! You should never see this error!)", true);
	}
	else if(printedOne->mType == vType_NUM)
	{
		switch(printedOne->mPrec){
			case p8: cout << printedOne->num8;
				break;
			case p16: cout << printedOne->num16;
				break;
			case p32: cout << printedOne->num32;
				break;
			case p64: cout << printedOne->num64;
				break;
			case pFp: cout << printedOne->numfp;
				break;
		}
	}
	else if(printedOne->mType == vType_BOOL)
	{
		if(printedOne->mbool)
		{
			cout << "true";
		}
		else
		{
			cout << "false";
		}
	}
	else if(printedOne->mType == vType_STR)
	{
		cout << printedOne->mstring;
	}
}

void 
Interpreter::doPrint(vector<Statement*> args, StackFrame* theFrame)
{
	for(auto it = args.begin(); it != args.end(); ++it)
	{
		if((*it)->fetchId() == kState_VALUE_REFRENCE || (*it)->fetchId() == kState_VALUE)
		{
			Variant* printedOne = nullptr;
			if((*it)->fetchId() == kState_VALUE_REFRENCE)
			{
				ValueRefrenceStatement* myVar = dynamic_cast<ValueRefrenceStatement*>(*it);
				printedOne = findVariant(myVar->refName->cargo, theFrame);
				
			}   
			else
			{
				ValueStatement* myVal = dynamic_cast<ValueStatement*>(*it);
				printedOne = new Variant;
				*printedOne = ValState_to_Variant(*myVal);
			}

			printVariant(printedOne);
			delete printedOne;
			

		}
		else if((*it)->fetchId() == kState_BINEXPR)
		{
			Variant *ans = new Variant;
			BinExpressionStatement* theExpr = dynamic_cast<BinExpressionStatement*>(*it);
			EvaluateBinExpr(ans, theExpr);
			printVariant(ans);
			delete ans;
		}

		delete *it;
	}
	args.clear();
}

/******************************************************************************




Expr/Variant Evaluation




******************************************************************************/

Variant* 
Interpreter::findVariant(string name, StackFrame* theFrame)
{
	if(theFrame == nullptr)
		theFrame = sfStack.top();
	
	auto finder = theFrame->VariantMap.find(name);
	if(finder == theFrame->VariantMap.end())
		return nullptr;

	return finder->second;
}

Variant
Interpreter::ValState_to_Variant(ValueStatement valState)
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

Variant*
Interpreter::StatementToVariant(Statement* theStatement)
{
	Variant *myVariant;
	if(theStatement->fetchId() == kState_FUNCCALL)
	{

	}
	else if(theStatement->fetchId() == kState_VALUE)
	{
		myVariant = new Variant;
		*myVariant = ValState_to_Variant(*dynamic_cast<ValueStatement*>(theStatement));
	}
	else if(theStatement->fetchId() == kState_VAR_PH)
	{
		myVariant = new Variant;
		VariantPlaceHolder* vph = dynamic_cast<VariantPlaceHolder*>(theStatement);
		*myVariant = *(vph->mVar);
	}
	else if(theStatement->fetchId() == kState_VALUE_REFRENCE)
	{
		ValueRefrenceStatement* theref = dynamic_cast<ValueRefrenceStatement*>(theStatement);
		myVariant = findVariant(theref->refName->cargo);
	}
	else
	{
		//cannot be converted to variant!
		myVariant = nullptr;
	}
	return myVariant;
}

void
Interpreter::EvaluateBinExpr(Variant* dest, BinExpressionStatement* theExpr)
{
	vector<Statement*> exprVector = theExpr->mTermVector;
	while(exprVector.size() != 1)
	{
		for(auto it = exprVector.begin(); it != exprVector.end(); ++it)
		{
			if((*it)->fetchId() == kState_OP)
			{
				OperatorStatement* myOp = dynamic_cast<OperatorStatement*>(*it);
				Variant val1Copy, val2Copy, *result = new Variant, *val1Original, *val2Original;
				it--;

				val1Original = StatementToVariant(*it);
				it--;
				val2Original = StatementToVariant(*it);

				val1Copy = *val1Original;
				val2Copy = *val2Original;


				if(val2Copy.mType == vType_NUM)
				{
					result->mType = vType_NUM;
					//result->mPrec = val2Copy.mPrec > val1Copy.mPrec ? val2Copy.mPrec : val1Copy.mPrec;
					result->mPrec = val2Copy.mPrec;

					if(val1Copy.mType != vType_NUM)
					{

					}
					if(val2Copy.mPrec != val1Copy.mPrec)
					{
						if(val2Copy.mPrec > val1Copy.mPrec)
						{
							val1Copy.cast(val2Copy.mType, val2Copy.mPrec);
							result->mPrec = val2Copy.mPrec;
						}
						else
						{	val2Copy.cast(val1Copy.mType, val1Copy.mPrec);
							result->mPrec = val1Copy.mPrec;
						}
					}

					switch(val2Copy.mPrec)
					{
						case p8:
						switch(myOp->mValue)
						{
							case kToken_ADD: result->num8 = val1Copy.num8 + val2Copy.num8;
							break;
							case kToken_SUBTRACT: result->num8 = val2Copy.num8 - val1Copy.num8;
							break;
							case kToken_MULTIPLY: result->num8 = val1Copy.num8 * val2Copy.num8;
							break;
							case kToken_DIVIDE: result->num8 = val2Copy.num8 / val1Copy.num8;
							break;
							case kToken_MODULO: result->num8 = val2Copy.num8 % val1Copy.num8;
							break;
							case kToken_POWER: result->num8 = pow(val2Copy.num8, val1Copy.num8);
							break;
							case kToken_EQUALITY: result->mbool = val2Copy.num8 == val1Copy.num8; result->num8 = (int8_t)result->mbool;
							break;
							case kToken_LESS: result->mbool = val2Copy.num8 < val1Copy.num8; result->num8 = (int8_t)result->mbool;
							break;
							case kToken_GREATER: result->mbool = val2Copy.num8 > val1Copy.num8; result->num8 = (int8_t)result->mbool;
							break;
							case kToken_LESS_EQUAL: result->mbool = val2Copy.num8 <= val1Copy.num8; result->num8 = (int8_t)result->mbool;
							break;
							case kToken_GREATER_EQUAL: result->mbool = val2Copy.num8 >= val1Copy.num8; result->num8 = (int8_t)result->mbool;
							break;
							case kToken_NOT_EQUAL: result->mbool = val2Copy.num8 != val1Copy.num8; result->num8 = (int8_t)result->mbool;
							break;
							default: error(6, "Spooky Scary Skeletons!", true); break;
						}
						break;
						case p16:
						switch(myOp->mValue)
						{
							case kToken_ADD: result->num16 = val1Copy.num16 + val2Copy.num16;
							break;
							case kToken_SUBTRACT: result->num16 = val2Copy.num16 - val1Copy.num16;
							break;
							case kToken_MULTIPLY: result->num16 = val1Copy.num16 * val2Copy.num16;
							break;
							case kToken_DIVIDE: result->num16 = val2Copy.num16 / val1Copy.num16;
							break;
							case kToken_MODULO: result->num16 = val2Copy.num16 % val1Copy.num16;
							break;
							case kToken_POWER: result->num16 = pow(val2Copy.num16, val1Copy.num16);
							break;
							case kToken_EQUALITY: result->mbool = val2Copy.num16 == val1Copy.num16; result->num16 = (int16_t)result->mbool;
							break;
							case kToken_LESS: result->mbool = val2Copy.num16 < val1Copy.num16; result->num16 = (int16_t)result->mbool;
							break;
							case kToken_GREATER: result->mbool = val2Copy.num16 > val1Copy.num16; result->num16 = (int16_t)result->mbool;
							break;
							case kToken_LESS_EQUAL: result->mbool = val2Copy.num16 <= val1Copy.num16; result->num16 = (int16_t)result->mbool;
							break;
							case kToken_GREATER_EQUAL: result->mbool = val2Copy.num16 >= val1Copy.num16; result->num16 = (int16_t)result->mbool;
							break;
							case kToken_NOT_EQUAL: result->mbool = val2Copy.num16 != val1Copy.num16; result->num16 = (int16_t)result->mbool;
							break;
							default: error(6, "Spooky Scary Skeletons!", true); break;
						}
						break;
						case p32:
						switch(myOp->mValue)
						{
							case kToken_ADD: result->num32 = val1Copy.num32 + val2Copy.num32;
							break;
							case kToken_SUBTRACT: result->num32 = val2Copy.num32 - val1Copy.num32;
							break;
							case kToken_MULTIPLY: result->num32 = val1Copy.num32 * val2Copy.num32;
							break;
							case kToken_DIVIDE: result->num32 = val2Copy.num32 / val1Copy.num32;
							break;
							case kToken_MODULO: result->num32 = val2Copy.num32 % val1Copy.num32;
							break;
							case kToken_POWER: result->num32 = pow(val2Copy.num32, val1Copy.num32);
							break;
							case kToken_EQUALITY: result->mbool = val2Copy.num32 == val1Copy.num32; result->num32 = (int32_t)result->mbool;
							break;
							case kToken_LESS: result->mbool = val2Copy.num32 < val1Copy.num32; result->num32 = (int32_t)result->mbool;
							break;
							case kToken_GREATER: result->mbool = val2Copy.num32 > val1Copy.num32; result->num32 = (int32_t)result->mbool;
							break;
							case kToken_LESS_EQUAL: result->mbool = val2Copy.num32 <= val1Copy.num32; result->num32 = (int32_t)result->mbool;
							break;
							case kToken_GREATER_EQUAL: result->mbool = val2Copy.num32 >= val1Copy.num32; result->num32 = (int32_t)result->mbool;
							break;
							case kToken_NOT_EQUAL: result->mbool = val2Copy.num32 != val1Copy.num32; result->num32 = (int32_t)result->mbool;
							break;
							default: error(6, "Spooky Scary Skeletons!", true); break;
						}
						break;
						case p64: 
						switch(myOp->mValue)
						{
							case kToken_ADD: result->num64 = val1Copy.num64 + val2Copy.num64;
							break;
							case kToken_SUBTRACT: result->num64 = val2Copy.num64 - val1Copy.num64;
							break;
							case kToken_MULTIPLY: result->num64 = val1Copy.num64 * val2Copy.num64;
							break;
							case kToken_DIVIDE: result->num64 = val2Copy.num64 / val1Copy.num64;
							break;
							case kToken_MODULO: result->num64 = val2Copy.num64 % val1Copy.num64;
							break;
							case kToken_POWER: result->num64 = pow(val2Copy.num64, val1Copy.num64);
							break;
							case kToken_EQUALITY: result->mbool = val2Copy.num64 == val1Copy.num64; result->num64 = (int64_t)result->mbool;
							break;
							case kToken_LESS: result->mbool = val2Copy.num64 < val1Copy.num64; result->num64 = (int64_t)result->mbool;
							break;
							case kToken_GREATER: result->mbool = val2Copy.num64 > val1Copy.num64; result->num64 = (int64_t)result->mbool;
							break;
							case kToken_LESS_EQUAL: result->mbool = val2Copy.num64 <= val1Copy.num64; result->num64 = (int64_t)result->mbool;
							break;
							case kToken_GREATER_EQUAL: result->mbool = val2Copy.num64 >= val1Copy.num64; result->num64 = (int64_t)result->mbool;
							break;
							case kToken_NOT_EQUAL: result->mbool = val2Copy.num64 != val1Copy.num64; result->num64 = (int64_t)result->mbool;
							break;
							default: error(6, "Spooky Scary Skeletons!", true); break;
						}
						break;
						case pFp:
						switch(myOp->mValue)
						{
							case kToken_ADD: result->numfp = val1Copy.numfp + val2Copy.numfp;
							break;
							case kToken_SUBTRACT: result->numfp = val2Copy.numfp - val1Copy.numfp;
							break;
							case kToken_MULTIPLY: result->numfp = val1Copy.numfp * val2Copy.numfp;
							break;
							case kToken_DIVIDE: result->numfp = val2Copy.numfp / val1Copy.numfp;
							break;
							case kToken_MODULO: error(6, "modulo cannot be used with numfp", true);
							break;
							case kToken_POWER: result->numfp = pow(val2Copy.numfp, val1Copy.numfp);
							break;
							case kToken_EQUALITY: result->mbool = val2Copy.numfp == val1Copy.numfp; result->numfp = (long double)result->mbool;
							break;
							case kToken_LESS: result->mbool = val2Copy.numfp < val1Copy.numfp; result->numfp = (long double)result->mbool;
							break;
							case kToken_GREATER: result->mbool = val2Copy.numfp > val1Copy.numfp; result->numfp = (long double)result->mbool;
							break;
							case kToken_LESS_EQUAL: result->mbool = val2Copy.numfp <= val1Copy.numfp; result->numfp = (long double)result->mbool;
							break;
							case kToken_GREATER_EQUAL: result->mbool = val2Copy.numfp >= val1Copy.numfp; result->numfp = (long double)result->mbool;
							break;
							case kToken_NOT_EQUAL: result->mbool = val2Copy.numfp != val1Copy.numfp; result->numfp = (long double)result->mbool;
							break;
							default: error(6, "Spooky Scary Skeletons!", true); break;
						}
						break;
					}


				}
				else if(val2Copy.mType == vType_BOOL)
				{

				}
				else if(val2Copy.mType == vType_STR)
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
	Variant ans;
	if(exprVector[0]->fetchId() == kState_VAR_PH)
	{
		
		ans = *(dynamic_cast<VariantPlaceHolder*>(exprVector[0]))->mVar;
		delete exprVector[0];

	}
	else if(exprVector[0]->fetchId() == kState_VALUE)
	{
		
		ans = ValState_to_Variant(*dynamic_cast<ValueStatement*>(exprVector[0]));
		delete exprVector[0];
		
	}
    else if(exprVector[0]->fetchId() == kState_VALUE_REFRENCE)
    {
        ValueRefrenceStatement* valRef = dynamic_cast<ValueRefrenceStatement*>(exprVector[0]);
        Variant* theVariant = findVariant(valRef->refName->cargo);
        if(theVariant == nullptr)
        {
            error(6, "trying to look up non-existent variable '^0^' at ^1^:^2^", true, valRef->refName);
        }
        ans = *theVariant;
    }


	if(dest->mType != vType_UNSET)
	{
		if(dest->mType == vType_BOOL && ans.mType != vType_BOOL)
		{
			ans.mType = vType_BOOL;
		}
		else if(ans.mType != dest->mType || ans.mPrec != dest->mPrec)
		{
			ans.cast(dest->mType, dest->mPrec);
		}
	}

	*dest = ans;
}




/******************************************************************************




Basic Interpreting




******************************************************************************/

void
Interpreter::interpret()
{
	interpret(gFunc->mStatementArray);
}


void
Interpreter::interpret(vector<Statement*> statementVector, bool needNewStack)
{

	StackFrame* theStack = nullptr;
	if(needNewStack)
	{
		theStack = new StackFrame;
		sfStack.push(theStack);
	}
	else
	{
		theStack = sfStack.top();
	}
	
	

	for(vector<Statement*>::iterator it = statementVector.begin(); it != statementVector.end(); ++it)
	{
		Statement* currentStatement = *it;
		StatementID theID = currentStatement->fetchId();
		//take a variable statement and convert it into a variant;
		if(theID == kState_VAR)
		{
			VarStatement* theVar = dynamic_cast<VarStatement*>(currentStatement);
			Variant* myVariant = new Variant;

			//determine typing
			if(theVar->mType->type == kToken_NUMBER)
			{
				myVariant->mType = vType_NUM;
				switch(theVar->mType->subType)
				{
					case kToken_NUMBER8: myVariant->mPrec = p8;
					break;
					case kToken_NUMBER16: myVariant->mPrec = p16;
					break;
					case kToken_NUMBER32: myVariant->mPrec = p32;
					break;
					case kToken_NUMBER64: myVariant->mPrec = p64;
					break;
					case kToken_NUMBER_FP: myVariant->mPrec = pFp;
					break;
				}
			}
			else if(theVar->mType->type == kToken_STRING)
			{
				myVariant->mType = vType_STR;
			}
			else if(theVar->mType->type == kToken_BOOL)
			{
				myVariant->mType = vType_BOOL;
			}
			else if(theVar->mType->type == kToken_POINTER)
			{
				myVariant->mType = vType_PTR;
			}
			else if(theVar->mType->type == kToken_OBJECT)
			{
				//todo: objects...
			}


			if(theVar->mValue == nullptr)
			{


				//set memory later...
			}
			else if(theVar->mValue->fetchId() == kState_BINEXPR)
			{
				BinExpressionStatement* theExpr = dynamic_cast<BinExpressionStatement*>(theVar->mValue);
				EvaluateBinExpr(myVariant, theExpr);
				
			}
			theStack->VariantMap.insert(make_pair(theVar->mName, myVariant));
		}
		else if(theID == kState_SPEC_FUNC)
		{
			SpecializedFunctionCall* mySpecFunc = dynamic_cast<SpecializedFunctionCall*>(currentStatement);
			if(mySpecFunc->fId == kFunc_PRINT)
			{
				doPrint(mySpecFunc->mArgArray, theStack);
			}
		}
		else if(theID == kState_VALUE_REFRENCE)
		{
			ValueRefrenceStatement* theRef = dynamic_cast<ValueRefrenceStatement*>(currentStatement);
			Variant* refrencingMe = findVariant(theRef->refName->cargo);
			if(refrencingMe == nullptr)
			{
				//todo: error
			}
			else
			{
				if(theRef->mExpr == nullptr)
				{
					//todo: error
				}
				else if(theRef->mExpr->fetchId() == kState_BINEXPR)
				{
					BinExpressionStatement* theExpr = dynamic_cast<BinExpressionStatement*>(theRef->mExpr);
					EvaluateBinExpr(refrencingMe, theExpr);
				}
			}

		}
	}


	sfStack.pop();
}
