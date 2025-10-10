#include <bits/stdc++.h>
using namespace std;
constexpr int N = 200005;
int a[N], b[N], p[N];
bool vis[N];
int main() {
    cin.tie(nullptr) -> sync_with_stdio(false);
    int T, n;
    cin >> T;
    while (T--) {
        cin >> n;
        for (int i = 1; i <= n; i++) cin >> a[i];
        int m = unique(a+1, a+1 + n) - a-1;
        while (m > 0 && a[m] == a[1]) --m;
        a[++m] = a[1];
        for (int i = 1; i < m; i++)
            b[i] = abs(a[i+1] - a[i]);
        memset(vis, 0, sizeof(bool) * (m+1));
        iota(p+1, p + m, 1);
        sort(p+1, p + m, [](int x, int y){return b[x] < b[y];});
        int cnt = 0;
        long long ans = 0;
        for (int i = 1; i < m; i++) {
            if (vis[p[i]] || vis[p[i]+1]) continue;
            ans += b[p[i]]; ++cnt;
            vis[p[i]] = vis[p[i] + 1] = true;
            if (cnt > (m-1 >> 1)) break;
        }
        if (m-1 & 1) {
            int unvis = -1;
            for (int i = 1; i < m; i++)
                if (!vis[i]) { unvis = i; break; }
            auto fix = [m](int x, int a) {
                x += a;
                if (x >= m) x -= m-1;
                else if (x < 1) x += m-1;
                return x;
            };
            ans += min({b[unvis], b[fix(unvis, -1)], abs(a[unvis] - a[fix(unvis, -2)]), abs(a[unvis] - a[fix(unvis, 2)])});
        }
        cout << ans << '\n';
    }
    return 0;
}