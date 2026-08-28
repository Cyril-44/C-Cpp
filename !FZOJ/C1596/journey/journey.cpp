#include <cstdio>
#include <cstring>
#include <vector>
#ifdef CLANGD
constexpr int N = 14;
#else
constexpr int N = 1000004;
#endif
char s[N];
// f(i, j) = sum floor(sz_u / i)
struct Node {
    int sz, ch[26];
    int& operator[](char c) { return ch[c - 'a']; }
} tr[N];
int f[N];
int top;
std::vector<int> facs[N];
int main() {
    int taskid, n;
    scanf("%d%d", &taskid, &n);
    for (int i = 1; i <= n; i++)
        for (int j = i; j <= n; j += i)
            facs[j].push_back(i);
    int ans = 0;
    for (int i = 1; i <= n; i++) {
        scanf("%s", s + 1);
        int u = 0;
        for (int j = 1; s[j]; j++) {
            if (!tr[u][s[j]]) tr[u][s[j]] = ++top;
            u = tr[u][s[j]];
            ++tr[u].sz;
            for (int fac : facs[tr[u].sz]) {
                ans ^= f[fac] * fac;
                ++f[fac];
                ans ^= f[fac] * fac;
            }
        }
        printf("%d\n", ans);
    }
    return 0;
}