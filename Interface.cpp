/**
 * @author Emanuel B.
 * Program Interface class source code and side functions
 *
**/

#include "Interface.h"

Interface::Interface(int argc, char **argv) {
	string thisName = argv[0];
	thisName.erase(0,2);
	canRun = true;
	try {
		numArgs = argc;
		if (numArgs==1) {
			throw 1;
		}
		for (int i = 0;i<argc;i++) {
			arrayArgs.push_back(string(argv[i]));
		}
	}
	catch (int &x) {
		cout << "Incorrect Usage!" << endl;
		cout << "@ Correct uses:" << endl;
		showHelp(thisName);
		canRun = false;
	}
}

Interface::~Interface() {
	try {
		arrayArgs.clear();
	}
	catch (...) {

	}
}

int Interface::runInterface() {
	int cmd;
	string simArgAux;
	bool ofbCheck = false;
	vector<string> *auxiliarArg;
	//cout << "n[" << numArgs << "]" << endl;
	//cout << "a[" << arrayArgs.at(1) << "]" << endl;
	//cout << "This name is " << thisName << endl;
	if (!canRun) {
		return -1;
	}
	string thisName = arrayArgs.at(0);
	thisName.erase(0,2);
	try {
		if (numArgs==1) {
			throw 1;
		}
		cmd = findCommand(arrayArgs.at(1));
		//cout << "cmd[" << cmd << "]" << endl;
		switch (cmd) {
			case 1:
				if (numArgs!=4) {
					throw 1;
				}
				auxPre(arrayArgs.at(2),arrayArgs.at(3));
				break;
			case 2:
			case 3:
				if (numArgs!=4) {
					throw 1;
				}
				auxMcr(arrayArgs.at(2),arrayArgs.at(3),cmd,3);
				break;
			case 4:
			case 5:
				if (numArgs!=4) {
					throw 1;
				}
				completeAssemble(arrayArgs.at(2),arrayArgs.at(3),cmd,5);
				break;
			case 6:
				if (numArgs!=4 && numArgs!=6) {
					throw 1;
				}
				auxiliarArg = new vector<string>(arrayArgs.begin()+2,arrayArgs.begin()+numArgs);
				auxTranslate(*auxiliarArg,thisName);
				delete auxiliarArg;
				break;
			case 7:
				if (numArgs!=4 && numArgs!=5) {
					throw 1;
				}
				auxiliarArg = new vector<string>(arrayArgs.begin()+2,arrayArgs.begin()+numArgs);
				auxMips(*auxiliarArg,thisName);
				delete auxiliarArg;
				break;
			case 8:
				if (numArgs!=5) {
					throw 1;
				}
				auxLink(arrayArgs.at(2),arrayArgs.at(3),arrayArgs.at(4));
				break;
			case 9:
			case 10:
				if (numArgs!=5) {
					throw 1;
				}
				completeLink(arrayArgs.at(2),arrayArgs.at(3),arrayArgs.at(4),cmd,10);
				break;
			case 11:
				if (numArgs!=3 && numArgs!=4) {
					throw 1;
				}
				ofbCheck = false;
				if (numArgs<=3) {
					simArgAux = arrayArgs.at(2);
				}
				else {
					simArgAux = arrayArgs.at(3);
					if (arrayArgs.at(3)!="-16bit") {
						throw 1;
					}
					ofbCheck = true;
				}
				auxSim(simArgAux,ofbCheck);
				break;
			case 12:
				if (numArgs!=2) {
					throw 1;
				}
				cout << "@ \"" << thisName << "\" Assembler Help:" << endl;
				helpStart();
				cout << "@ Correct uses:" << endl;
				showHelp(thisName);
				cout << "@ Program Options:" << endl;
				showOptions(); 
				break;
			case 13:
				if (numArgs!=3) {
					throw 1;
				}
				showSpecificHelp(thisName,arrayArgs.at(2));
				break;
			case 14:
				break;
			default:
				throw 1;
		}
	}
	catch (int &x) {
		cout << "Incorrect Usage!" << endl;
		cout << "@ Correct uses:" << endl;
		showHelp(thisName);
	}
	return 0;
}

