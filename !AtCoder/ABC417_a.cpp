#include <cstdio>
#include <algorithm>
#include <cstring>
const int N = 25;
char s[N];
int main() {
    int n, a, b;
    scanf("%d%d%d", &n, &a, &b);
    scanf("%s", s);
    s[n-b] = '\0';
    puts(s + a);
    return 0;
}