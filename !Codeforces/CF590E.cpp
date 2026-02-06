#include <stdio.h>
constexpr int N = 755, M = (int)1e7 + 5;
char str[M];
bool g[N][N]; // Substr Order Graph
namespace ACAM {
struct Node {
    int son[2], fa, fail, id;
} ac[M];
int fa[M];
int tot = 0;
inline int newnode(int fa) {
    ac[++tot].fa = fa;
    return ++tot;
}
inline int insert(char *s, int id) {
    int u = 0;
    for (; *s; ++s) {
        int &v = ac[u].son[*s - 'a'];
        if (!v) v = newnode(u);
        u = v;
    }
    ac[u].id = id;
    return u;
}
inline void build() {
    static int que[M];
    int head = 0, tail = 0;
    que[tail++] = 0;
    while (head < tail) {
        int u = que[head++];
        for (int i = 0; i < 2; i++)
            if (ac[u].son[i]) { // u->son_i->fail = u->fail->son_i
                ac[ac[u].son[i]].fail = ac[ac[u].fail].son[i];
                que[tail++] = ac[u].son[i];
            } else { // u->son_i = u->fail->son_i
                ac[u].son[i] = ac[ac[u].fail].son[i];
            }
    }
    for (int i = 1; i <= tot; i++)
        fa[i] = ac[i].id ? i : ac[i].fail;
}
inline int find(int x) { return (x ^ fa[x]) ? (fa[x] = find(fa[x])) : x; }
}
int idx[N];
int main() {
    int n;
    scanf("%d", &n);
    for (int i = 1; i <= n; i++) {
        scanf("%s", str);
        idx[i] = ACAM::insert(str, i);
    }
    ACAM::build();
    for (int i = 1; i <= n; i++) {
        int u = idx[i];
        for (int u = idx[i]; u; u = ACAM::ac[u].fa) {
            int j = ACAM::find(u);
            if (j) g[ACAM::ac[j].id][i] = 1; // j \in i
        }
    }
    return 0;
}