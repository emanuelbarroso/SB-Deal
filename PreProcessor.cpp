/**
 * @author Emanuel B.
 * Pre Processor class source code and side functions
 *
**/

#include "PreProcessor.h"
#include "Assembler.h"

PreProcessor::PreProcessor(string inName,string outName,bool hasDebug) : Assembler(inName,outName,hasDebug) {
	//cout << "File(s): \"" << this->fileIn << "\" and \"" << this->fileOut << "\"." << endl;
}

int PreProcessor::preProcess() {
	string curLine;
	string errorBuildString;
	int numLine = 1;
	fileIn.append(".asm");
	fileOut.append(".pre");
	ifstream inputFile(fileIn,ios::in);
	map<string,string> equTable;
	bool textSegment = false;
	bool dataSegment = false;
	bool hasTextSegment = false;
	bool hasDataSegment = false;
	bool ifResult = true;
	bool canWrite = true;
	bool equError = false;
	smatch auxMatch;
	regex numRegex(this->numReg);
	vector<string> currentCode;
	
	if (!inputFile) {
		cerr	<< "Fatal Error: Could not open file " << fileIn << endl; 
		return -1;
	}
	cout << "Opened " << fileIn << " for pre-processing..." << endl;
	ofstream outputFile(fileOut,ios::out);
	if (!outputFile) {
		cerr	<< "Fatal Error: Could not create file " << fileOut << endl; 
		return -1;
	}

	while (getline(inputFile,curLine)) {
		prepareLine(curLine);
		if (curLine.find("SECTION ")!=string::npos && regex_search(curLine,auxMatch,regex("^SECTION "))) {
			//cout << curLine << endl;
			if (curLine.find("SECTION ")!=0) {
				addError(numLine,string("Syntactical Error: SECTION cannot be used with a lvalue"));
			}
			if (regex_match(curLine.substr(8),regex("\\bTEXT$"))) {
				if (hasTextSegment) {
					addError(numLine,string("Semantical Error: Multiple SECTION TEXT detected"));
				}
				//if (dataSegment) {
				//	addError(numLine,string("Semantical Error: SECTION TEXT must go before SECTION DATA"));
				//}
				hasTextSegment = true;
				dataSegment = false;
				textSegment = true;
			}
			else if (regex_match(curLine.substr(8),regex("\\bDATA$"))) {
				if (hasDataSegment) {
					addError(numLine,string("Semantical Error: Multiple SECTION DATA detected"));
				}
				hasDataSegment = true;
				dataSegment = true;
				textSegment = false;
			}
			else {
				dataSegment = false;
				textSegment = false;
				addError(numLine,string("Syntactical Error: SECTION only allows TEXT or DATA"));
			}
		}
		else if (curLine.find(" EQU ")!=string::npos || regex_search(curLine,auxMatch,regex(" EQU\\b|\\bEQU |\\bEQU\\b"))) {
			equError = false;
			size_t equPos = curLine.find(" EQU ");
			if (!dataSegment) {
				equError = true;
				addError(numLine,string("Semantical Error: EQU definitions must go in SECTION DATA"));
			}
			else if (equPos!=string::npos) {
				string leftEqu = curLine.substr(0,equPos);
				string rightEqu = curLine.substr(equPos+5,curLine.length());
				//cout	<< "[" << leftEqu << "][" << rightEqu << "] (" 
				//		<< leftEqu.length() << ")(" << rightEqu.length() << ")" << endl;
				if (!regex_match(rightEqu,regex(numReg))) {
					equError = true;
					if (rightEqu.find(',')!=string::npos || rightEqu.find(' ')!=string::npos) {
						addError(numLine,string("Syntactical Error: Too many operands for EQU statement"));
					}
					else if (!regex_match(rightEqu,regex(alnumReg))) {
						addError(numLine,string("Lexical Error: Invalid token"));
					}
					else {
						addError(numLine,string("Syntactical Error: EQU's operand must be a number"));
					}
				}
				if (!regex_match(leftEqu,regex(labelReg))) {
					equError = true;
					size_t colonFirst = leftEqu.find_first_of(':');
					size_t colonLast = leftEqu.find_last_of(':');
					if (colonFirst != string::npos) {
						if (colonFirst == colonLast) {
							addError(numLine,string("Lexical Error: Invalid token"));
						}
						else {
							addError(numLine,string("Syntactical Error: Too many labels for EQU statement"));
						}
					}
					else {
						if (!regex_match(leftEqu,regex(auxlabelReg))) {
							addError(numLine,string("Lexical Error: Invalid token"));
						}
						else {
							addError(numLine,string("Syntactical Error: EQU's lvalue must be a label"));
						}
					}
				}
				else {
					string normalLeftEqu = leftEqu.substr(0,leftEqu.length()-1);
					if (directiveTable.isDirective(normalLeftEqu)||opcodeTable.isInstruction(normalLeftEqu)) {
						equError = true;
						addError(numLine,string("Semantical Error: Keyword used as EQU label"));
					}					
				}
				if (!equError) {
					map<string,string>::iterator alreadyHadEqu = equTable.find(leftEqu.substr(0,leftEqu.length()-1));					
					if (alreadyHadEqu == equTable.end()) {					
						equTable.emplace(leftEqu.substr(0,leftEqu.length()-1),rightEqu);
					}
					else {
						addError(numLine,string("Semantical Error: EQU already defined"));
					}
				}
			}
			else {
				if (regex_search(curLine,auxMatch,regex(" EQU\\b"))) {
					addError(numLine,string("Syntactical Error: EQU must have an operand"));
				}
				else if (regex_search(curLine,auxMatch,regex("\\bEQU "))) {
					addError(numLine,string("Syntactical Error: EQU must have a label definition"));
				}
				else if (regex_match(curLine,regex("EQU"))) {
					addError(numLine,string("Syntactical Error: EQU must have an operand and a label definition"));
				}
			}
		}
		//cout	<< left << setw(50) << curLine << " $ " << boolalpha << regex_search(curLine,auxMatch,numRegex) << endl;
		//cout	<< left << setw(50) << curLine << "; " << left << curLine.length() << " # " << curLine.size() << " -> " << numLine << endl;
		currentCode.push_back(curLine);
		numLine++;
	}
	if (!hasTextSegment) {
		addError(0,string("Semantical Error: Missing SECTION TEXT"));
	}
	if (!hasDataSegment) {
		addError(0,string("Semantical Error: Missing SECTION DATA"));
	}
	/*for (map<string,string>::iterator sit = equTable.begin();sit != equTable.end();sit++) {
		cout << left << setw(20) << sit->first << " # " << left << setw(20) << sit->second << endl;
	}*/
	vector<string>::iterator codeIterator;
	for (numLine=1,codeIterator=currentCode.begin();codeIterator!=currentCode.end();codeIterator++,numLine++) {
		curLine = *codeIterator;
		canWrite = true;
		for (map<string,string>::iterator sit = equTable.begin();sit != equTable.end();sit++) {
			string equRegex = string("(\\b") + sit->first + string("\\b)");
			//cout << "Current REGEX: " << equRegex << endl;
			curLine = regex_replace(curLine,regex(equRegex),sit->second);
		}
		if (curLine.find("SECTION ")!=string::npos && regex_search(curLine,auxMatch,regex("\\bSECTION "))) {
			if (regex_search(curLine,auxMatch,regex("\\bTEXT\\b"))) {
				dataSegment = false;
				textSegment = true;
			}
			else if (regex_search(curLine,auxMatch,regex("\\bDATA\\b"))) {
				dataSegment = true;
				textSegment = false;
			}
			else {
				dataSegment = false;
				textSegment = false;
			}
			outputFile << curLine << "\n";
		}
		else if (curLine.find(" EQU ")!=string::npos) {
			canWrite = false;
		}
		else if (curLine.find("IF ")!=string::npos && regex_search(curLine,auxMatch,regex("\\bIF "))) {
			canWrite = false;
			if (!textSegment) {
				addError(numLine,"Semantical Error: IF statements must go in SECTION TEXT");
			}
			if (curLine.find("IF ")!=0) {
				addError(numLine,"Syntactical Error: IF cannot take a label definition");
			}
			string ifValue = curLine.substr(curLine.find("IF ")+3,curLine.length());
			if (ifValue.find(',')!=string::npos || ifValue.find(' ')!=string::npos) {
				addError(numLine,"Syntactical Error: Too many operands for IF statement");
			}
			else if (directiveTable.isDirective(ifValue)||opcodeTable.isInstruction(ifValue)) {
				addError(numLine,"Semantical Error: Keyword used as IF operand");
			}
			else if (!regex_match(ifValue,regex(alnumReg))) {
				addError(numLine,"Lexical Error: Invalid Token");
			}
			else {
				ifResult = (getIntegerConstant(ifValue)!=0);
			}
		}
		else if (regex_match(curLine,regex("\\bIF\\b")) || regex_search(curLine,auxMatch,regex(" IF\\b"))) {
			canWrite = false;
			addError(numLine,"Syntactical Error: IF must have an operand");
			if (!textSegment) {
				addError(numLine,"Semantical Error: IF statements must go in SECTION TEXT");
			} 
			if (curLine.find("IF")!=0) {
				addError(numLine,"Syntactical Error: IF cannot take a label definition");
			}
		}
		else {
			if (!canWrite) {
				canWrite = true;
			}
			else if (!ifResult) {
				ifResult = true;
			}
			else {
				if (curLine.size()) {
					outputFile << curLine << "\n";
				}
			}
		}
		
	}
	if (this->errorList.size()) {
		printErrors();
		outputFile.close();
		remove(fileOut.c_str());
	}
	return this->errorList.size();
}

