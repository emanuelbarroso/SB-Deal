section text
	load a
	and b
	store c
	output c
	or f
	store c
	output c
	load c
	xor c
	store c
	output c
	load f
	and a
	input q
	push_ac
	push_m q
	pop_m c
	output c
	pop_ac
	store d
	output d
	stop
section data
a:	const 0x17
b:	const 0x6
c:	space 1
d:	space
q:	space
f:	const 0x9
