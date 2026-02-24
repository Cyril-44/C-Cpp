#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef pair<ll, int> pli;
const int N = 1000010, Mod = 998244353;
int n, a[N], b[N];
int aa[N], bb[N];
int ha[N], hb[N];
int pre[N], nxt[N];
struct Seg {
    int l, r;
    bool operator<(const Seg &x) {
        if (x.r - x.l + 1 == r - l + 1)
            return l < x.l;
        else
            return r - l + 1 < x.r - x.l + 1;
    }
} seg[N];
int sz[N];
int fa[N], nd[N], mn[N], mx[N];
void init(int n) {
    for (int i = 0; i <= n; i++) fa[i] = mn[i] = mx[i] = i, nd[i] = 0;
}
int find(int x) {
    if (fa[x] == x) return x;
    return fa[x] = find(fa[x]);
}
bool merge(int x, int y) {
    x = find(x);
    y = find(y);
    if (x == y) return 0;
    fa[y] = x;
    mn[x] = min(mn[x], mn[y]);
    mx[x] = max(mx[x], mx[y]);
    return 1;
}
ll ans;
void work() {
    cin >> n;
    ans = 1;
    aa[0] = bb[n + 1] = 0;
    init(n);
    // memset(sz,0,sizeof(int)*(n+2));
    memset(ha, 0, sizeof(int) * (n + 2));
    memset(hb, 0, sizeof(int) * (n + 2));
    for (int i = 1; i <= n; i++) cin >> a[i];
    for (int i = 1; i <= n; i++) cin >> b[i];
    for (int i = 1; i <= n; i++) {
        if (a[i] == -1)
            aa[i] = aa[i - 1], pre[i] = pre[i - 1];
        else
            aa[i] = a[i], ha[a[i]] = i, pre[i] = i;
    }
    for (int i = n; i >= 1; i--) {
        if (b[i] == -1)
            bb[i] = bb[i + 1], nxt[i] = nxt[i + 1];
        else
            bb[i] = b[i], hb[b[i]] = i, nxt[i] = i;
    }
    aa[n] = bb[1] = n;
    // for(int i=1;i<=n;i++) printf("%d ",aa[i]); putchar('\n');
    // for(int i=1;i<=n;i++) printf("%d ",bb[i]); putchar('\n');
    reverse(bb + 1, bb + n + 1);
    for (int i = 0; i < n; i++) {
        int l = n - (upper_bound(bb + 1, bb + n + 1, i) - bb) + 1, r = upper_bound(aa + 1, aa + n + 1, i) - aa;
        // printf("(%d,%d)",l,r);
        if (ha[i]) l = max(l, pre[r - 1] + 1);
        if (hb[i]) r = min(r, nxt[l + 1] - 1);
        seg[i + 1] = (Seg){l, r};
        // printf("[%d,%d]\n",l,r);
    }
    sort(seg + 1, seg + n + 1);
    for (int i = 1; i <= n; i++) {
        sz[i] = 0;
        for (int j = seg[i].l;;) {
            int cur = find(j);
            if (nd[cur]) sz[i] += sz[nd[cur]];
            if (j != seg[i].l) merge(j - 1, j);
            if (mx[cur] < seg[i].r)
                j = mx[cur] + 1;
            else
                break;
        }
        (ans *= (seg[i].r - seg[i].l + 1 - sz[i])) %= Mod;
        sz[i]++;
        nd[find(seg[i].l)] = i;
    }
    printf("%lld\n", ans);
}
int main() {
    freopen("legend.in", "r", stdin);
    freopen("legend.out", "w", stdout);
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    int t;
    cin >> t;
    while (t--) work();
}