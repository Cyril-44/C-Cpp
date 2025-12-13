#include <stdio.h>
#include <string.h>
const int N = 500005;
struct Edge {
	int to, nxt, toid;
} e[N << 1];
int head[N], topid;
inline void addedg(const int &fr, const int &to, const int &id) {
	e[++topid] = Edge{to, head[fr], id};
	head[fr] = topid;
}
inline void out(int x) {
	static int sta[10];
	for (sta[sta[0] = 1] = x % 10, x /= 10; x; x /= 10)
		sta[++sta[0]] = x % 10;
	while (sta[0]) putchar(sta[sta[0]--] | '0');
	putchar(' ');
}
bool vis[N<<1];
void euler(const int& u) {
	for (int i = head[u]; i; i = e[i].nxt)
		if (!vis[i]) {
			vis[i] = true;
			euler(e[i].to);
			out(e[i].toid);
		}
}
int main() {
	int n;
	while (~scanf("%d", &n)) {
		memset(head, 0, n+1<<2);  // n*sizeof(int)
		memset(vis, 0, n+1<<1); // 2n*sizeof(bool)
        topid = 0;
		for (int i = 0; i < n; i++) {
			addedg((i<<1) % n, i, i<<1);
			addedg((i<<1|1) % n, i, i<<1|1);
		}
		euler(0);
		putchar('\n');
	}
	return 0;
}