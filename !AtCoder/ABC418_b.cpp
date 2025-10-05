#include <cstdio>
#include <algorithm>
#include <cstring>
constexpr int N = 105;
char s[N];
int t[N];
int main() {
    scanf("%s", s+1);
    int n = strlen(s+1);
    for (int i = 1; i <= n; i++)
        t[i] = t[i-1] + (s[i] == 't');
    long double ans = 0.;
    for (int i = 1; i <= n; i++)
        for (int j = i + 2; j <= n; j++)
            if (s[i] == 't' && s[j] == 't')
                ans = std::max(ans, (long double)(t[j] - t[i-1] - 2) / (j - i - 1));
    printf("%.15Lf", ans);
    return 0;
}