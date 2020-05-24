#include <stdio.h>
#include <stdlib.h>

int main() {
	int *numbers = malloc(20);
		
	for (int i = 0; i<5; i++) {
		numbers[i] = i * 10;
	}
	
	for (int i = 0; i<5; i++) {
		printf("%d\n", numbers[i]);
	}
	
	return 0;
}
