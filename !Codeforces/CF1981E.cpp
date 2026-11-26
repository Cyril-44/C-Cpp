#include <bits/stdc++.h>
constexpr int N = 500005;
#ifdef CLANGD
#define N 55
#endif
using Pii = std::pair<int,int>;
using Info = std::pair<Pii, Pii>;
Info getmin(Info x, Pii y) {
    if (x.first > y) x.first = y;
    else if (y.second != x.first.second) x.second = y;
    return x;
}
Info getmax(Info x, Pii y) {
    if (x.first < y) x.first = y;
    else if (y.second != x.first.second) x.second = y;
    return x;
}
Info getmin(Info x, Info y) {
    x = getmin(x, y.first);
    x = getmin(x, y.second);
    return x;
}
Info getmax(Info x, Info y) {
    x = getmax(x, y.first);
    x = getmax(x, y.second);
    return x;
}
int all[N], m;
std::set<Pii> st[N];
struct SegTr {
    Info mn[N << 2], mx[N << 2];
    int P; Info X, Y;
    void upd(int u=1, int l=1, int r=m) {
        if (l == r) mn[u] = X, mx[u] = Y;
        else {
            int mid = l + r >> 1;
            P <= mid ? upd(u<<1, l, mid) : upd(u<<1|1, mid+1, r);
            mn[u] = getmin(mn[u<<1], mn[u<<1|1]);
            mx[u] = getmax(mx[u<<1], mx[u<<1|1]);
        }
    }
    int L, R;
    void build(int u=1, int l=1, int r=m) {
        mn[u] = {{m+1,0},{m+1,0}}, mx[u] = {{0,0},{0,0}};
        if (l != r) {
            int mid = l + r >> 1;
            build(u<<1, l, mid);
            build(u<<1|1, mid+1, r);
        }
    }
    Info inqmn(int u=1, int l=1, int r=m) {
        if (L <= l && r <= R) return mn[u];
        int mid = l + r >> 1;
        if (R <= mid) return inqmn(u<<1, l, mid);
        if (mid < L) return inqmn(u<<1|1, mid+1, r);
        return getmin(inqmn(u<<1, l, mid), inqmn(u<<1|1, mid+1, r));
    }
    Info inqmx(int u=1, int l=1, int r=m) {
        if (L <= l && r <= R) return mx[u];
        int mid = l + r >> 1;
        if (R <= mid) return inqmx(u<<1, l, mid);
        if (mid < L) return inqmx(u<<1|1, mid+1, r);
        return getmax(inqmx(u<<1, l, mid), inqmx(u<<1|1, mid+1, r));
    }
} f;
struct Node {
    int l, r, x, id;
    bool operator<(const Node &o) const { return l < o.l || l == o.l && id < o.id; }
} a[N], b[N];
int fa[N];
inline int find(int u) { return u == fa[u] ? u : (fa[u] = find(fa[u])); }
std::map<int, int> vec[N];
int premxr[N];
Pii to[N];
int main() {
    int T, n;
    scanf("%d", &T);
    while (T--) {
        scanf("%d", &n);
        for (int i = 1; i <= n; i++) {
            scanf("%d%d%d", &a[i].l, &a[i].r, &a[i].x);
            all[i] = a[i].x;
        }
        std::sort(a+1, a+1+n);
        std::sort(all+1, all+1+n);
        m = std::unique(all+1, all+1+n) - all-1;
        bool nosol = false;
        for (int i = 1; i <= n; i++) {
            if (i > 1 && a[i].l > premxr[i-1]) {
                nosol = true; break;
            }
            a[i].x = std::lower_bound(all+1, all+1+m, a[i].x) - all, a[i].id = i;
            premxr[i] = std::max(premxr[i-1], a[i].r);
        }
        if (nosol) { puts("-1"); continue; }
        memcpy(b, a, sizeof(Node) * (n+1));
        std::sort(b+1, b+1+n, [](const Node &x, const Node &y) { return x.r < y.r || x.r == y.r && x.id < y.id; });
        std::iota(fa+1, fa+1+n, 1);
        int64_t ans = 0;
        int cnt = 0;
        auto update = [&](int v) {
            switch (vec[v].size()) {
            case 0:  f.X = {{m+1,0},{m+1,0}}, f.Y = {{0,0},{0,0}}; break;
            case 1:  f.X = {{v,vec[v].begin()->first},{m+1,0}}, f.Y = {{v,vec[v].begin()->first},{0,0}}; break;
            default: f.X = f.Y = {{v,vec[v].begin()->first},{v,vec[v].rbegin()->first}};
            }
            f.P = v;
            f.upd();
        };
        while (cnt < n-1) {
            memset(to, 0x3f, sizeof(Pii) * (n+1));
            f.build();
            for (int i = 1; i <= n; i++) std::set<Pii>().swap(st[i]), std::map<int,int>().swap(vec[i]);
            for (int i = 1, j = 1; i <= n; i++) {
                const int rt = find(i);
                for (; j <= n && b[j].r < a[i].l; j++) {
                    if (!--vec[b[j].x][find(b[j].id)]) vec[b[j].x].erase(find(b[j].id));
                    update(b[j].x);
                }
                if (i > 1) {
                    f.L = 1, f.R = a[i].x;
                    auto res1 = f.inqmx();
                    auto pir1 = res1.first.second != rt ? res1.first : res1.second;
                    pir1.first = pir1.second ? all[a[i].x] - all[pir1.first] : (int)2e9;
                    f.L = a[i].x, f.R = m;
                    auto res2 = f.inqmn();
                    auto pir2 = res2.first.second != rt ? res2.first : res2.second;
                    pir2.first = pir2.second ? all[pir2.first] - all[a[i].x] : (int)2e9;
                    to[rt] = std::min(to[rt], std::min(pir1, pir2));
                }
                ++vec[a[i].x][rt];
                update(a[i].x);
            }
            
            f.build();
            for (int i = 1; i <= n; i++) std::set<Pii>().swap(st[i]), std::map<int,int>().swap(vec[i]);
            for (int i = n, j = n; i >= 1; i--) {
                const int rt = find(i);
                for (; j >= 1 && b[j].r < a[i].l; j--) {
                    if (!--vec[a[j].x][find(j)]) vec[a[j].x].erase(find(j));
                    update(a[j].x);
                }
                if (i < n) {
                    f.L = 1, f.R = b[i].x;
                    auto res1 = f.inqmx();
                    auto pir1 = res1.first.second != rt ? res1.first : res1.second;
                    pir1.first = pir1.second ? all[b[i].x] - all[pir1.first] : (int)2e9;
                    f.L = b[i].x, f.R = m;
                    auto res2 = f.inqmn();
                    auto pir2 = res2.first.second != rt ? res2.first : res2.second;
                    pir2.first = pir2.second ? all[pir2.first] - all[b[i].x] : (int)2e9;
                    to[rt] = std::min(to[rt], std::min(pir1, pir2));
                }
                ++vec[b[i].x][rt];
                update(b[i].x);
            }


            std::vector<int> pnts;
            for (int i = 1; i <= n; i++) if (i == find(i)) pnts.push_back(i);
            for (int i : pnts) if (to[i].second != 0x3f3f3f3f)
                to[to[i].second] = std::min(to[to[i].second], {to[i].first, i});
            for (int i : pnts) if (to[i].second != 0x3f3f3f3f && find(i) != find(to[i].second)) {
                ++cnt;
                ans += to[i].first;
                fa[find(to[i].second)] = find(i);
            }
        }
        printf("%lld\n", ans);
    }
}