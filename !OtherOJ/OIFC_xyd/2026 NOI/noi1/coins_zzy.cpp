#include <bits/stdc++.h>
#define ll long long
#define pr pair<int, int>
#define mk make_pair
using namespace std;
template <typename T> void in(T &x) {
    int f = 1;
    char c = getchar();
    while (!isdigit(c) && c != '-') c = getchar();
    if (c == '-') {
        c = getchar();
        f = -1;
    }
    for (x = 0; isdigit(c); c = getchar()) x = x * 10 + c - '0';
    x *= f;
}
const int N = 1e5 + 5, K = 11, B = 10000;
int n, m, k;
ll ans[N];
int p[N], b[N], bb[N], b_[N], c[N];
bool ok[N], is[N];
struct node {
    int i, x, y;
};
vector<node> U[N], Q;
int bel[N];
bool cmp(node a, node b) {
    if (bel[a.x] ^ bel[b.x])
        return a.x < b.x;
    else if (bel[a.y] ^ bel[b.y])
        return a.y < b.y;
    else
        return (bel[a.y] & 1) ^ (a.i < b.i);
}
int t[3628800];
ll cur;
void add(int x) {
    cur += t[x]++;
}
void del(int x) {
    cur -= --t[x];
}
const int fac[K] = {1, 1, 2, 6, 24, 120, 720, 5040, 40320, 362880, 3628800};
int getrk(int *p) {
    int res = 0;
    for (int i = k - 1; i >= 0; i--)
        for (int j = i + 1; j < k; ++j)
            if (p[j] < p[i]) res += fac[k - 1 - i];
    return res;
}
struct BIT {
    int tr[N];
    void upd(int x, int s) {
        for (; x <= n; x += x & -x) tr[x] += s;
    }
    int que(int x) {
        int res = 0;
        for (; x; x -= x & -x) res += tr[x];
        return res;
    }
} A;
int main() {
    freopen("stars.in", "r", stdin);
    freopen("stars.out", "w", stdout);
    in(n), in(m), in(k);
    for (int i = 1; i <= n; ++i) bel[i] = (i - 1) / B + 1;
    for (int i = 1; i <= n; ++i) in(p[i]);
    for (int i = 1; i <= n - k + 1; ++i) b[i] = getrk(p + i), bb[i] = b[i];
    for (int i = 1; i <= n - k + 1; ++i) {
        for (int j = i + 1; j <= i + k - 1 && j <= n - k + 1; ++j) c[i] += b[i] == b[j];
        A.upd(i, c[i]);
    }

    for (int i = 1, op, x, y; i <= m; ++i) {
        in(op), in(x), in(y);
        if (op == 1) {
            swap(p[x], p[y]);
            vector<int> id;
            for (int j = max(1, x - k + 1); j <= x && j <= n - k + 1; ++j) ok[j] = 1, b_[j] = getrk(p + j);
            for (int j = max(x + 1, y - k + 1); j <= y && j <= n - k + 1; ++j) ok[j] = 1, b_[j] = getrk(p + j);
            for (int j = max(1, x - 2 * k + 1); j <= x && j <= n - k + 1; ++j) id.push_back(j);
            for (int j = max(x + 1, y - 2 * k + 1); j <= y && j <= n - k + 1; ++j) id.push_back(j);
            for (int u : id) {
                A.upd(u, -c[u]), c[u] = 0;
                for (int j = u + 1; j <= u + k - 1 && j <= n - k + 1; ++j)
                    c[u] += (ok[u] ? b_[u] : b[u]) == (ok[j] ? b_[j] : b[j]);
                A.upd(u, c[u]);
            }
            for (int u : id)
                if (ok[u]) ok[u] = 0, U[i].push_back((node){u, b[u], b_[u]}), b[u] = b_[u];
        } else {
            is[i] = 1;
            if (y - x + 1 < 2 * k) continue;
            ans[i] = A.que(y - k + 1) - A.que(x - 1);
            for (int j = y - 2 * k + 1; j <= y - k + 1; ++j)
                for (int z = y - k + 2; z <= j + k - 1 && z <= n - k + 1; ++z) ans[i] -= b[j] == b[z];
            ans[i] = -ans[i];
            Q.push_back((node){i, x, y});
        }
    }
    sort(Q.begin(), Q.end(), cmp);
    memcpy(b, bb, sizeof(b));
    int l = 1, r = 0, t = 0;
    for (node now : Q) {
        while (r < now.y)
            if ((++r) - l + 1 >= k) add(b[r - k + 1]);
        while (l > now.x)
            if (r - (--l) + 1 >= k) add(b[l]);
        while (r > now.y)
            if ((r--) - l + 1 >= k) del(b[r + 1 - k + 1]);
        while (l < now.x)
            if (r - (l++) + 1 >= k) del(b[l - 1]);
        while (t < now.i)
            if (!is[++t])
                for (node nn : U[t]) {
                    if (l <= nn.i && nn.i <= r - k + 1) del(nn.x), add(nn.y);
                    b[nn.i] = nn.y;
                }
        while (t > now.i)
            if (!is[t--])
                for (node nn : U[t + 1]) {
                    if (l <= nn.i && nn.i <= r - k + 1) del(nn.y), add(nn.x);
                    b[nn.i] = nn.x;
                }
        ans[now.i] += cur;
    }
    for (int i = 1; i <= m; ++i)
        if (is[i]) printf("%lld\n", ans[i]);
    return 0;
}