/**
 * @author Emanuel B.
 * Assembler class source code and side overloads and functions
 *
**/

#include "Assembler.h"

Assembler::Assembler(string inName,string outName,bool hasDebug) {
	fileIn = inName;
	fileOut = outName;
	keepDebug = hasDebug;
	haveLinks = false;
}

Assembler::~Assembler() {

}

void Assembler::addError(int lineNumber,string errorWhat) {
	errorList.emplace(lineNumber,errorWhat);
}

void Assembler::printErrors() const {
	cout	<< "Assembling not successful due to following error(s):" << endl;
	cout	<< errorList << endl;
	cout	<< "Object File not generated.\n" << endl; 
}

int Assembler::getIntegerConstant(string token) {
	int signal = 1;
	regex pureInt("([\\d]+)");
	regex dirtyHex("([\\da-fA-F]+[hH])");
	regex pureHex("(0[xX][\\da-fA-F]+)");
	if (token[0]=='-') {
		signal = -1;
	}
	if (token[0]=='-'||token[0]=='+') {
		token.erase(0,1);
	}
	if (regex_match(token,pureInt)) {
		return signal*atoi(token.c_str());
	}
	if (regex_match(token,dirtyHex)) {
		token.erase(token.find('H'),1);
		return signal*static_cast<int>(strtol(token.c_str(),NULL,16));
	}
	if (regex_match(token,pureHex)) {
		token.erase(token.find("0X"),2);
		return signal*static_cast<int>(strtol(token.c_str(),NULL,16));
	}
	return 0;
}

void Assembler::splitTokenize(const string &line,vector<string> &tokens,const string &delimiters) {
	size_t lastPos = line.find_first_not_of(delimiters,0);
	size_t pos = line.find_first_of(delimiters,lastPos);
	while (string::npos != pos || string::npos != lastPos) {
		tokens.push_back(line.substr(lastPos,pos-lastPos));
		lastPos = line.find_first_not_of(delimiters,pos);
		pos = line.find_first_of(delimiters,lastPos);
	}
}

template <class K,class V> 
ostream &operator<<(ostream &output,const multimap<K,V> &obj) {
	for (const auto &i : obj) {
		output	<< "At line " << i.first << ": " << i.second << endl;
	}
	return output;
}

ofstream &operator<<(ofstream &output,const vector<int> &obj) {
	for (unsigned i = 0;i<obj.size();i++) {
		output	<< obj[i];
		if (i<(obj.size()-1)) {
			output	<< " ";
		}
	}
	return output;
}

ofstream &operator<<(ofstream &output,const vector<bool> &obj) {
	output << noboolalpha;
	for (unsigned i = 0;i<obj.size();i++) {
		output	<< obj[i];
		if (i<(obj.size()-1)) {
			output	<< " ";
		}
	}
	return output;
}

ofstream &operator<<(ofstream &output,const vector<string> &obj) {
	for (unsigned i = 0;i<obj.size();i++) {
		output	<< obj[i];
		if (i<(obj.size()-1)) {
			output	<< "\n";
		}
	}
	return output;
}

template <class T>
ostream &operator<<(ostream &output,const vector<T> &obj) {
	for (unsigned i = 0;i<obj.size();i++) {
		output	<< '[' << obj[i] << ']';
		if (i<(obj.size()-1)) {
			output	<< " ";
		}
	}
	return output;
}

ostream &operator<<(ostream &output,const map<int,int> &mp) {
	for (const auto &i : mp) {
		output << "(" << i.first << "," << i.second << ")" << endl;
	}
	return output;
}

vector<int> operator-(const vector<int> &lhs,const vector<int> &rhs) {
	vector<int> result(lhs.size());
	if (lhs.size()!=rhs.size()) {
		return vector<int>();
	}
	for (unsigned i = 0;i<lhs.size();i++) {
		result.at(i) = lhs.at(i)-rhs.at(i);
	}
	return result;
}

bool Assembler::hasLinkables() {
	return symbolTable.foundLinks();
}

SymbolTable Assembler::getSymbolTable() const {
	return symbolTable;
}

vector<int> Assembler::getResultCode() const {
	return resultCode;
}

vector<int> Assembler::getRawCode() const {
	return rawCode;
}

