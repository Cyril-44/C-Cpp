#include <stdio.h>
#include <string.h>
#include <algorithm>
#define bcase break; case
using u64 = unsigned long long;
constexpr int N = 2005;
constexpr u64 Mask = (1ull << 60) - 1; 
u64 C[N][N];
auto _initC_sig = [] () -> int {
	for (int i = 0; i <= 1000; i++) {
		C[i][0] = C[i][i] = 1;
		for (int j = 1; j < i; j++)
			C[i][j] = C[i-1][j-1] + C[i-1][j];
	}
	return 0;
} ();
enum Operation { UB, LT, LE, GT, GE, EQ };
struct Rule {
	Operation op;
	int x, y;
	Rule () : op(UB), x(0), y(0) {}
	Rule (int lhs, char* mhs, int rhs) : x(lhs), y(rhs) {
		switch (mhs[0]) {
		 case '<': op = (mhs[1] == '=') ? LE : LT;
		bcase '>': op = (mhs[1] == '=') ? GE : GT;
		bcase '=': op = EQ;
		break; default: op = UB;
		}
	}
} r[N * 3];
bool check(Operation op, int x, int y) {
	switch (op) {
	case LT: return x < y;
	case LE: return x <= y;
	case GT: return x > y;
	case GE: return x >= y;
	case EQ: return x == y;
	default: return false;
	}
}
struct Rules {
	Rule *r;
	int m;
	Rules(Rule *lhs, int rhs) : r(lhs), m(rhs) {}
	bool operator() (int *h) const {
		for (int i = 1; i <= m; i++) {
			if (!check(r[i].op, h[r[i].x], h[r[i].y])) return false;
		}
		return true;
	}
};
namespace Dat1{
	int h[N], flg[N], n, m, ans;
	Rules* checker;
	void dfs(int x, int p) {
		for (int i = x; i < n; i++) {
			if (p < n*2) flg[i] = 0, h[p] = h[p+1] = i, dfs(i+1, p+2);
			flg[i] = 1, h[p] = i, dfs(i+1, p+1);
			flg[i] = 2;
		}
		h[p] = h[p+1] = n;
		for (x = n-1, p += 2; p <= n*2; ++p, --x) {
			while (x && !flg[x]) --x;
			h[p] = x;
			if (flg[x] == 2) h[++p] = x;
		}
		if ((*checker)(h)) ++ans;
	}
	int solve(const int n, const int m) {
		Dat1::n = n, Dat1::m = m;
		checker = new Rules(r, m);
		ans = 0;
		dfs(1, 1);
		return ans;
	}
}
namespace Dat3 {
	u64 solve(const int n) {
		u64 ans = 0;
		for (int i = 2; i <= n*2; i++)
			for (int j = 0; j < n && j*2 < i-1; ++j)
//				fprintf(stderr, "i=%d, j=%d: +C(%d,%d)*C(%d,%d) = +%llu*%llu\n", i, j, n-1, j, n-1-j, i-2-j*2, C[n-1][j], C[n-1-j][i-2-j*2]),
				ans += C[n-1][j] * C[n-1-j][i-2-j*2];
		return ans;
	}
}
u64 qpow(u64 b, unsigned n) {
	u64 res = 1;
	while (n) {
		if (n & 1) res *= b;
		b *= b;
		n >>= 1;
	}
	return res;
}
namespace Dat {
	int sum[N][N];
	bool lt[N][N];
	u64 f[N][N];
	u64 solve(const int n, const int m) {
		memset(lt, 0, sizeof lt);
		memset(sum, 0, sizeof sum);
		memset(f, 0, sizeof f);
		for (int i = 1; i <= m; i++) {
			switch (r[i].op) {
			 case LT: lt[r[i].x][r[i].y] = true;
			 case LE: sum[r[i].y][r[i].x] = 1;
			bcase GT: lt[r[i].y][r[i].x] = true;
			 case GE: sum[r[i].x][r[i].y] = 1;
			bcase EQ: sum[r[i].x][r[i].y] = sum[r[i].y][r[i].x] = 1;
			}
		}
		for (int i = 1; i <= n*2; i++)
			for (int j = 1; j <= n*2; j++)
				sum[i][j] += sum[i][j-1];
		for (int i = 1; i < n*2; i++)
			f[i][i+1] = !lt[i][i+1] && !lt[i+1][i];
		for (int i = 2; i <= n; i++) {
			for (int l = 1, r = i << 1; r <= n*2; ++l, ++r) {
				if (!lt[l][l+1] && !lt[l+1][l] && !(sum[l][r] - sum[l][l+1]) && !(sum[l+1][r] - sum[l+1][l+1]))
					f[l][r] += f[l+2][r]; // jj_
				if (!lt[r-1][r] && !lt[r][r-1] && !(sum[r][r-2] - sum[r][l-1]) && !(sum[r-1][r-2] - sum[r-1][l-1]))
					f[l][r] += f[l][r-2]; // _jj
				if (!lt[l][r] && !lt[r][l] && !(sum[l][r-1] - sum[l][l]) && !(sum[r][r-1] - sum[r][l]))
					f[l][r] += f[l+1][r-1]; // j_j
			}
		}
		return f[1][n*2];
	}
}
int main() {
	int n, m;
	char ops[4];
	while (~scanf("%d%d", &n, &m)) {
		for (int x, y, i=1; i <= m; i++) {
			scanf("%d %s %d", &x, ops, &y);
			r[i] = std::move(Rule(x, ops, y));
		}
		u64 ans;
		if (m == 0) ans = Dat3::solve(n);
		else if (n <= 5) ans = Dat1::solve(n, m);
		else ans = Dat::solve(n, m);
		printf("%llu\n", ans & Mask);
	}
	/* FILE* wrong = fopen("wrong.txt", "wt");
	u64 ___a, ___b;
	for (int i = 1; i <= 1000; i++) {
		if ((___a = Dat3::solve(i)) != (___b = qpow(3, i-1))) {
			fprintf(wrong, "Wrong! %d: [0]<%llu %llu>[1]\n", i, ___a, ___b);
		}
	}
	fclose(wrong); */
	return 0;
}