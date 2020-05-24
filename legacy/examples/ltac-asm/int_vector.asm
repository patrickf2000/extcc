section data
	string fmt "%d\n"

section code

func main
	var x int 0
	var x1 int 100
	var x2 int 10
	var i int 0
	
	pusharg int 32
	call malloc
	
	ptr numbers1 int
	str.ret numbers1
	
	pusharg int 32
	call malloc
	
	ptr numbers2 int
	str.ret numbers2
	
lbl loop
	arrayset numbers1 var i var x1
	arrayset numbers2 var i var x2
	
	iadd var x1 int 100
	iadd var x2 int 10
	iadd var i int 1
lbl end
	icmp var i int 8
	jl loop
	
	vldr 1 numbers1
	vldr 2 numbers2
	
	visub reg 1 reg 2
	
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
