#include <bits/stdc++.h>
using namespace std;

using ll = long long;
using u64 = unsigned long long;

// splitmix64 hash
struct CustomHash {
    static uint64_t splitmix64(uint64_t x) {
        x += 0x9e3779b97f4a7c15ULL;
        x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9ULL;
        x = (x ^ (x >> 27)) * 0x94d049bb133111ebULL;
        return x ^ (x >> 31);
    }
    size_t operator()(uint64_t x) const {
        static const uint64_t FIXED_RANDOM =
            chrono::steady_clock::now().time_since_epoch().count();
        return splitmix64(x + FIXED_RANDOM);
    }
};

struct Eertree {
    int n;
    vector<int> a;                 // compressed values
    vector<int> len, link, firstPos;
    vector<char> good;
    vector<int> lastOrd;           // longest pal suffix node at each position

    // all transitions in one hash table: key = (node << 32) | ch
    unordered_map<u64, int, CustomHash> nxt;

    int last; // current longest pal suffix
    int nodes;

    Eertree(const vector<int>& arr) : n((int)arr.size()), a(arr) {
        // roots:
        // 0: len = -1
        // 1: len = 0
        len = {-1, 0};
        link = {0, 0};
        firstPos = {-1, -1};
        good = {0, 1}; // empty string root is good
        last = 1;
        nodes = 2;
        lastOrd.reserve(n);
        nxt.reserve((size_t)n * 2 + 10);
    }

    static u64 key(int node, int ch) {
        return (u64(uint32_t(node)) << 32) | uint32_t(ch);
    }

    int get_next(int node, int ch) const {
        auto it = nxt.find(key(node, ch));
        if (it == nxt.end()) return -1;
        return it->second;
    }

    void set_next(int node, int ch, int to) {
        nxt[key(node, ch)] = to;
    }

    int get_fail(int v, int pos) const {
        // find largest v such that a[pos-1-len[v]] == a[pos]
        while (pos - 1 - len[v] < 0 || a[pos - 1 - len[v]] != a[pos]) {
            v = link[v];
        }
        return v;
    }

    void add_char(int pos, const vector<int>& incEnd) {
        int c = a[pos];
        int cur = get_fail(last, pos);

        int nx = get_next(cur, c);
        if (nx != -1) {
            last = nx;
            lastOrd.push_back(last);
            return;
        }

        int now = nodes++;
        len.push_back(len[cur] + 2);
        link.push_back(0);
        firstPos.push_back(pos);
        good.push_back(0);
        set_next(cur, c, now);

        if (len[now] == 1) {
            link[now] = 1;
        } else {
            int q = get_fail(link[cur], pos);
            link[now] = get_next(q, c);
        }

        // judge whether this palindrome node is good
        int L = len[now];
        int h = (L + 1) / 2;
        int mid = pos - L + h; // 0-based index of the end of left half
        if (incEnd[mid] >= h) good[now] = 1;

        last = now;
        lastOrd.push_back(last);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int Cid,T;
    cin >> Cid>>T;
    while (T--) {
        int n;
        string s;
        cin >> s;
        n = s.length();
        vector<ll> raw(n);
        for (int i = 0; i < n; ++i) raw[i] = s[i] - 'a' + 1;

        // coordinate compression
        vector<ll> xs = raw;
        sort(xs.begin(), xs.end());
        xs.erase(unique(xs.begin(), xs.end()), xs.end());
        vector<int> a(n);
        for (int i = 0; i < n; ++i) {
            a[i] = lower_bound(xs.begin(), xs.end(), raw[i]) - xs.begin();
        }

        // incEnd[i] = length of longest nondecreasing suffix ending at i
        vector<int> incEnd(n);
        for (int i = 0; i < n; ++i) {
            if (i > 0 && a[i - 1] <= a[i]) incEnd[i] = incEnd[i - 1] + 1;
            else incEnd[i] = 1;
        }

        // build eertree
        Eertree et(a);
        for (int i = 0; i < n; ++i) {
            et.add_char(i, incEnd);
        }

        int tot = et.nodes;
        int LOG = 1;
        while ((1 << LOG) <= tot) ++LOG;

        // binary lifting on suffix links
        vector<vector<int>> up(LOG, vector<int>(tot, 0));
        up[0] = et.link;
        for (int k = 1; k < LOG; ++k) {
            for (int v = 0; v < tot; ++v) {
                up[k][v] = up[k - 1][up[k - 1][v]];
            }
        }

        auto nearest_good_ancestor = [&](int v) {
            if (et.good[v]) return v;
            int u = v;
            for (int k = LOG - 1; k >= 0; --k) {
                int anc = up[k][u];
                if (anc != 0 && !et.good[anc]) u = anc;
            }
            // now parent of u should be the first good ancestor
            return et.link[u];
        };

        vector<int> lastGood(n);
        for (int i = 0; i < n; ++i) {
            lastGood[i] = nearest_good_ancestor(et.lastOrd[i]);
        }

        // series links and diff
        vector<int> diff(tot, 0), seriesLink(tot, 0);
        for (int v = 0; v < tot; ++v) {
            if (et.len[v] <= 0) {
                diff[v] = 0;
                seriesLink[v] = 0;
            } else {
                diff[v] = et.len[v] - et.len[et.link[v]];
                if (diff[v] == diff[et.link[v]]) seriesLink[v] = seriesLink[et.link[v]];
                else seriesLink[v] = et.link[v];
            }
        }

        const int INF = 1e9;
        vector<int> dp(n + 1, INF);
        dp[0] = 0;

        vector<int> seriesAns(tot, INF);

        for (int i = 1; i <= n; ++i) {
            int v = lastGood[i - 1];
            dp[i] = INF;

            // standard eertree partition DP on good suffix chain
            for (int u = v; et.len[u] > 0; u = seriesLink[u]) {
                int sl = seriesLink[u];
                int pos = i - (et.len[sl] + diff[u]);
                seriesAns[u] = dp[pos];

                if (diff[u] == diff[et.link[u]]) {
                    seriesAns[u] = min(seriesAns[u], seriesAns[et.link[u]]);
                }

                dp[i] = min(dp[i], seriesAns[u] + 1);
            }
        }

        cout << dp[n] << '\n';
    }
    return 0;
}