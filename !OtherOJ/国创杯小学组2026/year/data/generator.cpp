#include <bits/stdc++.h>
#include "testlib.h"
using namespace std;
constexpr int SubtaskConfig[] = {2,4,6,10,14,18,25};
constexpr int SampleConfig[] =  {1,1,1,1 ,1 ,1 ,0 };

inline int64_t calc(int64_t m, int a, int b, int c) {
    return m / a - m / b + m / c;
}
inline int64_t solve(int64_t m, int a, int b, int c, int64_t n) {
    --m; n += m / a - m / b + m / c;
    c /= b, b /= a;
    int64_t ans = a * (n / (b * c - c + 1)) * (b * c);
    n %= b * c - c + 1;
    if (n) {
        ans += a * ((n - 1) / (b - 1)) * b;
        n = (n-1) % (b-1) + 1;
        ans += a * n;
    }
    return ans;
}
inline pair<int64_t, int64_t> mngen(const pair<int64_t, int64_t> &mLmt, const pair<int64_t,int64_t> &nLmt, int a, int b, int c) {
    int64_t n = rnd.wnext(nLmt.first, nLmt.second, 2);
    switch (rnd.next(10)) {
    case 1: case 4:
        return {rnd.next(mLmt.first, mLmt.second), n};
    case 0: case 5: case 6: {
        int64_t m;
        switch (rnd.next(4)) {
            case 1: m = rnd.next((mLmt.first + a - 1) / a, mLmt.second / a) * a; break;
            case 2: m = rnd.next((mLmt.first + b - 1) / b, mLmt.second / b) * b; break;
            case 3: m = rnd.next((mLmt.first + c - 1) / c, mLmt.second / c) * c; break;
            case 0: m = rnd.next(mLmt.first, mLmt.second);
        }
        return {m, n};
    }
    default: {
        int64_t ans, m=-1;
        for (unsigned char MaxTry = 4; MaxTry && !(mLmt.first <= m && m <= mLmt.second); --MaxTry) {
            ans = b * (rnd.next((mLmt.first + b - 1) / b + n / (b / a), mLmt.second / b + n / (b / a)));
            switch (rnd.next(3)) { case 1: ans += a; break; case 2: ans -= a; break; }
            m = solve(calc(ans, a, b, c) - n, a, b, c, n);
        }
        if (!(mLmt.first <= m && m <= mLmt.second)) m = rnd.next(mLmt.first, mLmt.second);
        return {m, n};
    }
    }
}
inline pair<int64_t, int64_t> mngenEasy(pair<int64_t,int64_t>, pair<int64_t,int64_t> nLmt, int a, int b, int c) {
    return {c * rnd.next((int64_t)1, (int64_t)1e9), rnd.next(nLmt.first, nLmt.second)};
}

inline tuple<int,int,int> abcgen(const int limit) {
    int a = rnd.next(1, limit / 4);
    int b = a * rnd.next(2, limit / a / 2);
    int c = b * rnd.next(2, limit / b);
    return {a, b, c};
}

void (*testGen[])(const string&, int) = {
    // m 1000, n 1, abc 100
    [](const string &path, int Tid) { ofstream file(path); file << Tid << ' ';
        const int T = path.find("down") == string::npos ? int(2e6) : 100;
        file << T << '\n';
        for (int t = 1; t <= T; t++) {
            auto [a, b, c] = abcgen(100);
            auto [m, n] = mngen({1, 1000}, {1, 1}, a, b, c);
            file << m << ' ' << a << ' ' << b << ' ' << c << ' ' << n << '\n';
        }
    },
#define Header [](const string &path, int Tid) { \
    ofstream file(path); file << Tid << ' '; \
    vector<tuple<int64_t,int,int,int,int>> out;
#define Footer \
        out.emplace_back(m, a, b, c, n); \
        leftn -= n; \
    } file << out.size() << '\n'; \
    for (const auto &[m, a, b, c, n] : out) \
        file << m << ' ' << a << ' ' << b << ' ' << c << ' ' << n << '\n'; \
}
#define Generator1(M, N, SN, ABC) Header \
    int leftn = SN; std::vector<int> alln; \
    for (int s = 10; s <= N; s *= 10) alln.push_back(s); \
    while (leftn > 10) { \
        auto [a, b, c] = abcgen(ABC); \
        auto [m, n] = mngen({1, M}, {1, rnd.any(alln)}, a, b, c); \
        while (!alln.empty() && alln.back() > leftn - n) alln.pop_back(); \
    Footer
#define Generator2(SN, SSN, GFunc) Header \
    out.reserve(5e6); \
    int64_t leftn = path.find("down") == string::npos ? SN : SSN; \
    while (leftn > (int)5e8) { \
        auto [a, b, c] = abcgen(1e6); \
        auto [m, n] = GFunc({1, 1e15}, {5e8, 1e9}, a, b, c); \
    Footer

    Generator1(1e3,  1e2, 1e3, 1e3),
    Generator1(1e6,  1e3, 1e4, 1e6),
    Generator1(1e15, 1e5, 1e6, 1e6),
    Generator2(1e15, 1e12, mngenEasy),
    Generator2(1e14, 1e12, mngen),
    Generator2(1e15, 1e12, mngen)
};
int main(int argc, char** argv) {
    registerGen(argc, argv, 1);
    int testId = 1, subtaskId = 0, sampleId = 2;
    for (int endId : SubtaskConfig) {
        if (SampleConfig[subtaskId]) testGen[subtaskId]("down/year" + to_string(sampleId++) + ".in", testId);
        for (; testId <= endId; testId++)
            testGen[subtaskId]("tests/dat" + to_string(testId) + ".in", testId);
        ++subtaskId;
    }
}