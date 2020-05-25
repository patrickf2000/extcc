#include <stdio.h>

int main() {
	int x = 0;
	
	while (x < 10) {
		printf("X: %d\n", x);
		
		if (x == 5) {
			puts("Halfway!");
		}
		
		x++;
	}

	return 0;
}
