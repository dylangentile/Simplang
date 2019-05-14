#include "scanner.h"
#include <fstream>
#include <sstream>


using namespace std;

Scanner::Scanner()
{
	mSourceText = new string;
}

Scanner::~Scanner()
{

}


void
Scanner::init(std::string srcFile)
{
	std::ifstream ifs(srcFile);
  	std::string content( (std::istreambuf_iterator<char>(ifs) ),
                       (std::istreambuf_iterator<char>()    ) );
  	*mSourceText = content;
  	mFilename = srcFile;
  	mLastIndex = mSourceText->length() - 1;
  	mCurrentIndex = -1;
  	mLineIndex = 0;
  	mColumnIndex = -1;
}

Character*
Scanner::getChar()
{
	mCurrentIndex++;
	if(mCurrentIndex > 0)
	{
		if((*mSourceText)[mCurrentIndex - 1] == '\n')
		{
			mLineIndex++;
			mColumnIndex = -1;
		}
	}
	mColumnIndex++;
	Character *theChar = new Character;
	if(mCurrentIndex > mLastIndex)
		theChar->cargo = "\0";
	else
		theChar->cargo = (*mSourceText)[mCurrentIndex];

	theChar->lineIndex = mLineIndex;
	theChar->columnIndex = mColumnIndex;
	theChar->sourceIndex = mCurrentIndex;
	theChar->sourceText = mSourceText;
	theChar->memberOfFile = mFilename; 
	return theChar;
}

string
Scanner::lookAhead(int offset)
{
	int mIndex = mCurrentIndex + offset;
	stringstream ss; //string stream allows escape code preservation.
	string target;
	if(mIndex > mLastIndex)
		return "\0";
	

	ss << (*mSourceText)[mIndex];
	ss >> target;
	return target;

}
