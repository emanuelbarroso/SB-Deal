section text
	input p
	input p+1
	output p
	output p+1
	swap p,p+1
	output p
	output p+1
	load_r p,q
	store_r r,q
	output r+1
	stop
section data
p:	space 2
q:	const 1
r:	space 2
