/**
 * @author Emanuel B.
 * Symbol Table class source code
 *
**/

#include "SymbolTable.h"

void SymbolTable::insertSymbol(string name,int address,int actualLine,bool isConst,bool isSpace,bool isDef,bool isExt,bool isPub) {
	map<string,int>::iterator auxIterator = addressTable.find(name);
	map<string,vector<int> >::iterator vecIterator;
	vector<int> auxVector;
	if (auxIterator != addressTable.end() && !isDef) {
		vecIterator = usedLines.find(name);
		auxVector = vecIterator->second;
		auxVector.push_back(actualLine);
		vecIterator->second = auxVector;

		vecIterator = usedAddress.find(name);
		auxVector = vecIterator->second;
		auxVector.push_back(address);
		vecIterator->second = auxVector;
	}
	else {
		addressTable.emplace(name,address);
		lineDefined.emplace(name,actualLine);
		dataSymbol.emplace(name,make_pair(isConst,isSpace));
		definedSymbol.emplace(name,isDef);
		externSymbol.emplace(name,isExt);
		publicSymbol.emplace(name,isPub);
		valueSymbol.emplace(name,0);
		usedLines.emplace(name,vector<int>());
		usedAddress.emplace(name,vector<int>());
		if (!isDef) {
			vecIterator = usedLines.find(name);
			auxVector = vecIterator->second;
			auxVector.push_back(actualLine);
			vecIterator->second = auxVector;

			vecIterator = usedAddress.find(name);
			auxVector = vecIterator->second;
			auxVector.push_back(address);
			vecIterator->second = auxVector;
		}
	}
}

void SymbolTable::setSymbolFlags(string name,bool cntFlag, bool spcFlag,bool defFlag,bool extFlag,bool pubFlag) {
	map<string,bool>::iterator boolIterator;
	boolIterator = definedSymbol.find(name);
	if (boolIterator != definedSymbol.end() && !(boolIterator->second)) {		
		boolIterator->second = defFlag;
	}
	boolIterator = externSymbol.find(name);
	if (boolIterator != externSymbol.end() && !(boolIterator->second)) {
		boolIterator->second = extFlag;
	}
	boolIterator = publicSymbol.find(name);
	if (boolIterator != publicSymbol.end() && !(boolIterator->second)) {
		boolIterator->second = pubFlag;
	}
	map<string,pair<bool,bool> >::iterator pairBoolIterator;
	pairBoolIterator = dataSymbol.find(name);
	if (pairBoolIterator != dataSymbol.end()) {
		bool firstPair = ((pairBoolIterator->second).first) ? true : cntFlag; 
		bool secondPair = ((pairBoolIterator->second).second) ? true : spcFlag; 
		pairBoolIterator->second = make_pair(firstPair,secondPair);
	}
}

void SymbolTable::setSymbolValue(string name,int val) {
	map<string,int>::iterator it = valueSymbol.find(name);
	if (it != valueSymbol.end()) {
		it->second = val;
	}
}

void SymbolTable::setSymbolLine(string name,int val) {
	map<string,int>::iterator it = lineDefined.find(name);
	if (it != lineDefined.end()) {
		it->second = val;
	}
}

void SymbolTable::setSymbolAddress(string name,int val) {
	map<string,int>::iterator it = addressTable.find(name);
	if (it != addressTable.end()) {
		it->second = val;
	}
}

void SymbolTable::addDataOffset(int offset) {
	for (auto &i : addressTable) {
		if (isDataSymbol(i.first)) {
			i.second += offset;
		}
	}
}

void SymbolTable::addReferences(string name,int address,int line) {
	map<string,vector<int> >::iterator it = usedLines.find(name);
	vector<int> aux;
	if (it != usedLines.end()) {
		aux = it->second;
		aux.push_back(line);
		it->second = aux;
	}

	it = usedAddress.find(name);
	if (it != usedAddress.end()) {
		aux = it->second;
		aux.push_back(address);
		it->second = aux;
	}
}

bool SymbolTable::isDefinedSymbol(string name) {
	map<string,bool>::iterator it = definedSymbol.find(name);
	if (it != definedSymbol.end()) {
		return it->second;
	}
	return false;
}

bool SymbolTable::isExternSymbol(string name) {
	map<string,bool>::iterator it = externSymbol.find(name);
	if (it != externSymbol.end()) {
		return it->second;
	}
	return false;
}

bool SymbolTable::isPublicSymbol(string name) {
	map<string,bool>::iterator it = publicSymbol.find(name);
	if (it != publicSymbol.end()) {
		return it->second;
	}
	return false;
}

bool SymbolTable::isSymbol(string name) {
	return (addressTable.find(name) != addressTable.end());
}

