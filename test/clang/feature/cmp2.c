#include <stdio.h>

void test(int x) {
	if (x > 10) {
		if (x > 20) {
			if (x >= 100) {
				puts("X is >= 100.");
			} else {
				puts("X is > 20 but < 100");
			}
		} else {
			puts("X is > 10 but < 20");
		}
	} else {
		puts("X < 10.");
	}
}

int main() {
	test(1);
	test(10);
	test(11);
	test(21);
	test(100);
	test(121);

	return 0;
}
