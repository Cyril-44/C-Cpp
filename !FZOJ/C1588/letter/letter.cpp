#include <cstdio>
#include <cstring>
#define For(i, s, t) for(int i = (s); i <= (t); i++)
#define roF(i, s, t) for(int i = (s); i >= (t); i--)
constexpr int N = 200005, S = 26;
char s[N], t[N];
int beg[256], end[256];
bool intr[N];
int main() {
    int Tid, T;
    scanf("%d%d", &Tid, &T);
    while (T--) {
        int n, m;
        scanf("%d%d %s %s", &n, &m, s+1, t+1);
        For(i, 1, n) s[i] -= 'a';
        For(i, 1, m) t[i] -= 'a';
        memset(beg, 0, sizeof beg);
        memset(end, 0, sizeof end);
        memset(intr, 0, sizeof intr);
        For(i, 1, m) if (!beg[s[i]]) beg[t[i]] = i, intr[i] = true;
        roF(i, m, 1) if (!end[s[i]]) end[t[i]] = i, intr[i+1] = true;
        
        
    }
    return 0;
}
