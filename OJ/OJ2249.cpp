#include <stdio.h>
const int N = 105;
struct Edge {
	int to, nxt;
} e[N*N];
int head[N], sta[N], dfn[N], low[N], grp[N], timestmp, curscc;
bool indeg[N], ind_nxt[N], outd_nxt[N], ins[N];
inline void addedg(const int &fr, const int &to) {
	e[++head[0]] = Edge{to, head[fr]};
	head[fr] = head[0];
}
inline void umin(int &x, const int &y) { x = x < y ? x : y; }
void tarj(const int &u) {
	sta[++sta[0]] = u; ins[u] = true;
	dfn[u] = low[u] = ++timestmp;
	for (int i = head[u]; i; i = e[i].nxt) {
		if (!dfn[e[i].to]) {
			tarj(e[i].to);
			umin(low[u], low[e[i].to]);
		}	 /* !grp[e[i].to] */
		else if (ins[e[i].to]) umin(low[u], dfn[e[i].to]);
	}
	if (dfn[u] == low[u]) {
		++curscc;
		do grp[sta[sta[0]]] = curscc, ins[sta[sta[0]]] = false;
		while (sta[sta[0]--] ^ u);
	}
}
int main() {
	int n, v;
	scanf("%d", &n);
	for (int u = 1; u <= n; u++)
		while (scanf("%d", &v) && v) {
			addedg(u, v);
			indeg[v] = true;
		}
	for (int i = 1; i <= n; i++)
		if (!dfn[i]) tarj(i);
	if (curscc == 1) { // 只有一个 输出 1 0
		puts("1\n0");
		return 0; 
	}
	for (int i = 1; i <= n; i++)
		for (int j = head[i]; j; j = e[j].nxt)
			if (grp[i] ^ grp[e[j].to]) {
				outd_nxt[grp[i]] = true;
				ind_nxt[grp[e[j].to]] = true;
			}
	int in0 = 0, out0 = 0;
	for (int i = 1; i <= curscc; i++) {
		in0 += !ind_nxt[i];
		out0 += !outd_nxt[i];
	}
	printf ("%d\n%d\n", in0, (in0 > out0 ? in0 : out0));
	return 0;
}