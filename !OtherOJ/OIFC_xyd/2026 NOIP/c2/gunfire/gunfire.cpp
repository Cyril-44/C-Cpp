#include <bits/stdc++.h>
constexpr int N = 16;
constexpr double EPS = 1e-10;
double p[N];
int n, R;
std::valarray<double> f[2][1<<N];
int main() {
    freopen("gunfire.in", "r", stdin);
    freopen("gunfire.out", "w", stdout);
    scanf("%d%d", &n, &R);
    for (int i = 0; i < n; i++)
        scanf("%lf", &p[i]);
    auto fpre = f[0], fnow = f[1];
    fpre[0].resize(n);
    for (int s = 1; s < (1<<n); s++) {
        fpre[s].resize(n);
        const double avg = 1. / __builtin_popcount(s);
        for (int i = 0; i < n; i++)
            if (s >> i & 1) fpre[s][i] = avg;
    }
    for (int r = R; r >= 1; r--) {
        for (int i = n-1; i >= 0; i--) {
            for (int s = 0; s < (1<<n); s++) {
                fnow[s] = fpre[s];
                if (s>>i & 1) {
                    int cnt = 1;
                    double best = fnow[s][i];
                    for (int j = 0; j < n; j++) if (i != j && (s>>j & 1)) {
                        auto curr = p[i] * fpre[s ^ (1<<j)] + (1 - p[i]) * fpre[s];
                        if (std::abs(best - curr[i]) <= EPS) cnt++, fnow[s] += curr;
                        else if (curr[i] > best) best = curr[i], fnow[s] = curr, cnt = 1;
                    }
                    fnow[s] /= cnt;
                }
            }
            std::swap(fpre, fnow);
        }
    }
    printf("%.10lf\n", fpre[(1<<n)-1][0]);
}