#include <cstdio>
#include <cstring>
#include <algorithm>
#include <vector>
#include <tuple>
namespace FastI {
char buf[1 << 20], *p1{}, *p2{};
inline char gc() {
    if (p1 == p2) p2 = (p1=buf) + fread(buf, 1, sizeof buf, stdin);
    if (p1 == p2) return EOF;
    return *p1++;
}
inline void in(int &x) {
    char ch = gc();
    while (ch < '0' || ch > '9') ch = gc();
    for (x = 0; ch >= '0' && ch <= '9'; ch = gc())
        x = (x << 3) + (x << 1) + (ch & 0xf);
}
} using FastI::in;
#ifdef CLANGD
constexpr int N = 54;
#else
constexpr int N = 500004;
#endif
std::vector<std::pair<int,int>> a[N];
int lastpos[N][2], n;
struct BIT {
    int tr[N], all;
    void upd(int p, int x) { all += x; for (; p <= n; p += p & -p) tr[p] += x; }
    int sum(int p) const { int res = 0; for (; p > 0; p -= p & -p) res += tr[p]; return res; }
    int sum() const { return all; }
} fs;
std::vector<std::tuple<int,int,int>> qry[N];
int ans[N];
int main() {
    int testid, m, q;
    in(testid), in(n), in(m);
    for (int i = 1; i <= n; i++) {
        int k; in(k); a[i].resize(k);
        for (int j = 0; j < k; j++) {
            int col; in(col);
            if (lastpos[col][0]) a[lastpos[col][0]][lastpos[col][1]].second = i;
            a[i][j] = {lastpos[col][0], n+1};
            lastpos[col][0] = i, lastpos[col][1] = j;
        }
    }
    in(q);
    for (int i = 1, l, r, p; i <= q; i++) {
        in(l), in(r), in(p);
        qry[p].emplace_back(l, r, i);
    }
    for (int i = 1; i <= n; i++) {
        std::sort(a[i].begin(), a[i].end());
        std::sort(qry[i].begin(), qry[i].end());
        int j = 0;
        for (const auto &[l, r, id] : qry[i]) {
            while (j < (int)a[i].size() && a[i][j].first < l)
                fs.upd(a[i][j++].second, 1);
            ans[id] = fs.sum() - fs.sum(r);
        }
        while (j) fs.upd(a[i][--j].second, -1);
    }
    for (int i = 1; i <= q; i++) printf("%d\n", ans[i]);
    return 0;
}