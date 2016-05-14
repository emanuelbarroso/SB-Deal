# Makefile for SB_DEAL Assembler
# Author: Emanuel B.


# Flags
CXX = g++-5
FLAG = -I. -c -Wall -std=c++11

# Sources
SOURCES = main.cpp DirectiveTable.cpp OpcodeTable.cpp Interface.cpp PreProcessor.cpp Assembler.cpp DefinitionTable.cpp MacroExtender.cpp UseTable.cpp SymbolTable.cpp Linker.cpp Simulator.cpp IA32Translator.cpp ElfCreator.cpp MipsTranslator.cpp

# Objects
OBJECTS = $(patsubst %.cpp,%.o,$(SOURCES))

# Executable file
EXECUTABLE = sbdeal

# Compile
all:
	$(CXX) $(FLAG) $(SOURCES)
	$(CXX) $(OBJECTS) -o $(EXECUTABLE)
	$(CXX) linc.cpp -o linc -std=c++11

# Clean objects and executable
PHONY: clean
clean:
	rm -f *.o $(EXECUTABLE)

# Clean only objects
PHONY: orm
orm:
	rm -f *.o

# Make code statistics using linc
PHONY: statistics
statistics:
	./linc >> code_stats.txt

# Backup Files
PHONY: backup
backup:
	mkdir ~/backup_sbdeal_sources
	cp *.cpp ~/backup_sbdeal_sources
	cp *.h ~/backup_sbdeal_sources
	cp *.asm ~/backup_sbdeal_sources
	cp Makefile ~/backup_sbdeal_sources
	cp -ra elfio ~/backup_sbdeal_sources
