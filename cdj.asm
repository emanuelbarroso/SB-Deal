section text
	input k
	load k
	jmple m,fimdes
	output k
	jmp pare
fimdes:
	output m
pare:
	zero_m k
	output k
	stop
section data
k:	space
m:	const 3
