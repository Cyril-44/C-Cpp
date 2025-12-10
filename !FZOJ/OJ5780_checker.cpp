#include <bits/stdc++.h>
using LL = long long;
using Point = std::pair<int,int>;
#define x first
#define y second
inline LL calc(Point a, Point b, Point c) {
    if (b < a) std::swap(a, b);
    if (c < a) std::swap(a, c);
    if (c < b) std::swap(b, c);
    return llabs(LL(c.x - a.x) * (b.y - c.y) + LL(c.y - a.y) * (c.x - b.x));
}
#undef x
#undef y
int main() {
    std::mt19937_64 rng(std::chrono::steady_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<int> rgN(1, 100), rgXY(-1000'000'000, 1000'000'000), rgT(500, 5000);
    for (int NOW = 1; NOW <= 100; ++NOW) {
        printf("Calculating..."); fflush(stdout);
        FILE *input = fopen("OJ5780.in", "w");
        FILE *answer = fopen("OJ5780.ans", "w");
        if (!input || !answer) return perror("fopen"), 1;
        int T = rgT(rng);
        fprintf(input, "%d\n", T);
        while (T--) {
            int as = rgN(rng), bs = rgN(rng), cs = rgN(rng);
            fprintf(input, "%d\n", as + bs + cs);
            std::vector<std::pair<int,int>> a(as), b(bs), c(cs);
    #define GEN(a, id)                          \
            for (auto &[x, y] : a) {            \
                x = rgXY(rng), y = rgXY(rng);   \
                fprintf(input, "%d %d %d\n", x, y, id); \
            }
            GEN(a, 0); GEN(b, 1); GEN(c, 2);
    #undef GEN
            LL ans = 0.;
            for (const auto &p1 : a)
                for (const auto &p2 : b)
                    for (const auto &p3 : c)
                        ans = std::max(ans, calc(p1, p2, p3));
            fprintf(answer, "%lld.%c\n", ans >> 1, ans & 1 ? '5' : '0');
        }
        fclose(input), fclose(answer);
        printf("Running..."); fflush(stdout);
        if (system("\\time -f %es ./OJ5780 <OJ5780.in >OJ5780.out")) return puts("\033[1;35mRuntime Error!\033[0m"), 2;
        if (system("diff -b OJ5780.out OJ5780.ans")) return puts("\033[1;31mWrong answer!\033[0m"), 3;
        printf("\033[32mAccepted! #%d\033[0m\n", NOW);
    }
    printf("\033[1;32mPretest passed!\033[0m\n");
    return 0;
}