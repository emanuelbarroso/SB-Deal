section text
c_input x
c_input x+1
copy y,x+2
copy x,x+3
s_output x
s_input z
c_output z+1
stop
section data
x: space 4
y: const 0x41
z: space 8
