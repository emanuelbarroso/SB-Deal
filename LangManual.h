/**
 * @author Emanuel B.
 * Language Manual class header
 *
**/

#include <iostream>
#include <iomanip>
#include <string>
#include <map>
#include <vector>
#include <cctype>
#include "OpcodeTable.h"
#include "DirectiveTable.h"
using namespace std;

typedef struct OpcodeInfo {
	int opcode;
	string name;
	int numOps;
	string desc;
	vector<string> algorithm;
	OpcodeInfo() :
		opcode(0), name(""), numOps(0), desc(""), algorithm({}) {}
	OpcodeInfo(int x,string n, int o, string d, vector<string> a) :
		opcode(x), name(n), numOps(o), desc(d), algorithm(a) {}
} OpcodeInfo;

class LangManual {
private:
	OpcodeTable opcodeTable;
	DirectiveTable directiveTable;
	string inputString;
	vector<OpcodeInfo> opcodePages;
	map<string,string> directivePages;
public:
	explicit LangManual(string);
	~LangManual() {};
	void showManual();
	void initOpcodeInfo();
	void initDirectiveInfo();
	void printInfo();
};

ostream &operator<<(ostream &,const OpcodeInfo &);
