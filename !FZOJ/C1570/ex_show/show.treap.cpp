
struct Treap {
    constexpr static int64_t COV_DFT = (1ll << 62) - 1;
    std::mt19937 rng;
    int totcnt;
    Treap() : rng(std::random_device{}()), totcnt(), rt() {}
    struct Node {
        int ls, rs, cnt;
        int64_t val, lmx, rmn, add, cov;
        uint64_t weight;
        inline void pull(int64_t p, int64_t c) {
            if (c != COV_DFT) val = lmx = rmn = cov = c, add = 0;
            if (p) val += p, lmx += p, rmn += p, add += p;
        }
    } tr[N << 2];
    int rt;
    inline void pushup(int u) {
        tr[u].cnt = tr[tr[u].ls].cnt + 1 + tr[tr[u].rs].cnt;
        tr[u].lmx = tr[u].ls ? tr[tr[u].ls].lmx : tr[u].val;
        tr[u].rmn = tr[u].rs ? tr[tr[u].rs].rmn : tr[u].val;
    }
    inline void pushdown(int u) {
        if (tr[u].add == 0 && tr[u].cov == COV_DFT) return;
        if (tr[u].ls) tr[tr[u].ls].pull(tr[u].add, tr[u].cov);
        if (tr[u].rs) tr[tr[u].rs].pull(tr[u].add, tr[u].cov);
        tr[u].add = 0, tr[u].cov = COV_DFT;
    }
    inline int alloc() {
        tr[++totcnt] = { 0,0, 1, 0ll,0ll,0ll,0ll, COV_DFT, rng() };
        return totcnt;
    }
    inline void push_back() {
        rt = merge(rt, alloc());
    }
    int merge(int u, int v) {
        if (!u || !v) return u | v;
        if (tr[u].weight < tr[v].weight)
            return pushdown(u), tr[u].rs = merge(tr[u].rs, v), pushup(u), u;
        return pushdown(v), tr[v].ls = merge(u, tr[v].ls), pushup(v), v;
    }
    std::pair<int,int> lower_bound(int u, int64_t w) { // Find the first position to insert w, and split into 2 parts
        if (tr[u].lmx <= w) return {0, u};
        if (tr[u].rmn > w) return {u, 0};
        pushdown(u);
        if (w > tr[u].val) {
            auto ret = lower_bound(tr[u].ls, w);
            tr[u].ls = ret.second;
            pushup(u);
            return {ret.first, u};
        }
        auto ret = lower_bound(tr[u].rs, w);
        tr[u].rs = ret.first;
        pushup(u);
        return {u, ret.second};
    }
    std::pair<int,int> split(int u, int w) { // Split the tree to {lsize=w, r}
        if (w == 0) return {0, u};
        if (tr[u].cnt == w) return {u, 0};
        // if (tr[u].cnt < w) assert(0);
        pushdown(u);
        if (tr[tr[u].ls].cnt >= w) {
            auto ret = split(tr[u].ls, w);
            tr[u].ls = ret.second;
            pushup(u);
            return {ret.first, u};
        }
        auto ret = split(tr[u].rs, w - tr[tr[u].ls].cnt - 1);
        tr[u].rs = ret.first;
        pushup(u);
        return {u, ret.second};
    }
    inline int64_t inquireLast(int u) { return tr[u].rmn; }
    void debug(int u) {
        if (!u) return;
        pushdown(u);
        assert(!tr[u].ls || tr[u].weight < tr[tr[u].ls].weight);
        assert(!tr[u].rs || tr[u].weight < tr[tr[u].rs].weight);
        debug(tr[u].ls);
        printf(" %ld[%d] ", tr[u].val, tr[u].cnt);
        debug(tr[u].rs);
    }
} f; // 当前这一列的 dp 数组