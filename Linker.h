/**
 * @author Emanuel B.
 * Linker class header
 *
**/

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <algorithm>
#include "Assembler.h"
#include "MacroExtender.h"		// ostream &operator<<(ostream &,const vector<string> &);
#include "DefinitionTable.h"
#include "UseTable.h"
using namespace std;

#ifndef LINKER_H
#define LINKER_H

class Linker{
private:
	vector<string> linkErrors;
	DefinitionTable defTableA,defTableB,globalDefs;
	UseTable useTableA,useTableB;
	vector<int> linkedCode;
	string fileNameA,fileNameB,fileNameResult;
public:
	explicit Linker(string,string,string);
	void integerTokenize(const string &,vector<int> &,const string & = " ");
	void booleanTokenize(const string &,vector<bool> &,const string & = " ");
	int linkCodes();
	void printLinkErrors();
};

bool isEmpty(string);
void printObjects(vector<int>,vector<bool>);
bool haveSameLabel(DefinitionTable,DefinitionTable);

#endif
