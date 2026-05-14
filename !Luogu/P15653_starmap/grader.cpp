#include <bits/stdc++.h>
#include "starmap.h"

namespace grader {
    int n, k, p, m, c;
    struct Bitset {
        std::vector<uint64_t> b;
        Bitset(int n) {b.resize((n + 63) >> 6);}
        void set(int x) {b[x >> 6] |= 1ull << (x & 63);}
        void reset(int x) {b[x >> 6] &= ~(1ull << (x & 63));}
        int ask(int x) {return b[x >> 6] >> (x & 63) & 1;}
        void operator^=(const Bitset &B) {
            for (size_t i = 0; i < b.size(); i++)
                b[i] ^= B.b[i];
        }
        int count() {
            int r = 0;
            for (size_t i = 0; i < b.size(); i++)
                r += __builtin_popcountll(b[i]);
            return r;
        }
    };
    std::vector<Bitset> B;
    void ensure(bool expr) {
        if (!expr) {
            std::cout << "Invalid operation." << std::endl;
            exit(0);
        }
    }
    void init() {
        m = 0;
        c = -1;
        B.assign(n, n);
    }
    void add(int u, int v) {
        B[--u].set(--v);
    }
    void report(int c) {
        ensure(grader::c == -1);
        ensure(0 <= c and c <= n * (n - 1) / 2);
        grader::c = c;
    }
    void invert(const std::vector<int> &s) {
        ensure(c != -1);
        ensure(static_cast<int>(s.size()) == k);
        ensure(m < p);
        m++;
        Bitset F(n);
        for (int i = 0; i < k; i++) {
            ensure(1 <= s[i] and s[i] <= n and !F.ask(s[i] - 1));
            F.set(s[i] - 1);
        }
        for (int i = 0; i < n; i++)
            if (F.ask(i)) {
                F.reset(i);
                B[i] ^= F;
            }
    }
    void output() {
        int r = 0;
        for (int i = 0; i < n; i++)
            r += B[i].count();
        std::cout << c << ' ' << (r == c) << std::endl;
    }
}
void report(int c) {
    grader::report(c);
}
void invert(std::vector<int> s) {
    grader::invert(s);
}

signed main() {
    int c, t;
    std::cin >> c >> t;
    init(c, t);
    for (int i = 1; i <= t; i++) {
        int n, m, k, p;
        std::cin >> n >> m >> k >> p;
        grader::n = n;
        grader::k = k;
        grader::p = p;
        std::vector<int> u(m), v(m);
        grader::init();
        for (int j = 0; j < m; j++)
            std::cin >> u[j] >> v[j], grader::add(u[j], v[j]);
        starmap(n, m, k, p, u, v);
        grader::output();
    }
    return 0;
}
