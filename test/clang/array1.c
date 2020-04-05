#include <stdio.h>

void int_array() {
	int numbers[5] = {1, 2, 3, 4, 5};
	
	for (int i = 0; i<5; i = i + 1) {
		int x = numbers[i];
		printf("%d ", x);
	}
	puts("");
}

void float_array() {
	float numbers[5] = {1.1, 2.2, 3.3, 4.4, 5.5};
	
	for (int i = 0; i<5; i = i + 1) {
		float x = numbers[i];
		printf("%f ", x);
	}
	puts("");
}

int main() {
	int_array();

	return 0;
}
