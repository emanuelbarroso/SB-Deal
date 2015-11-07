/**
 * @author Emanuel B.
 * Linker class source code and side functions
 *
**/


#include "Linker.h"

Linker::Linker(string objA,string objB,string ext) {
	fileNameA = objA;
	fileNameB = objB;
	fileNameResult = ext;
	linkErrors.clear();
	linkedCode.clear();
}

void Linker::integerTokenize(const string &line,vector<int> &integers,const string &delimiters) {
	size_t lastPos = line.find_first_not_of(delimiters,0);
	size_t pos = line.find_first_of(delimiters,lastPos);
	while (string::npos != pos || string::npos != lastPos) {
		integers.push_back(atoi((line.substr(lastPos,pos-lastPos)).c_str()));
		lastPos = line.find_first_not_of(delimiters,pos);
		pos = line.find_first_of(delimiters,lastPos);
	}
}

void Linker::booleanTokenize(const string &line,vector<bool> &booleans,const string &delimiters) {
	size_t lastPos = line.find_first_not_of(delimiters,0);
	size_t pos = line.find_first_of(delimiters,lastPos);
	while (string::npos != pos || string::npos != lastPos) {
		booleans.push_back(line.substr(lastPos,pos-lastPos)==string("1"));
		lastPos = line.find_first_not_of(delimiters,pos);
		pos = line.find_first_of(delimiters,lastPos);
	}
}

