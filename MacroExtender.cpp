/**
 * @author Emanuel B.
 * Macro Extender class source code and side functions
 *
**/


#include "MacroExtender.h"

MacroExtender::MacroExtender(string inName,string outName,bool hasDebug) : Assembler(inName,outName,hasDebug) {
	//cout << "File(s): \"" << this->fileIn << "\" and \"" << this->fileOut << "\"." << endl;
}

int MacroExtender::macroExtend() {
	bool canWrite = false;
	int numLine;
	int macroBound = 0;
	int macroLastStart = 0,macroLastEnd = 0;
	bool textSegment = false;
	bool isMacroBody = false;
	smatch auxMatch;
	string curLine;
	vector<string> currentCode;
	vector<string> macroParams;
	vector<string> macroLines;
	fileIn.append(".pre");
	fileOut.append(".mcr");
	ifstream inputFile(fileIn,ios::in);
		
	if (!inputFile) {
		cerr	<< "Fatal Error: Could not open file " << fileIn << endl; 
		return -1;
	}
	cout << "Opened " << fileIn << " for macro-extending..." << endl;
	ofstream outputFile(fileOut,ios::out);
	if (!outputFile) {
		cerr	<< "Fatal Error: Could not create file " << fileOut << endl; 
		return -1;
	}

	while (getline(inputFile,curLine)) {
		// prepareLine(curLine);
		if (curLine.find("SECTION ")!=string::npos && regex_search(curLine,auxMatch,regex("\\bSECTION "))) {
			if (regex_search(curLine,auxMatch,regex("\\bTEXT$"))) {
				textSegment = true;
			}
			else {
				textSegment = false;
			}
		}
		else if (curLine.find(" MACRO ")!=string::npos || regex_search(curLine,auxMatch,regex(" MACRO$")) || regex_search(curLine,auxMatch,regex("\\bMACRO "))) {
			macroBound++;
			macroLastStart = numLine;
			if (!textSegment) {
				addError(numLine,string("Semantical Error: MACRO definitions must go in SECTION TEXT"));
			}
		}
		else if (regex_search(curLine,auxMatch,regex("\\bMACRO "))) {
			macroBound++;
			macroLastStart = numLine;
			if (!textSegment) {
				addError(numLine,string("Semantical Error: MACRO definitions must go in SECTION TEXT"));
			}
		}
		else if (regex_search(curLine,auxMatch,regex("\\bENDMACRO$"))) {
			macroBound--;
			macroLastEnd = numLine;
			if (!textSegment) {
				addError(numLine,string("Semantical Error: ENDMACRO must go in SECTION TEXT"));
			}
		}
		currentCode.push_back(curLine);
		numLine++;
	}
	//cout << "Macro Bound: " << macroBound << endl;
	if (macroBound>0) {
		addError(macroLastStart,string("Semantical Error: Caught at least one MACRO without a corresponding ENDMACRO"));
	}
	else if (macroBound<0) {
		addError(macroLastEnd,string("Semantical Error: Caught at least one ENDMACRO without a corresponding MACRO"));
	}

	vector<string>::iterator codeIterator;
	for (numLine=1,codeIterator=currentCode.begin();codeIterator!=currentCode.end();codeIterator++,numLine++) {
		curLine = *codeIterator;
		canWrite = true;
		//cout << "@" << hasMacro(curLine) << endl; 
		if (curLine.find("SECTION ")!=string::npos && regex_search(curLine,auxMatch,regex("\\bSECTION "))) {
			if (regex_search(curLine,auxMatch,regex("\\bTEXT$"))) {
				textSegment = true;
			}
			else {
				textSegment = false;
			}
			outputFile << curLine << "\n";
		}
		else if (curLine.find(" MACRO ")!=string::npos || regex_search(curLine,auxMatch,regex(" MACRO$")) || regex_search(curLine,auxMatch,regex("\\bMACRO "))) {
			macroParams.clear();
			macroLines.clear();
			if (regex_search(curLine,auxMatch,regex("\\bMACRO ")) && curLine.find("MACRO ")==0) {
				addError(numLine,string("Syntactical Error: MACRO must have a definition label"));
			}
			else if (!regex_search(curLine,auxMatch,regex(labelReg))) {
				addError(numLine,string("Syntactical Error: MACRO must have a definition label"));
			}
			else if (macroBound<=0 || (macroBound>0 && numLine!=macroLastStart)) {
				int macroDefLine = numLine;
				int numParameters; 
				string macroLabel = curLine.substr(0,curLine.find_last_of(':'));
				if (macroLabel.find(':')!=string::npos) {
					addError(numLine,string("Syntactical Error: Too many definitions for MACRO constructor"));
				}
				if (curLine.find(" MACRO ") == string::npos) {
					numParameters = 0;
				}
				else {
					string auxParameters = curLine.substr(curLine.find(" MACRO ")+7);
					splitTokenize(auxParameters,macroParams,",");
					if (hasDuplicates(macroParams)) {
						addError(numLine,"Semantical Errors: Duplicate parameter detected");
					}
					for (const string &macroRef : macroParams) {
						if (!regex_match(macroRef,regex("\\&[\\w]"))) {
							addError(numLine,"Syntactical Error: Invalid parameter(s) for MACRO statement");
							break;
						}
					}
					numParameters = macroParams.size();
					if (numParameters>9) {
						addError(numLine,"Semantical Error: Too many parameters for MACRO definition (max at 9)");
					}
					//cout << macroParams << endl;
				}
				if (MNT.find(macroLabel) != MNT.end()) {
					addError(numLine,"Semantical Error: Macro redefinition detected");
				}
				MNT.emplace(macroLabel,make_pair(macroDefLine,numParameters));
				isMacroBody = true;
				codeIterator++;
				while (isMacroBody && codeIterator != currentCode.end()) {
					curLine = *codeIterator;
					if (curLine.find(" EXTERN") != string::npos && regex_search(curLine,auxMatch,regex(" EXTERN$"))) {
						addError(numLine,"Semantical Error: EXTERN directive inside a macro body is forbidden");
					}
					if (curLine.find(" BEGIN") != string::npos && regex_search(curLine,auxMatch,regex(" BEGIN$"))) {
						addError(numLine,"Semantical Error: BEGIN directive inside a macro body is forbidden");
					}
					if (regex_search(curLine,auxMatch,regex("^\\bEND[\\b]?$"))) {
						addError(numLine,"Semantical Error: END directive inside a macro body is forbidden");
					}
					if (curLine.find("PUBLIC ") != string::npos && regex_search(curLine,auxMatch,regex("^PUBLIC "))) {
						addError(numLine,"Semantical Error: PUBLIC directive inside a macro body is forbidden");
					}
					if (curLine.find(':') != string::npos) {
						addError(numLine,"Semantical Error: MACRO does not support labels inside");
						continue;
					}
					if (regex_search(curLine,auxMatch,regex("\\bENDMACRO$"))) {
						isMacroBody = false;
						break;
					}
					if (curLine.find("SECTION ")!=string::npos && regex_search(curLine,auxMatch,regex("^SECTION "))) {
						addError(numLine,"Semantical Error: SECTION directive inside a macro body is forbidden");
					}
					else {
						for (unsigned i=0;i<macroParams.size();i++) {
							string paramRegex = macroParams[i];
							string paramSub = string("#") + to_string(i+1);
							curLine = regex_replace(curLine,regex(paramRegex),paramSub);
						}
						macroLines.push_back(curLine);
					}
					codeIterator++;
					numLine++;
				}
				numLine++;
				MDT.emplace(macroLabel,macroLines);
			}
		}
		//else if (regex_search(curLine,auxMatch,regex("\\bENDMACRO\\b"))) {
		//	canWrite = false;
		//}
		else if (hasMacro(curLine)!=";") {
			string macroFound = hasMacro(curLine);
			//cout << "Found Macro \"" << macroFound << "\"at line " << numLine << endl;
			map<string,std::pair<int,int> >::iterator MNTCell = MNT.find(macroFound);
			map<string,vector<string> >::iterator MDTCell = MDT.find(macroFound);
			std::pair<int,int> MNTDefs = MNTCell->second;
			vector<string> auxiliarLines = MDTCell->second;
			vector<string> paramsToSub;
			try	{
				string finalLine = curLine.substr(curLine.find(macroFound)+macroFound.length()+1);
				if (curLine.find(':') != string::npos) {
					string curLabel = curLine.substr(0,curLine.find(':')+1);
					outputFile << curLabel << " ";
				}
				splitTokenize(finalLine,paramsToSub,",");
				if (paramsToSub.size() != static_cast<unsigned>(MNTDefs.second)) {
					addError(numLine,"Syntactical Error: Wrong number of parameters for current MACRO");
				}
				else {
					for (const string &subLines : auxiliarLines) {
						string auxSubLine = subLines;
						for (int j=0;j<MNTDefs.second;j++) {
							string MDTParamRegex = string("#") + to_string(j+1);
							auxSubLine = regex_replace(auxSubLine,regex(MDTParamRegex),paramsToSub[j]);
						}
						outputFile << auxSubLine << "\n";	
					}
				}
			}
			catch (std::out_of_range &e) {
				//cout << "Macro without params: " << e.what() << endl;
				if (curLine.find(':') != string::npos) {
					string curLabel = curLine.substr(0,curLine.find(':')+1);
					outputFile << curLabel << " ";
				}
				for (const string &subLines : auxiliarLines) {
					outputFile << subLines << "\n";	
				}
			}
		}
		else {
			if (!canWrite) {
				canWrite = true;
			}
			else {
				if (curLine.size()) {
					outputFile << curLine << "\n";
				}
			}
		}
	}
	//cout << "MNT: " << endl;
	//cout << MNT << endl;
	//cout << "MDT: " << endl;
	//cout << MDT << endl;
	if (this->errorList.size()) {
		printErrors();
		outputFile.close();
		remove(fileOut.c_str());
	}
	return this->errorList.size();
}

