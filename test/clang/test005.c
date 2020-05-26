#include <stdio.h>

void print_char(char c) {
	printf("Your char: %c\n", c);
}

int main() {
	char c = 'A';
	printf("%c\n", c);
	
	printf("%c\n", 'B');
	
	puts("");
	
	print_char(c);
	print_char('T');
	
	return 0;
}
