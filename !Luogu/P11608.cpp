#include <stdio.h>
#include <vector>
#include <array>
#include <algorithm>
constexpr int N = 1000005;
struct Node {
    int k, b;
    inline bool operator< (const Node &rhs) const {
        return k < rhs.k;
    }
} a[N];

namespace Splay {
enum {L, R, FA};
struct Node {
    int s[3];
    inline int& operator[](int x) { return s[x]; }
    Node& operator()(int);
};
std::array<Node, N> tr;
inline Node& Node::operator()(int x) { return tr[s[x]]; }
inline bool type(int u) { return tr[u](FA)[R] == u; }
inline void rotate(int u) {
    pushdown(u[FA]);
    pushdown(u);
    int anc = tr[u](FA)[FA];
    bool tp = type(u);
    tr[anc][type(tr[u][FA])] = u;
    tr[u](FA)[tp] = tr[u][!tp];
    if (tr[u][!tp]) tr[u](!tp)[FA] = tr[u][FA];
    tr[u](FA)[FA] = u, tr[u][!tp] = tr[u][FA], tr[u][FA] = anc;
}
};

int main() {
    int n;
    scanf("%d", &n);
    for (int i = 1; i <= n; i++)
        scanf("%d%d", &a[i].k, &a[i].b);
    std::sort(a+1, a+1 + n);
/*
  f[i][j] = min(f[i-1][j], f[i-1][j-1] + (j-1) * a[i].k + a[i].b);
  g[i][j] = f[i][j] - f[i][j-1]
  每次一定是在已知最优集合中加一个当前天最优的，所以一定存在一个单调性
  g[i][j] = (j < k) g[i-1][j]
            (j = k) (j-1) * a[i].k + a[i].b
            (j > k) g[i-1][j-1] + a[i].k
*/
    for (int i = 1; i <= n; i++) {
        
    }
    return 0;
}