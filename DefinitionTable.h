/**
 * @author Emanuel B.
 * Definition Table class header
 *
**/

#include <iostream>
#include <iomanip>
#include <string>
#include <map>
#include <vector>
#include <fstream>

using namespace std;

#ifndef DEFINITION_TABLE_H
#define DEFINITION_TABLE_H

class DefinitionTable {
private:
	map<string,int> addressTable;
public:
	int getAddress(string) const;
	bool isDefinedLabel(string) const;
	void insertAddress(string,int);
	friend ostream &operator<<(ostream &,const DefinitionTable &);
	friend ofstream &operator<<(ofstream &,const DefinitionTable &);
	DefinitionTable operator+(const int &);
	map<string,int> getTable() const;
};

DefinitionTable &operator<<(DefinitionTable &,const DefinitionTable &);
#endif
