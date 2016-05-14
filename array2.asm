section text
	input num
	load num
	jmpg maxsz,noprog
	zero_a
in1:
	jmpe num,endin
	store index
	input temp
	push_ac
	load temp
	store_r array,index
	pop_ac
	incr_a
	jmp in1
endin:
	zero_a
out1:
	jmpe num,endout
	store index
	push_ac
	load_r array,index
	store temp
	pop_ac
	output temp
	incr_a
	jmp out1
endout:
	zero_a
lvl1:
	jmpe num,endlvl
	store index
	push_ac
	load_r array,index
	store temp
	copy num,ntemp
	level temp,ntemp
	output temp
	pop_ac
	incr_a
	jmp lvl1
endlvl:
	copy maxsz,ntemp
	copy num,temp
	level temp,ntemp
	h_output temp
noprog:
	stop
section data
array:	space 15
num:	space
temp:	space
ntemp:	space
index:	space
maxsz:	const 15
