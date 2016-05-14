section text
	zero_a
interval:
	input k
	input m
	load k
	jmpge m,interval
test_a:
	a_input
	jmpn test_a
	jmpz test_a
prog_loop:
	jmpz end_prog
	push_ac
	g_randl k,m
	a_output
	ab_output
	pop_ac
	decr_a
	jmp prog_loop
end_prog:
	stop

section data
k:	space
m:	space