int completeAssemble(string s1,string s2,int command,int control) {
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
				if (command != control) {
					string preFile = s1+".pre";
					remove(preFile.c_str());
					string mcrFile = s1+".mcr";
					remove(mcrFile.c_str());
				}
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

void auxPre(string s1,string s2) {
	PreProcessor preProcessor(s1,s2);
	if (!preProcessor.preProcess()) {
		cout	<< "File " << s1+".asm" << " pre-processed into file " << s2+".pre" << endl; 
	}
}

void auxMcr(string s1,string s2,int cmd,int control) {
	PreProcessor preProcessor(s1,s1);
	MacroExtender macroExtender(s1,s2);
	if (!preProcessor.preProcess()) {
		cout	<< "File " << s1+".asm" << " pre-processed into file " << s1+".pre" << endl;			
		if (!macroExtender.macroExtend()) {
			cout	<< "File " << s1+".pre";
			cout	<< " macro-extended into file " << s2+".mcr" << endl;
			if (cmd != control) {
				string preFile = s1+".pre";
				remove(preFile.c_str());
			}
		}
	}
}

void auxLink(string s1,string s2,string s3) {
	Linker linker(s1,s2,s3);
	if (!linker.linkCodes()) {
		cout	<< "Files " << s1+".o" << " and " << s2+".o";
		cout	<< " linked into file " << s3+".e" << endl;
	}
}

void completeLink(string s1,string s2,string s3,int cmd,int control) {
	Linker linker(s1,s2,s3);
	if (!completeAssemble(s1,s1,cmd,control) && !completeAssemble(s2,s2,cmd,control)) {
		if (!linker.linkCodes()) {
			cout	<< "Files " << s1+".asm" << " and " << s2+".asm";
			cout	<< " source-linked into file " << s3+".e" << endl;
			if (cmd != control) {
				string objFile = s1 + ".o";
				remove(objFile.c_str());
				objFile = s2 + ".o";
				remove(objFile.c_str());
			}
		}
	} 
}

void auxTranslate(vector<string> arg,string thisName) {
	vector<string> auxiliar = arg;
	string arg1,arg2;
	bool isRaw = false;
	bool isElf = false;
	bool hasDeb = false;
	bool hasnDeb = false;
	try {
		if (auxiliar.at(0)=="-raw") {
			if (isRaw) {
				throw;
			}
			isRaw = true;
		}
		else if (auxiliar.at(0)=="-elf") {
			if (isElf) {
				throw;
			}
			isElf = true;
		}
		else if ((auxiliar.at(0)).find('-')==0) {
			throw;
		}
		else {
			arg1 = auxiliar.at(0);
		}
		auxiliar.erase(auxiliar.begin());

		if (auxiliar.at(0)=="-deb") {
			if (hasDeb) {
				throw;
			}
			hasDeb = true;
		}
		else if (auxiliar.at(0)=="-ndeb") {
			if (hasnDeb) {
				throw;
			}
			hasnDeb = true;
		}
		else if ((auxiliar.at(0)).find('-')==0) {
			throw;
		}
		else {
			arg2 = auxiliar.at(0);
		}
		auxiliar.erase(auxiliar.begin());
		if (auxiliar.size()) {
			arg1 = auxiliar.at(0);
			arg2 = auxiliar.at(1);
		}
		Intel32Translator *translator = new Intel32Translator(arg1,arg2,hasDeb,isElf,0x08048000,0,false);
		translator->translateCode();
		delete translator;		
	}
	catch (...) {
		cout << "Incorrect Usage!" << endl;
		cout << "@ Correct uses:" << endl;
		showHelp(thisName);
	}
}

void auxMips(vector<string> arg,string thisName) {
	vector<string> auxiliar = arg;
	string arg1,arg2;
	bool hasDeb = false;
	bool hasnDeb = false;

	try {
		if (auxiliar.at(0)=="-deb") {
			if (hasDeb) {
				throw;
			}
			hasDeb = true;
		}
		else if (auxiliar.at(0)=="-ndeb") {
			if (hasnDeb) {
				throw;
			}
			hasnDeb = true;
		}
		else if ((auxiliar.at(0)).find('-')==0) {
			throw;
		}

		if ((auxiliar.at(0)).find('-')==0) {
			auxiliar.erase(auxiliar.begin());
		}
		if (auxiliar.size()) {
			arg1 = auxiliar.at(0);
			arg2 = auxiliar.at(1);
		}
		MipsTranslator *translator = new MipsTranslator(arg1,arg2,hasDeb);
		translator->translateMIPSCode();
		delete translator;
	}
	catch (...) {
		cout << "Incorrect Usage!" << endl;
		cout << "@ Correct uses:" << endl;
		showHelp(thisName);
	}
}

void auxSim(string s1,bool ofbCheck) {
	Simulator simulator(s1);
	simulator.simulateCode(ofbCheck);
}

ostream &operator<<(ostream &output,const Interface &interface) {
	output	<< "Lin_Com[" << interface.numArgs << "]:";
	for (int i = 0;i<interface.numArgs;i++) {
		output	<< ' ' << interface.arrayArgs[i];
	}
	output << endl;
	return output; 
}

int findCommand(string argCommand) {
	map<string,int> possibleArgs;
	possibleArgs.emplace("-p",1);		// Pre-process
	possibleArgs.emplace("-m",2);		// Macro-Extend
	possibleArgs.emplace("-Dm",3);		// Debug Macro-Extend
	possibleArgs.emplace("-o",4);		// Assemble
	possibleArgs.emplace("-Do",5);		// Debug Assemble
	possibleArgs.emplace("-tia32",6);	// IA32 translate
	possibleArgs.emplace("-tmips",7);	// Mips translate
	possibleArgs.emplace("-l",8);		// Link
	possibleArgs.emplace("-L",9);		// Link from sources
	possibleArgs.emplace("-DL",10);		// Debug Link from sources 
	possibleArgs.emplace("-s",11);		// Simulate
	possibleArgs.emplace("-h",12);		// General Help
	possibleArgs.emplace("-H",13);		// Command Help
	possibleArgs.emplace("-man",14);	// Language Keyword Help
	// to-do: Specific Help

	if (possibleArgs.find(argCommand) != possibleArgs.end()) {
		return possibleArgs.find(argCommand)->second;
	}
	return 0;
}

void showHelp(string name) {
	string lineStart = "\t$ ./" + name;
	cout	<< lineStart << " -p <file_in> <file_out>" << endl;
	cout	<< lineStart << " -m <file_in> <file_out>" << endl;
	cout	<< lineStart << " -Dm <file_in> <file_out>" << endl;
	cout	<< lineStart << " -o <file_in> <file_out>" << endl;
	cout	<< lineStart << " -Do <file_in> <file_out>" << endl;
	cout	<< lineStart << " -tia32 <argt> <argd> <file_in> <file_out>" << endl;
	cout	<< lineStart << " -tmips <argd> <file_in> <file_out>" << endl;
	cout	<< lineStart << " -l <file_in1> <file2_in> <file_out>" << endl;
	cout	<< lineStart << " -L <file_in1> <file2_in> <file_out>" << endl;
	cout	<< lineStart << " -DL <file_in1> <file2_in> <file_out>" << endl;
	cout	<< lineStart << " -s <file>.<extsim>" << endl;
	cout	<< lineStart << " -s -16bit <file>.<extsim>" << endl;
	cout	<< lineStart << " -h" << endl << endl;
	cout	<< "\t<argt>: -raw,-elf" << endl;
	cout	<< "\t<argd>: -deb,-ndeb" << endl;
	cout	<< "\t<extsim>: o,e" << endl << endl;
}

void helpStart() {
	cout	<< "\t- Author: Emanuel B." << endl << endl;
	cout	<< "\t- This assembler focuses in understand how to translate a simple, hypothetical language to\n";
	cout	<< "\t  something a Linux OS can understand. For such achievement, it's needed to translate this\n";
	cout	<< "\t  language to another one - Intel x86_32bit Assembly language. This assembler can generate\n";
	cout	<< "\t  various files, using things such as a pre-processor, macro-extender, hypothetical object\n";
	cout	<< "\t  code generator, Intel x86 assembly translator, a MIPS translator (for testing purposes),\n";
	cout	<< "\t  a linker and a hypothetical object code simulator." << endl;
	cout	<< "\t- How to use: This is a command-line program. Just enter the file names and parameters, so\n";
	cout	<< "\t  the software will execute the appropriate commands." << endl << endl;
}

void showOptions() {
	map<string,string> optionsHelp;
	optionsHelp.emplace("-p","Pre-process only");
	optionsHelp.emplace("-m","Macro-Extend only");
	optionsHelp.emplace("-Dm","Macro-Extend and keep debug files");
	optionsHelp.emplace("-o","Assemble only");
	optionsHelp.emplace("-Do","Assemble and keep debug files");
	optionsHelp.emplace("-tia32","Translate to x86 Assembly, use with two more parameters");
	optionsHelp.emplace("-tmips","Translate to MIPS Assembly, use with one more parameter");
	optionsHelp.emplace("-raw","Generate raw x86 assembly and machine code, use with -tia32");
	optionsHelp.emplace("-elf","Generate an ELF executable, use with -tia32");
	optionsHelp.emplace("-deb","Keep debug files, used with -tia32 or -tmips");
	optionsHelp.emplace("-ndeb","Trash debug files, used with -tia32 or -tmips");
	optionsHelp.emplace("-l","Link using object files");
	optionsHelp.emplace("-L","Link directly from sources");
	optionsHelp.emplace("-DL","Link directly from sources and keep debug files");
	optionsHelp.emplace("-s","Simulate hypothetical object code");
	optionsHelp.emplace("-16bit","Enable restricted simulation mode, used with -s");
	optionsHelp.emplace("-h","Show main help contents");

	for (auto &x : optionsHelp) {
		cout	<< "\t" << left << setw(10) << x.first << " " << x.second << endl;
	}
	cout << endl;
}

void showSpecificHelp(string exeName,string specHelp) {
	int cmd = findCommand(specHelp);
	cout	<< "@ Help for command \"" << specHelp << "\":" << endl;
	switch (cmd) {
		case 1:
			cout	<< "\t# Pre-Processor Mode:" << endl;
			cout	<< "\t- Given a hypothetical language source file, gives a .pre file. This pre-processor handles\n";
			cout	<< "\t  line formatting and EQU/IF treatments.\n";
			cout	<< "\t- Usage:\n\t\t$ ./" << exeName << " -p <fileIn> <fileOut>";
			break;
		case 2:
			cout	<< "\t# Macro-Extender Mode without Debugs:" << endl;
			cout	<< "\t- Given a hypothetical language source file, gives a .mcr file. The macro-extender outputs\n";
			cout	<< "\t  a file with macros extended. This option trashes any temporary files created.\n";
			cout	<< "\t- Usage:\n\t\t$ ./" << exeName << " -m <fileIn> <fileOut>";
			break;
		case 3:
			cout	<< "\t# Macro-Extender Mode with Debugs:" << endl;
			cout	<< "\t- Given a hypothetical language source file, gives a .mcr file. The macro-extender outputs\n";
			cout	<< "\t  a file with macros extended. This option keeps any temporary files created.\n";
			cout	<< "\t- Usage:\n\t\t$ ./" << exeName << " -Dm <fileIn> <fileOut>";
			break;
		case 4:
			cout	<< "\t# Object Assembler Mode without Debugs:" << endl;
			cout	<< "\t- Given a hypothetical language source file, gives a .o file. The object assembler outputs\n";
			cout	<< "\t  an object code file which can have an additional bitmap, and linking tables, such as use\n";
			cout	<< "\t  and definition tables. This option trashes any temporary files created.\n";
			cout	<< "\t- Usage:\n\t\t$ ./" << exeName << " -o <fileIn> <fileOut>";
			break;
		case 5:
			cout	<< "\t# Object Assembler Mode with Debugs:" << endl;
			cout	<< "\t- Given a hypothetical language source file, gives a .o file. The object assembler outputs\n";
			cout	<< "\t  an object code file which can have an additional bitmap, and linking tables, such as use\n";
			cout	<< "\t  and definition tables. This option keeps any temporary files created.\n";
			cout	<< "\t- Usage:\n\t\t$ ./" << exeName << " -Do <fileIn> <fileOut>";
			break;
		case 6:
			cout	<< "\t# Intel x86 Assembly Translator Mode:" << endl;
			cout	<< "\t- Given a hypothetical language source file, gives several files. This translator receives\n";
			cout	<< "\t  the hypothetical language and translates it to Intel x86 Assembly. The output depends on\n";
			cout	<< "\t  the following arguments:\n";
			cout	<< "\t\t-raw   => Outputs raw x86 machine and assembly code (.s and .cod files)\n";
			cout	<< "\t\t-elf   => Outputs a valid ELF executable file\n";
			cout	<< "\t\t-deb   => Keeps temporary files\n";
			cout	<< "\t\t-ndeb  => Trashes temporary files\n\t- Usages:\n";
			cout	<< "\t\t$ ./" << exeName << " -tia32 -raw -deb <fileIn> <fileOut>\n";
			cout	<< "\t\t$ ./" << exeName << " -tia32 -raw -ndeb <fileIn> <fileOut>\n";
			cout	<< "\t\t$ ./" << exeName << " -tia32 -elf -deb <fileIn> <fileOut>\n";
			cout	<< "\t\t$ ./" << exeName << " -tia32 -elf -ndeb <fileIn> <fileOut>";
			break;
		case 7:
			cout	<< "\t# MIPS Assembly Translator Mode:" << endl;
			cout	<< "\t - (TODO)";
			break;
		case 8:
			cout	<< "\t# Object Linker Mode:" << endl;
			cout	<< "\t- Given two hypothetical object codes, gives a .e file. The linker creates and object code\n";
			cout	<< "\t  joining the two raw objects, given their use and definition tables.\n";
			cout	<< "\t- Usage:\n\t\t$ ./" << exeName << " -l <fileIn_1> <fileIn_2> <fileOut>";
			break;
		case 9:
			cout	<< "\t# Source Linker Mode without debugs:" << endl;
			cout	<< "\t- Given two hypothetical source codes, gives a .e file. This linker assembles the original\n";
			cout	<< "\t  codes, then link them if possible. This option trashes any temporary files.\n";
			cout	<< "\t- Usage:\n\t\t$ ./" << exeName << " -L <fileIn_1> <fileIn_2> <fileOut>";
			break;
		case 10:
			cout	<< "\t# Source Linker Mode with debugs:" << endl;
			cout	<< "\t- Given two hypothetical source codes, gives a .e file. This linker assembles the original\n";
			cout	<< "\t  codes, then link them if possible. This option keeps any temporary files.\n";
			cout	<< "\t- Usage:\n\t\t$ ./" << exeName << " -DL <fileIn_1> <fileIn_2> <fileOut>";
			break;
		case 11:
			cout	<< "\t# Simulator Mode:" << endl;
			cout	<< "\t- Given a hypothetical machine code, simulates its contents. This simulation can have some\n";
			cout	<< "\t  restrictions, given by -16bit command. With this, only 16-bit data will be allowed.\n";
			cout	<< "\t- Usages:\n";
			cout	<< "\t\t$ ./" << exeName << " -s <file>.o\n";
			cout	<< "\t\t$ ./" << exeName << " -s <file>.e\n";
			cout	<< "\t\t$ ./" << exeName << " -s -16bit <file>.o\n";
			cout	<< "\t\t$ ./" << exeName << " -s -16bit <file>.e\n";
			break;
		case 12:
			cout	<< "\t# General Help Mode:" << endl;
			cout	<< "\t- Shows the general help contents.\n";
			cout	<< "\t- Usage:\n\t\t$ ./" << exeName << " -h";
			break;
		case 13:
			cout	<< "\t# Command Help Mode:" << endl;
			cout	<< "\t- Shows help contents for any supported command from this assembler.\n";
			cout	<< "\t- Usage:\n\t\t$ ./" << exeName << " -H <Command>";
			break;
		case 14:
			cout	<< "\t# Assembler Manual Mode:" << endl;
			cout	<< "\t- Shows help contents for any supported keyword from the currently assembled language.\n";
			cout	<< "\t- Usage:\n\t\t$ ./" << exeName << " -man <Keyword>";
			break;
		default:
			cout	<< "\t- Unknown command detected!";
			break;
	}
	cout << endl << endl;
}
