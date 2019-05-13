#include "error.h"
#include <cstdlib> //abort
using namespace std;


ErrorData::ErrorData()
{
	msg = "";
	errorCount = 0;
}

ErrorData::~ErrorData()
{

}

ErrorData *myErrors = new ErrorData;

/*

^0^ == print cargo
^1^ == line Index
^2^ == column index

*/

void
error(unsigned id, string theMsg, Token *mToken)
{
	myErrors->errorCount++;
	myErrors->msg += "\n\n" + theMsg;
	abort();
}

string
errorOut()
{
	if(myErrors->errorCount == 0)
		return "\nSuccess!";
	return myErrors->msg + "\nSimplang exited with " + to_string(myErrors->errorCount) + " errors.";
}