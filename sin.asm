section text
	input k
	input k+1
	load l
	xchg k+1
	store m
	output k+1
	b_output k
	output k
	load k
	not_a
	store k
	b_output k
	output k
	output m
	stop
section data
k:	space 2
l:	const 0xd
m: 	space
