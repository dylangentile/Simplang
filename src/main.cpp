#include <iostream>
#include "error.h"

using namespace std;


int main(int argc, char const *argv[])
{
	cout << "\n";
	if(argc == 1)
	{
		error(0, "No input file!");
	}
	
    string theFile = argv[1];



	cout << errorOut() << "\n";
	return 0;
}
