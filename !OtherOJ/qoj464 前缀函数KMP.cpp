#include <cstdio>
#include <cstring>
#include <algorithm>
constexpr int N = 2000005;
char s[N];
int fail[N];
int main() {
    scanf("%s", s+1);
    int n = strlen(s+1);
    for (int i = 2, j = 0; i <= n; i++) {
        while (j && s[j+1] != s[i]) j = fail[j];
        if (s[j+1] == s[i]) ++j;
        fail[i] = j;
    }
    for (int i = 1; i <= n; i++)
        printf("%d ", fail[i]);
    putchar('\n');
    return 0;
}