#include <stdio.h>
#include <math.h>
#include <algorithm>
#include <random>
std::mt19937_64 rng(std::random_device{}());
std::uniform_real_distribution<double> degGen(0, 2 * M_PI), probGen(0, 1);

constexpr double EPS = 1e-2, R = 0.993;
constexpr int K = 1005, M = 50, L = 2;
struct Point { int x, y; } a[K];
struct PointEx { double x, y, e; };
inline bool operator< (const PointEx &p, const PointEx &q) { return p.e < q.e; }
int c;
inline double dist(Point p, double x, double y) {
	x -= p.x, y -= p.y;
	return sqrt(x * x + y * y);
}
inline double E(double x, double y) {
	double mn = INFINITY;
	for (int i = 1; i <= c; i++)
		mn = std::min(mn, dist(a[i], x, y));
	return mn;
}
int main() {
	int n, m;
	scanf("%d%d%d", &n, &m, &c);
	for (int i = 1; i <= c; i++)
		scanf("%d%d", &a[i].x, &a[i].y);
	static PointEx points[M];
	std::uniform_real_distribution<double> xGen(0, n), yGen(0, m);
	PointEx ans{0, 0, E(0, 0)};
	for (int i = 0; i < M; i++) {
		double x = xGen(rng), y = yGen(rng);
		points[i] = {x, y, E(x, y)};
		ans = std::max(ans, points[i]);
	}
	for (double T = std::sqrt(n * n + m * m) + EPS; T >= EPS; T *= R) {
		for (int i = 0; i < M; i++) {
			for (int l = L; l--; ) {
				double theta = degGen(rng);
				double x = points[i].x + cos(theta) * T * probGen(rng), y = points[i].y + cos(theta) * T * probGen(rng);
				if (x < 0 || x > n || y < 0 || y > m) continue;
				double e = E(x, y), deltaE = e - points[i].e;
				if (deltaE >= 0 || exp(deltaE / T) >= probGen(rng))
					points[i] = {x, y, e};
				ans = std::max(ans, points[i]);
			}
		}
	}
	printf("The safest point is (%.1lf, %.1lf).", ans.x, ans.y);
	return 0;
}