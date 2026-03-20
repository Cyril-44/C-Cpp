#ifndef GRADER_H
#define GRADER_H

#include <bits/stdc++.h>
using namespace std;

namespace {
    constexpr int LMT = 10001;
    struct S {
        vector<int> p;
        int n;
        long long qc;
        uint64_t key;
        bool accepted;
    };

    S& G() {
        static S s;
        return s;
    }

    uint64_t mix(uint64_t x) {
        x ^= (x >> 30);
        x *= 0xbf58476d1ce4e5b9ULL;
        x ^= (x >> 27);
        x *= 0x94d049bb133111ebULL;
        x ^= (x >> 31);
        return x;
    }

    void init_local(int n, int mode, uint64_t seed) {
        auto &g = G();
        g.n = n;
        g.key = mix(seed ^ 0x9e3779b97f4a7c15ULL);
        g.qc = 0;

        vector<int> a(n);
        iota(a.begin(), a.end(), 1);

        uint64_t r = seed ^ (g.key * (mode + 1));
        for (int i = n - 1; i >= 1; i--) {
            r = mix(r);
            int j = r % (i + 1);
            swap(a[i], a[j]);
        }

        g.p = a;
    }
}

int query(const vector<int>& q) {
    auto &g = G();
    if ((int)q.size() != g.n) (cout << (1ll << 32) << endl), exit(0);
    std::vector<bool> f(g.n);
    for (int i : q) {
        if (i < 1 || i > g.n) (cout << (1ll << 34) << endl), exit(0);
        if (f[i-1]) (cout << (1ll << 35) << endl), exit(0);
        f[i-1] = true;
    }
    if (++g.qc == LMT) (cout << (1ll << 33) << endl), exit(0);

    int m = 0;
    for (int i = 0; i < g.n; i++)
        m += (q[i] == g.p[i]);
    if (m == g.n) g.accepted = true;
    return m;
}

void solve(int); // 选手实现

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int T; cin >> T;
    while (T--) {
        int n, mode;
        uint64_t seed;
        cin >> n >> mode >> seed;

        init_local(n, mode, seed);

        solve(n);

        auto &g = G();
        if (!g.accepted) cout << -g.qc << endl;
        else cout << g.qc << endl;
        cout.flush();
    }
    return 0;
}

#endif