bool SymbolTable::isDataSymbol(string name) {
	map<string,pair<bool,bool> >::iterator it = dataSymbol.find(name);
	if (it != dataSymbol.end()) {
		pair<bool,bool> i = it->second;
		return ((i.first) || (i.second));
	}
	return false;
}

bool SymbolTable::isConstSymbol(string name) {
	map<string,pair<bool,bool> >::iterator it = dataSymbol.find(name);
	if (it != dataSymbol.end()) {
		return ((it->second).first);
	}
	return false;
}

vector<int> SymbolTable::getUsedLines(string name) {
	map<string,vector<int> >::iterator it = usedLines.find(name);
	if (it != usedLines.end()) {
		return it->second;
	}
	return vector<int>();
}

vector<int> SymbolTable::getUsedAddress(string name) {
	map<string,vector<int> >::iterator it = usedAddress.find(name);
	if (it != usedAddress.end()) {
		return it->second;
	}
	return vector<int>();
}

vector<string> SymbolTable::getEntries() {
	vector<string> result;
	for (const auto &i : addressTable) {
		result.push_back(i.first);
	}
	return result;
}

int SymbolTable::getAddress(string name) {
	map<string,int>::iterator it = addressTable.find(name);
	if (it != addressTable.end()) {
		return it->second;
	}
	return -1;
}

int SymbolTable::getDefLine(string name) {
	map<string,int>::iterator it = lineDefined.find(name);
	if (it != lineDefined.end()) {
		return it->second;
	}
	return -1;
}

int SymbolTable::getSizeSpace(string name) {
	map<string,int>::iterator it = valueSymbol.find(name);
	map<string,pair<bool,bool> >::iterator pt = dataSymbol.find(name);

	if (it != valueSymbol.end()) {
		pair<bool,bool> dSym = pt->second;
		if (!(dSym.first ^ dSym.second)) {
			return -1;
		}
		else if (dSym.first) {
			return 0;
		}
		else {
			return (it->second - 1);
		}
	}
	return -1;
}

int SymbolTable::getConstValue(string name) {
	map<string,int>::iterator it = valueSymbol.find(name);
	if (isConstSymbol(name) && it != valueSymbol.end()) {
		return it->second;
	}
	return 100000;
}

string SymbolTable::getSymbol(int address) {
	map<string,int>::iterator it;
	for (it = addressTable.begin();it != addressTable.end();it++) {
		if (address==it->second) {
			return (it->first);
		}
	}
	return string("");
}

bool SymbolTable::foundLinks() {
	for (const auto &i : addressTable) {
		if (isPublicSymbol(i.first) || isExternSymbol(i.first)) {
			return true;
		}
	}
	return false;
}

ostream &operator<<(ostream &output,SymbolTable &symTable) {
	map<string,int>::iterator intIterator;
	map<string,bool>::iterator boolIterator;
	map<string,pair<bool,bool> >::iterator pairIterator;
	map<string,vector<int> >::iterator vectorIterator;
	vector<int> auxiliarVector;

	output	<< "\nNAME[ADDRESS][LINE](VALUE): $<(DEF)(EXT)(PUB)(CNT)(SPC)> { V(LIN) V(ADD) }\n\n"; 

	output	<< noboolalpha;
	for (const auto &i : symTable.addressTable) {
		output	<< i.first << "[" << i.second << "][";
		intIterator = (symTable.lineDefined).find(i.first);
		output	<< intIterator->second << "](";
		intIterator = (symTable.valueSymbol).find(i.first);
		output	<< intIterator->second << "): $<";
		boolIterator = (symTable.definedSymbol).find(i.first);
		output	<< boolIterator->second;
		boolIterator = (symTable.externSymbol).find(i.first);
		output	<< boolIterator->second;
		boolIterator = (symTable.publicSymbol).find(i.first);
		output	<< boolIterator->second;
		pairIterator = (symTable.dataSymbol).find(i.first);
		output	<< (pairIterator->second).first << (pairIterator->second).second << "> {\n";
		output	<< "\t[";
		vectorIterator = (symTable.usedLines).find(i.first);
		auxiliarVector = vectorIterator->second;
		for (unsigned i = 0;i < auxiliarVector.size();i++) {
			output	<< auxiliarVector.at(i);
			if (i != auxiliarVector.size()-1) {
				output	<< ",";
			}
		}
		output	<< "]\n\t[";
		vectorIterator = (symTable.usedAddress).find(i.first);
		auxiliarVector = vectorIterator->second;
		for (unsigned j = 0;j < auxiliarVector.size();j++) {
			output	<< auxiliarVector.at(j);
			if (j != auxiliarVector.size()-1) {
				output	<< ",";
			}
		}
		output	<< "]\n}\n";
	}
	return output;
}
