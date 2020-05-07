section data
	float no1 10.19
	float no2 2.2
	float answer 0.0
	
	string fmt "%f\n"

section code

func main
	var n1 float no1
	var n2 float no2
	var result float answer
	
	fldr 1 n1
	fldr 2 n2
	
	f32.add reg 1 reg 2
	
	fstr 1 result
	
	pusharg string fmt
	pusharg var result
	call printf

	ret
