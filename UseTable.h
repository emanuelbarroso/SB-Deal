/**
 * @author Emanuel B.
 * Use Table class header
 *
**/

#include <iostream>
#include <iomanip>
#include <string>
#include <map>
#include <vector>
#include <fstream>

using namespace std;

#ifndef USE_TABLE_H
#define USE_TABLE_H

class UseTable {
private:
	multimap<string,int> addressTable;
public:
	void insertAddress(string,int);
	multimap<string,int> getTable() const;
	bool isUsed(int);
	friend ostream &operator<<(ostream &,const UseTable &);
	friend ofstream &operator<<(ofstream &,const UseTable &);
};

#endif
