section text
	load i
nloop:
	push_ac
	g_randl z,r
	store k
	output k
	pop_ac
	decr_a
	jmpp nloop
	stop
section data
k:	space 1
r:	const 2
z:	const -4
i:	const 10
