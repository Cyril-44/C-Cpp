#include <bits/stdc++.h>
#include "perm.h"

namespace grader {
    int n, m;
    std::vector<int> p;
    int ask(std::vector<int> w) {
        std::vector<int> s(n);
        for (int x: w) s[x] = 1;
        for (int i = 0; i < n; i++)
            if (!s[i])
                return i;
        return n;
    }
    int query(int l, int r) {
        if (l < 0 or r > n - 1 or l > r) {
            std::cout << "Invalid operation." << std::endl;
            exit(0);
        }
        m++;
        return ask(std::vector<int>(p.begin() + l, p.begin() + r + 1));
    }
}
int query (int l, int r) {
    return grader::query(l, r);
}

signed main() {
    int c, t, m = 0;
    std::cin >> c >> t;
    init(c, t);
    for (int i = 1; i <= t; i++) {
        int n;
        std::cin >> n;
        std::vector<int> p(n);
        for (int j = 0; j < n; j++) std::cin >> p[j];
        grader::n = n;
        grader::m = 0;
        grader::p = p;
        auto q = perm(n);
        if (static_cast<int>(q.size()) != n) {
            std::cout << "Invalid operation." << std::endl;
            return 0;
        }
        std::vector<int> f(n);
        for (int j = 0; j < n; j++) {
            if (q[j] < 0 or q[j] > n - 1 or f[q[j]]) {
                std::cout << "Wrong answer." << std::endl;
                return 0;
            }
            f[q[j]] = 1;
        }
        for (int l = 0; l < n; l++)
            for (int r = l + 1; r < n; r++) {
                int resp = grader::ask(std::vector<int>(p.begin() + l, p.begin() + r + 1));
                int resq = grader::ask(std::vector<int>(q.begin() + l, q.begin() + r + 1));
                if (resp != resq) {
                    std::cout << "Wrong answer." << std::endl;
                    return 0;
                }
            }
        m = std::max(m, grader::m);
    }
    std::cout << "Correct.\n" << m << std::endl;
    return 0;
}
