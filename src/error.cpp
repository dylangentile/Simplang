#include "error.h"
#include <iostream>
#include <sstream>
#include <cstdlib> //abort
using namespace std;

//TODO: properly implement this instead of this junk!
ErrorData::ErrorData(string file, bool doColors)
{
    srcFile = file;
    color = doColors;
	msg = "";
	errorCount = 0;
}

ErrorData::~ErrorData()
{

}

unsigned totalCount = 0;

vector<ErrorData*> vError;

/*

^0^ == print cargo
^1^ == line Index
^2^ == column index

*/

void
error(unsigned id, string theMsg, Token *mToken, bool stop)
{

    totalCount++;
   ErrorData* theOne = nullptr;
   if(mToken == nullptr)
   {
       mToken = new Token;
       mToken->sourceName = new string;
       mToken->lineIndex = new int;
       mToken->colIndex = new int;
       mToken->strIndex = new int;
       *mToken->sourceName = "NON-FILE_BASED_ERROR";
       *mToken->lineIndex = 0;
       *mToken->colIndex = 0;
       *mToken->strIndex = 0;
       mToken->cargo = "NOT A FILE ERROR!";
   }
   for(auto it = vError.begin(); it != vError.end(); ++it)
   {
       if((*it)->srcFile == *(mToken->sourceName))
       {
           theOne = *it;
       }
   }
   if(theOne == nullptr)
   {
       theOne = new ErrorData(*(mToken->sourceName));
   }
   ++(theOne->errorCount);
   string repMsg = "";
   for(auto it = theMsg.begin(); it != theMsg.end(); ++it)
   {
       string z = "";
       if(*it == '^')
       {
           unsigned t = 1;
           while(*(it + t) != '^')
           {

               if(it + t == theMsg.end())
               {
                   cout << "This is probably my(dylan gentile's) fault."
                          << "\nSomewhere the program called the error function, but I incorrectly formatted the error msg"
                          << "\nThe error was called at: " << *(mToken->lineIndex) << ":" << *(mToken->colIndex) << " in your code."
                          << "\nPlease alert the Simplang project of this error immediately. Give us the following info: "
                          << "\n\nError Code: " << id << " was called with a msg of: '" << theMsg << "', and a stop of: " << stop << ", in the function void error() in the error.cpp src file."
                          << "\n\nWhat this doesn't mean is that your code is error free. Lookup the error code online, or just read the message, "
                          << "\nand fix your error. This is a bug in the error reporting mechanism, and therefore if there are no errors this bug can't be exploited! ;)\n";
                   abort(); //incorrect error message formatting.
               }
               ++t;
           }
           string tNum = "";
           ++it;
           for(unsigned i = 0; i < t; i++)
           {
               tNum.push_back(*it);
               ++it;
           }
           ++it; //get past the closing caret


           unsigned aNum;
           stringstream ss;
           ss << tNum;
           ss >> aNum;
           switch(aNum)
           {
           case 0: z += mToken->cargo;
               break;
           case 1: z += to_string(*(mToken->lineIndex));
               break;
           case 2: z += to_string(*(mToken->colIndex));
               break;
           }
           z.push_back(*it);


       }
       repMsg += z;
   }

   switch(id)
   {
   case 0:
       break;
   case 1:
       break;
   default: repMsg = "Simplang compiler exited with default error:\n\t" + repMsg;
   }


   theOne->msg += "\n\n" + repMsg;
   if(stop)
   {
       cout << errorOut();
       exit(1);
   }
}

string
errorOut()
{
    string finalMsg = "";
	if(totalCount == 0)
		return "\nSuccess!";

	for(auto it = vError.begin(); it != vError.end(); ++it)
    {
	    finalMsg += "\n\n-----------------" + (*it)->srcFile + "---------------------" + "\n" + to_string((*it)->errorCount) + " errors in " + (*it)->srcFile;
	    finalMsg += (*it)->msg;
    }

	return finalMsg + "\nSimplang exited with " + to_string(totalCount) + " errors.";
}