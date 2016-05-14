/**
 * @author Emanuel B.
 * Intel IA32 Translator class source code
 *
**/

#include "IA32Translator.h"

Intel32Translator::Intel32Translator(string si,string so,bool db,bool elf,int base,int another,bool stopProp) {
	objectName = so;
	inName = si;
	elfMode = elf;
	keepDebug = db;
	baseAddress = base;
	anotherOffset = another;
	debugAtStop = stopProp;
	machineText.clear();
	machineData.clear();
	machineCode.clear();
	arraySizes =	{
						 6, 6, 6, 7, 5, 9, 9, 9,12, 5,
						 5,12,12,12,12,12,17,17,24,13,
						13, 6, 6, 6, 6, 6, 1, 1, 9, 6,
						 6, 1, 1,12,12,12,12,12,12,10,
						 2,12,12,12, 2,12,28, 8, 8, 8,
						19,21, 9, 9, 9, 9, 9, 9, 9, 9,
						17,17,17,17, 9
					};
	objSizes =	{
					2,2,2,2,2,2,2,2,3,2,
					2,2,2,1,2,2,2,2,3,3,
					3,2,2,2,2,2,1,1,2,2,
					2,1,1,3,3,3,3,3,3,2,
					1,2,2,2,1,2,3,2,2,2,
					1,3,1,1,1,1,1,1,1,2,
					3,3,3,3,1
				};
	inputCode = {
		0xc8,0x18,0x00,0x00,0x89,0x4d,0xfc,0x29,
		0xff,0xb8,0x03,0x00,0x00,0x00,0xbb,0x00,
		0x00,0x00,0x00,0x89,0xe9,0x83,0xe9,0x14,
		0xba,0x0c,0x00,0x00,0x00,0xcd,0x80,0x89,
		0xee,0x83,0xee,0x14,0x80,0x3e,0x2d,0x75,
		0x0d,0xc7,0x45,0xe8,0x01,0x00,0x00,0x00,
		0x46,0xbb,0x00,0x00,0x00,0x00,0x80,0x3e,
		0x30,0x7c,0x1a,0x80,0x3e,0x39,0x7f,0x15,
		0x8a,0x16,0x81,0xe2,0xff,0x00,0x00,0x00,
		0x66,0x83,0xea,0x30,0x66,0x52,0x46,0x43,
		0x83,0xfb,0x0c,0x75,0xe1,0x29,0xc0,0x29,
		0xf6,0xbf,0x01,0x00,0x00,0x00,0x83,0xfb,
		0x00,0x74,0x1a,0x66,0x5e,0x50,0x89,0xf0,
		0xf7,0xe7,0x89,0xc1,0x89,0xf8,0xbe,0x0a,
		0x00,0x00,0x00,0xf7,0xe6,0x89,0xc7,0x58,
		0x01,0xc8,0x4b,0xeb,0xe1,0x83,0x7d,0xe8,
		0x01,0x75,0x02,0xf7,0xd8,0x8b,0x4d,0xfc,
		0x89,0x01,0xc9,0xc3
	};
	outputCode = {
		0xc8,0x08,0x00,0x00,0x8b,0x01,0x89,0x45,
		0xf8,0xc7,0x45,0xfc,0x2d,0x00,0x00,0x00,
		0x83,0xf8,0x00,0x7f,0x1e,0x83,0xf8,0x00,
		0x74,0x19,0xf7,0x5d,0xf8,0xb8,0x04,0x00,
		0x00,0x00,0xbb,0x01,0x00,0x00,0x00,0x89,
		0xe9,0x83,0xe9,0x04,0xba,0x01,0x00,0x00,
		0x00,0xcd,0x80,0x8b,0x45,0xf8,0x29,0xd2,
		0x29,0xf6,0xbb,0x0a,0x00,0x00,0x00,0x99,
		0xf7,0xf3,0x83,0xc2,0x30,0x52,0x46,0x83,
		0xf8,0x00,0x75,0xf3,0x8f,0x45,0xf8,0xb8,
		0x04,0x00,0x00,0x00,0xbb,0x01,0x00,0x00,
		0x00,0x89,0xe9,0x83,0xe9,0x08,0xba,0x01,
		0x00,0x00,0x00,0xcd,0x80,0x4e,0x83,0xfe,
		0x00,0x75,0xe1,0xc7,0x45,0xf8,0x0a,0x00,
		0x00,0x00,0xb8,0x04,0x00,0x00,0x00,0xbb,
		0x01,0x00,0x00,0x00,0x89,0xe9,0x83,0xe9,
		0x08,0xba,0x01,0x00,0x00,0x00,0xcd,0x80,
		0xc9,0xc3
	};
	cinputCode = {
		0xc8,0x00,0x00,0x00,0xb8,0x03,0x00,0x00,
		0x00,0xbb,0x00,0x00,0x00,0x00,0xba,0x02,
		0x00,0x00,0x00,0xcd,0x80,0x81,0x21,0xff,
		0x00,0x00,0x00,0xc9,0xc3
	};
	coutputCode = {
		0xc8,0x00,0x00,0x00,0xba,0x00,0x0a,0x00,
		0x00,0x09,0x11,0xb8,0x04,0x00,0x00,0x00,
		0xbb,0x01,0x00,0x00,0x00,0xba,0x02,0x00,
		0x00,0x00,0xcd,0x80,0x81,0x21,0xff,0x00,
		0x00,0x00,0xc9,0xc3    
	};
	sinputCode = {
		0xc8,0x18,0x00,0x00,0xc7,0x45,0xfc,0x74,
		0x21,0x0d,0x0a,0xc7,0x45,0xf8,0x69,0x6e,
		0x70,0x75,0xc7,0x45,0xf4,0x6c,0x69,0x64,
		0x20,0xc7,0x45,0xf0,0x49,0x6e,0x76,0x61,
		0x89,0xce,0x89,0xd7,0x83,0xff,0x00,0x7c,
		0x27,0xb8,0x03,0x00,0x00,0x00,0xbb,0x00,
		0x00,0x00,0x00,0x89,0xe9,0x83,0xe9,0x14,
		0xba,0x01,0x00,0x00,0x00,0xcd,0x80,0x83,
		0x7d,0xec,0x0a,0x74,0x3d,0x8b,0x55,0xec,
		0x89,0x16,0x83,0xc6,0x04,0x4f,0xeb,0xd4,
		0xb8,0x04,0x00,0x00,0x00,0xbb,0x01,0x00,
		0x00,0x00,0x89,0xe9,0x83,0xe9,0x10,0xba,
		0x10,0x00,0x00,0x00,0xcd,0x80,0xb8,0x03,
		0x00,0x00,0x00,0xbb,0x00,0x00,0x00,0x00,
		0x89,0xe9,0x83,0xe9,0x14,0xba,0x01,0x00,
		0x00,0x00,0xcd,0x80,0x83,0x7d,0xec,0x20,
		0x7f,0xe4,0xc9,0xc3
	};
	soutputCode = {
		0xc8,0x04,0x00,0x00,0xc7,0x45,0xfc,0x0a,
		0x0d,0x00,0x00,0x89,0xce,0x89,0xd7,0xb8,
		0x04,0x00,0x00,0x00,0x99,0xf7,0xe7,0x89,
		0xc7,0x83,0xff,0x00,0x74,0x1b,0xb8,0x04,
		0x00,0x00,0x00,0xbb,0x01,0x00,0x00,0x00,
		0x89,0xf1,0xba,0x01,0x00,0x00,0x00,0xcd,
		0x80,0x83,0xc6,0x04,0x83,0xef,0x04,0xeb,
		0xe0,0xb8,0x04,0x00,0x00,0x00,0xbb,0x01,
		0x00,0x00,0x00,0x89,0xe9,0x83,0xe9,0x04,
		0xba,0x02,0x00,0x00,0x00,0xcd,0x80,0xc9,
		0xc3
	};
	boutputCode = {
		0xc8,0x0a,0x00,0x00,0xc7,0x45,0xfc,0x20,
		0x00,0x00,0x00,0xc7,0x45,0xf8,0x0a,0x00,
		0x00,0x00,0x57,0x31,0xd2,0x8b,0x39,0xb9,
		0x20,0x00,0x00,0x00,0x66,0xba,0x30,0x00,
		0xf7,0xc7,0x01,0x00,0x00,0x00,0x74,0x01,
		0x42,0x66,0x52,0xd1,0xff,0xe2,0xed,0xb9,
		0x20,0x00,0x00,0x00,0x31,0xd2,0x66,0x5a,
		0x66,0x89,0x55,0xf6,0x51,0x83,0xf9,0x20,
		0x74,0x1e,0xf7,0xc1,0x03,0x00,0x00,0x00,
		0x75,0x16,0xb8,0x04,0x00,0x00,0x00,0xbb,
		0x01,0x00,0x00,0x00,0x89,0xe9,0x83,0xe9,
		0x04,0xba,0x01,0x00,0x00,0x00,0xcd,0x80,
		0xb8,0x04,0x00,0x00,0x00,0xbb,0x01,0x00,
		0x00,0x00,0x89,0xe9,0x83,0xe9,0x0a,0xba,
		0x01,0x00,0x00,0x00,0xcd,0x80,0x59,0xe2,
		0xbb,0xb8,0x04,0x00,0x00,0x00,0xbb,0x01,
		0x00,0x00,0x00,0x89,0xe9,0x83,0xe9,0x08,
		0xba,0x01,0x00,0x00,0x00,0xcd,0x80,0x5f,
		0xc9,0xc3
	};
	ooutputCode = {
		0xc8,0x08,0x00,0x00,0x57,0x8b,0x39,0xc7,
		0x45,0xfc,0x20,0x00,0x0a,0x00,0xb9,0x0c,
		0x00,0x00,0x00,0x89,0x7d,0xf8,0x83,0x65,
		0xf8,0x07,0x83,0x45,0xf8,0x30,0xff,0x75,
		0xf8,0xc1,0xef,0x03,0xe2,0xed,0xb9,0x0c,
		0x00,0x00,0x00,0x8f,0x45,0xf8,0x51,0x83,
		0xf9,0x0c,0x74,0x24,0x89,0xc8,0xbb,0x03,
		0x00,0x00,0x00,0xf7,0xf3,0x83,0xfa,0x01,
		0x75,0x16,0xb8,0x04,0x00,0x00,0x00,0xbb,
		0x01,0x00,0x00,0x00,0x89,0xe9,0x83,0xe9,
		0x04,0xba,0x01,0x00,0x00,0x00,0xcd,0x80,
		0xb8,0x04,0x00,0x00,0x00,0xbb,0x01,0x00,
		0x00,0x00,0x89,0xe9,0x83,0xe9,0x08,0xba,
		0x01,0x00,0x00,0x00,0xcd,0x80,0x59,0xe2,
		0xba,0xb8,0x04,0x00,0x00,0x00,0xbb,0x01,
		0x00,0x00,0x00,0x89,0xe9,0x83,0xe9,0x02,
		0xba,0x01,0x00,0x00,0x00,0xcd,0x80,0x5f,
		0xc9,0xc3
	};
	houtputCode = {
		0xc8,0x08,0x00,0x00,0x57,0x8b,0x39,0xc7,
		0x45,0xfc,0x30,0x78,0x0a,0x00,0xb9,0x08,
		0x00,0x00,0x00,0x89,0x7d,0xf8,0x83,0x65,
		0xf8,0x0f,0x83,0x7d,0xf8,0x0a,0x7c,0x04,
		0x83,0x45,0xf8,0x07,0x83,0x45,0xf8,0x30,
		0xff,0x75,0xf8,0xc1,0xff,0x04,0xe2,0xe3,
		0xb8,0x04,0x00,0x00,0x00,0xbb,0x01,0x00,
		0x00,0x00,0x89,0xe9,0x83,0xe9,0x04,0xba,
		0x02,0x00,0x00,0x00,0xcd,0x80,0xb9,0x08,
		0x00,0x00,0x00,0x8f,0x45,0xf8,0x51,0xb8,
		0x04,0x00,0x00,0x00,0xbb,0x01,0x00,0x00,
		0x00,0x89,0xe9,0x83,0xe9,0x08,0xba,0x01,
		0x00,0x00,0x00,0xcd,0x80,0x59,0xe2,0xe3,
		0xb8,0x04,0x00,0x00,0x00,0xbb,0x01,0x00,
		0x00,0x00,0x89,0xe9,0x83,0xe9,0x02,0xba,
		0x01,0x00,0x00,0x00,0xcd,0x80,0x5f,0xc9,
		0xc3
	};
	randomCode = {
		0xc8,0x14,0x00,0x00,0x57,0x89,0xcf,0x89,
		0x5d,0xfc,0x89,0x55,0xf8,0xc7,0x45,0xf4,
		0x8f,0xbc,0x00,0x00,0xc7,0x45,0xf0,0xff,
		0xff,0xff,0x7f,0xc7,0x45,0xec,0x00,0x00,
		0x00,0x00,0x8b,0x45,0xfc,0xf7,0xd8,0x01,
		0x45,0xf8,0xff,0x45,0xf8,0x0f,0x31,0x89,
		0xd1,0x81,0xe1,0xff,0xff,0x00,0x00,0xc1,
		0xe9,0x02,0xf7,0x65,0xf4,0x03,0x45,0xec,
		0x23,0x45,0xf0,0xe2,0xf5,0x99,0xf7,0x75,
		0xf8,0x89,0xd0,0x03,0x45,0xfc,0x89,0x07,
		0x5f,0xc9,0xc3
	};
	minmCode = {
		0xc8,0x00,0x00,0x00,0x39,0xcb,0x7c,0x04,
		0x89,0xc8,0xeb,0x02,0x89,0xd8,0xc9,0xc3
	};
	maxmCode = {
		0xc8,0x00,0x00,0x00,0x39,0xcb,0x7f,0x04,
		0x89,0xc8,0xeb,0x02,0x89,0xd8,0xc9,0xc3
	};
	gcdCode = {
		0xc8,0x04,0x00,0x00,0x83,0xfb,0x00,0x74,
		0x27,0x83,0xf9,0x00,0x75,0x04,0x89,0xd9,
		0xeb,0x1e,0x83,0xfb,0x00,0x7f,0x02,0xf7,
		0xdb,0x83,0xf9,0x00,0x7f,0x02,0xf7,0xd9,
		0x39,0xcb,0x74,0x0c,0x39,0xcb,0x7f,0x04,
		0x29,0xd9,0xeb,0xf4,0x29,0xcb,0xeb,0xf0,
		0x89,0xc8,0xc9,0xc3
	};
	lcmCode = {
		0xc8,0x00,0x00,0x00,0x56,0x53,0x51,0xe8,
		0xc0,0xff,0xff,0xff,0x89,0xc6,0x59,0x5b,
		0x89,0xd8,0xf7,0xe9,0xf7,0xfe,0x83,0xf8,
		0x00,0x7f,0x02,0xf7,0xd8,0x5e,0xc9,0xc3
	};
	intelText.push_back(string("section .text"));
	intelText.push_back(string("\tglobal _start"));
	intelText.push_back(string("_start:"));
	intelData.push_back(string("section .data"));
}

