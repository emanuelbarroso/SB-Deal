/**
 * @author Emanuel B.
 * Definition Table class source code
 *
**/

#include "DefinitionTable.h"

void DefinitionTable::insertAddress(string label,int address) {
	addressTable.emplace(label,address);
}

ostream &operator<<(ostream &output,const DefinitionTable &defTable) {
	map<string,int> auxiliarMap = defTable.addressTable;
	map<string,int>::iterator it;
	output	<< "\nDEFINITION TABLE:\n";
	for (it = auxiliarMap.begin(); it != auxiliarMap.end(); it++) {
		output	<< "\tdef(" << left << setw(15) << it->first << " , " << it->second << ")\n";
	}
	output	<< "END OF DEFINITION TABLE\n" << endl;
	return output;
}

int DefinitionTable::getAddress(string token) const {
	map<string,int> auxMap = this->addressTable;
	map<string,int>::iterator auxFind = auxMap.find(token);
	if (isDefinedLabel(token)) {
		return (auxFind->second);
	}
	return -1;
}

bool DefinitionTable::isDefinedLabel(string token) const {
	map<string,int> auxMap = this->addressTable;
	return (auxMap.find(token) != auxMap.end());
}

map<string,int> DefinitionTable::getTable() const {
	return addressTable;
}

ofstream &operator<<(ofstream &output,const DefinitionTable &defTable) {
	output	<< "TABLE DEFINITION\n";
	for (const auto &i : (defTable.addressTable)) {
		output << i.first << " " << i.second << "\n";
	}
	return output;
}

DefinitionTable DefinitionTable::operator+(const int &rhs) {
	DefinitionTable result;
	for (auto &i : this->addressTable) {
		result.insertAddress(i.first,i.second+rhs);
	}
	return result;
}

DefinitionTable &operator<<(DefinitionTable &lhs,const DefinitionTable &rhs) {
	map<string,int> rhsMap = rhs.getTable();
	for (const auto &i : rhsMap) {
		lhs.insertAddress(i.first,i.second);
	}
	return lhs;
}
