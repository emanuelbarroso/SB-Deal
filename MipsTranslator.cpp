/**
 * @author Emanuel B.
 * TODO Mips Translator class source code
 *
**/

#include "MipsTranslator.h"

MipsTranslator::MipsTranslator(string inName,string outName,bool hasDebugs) 
	: Intel32Translator(inName,outName,hasDebugs,false,0,0,false) {
	objectName = outName + string("_mips.s");
	textMIPS.clear();
	dataMIPS.clear();
	textMIPS.push_back(".text\n\t.globl _start\n_start:");
	dataMIPS.push_back(".data");
}

void MipsTranslator::setSpecialIO(vector<int> code,bool &op17,bool &op18,bool &op43,bool &op5r) {
	unsigned curPC = 0;
	try {
		while (curPC < code.size()) {
			int op = code.at(curPC);
			int offset = objSizes.at(op-1);
			if (op==17) {
				op17 = true;
			}
			else if(op==43 || op==56) {
				op43 = true;
			}
			else if(op==18) {
				op18 = true;
			}
			else if(op==51 || op==52) {
				op5r = true;
			}
			curPC += offset;
		}
	}
	catch (...) {

	}
}

int MipsTranslator::translateMIPSCode() {
	bool links;
	if (verboseAssemble(inName,inName,objSymbols,rawCode,resultCode,objDataSize,links)>0) {
		cerr	<< "Translation Error: Cannot translate code with assembling errors" << endl;
		return -1;
	}
	if (!keepDebug) {
		remove((string(inName + ".pre")).c_str());
		remove((string(inName + ".mcr")).c_str());
		remove((string(inName + ".o")).c_str());
	}
	if (links) {
		cerr	<< "Translation Error: Cannot translate code with linking references" << endl;
		return -1;
	}
	vector<int> textSection(resultCode.begin(),resultCode.begin()+objDataSize);
	vector<int> dataSection(resultCode.begin()+objDataSize,resultCode.end());

	int dataCurAddress = textSection.size();
	for (unsigned i = 0;i < dataSection.size();i++,dataCurAddress++) {
		string testEntry = objSymbols.getSymbol(dataCurAddress);
		if (testEntry.length() && objSymbols.isDataSymbol(testEntry)) {
			if (objSymbols.isConstSymbol(testEntry)) {
				int objAux = objSymbols.getConstValue(testEntry);
				string buildExp = testEntry + string(":\t.half ") + to_string(objAux);
				dataMIPS.push_back(buildExp);
			}
			else {
				string buildExp;
				int spaceSize = 1 + objSymbols.getSizeSpace(testEntry);
				buildExp = testEntry + string(":\t");
				if (spaceSize==1) {
					buildExp = buildExp + string(".half 0");
				}
				else {
					buildExp = buildExp + string(".half 0 : ") + to_string(spaceSize);
				}
				dataMIPS.push_back(buildExp);
			}
		}
	}	
	//for (string &k : dataMIPS) {
	//	cout	<< k << endl;
	//}
	dataMIPS.push_back(string(""));
	unsigned curAdd = 0;
	while (curAdd < textSection.size()) {
		int curOpcode = textSection.at(curAdd);
		string lineBuilder;
		int realAdd1;
		int arrayOff1; 
		int realAdd2;
		int arrayOff2;
		int stringOffset;
		string curLabel = objSymbols.getSymbol(curAdd);
		if (curLabel.length()) {
			curLabel.append(":");
			textMIPS.push_back(curLabel);
		}
		try {
			if (objSizes.at(curOpcode-1) != 1) {
				realAdd1 = findSymbol(curAdd+1);
				arrayOff1 = textSection.at(curAdd+1) - realAdd1;
				//cout << "arrayoff1[" << arrayOff1 << "]" << endl;
			}
			if (objSizes.at(curOpcode-1) == 3) {
				realAdd2 = findSymbol(curAdd+2);
				arrayOff2 = textSection.at(curAdd+2) - realAdd2;
			}
		}
		catch (...) {
			break;
		}
		string name1 = objSymbols.getSymbol(realAdd1);
		string name2 = objSymbols.getSymbol(realAdd2);
		stringOffset = 1;
		switch (curOpcode) {
			case 1:
				lineBuilder = "\tla $t0," + name1 + "\n\tlh $t1," + to_string(2*arrayOff1) + "($t0)";
				textMIPS.push_back(lineBuilder);
				textMIPS.push_back("\tadd $s0,$s0,$t1");
				break;
			case 2:
				lineBuilder = "\tla $t0," + name1 + "\n\tlh $t1," + to_string(2*arrayOff1) + "($t0)";
				textMIPS.push_back(lineBuilder);
				textMIPS.push_back("\tsub $s0,$s0,$t1");
				break;
			case 3:
				lineBuilder = "\tla $t0," + name1 + "\n\tlh $t1," + to_string(2*arrayOff1) + "($t0)";
				textMIPS.push_back(lineBuilder);
				textMIPS.push_back("\tmult $s0,$t1\n\tmflo $s0");
				break;
			case 4:
				lineBuilder = "\tla $t0," + name1 + "\n\tlh $t1," + to_string(2*arrayOff1) + "($t0)";
				textMIPS.push_back(lineBuilder);
				textMIPS.push_back("\tdiv $s0,$t1\n\tmflo $s0");
				break;
			case 5:
				lineBuilder = "\tj " + name1;
				textMIPS.push_back(lineBuilder);
				break;
			case 6:
				lineBuilder = "\tbltz $s0," + name1;
				textMIPS.push_back(lineBuilder);
				break;
			case 7:
				lineBuilder = "\tbgtz $s0," + name1;
				textMIPS.push_back(lineBuilder);
				break;
			case 8:
				lineBuilder = "\tbeq $s0,$zero," + name1;
				textMIPS.push_back(lineBuilder);
				break;
			case 9:
				lineBuilder = "\tla $t0," + name1 + "\n\tlh $t1," + to_string(2*arrayOff1) + "($t0)";
				textMIPS.push_back(lineBuilder);
				lineBuilder = "\tla $t0," + name2 + "\n\tsh $t1," + to_string(2*arrayOff2) + "($t0)";
				textMIPS.push_back(lineBuilder);
				break;
			case 10:
				lineBuilder = "\tla $t0," + name1 + "\n\tlh $s0," + to_string(2*arrayOff1) + "($t0)";
				textMIPS.push_back(lineBuilder);
				break;
			case 11:
				lineBuilder = "\tla $t0," + name1 + "\n\tsh $s0," + to_string(2*arrayOff1) + "($t0)";
				textMIPS.push_back(lineBuilder);
				break;
			case 12:
				textMIPS.push_back("\tli $v0,5\n\tsyscall");
				lineBuilder = "\tla $t0," + name1 + "\n\tsh $v0," + to_string(2*arrayOff1) + "($t0)";
				textMIPS.push_back(lineBuilder);
				break;
			case 13:
				lineBuilder = "\tla $t0," + name1 + "\n\tlh $a0," + to_string(2*arrayOff1) + "($t0)";
				textMIPS.push_back(lineBuilder);
				textMIPS.push_back("\tli $v0,1\n\tsyscall\n\tli $v0,11\n\tli $a0,10\n\tsyscall");
				break;
			case 14:
				textMIPS.push_back("\tli $v0,10\n\tsyscall");
				break;
			case 15:
				textMIPS.push_back("\tli $v0,12\n\tsyscall");
				lineBuilder = "\tla $t0," + name1 + "\n\tsh $v0," + to_string(2*arrayOff1) + "($t0)";
				textMIPS.push_back(lineBuilder);
				break;
			case 16:
				lineBuilder = "\tla $t0," + name1 + "\n\tlh $a0," + to_string(2*arrayOff1) + "($t0)";
				textMIPS.push_back(lineBuilder);
				textMIPS.push_back("\tli $v0,11\n\tsyscall\n\tli $v0,11\n\tli $a0,10\n\tsyscall");
				break;
			case 17:
				if (!objSymbols.isConstSymbol(name1) && objSymbols.isDataSymbol(name1)) {
					stringOffset = objSymbols.getSizeSpace(name1) + 1;
				}
				lineBuilder = "\tla $a0," + name1 + "\n\tli $a1," + to_string(stringOffset) + "\n\tjal LeerString";
				textMIPS.push_back(lineBuilder);
				break;
			case 18:
				if (!objSymbols.isConstSymbol(name1) && objSymbols.isDataSymbol(name1)) {
					stringOffset = objSymbols.getSizeSpace(name1) + 1;
				}
				lineBuilder = "\tla $a0," + name1 + "\n\tli $a1," + to_string(stringOffset) + "\n\tjal EscreverString";
				textMIPS.push_back(lineBuilder);
				break;
			case 19:
				lineBuilder = "\tla $t0," + name1 + "\n\tlh $t1," + to_string(2*arrayOff1) + "($t0)";
				textMIPS.push_back(lineBuilder);
				lineBuilder = "\tla $t2," + name2 + "\n\tlh $t3," + to_string(2*arrayOff2) + "($t2)";
				textMIPS.push_back(lineBuilder);
				lineBuilder = "\tsh $t3," + to_string(2*arrayOff1) + "($t0)\n\tsh $t1," + to_string(2*arrayOff2) + "($t2)";
				textMIPS.push_back(lineBuilder);
				break;
			case 20:
				lineBuilder = "\tla $t0," + name2 + "\n\tlh $t1," + to_string(2*arrayOff2) + "($t0)\n\tsll $t1,$t1,1";
				textMIPS.push_back(lineBuilder);
				lineBuilder = "\tla $t0," + name1 + "\n\tadd $t0,$t0,$t1\n\tlh $s0," + to_string(2*arrayOff1) + "($t0)";
				textMIPS.push_back(lineBuilder);
				break;
			case 21:
				lineBuilder = "\tla $t0," + name2 + "\n\tlh $t1," + to_string(2*arrayOff2) + "($t0)\n\tsll $t1,$t1,1";
				textMIPS.push_back(lineBuilder);
				lineBuilder = "\tla $t0," + name1 + "\n\tadd $t0,$t0,$t1\n\tsh $s0," + to_string(2*arrayOff1) + "($t0)";
				textMIPS.push_back(lineBuilder);
				break;
			case 22:
				lineBuilder = "\tla $t0," + name1 + "\n\tlh $t1," + to_string(2*arrayOff1) + "($t0)";
				textMIPS.push_back(lineBuilder);
				textMIPS.push_back("\tand $s0,$s0,$t1");
				break;
			case 23:
				lineBuilder = "\tla $t0," + name1 + "\n\tlh $t1," + to_string(2*arrayOff1) + "($t0)";
				textMIPS.push_back(lineBuilder);
				textMIPS.push_back("\tor $s0,$s0,$t1");
				break;
			case 24:
				lineBuilder = "\tla $t0," + name1 + "\n\tlh $t1," + to_string(2*arrayOff1) + "($t0)";
				textMIPS.push_back(lineBuilder);
				textMIPS.push_back("\txor $s0,$s0,$t1");
				break;
			case 25:
				lineBuilder = "\tla $t0," + name1 + "\n\tlh $t1," + to_string(2*arrayOff1) + "($t0)";
				textMIPS.push_back(lineBuilder);
				textMIPS.push_back("\taddi $sp,$sp,-4\n\tsw $t1,0($sp)");
				break;
			case 26:
				textMIPS.push_back("\tlw $t1,0($sp)\n\taddi $sp,$sp,4");
				lineBuilder = "\tla $t0," + name1 + "\n\tsh $t1," + to_string(2*arrayOff1) + "($t0)";
				textMIPS.push_back(lineBuilder);
				break;
			case 27:
				textMIPS.push_back("\taddi $sp,$sp,-4\n\tsw $s0,0($sp)");
				break;
			case 28:
				textMIPS.push_back("\tlw $s0,0($sp)\n\taddi $sp,$sp,4");
				break;
			case 29:
				lineBuilder = "\tla $t0," + name1 + "\n\tlh $t1," + to_string(2*arrayOff1) + "($t0)";
				textMIPS.push_back(lineBuilder);
				textMIPS.push_back("\tdiv $s0,$t1\n\tmfhi $s0");
				break;
			case 30:
				lineBuilder = "\tla $t0," + name1 + "\n\tlh $t1," + to_string(2*arrayOff1) + "($t0)";
				textMIPS.push_back(lineBuilder);
				textMIPS.push_back("\taddi $t1,$t1,1");
				lineBuilder = "\tsh $t1," + to_string(2*arrayOff1) + "($t0)";
				textMIPS.push_back(lineBuilder);
				break;
			case 31:
				lineBuilder = "\tla $t0," + name1 + "\n\tlh $t1," + to_string(2*arrayOff1) + "($t0)";
				textMIPS.push_back(lineBuilder);
				textMIPS.push_back("\taddi $t1,$t1,-1");
				lineBuilder = "\tsh $t1," + to_string(2*arrayOff1) + "($t0)";
				textMIPS.push_back(lineBuilder);
				break;
			case 32:
				textMIPS.push_back("\taddi $s0,$s0,1");
				break;
			case 33:
				textMIPS.push_back("\taddi $s0,$s0,-1");
				break;
			case 34:
				lineBuilder = "\tla $t0," + name1 + "\n\tlh $t1," + to_string(2*arrayOff1) + "($t0)";
				textMIPS.push_back(lineBuilder);
				lineBuilder = "\tbeq $s0,$t1," + name2;
				textMIPS.push_back(lineBuilder);
				break;
			case 35:
				lineBuilder = "\tla $t0," + name1 + "\n\tlh $t1," + to_string(2*arrayOff1) + "($t0)";
				textMIPS.push_back(lineBuilder);
				lineBuilder = "\tbne $s0,$t1," + name2;
				textMIPS.push_back(lineBuilder);
				break;
			case 36:
				lineBuilder = "\tla $t0," + name1 + "\n\tlh $t1," + to_string(2*arrayOff1) + "($t0)";
				textMIPS.push_back(lineBuilder);
				lineBuilder = "\tbgt $s0,$t1," + name2;
				textMIPS.push_back(lineBuilder);
				break;
			case 37:
				lineBuilder = "\tla $t0," + name1 + "\n\tlh $t1," + to_string(2*arrayOff1) + "($t0)";
				textMIPS.push_back(lineBuilder);
				lineBuilder = "\tbge $s0,$t1," + name2;
				textMIPS.push_back(lineBuilder);
				break;
			case 38:
				lineBuilder = "\tla $t0," + name1 + "\n\tlh $t1," + to_string(2*arrayOff1) + "($t0)";
				textMIPS.push_back(lineBuilder);
				lineBuilder = "\tblt $s0,$t1," + name2;
				textMIPS.push_back(lineBuilder);
				break;
			case 39:
				lineBuilder = "\tla $t0," + name1 + "\n\tlh $t1," + to_string(2*arrayOff1) + "($t0)";
				textMIPS.push_back(lineBuilder);
				lineBuilder = "\tble $s0,$t1," + name2;
				textMIPS.push_back(lineBuilder);
				break;
			case 40:
				lineBuilder = "\tla $t0," + name1 + "\n\tsh $zero," + to_string(2*arrayOff1) + "($t0)";
				textMIPS.push_back(lineBuilder);
				break;
			case 41:
				textMIPS.push_back("\tandi $s0,$s0,0");
				break;
			case 42:
				lineBuilder = "\tla $t0," + name1 + "\n\tlh $a0," + to_string(2*arrayOff1) + "($t0)";
				textMIPS.push_back(lineBuilder);
				textMIPS.push_back("\tli $v0,35\n\tsyscall\n\tli $v0,11\n\tli $a0,10\n\tsyscall");
				break;
			case 43:
				lineBuilder = "\tla $t0," + name1 + "\n\tlh $a0," + to_string(2*arrayOff1) + "($t0)";
				textMIPS.push_back(lineBuilder);
				textMIPS.push_back("\tjal EscreverOct");
				break;
			case 44:
				lineBuilder = "\tla $t0," + name1 + "\n\tlh $a0," + to_string(2*arrayOff1) + "($t0)";
				textMIPS.push_back(lineBuilder);
				textMIPS.push_back("\tli $v0,34\n\tsyscall\n\tli $v0,11\n\tli $a0,10\n\tsyscall");
				break;
			case 45:
				textMIPS.push_back("\taddi $t0,$zero,-1\n\txor $s0,$s0,$t0");
				break;
			case 46:
				lineBuilder = "\tla $t0," + name1 + "\n\tlh $t1," + to_string(2*arrayOff1) + "($t0)";
				textMIPS.push_back(lineBuilder);
				lineBuilder = "\tsh $s0," + to_string(2*arrayOff1) + "($t0)\n\taddi $s0,$t1,0";
				textMIPS.push_back(lineBuilder);
				break;
			case 47:
				lineBuilder = "\tla $t0," + name1 + "\n\tlh $t1," + to_string(2*arrayOff1) + "($t0)";
				textMIPS.push_back(lineBuilder);
				lineBuilder = "\tla $t2," + name2 + "\n\tlh $t3," + to_string(2*arrayOff2) + "($t2)";
				textMIPS.push_back(lineBuilder);
				textMIPS.push_back("\tadd $t1,$t1,$t3\n\tsra $t1,$t1,1");
				lineBuilder = "\tsh $t1," + to_string(2*arrayOff1) + "($t0)\n\tsh $t1," + to_string(2*arrayOff2) + "($t2)";
				textMIPS.push_back(lineBuilder);
				break;
			case 48:
				lineBuilder = "\tla $t0," + name1 + "\n\tlh $t1," + to_string(2*arrayOff1) + "($t0)";
				textMIPS.push_back(lineBuilder);
				textMIPS.push_back("\tsllv $s0,$s0,$t1");
				break;
			case 49:
				lineBuilder = "\tla $t0," + name1 + "\n\tlh $t1," + to_string(2*arrayOff1) + "($t0)";
				textMIPS.push_back(lineBuilder);
				textMIPS.push_back("\tsrlv $s0,$s0,$t1");
				break;
			case 50:
				lineBuilder = "\tla $t0," + name1 + "\n\tlh $t1," + to_string(2*arrayOff1) + "($t0)";
				textMIPS.push_back(lineBuilder);
				textMIPS.push_back("\tsrav $s0,$s0,$t1");
				break;
			case 51:
				textMIPS.push_back("\tli $a0,-32768\n\tli $a1,32767\n\tjal Randomizer\n\taddi $s0,$v1,0");
				break;
			case 52:
				lineBuilder = "\tla $t0," + name1 + "\n\tlh $a0," + to_string(2*arrayOff1) + "($t0)";
				textMIPS.push_back(lineBuilder);
				lineBuilder = "\tla $t0," + name2 + "\n\tlh $a1," + to_string(2*arrayOff2) + "($t0)";
				textMIPS.push_back(lineBuilder);
				textMIPS.push_back("\tjal Randomizer\n\taddi $s0,$v1,0");
				break;
			case 53:
				textMIPS.push_back("\tli $v0,5\n\tsyscall\n\taddi $s0,$v0,0");
				break;
			case 54:
				textMIPS.push_back("\taddi $a0,$s0,0\n\tli $v0,1\n\tsyscall\n\tli $v0,11\n\tli $a0,10\n\tsyscall");
				break;
			case 55:
				textMIPS.push_back("\taddi $a0,$s0,0\n\tli $v0,35\n\tsyscall\n\tli $v0,11\n\tli $a0,10\n\tsyscall");
				break;
			case 56:
				textMIPS.push_back("\taddi $a0,$s0,0\n\tjal EscreverOct");
				break;
			case 57:
				textMIPS.push_back("\taddi $a0,$s0,0\n\tli $v0,34\n\tsyscall\n\tli $v0,11\n\tli $a0,10\n\tsyscall");
				break;
			case 58:
				textMIPS.push_back("\tli $v0,12\n\tsyscall\n\taddi $s0,$v0,0");
				break;
			case 59:
				textMIPS.push_back("\taddi $a0,$s0,0\n\tli $v0,11\n\tsyscall\n\tli $v0,11\n\tli $a0,10\n\tsyscall");
				break;
			default:
				break;
		}
		textMIPS.shrink_to_fit();
		curAdd += objSizes.at(curOpcode-1);
	}
	//for (string &k : textMIPS) {
	//	cout	<< k << endl;
	//}
	bool has17 = false, has18 = false, has43 = false, has5r = false;
	setSpecialIO(textSection,has17,has18,has43,has5r);
	if (has17 || has18 || has43 || has5r) {
		textMIPS.push_back(string(""));
	}
	if (has17) {
		textMIPS.push_back("LeerString:\n\taddi $sp,$sp,-4\n\tsw $ra,0($sp)\n\taddi $t1,$a0,0\n\tli $v0,8\n\taddi $a0,$gp,0");
		textMIPS.push_back("\tsyscall\n\tli $t0,0\n\taddi $t2,$a0,0\nrs_loop:\n\tbeq $t0,$a1,rs_end\n\tlbu $t3,0($t2)");
		textMIPS.push_back("\tsh $t3,0($t1)\n\taddi $t2,$t2,1\n\taddi $t1,$t1,2\n\taddi $t0,$t0,1\n\tj rs_loop");
		textMIPS.push_back("rs_end:\n\tlw $ra,0($sp)\n\taddi $sp,$sp,4\n\tjr $ra");
		textMIPS.push_back(string(""));
	}
	if (has18) {
		textMIPS.push_back("EscreverString:\n\taddi $sp,$sp,-4\n\tsw $ra,0($sp)\n\tli $t0,0\n\taddi $t1,$gp,0");
		textMIPS.push_back("\taddi $t2,$a0,0\n\taddi $t3,$a1,0\nws_loop:\n\tbeq $t0,$t3,ws_mid\n\tlh $t4,0($t2)");
		textMIPS.push_back("\tsb $t4,0($t1)\n\taddi $t1,$t1,1\n\taddi $t2,$t2,2\n\taddi $t0,$t0,1\n\tj ws_loop\nws_mid:");
		textMIPS.push_back("\tli $t0,0\n\taddi $t1,$gp,0\nws_write:\n\tbeq $t0,$t3,ws_end\n\tlbu $a0,0($t1)");
		textMIPS.push_back("\tbeq $a0,$zero,ws_end\n\tli $v0,11\n\tsyscall\n\taddi $t1,$t1,1\n\taddi $t0,$t0,1\n\tj ws_write");
		textMIPS.push_back("ws_end:\n\tli $v0,11\n\tli $a0,10\n\tsyscall\n\tlw $ra,0($sp)\n\taddi $sp,$sp,4\n\tjr $ra");
		textMIPS.push_back(string(""));
	}
	if (has43) {
		textMIPS.push_back("EscreverOct:\n\taddi $sp,$sp,-4\n\tsw $ra,0($sp)\n\taddi $t0,$a0,0\n\tli $t2,0\nwo_loop:");
		textMIPS.push_back("\tandi $t1,$t0,7\n\taddi $sp,$sp,-1\n\tsb $t1,0($sp)\n\taddi $t2,$t2,1\n\tsrl $t0,$t0,3");
		textMIPS.push_back("\tbne $t0,$zero,wo_loop\n\tli $v0,1\n\tli $a0,0\n\tsyscall\nwo_write:\n\tblez $t2,wo_end");
		textMIPS.push_back("\tlbu $a0,0($sp)\n\taddi $sp,$sp,1\n\tli $v0,1\n\tsyscall\n\taddi $t2,$t2,-1\n\tj wo_write\nwo_end:");
		textMIPS.push_back("\tli $v0,11\n\tli $a0,10\n\tsyscall\n\tlw $ra,0($sp)\n\taddi $sp,$sp,4\n\tjr $ra");
		textMIPS.push_back(string(""));
	}
	if (has5r) {
		textMIPS.push_back("Randomizer:\n\taddi $sp,$sp,-4\n\tsw $ra,0($sp)\n\taddi $t0,$a0,0\n\taddi $t1,$a1,0");
		textMIPS.push_back("\tsub $t2,$zero,$t0\n\taddi $t2,$t2,1\n\tadd $t1,$t1,$t2\nrnd_nosum:\n\taddi $a1,$t1,0");
		textMIPS.push_back("\tli $v0,41\n\tsyscall\n\tdiv $a0,$t1\n\tmfhi $a0\n\tabs $a0,$a0\n\tadd $v1,$t0,$a0");
		textMIPS.push_back("\tlw $ra,0($sp)\n\taddi $sp,$sp,4\n\tjr $ra");
		textMIPS.push_back(string(""));
	}
	for (string &k : dataMIPS) {
		codeMIPS.push_back(k);
	}
	for (string &k : textMIPS) {
		codeMIPS.push_back(k);
	}
	ofstream assemblyOutput(objectName,ios::out);
	if (!assemblyOutput) {
		cerr	<< "Fatal Error: Could not create file " << objectName << endl; 
		return -1;
	}
	for (string &k : codeMIPS) {
		assemblyOutput << k << endl;
	}
	assemblyOutput.close();
	return 0;
}
