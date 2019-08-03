#include "object.h"
#include "error.h"

using namespace std;

Variant::Variant()
{
	mType = vType_UNSET;
	dataType = vType_UNSET;
	num8 = 0;
	num16 = 0;
	num32 = 0;
	num64 = 0;
	numfp = 0.0;
	mstring = "";
	mbool = false;
	mptr = (void*)0;
	
}

Variant::~Variant()
{

}


void
Variant::cast(ValType toWhat, Precision thePrec)
{
	if(mType == toWhat && mType == vType_NUM)
	{
		switch(mPrec)
		{
			case p8: switch(thePrec){
				case p8: mPrec = p8; num8 = (int8_t)num8;
				break;
				case p16: mPrec = p16; num16 = (int16_t)num8;
				break;
				case p32: mPrec = p32; num32 = (int32_t)num8;
				break;
				case p64: mPrec = p64; num64 = (int64_t)num8;
				break;
				case p128: //do nothing
				break;
				case pFp: mPrec = pFp; numfp = (long double)num8;
				break;
			}
			break;
			case p16: switch(thePrec){
				case p8: mPrec = p8; num8 = (int8_t)num16;
				break;
				case p16: mPrec = p16; num16 = (int16_t)num16;
				break;
				case p32: mPrec = p32; num32 = (int32_t)num16;
				break;
				case p64: mPrec = p64; num64 = (int64_t)num16;
				break;
				case p128: //do nothing
				break;
				case pFp: mPrec = pFp; numfp = (long double)num16;
				break;
			}
			break;
			case p32: switch(thePrec){
				case p8: mPrec = p8; num8 = (int8_t)num32;
				break;
				case p16: mPrec = p16; num16 = (int16_t)num32;
				break;
				case p32: mPrec = p32; num32 = (int32_t)num32;
				break;
				case p64: mPrec = p64; num64 = (int64_t)num32;
				break;
				case p128: //do nothing
				break;
				case pFp: mPrec = pFp; numfp = (long double)num32;
				break;
			}
			break;
			case p64: switch(thePrec){
				case p8: mPrec = p8; num8 = (int8_t)num64;
				break;
				case p16: mPrec = p16; num16 = (int16_t)num64;
				break;
				case p32: mPrec = p32; num32 = (int32_t)num64;
				break;
				case p64: mPrec = p64; num64 = (int64_t)num64;
				break;
				case p128: //do nothing
				break;
				case pFp: mPrec = pFp; numfp = (long double)num64;
				break;
			}
			break;
			case p128: //do nothing
			break;
			case pFp: switch(thePrec){
				case p8: mPrec = p8; num8 = (int8_t)numfp;
				break;
				case p16: mPrec = p16; num16 = (int16_t)numfp;
				break;
				case p32: mPrec = p32; num32 = (int32_t)numfp;
				break;
				case p64: mPrec = p64; num64 = (int64_t)numfp;
				break;
				case p128: //do nothing
				break;
				case pFp: mPrec = pFp; numfp = (long double)numfp;
				break;
			}
			break;
		}
	}
	else if(mType == vType_NUM)
	{
		if(toWhat == vType_BOOL)
		{
			mType = vType_BOOL;
			switch(mPrec)
			{
				case p8: mbool = (num8 != 0);
				break;
				case p16: mbool = (num16 != 0);
				break;
				case p32: mbool = (num32 != 0);
				break;
				case p64: mbool = (num64 != 0);
				break;
				case p128: mbool = (1 != 0);
				break;
				case pFp: mbool = (numfp != 0.0);
				break;
			}
		}
		else if(toWhat == vType_STR)
		{
			mType = vType_STR;
			switch(mPrec)
			{
				case p8: mstring = to_string(num8);
				break;
				case p16: mstring = to_string(num16);
				break;
				case p32: mstring = to_string(num32);
				break;
				case p64: mstring = to_string(num64);
				break;
				case p128: mstring = "num128 is unsupported!";
				break;
				case pFp: mstring = to_string(numfp);
				break;
			}
		}
		else if(toWhat == vType_PTR)
		{
			mType = vType_PTR;
			switch(mPrec)
			{
				case p8: mptr = (void*)num8;
				break;
				case p16: mptr = (void*)num16;
				break;
				case p32: mptr = (void*)num32;
				break;
				case p64: mptr = (void*)num64;
				break;
				case p128: //do nothing
				break;
				case pFp: error(7, "cannot cast double to pointer!");
				break;
			}
		}
	}
	else if(mType == vType_BOOL)
	{
		if(toWhat == vType_STR)
		{
			//todo: should it be 1 and 0 or true and false??
			mType = vType_STR;
			if(mbool)
				mstring = "true";
			else
				mstring = "false";
		}
		else if(toWhat == vType_PTR)
		{
			error(7, "cannot cast bool to pointer!(why would anyone ever need that?) As a workaround you can cast the bool to an num, and then the num to a pointer ;)");
		}
		else if(toWhat == vType_NUM)
		{
			mType = vType_NUM;
			if(mbool)
				num8 = 1;
			else
				num8 = 0;

			cast(vType_NUM, thePrec);
		}
	}
	else if(mType == vType_STR)
	{
		if(toWhat == vType_NUM)
		{
			mPrec = thePrec;
			mType = vType_NUM;
			switch(thePrec)
			{
				case p8: num8 = (int8_t)stoi(mstring);
				break;
				case p16: num16 = (int16_t)stol(mstring);
				break;
				case p32: num32 = (int32_t)stoll(mstring);
				break;
				case p64: num64 = (int64_t)stoll(mstring);
				break;
				case p128: //do nothing
				break;
				case pFp: numfp = (long double)stold(mstring);
				break;
			}
		}
		else if(toWhat == vType_BOOL)
		{
			mType = vType_BOOL;
			if(mstring == "true" || stoi(mstring) != 0)
				mbool = true;
			else
				mbool = false;
		}
		else if(toWhat == vType_PTR)
		{
			error(7, "cannot cast string to pointer!");
		}
	}
}

/*
case p8:
break;
case p16:
break;
case p32:
break;
case p64:
break;
case p128:
break;
case pFp:
break;
*/