void PreProcessor::printErrors() const {
	cout	<< "Pre-processing not successful due to following error(s):" << endl;
	cout	<< errorList << endl;
	cout	<< "Pre-Processed File not generated.\n" << endl; 
}

void stringTrim (string &line,char token, bool trimAfter) {
	size_t auxPos = 0;
	size_t tokPos;
	do {
		tokPos = line.find(token,auxPos);
		if (tokPos != string::npos) {
			auxPos = tokPos+1;
			if (line[tokPos-1] == ' ') {
				line.erase(tokPos-1,1);
			}
		}
	} while (tokPos != string::npos);
	if (trimAfter) {
		auxPos = 0;
		do {
			tokPos = line.find(token,auxPos);
			if (tokPos != string::npos) {
				//cout << "Found + at (" << tokPos << ")" << endl;
				auxPos = tokPos+1;
				if (line[tokPos+1] == ' ') {
					line.erase(tokPos+1,1);
				}
			}
		} while (tokPos != string::npos);
	}
}

void stringUpperCase(string &line) {
	for (unsigned i=0;i<line.length();i++) {
		if (line[i]>='a' && line[i]<='z') {
			line[i]-=('a'-'A');
		}
	}
}

void prepareLine(string &curLine) {
	size_t commentPos = curLine.find(';');
	if (commentPos!=string::npos) {
		curLine.erase(commentPos,curLine.size()-commentPos);
	}
	replace(curLine.begin(), curLine.end(), '\t', ' ');
	string::iterator new_end = unique(curLine.begin(), curLine.end(),[](char a, char b) { return a == b && b == ' '; } );
	curLine.erase(new_end, curLine.end());
	if (curLine[0]==' ') {
		curLine.erase(0,1);
	}
	if (curLine[curLine.length()-1]==' ')  {
		curLine.erase((size_t)curLine.length()-1,1);
	}
	stringTrim(curLine,'+');
	stringTrim(curLine,'-');
	stringTrim(curLine,'[');
	stringTrim(curLine,']');
	stringTrim(curLine,',');
	stringTrim(curLine,':',false);
	stringUpperCase(curLine);
	curLine = regex_replace(curLine,regex(": SPACE\\+"),": SPACE \\+");
	curLine = regex_replace(curLine,regex(": SPACE-"),": SPACE -");
	curLine = regex_replace(curLine,regex(": CONST\\+"),": CONST \\+");
	curLine = regex_replace(curLine,regex(": CONST-"),": CONST -");
}
