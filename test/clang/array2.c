#include <stdio.h>
#include <stdlib.h>

int main() {
	int *numbers = malloc(20);
		
	for (int i = 0; i<5; i = i + 1) {
		numbers[i] = i * 10;
	}
	
	for (int i = 0; i<5; i = i + 1) {
		int x = numbers[i];
		printf("%d\n", x);
	}
	
	return 0;
}
