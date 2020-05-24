@str1 = internal constant [13 x i8] c"Hello world!\00"

declare void @puts(i8*)

define i32 @main() {
	%1 = getelementptr [13 x i8], [13 x i8]* @str1, i32 0, i32 0
	call void(i8*) @puts(i8* %1) nounwind

	ret i32 0
}

