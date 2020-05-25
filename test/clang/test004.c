#include <stdio.h>

void println(char *s) {
	puts("STR:");
	puts(s);
}

int main() {
	char *s = "Hello!";
	puts(s);
	
	char *s2 = "How are you?";
	println(s2);
	
	return 0;
}
