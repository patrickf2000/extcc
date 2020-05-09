section data
	string fmt "%f\n"

section code

func main
	var x float 0.0
	var x1 float 100.0
	var x2 float 10.0
	var i int 0
	
	pusharg int 32
	call malloc
	
	ptr numbers1 float
	str.ret numbers1
	
	pusharg int 32
	call malloc
	
	ptr numbers2 float
	str.ret numbers2
	
lbl loop
	arrayset numbers1 var i var x1
	arrayset numbers2 var i var x2
	
	f32.add var x1 float 100.0
	f32.add var x2 float 10.0
	iadd var i int 1
lbl end
	icmp var i int 8
	jl loop
	
	vldr 1 numbers1
	vldr 2 numbers2
	
	f32.vadd reg 1 reg 2
	
	vstr 1 numbers1
	
	var i int 0
	
lbl print_loop
	arrayacc numbers1 var i x
	
	pusharg string fmt
	pusharg var x
	call printf
	
	iadd var i int 1
	
	icmp var i int 8
	jl print_loop
		
	ret
