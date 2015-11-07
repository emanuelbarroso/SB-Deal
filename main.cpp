#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include "Interface.h"
#include "IA32Translator.h"
#pragma message "Author: Emanuel B. All rights reserved."
using namespace std;

int main(int argc,char *argv[]) {
	Interface interface(argc,argv);
	interface.runInterface();
	//Intel32Translator iaTranslator(arg1,arg2,false,true,0x08048000,0,false);
	//iaTranslator.translateCode();
	return 0;
}
