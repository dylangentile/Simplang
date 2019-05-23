#include <iostream>
#include <vector>
#include "error.h"
#include "parser.h"
#include "interpreter.h"

using namespace std;


int main(int argc, char const *argv[])
{
	cout << "\n";
	if(argc <= 1)
		error(0, "No input file!", true);
	

    string theFile = argv[1];
	Parser *myParser = new Parser(theFile, true);
	Interpreter* myInterpreter = new Interpreter(myParser->parse());
	cout << "\n\n" << myParser->mLog << "\n\n";
	cout << warningOut() << "\n\n";
	cout << errorOut() << "\n";
	myInterpreter->interpret();
	

   
	return 0;
}
