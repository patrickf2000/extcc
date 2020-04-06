#include <stdio.h>

int main() {
	for (int i = 0; i<10; i++) {
		printf("I: %d\n", i);
		
		if (i == 5) {
			puts("Halfway!");
		}
	}

	return 0;
}
