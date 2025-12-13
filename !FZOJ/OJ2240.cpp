#include <stdio.h>
#include <string.h>
#include <queue>
const int N = 25;
struct Edge{
	int to, nxt, w;
} e[N * N];
int _head[N], r[N], h[N];
int htop, *head = _head+1;
inline void addedg(const int &fr, const int &to, const int &limit) {
	e[++htop] = (Edge) {to, head[fr], limit};
	head[fr] = htop;
}
inline void rmedg(const int &fr, const int &to) {
    int *pre = head + fr;
    for (int i = head[fr]; i; i = e[i].nxt) {
        if (e[i].to == to) {
            *pre = e[i].nxt;
            return;
        }
        pre = &e[i].nxt;
    }
}
bool vis[N];
int d[N], cnt[N];
inline int plus(int &n) {
    register int ret = n;
    n = (n+1) % (N<<1);
    return ret;
}
inline bool spfa(int u = 0) {
    int q[N<<1] = {}, h = 0, t = 0;
	memset(d, -0x3f, sizeof d);
	memset(vis, 0, sizeof vis);
	memset(cnt, 0, sizeof cnt);
	q[plus(t)] = u;
	d[u] = 0, cnt[u] = 1;
	while (h ^ t) {
		u = q[plus(h)], vis[u] = false;
		for (int i = head[u], v; i; i = e[i].nxt) {
			if (d[v = e[i].to] < d[u] + e[i].w) {
				d[v] = d[u] + e[i].w;
				if (!vis[v]) {
					if (cnt[v] > 24) return false;
					vis[v] = true;
					++cnt[v];
					q[plus(t)] = v;
				}
			}
		}
	}
	return true;
}
int main() {
	int c, t, x;
	scanf("%d", &c);
	while (c--) {
        memset(h, 0, sizeof h);
        memset(_head, 0, sizeof _head), htop = 0;
		for (int i = 0; i < 24; i++)
			scanf("%d", &r[i]);
		scanf("%d", &t);
		for (int i = 0; i < t; i++) {
			scanf("%d", &x);
			++h[x];
		}
		for (int i = 0; i < 24; i++)
			addedg(i-1, i, 0);	                // d_i - d_{i-1} >= 0
		for (int i = 0; i < 24; i++)
			addedg(i, i-1, -h[i]);		        // d_i - d_{i-1} <= h_i
		for (int i = 8-1; i < 24; i++)
			addedg(i-8, i, r[i]);		        // d_i - d_{i-8} >= r_i
		int left = 0, right = t, mid;
		while (left <= right) {
			mid = left + right >> 1;
			for (int i = 0; i < 7;  i++)
				addedg(i+16, i, r[i] - mid);    // d_i - d_{i+16} >= r_i - limit
            addedg(-1, 23, mid);                 // d_23 - d_-1 >= limit
			if (!spfa()) left = mid + 1;
            else right = mid - 1;
            for (int i = 0; i < 7; i++)
                rmedg(i+16, i);
		}
        if (left >= t) puts("No Solution");
        else printf("%d\n", left);
	}
	return 0;
}