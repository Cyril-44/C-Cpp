#include <stdio.h>
inline int getsum(int n) {
    int sum = n % 10;
    while (n /= 10) sum += n % 10;
    return sum; 
}
int main() {
    int n, k;
    scanf("%d%d", &n, &k);
    int cnt = 0;
    for (int i = 1; i <= n; i++)
        cnt += getsum(i) == k;
    printf("%d\n", cnt);
    return 0;
}