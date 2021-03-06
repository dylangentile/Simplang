#pragma once
#include <cstdint>
#include <map>
#include "token.h"

typedef enum
{
	p8 = 8,
	p16 = 16,
	p32 = 32,
	p64 = 64,
	pFp = 256
} Precision;


typedef enum
{
	vType_NUM,
	vType_BOOL,
	vType_STR,
	vType_PTR,
	vType_UNSET,
	vType_TYPENAME
} ValType;


class Variant
{
public:
	Variant();
	Variant(const Variant& copyMe);
	~Variant();
	ValType mType;
	ValType dataType; //for ptr and typename
	Precision mPrec;
	int8_t num8;
	int16_t num16;
	int32_t num32;
	int64_t num64;
	long double numfp;
	std::string mstring;
	bool mbool;
	void* mptr;

	Variant& operator=(const Variant& copyMe)
	{
		this->mType = copyMe.mType;
		this->mPrec = copyMe.mPrec;
		this->dataType = copyMe.dataType;
		this->num8 = copyMe.num8;
		this->num16 = copyMe.num16;
		this->num32 = copyMe.num32;
		this->num64 = copyMe.num64;
		this->numfp = copyMe.numfp;
		this->mstring = copyMe.mstring;
		this->mbool = copyMe.mbool;
		this->mptr = copyMe.mptr;
		return *this;
	}

	void cast(ValType toWhat, Precision thePrec = p64);
  
};



/*

class Object{
public:
	Object();
	~Object();
	virtual ValType getType(Precision *x = nullptr) = 0;
public:
	std::string *name;
	FuncStatement *constructor, *destructor;
	std::map<TokenID, FuncStatement*> opOverloadMap;
	std::map<std::string, FuncStatement*> memberFunctionMap;

};


class IntObj : public Object
{
public:
	IntObj(){mValue = nullptr;}
	~IntObj(){}
	Object* add(Object& x);
	void* mValue;
	Precision bits;
	ValType getType(Precision *x = nullptr){*x = bits; return vType_NUM;}

};


class StrObj : public Object
{
public:
	StrObj(){mValue = nullptr;}
	~StrObj(){}
	ValType getType(Precision *x = nullptr){return vType_STR;}
	Object* add(Object& x);
	std::string* mValue;
};


class BoolObj : public Object
{
public:
	BoolObj(){ mValue = nullptr;}
	~BoolObj(){}
	ValType getType(Precision *x = nullptr){return vType_BOOL;}

	Object* add(Object& x);


	bool* mValue;
};




class Variant
{
public:
	Variant();
	~Variant();
	Object* mObj;
	std::map<std::string, Object*> VariableMap;

};
*/


