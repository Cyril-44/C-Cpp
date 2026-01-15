#include <stdio.h>
#include <algorithm>
#include <vector>
#include <array>
#include <cassert>
constexpr int N = 200005;
int ch[N][2], orifa[N];
int fa[N], light[N];
long long lightsum = 0, ans[N];

int n, op[N], dfnts = 0, dfn[N], lst[N];
bool tobeadded[N], type[N];

void dfs(int u) {
    dfn[u] = ++dfnts;
    if (ch[u][0]) dfs(ch[u][0]);
    if (ch[u][1]) dfs(ch[u][1]);
    lst[u] = dfnts;
}

namespace PSGT {
struct Node {
    int ls, rs;
    std::pair<int,int> mx;
    Node() : ls(), rs(), mx() {}
};
std::array<Node, N << 5> tr;
int root[N], tot = 0;
int P, X;
void upd(int &u, int l, int r) {
    tr[++tot] = tr[u], u = tot;
    if (l == r) return tr[u].mx = {X, P}, void();
    int mid = l + r >> 1;
    if (P <= mid) upd(tr[u].ls, l, mid);
    else upd(tr[u].rs, mid + 1, r);
    tr[u].mx = std::max(tr[tr[u].ls].mx, tr[tr[u].rs].mx);
}
inline void insert(int ts, int dfn) {
    P = dfn, X = ts;
    upd(root[ts] = root[ts - 1], 1, n);
}
int L, R;
std::pair<int,int> que(int u, int l, int r) {
    if (!u) return {0, 0};
    if (L <= l && r <= R) return tr[u].mx;
    int mid = l + r >> 1;
    std::pair<int,int> ret(0, 0);
    if (L <= mid) ret = std::max(ret, que(tr[u].ls, l, mid));
    if (mid < R) ret = std::max(ret, que(tr[u].rs, mid + 1, r));
    return ret;
}
inline int query(int ts, int l, int r) { // 找 ts 之前 dfn 在 l~r 中的最后加入的节点的 dfn
    L = l, R = r;
    return que(root[ts - 1], 1, n).second;
}
}