void MacroExtender::printErrors() const {
	cout	<< "Macro-extending not successful due to following error(s):" << endl;
	cout	<< errorList << endl;
	cout	<< "Macro-Extended File not generated.\n" << endl; 
}

string MacroExtender::hasMacro(string line) {
	string consideredLine;
	if (regex_match(line,regex(labelReg))) {
		return ";";		
	}
	size_t labelPos = line.find_first_of(':');
	if (labelPos != string::npos && labelPos+1<line.length()) {
		consideredLine = line.substr(labelPos+2);
	}
	else {
		consideredLine = line;
	}
	for (const auto &i : MNT) {
		string auxFinder = string(" ") + i.first + string(" ");
		//cout << "Testing line [" << consideredLine << "] to [" << i.first << "]" << endl;
		if (consideredLine.find(auxFinder)!=string::npos) {
			return string(i.first);
		}
		auxFinder = string(" ") + i.first;
		size_t auxPos = consideredLine.find(auxFinder);
		size_t sumLen = (i.first).length()+1;
		if (auxPos!=string::npos && (auxPos+sumLen+1)<consideredLine.length()) {
			return string(i.first);
		}
		auxFinder = i.first + string(" ");
		auxPos = consideredLine.find(auxFinder);
		if (auxPos==0) {
			return string(i.first);
		}
		auxFinder = string("\\b") + i.first + string("\\b");
		if (regex_match(consideredLine,regex(auxFinder))) {
			return string(i.first);
		}
	}
	return string(";");
}

bool MacroExtender::hasDuplicates(const vector<string> listParams) {
	bool result = false;
	vector<string> auxVector = listParams;
	sort(auxVector.begin(),auxVector.end());
	const string empty = {""};
	const string *ptr = &empty;
	for (const string &i : auxVector) {
		if (*ptr == i) {
			result = true;
			break;
		}
		ptr = &i;
	}
	return result;
}

ostream &operator<<(ostream &output,const map<string,std::pair<int,int> > &MNTCell) {
	for (const auto &i : MNTCell) {
		output	<< "(" << i.first << ")" << i.second << endl;
	}
	return output;
}

ostream &operator<<(ostream &output,const std::pair<int,int> &twoVals) {
	output	<< "<" << twoVals.first << "," << twoVals.second << ">";
	return output;
}

ostream &operator<<(ostream &output,const vector<string> &arrayString) {
	for (const auto &i : arrayString) {
		output	<< "@\t" << i << endl;
	}
	return output;
}

ostream &operator<<(ostream &output,const map<string,vector<string> > &MDTCell) {
	for (const auto &i : MDTCell) {
		output	<< "$[" << i.first << "]\n" << i.second << endl;
	}
	return output;
}
