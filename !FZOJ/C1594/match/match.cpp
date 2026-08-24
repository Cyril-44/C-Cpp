#include <cstdio>
#include <cstring>
#include <algorithm>
#include <random>
constexpr int N = 100004;
std::mt19937 rng(std::random_device{}());
struct Treap {
    struct Node {
        int ls, rs, sz;
        unsigned weight;
        bool val;
    } tr[N];
    int tot, rt;
    void pushup(int u) { tr[u].sz = tr[tr[u].ls].sz + 1 + tr[tr[u].rs].sz; }
    int alloc(bool val = 0) { return tr[++tot] = {0, 0, 1, (unsigned)rng(), val}, tot; }
    int merge(int u, int v) {
        if (!u || !v) return u | v;
        if (tr[u].weight < tr[v].weight)
            return tr[u].rs = merge(tr[u].rs, v), pushup(u), u;
        else
            return tr[v].ls = merge(u, tr[v].ls), pushup(v), v;
    }
    std::pair<int,int> split(int u, int s) {
        if (s == 0) return {0, u};
        if (tr[u].sz == s) return {u, 0};
        if (s <= tr[tr[u].ls].sz) {
            auto [ll, lr] = split(tr[u].ls, s);
            tr[u].ls = lr; return {ll, u};
        }
        auto [rl, rr] = split(tr[u].rs, s - tr[tr[u].ls].sz - 1);
        tr[u].rs = rl; return {u, rr};
    }
    void build(int n) { for (int i = 1; i <= n; i++) rt = merge(rt, alloc()); }
    void operate(int l, int r) {
        auto [ul, tp] = split(rt, l - 1);
        auto [u, ur] = split(tp, r - l + 1);
        if (l == r) {
            tr[u].val = 1;
            rt = merge(merge(ul, u), ur);
        } else {
            auto [u1, u2] = split(u, r - l);
            tr[u2].val = 1;
            rt = merge(merge(ul, u2), merge(u1, ur));
        }
    }
    int ans = 0;
    void fetch(int u) {
        if (!u) return;
        fetch(tr[u].ls);
        ans += tr[u].val;
        printf("%d ", ans);
        fetch(tr[u].rs);
    }
    void print() { ans = 0; fetch(rt); printf("%d\n", ans); }
} f;
int main() {
    int n;
    scanf("%d", &n);
    f.build(n);
    for (int i = 1, l, r; i <= n; i++) {
        scanf("%d%d", &l, &r);
        f.operate(l, r);
        f.print();
    }
    f.print();
    return 0;
}