int Assembler::onePassAssemble() {
	string curLine;
	int numLine = 1;
	int curAddress = 0;
	int curDataAddress = 0;
	bool textSegment = false;
	bool dataSegment = false;
	bool hasBegin = false;
	bool hasEnd = false;
	string beginModule;
	int beginLine,endLine;
	vector<string> tokList;
	vector<string> opList;
	vector<int> textCode;
	vector<int> dataCode;
	fileIn.append(".mcr");
	fileOut.append(".o");
	ifstream inputFile(fileIn,ios::in);

	if (!inputFile) {
		cerr	<< "Fatal Error: Could not open file " << fileIn << endl; 
		return -1;
	}
	cout << "Opened " << fileIn << " for assembling..." << endl;
	ofstream outputFile(fileOut,ios::out);
	if (!outputFile) {
		cerr	<< "Fatal Error: Could not create file " << fileOut << endl; 
		return -1;
	}
	resultCode.clear();
	rawCode.clear();
	bitMap.clear();
	textCode.clear();
	dataCode.clear();
	while (getline(inputFile,curLine)) {
		tokList.clear();
		opList.clear();
		if(curLine.length()>0) {
			splitTokenize(curLine,tokList,string(" "));
			//cout << "Tokens: " << tokList << endl;
			if (tokList.at(0)==string("SECTION")) {
				if (tokList.at(1)==string("TEXT")) {
					textSegment = true;
					dataSegment = false;
				}
				else {
					textSegment = false;
					dataSegment = true;
				}
			}
			else if ((tokList.at(0)).find(':') != string::npos) {
				if (!regex_match(tokList.at(0),regex(labelReg))) {
					addError(numLine,"Lexical Error: Invalid token");
				}
				string labelName = (tokList.at(0)).substr(0,(tokList.at(0)).find(':'));
				if (opcodeTable.isInstruction(labelName) || directiveTable.isDirective(labelName)) {
					addError(numLine,"Semantical Error: Labels must not be key words");
				}
				else if(symbolTable.isDefinedSymbol(labelName)) {
					string redefError("Semantical Error: Redefined label");
					int firstOcurrence = symbolTable.getDefLine(labelName);
					if (!alreadyHasError(firstOcurrence,redefError)) {
						addError(firstOcurrence,redefError);
					}
					addError(numLine,redefError);
				}
				else {
					symbolTable.insertSymbol(labelName,curAddress,numLine,false,false,true);
					symbolTable.setSymbolLine(labelName,numLine);
					symbolTable.setSymbolAddress(labelName,curAddress);
					symbolTable.setSymbolFlags(labelName,false,false,true);
				}
				tokList.erase(tokList.begin());
				
				for (unsigned i = 0;i < tokList.size();i++) {
					if ((tokList.at(i)).find(':') != string::npos) {
						addError(numLine,"Syntactical Error: Multiple labels detected");
						if(!i) {
							tokList.erase(tokList.begin());
						}
						break;
					}
				}
				//cout << "Tokens[label_line]: " << tokList << endl;
				if (tokList.size()>0) {
					int opcVal = opcodeTable.getOpcode(tokList.at(0));
					if (tokList.at(0)=="BEGIN") {
						if (hasBegin) {
							addError(numLine,"Semantical Error: BEGIN can be used only once");
						}
						else {
							beginLine = numLine;
						}
						if (!textSegment) {
							addError(numLine,"Semantical Error: BEGIN must be at SECTION TEXT");
						}
						if (tokList.size()>1) {
							addError(numLine,"Syntactical Error: BEGIN must not take operands");
						}
						symbolTable.setSymbolFlags(labelName,false,false,true);
						beginModule = string(labelName);
						hasBegin = true;
						if (curAddress) {
							addError(numLine,"Semantical Error: BEGIN must be the first thing in SECTION TEXT if exists");
						}
					}
					else if (tokList.at(0)=="END") {
						if (hasEnd) {
							addError(numLine,"Semantical Error: END can be used only once");
						}
						else {
							endLine = numLine;
						}
						if (tokList.size()>1) {
							addError(numLine,"Syntactical Error: END must not take operands");
						}
						hasEnd = true;
						if (!hasBegin) {
							addError(numLine,"Semantical Error: END without a matching BEGIN statement");
						}
						if (!dataSegment) {
							addError(numLine,"Semantical Error: END must be at SECTION DATA");
						}
						symbolTable.setSymbolFlags(labelName,false,false,true);
					}
					else if (tokList.at(0)=="EXTERN") {
						if (tokList.size()>1) {
							addError(numLine,"Syntactical Error: EXTERN must not take operands");
						}
						if (curAddress) {
							addError(numLine,"Semantical Error: EXTERN must go at beginning of code");
						}
						symbolTable.setSymbolFlags(labelName,false,false,true,true);
						symbolTable.setSymbolAddress(labelName,0);
					}
					else if (tokList.at(0)=="PUBLIC") {
						addError(numLine,"Syntactical Error: PUBLIC must not have a definition label");
						tokList.erase(tokList.begin());
						if (curAddress) {
							addError(numLine,"Semantical Error: PUBLIC must go at beginning of code");
						}
						if (!tokList.size()) {
							addError(numLine,"Syntactical Error: PUBLIC must take an operand");
						}
						else if (tokList.size()>1) {
							addError(numLine,"Syntactical Error: Too many operands for PUBLIC statement");
						}
						else {
							string pubToken = tokList.at(0);
							if (!regex_match(pubToken,regex(auxlabelReg))) {
								if (regex_match(pubToken,regex(alnumReg))) {
									addError(numLine,"Syntactical Error: Invalid operand for PUBLIC statement");
								}
								else {
									addError(numLine,"Lexical Error: Invalid token");
								}
							}
							else {
								if (!symbolTable.isSymbol(pubToken)) {
									symbolTable.insertSymbol(pubToken,0,numLine,false,false,false,false,true);
								}
								else if (symbolTable.isExternSymbol(pubToken)) {
									addError(numLine,"Semantical Error: A symbol cannot be EXTERN and PUBLIC at the same time");
								}
								symbolTable.setSymbolFlags(pubToken,false,false,false,false,true);
							}
						}
					}
					else if (tokList.at(0)=="SPACE") {
						int spaceSize = 0;
						tokList.erase(tokList.begin());
						if (!dataSegment) {
							addError(numLine,"Semantical Error: SPACE is allowed only in SECTION DATA");
						}
						if (!tokList.size()) {
							symbolTable.setSymbolValue(labelName,1);
							symbolTable.setSymbolAddress(labelName,curDataAddress);
							curDataAddress++;
							dataCode << 0;
						}
						else if (tokList.size()>1) {
							addError(numLine,"Syntactical Error: Too many operands for SPACE directive");
						}
						else {
							if (regex_match(tokList.at(0),regex(numReg))) {
								spaceSize = (getIntegerConstant(tokList.at(0)) > 0) ? getIntegerConstant(tokList.at(0)) : 0;
								if (!spaceSize) {
									addError(numLine,"Syntactical Error: Invalid numerical argument for SPACE directive");
								}
								symbolTable.setSymbolValue(labelName,spaceSize);
							}
							else if (regex_match(tokList.at(0),regex(alnumReg))) {
								addError(numLine,"Syntactical Error: Invalid argument for SPACE directive");
							}
							else {
								addError(numLine,"Lexical Error: Invalid token");
							}
							symbolTable.setSymbolAddress(labelName,curDataAddress);
							curDataAddress += spaceSize;
							dataCode << vector<int>(spaceSize,0);
						}
						symbolTable.setSymbolFlags(labelName,false,true,true);
					}
					else if (tokList.at(0)=="CONST") {
						int constVal = 0;
						tokList.erase(tokList.begin());
						if (!dataSegment) {
							addError(numLine,"Semantical Error: CONST is allowed only in SECTION DATA");
						}
						if (!tokList.size()) {
							addError(numLine,"Syntactical Error: Too few operands for CONST directive");
						}
						else if (tokList.size()>1) {
							addError(numLine,"Syntactical Error: Too many operands for CONST directive");
						}
						else {
							if (regex_match(tokList.at(0),regex(numReg))) {
								constVal = getIntegerConstant(tokList.at(0));
								symbolTable.setSymbolValue(labelName,constVal);
							}
							else if (regex_match(tokList.at(0),regex(alnumReg))) {
								addError(numLine,"Syntactical Error: Invalid argument for CONST directive");
							}
							else {
								addError(numLine,"Lexical Error: Invalid token");
							}
						}
						symbolTable.setSymbolAddress(labelName,curDataAddress);
						symbolTable.setSymbolFlags(labelName,true,false,true);
						curDataAddress++;
						dataCode << constVal;
					}
					else if (getTier(opcVal)==0) {
						if (!textSegment) {
							addError(numLine,"Instructions are allowed only in SECTION TEXT");
						}
						tokList.erase(tokList.begin());
						if (!tokList.size()) {
							string errorBuild = string("Syntactical Error: ") + opcodeTable.getName(opcVal);
							errorBuild += string(" without an operand");
							addError(numLine,errorBuild);
						}
						else if (tokList.size()>1) {
							string errorBuild = string("Syntactical Error: Too many operands for ") + opcodeTable.getName(opcVal);
							addError(numLine,errorBuild);
						}
						else {
							//cout << boolalpha << "For " << tokList.at(0) << ": " << regex_match(tokList.at(0),regex(auxlabelReg));
							//cout << noboolalpha << endl;
							if (!regex_match(tokList.at(0),regex(auxlabelReg))) {
								string errTok = tokList.at(0);
								if (regex_match(errTok,regex(alnumReg))) {
									string errorBuild = string("Syntactical Error: Invalid operand for ");
									errorBuild += opcodeTable.getName(opcVal);
									addError(numLine,errorBuild);
								}
								else if (errTok.find_first_of("+[]")!=string::npos) {
									addError(numLine,"Syntactical Error: Jump commands must not take array notations");
								}
								else {
									addError(numLine,"Lexical Error: Invalid token");
								}
							}
							else {
								symbolTable.insertSymbol(tokList.at(0),curAddress+1,numLine);
							}
							jumpOperands.emplace(curAddress+1,numLine);
						}
						textCode	<< opcVal << 0; 
						bitMap	<< false << true;
						curAddress += 2;
					}
					else if (getTier(opcVal)==1) {
						int arrayOffset = 0;
						if (!textSegment) {
							addError(numLine,"Instructions are allowed only in SECTION TEXT");
						}
						tokList.erase(tokList.begin());
						if (!tokList.size()) {
							string errorBuild = string("Syntactical Error: ") + opcodeTable.getName(opcVal);
							errorBuild += string(" without an operand");
							addError(numLine,errorBuild);
						}
						else if (tokList.size()>1) {
							string errorBuild = string("Syntactical Error: Too many operands for ") + opcodeTable.getName(opcVal);
							addError(numLine,errorBuild);
						}
						else {
							string nowOp = tokList.at(0);
							string arrayOp;
							if (regex_match(nowOp,regex(gensumReg))) {
								splitTokenize(nowOp,opList,"+");
								arrayOp = opList.at(1);
							}
							else if (regex_match(nowOp,regex(genarrayReg))) {
								splitTokenize(nowOp,opList,"[]");
								arrayOp = opList.at(1);
							}
							else {
								opList.push_back(nowOp);
								arrayOp = string();
							}
							//cout << "Operand for " << opcodeTable.getName(opcVal) << ": " << opList << endl;
							nowOp = opList.at(0);
							if (!regex_match(nowOp,regex(auxlabelReg))) {
								if (regex_match(nowOp,regex(alnumReg))) {
									string errorBuild = string("Syntactical Error: Invalid operand for ");
									errorBuild += opcodeTable.getName(opcVal);
									addError(numLine,errorBuild);
								}
								else if (nowOp.find(',') == string::npos) {
									addError(numLine,"Lexical Error: Invalid token");
								}
								else {
									string errorBuild = string("Syntactical Error: Too many operands for ") + opcodeTable.getName(opcVal);
									addError(numLine,errorBuild);
								}
							}
							else {
								symbolTable.insertSymbol(nowOp,curAddress+1,numLine);
							}

							if (arrayOp.size()) {
								if (regex_match(arrayOp,regex(decReg))) {
									arrayOffset = atoi(arrayOp.c_str());
								}
								else {
									if (regex_match(arrayOp,regex(alnumReg))) {
										string errorBuild = string("Syntactical Error: Invalid operand for ");
										errorBuild += opcodeTable.getName(opcVal);
										addError(numLine,errorBuild);
									}
									else {
										addError(numLine,"Lexical Error: Invalid token");
									}
								}
							}
							normalOperands.emplace(curAddress+1,numLine);
						}
						textCode	<< opcVal << arrayOffset; 
						bitMap	<< false << true;
						curAddress += 2;
					}
					else if (getTier(opcVal)==2) {
						if (!textSegment) {
							addError(numLine,"Instructions are allowed only in SECTION TEXT");
						}
						tokList.erase(tokList.begin());
						if (tokList.size()) {
							string errorBuild = "Syntactical Error: " + opcodeTable.getName(opcVal);
							errorBuild = errorBuild + " does not allow operands";
							addError(numLine,errorBuild);
						}
						textCode << opcVal;
						bitMap	<< false;
						curAddress++;
					}
					else if (getTier(opcVal)==3) {
						vector<string> copyTokens;
						int arrayOffset = 0;
						int arrayOffset2 = 0;
						if (!textSegment) {
							addError(numLine,"Instructions are allowed only in SECTION TEXT");
						}
						tokList.erase(tokList.begin());
						if (!tokList.size()) {
							string errorBuild = string("Syntactical Error: ") + opcodeTable.getName(opcVal);
							errorBuild += string(" without an operand");
							addError(numLine,errorBuild);
						}
						else if (tokList.size()>1) {
							if (tokList.size()==2) {
								string errorBuild = "Syntactical Error: " + opcodeTable.getName(opcVal);
								errorBuild += string(" Operands must be separated by comma");
								addError(numLine,errorBuild);
							}
							else {
								string errorBuild = string("Syntactical Error: Too many operands for ") + opcodeTable.getName(opcVal);
								addError(numLine,errorBuild);
							}
						}
						else if ((tokList.at(0)).find(',') == string::npos) {
							string nowOp = tokList.at(0);
							string errorBuild = "Syntactical Error: Too few operands for " + opcodeTable.getName(opcVal);
							addError(numLine,errorBuild);
							if (!regex_match(nowOp,regex(auxlabelReg))) {
								if (regex_match(nowOp,regex(alnumReg))) {
									string errorBuild = string("Syntactical Error: Invalid first operand for ");
									errorBuild += opcodeTable.getName(opcVal);
									addError(numLine,errorBuild);
								}
								else {
									addError(numLine,"Lexical Error: Invalid token");
								}
							}
						}
						else {
							splitTokenize(tokList.at(0),copyTokens,",");
							if (copyTokens.size()>2) {
								string errorBuild = "Syntactical Error: Too many operands for " + opcodeTable.getName(opcVal);
								addError(numLine,errorBuild);
							}
							string nowOp = copyTokens.at(0);
							string arrayOp;
							if (regex_match(nowOp,regex(gensumReg))) {
								splitTokenize(nowOp,opList,"+");
								arrayOp = opList.at(1);
							}
							else if (regex_match(nowOp,regex(genarrayReg))) {
								splitTokenize(nowOp,opList,"[]");
								arrayOp = opList.at(1);
							}
							else {
								opList.push_back(nowOp);
								arrayOp = string();
							}
							//cout << "Operand[1] for " << opcodeTable.getName(opcVal) << ": " << opList << endl;
							nowOp = opList.at(0);
							if (!regex_match(nowOp,regex(auxlabelReg))) {
								if (regex_match(nowOp,regex(alnumReg))) {
									string errorBuild = "Syntactical Error: Invalid first operand for ";
									errorBuild = errorBuild + opcodeTable.getName(opcVal);
									addError(numLine,errorBuild);
								}
								else {
									addError(numLine,"Lexical Error: Invalid token");
								}
							}
							else {
								symbolTable.insertSymbol(nowOp,curAddress+1,numLine);
							}
							arrayOffset = 0;
							if (arrayOp.size()) {
								if (regex_match(arrayOp,regex(decReg))) {
									arrayOffset = atoi(arrayOp.c_str());
								}
								else {
									if (regex_match(arrayOp,regex(alnumReg))) {
										string errorBuild = "Syntactical Error: Invalid first operand for ";
										errorBuild = errorBuild + opcodeTable.getName(opcVal);
										addError(numLine,errorBuild);
									}
									else {
										addError(numLine,"Lexical Error: Invalid token");
									}
								}
							}
							normalOperands.emplace(curAddress+1,numLine);
							opList.clear();

							//cout << "Copy Token Size = " << copyTokens.size() << endl;

							nowOp = copyTokens.at(1);
							if (regex_match(nowOp,regex(gensumReg))) {
								splitTokenize(nowOp,opList,"+");
								arrayOp = opList.at(1);
							}
							else if (regex_match(nowOp,regex(genarrayReg))) {
								splitTokenize(nowOp,opList,"[]");
								arrayOp = opList.at(1);
							}
							else {
								opList.push_back(nowOp);
								arrayOp = string();
							}
							//cout << "Operand[2] for " << opcodeTable.getName(opcVal) << ": " << opList << endl;
							nowOp = opList.at(0);
							if (!regex_match(nowOp,regex(auxlabelReg))) {
								if (regex_match(nowOp,regex(alnumReg))) {
									string errorBuild = "Syntactical Error: Invalid second operand for ";
									errorBuild = errorBuild + opcodeTable.getName(opcVal);
									addError(numLine,errorBuild);
								}
								else {
									addError(numLine,"Lexical Error: Invalid token");
								}
							}
							else {
								symbolTable.insertSymbol(nowOp,curAddress+2,numLine);
							}

							if (arrayOp.size()) {
								if (regex_match(arrayOp,regex(decReg))) {
									arrayOffset2 = atoi(arrayOp.c_str());
								}
								else {
									if (regex_match(arrayOp,regex(alnumReg))) {
										string errorBuild = "Syntactical Error: Invalid second operand for ";
										errorBuild = errorBuild + opcodeTable.getName(opcVal);
										addError(numLine,errorBuild);
									}
									else {
										addError(numLine,"Lexical Error: Invalid token");
									}
								}
							}
							copyOperands.emplace(curAddress+2,numLine);
						}
						textCode << opcVal << arrayOffset << arrayOffset2;
						bitMap << false << true << true;
						curAddress += 3;
					}
					else if (getTier(opcVal)==4) {
						vector<string> jumpTokens;
						int arrayOffset = 0;
						if (!textSegment) {
							addError(numLine,"Instructions are allowed only in SECTION TEXT");
						}
						tokList.erase(tokList.begin());
						if (!tokList.size()) {
							string errorBuild = string("Syntactical Error: ") + opcodeTable.getName(opcVal);
							errorBuild += string(" without an operand");
							addError(numLine,errorBuild);
						}
						else if (tokList.size()>1) {
							if (tokList.size()==2) {
								string errorBuild = "Syntactical Error: " + opcodeTable.getName(opcVal);
								errorBuild += string(" Operands must be separated by comma");
								addError(numLine,errorBuild);
							}
							else {
								string errorBuild = string("Syntactical Error: Too many operands for ") + opcodeTable.getName(opcVal);
								addError(numLine,errorBuild);
							}
						}
						else {
							splitTokenize(tokList.at(0),jumpTokens,",");
							if (jumpTokens.size()>2) {
								string errorBuild = "Syntactical Error: Too many operands for " + opcodeTable.getName(opcVal);
								addError(numLine,errorBuild);
							}
							string nowOp = jumpTokens.at(0);
							string arrayOp;
							if (regex_match(nowOp,regex(gensumReg))) {
								splitTokenize(nowOp,opList,"+");
								arrayOp = opList.at(1);
							}
							else if (regex_match(nowOp,regex(genarrayReg))) {
								splitTokenize(nowOp,opList,"[]");
								arrayOp = opList.at(1);
							}
							else {
								opList.push_back(nowOp);
								arrayOp = string();
							}
							//cout << "Operand[1] for " << opcodeTable.getName(opcVal) << ": " << opList << endl;
							nowOp = opList.at(0);
							if (!regex_match(nowOp,regex(auxlabelReg))) {
								if (regex_match(nowOp,regex(alnumReg))) {
									string errorBuild = "Syntactical Error: Invalid first operand for ";
									errorBuild = errorBuild + opcodeTable.getName(opcVal);
									addError(numLine,errorBuild);
								}
								else {
									addError(numLine,"Lexical Error: Invalid token");
								}
							}
							else {
								symbolTable.insertSymbol(nowOp,curAddress+1,numLine);
							}
							arrayOffset = 0;
							if (arrayOp.size()) {
								if (regex_match(arrayOp,regex(decReg))) {
									arrayOffset = atoi(arrayOp.c_str());
								}
								else {
									if (regex_match(arrayOp,regex(alnumReg))) {
										string errorBuild = "Syntactical Error: Invalid first operand for ";
										errorBuild = errorBuild + opcodeTable.getName(opcVal);
										addError(numLine,errorBuild);
									}
									else {
										addError(numLine,"Lexical Error: Invalid token");
									}
								}
							}
							normalOperands.emplace(curAddress+1,numLine);
							opList.clear();

							nowOp = jumpTokens.at(1);
							if (!regex_match(nowOp,regex(auxlabelReg))) {
								string errTok = nowOp;
								if (regex_match(errTok,regex(alnumReg))) {
									string errorBuild = string("Syntactical Error: Invalid operand for ");
									errorBuild += opcodeTable.getName(opcVal);
									addError(numLine,errorBuild);
								}
								else if (errTok.find_first_of("+[]")!=string::npos) {
									addError(numLine,"Syntactical Error: Jump commands must not take array notations");
								}
								else {
									addError(numLine,"Lexical Error: Invalid token");
								}
							}
							else {
								symbolTable.insertSymbol(nowOp,curAddress+2,numLine);
							}
							jumpOperands.emplace(curAddress+2,numLine);
						}
						textCode << opcVal << arrayOffset << 0;
						bitMap << false << true << true;
						curAddress += 3;
					}
					else {
						addError(numLine,"Syntactical Error: Invalid instruction");
					}
				}
			}
			else {
				if (tokList.size()>0) {
					int opcVal = opcodeTable.getOpcode(tokList.at(0));
					if (tokList.at(0)=="BEGIN") {
						addError(numLine,"Syntactical Error: BEGIN must come after a label");
						if (hasBegin) {
							addError(numLine,"Semantical Error: BEGIN can be used only once");
						}
						if (!textSegment) {
							addError(numLine,"Semantical Error: BEGIN must be at SECTION TEXT");
						}
						if (tokList.size()>1) {
							addError(numLine,"Syntactical Error: BEGIN must not take operands");
						}
						hasBegin = true;
						if (curAddress) {
							addError(numLine,"Semantical Error: BEGIN must be the first thing in SECTION TEXT if exists");
						}
					}
					else if (tokList.at(0)=="END") {
						if (hasEnd) {
							addError(numLine,"Semantical Error: END can be used only once");
						}
						else {
							endLine = numLine;
						}
						if (tokList.size()>1) {
							addError(numLine,"Syntactical Error: END must not take operands");
						}
						hasEnd = true;
						if (!hasBegin) {
							addError(numLine,"Semantical Error: END without a matching BEGIN statement");
						}
						if (!dataSegment) {
							addError(numLine,"Semantical Error: END must be at SECTION DATA");
						}
					}
					else if (tokList.at(0)=="EXTERN") {
						addError(numLine,"Syntactical Error: EXTERN must come after a label");
						if (tokList.size()>1) {
							addError(numLine,"Syntactical Error: EXTERN must not take operands");
						}
						if (curAddress) {
							addError(numLine,"Semantical Error: EXTERN must go at beginning of code");
						}
					}
					else if (tokList.at(0)=="PUBLIC") {
						tokList.erase(tokList.begin());
						if (curAddress) {
							addError(numLine,"Semantical Error: PUBLIC must go at beginning of code");
						}
						if (!tokList.size()) {
							addError(numLine,"Syntactical Error: PUBLIC must take an operand");
						}
						else if (tokList.size()>1) {
							addError(numLine,"Syntactical Error: Too many operands for PUBLIC statement");
						}
						else {
							string pubToken = tokList.at(0);
							if (!regex_match(pubToken,regex(auxlabelReg))) {
								if (regex_match(pubToken,regex(alnumReg))) {
									addError(numLine,"Syntactical Error: Invalid operand for PUBLIC statement");
								}
								else {
									addError(numLine,"Lexical Error: Invalid token");
								}
							}
							else {
								if (!symbolTable.isSymbol(pubToken)) {
									symbolTable.insertSymbol(pubToken,0,numLine,false,false,false,false,true);
								}
								else if (symbolTable.isExternSymbol(pubToken)) {
									addError(numLine,"Semantical Error: A symbol cannot be EXTERN and PUBLIC at the same time");
								}
								symbolTable.setSymbolFlags(pubToken,false,false,false,false,true);
							}
						}
					}
					else if (tokList.at(0)=="SPACE") {
						tokList.erase(tokList.begin());
						addError(numLine,"Syntactical Error: SPACE must come after a label");
						if (!dataSegment) {
							addError(numLine,"Semantical Error: SPACE is allowed only in SECTION DATA");
						}
						if (tokList.size()>1) {
							addError(numLine,"Syntactical Error: Too many operands for SPACE directive");
						}
						else if (tokList.size()==1 && !regex_match(tokList.at(0),regex(numReg))) {
							if (regex_match(tokList.at(0),regex(alnumReg))) {
								addError(numLine,"Syntactical Error: Invalid argument for SPACE directive");
							}
							else {
								addError(numLine,"Lexical Error: Invalid token");
							}
						}
					}
					else if (tokList.at(0)=="CONST") {
						tokList.erase(tokList.begin());
						addError(numLine,"Syntactical Error: CONST must come after a label");
						if (!dataSegment) {
							addError(numLine,"Semantical Error: CONST is allowed only in SECTION DATA");
						}
						if (!tokList.size()) {
							addError(numLine,"Syntactical Error: Too few operands for CONST directive");
						}
						else if (tokList.size()>1) {
							addError(numLine,"Syntactical Error: Too many operands for CONST directive");
						}
						else if (!regex_match(tokList.at(0),regex(numReg))){
							if (regex_match(tokList.at(0),regex(alnumReg))) {
								addError(numLine,"Syntactical Error: Invalid argument for CONST directive");
							}
							else {
								addError(numLine,"Lexical Error: Invalid token");
							}
						}
					}
					else if (getTier(opcVal)==0) {
						if (!textSegment) {
							addError(numLine,"Instructions are allowed only in SECTION TEXT");
						}
						tokList.erase(tokList.begin());
						if (!tokList.size()) {
							string errorBuild = string("Syntactical Error: ") + opcodeTable.getName(opcVal);
							errorBuild += string(" without an operand");
							addError(numLine,errorBuild);
						}
						else if (tokList.size()>1) {
							string errorBuild = string("Syntactical Error: Too many operands for ") + opcodeTable.getName(opcVal);
							addError(numLine,errorBuild);
						}
						else {
							//cout << boolalpha << "For " << tokList.at(0) << ": " << regex_match(tokList.at(0),regex(auxlabelReg));
							//cout << noboolalpha << endl;
							if (!regex_match(tokList.at(0),regex(auxlabelReg))) {
								string errTok = tokList.at(0);
								if (regex_match(errTok,regex(alnumReg))) {
									string errorBuild = string("Syntactical Error: Invalid operand for ");
									errorBuild += opcodeTable.getName(opcVal);
									addError(numLine,errorBuild);
								}
								else if (errTok.find_first_of("+[]")!=string::npos) {
									addError(numLine,"Syntactical Error: Jump commands must not take array notations");
								}
								else {
									addError(numLine,"Lexical Error: Invalid token");
								}
							}
							else {
								symbolTable.insertSymbol(tokList.at(0),curAddress+1,numLine);
							}
							jumpOperands.emplace(curAddress+1,numLine);
						}
						textCode	<< opcVal << 0; 
						bitMap	<< false << true;
						curAddress += 2;
					}
					else if (getTier(opcVal)==1) {
						int arrayOffset = 0;
						if (!textSegment) {
							addError(numLine,"Instructions are allowed only in SECTION TEXT");
						}
						tokList.erase(tokList.begin());
						if (!tokList.size()) {
							string errorBuild = string("Syntactical Error: ") + opcodeTable.getName(opcVal);
							errorBuild += string(" without an operand");
							addError(numLine,errorBuild);
						}
						else if (tokList.size()>1) {
							string errorBuild = string("Syntactical Error: Too many operands for ") + opcodeTable.getName(opcVal);
							addError(numLine,errorBuild);
						}
						else {
							string nowOp = tokList.at(0);
							string arrayOp;
							if (regex_match(nowOp,regex(gensumReg))) {
								splitTokenize(nowOp,opList,"+");
								arrayOp = opList.at(1);
							}
							else if (regex_match(nowOp,regex(genarrayReg))) {
								splitTokenize(nowOp,opList,"[]");
								arrayOp = opList.at(1);
							}
							else {
								opList.push_back(nowOp);
								arrayOp = string();
							}
							//cout << "Operand for " << opcodeTable.getName(opcVal) << ": " << opList << endl;
							nowOp = opList.at(0);
							if (!regex_match(nowOp,regex(auxlabelReg))) {
								if (regex_match(nowOp,regex(alnumReg))) {
									string errorBuild = string("Syntactical Error: Invalid operand for ");
									errorBuild += opcodeTable.getName(opcVal);
									addError(numLine,errorBuild);
								}
								else if (nowOp.find(',') == string::npos) {
									addError(numLine,"Lexical Error: Invalid token");
								}
								else {
									string errorBuild = string("Syntactical Error: Too many operands for ") + opcodeTable.getName(opcVal);
									addError(numLine,errorBuild);
								}
							}
							else {
								symbolTable.insertSymbol(nowOp,curAddress+1,numLine);
							}

							if (arrayOp.size()) {
								if (regex_match(arrayOp,regex(decReg))) {
									arrayOffset = atoi(arrayOp.c_str());
								}
								else {
									if (regex_match(arrayOp,regex(alnumReg))) {
										string errorBuild = string("Syntactical Error: Invalid operand for ");
										errorBuild += opcodeTable.getName(opcVal);
										addError(numLine,errorBuild);
									}
									else {
										addError(numLine,"Lexical Error: Invalid token");
									}
								}
							}
							normalOperands.emplace(curAddress+1,numLine);
						}
						textCode	<< opcVal << arrayOffset; 
						bitMap	<< false << true;
						curAddress += 2;
					}
					else if (getTier(opcVal)==2) {
						if (!textSegment) {
							addError(numLine,"Instructions are allowed only in SECTION TEXT");
						}
						tokList.erase(tokList.begin());
						if (tokList.size()) {
							string errorBuild = "Syntactical Error: " + opcodeTable.getName(opcVal);
							errorBuild = errorBuild + " does not allow operands";
							addError(numLine,errorBuild);
						}
						textCode << opcVal;
						bitMap	<< false;
						curAddress++;
					}
					else if (getTier(opcVal)==3) {
						vector<string> copyTokens;
						int arrayOffset = 0;
						int arrayOffset2 = 0;
						if (!textSegment) {
							addError(numLine,"Instructions are allowed only in SECTION TEXT");
						}
						tokList.erase(tokList.begin());
						if (!tokList.size()) {
							string errorBuild = string("Syntactical Error: ") + opcodeTable.getName(opcVal);
							errorBuild += string(" without an operand");
							addError(numLine,errorBuild);
						}
						else if (tokList.size()>1) {
							if (tokList.size()==2) {
								string errorBuild = "Syntactical Error: " + opcodeTable.getName(opcVal);
								errorBuild += string(" Operands must be separated by comma");
								addError(numLine,errorBuild);
							}
							else {
								string errorBuild = string("Syntactical Error: Too many operands for ") + opcodeTable.getName(opcVal);
								addError(numLine,errorBuild);
							}
						}
						else if ((tokList.at(0)).find(',') == string::npos) {
							string nowOp = tokList.at(0);
							string errorBuild = "Syntactical Error: Too few operands for " + opcodeTable.getName(opcVal);
							addError(numLine,errorBuild);
							if (!regex_match(nowOp,regex(auxlabelReg))) {
								if (regex_match(nowOp,regex(alnumReg))) {
									string errorBuild = string("Syntactical Error: Invalid first operand for ");
									errorBuild += opcodeTable.getName(opcVal);
									addError(numLine,errorBuild);
								}
								else {
									addError(numLine,"Lexical Error: Invalid token");
								}
							}
						}
						else {
							splitTokenize(tokList.at(0),copyTokens,",");
							if (copyTokens.size()>2) {
								string errorBuild = "Syntactical Error: Too many operands for " + opcodeTable.getName(opcVal);
								addError(numLine,errorBuild);
							}
							string nowOp = copyTokens.at(0);
							string arrayOp;
							if (regex_match(nowOp,regex(gensumReg))) {
								splitTokenize(nowOp,opList,"+");
								arrayOp = opList.at(1);
							}
							else if (regex_match(nowOp,regex(genarrayReg))) {
								splitTokenize(nowOp,opList,"[]");
								arrayOp = opList.at(1);
							}
							else {
								opList.push_back(nowOp);
								arrayOp = string();
							}
							//cout << "Operand[1] for " << opcodeTable.getName(opcVal) << ": " << opList << endl;
							nowOp = opList.at(0);
							if (!regex_match(nowOp,regex(auxlabelReg))) {
								if (regex_match(nowOp,regex(alnumReg))) {
									string errorBuild = "Syntactical Error: Invalid first operand for ";
									errorBuild = errorBuild + opcodeTable.getName(opcVal);
									addError(numLine,errorBuild);
								}
								else {
									addError(numLine,"Lexical Error: Invalid token");
								}
							}
							else {
								symbolTable.insertSymbol(nowOp,curAddress+1,numLine);
							}
							arrayOffset = 0;
							if (arrayOp.size()) {
								if (regex_match(arrayOp,regex(decReg))) {
									arrayOffset = atoi(arrayOp.c_str());
								}
								else {
									if (regex_match(arrayOp,regex(alnumReg))) {
										string errorBuild = "Syntactical Error: Invalid first operand for ";
										errorBuild = errorBuild + opcodeTable.getName(opcVal);
										addError(numLine,errorBuild);
									}
									else {
										addError(numLine,"Lexical Error: Invalid token");
									}
								}
							}
							normalOperands.emplace(curAddress+1,numLine);
							opList.clear();

							//cout << "Copy Token Size = " << copyTokens.size() << endl;

							nowOp = copyTokens.at(1);
							if (regex_match(nowOp,regex(gensumReg))) {
								splitTokenize(nowOp,opList,"+");
								arrayOp = opList.at(1);
							}
							else if (regex_match(nowOp,regex(genarrayReg))) {
								splitTokenize(nowOp,opList,"[]");
								arrayOp = opList.at(1);
							}
							else {
								opList.push_back(nowOp);
								arrayOp = string();
							}
							//cout << "Operand[2] for " << opcodeTable.getName(opcVal) << ": " << opList << endl;
							nowOp = opList.at(0);
							if (!regex_match(nowOp,regex(auxlabelReg))) {
								if (regex_match(nowOp,regex(alnumReg))) {
									string errorBuild = "Syntactical Error: Invalid second operand for ";
									errorBuild = errorBuild + opcodeTable.getName(opcVal);
									addError(numLine,errorBuild);
								}
								else {
									addError(numLine,"Lexical Error: Invalid token");
								}
							}
							else {
								symbolTable.insertSymbol(nowOp,curAddress+2,numLine);
							}

							if (arrayOp.size()) {
								if (regex_match(arrayOp,regex(decReg))) {
									arrayOffset2 = atoi(arrayOp.c_str());
								}
								else {
									if (regex_match(arrayOp,regex(alnumReg))) {
										string errorBuild = "Syntactical Error: Invalid second operand for ";
										errorBuild = errorBuild + opcodeTable.getName(opcVal);
										addError(numLine,errorBuild);
									}
									else {
										addError(numLine,"Lexical Error: Invalid token");
									}
								}
							}
							copyOperands.emplace(curAddress+2,numLine);
						}
						textCode << opcVal << arrayOffset << arrayOffset2;
						bitMap << false << true << true;
						curAddress += 3;
					}
					else if (getTier(opcVal)==4) {
						vector<string> jumpTokens;
						int arrayOffset = 0;
						if (!textSegment) {
							addError(numLine,"Instructions are allowed only in SECTION TEXT");
						}
						tokList.erase(tokList.begin());
						if (!tokList.size()) {
							string errorBuild = string("Syntactical Error: ") + opcodeTable.getName(opcVal);
							errorBuild += string(" without an operand");
							addError(numLine,errorBuild);
						}
						else if (tokList.size()>1) {
							if (tokList.size()==2) {
								string errorBuild = "Syntactical Error: " + opcodeTable.getName(opcVal);
								errorBuild += string(" Operands must be separated by comma");
								addError(numLine,errorBuild);
							}
							else {
								string errorBuild = string("Syntactical Error: Too many operands for ") + opcodeTable.getName(opcVal);
								addError(numLine,errorBuild);
							}
						}
						else {
							splitTokenize(tokList.at(0),jumpTokens,",");
							if (jumpTokens.size()>2) {
								string errorBuild = "Syntactical Error: Too many operands for " + opcodeTable.getName(opcVal);
								addError(numLine,errorBuild);
							}
							string nowOp = jumpTokens.at(0);
							string arrayOp;
							if (regex_match(nowOp,regex(gensumReg))) {
								splitTokenize(nowOp,opList,"+");
								arrayOp = opList.at(1);
							}
							else if (regex_match(nowOp,regex(genarrayReg))) {
								splitTokenize(nowOp,opList,"[]");
								arrayOp = opList.at(1);
							}
							else {
								opList.push_back(nowOp);
								arrayOp = string();
							}
							//cout << "Operand[1] for " << opcodeTable.getName(opcVal) << ": " << opList << endl;
							nowOp = opList.at(0);
							if (!regex_match(nowOp,regex(auxlabelReg))) {
								if (regex_match(nowOp,regex(alnumReg))) {
									string errorBuild = "Syntactical Error: Invalid first operand for ";
									errorBuild = errorBuild + opcodeTable.getName(opcVal);
									addError(numLine,errorBuild);
								}
								else {
									addError(numLine,"Lexical Error: Invalid token");
								}
							}
							else {
								symbolTable.insertSymbol(nowOp,curAddress+1,numLine);
							}
							arrayOffset = 0;
							if (arrayOp.size()) {
								if (regex_match(arrayOp,regex(decReg))) {
									arrayOffset = atoi(arrayOp.c_str());
								}
								else {
									if (regex_match(arrayOp,regex(alnumReg))) {
										string errorBuild = "Syntactical Error: Invalid first operand for ";
										errorBuild = errorBuild + opcodeTable.getName(opcVal);
										addError(numLine,errorBuild);
									}
									else {
										addError(numLine,"Lexical Error: Invalid token");
									}
								}
							}
							normalOperands.emplace(curAddress+1,numLine);
							opList.clear();

							nowOp = jumpTokens.at(1);
							if (!regex_match(nowOp,regex(auxlabelReg))) {
								string errTok = nowOp;
								if (regex_match(errTok,regex(alnumReg))) {
									string errorBuild = string("Syntactical Error: Invalid operand for ");
									errorBuild += opcodeTable.getName(opcVal);
									addError(numLine,errorBuild);
								}
								else if (errTok.find_first_of("+[]")!=string::npos) {
									addError(numLine,"Syntactical Error: Jump commands must not take array notations");
								}
								else {
									addError(numLine,"Lexical Error: Invalid token");
								}
							}
							else {
								symbolTable.insertSymbol(nowOp,curAddress+2,numLine);
							}
							jumpOperands.emplace(curAddress+2,numLine);
						}
						textCode << opcVal << arrayOffset << 0;
						bitMap << false << true << true;
						curAddress += 3;
					}
					else {
						addError(numLine,"Syntactical Error: Invalid instruction");
					}
				}
			}
		}
		numLine++;
	}
	dataStart = static_cast<int>(textCode.size());
	symbolTable.addDataOffset(dataStart);
	rawCode << textCode << dataCode;

	vector<int> lineEntry,addEntry;
	vector<string> symbolEntries = symbolTable.getEntries();

	//cout	<< "Jump Map:" << endl;
	//cout	<< jumpOperands << endl;
	vector <int> oldTextCode(textCode);
	for (string &entry : symbolEntries) {
		lineEntry = symbolTable.getUsedLines(entry);
		addEntry = symbolTable.getUsedAddress(entry);
		if (symbolTable.isPublicSymbol(entry)) {
			definitionTable.insertAddress(entry,symbolTable.getAddress(entry));
		}
		//cout	<< "entry(" << entry << ") {\n\tl[" << lineEntry << "]" << endl;
		//cout	<< "\ta[" << addEntry << "]" << endl << "}" << endl;
		for (unsigned k = 0;k < addEntry.size();k++) {
			if (jumpOperands.find(addEntry[k]) != jumpOperands.end()) {
				//cout << "Found a shit!" << endl;
				textCode.at(addEntry[k]) = symbolTable.getAddress(entry);
				if (symbolTable.isExternSymbol(entry)) {
					useTable.insertAddress(entry,addEntry[k]);
				}
				if (symbolTable.isDataSymbol(entry)) {
					addError(lineEntry[k],"Semantical Error: Attempted to jump to SECTION DATA");
				}
				else if (!symbolTable.isDefinedSymbol(entry) && symbolTable.isSymbol(entry)) {
					addError(lineEntry[k],"Semantical Error: Undefined label detected");
				}
			}
			else if (normalOperands.find(addEntry[k]) != normalOperands.end()) {
				int gotOffset = oldTextCode.at(addEntry[k]);
				int curOpcode = oldTextCode.at(addEntry[k]-1);
				int sizOffset = symbolTable.getSizeSpace(entry);
				//cout	<< "[e(o)][g,s] = [" << entry << "(" << curOpcode << ")][" << gotOffset << "," << sizOffset << "]" << endl; 
				textCode.at(addEntry[k]) += symbolTable.getAddress(entry); 
				if (symbolTable.isExternSymbol(entry)) {
					useTable.insertAddress(entry,addEntry[k]);
				}
				if (!symbolTable.isDataSymbol(entry) && symbolTable.isDefinedSymbol(entry) && !symbolTable.isExternSymbol(entry)) {
					addError(lineEntry[k],"Semantical Error: Attempted to operate inside SECTION TEXT");
				}
				else if (!symbolTable.isDefinedSymbol(entry) && symbolTable.isSymbol(entry)) {
					addError(lineEntry[k],"Semantical Error: Undefined label detected");
				}
				else if (cannotModifyConst(curOpcode) && symbolTable.isConstSymbol(entry)) {
					addError(lineEntry[k],"Semantical Error: Attempted to modify a CONST object");
				}
				else if ((curOpcode==4 || curOpcode==29) && symbolTable.isConstSymbol(entry) && !symbolTable.getConstValue(entry)) {
					addError(lineEntry[k],"Semantical Error: Attempted to divide by zero");
				}
				else if (gotOffset > sizOffset && !symbolTable.isExternSymbol(entry)) {
					addError(lineEntry[k],"Semantical Error: Attempted to access unallocated memory");
				}
			}
			else if (copyOperands.find(addEntry[k]) != copyOperands.end()) {
				int gotOffset = oldTextCode.at(addEntry[k]);
				int curOpcode = oldTextCode.at(addEntry[k]-2);
				int sizOffset = symbolTable.getSizeSpace(entry);
				//cout	<< "COPY(g,s) = [" << gotOffset << "," << sizOffset << "]" << endl;
				//cout	<< "opc = " << curOpcode << endl;
				textCode.at(addEntry[k]) += symbolTable.getAddress(entry); 
				if (symbolTable.isExternSymbol(entry)) {
					useTable.insertAddress(entry,addEntry[k]);
				}
				if (!symbolTable.isDataSymbol(entry) && symbolTable.isDefinedSymbol(entry) && !symbolTable.isExternSymbol(entry)) {
					addError(lineEntry[k],"Semantical Error: Attempted to operate inside SECTION TEXT");
				}
				else if (!symbolTable.isDefinedSymbol(entry) && symbolTable.isSymbol(entry)) {
					addError(lineEntry[k],"Semantical Error: Undefined label detected");
				}
				else if ((curOpcode==9 || curOpcode==19 || curOpcode==47) && symbolTable.isConstSymbol(entry)) {
					addError(lineEntry[k],"Semantical Error: Attempted to modify a CONST object");
				}
				else if (gotOffset > sizOffset && !symbolTable.isExternSymbol(entry)) {
					addError(lineEntry[k],"Semantical Error: Attempted to access unallocated memory");
				}
			}
		}
	}
	if (hasBegin && !hasEnd) {
		string errorBuild = string("Semantical Error: No END to match BEGIN statement at line ") + to_string(beginLine);
		addError(numLine-1,errorBuild);
	}
	if (hasEnd && (endLine != numLine-1)) {
		addError(endLine,"Semantical Error: END must be at the last line of code");
	}

	bitMap << vector<bool>(dataCode.size(),false);
	resultCode << textCode << dataCode;
	//cout << "Raw[" << rawCode << "]\nRes[" << resultCode << "]" << endl;
	if (resultCode.size()>MEMORY_MAX_SIZE) {
		addError(0,"Code Size Error: Code too big to fit");
	}
	if (!resultCode.size()) {
		addError(0,"Code Size Error: No code generated");
	}
	if (!textCode.size()) {
		addError(0,"Code Size Error: No executable code generated");
	}
	if (!symbolTable.foundLinks()) {
		outputFile << resultCode;
	}
	else {
		outputFile	<< useTable << "\n";
		outputFile	<< definitionTable << "\nCODE\n";
		outputFile	<< resultCode << "\nBITMAP\n";
		outputFile	<< bitMap;
	}

	if (this->errorList.size()) {
		printErrors();
		outputFile.close();
		remove(fileOut.c_str());
	}
	//cout	<< "\nSYMBOL TABLE:" << endl;
	//cout	<< symbolTable << endl;
	//cout	<< "END OF SYMBOL TABLE" << endl;
	//cout	<< "CODE IS\n\tt[" << textCode << "]" << endl;
	//cout	<< "\tb[" << bitMap << "]" << endl;
	//cout	<< "\td[" << dataCode << "]\nEND CODE" << endl;
	//cout	<< useTable << endl;
	//cout	<< definitionTable << endl;
	return this->errorList.size();
}

