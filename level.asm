section text
	input k
	input k+1
	output k
	output k+1
	level k,k+1
	output k
	output k+1
	zero_a
	incr_a
	incr_a
	store k+1
	zero_a
	sub k+1
	sub k+1
	store k
	output k
	output k+1
	load k
	shl k+1
	store m
	b_output m
	load k
	shr k+1
	store m
	b_output m
	load k
	sar k+1
	store m
	b_output m
	stop
section data
k:	space 2
m:	space
