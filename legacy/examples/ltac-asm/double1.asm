section data
	string fmt "%f\n"

section code

func main
	var n1 double 1.1
	var n2 double 2.3
	var answer double 0

	f64.ldr 1 n1
	f64.ldr 2 n2
	f64.add reg 1 reg 2
	f64.str 1 answer

	pusharg string fmt
	pusharg var answer
	call printf

	ret