bool Assembler::alreadyHasError(int line,string msg) {
	pair <multimap<int,string>::iterator,multimap<int,string>::iterator> itPair;
	multimap<int,string>::iterator it;
	itPair = errorList.equal_range(line);
	try {
		for (it = itPair.first;it != itPair.second;it++) {
			if (it->second == msg) {
				return true;
			}
		}
	}
	catch(std::out_of_range &e) {
		return false;
	}	
	return false;
}

int Assembler::getTier(int code) {
	vector<int> tier0 =	{ 
							 5, 6, 7, 8,60
						};
	vector<int> tier1 =	{ 
							 1, 2, 3, 4,10,11,12,13,15,16,17,
							18,22,23,24,25,26,29,30,31,40,42,
							43,44,46,48,49,50
						};
	vector<int> tier2 =	{
							14,27,28,32,33,41,45,51,53,54,55,
							56,57,58,59,65
						};
	vector<int> tier3 =	{
							 9,19,20,21,47,52,61,62,63,64
						};
	vector<int> tier4 = {
							34,35,36,37,38,39
						};

	if (find(tier0.begin(),tier0.end(),code)!=tier0.end()) {
		return 0;
	}
	if (find(tier1.begin(),tier1.end(),code)!=tier1.end()) {
		return 1;
	}
	if (find(tier2.begin(),tier2.end(),code)!=tier2.end()) {
		return 2;
	}
	if (find(tier3.begin(),tier3.end(),code)!=tier3.end()) {
		return 3;
	}
	if (find(tier4.begin(),tier4.end(),code)!=tier4.end()) {
		return 4;
	}
	return -1;
}

bool Assembler::cannotModifyConst(int code) {
	vector<int> tier =	{
							11,12,19,21,26,30,31,40,46,47
						};
	return (find(tier.begin(),tier.end(),code)!=tier.end());
}

vector<int> &operator<<(vector<int> &output,const vector<int> &vec) {
	for (const int &i : vec) {
		output.push_back(i);
	}
	output.shrink_to_fit();
	return output;
}

vector<int> &operator<<(vector<int> &output,const int &val) {
	output.push_back(val);
	output.shrink_to_fit();
	return output;
}

void bigEndianAddInt(int val,vector<int> &vec) {
	for (unsigned i = 0; i < static_cast<unsigned>(sizeof(int)); i++) {
		vec.push_back((val>>(8*i))&0xFF);
	}
}

vector<bool> &operator<<(vector<bool> &output,const vector<bool> &vec) {
	for (const bool &i : vec) {
		output.push_back(i);
	}
	output.shrink_to_fit();
	return output;
}

vector<bool> &operator<<(vector<bool> &output,const bool &val) {
	output.push_back(val);
	output.shrink_to_fit();
	return output;
}

int Assembler::getDataStart() const {
	return dataStart;
}
