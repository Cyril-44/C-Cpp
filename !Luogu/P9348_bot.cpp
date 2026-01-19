#include <bits/stdc++.h>
using namespace std;

/*
  Duval: 返回字符串 s 中普通字典序最小后缀的起点（0-index）。
  复杂度 O(|s|)
*/
static inline int duval_min_suffix(const string &s) {
    int n = (int)s.size();
    if (n == 0) return 0;
    int i = 0, j = 1, k = 0;
    while (i < n && j < n && k < n) {
        int a = i + k, b = j + k;
        if (a >= n || b >= n) break;
        if (s[a] == s[b]) {
            ++k;
            continue;
        }
        if (s[a] < s[b]) {
            j = j + k + 1;
        } else {
            int ni = i + k + 1;
            i = max(ni, j);
            j = i + 1;
        }
        k = 0;
    }
    return min(i, j);
}

/*
  KMP 前缀函数 pi，pi[i] = 最长真前缀也是后缀长度（0-index）
  复杂度 O(|s|)
*/
static inline vector<int> prefix_function(const string &s) {
    int n = (int)s.size();
    vector<int> pi(n, 0);
    for (int i = 1; i < n; ++i) {
        int j = pi[i-1];
        while (j > 0 && s[i] != s[j]) j = pi[j-1];
        if (s[i] == s[j]) ++j;
        pi[i] = j;
    }
    return pi;
}

/*
  从 rep（字符串）沿 pi 跳出所有 border 长度，返回升序的长度列表
  复杂度 O(|rep|)
*/
static inline vector<int> borders_chain_lengths(const string &rep) {
    vector<int> res;
    int m = (int)rep.size();
    if (m == 0) return res;
    auto pi = prefix_function(rep);
    int cur = m;
    while (cur > 0) {
        res.push_back(cur);
        cur = pi[cur-1];
    }
    reverse(res.begin(), res.end());
    return res;
}

/*
  根据前缀最小规则把 s[1..o] 分成 C2（反转后非降）和 D2（保持原序）
  输入 s 为 1-indexed 字符串（s[0] 占位）
  返回 pair(C2, D2)
  复杂度 O(o)
*/
static inline pair<string,string> build_CD_prefix(const string &s, int o) {
    string C2, D2;
    C2.reserve(o); D2.reserve(o);
    int p = 1;
    for (int i = 1; i <= o; ++i) {
        if (s[i] <= s[p]) {
            C2.push_back(s[i]);
            p = i;
        } else {
            D2.push_back(s[i]);
        }
    }
    reverse(C2.begin(), C2.end()); // 操作1会反转，得到非降序 C2
    return {C2, D2};
}

/*
  按字符贪心合并 C (非降序) 与 B (原序)，返回合并结果 E
  规则：每步比较 C[cur] 与 next char of B; 谁小取谁；相等时取 B 的字符
  复杂度 O(|C| + |B|)
*/
static inline string merge_greedy(const string &C, const string &B) {
    string E;
    E.reserve(C.size() + B.size());
    int ci = 0, nc = (int)C.size();
    int bi = 0, nb = (int)B.size();
    while (ci < nc && bi < nb) {
        if (C[ci] < B[bi]) {
            E.push_back(C[ci++]);
        } else {
            // C[ci] >= B[bi], 相等时也取 B（题意）
            E.push_back(B[bi++]);
        }
    }
    while (ci < nc) E.push_back(C[ci++]);
    while (bi < nb) E.push_back(B[bi++]);
    return E;
}

/*
  simulate: 给定分界 o（0 <= o <= n），构造最终 T：
  - 从 s[1..o] 构造 C2, D2
  - B = s[o+1..n]
  - E = merge_greedy(C2, B)
  - T = E + D2
  s 为 1-indexed 输入
*/
static inline string simulate_split(const string &s, int n, int o) {
    auto cd = build_CD_prefix(s, o);
    const string &C2 = cd.first;
    const string &D2 = cd.second;
    string B;
    if (o < n) B = s.substr(o+1); else B.clear(); // s is 1-indexed with s[0] placeholder
    string E = merge_greedy(C2, B);
    E += D2;
    return E;
}

/* -------------------- 主程序 -------------------- */
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int T;
    if (!(cin >> T)) return 0;
    while (T--) {
        string s0; cin >> s0;
        int n = (int)s0.size();
        // make 1-indexed string with dummy at s[0]
        string s; s.reserve(n+1);
        s.push_back(' '); // dummy
        s += s0;

        // 1) 找到 pos = 最后一个前缀最小位置（题中固定到全局最小最后一次出现）
        string C, D;
        C.reserve(n); D.reserve(n);
        int pos = 1;
        for (int i = 1; i <= n; ++i) {
            if (s[i] <= s[pos]) {
                C.push_back(s[i]);
                for (int j = pos+1; j < i; ++j) D.push_back(s[j]);
                pos = i;
            }
        }
        reverse(C.begin(), C.end()); // C 非降
        // T0 = C + D + suffix from pos+1..n
        string T0; T0.reserve(n);
        T0 += C; T0 += D;
        for (int i = pos+1; i <= n; ++i) T0.push_back(s[i]);

        // X = suffix after pos in T0 (0-indexed relative to T0)
        string X;
        if (pos < n) {
            // T0 的 pos 对应原 s 的 pos+? 但我们只需要 X 内容
            X = T0.substr(pos);
        } else X.clear();

        // slen: 存候选后缀长度集合（相对于 X 的长度）
        vector<int> slen;
        slen.push_back(0); // 空长度作为候选（表示不取 B）
        if (!X.empty()) {
            int st = duval_min_suffix(X);
            string rep = X.substr(st);
            auto lens = borders_chain_lengths(rep);
            for (int L : lens) slen.push_back(L);
        }
        int ns = (int)slen.size() - 1;

        // ban 标记等差中间项，减少枚举
        vector<char> ban(max(0, ns+2), 0);
        for (int i = 2; i < ns; ++i) {
            if (slen[i] - slen[i-1] == slen[i+1] - slen[i]) ban[i] = 1;
        }

        string ans;
        bool has = false;

        // 枚举被保留的 slen 项（映射到原串分界 o = n - L）
        for (int i = 0; i <= ns; ++i) {
            // 保留端点与非被 ban 的项（与原代码逻辑一致）
            if (i <= 1 || i == ns || !ban[i-1] || !ban[i]) {
                int L = slen[i];
                int o = n - L; // 分界位置 o (0..n)
                string cand = simulate_split(s, n, o);
                if (!has || cand < ans) { ans = move(cand); has = true; }
            }
        }

        cout << ans << '\n';
    }
    return 0;
}
