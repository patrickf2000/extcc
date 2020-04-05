#include <stdio.h>

int square(int x) {
	int answer = x * x;
	return answer;
}

int main() {
	puts("Squaring each number 1 to 10.");

	for (int i = 1; i<=10; i = i + 1) {
		int s = square(i);
		int s2 = square(i) + 10;
		
		printf("I %d | Squared: %d | Squared + 10: %d\n", i, s, s2);
	}

	return 0;
}