int Linker::linkCodes() {
	vector<string> objA,objB;
	vector<int> codeA,codeB;
	vector<bool> bitA,bitB;
	int codeOffset;
	string auxCode;
	string label;
	int ID;
	ifstream fileA(fileNameA+string(".o"),ios::in);
	ifstream fileB(fileNameB+string(".o"),ios::in);

	if (!fileA) {
		cerr	<< "Fatal Error: Could not open file " << fileNameA+string(".o") << endl; 
		return -1;
	}
	if (!fileB) {
		cerr	<< "Fatal Error: Could not open file " << fileNameB+string(".o") << endl; 
		return -1;
	}

	cout	<< "Linking " << fileNameA+string(".o") << " and " << fileNameB+string(".o") << "..." << endl;

	while(getline(fileA,auxCode)) {
		objA.push_back(auxCode);
	}
	while(getline(fileB,auxCode)) {
		objB.push_back(auxCode);
	}
	remove_if(objA.begin(),objA.end(),isEmpty);
	remove_if(objB.begin(),objB.end(),isEmpty);

	//cout << objA << endl << objB << endl;

	if (find(objA.begin(),objA.end(),"TABLE USE") == objA.end() || find(objB.begin(),objB.end(),"TABLE USE") == objB.end()) {
		linkErrors.push_back("Link Error: At least one of the object codes is not linkable");
	}
	else if (find(objA.begin(),objA.end(),"TABLE DEFINITION")==objA.end()||find(objB.begin(),objB.end(),"TABLE DEFINITION")==objB.end()) {
		linkErrors.push_back("Link Error: At least one of the object codes is not linkable");		
	} 
	else if (find(objA.begin(),objA.end(),"CODE") == objA.end() || find(objB.begin(),objB.end(),"CODE") == objB.end()) {
		linkErrors.push_back("Link Error: At least one of the object codes is not linkable");
	}
	else if (find(objA.begin(),objA.end(),"BITMAP") == objA.end() || find(objB.begin(),objB.end(),"BITMAP") == objB.end()) {
		linkErrors.push_back("Link Error: At least one of the object codes is not linkable");
	}
	else {
		vector<string>::iterator it;
		string curCode;
		it = find(objA.begin(),objA.end(),"CODE");
		curCode = *(it+1);
		integerTokenize(curCode,codeA," ");
		it = find(objB.begin(),objB.end(),"CODE");
		curCode = *(it+1);
		integerTokenize(curCode,codeB," ");
		it = find(objA.begin(),objA.end(),"BITMAP");
		curCode = *(it+1);
		booleanTokenize(curCode,bitA," ");
		it = find(objB.begin(),objB.end(),"BITMAP");
		curCode = *(it+1);
		booleanTokenize(curCode,bitB," ");
		codeOffset = codeA.size();
		for (it = find(objA.begin(),objA.end(),"TABLE USE");it != find(objA.begin(),objA.end(),"TABLE DEFINITION");it++) {
			if ((*it)!="TABLE USE") {
				stringstream scin(*it);
				scin >> label >> ID;
				useTableA.insertAddress(label,ID);
				//cout	<< label << " - " << ID << endl;
			}
		}
		for (it = find(objB.begin(),objB.end(),"TABLE USE");it != find(objB.begin(),objB.end(),"TABLE DEFINITION");it++) {
			if ((*it)!="TABLE USE") {
				stringstream scin(*it);
				scin >> label >> ID;
				useTableB.insertAddress(label,ID);
				//cout	<< label << " - " << ID << endl;
			}
		}
		for (it = find(objA.begin(),objA.end(),"TABLE DEFINITION");it != find(objA.begin(),objA.end(),"CODE");it++) {
			if ((*it)!="TABLE DEFINITION") {
				stringstream scin(*it);
				scin >> label >> ID;
				defTableA.insertAddress(label,ID);
				//cout	<< label << " - " << ID << endl;
			}
		}
		for (it = find(objB.begin(),objB.end(),"TABLE DEFINITION");it != find(objB.begin(),objB.end(),"CODE");it++) {
			if ((*it)!="TABLE DEFINITION") {
				stringstream scin(*it);
				scin >> label >> ID;
				defTableB.insertAddress(label,ID);
				//cout	<< label << " - " << ID << endl;
			}
		}
		if (haveSameLabel(defTableA,defTableB)) {
			linkErrors.push_back("Link Error: Same label discovered in both Definition Tables");
		}
		else {
			globalDefs	<< defTableA;
			globalDefs	<< (defTableB + codeOffset);
			//cout	<< globalDefs << endl;
			multimap<string,int> useMapAux = useTableA.getTable();
			for (auto &i : useMapAux) {
				int useAddress = globalDefs.getAddress(i.first);
				if (useAddress > -1 && bitA.at(i.second)) {
					codeA.at(i.second) += useAddress;
				}
				else {
					linkErrors.push_back(string("Link Error: Label \"")+i.first+string("\" is not defined in Global Definition Table"));
				}
			}
			useMapAux = useTableB.getTable();	
			for (auto &i : useMapAux) {
				int useAddress = globalDefs.getAddress(i.first);
				if (useAddress > -1 && bitB.at(i.second)) {
					codeB.at(i.second) += useAddress;
				}
				else {
					linkErrors.push_back(string("Link Error: Label \"")+i.first+string("\" is not defined in Global Definition Table"));
				}
			}
			for (unsigned j = 0;j < codeB.size();j++) {
				if (bitB.at(j) && !useTableB.isUsed(j)) {
					codeB.at(j) += codeOffset;
				}
			}
			linkedCode << codeA << codeB;		
		}
	}

	if (linkErrors.size()) {
		printLinkErrors();
	}
	else {
		ofstream fileOut(fileNameResult + ".e",ios::out);
		if (!fileOut) {
			cerr	<< "Fatal Error: Could not create file " << fileNameResult + ".e" << endl; 
			return -1;
		}
		fileOut	<< linkedCode;
	}

	return linkErrors.size();
}

void Linker::printLinkErrors() {
	cout	<< "Linking not successful due to following error(s):" << endl;
	for (const auto &i : linkErrors) {
		cout	<< "\t" << i << endl;
	}
	cout	<< "Linked Object File not generated.\n" << endl; 
}

bool isEmpty(string s) {
	return (!s.length());
}

void printObjects(vector<int> i,vector<bool> b) {
	for (unsigned j = 0;j<i.size() && j<b.size();j++) {
		cout	<< boolalpha << "(" << i[j] << " - " << b[j] << ")";
	}
	cout	<< noboolalpha << endl;
}

bool haveSameLabel(DefinitionTable defA,DefinitionTable defB) {
	map<string,int> mapA = defA.getTable();
	map<string,int> mapB = defB.getTable();
	for (auto &i : mapA) {
		if (mapB.find(i.first)!=mapB.end()) {
			return true;
		}
	}
	return false;
}
