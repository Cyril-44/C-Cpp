#include <stdio.h>
int h[10];
int main() {
	int ch;
	while ((ch = getchar()) != EOF)
		++h[ch ^ '0'];
	for (int i = 9; i >= 0; i--)
		while (h[i]--) putchar(i ^ '0');
	putchar('\n');
	return 0;
}