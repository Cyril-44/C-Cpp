#include <stdio.h>
const int N = 100005;
struct Edge{
	int to, nxt, w;
} e[N<<1];
int head[N], f[N][3], mxl, mxrt;
inline void addedg(const int &fr, const int &to) {
	e[++head[0]] = Edge{to, head[fr], 1};
	head[fr] = head[0];
}
inline int max(const int &x, const int &y) {
	return x > y ? x : y;
}
void dfs(const int &u, const int &fa) {
	int mx = 0, mx0 = 0, id = 0, id0 = 0;
	for (int i = head[u], v; i; i = e[i].nxt)
	if ((v = e[i].to) ^ fa) {
		dfs(v, u);
		if (f[v][0] + e[i].w > mx) {
			mx0 = mx, id0 = id;
			mx = f[v][0] + e[i].w, id = i;
		}
		else if (f[v][0] + e[i].w > mx0) {
			mx0 = f[v][0] + e[i].w, id0 = i;
		}
	}
	f[u][0] = mx;
	f[u][1] = id;
	f[u][2] = id0;
	if (mx + mx0 > mxl) {
		mxl = mx + mx0;
		mxrt = u;
	}
}
int main() {
	int n, k, a, b, ans;
	scanf("%d%d", &n, &k);
	for (int i = 1; i < n; i++) {
		scanf("%d%d", &a, &b);
		addedg(a, b);
		addedg(b, a);
	}
	ans = (n - 1) << 1;
	while (k--) {
		mxl = 0;
		dfs(1, -1);
		ans -= mxl - 1;
		for (int i = f[mxrt][1]; i; i = f[e[i].to][1])
			e[i].w = e[(i&1) ? (i+1) : (i-1)].w = -1;
		for (int i = f[mxrt][2]; i; i = f[e[i].to][1])
			e[i].w = e[(i&1) ? (i+1) : (i-1)].w = -1;
	}
	printf("%d\n", ans);
	return 0;
}