section data
	string str1 "Hello world!"

section code
	extern void puts string

func main
	pusharg string str1
	call puts
	
	ret
