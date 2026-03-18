#include <cstdio>
#include <cstring>
#include <bitset>
constexpr int N = 100005, M = 128;
std::bitset<N> ok[M];
char s[N], t[N];
char ns[N], nt[N];
char ask[N];
int main() {
    scanf("%s %s", s+1, t+1);
    int n = strlen(s+1), m = strlen(t+1), q;
    for (signed char c1 = 'a'; c1 <= 'z'; c1++)
        for (signed char c2 = c1; c2 <= 'z'; c2++) {
            int tots = 0, tott = 0;
            for (int i = 1; i <= n; i++)
                if (s[i] == c1 || s[i] == c2) ns[tots++] = s[i];
            for (int i = 1; i <= m; i++)
                if (t[i] == c1 || t[i] == c2) nt[tott++] = t[i];
            ok[c1][c2] = ok[c2][c1] = tots == tott && !memcmp(ns, nt, tots);
        }
    scanf("%d", &q);
    while (q--) {
        scanf(" %s", ask);
        bool flg = true;
        for (int i = 0; ask[i]; i++)
            for (int j = i; ask[j]; j++)
                if (!ok[ask[i]][ask[j]]) { flg = false; break; }
        putchar(flg ? 'Y' : 'N');
    }
    putchar('\n');
    return 0;
}