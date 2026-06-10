#include <cstdio>
#include <algorithm>
#include <cstring>
#include <cstdint>
#include <cmath>
#include <vector>
#include <iostream>
constexpr int N = 1005;
#define For(i, s, t) for (int i = (s); i <= (t); ++i)
#define roF(i, s, t) for (int i = (s); i >= (t); --i)
int p[N], n, len;
unsigned fact[15], perm[N]; // 以 i 开头的排列 p[i..i+len-1] 的阶乘数
inline unsigned calcPerm(int i) { // 计算 p[i..i+len-1] 的阶乘数
    unsigned res = 0;
    for (int j = 0; j < len; j++)
        for (int k = j+1; k < len; k++)
            if (p[i+k] < p[i+j]) res += fact[len - j - 1];
    return res;
}
struct UpdateNode { int idx; unsigned pre, nxt; };
using Update = std::vector<UpdateNode>;
std::vector<UpdateNode> upds[N]; // 每一个更新（swap p[x],p[y]）所影响到的所有 perm[] 的值
inline void update(Update& upd, int i) {
    unsigned res = calcPerm(i);
    upd.push_back({i, perm[i], res});
    perm[i] = res;
}
inline int calcRepeated(int i) { // 计算以 [i+1, i+len-1] 开头中有多少排列与以 i 开头的排列相等
    int res = 0;
    for (int j = 1; j < len; j++) res += perm[i] == perm[i+j];
    return res;
}
struct BIT {
    int tr[N], orig[N];
    void add(int p, int val) { for (; p <= n; p += p & -p) tr[p] += val; }
    void modify(int p) { int val = calcRepeated(p); add(p, val - orig[p]); orig[p] = val; }
    int sum(int p) { int res = 0; for (; p; p -= p & -p) res += tr[p]; return res; }
    int sum(int l, int r) { return sum(r) - sum(l - 1); }
} frep; // 重复排列的数量，支持区间求和
int BSz;
struct Query {
    int l, r, t, lid, rid, id;
    friend bool operator<(const Query& x, const Query& y) { return x.lid > y.lid || x.lid == y.lid && (x.rid > y.rid || x.rid == y.rid && x.t > y.t); }
} ques[N];
struct MoImpl { // 莫队的实现
    int cnt[N]; // 将贡献 C(x,2) 拆成 0 + 1 + 2 + ... + (x-1)，每次加入贡献的增量就是 cnt[x]-1
    int64_t ans;
    int l, r, t;
    void addPerm(unsigned permCode) { ans += cnt[permCode]++; }
    void delPerm(unsigned permCode) { ans += --cnt[permCode]; }
#define chk(expr...) (l <= upd.idx && upd.idx <= r) && ((expr), 1)
    /*让时间前进*/ void patch() { ++t; for(const auto &upd : upds[t]) chk(delPerm(upd.pre)), perm[upd.idx] = upd.nxt, chk(addPerm(upd.nxt)); }
    /*让时间后退*/ void unpatch() { for(const auto &upd : upds[t]) chk(delPerm(upd.nxt)), perm[upd.idx] = upd.pre, chk(addPerm(upd.pre)); --t; }
    void update(int ql, int qr, int qt) {
        while (l > ql) addPerm(perm[--l]);
        while (r < qr) addPerm(perm[++r]);
        while (l < ql) delPerm(perm[l++]);
        while (r > qr) addPerm(perm[r--]);
        while (t < qt) patch();
        while (t > qt) unpatch();
    }
    int64_t inquire() const { return ans; }
};
int64_t ans[N]; // ans 的贡献拆分成：{l~r 所有数对的数量（允许重叠）} - {l~r 中重叠数对的贡献} + {多减去的，在 r-len+1...r 这个范围内多算出来的 重叠数对}
// 如果把合法点对描述在平面直角坐标系上，那就是 s1-s2+s3 ![](https://cdn.luogu.com.cn/upload/image_hosting/0njcu9hp.png)
int main() {
    freopen("stars.in", "r", stdin);
    freopen("stars.out", "w", stdout);
    std::cin.tie(nullptr) -> sync_with_stdio(false);
    fact[0] = 1;
    For(i, 1, 10) fact[i] = fact[i-1] * i;
    int m; std::cin >> n >> m >> len;
    double B = std::pow(n, 2./3.);
    For(i, 1, n) std::cin >> p[i];
    For(i, 1, n-len+1) perm[i] = calcPerm(i);
    For(i, 1, n-len+1) frep.modify(i);
    int ts = 0, qs = 0, tm = 0;
    for (int op, x, y; m--; ) {
        std::cin >> op >> x >> y;
        if (op == 1) {
            std::swap(p[x], p[y]);
            upds[++ts].reserve(2*len);
            if (x > y) std::swap(x, y);
            if (y - x <= len) {
                For (i, std::max(1, x-len+1), std::min(y, n-len+1)) update(upds[ts], i);
                For (i, std::max(1, x-len+1), std::min(y+len-1, n-len+1)) frep.modify(i);
            }
            For (i, std::max(1, x-len+1), std::min(x, n-len+1)) perm[i] = calcPerm(i);
            For (i, std::max(1, y-len+1), std::min(y, n-len+1)) perm[i] = calcPerm(i);
            For (i, std::max(1, x-len+1), std::min(x+len-1, n-len+1)) frep.modify(i);
            For (i, std::max(1, y-len+1), std::min(y+len-1, n-len+1)) frep.modify(i);
        }
        else {
            y -= len-1;
            if (y < x) { ans[++tm] = 0; continue; }
            ques[++qs] = {x, y, ts, int(x / B), int(y / B), ++tm};
            ans[qs] = -frep.sum(x, y) + ;
        }
    }
    return 0;
}