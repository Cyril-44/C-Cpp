#include <bits/stdc++.h>
#include <cwchar>
constexpr int MOD = (int)1e9 + 7, B = 1024;
std::map<int,int> mp;
using Cube = std::array<int, 8>;
inline int gethash(const Cube &x) {
    int res = 0;
    for (int v : x) res = res * 8 + v;
    return res;
}
inline Cube transform(Cube x, auto func) {
    for (int &v : x) v = func(v);
    return x;
}
inline int tx(int s) { return (s >> 1 & 1) << 2 | !(s >> 2& 1) << 1 | (s & 1); }
inline int ty(int s) { return (s >> 2 & 1) << 2 | (s & 1) << 1 | !(s >> 1 & 1); }
inline int tz(int s) { return (s & 1) << 2 | (s >> 1 & 1) << 1 | !(s >> 2 & 1); }
std::decay_t<decltype(tx)> trans[3][3] = {
    {tx, [](int x) { return tx(tx(x)); }, [](int x) { return tx(tx(tx(x))); }}, 
    {ty, [](int x) { return ty(ty(x)); }, [](int x) { return ty(ty(ty(x))); }}, 
    {tz, [](int x) { return tz(tz(x)); }, [](int x) { return tz(tz(tz(x))); }}, 
};
struct Mat {
    static constexpr int N = 24;
    int a[N][N];
    friend Mat operator*(const Mat &x, const Mat &y) {
        __uint128_t res[N][N]{};
        for (int i = 0; i < N; i++)
            for (int j = 0; j < N; j++)
                for (int k = 0; k < N; k++)
                    res[i][k] += 1ull * x.a[i][j] * y.a[j][k];
        Mat ans;
        for (int i = 0; i < N; i++)
            for (int j = 0; j < N; j++)
                ans.a[i][j] = res[i][j] % MOD;
        return ans;
    }
    void one() { for (int i = 0; i < N; i++) a[i][i] = 1; }
} a[3][B];
struct Vec {
    constexpr static int N = Mat::N;
    int a[N];
    friend Vec operator*(const Mat &x, const Vec &y) {
        __uint128_t res[N]{};
        for (int i = 0; i < N; i++)
            for (int j = 0; j < N; j++)
                res[i] += 1ull * x.a[i][j] * y.a[j];
        Vec ans;
        for (int i = 0; i < N; i++) ans.a[i] = res[i] % MOD;
        return ans;
    }
};
int main() {
    freopen("stone.in", "r", stdin);
    freopen("stone.out", "w", stdout);
    std::vector<std::array<int, 8>> que;
    que.push_back({0,1,2,3,4,5,6,7});
    int top = 0;
    mp[gethash(que.front())] = top++;
    int front = 0;
    while (front != (int)que.size()) {
        auto cube = que[front++];
        for (const auto& t : {tx, ty, tz}) {
            auto cubet = transform(cube, t);
            if (!mp.count(gethash(cubet))) {
                mp[gethash(cubet)] = top++;
                que.push_back(cubet);
            }
        }
    }

    int m, q; scanf("%d%d", &m, &q);
    Mat base{};
    for (int i = 1; i <= m; i++) {
        char ch; int p;
        scanf(" %c%d", &ch, &p);
        for (const auto &cube : que) {
            auto cubet = transform(cube, trans[ch - 'A'][p-1]);
            ++base.a[mp[gethash(cubet)]][mp[gethash(cube)]];
        }
    }
    a[0][0].one();
    for (int i = 1; i < B; i++) a[0][i] = base * a[0][i-1];
    a[1][0].one(); Mat base1 = a[0][B-1] * base;
    for (int i = 1; i < B; i++) a[1][i] = base1 * a[1][i-1];
    a[2][0].one(); Mat base2 = a[1][B-1] * base1;
    for (int i = 1; i < B; i++) a[2][i] = base2 * a[2][i-1];
    while (q--) {
        int64_t n; Cube cube;
        scanf("%lld", &n);
        for (int &x : cube) scanf("%d", &x);
        n %= MOD - 1;
        Vec ans{}; ans.a[0] = 1;
        ans = a[2][n / B / B] * ans;
        ans = a[1][(n / B) % B] * ans;
        ans = a[0][n % B] * ans;
        printf("%d\n", ans.a[mp[gethash(cube)]]);
    }
    return 0;
}