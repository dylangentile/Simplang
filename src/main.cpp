#include <iostream>
#include <vector>
#include "error.h"
#include "parser.h"
#include "interpreter.h"
#include "data.h"

using namespace std;





int main(int argc, const char *argv[])
{
	cout << "\n";
	vector<string> allSources = theData.parseArgs(argc, argv);

	
	

    string theFile = allSources[0];
	Parser *myParser = new Parser(theFile, !theData.hush);
	Interpreter* myInterpreter = new Interpreter(myParser->parse());
	if(theData.printValues) cout << "\n\n" << myParser->mLog << "\n\n";
	if(theData.printWarnings) cout << warningOut() << "\n\n";
	//cout << errorOut() << "\n\n\n";
	myInterpreter->interpret();
	

   
	return 0;
}
