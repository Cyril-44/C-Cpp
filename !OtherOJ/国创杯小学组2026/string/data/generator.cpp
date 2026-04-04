#include "testlib.h"
#include <bits/stdc++.h>
inline std::string genPal(int len, const int incProb, const int contProb, const char startChar) {
    std::string res; res.resize(len);
    char ch = rnd.wnext(startChar, 'z', -10);
    for (int i = 0, cur; i < (len+1) / 2; i++) {
        if (ch < 'z' && (cur = rnd.next(0, 100)) <= incProb) ++ch;
        if (ch < 'z' && cur > contProb) {i--; continue;}
        res[i] = ch;
    }
    std::reverse_copy(res.begin(), res.begin() + len / 2, res.begin() + (len + 1) / 2);
    return res;
}
inline std::string genHack(int len, const int incProb, const int contProb) {
    assert(len >= 6); // aababa
    char ch = rnd.wnext('a', 'y', -26);
    int l1 = rnd.wnext(2, len - 4, -26);
    int l2 = rnd.wnext(1, (len - l1) / 2 - 1, -26);
    int l3 = rnd.wnext(1, len - l1 - 2 * l2 - 1, -10);
    int l4 = rnd.wnext(1, len - l1 - 2 * l2 - 1, -10);
    std::string base = // [l1](l3)[l2](l4)[l2]
        std::string(l1, ch) + 
        genPal(l3, incProb, contProb, ch + 1) + 
        std::string(l2, ch) + 
        genPal(l4, incProb, contProb, ch + 1) + 
        std::string(l2, ch);
    len -= l1 + l3 + l2 + l4 + l2;
    for (bool state = rnd.next(0, 1); len > 0; state ^= 1) {
        int curlen = rnd.wnext(1, len, -5);
        if (state) base += genPal(curlen, incProb, contProb, ch + 1);
        else       base += std::string(curlen, ch);
        len -= curlen;
    }
    return base;
}
inline std::string genRand(int len) {
    std::string res; res.resize(len);
    for (int i = 0; i < len; i++) res[i] = rnd.next('a', 'z');
    return res;
}
inline std::string gen(int len, const int w, const int incProb, const int contProb, const int startState = -1) {
    std::string res; res.reserve(len);
    for (bool state = ~startState ? startState : rnd.next(0, 1); len > 0; state ^= 1) {
        if (len < 6) state = 0;
        int curlen = rnd.wnext(state ? 6 : 1, len, w);
        if (state) {
            std::string now = genHack(curlen, incProb, contProb);
            if (rnd.next(0, 1)) std::reverse(now.begin(), now.end());
            res += now;
        }
        else res += genRand(curlen);
        len -= curlen;
    }
    return res;
}
inline std::string genB(int len, const int w, const int contProb) {
    std::string res; res.reserve(len);
    for (bool state = rnd.next(0, 1); len > 0; state ^= 1) {
        int curlen = rnd.wnext(1, len, w);
        if (state) res += genPal(curlen, 100, contProb, res.empty() ? 'a' : res.back() + 1);
        else {
            for (int i = 1; i <= curlen; i++) {
                char ch = rnd.next('a', 'x');
                ch += ch >= res.back();
                res += ch;
            }
        }
        len -= curlen;
    }
    return res;
}

#define Gen (const std::string &path, const int taskId)
#define Generator(T, SN, N, w, str) [] Gen {                                                            \
    std::ofstream file(path);                                                                           \
    int t = T, sn = SN;                                                                                 \
    file << taskId << ' ' << t << '\n';                                                                 \
    while (--t > 0) {                                                                                   \
        int n = rnd.wnext((int)std::max(1ll, sn - (long long)(N) * t), std::min(sn - t, int(N)), w);    \
        sn -= n; file << str << '\n';                                                                   \
    }                                                                                                   \
    int n = sn; file << str << '\n';                                                                    \
}
void (*testGen[]) Gen {
    Generator(1e3, 4e7, 5e6, -100, std::string(n, 'a')),
    Generator(2,   20,  10,  0, gen(n, 26, 70, 40, 1)),
    Generator(5,   500, 200, 0, gen(n, 2,  20, 60)),
    Generator(15,  5e4, 5e3, 5, gen(n, 2,  15, 65)),
    Generator(1e2, 2e6, 1e5, -2, genB(n, 2, 85)),
    Generator(1e2, 4e7, 5e6, -5, genB(n, 5, 95)),
    Generator(1e2, 4e7, 5e6, -5, genRand(n)),
    Generator(1e2, 2e6, 1e5, -2, gen(n, 5, 10, 75)),
    Generator(1e2, 4e7, 5e6, -5, gen(n, 5, 5,  85))
};
void (*sampleGen[]) Gen {
    testGen[1],
    testGen[2],
    testGen[3],
    testGen[4],
    Generator(1e2, 2e6, 1e5, -2, genRand(n)),
    testGen[7]
};
constexpr int SubtaskConfig[] {1, 2, 4, 7, 9, 12, 15, 19, 25};
constexpr int SampleConfig[] {2, 3, 5, 8, 13, 16};
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
                testGen[subtaskId]("tests/dat" + std::to_string(testId) + ".in", testId);
            }
        ++subtaskId;
    }
    for (const auto &sgen : sampleGen)
        if (generateOneTest == 0 || -generateOneTest == sampleId) {
            std::cout << "Generating Sample " << sampleId << "..." << std::endl;
            sgen("down/string" + std::to_string(sampleId++) + ".in", SampleConfig[sampleCount++]);
        }
}