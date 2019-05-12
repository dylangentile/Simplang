#include "lexer.h"

using namespace std;

Lexer::Lexer()
{
	myScanner = new Scanner;
}

Lexer::~Lexer()
{
	delete myScanner;
}


void 
Lexer::init(string fileName)
{
	myScanner->init(fileName);
}

void
Lexer::getCharPackage()
{
	c1Char = myScanner->getChar();
	
	c1 = myChar->cargo;
	c2 = c1 + myScanner->lookAhead();

	


}


Token*
Lexer::fetchTokenPtr()
{

}



