#include <bits/stdc++.h>
using namespace std;
constexpr int N = 5000005;
constexpr uint64_t B1 = 403, B2 = 471, P1 = (uint64_t)4e15 + 7, P2 = (uint64_t)5e16 + 3;
inline uint64_t xorshift64(uint64_t x) {
    static const uint64_t FixedRandom = mt19937_64(time(nullptr))();
    x ^= FixedRandom;
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    x ^= FixedRandom;
    return x;
}
int ts=0;
struct Tr {
    map<char, int> son;
    vector<string> ssuf;
    vector<pair<string, int>> tsuf;
} tr[N];
struct Trie {
    map<char, int> son;
    int cnt; // Trie 树上 text cnt 的前缀和
} trie[N];
inline uint64_t gethash(const string& s1, const string& s2, int l, int r) {
    uint64_t hash(0ULL);
    for (int i = l; i <= r; i++)
        hash = xorshift64(hash) + ((s1[i] - 'a') * 31 + (s2[i] - 'a'));
    return hash;
}
inline void ins(int& u, const string& str, const int l) {
    for (int i = l-1; i >= 0; i--) {
        int &v = tr[u].son[str[i]];
        if (!v) v = ++ts;
        u = v;
    }
}
inline void ins(int u, const string& str, const int l, const int r) {
    ins(u, str, l);
    tr[u].ssuf.emplace_back(str.begin() + r + 1, str.end());
}
inline void ins(int u, const string& str, const int l, const int r, const int id) {
    ins(u, str, l);
    tr[u].tsuf.emplace_back(string(str.begin() + r + 1, str.end()), id);
}
int ans[N], rt;
void dfs(int now) {
    vector<int> sids;
    sids.reserve(tr[now].ssuf.size());
    for (const string& str : tr[now].ssuf) {
        int u = rt;
        for (char ch : str) {
            int &v = trie[u].son[ch];
            if (!v) v = ++ts;
            u = v;
        }
        sids.push_back(u);
        ++trie[u].cnt;
    }
    for (const auto& [str, id] : tr[now].tsuf) { // 查询串的答案贡献一定是查询串的子串，即前面 dfs 的结果。
        int u = rt;
        bool find = true;
        ans[id] = trie[u].cnt;
        for (char ch : str) {
            u = trie[u].son[ch];
            if (!u) break;
            ans[id] += trie[u].cnt;
        }
    }
    for (const auto& [ch, v] : tr[now].son)
        dfs(v);
    for (int u : sids) --trie[u].cnt; // 只保留当前 dfs 上的答案
}
int main() {
    unordered_map<uint64_t, int> mp;
    string s1, s2;
    int n, m;
    cin >> n >> m;
    for (int i = 1; i <= n; i++) {
        cin >> s1 >> s2;
        if (s1 == s2) continue;
        int l = 0, r = s1.length() - 1;
        while (s1[l] == s2[l]) ++l;
        while (s1[r] == s2[r]) --r;
        // cerr << l << ' ' << r << endl;
        int &u = mp[gethash(s1, s2, l, r)];
        if (!u) u = ++ts;
        ins(u, s1, l, r);
    }
    for (int i = 1; i <= m; i++) {
        cin >> s1 >> s2;
        if (s1.length() != s2.length()) continue;
        int l = 0, r = s1.length() - 1;
        while (s1[l] == s2[l]) ++l;
        while (s1[r] == s2[r]) --r;
        // cerr << l << ' ' << r << endl;
        int &u = mp[gethash(s1, s2, l, r)];
        if (!u) u = ++ts;
        ins(u, s1, l, r, i);
    }
    ts = 0;
    for (auto [hash, u] : mp) dfs(u);
    for (int i = 1; i <= m; i++)
        printf("%d\n", ans[i]);
    return 0;
}
