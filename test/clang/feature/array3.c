#include <stdio.h>
#include <stdlib.h>

void print_array(int *numbers, int size) {
	printf("[");
	for (int i = 0; i<size; i++) {
		printf("%d ", numbers[i]);
	}
	puts("]");
}

int main() {
	int *numbers = malloc(20);
		
	for (int i = 0; i<5; i++) {
		numbers[i] = i * 10;
	}
	
	print_array(numbers, 5);
	
	for (int i = 1; i<=5; i++) {
		numbers[i] = i + 8;
	}
	
	print_array(numbers, 5);
	
	free(numbers);
	return 0;
}
