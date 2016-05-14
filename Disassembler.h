#include <climits>
#include <cstdlib>
#include <ctime>
#include <cstdio>
#include <stdexcept>
#include <vector>
#include <iostream>
#include <fstream>
#include <iomanip>
using namespace std;

#ifndef DISASSEMBLER_H
#define DISASSEMBLER_H

class Disassembler {
private:
	string fileName;
	vector<int> currentCode;
	int curPC;
public:
	explicit Disassembler(string);
	int disassembleCode();
	void integerTokenize(const string &,vector<int> &,const string & = " ");
};

#endif
