#include <bits/stdc++.h>
constexpr int N = 500005;
std::string t[N];
long long ans[N];

struct BIT {
    BIT(int n_) : tr(new long long[n_+1]()), n(n_) {}
    ~BIT() { delete[] tr; }
    inline void add(int p, long long x) { for (; p <= n; p += p & -p) tr[p] += x; }
    inline long long sum(int p) { long long sum = 0; for (; p > 0; p -= p & -p) sum += tr[p]; return sum; }
    long long *tr;
    int n;
};
struct Query {
    int l, r, id;
    inline bool operator< (const Query &o) const {
        return l < o.l;
    }
} a[N];

std::vector<std::pair<int,int>> subs;
int val[N];

namespace ACAM {
struct Node { int ch[26], fail, cnt; std::vector<int> vec; } ac[N*2];
int tot;
inline void build() {
    static int que[N];
    int hd = 0, tl = 0;
    for (int i = 0; i < 26; i++)
        if (ac[0].ch[i]) que[tl++] = ac[0].ch[i];
    while (hd ^ tl) {
        int u = que[hd++];
        for (int i = 0; i < 26; i++) {
            if (ac[u].ch[i]) ac[ac[u].ch[i]].fail = ac[ac[u].fail].ch[i], que[tl++] = ac[u].ch[i];
            else ac[u].ch[i] = ac[ac[u].fail].ch[i];
        }
    }
}
inline void handleS(const std::string& s) {
    int n = s.length();
    static unsigned long long hash[N], rhash[N], pow[N];
    static constexpr auto Base = 31ull;
    pow[0] = 1;
    for (int i = 1; i <= n; i++) pow[i] = pow[i-1] * Base;
    for (int i = 0; i < n; i++) // 1-index
        hash[i+1] = hash[i] * Base + 1ull*(s[i] - 'a');
    for (int i = n-1; i >= 0; i--) // 0-index
        rhash[i] = rhash[i+1] * Base + 1ull*(s[i] - 'a');
    for (int i = 0; i < n; i++) {
        int u = 0;
        for (int j = i; j < n; j++) {
            int &v = ac[u].ch[s[j] - 'a'];
            if (!v) v = ++tot;
            u = v;
            if (hash[j+1] - hash[i] * pow[j-i+1] == rhash[i] - rhash[j+1] * pow[j-i+1]) {
                ac[u].vec.push_back((int)subs.size());
                subs.emplace_back(i+1, j+1); // 1-Index
            }
        }
    }
    build();
}
inline void handleT(int m) {
    for (int i = 1; i <= m; i++) {
        int u = 0;
        for (const char &c : t[i]) {
            u = ac[u].ch[c - 'a'];
            for (int v = u; v; v = ac[v].fail)
                ++ac[v].cnt;
        }
    }
    for (int i = 1; i <= tot; i++)
        for (int j : ac[i].vec)
            val[j] = ac[i].cnt;
}
}

int main() {
    freopen("palindrome.in", "r", stdin);
    freopen("palindrome.out", "w", stdout);
    std::cin.tie(nullptr) -> sync_with_stdio(false);
    int n, m, q; std::string s;
    std::cin >> n >> m >> q >> s;
    for (int i = 1; i <= m; i++) std::cin >> t[i];
    for (int i = 1; i <= q; i++) {
        a[i].id = i;
        std::cin >> a[i].l >> a[i].r;
    }
    std::sort(a+1, a+1 + q);
    ACAM::handleS(s);
    ACAM::handleT(m);
    BIT f(n);
    for (int i = 0; i < (int)subs.size(); i++) {
        f.add(subs[i].second, val[i]);
        // fprintf(stderr, "Found palin [%d,%d], occured %d times\n", subs[i].first, subs[i].second, val[i]);
    }
    int idx = 0;
    for (int i = 1; i <= q; i++) {
        while (idx < (int)subs.size() && subs[idx].first < a[i].l)
            f.add(subs[idx].second, -val[idx]), ++idx;
        ans[a[i].id] = f.sum(a[i].r);
    }
    for (int i = 1; i <= q; i++)
        printf("%lld\n", ans[i]);
    return 0;
}