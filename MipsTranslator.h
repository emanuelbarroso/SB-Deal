/**
 * @author Emanuel B.
 * TODO Mips Translator class header
 *
**/

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <vector>
#include <cstdio>
#include <map>
#include "SymbolTable.h"
#include "Assembler.h"
#include "IA32Translator.h"
using namespace std;

#ifndef MIPS_TRANSLATOR_H
#define MIPS_TRANSLATOR_H

class MipsTranslator : public Intel32Translator {
private:
	vector<string> textMIPS;
	vector<string> dataMIPS;
	vector<string> codeMIPS;
	void setSpecialIO(vector<int>,bool &,bool &,bool &,bool &);
public:
	explicit MipsTranslator(string,string,bool);
	int translateMIPSCode();
};

#endif
