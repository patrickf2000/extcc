#include <stdio.h>

int main() {
	for (int i = 0; i<10; i = i + 1) {
		printf("I: %d\n", i);
		
		if (i == 5) {
			puts("Halfway!");
		}
	}

	return 0;
}
