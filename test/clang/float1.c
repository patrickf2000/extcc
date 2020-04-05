#include <stdio.h>

void test1() {
	float x = 3.14;
	printf("X: %f\n", x);
	
	float y = x;
	printf("Y: %f\n",  y);
}

void test2() {
	double x = 3.14;
	printf("X (db): %f\n", x);
	
	double y = x;
	printf("Y (db): %f\n",  y);
}

int main() {
	test1();
	test2();

	return 0;
}
