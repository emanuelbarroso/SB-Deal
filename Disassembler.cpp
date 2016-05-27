/**
 * @author Emanuel B.
 * Disassembler class source code
 *
**/

#include "Disassembler.h"

Disassembler::Disassembler(string fileEntry) {
	fileName = fileEntry;
	outName = string("");
	unsigned int np = fileEntry.find('.');
	int nq = (np == string::npos) ? fileEntry.length() : np;
	for (int i = 0; i < nq; i++) outName += fileEntry[i];
	outName += string(".disasm");
	curPC = 0;
	opcsClass0.push_back(0);
	for (int i = 1; i < opcodeTable.size(); i++) {
		vector<int> *ref = nullptr;
		switch(opcodeTable.getInstructionSize(i)) {
			case 0:
				ref = &opcsClass0;
				break;
			case 1:
				ref = &opcsClass1;
				break;
			default:
				ref = &opcsClass2;
				break;
		}
		ref->push_back(i);
	}

	//for (auto &i : opcsClass2) cout << i << endl;
}

void Disassembler::printCode() {
	cout	<< "### CODE {\n\t";
	for (auto &x : currentCode) {
		cout	<< setw(2) << x << " ";
	}
	cout << endl << "}" << endl << endl;
}

int Disassembler::disassembleCode() {
	ifstream inputFile(fileName.c_str(),ios::in);

	if (!inputFile) {
		cerr	<< "Fatal Error: Could not open file " << fileName << endl; 
		return -1;
	}

	string auxTokenizer;

	getline(inputFile,auxTokenizer);
	if (auxTokenizer[0] < '0' || auxTokenizer[0] > '9') {
		cerr	<< "Fatal Error: Tried to disassemble code with linking references" << endl;
		return -1;
	}
	integerTokenize(auxTokenizer,currentCode," ");
	inputFile.close();
	//printCode();
	int maxSize = currentCode.size();
	ofstream outputFile(outName.c_str(),ios::out);

	if (!outputFile) {
		cerr	<< "Fatal Error: Could not create file " << outName << endl; 
		return -1;
	}

	while (curPC < maxSize) {
		try {
			string linaux("");
			stringstream line(linaux);
			line << setfill('0');
			line << "ADD_" << setw(5) << curPC << ":\t";
			string cmdaux("");
			stringstream cmd(cmdaux);
			cmd << setfill('0'); 
			line << setfill(' ');
			bool hasOp = curPC < (maxSize - 1);
			bool hasOps = curPC < (maxSize - 2);
			if (find(opcsClass0.begin(),opcsClass0.end(),currentCode.at(curPC)) != opcsClass0.end()) {
				cmd << setw(5) << currentCode.at(curPC);
				line << left << setw(24) << cmd.str() << " ";
				line << (currentCode.at(curPC) ? opcodeTable.getName(currentCode.at(curPC)) : "#NUL");
				curPC++;
			}
			else if (find(opcsClass1.begin(),opcsClass1.end(),currentCode.at(curPC)) != opcsClass1.end()) {
				cmd << setw(5) << currentCode.at(curPC);
				if (hasOp) cmd << " " << setw(5) << currentCode.at(curPC+1);
				line << left << setw(24) << cmd.str() << " ";
				line << opcodeTable.getName(currentCode.at(curPC)) << " ";
				if (!hasOp) line << "#???";
				else {
					string curAdd("");
					stringstream addStr(curAdd);
					addStr << setfill('0');
					addStr << "ADD_" << setw(5) << currentCode.at(curPC+1);
					line << addStr.str();
				}
				curPC += 2;
			}
			else if (find(opcsClass2.begin(),opcsClass2.end(),currentCode.at(curPC)) != opcsClass2.end()) {
				cmd << setw(5) << currentCode.at(curPC);
				if (hasOp) cmd << " " << setw(5) << currentCode.at(curPC+1);
				if (hasOps) cmd << " " << setw(5) << currentCode.at(curPC+2);
				line << left << setw(24) << cmd.str() << " ";
				line << opcodeTable.getName(currentCode.at(curPC)) << " ";
				if (!hasOp) line << "#???, #???";
				else {
					string curAdd("");
					stringstream addStr(curAdd);
					addStr << setfill('0');
					addStr << "ADD_" << setw(5) << currentCode.at(curPC+1) << ", ";
					if (!hasOps) addStr << "#???";
					else {
						addStr << "ADD_" << setw(5) << currentCode.at(curPC+2);
					}
					line << addStr.str();
				}
				curPC += 3;
			}
			else {
				cmd << setw(5) << currentCode.at(curPC);
				line << left << setw(24) << cmd.str() << " ";
				line << "#BAD";
				curPC++;
			}
			outputFile << line.str() << endl;
		}
		catch(...) {
			cerr	<< "Disassembler Error: Attempted to reach out-of-bound code" << endl;
		}
	}
	outputFile.close();
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
