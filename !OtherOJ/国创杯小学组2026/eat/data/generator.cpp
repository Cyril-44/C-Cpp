#include <bits/stdc++.h>
#include "testlib.h"
#define Generator(N, Qr1, Qr2, prob, P, Agent)                                                                         \
    [](const std::string &path, const int Tid) {                                                                       \
        std::ofstream fin(path + ".in"), fans(path + ".ans");                                                          \
        fin << Tid << '\n';                                                                                            \
        const int n = N;                                                                                               \
        std::vector<int> a(n + 1);                                                                                     \
        for (int i = 1; i <= n; i++) a[i] = rnd.next(1, (int)1e9);                                                     \
        Agent;                                                                                                         \
        int l = 1, r = n;                                                                                              \
        std::vector<std::pair<int, int64_t>> ques;                                                                     \
        ques.reserve(n);                                                                                               \
        std::vector<std::pair<char, int>> anss;                                                                        \
        anss.reserve(n);                                                                                               \
        while (l <= r) {                                                                                               \
            int p = rnd.next(0, int(P)),                                                                               \
                qr = std::min(rnd.next() <= (prob) ? (int)Qr1 : (int)Qr2, (r - l) / 2 + 1),                            \
                ans = rnd.wnext(std::max(1, qr / 10), qr, -2), k;                                                      \
            std::set<int64_t> st;                                                                                      \
            bool lst = true, rst = true, lst0 = true, rst0 = true, same = false;                                       \
            int64_t lval, rval;                                                                                        \
            auto calc = [&](int pos) { return 1ll * a[pos] * a[l + (pos - l + p) % (r - l + 1)]; };                    \
            for (k = 1; k <= ans; k++) {                                                                               \
                lval = calc(l + k - 1);                                                                                \
                rval = calc(r - k + 1);                                                                                \
                lst0 = lst, rst0 = rst;                                                                                \
                lst &= st.insert(lval).second;                                                                         \
                rst &= st.insert(rval).second;                                                                         \
                if (lval == rval) { same = true; break; }                                                              \
                if (!lst && !rst) break;                                                                               \
            }                                                                                                          \
            bool type = rnd.next(2);                                                                                   \
            if (same) {                                                                                                \
                if (!lst && !rst) --k; else type = 1;                                                                  \
                ques.emplace_back(p, type ? calc(l + k - 1) : calc(r - (k - 1)));                                      \
                anss.emplace_back(type ? 'L' : 'R', k);                                                                \
                type ? (l += k) : (r -= k);                                                                            \
            } else if (lst && rst) {                                                                                   \
                ques.emplace_back(p, type ? lval : rval);                                                              \
                anss.emplace_back(type ? 'L' : 'R', ans);                                                              \
                type ? (l += ans) : (r -= ans);                                                                        \
            } else if (lst) {                                                                                          \
                ques.emplace_back(p, lval);                                                                            \
                anss.emplace_back('L', ans);                                                                           \
                l += ans;                                                                                              \
            } else if (rst) {                                                                                          \
                ques.emplace_back(p, rval);                                                                            \
                anss.emplace_back('R', ans);                                                                           \
                r -= ans;                                                                                              \
            } else if (lst0 && rst0) {                                                                                 \
                ques.emplace_back(p, type ? calc(l + (k - 1) - 1) : calc(r - (k - 1) + 1));                            \
                anss.emplace_back(type ? 'L' : 'R', k - 1);                                                            \
                type ? (l += (k - 1)) : (r -= (k - 1));                                                                \
            } else if (lst0) {                                                                                         \
                ques.emplace_back(p, calc(l + (k - 1) - 1));                                                           \
                anss.emplace_back('L', k - 1);                                                                         \
                l += (k - 1);                                                                                          \
            } else if (rst0) {                                                                                         \
                ques.emplace_back(p, calc(r - (k - 1) + 1));                                                           \
                anss.emplace_back('R', k - 1);                                                                         \
                r -= (k - 1);                                                                                          \
            }                                                                                                          \
        }                                                                                                              \
        int omitCnt = rnd.next(0, std::min(2, (int)ques.size()));                                                      \
        for (int i = omitCnt; i--;) ques.pop_back(), anss.pop_back();                                                  \
        fin << n << ' ' << ques.size() << '\n';                                                                        \
        for (int i = 1; i <= n; i++) fin << a[i] << (i == n ? '\n' : ' ');                                             \
        for (const auto &[x, y] : ques) fin << x << ' ' << y << '\n';                                                  \
        for (const auto &[c, x] : anss) fans << c << ' ' << x << '\n';                                                 \
    }
void (*testGen[])(const std::string&, const int) {
    //        n    Qr1  Qr2  prob P
    Generator(10,  3,   6,   0.5,    1e6, ),
    Generator(1e3, 10,  3e2, 0.999,  1e6, ),
    Generator(1e5, 300, 1e4, 0.7,    1e6, ),
    Generator(1e6, 1,   0,   1,      1e6, for (int i = 2; i <= n; i++) a[i] = a[1]),
    Generator(1e6, 10,  0,   1,      1e6, ),
    Generator(1e6, 20,  3e5, 0.9999, 0,   ),
    Generator(1e6, 20,  3e5, 0.9999, 1e6, )
};
void (*sampleGen[])(const std::string&, const int) {
    testGen[1],
    Generator(5e4, 300, 1e3, 0.999,  1e6, ),
    Generator(5e4, 1,   0,   1,      1e6, for (int i = 2; i <= n; i++) a[i] = a[1]),
    Generator(5e4, 10,  0,   1,      1e6, ),
    Generator(5e4, 40,  5e3, 0.999,  0,   ),
    Generator(5e4, 40,  5e3, 0.999,  1e6, )
};
constexpr int SubtaskConfig[] {1,2,4,5,8,12,20};
constexpr int SampleConfig[] {2,3,5,6,9,13};
int main(int argc, char **argv) {
    registerGen(argc, argv, 1);
    if (has_opt("help")) return suppressEnsureNoUnusedOpts(), puts(
        "Generate samples to down/, data to tests/.\n"
        "Options:\n"
        "    --id=<x>    ----    If given, the generator will only generates the specific test case <x>.\n"
        "                        If <x> is negative, it's absolute value refers to a sample ID.\n"
        "    --help      ----    Print this help.\n"
        "\e[31mCaution\e[0m: The generator won't automatically clear or create the down/ and tests/ folder."), 0;
    int generateOneTest = opt<int>("id", 0);
    int testId = 1, subtaskId = 0, sampleId = 3, sampleCount = 0;
    for (int endId : SubtaskConfig) {
        for (; testId <= endId; testId++)
            if (generateOneTest == 0 || generateOneTest == testId) {
                std::cout << "Generating Test Case " << testId << "..." << std::endl;
                testGen[subtaskId]("tests/dat" + std::to_string(testId), testId);
            }
        ++subtaskId;
    }
    for (const auto &sgen : sampleGen)
        if (generateOneTest == 0 || -generateOneTest == sampleId) {
            std::cout << "Generating Sample " << sampleId << "..." << std::endl;
            sgen("down/eat" + std::to_string(sampleId++), SampleConfig[sampleCount++]);
        }
}