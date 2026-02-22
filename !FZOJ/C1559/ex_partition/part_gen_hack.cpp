#include <stdio.h>
#include <stdlib.h>
#include <time.h>
int main() {
    srand(time(NULL));
    puts("1");
    for (int i = 1; i <= 100000; i++)
        // putchar('1');
        putchar(rand() & 1 | '0');
    return 0;
}