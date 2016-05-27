#include <climits>
#include <cstdlib>
#include <ctime>
#include <cstdio>
#include <stdexcept>
#include <vector>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include "OpcodeTable.h"
using namespace std;

#ifndef DISASSEMBLER_H
#define DISASSEMBLER_H

class Disassembler {
private:
	string fileName;
	string outName;
	OpcodeTable opcodeTable;
	vector<int> currentCode;
	vector<int> opcsClass0;
	vector<int> opcsClass1;
	vector<int> opcsClass2;
	int curPC;
public:
	explicit Disassembler(string);
	~Disassembler() {};
	int disassembleCode();
	void integerTokenize(const string &,vector<int> &,const string & = " ");
	void printCode();
};

#endif
