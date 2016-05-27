/**
 * @author Emanuel B.
 * Opcode Table class header
 *
**/

#include <iostream>
#include <map>
#include <iomanip>
#include <string>
using namespace std;

#ifndef OPCODE_TABLE_H
#define OPCODE_TABLE_H

class OpcodeTable {
private:
	map<string,int> opcodeTable;
	map<string,int> sizeTable;

public:
	OpcodeTable();
	bool isInstruction(string);
	int getOpcode(string);
	string getName(int);
	int getInstructionSize(string);
	int getInstructionSize(int);
	int size();
	friend ostream &operator<<(ostream &,const OpcodeTable &);
};

#endif
