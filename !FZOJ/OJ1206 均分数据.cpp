#include <stdio.h>
#include <math.h>
#include <random>
#include <algorithm>
std::mt19937_64 rng(std::random_device{}());
std::uniform_real_distribution<double> degGen(0, 2 * M_PI), probGen(0, 1);
constexpr double EPS = 1e-6, R = 0.999;
constexpr int N = 25, M = 50;
int a[N], bl[M][N];
double e[M];
int n, k;
inline double sigma(int *bl2) {
	int h[10]{};
	for (int i = 1; i <= n; i++)
		h[bl2[i]] += a[i];
	int sum = 0, sum2 = 0;
	for (int i = 1; i <= k; i++)
		sum += h[i], sum2 += h[i] * h[i];
	return sqrt((double)sum2 / k - sum * sum / (double)(k * k));
}
int main() {
	scanf("%d%d", &n, &k);
	for (int i = 1; i <= n; i++) scanf("%d", &a[i]);
	std::uniform_int_distribution<int> groupGen(1, k), posGen(1, n);
	for (int j = 0; j < M; j++) {
		for (int i = 1; i <= n; i++)
			bl[j][i] = groupGen(rng);
		e[j] = sigma(bl[j]);
	}
	for (double T = 1000; T >= EPS; T *= R) {
		for (int j = 0; j < M; j++) {
			int pos1 = posGen(rng), pos2 = posGen(rng);
			int bak1 = bl[j][pos1], bak2 = bl[j][pos2];
			bl[j][pos1] = groupGen(rng), bl[j][pos2] = groupGen(rng);
			double ee = sigma(bl[j]), dE = e[j] - ee;
			if (dE >= 0 || exp(dE / T) >= probGen(rng)) e[j] = ee;
			else bl[j][pos1] = bak1, bl[j][pos2] = bak2;
		}
	}
	printf("%.2lf", *std::min_element(e, e+M));
	return 0;
}