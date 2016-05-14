section text
	input k
	load k
	div nine
	store m
	load k
	rem nine
	store m[1]
	output m
	output m[1]
	load m
	sub m[1]
	a_output
	stop
section data
k:	space
nine: const 9
m:	space 2
