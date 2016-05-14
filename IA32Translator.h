/**
 * @author Emanuel B.
 * Intel IA32 Translator class header
 *
**/

#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <climits>
#include <vector>
#include <map>
#include <algorithm>
#include "PreProcessor.h"
#include "MacroExtender.h"
#include "Assembler.h"
#include "SymbolTable.h"
#include "ElfCreator.h"
using namespace std;

#ifndef IA32_TRANSLATOR_H
#define IA32_TRANSLATOR_H
#define STD_EXIT_CODE 0

class Intel32Translator {
private:
	bool elfMode;
	bool debugAtStop;
	SymbolTable intelSymbols;
	//map<int,int> jmpOffsets;
	//map<int,int> jeOffsets;
	//map<int,int> jgOffsets;
	//map<int,int> jlOffsets;
	//map<int,int> callOffsets;
	//map<int,int> normalOffsets;
	//map<int,bool> jumpResized;
	vector<string> intelText;
	vector<string> intelData;
	vector<string> intelCode;
	vector<unsigned char> machineCode;
	vector<unsigned char> machineText;
	vector<unsigned char> machineData;
	vector<int> inputCode;
	vector<int> outputCode;
	vector<int> cinputCode;
	vector<int> coutputCode;
	vector<int> sinputCode;
	vector<int> soutputCode;
	vector<int> boutputCode;
	vector<int> ooutputCode;
	vector<int> houtputCode;
	vector<int> randomCode;
	vector<int> minmCode;
	vector<int> maxmCode;
	vector<int> gcdCode;
	vector<int> lcmCode;
	int getTextSize(vector<int>,unsigned = UINT_MAX);
	void setIOFlags(vector<int>,bool &,bool &,bool &,bool &,bool &,bool &);
	void setIONewFlags(vector<int>,bool &,bool &,bool &,bool &);
	void setAdvFlags(vector<int>,bool &,bool &,bool &,bool &);
protected:
	bool keepDebug;
	bool hasData;
	string objectName;
	string inName;
	SymbolTable objSymbols;
	vector<int> rawCode;
	vector<int> resultCode;
	vector<int> arraySizes;
	vector<int> objSizes;
	int baseAddress;
	int textSize;
	int objDataSize;
	int anotherOffset;
public:
	Intel32Translator(string,string,bool=false,bool=false,int=0x08048080,int=0,bool=false);
	int findSymbol(int);
	int translateCode();
};

int verboseAssemble(string,string,SymbolTable &,vector<int> &,vector<int> &,int &,bool &);
ostream &operator<<(ostream &,const vector<int> &);
ostream &operator<<(ostream &,const vector<unsigned char> &);
ofstream &operator<<(ofstream &,const vector<unsigned char> &);
vector<unsigned char> &operator<<(vector<unsigned char> &,const vector<unsigned char> &);
vector<unsigned char> &operator<<(vector<unsigned char> &,const vector<int> &);
vector<unsigned char> &operator<<(vector<unsigned char> &,const unsigned char &);
vector<unsigned char> &operator<<(vector<unsigned char> &,const int &);
vector<string> &operator<<(vector<string> &,string &);

#endif
