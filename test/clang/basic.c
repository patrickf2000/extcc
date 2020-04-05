#include <stdio.h>

void sayHello() {
	puts("Hello!");
}

int goUp(int x) {
	int answer = x * 10;
	return answer;
}

int main() {
	sayHello();
	sayHello();
	puts("");
	
	int x = 10;
	int y = 20;
	int z = x + y;
	
	printf("X: %d | Y: %d | Z: %d\n", x, y, z);
	
	z = 7 * z;
	printf("Z * 7= %d\n", z);
	
	puts("");
	
	x = goUp(x);
	y = goUp(y);
	
	printf("X: %d | Y: %d\n", x, y);
}
