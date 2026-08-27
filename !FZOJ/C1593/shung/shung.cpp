#include <cstdio>
#include <cstring>
#include <algorithm>
#include <map>
#include <set>
constexpr int N = 200002;
int p[N], pre[N], nxt[N], linc[N], ldec[N], l[N], n;
struct BIT {
    int tr[N];
    void upd(int p, int x) { for (; p <= n; p += p & -p) tr[p] = std::max(tr[p], x); }
    int max(int p) { int res = 0; for (; p > 0; p -= p & -p) res = std::max(res, tr[p]); return res; }
} rsinc, rsdec; // 倒过来升序/降序

int main() {
    scanf("%d", &n);
    for (int i = 1; i <= n; i++)
        scanf("%d", &p[i]);
    std::set<int> st({0, n+1});
    for (int i = 1; i <= n; i++) {
        auto it = st.upper_bound(p[i]);
        nxt[i] = *it;
        pre[i] = *std::prev(it);
        l[i] = (linc[p[i]] = linc[pre[i]] + 1) + (ldec[p[i]] = ldec[nxt[i]] + 1) - 1;
        st.insert(p[i]);
    }
    int ans = 0;
    for (int i = n; i >= 1; i--) {
        ans = std::max({ans, l[i] + rsinc.max(nxt[i]) + rsdec.max(n - nxt[i] + 1), l[i] + rsdec.max(n - pre[i] + 1) + rsinc.max(pre[i])});
        rsinc.upd(p[i], rsinc.max(p[i]) + 1);
        rsdec.upd(n - p[i] + 1, rsdec.max(n - p[i] + 1) + 1);
    }
    printf("%d\n", ans);
    return 0;
}