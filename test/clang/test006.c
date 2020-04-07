#include <stdio.h>

struct coord {
	int x;
	int y;
};

struct coord build_square() {
	struct coord square;
	square.x = 100;
	square.y = 30;
	return square;
}

int main() {
	struct coord square;
	square = build_square();
	
	int c = square.x;
	printf("C: %d\n", c);
	printf("c->Y: %d\n", square.y);
	
	return 0;
}
