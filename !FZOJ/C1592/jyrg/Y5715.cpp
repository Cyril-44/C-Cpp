#include <bits/stdc++.h>
#define ll long long
using namespace std;
template <typename T> void read(T &x) {
    x = 0;
    char ch = getchar();
    while (ch < '0' || ch > '9') ch = getchar();
    while (ch >= '0' && ch <= '9') {
        x = (x << 3) + (x << 1) + ch - 48;
        ch = getchar();
    }
}
int t, n;
ll a[100005], ans;
struct node {
    ll x;
    int id;
} p[100005];
bool cmp(node x, node y) {
    return x.x < y.x;
}
bool check() {
    for (int i = 1; i < n; i++) {
        if (p[i].x == p[i + 1].x) return 0;
    }
    return 1;
}
struct segtree {
    ll mx[400005], lzy[400005];
    void push_up(int x) { mx[x] = max(mx[x << 1], mx[x << 1 | 1]); }
    void push_down(int x) {
        mx[x << 1] += lzy[x], lzy[x << 1] += lzy[x];
        mx[x << 1 | 1] += lzy[x], lzy[x << 1 | 1] += lzy[x];
        lzy[x] = 0;
    }
    void build(int x, int l, int r) {
        lzy[x] = 0;
        if (l == r) {
            mx[x] = a[l] + l;
            return;
        }
        int mid = l + r >> 1;
        build(x << 1, l, mid);
        build(x << 1 | 1, mid + 1, r);
        push_up(x);
    }
    void modify(int x, int l, int r, int w, ll k) {
        if (l == r) {
            mx[x] = k, lzy[x] = 0;
            return;
        }
        push_down(x);
        int mid = l + r >> 1;
        if (mid >= w)
            modify(x << 1, l, mid, w, k);
        else
            modify(x << 1 | 1, mid + 1, r, w, k);
        push_up(x);
    }
    void update(int x, int l, int r, int L, int R, ll k) {
        if (l >= L && r <= R) {
            mx[x] += k, lzy[x] += k;
            return;
        }
        push_down(x);
        int mid = l + r >> 1;
        if (mid >= L) update(x << 1, l, mid, L, R, k);
        if (mid < R) update(x << 1 | 1, mid + 1, r, L, R, k);
        push_up(x);
    }
    ll query(int x, int l, int r, int L, int R) {
        if (l >= L && r <= R) return mx[x];
        push_down(x);
        int mid = l + r >> 1;
        ll cnt = -4e18;
        if (mid >= L) cnt = max(cnt, query(x << 1, l, mid, L, R));
        if (mid < R) cnt = max(cnt, query(x << 1 | 1, mid + 1, r, L, R));
        return cnt;
    }
} seg;
int main() {
    read(t);
    while (t--) {
        read(n);
        for (int i = 1; i <= n; i++) {
            read(a[i]);
            p[i].x = a[i] - i;
            p[i].id = i;
        }
        sort(p + 1, p + n + 1, cmp);
        if (!check()) {
            puts("-1");
            continue;
        }
        seg.build(1, 1, n);
        ans = 0;
        for (int i = 1; i <= n; i++) {
            ll x = (p[i].id < n ? seg.query(1, 1, n, p[i].id + 1, n) : (ll)-4e18);
            ans = max(ans, p[i].x + x - 1);
            seg.modify(1, 1, n, p[i].id, -4e18);
            if (p[i].id < n) seg.update(1, 1, n, p[i].id + 1, n, -2);
        }
        printf("%lld\n", ans);
    }
    return 0;
}