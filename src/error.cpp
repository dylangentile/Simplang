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

unsigned totalCount = 0, wCount = 0;

vector<ErrorData*> vError, vWarning;

/*

^0^ == print cargo
^1^ == line Index
^2^ == column index

*/

void
error(unsigned id, string theMsg, bool stop, Token* mToken)
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
	   vError.push_back(theOne);
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
           for(unsigned i = 0; i < t - 1; i++)
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
           case 1: z += to_string(*(mToken->lineIndex) + 1);
               break;
           case 2: z += to_string(*(mToken->colIndex) + 1);
               break;

           }
          


       }
	  
       repMsg += z; 
	   if (it != theMsg.end())
		   repMsg.push_back(*it);
	   else
		   break;
   }

   switch(id)
   {
   case 0:
       break;
   case 1:
       break;
   case 5: repMsg = *(mToken->sourceName) + ":" + to_string(*(mToken->lineIndex) + 1) + ":" + to_string(*(mToken->colIndex) + 1) + "-->" + theMsg;
	   break;
   case 6: repMsg = *(mToken->sourceName) + ":" + to_string(*(mToken->lineIndex) + 1) + ":" + to_string(*(mToken->colIndex) + 1) + "-->" + repMsg;
	   break;
   default: repMsg = "Simplang compiler exited with default error:\n\t" + repMsg;
   }


   theOne->msg += "\n\n" + repMsg;
   if(stop)
   {
	   cout << errorOut();
	   {
		   exit(1);
	   }
   }
}


void
warning(unsigned id, string theMsg, Token* mToken)
{

    wCount++;
    ErrorData* theOne = nullptr;
    if(mToken == nullptr)
    {
        mToken = new Token;
        mToken->sourceName = new string;
        mToken->lineIndex = new int;
        mToken->colIndex = new int;
        mToken->strIndex = new int;
        *mToken->sourceName = "NON-FILE_BASED_WARNING";
        *mToken->lineIndex = 0;
        *mToken->colIndex = 0;
        *mToken->strIndex = 0;
        mToken->cargo = "NOT A FILE WARNING!";
    }
    for(auto it = vWarning.begin(); it != vWarning.end(); ++it)
    {
        if((*it)->srcFile == *(mToken->sourceName))
        {
            theOne = *it;
        }
    }
    if(theOne == nullptr)
    {
        theOne = new ErrorData(*(mToken->sourceName));
        vWarning.push_back(theOne);
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
                         << "\nSomewhere the program called the warning function, but I incorrectly formatted the warning msg"
                         << "\nThe error was called at: " << *(mToken->lineIndex) << ":" << *(mToken->colIndex) << " in your code."
                         << "\nPlease alert the Simplang project of this error immediately. Give us the following info: "
                         << "\n\nError Code: " << id << " was called with a msg of: '" << theMsg << "', in the function void warning() in the error.cpp src file."
                         << "\n\nWhat this doesn't mean is that your code is warning free. Lookup the warning code online, or just read the message, "
                         << "\nand fix your error. This is a bug in the warning reporting mechanism, and therefore if there are no warnings this bug can't be exploited! ;)\n";
                    abort(); //incorrect error message formatting.
                }
                ++t;
            }
            string tNum = "";
            ++it;
            for(unsigned i = 0; i < t - 1; i++)
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
            case 1: z += to_string(*(mToken->lineIndex) + 1);
                break;
            case 2: z += to_string(*(mToken->colIndex) + 1);
                break;

            }



        }

        repMsg += z;
        if (it != theMsg.end())
            repMsg.push_back(*it);
        else
            break;
    }

    switch(id)
    {
    case 0:
        break;
    case 1:
        break;
    case 5: repMsg = *(mToken->sourceName) + ":" + to_string(*(mToken->lineIndex) + 1) + ":" + to_string(*(mToken->colIndex) + 1) + "-->" + theMsg;
        break;
    case 6: repMsg = *(mToken->sourceName) + ":" + to_string(*(mToken->lineIndex) + 1) + ":" + to_string(*(mToken->colIndex) + 1) + "-->" + repMsg;
        break;
    default: repMsg = "Simplang compiler exited with default warning:\n\t" + repMsg;
    }


    theOne->msg += "\n\n" + repMsg;
}



string
errorOut()
{
    string finalMsg = "";
	if(totalCount == 0)
		return "";

	for(auto it = vError.begin(); it != vError.end(); ++it)
    {
	    finalMsg += "\n\n-----------------" + (*it)->srcFile + "---------------------" + "\n" + to_string((*it)->errorCount) + " errors in " + (*it)->srcFile;
	    finalMsg += (*it)->msg;
    }

	return finalMsg + "\n\nSimplang exited with " + to_string(totalCount) + " errors.";
}

string
warningOut()
{
    string finalMsg = "";
    if(wCount == 0)
        return "";

    for(auto it = vWarning.begin(); it != vWarning.end(); ++it)
    {
        finalMsg += "\n\n-----------------" + (*it)->srcFile + "---------------------" + "\n" + to_string((*it)->errorCount) + " warnings in " + (*it)->srcFile;
        finalMsg += (*it)->msg;
    }

    return finalMsg + "\n\nSimplang exited with " + to_string(wCount) + " warnings.";
}