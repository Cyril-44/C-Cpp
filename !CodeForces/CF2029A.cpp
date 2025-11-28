#include <stdio.h>
#include <iostream>
using std::max;
int main() {
    int t, l, r, k;
    scanf("%d", &t);
    while (t--) {
        scanf("%d%d%d", &l, &r, &k);
        printf("%d\n", max(0, r/k - l + 1));
    }
    return 0;
}