#include <bits/stdc++.h>
using namespace std;

#define For(i, l, r) for (int i = (l); i <= (r); ++i)
#define roF(i, r, l) for (int i = (r); i >= (l); --i)

constexpr int N = 5005;
constexpr int INF = 1'000'000'000;

int n, T;
pair<int, int> a[N];
vector<int> g[N];
int sz[N], len[N];
int v[N][N], presum[N][N], dp[N][N];
int h[N], f[N], tf[N], st[N];

void dfs(int x) {
    sz[x] = 1;
    for (int y : g[x]) {
        dfs(y);
        sz[x] += sz[y];
    }

    fill(h, h + n + 2, 0);
    h[1] = a[x].second - a[x].first + 1;
    for (int y : g[x]) {
        h[1] -= len[y];
        For(i, 1, len[y])
            ++h[v[y][i] + 1];
    }

    len[x] = 0;
    For(i, 1, n)
        For(j, 1, h[i])
            v[x][++len[x]] = i;

    presum[x][0] = 0;
    partial_sum(v[x] + 1, v[x] + len[x] + 1, presum[x] + 1);

    int *cur = f, *nxt = tf;
    cur[0] = 0;
    int cnt = 0;
    for (int y : g[x]) {
        fill(nxt, nxt + cnt + sz[y] + 1, INF);
        For(i, 0, cnt)
            For(j, 0, sz[y])
                nxt[i + j] = min(nxt[i + j], cur[i] + dp[y][j]);
        cnt += sz[y];
        swap(cur, nxt);
    }

    dp[x][0] = INF;
    For(i, 0, cnt)
        dp[x][i + 1] = cur[i];

    if (x == 0)
        return;

    cur[0] = len[x];
    cnt = 0;
    roF(i, static_cast<int>(g[x].size()) - 1, 0) {
        int y = g[x][i];
        fill(nxt, nxt + cnt + sz[y] + 1, INF);
        For(j, 0, cnt)
            For(k, 0, sz[y]) {
                int z = max(0, len[y] - k);
                int val = presum[y][z] - min(k, len[y]);
                int cost = min(dp[y][k], val);
                nxt[j + k] = min(nxt[j + k], cur[j] + cost);
            }
        cnt += sz[y];
        swap(cur, nxt);
    }

    For(i, 0, cnt)
        dp[x][i] = min(dp[x][i], cur[i]);
}

int main() {
    freopen("war.in", "r", stdin);
    freopen("war.out", "w", stdout);
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int cid;
    cin >> cid >> T;
    while (T--) {
        cin >> n;
        a[0] = {1, n};
        For(i, 1, n)
            cin >> a[i].first >> a[i].second;

        sort(a + 1, a + n + 1, [](const auto& x, const auto& y) {
            if (x.first != y.first)
                return x.first < y.first;
            return x.second > y.second;
        });

        For(i, 0, n)
            g[i].clear();
        int top = 0;
        st[0] = 0;
        For(i, 1, n) {
            while (a[st[top]].second < a[i].first)
                --top;
            g[st[top]].push_back(i);
            st[++top] = i;
        }

        dfs(0);
        For(i, 1, n + 1)
            cout << dp[0][i] << ' ';
        cout << '\n';
    }
}
