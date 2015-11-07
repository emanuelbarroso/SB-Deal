/**
 * @author Emanuel B.
 * Assembler class header
 *
**/

#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <cstdio>
#include <cstdlib>
#include <map>
#include "OpcodeTable.h"
#include "DirectiveTable.h"
#include "DefinitionTable.h"
#include "UseTable.h"
#include "SymbolTable.h"
using namespace std;

#ifndef ASSEMBLER_H
#define ASSEMBLER_H
#define MEMORY_MAX_SIZE 20000
	/**
	 * WRG: WRONG
	 * LIN: LINE
	 * JST: JUST
	 * LAB: LABEL
	 * DIR: DIRECTIVE
	 * EXP: EXPRESSION
	 * LDR: LABEL+DIRECTIVE
	 * CDD: COPY DIRECTIVE+DIRECTIVE
	 * CED: COPY EXPRESSION+DIRECTIVE
	 * CEE: COPY EXPRESSION+EXPRESSION
	 * CDE: COPY DIRECTIVE+EXPRESSION
	 * MBD: MACRO BODY	
	 */	

enum expression_tier {
	WRG_LIN,JST_LAB,JST_DIR,DIR_DIR,LAB_DIR,DIR_LAB,DIR_EXP,DIR_CDD,
	DIR_CED,DIR_CEE,DIR_CDE,LDR_LAB,LDR_EXP,LDR_CDD,LDR_CED,LDR_CDE,
	LDR_CEE,LDR_MBD
};

class Assembler {
protected:
	bool keepDebug;
	bool haveLinks;
	OpcodeTable opcodeTable;
	DirectiveTable directiveTable;
	UseTable useTable;
	DefinitionTable definitionTable;
	SymbolTable symbolTable;
	string fileIn;
	string fileOut;
	multimap<int,string> errorList;
	vector<bool> bitMap;
	vector<int> resultCode;
	vector<int> rawCode;
	map<int,int> jumpOperands;
	map<int,int> normalOperands;
	map<int,int> copyOperands;
	int dataStart;
	string numReg = "([\\+|-]?[\\d]+|[\\+|-]?[\\da-fA-F]+[hH]|[\\+|-]?0[xX][\\da-fA-F]+)";
	string decReg = "([\\d]+)";
	string labelReg = "([A-Z_][\\w]*)\x3A";
	string directiveReg = "\\b([_A-Z]+)\\b";
	string alnumReg = "\\b([\\w]+)\\b";
	string auxlabelReg = "([A-Z_][\\w]*)";
	string gensumReg = "([\\S]+[\\+][\\S^\\+]+)";
	string genarrayReg = "([\\S]+\\[[\\S]+\\])";
	
public:
	Assembler(string,string,bool=false);
	~Assembler();
	void addError(int,string);
	virtual void printErrors() const;
	int getIntegerConstant(string);
	void splitTokenize(const string &,vector<string> &,const string & = ",");
	bool hasLinkables();
	int onePassAssemble();
	SymbolTable getSymbolTable() const;
	vector<int> getResultCode() const;
	vector<int> getRawCode() const;
	int getDataStart() const;
	bool alreadyHasError(int,string);
	int getTier(int);
	bool cannotModifyConst(int);
};

template <class T> ostream &operator<<(ostream &,const vector<T> &);
ostream &operator<<(ostream &,const map<int,int> &);
ofstream &operator<<(ofstream &,const vector<int> &);
ofstream &operator<<(ofstream &,const vector<string> &);
template <class K,class V> ostream &operator<<(ostream &,const multimap<K,V> &);
vector<int> operator-(const vector<int> &,const vector<int> &);
vector<int> &operator<<(vector<int> &,const vector<int> &);
vector<int> &operator<<(vector<int> &,const int &);
vector<bool> &operator<<(vector<bool> &,const vector<bool> &);
vector<bool> &operator<<(vector<bool> &,const bool &);
void bigEndianAddInt(int,vector<int> &);
#endif
