#include <bits/stdc++.h>
constexpr int N = 1000005;
#ifdef CLANGD
#define N 15
#endif
std::vector<int> g[N];
std::set<int> sons[N];
char outputbuf[N * 2];
int p[N], ssz[N], fa[N], lv[N], lsum[N], que[N];
constexpr uint64_t B = (int)1e9 + 7;
uint64_t base[N];
std::mt19937 rng(std::random_device{}());
template<class Comp>
struct DelPQ {
    std::priority_queue<int, std::vector<int>, Comp> q, dq;
    int top() {
        if (q.empty()) return -1;
        int res = q.top();
        while (!dq.empty() && res == dq.top()) {
            q.pop(), dq.pop();
            if (q.empty()) return -1;
            res = q.top();
        }
        return res;
    }
    void push(int x) { q.push(x); }
    void erase(int x) { dq.push(x); }
};
DelPQ<std::greater<int>> pq1[N];
DelPQ<std::less<int>> pq2[N];
struct Treap {
    struct Node {
        uint64_t hash;
        int bh, bv, vl, vr, sz, ls, rs;
        unsigned w;
    } tr[N*3];
    int bin[N*3], rt[3][N], top;
    int alloc(int hsh, int rnk) {
        int u = bin[0] ? bin[bin[0]--] : ++top;
        tr[u] = {(uint64_t)hsh, hsh, rnk, rnk, rnk, 1, 0, 0, (unsigned)rng()};
        return u;
    }
    void pushup(int u) {
        tr[u].sz = tr[tr[u].ls].sz + 1 + tr[tr[u].rs].sz;
        tr[u].vl = tr[u].ls ? tr[tr[u].ls].vl : tr[u].bv;
        tr[u].vr = tr[u].rs ? tr[tr[u].rs].vr : tr[u].bv;
        tr[u].hash = tr[tr[u].ls].hash * base[tr[tr[u].rs].sz + 1] + tr[u].bh * base[tr[tr[u].rs].sz] + tr[tr[u].rs].hash;
    }
    int merge(int u, int v) {
        if (!u || !v) return u | v;
        if (tr[u].w < tr[v].w) return tr[u].rs = merge(tr[u].rs, v), pushup(u), u;
        return tr[v].ls = merge(u, tr[v].ls), pushup(v), v;
    }
    std::pair<int,int> split(int u, int sz) {
        if (sz == 0) return {0, u};
        if (!u) assert(0);
        if (sz == tr[u].sz) return {u, 0};
        if (sz <= tr[tr[u].ls].sz) {
            auto [l1, l2] = split(tr[u].ls, sz);
            tr[u].ls = l2, pushup(u);
            return {l1, u};
        }
        auto [r1, r2] = split(tr[u].rs, sz - tr[tr[u].ls].sz - 1);
        tr[u].rs = r1, pushup(u);
        return {u, r2};
    }
    std::pair<int,int> lower_bound(int u, int val) { // [1, val) [val, n]
        if (!u) return {0, 0};
        if (val <= tr[u].vl) return {0, u};
        if (val > tr[u].vr) return {u, 0};
        if (val <= tr[u].bv) {
            auto [l1, l2] = lower_bound(tr[u].ls, val);
            tr[u].ls = l2, pushup(u);
            return {l1, u};
        }
        auto [r1, r2] = lower_bound(tr[u].rs, val);
        tr[u].rs = r1, pushup(u);
        return {u, r2};
    }
    void recycle(int u) { bin[++bin[0]] = u; }
    bool equal(int i) { return tr[rt[0][i]].hash == tr[rt[1][i]].hash; }
    int insert(bool f, int q, int x) {
        if (!ssz[q]) return 0;
        const int pre = !equal(lv[q]);
        int &u = rt[f][lv[q]];
        auto [l, r] = lower_bound(u, x);
        // fprintf(stderr, "Insert (%d,%d) into %d[%d][%d]: ", q, x, u, (int)f, lv[q]);
        // audit(l); fprintf(stderr, "| "); audit(r); fprintf(stderr, "\n");
        u = merge(merge(l, alloc(q, x)), r);
        const int now = !equal(lv[q]);
        return now - pre;
    }
    int erase(bool f, int q, int x) {
        if (!ssz[q]) return 0;
        const int pre = !equal(lv[q]);
        int &u = rt[f][lv[q]];
        auto [l, tmp] = lower_bound(u, x);
        auto [v, r] = lower_bound(tmp, x+1);
        // fprintf(stderr, "Erase (%d,%d) from %d[%d][%d]: ", q, x, u, (int)f, lv[q]);
        // audit(l); fprintf(stderr, "| "); audit(v); fprintf(stderr, "| "); audit(r); fprintf(stderr, "\n");
        assert(v); recycle(v);
        u = merge(l, r);
        const int now = !equal(lv[q]);
        return now - pre;
    }
    void audit(int u) {
        if (!u) return;
        audit(tr[u].ls);
        fprintf(stderr, "(%d,%d [%d,%d]) ", tr[u].bh, tr[u].bv, tr[u].vl, tr[u].vr);
        audit(tr[u].rs);
    }
} ftr;
int lv1[N], lv2[N];
int main() {
    freopen("bfs.in", "r", stdin);
    freopen("bfs.out", "w", stdout);
    std::cin.tie(nullptr) -> sync_with_stdio(false);
    int n, q;
    std::cin >> n >> q;
    base[0] = 1;
    for (int i = 1; i <= n; i++) base[i] = base[i-1] * B;
    for (int i = 1; i <= n; i++) std::cin >> p[i];
    for (int i = 1, u, v; i < n; i++) {
        std::cin >> u >> v;
        g[u].push_back(v), g[v].push_back(u);
    }
    for (int i = 1; i <= n; i++) ssz[i] = (int)g[i].size() - (i != 1);
    for (int hd=0, tl=que[0]=lv[1]=1; hd < tl; hd++) {
        int u = que[hd]; ++lsum[lv[u]];
        for (int v : g[u]) if (!lv[v])
            fa[v] = u, lv[v] = lv[u] + 1, que[tl++] = v;
    }
    for (int i = 1; i <= n; i++) lsum[i] += lsum[i-1];

    int rangeErr = 0, hashErr = 0;
    auto inRange = [] (int i) { return lsum[lv[p[i]] - 1] < i && i <= lsum[lv[p[i]]]; };
    for (int i = 1; i <= n; i++) {
        pq[fa[p[i]]].push(i);
        rangeErr += !inRange(i);
    }
    for (int i = 1; i <= n; i++) {
        hashErr += ftr.insert(0, p[i], i);
        hashErr += ftr.insert(1, p[i], pq1[p[i]].top());
    }
    /* for (int i = 1; i <= n; i++) if (ftr.rt[0][i]) {
        fprintf(stderr, "Level%3d: ", i); ftr.audit(ftr.rt[0][i]); fprintf(stderr, "\n");
        fprintf(stderr, "          ");    ftr.audit(ftr.rt[1][i]); fprintf(stderr, "\n");
    } */
    for (int x, y, i = 0; i < q; i++) {
        // fprintf(stderr, "=========================\n");
        std::cin >> x >> y;
        if (x != y) {
            const int fax = fa[p[x]], fay = fa[p[y]], 
                      pretopx = pq[fax].top(),
                      pretopy = pq[fay].top();
            if (fax) pq[fax].erase(x), pq[fax].push(y);
            if (fay) pq[fay].erase(y), pq[fay].push(x);
            hashErr += ftr.erase(0, p[x], x);
            hashErr += ftr.erase(0, p[y], y);
            if (fax) hashErr += ftr.erase(1, fax, pretopx);
            if (fay && fax != fay) hashErr += ftr.erase(1, fay, pretopy);
            rangeErr -= !inRange(x);
            rangeErr -= !inRange(y);
            std::swap(p[x], p[y]);
            rangeErr += !inRange(x);
            rangeErr += !inRange(y);
            hashErr += ftr.insert(0, p[x], x);
            hashErr += ftr.insert(0, p[y], y);
            const int nowtopx = pq[fax].top(),
                      nowtopy = pq[fay].top();
            if (fax) hashErr += ftr.insert(1, fax, nowtopx);
            if (fay && fax != fay) hashErr += ftr.insert(1, fay, nowtopy);
        }
        outputbuf[i<<1] = (!rangeErr && !hashErr) | '0';
        outputbuf[i<<1|1] = '\n';
        fprintf(stderr, "%d %d\n", rangeErr, hashErr);
        /* for (int i = 1; i <= n; i++) if (ftr.rt[0][i]) {
            fprintf(stderr, "Level%3d: ", i); ftr.audit(ftr.rt[0][i]); fprintf(stderr, "\n");
            fprintf(stderr, "          ");    ftr.audit(ftr.rt[1][i]); fprintf(stderr, "\n");
        } */
    }
    fwrite(outputbuf, q, 2, stdout);
}