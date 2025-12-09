#include <bits/stdc++.h>
using LL = long long;
using Point = std::pair<int,int>;
#define x first
#define y second
inline double len(const Point &a, const Point &b) {
    return sqrt(LL(a.x - b.x) * (a.x - b.x) + LL(a.y - b.y) * (a.y - b.y));
}
inline double calc(const Point &a, const Point &b, const Point &c) {
    double l1 = len(a, b), l2 = len(b, c), l3 = len(c, a);
    double avg = (l1 + l2 + l3) / 2.;
    return sqrt(avg * (avg - l1) * (avg - l2) * (avg - l3));
}
#undef x
#undef y
int main() {
    std::mt19937_64 rng(std::chrono::steady_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<int> rgN(1, 20), rgXY(-10, 10);
    for (int NOW = 1;; ++NOW) {
        FILE *input = fopen("OJ5780.in", "w");
        FILE *answer = fopen("OJ5780.ans", "w");
        if (!input || !answer) return perror("fopen"), 1;
        int T = 10;
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
            double ans = 0.;
            for (const auto &p1 : a)
                for (const auto &p2 : b)
                    for (const auto &p3 : c)
                        ans = std::max(ans, calc(p1, p2, p3));
            fprintf(answer, "%.1lf\n", ans);
        }
        fclose(input), fclose(answer);
        if (system("\\time -f %ems ./OJ5780 <OJ5780.in >OJ5780.out 2>OJ5780.txt")) return puts("\033[1;35mRuntime Error!\033[0m"), 2;
        if (system("diff -b OJ5780.out OJ5780.ans")) return puts("\033[1;31mWrong answer!\033[0m"), 3;
        printf("\033[32mAccepted! #%d\033[0m\n", NOW);
    }
    return 0;
}