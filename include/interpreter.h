#pragma once
#include "statement.h"
#include "error.h"
#include <iostream>
#include <map>
#include <cstdint>
/*
template <typename T>
class Variant
{
public:
	Variant(T in) { value = in; }
	~Variant() = default;
	T value;

};
*/

class GenericTerm
{
public:
    GenericTerm(){}
    ~GenericTerm(){}
    bool z;
};

class GenericTermOp : public GenericTerm
{
public:
    GenericTermOp(){}
    ~GenericTermOp(){}
    TokenID mId;
};


template <typename T>
class GenericTermVal : public GenericTerm
{
public:
    GenericTermVal(){}
    ~GenericTermVal(){}
    T mValue;

};

class Variant
{
public:
    Variant(uint8_t addr, TokenID x, TokenID y = kToken_NULL);
    Variant& operator=(int8_t x)
    {


        if(mType != kToken_NUMBER)
        {
            if(mType == kToken_STRING)
            {
                std::string p = std::to_string(x);
                mValue = &p;
            }
            else if(mType == kToken_BOOL)
            {
                bool *p = new bool;
                if(x)
                    *p = true;
                else
                    *p = false;
                mValue = p;
            }
        }
        else
        {
            int8_t *p = new int8_t;
            *p = x;
            mValue = p;
        }
        return *this;
    }


    friend std::ostream &operator<<(std::ostream &output, Variant x)
    {
        if(x.mType == kToken_NUMBER)
        {
            if(x.subType != kToken_NUMBER_FP)
            {
                int64_t* p = static_cast<int64_t*>(x.mValue);
                output << *p;
            }
            else
            {
                double* p = static_cast<double*>(x.mValue);
                output << *p;
            }
            return output;
        }
        else if(x.mType == kToken_BOOL)
        {
            bool *p = static_cast<bool*>(x.mValue);
            if(p)
                output << "true";
            else
                output << "false";

            return output;
        }
        else if(x.mType == kToken_STRING)
        {
            std::string *p = static_cast<std::string*>(x.mValue);
        }
        else if(x.mType == kToken_POINTER)
        {

        }




        error(6, "Cannot print Variant!", true);
    }
//protected:
    uint8_t address;
//private:
    void* mValue;
    TokenID mType;
    TokenID subType;


};


class StackFrame
{
public:
	StackFrame();
	~StackFrame();
	std::map<std::string, Variant*> VariantMap;
};


typedef enum
{
	kState_VAR,
	kState_FUNCCALL,
	kState_FUNC,
	kState_BINEXPR,
	kState_EXPR,
	kState_OP,
	kState_VALUE
}StatementID;


class Interpreter
{
public:
	Interpreter(FuncStatement* v);
	~Interpreter();

	void interpret();

private:
	FuncStatement *gFunc;
	std::map<size_t, StatementID> typeHashMatch;
	std::map<uint8_t, Variant*> addressMap;
    //std::map<std::string, uint8_t> nameMap;
	std::stack<StackFrame*> sfStack;
	uint8_t currentAddress;

    StatementID MatchType(Statement* x);

};