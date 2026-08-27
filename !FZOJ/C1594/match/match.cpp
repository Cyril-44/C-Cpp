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
        int sum;
        bool val;
    } tr[N];
    int tot, rt;
    void pushup(int u) {
        tr[u].sz = tr[tr[u].ls].sz + 1 + tr[tr[u].rs].sz;
        tr[u].sum = tr[tr[u].ls].sum + tr[u].val + tr[tr[u].rs].sum;
    }
    int alloc(bool val = 0) { return tr[++tot] = {0, 0, 1, (unsigned)rng(), (int)val, val}, tot; }
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
            tr[u].ls = lr; pushup(u); return {ll, u};
        }
        auto [rl, rr] = split(tr[u].rs, s - tr[tr[u].ls].sz - 1);
        tr[u].rs = rl; pushup(u); return {u, rr};
    }
    void build(int n) { for (int i = 1; i <= n; i++) rt = merge(rt, alloc()); }
    void delfirst1(int u) {
        if (!tr[u].sum) return;
        if (tr[tr[u].ls].sum) delfirst1(tr[u].ls);
        else if (tr[u].val) tr[u].val = 0;
        else delfirst1(tr[u].rs);
        pushup(u);
    }
    void operate(int l, int r) {
        auto [ul, tp] = split(rt, l - 1);
        auto [u, ur] = split(tp, r - l + 1);
        if (l == r) {
            if (!tr[u].val) {
                tr[u].val = 1; ++tr[u].sum;
                delfirst1(ur);
            }
            rt = merge(merge(ul, u), ur);
        } else {
            auto [u1, u2] = split(u, r - l);
            if (!tr[u2].val) {
                tr[u2].val = 1; ++tr[u2].sum;
                delfirst1(ur);
            }
            rt = merge(merge(ul, u2), merge(u1, ur));
        }
    }
    int ans = 0;
    void fetch(int u) {
        if (!u) return;
        fetch(tr[u].ls);
        ans += tr[u].val;
        fprintf(stderr, "%d #%d, val=%d, sum=%d, ls=%d, rs=%d\n", ans, u, (int)tr[u].val, tr[u].sum, tr[u].ls, tr[u].rs);
        fetch(tr[u].rs);
    }
    void print() { printf("%d\n", tr[rt].sum); }
} f;
int main() {
    int n;
    scanf("%d", &n);
    f.build(n);
    for (int i = 1, l, r; i <= n; i++) {
        scanf("%d%d", &l, &r);
        f.operate(l, r);
        // f.ans = 0; f.fetch(f.rt);
        // fprintf(stderr, "\n");
    }
    f.print();
    return 0;
}