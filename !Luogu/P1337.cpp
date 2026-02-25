#include <stdio.h>
#include <math.h>
#include <random>
constexpr int N = 1005;
std::mt19937_64 rng(std::random_device{}());
std::uniform_real_distribution<double> probGen(0, 1);
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
int main() {
    scanf("%d", &n);
    for (int i = 1; i <= n; i++)
        scanf("%d%d%d", &a[i].x, &a[i].y, &a[i].w);
    
    return 0;
}
