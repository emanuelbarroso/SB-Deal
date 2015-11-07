/**
 * @author Emanuel B.
 * Symbol Table class header
 *
**/

#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <algorithm>
using namespace std;

#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

class SymbolTable {
private:
	map<string,int> addressTable;
	map<string,int> lineDefined;
	map<string,int> valueSymbol;
	map<string,bool> definedSymbol;
	map<string,bool> externSymbol;
	map<string,bool> publicSymbol;
	map<string,pair<bool,bool> > dataSymbol;
	map<string,vector<int> > usedLines;
	map<string,vector<int> > usedAddress;

public:
	void insertSymbol(string,int,int,bool=false,bool=false,bool=false,bool=false,bool=false);
	void setSymbolFlags(string,bool=false,bool=false,bool=false,bool=false,bool=false);
	void setSymbolValue(string,int);
	void setSymbolLine(string,int);
	void setSymbolAddress(string,int);
	void addDataOffset(int);
	void addReferences(string,int,int);
	bool isDefinedSymbol(string);
	bool isExternSymbol(string);
	bool isPublicSymbol(string);
	bool isSymbol(string);
	bool isDataSymbol(string);
	bool isConstSymbol(string);
	vector<int> getUsedLines(string);
	vector<int> getUsedAddress(string);
	vector<string> getEntries();
	int getAddress(string);
	int getDefLine(string);
	int getSizeSpace(string);
	int getConstValue(string);
	string getSymbol(int);
	bool foundLinks();
	friend ostream &operator<<(ostream &,SymbolTable &);
};

#endif

