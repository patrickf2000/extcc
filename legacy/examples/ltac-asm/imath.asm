section data
	string str1 "First number: %d\n"
	string str2 "Second number: %d\n"
	string nl "\n"
	string add_msg "Result (add): %d\n"
	string sub_msg "Result (sub): %d\n"
	string mul_msg "Result (mul): %d\n"
	string div_msg "Result (div): %d\n"
	string mod_msg "Result (mod): %d\n"

section code

func main
	var no1 int 20
	var no2 int 5
	var answer int 0
	
	pusharg string str1
	pusharg var no1
	call printf
	
	pusharg string str2
	pusharg var no2
	call printf
	
	pusharg string nl
	call puts
	
	ldr 1 no1
	ldr 2 no2
	
	iadd reg 1 reg 2
	str 1 answer
	
	pusharg string add_msg
	pusharg var answer
	call printf
	
	ldr 1 no1
	ldr 2 no2
	
	imul reg 1 reg 2
	str 1 answer
	
	pusharg string mul_msg
	pusharg var answer
	call printf
	
	ret
