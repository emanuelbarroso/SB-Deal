/**
 * @author Emanuel B.
 * Macro Extender class header
 *
**/

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <stdexcept>
#include <cstdio>
#include <map>
#include <regex>
#include <vector>
#include <algorithm>
#include "Assembler.h"
using namespace std;

#ifndef MACRO_EXTENDER_H
#define MACRO_EXTENDER_H

class MacroExtender : public Assembler {
private:
	map<string,std::pair<int,int> > MNT;
	map<string,vector<string> > MDT;
public:
	MacroExtender(string,string,bool=true);
	int macroExtend();
	void printErrors() const;
	string hasMacro(string);
	bool hasDuplicates(const vector<string>);
	//void MNT_Insert(string,std::pair<int,int>);
	//void MDT_Insert(string,vector<string>);
};

ostream &operator<<(ostream &,const map<string,std::pair<int,int> > &);
ostream &operator<<(ostream &,const std::pair<int,int> &);
ostream &operator<<(ostream &,const vector<string> &);
ostream &operator<<(ostream &,const map<string,vector<string> > &); 

#endif
