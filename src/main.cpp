#include <iostream>
#include <vector>
#include "error.h"
#include "lexer.h"
#include "token.h"

using namespace std;


int main(int argc, char const *argv[])
{
	cout << "\n";
	if(argc == 1)
	{
		error(0, "No input file!");
	}
	
    string theFile = argv[1];
    Lexer *myLexer = new Lexer;
    myLexer->init(theFile);
    vector<Token*>* out = myLexer->lex();


	cout << errorOut() << "\n";
	return 0;
}