void Intel32Translator::setIOFlags(vector<int> code,bool &op12,bool &op13,bool &op15,bool &op16,bool &op17,bool &op18) {
	unsigned curPC = 0;
	try {
		while (curPC < code.size()) {
			int op = code.at(curPC);
			int offset = objSizes.at(op-1);
			if (op==12 || op==53) {
				op12 = true;
			}
			else if(op==13 || op==54) {
				op13 = true;
			}
			else if(op==15 || op==58) {
				op15 = true;
			}
			else if(op==16 || op==59) {
				op16 = true;
			}
			else if(op==17) {
				op17 = true;
			}
			else if(op==18) {
				op18 = true;
			}
			curPC += offset;
		}
	}
	catch (...) {

	}
}

void Intel32Translator::setIONewFlags(vector<int> code,bool &op42,bool &op43,bool &op44,bool &op5r) {
	unsigned curPC = 0;
	try {
		while (curPC < code.size()) {
			int op = code.at(curPC);
			int offset = objSizes.at(op-1);
			if (op==42 || op==55) {
				op42 = true;
			}
			else if(op==43 || op==56) {
				op43 = true;
			}
			else if(op==44 || op==57) {
				op44 = true;
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

void Intel32Translator::setAdvFlags(vector<int> code,bool &op61,bool &op62,bool &op63,bool &op64) {
	unsigned curPC = 0;
	try {
		while (curPC < code.size()) {
			int op = code.at(curPC);
			int offset = objSizes.at(op-1);
			if (op==61) {
				op61 = true;
			}
			else if(op==62) {
				op62 = true;
			}
			else if(op==63) {
				op63 = true;
			}
			else if(op==64) {
				op64 = true;
			}
			curPC += offset;
		}
	}
	catch (...) {

	}
}

int Intel32Translator::getTextSize(vector<int> code,unsigned breakPoint) {
	unsigned curPC = 0;
	int result = 0;
	try {
		while (curPC < breakPoint && curPC < code.size()) {
			int offset = code.at(curPC)-1;
			result += arraySizes.at(offset);
			curPC += objSizes.at(offset);
		}
	}
	catch (std::out_of_range &e) {

	}
	return result;
}

int Intel32Translator::translateCode() {
	bool links;
	if (verboseAssemble(inName,inName,objSymbols,rawCode,resultCode,objDataSize,links) > 0) {
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

	//cout << "\tcur[" << resultCode << "]" << endl;
	vector<int> arrayOffsets = resultCode - rawCode;
	//cout << "\toff[" << arrayOffsets << "]" << endl;
	//cout << "Symbol Table:\n\n" << objSymbols << endl;
	vector<int> textSection(resultCode.begin(),resultCode.begin()+objDataSize);
	vector<int> dataSection(resultCode.begin()+objDataSize,resultCode.end());
	//cout	<< "\tt[" << textSection << "]\n\td[" << dataSection << "]" << endl;
	//cout	<< "IA32 size will be at least (" << getTextSize(textSection) << ")" << endl;

	vector<string> symEntries = objSymbols.getEntries();
	for (string &oneEntry : symEntries) {
		if (!objSymbols.isDataSymbol(oneEntry)) {
			int intelOffset = getTextSize(textSection,objSymbols.getAddress(oneEntry));
			intelSymbols.insertSymbol(oneEntry,baseAddress + intelOffset,0);
			intelSymbols.setSymbolFlags(oneEntry,false,false,true);
		}
	}

	bool has12 = false,has13 = false,has15 = false,has16 = false,has17 = false,has18 = false;
	bool has42 = false,has43 = false,has44 = false,has5r = false;
	bool has61 = false,has62 = false,has63 = false,has64 = false;
	setIOFlags(textSection,has12,has13,has15,has16,has17,has18);
	setIONewFlags(textSection,has42,has43,has44,has5r);
	setAdvFlags(textSection,has61,has62,has63,has64);
	textSize = getTextSize(textSection);
	if (has12) {
		intelSymbols.insertSymbol("LeerInteiro",baseAddress + textSize,0);
		intelSymbols.setSymbolFlags("LeerInteiro",false,false,true);
		intelSymbols.insertSymbol("r_no_sgn",baseAddress + textSize + 0x36,0);
		intelSymbols.setSymbolFlags("r_no_sgn",false,false,true);
		intelSymbols.insertSymbol("read_phase2",baseAddress + textSize + 0x55,0);
		intelSymbols.setSymbolFlags("read_phase2",false,false,true);
		intelSymbols.insertSymbol("read_loop",baseAddress + textSize + 0x5e,0);
		intelSymbols.setSymbolFlags("read_loop",false,false,true);
		intelSymbols.insertSymbol("end_read",baseAddress + textSize + 0x7d,0);
		intelSymbols.setSymbolFlags("end_read",false,false,true);
		intelSymbols.insertSymbol("no_negate",baseAddress + textSize + 0x85,0);
		intelSymbols.setSymbolFlags("no_negate",false,false,true);
		textSize += inputCode.size();
	}
	if (has13) {
		intelSymbols.insertSymbol("EscreverInteiro",baseAddress + textSize,0);
		intelSymbols.setSymbolFlags("EscreverInteiro",false,false,true);
		intelSymbols.insertSymbol("w_no_sgn",baseAddress + textSize + 0x33,0);
		intelSymbols.setSymbolFlags("w_no_sgn",false,false,true);
		intelSymbols.insertSymbol("write_loop1",baseAddress + textSize + 0x3f,0);
		intelSymbols.setSymbolFlags("write_loop1",false,false,true);
		intelSymbols.insertSymbol("write_loop2",baseAddress + textSize + 0x4c,0);
		intelSymbols.setSymbolFlags("write_loop2",false,false,true);
		textSize += outputCode.size();
	}
	if (has15) {
		intelSymbols.insertSymbol("LeerChar",baseAddress + textSize,0);
		intelSymbols.setSymbolFlags("LeerChar",false,false,true);
		textSize += cinputCode.size();
	}
	if (has16) {
		intelSymbols.insertSymbol("EscreverChar",baseAddress + textSize,0);
		intelSymbols.setSymbolFlags("EscreverChar",false,false,true);
		textSize += coutputCode.size();
	}
	if (has17) {
		intelSymbols.insertSymbol("LeerString",baseAddress + textSize,0);
		intelSymbols.setSymbolFlags("LeerString",false,false,true);
		intelSymbols.insertSymbol("rs_loop",baseAddress + textSize + 0x24,0);
		intelSymbols.setSymbolFlags("rs_loop",false,false,true);
		intelSymbols.insertSymbol("rs_error",baseAddress + textSize + 0x50,0);
		intelSymbols.setSymbolFlags("rs_error",false,false,true);
		intelSymbols.insertSymbol("rs_flush",baseAddress + textSize + 0x66,0);
		intelSymbols.setSymbolFlags("rs_flush",false,false,true);
		intelSymbols.insertSymbol("rs_advance",baseAddress + textSize + 0x82,0);
		intelSymbols.setSymbolFlags("rs_advance",false,false,true);
		textSize += sinputCode.size();
	}
	if (has18) {
		intelSymbols.insertSymbol("EscreverString",baseAddress + textSize,0);
		intelSymbols.setSymbolFlags("EscreverString",false,false,true);
		intelSymbols.insertSymbol("ws_loop",baseAddress + textSize + 0x19,0);
		intelSymbols.setSymbolFlags("ws_loop",false,false,true);
		intelSymbols.insertSymbol("ws_loop_end",baseAddress + textSize + 0x39,0);
		intelSymbols.setSymbolFlags("ws_loop_end",false,false,true);
		textSize += soutputCode.size();
	}
	if (has42) {
		intelSymbols.insertSymbol("EscreverBin",baseAddress + textSize,0);
		intelSymbols.setSymbolFlags("EscreverBin",false,false,true);
		intelSymbols.insertSymbol("wb_loop1",baseAddress + textSize + 0x1c,0);
		intelSymbols.setSymbolFlags("wb_loop1",false,false,true);
		intelSymbols.insertSymbol("wb_loop1_adv",baseAddress + textSize + 0x29,0);
		intelSymbols.setSymbolFlags("wb_loop1_adv",false,false,true);
		intelSymbols.insertSymbol("wb_loop2",baseAddress + textSize + 0x34,0);
		intelSymbols.setSymbolFlags("wb_loop2",false,false,true);
		intelSymbols.insertSymbol("wb_no_spc",baseAddress + textSize + 0x60,0);
		intelSymbols.setSymbolFlags("wb_no_spc",false,false,true);
		textSize += boutputCode.size();
	}
	if (has43) {
		intelSymbols.insertSymbol("EscreverOct",baseAddress + textSize,0);
		intelSymbols.setSymbolFlags("EscreverOct",false,false,true);
		intelSymbols.insertSymbol("wo_loop1",baseAddress + textSize + 0x13,0);
		intelSymbols.setSymbolFlags("wo_loop1",false,false,true);
		intelSymbols.insertSymbol("wo_loop2",baseAddress + textSize + 0x2b,0);
		intelSymbols.setSymbolFlags("wo_loop2",false,false,true);
		intelSymbols.insertSymbol("wo_no_spc",baseAddress + textSize + 0x58,0);
		intelSymbols.setSymbolFlags("wo_no_spc",false,false,true);
		textSize += ooutputCode.size();
	}
	if (has44) {
		intelSymbols.insertSymbol("EscreverHex",baseAddress + textSize,0);
		intelSymbols.setSymbolFlags("EscreverHex",false,false,true);
		intelSymbols.insertSymbol("wh_loop1",baseAddress + textSize + 0x13,0);
		intelSymbols.setSymbolFlags("wh_loop1",false,false,true);
		intelSymbols.insertSymbol("wh_no_sum",baseAddress + textSize + 0x24,0);
		intelSymbols.setSymbolFlags("wh_no_sum",false,false,true);
		intelSymbols.insertSymbol("wh_loop2",baseAddress + textSize + 0x4b,0);
		intelSymbols.setSymbolFlags("wh_loop2",false,false,true);
		textSize += houtputCode.size();
	}
	if (has5r) {
		intelSymbols.insertSymbol("Randomizer",baseAddress + textSize,0);
		intelSymbols.setSymbolFlags("Randomizer",false,false,true);
		intelSymbols.insertSymbol("random_adv",baseAddress + textSize + 0x2d,0);
		intelSymbols.setSymbolFlags("random_adv",false,false,true);
		intelSymbols.insertSymbol("random_loop",baseAddress + textSize + 0x3a,0);
		intelSymbols.setSymbolFlags("random_loop",false,false,true);
		textSize += randomCode.size();
	}
	if (has61) {
		intelSymbols.insertSymbol("Least",baseAddress + textSize,0);
		intelSymbols.setSymbolFlags("Least",false,false,true);
		intelSymbols.insertSymbol("d_min",baseAddress + textSize + 0x0c,0);
		intelSymbols.setSymbolFlags("d_min",false,false,true);
		intelSymbols.insertSymbol("min_end",baseAddress + textSize + 0x0e,0);
		intelSymbols.setSymbolFlags("min_end",false,false,true);
		textSize += minmCode.size();
	}
	if (has62) {
		intelSymbols.insertSymbol("Greatest",baseAddress + textSize,0);
		intelSymbols.setSymbolFlags("Greatest",false,false,true);
		intelSymbols.insertSymbol("d_max",baseAddress + textSize + 0x0c,0);
		intelSymbols.setSymbolFlags("d_max",false,false,true);
		intelSymbols.insertSymbol("max_end",baseAddress + textSize + 0x0e,0);
		intelSymbols.setSymbolFlags("max_end",false,false,true);
		textSize += maxmCode.size();
	}
	if (has63 || has64) {
		intelSymbols.insertSymbol("ComputeGCD",baseAddress + textSize,0);
		intelSymbols.setSymbolFlags("ComputeGCD",false,false,true);
		intelSymbols.insertSymbol("gcd_adv1",baseAddress + textSize + 0x12,0);
		intelSymbols.setSymbolFlags("gcd_adv1",false,false,true);
		intelSymbols.insertSymbol("gcd_adv2",baseAddress + textSize + 0x19,0);
		intelSymbols.setSymbolFlags("gcd_adv2",false,false,true);
		intelSymbols.insertSymbol("gcd_loop",baseAddress + textSize + 0x20,0);
		intelSymbols.setSymbolFlags("gcd_loop",false,false,true);
		intelSymbols.insertSymbol("gcd_bc",baseAddress + textSize + 0x2c,0);
		intelSymbols.setSymbolFlags("gcd_bc",false,false,true);
		intelSymbols.insertSymbol("end_gcd",baseAddress + textSize + 0x30,0);
		intelSymbols.setSymbolFlags("end_gcd",false,false,true);
		textSize += gcdCode.size();
	}
	if (has64) {
		intelSymbols.insertSymbol("ComputeLCM",baseAddress + textSize,0);
		intelSymbols.setSymbolFlags("ComputeLCM",false,false,true);
		intelSymbols.insertSymbol("lcm_end",baseAddress + textSize + 0x1d,0);
		intelSymbols.setSymbolFlags("lcm_end",false,false,true);
		textSize += lcmCode.size();
	}
	int intelDataStart = baseAddress + textSize + anotherOffset;
	intelDataStart += (4 - (intelDataStart % 4));

	for (string &oneEntry : symEntries) {
		if (objSymbols.isDataSymbol(oneEntry)) {
			int intelDataOffset = objSymbols.getAddress(oneEntry) - (int) textSection.size();
			//cout << "iDO[" << intelDataOffset << "]" << endl;
			intelSymbols.insertSymbol(oneEntry,intelDataStart + 4*intelDataOffset,0);
			intelSymbols.setSymbolFlags(oneEntry,objSymbols.isConstSymbol(oneEntry),!objSymbols.isConstSymbol(oneEntry),true);
		}
	}

	int dataCurAddress = textSection.size();
	for (unsigned i = 0;i < dataSection.size();i++,dataCurAddress++) {
		machineData << dataSection.at(i);
		string testEntry = objSymbols.getSymbol(dataCurAddress);
		if (testEntry.length() && objSymbols.isDataSymbol(testEntry)) {
			if (objSymbols.isConstSymbol(testEntry)) {
				int objAux = objSymbols.getConstValue(testEntry);
				string buildExp = testEntry + string(":\tdd ") + to_string(objAux);
				intelData	<< buildExp;
			}
			else {
				string buildExp;
				int spaceSize = 1 + objSymbols.getSizeSpace(testEntry);
				buildExp = testEntry + string(":\t");
				if (spaceSize==1) {
					buildExp = buildExp + string("dd 0");
				}
				else {
					buildExp = buildExp + string("times ") + to_string(spaceSize) + string(" dd 0");
				}
				intelData	<< buildExp;
			}
		}
	}

	unsigned curAdd = 0,iaAdd = baseAddress;
	while (curAdd < textSection.size()) {
		int curOpcode = textSection.at(curAdd);
		string lineBuilder;
		int realAdd1;
		int arrayOff1; 
		int realAdd2;
		int arrayOff2;
		int addHolder;
		int relativeOffset;
		int stringOffset;
		string curLabel = intelSymbols.getSymbol(iaAdd);
		if (curLabel.length()) {
			curLabel.append(":");
			intelText.push_back(curLabel);
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
		//cout	<< "bum(" << curAdd << "," << hex << iaAdd << ")" << endl;
		switch(curOpcode) {
			case 1:
				lineBuilder = "\tadd eax,[" + name1;
				if (arrayOff1) {
					lineBuilder = lineBuilder + "+" + to_string(4*arrayOff1);
				}
				lineBuilder = lineBuilder + "]";
				addHolder = intelSymbols.getAddress(name1) + 4*arrayOff1;
				intelText.push_back(lineBuilder);
				machineText.push_back(0x03);
				machineText.push_back(0x05);
				machineText << addHolder;
				break;
			case 2:
				lineBuilder = "\tsub eax,[" + name1;
				if (arrayOff1) {
					lineBuilder = lineBuilder + "+" + to_string(4*arrayOff1);
				}
				lineBuilder = lineBuilder + "]";
				addHolder = intelSymbols.getAddress(name1) + 4*arrayOff1;
				intelText.push_back(lineBuilder);
				machineText.push_back(0x2b);
				machineText.push_back(0x05);
				machineText << addHolder;
				break;
			case 3:
				lineBuilder = "\timul dword [" + name1;
				if (arrayOff1) {
					lineBuilder = lineBuilder + "+" + to_string(4*arrayOff1);
				}
				lineBuilder = lineBuilder + "]";
				addHolder = intelSymbols.getAddress(name1) + 4*arrayOff1;
				intelText.push_back(lineBuilder);
				machineText.push_back(0xf7);
				machineText.push_back(0x2d);
				machineText << addHolder;
				break;
			case 4:
				lineBuilder = "\tidiv dword [" + name1;
				if (arrayOff1) {
					lineBuilder = lineBuilder + "+" + to_string(4*arrayOff1);
				}
				lineBuilder = lineBuilder + "]";
				addHolder = intelSymbols.getAddress(name1) + 4*arrayOff1;
				intelText.push_back(string("\tcdq"));
				intelText.push_back(lineBuilder);
				machineText.push_back(0x99);
				machineText.push_back(0xf7);
				machineText.push_back(0x3d);
				machineText << addHolder;
				break;
			case 5:
				lineBuilder = "\tjmp " + name1;
				relativeOffset = intelSymbols.getAddress(name1) - (iaAdd + 5);
				intelText.push_back(lineBuilder);
				machineText.push_back(0xe9);
				machineText << relativeOffset;
				break;
			case 6:
				lineBuilder = "\tcmp eax,0\n\tjl " + name1;
				relativeOffset = intelSymbols.getAddress(name1) - (iaAdd + 9);
				intelText.push_back(lineBuilder);
				machineText.push_back(0x83);
				machineText.push_back(0xf8);
				machineText.push_back(0x00);
				machineText.push_back(0x0f);
				machineText.push_back(0x8c);
				machineText << relativeOffset;
				break;
			case 7:
				lineBuilder = "\tcmp eax,0\n\tjg " + name1;
				relativeOffset = intelSymbols.getAddress(name1) - (iaAdd + 9);
				intelText.push_back(lineBuilder);
				machineText.push_back(0x83);
				machineText.push_back(0xf8);
				machineText.push_back(0x00);
				machineText.push_back(0x0f);
				machineText.push_back(0x8f);
				machineText << relativeOffset;
				break;
			case 8:
				lineBuilder = "\tcmp eax,0\n\tje " + name1;
				relativeOffset = intelSymbols.getAddress(name1) - (iaAdd + 9);
				intelText.push_back(lineBuilder);
				machineText.push_back(0x83);
				machineText.push_back(0xf8);
				machineText.push_back(0x00);
				machineText.push_back(0x0f);
				machineText.push_back(0x84);
				machineText << relativeOffset;
				break;
			case 9:
				lineBuilder = "\tmov ecx,[" + name1;
				if (arrayOff1) {
					lineBuilder = lineBuilder + "+" + to_string(4*arrayOff1);
				}
				lineBuilder = lineBuilder + "]";
				addHolder = intelSymbols.getAddress(name1) + 4*arrayOff1;
				intelText.push_back(lineBuilder);
				machineText.push_back(0x8b);
				machineText.push_back(0x0d);
				machineText << addHolder;
				lineBuilder = "\tmov dword [" + name2;
				if (arrayOff2) {
					lineBuilder = lineBuilder + "+" + to_string(4*arrayOff2);
				}
				lineBuilder = lineBuilder + "],ecx";
				addHolder = intelSymbols.getAddress(name2) + 4*arrayOff2;
				intelText.push_back(lineBuilder);
				machineText.push_back(0x89);
				machineText.push_back(0x0d);
				machineText << addHolder;
				break;
			case 10:
				lineBuilder = "\tmov eax,[" + name1;
				if (arrayOff1) {
					lineBuilder = lineBuilder + "+" + to_string(4*arrayOff1);
				}
				lineBuilder = lineBuilder + "]";
				addHolder = intelSymbols.getAddress(name1) + 4*arrayOff1;
				intelText.push_back(lineBuilder);
				machineText.push_back(0xa1);
				machineText << addHolder;
				break;
			case 11:
				lineBuilder = "\tmov dword [" + name1;
				if (arrayOff1) {
					lineBuilder = lineBuilder + "+" + to_string(4*arrayOff1);
				}
				lineBuilder = lineBuilder + "],eax";
				addHolder = intelSymbols.getAddress(name1) + 4*arrayOff1;
				intelText.push_back(lineBuilder);
				machineText.push_back(0xa3);
				machineText << addHolder;
				break;
			case 12:
				lineBuilder = "\tpush eax\n\tmov ecx," + name1;
				if (arrayOff1) {
					lineBuilder = lineBuilder + "+" + to_string(4*arrayOff1);
				}
				addHolder = intelSymbols.getAddress(name1) + 4*arrayOff1;
				intelText.push_back(lineBuilder);
				intelText.push_back(string("\tcall LeerInteiro\n\tpop eax"));
				machineText.push_back(0x50);
				machineText.push_back(0xb9);
				machineText << addHolder;
				machineText.push_back(0xe8);
				relativeOffset = intelSymbols.getAddress(string("LeerInteiro")) - (iaAdd + 11);
				machineText << relativeOffset;
				machineText.push_back(0x58);
				break;
			case 13:
				lineBuilder = "\tpush eax\n\tmov ecx," + name1;
				if (arrayOff1) {
					lineBuilder = lineBuilder + "+" + to_string(4*arrayOff1);
				}
				addHolder = intelSymbols.getAddress(name1) + 4*arrayOff1;
				intelText.push_back(lineBuilder);
				intelText.push_back(string("\tcall EscreverInteiro\n\tpop eax"));
				machineText.push_back(0x50);
				machineText.push_back(0xb9);
				machineText << addHolder;
				machineText.push_back(0xe8);
				relativeOffset = intelSymbols.getAddress(string("EscreverInteiro")) - (iaAdd + 11);
				machineText << relativeOffset;
				machineText.push_back(0x58);
				break;
			case 14:
				if (!debugAtStop) {
					intelText.push_back("\tmov eax,1");
					intelText.push_back(string("\tmov ebx,") + to_string(STD_EXIT_CODE));
					intelText.push_back("\tint 0x80");
					machineText.push_back(0xb8);
					machineText << static_cast<int>(1);
					machineText.push_back(0xbb);
					machineText << static_cast<int>(STD_EXIT_CODE);
					machineText.push_back(0xcd);
					machineText.push_back(0x80);
				}
				else {
					intelText.push_back("\tmov ebx,eax");
					intelText.push_back("\tmov eax,1");
					intelText.push_back("\tint 0x80");
					machineText.push_back(0x89);
					machineText.push_back(0xc3);
					machineText.push_back(0xb8);
					machineText << static_cast<int>(1);
					machineText.push_back(0xcd);
					machineText.push_back(0x80);
					iaAdd -= 3;
				}
				break;
			case 15:
				lineBuilder = "\tpush eax\n\tmov ecx," + name1;
				if (arrayOff1) {
					lineBuilder = lineBuilder + "+" + to_string(4*arrayOff1);
				}
				addHolder = intelSymbols.getAddress(name1) + 4*arrayOff1;
				intelText.push_back(lineBuilder);
				intelText.push_back(string("\tcall LeerChar\n\tpop eax"));
				machineText.push_back(0x50);
				machineText.push_back(0xb9);
				machineText << addHolder;
				machineText.push_back(0xe8);
				relativeOffset = intelSymbols.getAddress(string("LeerChar")) - (iaAdd + 11);
				machineText << relativeOffset;
				machineText.push_back(0x58);
				break;
			case 16:
				lineBuilder = "\tpush eax\n\tmov ecx," + name1;
				if (arrayOff1) {
					lineBuilder = lineBuilder + "+" + to_string(4*arrayOff1);
				}
				addHolder = intelSymbols.getAddress(name1) + 4*arrayOff1;
				intelText.push_back(lineBuilder);
				intelText.push_back(string("\tcall EscreverChar\n\tpop eax"));
				machineText.push_back(0x50);
				machineText.push_back(0xb9);
				machineText << addHolder;
				machineText.push_back(0xe8);
				relativeOffset = intelSymbols.getAddress(string("EscreverChar")) - (iaAdd + 11);
				machineText << relativeOffset;
				machineText.push_back(0x58);
				break;
			case 17:
				lineBuilder = "\tpush eax\n\tmov ecx," + name1;
				if (arrayOff1) {
					lineBuilder = lineBuilder + "+" + to_string(4*arrayOff1);
				}
				addHolder = intelSymbols.getAddress(name1) + 4*arrayOff1;
				intelText.push_back(lineBuilder);
				if (!objSymbols.isConstSymbol(name1) && objSymbols.isDataSymbol(name1)) {
					stringOffset = objSymbols.getSizeSpace(name1) + 1;
				}
				lineBuilder = "\tmov edx," + to_string(stringOffset);
				intelText.push_back(lineBuilder);
				intelText.push_back(string("\tcall LeerString\n\tpop eax"));
				machineText.push_back(0x50);
				machineText.push_back(0xb9);
				machineText << addHolder;
				machineText.push_back(0xba);
				machineText << stringOffset;
				machineText.push_back(0xe8);
				relativeOffset = intelSymbols.getAddress(string("LeerString")) - (iaAdd + 16);
				machineText << relativeOffset;
				machineText.push_back(0x58);
				break;
			case 18:
				lineBuilder = "\tpush eax\n\tmov ecx," + name1;
				if (arrayOff1) {
					lineBuilder = lineBuilder + "+" + to_string(4*arrayOff1);
				}
				addHolder = intelSymbols.getAddress(name1) + 4*arrayOff1;
				intelText.push_back(lineBuilder);
				if (!objSymbols.isConstSymbol(name1) && objSymbols.isDataSymbol(name1)) {
					stringOffset = objSymbols.getSizeSpace(name1) + 1;
				}
				lineBuilder = "\tmov edx," + to_string(stringOffset);
				intelText.push_back(lineBuilder);
				intelText.push_back(string("\tcall EscreverString\n\tpop eax"));
				machineText.push_back(0x50);
				machineText.push_back(0xb9);
				machineText << addHolder;
				machineText.push_back(0xba);
				machineText << stringOffset;
				machineText.push_back(0xe8);
				relativeOffset = intelSymbols.getAddress(string("EscreverString")) - (iaAdd + 16);
				machineText << relativeOffset;
				machineText.push_back(0x58);
				break;
			case 19:
				lineBuilder = "\tmov ebx,[" + name1;
				if (arrayOff1) {
					lineBuilder = lineBuilder + "+" + to_string(4*arrayOff1);
				}
				lineBuilder = lineBuilder + "]";
				addHolder = intelSymbols.getAddress(name1) + 4*arrayOff1;
				intelText.push_back(lineBuilder);
				machineText.push_back(0x8b);
				machineText.push_back(0x1d);
				machineText << addHolder;
				lineBuilder = "\tmov ecx,[" + name2;
				if (arrayOff2) {
					lineBuilder = lineBuilder + "+" + to_string(4*arrayOff2);
				}
				lineBuilder = lineBuilder + "]";
				addHolder = intelSymbols.getAddress(name2) + 4*arrayOff2;
				intelText.push_back(lineBuilder);
				machineText.push_back(0x8b);
				machineText.push_back(0x0d);
				machineText << addHolder;
				lineBuilder = "\tmov dword [" + name2;
				if (arrayOff2) {
					lineBuilder = lineBuilder + "+" + to_string(4*arrayOff2);
				}
				lineBuilder = lineBuilder + "],ebx";
				addHolder = intelSymbols.getAddress(name2) + 4*arrayOff2;
				intelText.push_back(lineBuilder);
				machineText.push_back(0x89);
				machineText.push_back(0x1d);
				machineText << addHolder;
				lineBuilder = "\tmov dword [" + name1;
				if (arrayOff1) {
					lineBuilder = lineBuilder + "+" + to_string(4*arrayOff1);
				}
				lineBuilder = lineBuilder + "],ecx";
				addHolder = intelSymbols.getAddress(name1) + 4*arrayOff1;
				intelText.push_back(lineBuilder);
				machineText.push_back(0x89);
				machineText.push_back(0x0d);
				machineText << addHolder;
				break;
			case 20:
				lineBuilder = "\tmov ecx,[" + name2;
				if (arrayOff2) {
					lineBuilder = lineBuilder + "+" + to_string(4*arrayOff2);
				}
				lineBuilder = lineBuilder + "]";
				addHolder = intelSymbols.getAddress(name2) + 4*arrayOff2;
				intelText.push_back(lineBuilder);
				machineText.push_back(0x8b);
				machineText.push_back(0x0d);
				machineText << addHolder;
				lineBuilder = "\tmov eax,[4*ecx+" + name1;
				if (arrayOff1) {
					lineBuilder = lineBuilder + "+" + to_string(4*arrayOff1);
				}
				lineBuilder = lineBuilder + "]";
				addHolder = intelSymbols.getAddress(name1) + 4*arrayOff1;
				intelText.push_back(lineBuilder);
				machineText.push_back(0x8b);
				machineText.push_back(0x04);
				machineText.push_back(0x8d);
				machineText << addHolder;
				break;
			case 21:
				lineBuilder = "\tmov ecx,[" + name2;
				if (arrayOff2) {
					lineBuilder = lineBuilder + "+" + to_string(4*arrayOff2);
				}
				lineBuilder = lineBuilder + "]";
				addHolder = intelSymbols.getAddress(name2) + 4*arrayOff2;
				intelText.push_back(lineBuilder);
				machineText.push_back(0x8b);
				machineText.push_back(0x0d);
				machineText << addHolder;
				lineBuilder = "\tmov dword [4*ecx+" + name1;
				if (arrayOff1) {
					lineBuilder = lineBuilder + "+" + to_string(4*arrayOff1);
				}
				lineBuilder = lineBuilder + "],eax";
				addHolder = intelSymbols.getAddress(name1) + 4*arrayOff1;
				intelText.push_back(lineBuilder);
				machineText.push_back(0x89);
				machineText.push_back(0x04);
				machineText.push_back(0x8d);
				machineText << addHolder;
				break;
			case 22:
				lineBuilder = "\tand eax,[" + name1;
				if (arrayOff1) {
					lineBuilder = lineBuilder + "+" + to_string(4*arrayOff1);
				}
				lineBuilder = lineBuilder + "]";
				addHolder = intelSymbols.getAddress(name1) + 4*arrayOff1;
				intelText.push_back(lineBuilder);
				machineText.push_back(0x23);
				machineText.push_back(0x05);
				machineText << addHolder;
				break;
			case 23:
				lineBuilder = "\tor eax,[" + name1;
				if (arrayOff1) {
					lineBuilder = lineBuilder + "+" + to_string(4*arrayOff1);
				}
				lineBuilder = lineBuilder + "]";
				addHolder = intelSymbols.getAddress(name1) + 4*arrayOff1;
				intelText.push_back(lineBuilder);
				machineText.push_back(0x0b);
				machineText.push_back(0x05);
				machineText << addHolder;
				break;
			case 24:
				lineBuilder = "\txor eax,[" + name1;
				if (arrayOff1) {
					lineBuilder = lineBuilder + "+" + to_string(4*arrayOff1);
				}
				lineBuilder = lineBuilder + "]";
				addHolder = intelSymbols.getAddress(name1) + 4*arrayOff1;
				intelText.push_back(lineBuilder);
				machineText.push_back(0x33);
				machineText.push_back(0x05);
				machineText << addHolder;
				break;
			case 25:
				lineBuilder = "\tpush dword [" + name1;
				if (arrayOff1) {
					lineBuilder = lineBuilder + "+" + to_string(4*arrayOff1);
				}
				lineBuilder = lineBuilder + "]";
				addHolder = intelSymbols.getAddress(name1) + 4*arrayOff1;
				intelText.push_back(lineBuilder);
				machineText.push_back(0xff);
				machineText.push_back(0x35);
				machineText << addHolder;
				break;
			case 26:
				lineBuilder = "\tpop dword [" + name1;
				if (arrayOff1) {
					lineBuilder = lineBuilder + "+" + to_string(4*arrayOff1);
				}
				lineBuilder = lineBuilder + "]";
				addHolder = intelSymbols.getAddress(name1) + 4*arrayOff1;
				intelText.push_back(lineBuilder);
				machineText.push_back(0x8f);
				machineText.push_back(0x05);
				machineText << addHolder;
				break;
			case 27:
				intelText.push_back("\tpush eax");
				machineText.push_back(0x50);
				break;
			case 28:
				intelText.push_back("\tpop eax");
				machineText.push_back(0x58);
				break;
			case 29:
				lineBuilder = "\tidiv dword [" + name1;
				if (arrayOff1) {
					lineBuilder = lineBuilder + "+" + to_string(4*arrayOff1);
				}
				lineBuilder = lineBuilder + "]";
				addHolder = intelSymbols.getAddress(name1) + 4*arrayOff1;
				intelText.push_back(string("\tcdq"));
				intelText.push_back(lineBuilder);
				intelText.push_back(string("\tmov eax,edx"));
				machineText.push_back(0x99);
				machineText.push_back(0xf7);
				machineText.push_back(0x3d);
				machineText << addHolder;
				machineText.push_back(0x89);
				machineText.push_back(0xd0);
				break;
			case 30:
				lineBuilder = "\tinc dword [" + name1;
				if (arrayOff1) {
					lineBuilder = lineBuilder + "+" + to_string(4*arrayOff1);
				}
				lineBuilder = lineBuilder + "]";
				addHolder = intelSymbols.getAddress(name1) + 4*arrayOff1;
				intelText.push_back(lineBuilder);
				machineText.push_back(0xff);
				machineText.push_back(0x05);
				machineText << addHolder;
				break;
			case 31:
				lineBuilder = "\tdec dword [" + name1;
				if (arrayOff1) {
					lineBuilder = lineBuilder + "+" + to_string(4*arrayOff1);
				}
				lineBuilder = lineBuilder + "]";
				addHolder = intelSymbols.getAddress(name1) + 4*arrayOff1;
				intelText.push_back(lineBuilder);
				machineText.push_back(0xff);
				machineText.push_back(0x0d);
				machineText << addHolder;
				break;
			case 32:
				intelText.push_back("\tinc eax");
				machineText.push_back(0x40);
				break;
			case 33:
				intelText.push_back("\tdec eax");
				machineText.push_back(0x48);
				break;
			case 34:
				lineBuilder = "\tcmp eax,[" + name1;
				if (arrayOff1) {
					lineBuilder = lineBuilder + "+" + to_string(4*arrayOff1);
				}
				lineBuilder = lineBuilder + "]";
				addHolder = intelSymbols.getAddress(name1) + 4*arrayOff1;
				intelText.push_back(lineBuilder);
				lineBuilder = "\tje " + name2;
				intelText.push_back(lineBuilder);
				relativeOffset = intelSymbols.getAddress(name2) - (iaAdd + 12);
				machineText.push_back(0x3b);
				machineText.push_back(0x05);
				machineText << addHolder;
				machineText.push_back(0x0f);
				machineText.push_back(0x84);
				machineText << relativeOffset;
				break;
			case 35:
				lineBuilder = "\tcmp eax,[" + name1;
				if (arrayOff1) {
					lineBuilder = lineBuilder + "+" + to_string(4*arrayOff1);
				}
				lineBuilder = lineBuilder + "]";
				addHolder = intelSymbols.getAddress(name1) + 4*arrayOff1;
				intelText.push_back(lineBuilder);
				lineBuilder = "\tjne " + name2;
				intelText.push_back(lineBuilder);
				relativeOffset = intelSymbols.getAddress(name2) - (iaAdd + 12);
				machineText.push_back(0x3b);
				machineText.push_back(0x05);
				machineText << addHolder;
				machineText.push_back(0x0f);
				machineText.push_back(0x85);
				machineText << relativeOffset;
				break;
			case 36:
				lineBuilder = "\tcmp eax,[" + name1;
				if (arrayOff1) {
					lineBuilder = lineBuilder + "+" + to_string(4*arrayOff1);
				}
				lineBuilder = lineBuilder + "]";
				addHolder = intelSymbols.getAddress(name1) + 4*arrayOff1;
				intelText.push_back(lineBuilder);
				lineBuilder = "\tjg " + name2;
				intelText.push_back(lineBuilder);
				relativeOffset = intelSymbols.getAddress(name2) - (iaAdd + 12);
				machineText.push_back(0x3b);
				machineText.push_back(0x05);
				machineText << addHolder;
				machineText.push_back(0x0f);
				machineText.push_back(0x8f);
				machineText << relativeOffset;
				break;
			case 37:
				lineBuilder = "\tcmp eax,[" + name1;
				if (arrayOff1) {
					lineBuilder = lineBuilder + "+" + to_string(4*arrayOff1);
				}
				lineBuilder = lineBuilder + "]";
				addHolder = intelSymbols.getAddress(name1) + 4*arrayOff1;
				intelText.push_back(lineBuilder);
				lineBuilder = "\tjge " + name2;
				intelText.push_back(lineBuilder);
				relativeOffset = intelSymbols.getAddress(name2) - (iaAdd + 12);
				machineText.push_back(0x3b);
				machineText.push_back(0x05);
				machineText << addHolder;
				machineText.push_back(0x0f);
				machineText.push_back(0x8d);
				machineText << relativeOffset;
				break;
			case 38:
				lineBuilder = "\tcmp eax,[" + name1;
				if (arrayOff1) {
					lineBuilder = lineBuilder + "+" + to_string(4*arrayOff1);
				}
				lineBuilder = lineBuilder + "]";
				addHolder = intelSymbols.getAddress(name1) + 4*arrayOff1;
				intelText.push_back(lineBuilder);
				lineBuilder = "\tjl " + name2;
				intelText.push_back(lineBuilder);
				relativeOffset = intelSymbols.getAddress(name2) - (iaAdd + 12);
				machineText.push_back(0x3b);
				machineText.push_back(0x05);
				machineText << addHolder;
				machineText.push_back(0x0f);
				machineText.push_back(0x8c);
				machineText << relativeOffset;
				break;
			case 39:
				lineBuilder = "\tcmp eax,[" + name1;
				if (arrayOff1) {
					lineBuilder = lineBuilder + "+" + to_string(4*arrayOff1);
				}
				lineBuilder = lineBuilder + "]";
				addHolder = intelSymbols.getAddress(name1) + 4*arrayOff1;
				intelText.push_back(lineBuilder);
				lineBuilder = "\tjle " + name2;
				intelText.push_back(lineBuilder);
				relativeOffset = intelSymbols.getAddress(name2) - (iaAdd + 12);
				machineText.push_back(0x3b);
				machineText.push_back(0x05);
				machineText << addHolder;
				machineText.push_back(0x0f);
				machineText.push_back(0x8e);
				machineText << relativeOffset;
				break;
			case 40:
				lineBuilder = "\tmov dword [" + name1;
				if (arrayOff1) {
					lineBuilder = lineBuilder + "+" + to_string(4*arrayOff1);
				}
				lineBuilder = lineBuilder + "],0";
				addHolder = intelSymbols.getAddress(name1) + 4*arrayOff1;
				intelText.push_back(lineBuilder);
				machineText.push_back(0xc7);
				machineText.push_back(0x05);
				machineText << addHolder;
				machineText << static_cast<int>(0);
				break;
			case 41:
				intelText.push_back("\txor eax,eax");
				machineText.push_back(0x31);
				machineText.push_back(0xc0);
				break;
			case 42:
				lineBuilder = "\tpush eax\n\tmov ecx," + name1;
				if (arrayOff1) {
					lineBuilder = lineBuilder + "+" + to_string(4*arrayOff1);
				}
				addHolder = intelSymbols.getAddress(name1) + 4*arrayOff1;
				intelText.push_back(lineBuilder);
				intelText.push_back(string("\tcall EscreverBin\n\tpop eax"));
				machineText.push_back(0x50);
				machineText.push_back(0xb9);
				machineText << addHolder;
				machineText.push_back(0xe8);
				relativeOffset = intelSymbols.getAddress(string("EscreverBin")) - (iaAdd + 11);
				machineText << relativeOffset;
				machineText.push_back(0x58);
				break;
			case 43:
				lineBuilder = "\tpush eax\n\tmov ecx," + name1;
				if (arrayOff1) {
					lineBuilder = lineBuilder + "+" + to_string(4*arrayOff1);
				}
				addHolder = intelSymbols.getAddress(name1) + 4*arrayOff1;
				intelText.push_back(lineBuilder);
				intelText.push_back(string("\tcall EscreverOct\n\tpop eax"));
				machineText.push_back(0x50);
				machineText.push_back(0xb9);
				machineText << addHolder;
				machineText.push_back(0xe8);
				relativeOffset = intelSymbols.getAddress(string("EscreverOct")) - (iaAdd + 11);
				machineText << relativeOffset;
				machineText.push_back(0x58);
				break;
			case 44:
				lineBuilder = "\tpush eax\n\tmov ecx," + name1;
				if (arrayOff1) {
					lineBuilder = lineBuilder + "+" + to_string(4*arrayOff1);
				}
				addHolder = intelSymbols.getAddress(name1) + 4*arrayOff1;
				intelText.push_back(lineBuilder);
				intelText.push_back(string("\tcall EscreverHex\n\tpop eax"));
				machineText.push_back(0x50);
				machineText.push_back(0xb9);
				machineText << addHolder;
				machineText.push_back(0xe8);
				relativeOffset = intelSymbols.getAddress(string("EscreverHex")) - (iaAdd + 11);
				machineText << relativeOffset;
				machineText.push_back(0x58);
				break;
			case 45:
				intelText.push_back("\tnot eax");
				machineText.push_back(0xf7);
				machineText.push_back(0xd0);
				break;
			case 46:
				intelText.push_back("\tpush eax");
				machineText.push_back(0x50);
				lineBuilder = "\tmov eax,[" + name1;
				if (arrayOff1) {
					lineBuilder = lineBuilder + "+" + to_string(4*arrayOff1);
				}
				lineBuilder = lineBuilder + "]";
				addHolder = intelSymbols.getAddress(name1) + 4*arrayOff1;
				intelText.push_back(lineBuilder);
				machineText.push_back(0xa1);
				machineText << addHolder;
				lineBuilder = "\tpop dword [" + name1;
				if (arrayOff1) {
					lineBuilder = lineBuilder + "+" + to_string(4*arrayOff1);
				}
				lineBuilder = lineBuilder + "]";
				addHolder = intelSymbols.getAddress(name1) + 4*arrayOff1;
				intelText.push_back(lineBuilder);
				machineText.push_back(0x8f);
				machineText.push_back(0x05);
				machineText << addHolder;
				break;
			case 47:
				lineBuilder = "\tmov ecx,[" + name1;
				if (arrayOff1) {
					lineBuilder = lineBuilder + "+" + to_string(4*arrayOff1);
				}
				lineBuilder = lineBuilder + "]";
				addHolder = intelSymbols.getAddress(name1) + 4*arrayOff1;
				intelText.push_back(lineBuilder);
				machineText.push_back(0x8b);
				machineText.push_back(0x0d);
				machineText << addHolder;
				lineBuilder = "\tmov edx,[" + name2;
				if (arrayOff2) {
					lineBuilder = lineBuilder + "+" + to_string(4*arrayOff2);
				}
				lineBuilder = lineBuilder + "]";
				addHolder = intelSymbols.getAddress(name2) + 4*arrayOff2;
				intelText.push_back(lineBuilder);
				machineText.push_back(0x8b);
				machineText.push_back(0x15);
				machineText << addHolder;
				intelText.push_back("\tadd ecx,edx\n\tsar ecx,1");
				machineText << static_cast<int>(0xf9d1d101);
				lineBuilder = "\tmov dword [" + name1;
				if (arrayOff1) {
					lineBuilder = lineBuilder + "+" + to_string(4*arrayOff1);
				}
				lineBuilder = lineBuilder + "],ecx";
				addHolder = intelSymbols.getAddress(name1) + 4*arrayOff1;
				intelText.push_back(lineBuilder);
				machineText.push_back(0x89);
				machineText.push_back(0x0d);
				machineText << addHolder;
				lineBuilder = "\tmov dword [" + name2;
				if (arrayOff2) {
					lineBuilder = lineBuilder + "+" + to_string(4*arrayOff2);
				}
				lineBuilder = lineBuilder + "],ecx";
				addHolder = intelSymbols.getAddress(name2) + 4*arrayOff2;
				intelText.push_back(lineBuilder);
				machineText.push_back(0x89);
				machineText.push_back(0x0d);
				machineText << addHolder;
				break;
			case 48:
				lineBuilder = "\tmov cl,[" + name1;
				if (arrayOff1) {
					lineBuilder = lineBuilder + "+" + to_string(4*arrayOff1);
				}
				lineBuilder = lineBuilder + "]";
				addHolder = intelSymbols.getAddress(name1) + 4*arrayOff1;
				intelText.push_back(lineBuilder);
				machineText.push_back(0x8a);
				machineText.push_back(0x0d);
				machineText << addHolder;
				intelText.push_back("\tshl eax,cl");
				machineText.push_back(0xd3);
				machineText.push_back(0xe0);
				break;
			case 49:
				lineBuilder = "\tmov cl,[" + name1;
				if (arrayOff1) {
					lineBuilder = lineBuilder + "+" + to_string(4*arrayOff1);
				}
				lineBuilder = lineBuilder + "]";
				addHolder = intelSymbols.getAddress(name1) + 4*arrayOff1;
				intelText.push_back(lineBuilder);
				machineText.push_back(0x8a);
				machineText.push_back(0x0d);
				machineText << addHolder;
				intelText.push_back("\tshr eax,cl");
				machineText.push_back(0xd3);
				machineText.push_back(0xe8);
				break;
			case 50:
				lineBuilder = "\tmov cl,[" + name1;
				if (arrayOff1) {
					lineBuilder = lineBuilder + "+" + to_string(4*arrayOff1);
				}
				lineBuilder = lineBuilder + "]";
				addHolder = intelSymbols.getAddress(name1) + 4*arrayOff1;
				intelText.push_back(lineBuilder);
				machineText.push_back(0x8a);
				machineText.push_back(0x0d);
				machineText << addHolder;
				intelText.push_back("\tsar eax,cl");
				machineText.push_back(0xd3);
				machineText.push_back(0xf8);
				break;
			case 51:
				intelText.push_back("\tpush eax\n\tmov ecx,esp\n\tmov ebx,0xffff8000\n\tmov edx,0x7fff\n\tcall Randomizer\n\tpop eax");
				machineText << static_cast<int>(0xbbe18950);
				machineText << static_cast<int>(0x0080ffff);
				machineText.push_back(0xba);
				machineText << static_cast<int>(0xff7f0000);
				machineText.push_back(0xe8);
				relativeOffset = intelSymbols.getAddress(string("Randomizer")) - (iaAdd + 18);
				machineText << relativeOffset;
				machineText.push_back(0x58);
				break;
			case 52:
				intelText.push_back("\tpush eax\n\tmov ecx,esp");
				lineBuilder = "\tmov ebx,[" + name1;
				if (arrayOff1) {
					lineBuilder = lineBuilder + "+" + to_string(4*arrayOff1);
				}
				lineBuilder = lineBuilder + "]";
				addHolder = intelSymbols.getAddress(name1) + 4*arrayOff1;
				intelText.push_back(lineBuilder);
				machineText << static_cast<int>(0x8be18950);
				machineText.push_back(0x1d);
				machineText << addHolder;
				lineBuilder = "\tmov edx,[" + name2;
				if (arrayOff2) {
					lineBuilder = lineBuilder + "+" + to_string(4*arrayOff2);
				}
				lineBuilder = lineBuilder + "]";
				addHolder = intelSymbols.getAddress(name2) + 4*arrayOff2;
				intelText.push_back(lineBuilder);
				machineText.push_back(0x8b);
				machineText.push_back(0x15);
				machineText << addHolder;
				machineText.push_back(0xe8);
				relativeOffset = intelSymbols.getAddress(string("Randomizer")) - (iaAdd + 20);
				machineText << relativeOffset;
				machineText.push_back(0x58);
				intelText.push_back("\tcall Randomizer\n\tpop eax");
				break;
			case 53:
				intelText.push_back("\tpush eax\n\tmov ecx,esp\n\tcall LeerInteiro\n\tpop eax");
				machineText << static_cast<int>(0xe8e18950);
				relativeOffset = intelSymbols.getAddress(string("LeerInteiro")) - (iaAdd + 8);
				machineText << relativeOffset;
				machineText.push_back(0x58);
				break;
			case 54:
				intelText.push_back("\tpush eax\n\tmov ecx,esp\n\tcall EscreverInteiro\n\tpop eax");
				machineText << static_cast<int>(0xe8e18950);
				relativeOffset = intelSymbols.getAddress(string("EscreverInteiro")) - (iaAdd + 8);
				machineText << relativeOffset;
				machineText.push_back(0x58);
				break;
			case 55:
				intelText.push_back("\tpush eax\n\tmov ecx,esp\n\tcall EscreverBin\n\tpop eax");
				machineText << static_cast<int>(0xe8e18950);
				relativeOffset = intelSymbols.getAddress(string("EscreverBin")) - (iaAdd + 8);
				machineText << relativeOffset;
				machineText.push_back(0x58);
				break;
			case 56:
				intelText.push_back("\tpush eax\n\tmov ecx,esp\n\tcall EscreverOct\n\tpop eax");
				machineText << static_cast<int>(0xe8e18950);
				relativeOffset = intelSymbols.getAddress(string("EscreverOct")) - (iaAdd + 8);
				machineText << relativeOffset;
				machineText.push_back(0x58);
				break;
			case 57:
				intelText.push_back("\tpush eax\n\tmov ecx,esp\n\tcall EscreverHex\n\tpop eax");
				machineText << static_cast<int>(0xe8e18950);
				relativeOffset = intelSymbols.getAddress(string("EscreverHex")) - (iaAdd + 8);
				machineText << relativeOffset;
				machineText.push_back(0x58);
				break;
			case 58:
				intelText.push_back("\tpush eax\n\tmov ecx,esp\n\tcall LeerChar\n\tpop eax");
				machineText << static_cast<int>(0xe8e18950);
				relativeOffset = intelSymbols.getAddress(string("LeerChar")) - (iaAdd + 8);
				machineText << relativeOffset;
				machineText.push_back(0x58);
				break;
			case 59:
				intelText.push_back("\tpush eax\n\tmov ecx,esp\n\tcall EscreverChar\n\tpop eax");
				machineText << static_cast<int>(0xe8e18950);
				relativeOffset = intelSymbols.getAddress(string("EscreverChar")) - (iaAdd + 8);
				machineText << relativeOffset;
				machineText.push_back(0x58);
				break;
			case 60:
				lineBuilder = "\tcmp eax,0\n\tjne " + name1;
				relativeOffset = intelSymbols.getAddress(name1) - (iaAdd + 9);
				intelText.push_back(lineBuilder);
				machineText.push_back(0x83);
				machineText.push_back(0xf8);
				machineText.push_back(0x00);
				machineText.push_back(0x0f);
				machineText.push_back(0x85);
				machineText << relativeOffset;
				break;
			case 61:
				lineBuilder = "\tmov ebx,[" + name1;
				if (arrayOff1) {
					lineBuilder = lineBuilder + "+" + to_string(4*arrayOff1);
				}
				lineBuilder = lineBuilder + "]\n\tmov ecx,[" + name2;
				if (arrayOff2) {
					lineBuilder = lineBuilder + "+" + to_string(4*arrayOff2);
				}
				lineBuilder = lineBuilder + "]\n\tcall Least";
				intelText.push_back(lineBuilder);
				relativeOffset = intelSymbols.getAddress(string("Least")) - (iaAdd + 17);
				machineText.push_back(0x8b);
				machineText.push_back(0x1d);
				addHolder = intelSymbols.getAddress(name1) + 4*arrayOff1;
				machineText << addHolder;
				machineText.push_back(0x8b);
				machineText.push_back(0x0d);
				addHolder = intelSymbols.getAddress(name2) + 4*arrayOff2;
				machineText << addHolder;
				machineText.push_back(0xe8);
				machineText << relativeOffset;
				break;
			case 62:
				lineBuilder = "\tmov ebx,[" + name1;
				if (arrayOff1) {
					lineBuilder = lineBuilder + "+" + to_string(4*arrayOff1);
				}
				lineBuilder = lineBuilder + "]\n\tmov ecx,[" + name2;
				if (arrayOff2) {
					lineBuilder = lineBuilder + "+" + to_string(4*arrayOff2);
				}
				lineBuilder = lineBuilder + "]\n\tcall Greatest";
				intelText.push_back(lineBuilder);
				relativeOffset = intelSymbols.getAddress(string("Greatest")) - (iaAdd + 17);
				machineText.push_back(0x8b);
				machineText.push_back(0x1d);
				addHolder = intelSymbols.getAddress(name1) + 4*arrayOff1;
				machineText << addHolder;
				machineText.push_back(0x8b);
				machineText.push_back(0x0d);
				addHolder = intelSymbols.getAddress(name2) + 4*arrayOff2;
				machineText << addHolder;
				machineText.push_back(0xe8);
				machineText << relativeOffset;
				break;
			case 63:
				lineBuilder = "\tmov ebx,[" + name1;
				if (arrayOff1) {
					lineBuilder = lineBuilder + "+" + to_string(4*arrayOff1);
				}
				lineBuilder = lineBuilder + "]\n\tmov ecx,[" + name2;
				if (arrayOff2) {
					lineBuilder = lineBuilder + "+" + to_string(4*arrayOff2);
				}
				lineBuilder = lineBuilder + "]\n\tcall ComputeGCD";
				intelText.push_back(lineBuilder);
				relativeOffset = intelSymbols.getAddress(string("ComputeGCD")) - (iaAdd + 17);
				machineText.push_back(0x8b);
				machineText.push_back(0x1d);
				addHolder = intelSymbols.getAddress(name1) + 4*arrayOff1;
				machineText << addHolder;
				machineText.push_back(0x8b);
				machineText.push_back(0x0d);
				addHolder = intelSymbols.getAddress(name2) + 4*arrayOff2;
				machineText << addHolder;
				machineText.push_back(0xe8);
				machineText << relativeOffset;
				break;
			case 64:
				lineBuilder = "\tmov ebx,[" + name1;
				if (arrayOff1) {
					lineBuilder = lineBuilder + "+" + to_string(4*arrayOff1);
				}
				lineBuilder = lineBuilder + "]\n\tmov ecx,[" + name2;
				if (arrayOff2) {
					lineBuilder = lineBuilder + "+" + to_string(4*arrayOff2);
				}
				lineBuilder = lineBuilder + "]\n\tcall ComputeLCM";
				intelText.push_back(lineBuilder);
				relativeOffset = intelSymbols.getAddress(string("ComputeLCM")) - (iaAdd + 17);
				machineText.push_back(0x8b);
				machineText.push_back(0x1d);
				addHolder = intelSymbols.getAddress(name1) + 4*arrayOff1;
				machineText << addHolder;
				machineText.push_back(0x8b);
				machineText.push_back(0x0d);
				addHolder = intelSymbols.getAddress(name2) + 4*arrayOff2;
				machineText << addHolder;
				machineText.push_back(0xe8);
				machineText << relativeOffset;
				break;
			case 65:
				intelText.push_back("\tmov ebx,eax");
				intelText.push_back("\tmov eax,1");
				intelText.push_back("\tint 0x80");
				machineText.push_back(0x89);
				machineText.push_back(0xc3);
				machineText.push_back(0xb8);
				machineText << static_cast<int>(1);
				machineText.push_back(0xcd);
				machineText.push_back(0x80);
				break;
			default:
				break;
		}
		machineText.shrink_to_fit();
		curAdd += objSizes.at(curOpcode-1);
		iaAdd += arraySizes.at(curOpcode-1);
	}
	if (has12 || has13 || has15 || has16 || has17 || has18 || has42 || has43 || has44 || has5r) {
		intelText.push_back(string(""));
	}
	if (has12) {
		intelText.push_back(string("LeerInteiro:\n\tenter 24,0\n\tmov dword [ebp-4],ecx\n\tsub edi,edi\n\tmov eax,3"));
		intelText.push_back(string("\tmov ebx,0\n\tmov ecx,ebp\n\tsub ecx,20\n\tmov edx,12\n\tint 80h\n\tmov esi,ebp"));
		intelText.push_back(string("\tsub esi,20\n\tcmp byte [esi],45\n\tjne r_no_sgn\n\tmov dword [ebp-24],1"));
		intelText.push_back(string("\tinc esi\n\tmov ebx,0\nr_no_sgn:\n\tcmp byte [esi],48\n\tjl read_phase2"));
		intelText.push_back(string("\tcmp byte [esi],57\n\tjg read_phase2\n\tmov dl,[esi]\n\tand edx,0xFF\n\tsub dx,0x30"));
		intelText.push_back(string("\tpush dx\n\tinc esi\n\tinc ebx\n\tcmp ebx,12\n\tjne r_no_sgn\nread_phase2:\n\tsub eax,eax"));
		intelText.push_back(string("\tsub esi,esi\n\tmov edi,1\nread_loop:\n\tcmp ebx,0\n\tje end_read\n\tpop si\n\tpush eax"));
		intelText.push_back(string("\tmov eax,esi\n\tmul edi\n\tmov ecx,eax\n\tmov eax,edi\n\tmov esi,10\n\tmul esi"));
		intelText.push_back(string("\tmov edi,eax\n\tpop eax\n\tadd eax,ecx\n\tdec ebx\n\tjmp read_loop\nend_read:"));
		intelText.push_back(string("\tcmp dword [ebp-24],1\n\tjne no_negate\n\tneg eax\nno_negate:\n\tmov ecx,[ebp-4]"));
		intelText.push_back(string("\tmov dword [ecx],eax\n\tleave\n\tret"));
		intelText.push_back(string(""));
		machineText << inputCode;
	}
	if (has13) {
		intelText.push_back(string("EscreverInteiro:\n\tenter 8,0\n\tmov eax,[ecx]\n\tmov dword [ebp-8],eax"));
		intelText.push_back(string("\tmov dword [ebp-4],45\n\tcmp eax,0\n\tjg w_no_sgn"));
		intelText.push_back(string("\tcmp eax,0\n\tje w_no_sgn"));
		intelText.push_back(string("\tneg dword [ebp-8]\n\tmov eax,4\n\tmov ebx,1\n\tmov ecx,ebp"));
		intelText.push_back(string("\tsub ecx,4\n\tmov edx,1\n\tint 80h\nw_no_sgn:"));
		intelText.push_back(string("\tmov eax,[ebp-8]\n\tsub edx,edx\n\tsub esi,esi\n\tmov ebx,10\nwrite_loop1:"));
		intelText.push_back(string("\tcdq\n\tdiv ebx\n\tadd edx,48\n\tpush edx\n\tinc esi\n\tcmp eax,0"));
		intelText.push_back(string("\tjne write_loop1\nwrite_loop2:\n\tpop dword [ebp-8]\n\tmov eax,4"));
		intelText.push_back(string("\tmov ebx,1\n\tmov ecx,ebp\n\tsub ecx,8\n\tmov edx,1\n\tint	80h\n\tdec esi\n\tcmp esi,0"));
		intelText.push_back(string("\tjne write_loop2\n\tmov dword [ebp-8],0ah\n\tmov eax,4\n\tmov ebx,1\n\tmov	ecx,ebp"));
		intelText.push_back(string("\tsub ecx,8\n\tmov edx,1\n\tint	80h\n\tleave\n\tret"));
		intelText.push_back(string(""));
		machineText << outputCode;
	}
	if (has15) {
		intelText.push_back(string("LeerChar:\n\tenter 0,0\n\tmov eax,3\n\tmov ebx,0"));
		intelText.push_back(string("\tmov edx,2\n\tint 80h\n\tand dword [ecx],0xff\n\tleave\n\tret"));
		intelText.push_back(string(""));
		machineText << cinputCode;
	}
	if (has16) {
		intelText.push_back(string("EscreverChar:\n\tenter 0,0\n\tmov edx,0x0a00\n\tor dword [ecx],edx"));
		intelText.push_back(string("\tmov eax,4\n\tmov ebx,1\n\tmov edx,2\n\tint 80h\n\tand dword [ecx],0xff\n\tleave\n\tret"));
		intelText.push_back(string(""));
		machineText << coutputCode;
	}
	if (has17) {
		intelText.push_back(string("LeerString:\n\tenter 24,0\n\tmov dword [ebp-4],0x0a0d2174\n\tmov dword [ebp-8],0x75706e69"));
		intelText.push_back(string("\tmov dword [ebp-12],0x2064696c\n\tmov dword [ebp-16],0x61766e49\n\tmov esi,ecx"));
		intelText.push_back(string("\tmov edi,edx\nrs_loop:\n\tcmp edi,0\n\tjl rs_error\n\tmov eax,3\n\tmov ebx,0"));
		intelText.push_back(string("\tmov ecx,ebp\n\tsub ecx,20\n\tmov edx,1\n\tint 0x80\n\tcmp dword [ebp-20],0x0a"));
		intelText.push_back(string("\tje rs_advance\n\tmov edx,[ebp-20]\n\tmov dword [esi],edx\n\tadd esi,4"));
		intelText.push_back(string("\tdec edi\n\tjmp rs_loop\nrs_error:\n\tmov eax,4\n\tmov ebx,1\n\tmov ecx,ebp"));
		intelText.push_back(string("\tsub ecx,16\n\tmov edx,16\n\tint 0x80\nrs_flush:\n\tmov eax,3\n\tmov ebx,0"));
		intelText.push_back(string("\tmov ecx,ebp\n\tsub ecx,20\n\tmov edx,1\n\tint 0x80\n\tcmp dword [ebp-20],0x20"));
		intelText.push_back(string("\tjg rs_flush\nrs_advance:\n\tleave\n\tret"));
		intelText.push_back(string(""));
		machineText << sinputCode;
	}
	if (has18) {
		intelText.push_back(string("EscreverString:\n\tenter 4,0\n\tmov dword [ebp-4],0x0d0a\n\tmov esi,ecx"));
		intelText.push_back(string("\tmov edi,edx\n\tmov eax,4\n\tcdq\n\tmul edi\n\tmov edi,eax"));
		intelText.push_back(string("ws_loop:\n\tcmp edi,0\n\tje ws_loop_end\n\tmov eax,4\n\tmov ebx,1\n\tmov ecx,esi"));
		intelText.push_back(string("\tmov edx,1\n\tint 0x80\n\tadd esi,4\n\tsub edi,4\n\tjmp ws_loop"));
		intelText.push_back(string("ws_loop_end:\n\tmov eax,4\n\tmov ebx,1\n\tmov ecx,ebp\n\tsub ecx,4"));
		intelText.push_back(string("\tmov edx,2\n\tint 0x80\n\tleave\n\tret"));
		intelText.push_back(string(""));
		machineText << soutputCode;
	}
	if (has42) {
		intelText.push_back(string("EscreverBin:\n\tenter 10,0\n\tmov dword [ebp-4],0x20\n\tmov dword [ebp-8],0x0a"));
		intelText.push_back(string("\tpush edi\n\txor edx,edx\n\tmov edi,[ecx]\n\tmov ecx,0x20\nwb_loop1:\n\tmov dx,0x30"));
		intelText.push_back(string("\ttest edi,1\n\tje wb_loop1_adv\n\tinc edx\nwb_loop1_adv:\n\tpush dx\n\tsar edi,1"));
		intelText.push_back(string("\tloop wb_loop1\n\tmov ecx,0x20\nwb_loop2:\n\txor edx,edx\n\tpop dx\n\tmov word [ebp-10],dx"));
		intelText.push_back(string("\tpush ecx\n\tcmp ecx,0x20\n\tje wb_no_spc\n\ttest ecx,0x3\n\tjne wb_no_spc\n\tmov eax,4"));
		intelText.push_back(string("\tmov ebx,1\n\tmov ecx,ebp\n\tsub ecx,4\n\tmov edx,1\n\tint 0x80\nwb_no_spc:\n\tmov eax,4"));
		intelText.push_back(string("\tmov ebx,1\n\tmov ecx,ebp\n\tsub ecx,10\n\tmov edx,1\n\tint 0x80\n\tpop ecx\n\tloop wb_loop2"));
		intelText.push_back(string("\tmov eax,4\n\tmov ebx,1\n\tmov ecx,ebp\n\tsub ecx,8\n\tmov edx,1\n\tint 0x80"));
		intelText.push_back(string("\tpop edi\n\tleave\n\tret"));
		intelText.push_back(string(""));
		machineText << boutputCode;
	}
	if (has43) {
		intelText.push_back(string("EscreverOct:\n\tenter 8,0\n\tpush edi\n\tmov edi,[ecx]\n\tmov dword [ebp-4],0x000a0020"));
		intelText.push_back(string("\tmov ecx,0xc\nwo_loop1:\n\tmov dword [ebp-8],edi\n\tand dword [ebp-8],0x7"));
		intelText.push_back(string("\tadd dword [ebp-8],0x30\n\tpush dword [ebp-8]\n\tshr edi,3\n\tloop wo_loop1"));
		intelText.push_back(string("\tmov ecx,0xC\nwo_loop2:\n\tpop dword [ebp-8]\n\tpush ecx\n\tcmp ecx,0xc"));
		intelText.push_back(string("\tje wo_no_spc\n\tmov eax,ecx\n\tmov ebx,3\n\tdiv ebx\n\tcmp edx,1\n\tjne wo_no_spc"));
		intelText.push_back(string("\tmov eax,4\n\tmov ebx,1\n\tmov ecx,ebp\n\tsub ecx,4\n\tmov edx,1\n\tint 0x80\nwo_no_spc:"));
		intelText.push_back(string("\tmov eax,4\n\tmov ebx,1\n\tmov ecx,ebp\n\tsub ecx,8\n\tmov edx,1\n\tint 0x80\n\tpop ecx"));
		intelText.push_back(string("\tloop wo_loop2\n\tmov eax,4\n\tmov ebx,1\n\tmov ecx,ebp\n\tsub ecx,2\n\tmov edx,1"));
		intelText.push_back(string("\tint 0x80\n\tpop edi\n\tleave\n\tret"));
		intelText.push_back(string(""));
		machineText << ooutputCode;
	}
	if (has44) {
		intelText.push_back(string("EscreverHex:\n\tenter 8,0\n\tpush edi\n\tmov edi,[ecx]\n\tmov dword [ebp-4],0x000a7830"));
		intelText.push_back(string("\tmov ecx,8\nwh_loop1:\n\tmov dword [ebp-8],edi\n\tand dword [ebp-8],0xf"));
		intelText.push_back(string("\tcmp dword [ebp-8],0xa\n\tjl wh_no_sum\n\tadd dword [ebp-8],0x7\nwh_no_sum:"));
		intelText.push_back(string("\tadd dword [ebp-8],0x30\n\tpush dword [ebp-8]\n\tsar edi,4\n\tloop wh_loop1\n\tmov eax,4"));
		intelText.push_back(string("\tmov ebx,1\n\tmov ecx,ebp\n\tsub ecx,4\n\tmov edx,2\n\tint 0x80\n\tmov ecx,8\nwh_loop2:"));
		intelText.push_back(string("\tpop dword [ebp-8]\n\tpush ecx\n\tmov eax,4\n\tmov ebx,1\n\tmov ecx,ebp\n\tsub ecx,8"));
		intelText.push_back(string("\tmov edx,1\n\tint 0x80\n\tpop ecx\n\tloop wh_loop2\n\tmov eax,4\n\tmov ebx,1\n\tmov ecx,ebp"));
		intelText.push_back(string("\tsub ecx,2\n\tmov edx,1\n\tint 0x80\n\tpop edi\n\tleave\n\tret"));
		intelText.push_back(string(""));
		machineText << houtputCode;
	}
	if (has5r) {
		intelText.push_back(string("Randomizer:\n\tenter 20,0\n\tpush edi\n\tmov edi,ecx\n\tmov dword [ebp-4],ebx"));
		intelText.push_back(string("\tmov dword [ebp-8],edx\n\tmov dword [ebp-12],0xbc8f\n\tmov dword [ebp-16],0x7fffffff"));
		intelText.push_back(string("\tmov dword [ebp-20],0x00\n\tmov eax,[ebp-4]"));
		intelText.push_back(string("\tneg eax\n\tadd dword [ebp-8],eax\n\tinc dword [ebp-8]\nrandom_adv:\n\trdtsc"));
		intelText.push_back(string("\tmov ecx,edx\n\tand ecx,0xffff\n\tshr ecx,2\nrandom_loop:\n\tmul dword [ebp-12]"));
		intelText.push_back(string("\tadd eax,[ebp-20]\n\tand eax,[ebp-16]\n\tloop random_loop\n\tcdq\n\tdiv dword [ebp-8]"));
		intelText.push_back(string("\tmov eax,edx\n\tadd eax,[ebp-4]\n\tmov dword [edi],eax\n\tpop edi\n\tleave\n\tret"));
		intelText.push_back(string(""));
		machineText << randomCode;
	}
	if (has61) {
		intelText.push_back(string("Least:\n\tenter 0,0\n\tcmp ebx,ecx\n\tjl d_min\n\tmov eax,ecx"));
		intelText.push_back(string("\tjmp min_end\nd_min:\n\tmov eax,ebx\nmin_end:\n\tleave\n\tret"));
		machineText << minmCode;
	}
	if (has62) {
		intelText.push_back(string("Greatest:\n\tenter 0,0\n\tcmp ebx,ecx\n\tjg d_max\n\tmov eax,ecx"));
		intelText.push_back(string("\tjmp max_end\nd_max:\n\tmov eax,ebx\nmax_end:\n\tleave\n\tret"));
		machineText << maxmCode;
	}
	if (has63 || has64) {
		intelText.push_back(string("ComputeGCD:\n\tenter 4,0\n\tcmp ebx,0\n\tje end_gcd\n\tcmp ecx,0\n\tjne gcd_adv1"));
		intelText.push_back(string("\tmov ecx,ebx\n\tjmp end_gcd\ngcd_adv1:\n\tcmp ebx,0\n\tjg gcd_adv2\n\tneg ebx"));
		intelText.push_back(string("gcd_adv2:\n\tcmp ecx,0\n\tjg gcd_loop\n\tneg ecx\ngcd_loop:\n\tcmp ebx,ecx\n\tje end_gcd"));
		intelText.push_back(string("\tcmp ebx,ecx\n\tjg gcd_bc\n\tsub ecx,ebx\n\tjmp gcd_loop\ngcd_bc:\n\tsub ebx,ecx"));
		intelText.push_back(string("\tjmp gcd_loop\nend_gcd:\n\tmov eax,ecx\n\tleave\n\tret"));
		machineText << gcdCode;
	}
	if (has64) {
		intelText.push_back(string("ComputeLCM:\n\tenter 0,0\n\tpush esi\n\tpush ebx\n\tpush ecx\n\tcall ComputeGCD"));
		intelText.push_back(string("\tmov esi,eax\n\tpop ecx\n\tpop ebx\n\tmov eax,ebx\n\timul ecx\n\tidiv esi"));
		intelText.push_back(string("\tcmp eax,0\n\tjg lcm_end\n\tneg eax\nlcm_end:\n\tpop esi\n\tleave\n\tret"));
		machineText << lcmCode;
	} 
	//cout << "Intel Symbol Table:\n\n" << hex << setfill('0') << uppercase << setw(2) << intelSymbols << endl;
	//cout << "Text Machine: [" << machineText << "]" << endl;
	//cout << "Data Machine: [" << machineData << "]" << endl;
	//cout << "Code:" << endl;
	for (string &k : intelText) {
		intelCode.push_back(k);
		//cout << k << endl;
	}
	string lastText = intelText.back();
	if (lastText.length()) {
		intelCode.push_back(string(""));
	}
	//cout << endl;
	for (string &k : intelData) {
		intelCode.push_back(k);
		//cout << k << endl;
	}
	machineCode << machineText;
	machineCode << machineData;
	if (!elfMode) {
		ofstream assemblyOutput(objectName+string(".s"),ios::out);
		if (!assemblyOutput) {
			cerr	<< "Fatal Error: Could not create file " << objectName+string(".s") << endl; 
			return -1;
		}
		for (string &k : intelCode) {
			assemblyOutput << k << endl;
		}
		assemblyOutput.close();

		ofstream codOutput(objectName+string(".cod"),ios::out);
		if (!codOutput) {
			cerr	<< "Fatal Error: Could not create file " << objectName+string(".cod") << endl; 
			return -1;
		}
		codOutput << machineCode;
		codOutput.close();
	}
	else {
		ElfCreator *elfCreator = new ElfCreator(objectName,machineText,machineData,baseAddress,intelDataStart,intelSymbols);
		elfCreator->elfCreate();
		delete elfCreator;
	}
	return 0;
}

int Intel32Translator::findSymbol(int reference) {
	vector<string> allEntries = objSymbols.getEntries();
	for (string &entry : allEntries) {
		vector<int> refAddress = objSymbols.getUsedAddress(entry);
		vector<int>::iterator it = find(refAddress.begin(),refAddress.end(),reference);
		if (it != refAddress.end()) {
			return objSymbols.getAddress(entry);
		}
	}
	return -1;
}

int verboseAssemble(string s1,string s2,SymbolTable &symTable,vector<int> &raw,vector<int> &result,int &dataOff,bool &hasLink) {
	PreProcessor preProcessor(s1,s1);
	MacroExtender macroExtender(s1,s1);
	Assembler assembler(s1,s2);
	int errors = 0;
	if (!preProcessor.preProcess()) {
		cout	<< "File " << s1+".asm" << " pre-processed into file " << s1+".pre" << endl;
		if (!macroExtender.macroExtend()) {
			cout	<< "File " << s1+".pre";
			cout	<< " macro-extended into file " << s1+".mcr" << endl;
			if (!assembler.onePassAssemble()) {
				cout	<< "File " << s1+".mcr";
				cout	<< " assembled into file " << s2+".o" << endl;
				symTable = assembler.getSymbolTable();
				raw = assembler.getRawCode();
				result = assembler.getResultCode();
				dataOff = assembler.getDataStart();
				hasLink = assembler.hasLinkables();
			}
			else {
				errors++;
			}
		}
		else {
			errors++;
		}
	}
	else {
		errors++;
	}
	return errors;
}

ostream &operator<<(ostream &output,const vector<int> &array) {
	for (unsigned j = 0;j < array.size();j++) {
		output	<< array.at(j);
		if (j < array.size()-1) {
			output << " ";
		}
	}
	return output;
}

ostream &operator<<(ostream &output,const vector<unsigned char> &array) {
	output << setfill('0');
	for (unsigned j = 0;j < array.size();j++) {
		output	<< hex << setw(2) << uppercase << static_cast<int>(array.at(j));
		if (j < array.size()-1) {
			output << " ";
		}
	}
	return output;
}

ofstream &operator<<(ofstream &output,const vector<unsigned char> &array) {
	output << setfill('0');
	for (unsigned j = 0;j < array.size();j++) {
		output	<< hex << setw(2) << uppercase << static_cast<int>(array.at(j));
		if (j < array.size()-1) {
			output << " ";
		}
	}
	return output;
}

vector<unsigned char> &operator<<(vector<unsigned char> &output,const vector<unsigned char> &vec) {
	for (const auto &i : vec) {
		output.push_back(i);
	}
	return output;
}

vector<unsigned char> &operator<<(vector<unsigned char> &output,const vector<int> &vec) {
	for (const auto &i : vec) {
		output.push_back(static_cast<unsigned char>(i & 0xFF));
	}
	return output;
}

vector<unsigned char> &operator<<(vector<unsigned char> &output,const unsigned char &c) {
	output.push_back(c);
	return output;
}

vector<unsigned char> &operator<<(vector<unsigned char> &output,const int &integer) {
	for (size_t j = 0;j < sizeof(int);j++) {
		output.push_back(static_cast<unsigned char>((integer >> (8*j)) & 0xFF));
	}
	return output;
}

vector<string> &operator<<(vector<string> &output,string &s) {
	output.push_back(s);
	return output;
}
