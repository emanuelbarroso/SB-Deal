/**
 * @author Emanuel B.
 * Simulator class source code
 *
**/

#include "Simulator.h"

Simulator::Simulator(string fileEntry) {
	fileName = fileEntry;
	curPC = 0;
	exitStatus = false;
}

int Simulator::simulateCode(bool ofbCheck) {
	enum INSTRUCTIONS {
		ADD=1,SUB,MULT,DIV,JMP,JMPN,JMPP,JMPZ,COPY,
		LOAD,STORE,INPUT,OUTPUT,STOP,C_INPUT,C_OUTPUT,
		S_INPUT,S_OUTPUT,SWAP,LOAD_R,STORE_R,AND,OR,
		XOR,PUSH_M,POP_M,PUSH_AC,POP_AC,REM,INCR_M,
		DECR_M,INCR_A,DECR_A,JMPE,JMPNE,JMPG,JMPGE,
		JMPL,JMPLE,ZERO_M,ZERO_A,B_OUTPUT,O_OUTPUT,
		H_OUTPUT,NOT_A,XCHG,LEVEL,SHL,SHR,SAR,G_RAND,
		G_RANDL,A_INPUT,A_OUTPUT,AB_OUTPUT,AO_OUTPUT,
		AH_OUTPUT,AC_INPUT,AC_OUTPUT,JMPNZ,MINM,MAXM,
		GCD,LCM,EXIT
	};

	ifstream inputFile(fileName,ios::in);
	string auxTokenizer;
	char auxChar;
	string buffer;
	int curAdd1,curAdd2;
	int strAdd;
	int temp,offset;
	stack<int> curStack;
	int binDigits[33];
	int binIndex;
	OpcodeTable opcodeTable;
	int returnVal = 0;

	if (!inputFile) {
		cerr	<< "Fatal Error: Could not open file " << fileName << endl; 
		return -1;
	}
	
	getline(inputFile,auxTokenizer);
	integerTokenize(auxTokenizer,currentCode," ");
	try {
		curPC = 0;
		curReg = 0;
		while (!exitStatus) {
			curAdd1 = 0;
			curAdd2 = 0;
			int auxOpc = currentCode.at(curPC);
			if (opcodeTable.getInstructionSize(auxOpc)>0) {
				curAdd1 = currentCode.at(curPC+1);
				curAdd2 = currentCode.at(curPC+2);
			}

			//if (currentCode.at(curPC)==14) {
			//	cout << "\t14" << endl;
			//}
			//else if (currentCode.at(curPC)==9) {
			//	cout << "\t09 " << curAdd1 << " " << curAdd2 << endl;
			//}
			//else {
			//	cout << "\t" << currentCode.at(curPC) << " " << curAdd1 << endl;
			//}
			//cout	<< "ACC IS " << curReg << endl;
			cin.ignore(cin.rdbuf()->in_avail());
			switch (currentCode.at(curPC)) {
				case ADD:
					curReg += currentCode.at(curAdd1);
					curPC += 2;
					break;
				case SUB:
					curReg -= currentCode.at(curAdd1);
					curPC += 2;
					break;
				case MULT:
					curReg *= currentCode.at(curAdd1);
					curPC += 2;
					break;
				case DIV:
					curReg /= currentCode.at(curAdd1);
					curPC += 2;
					break;
				case JMP:
					curPC = curAdd1;
					break;
				case JMPN:
					curPC = (curReg<0) ? curAdd1 : (curPC+2);
					break;
				case JMPP:
					curPC = (curReg>0) ? curAdd1 : (curPC+2);
					break;
				case JMPZ:
					curPC = (curReg==0) ? curAdd1 : (curPC+2);
					break;
				case COPY:
					currentCode.at(curAdd2) = currentCode.at(curAdd1);
					curPC += 3;
					break;
				case LOAD:
					curReg = currentCode.at(curAdd1);
					curPC += 2;
					break;
				case STORE:
					currentCode.at(curAdd1) = curReg;
					curPC += 2;
					break;
				case INPUT:
					cin	>> currentCode.at(curAdd1);
					curPC += 2;
					break;
				case OUTPUT:
					cout << currentCode.at(curAdd1) << endl;
					curPC += 2;
					break;
				case STOP:
					exitStatus = true;
					curPC++;
					break;
				case C_INPUT:
					cin >> auxChar;
					currentCode.at(curAdd1) = static_cast<int>((unsigned) auxChar);
					curPC += 2;
					break;
				case C_OUTPUT:
					auxChar = static_cast<char>(currentCode.at(curAdd1) & 0xFF);
					cout << auxChar << endl;
					curPC += 2;
					break;
				case S_INPUT:
					getline(cin,buffer);
					strAdd = curAdd1;
					for (unsigned j = 0;j < buffer.length();j++,strAdd++) {
						currentCode.at(strAdd) = static_cast<int>((unsigned) buffer[j]);
					}
					currentCode.at(strAdd) = 0x0A;
					curPC += 2;
					break;
				case S_OUTPUT:
					strAdd = curAdd1;
					while (static_cast<char>(currentCode.at(strAdd)) > 0x0D) {
						cout << static_cast<char>(currentCode.at(strAdd));
						strAdd++;
					}
					cout << endl;	
					curPC += 2;
					break;
				case SWAP:
					temp = currentCode.at(curAdd2);
					currentCode.at(curAdd2) = currentCode.at(curAdd1);
					currentCode.at(curAdd1) = temp;
					curPC += 3;
					break;
				case LOAD_R:
					offset = curAdd1 + currentCode.at(curAdd2);
					curReg = currentCode.at(offset);
					curPC += 3;
					break;
				case STORE_R:
					offset = curAdd1 + currentCode.at(curAdd2);
					currentCode.at(offset) = curReg;
					curPC += 3;
					break;
				case AND:
					curReg &= currentCode.at(curAdd1);
					curPC += 2;
					break;
				case OR:
					curReg |= currentCode.at(curAdd1);
					curPC += 2;
					break;
				case XOR:
					curReg ^= currentCode.at(curAdd1);
					curPC += 2;
					break;
				case PUSH_M:
					curStack.push(currentCode.at(curAdd1));
					curPC += 2;
					break;
				case POP_M:
					if (curStack.empty()) {
						throw 1;
					}
					currentCode.at(curAdd1) = curStack.top();
					curStack.pop();
					curPC += 2;
					break;
				case PUSH_AC:
					curStack.push(curReg);
					curPC++;
					break;
				case POP_AC:
					if (curStack.empty()) {
						throw 1;
					}
					curReg = curStack.top();
					curStack.pop();
					curPC++;
					break;
				case REM:
					curReg %= currentCode.at(curAdd1);
					curPC += 2;
					break;
				case INCR_M:
					(currentCode.at(curAdd1))++;
					curPC += 2;
					break;
				case DECR_M:
					(currentCode.at(curAdd1))--;
					curPC += 2;
					break;
				case INCR_A:
					curReg++;
					curPC++;
					break;
				case DECR_A:
					curReg--;
					curPC++;
					break;
				case JMPE:
					curPC = (curReg==currentCode.at(curAdd1)) ? curAdd2 : (curPC+3);
					break;
				case JMPNE:
					curPC = (curReg!=currentCode.at(curAdd1)) ? curAdd2 : (curPC+3);
					break;
				case JMPG:
					curPC = (curReg>currentCode.at(curAdd1)) ? curAdd2 : (curPC+3);
					break;
				case JMPGE:
					curPC = (curReg>=currentCode.at(curAdd1)) ? curAdd2 : (curPC+3);
					break;
				case JMPL:
					curPC = (curReg<currentCode.at(curAdd1)) ? curAdd2 : (curPC+3);
					break;
				case JMPLE:
					curPC = (curReg<=currentCode.at(curAdd1)) ? curAdd2 : (curPC+3);
					break;
				case ZERO_M:
					currentCode.at(curAdd1) = 0;
					curPC += 2;
					break;
				case ZERO_A:
					curReg = 0;
					curPC++;
					break;
				case B_OUTPUT:
					temp = currentCode.at(curAdd1);
					for (binIndex = 31;binIndex>-1;binIndex--) {
						binDigits[binIndex] = temp & 1;
						temp >>= 1;
					}
					for (binIndex = 0;binIndex<32;binIndex++) {
						cout	<< binDigits[binIndex];
						if (!((binIndex+1)%4) && binIndex<32) {
							cout << " ";
						}
					}
					cout	<< endl;
					curPC += 2;
					break;
				case O_OUTPUT:
					cout	<< oct << currentCode.at(curAdd1) << dec << endl;
					curPC += 2;
					break;
				case H_OUTPUT:
					cout	<< "0x" << hex << uppercase << currentCode.at(curAdd1) << dec << endl;
					curPC += 2;
					break;
				case NOT_A:
					curReg = ~curReg;
					curPC++;
					break;
				case XCHG:
					temp = currentCode.at(curAdd1);
					currentCode.at(curAdd1) = curReg;
					curReg = temp;
					curPC += 2;
					break;
				case LEVEL:
					temp = currentCode.at(curAdd1) + currentCode.at(curAdd2);
					currentCode.at(curAdd1) = temp/2;
					currentCode.at(curAdd2) = temp/2;
					curPC += 3;
					break;
				case SHL:
					curReg <<= currentCode.at(curAdd1);
					curPC += 2;
					break;
				case SHR:
					curReg = (unsigned) curReg >> currentCode.at(curAdd1);
					curPC += 2;
					break;
				case SAR:
					curReg >>= currentCode.at(curAdd1);
					curPC += 2;
					break;
				case G_RAND:
					curReg = genRand();
					curPC++;
					break;
				case G_RANDL:
					curReg = genRand(currentCode.at(curAdd1),currentCode.at(curAdd2));
					curPC += 3;
					break;
				case A_INPUT:
					cin	>> curReg;
					curPC++;
					break;
				case A_OUTPUT:
					cout << curReg << endl;
					curPC++;
					break;
				case AB_OUTPUT:
					temp = curReg;
					for (binIndex = 31;binIndex>-1;binIndex--) {
						binDigits[binIndex] = temp & 1;
						temp >>= 1;
					}
					for (binIndex = 0;binIndex<32;binIndex++) {
						cout	<< binDigits[binIndex];
						if (!((binIndex+1)%4) && binIndex<32) {
							cout << " ";
						}
					}
					cout	<< endl;
					curPC++;
					break;
				case AO_OUTPUT:
					cout	<< oct << curReg << dec << endl;
					curPC++;
					break;
				case AH_OUTPUT:
					cout	<< "0x" << hex << uppercase << curReg << dec << endl;
					curPC++;
					break;
				case AC_INPUT:
					cin >> auxChar;
					curReg = static_cast<int>((unsigned) auxChar);
					curPC++;
					break;
				case AC_OUTPUT:
					auxChar = static_cast<char>(curReg & 0xFF);
					cout << auxChar << endl;
					curPC++;
					break;
				case JMPNZ:
					curPC = (curReg!=0) ? curAdd1 : (curPC+2);
					break;
				case MINM:
					curReg = minVal(currentCode.at(curAdd1),currentCode.at(curAdd2));
					curPC += 3;
					break;
				case MAXM:
					curReg = maxVal(currentCode.at(curAdd1),currentCode.at(curAdd2));
					curPC += 3;
					break;
				case GCD:
					curReg = getGCD(currentCode.at(curAdd1),currentCode.at(curAdd2));
					curPC += 3;
					break;
				case LCM:
					curReg = getLCM(currentCode.at(curAdd1),currentCode.at(curAdd2));
					curPC += 3;
					break;
				case EXIT:
					exitStatus = true;
					returnVal = curReg;
					curPC++;
					break;
				default:
					throw std::invalid_argument("Invalid code detected");
			}
			if (ofbCheck && (curReg < -32768 || curReg > 32767)) {
				throw std::runtime_error("Register's value got out of bounds");
			}
		}
		cout	<< "$$$ Simulation ended with status " << returnVal << " $$$" << endl;
	}
	catch(const std::out_of_range &e) {
		cerr	<< "Simulation Error: Bad code or memory out-of-bounds" << endl;
	}
	catch(const std::invalid_argument &f) {
		cerr	<< "Simulation Error: " << f.what() << endl;
	}
	catch(const std::runtime_error &g) {
		cerr	<< "Simulation Error: " << g.what() << endl;
	}
	catch(const int &x) {
		cerr	<< "Simulation Error: Stack Exception" << endl;
	}
	return 0;
}

void Simulator::integerTokenize(const string &line,vector<int> &integers,const string &delimiters) {
	size_t lastPos = line.find_first_not_of(delimiters,0);
	size_t pos = line.find_first_of(delimiters,lastPos);
	while (string::npos != pos || string::npos != lastPos) {
		integers.push_back(atoi((line.substr(lastPos,pos-lastPos)).c_str()));
		lastPos = line.find_first_not_of(delimiters,pos);
		pos = line.find_first_of(delimiters,lastPos);
	}
}

int Simulator::genRand(int lower,int upper) {
	int lBound = lower,uBound = upper;
	if (lBound < 0) {
		uBound += (1-lBound);
	}
	srand(time(NULL));
	return ((rand()%uBound)+lBound);
}

int Simulator::getGCD(int lhs,int rhs) {
	int t;
	if (!lhs) return rhs;
	if (!rhs) return lhs;
	while (rhs) {
		t = rhs;
		rhs = lhs % rhs;
		lhs = t;
	}
	return lhs;
}

int Simulator::getLCM(int lhs,int rhs) {
	if (!lhs && !rhs) return 0;
	int cm = lhs*rhs;
	if (cm < 0) cm = -cm;
	return (cm/getGCD(lhs,rhs));
}
