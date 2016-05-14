section text
	load zero
	store m1
	output m1
	incr_m m1
	load m1
	incr_a
	store m2
	output m2
	load three
	decr_a
	store m1
	output m1
	stop
section data
m1:	space
m2: space
zero:	const 0
three: const 3
