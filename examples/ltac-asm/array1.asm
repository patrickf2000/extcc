section data
	string fmt "%d\n"

section code

func main
	var x int 0
	var i int 0

	pusharg int 32
	call malloc
	
	ptr numbers1 int
	str.ret numbers1
	
	pusharg int 32
	call malloc
	
	ptr numbers2 int
	str.ret numbers2
	
	arrayset numbers1 var i int 10
	var i int 1
	arrayset numbers1 var i int 20
	var i int 2
	arrayset numbers1 var i int 30
	var i int 3
	arrayset numbers1 var i int 40
	var i int 4
	arrayset numbers1 var i int 50
	var i int 5
	arrayset numbers1 var i int 60
	var i int 6
	arrayset numbers1 var i int 70
	var i int 7
	arrayset numbers1 var i int 80
	
	var i int 0
	arrayset numbers2 var i int 1
	var i int 1
	arrayset numbers2 var i int 2
	var i int 2
	arrayset numbers2 var i int 3
	var i int 3
	arrayset numbers2 var i int 4
	var i int 4
	arrayset numbers2 var i int 5
	var i int 5
	arrayset numbers2 var i int 6
	var i int 6
	arrayset numbers2 var i int 7
	var i int 7
	arrayset numbers2 var i int 8
	
	arrayacc numbers1 int 2 x
	
	pusharg string fmt
	pusharg var x
	call printf
		
	ret
