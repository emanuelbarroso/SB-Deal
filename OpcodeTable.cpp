/**
 * @author Emanuel B.
 * Opcode Table class source code
 *
**/

#include "OpcodeTable.h"

OpcodeTable::OpcodeTable() {
	opcodeTable.emplace("ADD",1);			sizeTable.emplace("ADD",1);
	opcodeTable.emplace("SUB",2);			sizeTable.emplace("SUB",1);
	opcodeTable.emplace("MULT",3);			sizeTable.emplace("MULT",1);
	opcodeTable.emplace("DIV",4);			sizeTable.emplace("DIV",1);
	opcodeTable.emplace("JMP",5);			sizeTable.emplace("JMP",1);
	opcodeTable.emplace("JMPN",6);			sizeTable.emplace("JMPN",1);
	opcodeTable.emplace("JMPP",7);			sizeTable.emplace("JMPP",1);
	opcodeTable.emplace("JMPZ",8);			sizeTable.emplace("JMPZ",1);
	opcodeTable.emplace("COPY",9);			sizeTable.emplace("COPY",2);
	opcodeTable.emplace("LOAD",10);			sizeTable.emplace("LOAD",1);
	opcodeTable.emplace("STORE",11);		sizeTable.emplace("STORE",1);
	opcodeTable.emplace("INPUT",12);		sizeTable.emplace("INPUT",1);
	opcodeTable.emplace("OUTPUT",13);		sizeTable.emplace("OUTPUT",1);
	opcodeTable.emplace("STOP",14);			sizeTable.emplace("STOP",0);
	opcodeTable.emplace("C_INPUT",15);		sizeTable.emplace("C_INPUT",1);
	opcodeTable.emplace("C_OUTPUT",16);		sizeTable.emplace("C_OUTPUT",1);
	opcodeTable.emplace("S_INPUT",17);		sizeTable.emplace("S_INPUT",1);
	opcodeTable.emplace("S_OUTPUT",18);		sizeTable.emplace("S_OUTPUT",1);
	opcodeTable.emplace("SWAP",19);			sizeTable.emplace("SWAP",2);
	opcodeTable.emplace("LOAD_R",20);		sizeTable.emplace("LOAD_R",2);
	opcodeTable.emplace("STORE_R",21);		sizeTable.emplace("STORE_R",2);
	opcodeTable.emplace("AND",22);			sizeTable.emplace("AND",1);
	opcodeTable.emplace("OR",23);			sizeTable.emplace("OR",1);
	opcodeTable.emplace("XOR",24);			sizeTable.emplace("XOR",1);
	opcodeTable.emplace("PUSH_M",25);		sizeTable.emplace("PUSH_M",1);
	opcodeTable.emplace("POP_M",26);		sizeTable.emplace("POP_M",1);
	opcodeTable.emplace("PUSH_AC",27);		sizeTable.emplace("PUSH_AC",0);
	opcodeTable.emplace("POP_AC",28);		sizeTable.emplace("POP_AC",0);
	opcodeTable.emplace("REM",29);			sizeTable.emplace("REM",1);
	opcodeTable.emplace("INCR_M",30);		sizeTable.emplace("INCR_M",1);
	opcodeTable.emplace("DECR_M",31);		sizeTable.emplace("DECR_M",1);
	opcodeTable.emplace("INCR_A",32);		sizeTable.emplace("INCR_A",0);
	opcodeTable.emplace("DECR_A",33);		sizeTable.emplace("DECR_A",0);
	opcodeTable.emplace("JMPE",34);			sizeTable.emplace("JMPE",2);
	opcodeTable.emplace("JMPNE",35);		sizeTable.emplace("JMPNE",2);
	opcodeTable.emplace("JMPG",36);			sizeTable.emplace("JMPG",2);
	opcodeTable.emplace("JMPGE",37);		sizeTable.emplace("JMPGE",2);
	opcodeTable.emplace("JMPL",38);			sizeTable.emplace("JMPL",2);
	opcodeTable.emplace("JMPLE",39);		sizeTable.emplace("JMPLE",2);
	opcodeTable.emplace("ZERO_M",40);		sizeTable.emplace("ZERO_M",1);
	opcodeTable.emplace("ZERO_A",41);		sizeTable.emplace("ZERO_A",0);
	opcodeTable.emplace("B_OUTPUT",42);		sizeTable.emplace("B_OUTPUT",1);
	opcodeTable.emplace("O_OUTPUT",43);		sizeTable.emplace("O_OUTPUT",1);
	opcodeTable.emplace("H_OUTPUT",44);		sizeTable.emplace("H_OUTPUT",1);
	opcodeTable.emplace("NOT_A",45);		sizeTable.emplace("NOT_A",0);
	opcodeTable.emplace("XCHG",46);			sizeTable.emplace("XCHG",1);
	opcodeTable.emplace("LEVEL",47);		sizeTable.emplace("LEVEL",2);
	opcodeTable.emplace("SHL",48);			sizeTable.emplace("SHL",1);
	opcodeTable.emplace("SHR",49);			sizeTable.emplace("SHR",1);
	opcodeTable.emplace("SAR",50);			sizeTable.emplace("SAR",1);
	opcodeTable.emplace("G_RAND",51);		sizeTable.emplace("G_RAND",0);
	opcodeTable.emplace("G_RANDL",52);		sizeTable.emplace("G_RANDL",2);
	opcodeTable.emplace("A_INPUT",53);		sizeTable.emplace("A_INPUT",0);
	opcodeTable.emplace("A_OUTPUT",54);		sizeTable.emplace("A_OUTPUT",0);
	opcodeTable.emplace("AB_OUTPUT",55);	sizeTable.emplace("AB_OUTPUT",0);
	opcodeTable.emplace("AO_OUTPUT",56);	sizeTable.emplace("AO_OUTPUT",0);
	opcodeTable.emplace("AH_OUTPUT",57);	sizeTable.emplace("AH_OUTPUT",0);
	opcodeTable.emplace("AC_INPUT",58);		sizeTable.emplace("AC_INPUT",0);
	opcodeTable.emplace("AC_OUTPUT",59);	sizeTable.emplace("AC_OUTPUT",0);
	opcodeTable.emplace("JMPNZ",60);		sizeTable.emplace("JMPNZ",1);
	opcodeTable.emplace("MINM",61);			sizeTable.emplace("MINM",2);
	opcodeTable.emplace("MAXM",62);			sizeTable.emplace("MAXM",2);
	opcodeTable.emplace("GCD",63);			sizeTable.emplace("GCD",2);
	opcodeTable.emplace("LCM",64);			sizeTable.emplace("LCM",2);
	opcodeTable.emplace("EXIT",65);			sizeTable.emplace("EXIT",0);
}

