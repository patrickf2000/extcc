section data
	float no1 1.1
	float no2 2.2
	
	string fmt "%f\n"
	string fmt2 "%d\n"

section code

func main
	var n1 float no1
	var n2 int 10

	pusharg string fmt
	pusharg var n1
	call printf
	
	pusharg string fmt2
	pusharg var n2
	call printf

	ret
