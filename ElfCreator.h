/**
 * @author Emanuel B.
 * Elf Creator class source code
 * Uses elfio header
**/

#include "elfio/elfio.hpp"
#include "SymbolTable.h"
#include <vector>
#include <cstdlib>
using namespace std;
using namespace ELFIO;

#ifndef ELF_CREATOR_H
#define ELF_CREATOR_H

class ElfCreator {
private:
	char *textSeg;
	char *dataSeg;
	int textStart;
	int dataStart;
	int textSize,dataSize;
	string fileDsc;
	SymbolTable symTable;
public:
	explicit ElfCreator(string,vector<unsigned char>,vector<unsigned char>,int,int,SymbolTable);
	~ElfCreator();
	void elfCreate();
};

#endif

