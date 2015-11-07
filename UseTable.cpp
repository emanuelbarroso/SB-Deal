/**
 * @author Emanuel B.
 * Use Table class source code
 *
**/

#include "UseTable.h"

void UseTable::insertAddress(string label,int address) {
	addressTable.emplace(label,address);
}

multimap<string,int> UseTable::getTable() const {
	return addressTable;
}

bool UseTable::isUsed(int address) {
	for (auto &i : addressTable) {
		if (i.second == address) {
			return true;
		}
	}
	return false;
}

ostream &operator<<(ostream &output,const UseTable &useTable) {
	output	<< "\nUSE TABLE:\n";
	for (const auto &it : (useTable.addressTable)) {
		output	<< "\tuse(" << left << setw(15) << it.first << " , " << it.second << ")\n";
	}
	output	<< "\nEND OF USE TABLE\n" << endl;
	return output;
}

ofstream &operator<<(ofstream &output,const UseTable &useTable) {
	output	<< "TABLE USE\n";
	for (const auto &i : (useTable.addressTable)) {
		output << i.first << " " << i.second << "\n";
	}
	return output;
}
