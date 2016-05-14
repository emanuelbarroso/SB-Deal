;entrada: 
;Soma dígitos de um número decimal
section text
	input N
	copy N, aux
	copy zero, sum

loop: load N
	jmpz out
	div dez
	mult dez
	store aux
	load N
	sub aux
	add sum
	store sum
	load N
	div dez
	store N
	jmp loop
out: output sum
	stop

section data
	zero: const 0x00
	dez: const 0xA
	sum: space
	aux: space
	N: space
