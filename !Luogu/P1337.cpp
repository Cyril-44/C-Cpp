#include <cmath>
#include <cstdio>
#include <random>
constexpr int N = 1005;
std::mt19937_64 rng(std::random_device{}());
std::uniform_real_distribution<double> probGen(0, 1), mulGen(-1, 1);
struct Stuff { int x, y, w; } a[N];
int n;
inline double E(double x, double y) {
    double e = 0;
    for (int i = 1; i <= n; i++) { // 最稳定的时候重力势能最小
        double dx = x - a[i].x, dy = y - a[i].y;
        e += a[i].w * sqrt(dx*dx + dy*dy);
    }
    return e;
}
inline void simuAnn(double nowx, double nowy) {
    double ansx=nowx, ansy=nowy, anse=E(nowx,nowy), nowe=anse;
    for (double T = 1e15; T > 0.001; T *= 0.97) {
        double nxtx = nowx + std::min(T, 10000.) * mulGen(rng), nxty = nowy + std::min(T, 10000.) * mulGen(rng);
        double nxte = E(nxtx, nxty);
        double delta = nxte - nowe;
        if (nxte < anse) anse = nxte, ansx = nxtx, ansy = nxty;
        nowe = nxte;
        if (std::exp(-delta / T) > probGen(rng)) nowx = nxtx, nowy = nxty;
    }
    for (int fix = 1; fix <= 100000; fix++) { // 注意这里必须拿 ansx ansy 去迭代
        double nxtx = ansx + 0.001 * mulGen(rng), nxty = ansy + 0.001 * mulGen(rng);
        double nxte = E(nxtx, nxty);
        if (nxte < anse) anse = nxte, ansx = nxtx, ansy = nxty;
    }
    printf("%.3lf %.3lf\n", ansx, ansy);
}
int main() {
    scanf("%d", &n);
    for (int i = 1; i <= n; i++)
        scanf("%d%d%d", &a[i].x, &a[i].y, &a[i].w);
    simuAnn(0, 0);
    return 0;
}
