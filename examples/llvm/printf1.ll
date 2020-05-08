@str1 = internal constant [7 x i8] c"Hello!\00"
@fmt = internal constant [4 x i8] c"%d\0A\00"

declare void @puts(i8*)
declare void @printf(i8*,...)

define i32 @main() {
	%1 = getelementptr [7 x i8], [7 x i8]* @str1, i32 0, i32 0
	call void(i8*) @puts(i8* %1) nounwind

	%2 = alloca i32, align 4
	store i32 10, i32* %2, align 4

	%3 = getelementptr [4 x i8], [4 x i8]* @fmt, i32 0, i32 0
	%4 = load i32, i32* %2, align 4
	call void(i8*,...) @printf(i8* %3, i32 %4) nounwind

	ret i32 0
}

