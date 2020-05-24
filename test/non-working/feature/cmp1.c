#include <stdio.h>

void test1(int x, int y) {
	if (x == y) {
		puts("Equal!");
	} else {
		puts("Not equal");
	}
}

void test2(int x) {
	if (x > 10) {
		puts("Greater than 10!");
	} else if (x < 10) {
		puts("Less than 10!");
	} else {
		puts("Equal to 10?");
	}
}

void test3(int x) {
	if (x >= 10) {
		puts("Greater or equal to 10");
	} else {
		puts("Less than 10");
	}
}

void test4(int x) {
	if (x <= 10) {
		puts("Less than or equal to 10");
	} else {
		puts("Greater than 10!");
	}
}

int main() {
	int x = 10;
	int y = 20;
	
	test1(x, y);
	test1(x, 10);
	
	test2(x);
	test2(1);
	test2(y);
	
	test3(10);
	test3(11);
	test3(1);
	
	test4(10);
	test4(1);
	test4(1000);

	return 0;
}
