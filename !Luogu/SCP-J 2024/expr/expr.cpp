#include <stdio.h>
#include <algorithm>
using std::max;
using std::min;
const int N = 100005;
char s[N * 20];
int top, g[N];
struct Range {
    bool gt, lt; // >=1:g 0:-oo <=1:l 0:oo
    int g, l, x; // (g, l) -> x
    inline bool operator<(const Range &x) const {
        return g < x.g || g == x.g && l < x.l;
    }
} fin[N];
inline void get(int &x, int &p) {
    for (x = 0; s[p] >= '0' && s[p] <= '9'; ++p)
        x = (x << 3) + (x << 1) + (s[p] ^ '0');
}
inline void dfs(int &p, Range cur={}) {
    if (s[p] ^ 'x') {
        int x;
        get(x, p);
        if (!cur.lt || !cur.gt || cur.g+1 <= cur.l-1) {   // 合法区间判断
            cur.x = x;
            fin[++top] = cur;
        }
    }
    else {
        char c = s[++p];    // x[>]12
        int n;              // x>[12]
        get(n, ++p);
        ++p;                // x>12[?]

        Range tmp;
        const auto updLt = [&tmp](const int &t) -> void {
            if (!tmp.lt) tmp.l = t, tmp.lt = true;
            else tmp.l = min(tmp.l, t);
        };
        const auto updGt = [&tmp](const int &t) -> void {
            if (!tmp.gt) tmp.g = t, tmp.gt = true;
            else tmp.g = max(tmp.g, t);
        };

        tmp = cur;
        switch (c) {
        case '>':   // >n -> x
            updGt(n); break;
        case '<':   // <n -> x
            updLt(n); break;
        }

        dfs(p, tmp); // x>12?[x]>

        ++p;                // x>12?2[:]4

        tmp = cur;
        switch (c) {
        case '>':   // <n+1 -> x
            updLt(n+1); break;
        case '<':   // >n-1 -> x
            updGt(n-1); break;
        }

        dfs(p, tmp);
    }
}
int main() {
#ifndef ONLINE_JUDGE
    freopen("expr.in", "r", stdin);
    freopen("expr.out", "w", stdout);
#endif
    int m, q, p, x;
    scanf("%d%d %s", &m, &q, s);
    p = 0; dfs(p);
    std::sort(fin+1, fin+1 + top);
    for (int i = 1; i <= top; i++)
        g[i] = fin[i].g;
    while (q--) {
        scanf("%d", &x);
        printf("%d\n", fin[(
                std::lower_bound(g+1, g+1 + top, x) - g - 1)
            ].x);
    }
    return 0;
}