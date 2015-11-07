/**
 * @author Emanuel B.
 * Simulator class header
 *
**/

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdio>
#include <climits>
#include <cstdlib>
#include <ctime>
#include <stdexcept>
#include <stack>
#include "OpcodeTable.h"
using namespace std;

#ifndef SIMULATOR_H
#define SIMULATOR_H

class Simulator {
private:
	string fileName;
	vector<int> currentCode;
	int curPC;
	int curReg;
	bool exitStatus;
public:
	explicit Simulator(string);
	int simulateCode(bool);
	void integerTokenize(const string &,vector<int> &,const string & = " ");
	int genRand(int = -32768,int = 32767);
};

#endif
