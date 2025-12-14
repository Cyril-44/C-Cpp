#include <stdio.h>
#include <string.h>
#include <iostream>
#include <set>
using std::min;
using std::max;
const int N = 100005;
struct Edge {
    int to, nxt;
} e[N << 1];
struct SortedEdge {
    int a, b;
    inline SortedEdge(const int &x=0, const int &y=0) :
        a(min(x, y)), b(max(x, y)) {}
    inline bool operator<(const SortedEdge &oth) const {
        return a < oth.a || a == oth.a && b < oth.b;
    }
};
std::set<SortedEdge> all;
int head[N], sta[N], k, ans[N<<2][3];
inline void addedg(const int &fr, const int &to) {
    e[++head[0]] = Edge{to, head[fr]};
    head[fr] = head[0];
}
bool dfn[N];
inline void tarj(const int &u) {
    dfn[u] = true;
    sta[++sta[0]] = u;
    for (int i = head[u], v; i; i = e[i].nxt) {
        v = e[i].to;
        if (!dfn[v]) {
            tarj(v);
        }
        else {
            for (int i = sta[0]; i > 1 && (sta[i] ^ v); i--) {
                all.erase(SortedEdge(sta[i - 1], sta[i]));
                ++k;
                ans[k][0] = v;
                ans[k][1] = sta[i - 1];
                ans[k][2] = sta[i];
            }
        }
    }
}
int main() {
    int t, n, m, ui, vi;
    scanf("%d", &t);
    while (t--) {
        scanf("%d%d", &n, &m);
        all.clear();
        while (m--) {
            scanf("%d%d", &ui, &vi);
            addedg(ui, vi);
            addedg(vi, ui);
            all.insert(SortedEdge(ui, vi));
        }
        memset(dfn, 0, n+1);
        for (int i = 1; i <= n; i++)
            if (!dfn[i]) tarj(i);
    }
    return 0;
}