#include <bits/stdc++.h>
constexpr int N = 100005;
int a[N];
int n, m;
std::vector<uint16_t> allks;
struct Query {
    int l, r, lid, rid, id;
    friend bool operator<(const Query& p, const Query& q) {
        return p.lid < q.lid || p.lid == q.lid && (p.lid&1 ? p.r < q.r : p.r > q.r);
    }
} q[N];
struct Query2 { int id, l, r; };
std::vector<Query2> scan[N];
int64_t fsum1[N], fsum2[N], ans[N], ans2[N];
int h[1<<14];
int main() {
    int K;
    scanf("%d%d%d", &n, &m, &K);
    for (uint16_t i = 0; !(i >> 14); i++)
        if (__builtin_popcount(i) == K)
            allks.push_back(i);
    int B = sqrt(n);
    for (int i = 1; i <= n; i++) {
        scanf("%d", &a[i]);
        for (auto val : allks)
            fsum1[i] += h[val ^ a[i]];
        ++h[a[i]];
        for (auto val : allks)
            fsum2[i] += h[val ^ a[i]];
        fprintf(stderr, "%lld %lld\n", fsum1[i], fsum2[i]);
        fsum1[i] += fsum1[i-1];
        fsum2[i] += fsum2[i-1];
    }
    
    for (int i = 1; i <= m; i++) {
        scanf("%d%d", &q[i].l, &q[i].r);
        q[i].lid = q[i].l / B, q[i].rid = q[i].r / B;
        q[i].id = i;
    }
    std::sort(q+1, q+1+m);
    
    int l = 1, r = 0;
    for (int i = 1; i <= m; i++) {
        if (l > q[i].l) { // fs([ql, l-1], r) - fs(l-1, l-1) - fs(l-2, l-2) - ... - fs(ql, ql)
            scan[r].push_back({i, q[i].l, l-1});
            ans[i] -= fsum2[l-1] - fsum2[q[i].l-1];
            l = q[i].l;
        }
        if (r < q[i].r) { // fs(r+1, r) + fs(r+2, r+1) + ... + fs(qr, qr-1) - fs([r+1, qr], l-1)
            ans[i] += fsum1[q[i].r] - fsum1[r];
            scan[l-1].push_back({-i, r+1, q[i].r});
            r = q[i].r;
        }
        if (l < q[i].l) { // - fs([l, ql-1], r) + fs(l, l) + fs(l+1, l+1) + ... + fs(ql-1, ql-1)
            scan[r].push_back({-i, l, q[i].l-1});
            ans[i] += fsum2[q[i].l-1] - fsum2[l-1];
            l = q[i].l;
        }
        if (r > q[i].r) { // - fs(r, r-1) - fs(r-1, r-2) - ... - fs(qr+1, qr) + fs([qr+1, r], l-1)
            ans[i] -= fsum1[r] - fsum1[q[i].r];
            scan[l-1].push_back({i, q[i].r+1, r});
            r = q[i].r;
        }
    }
    memset(h, 0, sizeof h);
    for (int i = 1; i <= n; i++) {
        for (auto val : allks)
            ++h[a[i] ^ val];
        for (const Query2& qry : scan[i]) {
            int64_t sum = 0;
            for (int j = qry.l; j <= qry.r; j++)
                sum += h[a[j]];
            if (qry.id < 0) ans[-qry.id] -= sum;
            else ans[qry.id] += sum;
        }
    }
    for (int i = 1; i <= m; i++) ans[i] += ans[i-1];
    for (int i = 1; i <= m; i++) ans2[q[i].id] = ans[i];
    for (int i = 1; i <= m; i++) printf("%lld\n", ans2[i]);
    return 0;
}