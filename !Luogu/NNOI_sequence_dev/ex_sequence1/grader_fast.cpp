#include <bits/stdc++.h>
std::string Alice(std::vector<int>);
std::vector<int> Bob(int, std::string);
int main() {
    struct FastI {
        char buf[1 << 20], *p1, *p2;
        FastI() : p1(nullptr), p2(nullptr) {}
        __attribute__((always_inline)) int operator() () {
            return (p1 == p2) && (p2 = (p1 = buf) + fread(buf, 1, sizeof buf, stdin), p1 == p2) ? EOF : *p1++;
        }
    } inputS;
    struct FastO {
        char buf[1 << 20], *p1;
        const char *p2;
        FastO() : p1(buf), p2(buf + sizeof buf) {}
        __attribute__((always_inline)) void operator() (int c) {
            *p1++ = c;
            if (p1 == p2) fwrite(buf, sizeof buf, 64, stdout), p1 = buf;
        }
        void close() { fwrite(buf, p1 - buf, 1, stdout); }
    } outputS;
    const auto in = [&inputS] (int &x) -> void {
        char ch = inputS();
        while (ch < '0' || ch > '9') ch = inputS();
        for (x = 0; ch >= '0' && ch <= '9'; ch = inputS())
            x = (x << 3) + (x << 1) + (ch ^ '0');
    };
    const auto out = [&outputS] (int x) -> void {
        static int sta[32];
        if (x < 0) outputS('-'), x = -x;
        sta[sta[0] = 1] = x % 10;
        while (x /= 10) sta[++sta[0]] = x % 10;
        while (sta[0]) outputS(sta[sta[0]--] | '0');
    };
    int t, n;
    in(t);
    while (t--) {
        in(n);
        std::vector<int> a(n);
        for (int i = 0; i < n; i++) in(a[i]);
        std::string data = Alice(std::move(a));
        for (char c : data) outputS(c);
        outputS('\n');
        std::vector<int> ans = Bob(n, std::move(data));
        out(ans.size());
        for (int i : ans) out(i), outputS(' ');
        outputS('\n');
    }
    outputS.close();
    return 0;
}