
SECTION TEXT
INPUT N
LOAD N
FAT: SUB ONE
JMPZ FATFIM
STORE AUX
MULT N
STORE N
LOAD AUX
JMP FAT
FATFIM: OUTPUT N
STOP
SECTION DATA
AUX: SPACE 1
N: SPACE 1
ONE: CONST 1
