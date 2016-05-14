/**
 * @author Emanuel B.
 * Disassembler class source code
 *
**/

#include "Disassembler.h"

Disassembler::Disassembler(string fileEntry) {
	fileName = fileEntry;
	curPC = 0;
}

int Disassembler::disassembleCode() {
	string auxName = fileName + string(".o");
	ifstream inputFile(auxName.c_str(),ios::in);

	if (!inputFile) {
		cerr	<< "Fatal Error: Could not open file " << auxName << endl; 
		return -1;
	}

	string auxTokenizer;

	getline(inputFile,auxTokenizer);
	integerTokenize(auxTokenizer,currentCode," ");

	return 0;
}

void Disassembler::integerTokenize(const string &line,vector<int> &integers,const string &delimiters) {
	size_t lastPos = line.find_first_not_of(delimiters,0);
	size_t pos = line.find_first_of(delimiters,lastPos);
	while (string::npos != pos || string::npos != lastPos) {
		integers.push_back(atoi((line.substr(lastPos,pos-lastPos)).c_str()));
		lastPos = line.find_first_not_of(delimiters,pos);
		pos = line.find_first_of(delimiters,lastPos);
	}
}
