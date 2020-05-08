section data
	string str1 "Hello!"
	string fmt "%d\n"

section code
	extern void puts string
	extern void printf string any

func main
	pusharg string str1
	call puts
	
	var no1 int 10
	
	pusharg string fmt
	pusharg var no1
	call printf
	
	ret
