SECTION TEXT
	INPUT NUM
LACO:
	LOAD NUM
	DIV TEN
	STORE R1
	MULT TEN
	STORE R2
	LOAD NUM
	SUB R2
	STORE RESULT
	OUTPUT RESULT
	LOAD R1
	SUB TEN
	COPY R1, NUM
	JMPP LACO
	ADD TEN
	STORE ULTIMO
	OUTPUT ULTIMO
	STOP

SECTION DATA
NUM: SPACE
TEN: CONST 10
R1: SPACE
R2: SPACE
RESULT: SPACE
ULTIMO: SPACE
