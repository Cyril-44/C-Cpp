#include <bits/stdc++.h>
constexpr int N = 5005;
int a[N];
namespace FastI {
    char buf[1 << 20], *p1, *p2;
    [[gnu::always_inline]] inline char gc() {
        if (p1 == p2) [[unlikely]] p2 = (p1=buf) + fread(buf, 1, sizeof buf, stdin);
        return *p1++; 
    }
    inline void in(int &x) {
        char ch = gc();
        while (ch < '0' || ch > '9') ch = gc();
        for (x = 0; ch >= '0' && ch <= '9'; ch = gc())
            x = (x << 3) + (x << 1) + (ch ^ '0');
    }
} using FastI::in;
namespace FastO {
    char buf[1 << 20], *p1=buf;
    const char* p2 = buf + sizeof(buf);
    [[gnu::always_inline]] inline void pc(char x) {
        if (p1 == p2) [[unlikely]] fwrite(buf, sizeof buf, 1, stdout), p1 = buf;
        *p1++ = x;
    }
    struct Cleaner { ~Cleaner() { fwrite(buf, p1-buf, 1, stdout); } } _cleaner;
    inline void out(int x, bool f) {
        static int sta[15];
        sta[sta[0] = 1] = x % 10;
        while (x /= 10) sta[++sta[0]] = x % 10;
        while (sta[0]) pc(sta[sta[0]--] | '0');
        pc(f ? '\n' : ' ');
    }
} using FastO::out;
int main() {
    std::mt19937 rng(std::random_device{}());
    int T = 11 * 10 * 9 * 8 * 7 * 6 * 5 * 4 * 3 * 2 * 1;
    out(T, 1);
    int n = 11;
    std::iota(a+1, a+1+n, 1);
    int num = 0;
    do {
        out(n, 0);
        for (int i = 1; i <= n; i++)
            out(a[i], i==n);
        if (0 == (++num & ((1<<20)-1))) fprintf(stderr, "Done %d\n", num);
    } while (std::next_permutation(a+1, a+1+n));
    // if (system("./bf < test.in > test.ans") || system("./vspirit < test.in > test.out")) {
    //     puts("Runtime Error!\n");
    //     return 1;
    // }
    // if (system("diff -b test.out test.ans")) {
    //     puts("Wrong answer!");
    //     return 2;
    // }
    // puts("Accepted!");
    return 0;
}