/**
 * @author Emanuel B.
 * Program Interface class header
 *
**/

#include <iostream>
#include <iomanip>
#include <string>
#include <map>
#include <vector>
#include <cstdio>
#include "MipsTranslator.h"
#include "IA32Translator.h"
#include "Assembler.h"
#include "PreProcessor.h"
#include "MacroExtender.h"
#include "Linker.h"
#include "Simulator.h"
#include "Disassembler.h"
#include "LangManual.h"
using namespace std;

#ifndef INTERFACE_H
#define INTERFACE_H

class Interface {
private:
	vector<string> arrayArgs;
	map<string,int> possibleArgs;
	int numArgs;
	bool canRun;
public:
	explicit Interface(int,char **);
	~Interface();
	int runInterface();
	int findCommand(string);
	void showSpecificHelp(string,string);
	friend ostream &operator<<(ostream &,const Interface &);
};

void auxPre(string,string);
void auxMcr(string,string,int,int);
void auxLink(string,string,string);
void auxTranslate(vector<string>,string);
void auxMips(vector<string>,string);
int completeAssemble(string,string,int,int);
void completeLink(string,string,string,int,int); 
void auxSim(string,bool=false);
void auxDis(string);
void showHelp(string);
void helpStart();
void showOptions();
void manPages(string);
#endif
