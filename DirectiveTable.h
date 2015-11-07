/**
 * @author Emanuel B.
 * Directive Table class header
 *
**/

#include <iostream>
#include <map>
#include <string>
#include <iomanip>
using namespace std;

#ifndef DIRECTIVE_TABLE_H
#define DIRECTIVE_TABLE_H

class DirectiveTable {
private:
	map<string,int> directiveTable; // Maps directive name to directive

public:
	DirectiveTable();
	bool isDirective(string);
	int getOperandNum(string);
	friend ostream &operator<<(ostream &,const DirectiveTable &);
};


#endif
