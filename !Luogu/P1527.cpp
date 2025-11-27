#include <cstdio>
#include <cstring>
const int N = 505, M = 60005;
int a[N][N];
int ans[M], n;
enum Status {QUERY, DATA};
struct Query {
	int x1, y1, x2, y2, k, id;
    Status type;
} q[M + N * N];
inline void dfs(int l, int r, int L, int R) {
	static int tr[N][N];
	static Query lq[M + N * N], rq[M + N * N];
	static auto add = [&] (const int &p, const int &q, const int &x) -> void {
		for (int i = p; i <= n; i += i & -i)
			for (int j = q; j <= n; j += j & -j)
				tr[i][j] += x;
	};
	static auto query = [&] (const int &p, const int &q) -> int {
		int res(0);
		for (int i = p; i > 0; i -= i & -i)
			for (int j = q; j > 0; j -= j & -j)
				res += tr[i][j];
		return res;
	};
	static auto sum = [&] (const int &x1, const int &y1, const int &x2, const int &y2) -> int {
		return query(x2, y2) - query(x1-1, y2) - query(x2, y1-1) + query(x1-1, y1-1);
	};
    if (l > r) return;
	// memset(tr, 0, sizeof tr);
	int lt(0), rt(0);
	if (L == R) {
		for (int i = l; i <= r; i++)
			if (q[i].type == QUERY)
				ans[q[i].id] = L;
		return;
	}
	int mid(L + R >> 1);
	for (int i = l; i <= r; i++) {
		switch (q[i].type) {
		case DATA:
			if (q[i].k <= mid) {
				add(q[i].x1, q[i].y1, 1);
				lq[++lt] = q[i];
			}
			else rq[++rt] = q[i];
			break;
		case QUERY:
			int cnt(sum(q[i].x1, q[i].y1, q[i].x2, q[i].y2));
			if (cnt >= q[i].k) lq[++lt] = q[i];
			else q[i].k -= cnt, rq[++rt] = q[i];
			break;
		}
	}
	for (int i = 1; i <= lt; i++)
		q[l + i - 1] = lq[i];
	for (int i = 1; i <= rt; i++)
		q[l + lt + i - 1] = rq[i];
    for (int i = 1; i <= lt; i++)   // 还原/清空 tr
        if (lq[i].type == DATA)
            add(lq[i].x1, lq[i].y1, -1);
        else break;     // 前面是数据，后面是评论
	dfs(l, l + lt - 1, L, mid);
	dfs(l + lt, r, mid+1, R);
}
int main() {
	int m;
	scanf("%d%d", &n, &m);
	for (int i = 1; i <= n; i++)
		for (int j = 1; j <= n; j++) {
			scanf("%d", &a[i][j]);
			q[(i-1) * n + j] = Query{i,j, 0,0, a[i][j], 0, DATA};
		}
	for (int i = 1; i <= m; i++) {
		auto & tmp = q[i + n * n];
		scanf("%d%d%d%d%d", &tmp.x1, &tmp.y1, &tmp.x2, &tmp.y2, &tmp.k);
		tmp.type = QUERY;
		tmp.id = i;
	}
	dfs(1, n * n + m, 0, static_cast<int>(1e9));
	for (int i = 1; i <= m; i++)
		printf("%d\n", ans[i]);
	return 0;
}