// 喵喵喵。为什么 bnds 这么喜欢历史和？？？？？？？？？？？？？
// 喵喵喵。为什么 bnds 这么喜欢历史和？？？？？？？？？？？？？
// 喵喵喵。为什么 bnds 这么喜欢历史和？？？？？？？？？？？？？
// 喵喵喵。为什么 bnds 这么喜欢历史和？？？？？？？？？？？？？
// 喵喵喵。为什么 bnds 这么喜欢历史和？？？？？？？？？？？？？
// 喵喵喵。为什么 bnds 这么喜欢历史和？？？？？？？？？？？？？
// 喵喵喵。为什么 bnds 这么喜欢历史和？？？？？？？？？？？？？
// 喵喵喵。为什么 bnds 这么喜欢历史和？？？？？？？？？？？？？
#include "bits/stdc++.h"
using namespace std;
template <typename T> void in(T &n) {
    n = 0;
    char c = getchar();
    bool flag = 0;
    for (; c < '0' || c > '9'; c = getchar())
        if (c == '-')
            flag = 1;
    for (; c >= '0' && c <= '9'; c = getchar())
        (n *= 10) += (c ^ 48);
    if (flag)
        n = -n;
}
int wlsk[45];
int wltp;
template <typename T> void out(T n, char c = 0) {
    if (n == 0) {
        putchar('0');
        if (c)
            putchar(c);
        return;
    }
    if (n < 0)
        putchar('-'), n = -n;
    while (n)
        wlsk[++wltp] = (n % 10), n /= 10;
    while (wltp)
        putchar(wlsk[wltp--] ^ 48);
    if (c)
        putchar(c);
}
typedef long long ll1;
#define pii pair<int, int>
#define mkp make_pair
#define fir first
#define sec second
const int N = 1e6 + 6;
const int M = 4e6 + 5;
const int inf = 1e9 + 7;
pii stkd[N];
int topd = 0;
#define ls (o << 1)
#define rs (ls | 1)
#define mid ((l + r) >> 1)
namespace sgt1 {
int mx[M], tgmx[M];
int mn[M], tgmn[M];
int g[M];
bool c[M];
void pup(int o) {
    mx[o] = max(mx[ls], mx[rs]), mn[o] = min(mn[ls], mn[rs]);
    g[o] = min(g[ls], g[rs]), c[o] = c[ls] + c[rs];
    if (!c[o])
        g[o] = inf;
}
void build(int o, int l, int r, int n) {
    mx[o] = 0, mn[o] = n + 1;
    tgmx[o] = -inf, tgmn[o] = inf;
    g[o] = n + 1, c[o] = 1;
    if (l == r)
        return;
    build(ls, l, mid, n), build(rs, mid + 1, r, n);
    pup(o);
}
void Covmx(int o, int x) {
    if (!c[o])
        return;
    mx[o] = max(mx[o], x), tgmx[o] = max(tgmx[o], x);
    g[o] = min(g[o], mn[o] - x); // cover
}
void Covmn(int o, int x) {
    if (!c[o])
        return;
    mn[o] = min(mn[o], x), tgmn[o] = min(tgmn[o], x);
    g[o] = min(g[o], x - mx[o]); // cover
}
void psd(int o) {
    if (tgmx[o] != -inf)
        Covmx(ls, tgmx[o]), Covmx(rs, tgmx[o]);
    if (tgmn[o] != inf)
        Covmn(ls, tgmn[o]), Covmn(rs, tgmn[o]);
    tgmx[o] = -inf, tgmn[o] = inf;
}
void chkmx(int o, int l, int r, int lt, int rt, int x) {
    if (l >= lt && r <= rt)
        return Covmx(o, x);
    psd(o);
    if (lt <= mid)
        chkmx(ls, l, mid, lt, rt, x);
    if (rt > mid)
        chkmx(rs, mid + 1, r, lt, rt, x);
    pup(o);
}
void chkmn(int o, int l, int r, int lt, int rt, int x) {
    if (l >= lt && r <= rt)
        return Covmn(o, x);
    psd(o);
    if (lt <= mid)
        chkmn(ls, l, mid, lt, rt, x);
    if (rt > mid)
        chkmn(rs, mid + 1, r, lt, rt, x);
    pup(o);
}
void getdiv(int o, int l, int r, bool op) {
    if (g[o] > 0)
        return;
    if (l == r) {
        stkd[++topd] = mkp(l, (op == 0 ? mx[o] : mn[o] - 1));
        mx[o] = -inf, mn[o] = inf, c[o] = 0, g[o] = inf;
        return;
    }
    psd(o), getdiv(ls, l, mid, op), getdiv(rs, mid + 1, r, op), pup(o);
}
} // namespace sgt1
namespace sgtbeats { // 区间 chkmx，区间历史和
int L[M], R[M];
int cl[M]; // 点亮的点数，这个才是真 len！
bool fl1, fl2;
struct Node {
    ll1 sm, hsm;
    int mn1, cnt, mn2;
    int ad1, ad2;
    ll1 ad1_, ad2_;
    int h; // 叠加历史
} tr[M];
void pup(int o) {
    tr[o].sm = tr[ls].sm + tr[rs].sm, tr[o].hsm = tr[ls].hsm + tr[rs].hsm;
    tr[o].mn1 = min(tr[ls].mn1, tr[rs].mn1);
    if (tr[ls].mn1 == tr[rs].mn1)
        tr[o].cnt = tr[ls].cnt + tr[rs].cnt,
        tr[o].mn2 = min(tr[ls].mn2, tr[rs].mn2);
    else if (tr[ls].mn1 < tr[rs].mn1)
        tr[o].cnt = tr[ls].cnt, tr[o].mn2 = min(tr[ls].mn2, tr[rs].mn1);
    /*if(tr[rs].mn1< tr[ls].mn1)*/ else
        tr[o].cnt = tr[rs].cnt, tr[o].mn2 = min(tr[rs].mn2, tr[ls].mn1);
    cl[o] = cl[ls] + cl[rs];
}
void build(int o, int l, int r) {
    L[o] = l, R[o] = r, cl[o] = 0;
    tr[o].sm = tr[o].hsm = 0, tr[o].mn1 = 0, tr[o].mn2 = inf, tr[o].cnt = 0;
    tr[o].ad1 = tr[o].ad2 = tr[o].ad1_ = tr[o].ad2_ = 0, tr[o].h = 0;
    if (l != r)
        build(ls, l, mid), build(rs, mid + 1, r), pup(o);
}
void update(int o, int h) {
    tr[o].hsm += tr[o].sm * h;
    tr[o].ad1_ += 1ll * tr[o].ad1 * h;
    tr[o].ad2_ += 1ll * tr[o].ad2 * h;
    tr[o].h += h;
}
void Addnw(int o, int k1, int k2) {
    tr[o].sm += 1ll * tr[o].cnt * k1;
    tr[o].sm += 1ll * (cl[o] - tr[o].cnt) * k2;
    tr[o].mn1 += k1;
    if (tr[o].mn2 != inf)
        tr[o].mn2 += k2;
    tr[o].ad1 += k1, tr[o].ad2 += k2;
}
void Addh(int o, ll1 k1_, ll1 k2_) {
    tr[o].hsm += tr[o].cnt * k1_;
    tr[o].hsm += (cl[o] - tr[o].cnt) * k2_;
    tr[o].ad1_ += k1_, tr[o].ad2_ += k2_;
}
void psd(int o) {
    if (tr[o].h)
        update(ls, tr[o].h), update(rs, tr[o].h);
    int mn1 = min(tr[ls].mn1, tr[rs].mn1);
    fl1 = (tr[ls].mn1 == mn1), fl2 = (tr[rs].mn1 == mn1);
    if (tr[o].ad1_ || tr[o].ad2_) {
        fl1 ? Addh(ls, tr[o].ad1_, tr[o].ad2_)
            : Addh(ls, tr[o].ad2_, tr[o].ad2_);
        fl2 ? Addh(rs, tr[o].ad1_, tr[o].ad2_)
            : Addh(rs, tr[o].ad2_, tr[o].ad2_);
    }
    if (tr[o].ad1 || tr[o].ad2) {
        fl1 ? Addnw(ls, tr[o].ad1, tr[o].ad2) : Addnw(ls, tr[o].ad2, tr[o].ad2);
        fl2 ? Addnw(rs, tr[o].ad1, tr[o].ad2) : Addnw(rs, tr[o].ad2, tr[o].ad2);
    }
    tr[o].h = 0, tr[o].ad1 = tr[o].ad2 = tr[o].ad1_ = tr[o].ad2_ = 0;
}
void light(int o, int p) {
    int l = L[o], r = R[o];
    if (l == r) {
        tr[o].cnt = 1, cl[o] = 1, tr[o].sm = tr[o].mn1;
        return;
    }
    psd(o), light(p <= mid ? ls : rs, p), pup(o);
}
void chkmx(int o, int lt, int rt, int x) {
    int l = L[o], r = R[o];
    if (tr[o].mn1 >= x)
        return;
    if (l >= lt && r <= rt && tr[o].mn2 > x)
        return Addnw(o, x - tr[o].mn1, 0);
    psd(o);
    if (lt <= mid)
        chkmx(ls, lt, rt, x);
    if (rt > mid)
        chkmx(rs, lt, rt, x);
    pup(o);
}
ll1 qryh(int o, int lt, int rt) {
    int l = L[o], r = R[o];
    if (l >= lt && r <= rt)
        return tr[o].hsm;
    psd(o);
    ll1 res = 0;
    if (lt <= mid)
        res += qryh(ls, lt, rt);
    if (rt > mid)
        res += qryh(rs, lt, rt);
    return res;
}
} // namespace sgtbeats
#undef ls
#undef rs
#undef mid
int n, q;
struct Mat {
    int lt, rt, l, r, w;
} mts[N];
bool cmpw(Mat x, Mat y) { return x.w > y.w; }
int dv[N];
vector<int> pl[N], pr[N]; // 点亮的点
vector<int> ml[N], mr[N];
struct Qrs {
    int l, r, id;
};
vector<Qrs> qrsl[N], qrsr[N];
ll1 Ans[N];
void init() {
    sort(mts + 1, mts + 1 + n, cmpw);
    sgt1::build(1, 1, n, n);
    for (int i = 1; i <= n; i++)
        dv[i] = -1;
    for (int i = 1; i <= n; i++) {
        auto [lt, rt, l, r, w] = mts[i];
        topd = 0;
        if (lt == 1) {
            ml[rt].emplace_back(i);
            sgt1::chkmx(1, 1, n, l, r, rt);
            sgt1::getdiv(1, 1, n, 1); // 加入左边冲突了，说明右边更大
        } else {
            mr[lt].emplace_back(i);
            sgt1::chkmn(1, 1, n, l, r, lt);
            sgt1::getdiv(1, 1, n, 0); // 加入右边冲突了，说明左边更大
        }
        while (topd) {
            dv[stkd[topd].fir] = stkd[topd].sec;
            topd--;
        }
    }
    for (int i = 1; i <= n; i++) {
        if (dv[i] == -1)
            pl[n].emplace_back(i), pr[1].emplace_back(i);
        else
            pl[dv[i]].emplace_back(i), pr[dv[i] + 1].emplace_back(i);
    }
}
void work() {
    // solve L
    sgtbeats::build(1, 1, n);
    for (int nw = n; nw >= 1; nw--) {
        for (auto p : pl[nw])
            sgtbeats::light(1, p);
        for (auto i : ml[nw]) {
            auto [lt, rt, l, r, w] = mts[i];
            sgtbeats::chkmx(1, l, r, w);
        }
        sgtbeats::update(1, 1);
        for (auto [l, r, id] : qrsl[nw]) {
            if (id > 0)
                Ans[id] += sgtbeats::qryh(1, l, r);
            else
                Ans[-id] -= sgtbeats::qryh(1, l, r);
        }
    }
    // solve R
    sgtbeats::build(1, 1, n);
    for (int nw = 1; nw <= n; nw++) {
        for (auto p : pr[nw])
            sgtbeats::light(1, p);
        for (auto i : mr[nw]) {
            auto [lt, rt, l, r, w] = mts[i];
            sgtbeats::chkmx(1, l, r, w);
        }
        sgtbeats::update(1, 1);
        for (auto [l, r, id] : qrsr[nw]) {
            if (id > 0)
                Ans[id] += sgtbeats::qryh(1, l, r);
            else
                Ans[-id] -= sgtbeats::qryh(1, l, r);
        }
    }
}
int main() {
    freopen("rainfall.in", "r", stdin);
    freopen("rainfall.out", "w", stdout);
    int Tid = 0;
    in(Tid);
    in(n), in(q);
    for (int i = 1, x, y, l, r, w; i <= n; i++) {
        in(x), in(y), in(l), in(r), in(w);
        mts[i] = (Mat){x, y, l, r, w};
    }
    init();
    for (int i = 1, lt, rt, l, r; i <= q; i++) {
        in(lt), in(rt), in(l), in(r);
        qrsl[lt].emplace_back((Qrs){l, r, i});
        qrsl[rt + 1].emplace_back((Qrs){l, r, -i});
        qrsr[rt].emplace_back((Qrs){l, r, i});
        qrsr[lt - 1].emplace_back((Qrs){l, r, -i});
    }
    work();
    for (int i = 1; i <= q; i++)
        out(Ans[i], '\n');
    return 0;
}
