#include <stdio.h>
#include <string.h>
#include <algorithm>
constexpr int MOD = 998244353, N = 100005;
struct FastIS {
	char buf[1 << 20], *p1, *p2;
	FastIS() : p1(nullptr), p2(nullptr) {}
	__attribute__((always_inline)) inline int get() {
		return (p1 == p2) && (p2 = (p1=buf) + fread(buf, 1, sizeof buf, stdin), p1 == p2) ? EOF : *p1++;
	}
} is;
template<typename T, typename = typename std::enable_if<std::is_integral<T>::value, void>::type>
inline void in(T& x) {
	int ch = is.get();
	while (~ch && (ch < '0' || ch > '9')) ch = is.get();
	for (x = 0; ch >= '0' && ch <= '9'; ch = is.get())
		x = (x << 3) + (x << 1) + (ch ^ '0');
}
int n, m, pr, k;
int a[N], prn[N], prin[N];
int rt;
struct RangeSegTree {
	constexpr static int Range = 1e9;
	int tr[N * 20]{}, s[N * 20][2]{}, top;
	RangeSegTree() : top(0) {}
	void upd(const int p, const int x, int& u=rt, int l=0, int r=Range) {
		if (!u) u = ++top;
		if (l == r) {
			tr[u] += x;
			return;
		}
		int mid = l + (r - l >> 1);
		if (p <= mid) upd(p, x, s[u][0], l, mid);
		else upd(p, x, s[u][1], mid+1, r);
		tr[u] = tr[s[u][0]] + tr[s[u][1]];
	}
	int query(const int L, const int R, int u=rt, int l=0, int r=Range) const {
		if (!u) return 0;
		if (L <= l && r <= R) return tr[u];
		int mid = l + (r - l >> 1), res = 0;
		if (L <= mid) res += query(L, R, s[u][0], l, mid);
		if (mid < R) res += query(L, R, s[u][1], mid+1, r);
		return res;
	}
} f;
int main() {
#ifdef HS_LOCALDBG
	freopen("D.in", "r", stdin);
#endif
	in(n), in(m), in(pr), in(k);
	for (int i = 1; i <= n; i++) {
		in(a[i]);
		f.upd(a[i], 1);
	}
	prn[0] = prin[0] = 1;
	for (int i = 1; i <= n; i++)
		prn[i] = 1ll * prn[i-1] * pr % MOD;
	pr = (MOD + 1 - pr) % MOD;
	for (int i = 1; i <= n; i++)
		prin[i] = 1ll * prin[i-1] * pr % MOD;
	for (int op, x, y; m--; ) {
		in(op), in(x);
		if (op == 1) {
			in(y);
			f.upd(a[x], -1);
			a[x] = y;
			f.upd(y, 1);
		}
		else if (k == 1) puts("1");
		else {
			printf("%d\n", (prin[f.query((a[x]-1) / k + 1, a[x] - 1) + 1] + prn[f.query(a[x], a[x] * k - 1)]) % MOD);
		}
	}
	return 0;
}
/*
#include <bits/stdc++.h>
constexpr int N = 200005;
struct Shop {
	int a, b;
	inline bool operator< (const Shop &rhs) const {
		return a < rhs.a || a == rhs.a && b < rhs.b;
	}
} a[N], aans[N];
int main() {
	std::mt19937 rng(time(nullptr));
	FILE *f;
	int n, T;
start:
	n = rng() % 10 + 1, T = rng() % 1000000000 + 1;
	for (int i = 1; i <= n; i++) {
		a[i].a = rng() % 100, a[i].b = rng() % 1000;
	}
	
	while (!(f = fopen("C.in", "wt")));
	fprintf(f, "%d %d\n", n, T);
	for (int i = 1; i <= n; i++)
		fprintf(f, "%d %d\n", a[i].a, a[i].b);
	fclose(f);
	
	std::sort(a+1, a+1 + n);
	int ans = 0;
	do {
		int cnt = 0;
		long long t = 0;
		for (int i = 1; i <= n; i++) {
			++t;
			t += a[i].a * t + a[i].b;
			if (t > T) break;
			++cnt;
		}
		if (cnt > ans) {
			memcpy(aans, a, sizeof(Shop) * (n+1));
			ans = cnt;
		}
	} while (std::next_permutation(a+1, a+1 + n));
	
	unsigned st = clock();
	system("C.exe > C.out");
	unsigned ed = clock();
	printf("n=%d ", n);
	printf("Timer: %ums. ", ed - st);
	
	while (!(f = fopen("C.out", "rt")));
	int scan;
	fscanf(f, "%d", &scan);
	fclose(f);
	printf("Expected %d, found %d. ", ans, scan);
	
	if (ans != scan) {
		puts("Wrong Answer!");
		for (int i = 1; i <= n; i++)
			printf("%d %d\n", aans[i].a, aans[i].b);
		throw ;
	}
	puts("Accepted.");
	goto start;
	return 0;
}
#include <bits/stdc++.h>
constexpr int N = 300005;
int a[N], n;
bool f[N], fans[N];
long long ans;
long long calc() {
	int cnt, i;
	long long front = 0, back = 0;
	for (cnt = 0, i = 1; cnt < n; i++)
		if (!f[i]) ++cnt, front += a[i];
	for (cnt = 0; cnt < n; i++)
		if (!f[i]) ++cnt, back += a[i];
	return front - back;
}
void dfs(int x, int p) {
	if (x > n) {
		long long tmp = calc();
		if (tmp > ans) {
			ans = tmp;
			memcpy(fans, f, sizeof(int) * (n*3 + 1));
		}
		return;
	}
	int range = (n<<1) + x;
	for (int i = p; i <= range; i++) {
		f[i] = true;
		dfs(x + 1, i + 1);
		f[i] = false;
	}
}
int main() {
	std::mt19937 rng (time(nullptr));
	FILE* file;
	while (true) {
		n = rng() % 5 + 1;
		for (int i = 1; i <= n*3; i++)
			a[i] = rng() % 10 + 1;
			
		while (!(file = fopen("B.in", "wt"))) putchar('!');
		fprintf(file, "%d\n", n);
		for (int i = 1; i <= n*3; i++)
			fprintf(file, "%d ", a[i]);
		fprintf(file, "\n");
		fclose(file);
		
		unsigned st = clock();
		system("B.exe > B.out");
		unsigned ed = clock();
		printf("Timer: %3ums. ", ed - st);
		
		ans = 1ll << 63;
		dfs(1, 1);
		
		while (!(file = fopen("B.out", "rt"))) putchar('?');
		long long scan;
		fscanf(file, "%lld", &scan);
		printf("Expected %lld, found %lld. ", ans, scan);
		
		if (ans != scan) {
			puts("Wrong Answer!");
			for (int i = 1; i <= 3*n; i++)
				printf("%3d", a[i]);
			putchar('\n');
			for (int i = 1; i <= 3*n; i++)
				printf("%s", fans[i] ? "xxx" : "___");
			putchar('\n');
			throw "Wrong Answer!";
		}
		else puts("Accepted.");
	}
	return 0;
}
#include <bits/stdc++.h>
constexpr int N = 2005;
long long a[N][N];
int main() {
	system("mode con cols=100");
	std::mt19937 rng (time(nullptr));
	while (true) {
		memset(a, 0, sizeof a);
		FILE* input = fopen("A.in", "wt");
		while (!input) input = fopen("A.in", "wt");
		int n = rng() % 1000 + 1, q = n <= 50 ? rng() % 250000 + 50001 : rng() % 5000 + 5001;
		fprintf(input, "%d %d\n", n, q);
		for (int _ = 0; _ < q; ++_) {
			int r = rng() % n + 1, c = rng() % n + 1, l = rng() % n + 1, s = rng() % 1000000000 + 1;
			fprintf(input, "%d %d %d %d\n", r, c, l, s);
			for (int i = 0; i < l; i++)
				a[r+i][c] += s, a[r+i][c+i+1] -= s;
		}
		fclose(input);
		long long xorsum = 0;
		for (int i = 1; i <= n; i++)
			for (int j = 1; j <= n; j++)
				xorsum ^= a[i][j] += a[i][j-1];
		unsigned st = clock();
		system("A.exe > A.out");
		unsigned ed = clock();
		FILE* output = fopen("A.out", "r");
		while (!output) output = fopen("A.in", "wt");
		long long scanAns;
		fscanf(output, "%lld", &scanAns);
		printf("n=%4d, q=%6d. Timer: %3ums. Expected: %lld, found: %lld ", n, q, ed - st, xorsum, scanAns);
		if (scanAns != xorsum) {
			puts("Wrong Answer!");
			throw;
		}
		else puts("Accepted!");
	}
	return 0;
} */