bool OpcodeTable::isInstruction(string token) {
	return (opcodeTable.find(token) != opcodeTable.end());
}

int OpcodeTable::getOpcode(string token) {
	if (!isInstruction(token)) {
		return 0;
	}
	return opcodeTable.find(token)->second;
}

string OpcodeTable::getName(int opcode) {
	map<string,int>::iterator it;
	for (it = opcodeTable.begin();it != opcodeTable.end();it++) {
		if (opcode==(it->second)) {
			return (it->first);
		}
	}
	return string();
}

int OpcodeTable::getInstructionSize(string token) {
	if (!isInstruction(token)) {
		return -1;
	}
	return sizeTable.find(token)->second;
}

int OpcodeTable::getInstructionSize(int opcode) {
	return getInstructionSize(getName(opcode));
}

ostream &operator<<(ostream &output,const OpcodeTable &opcodeRef) {
	map<string,int> mapOp = opcodeRef.opcodeTable; 
	map<string,int> mapSz = opcodeRef.sizeTable;
	map<string,int>::iterator auxOp,auxSz;

	output	<< "\nOPCODE TABLE:\n";
	for (auxOp = mapOp.begin(),auxSz = mapSz.begin();auxOp != mapOp.end() && auxSz != mapSz.end();auxOp++,auxSz++) {
		output	<< "\t[(" << left << setw(2) << auxOp->second << ") " << left << setw(9) << auxOp->first
				<< ": #" << left << setw(1) << auxSz->second << " ]" << endl; 
	}
	output	<< "END OF OPCODE TABLE\n" << endl;
	return output;
}
