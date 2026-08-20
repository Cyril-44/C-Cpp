#include <cstdio>
#include <cstdint>
#include <algorithm>
#include <queue>
#include <cassert>
constexpr int N = 200000;
#define L first
#define R second
using Pii = std::pair<int,int>;
struct BIT {
    uint64_t tr[N+4];
    void upd(int p, uint64_t x) { for (; p <= N; p += p & -p) tr[p] += x; }
    uint64_t sum(int p) const { uint64_t res = 0; for (; p > 0; p -= p & -p) res += tr[p]; return res; }
};
struct BIT2 {
    BIT bit, biti;
    void upd(int l, int r, uint64_t x) { bit.upd(l, x), bit.upd(r, -x), biti.upd(l, l * x), biti.upd(r, -r * x); }
    uint64_t sum(int p) const { return (p+1ull) * bit.sum(p) - biti.sum(p); }
    uint64_t sum(int l, int r) const { return sum(r) - sum(l-1); }
} fs;
Pii ranges[N+4], ufidx[N+4];
int rangeTop = 0;
int Mid;
struct UFS { // Union Find Set
    constexpr static int UN = N * 32 + 4;
    int fa[UN], sz[UN], pnt[UN];
    std::vector<int> idxs[UN];
    int tot;
    int alloc(int i, int j) { ++tot, fa[tot] = tot, sz[tot] = 1, pnt[tot] = j, idxs[tot] = {i}; return tot; } // 创建一个实节点，代表第 i 个区间的端点 j
    int valloc(int i) { ++tot, fa[tot] = tot, pnt[tot] = i; return tot; } // 创建一个虚节点，代表端点 i
    int find(int u) { int v = u, x; while (v != fa[v]) v = fa[v]; while (u != v) x = fa[u], fa[u] = v, u = x; return u; }
    void erase(int u) { std::vector<int>().swap(idxs[u]); }
    void merge(int &u, int v) { // 把 v 的所有端点并到 u 上，按秩合并，注意清除 idxs 中的僵尸点（已经被从分治区间移除，但是有一端还残留在分治区间中）
        if (sz[u] < sz[v]) pnt[v] = pnt[u], std::swap(u, v);
        idxs[u].reserve(idxs[u].size() + idxs[v].size());
        for (int i : idxs[v]) if (ranges[i].L <= Mid && Mid < ranges[i].R) idxs[u].push_back(i);
        erase(v), fa[v] = u, sz[u] += sz[v];
    }
} uf;
class CatTr {
    struct Node { // 优先队列里面的二元组 (v, i) 存的是当前端点为 v 的并查集代表 i
        std::priority_queue<Pii, std::vector<Pii>, std::greater<Pii>> ls;
        std::priority_queue<Pii> rs;
    } tr[N + 4 << 2];
    int idx, mL, mR, uL, uR;
    void ins(int u, int l, int r) { // 插入第 idx 个区间 [mL, mR]
        int mid = l + r >> 1;
        if (mL <= mid && mid < mR) {
            int mLid = uf.alloc(idx, mL), mRid = uf.alloc(idx, mR);
            ufidx[idx] = {mLid, mRid};
            tr[u].ls.emplace(mL, mLid);
            tr[u].rs.emplace(mR, mRid);
            fs.upd(mL, mR, 1);
        }
        else if (mR <= mid) ins(u<<1, l, mid);
        else ins(u<<1|1, mid+1, r);
    }
    void upd(int u, int l, int r) {
        if (uL <= l && r <= uR) return;
        int mid = l + r >> 1;
        if (uL <= mid && mid < uR) { // 将区间内左端点 < uL、右端点 > uR 的所有线段缩起来
            int lj = uf.valloc(uL), rj = uf.valloc(uR);
            Mid = mid; // 帮助在 merge 的时候清理僵尸点
            while (!tr[u].ls.empty()) {
                auto [v, j] = tr[u].ls.top();
                if (v > uL) break;
                tr[u].ls.pop();
                if (!uf.sz[j]) continue;
                fs.upd(v, uL, -uf.sz[j]);
                // for (int i : uf.idxs[j]) fprintf(stderr, "%d: [%d, %d] -> [%d, %d]\n", i, ranges[i].L, ranges[i].R, uL, ranges[i].R);
                uf.merge(lj, j);
            }
            while (!tr[u].rs.empty()) {
                auto [v, j] = tr[u].rs.top();
                if (v < uR) break;
                tr[u].rs.pop();
                if (!uf.sz[j]) continue;
                fs.upd(uR, v, -uf.sz[j]);
                // for (int i : uf.idxs[j]) fprintf(stderr, "%d: [%d, %d] -> [%d, %d]\n", i, ranges[i].L, ranges[i].R, ranges[i].L, uR);
                uf.merge(rj, j);
            }
            if (uf.sz[lj]) tr[u].ls.emplace(uL, lj);
            if (uf.sz[rj]) tr[u].rs.emplace(uR, rj);
            upd(u<<1, l, mid);
            upd(u<<1|1, mid+1, r);
        } else {
            auto downgrade = [&](int i) { // 将区间 i 与 [uL, uR] 取并同时降级到下面的分治区间
                if (ranges[i].L > mid || mid >= ranges[i].R) return;
                int rtidxl = uf.find(ufidx[i].L), rtidxr = uf.find(ufidx[i].R);
                --uf.sz[rtidxl], --uf.sz[rtidxr];
                // fprintf(stderr, "[%d, %d] -> ", ranges[i].L, ranges[i].R);
                ranges[i] = {uf.pnt[rtidxl], uf.pnt[rtidxr]};
                fs.upd(ranges[i].L, ranges[i].R, -1);
                // fprintf(stderr, "[%d, %d] -> ", ranges[i].L, ranges[i].R);
                ranges[i] = {std::max(ranges[i].L, uL), std::min(ranges[i].R, uR)};
                // fprintf(stderr, "[%d, %d]\n", ranges[i].L, ranges[i].R);
                assert(ranges[i].L <= ranges[i].R);
                insert(i);
            };
            if (uR <= mid) {
                while (!tr[u].ls.empty()) {
                    auto [v, j] = tr[u].ls.top();
                    if (v > uR) break;
                    tr[u].ls.pop();
                    for (int i : uf.idxs[j]) downgrade(i);
                    uf.erase(j);
                }
                upd(u<<1, l, mid);
            } else {
                while (!tr[u].rs.empty()) {
                    auto [v, j] = tr[u].rs.top();
                    if (v < uL) break;
                    tr[u].rs.pop();
                    for (int i : uf.idxs[j]) downgrade(i);
                    uf.erase(j);
                }
                upd(u<<1|1, mid+1, r);
            }
        }
    }
public:
    void insert(int i) { if (ranges[i].L != ranges[i].R) idx = i, mL = ranges[i].L, mR = ranges[i].R, ins(1, 1, N); }
    void intersect(int l, int r) { uL = l, uR = r, upd(1, 1, N); }
} f;
int main() {
    int q, type, last = 0;
    scanf("%d%d", &q, &type);
    for (int op, l, r; q--; ) {
        scanf("%d%d%d", &op, &l, &r);
        l = (l + type * last) % 200001, r = (r + type * last) % 200001;
        switch (op) {
        case 1:
            ranges[++rangeTop] = {l, r};
            f.insert(rangeTop);
            break;
        case 2:
            f.intersect(l, r);
            break;
        case 3:
            auto ans = l == r ? (uint64_t)0 : fs.sum(l, r-1);
            printf("%llu\n", ans);
            last = ans % 200001;
        }
        // if (q % 1000 == 0) fprintf(stderr, "%d left\n", q), fflush(stderr);
    }
    return 0;
}