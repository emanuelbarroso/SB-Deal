/**
 * @author Emanuel B.
 * Definition Table class source code
 *
**/

#include "DirectiveTable.h"

DirectiveTable::DirectiveTable() {
	directiveTable.emplace("SPACE", 1);
	directiveTable.emplace("CONST", 1);
	directiveTable.emplace("PUBLIC", 0);
	directiveTable.emplace("EXTERN", 0);
	directiveTable.emplace("BEGIN", 0);
	directiveTable.emplace("END", 0);
	directiveTable.emplace("ORG",1);
	directiveTable.emplace("EQU",1);
	directiveTable.emplace("IF",1);
	directiveTable.emplace("MACRO",1);
	directiveTable.emplace("ENDMACRO",0);
	directiveTable.emplace("SECTION", 0);
	directiveTable.emplace("TEXT",0);
	directiveTable.emplace("DATA",0);
}

bool DirectiveTable::isDirective(string token) {
	return (directiveTable.find(token) != directiveTable.end());
}

int DirectiveTable::getOperandNum(string token) {
	if (isDirective(token)) {
		return directiveTable.find(token)->second;
	}
	return -1;
}

ostream &operator<<(ostream &output,const DirectiveTable &dirTable) {
	map<string,int> auxiliar = dirTable.directiveTable; 
	map<string,int>::iterator it;
	output	<< "\nDIRECTIVE TABLE:\n";
	for (it = auxiliar.begin(); it != auxiliar.end(); it++) {
		output	<< "\t[" << left << setw(9) << it->first << " : " << it->second << "]\n";
	}
	output	<< "END OF DIRECTIVE TABLE\n" << endl;
	return output;
}
