#pragma once
#include <string>

typedef struct{
	std::string cargo;
	int sourceIndex, lineIndex, columnIndex;
	std::string *sourceText;
	std::string memberOfFile;
}Character;

class Scanner{
public:
	Scanner();
	~Scanner();
	void init(std::string srcFile);
	Character* getChar();
	std::string lookAhead(int offset = 1);
public:
	std::string *mSourceText; 
	std::string mFilename;
	int mLastIndex, mCurrentIndex, mColumnIndex, mLineIndex;

};