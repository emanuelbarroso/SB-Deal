/**
 * @author Emanuel B.
 * Pre Processor class header
 *
**/

#include <iostream>
#include <iomanip>
#include <fstream>
#include <map>
#include <string>
#include <regex>
#include <algorithm>
#include <cstdio>
#include "Assembler.h"

#ifndef PREPROCESSOR_H
#define PREPROCESSOR_H

class PreProcessor : public Assembler {
public:
	PreProcessor(string,string,bool=true);
	int preProcess();
	void printErrors() const;
};
void stringTrim(string &,char,bool=true);
void stringUpperCase(string &);
void prepareLine(string &);
#endif
