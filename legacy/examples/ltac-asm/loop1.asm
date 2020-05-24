section data
	string fmt "%d\n"

section code

func main
	var x int 0
	
lbl loop
	pusharg string fmt
	pusharg var x
	call printf
	
	iadd var x int 1
lbl end
	icmp var x int 10
	jl loop
		
	ret
