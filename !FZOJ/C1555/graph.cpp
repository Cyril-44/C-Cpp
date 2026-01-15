#include <stdio.h>
#include <algorithm>
#include <vector>
constexpr int N = (int)1e6 + 5, M = (int)2e6 + 5;
int a[N];
bool h[M];
namespace FastI {
    char buf[1 << 20], *p1{}, *p2{};
    inline char gc() {
        if (p1 == p2) p2 = (p1 = buf) + fread(buf, 1, sizeof buf, stdin);
        return *p1++;
    }
    inline void in(int &x) {
        char ch = gc();
        while (ch < '0' || ch > '9') ch = gc();
        for (x = 0; ch >= '0' && ch <= '9'; ch = gc())
            x = (x << 1) + (x << 3) + (ch ^ '0');
    }
    inline void in(bool &x) {
        char ch = gc();
        while ((ch ^ '0') && (ch ^ '1')) ch = gc();
        x = ch & 1;
    }
} using FastI::in;
inline void bruteForceSolve(int n) {
    std::vector<int> f1(n+1), g1(n+1), f0(n+1), g0(n+1);
    std::vector<std::vector<int>> h0, h1;
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j < i; j++) {
            if (h[std::__gcd(a[i], a[j])])
                h1[i].push_back(j);
            else h0[j].push_back(i);
        }
    }
    for (int i = 1; i <= n; i++) {
        
    }
}
int main() {
    int n, m;
    in(n), in(m);
    for (int i = 1; i <= m; i++) in(h[i]);
    for (int i = 1; i <= n; i++) in(a[i]);
    std::sort(a+1, a+1 + n);
    return 0;
}