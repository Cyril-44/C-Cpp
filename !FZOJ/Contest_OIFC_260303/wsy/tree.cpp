#include <bits/stdc++.h>

using namespace std;

#define ll long long

const int N = 262150;

int n, q, a[N];
struct Node {
    ll p[20], c, v;
    void merge(Node &x, Node &y) {
        v = x.v + y.v;
        for (int i = 1; i <= y.c; i++) {
            p[i] = x.p[i] + y.p[i];
        }
        c = y.c;
    }
    void clear() {v = c = 0;}
    void insert(ll x) {
        if (c) {
            if (x < p[1]) v += c * (p[1] - x);
            else v += x - p[1];
        }
        p[++c] = x;
        for (int i = c - 1; i >= 1; i--) {
            if (p[i] > p[i + 1]) swap(p[i], p[i + 1]);
            else break;
        }
    }
    ll getmin() {
        ll sum = v, ans = v, now = 2 - c;
        for (int i = 2; i <= c; i++) {
            sum += (p[i] - p[i - 1]) * now, ans = min(ans, sum), now += 2;
        }
        return ans;
    }
} t[N];

void dfs(int u) {
    if ((u << 1) < (1 << n)) {
        dfs(u << 1);
        dfs(u << 1 | 1);
        t[u].merge(t[u << 1], t[u << 1 | 1]);
    }
    t[u].insert(a[u]);
}

int main() {
    freopen("tree.in", "r", stdin);
    freopen("tree.out", "w", stdout);
    scanf("%d", &n);
    for (int i = 1; i < (1 << n); i++) {
        scanf("%d", &a[i]);
    }
    dfs(1);
    scanf("%d", &q);
    while (q--) {
        int x, y;
        scanf("%d%d", &x, &y);
        a[x] = y;
        while (x) {
            if ((x << 1) < (1 << n)) t[x].merge(t[x << 1], t[x << 1 | 1]);
            else t[x].clear();
            t[x].insert(a[x]);
            x >>= 1;
        }
        printf("%lld\n", t[1].getmin());
    }
    return 0;
}