#include <iostream>
#include <vector>
#include "error.h"
#include "parser.h"

using namespace std;


int main(int argc, char const *argv[])
{
	cout << "\n";
	if(argc <= 1)
		error(0, "No input file!", true);
	

    string theFile = argv[1];
	Parser *myParser = new Parser(theFile, true);
	myParser->parse();
    cout << "\n\n"<< myParser->mLog << "\n\n";

	cout << errorOut() << "\n";
	return 0;
}
