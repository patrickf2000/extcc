#include <stdio.h>

struct coord {
	int x;
	int y;
};

int main() {
	struct coord square;
	square.x = 100;
	square.y = 30;
	
	int c = square.x;
	printf("C: %d\n", c);
	printf("c->Y: %d\n", square.y);
	
	return 0;
}
