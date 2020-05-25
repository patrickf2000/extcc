#include <stdio.h>

int main() {
	puts("You should see 10 stars, even though the loop goes up to 20.");

	for (int i = 0; i<10; i++) {
		printf("I: %d\n", i);
		
		for (int j = 0; j<3; j++) {
			printf("J-Inner ");
			
			for (int k = 0; k<20; k = k + 2) {
				printf("* ");
			}
			
			puts("");
		}
		puts("!");
	}

	return 0;
}
