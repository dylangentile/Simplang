#include "error.h"
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



void
error(unsigned id, string theMsg)
{
	myErrors->errorCount++;
	myErrors->msg += "\n\n" + theMsg;
}

string
errorOut()
{
	if(myErrors->errorCount == 0)
		return "\nSuccess!";
	return myErrors->msg + "\nSimplang exited with " + to_string(myErrors->errorCount) + " errors.";
}