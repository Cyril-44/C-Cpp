#include <cstdio>
#include <cstring>
constexpr int N = 1000005;
char s1[N], s2[N];
int fail1[N], fail2[N];
int main() {
    scanf("%s %s", s1+1, s2+1);
    int n = strlen(s1+1), m = strlen(s2+1);
    for (int i = 2, j = 0; i <= m; i++) {
        while (j && s2[j+1] != s2[i]) j = fail2[j];
        if (s2[j+1] == s2[i]) ++j;
        fail2[i] = j;
    }
    for (int i = 1, j = fail2[n]; i <= n; i++) {
        while (j == n || j && s2[j+1] != s1[i]) j = fail2[j];
        if (s2[j+1] == s1[i]) ++j;
        fail1[i] = j;
    }
    for (int i = 1; i <= n; i++)
        if (fail1[i] == m) printf("%d\n", i - m + 1);
    for (int i = 1; i <= m; i++)
        printf("%d ", fail2[i]);
    putchar('\n');
    return 0;
}