SECTION TEXT
	INPUT Q
	COPY Q,Q+1
	COPY Q+1,Q+2
	LOAD Q+2
	ADD M1
	STORE Q+3
	OUTPUT Q
	OUTPUT Q+1
	OUTPUT Q+2
	OUTPUT Q+3
	STOP
SECTION DATA
Q:	SPACE 4
M1:	CONST 10