/*
  用 Splay 动态维护重链，因为轻边只有 log 条
  Splay 的下标就是对应节点，省一个 struct member
*/
namespace Splay {
enum {L, R, FA};
struct Node {
    int s[3], size, add;
    inline int& operator[](unsigned c) { return s[c]; }
    Node& operator()(unsigned);
    inline void pull(int x) { add += x, size += x; }
} tr[N];
inline Node& Node::operator()(unsigned c) { return tr[s[c]]; }
inline bool type(int u) { return tr[u](FA)[R] == u; }
inline void pushdown(int u) {
    if (!tr[u].add) return;
    if (tr[u][L]) tr[u](L).pull(tr[u].add);
    if (tr[u][R]) tr[u](R).pull(tr[u].add);
    tr[u].add = 0;
}
inline void rotate(int u) {
    int fa = tr[u][FA], anc = tr[u](FA)[FA]; bool tp = type(u);
    pushdown(fa), pushdown(u);
    if (anc) tr[anc][type(tr[u][FA])] = u;
    tr[fa][tp] = tr[u][!tp]; if (tr[u][!tp]) tr[u](!tp)[FA] = fa;
    tr[u][!tp] = tr[u][FA], tr[fa][FA] = u, tr[u][FA] = anc;
}
inline void splay(int u, int goal = 0) {
    while (tr[u][FA] ^ goal) {
        if (tr[u](FA)[FA] ^ goal)
            rotate(type(u) == type(tr[u][FA]) ? tr[u][FA] : u);
        rotate(u);
    }
}
void dfs(int u) {
    tr[u].size = 1;
    if (ch[u][0]) dfs(ch[u][0]), tr[u].size += tr[ch[u][0]].size;
    if (ch[u][1]) dfs(ch[u][1]), tr[u].size += tr[ch[u][1]].size;
    if (tr[ch[u][0]].size > tr[ch[u][1]].size)
        tr[u][R] = ch[u][0], fa[ch[u][1]] = u, lightsum += light[u] = ch[u][1];
    else tr[u][R] = ch[u][1], fa[ch[u][0]] = u, lightsum += light[u] = ch[u][0];
}
inline int getfront(int u) {
    while (tr[u][L]) pushdown(u), u = tr[u][L];
    splay(u);
    return u;
}
inline void add(int u, int ts) {
    tr[u].size = 1;
    fa[u] = orifa[u];
    assert(light[fa[u]] == 0);
    lightsum += light[fa[u]] = u;
    int last = u;
    for (u = fa[u]; u; u = fa[last = getfront(u)]) {
        splay(u);
        ++tr[u].size;
        if (tr[u][L]) tr[u](L).pull(1); // 给 u 到 top 整体 size += 1
        bool check = tr[last].size * 2 >= tr[u].size;
        // printf("[INFO] checking %d->%d sz(%d->%d)\n", u, last, tr[u].size, tr[last].size);
        if (!check && tr[u].size - 1 == tr[last].size * 2) {
            // printf("[WARN] Entering key judgement: szl = szr = %d\n", tr[last].size);
            int firstop = PSGT::query(ts, dfn[u], lst[u]);
            // printf("[WARN] Queried first add dfn(%d) after time %d\n", firstop, ts);
            // printf("[WARN] Last type: %c, dfn[%d,%d]\n", ::type[last]?'R':'L', dfn[last], lst[last]);
            if (firstop == 0) check |= !::type[last];
            else check |= dfn[last] <= firstop && firstop <= lst[last];
        }
        if (check) {
            int rfront = tr[u][R];
            if (rfront) {
                while (tr[rfront][L]) rfront = tr[rfront][L];
                splay(rfront, u);
            }
            // printf("[FAIL] Found incompatible %d->%d! concated %d->%d, light changed from %d to %d\n", u, rfront, u, last, light[u], rfront);
            fa[rfront] = u;
            lightsum -= light[u];
            lightsum += light[u] = rfront;
            tr[u](R)[FA] = 0, tr[u][R] = last, tr[last][FA] = u;
        }
    }
}
}

int main() {
    int q;
    scanf("%d", &n);
    for (int i = 1; i <= n; i++) {
        scanf("%d%d", &ch[i][0], &ch[i][1]);
        orifa[ch[i][0]] = orifa[ch[i][1]] = i;
        type[ch[i][0]] = 0, type[ch[i][1]] = 1;
    }
    long long sum = (1ll + n) * n / 2;
    int root = 0;
    for (int i = 1; i <= n; i++)
        if (!orifa[i]) root = i;
    dfs(root);
    sum -= root;
    scanf("%d", &q);
    for (int i = 1; i <= q; i++) {
        scanf("%d", &op[i]);
        PSGT::insert(i, dfn[op[i]]);
        tobeadded[op[i]] = true;
        sum -= op[i];
    }
    for (int i = 1; i <= n; i++) {
        if (tobeadded[ch[i][0]]) ch[i][0] = 0;
        if (tobeadded[ch[i][1]]) ch[i][1] = 0;
    }
    Splay::dfs(root); fa[0] = 0;
    for (int i = q; i >= 1; i--) {
        /* printf("[ ans %d ] ============\n", i);
        for (int j = 1; j <= n; j++)
            if (light[j]) printf("%d->%d ", j, light[j]);
        putchar('\n'); */
        ans[i] = sum - lightsum;
        // printf("[DEBUG] calced %lld - %lld = %lld\n", sum, lightsum, ans[i]);
        sum += op[i];
        Splay::add(op[i], i);
    }
    ans[0] = sum - lightsum;
    for (int i = 0; i <= q; i++)
        printf("%lld\n", ans[i]);
    return 0;
}
