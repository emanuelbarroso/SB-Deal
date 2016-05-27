/**
 * @author Emanuel B.
 * Language Manual class source code and side functions
 *
**/

#include "LangManual.h"

LangManual::LangManual(string inStr) {
	inputString = inStr;
	for (int i = 0; i < (int) inputString.length(); i++) {
		inputString[i] = toupper(inputString[i]);
	}
	initOpcodeInfo();
	initDirectiveInfo();
}

void LangManual::initOpcodeInfo() {
	opcodePages = {
		{
			1,"ADD",1,"Adds data to accumulator",
			{
				"Acc += MEM[Op1];"
			}
		},
		{
			2,"SUB",1,"Subtracts data from accumulator",
			{
				"Acc -= MEM[Op1];"
			}
		},
		{
			3,"MULT",1,"Multiplies data to accumulator",
			{
				"Acc *= MEM[Op1];"
			}
		},
		{
			4,"DIV",1,"Divides data from accumulator. Division by 0 is not permitted",
			{
				"Acc /= MEM[Op1];"
			}
		},
		{
			5,"JMP",1,"Jumps to a label always",
			{
				"goto LABEL;"
			}
		},
		{
			6,"JMPN",1,"Jumps to a label if accumulator is negative",
			{
				"if (Acc < 0) goto LABEL;"
			}
		},
		{
			7,"JMPP",1,"Jumps to a label if accumulator is positive",
			{
				"if (Acc > 0) goto LABEL;"
			}
		},
		{
			8,"JMPZ",1,"Jumps to a label if accumulator is zero",
			{
				"if (Acc == 0) goto LABEL;"
			}
		},
		{
			9,"COPY",2,"Copies memory contents to another memory portion",
			{
				"MEM[Op2] = MEM[Op1];"
			}
		},
		{
			10,"LOAD",1,"Loads data to accumulator from memory",
			{
				"Acc = MEM[Op1];"
			}
		},
		{
			11,"STORE",1,"Stores data from accumulator to memory",
			{
				"MEM[Op1] = Acc;"
			}
		},
		{
			12,"INPUT",1,"Inputs integer data to memory",
			{
				"scanf(\"%d\",&MEM[Op1]);"
			}
		},
		{
			13,"OUTPUT",1,"Outputs integer data from memory",
			{
				"printf(\"%d\\n\",MEM[Op1]);"
			}
		},
		{
			14,"STOP",0,"Terminates the program returning zero",
			{
				"return 0;"
			}
		},
		{
			15,"C_INPUT",1,"Inputs character data to memory",
			{
				"scanf(\"%c\",&MEM[Op1]);"
			}
		},
		{
			16,"C_OUTPUT",1,"Outputs character data from memory",
			{
				"printf(\"%c\\n\",MEM[Op1]);"
			}
		},
		{
			17,"S_INPUT",1,"Inputs string data to memory",
			{
				"scanf(\"%s\",MEM[Op1]);"
			}
		},
		{
			18,"S_OUTPUT",1,"Outputs string data from memory",
			{
				"printf(\"%s\\n\",MEM[Op1]);"
			}
		},
		{
			19,"SWAP",2,"Swaps memory contents over two given positions",
			{
				"_T = MEM[Op1];",
				"MEM[Op1] = MEM[Op2];",
				"MEM[Op2] = _T;"
			}
		},
		{
			20,"LOAD_R",2,"Loads data to accumulator from memory, position relative",
			{
				"Acc = MEM[Op1 + MEM[Op2]];"
			}
		},
		{
			21,"STORE_R",2,"Stores data from accumulator to memory, position relative",
			{
				"MEM[Op1 + MEM[Op2]] = Acc;"
			}
		},
		{
			22,"AND",1,"Performs AND logic to accumulator",
			{
				"Acc &= MEM[Op1];"
			}
		},
		{
			23,"OR",1,"Performs OR logic to accumulator",
			{
				"Acc |= MEM[Op1];"
			}
		},
		{
			24,"XOR",1,"Performs XOR logic to accumulator",
			{
				"Acc ^= MEM[Op1];"
			}
		},
		{
			25,"PUSH_M",1,"Pushes memory contents onto the stack",
			{
				"Stack[++Top] = MEM[Op1];"
			}
		},
		{
			26,"POP_M",1,"Pops memory contents from the stack",
			{
				"MEM[Op1] = Stack[Top--];"
			}
		},
		{
			27,"PUSH_AC",0,"Pushes accumulator onto the stack",
			{
				"Stack[++Top] = Acc;"
			}
		},
		{
			28,"POP_AC",0,"Pops accumulator from the stack",
			{
				"Acc = Stack[Top--];"
			}
		},
		{
			29,"REM",1,"Gets division remainder from accumulator. Division by 0 is not permitted",
			{
				"Acc %= MEM[Op1];"
			}
		},
		{
			30,"INCR_M",1,"Increments memory contents by one",
			{
				"MEM[Op1]++;"
			}
		},
		{
			31,"DECR_M",1,"Decrements memory contents by one",
			{
				"MEM[Op1]--;"
			}
		},
		{
			32,"INCR_A",0,"Increments accumulator by one",
			{
				"Acc++;"
			}
		},
		{
			33,"DECR_A",0,"Decrements accumulator by one",
			{
				"Acc--;"
			}
		},
	};
}

void LangManual::initDirectiveInfo() {

}

void LangManual::printInfo() {
	int opcTry = opcodeTable.getOpcode(inputString);
	if (opcTry) {
		try {
			cout	<< opcodePages.at(opcTry - 1) << endl;
		}
		catch(...) {
			cerr	<< "Instruction not explained yet!" << endl << endl;
		}
	}
}

ostream &operator<<(ostream &output, const OpcodeInfo &ref) {
	vector<string> aux = ref.algorithm;
	output	<< "\t--> Instruction: " << left << setw(10) << ref.name;
	output	<< " Opcode: " << left << setw(3) << ref.opcode << " Operands: " << left << setw(2) << ref.numOps << "\n";
	output	<< "\t -> Description:\n\t\t --> " << ref.desc << "\n\n";
	output	<< "\t -> Algorithm:\n";
	for (string &i : aux) {
		output	<< "\t\t $ " << i << "\n";
	}
	return output;
}
