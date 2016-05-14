section text
	input k
	zero_m count
loopthis:
	load count
	jmpge mcount,endthisloop
	load k
	store_r array,count
	incr_m k
	incr_m count
	jmp loopthis
endthisloop:
	zero_m count
loprint:
	load count
	jmpge mcount,endprog
	load_r array,count
	store m
	output m
	incr_m count
	jmp loprint
endprog:
	stop
	
section data
array:	space 15
k:		space
m:		space
count:	space
mcount: const 15
